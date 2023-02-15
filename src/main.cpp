#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <algorithm>

using namespace std;
namespace py = pybind11;

enum C_Building {
    Undeveloped = 0,
    Settlement = 1,
    City = 2
};

enum C_Resource {
    Brick,
    Lumber,
    Wool,
    Grain,
    Ore
};

enum C_Terrain {
    Ocean,
    Desert,
    Hill,
    Forest,
    Mountian,
    Field,
    Pasture
};

enum C_Harbor {
    NoHarbor,
    GenericHarbor,
    BrickHarbor,
    LumberHarbor,
    WoolHarbor,
    GrainHarbor,
    OreHarbor
};


class C_BoardCell {
    int row, col, id, owner;
    string type;
    C_Building building;
    C_Harbor harbor;
    vector<int> adjacent;

    public:
    C_BoardCell(int row_, int col_, int id_, string type_) {
        row = row_;
        col = col_;
        id = id_;
        type = type_;

        owner = 0;
        building = Undeveloped;
        harbor = NoHarbor;
    }

    int getRow() {return row;}

    int getCol() {return col;}

    int getId() {return id;}

    string getType() {return type;}

    int getOwner() {return owner;}

    C_Harbor getHarbor() {return harbor;}

    C_Building getBuilding() {return building;}

    vector<int> getAdjacent() {return adjacent;}


    void setOwner(int owner_) {owner = owner_;}

    void setHarbor(C_Harbor harbor_) {harbor = harbor_;}

    void setBuilding(C_Building building_) {building = building_;}

    void setAdjacent(vector<int> adjacent_) {adjacent = adjacent_;}
};

class C_Tile {
    int row, col, id, number;
    C_Terrain terrain;
    vector<int> adjacencies;
    bool robber = false;

    public:
    C_Tile(int row_, int col_, int id_, C_Terrain terrain_, vector<int> adjacencies_) {
        row = row_;
        col = col_;
        id = id_;
        number = 0;
        terrain = terrain_;
        adjacencies = adjacencies_;
    }

    bool hasRobber() {return robber;}

    int getRow() {return row;}

    int getCol() {return col;}

    int getId() {return id;}

    int getNumber() {return number;}

    C_Terrain getTerrain() {return terrain;}

    vector<int> getAdjacencies() {return adjacencies;}

    void setRobber(bool robber_) {robber = robber_;}

    void setNumber(int number_) {number = number_;}
};

class C_Road {
    int cell1, cell2, owner;

    public:
    C_Road(int cell1_, int cell2_) {
        cell1 = cell1_;
        cell2 = cell2_;
        owner = 0;
    }

    int getCell(int c) {
        switch(c) {
            case 1:
                return cell1;
            case 2:
                return cell2;
            default:
                return -1;
        }
    }

    int getOwner() {return owner;}

    void setOwner(int owner_) {owner = owner_;}
};

class C_Board {
    int numRows, numCols, robber_tile;
    string filename;
    vector<vector<C_BoardCell *>> board;
    vector<C_Tile *> tiles;
    vector<C_Road *> roads;

    bool roadMatch(C_Road road, int cell1, int cell2) {
        return road.getCell(1)==min(cell1,cell2) && road.getCell(2)==max(cell1,cell2);
    }

    int roadLength(int owner_, vector<int> owned_roads_) {
        if (owned_roads_.size() == 0)
            return -1;
        owned_roads_.erase(owned_roads_.begin());
        tuple <int,int> coords = getBoardCellCoords(roads[owned_roads_[0]]->getCell(1));
        C_BoardCell* cell = board[get<0>(coords)][get<1>(coords)];
        return 1;
    }

    public:
    C_Board() {
        filename = "";
        numRows = 0;
        numCols = 0;
        robber_tile = 0;
    }

    C_Board(int seed_) {
        filename = "";
        numRows = 0;
        numCols = 0;
        robber_tile = 0;
        srand(seed_);
    }

