#include <catch2/catch_all.hpp>
#include <boardcell.h>
#include <components.h>

TEST_CASE("Get Functions", "[boardcell]") {
    BoardCell cell = BoardCell(0,0,0,"O");
    
    REQUIRE(cell.get_row() == 0);
    REQUIRE(cell.get_col() == 0);
    REQUIRE(cell.getId() == 0);
    REQUIRE(cell.getOwner() == 0);
    REQUIRE(cell.getType() == "O");
    REQUIRE(cell.getBuilding() == Undeveloped);
    REQUIRE(cell.getHarbor() == NoHarbor);
    REQUIRE(cell.getAdjacent().empty());
}

TEST_CASE("Set Functions","[boardcell]") {
    BoardCell cell = BoardCell(0,0,0,"O");
    vector<BoardCell *> test_adj{new BoardCell(1,0,11,"O"),new BoardCell(0,1,1,"O")}; 

    cell.setOwner(1);
    cell.setHarbor(GenericHarbor);
    cell.setBuilding(Settlement);
    cell.setAdjacent(test_adj);
    REQUIRE(cell.getOwner() == 1);
    REQUIRE(cell.getHarbor() == GenericHarbor);
    REQUIRE(cell.getBuilding() == Settlement);
    REQUIRE(cell.getAdjacent() == test_adj);
}

int main(int argc, char* argv[]) {
    Catch::Session session;
    int returnCode = session.applyCommandLine( argc, argv );
    if( returnCode != 0 )
        return returnCode;
    return session.run();
}