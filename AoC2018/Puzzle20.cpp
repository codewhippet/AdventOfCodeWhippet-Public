#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle20_2018_Types
{
	struct Regex
	{
		string Pattern;
		vector<vector<size_t>> Branches;
		vector<size_t> ScopeEnd;
	};

	struct Facility
	{
		set<Point2> Rooms;
		set<pair<Point2, Point2>> Doors;
	};

	struct SearchNode
	{
		size_t StartFrom;
		Point2 RoomLocation;

		auto operator<=>(const SearchNode&) const = default;
	};

	struct SearchQueue
	{
		void Add(const SearchNode& node)
		{
			if (Queued.contains(node) == false)
			{
				Queue.push_back(node);
				Queued.insert(node);
			}
		}

		bool IsEmpty() const
		{
			return Queue.empty();
		}

		SearchNode GetFirst()
		{
			SearchNode current = Queue.front();
			Queue.pop_front();
			return current;
		}

	private:
		deque<SearchNode> Queue;
		set<SearchNode> Queued;
	};
}

using namespace Puzzle20_2018_Types;

static size_t FormBranches(Regex* r, size_t patternIndex)
{
	assert(r->Pattern[patternIndex] == '(');
	size_t activeBranchIndex = patternIndex;
	r->Branches[activeBranchIndex].push_back(patternIndex);
	patternIndex++;
	

	while (patternIndex < r->Pattern.size())
	{
		switch (r->Pattern[patternIndex])
		{
		case '(':
			patternIndex = FormBranches(r, patternIndex);
			break;

		case '|':
			r->Branches[activeBranchIndex].push_back(patternIndex);
			patternIndex++;
			break;

		case ')':
			return patternIndex + 1;

		default:
			patternIndex++;
			break;
		}
	}

	return patternIndex;
}

static void FormScopeEnds(Regex* r)
{
	stack<size_t> scopes;
	scopes.push(r->Pattern.size());
	for (int64_t patternIndex = r->Pattern.size() - 1; patternIndex > -1; patternIndex--)
	{
		switch (r->Pattern[patternIndex])
		{
		case '(':
			r->ScopeEnd[patternIndex] = scopes.top();
			scopes.pop();
			break;

		case ')':
			scopes.push(patternIndex);
			r->ScopeEnd[patternIndex] = scopes.top();
			break;

		default:
			r->ScopeEnd[patternIndex] = scopes.top();
			break;
		}
	}
}

static Regex ParseRegex(istream& input)
{
	Regex r;

	r.Pattern = ReadSingleLine(input);
	r.Pattern.front() = '(';
	r.Pattern.back() = ')';

	r.Branches.resize(r.Pattern.size());
	r.ScopeEnd.resize(r.Pattern.size());
	FormBranches(&r, 0);
	FormScopeEnds(&r);

	r.Pattern.front() = '^';
	r.Pattern.back() = '$';

	return r;
}

static void MapFacility(const Regex& regex, const Point2& startingPoint, Facility* facility)
{
	SearchQueue queue;
	queue.Add({ 0, startingPoint });

	while (queue.IsEmpty() == false)
	{
		SearchNode current = queue.GetFirst();
		Point2 roomLocation = current.RoomLocation;

		for (size_t patternIndex = current.StartFrom; patternIndex < regex.Pattern.size(); /***/)
		{
			facility->Rooms.insert(roomLocation);

			switch (regex.Pattern[patternIndex])
			{
			case '^':
			case '$':
				patternIndex++;
				break;

			case '(':
				for (size_t branchTarget : regex.Branches[patternIndex])
				{
					queue.Add({ branchTarget + 1, roomLocation });
				}
				patternIndex = regex.Pattern.size();
				break;

			case '|':
			case ')':
				queue.Add({ regex.ScopeEnd[patternIndex] + 1, roomLocation });
				patternIndex = regex.Pattern.size();
				break;

			case 'N':
				{
					Point2 nextRoom = roomLocation + Point2::North();
					facility->Doors.insert({ roomLocation, nextRoom });
					facility->Doors.insert({ nextRoom, roomLocation });
					roomLocation = nextRoom;
					patternIndex++;
				}
				break;

			case 'E':
				{
					Point2 nextRoom = roomLocation + Point2::East();
					facility->Doors.insert({ roomLocation, nextRoom });
					facility->Doors.insert({ nextRoom, roomLocation });
					roomLocation = nextRoom;
					patternIndex++;
				}
				break;

			case 'S':
				{
					Point2 nextRoom = roomLocation + Point2::South();
					facility->Doors.insert({ roomLocation, nextRoom });
					facility->Doors.insert({ nextRoom, roomLocation });
					roomLocation = nextRoom;
					patternIndex++;
				}
				break;

			case 'W':
				{
					Point2 nextRoom = roomLocation + Point2::West();
					facility->Doors.insert({ roomLocation, nextRoom });
					facility->Doors.insert({ nextRoom, roomLocation });
					roomLocation = nextRoom;
					patternIndex++;
				}
				break;

			default:
				assert(false);
				break;
			}
		}
	}
}

static map<int64_t, set<Point2>> RoomDistances(const Facility& facility, const Point2& startingPoint)
{
	vector<pair<Point2, int64_t>> searchQueue{ { startingPoint, 0 } };

	map<int64_t, set<Point2>> roomDistances;

	set<Point2> visited;
	for (size_t i = 0; i < searchQueue.size(); i++)
	{
		pair<Point2, int64_t> current = searchQueue[i];
		if (visited.contains(current.first))
			continue;

		visited.insert(current.first);

		roomDistances[current.second].insert(current.first);

		for (const Point2& dir : Point2::CardinalDirections())
		{
			Point2 nextDoor = current.first + dir;
			if (facility.Doors.contains({ current.first, nextDoor }))
			{
				searchQueue.push_back({ nextDoor, current.second + 1 });
			}
		}
	}

	return roomDistances;
}

static void Puzzle20_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Regex r = ParseRegex(input);

	Facility facility;
	MapFacility(r, {}, &facility);

	int64_t answer = RoomDistances(facility, {}).crbegin()->first;

	printf("[2018] Puzzle20_A: %" PRId64 "\n", answer);
}


static void Puzzle20_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Regex r = ParseRegex(input);

	Facility facility;
	MapFacility(r, {}, &facility);

	map<int64_t, set<Point2>> roomDistances = RoomDistances(facility, {});

	auto furthestRooms = roomDistances
		| views::filter([](const auto& kvp) { return kvp.first >= 1000; })
		| views::transform([](const auto& kvp) { return kvp.second.size(); });

	int64_t answer = accumulate(furthestRooms.begin(), furthestRooms.end(), 0ll);

	printf("[2018] Puzzle20_B: %" PRId64 "\n", answer);
}

void Puzzle20_2018(const string& filename)
{
	Puzzle20_A(filename);
	Puzzle20_B(filename);
}
