#pragma once
#include "DLS.h"
#include <vector>
#include <iostream>
#include <fstream>

int main()
{
	bool formatedOutput = true;
	
	using namespace DLS;
	RIFF::File name("gm.dls");
	DLS::File file(&name);
	std::vector<Instrument*> instruments;
	Instrument* temp = NULL;
	instruments.push_back(file.GetFirstInstrument());
	while (temp = file.GetNextInstrument(), temp != NULL)
		instruments.push_back(temp);

	//std::cout << instruments.size() << "\n";
	std::cout << "gmInstrument** list = new gmInstrument*[" << instruments.size() << "];\n";
	std::cout << "gmSoundRegion* tempRegions;\ngmSoundRegion& tempRegion;\n\n";
	size_t counter = 0u;
	for (auto instr : instruments)
	{
		auto region = instr->GetFirstRegion();

		Region* tempRegion = NULL;
		std::vector<Region*> regions;
		regions.push_back(region);
		while (tempRegion = instr->GetNextRegion(), tempRegion != NULL)
		{
			regions.push_back(tempRegion);
		}

		if (!formatedOutput)
		{
			std::cout << instr->pInfo->Name << "\n";
			std::cout << regions.size() << "\n";

			for (unsigned int i = 0u; i < regions.size(); i++)
			{
				auto shit = regions[i]->GetSample();
				if (regions[i]->GetSample() == nullptr) continue;

				std::cout << regions[i]->KeyRange.low << " " << regions[i]->KeyRange.high << "\n";
				std::cout << static_cast<unsigned int>(regions[i]->UnityNote) << "\n";
				if (regions[i]->pSampleLoops != nullptr)
					std::cout << regions[i]->pSampleLoops->LoopStart << " " << regions[i]->pSampleLoops->LoopLength << "\n";
				else
					std::cout << "NOLOOP\n";
				std::cout << regions[i]->GetSample()->pCkData->GetSize() << "\n";
				std::cout << regions[i]->GetSample()->pCkData->GetFilePos() << "\n";
			}
			std::cout << "\n";
		}
		else
		{
			std::cout << "tempRegions = new gmSoundRegion[" << regions.size() << "];\n";
			for (unsigned int i = 0u; i < regions.size(); i++)
			{
				std::cout << "tempRegion = gmSoundRegion();\n";
				if (regions[i]->GetSample() == nullptr) continue;

				std::cout << "tempRegion.lowest = " << regions[i]->KeyRange.low << ";\ntempRegion.highest = " << regions[i]->KeyRange.high << ";\n";
				//std::cout << regions[i]->KeyRange.low << " " << regions[i]->KeyRange.high << "\n";
				//std::cout << static_cast<unsigned int>(regions[i]->UnityNote) << "\n";
				std::cout << "tempRegion.rootNote = " << static_cast<unsigned int>(regions[i]->UnityNote) << ";\n";

				if (regions[i]->pSampleLoops != nullptr) {
					std::cout << "tempRegion.loopStart = " << regions[i]->pSampleLoops->LoopStart << ";\ntempRegion.loopLength = " << regions[i]->pSampleLoops->LoopLength << ";\n";
					std::cout << "tempRegion.isLoopable = true;\n";
				}
				else
					std::cout << "tempRegion.isLoopable = false;\n";

				std::cout << "tempRegion.sampleLength = " << regions[i]->GetSample()->pCkData->GetSize() << ";\n";
				std::cout << "tempRegion.startByte = " << regions[i]->GetSample()->pCkData->GetFilePos() << ";\n";
				std::cout << "tempRegions[" << i << "] = tempRegion;\n";


			}

			std::cout << "list[" << counter << "] = new gmInstrument(\"" << instr->pInfo->Name << "\"," << regions.size() << ", tempRegions);\n";
			std::cout << "delete[] tempRegions;\ntempRegions = nullptr;\n\n";
		}
		counter++;
	}

	return 0;
}