    void loadBoardConfig(string filename_) {
        filename = filename_;

        vector<C_BoardCell *> row;
        string line,word;

        fstream file (filename, ios::in);
        if(file.is_open()) {
            int i=0;
            int x=0;
            while(getline(file,line)) {
                row.clear();
                stringstream str(line);
                int j=0;
                while(getline(str,word,',')) {
                    if (word[word.size()-1] == '\r')
                        word.erase(word.size() - 1);
                    row.push_back(new C_BoardCell(i,j,x,word));
                    j++;
                    x++;
                }
                board.push_back(row);
                i++;
            }
            numRows = board.size();
            numCols = board[0].size();
        } else {
            cout << "FileNotFound" << endl;
        }

    }

    void calcAdjacencies() {
        vector<int> adj;
        C_BoardCell* curr;
        for (int i=0;i<board.size();i++) {
            for (int j=0;j<board[i].size();j++){
                adj.clear();
                curr = board[i][j];
                if (curr->getType() == "O")
                    continue;
                if (i > 0 && (i+j)%2 == 1 && board[i-1][j]->getType() != "O")
                    adj.push_back(board[i-1][j]->getId());
                if (i < board.size()-1  && (i+j)%2 == 0 && board[i+1][j]->getType() != "O")
                    adj.push_back(board[i+1][j]->getId());
                if (j > 0 && board[i][j-1]->getType() != "O")
                    adj.push_back(board[i][j-1]->getId());
                if (j < board[i].size()-1 && board[i][j+1]->getType() != "O")
                    adj.push_back(board[i][j+1]->getId());
                curr->setAdjacent(adj);
            }
        }
    }

