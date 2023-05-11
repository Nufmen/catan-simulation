#ifndef CATAN_SIMULATION_BOARD_H
#define CATAN_SIMULATION_BOARD_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <vector>

#include "boardcell.h"
#include "components.h"
#include "road.h"
#include "tile.h"

using namespace std;

class Board {
    struct Board_Config board_config;
    vector<vector<BoardCell *>> board;
    vector<unordered_set<int>> owned_cells;
    vector<unordered_set<int>> owned_road_cells;
    vector<int> num_settlements, num_roads;
    vector<Tile *> tiles;
    vector<Road *> roads;
    default_random_engine generator;

    string remove_r(string s);

    vector<int> string_list_to_vector(string list);

    bool road_match(Road road, BoardCell* cell1, BoardCell* cell2); 

    int road_length(bool first,int owner, BoardCell* cell, vector<int> *road_input);

    void load_board_config();

    void calc_adjacencies();

    void generate_ports();

    void generate_tiles();

    void generate_roads();

public:

    Board(Board_Config board_config, default_random_engine generator);
    
    vector<int> calc_road_length();

    void add_road(int road_id, int owner);

    void add_settlement(int row, int col, int owner);

    void upgrade_settlement(int row, int col);

    void move_robber(int tile);

    vector<int> robber_options();

    vector<int> road_options(int owner, bool start);

    vector<int> settlement_options(int owner, bool start);

    vector<int> city_options(int owner);

    bool is_too_close(int row, int col);

    vector<vector<vector<int>>> get_board();

    string get_board_state();

    vector<vector<int>> get_resources(int number);

    BoardCell* get_boardcell(int row, int col) {return board[row][col];}

    tuple<int,int> get_boardcell_coords(int id) {return make_tuple(floor(id/board_config.num_cols),id%board_config.num_cols);}

    vector<int> get_roads();

    Road* get_road(int index) {return roads[index];}

    int get_road_index(BoardCell* cell1, BoardCell* cell2);

    vector<int> get_tile_index(int number);

    Tile* get_tile(int id) {return tiles[id];}

    string get_tile_state();

    vector<vector<int>> get_tiles();

    string get_board_config_file() {return board_config.board_config_file;}

    int get_num_rows() {return board_config.num_rows;}

    int get_num_cols() {return board_config.num_cols;}

    int get_num_tiles() {return board_config.num_tiles;}

    int get_max_players() {return board_config.max_players;}

    string get_ocean_spot() {return board_config.ocean_spot;}

    string get_port_spot() {return board_config.port_spot;}

    string get_building_spot() {return board_config.building_spot;}

    vector<int> get_tile_order() {return board_config.tile_order;}

    vector<int> get_tile_count() {return board_config.tile_count;}

    vector<int> get_tile_numbers() {return board_config.tile_numbers;}

    vector<int> get_num_settlements() {return num_settlements;}

    vector<int> get_num_roads() {return num_roads;}
};

#endif