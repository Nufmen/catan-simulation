#ifndef CATAN_SIMULATION_GAME_STATE_H
#define CATAN_SIMULATION_GAME_STATE_H

#include "board.h"
#include "bank.h"
#include "components.h"

class Bank;

struct Player_State {
    int victory_points;
    int num_cards;
    int dev_cards;
    int knights;
    int road_length;
};

struct Game_State {
    vector<vector<vector<int>>> board;
    vector<Player_State*> players;
};

#endif