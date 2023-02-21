#ifndef CATAN_SIMULATION_ROAD_H
#define CATAN_SIMULATION_ROAD_H

using namespace std;

class Road {
    int cell1, cell2, owner;

public:
    Road(int cell1_, int cell2_);

    int getCell(int c);

    int getOwner();

    void setOwner(int owner_);
};


#endif