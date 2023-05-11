#include "board.h"
#include <string>
using namespace std;

Board::Board(Board_Config board_config_, default_random_engine generator_) {
    board_config = board_config_;
    generator = generator;
    try {
    load_board_config();
    } catch(const exception& e) {
        throw e;
    }
    calc_adjacencies();
    generate_ports();
    generate_tiles();
    generate_roads();
}


string Board::remove_r(string s) {
    if (s[s.size()-1] == '\r')
        s.erase(s.size()-1);
    return s;
}

vector<int> Board::string_list_to_vector(string list) {
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

bool Board::road_match(Road road, BoardCell* cell1, BoardCell* cell2) {
    return road.get_cells()[0]->get_id()==min(cell1->get_id(),cell2->get_id()) && road.get_cells()[1]->get_id()==max(cell1->get_id(),cell2->get_id());
}

int Board::road_length(bool first,int owner_, BoardCell* cell, vector<int>* road_input) {
    if (road_input->size() <= 0)
        return 0;
    if(cell->get_owner() != owner_ && cell->get_owner() != 0 && !first)
        return 0;
    vector<BoardCell*> adjacent = cell->get_adjacent();
    int road = 0,sum=0;

    for (BoardCell* c: adjacent) {
        road = get_road_index(c,cell);
        auto it = find(road_input->begin(),road_input->end(),road);
        if (it != road_input->end()) {
            road_input->erase(road_input->begin() + (it-road_input->begin()));
            sum = max(road_length(false,owner_,c,road_input) + 1,sum);
        }
    }
    return sum;
}

void Board::load_board_config() {
    vector<BoardCell*> row;
    string line,word;

    ifstream file (board_config.board_config_file);
    if(!file.is_open()) {
        throw runtime_error("File Not Found Exception!");
    }
    int i=0;
    int x=0;
    while(getline(file,line)) {
        row.clear();
        stringstream str(line);
        int j=0;
        while(getline(str,word,',')) {
            if (word[word.size()-1] == '\r')
                word.erase(word.size() - 1);
            row.push_back(new BoardCell(i,j,x,word));
            j++;
            x++;
        }
        board.push_back(row);
        i++;
    }
    file.close();
    bool test = false;
    i=0;
    for (vector<BoardCell*> b: board) {
        if(board_config.num_cols != board[i].size()) {
            test = true;
            break;
        }
        i++;
    }
    if (board_config.num_rows != board.size() || test) {
        throw runtime_error("Board Size Inconsistent!");
    }
}

void Board::calc_adjacencies() {
    vector<BoardCell*> adj;
    BoardCell* curr;
    for (int i=0;i<board_config.num_rows;i++) {
        for (int j=0;j<board[i].size();j++) {
            adj.clear();
            curr = board[i][j];
            if (curr->get_type() == board_config.ocean_spot)
                continue;
            if (i > 0 && (i+j)%2 == 1 && board[i-1][j]->get_type() != board_config.ocean_spot)
                adj.push_back(board[i-1][j]);
            if (i < board_config.num_rows-1  && (i+j)%2 == 0 && board[i+1][j]->get_type() != board_config.ocean_spot)
                adj.push_back(board[i+1][j]);
            if (j > 0 && board[i][j-1]->get_type() != board_config.ocean_spot)
                adj.push_back(board[i][j-1]);
            if (j < board[i].size()-1 && board[i][j+1]->get_type() != board_config.ocean_spot)
                adj.push_back(board[i][j+1]);
            curr->set_adjacent(adj);
        }
    }
    for(int i=0;i<board_config.num_players;i++) {
        unordered_set<int> cells;
        owned_cells.push_back(cells);
        num_settlements.push_back(0);
    }
}

void Board::generate_ports() {
    vector<Harbor> harbors;
    for(int i=0;i<board_config.harbor_count.size();i++){
        for(int j=0;j<board_config.harbor_count[i];j++) {
            harbors.push_back((Harbor)i);
        }
    }
    shuffle(begin(harbors),end(harbors),generator);
    for (int i=0;i<board_config.num_rows;i++) {
        for (int j=0;j<board[i].size();j++) {
            if(board[i][j]->get_type()[0] != board_config.port_spot[0])
                continue;
            board[i][j]->set_harbor(harbors[stoi(board[i][j]->get_type().substr(1))-1]);
        }
    }
}

void Board::generate_tiles() {
    int corners[6][2] = {{0,0},{0,1},{0,2},{1,0},{1,1},{1,2}};
    vector<int> tile_num = board_config.tile_count;
    vector<BoardCell*> adj;

    int tiles_left = board_config.num_tiles;
    int tile_id = 0;
    int x = 0;
    int rand_tile;

    bool empty;
    bool robber_placed = false;

    vector<Terrain> tile_type;
    for (int t: board_config.tile_count) {
        for(int i=0;i<t;i++) {
            tile_type.push_back((Terrain)x);
        }
        x++;
    }
    shuffle(begin(tile_type),end(tile_type),generator);
    for (int i=0;i<board_config.num_rows-1;i++) {
        for (int j=i%2;j<board_config.num_cols-2;j+=2) {
            adj.clear();
            empty = false;
            for (auto& c: corners) {
                if (board[c[0]+i][c[1]+j]->get_type() == board_config.ocean_spot) {
                    empty = true;
                    break;
                }
                adj.push_back(board[c[0]+i][c[1]+j]);
            }
            if (empty)
                continue;
            tiles.push_back(new Tile(i,j,tile_id,tile_type[tile_id],adj));
            tile_id++;
        }
    }
    x = 0;
    for (int i=0;i<board_config.num_tiles;i++) {
        if (tiles[board_config.tile_order[i]]->get_terrain() == Ocean)
            continue;
        if (tiles[board_config.tile_order[i]]->get_terrain() == Desert) {
            tiles[board_config.tile_order[i]]->set_number(0);
            if (!robber_placed) {
                tiles[board_config.tile_order[i]]->set_robber(true);
                board_config.robber_tile = board_config.tile_order[i];
                robber_placed = true;
            }
            continue;
        }
        tiles[board_config.tile_order[i]]->set_number(board_config.tile_numbers[x]);
        x++;
    }
}

void Board::generate_roads() {
    BoardCell* curr_id;
    vector<BoardCell*> curr_adj;
    for (int i=0;i<board_config.num_rows;i++) {
        for (int j=0;j<board_config.num_cols;j++) {
            curr_id = board[i][j];
            curr_adj = board[i][j]->get_adjacent();
            for (int k=0;k<curr_adj.size();k++) {
                if(get_road_index(curr_id,curr_adj[k]) != -1)
                    continue;
                if(curr_id->get_id() < curr_adj[k]->get_id()) {
                    roads.push_back(new Road(curr_id, curr_adj[k]));
                } else {
                    roads.push_back(new Road(curr_adj[k],curr_id));
                }
            }
        }
    }
    for(int i=0;i<board_config.num_players;i++) {
        unordered_set<int> cells;
        owned_road_cells.push_back(cells);
        num_roads.push_back(0);
    }

}

vector<int> Board::calc_road_length() {
    vector<int> length_out(board_config.num_players), road_list;
    vector<vector<int>> road_input(board_config.num_players), starting_cells(board_config.num_players);
    map<int,int> cell_map;
    int owner, sum;

    for(int i=0;i<roads.size();i++) {
        owner = roads[i]->get_owner()-1;
        if (owner < 0)
            continue;
        road_input[owner].push_back(i);
    }
    for(int i=0;i<board_config.num_players;i++) {
        if (road_input[i].size() == 0)
            continue;
        cell_map.clear();
        for(int j=0;j<road_input[i].size();j++) {
            cell_map[roads[road_input[i][j]]->get_cells()[0]->get_id()]++;
            cell_map[roads[road_input[i][j]]->get_cells()[1]->get_id()]++;
        }
        for (const auto & kv: cell_map) {
            tuple<int,int> coords = get_boardcell_coords(kv.first);
            if(board[get<0>(coords)][get<1>(coords)]->get_owner() != i+1 
            && board[get<0>(coords)][get<1>(coords)]->get_owner() != 0) {
                starting_cells[i].push_back(kv.first);
                continue;
            }
            if (kv.second%2 == 1)
                starting_cells[i].push_back(kv.first);
        }
        if (starting_cells[i].size() == 0) {
            road_list = road_input[i];
            vector<int>* road_add = &road_list;
            length_out[i] = road_length(true,i+1,roads[road_input[i][0]]->get_cells()[0],road_add);
        } else {
            sum = 0;
            for(int cell: starting_cells[i]) {
                road_list = road_input[i];
                vector<int>* road_add = &road_list;
                tuple<int,int> coords = get_boardcell_coords(cell);
                sum = max(road_length(true,i+1,get_boardcell(get<0>(coords),get<1>(coords)),road_add),sum);
            }
            length_out[i] = sum;
        }
    }
    return length_out;
}

void Board::add_road(int road_, int owner_) {
    
    if (road_ == -1 || roads[road_]->get_owner() != 0)
        return;
    roads[road_]->set_owner(owner_);
    owned_road_cells[owner_-1].insert(roads[road_]->get_cells()[0]->get_id());
    owned_road_cells[owner_-1].insert(roads[road_]->get_cells()[1]->get_id());
    num_roads[owner_-1]++;
}

void Board::add_settlement(int row_, int col_, int owner_) {
    if(settlement_options(owner_,false).size() == 0) {
        board[row_][col_]->set_start(true);
    }
    if(board[row_][col_]->get_type() != board_config.ocean_spot && !is_too_close(row_,col_)) {
        board[row_][col_]->set_owner(owner_);
        board[row_][col_]->set_building(Settlement);
        owned_cells[owner_-1].insert(board[row_][col_]->get_id());
        board[row_][col_]->set_settlement(owned_cells[owner_-1].size());
        num_settlements[owner_-1]++;
    }
}

void Board::upgrade_settlement(int row_, int col_) {
    if(board[row_][col_]->get_building() == Settlement) {
        board[row_][col_]->set_building(City);
    }
}

void Board::move_robber(int tile_) {
    if (board_config.robber_tile == tile_)
        return;
    tiles[board_config.robber_tile]->set_robber(false);
    tiles[tile_]->set_robber(true);
    board_config.robber_tile = tile_;
}

vector<int> Board::robber_options() {
    vector<int> robber_spots;
    for(int i=0;i<board_config.num_tiles;i++) {
        if(i == board_config.robber_tile)
            continue;
        robber_spots.push_back(i);
    }
    return robber_spots;
}

vector<int> Board::road_options(int owner_, bool start_) {
    vector<int> road_spots{};

    if(start_) {
        if(num_roads[owner_-1] >= num_settlements[owner_-1])
            return road_spots;
        for(int r: owned_cells[owner_-1]) {
            tuple<int,int> coords = get_boardcell_coords(r);
            if(board[get<0>(coords)][get<1>(coords)]->get_settlement() != owned_cells[owner_-1].size())
                continue;
            for(BoardCell* c: board[get<0>(coords)][get<1>(coords)]->get_adjacent()) {
                road_spots.push_back(get_road_index(board[get<0>(coords)][get<1>(coords)],c));
            }
        }
    } else {
        int idx = -1;
        for(int r: owned_road_cells[owner_-1]) {
            tuple<int,int> coords = get_boardcell_coords(r);
            if (board[get<0>(coords)][get<1>(coords)]->get_owner() != owner_ && board[get<0>(coords)][get<1>(coords)]->get_owner() != 0)
                continue;
            for(BoardCell* c: board[get<0>(coords)][get<1>(coords)]->get_adjacent()) {
                idx = get_road_index(board[get<0>(coords)][get<1>(coords)],c);
                if(count(road_spots.begin(),road_spots.end(),idx) != 0 || roads[idx]->get_owner() != 0)
                    continue;
                road_spots.push_back(get_road_index(board[get<0>(coords)][get<1>(coords)],c));
            }
        }
    }
    return road_spots;
}

vector<int> Board::settlement_options(int owner_, bool start_) {
    vector<int> settlement_spots;
    if(start_) {
        for(int i=0;i<board_config.num_rows;i++) {
            for(int j=0;j<board_config.num_cols;j++) {
                if(board[i][j]->get_type() != board_config.ocean_spot && !is_too_close(i,j))
                    settlement_spots.push_back(board[i][j]->get_id());
            }
        }
    } else {
        for(int c: owned_road_cells[owner_-1]) {
            tuple<int,int> coords = get_boardcell_coords(c);
            if(!is_too_close(get<0>(coords),get<1>(coords)))
                settlement_spots.push_back(c);
        }
    }
    sort(settlement_spots.begin(),settlement_spots.end());
    return settlement_spots;
}

vector<int> Board::city_options(int owner_) {
    vector<int> options;
    for(int c: owned_cells[owner_-1]) {
        tuple<int,int> coords = get_boardcell_coords(c);
        if(board[get<0>(coords)][get<1>(coords)]->get_owner() != owner_ || board[get<0>(coords)][get<1>(coords)]->get_building() != Settlement)
            continue;
        options.push_back(c);
    }
    return options;
}

bool Board::is_too_close(int row_, int col_) {
    if (board[row_][col_]->get_building() != Undeveloped)
        return true;
    vector<BoardCell*> adj = board[row_][col_]->get_adjacent();
    for (BoardCell* cell: adj) {
        if (cell->get_building() != Undeveloped)
            return true;
    }
    return false;
}

int Board::get_road_index(BoardCell* cell1, BoardCell* cell2) {
    auto it = find_if(roads.begin(), roads.end(), [&,cell1,cell2](Road* road) {return road_match(*road,cell1,cell2);});
    if(it==roads.end())
        return -1;
    return distance(roads.begin(),it);
}

vector<int> Board::get_tile_index(int number_) {
    vector<int> tile_out;
    for (int i=0;i<board_config.num_tiles;i++) {
        if(tiles[i]->get_number() == number_) {
            tile_out.push_back(i);
        }
    }
    return tile_out;
}

vector<vector<vector<int>>> Board::get_board() {
    vector<vector<vector<int>>> board_out;
    vector<vector<int>> cols;
    vector<int> cell;
    for(int i=0;i<board_config.num_rows;i++) {
        cols.clear();
        for(int j=0;j<board_config.num_cols;j++) {
            cell = {board[i][j]->get_owner(),static_cast<int>(board[i][j]->get_building())};
            cols.push_back(cell);
        }
        board_out.push_back(cols);
    }
    return board_out;
}

string Board::get_board_state() {
    string output = "Board State:\n";
    string character;
    for(int i=0;i<board_config.num_rows;i++){
        for(int j=0;j<board_config.num_cols;j++){
            character = board[i][j]->get_type().at(0);
            if(character == board_config.port_spot){
                switch(board[i][j]->get_harbor()){
                    case GenericHarbor:
                        output += "H";
                        break;
                    case BrickHarbor:
                        output += "C";
                        break;
                    case LumberHarbor:
                        output += "L";
                        break;
                    case WoolHarbor:
                        output += "W";
                        break;
                    case GrainHarbor:
                        output += "G";
                        break;
                    case OreHarbor:
                        output += "R";
                        break;
                    default:
                        output += "X";
                        break;
                }
                continue;
            }
            output += board[i][j]->get_type();
        }
        output += "\n";
    }
    return output;
}

vector<vector<int>> Board::get_resources(int number_) {
    vector<vector<int>> resources_out(board_config.num_resources,vector<int>(board_config.num_players));
    BoardCell* cell;
    Tile* tile;
    int owner, tile_type;
    for (int idx: get_tile_index(number_)) {
        tile = tiles[idx];
        if(tile->has_robber())
            continue;
        for (BoardCell* cell: tile->get_adjacencies()) {
            owner = cell->get_owner()-1;
            tile_type = static_cast<int>(tile->get_terrain())-2;
            if (owner < 0 || tile->get_terrain() == Desert || tile->get_terrain() == Ocean)
                continue;
            if (cell->get_building() == Settlement)
                resources_out[tile_type][owner]++;
            if (cell->get_building() == City)
                resources_out[tile_type][owner]+=2;
        }
    }
    return resources_out;
}

vector<int> Board::get_roads() {
    vector<int> road_out;
    for (int i=0;i<roads.size();i++) {
        road_out.push_back(roads[i]->get_owner());
    }
    return road_out;
}

vector<vector<int>> Board::get_tiles() {
    vector<vector<int>> tiles_out;
    vector<int> tile_data;
    for (int i=0;i<board_config.num_tiles;i++) {
        tile_data = {tiles[i]->get_number(),static_cast<int>(tiles[i]->get_terrain()),tiles[i]->has_robber()};
        tiles_out.push_back(tile_data);
    }
    return tiles_out;
}

string Board::get_tile_state() {
    string output = "Tile State:\n";
        for(int i=0;i<board_config.num_tiles;i++){
            switch(tiles[i]->get_terrain()){
                case Ocean:
                    output += "O";
                    break;
                case Desert:
                    output += "D";
                    break;
                case Hill:
                    output += "H";
                    break;
                case Forest:
                    output += "T";
                    break;
                case Pasture:
                    output += "P";
                    break;
                case Field:
                    output += "F";
                    break;
                case Mountian:
                    output += "M";
                    break;
                default:
                    output += "X";
                    break;

            }
            output += to_string(tiles[i]->get_number());
        }
    return output + "\n";
}
