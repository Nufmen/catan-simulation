#ifndef CATAN_SIMULATION_TILE_H
#define CATAN_SIMULATION_TILE_H

#include <vector>

#include "boardcell.h"
#include "components.h"

using namespace std;

class Tile {
    int row, col, id, number;
    Terrain terrain;
    vector<BoardCell *> adjacencies;
    bool robber = false;

    public:

    Tile(int row_, int col_, int id_, Terrain terrain_, vector<BoardCell *> adjacencies_);

    bool hasRobber();

    int getRow();

    int getCol();

    int getId();

    int getNumber();

    Terrain getTerrain();

    vector<BoardCell *> getAdjacencies();

    void setNumber(int number_);

    void setRobber(bool robber_);
};
#endif