#include "stdafx.h"

using namespace std;

static string dummy =
R"()";

namespace Puzzle20_2020_Types
{
	enum class EdgeName
	{
		Top,
		Bottom,
		Left,
		Right
	};

	struct Tile
	{
		int64_t Id = -1;
		map<int, set<Point2>> Pixels;
		set<uint32_t> Edges;
		map<pair<int, EdgeName>, uint32_t> EdgesByTransformAndName;
	};

	struct TilePair
	{
		TilePair(int64_t a, int64_t b)
		{
			assert(a != b);
			TileA = min(a, b);
			TileA = max(a, b);
		}

		int64_t TileA;
		int64_t TileB;
	};

	bool operator<(const TilePair& a, const TilePair& b)
	{
		if (a.TileA != b.TileA) return a.TileA < b.TileA;
		return a.TileB < b.TileB;
	}
}

using namespace Puzzle20_2020_Types;

static void MakeFlippingRotations(Tile* tile)
{
	for (const Point2& p : tile->Pixels[0])
	{
		tile->Pixels[1].insert({ p.Y, p.X });
	}

	for (int i = 2; i < 8; i++)
	{
		for (const Point2& p : tile->Pixels[i - 2])
		{
			Point2 rotated{ 9 - p.Y, p.X };
			tile->Pixels[i].insert(rotated);
		}
	}
}

static map<int64_t, Tile> ReadTiles(istream& input)
{
	vector<string> lines = ReadAllLines(input);

	map<int64_t, Tile> tiles;
	for (size_t i = 0; i < lines.size(); i += 12)
	{
		Tile t;
		int scanned = sscanf(lines[i].c_str(), "Tile %" SCNd64 ":", &t.Id);
		assert(scanned == 1);
		(void)scanned;

		for (int64_t y = 0; y < 10; y++)
		{
			for (int64_t x = 0; x < 10; x++)
			{
				if (lines[i + y + 1][x] == '#')
				{
					t.Pixels[0].insert(Point2{ x, y });
				}
			}
		}

		MakeFlippingRotations(&t);

		tiles[t.Id] = move(t);
	}

	struct EdgeDescription
	{
		EdgeName Name;
		Point2 From;
		Point2 To;
	};

	const vector<EdgeDescription> edges = {
		{ EdgeName::Top, Point2{ 0, 0 }, Point2{ 9, 0 } },
		{ EdgeName::Bottom , Point2{ 0, 9 }, Point2{ 9, 9 } },
		{ EdgeName::Left , Point2{ 0, 0 }, Point2{ 0, 9 } },
		{ EdgeName::Right , Point2{ 9, 0 }, Point2{ 9, 9 } },
	};

	for (decltype(tiles)::reference t : tiles)
	{
		for (int transform = 0; transform < 8; transform++)
		{
			for (size_t edge = 0; edge < edges.size(); edge++)
			{
				uint32_t edgeValue = 0;
				for (Point2 p : Enumerable::Line(edges[edge].From, edges[edge].To)->ToVector())
				{
					edgeValue = edgeValue << 1;
					if (t.second.Pixels[transform].contains(p))
					{
						edgeValue |= 1;
					}
				}
				t.second.Edges.insert(edgeValue);

				t.second.EdgesByTransformAndName[{ transform, edges[edge].Name }] = edgeValue;
			}
		}
	}

	return tiles;
}

static void Puzzle20_A(const string &filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	map<int64_t, Tile> tiles = ReadTiles(input);

	map<uint32_t, set<int64_t>> edgesToTileIds;
	for (decltype(tiles)::const_reference t : tiles)
	{
		for (decltype(t.second.Edges)::const_reference edge: t.second.Edges)
		{
			edgesToTileIds[edge].insert(t.second.Id);
		}
	}

	map<int64_t, int> uniqueEdgesCount;
	for (map<uint32_t, set<int64_t>>::const_reference tilesWithEdge : edgesToTileIds)
	{
		if (tilesWithEdge.second.size() == 1)
		{
			uniqueEdgesCount[*tilesWithEdge.second.begin()]++;
		}
	}

	set<int64_t> corners;
	for (map<int64_t, int>::const_reference uniqueEdge : uniqueEdgesCount)
	{
		if (uniqueEdge.second == 4)
		{
			corners.insert(uniqueEdge.first);
		}
	}

	int64_t answer = accumulate(corners.begin(), corners.end(), 1ll, multiplies<int64_t>{});

	printf("[2020] Puzzle20_A: %" PRId64 "\n", answer);
}

