#ifndef CATAN_SIMULATION_GAME_H
#define CATAN_SIMULATION_GAME_H

#include <fstream>

#include "board.h"
#include "player.h"
#include "bank.h"
#include "game_state.h"

class Game {
    string data_config_file, board_config_file;
    Bank* bank;
    Board* board;
    Game_State* game_state;
    vector<Player*> players;
    vector<int> dev_card_count;
    default_random_engine generator;
    uniform_int_distribution<> dice;
    struct Board_Config board_config;
    int NUM_RESOURCES = 5;
    int turn;
    int longest_road, largest_army, longest_road_owner, largest_army_owner;
    bool rolled_dice, played_dev_card;

    string remove_r(string s);

    vector<int> string_list_to_vector(string list);

    void load_data_config();

    void initialise_game();

    public:
    Game(string data_config_file, string board_config_file, vector<Player*> players);

    void simulation(string log_file);

    void start_game();

    void trading();

    void trade_bank();

    void roll_dice();

    void move_robber(bool seven);

    void place_game_piece(Player_Move move);

    void use_dev_card();

    void year_of_plenty(vector<Resource> resources);

    void monopoly(Resource resource);

    void place_road(int road_spot, int owner);

    Game_State* get_game_state() {return game_state;};

    int get_num_players() {return board_config.num_players;}

    Player* get_player(int id) {return players[id];}

    Board* get_board() {return board;}

};

#endif