#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle24_2024_Types
{
	struct Gate
	{
		size_t Id;
		string A, B;
		string Output;
		string Operation;
		int WaitCount = 2;
	};

	struct Node
	{
		int64_t Value = -1;
		vector<shared_ptr<Gate>> FollowOn;
	};

	struct Puzzle
	{
		map<string, shared_ptr<Node>> Nodes;
		vector<shared_ptr<Gate>> Gates;

		vector<pair<string, int64_t>> ExecQueue;
	};
}

using namespace Puzzle24_2024_Types;

static Puzzle ParsePuzzle(istream& input)
{
	Puzzle p;

	regex nodeRegex(R"((\w+): (\d))");
	regex gateRegex(R"((\w+) (\w+) (\w+) -> (\w+))");

	for (const string& line : ReadAllLines(input))
	{
		smatch match;
		if (regex_match(line, match, nodeRegex))
		{
			shared_ptr<Node> newNode{ new Node() };
			p.Nodes[match[1].str()] = newNode;

			p.ExecQueue.push_back({ match[1].str(), stoi(match[2].str()) });
		}
		else if (regex_match(line, match, gateRegex))
		{
			shared_ptr<Gate> newGate{ new Gate() };
			newGate->Id = p.Gates.size();
			newGate->A = match[1].str();
			newGate->B = match[3].str();
			newGate->Output = match[4].str();
			newGate->Operation = match[2].str();

			if (p.Nodes.contains(newGate->A) == false)
			{
				p.Nodes.insert({ newGate->A, shared_ptr<Node>{ new Node() } });
			}

			if (p.Nodes.contains(newGate->B) == false)
			{
				p.Nodes.insert({ newGate->B, shared_ptr<Node>{ new Node() } });
			}

			if (p.Nodes.contains(newGate->Output) == false)
			{
				p.Nodes.insert({ newGate->Output, shared_ptr<Node>{ new Node() } });
			}

			p.Nodes[newGate->A]->FollowOn.push_back(newGate);
			p.Nodes[newGate->B]->FollowOn.push_back(newGate);

			p.Gates.push_back(newGate);
		}
	}

	return p;
}

static void SetNumber(char c, int64_t value, Puzzle* p)
{
	for (int i = 0; i < sizeof(value) * CHAR_BIT; i++)
	{
		int64_t bit = (value >> i) & 0x1;

		char buffer[8];
		sprintf(buffer, "%c%02d", c, i);
		string nodeName{ buffer };
		if (p->Nodes.contains(nodeName))
		{
			p->ExecQueue.push_back({ string{ buffer }, bit });
		}
	}
}

static int64_t Execute(int64_t x, int64_t y, Puzzle* p)
{
	for (shared_ptr<Gate> g : p->Gates)
	{
		g->WaitCount = 2;
	}

	for (auto& nodePair : p->Nodes)
	{
		nodePair.second->Value = -1;
	}

	p->ExecQueue.clear();

	SetNumber('x', x, p);
	SetNumber('y', y, p);

	for (size_t i = 0; i < p->ExecQueue.size(); i++)
	{
		shared_ptr<Node> assignedNode = p->Nodes.at(p->ExecQueue[i].first);
		assert(assignedNode->Value == -1);
		assignedNode->Value = p->ExecQueue[i].second;

		for (shared_ptr<Gate> g : assignedNode->FollowOn)
		{
			g->WaitCount--;
			assert(g->WaitCount >= 0);
			if (g->WaitCount == 0)
			{
				int64_t result = -1;
				if (g->Operation == "AND")
				{
					result = p->Nodes.at(g->A)->Value & p->Nodes.at(g->B)->Value;
				}
				else if (g->Operation == "OR")
				{
					result = p->Nodes.at(g->A)->Value | p->Nodes.at(g->B)->Value;
				}
				else if (g->Operation == "XOR")
				{
					result = p->Nodes.at(g->A)->Value ^ p->Nodes.at(g->B)->Value;
				}
				else
				{
					assert(false);
				}

				p->ExecQueue.push_back({ g->Output, result });
			}
		}
	}

	int64_t z = 0;

	for (const auto& nodePair : p->Nodes)
	{
		if (nodePair.first[0] == 'z')
		{
			int shift = atoi(nodePair.first.data() + 1);
			z |= nodePair.second->Value << shift;
		}
	}

	return z;
}

static bool IsNetworkOkay(Puzzle p)
{
	for (int64_t i = 0; i < 41; i++)
	{
		for (int64_t x = 0; x < (1ll << 3); x++)
		{
			for (int64_t y = 0; y < (1ll << 3); y++)
			{
				int64_t zFromNetwork = Execute(x << i, y << i, &p);
				int64_t zShouldBe = (x << i) + (y << i);
				if (zFromNetwork != zShouldBe)
				{
					return false;
				}
			}
		}
	}
	return true;
}

