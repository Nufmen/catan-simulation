#ifndef CATAN_SIMULATION_GAME_H
#define CATAN_SIMULATION_GAME_H

#include "board.h"
#include "player.h"
#include "bank.h"
#include "game_state.h"

class Game {
    int num_rows, num_cols, num_tiles, max_players, robber_tile, num_players;
    string data_config_file, board_config_file, building_spot, ocean_spot, port_spot;
    vector<int> tile_order, tile_count, tile_numbers;
    struct Game_State* game_state;
    struct Board_Config board_config;

    string remove_r(string s);

    vector<int> string_list_to_vector(string list);

    public:
    Game(string data_config_file, string board_config_file);

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

    struct Game_State* get_game_state();

    int get_num_players();

};

#endif