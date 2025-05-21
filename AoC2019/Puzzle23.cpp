#include "stdafx.h"
#include "Intputer.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle23_2019_Types
{
}

using namespace Puzzle23_2019_Types;

static pair<int64_t, int64_t> RunUntilIdle(vector<Intputer>* nics, vector<IntputerIO>* network)
{
	pair<int64_t, int64_t> nat;
	size_t activeCards = 0;
	do
	{
		activeCards = 0;
		for (size_t card = 0; card < nics->size(); card++)
		{
			Intputer& nic = (*nics)[card];
			IntputerIO& io = (*network)[card];

			if (io.Read.empty())
			{
				io.Read.push_back(-1);
			}

			auto exec = nic.Execute();
			assert(exec == Intputer::ExecutionResult::PendingIo);
			(void)exec;

			if (io.Write.empty() == false)
			{
				activeCards++;

				assert((io.Write.size() % 3) == 0);

				for (int64_t packet = 0; packet < (int64_t)io.Write.size(); packet += 3)
				{
					int64_t packetX = io.Write[packet + 1];
					int64_t packetY = io.Write[packet + 2];
					int64_t address = io.Write[packet + 0];

					if (address == 255)
					{
						nat = { packetX, packetY };
					}
					else
					{
						(*network)[address].Read.push_back(packetX);
						(*network)[address].Read.push_back(packetY);
					}
				}

				io.Write.clear();
			}
		}

	} while (activeCards != 0);

	return nat;
}

static void Puzzle23_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<int64_t> firmware = ReadAsVectorOfNumbers(ReadSingleLine(input));

	size_t numDevices = 50;

	vector<Intputer> nics{ numDevices };
	vector<IntputerIO> network{ numDevices };

	for (size_t i = 0; i < nics.size(); i++)
	{
		nics[i].CopyProgram(firmware);
		nics[i].SetReadWriteQueues(&network[i]);
		network[i].Read.push_back(i);
		auto exec = nics[i].Execute();
		assert(exec == Intputer::ExecutionResult::PendingIo);
		(void)exec;
		assert(network[i].Write.size() == 0);
	}

	int64_t answer = RunUntilIdle(&nics, &network).second;
		
	printf("[2019] Puzzle23_A: %" PRId64 "\n", answer);
}


static void Puzzle23_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<int64_t> firmware = ReadAsVectorOfNumbers(ReadSingleLine(input));

	size_t numDevices = 50;

	vector<Intputer> nics{ numDevices };
	vector<IntputerIO> network{ numDevices };

	for (size_t i = 0; i < nics.size(); i++)
	{
		nics[i].CopyProgram(firmware);
		nics[i].SetReadWriteQueues(&network[i]);
		network[i].Read.push_back(i);
		auto exec = nics[i].Execute();
		assert(exec == Intputer::ExecutionResult::PendingIo);
		(void)exec;
		assert(network[i].Write.size() == 0);
	}

	int64_t answer = 0;

	set<pair<int64_t, int64_t>> packets;
	while (true)
	{
		auto restartPacket = RunUntilIdle(&nics, &network);
		if (packets.insert(restartPacket).second == false)
		{
			answer = restartPacket.second;
			break;
		}
		
		network[0].Read.push_back(restartPacket.first);
		network[0].Read.push_back(restartPacket.second);
	}

	printf("[2019] Puzzle23_B: %" PRId64 "\n", answer);
}

void Puzzle23_2019(const string& filename)
{
	Puzzle23_A(filename);
	Puzzle23_B(filename);
}
