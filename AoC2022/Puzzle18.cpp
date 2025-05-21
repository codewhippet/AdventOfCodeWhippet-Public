#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle18_2022_Types
{
	struct Volume
	{
		Volume(const Vector3& dimensions)
			: Dimensions(dimensions)
		{
			Data.resize(Dimensions.X * Dimensions.Y * Dimensions.Z);
		}

		const uint8_t &operator[](const Vector3 &v) const
		{
			return Data[(Dimensions.X * Dimensions.Y * v.Z) + (Dimensions.X * v.Y) + (v.X)];
		}

		uint8_t &operator[](const Vector3 &v)
		{
			return Data[(Dimensions.X * Dimensions.Y * v.Z) + (Dimensions.X * v.Y) + (v.X)];
		}

		Vector3 Dimensions;
		vector<uint8_t> Data;
	};

	struct Cube : ICustomIterator<Vector3>
	{
		Vector3 Begin;
		Vector3 End;

		Cube(const Vector3 &begin, const Vector3 &end)
			: Begin(begin)
			, End(end)
		{
		}

		virtual NextValueResult Ctor(Vector3 *v) override
		{
			*v = Begin;
			return CheckForEnd(v);
		}

		virtual NextValueResult Next(Vector3 *v) override
		{
			assert((*v != Vector3{ Begin.X, Begin.Y, End.Z }));
			if (++v->X == End.X)
			{
				v->X = Begin.X;
				if (++v->Y == End.Y)
				{
					v->Y = Begin.Y;
					v->Z++;
				}
			}

			return CheckForEnd(v);
		}

		NextValueResult CheckForEnd(const Vector3 *v) const
		{
			return *v == Vector3{ Begin.X, Begin.Y, End.Z } ? NextValueResult::NoMoreValues : NextValueResult::MoreValuesAvailable;
		}

		bool operator==(const Cube&) const = default;
	};
}

using namespace Puzzle18_2022_Types;

static Volume ReadVolume(istream &input)
{
	// We pad the cube with an extra two layers around everything:
	// * Anything over 1 is required for flood fill to be guaranteed to work
	// * At least 2 are required for the exposed face calculation to work
	const Vector3 padding{ 2, 2, 2 };

	vector<Vector3> cubes = ScanfEachLine<int64_t, int64_t, int64_t>(input, "%lld,%lld,%lld")
		| views::transform([&](const auto &line) { return Vector3{ get<0>(line), get<1>(line), get<2>(line) } + padding; })
		| ranges::to<vector>();

	Vector3 minCube = ranges::fold_left(cubes,
		Vector3{ numeric_limits<int64_t>::max(), numeric_limits<int64_t>::max(), numeric_limits<int64_t>::max() },
		[](const auto &a, const auto &b) { return Vector3::MinElements(a, b); });
	assert(minCube.X > 0);
	assert(minCube.Y > 0);
	assert(minCube.Z > 0);
	(void)minCube;

	Vector3 maxCube = ranges::fold_left(cubes,
		Vector3{ numeric_limits<int64_t>::min(), numeric_limits<int64_t>::min(), numeric_limits<int64_t>::min() },
		[](const auto &a, const auto &b) { return Vector3::MaxElements(a, b); });

	Volume volume(maxCube + padding + Vector3{ 1, 1, 1 });
	ranges::for_each(cubes, [&](const auto &v) { volume[v] = 1; });
	return volume;
}

static int64_t CountExposedFaces(const Volume& volume, int cubeType)
{
	int64_t emptyNeighbours = 0;
	for (const Vector3& v : CustomRange<Cube>(Vector3{ 1, 1, 1 }, volume.Dimensions - Vector3{ 1, 1, 1 }))
	{
		if (volume[v] == cubeType)
		{
			emptyNeighbours += ranges::count_if(Vector3::CardinalDirections() | views::transform([&](const auto& dir) { return v + dir; }),
				[&](const auto& cube)
				{
					return volume[cube] != cubeType;
				});
		}
	}
	return emptyNeighbours;
}

static void Puzzle18_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Volume volume = ReadVolume(input);
	int64_t answer = CountExposedFaces(volume, 1);

	printf("[2022] Puzzle18_A: %" PRId64 "\n", answer);
}

static void Puzzle18_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	Volume volume = ReadVolume(input);

	// Flood fill around the droplet
	vector<Vector3> floodFill{ {} };
	for (size_t i = 0; i < floodFill.size(); i++)
	{
		Vector3 cube = floodFill[i];

		// Ignore already visited
		if (volume[cube] != 0)
			continue;

		// Mark as visited
		volume[cube] = 2;

		for (const Vector3& dir : Vector3::CardinalDirections())
		{
			Vector3 n = cube + dir;

			// Don't go outside the volume
			if ((n.X < 0) || (n.Y < 0) || (n.Z < 0) || (n.X >= volume.Dimensions.X) || (n.Y >= volume.Dimensions.Y) || (n.Z >= volume.Dimensions.Z))
				continue;

			// Don't overlap lava and ignore already visited
			if (volume[n] == 0)
			{
				floodFill.push_back(n);
			}
		}
	}

	int64_t answer = CountExposedFaces(volume, 2);

	printf("[2022] Puzzle18_B: %" PRId64 "\n", answer);
}

void Puzzle18_2022(const string& filename)
{
	Puzzle18_A(filename);
	Puzzle18_B(filename);
}
