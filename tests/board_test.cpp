#include <catch2/catch_all.hpp>
#include <board.h>
#include <fstream>
#include <iostream>
using namespace std;

string BOARD_CONFIG_FILE = "../config_files/catan_board.csv";
string BAD_BOARD_CONFIG_FILE = "../config_files/bad_board_1.csv";
string NO_BOARD_CONFIG_FILE = "../config_files/file_not_exist.csv";
struct Board_Config board_config;
default_random_engine generator;
Board* board;

TEST_CASE("Good Config File","[board]") {
    REQUIRE_NOTHROW(Board(board_config, generator));
}

TEST_CASE("Bad Config File","[board]") {
    Board_Config bad_board_config = board_config;
    bad_board_config.board_config_file = BAD_BOARD_CONFIG_FILE;
    REQUIRE_THROWS(Board(bad_board_config, generator));
}

TEST_CASE("No Config File","[board]") {
    Board_Config bad_board_config = board_config;
    bad_board_config.board_config_file = NO_BOARD_CONFIG_FILE;
    REQUIRE_THROWS(Board(bad_board_config, generator));
}

// Generated Board
TEST_CASE("Calc Adjacencies North Road","[board]") {
    vector<int> test{2,12,14};
    vector<BoardCell*> cells = board->get_boardcell(1,2)->get_adjacent();
    vector<int> cell_ids;
    for(BoardCell* c: cells) {
        cell_ids.push_back(c->get_id());
    }
    REQUIRE(cell_ids == test);
}

TEST_CASE("Calc Adjacencies South Road","[board]") {
    vector<int> test{25,13,15};
    vector<BoardCell*> cells = board->get_boardcell(1,3)->get_adjacent();
    vector<int> cell_ids;
    for(BoardCell* c: cells) {
        cell_ids.push_back(c->get_id());
    }
    REQUIRE(cell_ids == test);
}

TEST_CASE("Generate Harbors","[board]") {
    vector<int> count{0,0,0,0,0,0};
    vector<int> count_match{8,2,2,2,2,2};
    for (int i=0;i<board->get_num_rows();i++) {
        for (int j=0;j<board->get_num_cols();j++){
            if(board->get_boardcell(i,j)->get_harbor() == NoHarbor)
                continue;
            count[(int)board->get_boardcell(i,j)->get_harbor()]++;
        }
    }
    REQUIRE(count == count_match);
}

TEST_CASE("Generate Tiles Correct Numbers","[board]") {
    vector<int> tile_nums, test{0,2,3,3,4,4,5,5,6,6,8,8,9,9,10,10,11,11,12};
    for(int i=0;i<board_config.num_tiles;i++) {
        tile_nums.push_back(board->get_tile(i)->get_number());
    }
    sort(tile_nums.begin(),tile_nums.end());
    REQUIRE(tile_nums == test);
}

TEST_CASE("Generate Tiles Correct Adjacencies","[board]") {
    vector<int> tile_ids, test{12,13,14,23,24,25};
    vector<BoardCell*> cells = board->get_tile(3)->get_adjacencies();
    for(BoardCell* c:cells) {
        tile_ids.push_back(c->get_id());
    }
    REQUIRE(tile_ids == test);
}

TEST_CASE("Generate Tiles Correct Number of Terrains","[board]") {
    vector<int> tile_terrain{0,0,0,0,0,0,0}, test{0,1,3,4,4,4,3};
    for(int i=0;i<board_config.num_tiles;i++) {
        tile_terrain[(int)board->get_tile(i)->get_terrain()]++;
    }
    REQUIRE(tile_terrain == test);
}

TEST_CASE("Generate Tiles Robber On Desert","[board]") {
    int id;
    for(int i=0;i<board_config.num_tiles;i++){
        if(board->get_tile(i)->get_terrain() == Desert) {
            id = i;
            break;
        }
    }
    REQUIRE(board->get_tile(id)->has_robber());
}

TEST_CASE("Generate Roads Correct Number","[board]") {
    REQUIRE(board->get_roads().size() == 72);
}

TEST_CASE("Generate Roads Are Adjacent","[board]") {
    Road* road;
    bool test = true;
    for(int i=0;i<72;i++){
        road = board->get_road(i);
        test = true;
        for(BoardCell* c: road->get_cells()[0]->get_adjacent()){
            if(road->get_cells()[1]->get_id() == c->get_id()){
                test = false;
                break;
            }
        }
        if(test)
            break;
    }
    REQUIRE(!test);
}

//Calculate Road Length
TEST_CASE("Calc Road Lengths No Roads","[board]") {
    vector<int> test{0,0,0,0};
    REQUIRE(board->calc_road_length() == test);
}

TEST_CASE("Calc Road Lengths One Road","[board]") {
    vector<int> test{1,0,0,0};
    board->add_road(28,1);
    REQUIRE(board->calc_road_length() == test);
}

