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
    Hill,
    Forest,
    Mountian,
    Field,
    Pasture,
    Desert,
    Ocean
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
    C_Tile(int row_, int col_, int id_, int number_, C_Terrain terrain_, vector<int> adjacencies_) {
        row = row_;
        col = col_;
        id = id_;
        number = number_;
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
    string filename;
    vector<vector<C_BoardCell *>> board;
    vector<C_Road *> roads;

    bool roadMatch(C_Road road, int cell1, int cell2) {
        return road.getCell(1)==min(cell1,cell2) && road.getCell(2)==max(cell1,cell2);
    }

    int getRoadIndex(int cell1, int cell2) {
        auto it = find_if(roads.begin(), roads.end(), [&,cell1,cell2](C_Road* road){return roadMatch(*road,cell1,cell2);});
        if(it==roads.end())
            return -1;
        return distance(roads.begin(),it);
    }

    public:
    C_Board() {
        filename = "";
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

    void addRoad(int cell1_, int cell2_, int owner_) {
        int idx = getRoadIndex(cell1_,cell2_);
        if (idx == -1)
            return;
        if (roads[idx]->getOwner() == 0)
            roads[idx]->setOwner(owner_);
    }

    bool isValidRoad() {
        return false;
    }

    string getFilename() {return filename;}

    C_BoardCell getBoardCell(int row, int col) {return *board[row][col];}

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

    vector<int> getRoads() {
        vector<int> road_out;
        for (int i=0;i<roads.size();i++) {
            road_out.push_back(roads[i]->getOwner());
        }
        return road_out;
    }
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
    .def(py::init<int,int,int,int,C_Terrain,vector<int>>())
    .def("hasRobber", &C_Tile::hasRobber)
    .def("getRow", &C_Tile::getRow)
    .def("getCol", &C_Tile::getCol)
    .def("getId", &C_Tile::getId)
    .def("getNumber", &C_Tile::getNumber)
    .def("getTerrain", &C_Tile::getTerrain)
    .def("getAdjacencies", &C_Tile::getAdjacencies)
    .def("setRobber", &C_Tile::setRobber);

    py::class_<C_Road>(handle, "Road")
    .def(py::init<int,int>())
    .def("getCell", &C_Road::getCell)
    .def("getOwner", &C_Road::getOwner)
    .def("setOwner", &C_Road::setOwner);

    py::class_<C_Board>(handle, "Board")
    .def(py::init<>())
    .def("loadBoardConfig", &C_Board::loadBoardConfig)
    .def("calcAdjacencies", &C_Board::calcAdjacencies)
    .def("generateRoads", &C_Board::generateRoads)
    .def("addRoad", &C_Board::addRoad)
    .def("getFilename", &C_Board::getFilename)
    .def("getBoard", &C_Board::getBoard)
    .def("getBoardCell", &C_Board::getBoardCell)
    .def("getRoads", &C_Board::getRoads);

    /*
    py::class_<>(handle, "")
    .def(py::init<>())
    .def("", &);
    */
}