static bool RepairPuzzle(const Puzzle& brokenPuzzle, const vector<string>& mistakeCandidates, Puzzle *repairedPuzzle, vector<string> *actualMistakes)
{
	assert(mistakeCandidates.size() >= 8);
	if (mistakeCandidates.size() == 8)
	{
		vector<string> assumedMistakes = mistakeCandidates;
		do
		{
			Puzzle attemptedRepair = brokenPuzzle;

			function<void(const string&, const string&)> FixSwap =
				[&](const string& a, const string& b)
				{
					for (shared_ptr<Gate> g : attemptedRepair.Gates)
					{
						if (g->Output == a)
						{
							g->Output = b;
						}
						else if (g->Output == b)
						{
							g->Output = a;
						}
					}
				};

			for (size_t i = 0; (i + 1) < assumedMistakes.size(); i += 2)
			{
				FixSwap(assumedMistakes[i + 0], assumedMistakes[i + 1]);
			}

			if (IsNetworkOkay(attemptedRepair))
			{
				*repairedPuzzle = attemptedRepair;
				*actualMistakes = assumedMistakes;
				return true;
			}

		} while (next_permutation(assumedMistakes.begin(), assumedMistakes.end()));

		return false;
	}

	for (size_t i = 0; i < mistakeCandidates.size(); i++)
	{
		vector<string> reducedCandidates = mistakeCandidates;
		reducedCandidates.erase(reducedCandidates.begin() + i);
		if (RepairPuzzle(brokenPuzzle, reducedCandidates, repairedPuzzle, actualMistakes))
		{
			return true;
		}
	}

	return false;
}

static void Puzzle24_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Puzzle p = ParsePuzzle(input);

	for (size_t i = 0; i < p.ExecQueue.size(); i++)
	{
		shared_ptr<Node> assignedNode = p.Nodes.at(p.ExecQueue[i].first);
		assignedNode->Value = p.ExecQueue[i].second;

		for (shared_ptr<Gate> g : assignedNode->FollowOn)
		{
			g->WaitCount--;
			assert(g->WaitCount >= 0);
			if (g->WaitCount == 0)
			{
				int64_t result = -1;
				if (g->Operation == "AND")
				{
					result = p.Nodes.at(g->A)->Value & p.Nodes.at(g->B)->Value;
				}
				else if (g->Operation == "OR")
				{
					result = p.Nodes.at(g->A)->Value | p.Nodes.at(g->B)->Value;
				}
				else if (g->Operation == "XOR")
				{
					result = p.Nodes.at(g->A)->Value ^ p.Nodes.at(g->B)->Value;
				}
				else
				{
					assert(false);
				}

				p.ExecQueue.push_back({ g->Output, result });
			}
		}
	}

	int64_t answer = 0;

	for (const auto& nodePair : p.Nodes)
	{
		if (nodePair.first[0] == 'z')
		{
			int shift = atoi(nodePair.first.data() + 1);
			answer |= nodePair.second->Value << shift;
		}
	}

	printf("[2024] Puzzle24_A: %" PRId64 "\n", answer);
}

