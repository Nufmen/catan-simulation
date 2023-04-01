#ifndef CATAN_SIMULATION_PLAYER_H
#define CATAN_SIMULATION_PLAYER_H

#include <random>
#include <vector>

#include "board.h"
#include "bank.h"
#include "boardcell.h"
#include "components.h"
using namespace std;

class Player {
    vector<int> resources, trade_rate;
    vector<Development_Card> dev_cards;
    vector<Road_Data> road_order;
    default_random_engine generator;
    int victory_points, roads, settlements, cities;
    int owner, state;
    bool rolled_dice;

    public:
    
    Player(int owner, int num_resources, int num_roads, int num_settlements, int num_cities, default_random_engine generator);

    Player_Move call_player_action();

    bool is_valid_action(Player_Move move);

    Road_Data place_road();

    Coordinate_Data place_settlement();
    
    Coordinate_Data place_city();

    Development_Card use_dev_card();

    Trade trade_offer();

    Trade trade_bank();

    Trade discard_hand();

    bool accept_trade(Trade trade);

    Coordinate_Data move_robber();

    int steal_resource(vector<int> players);

    void set_state(int state) {state = state;}

    void set_rolled_dice(bool dice) {rolled_dice = dice;}

    void add_dev_card(Development_Card card) {dev_cards.push_back(card);}

    void add_resources(vector<Resource> resources);

    void remove_resources(vector<Resource> resources);
};

#endif