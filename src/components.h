#ifndef CATAN_SIMULATION_COMPONENTS_H
#define CATAN_SIMULATION_COMPONENTS_H

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

#endif