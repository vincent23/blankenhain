import gzip
import xml.etree.ElementTree as ElementTree
import itertools

def makeCppArray(name, type, list):
	lines = ['static {} {}[] = {{'.format(type, name)]
	lines.append(''.join(indented('{},'.format(value) for value in list)))
	lines.append('};')
	return lines

def indented(lines, level=1):
	return ('\t' * level + line for line in lines)
	
def makeMidiTracks(midiTracks):
	midiTracksCpp = []
	for trackIndex, track in enumerate(midiTracks):
		toplevelDeviceChain = track.find('./DeviceChain')
		trackNotes = []
		clips = toplevelDeviceChain.findall(
			'./MainSequencer/ClipTimeable/ArrangerAutomation/Events/MidiClip')
		for clip in clips:
			clipTime = clip.get('Time')
			keyTracks = clip.findall('./Notes/KeyTracks/KeyTrack')
			for keyTrack in keyTracks:
				key = int(keyTrack.find('./MidiKey').get('Value'))
				notes = keyTrack.findall('./Notes/MidiNoteEvent')
				for note in notes:
					timeInClip = note.get('Time')
					duration = float(note.get('Duration'))
					velocity = int(note.get('Velocity'))
					timeInTrack = float(clipTime) + float(timeInClip)
					trackNotes.append((timeInTrack, key, velocity))
					trackNotes.append((timeInTrack + float(duration), key, 0))
		trackNotes.sort()
		if not trackNotes:
			continue
		times, keys, velocities = zip(*trackNotes)
		bpm = 120 # TODO
		sampleRate = 44100 # TODO
		samplePositions = (round(timeInBeats / bpm * 60 * sampleRate) for timeInBeats in times)
		samplePositionsArray = makeCppArray(
			'midiSamplePositions_{}'.format(trackIndex),
			'unsigned int',
			samplePositions)
		keysArray = makeCppArray(
			'midiKeys_{}'.format(trackIndex),
			'unsigned int',
			keys)
		velocitiesArray = makeCppArray(
			'midiVelocities_{}'.format(trackIndex),
			'unsigned int',
			velocities)
		midiTracksCpp.extend(samplePositionsArray)
		midiTracksCpp.extend(keysArray)
		midiTracksCpp.extend(velocitiesArray)
	return midiTracksCpp

def makePluginParameterTracks(pluginDevice, pluginIndex):
	# name = plugin.find('./PluginDesc/VstPluginInfo/PlugName').get('Value')
	pluginParameterTracksCpp = []
	parameters = pluginDevice.findall('./ParameterList/PluginFloatParameter')
	parameterValuesList = []
	for parameter in parameters:
		parameterId = parameter.find('ParameterId').get('Value')
		if parameterId == '-1':
			continue
		events = parameter.findall('./ParameterValue/ArrangerAutomation/Events/FloatEvent')
		parameterValues = []
		for event in events:
			eventTime = max(0, float(event.get('Time')))
			bpm = 120 # TODO
			sampleRate = 44100 # TODO
			samplePosition = round(eventTime / bpm * 60 * sampleRate)
			value = float(event.get('Value'))
			parameterValues.append((samplePosition, value))
		parameterValues.sort()
		parameterValuesList.append(parameterValues)

	# also convert the special "on" parameter to a float parameter
	# and add it after the other ones
	onEvents = pluginDevice.findall('./On/ArrangerAutomation/Events/BoolEvent')
	onValues = []
	for event in onEvents:
		eventTime = max(0, float(event.get('Time')))
		bpm = 120 # TODO
		sampleRate = 44100 # TODO
		samplePosition = round(eventTime / bpm * 60 * sampleRate)
		onAsFloat = 1.0 if event.get('Value') == 'true' else 0.0
		onValues.append((samplePosition, onAsFloat))
	onValues.sort()
	parameterValuesList.append(onValues)

	for parameterIndex, parameterValues in enumerate(parameterValuesList):
		samplePositions, values = zip(*parameterValues)
		samplePositionsArray = makeCppArray(
			'parameterSamplePositions_{}_{}'.format(pluginIndex, parameterIndex),
			'unsigned int',
			samplePositions)
		valuesArray = makeCppArray(
			'parameterValues_{}_{}'.format(pluginIndex, parameterIndex),
			'float',
			values)
		pluginParameterTracksCpp.extend(samplePositionsArray)
		pluginParameterTracksCpp.extend(valuesArray)

	return pluginParameterTracksCpp

def getPluginDevices(liveSet):
	midiTracks = liveSet.findall('./Tracks/MidiTrack')
	for track in midiTracks:
		toplevelDeviceChain = track.find('./DeviceChain')
		deviceChain = toplevelDeviceChain.find('./DeviceChain')
		devices = deviceChain.findall('./Devices/*[@Id]')
		for device in devices:
			if device.tag == 'PluginDevice':
				yield device
			elif device.tag in {'InstrumentGroupDevice', 'AudioEffectGroupDevice'}:
				plugins = device.findall('./Branches/*/DeviceChain/*/Devices/PluginDevice')
				for plugin in plugins:
					yield plugin
	# TODO: plugins on master

def convertAlsFile(filename):
	with gzip.open(filename) as inputFile:
		ableton = ElementTree.parse(inputFile)
	
	liveSet = ableton.find('./LiveSet')
	
	midiTracks = liveSet.findall('./Tracks/MidiTrack')	
	midiTracksCpp = makeMidiTracks(midiTracks)
	
	pluginDevices = getPluginDevices(liveSet)
	pluginDevicesCpp = itertools.chain(*(makePluginParameterTracks(plugin, i) for i, plugin in enumerate(pluginDevices)))
	
	resultCpp = []
	resultCpp.extend(midiTracksCpp)
	resultCpp.extend(pluginDevicesCpp)
	return '\n'.join(resultCpp)
	
#convertAlsFile('bh2test.als')
print(convertAlsFile('bh2test.als'))