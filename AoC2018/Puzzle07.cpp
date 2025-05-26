#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle07_2018_Types
{
	struct Step
	{
		char Id = '\0';
		int64_t BlockedByCount = 0;
		vector<char> Unblocks;
		int64_t WorkRequired = 0;
		int64_t WorkPerformed = 0;
	};

	struct StepDictionary
	{
		shared_ptr<Step> GetOrCreate(char id)
		{
			auto existingIt = Steps.find(id);
			if (existingIt == Steps.end())
			{
				shared_ptr<Step> newStep = make_shared<Step>();
				newStep->Id = id;
				existingIt = Steps.insert({ id, newStep }).first;
			}
			return existingIt->second;
		}

		map<char, shared_ptr<Step>> Steps;
	};

	struct Worker
	{
		shared_ptr<Step> CurrentTask;
	};
}

using namespace Puzzle07_2018_Types;

static void Puzzle07_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	StepDictionary steps;
	for (const auto& line : ScanfEachLine<char, char>(input, "Step %c must be finished before step %c can begin."))
	{
		shared_ptr<Step> stepBefore = steps.GetOrCreate(get<0>(line));
		shared_ptr<Step> stepAfter = steps.GetOrCreate(get<1>(line));

		stepBefore->Unblocks.push_back(get<1>(line));
		stepAfter->BlockedByCount++;
	}

	set<char> executionQueue;
	ranges::copy(steps.Steps
		| views::filter([](const auto& kvp) { return kvp.second->BlockedByCount == 0; })
		| views::keys,
		inserter(executionQueue, executionQueue.end()));

	string answer;
	while (executionQueue.empty() == false)
	{
		auto headOfQueue = executionQueue.begin();
		char stepToExecute = *headOfQueue;
		answer += stepToExecute;
		shared_ptr<Step> firstStep = steps.GetOrCreate(stepToExecute);
		for (char c : firstStep->Unblocks)
		{
			if (--steps.GetOrCreate(c)->BlockedByCount == 0)
			{
				executionQueue.insert(c);
			}
		}
		executionQueue.erase(headOfQueue);
	}

	printf("[2018] Puzzle07_A: %s\n", answer.c_str());
}


static void Puzzle07_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const int64_t stepBaseCost = 60;
	const size_t numWorkers = 5;

	StepDictionary steps;
	for (const auto& line : ScanfEachLine<char, char>(input, "Step %c must be finished before step %c can begin."))
	{
		shared_ptr<Step> stepBefore = steps.GetOrCreate(get<0>(line));
		shared_ptr<Step> stepAfter = steps.GetOrCreate(get<1>(line));

		stepBefore->WorkRequired = (get<0>(line) - 'A' + 1) + stepBaseCost;
		stepAfter->WorkRequired = (get<1>(line) - 'A' + 1) + stepBaseCost;

		stepBefore->Unblocks.push_back(get<1>(line));
		stepAfter->BlockedByCount++;
	}

	set<char> executionQueue;
	ranges::copy(steps.Steps
		| views::filter([](const auto& kvp) { return kvp.second->BlockedByCount == 0; })
		| views::keys,
		inserter(executionQueue, executionQueue.end()));

	vector<Worker> workers(numWorkers);

	int64_t elapsedTime = 0;
	while (true)
	{
		// Idle workers pick up new tasks
		for (Worker& w : workers)
		{
			if (!w.CurrentTask)
			{
				auto headOfQueue = executionQueue.begin();
				if (headOfQueue != executionQueue.end())
				{
					char stepToExecute = *headOfQueue;
					w.CurrentTask = steps.Steps.at(stepToExecute);
					executionQueue.erase(headOfQueue);
				}
			}
		}

		// If there's nothing to do, we're done
		if (ranges::none_of(workers, [](const Worker& w) { return (bool)w.CurrentTask; }))
		{
			break;
		}

		// Workers progress tasks
		int64_t completedThisFrame = 0;
		for (Worker& w : workers)
		{
			if (w.CurrentTask)
			{
				if (++w.CurrentTask->WorkPerformed == w.CurrentTask->WorkRequired)
				{
					for (char c : w.CurrentTask->Unblocks)
					{
						if (--steps.Steps.at(c)->BlockedByCount == 0)
						{
							executionQueue.insert(c);
						}
					}
					w.CurrentTask.reset();
					completedThisFrame++;
				}
			}
		}
		assert(completedThisFrame < 2);

		elapsedTime++;
	}

	int64_t answer = elapsedTime;

	printf("[2018] Puzzle07_B: %" PRId64 "\n", answer);
}

void Puzzle07_2018(const string& filename)
{
	Puzzle07_A(filename);
	Puzzle07_B(filename);
}
