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

Game::Game(string data_config_file_, string board_config_file_, vector<Player*> players_) {
    data_config_file = data_config_file_;
    board_config_file = board_config_file_;
    game_state = new Game_State;
    players = players_;
    generator.seed(time(0));
    uniform_int_distribution<> dice_(1,6);
    dice = dice_;
    turn = 0;
    longest_road = 0;
    largest_army = 0;
    longest_road_owner = 4;
    largest_army_owner = 2;
    rolled_dice = false;
    played_dev_card = false;
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
        board_config.robber_tile = 0;
        board_config.board_config_file = board_config_file;
        board_config.num_resources = NUM_RESOURCES;
        board_config.num_rows = stoi(variable[0]);
        board_config.num_cols = stoi(variable[1]);
        board_config.num_tiles = stoi(variable[2]);
        board_config.num_players = stoi(variable[3]);
        board_config.max_players = stoi(variable[4]);
        board_config.ocean_spot = remove_r(variable[5]);
        board_config.port_spot = remove_r(variable[6]);
        board_config.building_spot = remove_r(variable[7]);
        board_config.tile_order = string_list_to_vector(variable[8]);
        board_config.tile_count = string_list_to_vector(variable[9]);
        board_config.tile_numbers = string_list_to_vector(variable[10]);
        dev_card_count = string_list_to_vector(variable[11]);
        board_config.harbor_count= string_list_to_vector(variable[12]);
    }
    catch (const exception& e){
        cout << "ERROR!" << endl;
        throw e;
    }
    if (board_config.tile_order.size() != board_config.num_tiles 
    || board_config.tile_count.size() != 7
    || board_config.tile_numbers.size() != board_config.num_tiles-1){
        throw runtime_error("Incorrect Size");
    }
    file.close();

}

void Game::initialise_game() { 
    vector<int> c{14,5,2,2,2};

    bank = new Bank(5,19,c,generator);
    board = new Board(board_config, generator);
    
    game_state->board = board->get_board();
    game_state->tiles = board->get_tiles();
    game_state->roads = board->get_roads();
    vector<Player_State*> player_states;
    for(Player* player : players){
        player_states.push_back(player->get_player_state());
    }
    game_state->players = player_states;

    game_state->log_stream << board->get_board_state() << endl << board->get_tile_state() << endl << bank->get_dev_card_state() << endl << "Game Log:" << endl;
}

void Game::simulation(string log_file) {
    Player_Move move = NoMove;
    int x = 0;
    bool game_run = true;
    start_game();
    game_state->log_stream << "Turn" << 1 << endl;
    while(x < 200 && game_run){
        game_state->log_stream << "P" << turn+1;
        do{
            move = players[turn]->call_player_action();
            switch(move){
                case PlaceRoad:
                case PlaceSettlement:
                case PlaceCity:
                case BuyDevCard:
                    place_game_piece(move);
                    break;
                case RollDice:
                    roll_dice();
                    break;
                case UseDevCard:
                    use_dev_card();
                    break;
                case TradeOffer:
                    trading();
                    break;
                case TradeBank:
                    trade_bank();
                    break;
                case EndTurn:
                    break;
            }
            if(players[turn]->get_victory_points() >= 10) {
                game_run = false;
            }
            game_state->board = board->get_board();
            game_state->roads = board->get_roads();
            game_state->tiles = board->get_tiles();
        }while(move != EndTurn && game_run);
        game_state->log_stream << "E" << endl;
        turn = (turn +1)%board_config.num_players;
        rolled_dice = false;
        played_dev_card = false;
        if (turn == 0 && game_run) {
            game_state->log_stream << "Turn" << x+2 << endl;
            x++;
        }
    }

    std::ofstream logFile(log_file);
    if (logFile.is_open()) {
        logFile << game_state->log_stream.str();
        logFile.close();
        std::cout << "Log file written" << std::endl;
    } else {
        std::cout << "Unable to open log file" << std::endl;
    }
}

