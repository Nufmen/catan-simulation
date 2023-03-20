#include <catch2/catch_all.hpp>
#include <tile.h>
#include <boardcell.h>
#include <components.h>

vector<BoardCell *> adj;

TEST_CASE("Has Robber","[tile]"){
    Tile tile = Tile(0,0,0,Hill,adj);
    REQUIRE(!tile.hasRobber());
    tile.setRobber(true);
    REQUIRE(tile.hasRobber());
}

TEST_CASE("Get Functions","[tile]"){
    Tile tile = Tile(0,0,0,Hill,adj);
    REQUIRE(tile.getRow() == 0);
    REQUIRE(tile.getCol() == 0);
    REQUIRE(tile.getId() == 0);
    REQUIRE(tile.getNumber() == 0);
    REQUIRE(tile.getAdjacencies() == adj);
    REQUIRE(tile.getTerrain() == Hill);
}

TEST_CASE("Set Functions","[tile]"){
    Tile tile = Tile(0,0,0,Hill,adj);
    tile.setNumber(4);
    tile.setRobber(true);
    REQUIRE(tile.hasRobber());
    REQUIRE(tile.getNumber() == 4);
}

int main(int argc, char* argv[]) {
    for (int i=0; i<2; i++){
        for (int j=0; j<3; j++){
            adj.push_back(new BoardCell(i,j,i*11+j,"B"));
        }
    }
    Catch::Session session;
    int returnCode = session.applyCommandLine( argc, argv );
    if( returnCode != 0 )
        return returnCode;
    return session.run();
}