    void generateTiles() {
        int corners[6][2] = {{0,0},{0,1},{0,2},{1,0},{1,1},{1,2}};
        int order[19] = {0,3,7,12,16,17,18,15,11,6,2,1,4,8,13,14,10,5,9};
        int tile_num[7] = {0,1,3,4,3,4,4};
        int numbers[18] = {5,2,6,3,8,10,9,12,11,4,8,10,9,4,5,6,3,11};
        int tiles_left = 19;
        int tile_id = 0;
        int rand_tile;
        C_Terrain selected_tile;
        vector<int> adj;

        bool empty;
        int x = 0;
        bool robber_placed = false;
        for (int i=0;i<board.size()-1;i++) {
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
                rand_tile = rand()%tiles_left;
                for (int i=0;i<int(sizeof(tile_num)/sizeof(int));i++){
                    if (rand_tile - tile_num[i] < 0) {
                        selected_tile = (C_Terrain)i;
                        tile_num[i]--;
                        tiles_left--;
                        break;
                    }
                    rand_tile-=tile_num[i];
                }
                tiles.push_back(new C_Tile(i,j,tile_id,selected_tile,adj));
                tile_id++;
            }
        }
        for (int i=0;i<19;i++){
            if (tiles[order[i]]->getTerrain() == Ocean)
                continue;
            if (tiles[order[i]]->getTerrain() == Desert){
                tiles[order[i]]->setNumber(0);
                if (!robber_placed) {
                    tiles[order[i]]->setRobber(true);
                    robber_tile = order[i];
                    robber_placed = true;
                }
                continue;
            }
            tiles[order[i]]->setNumber(numbers[x]);
            x++;
        }
    }

    void generateRoads() {
        int curr_id;
        vector<int> curr_adj;
        for (int i=0;i<board.size();i++) {
            for (int j=0;j<board[i].size();j++) {
                curr_id = board[i][j]->getId();
                curr_adj = board[i][j]->getAdjacent();
                for (int k=0;k<curr_adj.size();k++){
                    if(getRoadIndex(curr_id,curr_adj[k]) != -1)
                        continue;
                    roads.push_back(new C_Road(min(curr_id,curr_adj[k]),max(curr_id,curr_adj[k])));
                }
            }
        }
    }

    vector<int> calcRoadLength(){
        vector<int> length_out(4);
        vector<vector<int>> owners;
        int cell = 0;
        for (int i=0;i<4;i++){
            vector<int> owned_roads;
            owners.push_back(owned_roads);
        }
        for (int i=0;i<roads.size();i++){
            if(roads[i]->getOwner() == 0)
                continue;
            owners[roads[i]->getOwner()-1].push_back(getRoadIndex(roads[i]->getCell(1),roads[i]->getCell(2)));
        }
        for (int i=0;i<4;i++){
            length_out[i] = roadLength(i+1,owners[i]);
        }
        //cout << owners[0][0]->getOwner() << endl;

        return length_out;
    }

    void addRoad(int cell1_, int cell2_, int owner_) {
        int idx = getRoadIndex(cell1_,cell2_);
        if (idx == -1)
            return;
        if (roads[idx]->getOwner() == 0)
            roads[idx]->setOwner(owner_);
    }

    void addSettlement(int row_, int col_, int owner_) {
        if(board[row_][col_]->getType() != "O" && !isTooClose(row_,col_)) {
            board[row_][col_]->setOwner(owner_);
            board[row_][col_]->setBuilding(Settlement);
        }
    }

    void upgradeSettlement(int row_, int col_) {
        if(board[row_][col_]->getBuilding() == Settlement){
            board[row_][col_]->setBuilding(City);
        }
    }

    void moveRobber(int tile_) {
        if (robber_tile == tile_)
            return;
        tiles[robber_tile]->setRobber(false);
        tiles[tile_]->setRobber(true);
        robber_tile = tile_;
    }

    bool isTooClose(int row_, int col_) {
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

    bool hasRobber(int tile_) {
        return tiles[tile_]->hasRobber();
    }

    int getRoadIndex(int cell1, int cell2) {
        auto it = find_if(roads.begin(), roads.end(), [&,cell1,cell2](C_Road* road){return roadMatch(*road,cell1,cell2);});
        if(it==roads.end())
            return -1;
        return distance(roads.begin(),it);
    }

    vector<int> getTileIndex(int number_) {
        vector<int> tile_out;
        for (int i=0;i<tiles.size();i++) {
            if(tiles[i]->getNumber() == number_){
                tile_out.push_back(i);
            }
        }
        return tile_out;
    }

    string getFilename() {return filename;}

    C_BoardCell getBoardCell(int row_, int col_) {return *board[row_][col_];}

    tuple<int,int> getBoardCellCoords(int id_) {return make_tuple(floor(id_/numCols),id_%numCols);}

    vector<vector<int>> getResources(int number_) {
        vector<vector<int>> resources_out(5,vector<int>(4));
        C_BoardCell* cell;
        C_Tile* tile;
        int owner, tile_type;
        for (int idx: getTileIndex(number_)){
            tile = tiles[idx];
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

    vector<vector<vector<int>>> getBoard() {
        vector<vector<vector<int>>> board_out;
        vector<vector<int>> cols;
        vector<int> cell;
        for(int i=0;i<board.size();i++) {
            cols.clear();
            for(int j=0;j<board[i].size();j++) {
                cell = {board[i][j]->getOwner(),static_cast<int>(board[i][j]->getBuilding())};
                cols.push_back(cell);
            }
            board_out.push_back(cols);
        }
        return board_out;
    }

    vector<vector<int>> getTiles() {
        vector<vector<int>> tiles_out;
        vector<int> tile_data;
        for (int i=0;i<tiles.size();i++){
            tile_data = {tiles[i]->getNumber(),static_cast<int>(tiles[i]->getTerrain())};
            tiles_out.push_back(tile_data);
        }
        return tiles_out;
    }

    vector<int> getRoads() {
        vector<int> road_out;
        for (int i=0;i<roads.size();i++) {
            road_out.push_back(roads[i]->getOwner());
        }
        return road_out;
    }

    C_Tile getTile(int id_) {return *tiles[id_];}
};


PYBIND11_MODULE(catan, handle) {
    handle.doc() = "This is the module docs. Teehee";

    py::enum_<C_Building>(handle, "Building")
    .value("Undeveloped", Undeveloped)
    .value("Settlement",Settlement)
    .value("City",City)
    .export_values();

    py::enum_<C_Resource>(handle, "Resource")
    .value("Brick",Brick)
    .value("Lumber",Lumber)
    .value("Wool",Wool)
    .value("Grain",Grain)
    .value("Ore",Ore)
    .export_values();

    py::enum_<C_Terrain>(handle, "Terrain")
    .value("Hill",Hill)
    .value("Forest",Forest)
    .value("Mountian",Mountian)
    .value("Field",Field)
    .value("Pasture",Pasture)
    .value("Desert",Desert)
    .export_values();

    py::enum_<C_Harbor>(handle, "Harbor")
    .value("NoHarbor",NoHarbor)
    .value("GenericHarbor",GenericHarbor)
    .value("BrickHarbor",BrickHarbor)
    .value("LumberHarbor",LumberHarbor)
    .value("WoolHarbor",WoolHarbor)
    .value("GrainHarbor",GrainHarbor)
    .value("OreHarbor",OreHarbor)
    .export_values();

    /*
    py::enum_<>(handle, "")
    .value("",)
    .export_values();
    */

    py::class_<C_BoardCell>(handle, "BoardCell")
    .def(py::init<int,int,int,string>())
    .def("getRow", &C_BoardCell::getRow)
    .def("getCol", &C_BoardCell::getCol)
    .def("getId", &C_BoardCell::getId)
    .def("getType", &C_BoardCell::getType)
    .def("getOwner", &C_BoardCell::getOwner)
    .def("getHarbor", &C_BoardCell::getHarbor)
    .def("getBuilding", &C_BoardCell::getBuilding)
    .def("getAdjacent", &C_BoardCell::getAdjacent)
    .def("setOwner", &C_BoardCell::setOwner)
    .def("setHarbor", &C_BoardCell::setHarbor)
    .def("setBuilding", &C_BoardCell::setBuilding)
    .def("setAdjacent", &C_BoardCell::setAdjacent);

    py::class_<C_Tile>(handle, "Tile")
    .def(py::init<int,int,int,C_Terrain,vector<int>>())
    .def("hasRobber", &C_Tile::hasRobber)
    .def("getRow", &C_Tile::getRow)
    .def("getCol", &C_Tile::getCol)
    .def("getId", &C_Tile::getId)
    .def("getNumber", &C_Tile::getNumber)
    .def("getTerrain", &C_Tile::getTerrain)
    .def("getAdjacencies", &C_Tile::getAdjacencies)
    .def("setRobber", &C_Tile::setRobber)
    .def("setNumber", &C_Tile::setNumber);

    py::class_<C_Road>(handle, "Road")
    .def(py::init<int,int>())
    .def("getCell", &C_Road::getCell)
    .def("getOwner", &C_Road::getOwner)
    .def("setOwner", &C_Road::setOwner);

    py::class_<C_Board>(handle, "Board")
    .def(py::init<>())
    .def(py::init<int>())
    .def("loadBoardConfig", &C_Board::loadBoardConfig)
    .def("calcAdjacencies", &C_Board::calcAdjacencies)
    .def("generateTiles", &C_Board::generateTiles)
    .def("generateRoads", &C_Board::generateRoads)
    .def("calcRoadLength", &C_Board::calcRoadLength)
    .def("addRoad", &C_Board::addRoad)
    .def("addSettlement", &C_Board::addSettlement)
    .def("upgradeSettlement", &C_Board::upgradeSettlement)
    .def("moveRobber", &C_Board::moveRobber)
    .def("isTooClose", &C_Board::isTooClose)
    .def("hasRobber", &C_Board::hasRobber)
    .def("getFilename", &C_Board::getFilename)
    .def("getBoard", &C_Board::getBoard)
    .def("getBoardCell", &C_Board::getBoardCell)
    .def("getBoardCellCoords", &C_Board::getBoardCellCoords)
    .def("getTiles", &C_Board::getTiles)
    .def("getTile", &C_Board::getTile)
    .def("getTileIndex", &C_Board::getTileIndex)
    .def("getRoads", &C_Board::getRoads)
    .def("getRoadIndex", &C_Board::getRoadIndex)
    .def("getResources", &C_Board::getResources);

    /*
    py::class_<>(handle, "")
    .def(py::init<>())
    .def("", &);
    */
}