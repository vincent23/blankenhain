import dls
import config

import gzip
import xml.etree.ElementTree as ElementTree
import sys
import struct
import math

def indented(lines, level=1):
	return ('\t' * level + line for line in lines)

def eprint(*args, **kwargs):
	print(*args, file=sys.stderr, **kwargs)

def chunks(l, n):
    """Yield successive n-sized chunks from l."""
    for i in range(0, len(l), n):
        yield l[i:i + n]

def deltaEncoded(l):
	last = l[0]
	yield last
	for x in l[1:]:
		yield x - last
		last = x

routingMaster = 'master'
routingSends = 'sends'
routingGroup = 'group'

class Device:
	def fromXml(deviceXml):
		if deviceXml.tag == 'PluginDevice':
			name = deviceXml.find('./PluginDesc/VstPluginInfo/PlugName').get('Value')
			plugin = config.plugins.get(name)
			if plugin is None:
				if name.startswith('bh_'):
					# device is unknown, but probably it's a blankenhain device
					eprint('Warning: unrecognized device "{}"'.format(name))
					eprint('Did you forget to add it to the config.py file?')
				# ignore unrecognized devices
				return None
			pluginType = plugin['type']
			className = plugin['class']
			numberOfParameters = plugin['numberOfParameters']
			if pluginType == config.instrument:
				device = InstrumentDevice(className, numberOfParameters)
			elif pluginType == config.effect:
				device = EffectDevice(className, numberOfParameters)
			elif pluginType == config.midi:
				device = MidiDevice(className, numberOfParameters)
			else:
				eprint('Warning: device "{}" has unrecognized type "{}"'.format(name, pluginType))
				return None
		elif deviceXml.tag in {'AudioEffectGroupDevice', 'InstrumentGroupDevice'}:
			device = GroupDevice()
		elif deviceXml.tag == 'MidiPitcher':
			# TODO implement conversion from ableton pitch shift
			return None
		else:
			# ignore unrecognized devices
			return None
		device.parse(deviceXml)
		# Try / Except to check if the device is empty audiEffectRack. if yes, return None
		try:
			if device.isCurrentlyEmpty():
				return None
			else:
				return device
		except:
			return device

	def setInputTrackIndex(self, index):
		pass
	
	def __init__(self):
		self.isOffEntireTime = False

class ParameterEvent:
	def __init__(self, time, value):
		self.time = time
		self.value = value

