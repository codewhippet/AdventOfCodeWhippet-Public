#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle19_2021_Types
{
	struct Scanner
	{
		vector<Vector3> Beacons;
	};

	static Vector3 BasisVectors[6] =
	{
		Vector3{  1,  0,  0 },
		Vector3{  0,  1,  0 },
		Vector3{  0,  0,  1 },

		Vector3{ -1,  0,  0 },
		Vector3{  0, -1,  0 },
		Vector3{  0,  0, -1 },
	};

	static vector<Matrix43> Rotations;
	static map<pair<size_t, size_t>, Matrix43> MostLikelyTransforms;
}

using namespace Puzzle19_2021_Types;

static vector<Scanner> ReadPuzzle(istream& input)
{
	regex scannerPattern(R"(--- scanner (\d+) ---)");
	regex beaconPattern(R"((-?\d+),(-?\d+),(-?\d+))");

	vector<Scanner> scanners;
	for (const string& line : ReadAllLines(input))
	{
		if (regex_match(line, scannerPattern))
		{
			scanners.push_back(Scanner{});
			continue;
		}
		if (regex_match(line, beaconPattern))
		{
			Vector3 beacon;
			sscanf(line.c_str(), "%" SCNd64 ",%" SCNd64 ",%" SCNd64,
				&beacon.X, &beacon.Y, &beacon.Z);

			assert(scanners.empty() == false);
			scanners.back().Beacons.push_back(beacon);
			continue;
		}
	}
	return scanners;
}

static void CreateRotations()
{
	if (Rotations.empty())
	{
		for (int i = 0; i < _countof(BasisVectors); i++)
		{
			for (int j = 0; j < _countof(BasisVectors); j++)
			{
				// Ignore same and parallel basis vectors
				if ((i == j) || (abs(i - j) == 3))
				{
					continue;
				}

				Matrix43 m = Matrix::MakeRotation(BasisVectors[i], BasisVectors[j], Cross(BasisVectors[i], BasisVectors[j]));
				Rotations.push_back(m);
			}
		}
	}
}

void TryToMatchBeacons(vector<Vector3> a, const vector<Vector3>& b, vector<Matrix43>* matchedTransform)
{
	assert(a.size() == b.size());

	sort(a.begin(), a.end());

	// Try all rotations
	set<size_t> matchedRotations;
	for (size_t rotationIndex = 0; rotationIndex < Rotations.size(); rotationIndex++)
	{
		vector<Vector3> b_rotated = MakeEnumerator(b)
			->Select<Vector3>([rotationIndex](const Vector3& v) { return v * Rotations[rotationIndex]; })
			->ToVector();
		sort(b_rotated.begin(), b_rotated.end());

		set<Vector3> differences;
		for (size_t beaconIndex = 0; beaconIndex < a.size(); beaconIndex++)
		{
			Vector3 diff = b_rotated[beaconIndex] - a[beaconIndex];
			differences.insert(diff);
		}
		if (differences.size() == 1)
		{
			matchedTransform->push_back(Rotations[rotationIndex] * Matrix::MakeTranslation(-(*differences.begin())));
		}
	}
}

static int64_t Manhattan(const Vector3& a, const Vector3& b)
{
	return (abs(b.X - a.X) + abs(b.Y - a.Y) + abs(b.Z - a.Z));
}

