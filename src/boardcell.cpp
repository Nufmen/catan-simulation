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