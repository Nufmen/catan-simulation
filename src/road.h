#ifndef CATAN_SIMULATION_ROAD_H
#define CATAN_SIMULATION_ROAD_H

#include "boardcell.h"

using namespace std;

class Road {
    vector<BoardCell *> cells;
    int owner;

public:
    Road(BoardCell* cell1_, BoardCell* cell2_);

    vector<BoardCell *> getCells();

    int getOwner();

    void setOwner(int owner_);
};


#endif