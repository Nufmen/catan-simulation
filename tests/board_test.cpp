#include <catch2/catch_all.hpp>
#include <board.h>
using namespace std;

string BOARD_CONFIG_FILE = "../config_files/catan_board.csv";
struct Board_Config board_config;

TEST_CASE("This is a Test","[test]") {
    REQUIRE(true);
}

TEST_CASE("Load Board Config","[board]") {
    Board board = Board(board_config,1);
    REQUIRE(board.getBoardCell(0,0)->getType() == "O");
    REQUIRE(board.getBoardCell(1,1)->getType() == "P1");
    REQUIRE(board.getBoardCell(2,2)->getType() == "B");
    REQUIRE(board.getBoardCell(2,10)->getType() == "P5");
}

TEST_CASE("Bad Config Files","[board]") {
    //REQUIRE(false);
}

bool adjacency_test(vector<BoardCell *> cells, vector<int> adj){
    if (cells.size() != adj.size())
        return false;
    for (int i = 0; i < adj.size(); i++) {
        if(cells[i]->getId() != adj[i]){
            return false;
        }
    }
    return true;
}

TEST_CASE("Calc Adjacencies") {
    Board board = Board(board_config,1);
    int sum = 0;
    for (int i = 0; i < board.getNumRows(); i++) {
        for (int j = 0; j < board.getNumCols(); j++) {
            sum += board.getBoardCell(i,j)->getAdjacent().size();
        }
    }
    REQUIRE(int(sum/2) == 72);

    REQUIRE(adjacency_test(board.getBoardCell(0,0)->getAdjacent(),vector<int>{}));
    REQUIRE(adjacency_test(board.getBoardCell(2,4)->getAdjacent(),vector<int>{37,25,27}));
    REQUIRE(adjacency_test(board.getBoardCell(2,1)->getAdjacent(),vector<int>{12,22,24}));
    REQUIRE(adjacency_test(board.getBoardCell(2,2)->getAdjacent(),vector<int>{35,23,25}));
    REQUIRE(adjacency_test(board.getBoardCell(0,2)->getAdjacent(),vector<int>{13,3}));
    REQUIRE(adjacency_test(board.getBoardCell(0,3)->getAdjacent(),vector<int>{2,4}));
    REQUIRE(adjacency_test(board.getBoardCell(5,2)->getAdjacent(),vector<int>{46,58}));
    REQUIRE(adjacency_test(board.getBoardCell(5,3)->getAdjacent(),vector<int>{57,59}));
    REQUIRE(adjacency_test(board.getBoardCell(2,0)->getAdjacent(),vector<int>{33,23}));
    REQUIRE(adjacency_test(board.getBoardCell(3,0)->getAdjacent(),vector<int>{22,34}));
    REQUIRE(adjacency_test(board.getBoardCell(2,10)->getAdjacent(),vector<int>{43,31}));
    REQUIRE(adjacency_test(board.getBoardCell(3,10)->getAdjacent(),vector<int>{32,42}));

}

bool tiles_test(vector<BoardCell *> adj, vector<int> idx) {
    if (adj.size() != idx.size())
        return false;
    for (int i=0; i<idx.size();i++) {
        if (adj[i]->getId() != idx[i])
            return false;
    }
    return true;
}

TEST_CASE("Generate Tiles","[board]") {
    Board board = Board(board_config,1);
    vector<int> tile_count(7);
    for(vector<int> tile : board.getTiles()) {
        tile_count[tile[1]]++;
    }
    REQUIRE(tile_count == vector<int>{0,1,3,4,3,4,4});
    REQUIRE(board.getTile(1)->hasRobber());
    REQUIRE(tiles_test(board.getTile(7)->getAdjacencies(),vector<int>{22,23,24,33,34,35}));
    REQUIRE(tiles_test(board.getTile(3)->getAdjacencies(),vector<int>{12,13,14,23,24,25}));
}

TEST_CASE("Generate Roads","[board]") {
    Board board = Board(board_config,1);
    REQUIRE(board.getRoads()[0] == 0);
    REQUIRE(board.getRoads().size() == 72);
}

TEST_CASE("Calculate Road Lengths","[board]") {
    Board board = Board(board_config,1);
    //REQUIRE(false);
}

TEST_CASE("Add Road","[board]") {
    Board board = Board(board_config,1);
    board.addRoad(board.getBoardCell(0,0),board.getBoardCell(0,1),1);
    REQUIRE(board.getRoads().size() == 72);
    REQUIRE(board.getRoads()[0] == 0);
    board.addRoad(board.getBoardCell(0,2),board.getBoardCell(1,2),1);
    REQUIRE(board.getRoads()[0] == 1);
    board.addRoad(board.getBoardCell(0,2),board.getBoardCell(1,2),2);
    REQUIRE(board.getRoads()[0] == 1);
}

