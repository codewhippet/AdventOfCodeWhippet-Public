#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle22_2021_Types
{
	static int spawnCount = 0;

	struct Cube
	{
		bool On = false;
		Vector3 Min;
		Vector3 Max;
	};

	struct CoordinateMapping
	{
		map<int64_t, int64_t> ActualToVirtual;
		map<int64_t, int64_t> VirtualToActual;
	};

	struct LightVolume
	{
		LightVolume(int64_t x, int64_t y, int64_t z)
		{
			SizeX = x;
			SizeY = y;
			SizeZ = z;

			DataSize = SizeX * SizeY * SizeZ;
			Data = new bool[DataSize];
			memset(Data, 0, DataSize * sizeof(Data[0]));
		}

		bool& operator()(int64_t x, int64_t y, int64_t z)
		{
			int64_t xOffset = x;
			int64_t yOffset = y * SizeX;
			int64_t zOffset = z * SizeX * SizeY;
			return Data[xOffset + yOffset + zOffset];
		}

	private:
		int64_t SizeX, SizeY, SizeZ;
		int64_t DataSize;
		bool* Data;
	};
}

using namespace Puzzle22_2021_Types;

static vector<Cube> ReadCubes(istream& input)
{
	vector<Cube> ret;

	vector<string> lines = ReadAllLines(input);

	basic_regex format(R"((\w+) x=(-?\d+)..(-?\d+),y=(-?\d+)..(-?\d+),z=(-?\d+)..(-?\d+))");
	for (const string& line : lines)
	{
		smatch match;
		bool matched = regex_match(line, match, format);
		assert(matched);
		(void)matched;

		Cube c;
		c.On = match[1].str() == "on";

		c.Min.X = stoll(match[2].str());
		c.Max.X = stoll(match[3].str());

		c.Min.Y = stoll(match[4].str());
		c.Max.Y = stoll(match[5].str());

		c.Min.Z = stoll(match[6].str());
		c.Max.Z = stoll(match[7].str());

		ret.push_back(c);
	}

	return ret;
}

static bool Intersect(const Cube& a, const Cube& b, Cube* clipped)
{
	clipped->Min.X = max(a.Min.X, b.Min.X);
	clipped->Min.Y = max(a.Min.Y, b.Min.Y);
	clipped->Min.Z = max(a.Min.Z, b.Min.Z);

	clipped->Max.X = min(a.Max.X, b.Max.X);
	clipped->Max.Y = min(a.Max.Y, b.Max.Y);
	clipped->Max.Z = min(a.Max.Z, b.Max.Z);

	return
		clipped->Min.X <= clipped->Max.X &&
		clipped->Min.Y <= clipped->Max.Y &&
		clipped->Min.Z <= clipped->Max.Z;
}

static void Puzzle22_A(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	const Vector3 coordinateOffset{ 200000, 200000, 200000 };
	vector<Cube> cubes = ReadCubes(input);

	cubes = MakeEnumerator(cubes)
		->Select<Cube>([&](const Cube& c)
			{
				Cube offset = c;
				offset.Min = c.Min + coordinateOffset;
				assert(offset.Min.X > -1);
				assert(offset.Min.Y > -1);
				assert(offset.Min.Z > -1);
				offset.Max = c.Max + coordinateOffset;
				return offset;
			})
		->ToVector();

	Cube region;
	region.Min = Vector3{ -50, -50, -50 } + coordinateOffset;
	region.Max = Vector3{ 50, 50, 50 } + coordinateOffset;

	set<Vector3> on;
	for (const Cube& c : cubes)
	{
		Cube clipped;
		if (Intersect(region, c, &clipped))
		{
			for (int64_t x = clipped.Min.X; x <= clipped.Max.X; x++)
			{
				for (int64_t y = clipped.Min.Y; y <= clipped.Max.Y; y++)
				{
					for (int64_t z = clipped.Min.Z; z <= clipped.Max.Z; z++)
					{
						if (c.On)
						{
							on.insert(Vector3{ x, y, z });
						}
						else
						{
							on.erase(Vector3{ x, y, z });
						}
					}
				}
			}
		}
	}

	int64_t answer = (int64_t)on.size();

	printf("[2021] Puzzle22_A: %" PRId64 "\n", answer);
}

