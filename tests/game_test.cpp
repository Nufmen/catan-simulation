#include <catch2/catch_all.hpp>
#include <game.h>

string DATA_CONFIG_FILE = "../config_files/catan_data.txt";
string BOARD_CONFIG_FILE = "../config_files/catan_board.csv";

TEST_CASE("","[game]") {
    REQUIRE(false);
}

int main(int argc, char* argv[]) {
    Catch::Session session;
    int returnCode = session.applyCommandLine( argc, argv );
    if( returnCode != 0 )
        return returnCode;
    return session.run();
}