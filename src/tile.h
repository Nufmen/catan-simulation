#ifndef CATAN_SIMULATION_TILE_H
#define CATAN_SIMULATION_TILE_H

#include <vector>

#include "components.h"

using namespace std;

class Tile {
    int row, col, id, number;
    Terrain terrain;
    vector<int> adjacencies;
    bool robber = false;

    public:

    Tile(int row_, int col_, int id_, Terrain terrain_, vector<int> adjacencies_);

    bool hasRobber();

    int getRow();

    int getCol();

    int getId();

    int getNumber();

    Terrain getTerrain();

    vector<int> getAdjacencies();

    void setNumber(int number_);

    void setRobber(bool robber_);
};
#endif