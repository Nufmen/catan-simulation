#include "boardcell.h"

BoardCell::BoardCell(int row_, int col_, int id_, string type_) {
    row = row_;
    col = col_;
    id = id_;
    type = type_;

    owner = 0;
    building = Undeveloped;
    harbor = NoHarbor;
}

int BoardCell::getRow() {return row;}

int BoardCell::getCol() {return col;}

int BoardCell::getId() {return id;}

string BoardCell::getType() {return type;}

int BoardCell::getOwner() {return owner;}

Harbor BoardCell::getHarbor() {return harbor;}

Building BoardCell::getBuilding() {return building;}

vector<int> BoardCell::getAdjacent() {return adjacent;}

void BoardCell::setOwner(int owner_) {owner = owner_;}

void BoardCell::setHarbor(Harbor harbor_) {harbor = harbor_;}

void BoardCell::setBuilding(Building building_) {building = building_;}

void BoardCell::setAdjacent(vector<int> adjacent_) {adjacent = adjacent_;}