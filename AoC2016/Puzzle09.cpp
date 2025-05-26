#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle09_2016_Types
{
}

using namespace Puzzle09_2016_Types;

static int64_t DecompressedLength(const string& compressed, size_t begin, size_t end)
{
	int64_t decompressedSize = 0;
	for (size_t compressedIndex = begin; compressedIndex < end; /***/)
	{
		if (compressed[compressedIndex] != '(')
		{
			decompressedSize++;
			compressedIndex++;
		}
		else
		{
			int64_t count = 0;
			int64_t repeat = 0;
			int charactersRead = 0;
			int scanned = sscanf(&compressed[compressedIndex], "(%lldx%lld)%n",
				&count,
				&repeat,
				&charactersRead);
			assert(scanned == 2);
			(void)scanned;

			size_t segmentBegin = compressedIndex + charactersRead;
			size_t segmentEnd = segmentBegin + count;

			decompressedSize += DecompressedLength(compressed, segmentBegin, segmentEnd) * repeat;

			compressedIndex = segmentEnd;
		}
	}
	return decompressedSize;
}

static void Puzzle09_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	string compressed;
	getline(input, compressed);

	string decompressed;
	for (size_t compressedIndex = 0; compressedIndex < compressed.size(); /***/)
	{
		if (compressed[compressedIndex] != '(')
		{
			decompressed += compressed[compressedIndex];
			compressedIndex++;
		}
		else
		{
			int64_t count = 0;
			int64_t repeat = 0;
			int charactersRead = 0;
			int scanned = sscanf(&compressed[compressedIndex], "(%lldx%lld)%n",
				&count,
				&repeat,
				&charactersRead);
			assert(scanned == 2);
			(void)scanned;

			size_t sourceOffset = compressedIndex + charactersRead;
			string segmentToCopy(compressed, sourceOffset, count);
			for (int64_t copy = 0; copy < repeat; copy++)
			{
				decompressed += segmentToCopy;
			}

			compressedIndex = sourceOffset + count;
		}
	}

	int64_t answer = decompressed.size();

	printf("[2016] Puzzle09_A: %" PRId64 "\n", answer);
}


static void Puzzle09_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	string compressed;
	getline(input, compressed);

	int64_t answer = DecompressedLength(compressed, 0, compressed.size());

	printf("[2016] Puzzle09_B: %" PRId64 "\n", answer);
}

void Puzzle09_2016(const string& filename)
{
	Puzzle09_A(filename);
	Puzzle09_B(filename);
}
