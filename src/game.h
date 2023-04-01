#ifndef CATAN_SIMULATION_GAME_H
#define CATAN_SIMULATION_GAME_H

#include "board.h"
#include "player.h"
#include "bank.h"
#include "game_state.h"

class Game {
    string data_config_file, board_config_file;
    Bank* bank;
    Board* board;
    vector<Player*> players;
    vector<int> dev_card_count;
    default_random_engine generator;
    uniform_int_distribution<> dice;
    struct Board_Config board_config;

    string remove_r(string s);

    vector<int> string_list_to_vector(string list);

    public:
    Game(string data_config_file, string board_config_file, vector<Player*> players);

    void load_data_config();

    void initialise_game();

    void start_game();

    void trading();

    void roll_dice();

    void buy_game_piece();

    void place_game_piece();

    void use_dev_card();

    void move_robber();

    void year_of_plenty();

    void monopoly();

    int get_num_players() {return board_config.num_players;}

};

#endif