TEST_CASE("Add Settlement","[board]") {
    Board board = Board(board_config,1);
    REQUIRE(board.getBoardCell(2,3)->getBuilding() == Undeveloped);
    REQUIRE(board.getBoardCell(2,3)->getOwner() == 0);
    board.addSettlement(2,3,4);
    REQUIRE(board.getBoardCell(2,3)->getBuilding() == Settlement);
    REQUIRE(board.getBoardCell(2,3)->getOwner() == 4);
    board.addSettlement(2,2,4);
    REQUIRE(board.getBoardCell(2,2)->getBuilding() == Undeveloped);
    REQUIRE(board.getBoardCell(2,2)->getOwner() == 0);
    board.addSettlement(2,4,2);
    REQUIRE(board.getBoardCell(2,4)->getBuilding() == Undeveloped);
    REQUIRE(board.getBoardCell(2,4)->getOwner() == 0);
    board.addSettlement(1,3,3);
    REQUIRE(board.getBoardCell(1,3)->getBuilding() == Undeveloped);
    REQUIRE(board.getBoardCell(1,3)->getOwner() == 0);
    board.addSettlement(3,3,1);
    REQUIRE(board.getBoardCell(3,3)->getBuilding() == Settlement);
    REQUIRE(board.getBoardCell(3,3)->getOwner() == 1);
    board.addSettlement(2,1,4);
    REQUIRE(board.getBoardCell(2,1)->getBuilding() == Settlement);
    REQUIRE(board.getBoardCell(2,1)->getOwner() == 4);
    board.addSettlement(2,5,2);
    REQUIRE(board.getBoardCell(2,5)->getBuilding() == Settlement);
    REQUIRE(board.getBoardCell(2,5)->getOwner() == 2);
}

TEST_CASE("Upgrade Settlement","[board]") {
    Board board = Board(board_config,1);
    REQUIRE(board.getBoardCell(0,6)->getBuilding() == Undeveloped);
    board.addSettlement(0,6,1);
    REQUIRE(board.getBoardCell(0,6)->getBuilding() == Settlement);
    board.upgradeSettlement(0,6);
    REQUIRE(board.getBoardCell(0,6)->getBuilding() == City);
}

TEST_CASE("Move Robber","[board]") {
    Board board = Board(board_config,1);
    REQUIRE(board.getTile(1)->hasRobber());
    board.moveRobber(2);
    REQUIRE(!board.getTile(1)->hasRobber());
    REQUIRE(board.getTile(2)->hasRobber());
    board.moveRobber(2);
    REQUIRE(board.getTile(2)->hasRobber());
    board.moveRobber(8);
    REQUIRE(!board.getTile(2)->hasRobber());
    REQUIRE(board.getTile(8)->hasRobber());
    //REQUIRE(false); // REMOVE This, ADD get tile
}

TEST_CASE("Is Too Close","[board]") {
    Board board = Board(board_config,1);
    board.addSettlement(3,3,4);
    REQUIRE(board.isTooClose(3,3));
    REQUIRE(!board.isTooClose(2,3));
    REQUIRE(board.isTooClose(4,3));
    REQUIRE(board.isTooClose(3,2));
    REQUIRE(board.isTooClose(3,4));
}

TEST_CASE("Get Game State","[board]") {
    Board board = Board(board_config,1);
    //REQUIRE(false);
    /*
    Get Roads
    Get Resources
    Get Tile Index
    Get Tiles
    Get Board
    */
}

TEST_CASE("Get Functions","[board]") {
    Board board = Board(board_config,1);
    REQUIRE(board.getBoardConfigFile() == BOARD_CONFIG_FILE);
    REQUIRE(board.getBoardCell(0,2)->getId() == 2);
    //REQUIRE(false); // Board Cell Coords
}

int main(int argc, char* argv[]) {
    board_config.num_rows = 6;
    board_config.num_cols = 11;
    board_config.num_tiles = 19;
    board_config.num_players = 4;
    board_config.max_players = 4;
    board_config.robber_tile = 0;
    board_config.board_config_file = BOARD_CONFIG_FILE;
    board_config.ocean_spot = "O";
    board_config.port_spot = "P";
    board_config.building_spot = "B";
    board_config.tile_order = {0,3,7,12,16,17,18,15,11,6,2,1,4,8,13,14,10,5,9};
    board_config.tile_count = {0,1,3,4,3,4,4};
    board_config.tile_numbers = {5,2,6,3,8,10,9,12,11,4,8,10,9,4,5,6,3,11};
    Catch::Session session;
    int returnCode = session.applyCommandLine( argc, argv );
    if( returnCode != 0 )
        return returnCode;
    return session.run();
}