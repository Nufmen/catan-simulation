#include "tile.h"

Tile::Tile(int row_, int col_, int id_, Terrain terrain_, vector<BoardCell *> adjacencies_) {
    row = row_;
    col = col_;
    id = id_;
    number = 0;
    terrain = terrain_;
    adjacencies = adjacencies_;
}

vector<int> Tile::get_adjacent_owners() {
    vector<int> owners;
    for(BoardCell* c: adjacencies){
        if(c->get_owner() == 0)
            continue;
        owners.push_back(c->get_owner());
    }
    return owners;
}