static void Puzzle19_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Scanner> scanners = ReadPuzzle(input);

	map<double, multimap<size_t, Point2>> pairsToScanners;
	for (size_t scannerIndex = 0; scannerIndex < scanners.size(); scannerIndex++)
	{
		const Scanner& scanner = scanners[scannerIndex];
		for (size_t i = 0; i < scanner.Beacons.size(); i++)
		{
			for (size_t j = i + 1; j < scanner.Beacons.size(); j++)
			{
				Vector3 p = scanner.Beacons[i];
				Vector3 q = scanner.Beacons[j];

				Vector3 diff = q - p;
				double length = sqrt(Dot(diff, diff));

				pairsToScanners[length].insert(make_pair(scannerIndex, Point2{ (int64_t)i, (int64_t)j }));
			}
		}
	}

	for (map<double, multimap<size_t, Point2>>::const_iterator pairIt = pairsToScanners.begin(); pairIt != pairsToScanners.end(); /***/)
	{
		if (pairIt->second.size() > 1)
		{
			pairIt++;
		}
		else
		{
			pairIt = pairsToScanners.erase(pairIt);
		}
	}

	// Match!
	map<pair<size_t, size_t>, vector<Matrix43>> candidateTransforms;
	for (map<double, multimap<size_t, Point2>>::const_iterator pairIt = pairsToScanners.begin(); pairIt != pairsToScanners.end(); pairIt++)
	{
		const multimap<size_t, Point2>& scannerToIndices = pairIt->second;
		for (multimap<size_t, Point2>::const_iterator scannerAIt = scannerToIndices.begin(); scannerAIt != scannerToIndices.end(); scannerAIt++)
		{
			multimap<size_t, Point2>::const_iterator scannerBIt = scannerAIt;
			if (++scannerBIt == scannerToIndices.end())
			{
				continue;
			}

			for (/***/; scannerBIt != scannerToIndices.end(); scannerBIt++)
			{
				size_t scannerA = scannerAIt->first;
				Point2 beaconIndicesFromA = scannerAIt->second;

				size_t scannerB = scannerBIt->first;
				Point2 beaconIndicesFromB = scannerBIt->second;

				vector<Vector3> beaconsFromA;
				beaconsFromA.push_back(scanners[scannerA].Beacons[beaconIndicesFromA.X]);
				beaconsFromA.push_back(scanners[scannerA].Beacons[beaconIndicesFromA.Y]);

				vector<Vector3> beaconsFromB;
				beaconsFromB.push_back(scanners[scannerB].Beacons[beaconIndicesFromB.X]);
				beaconsFromB.push_back(scanners[scannerB].Beacons[beaconIndicesFromB.Y]);

				TryToMatchBeacons(beaconsFromA, beaconsFromB, &candidateTransforms[make_pair(scannerB, scannerA)]);
			}
		}
	}

	for (map<pair<size_t, size_t>, vector<Matrix43>>::const_iterator candidate = candidateTransforms.begin();
		candidate != candidateTransforms.end(); /***/)
	{
		if (candidate->second.size() < 10)
		{
			candidate = candidateTransforms.erase(candidate);
		}
		else
		{
			++candidate;
		}
	}

	for (map<pair<size_t, size_t>, vector<Matrix43>>::const_reference candidate : candidateTransforms)
	{
		pair<size_t, size_t> fromTo = candidate.first;
		map<Matrix43, int> transformCounts;
		for (const Matrix43& t : candidate.second)
		{
			transformCounts[t]++;
		}

		pair<Matrix43, int> mostCommon = *transformCounts.begin();
		for (const pair<Matrix43, int>& vote : transformCounts)
		{
			if (vote.second > mostCommon.second)
			{
				mostCommon = vote;
			}
		}

		MostLikelyTransforms[fromTo] = mostCommon.first;
	}

	// Make all matrices
	size_t allMatrixCombinations = (scanners.size() * scanners.size()) - scanners.size();
	while (MostLikelyTransforms.size() < allMatrixCombinations)
	{
		for (size_t from = 0; from < scanners.size(); from++)
		{
			for (size_t to = 0; to < scanners.size(); to++)
			{
				if (from == to)
					continue;

				map<pair<size_t, size_t>, Matrix43>::const_iterator fromToTo = MostLikelyTransforms.find(make_pair(from, to));
				if (fromToTo != MostLikelyTransforms.end())
				{
					MostLikelyTransforms[make_pair(to, from)] = Matrix::InvertOrthonormal(fromToTo->second);
					continue;
				}

				for (size_t intermediate = 0; intermediate < scanners.size(); intermediate++)
				{
					if ((intermediate == from) || (intermediate == to))
						continue;

					map<pair<size_t, size_t>, Matrix43>::const_iterator fromToIntermediate;
					map<pair<size_t, size_t>, Matrix43>::const_iterator intermediateToTo;

					fromToIntermediate = MostLikelyTransforms.find(make_pair(from, intermediate));
					intermediateToTo = MostLikelyTransforms.find(make_pair(intermediate, to));

					if ((fromToIntermediate != MostLikelyTransforms.end()) && (intermediateToTo != MostLikelyTransforms.end()))
					{
						MostLikelyTransforms[make_pair(from, to)] = fromToIntermediate->second * intermediateToTo->second;
					}
				}
			}
		}
	}

	set<Vector3> allUniqueBeacons = MakeEnumerator(scanners[0].Beacons)->ToSet();
	for (size_t scannerIndex = 1; scannerIndex < scanners.size(); scannerIndex++)
	{
		Matrix43 transformToScannerZero = MostLikelyTransforms[make_pair(scannerIndex, 0)];
		vector<Vector3> transformedBeacons = MakeEnumerator(scanners[scannerIndex].Beacons)
			->Select<Vector3>([transformToScannerZero](const Vector3& v) { return v * transformToScannerZero; })
			->ToVector();

		for (Vector3 v : transformedBeacons)
		{
			allUniqueBeacons.insert(v);
		}
	}

	int64_t answer = (int64_t)allUniqueBeacons.size();

	printf("[2021] Puzzle19_A: %" PRId64 "\n", answer);
}

static void Puzzle19_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Scanner> scanners = ReadPuzzle(input);

	int64_t manhattan = 0;
	for (size_t i = 0; i < scanners.size(); i++)
	{
		for (size_t j = 0; j < scanners.size(); j++)
		{
			if (i == j)
				continue;

			manhattan = max(manhattan, Manhattan(Vector3{ 0, 0, 0 }, Vector3{ 0, 0, 0 } *MostLikelyTransforms[make_pair(j, i)]));
		}
	}

	int64_t answer = manhattan;

	printf("[2021] Puzzle19_B: %" PRId64 "\n", answer);
}

void Puzzle19_2021(const string& filename)
{
	CreateRotations();

	Puzzle19_A(filename);
	Puzzle19_B(filename);
}
