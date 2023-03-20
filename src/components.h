#ifndef CATAN_SIMULATION_COMPONENTS_H
#define CATAN_SIMULATION_COMPONENTS_H

#include <vector>
#include <string>

using namespace std;

enum Building {
    Undeveloped = 0,
    Settlement = 1,
    City = 2
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
    Mountian,
    Field,
    Pasture
};

enum Harbor {
    NoHarbor,
    GenericHarbor,
    BrickHarbor,
    LumberHarbor,
    WoolHarbor,
    GrainHarbor,
    OreHarbor
};

enum Development_Card {
    Knight,
    VictoryPoint,
    RoadBuilding,
    YearOfPlenty,
    Monopoly
};

struct Board_Config {
    int num_rows, num_cols, num_tiles, max_players, robber_tile, num_players;
    string board_config_file, building_spot, ocean_spot, port_spot;
    vector<int> tile_order, tile_count, tile_numbers;
};

#endif