void Game::start_game() {
    int settle_spot, road_spot, tile_terrain;
    bool cell_test;
    vector<BoardCell*> cells;
    vector<Resource> resources;
    Road_Data road;
    for(int i=0;i<players.size();i++){
        settle_spot = players[i]->place_settlement(board->settlement_options(i+1,true));
        game_state->log_stream << "P" << to_string(i+1) << "S" << settle_spot;
        tuple<int,int> coords = board->get_boardcell_coords(settle_spot);
        board->add_settlement(get<0>(coords),get<1>(coords),i+1);
        players[i]->add_victory_points(1);
        players[i]->add_harbor(board->get_boardcell(get<0>(coords),get<1>(coords))->get_harbor());
        place_road(players[i]->place_road(board->road_options(i+1,true)),i+1);
    }
    for(int i=players.size()-1;i>=0;i--){
        resources.clear();
        settle_spot = players[i]->place_settlement(board->settlement_options(i+1,true));
        game_state->log_stream << "P" << to_string(i+1) << "S" << settle_spot;
        tuple<int,int> coords = board->get_boardcell_coords(settle_spot);
        board->add_settlement(get<0>(coords),get<1>(coords),i+1);
        players[i]->add_victory_points(1);
        players[i]->add_harbor(board->get_boardcell(get<0>(coords),get<1>(coords))->get_harbor());
        place_road(players[i]->place_road(board->road_options(i+1,true)),i+1);
        for(int i=0;i<board_config.num_tiles;i++) {
            cells = board->get_tile(i)->get_adjacencies();
            cell_test = false;
            for(int j=0;j<cells.size();j++){
                if(settle_spot == cells[j]->get_id()){
                    cell_test=true;
                    break;
                }
            }
            if(cell_test){
                tile_terrain = (int)board->get_tile(i)->get_terrain();
                resources.push_back((Resource)(tile_terrain-2));
            }
        }
        players[i]->add_resources(resources);
    }
    game_state->log_stream << endl;
    cout << game_state->log_stream.str();
}

void Game::trading() {
    Trade trade = players[turn]->trade_offer();
    vector<int> accept_player;
    for(Player* player: players) {
        if(player->get_owner() == turn)
            continue;
        if(player->accept_trade(trade))
            accept_player.push_back(player->get_owner());
    }
    int selected_player = players[turn]->select_trade(accept_player);
    if (selected_player == -1) {
        game_state->log_stream << "O" << to_string(0);
        return;
    }
    players[turn]->add_resources(trade.request);
    players[turn]->remove_resources(trade.offer);
    players[selected_player-1]->add_resources(trade.offer);
    players[selected_player-1]->remove_resources(trade.request);
    string offer, request;
    for(Resource r: trade.offer){
        offer += to_string((int)r);
    }
    for(Resource r: trade.request){
        request += to_string((int)r);
    }
    game_state->log_stream << "O" << selected_player << "G" << turn+1 << "X" << request << "L" << turn+1 << "X" << offer << "G" << selected_player << "X" << offer << "L" << selected_player << "X" << request;
}

void Game::trade_bank() {
    Trade trade = players[turn]->trade_bank();
    if(!bank->accept_trade_offer(trade, trade.harbor))
        return;
    if(!bank->remove_resources(trade.request))
        return;
    players[turn]->add_resources(trade.request);
    players[turn]->remove_resources(trade.offer);
    bank->add_resources(trade.offer);
    string offer, request;
    for(Resource r: trade.offer){
        offer += to_string((int)r);
    }
    for(Resource r: trade.request){
        request += to_string((int)r);
    }
    game_state->log_stream << "TG" << turn+1 << "X" << request << "L" << turn+1 << "X" << offer;
}

