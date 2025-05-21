#include "stdafx.h"
#include <generator>

using namespace std;

static string dummy =
R"()";

namespace Puzzle07_2022_Types
{
	struct Directory
	{
		string Name;
		int64_t LocalSize = 0;
		shared_ptr<Directory> Parent;
		vector<shared_ptr<Directory>> Children;
		int64_t TotalSize = 0;
	};

	template <typename CLASS, auto MEMBER>
	struct GetMember
	{
		auto operator()(const shared_ptr<CLASS> &ptr) const
		{
			return (ptr.get())->*MEMBER;
		}
	};

	struct LessEqual
	{
		int64_t Comparand;
		bool operator()(int64_t v) const
		{
			return v <= Comparand;
		}
	};

	struct GreaterEqual
	{
		int64_t Comparand;
		bool operator()(int64_t v) const
		{
			return v >= Comparand;
		}
	};
}

using namespace Puzzle07_2022_Types;

static shared_ptr<Directory> ParseDirectoryStructure(istream &input)
{
	shared_ptr<Directory> root;

	const regex cdPattern{ R"(\$ cd (.+))" };
	const regex filePattern{ R"((\d+) .+)" };

	shared_ptr<Directory> currentDirectory;
	for (const string &line : ReadEachLine(input))
	{
		smatch m;
		if (regex_match(line, m, cdPattern))
		{
			string dirName = m[1].str();
			if (dirName == "..")
			{
				currentDirectory = currentDirectory->Parent;
			}
			else
			{
				shared_ptr<Directory> dir{ new Directory() };
				dir->Name = dirName;
				dir->Parent = currentDirectory;

				if (currentDirectory)
				{
					currentDirectory->Children.push_back(dir);
				}

				currentDirectory = dir;

				if (!root)
				{
					root = currentDirectory;
				}
			}
		}
		else if (regex_match(line, m, filePattern))
		{
			int64_t fileSize = atoll(m[1].str().c_str());
			currentDirectory->LocalSize += fileSize;
		}
	}

	return root;
}

static void PopulateTotalSizes(const shared_ptr<Directory>& dir)
{
	for (const auto& child : dir->Children)
	{
		PopulateTotalSizes(child);
	}

	dir->TotalSize = dir->LocalSize + ranges::fold_left(dir->Children | views::transform([](const auto& d) { return d->TotalSize; }), 0ll, plus{});
}

static generator<const shared_ptr<Directory>&> ForAllDirectories(const shared_ptr<Directory>& dir)
{
	co_yield dir;

	for (const auto &child : dir->Children)
	{
		co_yield ranges::elements_of(ForAllDirectories(child));
	}
}

static void Puzzle07_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	shared_ptr<Directory> root = ParseDirectoryStructure(input);
	PopulateTotalSizes(root);

	int64_t answer = ranges::fold_left(ForAllDirectories(root)
		| views::transform(GetMember<Directory, &Directory::TotalSize>{})
		| views::filter(LessEqual{ 100000 }),
		0ll,
		plus{});

	printf("[2022] Puzzle07_A: %" PRId64 "\n", answer);
}

static void Puzzle07_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	shared_ptr<Directory> root = ParseDirectoryStructure(input);
	PopulateTotalSizes(root);

	int64_t freeSpace = 70000000 - root->TotalSize;
	int64_t neededSpace = 30000000;

	vector<int64_t> dirSizes = ForAllDirectories(root)
		| views::transform(GetMember<Directory, &Directory::TotalSize>{})
		| views::filter(GreaterEqual{ neededSpace - freeSpace })
		| ranges::to<vector>();

	ranges::sort(dirSizes);
	int64_t answer = dirSizes.front();

	printf("[2022] Puzzle07_B: %" PRId64 "\n", answer);
}

void Puzzle07_2022(const string& filename)
{
	Puzzle07_A(filename);
	Puzzle07_B(filename);
}