class EffectDevice(Device):
	def __init__(self, className, numberOfParameters):
		super(EffectDevice, self).__init__()
		self.parameters = [None] * numberOfParameters
		self.className = className
		self.inputTrackIndex = 0

	def parse(self, deviceXml):
		parametersXml = deviceXml.findall('./ParameterList/PluginFloatParameter')
		# get the parameter id and make a list of (id, parameterXml) tuples
		parametersXmlWithId = ((int(parameterXml.find('ParameterId').get('Value')), parameterXml)
			for parameterXml in parametersXml)
		for id, parameterXml in parametersXmlWithId:
			if id == -1:
				continue
			eventsXml = parameterXml.findall('./ParameterValue/ArrangerAutomation/Events/FloatEvent')
			parameterEvents = []
			for eventXml in eventsXml:
				eventTime = max(0, float(eventXml.get('Time')))
				value = float(eventXml.get('Value'))
				parameterEvents.append(ParameterEvent(eventTime, value))
			parameterEvents.sort(key=lambda x: x.time)
			self.parameters[id] = parameterEvents

		bufferXml = deviceXml.find('./PluginDesc/VstPluginInfo/Preset/VstPreset/Buffer')
		# strip whitespace and cut first 56 chars
		buffer = ''.join(bufferXml.text.split())[56:]
		# convert to float
		bufferFloats = [struct.unpack('<f', bytes.fromhex(x))[0] for x in chunks(buffer, 8)]
		for id, x in enumerate(self.parameters):
			if x is None:
				self.parameters[id] = [ParameterEvent(0, bufferFloats[id])]

		# also convert the special "on" parameter to a float parameter
		# and add it after the other ones
		onEventsXml = deviceXml.findall('./On/ArrangerAutomation/Events/BoolEvent')
		onEvents = []
		# Check if Effect is Off the entire time:
		if len(onEventsXml) == 1:
			self.isOffEntireTime = True if onEventsXml[0].get('Value') == 'false' else False
		for eventXml in onEventsXml:
			eventTime = max(0, float(eventXml.get('Time')))
			onAsFloat = 1.0 if eventXml.get('Value') == 'true' else 0.0
			onEvents.append(ParameterEvent(eventTime, onAsFloat))
		onEvents.sort(key=lambda x: x.time)
		self.parameters.append(onEvents)

	def emitSource(self, songInfo):
		deviceName = songInfo.nextDeviceName()
		if self.isOffEntireTime == False:
			parameterTracksName = self.emitParameterTracksSource(songInfo)
			effectName = deviceName + '_effect';
			songInfo.cppSource.append('{} {};'.format(self.className, effectName))
			songInfo.cppSource.append('EffectDevice {}({}, {});'.format(deviceName, effectName, parameterTracksName))
		return deviceName

	def emitParameterTracksSource(self, songInfo):
		arrayName = songInfo.currentDeviceName() + '_parameters'
		if self.isOffEntireTime == False:
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
				# remove default value if there is automation
				if len(samplePositions) >= 2 and samplePositions[0] == 0 and samplePositions[1] == 0:
					samplePositions = samplePositions[1:]
					normalizedValues = normalizedValues[1:]
				positionsName = songInfo.currentDeviceName() + '_parameter_{}_samplePositions'.format(parameterIndex)
				valuesName = songInfo.currentDeviceName() + '_parameter_{}_values'.format(parameterIndex)
				songInfo.appendCppArray(positionsName, 'unsigned int', samplePositions)
				songInfo.appendCppArray(valuesName, 'float', normalizedValues)
				parameterTracks.append('ParameterTrack({}, {}, {})'.format(len(parameter), positionsName, valuesName))
			songInfo.appendCppArray(arrayName, 'ParameterTrack', parameterTracks, linebreak=True)
		return arrayName

	def setInputTrackIndex(self, index):
		self.inputTrackIndex = index

class MidiDevice(EffectDevice):
	def emitSource(self, songInfo):
		deviceName = songInfo.nextDeviceName()
		parameterTracksName = self.emitParameterTracksSource(songInfo)
		midiName = deviceName + '_midi';
		# TODO parse routing to figure out the output index
		outputTrackIndex = 0
		songInfo.cppSource.append('{} {};'.format(self.className, midiName))
		songInfo.cppSource.append('MidiDevice {}({}, {}, {}, {});'.format(
			deviceName, midiName, parameterTracksName, self.inputTrackIndex, outputTrackIndex))
		return deviceName

