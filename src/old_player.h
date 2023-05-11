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
    vector<int> resources, trade_rate;
    vector<Harbor> harbors;
    vector<Development_Card> dev_cards;
    vector<Road_Data> road_order;
    Player_State* player_state;
    default_random_engine generator;
    int victory_points, roads, settlements, cities;
    int owner, state;
    bool rolled_dice, played_dev_card;

    public:
    
    Player(int owner, int num_resources, int num_roads, int num_settlements, int num_cities);

    Player_Move call_player_action();

    bool is_valid_action(Player_Move move);

    int place_road(vector<int> options);

    int place_settlement(vector<int> options);
    
    int place_city(vector<int> options);

    Development_Card use_dev_card();

    Trade trade_offer();

    Trade trade_bank();

    vector<Resource> discard_hand();

    bool accept_trade(Trade trade);

    int select_trade(vector<int> players);

    int move_robber(vector<int> options);

    int steal_resource(vector<int> players);

    vector<Resource> year_of_plenty();

    Resource monopoly();

    bool has_resources(vector<Resource> resources);

    Player_State* get_player_state() {return player_state;};

    int get_resource_count(Resource resource) {return resources[(int)resource];}

    int get_owner() {return owner;}

    int get_victory_points() {return victory_points;}

    void set_state(int state) {state = state;}

    void set_rolled_dice(bool rolled) {rolled_dice = rolled;}

    void set_trade_rate(Resource resource, int rate) {trade_rate[(int)resource] = rate;}

    void add_victory_points(int points) {victory_points+=points; player_state->victory_points+=points;}

    void remove_victory_points(int points) {victory_points-=points; player_state->victory_points-=points;}

    void add_dev_card(Development_Card card);

    vector<Development_Card> get_dev_card() {return dev_cards;}

    void remove_dev_card(Development_Card card);

    void add_resources(vector<Resource> resources);

    void add_harbor(Harbor harbor);

    void remove_resources(vector<Resource> resources);
};

#endif