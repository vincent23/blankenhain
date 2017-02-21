import gzip
import xml.etree.ElementTree as ElementTree
import sys

def indented(lines, level=1):
	return ('\t' * level + line for line in lines)

class Device:
	def fromXml(deviceXml):
		if deviceXml.tag == 'PluginDevice':
			# TODO look up device and get device type
			name = deviceXml.find('./PluginDesc/VstPluginInfo/PlugName').get('Value')
			device = EffectDevice()
		elif deviceXml.tag in {'AudioEffectGroupDevice', 'InstrumentGroupDevice'}:
			device = GroupDevice()
		elif deviceXml.tag == 'MidiPitcher':
			pass
		device.parse(deviceXml)
		return device

class ParameterEvent:
	def __init__(self, time, value):
		self.time = time
		self.value = value

class EffectDevice(Device):
	def __init__(self):
		self.parameters = []

	def parse(self, deviceXml):
		parametersXml = deviceXml.findall('./ParameterList/PluginFloatParameter')
		# get the parameter id and make a list of (id, parameterXml) tuples
		parametersXmlWithId = ((int(parameterXml.find('ParameterId').get('Value')), parameterXml)
			for parameterXml in parametersXml)
		# sort by id
		parametersXmlWithId = sorted(parametersXmlWithId, key=lambda x: x[0])
		# remove parameters with id = -1 and convert back to list of parameters
		parametersXml = [parameterXml for id, parameterXml in parametersXmlWithId if id != -1]
		for parameterXml in parametersXml:
			eventsXml = parameterXml.findall('./ParameterValue/ArrangerAutomation/Events/FloatEvent')
			parameterEvents = []
			for eventXml in eventsXml:
				eventTime = max(0, float(eventXml.get('Time')))
				value = float(eventXml.get('Value'))
				parameterEvents.append(ParameterEvent(eventTime, value))
			parameterEvents.sort(key=lambda x: x.time)
			self.parameters.append(parameterEvents)

		# also convert the special "on" parameter to a float parameter
		# and add it after the other ones
		onEventsXml = deviceXml.findall('./On/ArrangerAutomation/Events/BoolEvent')
		onEvents = []
		for eventXml in onEventsXml:
			eventTime = max(0, float(eventXml.get('Time')))
			onAsFloat = 1.0 if eventXml.get('Value') == 'true' else 0.0
			onEvents.append(ParameterEvent(eventTime, onAsFloat))
		onEvents.sort(key=lambda x: x.time)
		self.parameters.append(onEvents)

	def emitSource(self, songInfo):
		deviceName = songInfo.nextDeviceName()
		# TODO emit parameters
		songInfo.cppSource.append('EffectDevice {}(nullptr, nullptr);'.format(deviceName))
		return deviceName

class MidiDevice(EffectDevice):
	def parse(self, deviceXml):
		super().parse(deviceXml)
		# TODO

# TODO needs special handling of gm synth to set instrument
class InstrumentDevice(EffectDevice):
	def parse(self, deviceXml):
		super().parse(deviceXml)
		# TODO

class CombinedDevice(Device):
	def __init__(self):
		self.children = []

	def emitSource(self, songInfo):
		deviceNames = [device.emitSource(songInfo) for device in self.children]
		# take the id of the group device to be emitted for device array
		arrayName = 'deviceList_{}'.format(songInfo.nextFreeDeviceId)
		songInfo.appendCppArray(arrayName, 'Device*', deviceNames)
		return arrayName

class GroupDevice(CombinedDevice):
	# TODO 'on' parameter not supported for group right now
	def parse(self, deviceXml):
		branchesXml = deviceXml.findall('./Branches/AudioEffectBranch')
		deviceChainName = './DeviceChain/AudioToAudioDeviceChain'
		if not branchesXml:
			# this is an instrument group
			branchesXml = deviceXml.findall('./Branches/InstrumentBranch')
			deviceChainName = './DeviceChain/MidiToAudioDeviceChain'

		for branchXml in branchesXml:
			deviceChain = ChainDevice.fromXml(branchXml.find(deviceChainName))
			deviceChain.appendMixer(branchXml.find('MixerDevice'))
			self.children.append(deviceChain)

	def emitSource(self, songInfo):
		devicesArrayName = super().emitSource(songInfo)
		deviceName = songInfo.nextDeviceName()
		numberOfDevices = len(self.children)
		songInfo.cppSource.append('GroupDevice {}({}, {});'.format(deviceName, numberOfDevices, devicesArrayName))
		return deviceName

