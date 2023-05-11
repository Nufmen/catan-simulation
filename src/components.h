#ifndef CATAN_SIMULATION_COMPONENTS_H
#define CATAN_SIMULATION_COMPONENTS_H

#include <vector>
#include <string>

using namespace std;

enum Building {
    Undeveloped,
    Settlement,
    City
};

enum Resource {
    Brick,
    Lumber,
    Wool,
    Grain,
    Ore
};

enum Terrain {
    Ocean,
    Desert,
    Hill,
    Forest,
    Pasture,
    Field,
    Mountian
};

enum Harbor {
    GenericHarbor,
    BrickHarbor,
    LumberHarbor,
    WoolHarbor,
    GrainHarbor,
    OreHarbor,
    NoHarbor
};

enum Development_Card {
    Knight,
    VictoryPoint,
    RoadBuilding,
    YearOfPlenty,
    Monopoly,
    NoDevCard
};

enum Player_Move {
    RollDice,
    EndTurn,
    PlaceRoad,
    PlaceSettlement,
    PlaceCity,
    BuyDevCard,
    UseDevCard,
    TradeOffer,
    TradeBank,
    NoMove
};

struct Board_Config {
    int num_rows, num_cols, num_tiles, max_players, robber_tile, num_players, num_resources;
    string board_config_file, building_spot, ocean_spot, port_spot;
    vector<int> tile_order, tile_count, tile_numbers, harbor_count;
};

struct Road_Data {
    int row1, col1, row2, col2;
};

struct Coordinate_Data {
    int row, col;
};

struct Trade {
    vector<Resource> offer, request;
    Harbor harbor;
};

#endif