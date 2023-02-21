#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include "board.h"
#include "components.h"

namespace py = pybind11;

PYBIND11_MODULE(catan, handle) {
    handle.doc() = "This is the module docs. Teehee";

    py::register_exception<exception>(handle,"File_Error");


    py::enum_<Building>(handle, "Building")
    .value("Undeveloped", Undeveloped)
    .value("Settlement",Settlement)
    .value("City",City)
    .export_values();

    py::enum_<Resource>(handle, "Resource")
    .value("Brick",Brick)
    .value("Lumber",Lumber)
    .value("Wool",Wool)
    .value("Grain",Grain)
    .value("Ore",Ore)
    .export_values();

    py::enum_<Terrain>(handle, "Terrain")
    .value("Hill",Hill)
    .value("Forest",Forest)
    .value("Mountian",Mountian)
    .value("Field",Field)
    .value("Pasture",Pasture)
    .value("Desert",Desert)
    .export_values();

    py::enum_<Harbor>(handle, "Harbor")
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

    py::class_<BoardCell>(handle, "BoardCell")
    .def(py::init<int,int,int,string>())
    .def("getRow", &BoardCell::getRow)
    .def("getCol", &BoardCell::getCol)
    .def("getId", &BoardCell::getId)
    .def("getType", &BoardCell::getType)
    .def("getOwner", &BoardCell::getOwner)
    .def("getHarbor", &BoardCell::getHarbor)
    .def("getBuilding", &BoardCell::getBuilding)
    .def("getAdjacent", &BoardCell::getAdjacent)
    .def("setOwner", &BoardCell::setOwner)
    .def("setHarbor", &BoardCell::setHarbor)
    .def("setBuilding", &BoardCell::setBuilding)
    .def("setAdjacent", &BoardCell::setAdjacent);

    py::class_<Tile>(handle, "Tile")
    .def(py::init<int,int,int,Terrain,vector<int>>())
    .def("hasRobber", &Tile::hasRobber)
    .def("getRow", &Tile::getRow)
    .def("getCol", &Tile::getCol)
    .def("getId", &Tile::getId)
    .def("getNumber", &Tile::getNumber)
    .def("getTerrain", &Tile::getTerrain)
    .def("getAdjacencies", &Tile::getAdjacencies)
    .def("setNumber", &Tile::setNumber)
    .def("setRobber", &Tile::setRobber);

    py::class_<Road>(handle, "Road")
    .def(py::init<int,int>())
    .def("getCell", &Road::getCell)
    .def("getOwner", &Road::getOwner)
    .def("setOwner", &Road::setOwner);

    py::class_<Board>(handle, "Board")
    .def(py::init<string,string>())
    .def(py::init<string,string,int>())
    .def("loadDataConfig", &Board::loadDataConfig)
    .def("loadBoardConfig", &Board::loadBoardConfig)
    .def("calcAdjacencies", &Board::calcAdjacencies)
    .def("generateTiles", &Board::generateTiles)
    .def("generateRoads", &Board::generateRoads)
    .def("addRoad", &Board::addRoad)
    .def("calcRoadLength", &Board::calcRoadLength)
    .def("addSettlement", &Board::addSettlement)
    .def("upgradeSettlement", &Board::upgradeSettlement)
    .def("hasRobber", &Board::hasRobber)
    .def("moveRobber", &Board::moveRobber)
    .def("isTooClose", &Board::isTooClose)
    .def("getBoard", &Board::getBoard)
    .def("getResources", &Board::getResources)
    .def("getBoardCell", &Board::getBoardCell)
    .def("getBoardCellCoords", &Board::getBoardCellCoords)
    .def("getTile", &Board::getTile)
    .def("getTiles", &Board::getTiles)
    .def("getTileIndex", &Board::getTileIndex)
    .def("getRoads", &Board::getRoads)
    .def("getRoadIndex", &Board::getRoadIndex)
    .def("getDataConfigFile", &Board::getDataConfigFile)
    .def("getBoardConfigFile", &Board::getBoardConfigFile)
    .def("getNumRows", &Board::getNumRows)
    .def("getNumCols", &Board::getNumCols)
    .def("getNumTiles", &Board::getNumTiles)
    .def("getMaxPlayers", &Board::getMaxPlayers)
    .def("getOceanSpot", &Board::getOceanSpot)
    .def("getPortSpot", &Board::getPortSpot)
    .def("getBuildingSpot", &Board::getBuildingSpot)
    .def("getTileOrder", &Board::getTileOrder)
    .def("getTileCount", &Board::getTileCount)
    .def("getTileNumbers", &Board::getTileNumbers);

    /*
    py::class_<>(handle, "")
    .def(py::init<>())
    .def("", &);
    */
}