class ChainDevice(CombinedDevice):
	def fromXml(deviceChainXml):
		device = ChainDevice()
		device.parse(deviceChainXml)
		return device

	def parse(self, deviceChainXml):
		devicesXml = deviceChainXml.findall('./Devices/*[@Id]')
		self.children = [Device.fromXml(device) for device in devicesXml]

	def appendMixer(self, mixerXml):
		# TODO add volume and pan device
		# pan is called either "Pan" or "Panorama"
		pass
	
	def emitSource(self, songInfo):
		devicesArrayName = super().emitSource(songInfo)
		deviceName = songInfo.nextDeviceName()
		numberOfDevices = len(self.children)
		songInfo.cppSource.append('ChainDevice {}({}, {});'.format(deviceName, numberOfDevices, devicesArrayName))
		return deviceName

class Note:
	def __init__(self, start, duration, key, velocity):
		self.start = start
		self.duration = duration
		self.key = key
		self.velocity = velocity

class Track:
	def __init__(self):
		self.rootDevice = None
		self.notes = []

	def fromXml(trackXml):
		track = Track()
		track.parse(trackXml)
		return track

	def parse(self, trackXml):
		toplevelChainXml = trackXml.find('./DeviceChain')
		if not toplevelChainXml:
			# we got no device chain, probably this means we are on master
			toplevelChainXml = trackXml.find('./MasterChain')

		deviceChainXml = toplevelChainXml.find('./DeviceChain')
		self.rootDevice = ChainDevice.fromXml(deviceChainXml)

		# TODO sends
		mixerXml = toplevelChainXml.find('./Mixer')
		self.rootDevice.appendMixer(mixerXml)

		clipsXml = trackXml.findall(
			'./DeviceChain/MainSequencer/ClipTimeable/ArrangerAutomation/Events/MidiClip')
		if clipsXml:
			self.parseClips(clipsXml)

	def parseClips(self, clipsXml):
		for clipXml in clipsXml:
			clipTime = float(clipXml.get('Time'))
			keyTracks = clipXml.findall('./Notes/KeyTracks/KeyTrack')
			for keyTrack in keyTracks:
				key = int(keyTrack.find('./MidiKey').get('Value'))
				notesInTrack = keyTrack.findall('./Notes/MidiNoteEvent')
				for note in notesInTrack:
					timeInClip = float(note.get('Time'))
					duration = float(note.get('Duration'))
					velocity = int(note.get('Velocity'))
					timeInTrack = clipTime + timeInClip
					self.notes.append(Note(timeInTrack, duration, key, velocity))

class SongInfo:
	def __init__(self):
		self.bpm = 0
		self.songStart = 0
		self.songDuration = 0
		self.cppSource = []
		self.nextFreeDeviceId = 0
	
	def fromXml(songInfoXml):
		songInfo = SongInfo()
		songInfo.parse(songInfoXml)
		return songInfo

	def parse(self, liveSetXml):
		masterTrackXml = liveSetXml.find('./MasterTrack')
		self.bpm = float(masterTrackXml.find('./MasterChain/Mixer/Tempo/ArrangerAutomation/Events/FloatEvent').get('Value'))
		transportXml = liveSetXml.find('./Transport')
		self.songStart = float(transportXml.find('./LoopStart').get('Value'))
		self.songDuration = float(transportXml.find('./LoopLength').get('Value'))

	def beatsToSamples(self, timeInBeats):
		timeInBeats -= self.songStart
		sampleRate = 44100
		return round(timeInBeats / bpm * 60 * sampleRate)

	def appendCppArray(self, name, type, list):
		self.cppSource.append('{} {}[] = {{'.format(type, name))
		self.cppSource.append(''.join(indented('{},'.format(value) for value in list)))
		self.cppSource.append('};')
	
	def nextDeviceName(self):
		name = 'device_{}'.format(self.nextFreeDeviceId)
		self.nextFreeDeviceId += 1
		return name

def convert(filename):
	with gzip.open(filename) as inputFile:
		abletonXml = ElementTree.parse(inputFile)

	liveSetXml = abletonXml.find('./LiveSet')

	midiTracksXml = liveSetXml.findall('./Tracks/MidiTrack')
	groupTracksXml = liveSetXml.findall('./Tracks/GroupTrack')
	returnTracksXml = liveSetXml.findall('./Tracks/ReturnTrack')
	masterTrackXml = liveSetXml.find('./MasterTrack')

	midiTracks = [Track.fromXml(track) for track in midiTracksXml]
	# TODO group track and return track support
	# groupTracks = [Track.fromXml(track) for track in groupTracksXml]
	# returnTracks = [Track.fromXml(track) for track in returnTracksXml]
	masterTrack = Track.fromXml(masterTrackXml)
	
	songInfo = SongInfo.fromXml(liveSetXml)
	
	# build root device from master root device + group of all tracks
	rootChain = masterTrack.rootDevice
	midiTrackGroup = GroupDevice()
	midiTrackGroup.children = [midiTrack.rootDevice for midiTrack in midiTracks]
	rootChain.children.insert(0, midiTrackGroup)
	rootChain.emitSource(songInfo)
	print('\n'.join(songInfo.cppSource))

def main():
	convert(sys.argv[1])

if __name__ == '__main__':
	main()