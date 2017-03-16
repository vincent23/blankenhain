import dls
import config

import gzip
import xml.etree.ElementTree as ElementTree
import sys

def indented(lines, level=1):
	return ('\t' * level + line for line in lines)

def eprint(*args, **kwargs):
	print(*args, file=sys.stderr, **kwargs)

class Device:
	def fromXml(deviceXml):
		if deviceXml.tag == 'PluginDevice':
			# TODO look up device and get device type
			name = deviceXml.find('./PluginDesc/VstPluginInfo/PlugName').get('Value')
			plugin = config.plugins.get(name)
			if plugin is None:
				if name.startswith('bh_'):
					# device is unknown, but probably it's a blankenhain device
					eprint('Warning: unrecognized device "{}"'.format(name))
				# ignore unrecognized devices
				return None
			pluginType = plugin['type']
			className = plugin['class']
			if pluginType == config.instrument:
				device = InstrumentDevice(className)
			elif pluginType == config.effect:
				device = EffectDevice(className)
			elif pluginType == config.midi:
				device = MidiDevice(className)
			else:
				eprint('Warning: device "{}" has unrecognized type "{}"'.format(name, pluginType))
				return None
		elif deviceXml.tag in {'AudioEffectGroupDevice', 'InstrumentGroupDevice'}:
			device = GroupDevice()
		elif deviceXml.tag == 'MidiPitcher':
			# TODO implement conversion from ableton pitch shift
			pass
		else:
			# ignore unrecognized devices
			return None
		device.parse(deviceXml)
		return device

class ParameterEvent:
	def __init__(self, time, value):
		self.time = time
		self.value = value

class EffectDevice(Device):
	def __init__(self, className):
		self.parameters = []
		self.className = className

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
		parameterTracksName = self.emitParameterTracksSource(songInfo)
		songInfo.cppSource.append('EffectDevice {}(nullptr, {});'.format(deviceName, parameterTracksName))
		return deviceName

	def emitParameterTracksSource(self, songInfo):
		arrayName = songInfo.currentDeviceName() + '_parameters'
		parameterTracks = []
		for parameterIndex, parameter in enumerate(self.parameters):
			# clip values to start/end
			songStart = songInfo.songStart
			songEnd = songInfo.songStart + songInfo.songDuration
			startIndex = next((i for i, v in enumerate(parameter) if v.time >= songStart), None)
			afterEndIndex = next((i for i, v in enumerate(parameter) if v.time >= songEnd), len(parameter))
			samplePositions = []
			normalizedValues = []
			if startIndex is None:
				samplePositions.append(0)
				normalizedValues.append(parameter[-1].value)
			else:
				oneBeforeStart = max(startIndex - 1, 0)
				oneAfterEnd = afterEndIndex + 1
				samplePositions.extend((songInfo.beatsToSamples(event.time) for event in parameter[oneBeforeStart:oneAfterEnd]))
				normalizedValues.extend((event.value for event in parameter[oneBeforeStart:oneAfterEnd]))
			if samplePositions[0] < 0:
				if len(parameter) > 1:
					t = -samplePositions[0] / (samplePositions[1] - samplePositions[0])
					normalizedValues[0] = normalizedValues[0] * (1 - t) + normalizedValues[1] * t
				samplePositions[0] = 0
			positionsName = songInfo.currentDeviceName() + '_parameter_{}_samplePositions'.format(parameterIndex)
			valuesName = songInfo.currentDeviceName() + '_parameter_{}_values'.format(parameterIndex)
			songInfo.appendCppArray(positionsName, 'unsigned int', samplePositions)
			songInfo.appendCppArray(valuesName, 'float', normalizedValues)
			parameterTracks.append('ParameterTrack({}, {}, {})'.format(len(parameter), positionsName, valuesName))
		songInfo.appendCppArray(arrayName, 'ParameterTrack', parameterTracks, linebreak=True)
		return arrayName

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
		self.children = [x for x in (Device.fromXml(device) for device in devicesXml) if x is not None]

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
					if velocity == 0:
						eprint("Warning: note with velocity = 0")
						continue
					timeInTrack = clipTime + timeInClip
					self.notes.append(Note(timeInTrack, duration, key, velocity))

	def emitSource(self, songInfo):
		events = []
		for note in self.notes:
			if note.start < songInfo.songStart or note.start > (songInfo.songStart + songInfo.songDuration):
				continue
			startInSamples = songInfo.beatsToSamples(note.start)
			endInSamples = songInfo.beatsToSamples(note.start + note.duration)
			events.append((startInSamples, note.key, note.velocity))
			events.append((endInSamples, note.key, 0))
		events.sort()
		samplePositions, keys, velocities = zip(*events) if events else ([], [], [])
		trackName = songInfo.nextTrackName()
		samplePositionsName = trackName + '_samplePositions'
		keysName = trackName + '_keys'
		velocitiesName = trackName + '_velocities'
		songInfo.appendCppArray(samplePositionsName, 'unsigned int', samplePositions)
		songInfo.appendCppArray(keysName, 'unsigned int', keys)
		songInfo.appendCppArray(velocitiesName, 'unsigned int', velocities)
		songInfo.cppSource.append('MidiTrack {}({}, {}, {}, {});'.format(
			trackName, len(events), samplePositionsName, keysName, velocitiesName
		))

class SongInfo:
	def __init__(self):
		self.bpm = 0
		self.songStart = 0
		self.songDuration = 0
		self.cppSource = []
		self.nextFreeDeviceId = 0
		self.nextFreeTrackId = 0
	
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
		return round(timeInBeats / self.bpm * 60 * sampleRate)

	def appendCppArray(self, name, type, list, linebreak=False):
		self.cppSource.append('{} {}[] = {{'.format(type, name))
		if linebreak:
			self.cppSource.extend('\t{},'.format(x) for x in list)
		else:
			self.cppSource.append('\t' + ', '.join(str(x) for x in list))
		self.cppSource.append('};')
	
	def nextDeviceName(self):
		self.nextFreeDeviceId += 1
		return self.currentDeviceName()

	def currentDeviceName(self):
		return 'device_{}'.format(self.nextFreeDeviceId - 1)

	def nextTrackName(self):
		self.nextFreeTrackId += 1
		return self.currentTrackName()

	def currentTrackName(self):
		return 'track_{}'.format(self.nextFreeTrackId - 1)

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

	for midiTrack in midiTracks:
		midiTrack.emitSource(songInfo)

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