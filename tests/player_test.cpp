#include <catch2/catch_all.hpp>
#include <player.h>
#include <board.h>
#include <string>

using namespace std;

Game_State* game_state;

TEST_CASE("Roll Dice","[player]"){
    Player player = Player(1,5,15,5,4,game_state);
    for (int i = 0; i < 100; i++) {
        cout << player.roll_dice() << endl;
    }
    cout << "Test" << endl;
    REQUIRE(false);
}

TEST_CASE("End Turn","[player]"){
    REQUIRE(false);
}

TEST_CASE("Send Trade Offer","[player]"){
    REQUIRE(false);
}

TEST_CASE("Accept Trade Offer","[player]"){
    REQUIRE(false);
}

TEST_CASE("Play Dev Card","[player]"){
    REQUIRE(false);
}

TEST_CASE("Buy Dev Card","[player]"){
    REQUIRE(false);
}

TEST_CASE("Place Road","[player]"){
    REQUIRE(false);
}

TEST_CASE("Place Settlement","[player]"){
    REQUIRE(false);
}

TEST_CASE("Place City","[player]"){
    REQUIRE(false);
}

TEST_CASE("Call Player Action","[player]"){
    REQUIRE(false);
}

TEST_CASE("Get Functions","[player]"){
    REQUIRE(false);
}

TEST_CASE("Set Functions","[player]"){
    REQUIRE(false);
}

int main(int argc, char* argv[]) {
    Catch::Session session;
    int returnCode = session.applyCommandLine( argc, argv );
    if( returnCode != 0 )
        return returnCode;
    return session.run();
}