#ifndef CATAN_SIMULATION_BOARDCELL_H
#define CATAN_SIMULATION_BOARDCELL_H

#include <string>
#include <vector>

#include "components.h"

using namespace std;

class BoardCell {
    int row, col, id, owner, settlement;
    string type;
    Building building;
    Harbor harbor;
    vector<BoardCell *> adjacent;
    bool start = false;

public:
    BoardCell(int row, int col, int id, string type);

    int get_row() {return row;}

    int get_col() {return col;}

    int get_id() {return id;}

    string get_type() {return type;}

    int get_owner() {return owner;}

    Harbor get_harbor() {return harbor;}

    Building get_building() {return building;}

    vector<BoardCell *> get_adjacent() {return adjacent;}

    int get_settlement() {return settlement;}

    void set_owner(int owner_) {owner = owner_;}

    void set_harbor(Harbor harbor_) {harbor = harbor_;}

    void set_building(Building building_) {building = building_;}

    void set_adjacent(vector<BoardCell *> adjacent_) {adjacent = adjacent_;}

    void set_settlement(int settlement_num) {settlement = settlement_num;}

    void set_start(bool start_) {start = start_;}
};

#endif