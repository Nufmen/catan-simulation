#ifndef CATAN_SIMULATION_PLAYER_D_H
#define CATAN_SIMULATION_PLAYER_D_H

#include <random>
#include <vector>

#include "board.h"
#include "bank.h"
#include "boardcell.h"
#include "components.h"
#include "game_state.h"
#include "player.h"
using namespace std;

class Default_Player: public Player {
    int state;

    public:
    Default_Player(int owner, int num_resources, int num_roads, int num_settlements, int num_cities):
    Player(owner, num_resources, num_roads, num_settlements, num_cities){
        state = 1;
    }

    Player_Move call_player_action() override;

    int place_road(vector<int> options) override;

    int place_settlement(vector<int> options) override;
    
    int place_city(vector<int> options) override;

    Development_Card use_dev_card() override;

    Trade trade_offer() override;

    Trade trade_bank() override;

    bool accept_trade(Trade trade) override;

    int select_trade(vector<int> players) override;

    int move_robber(vector<int> options) override;

    int steal_resource(vector<int> players) override;

    vector<Resource> discard_hand() override;

    vector<Resource> year_of_plenty() override;

    Resource monopoly() override;

};

#endif