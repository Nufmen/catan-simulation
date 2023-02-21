#include "road.h"

    Road::Road(int cell1_, int cell2_) {
        cell1 = cell1_;
        cell2 = cell2_;
        owner = 0;
    }

    int Road::getCell(int c) {
        switch(c) {
            case 1:
                return cell1;
            case 2:
                return cell2;
            default:
                return -1;
        }
    }

    int Road::getOwner() {return owner;}

    void Road::setOwner(int owner_) {owner = owner_;}
