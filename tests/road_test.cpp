#include <catch2/catch_all.hpp>
#include <road.h>
#include <boardcell.h>

TEST_CASE("This is a Test","[test]"){
    REQUIRE(true);
}

TEST_CASE("Get Functions","[road]"){
    vector<BoardCell *> cells{new BoardCell(0,0,0,"B"),new BoardCell(0,1,1,"B")};
    Road road = Road(cells[0],cells[1]);
    
    REQUIRE(road.getCells() == cells);
    REQUIRE(road.getOwner() == 0);
}

TEST_CASE("Set Functions","[road]") {
    vector<BoardCell *> cells{new BoardCell(0,0,0,"B"),new BoardCell(0,1,1,"B")};
    Road road = Road(cells[0],cells[1]);

    road.setOwner(1);
    REQUIRE(road.getOwner() == 1);
}

int main(int argc, char* argv[]) {
    Catch::Session session;
    int returnCode = session.applyCommandLine( argc, argv );
    if( returnCode != 0 )
        return returnCode;
    return session.run();
}