#include <catch2/catch_all.hpp>
#include <game.h>

string DATA_CONFIG_FILE = "../config_files/catan_data.txt";
string BOARD_CONFIG_FILE = "../config_files/catan_board.csv";

TEST_CASE("Initialise Game","[Game]"){
    Game* game = new Game(DATA_CONFIG_FILE,BOARD_CONFIG_FILE);
    Game_State* game_state = game->get_game_state();
    vector<Player_State> players = *game_state->players;
    REQUIRE(players.size() == 4);
    REQUIRE(players[0].victory_points == 0);
    REQUIRE(players[0].num_cards == 0);
    REQUIRE(players[0].dev_cards == 0);
    REQUIRE(players[0].knights == 0);
    REQUIRE(players[0].road_length == 0);

    Bank bank = *game_state->bank;
    REQUIRE(bank.get_num_resources() == 5);
    REQUIRE(bank.get_resource_count() == 19);

    Board board = *game_state->board;
    REQUIRE(board.getNumRows() == 6);
    REQUIRE(board.getNumCols() == 11);
}

TEST_CASE("Start Game","[Game]"){
    REQUIRE(false);
}

TEST_CASE("Roll Dice","[Game]"){
    REQUIRE(false);
}

TEST_CASE("Use Dev Card","[Game]"){
    REQUIRE(false);
}

TEST_CASE("Move Robber","[Game]"){
    REQUIRE(false);
}

TEST_CASE("Year of Plenty","[Game]"){
    REQUIRE(false);
}

TEST_CASE("Monopoly","[Game]"){
    REQUIRE(false);
}

TEST_CASE("Get Functions","[Game]"){
    Game* game = new Game(DATA_CONFIG_FILE,BOARD_CONFIG_FILE);
    REQUIRE(game->get_num_players() == 4);
}

int main(int argc, char* argv[]) {
    Catch::Session session;
    int returnCode = session.applyCommandLine( argc, argv );
    if( returnCode != 0 )
        return returnCode;
    return session.run();
}