TEST_CASE("Calc Road Lengths Two Seperate Roads","[board]") {
    vector<int> test{1,0,0,0};
    board->add_road(42,1);
    REQUIRE(board->calc_road_length() == test);
}

TEST_CASE("Calc Road Lengths Two Connected Roads","[board]") {
    vector<int> test{2,0,0,0};
    board->add_road(27,1);
    REQUIRE(board->calc_road_length() == test);
}

TEST_CASE("Calc Road Lengths Loop","[board]") {
    vector<int> test{6,0,0,0};
    board->add_road(29,1);
    board->add_road(26,1);
    board->add_road(44,1);
    REQUIRE(board->calc_road_length() == test);
}

TEST_CASE("Calc Road Lengths Loop Plus One","[board]") {
    vector<int> test{7,0,0,0};
    board->add_road(13,1);
    REQUIRE(board->calc_road_length() == test);
}

TEST_CASE("Calc Road Lengths Loop Plus Two Seperate","[board]") {
    vector<int> test{7,0,0,0};
    board->add_road(30,1);
    REQUIRE(board->calc_road_length() == test);
}

TEST_CASE("Calc Road Lengths Figure 8","[board]") {
    vector<int> test{11,0,0,0};
    board->add_road(14,1);
    board->add_road(15,1);
    board->add_road(16,1);
    REQUIRE(board->calc_road_length() == test);
}

TEST_CASE("Calc Road Lengths Figure 8 Plus One","[board]") {
    vector<int> test{11,0,0,0};
    board->add_road(12,1);
    REQUIRE(board->calc_road_length() == test);
}

TEST_CASE("Calc Road Lengths Opponent Settlement On Road","[board]") {
    board->add_settlement(2,2,2);
    vector<int> test{10,0,0,0};
    REQUIRE(board->calc_road_length() == test);
}

TEST_CASE("Calc Road Lengths Alternate Roads","[board]") {
    vector<int> test{10,1,1,0};
    board->add_road(59,2);
    board->add_road(60,3);
    board->add_road(62,2);
    board->add_road(63,3);
    REQUIRE(board->calc_road_length() == test);
}

//Add/Upgrade Pieces
TEST_CASE("Add Road Owner Updated","[board]") {
    board = new Board(board_config, generator);
    board->add_road(30,1);
    REQUIRE(board->get_road(30)->get_owner() == 1);
}

TEST_CASE("Add Road No New Owner Updated","[board]") {
    board->add_road(30,2);
    REQUIRE(board->get_road(30)->get_owner() == 1);
}

TEST_CASE("No Settlement Building Default","[board]") {
    REQUIRE(board->get_boardcell(2,3)->get_building() == Undeveloped);
}

TEST_CASE("Add Settlement Owner Updated","[board]") {
    board->add_settlement(2,3,1);
    REQUIRE(board->get_boardcell(2,3)->get_owner() == 1);
}

TEST_CASE("Add Settlement No New Owner Updated","[board]") {
    board->add_settlement(2,3,2);
    REQUIRE(board->get_boardcell(2,3)->get_owner() == 1);
}


TEST_CASE("Add Settlement Building Updated","[board]") {
    REQUIRE(board->get_boardcell(2,3)->get_building() == Settlement);
}

TEST_CASE("Upgrade To City Building Updated","[board]") {
    board->upgrade_settlement(2,3);
    REQUIRE(board->get_boardcell(2,3)->get_building() == City);
}

//Move Robber
TEST_CASE("Only One Robber","[board]") {
    int count = 0;
    for(int i=0;i<board_config.num_tiles;i++) {
        if(board->get_tile(i)->has_robber())
            count++;
    }
    REQUIRE(count == 1);
}

TEST_CASE("Robber Starts on Desset","[board]") {
    Terrain terrain;
    for(int i=0;i<board_config.num_tiles;i++) {
        if(board->get_tile(i)->has_robber()){
            terrain = board->get_tile(i)->get_terrain();
            break;
        }
    }
    REQUIRE(terrain == Desert);
}

TEST_CASE("Robber Can't Move To Same Position","[board]") {
    vector<int> test{0,1,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18};
    board->move_robber(2);
    REQUIRE(board->robber_options() == test);
}

TEST_CASE("Robber Move To Tile","[board]") {
    board->move_robber(4);
    REQUIRE(board->get_tile(4)->has_robber());
}

TEST_CASE("Robber Previous Tile Has No Robber","[board]") {
    REQUIRE(!board->get_tile(2)->has_robber());
}

// Settlement Options
TEST_CASE("No Settlements Placed Start","[board]") {
    board = new Board(board_config, generator);
    REQUIRE(board->settlement_options(1,false) == vector<int>{});
}

TEST_CASE("First Settlement Spots Start","[board]") {
    vector<int> test_spots{2, 3, 4, 5, 6, 7, 8, 12, 13, 14, 15, 16, 17, 18, 19, 20, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 45, 46, 47, 48, 49, 50, 51, 52, 53, 57, 58, 59, 60, 61, 62, 63};
    REQUIRE(board->settlement_options(1,true) == test_spots);
}