void Game::roll_dice() {
    if (rolled_dice)
        return;
    vector<vector<Resource>> resources(board_config.num_players,vector<Resource>());
    vector<Resource> discard;
    string cards;
    int roll = dice(generator) + dice(generator);
    game_state->log_stream << "D" << roll;
    rolled_dice = true;
    if(roll == 7){
        for(Player* p: players){
            if(p->get_num_resources()<=7)
                continue;
            discard = p->discard_hand();
            p->remove_resources(discard);
            bank->add_resources(discard);
            cards = "";
            for(int i=0;i<discard.size();i++){
                cards += to_string((int)discard[i]);
            }
            game_state->log_stream << "L" << p->get_owner() << "X" << cards;
        }
        move_robber(true);
    }
    for(int i=0;i<board_config.num_tiles;i++) {
        if(board->get_tile(i)->get_number() != roll)
            continue;
        for(BoardCell* cell: board->get_tile(i)->get_adjacencies()) {
            if(cell->get_owner() == 0)
                continue;
            if(cell->get_building() == Settlement)
                resources[cell->get_owner()-1].push_back((Resource)((int)board->get_tile(i)->get_terrain()-2));
            if(cell->get_building() == City){
                resources[cell->get_owner()-1].push_back((Resource)((int)board->get_tile(i)->get_terrain()-2));
                resources[cell->get_owner()-1].push_back((Resource)((int)board->get_tile(i)->get_terrain()-2));
            }
        }
    }
    for(int i=0;i<board_config.num_players;i++) {
        players[i]->add_resources(resources[i]);
    }
}

void Game::move_robber(bool seven) {
    vector<int> tile_choices;
    vector<Resource> resource_choice;
    for(vector<int> tile : board->get_tiles()) {
                if(tile[2] == 1)
                    continue;
                tile_choices.push_back(tile[0]);
            }
    int robber_spot = players[turn]->move_robber(tile_choices);
    if(seven) 
        game_state->log_stream << "K" << robber_spot;
    else {
        game_state->log_stream << "U0K" << robber_spot;
        if(largest_army < game_state->players[turn]->knights) {
            if (largest_army_owner != 0)
                players[largest_army_owner-1]->remove_victory_points(2);
            players[turn]->add_victory_points(2);
            largest_army_owner = turn+1;
            largest_army = game_state->players[turn]->knights;
        }
    }
    board->move_robber(robber_spot);
    int selected_player = players[turn]->steal_resource(board->get_tile(robber_spot)->get_adjacent_owners());
    if(selected_player == -1 || selected_player == turn+1)
        return;
    int count;
    for(int i=0;i<NUM_RESOURCES;i++){
        for(int j=0;j<players[selected_player-1]->get_resource_count((Resource)i);j++){
            resource_choice.push_back((Resource)i);
        }
    }
    if(resource_choice.size() <=0)
        return;
    shuffle(resource_choice.begin(),resource_choice.end(),generator);
    Resource resource_selected = resource_choice[0];
    players[turn]->add_resources(vector<Resource>{resource_selected});
    players[selected_player-1]->remove_resources(vector<Resource>{resource_selected});
    game_state->log_stream << "G" << turn+1 << "X" << to_string((int)resource_selected) << "L" << selected_player << "X" << to_string((int)resource_selected);
}