static void Puzzle20_B(const string& filename)
{
	(void)filename;
	ifstream input(filename);
	//istringstream input(dummy);

	map<int64_t, Tile> tiles = ReadTiles(input);

	map<uint32_t, set<int64_t>> edgesToTileIds;
	for (decltype(tiles)::const_reference t : tiles)
	{
		for (decltype(t.second.Edges)::const_reference edge : t.second.Edges)
		{
			edgesToTileIds[edge].insert(t.second.Id);
		}
	}

	map<int64_t, set<int64_t>> tileAdjacencies;
	for (map<uint32_t, set<int64_t>>::const_reference tilesWithEdge : edgesToTileIds)
	{
		for (set<int64_t>::const_reference tileA : tilesWithEdge.second)
		{
			for (set<int64_t>::const_reference tileB : tilesWithEdge.second)
			{
				if (tileA != tileB)
				{
					tileAdjacencies[tileA].insert(tileB);
					tileAdjacencies[tileB].insert(tileA);
				}
			}
		}
	}

	int mapSizeInTiles = static_cast<int>(sqrt(tiles.size()));
	assert(mapSizeInTiles * mapSizeInTiles == tiles.size());

	map<Point2, int64_t> tilePositions;
	map<int64_t, set<int64_t>> candidateTiles = tileAdjacencies;
	for (int64_t y = 0; y < mapSizeInTiles; y++)
	{
		for (int64_t x = 0; x < mapSizeInTiles; x++)
		{
			Point2 current{ x, y };
			Point2 above = current - Point2{ 0, 1 };
			Point2 left = current - Point2{ 1, 0 };

			// Starting case, just pick a corner tile
			if ((x == 0) && (y == 0))
			{
				map<int64_t, set<int64_t>>::const_iterator cornerTile = find_if(
					candidateTiles.begin(),
					candidateTiles.end(),
					[](map<int64_t, set<int64_t>>::const_reference t)
					{
						return t.second.size() == 2;
					});
				tilePositions[current] = cornerTile->first;
				candidateTiles.erase(cornerTile);
				continue;
			}

			// Work out how many tiles we should be connected to
			size_t numNeighbours = 0;
			numNeighbours += (x == 0 ? 0 : 1);
			numNeighbours += (y == 0 ? 0 : 1);
			numNeighbours += (x == mapSizeInTiles - 1 ? 0 : 1);
			numNeighbours += (y == mapSizeInTiles - 1 ? 0 : 1);

			int64_t tileAbove = (tilePositions.contains(above) ? tilePositions[above] : -1);
			int64_t tileLeft = (tilePositions.contains(left) ? tilePositions[left] : -1);

			map<int64_t, set<int64_t>>::const_iterator tileToPick = find_if(
				candidateTiles.begin(),
				candidateTiles.end(),
				[numNeighbours, tileAbove, tileLeft](map<int64_t, set<int64_t>>::const_reference t)
				{
					if (t.second.size() != numNeighbours)
					{
						return false;
					}

					if ((tileAbove != -1) && (t.second.contains(tileAbove) == false))
					{
						return false;
					}

					if ((tileLeft != -1) && (t.second.contains(tileLeft) == false))
					{
						return false;
					}

					// Should be unique according to puzzle definition
					return true;
				});
			assert(tileToPick != candidateTiles.end());

			tilePositions[current] = tileToPick->first;
			candidateTiles.erase(tileToPick);
		}
	}

	const map<EdgeName, Point2> tileOffsets =
	{
		{ EdgeName::Top, Point2{ 0, -1 } },
		{ EdgeName::Bottom , Point2{ 0, 1 } },
		{ EdgeName::Left , Point2{ -1, 0 } },
		{ EdgeName::Right , Point2{ 1, 0 } },
	};

	map<Point2, int> tileTransforms;
	for (int64_t y = 0; y < mapSizeInTiles; y++)
	{
		for (int64_t x = 0; x < mapSizeInTiles; x++)
		{
			Point2 current{ x, y };

			const Tile& currentTile = tiles[tilePositions[current]];

			// Start with all potential rotations
			set<int> candidates = Enumerable::Range(0, 8)->ToSet();

			// Filter transforms based on edges
			for (int transform = 0; transform < 8; transform++)
			{
				for (EdgeName e : { EdgeName::Bottom, EdgeName::Left, EdgeName::Right, EdgeName::Top })
				{
					Point2 adjacentTilePosition = current + tileOffsets.at(e);
					if (tilePositions.contains(adjacentTilePosition) == false)
					{
						continue;
					}

					const Tile& adjacentTile = tiles[tilePositions[adjacentTilePosition]];
					uint32_t joiningEdge = currentTile.EdgesByTransformAndName.at({ transform, e });
					if (adjacentTile.Edges.contains(joiningEdge) == false)
					{
						candidates.erase(transform);
					}
				}
			}

			assert(candidates.size() == 1);
			tileTransforms[current] = *candidates.begin();
		}
	}

	// Strip off the borders
	{
		const vector<pair<Point2, Point2>> edges = {
			{ Point2{ 0, 0 }, Point2{ 9, 0 } },
			{ Point2{ 0, 9 }, Point2{ 9, 9 } },
			{ Point2{ 0, 0 }, Point2{ 0, 9 } },
			{ Point2{ 9, 0 }, Point2{ 9, 9 } },
		};

		for (decltype(tiles)::reference tile : tiles)
		{
			for (int transform = 0; transform < 8; transform++)
			{
				for (size_t edge = 0; edge < edges.size(); edge++)
				{
					for (Point2 p : Enumerable::Line(edges[edge].first, edges[edge].second)->ToVector())
					{
						tile.second.Pixels[transform].erase(p);
					}
				}

				tile.second.Pixels[transform] = MakeEnumerator(tile.second.Pixels[transform])
					->Select<Point2>([](const Point2& p) { return p - Point2{ 1, 1 }; })
					->ToSet();
			}
		}
	}

	set<Point2> image;
	for (int64_t tileY = 0; tileY < mapSizeInTiles; tileY++)
	{
		for (int64_t tileX = 0; tileX < mapSizeInTiles; tileX++)
		{
			Point2 tilePosition{ tileX, tileY };
			Point2 tileOffset = (Point2{ tileX, tileY } * 8);

			const Tile& tile = tiles[tilePositions[tilePosition]];
			int tileTransform = tileTransforms[tilePosition];
			const set<Point2>& tilePixels = tile.Pixels.at(tileTransform);

			for (const Point2& p : tilePixels)
			{
				image.insert(p + tileOffset);
			}
		}
	}

	//
	// Make image rotations
	//

	int imageSize = mapSizeInTiles * 8;

	map<int, set<Point2>> images;
	images[0] = image;

	for (const Point2& p : images[0])
	{
		images[1].insert({ p.Y, p.X });
	}

	for (int i = 2; i < 8; i++)
	{
		for (const Point2& p : images[i - 2])
		{
			Point2 rotated{ imageSize - p.Y, p.X };
			images[i].insert(rotated);
		}
	}

	//
	// Search for monsters!
	//

	const set<Point2> seaMonster =
	{
		Point2{ 18, 0 },

		Point2{ 0, 1 },

		Point2{ 5, 1 },
		Point2{ 6, 1 },

		Point2{ 11, 1 },
		Point2{ 12, 1 },

		Point2{ 17, 1 },
		Point2{ 18, 1 },
		Point2{ 19, 1 },

		Point2{ 1, 2 },
		Point2{ 4, 2 },
		Point2{ 7, 2 },
		Point2{ 10, 2 },
		Point2{ 13, 2 },
		Point2{ 16, 2 },
	};

	set<pair<int, Point2>> seaMonsterLocations;
	map<int, int> seaMonsterCounts;
	for (int imageIndex = 0; imageIndex < 8; imageIndex++)
	{
		for (int64_t y = 0; y + 2 < imageSize; y++)
		{
			for (int64_t x = 0; x + 19 < imageSize; x++)
			{
				Point2 offset{ x, y };
				size_t matchingPixels = count_if(seaMonster.begin(), seaMonster.end(),
					[&images, imageIndex, offset](const Point2& p)
					{
						return images[imageIndex].contains(p + offset);
					});
				if (matchingPixels == seaMonster.size())
				{
					seaMonsterCounts[imageIndex]++;
					seaMonsterLocations.insert({ imageIndex, offset });
				}
			}
		}
	}

	assert(seaMonsterCounts.size() == 1);
	int correctImage = seaMonsterCounts.begin()->first;

	for (decltype(seaMonsterLocations)::const_reference seaMonsterInstance : seaMonsterLocations)
	{
		for (const Point2& p : seaMonster)
		{
			images[correctImage].erase(p + seaMonsterInstance.second);
		}
	}

	size_t answer = images[correctImage].size();

	printf("[2020] Puzzle20_B: %" PRIu64 "\n", answer);
}

void Puzzle20_2020(const string& filename)
{
	Puzzle20_A(filename);
	Puzzle20_B(filename);
}