TEST_CASE("Second Settlement Spots Start","[board]") {
    board->add_settlement(2,3,1);
    vector<int> test_spots{2, 3, 4, 5, 6, 7, 8, 12, 13, 15, 16, 17, 18, 19, 20, 22, 23, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 45, 46, 47, 48, 49, 50, 51, 52, 53, 57, 58, 59, 60, 61, 62, 63};
    REQUIRE(board->settlement_options(2,true) == test_spots);
}

TEST_CASE("No Settlement Spots","[board]") {
    vector<int> test_spots{};
    REQUIRE(board->settlement_options(1,false) == test_spots);
}

TEST_CASE("One Settlement Spot","[board]") {
    board->add_road(28, 1);
    board->add_road(30, 1);
    vector<int> test_spots{27};
    REQUIRE(board->settlement_options(1,false) == test_spots);
}

TEST_CASE("Multiple Settlement Spots","[board]") {
    board->add_road(31, 1);
    vector<int> test_spots{27, 28};
    REQUIRE(board->settlement_options(1,false) == test_spots);
}


// Road Options
TEST_CASE("First Settlement Roads","[board]") {
    board = new Board(board_config, generator);
    board->add_settlement(2,3,1);
    REQUIRE(board->road_options(1,true).size() == 3);
}

TEST_CASE("First Settlement Roads Initial Coordinate","[board]") {
    REQUIRE(board->road_options(1,true)[0] == 13);
}

TEST_CASE("First Road","[board]") {
    board->add_road(28,1);
    vector<Road_Data> test_spots{};
    REQUIRE(board->road_options(1,true).size() == 0);
}

TEST_CASE("Second Settlement Roads","[board]") {
    board->add_settlement(2,5,1);
    REQUIRE(board->road_options(1,true).size() == 3);
}

TEST_CASE("Second Settlement Roads Initial Coordinate","[board]") {
    REQUIRE(board->road_options(1,true)[0] == 16);
}

TEST_CASE("Second Road","[board]") {
    board->add_road(31,1);
    REQUIRE(board->road_options(1,true).size() == 0);
}

TEST_CASE("Placing Roads After Start","[board]") {
    REQUIRE(board->road_options(1,false).size() == 7);
}

TEST_CASE("Connecting 2 Roads","[board]") {
    board->add_road(30,1);
    REQUIRE(board->road_options(1,false).size() == 6);
}

TEST_CASE("Expanding Road","[board]") {
    board->add_road(33,1);
    REQUIRE(board->road_options(1,false).size() == 7);
}

TEST_CASE("Place Opponent Settlement on Road","[board]") {
    board->add_settlement(2,7,2);
    REQUIRE(board->road_options(1,false).size() == 5);
}

// City Options
TEST_CASE("No City Spots","[board]") {
    board = new Board(board_config, generator);
    vector<int> test{};
    REQUIRE(board->city_options(1) == test);
}

TEST_CASE("Once City Spot","[board]") {
    vector<int> test{24};
    board->add_settlement(2,2,1);
    REQUIRE(board->city_options(1) == test);
}

TEST_CASE("Multiple City Spots","[board]") {
    vector<int> test{26,24};
    board->add_settlement(2,4,1);
    REQUIRE(board->city_options(1) == test);
}

TEST_CASE("One City, No Settlements","[board]") {
    board = new Board(board_config, generator);
    vector<int> test{};
    board->add_settlement(2,2,1);
    board->upgrade_settlement(2,2);
    REQUIRE(board->city_options(1) == test);
}

TEST_CASE("One City and Settlements","[board]") {
    vector<int> test{26};
    board->add_settlement(2,4,1);
    REQUIRE(board->city_options(1) == test);
}

TEST_CASE("Opponent City Spot","[board]") {
    vector<int> test{};
    REQUIRE(board->city_options(2) == test);
}

// Is Too Close
TEST_CASE("No Settlements","[board]") {
    board = new Board(board_config, generator);
    REQUIRE(!board->is_too_close(2,2));
}

TEST_CASE("One Settlement Too Close","[board]") {
    board->add_settlement(2,3,2);
    REQUIRE(board->is_too_close(2,2));
}

TEST_CASE("One Settlement Far Enough","[board]") {
    REQUIRE(!board->is_too_close(1,2));
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
    board_config.tile_count = {0,1,3,4,4,4,3};
    board_config.tile_numbers = {5,2,6,3,8,10,9,12,11,4,8,10,9,4,5,6,3,11};
    board_config.harbor_count = {4,1,1,1,1,1,0};
    generator.seed(time(0));
    board = new Board(board_config, generator);
    Catch::Session session;
    int returnCode = session.applyCommandLine( argc, argv );
    if( returnCode != 0 )
        return returnCode;
    return session.run();
}