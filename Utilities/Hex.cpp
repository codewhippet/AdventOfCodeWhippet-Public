#include "stdafx.h"
#include "Hex.h"

const std::vector<HexFlat::Hex>& HexFlat::Hex::NeighbourDirections()
{
    static const std::vector<HexFlat::Hex> directions =
    {
        HexFlat::Hex::North(),
        HexFlat::Hex::NorthEast(),
        HexFlat::Hex::SouthEast(),
        HexFlat::Hex::South(),
        HexFlat::Hex::SouthWest(),
        HexFlat::Hex::NorthWest()
    };
    return directions;
}

const std::vector<HexPointy::Hex>& HexPointy::Hex::NeighbourDirections()
{
    static const std::vector<HexPointy::Hex> directions =
    {
        HexPointy::Hex::NorthEast(),
        HexPointy::Hex::East(),
        HexPointy::Hex::SouthEast(),
        HexPointy::Hex::SouthWest(),
        HexPointy::Hex::West(),
        HexPointy::Hex::NorthWest()
    };
    return directions;
}