static void Puzzle24_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Puzzle p = ParsePuzzle(input);

	// This solution makes the following assumptions about the construction of the puzzle:
	// * The first adder is correct in all puzzle inputs
	// * Wires are ony switched within the context of an individual adder, there
	//   are no cases where part of one adder swaps with part of another adder

	for (int64_t x = 0; x < 2; x++)
	{
		for (int64_t y = 0; y < 2; y++)
		{
			int64_t z = Execute(x, y, &p);
			assert(z == (x + y));
			(void)z;
		}
	}

	// The puzzle specification says that gate outputs have been swapped. The input pins can therefore
	// be assumed to be pointing to the correct gates and so we can start to identify the components
	// in the individual adders

	// Full adder schematic:
	// https://en.wikipedia.org/wiki/Adder_(electronics)#/media/File:Full-adder_logic_diagram.svg
	//
	// My names for the gates:
	//
	// A xor B -> H = Half Adder
	// Cin xor X -> S = Adder Output
	// H and Cin -> Cf = Carry Forward
	// A and B -> C = Carry
	// C or Cf -> Cout

	// Identify the half adder and carry gates
	set<size_t> halfAdders;
	set<size_t> carries;
	for (const auto &gate : p.Gates)
	{
		bool pinOnlyInput = isdigit(gate->A[1]) && isdigit(gate->B[1]);
		if (pinOnlyInput)
		{
			if (gate->Operation == "XOR"sv)
			{
				halfAdders.insert(gate->Id);
			}
			else if (gate->Operation == "AND"sv)
			{
				carries.insert(gate->Id);
			}
		}

	}

	// By process of elimination we know the adder output and carry forward gates
	set<size_t> adderOutputs;
	set<size_t> carryForwards;
	for (const auto& gate : p.Gates)
	{
		if ((gate->Operation == "XOR"sv) && (halfAdders.contains(gate->Id) == false))
		{
			adderOutputs.insert(gate->Id);
		}
		else if ((gate->Operation == "AND"sv) && (carries.contains(gate->Id) == false))
		{
			carryForwards.insert(gate->Id);
		}
	}

	// All other gates are carry out gates
	set<size_t> carryOuts;
	ranges::copy(p.Gates
		| views::filter([](const auto& gate) { return gate->Operation == "OR"sv; })
		| views::transform([](const auto& gate) { return gate->Id; }),
		inserter(carryOuts, carryOuts.end()));

	// Outputs that might be wrong
	vector<string> mistakeCandidates;

	auto gateIndexToId = [&](size_t sourceGateIndex)
		{
			return p.Gates[sourceGateIndex]->Output;
		};

	auto doesntGoToAdderOutput = [&](size_t sourceGateIndex)
		{
			return ranges::none_of(p.Nodes.at(p.Gates[sourceGateIndex]->Output)->FollowOn,
				[&](const auto& gate)
				{
					return adderOutputs.contains(gate->Id);
				});
		};

	auto doesntGoToOutputNode = [&](size_t sourceGateIndex)
		{
			const auto& sourceGate = p.Gates[sourceGateIndex];
			bool isOutputNode = (sourceGate->Output[0] == 'z') && (isdigit(sourceGate->Output[1]));
			return !isOutputNode;
		};

	auto doesntGoToCarryOut = [&](size_t sourceGateIndex)
		{
			return ranges::none_of(p.Nodes.at(p.Gates[sourceGateIndex]->Output)->FollowOn,
				[&](const auto& gate)
				{
					return carryOuts.contains(gate->Id);
				});
		};

	// All of the half adder and carry out outputs that don't go to an adder output node
	ranges::copy(halfAdders
		| views::filter(doesntGoToAdderOutput)
		| views::transform(gateIndexToId)
		| views::filter([](const auto& g) { return g != "z00"sv; }), // The first half adder goes straight to output
		back_inserter(mistakeCandidates));
	ranges::copy(carryOuts
		| views::filter(doesntGoToAdderOutput)
		| views::transform(gateIndexToId)
		| views::filter([](const auto& g) { return g != "z45"sv; }), // The last carry out goes straight to output
		back_inserter(mistakeCandidates));

	// All of the adder outputs that don't go to an output node
	ranges::copy(adderOutputs | views::filter(doesntGoToOutputNode) | views::transform(gateIndexToId), back_inserter(mistakeCandidates));

	// All of the carry gates and carry forwards that don't go to a carry out
	ranges::copy(carries
		| views::filter(doesntGoToCarryOut)
		| views::filter([&](size_t gateIndex)
			{
				return !p.Gates[gateIndex]->A.ends_with("00"sv) && !p.Gates[gateIndex]->B.ends_with("00"sv); // The first carry goes straight to the next stage
			})
		| views::transform(gateIndexToId),
		back_inserter(mistakeCandidates));
	ranges::copy(carryForwards | views::filter(doesntGoToCarryOut) | views::transform(gateIndexToId), back_inserter(mistakeCandidates));

	ranges::sort(mistakeCandidates);
	auto duplicates = ranges::unique(mistakeCandidates);
	mistakeCandidates.erase(duplicates.begin(), duplicates.end());

	assert(mistakeCandidates.size() == 8); // Wishful thinking?

	// Repair the network - if we've only got 8 candidates then this isn't necessary, but belt'n'brances approach
	Puzzle repairedNetwork;
	vector<string> mistakes;
	bool repaired = RepairPuzzle(p, mistakeCandidates, &repairedNetwork, &mistakes);
	assert(repaired);
	(void)repaired;

	assert(IsNetworkOkay(repairedNetwork));

	string answer;
	ranges::sort(mistakes);
	for (const string& s : mistakes)
	{
		answer += s;
		answer += ',';
	}

	printf("[2024] Puzzle24_B: %s\n", answer.substr(0, answer.size() - 1).c_str());
}

void Puzzle24_2024(const string& filename)
{
	Puzzle24_A(filename);
	Puzzle24_B(filename);
}
