#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle11_2025_Types
{
	struct Device
	{
		int64_t WaitingFor = 0;
		set<string> Unlocks;
		Vector3 Paths{ 0, 0, 0 };
	};
}

using namespace Puzzle11_2025_Types;

static map<string, Device> ReadDevices(istream& input)
{
	map<string, Device> devices;

	string line;
	while (getline(input, line))
	{
		string deviceId = line.substr(0, 3);
		for (const string& unlock : StringSplit(line.substr(5), ' '))
		{
			devices[deviceId].Unlocks.insert(unlock);
			devices[unlock].WaitingFor++;
		}
	}

	return devices;
}

static void Puzzle11_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	map<string, Device> devices = ReadDevices(input);

	devices["you"].Paths = { 1, 0, 0 };

	list<string> executionQueue = devices
		| views::filter([](const auto& p) { return p.second.WaitingFor == 0; })
		| views::keys
		| ranges::to<list>();
	while (executionQueue.empty() == false)
	{
		const Device& device = devices[executionQueue.front()];
		for (const string& unlock : device.Unlocks)
		{
			Device& nextDevice = devices[unlock];
			nextDevice.Paths = nextDevice.Paths + device.Paths;
			if (--nextDevice.WaitingFor == 0)
			{
				executionQueue.push_back(unlock);
			}
		}
		executionQueue.pop_front();
	}

	int64_t answer = devices["out"].Paths.X;

	printf("[2025] Puzzle11_A: %" PRId64 "\n", answer);
}

static void Puzzle11_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	map<string, Device> devices = ReadDevices(input);

	devices["svr"].Paths = { 1, 0, 0 };
	devices["fft"].Paths = { 0, 1, 0 };
	devices["dac"].Paths = { 0, 0, 1 };

	list<string> executionQueue = devices
		| views::filter([](const auto& p) { return p.second.WaitingFor == 0; })
		| views::keys
		| ranges::to<list>();
	while (executionQueue.empty() == false)
	{
		const Device& device = devices[executionQueue.front()];
		for (const string& unlock : device.Unlocks)
		{
			Device& nextDevice = devices[unlock];
			nextDevice.Paths = nextDevice.Paths + device.Paths;
			if (--nextDevice.WaitingFor == 0)
			{
				executionQueue.push_back(unlock);
			}
		}
		executionQueue.pop_front();
	}

	int64_t srvToFft = devices["fft"].Paths.X;
	int64_t fftToDac = devices["dac"].Paths.Y;
	int64_t dacToOut = devices["out"].Paths.Z;

	int64_t srvToDac = devices["dac"].Paths.X;
	int64_t dacToFft = devices["fft"].Paths.Z;
	int64_t fftToOut = devices["out"].Paths.Y;

	int64_t answer = (srvToFft * fftToDac * dacToOut) + (srvToDac * dacToFft * fftToOut);

	printf("[2025] Puzzle11_B: %" PRId64 "\n", answer);
}

void Puzzle11_2025(const string& filename)
{
	Puzzle11_A(filename);
	Puzzle11_B(filename);
}
