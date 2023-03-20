#include "game.h"
#include <string>

using namespace std;

string Game::remove_r(string s) {
    if (s[s.size()-1] == '\r')
        s.erase(s.size()-1);
    return s;
}

vector<int> Game::string_list_to_vector(string list) {
    vector<int> vector_out;
    string delimiter = ",";
    string token;
    size_t pos = 0;
    list = remove_r(list);
    if (list[list.size()-1] == ']')
        list.erase(list.size()-1);
    if (list[0] == '[')
        list.erase(0,1);
    while ((pos = list.find(delimiter)) != string::npos) {
        token = list.substr(0, pos);
        vector_out.push_back(stoi(token));
        list.erase(0, pos + delimiter.length());
    }
    vector_out.push_back(stoi(list));
    return vector_out;
}

Game::Game(string data_config_file_, string board_config_file_) {
    data_config_file = data_config_file_;
    board_config_file = board_config_file_;
    load_data_config();
    initialise_game();
};

void Game::load_data_config(){
    vector<string> variable;
    string line,variable_name,word;
    ifstream file(data_config_file);
    if(!file.is_open()) {
        throw;
    }
    while(getline (file, line)){
        stringstream str(line);
        getline(str,variable_name,':');
        getline(str,word,':');
        variable.push_back(word);
    }
    try {
        board_config.num_rows = stoi(variable[0]);
        board_config.num_cols = stoi(variable[1]);
        board_config.num_tiles = stoi(variable[2]);
        board_config.num_players = stoi(variable[3]);
        board_config.max_players = stoi(variable[4]);
        board_config.robber_tile = 0;
        board_config.board_config_file = board_config_file;
        board_config.ocean_spot = remove_r(variable[5]);
        board_config.port_spot = remove_r(variable[6]);
        board_config.building_spot = remove_r(variable[7]);
        board_config.tile_order = string_list_to_vector(variable[8]);
        board_config.tile_count = string_list_to_vector(variable[9]);
        board_config.tile_numbers = string_list_to_vector(variable[10]);
    }
    catch (const exception& e){
        cout << "ERROR!" << endl;
        throw e;
    }
    if (board_config.tile_order.size() != board_config.num_tiles 
    || board_config.tile_count.size() != 7
    || board_config.tile_numbers.size() != board_config.num_tiles-1){
        cout << "Size ERROR!" << endl; 
        throw;
    }
    file.close();

}

void Game::initialise_game() {
    vector<struct Player_State>* players = new vector<struct Player_State>;
    for(int i=0;i<board_config.num_players;i++){
        struct Player_State player = {0,0,0,0,0};
        players->push_back(player);
    }

    game_state = new Game_State;
    game_state->test = 1;
    game_state->bank = new Bank(5,19);
    game_state->board = new Board(board_config);
    game_state->players = players;
}

Game_State* Game::get_game_state() {return game_state;}

int Game::get_num_players() {return num_players;}