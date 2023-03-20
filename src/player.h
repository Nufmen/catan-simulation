#ifndef CATAN_SIMULATION_PLAYER_H
#define CATAN_SIMULATION_PLAYER_H

#include <random>
#include <vector>

#include "board.h"
#include "bank.h"
#include "boardcell.h"
#include "components.h"
#include "game_state.h"
using namespace std;

class Player {
    vector<int> resources;
    vector<Development_Card> dev_cards;
    int victory_points, roads, settlements, cities;
    Game_State* game_state;
    default_random_engine generator;
    uniform_int_distribution<> dice;
    int owner;

    public:
    
    Player(int owner, int num_resources, int num_roads, int num_settlements, int num_cities, Game_State* game_state);

    void call_player_action();

    int roll_dice();

    void end_turn();

    void send_trade_offer(vector<Resource> offer, vector<Resource> request);

    void accept_trade_offer();

    void play_dev_card(Development_Card dev_card);

    void buy_dev_card();

    void place_road(BoardCell* cell1, BoardCell* cell2);

    void place_settlement(int row, int col);

    void place_city(int row, int col);

};

#endif