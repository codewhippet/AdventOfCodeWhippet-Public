#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle20_2022_Types
{
	struct Node
	{
		int64_t Value = 0;

		Node *Prev = nullptr;
		Node *Next = nullptr;

		void Detach()
		{
			Prev->Next = Next;
			Next->Prev = Prev;

			Prev = nullptr;
			Next = nullptr;
		}

		void InsertAfter(Node* after)
		{
			Prev = after;
			Next = after->Next;

			Prev->Next = this;
			Next->Prev = this;
		}

		void InsertBefore(Node* before)
		{
			Prev = before->Prev;
			Next = before;

			Prev->Next = this;
			Next->Prev = this;
		}
	};
}

using namespace Puzzle20_2022_Types;

static void ReadFile(istream& input, vector<Node> *file)
{
	ranges::copy(ScanfEachLine<int64_t>(input, "%lld") | views::transform([](const auto &line) { return Node{ .Value = get<0>(line) }; }), back_inserter(*file));
	for (size_t i = 0; i < file->size(); i++)
	{
		(*file)[i].Next = &(*file)[(i + 1) % file->size()];
		(*file)[i].Prev = &(*file)[(i + file->size() - 1) % file->size()];
	}
}

static int64_t GetFileSum(const vector<Node>& file)
{
	Node zero = *ranges::find_if(file, [](const Node& n) { return n.Value == 0; });
	const Node* ith = &zero;

	int64_t sum = 0;
	for (int64_t i = 0; i < 3001; i++)
	{
		if ((i == 1000) || (i == 2000) || (i == 3000))
		{
			sum += ith->Value;
		}

		ith = ith->Next;
	}
	return sum;
}

static void Mix(vector<Node> *file)
{
	for (size_t nodeIndex = 0; nodeIndex < file->size(); nodeIndex++)
	{
		Node *current = &(*file)[nodeIndex];
		if (current->Value > 0)
		{
			Node *insertBefore = current->Next;
			current->Detach();

			int64_t rotateBy = current->Value % (file->size() - 1);
			for (int64_t n = 0; n < rotateBy; n++)
			{
				insertBefore = insertBefore->Next;
			}
			assert(insertBefore != current);

			current->InsertBefore(insertBefore);
		}
		else if (current->Value < 0)
		{
			Node *insertAfter = current->Prev;
			current->Detach();

			int64_t rotateBy = -current->Value % (file->size() - 1);
			for (int64_t n = 0; n < rotateBy; n++)
			{
				insertAfter = insertAfter->Prev;
			}
			assert(insertAfter != current);

			current->InsertAfter(insertAfter);
		}
	}
}

static void Puzzle20_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Node> file;
	ReadFile(input, &file);

	Mix(&file);
	int64_t answer = GetFileSum(file);

	printf("[2022] Puzzle20_A: %" PRId64 "\n", answer);
}

static void Puzzle20_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Node> file;
	ReadFile(input, &file);
	ranges::for_each(file, [](Node &n) { n.Value *= 811589153ll; });

	for (int i = 0; i < 10; i++)
	{
		Mix(&file);
	}
	int64_t answer = GetFileSum(file);

	printf("[2022] Puzzle20_B: %" PRId64 "\n", answer);
}

void Puzzle20_2022(const string& filename)
{
	Puzzle20_A(filename);
	Puzzle20_B(filename);
}