void Game::place_game_piece(Player_Move move_) {
    vector<Resource> road_cost{Brick,Lumber};
    vector<Resource> settlement_cost{Brick,Lumber,Wool,Grain};
    vector<Resource> city_cost{Grain,Grain,Ore,Ore,Ore};
    vector<Resource> dev_cost{Wool,Grain,Ore};
    int settlement_spot, city_spot, road_spot;
    tuple<int,int> coords;
    switch(move_) {
        case PlaceRoad:
            if(!players[turn]->has_resources(road_cost))
                break;
            road_spot = players[turn]->place_road(board->road_options(turn+1,false));
            place_road(road_spot,turn+1);
            players[turn]->remove_resources(vector<Resource>{Brick,Lumber});
            bank->add_resources(vector<Resource>{Brick,Lumber});
            break;
        case PlaceSettlement:
            if(!players[turn]->has_resources(settlement_cost))
                break;
            settlement_spot = players[turn]->place_settlement(board->settlement_options(turn+1,false));
            if(settlement_spot == -1 || players[turn]->get_settlement_count() >0)
                break;
            game_state->log_stream << "S" << settlement_spot;
            coords = board->get_boardcell_coords(settlement_spot);
            board->add_settlement(get<0>(coords),get<1>(coords),turn+1);
            players[turn]->add_harbor(board->get_boardcell(get<0>(coords),get<1>(coords))->get_harbor());
            players[turn]->remove_settlement();
            players[turn]->remove_resources(vector<Resource>{Brick,Lumber,Wool,Grain});
            bank->add_resources(vector<Resource>{Brick,Lumber,Wool,Grain});
            players[turn]->add_harbor(board->get_boardcell(get<0>(coords),get<1>(coords))->get_harbor());
            break;
        case PlaceCity:
            if(!players[turn]->has_resources(city_cost))
                break;
            city_spot = players[turn]->place_city(board->city_options(turn+1));
            if(city_spot == -1 || players[turn]->get_city_count() > 0)
                break;
            game_state->log_stream << "C" << city_spot;
            coords = board->get_boardcell_coords(city_spot);
            board->upgrade_settlement(get<0>(coords),get<1>(coords));
            players[turn]->remove_city();
            players[turn]->add_settlement();
            players[turn]->remove_resources(vector<Resource>{Grain,Grain,Ore,Ore,Ore});
            bank->add_resources(vector<Resource>{Grain,Grain,Ore,Ore,Ore});
            break;
        case BuyDevCard:
            if(!players[turn]->has_resources(dev_cost))
                break;
            game_state->log_stream << "B";
            players[turn]->add_dev_card(bank->draw_dev_card());
            players[turn]->remove_resources(vector<Resource>{Wool,Grain,Ore});
            bank->add_resources(vector<Resource>{Wool,Grain,Ore});
            break;
        default:
            break;
    }
}

void Game::use_dev_card() {
    if(played_dev_card)
        return;
    Development_Card card = players[turn]->use_dev_card();
    vector<BoardCell*> cells;
    int road_spot;
    Road_Data road;
    played_dev_card = true;
    switch(card) {
        case Knight:
            move_robber(false);
            break;
        case RoadBuilding:
            for(int i=0;i<2;i++){
                place_road(players[turn]->place_road(board->road_options(turn+1,false)),turn+1);
            }
            break;
        case YearOfPlenty:
            year_of_plenty(players[turn]->year_of_plenty());
            break;
        case Monopoly:
            monopoly(players[turn]->monopoly());
            break;
        default:
            break;
    }
    players[turn]->remove_dev_card(card);
}

void Game::year_of_plenty(vector<Resource> resources_) {
    string out;
    if(resources_.size() != 2)
        return;
    for(Resource r: resources_){
        out += to_string((int)r);
    }
    game_state->log_stream << "U3G" << turn << "X" << out;
    if (!bank->remove_resources(resources_))
        return;
    players[turn]->add_resources(resources_);
}

void Game::monopoly(Resource resource_) {
    vector<Resource> resources;
    int count;
    string gain, lose;
    for(Player* player: players) {
        if(player->get_owner() == turn+1)
            continue;
        count = player->get_resource_count(resource_);
        if(count<=0)
            continue;
        lose += "L" + to_string(player->get_owner()-1) + "X";
        for(int i=0;i<count;i++){
            resources.push_back(resource_);
            gain += to_string((int)resource_);
            lose += to_string((int)resource_);
        }
        player->remove_resources(resources);
        players[turn]->add_resources(resources);
    }
    game_state->log_stream << "U4G" << turn << "X" << gain << lose;
}

void Game::place_road(int road_spot_, int owner_) {
    if(road_spot_ == -1 || players[turn]->get_road_count() > 0)
        return;
    game_state->log_stream << "R" << road_spot_;
    board->add_road(road_spot_, owner_);
    players[turn]->remove_road();
    if (game_state->players[turn]->road_length <= longest_road || longest_road_owner == turn+1)
        return;
    if (longest_road_owner != 0)
        players[longest_road_owner-1] ->remove_victory_points(2);
    players[turn]->add_victory_points(2);
    longest_road_owner == turn + 1;
    longest_road = game_state->players[turn]->road_length;
}