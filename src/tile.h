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

    bool has_robber() {return robber;}

    int get_row() {return row;}

    int get_col() {return col;}

    int get_id() {return id;}

    int get_number() {return number;}

    Terrain get_terrain() {return terrain;}

    vector<BoardCell *> get_adjacencies() {return adjacencies;}

    vector<int> get_adjacent_owners();

    void set_robber(bool robber_) {robber = robber_;}

    void set_number(int number_) {number = number_;}
};
#endif