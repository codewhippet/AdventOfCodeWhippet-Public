#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle09_2024_Types
{
	struct Block
	{
		int Id;
		int Size;
	};
}

using namespace Puzzle09_2024_Types;

static void Puzzle09_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	string compressedDiskMap = ReadAllLines(input).front();

	vector<int> diskMap;
	diskMap.reserve(compressedDiskMap.size() * 10);
	for (int mapIndex = 0; mapIndex < (int)compressedDiskMap.size(); mapIndex++)
	{
		int writeId = mapIndex % 2 == 0 ? (mapIndex / 2) : -1;
		int count = compressedDiskMap[mapIndex] - '0';
		for (int i = 0; i < count; i++)
		{
			diskMap.push_back(writeId);
		}
	}

	{
		int* start = &diskMap[0];
		int* stop = &diskMap[diskMap.size() - 1];
		while (start < stop)
		{
			if (*start != -1)
			{
				start++;
				continue;
			}

			if (*stop == -1)
			{
				stop--;
				continue;
			}

			swap(*start++, *stop--);
		}
	}

	int64_t answer = 0;
	for (size_t i = 0; i < diskMap.size(); i++)
	{
		if (diskMap[i] != -1)
		{
			answer += i * diskMap[i];
		}
	}

	printf("[2024] Puzzle09_A: %" PRId64 "\n", answer);
}

static void Puzzle09_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	string compressedDiskMap = ReadAllLines(input).front();

	list<Block> diskMap;
	vector<list<Block>::iterator> blockStartingLocations;
	int biggestBlockId = -1;
	for (int mapIndex = 0; mapIndex < (int)compressedDiskMap.size(); mapIndex++)
	{
		int writeId = mapIndex % 2 == 0 ? (mapIndex / 2) : -1;
		int count = compressedDiskMap[mapIndex] - '0';
		list<Block>::iterator blockLocation = diskMap.insert(diskMap.end(), Block{ writeId, count });
		if (writeId != -1)
		{
			blockStartingLocations.push_back(blockLocation);
		}
		biggestBlockId = max(biggestBlockId, writeId);
	}

	for (int blockId = biggestBlockId; blockId > 0; blockId--)
	{
		// Grab the block
		auto blockLocation = blockStartingLocations[blockId];

		// Try to find a home for it
		for (list<Block>::iterator candidateLocation = diskMap.begin();
			candidateLocation != blockLocation;
			++candidateLocation)
		{
			if ((candidateLocation->Id == -1) && (candidateLocation->Size >= blockLocation->Size))
			{
				// Split the space
				int remainingSpace = candidateLocation->Size - blockLocation->Size;
				if (remainingSpace > 0)
				{
					list<Block>::iterator afterCandidate = candidateLocation;
					diskMap.insert(++afterCandidate, Block{ -1, remainingSpace });
				}

				// Copy the file
				candidateLocation->Id = blockLocation->Id;
				candidateLocation->Size = blockLocation->Size;

				// Delete the old one
				blockLocation->Id = -1;

				break;
			}
		}
	}

	vector<int> flattenedImage;
	flattenedImage.reserve(compressedDiskMap.size() * 10);
	for (const Block& block : diskMap)
	{
		for (int i = 0; i < block.Size; i++)
		{
			flattenedImage.push_back(block.Id);
		}
	}

	int64_t answer = 0;
	for (size_t i = 0; i < flattenedImage.size(); i++)
	{
		if (flattenedImage[i] != -1)
		{
			answer += i * flattenedImage[i];
		}
	}

	printf("[2024] Puzzle09_B: %" PRId64 "\n", answer);
}

void Puzzle09_2024(const string& filename)
{
	Puzzle09_A(filename);
	Puzzle09_B(filename);
}
