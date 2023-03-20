#include "tile.h"

Tile::Tile(int row_, int col_, int id_, Terrain terrain_, vector<BoardCell *> adjacencies_) {
    row = row_;
    col = col_;
    id = id_;
    number = 0;
    terrain = terrain_;
    adjacencies = adjacencies_;
}

bool Tile::hasRobber() {return robber;}

int Tile::getRow() {return row;}

int Tile::getCol() {return col;}

int Tile::getId() {return id;}

int Tile::getNumber() {return number;}

Terrain Tile::getTerrain() {return terrain;}

vector<BoardCell *> Tile::getAdjacencies() {return adjacencies;}

void Tile::setRobber(bool robber_) {robber = robber_;}

void Tile::setNumber(int number_) {number = number_;}
