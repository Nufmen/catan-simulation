#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>

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

    C_Harbor getHarbor() {return harbor;}

    int getOwner() {return owner;}

    C_Building getBuilding() {return building;}

    string getType() {return type;}

    void setOwner(int owner_) {owner = owner_;}
};

class C_Tile {
    int row, col;
    C_Terrain terrain;

    public:
    C_Tile(int row_, int col_, C_Terrain terrain_) {
        row = row_;
        col = col_;
        terrain = terrain_;
    }

    int getRow() {return row;}

    int getCol() {return col;}

    C_Terrain getTerrain() {return terrain;}
};

class C_Road {
    int cell1, cell2, owner;

    public:
    C_Road(int cell1_, int cell2_) {
        cell1 = cell1_;
        cell2 = cell2_;
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
};

class C_Board {
    string filename;
    vector<vector<C_BoardCell *>> board;

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

    string getFilename() {return filename;}

    C_BoardCell* getBoardCell(int row, int col) {return board[row][col];}

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
    .def("getHarbor", &C_BoardCell::getHarbor)
    .def("getOwner", &C_BoardCell::getOwner)
    .def("getBuilding", &C_BoardCell::getBuilding)
    .def("getType", &C_BoardCell::getType)
    .def("setOwner", &C_BoardCell::setOwner);

    py::class_<C_Tile>(handle, "Tile")
    .def(py::init<int,int,C_Terrain>())
    .def("getRow", &C_Tile::getRow)
    .def("getCol", &C_Tile::getCol)
    .def("getTerrain", &C_Tile::getTerrain);

    py::class_<C_Road>(handle, "Road")
    .def(py::init<int,int>())
    .def("getCell", &C_Road::getCell)
    .def("getOwner", &C_Road::getOwner);

    py::class_<C_Board>(handle, "Board")
    .def(py::init<>())
    .def("loadBoardConfig", &C_Board::loadBoardConfig)
    .def("getFilename", &C_Board::getFilename)
    .def("getBoard", &C_Board::getBoard)
    .def("getBoardCell", &C_Board::getBoardCell);

    /*
    py::class_<>(handle, "")
    .def(py::init<>())
    .def("", &);
    */
}