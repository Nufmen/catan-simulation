#include "board.h"
#include "pybind11/pybind11.h"
#include <string>
using namespace std;
namespace py = pybind11;

Board::Board(string dataConfigFile_, string boardConfigFile_) {
    data_config_file = dataConfigFile_;
    board_config_file = boardConfigFile_;
    num_rows = 0;
    num_cols = 0;
    robber_tile = 0;
    num_players = 4;
    generator.seed(time(0));
    loadDataConfig();
    loadBoardConfig();
    calcAdjacencies();
    generateTiles();
    generateRoads();
}

Board::Board(string dataConfigFile_, string boardConfigFile_, int seed_) {
    data_config_file = dataConfigFile_;
    board_config_file = boardConfigFile_;
    num_rows = 0;
    num_cols = 0;
    robber_tile = 0;
    num_players = 4;
    generator.seed(seed_);
    loadDataConfig();
    loadBoardConfig();
    calcAdjacencies();
    generateTiles();
    generateRoads();
}


string Board::removeR(string s) {
    if (s[s.size()-1] == '\r')
        s.erase(s.size()-1);
    return s;
}

vector<int> Board::stringListToVector(string list) {
    vector<int> vector_out;
    string delimiter = ",";
    string token;
    size_t pos = 0;
    list = removeR(list);
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

bool Board::roadMatch(Road road, int cell1, int cell2) {
    return road.getCell(1)==min(cell1,cell2) && road.getCell(2)==max(cell1,cell2);
}

int Board::roadLength(bool first,int owner_, int cell, vector<int> *road_input) {
    if (road_input->size() <= 0)
        return 0;
    tuple<int,int> coords = getBoardCellCoords(cell);
    if(board[get<0>(coords)][get<1>(coords)]->getOwner() != owner_ 
    && board[get<0>(coords)][get<1>(coords)]->getOwner() != 0 
    && !first)
        return 0;
    vector<int> adjacent = board[get<0>(coords)][get<1>(coords)]->getAdjacent();
    int road = 0,sum=0;

    for (int c: adjacent){
        road = getRoadIndex(c,cell);
        auto it = find(road_input->begin(),road_input->end(),road);
        if (it != road_input->end()){
            road_input->erase(road_input->begin() + (it-road_input->begin()));
            sum = max(roadLength(false,owner_,c,road_input) + 1,sum);
        }
    }
    return sum;
}

void Board::loadDataConfig(){
    vector<string> variable;
    string line,variable_name,word;
    ifstream file(data_config_file);
    if(!file.is_open()) {
        PyErr_SetString(PyExc_RuntimeError, "FileNotFoundException");
        throw py::error_already_set();
    }
    while(getline (file, line)){
        stringstream str(line);
        getline(str,variable_name,':');
        getline(str,word,':');
        variable.push_back(word);
    }
    try {
        num_rows = stoi(variable[0]);
        num_cols = stoi(variable[1]);
        num_tiles = stoi(variable[2]);
        max_players = stoi(variable[3]);
        ocean_spot = removeR(variable[4]);
        port_spot = removeR(variable[5]);
        building_spot = removeR(variable[6]);
        tile_order = stringListToVector(variable[7]);
        tile_count = stringListToVector(variable[8]);
        tile_numbers = stringListToVector(variable[9]);
    }
    catch (const exception& e){
        PyErr_SetString(PyExc_ValueError, "Incorrect Value Type");
        throw py::error_already_set();
    }
    if (tile_order.size() != num_tiles 
    || tile_count.size() != 7
    || tile_numbers.size() != num_tiles-1){
        PyErr_SetString(PyExc_ValueError, "Incorrect Array Length");
        throw py::error_already_set();
    }
    file.close();

}

void Board::loadBoardConfig() {
    vector<BoardCell *> row;
    string line,word;

    ifstream file (board_config_file);
    if(!file.is_open()) {
        PyErr_SetString(PyExc_RuntimeError, "FileNotFoundException");
        throw py::error_already_set();
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
    for (vector<BoardCell *> b : board){
        if(num_cols != board[i].size()){
            test = true;
            break;
        }
        i++;
    }
    if (num_rows != board.size() || test) {
        PyErr_SetString(PyExc_RuntimeError, "Board Size Inconsistent");
        throw py::error_already_set();
    }
}

void Board::calcAdjacencies() {
    vector<int> adj;
    BoardCell* curr;
    for (int i=0;i<num_rows;i++) {
        for (int j=0;j<board[i].size();j++){
            adj.clear();
            curr = board[i][j];
            if (curr->getType() == "O")
                continue;
            if (i > 0 && (i+j)%2 == 1 && board[i-1][j]->getType() != "O")
                adj.push_back(board[i-1][j]->getId());
            if (i < num_rows-1  && (i+j)%2 == 0 && board[i+1][j]->getType() != "O")
                adj.push_back(board[i+1][j]->getId());
            if (j > 0 && board[i][j-1]->getType() != "O")
                adj.push_back(board[i][j-1]->getId());
            if (j < board[i].size()-1 && board[i][j+1]->getType() != "O")
                adj.push_back(board[i][j+1]->getId());
            curr->setAdjacent(adj);
        }
    }
}

void Board::generateTiles() {
    int corners[6][2] = {{0,0},{0,1},{0,2},{1,0},{1,1},{1,2}};
    vector<int> tile_num = tile_count;
    vector<int> adj;

    int tiles_left = num_tiles;
    int tile_id = 0;
    int x = 0;
    int rand_tile;

    bool empty;
    bool robber_placed = false;

    vector<Terrain> tile_type;
    for (int t : tile_count) {
        for(int i=0;i<t;i++){
            tile_type.push_back((Terrain)x);
        }
        x++;
    }
    shuffle(begin(tile_type),end(tile_type),generator);
    for (int i=0;i<num_rows-1;i++) {
        for (int j=i%2;j<board[i].size()-2;j+=2) {
            adj.clear();
            empty = false;
            for (auto& c : corners) {
                if (board[c[0]+i][c[1]+j]->getType() == "O"){
                    empty = true;
                    break;
                }
                adj.push_back(board[c[0]+i][c[1]+j]->getId());
            }
            if (empty)
                continue;


            /*
            rand_tile = rand()%tiles_left;
            for (int i=0;i<int(sizeof(tile_num)/sizeof(int));i++){
                if (rand_tile - tile_num[i] < 0) {
                    selected_tile = (Terrain)i;
                    tile_num[i]--;
                    tiles_left--;
                    break;
                }
                rand_tile-=tile_num[i];
            }
            */
            tiles.push_back(new Tile(i,j,tile_id,tile_type[tile_id],adj));
            tile_id++;
        }
    }
    x = 0;
    for (int i=0;i<19;i++){
        if (tiles[tile_order[i]]->getTerrain() == Ocean)
            continue;
        if (tiles[tile_order[i]]->getTerrain() == Desert){
            tiles[tile_order[i]]->setNumber(0);
            if (!robber_placed) {
                tiles[tile_order[i]]->setRobber(true);
                robber_tile = tile_order[i];
                robber_placed = true;
            }
            continue;
        }
        tiles[tile_order[i]]->setNumber(tile_numbers[x]);
        x++;
    }
}

void Board::generateRoads() {
    int curr_id;
    vector<int> curr_adj;
    for (int i=0;i<num_rows;i++) {
        for (int j=0;j<board[i].size();j++) {
            curr_id = board[i][j]->getId();
            curr_adj = board[i][j]->getAdjacent();
            for (int k=0;k<curr_adj.size();k++){
                if(getRoadIndex(curr_id,curr_adj[k]) != -1)
                    continue;
                roads.push_back(new Road(min(curr_id,curr_adj[k]),max(curr_id,curr_adj[k])));
            }
        }
    }
}

vector<int> Board::calcRoadLength() {
    vector<int> length_out(num_players), road_list;
    vector<vector<int>> road_input(num_players), starting_cells(num_players);
    map<int,int> cell_map;
    int owner, sum;

    for(int i=0;i<roads.size();i++){
        owner = roads[i]->getOwner()-1;
        if (owner < 0)
            continue;
        road_input[owner].push_back(i);
    }
    for(int i=0;i<num_players;i++){
        if (road_input[i].size() == 0)
            continue;
        cell_map.clear();
        for(int j=0;j<road_input[i].size();j++){
            cell_map[roads[road_input[i][j]]->getCell(1)] += 1;
            cell_map[roads[road_input[i][j]]->getCell(2)] += 1;
        }
        for (const auto & kv : cell_map){
            tuple<int,int> coords = getBoardCellCoords(kv.first);
            if(board[get<0>(coords)][get<1>(coords)]->getOwner() != i+1 
            && board[get<0>(coords)][get<1>(coords)]->getOwner() != 0){
                starting_cells[i].push_back(kv.first);
                continue;
            }
            if (kv.second%2 == 1)
                starting_cells[i].push_back(kv.first);
        }
        if (starting_cells[i].size() == 0){
            road_list = road_input[i];
            vector<int> *road_add = &road_list;
            length_out[i] = roadLength(true,i+1,roads[road_input[i][0]]->getCell(1),road_add);
        } else {
            sum = 0;
            for(int cell : starting_cells[i]){
                road_list = road_input[i];
                vector<int> *road_add = &road_list;
                sum = max(roadLength(true,i+1,cell,road_add),sum);
            }
            length_out[i] = sum;
        }
    }
    return length_out;
}

void Board::addRoad(int cell1_, int cell2_, int owner_) {
    int idx = getRoadIndex(cell1_,cell2_);
    if (idx == -1)
        return;
    if (roads[idx]->getOwner() == 0)
        roads[idx]->setOwner(owner_);
}

void Board::addSettlement(int row_, int col_, int owner_) {
    if(board[row_][col_]->getType() != "O" && !isTooClose(row_,col_)) {
        board[row_][col_]->setOwner(owner_);
        board[row_][col_]->setBuilding(Settlement);
    }
}

void Board::upgradeSettlement(int row_, int col_) {
    if(board[row_][col_]->getBuilding() == Settlement){
        board[row_][col_]->setBuilding(City);
    }
}

void Board::moveRobber(int tile_) {
    if (robber_tile == tile_)
        return;
    tiles[robber_tile]->setRobber(false);
    tiles[tile_]->setRobber(true);
    robber_tile = tile_;
}

bool Board::isTooClose(int row_, int col_) {
    if (board[row_][col_]->getBuilding() != Undeveloped)
        return true;
    vector<int> adj = board[row_][col_]->getAdjacent();
    for (int cell : adj) {
        tuple<int,int> coords = getBoardCellCoords(cell);
        if (board[get<0>(coords)][get<1>(coords)]->getBuilding() != Undeveloped)
            return true;
    }
    return false;
}

bool Board::hasRobber(int tile_) {
    return tiles[tile_]->hasRobber();
}

int Board::getRoadIndex(int cell1, int cell2) {
    auto it = find_if(roads.begin(), roads.end(), [&,cell1,cell2](Road* road){return roadMatch(*road,cell1,cell2);});
    if(it==roads.end())
        return -1;
    return distance(roads.begin(),it);
}

vector<int> Board::getTileIndex(int number_) {
    vector<int> tile_out;
    for (int i=0;i<tiles.size();i++) {
        if(tiles[i]->getNumber() == number_){
            tile_out.push_back(i);
        }
    }
    return tile_out;
}

vector<vector<vector<int>>> Board::getBoard() {
    vector<vector<vector<int>>> board_out;
    vector<vector<int>> cols;
    vector<int> cell;
    for(int i=0;i<num_rows;i++) {
        cols.clear();
        for(int j=0;j<board[i].size();j++) {
            cell = {board[i][j]->getOwner(),static_cast<int>(board[i][j]->getBuilding())};
            cols.push_back(cell);
        }
        board_out.push_back(cols);
    }
    return board_out;
}

BoardCell Board::getBoardCell(int row_, int col_) {
    return *board[row_][col_];
    }

tuple<int,int> Board::getBoardCellCoords(int id_) {
    return make_tuple(floor(id_/num_cols),id_%num_cols);
    }

vector<vector<int>> Board::getResources(int number_) {
    vector<vector<int>> resources_out(num_resources,vector<int>(num_players));
    BoardCell* cell;
    Tile* tile;
    int owner, tile_type;
    for (int idx: getTileIndex(number_)){
        tile = tiles[idx];
        if(tile->hasRobber())
            continue;
        for (int id: tile->getAdjacencies()) {
            tuple<int,int> coords = getBoardCellCoords(id);
            cell = board[get<0>(coords)][get<1>(coords)];
            owner = cell->getOwner()-1;
            tile_type = static_cast<int>(tile->getTerrain())-2;
            if (owner < 0 || tile->getTerrain() == Desert || tile->getTerrain() == Ocean)
                continue;
            if (cell->getBuilding() == Settlement)
                resources_out[tile_type][owner]++;
            if (cell->getBuilding() == City)
                resources_out[tile_type][owner]+=2;
        }
    }
    return resources_out;
}

vector<int> Board::getRoads() {
    vector<int> road_out;
    for (int i=0;i<roads.size();i++) {
        road_out.push_back(roads[i]->getOwner());
    }
    return road_out;
}

vector<vector<int>> Board::getTiles() {
    vector<vector<int>> tiles_out;
    vector<int> tile_data;
    for (int i=0;i<tiles.size();i++){
        tile_data = {tiles[i]->getNumber(),static_cast<int>(tiles[i]->getTerrain())};
        tiles_out.push_back(tile_data);
    }
    return tiles_out;
}

Tile Board::getTile(int id_) {return *tiles[id_];}

string Board::getDataConfigFile() {return data_config_file;}

string Board::getBoardConfigFile() {return board_config_file;}

int Board::getNumRows() {return num_rows;}

int Board::getNumCols() {return num_cols;}

int Board::getNumTiles() {return num_tiles;}

int Board::getMaxPlayers() {return max_players;}

string Board::getOceanSpot() {return ocean_spot;}

string Board::getPortSpot() {return port_spot;}

string Board::getBuildingSpot() {return building_spot;}

vector<int> Board::getTileOrder() {return tile_order;}

vector<int> Board::getTileCount() {return tile_count;}

vector<int> Board::getTileNumbers() {return tile_numbers;}
