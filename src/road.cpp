#include "road.h"

    Road::Road(BoardCell* cell1_, BoardCell* cell2_) {
        cells.push_back(cell1_);
        cells.push_back(cell2_);
        owner = 0;
    }
