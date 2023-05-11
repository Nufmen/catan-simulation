#ifndef CATAN_SIMULATION_ROAD_H
#define CATAN_SIMULATION_ROAD_H

#include "boardcell.h"

using namespace std;

class Road {
    vector<BoardCell *> cells;
    int owner;

public:
    Road(BoardCell* cell1, BoardCell* cell2);

    vector<BoardCell *> get_cells() {return cells;}

    int get_owner() {return owner;}

    void set_owner(int owner_) {owner = owner_;}
};


#endif