class InstrumentDevice(EffectDevice):
	def emitSource(self, songInfo):
		deviceName = songInfo.nextDeviceName()
		parameterTracksName = self.emitParameterTracksSource(songInfo)
		instrumentName = deviceName + '_instrument'
		songInfo.cppSource.append('{} {};'.format(self.className, instrumentName))
		songInfo.cppSource.append('InstrumentDevice {}({}, {}, {});'.format(
			deviceName, instrumentName, parameterTracksName, self.inputTrackIndex))
		# special handling of gm synth to set instrument
		if self.className == config.plugins['bh_gm_synth']['class']:
			# get instrument id from parameter 9
			instrumentIndex = round(self.parameters[9][0].value * 235)
			if instrumentIndex > 234:
				eprint('Warning: gm instrument out of range')
				return deviceName
			regions = dls.instruments[instrumentIndex]['regions']
			regionsName = instrumentName + '_regions'
			songInfo.cppSource.append('gmSoundRegion {}[{}];'.format(regionsName, len(regions)))
			for index, region in enumerate(regions):
				regionName = '{}[{}]'.format(regionsName, index)
				songInfo.cppSource.append('{}.lowest = {};'.format(regionName, region['lowest']))
				songInfo.cppSource.append('{}.highest = {};'.format(regionName, region['highest']))
				songInfo.cppSource.append('{}.rootNote = {};'.format(regionName, region['rootNote']))
				songInfo.cppSource.append('{}.sampleLength = {};'.format(regionName, region['sampleLength']))
				songInfo.cppSource.append('{}.startByte = {};'.format(regionName, region['startByte']))
				if region['isLoopable']:
					songInfo.cppSource.append('{}.isLoopable = true;'.format(regionName))
					songInfo.cppSource.append('{}.loopStart = {};'.format(regionName, region['loopStart']))
					songInfo.cppSource.append('{}.loopLength = {};'.format(regionName, region['loopLength']))
				else:
					songInfo.cppSource.append('{}.isLoopable = false;'.format(regionName))
			gmInstrumentName = instrumentName + '_gm_instrument'
			songInfo.cppSource.append('gmInstrument {}(nullptr, {}, {});'.format(gmInstrumentName, len(regions), regionsName))
			songInfo.cppSource.append('{}.loadMidiInstrument({});'.format(instrumentName, gmInstrumentName))
		return deviceName

class SendDevice(EffectDevice):
	def __init__(self):
		self.parameters = []

	def parse(self, deviceXml):
		sendsXml = deviceXml.findall('./TrackSendHolder/Send/ArrangerAutomation/Events')
		for sendXml in sendsXml:
			eventsXml = sendXml.findall('./FloatEvent')
			sendEvents = []
			for eventXml in eventsXml:
				eventTime = max(0, float(eventXml.get('Time')))
				value = float(eventXml.get('Value'))
				# map value from linear to db
				value = 20 * math.log10(value)
				# map from db to ableton linear
				if value < -24:
					value = value**4 * 1.69759896e-07 + value**3 * 3.40300078e-05 + value**2 * 2.58462419e-03 + value * 9.47279067e-02 + 1.59883479
				else:
					value = 0.025 * value + 1
				sendEvents.append(ParameterEvent(eventTime, value))
			sendEvents.sort(key=lambda x: x.time)
			self.parameters.append(sendEvents)

	def emitSource(self, songInfo):
		deviceName = songInfo.nextDeviceName()
		parameterTracksName = self.emitParameterTracksSource(songInfo)
		songInfo.cppSource.append('SendDevice {}({});'.format(deviceName, parameterTracksName))
		return deviceName

class ReturnDevice(Device):
	def __init__(self, returnTrackIndex):
		self.returnTrackIndex = returnTrackIndex

	def emitSource(self, songInfo):
		deviceName = songInfo.nextDeviceName()
		songInfo.cppSource.append('ReturnDevice {}({});'.format(deviceName, self.returnTrackIndex))
		return deviceName

class CombinedDevice(Device):
	def __init__(self):
		super(CombinedDevice, self).__init__()
		self.children = []

	def emitSource(self, songInfo):
		deviceNames = []
		for device in self.children:
			if (device.isOffEntireTime == False):
				#check if we have a group device as child,
				# if yes, it might be empty, we need to take care of that
				try:
					if (len(device.children) != 0):
						deviceNames.append('&' + device.emitSource(songInfo))
				except AttributeError:
					#not a groupdevice as child
					deviceNames.append('&' + device.emitSource(songInfo))


		# take the id of the group device to be emitted for device array
		arrayName = 'deviceList_{}'.format(songInfo.nextFreeDeviceId)
		# Take care of special case: Empty Combined Device
		if len(deviceNames) != 0:
			songInfo.appendCppArray(arrayName, 'Device*', deviceNames)
		return arrayName

	def setInputTrackIndex(self, index):
		for x in self.children:
			x.setInputTrackIndex(index)
			
	def isCurrentlyEmpty(self):
		return len(self.children) == 0

