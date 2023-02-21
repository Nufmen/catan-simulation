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
    int num_rows, num_cols, num_tiles, max_players, robber_tile, num_players;
    string data_config_file, board_config_file, building_spot, ocean_spot, port_spot;
    vector<vector<BoardCell *>> board;
    vector<int> tile_order, tile_count, tile_numbers;
    vector<Tile *> tiles;
    vector<Road *> roads;
    default_random_engine generator;

    int num_resources = 5;

    string removeR(string s);

    int roadLength(bool first,int owner_, int cell, vector<int> *road_input);

    bool roadMatch(Road road, int cell1, int cell2); 

    vector<int> stringListToVector(string list);

public:

    Board(string dataConfigFile_, string boardConfigFile_);

    Board(string dataConfigFile_, string boardConfigFile_, int seed_);


    void loadDataConfig();

    void loadBoardConfig();

    void calcAdjacencies();

    void generateTiles();

    void generateRoads();
    

    void addRoad(int cell1_, int cell2_, int owner_);

    vector<int> calcRoadLength();

    void addSettlement(int row_, int col_, int owner_);

    void upgradeSettlement(int row_, int col_);

    bool hasRobber(int tile_);

    void moveRobber(int tile_);

    bool isTooClose(int row_, int col_);

    vector<vector<vector<int>>> getBoard();

    vector<vector<int>> getResources(int number_);

    BoardCell getBoardCell(int row_, int col_);

    tuple<int,int> getBoardCellCoords(int id_);

    vector<int> getRoads();

    int getRoadIndex(int cell1, int cell2);

    Tile getTile(int id_);

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