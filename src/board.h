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
#include <vector>

#include "boardcell.h"
#include "components.h"
#include "road.h"
#include "tile.h"

using namespace std;

class Board {
    struct Board_Config board_config;
    vector<vector<BoardCell *>> board;
    vector<Tile *> tiles;
    vector<Road *> roads;
    default_random_engine generator;

    int num_resources = 5;

    string removeR(string s);

    int roadLength(bool first,int owner_, BoardCell* cell, vector<int> *road_input);

    bool roadMatch(Road road, BoardCell* cell1, BoardCell* cell2); 

    vector<int> stringListToVector(string list);

public:

    Board(Board_Config board_config);

    Board(Board_Config board_config, int seed_);
    

    void loadBoardConfig();

    void calcAdjacencies();

    void generateTiles();

    void generateRoads();
    

    void addRoad(BoardCell* cell1_, BoardCell*   cell2_, int owner_);

    vector<int> calcRoadLength();

    void addSettlement(int row_, int col_, int owner_);

    void upgradeSettlement(int row_, int col_);

    void moveRobber(int tile_);

    bool isTooClose(int row_, int col_);

    vector<vector<vector<int>>> getBoard();

    vector<vector<int>> getResources(int number_);

    BoardCell* getBoardCell(int row_, int col_);

    tuple<int,int> getBoardCellCoords(int id_);

    vector<int> getRoads();

    int getRoadIndex(BoardCell* cell1, BoardCell* cell2);

    Tile* getTile(int id_);

    vector<vector<int>> getTiles();

    vector<int> getTileIndex(int number_);

    string getDataConfigFile();
    
    string getBoardConfigFile();
    
    int getNumRows();
    
    int getNumCols();
    
    int getNumTiles();
    
    int getMaxPlayers();
    
    string getOceanSpot();
    
    string getPortSpot();
    
    string getBuildingSpot();
    
    vector<int> getTileOrder();
    
    vector<int> getTileCount();
    
    vector<int> getTileNumbers();
};

#endif