class GroupDevice(CombinedDevice):
	def __init__(self): 
		super(GroupDevice, self).__init__()

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
			if not deviceChain.isCurrentlyEmpty():
				self.children.append(deviceChain)

	def emitSource(self, songInfo):
		devicesArrayName = super().emitSource(songInfo)
		deviceName = songInfo.nextDeviceName()
		numberOfDevices = len(self.children)
		# Take care of special case: Empty Combined Device
		if numberOfDevices != 0:
			songInfo.cppSource.append('GroupDevice {}({}, {});'.format(deviceName, numberOfDevices, devicesArrayName))
			return deviceName
		else:
			return None

class ChainDevice(CombinedDevice):
	def __init__(self): 
		super(ChainDevice, self).__init__()

	def fromXml(deviceChainXml):
		device = ChainDevice()
		device.parse(deviceChainXml)
		return device

	def parse(self, deviceChainXml):
		devicesXml = deviceChainXml.findall('./Devices/*[@Id]')
		self.children = [x for x in (Device.fromXml(device) for device in devicesXml) if (x is not None) and (x.isOffEntireTime == False)]

	def appendMixer(self, mixerXml):
		# pan is called either "Pan" or "Panorama"
		panXml = mixerXml.find('./Pan')
		if panXml is None:
			panXml = mixerXml.find('./Panorama')
		pan = EffectDevice(config.plugins['bh_width']['class'], config.plugins['bh_width']['numberOfParameters'])
		pan.parameters = [
			[ParameterEvent(0, 0.5)], # width
			[], # pan
			[ParameterEvent(0, 0.5)], # lfoAmount
			[ParameterEvent(0, 0)], # lfoSpeed
			[ParameterEvent(0, 0)], # lfoBeatMultiplier
			[ParameterEvent(0, 0)], # lfoWaveform
			[ParameterEvent(0, 0)], # lfoTemposync
			[ParameterEvent(0, 0)], # lfoPhase
			[ParameterEvent(0, 0)], # lfoBaseline
			[ParameterEvent(0, 1)]  # on
		]
		eventsXml = panXml.findall('./ArrangerAutomation/Events/FloatEvent')
		pan.parameters[1] = [
			ParameterEvent(
				max(0, float(eventXml.get('Time'))),
				0.5 + 0.5 * float(eventXml.get('Value'))
			) for eventXml in eventsXml]

		# Check if panning is always equal 0 ("middle" / "zeroPanning")
		# Only append panDevice if necessary
		if (len(pan.parameters[1]) != 1) or (pan.parameters[1][0].value != 0.5 ):
			self.children.append(pan)
		#self.children.append(pan)

		# TODO append volume device
		# we have to figure out how ableton maps slider <-> dB
		# (important for interpolation)
		# volumeXml = mixerXml.find('Volume')

		# if there are any sends, append a send device
		sendsXml = mixerXml.findall('./Sends/TrackSendHolder/Send')
		activeSends = (len(sendXml.findall('./ArrangerAutomation/Events/FloatEvent')) > 1
			or float(sendXml.find('./ArrangerAutomation/Events/FloatEvent').get('Value')) > 0.00032
			for sendXml in sendsXml)
		if any(activeSends):
			send = SendDevice()
			send.parse(mixerXml.find('./Sends'))
			self.children.append(send)
	
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
		self.outputRouting = None
		self.notes = []
		self.trackGroupId = -1
		self.trackId = None
		self.isOffEntireTime = False

	def fromXml(trackXml):
		track = Track()
		track.parse(trackXml)
		return track

	def parse(self, trackXml):
		trackIdXml = trackXml.get('Id')
		self.trackId = int(trackIdXml) if trackIdXml is not None else -1
		toplevelChainXml = trackXml.find('./DeviceChain')
		if not toplevelChainXml:
			# we got no device chain, probably this means we are on master
			toplevelChainXml = trackXml.find('./MasterChain')
		else:
			routingXml = toplevelChainXml.find('./AudioOutputRouting/Target')
			if routingXml.get('Value') == 'AudioOut/Master':
				self.outputRouting = routingMaster
			elif routingXml.get('Value') == 'AudioOut/GroupTrack':
				self.outputRouting = routingGroup
			elif routingXml.get('Value') == 'AudioOut/None':
				self.outputRouting = routingSends
			else:
				eprint("Unsupported routing: '{}'".format(routingXml.get('Value')))


		deviceChainXml = toplevelChainXml.find('./DeviceChain')
		self.rootDevice = ChainDevice.fromXml(deviceChainXml)

		# Check if Track is Off the entire time:
		onEventsXml = trackXml.findall('./DeviceChain/Mixer/Speaker/ArrangerAutomation/Events/BoolEvent')
		onEvents = []
		if len(onEventsXml) == 1:
			eventTime = max(0, float(onEventsXml[0].get('Time')))
			self.isOffEntireTime = True if onEventsXml[0].get('Value') == 'false' else False

		trackGroupIdXml = trackXml.find('./TrackGroupId')
		self.trackGroupId = int(trackGroupIdXml.get('Value'))

		mixerXml = toplevelChainXml.find('./Mixer')
		self.rootDevice.appendMixer(mixerXml)

		clipsXml = trackXml.findall(
			'./DeviceChain/MainSequencer/ClipTimeable/ArrangerAutomation/Events/MidiClip')
		if clipsXml:
			self.parseClips(clipsXml)

	def parseClips(self, clipsXml):
		for clipXml in clipsXml:
			clipTime = float(clipXml.get('Time'))
			# start of clip in track
			currentStart = float(clipXml.find('CurrentStart').get('Value'))
			if clipTime != currentStart:
				eprint("Warning: clipTime != currentStart")
			# end of clip in track
			currentEnd = float(clipXml.find('CurrentEnd').get('Value'))
			loopXml = clipXml.find('./Loop')
			# start of loop in clip
			loopStart = float(loopXml.find('./LoopStart').get('Value'))
			# end of loop in clip
			loopEnd = float(loopXml.find('./LoopEnd').get('Value'))
			loopLength = loopEnd - loopStart
			# clip start relative to loopStart
			startRelative = float(loopXml.find('./StartRelative').get('Value'))
			loopOn = loopXml.find('./LoopOn').get('Value') == 'true'
			keyTracks = clipXml.findall('./Notes/KeyTracks/KeyTrack')

			timeEps = 1e-6
			if not loopOn and (startRelative) > timeEps:
				eprint("Warning: loop is off but startRelative != 0")
			for keyTrack in keyTracks:
				key = int(keyTrack.find('./MidiKey').get('Value'))
				notesInTrack = keyTrack.findall('./Notes/MidiNoteEvent')
				for note in notesInTrack:
					timeInClip = float(note.get('Time'))
					if timeInClip < (loopStart + min(0, startRelative) - timeEps) or timeInClip >= loopEnd:
						continue
					duration = float(note.get('Duration'))
					velocity = round(float(note.get('Velocity')))
					if velocity == 0:
						eprint("Warning: note with velocity = 0")
						continue
					# clip duration to loop end
					duration = min(loopEnd - timeInClip, duration)

					timeInTrack = currentStart + timeInClip - (loopStart + startRelative)
					if timeInClip < loopStart - timeEps:
						# before loop
						self.notes.append(Note(timeInTrack, duration, key, velocity))
					else:
						if timeInClip < loopStart + startRelative - timeEps:
							# only looped
							# (startRelative is positive, otherwise we would be in the 'before loop' if branch)
							timeInTrack += loopLength
						while timeInTrack < currentEnd - timeEps:
							self.notes.append(Note(timeInTrack, duration, key, velocity))
							if not loopOn:
								break
							timeInTrack += loopLength

	def emitSource(self, songInfo):
		if self.isOffEntireTime == False:
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
			songInfo.appendCppArray(samplePositionsName, 'unsigned int', deltaEncoded(samplePositions))
			songInfo.appendCppArray(keysName, 'unsigned int', keys)
			songInfo.appendCppArray(velocitiesName, 'unsigned int', velocities)
			songInfo.cppSource.append('MidiTrack {}({}, {}, {}, {});'.format(
				trackName, len(events), samplePositionsName, keysName, velocitiesName))
		return trackName

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

	def appendCppArray(self, name, type, values, linebreak=False):
		if type == 'float':
			values = ['{:.8f}f'.format(x) for x in values]
		self.cppSource.append('{} {}[] = {{'.format(type, name))
		if linebreak:
			self.cppSource.extend('\t{},'.format(x) for x in values)
		else:
			self.cppSource.append('\t' + ', '.join(str(x) for x in values))
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
	groupTracks = [Track.fromXml(track) for track in groupTracksXml]
	returnTracks = [Track.fromXml(track) for track in returnTracksXml]
	masterTrack = Track.fromXml(masterTrackXml)

	songInfo = SongInfo.fromXml(liveSetXml)

	midiTrackNames = []
	# omit empty tracks and tracks that are off the entire time
	for trackIndex, midiTrack in enumerate((track for track in midiTracks if track.notes and not track.isOffEntireTime)):
		midiTrackNames.append('&' + midiTrack.emitSource(songInfo))
		midiTrack.rootDevice.setInputTrackIndex(trackIndex)

	# build group tracks
	for groupTrack in groupTracks:
		childTracks = [midiTrack for midiTrack in midiTracks
			if midiTrack.outputRouting == routingGroup and midiTrack.trackGroupId == groupTrack.trackId]
		midiTrackGroup = GroupDevice()
		midiTrackGroup.children = [track.rootDevice for track in childTracks if track.rootDevice.children]
		if midiTrackGroup.children:
			groupTrack.rootDevice.children.insert(0, midiTrackGroup)

	# build return tracks
	for i, returnTrack in enumerate(returnTracks):
		returnTrack.rootDevice.children.insert(0, ReturnDevice(i))

	# build root device from master root device + group of all tracks
	rootChain = masterTrack.rootDevice
	midiTrackGroup = GroupDevice()
	# ignore tracks that don't go to master or have no devices or are turned off 
	midiTrackGroup.children = [track.rootDevice for track in (midiTracks + groupTracks + returnTracks)
		if (track.outputRouting == routingMaster or track.outputRouting == routingSends) and track.rootDevice.children and not track.isOffEntireTime]
	rootChain.children.insert(0, midiTrackGroup)
	masterName = rootChain.emitSource(songInfo)

	songInfo.appendCppArray('midiTracks', 'MidiTrack*', midiTrackNames)

	if len(returnTracks) != 0:
		# todo buffer might be too big if we have unused return tracks
		songInfo.cppSource.append('Sample sendBuffers[constants::blockSize * {}];'.format(len(returnTracks)))
		lengthInSamples = songInfo.beatsToSamples(songInfo.songStart + songInfo.songDuration)
		songInfo.cppSource.append('SongInfo songInfo(midiTracks, sendBuffers, {}, {}, {});'.format(len(returnTracks), songInfo.bpm, lengthInSamples))
	else:
		lengthInSamples = songInfo.beatsToSamples(songInfo.songStart + songInfo.songDuration)
		songInfo.cppSource.append('SongInfo songInfo(midiTracks, nullptr, 0u, {}, {});'.format(songInfo.bpm, lengthInSamples))

	songInfo.cppSource.append('Song song(songInfo, {});'.format(masterName))
	songInfo.cppSource.append('#define LENGTH_IN_SAMPLES {}'.format(lengthInSamples))
	print('\n'.join(songInfo.cppSource))

def main():
	convert(sys.argv[1])

if __name__ == '__main__':
	main()