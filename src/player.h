#ifndef CATAN_SIMULATION_PLAYER_H
#define CATAN_SIMULATION_PLAYER_H

#include <random>
#include <vector>

#include "board.h"
#include "bank.h"
#include "boardcell.h"
#include "components.h"
#include "game_state.h"

class Player {
    protected:
    vector<int> resources, trade_rate;
    vector<Harbor> harbors;
    vector<Development_Card> dev_cards;
    vector<Road_Data> road_order;
    Player_State* player_state;
    default_random_engine generator;
    int victory_points, roads, settlements, cities;
    int owner;
    bool rolled_dice, played_dev_card;
    
    public:
    Player(int owner, int num_resources, int num_roads, int num_settlements, int num_cities);

    // Virtual Functions
    virtual Player_Move call_player_action() = 0;

    virtual int place_road(vector<int> options) = 0;

    virtual int place_settlement(vector<int> options) = 0;
    
    virtual int place_city(vector<int> options) = 0;

    virtual Development_Card use_dev_card() = 0;

    virtual Trade trade_offer() = 0;

    virtual Trade trade_bank() = 0;

    virtual bool accept_trade(Trade trade) = 0;

    virtual int select_trade(vector<int> players) = 0;

    virtual int move_robber(vector<int> options) = 0;

    virtual int steal_resource(vector<int> players) = 0;

    virtual vector<Resource> discard_hand() = 0;

    virtual vector<Resource> year_of_plenty() = 0;

    virtual Resource monopoly() = 0;


    // Non-Virtual Functions
    bool is_valid_action(Player_Move move);

    bool has_resources(vector<Resource> resources);

    void add_dev_card(Development_Card card);

    void remove_dev_card(Development_Card card);

    void add_resources(vector<Resource> resources);

    void remove_resources(vector<Resource> resources);

    void add_harbor(Harbor harbor);


    // Get/Set Functions
    Player_State* get_player_state() {return player_state;};

    int get_resource_count(Resource resource) {return resources[(int)resource];}

    int get_num_resources();

    int get_owner() {return owner;}

    int get_victory_points() {return victory_points;}

    void set_state(int state) {state = state;}

    void set_rolled_dice(bool rolled) {rolled_dice = rolled;}

    void set_trade_rate(Resource resource, int rate) {trade_rate[(int)resource] = rate;}

    void add_victory_points(int points) {victory_points+=points; player_state->victory_points+=points;}

    void remove_victory_points(int points) {victory_points-=points; player_state->victory_points-=points;}

    int get_road_count() {return roads;}

    void remove_road() {roads--;}

    int get_settlement_count() {return settlements;}

    void add_settlement() {settlements++;}
    
    void remove_settlement() {settlements--;} 

    int get_city_count() {return cities;}

    void remove_city() {cities--;}

    vector<Development_Card> get_dev_card() {return dev_cards;}


};

#endif
