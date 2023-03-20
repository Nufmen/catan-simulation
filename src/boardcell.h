#ifndef CATAN_SIMULATION_BOARDCELL_H
#define CATAN_SIMULATION_BOARDCELL_H

#include <string>
#include <vector>

#include "components.h"

using namespace std;

class BoardCell {
    int row, col, id, owner;
    string type;
    Building building;
    Harbor harbor;
    vector<BoardCell *> adjacent;

public:
    BoardCell(int row_, int col_, int id_, string type_);

    int getRow();

    int getCol();

    int getId();

    string getType();

    int getOwner();

    Harbor getHarbor();

    Building getBuilding();

    vector<BoardCell *> getAdjacent();


    void setOwner(int owner_);

    void setHarbor(Harbor harbor_);

    void setBuilding(Building building_);

    void setAdjacent(vector<BoardCell *> adjacent_);
};

#endif