static void Puzzle22_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	vector<Cube> cubes = ReadCubes(input);

	auto getAxisValues = [](const vector<Cube>& cubes, const function<pair<int64_t, int64_t>(const Cube&)>& get) -> vector<int64_t>
		{
			set<int64_t> axis;
			for (const Cube& c : cubes)
			{
				pair<int64_t, int64_t> axisValues = get(c);
				axis.insert(axisValues.first);
				axis.insert(axisValues.second);
				axis.insert(axisValues.second + 1);
			}
			return MakeEnumerator(axis)->ToVector();
		};

	vector<int64_t> sortedXValues = getAxisValues(cubes, [](const Cube& c) { return make_pair(c.Min.X, c.Max.X);  });
	vector<int64_t> sortedYValues = getAxisValues(cubes, [](const Cube& c) { return make_pair(c.Min.Y, c.Max.Y);  });
	vector<int64_t> sortedZValues = getAxisValues(cubes, [](const Cube& c) { return make_pair(c.Min.Z, c.Max.Z);  });

	auto generateMapping = [](const vector<int64_t>& sortedValues) -> CoordinateMapping
		{
			CoordinateMapping axisMapping;
			for (int64_t i = 0; i < (int64_t)sortedValues.size(); i++)
			{
				axisMapping.ActualToVirtual[sortedValues[i]] = i;
				axisMapping.VirtualToActual[i] = sortedValues[i];
			}
			return axisMapping;
		};

	CoordinateMapping xAxisMapping = generateMapping(sortedXValues);
	CoordinateMapping yAxisMapping = generateMapping(sortedYValues);
	CoordinateMapping zAxisMapping = generateMapping(sortedZValues);

	vector<Cube> virtualCubes = MakeEnumerator(cubes)
		->Select<Cube>([&xAxisMapping, &yAxisMapping, &zAxisMapping](const Cube& c)
			{
				Cube v;

				v.On = c.On;

				v.Min.X = xAxisMapping.ActualToVirtual.at(c.Min.X);
				v.Min.Y = yAxisMapping.ActualToVirtual.at(c.Min.Y);
				v.Min.Z = zAxisMapping.ActualToVirtual.at(c.Min.Z);

				v.Max.X = xAxisMapping.ActualToVirtual.at(c.Max.X);
				v.Max.Y = yAxisMapping.ActualToVirtual.at(c.Max.Y);
				v.Max.Z = zAxisMapping.ActualToVirtual.at(c.Max.Z);

				return v;
			})
		->ToVector();

	LightVolume turnedOn(sortedXValues.size() + 1, sortedYValues.size() + 1, sortedZValues.size() + 1);
	for (const Cube& cube : virtualCubes)
	{
		for (int64_t x = cube.Min.X; x <= cube.Max.X; x++)
		{
			for (int64_t y = cube.Min.Y; y <= cube.Max.Y; y++)
			{
				for (int64_t z = cube.Min.Z; z <= cube.Max.Z; z++)
				{
					turnedOn(x, y, z) = cube.On;
				}
			}
		}
	}

	int64_t sum = 0;
	for (int64_t x = 0; x + 1 < (int64_t)sortedXValues.size(); x++)
	{
		for (int64_t y = 0; y + 1 < (int64_t)sortedYValues.size(); y++)
		{
			for (int64_t z = 0; z + 1 < (int64_t)sortedZValues.size(); z++)
			{
				if (turnedOn(x, y, z))
				{
					Vector3 min = Vector3{ sortedXValues[x + 0], sortedYValues[y + 0], sortedZValues[z + 0] };
					Vector3 max = Vector3{ sortedXValues[x + 1], sortedYValues[y + 1], sortedZValues[z + 1] };

					Vector3 diff = max - min;
					int64_t volume = diff.X * diff.Y * diff.Z;
					sum += volume;
				}
			}
		}
	}

	int64_t answer = sum;

	printf("[2021] Puzzle22_B: %" PRId64 "\n", answer);
}

void Puzzle22_2021(const string& filename)
{
	Puzzle22_A(filename);
	Puzzle22_B(filename);
}
