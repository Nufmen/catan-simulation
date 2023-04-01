#include <catch2/catch_all.hpp>
#include <player.h>
#include <board.h>
#include <string>

using namespace std;

Player* player;
default_random_engine generator;

// Default Bot Tests
TEST_CASE("No Move","[player]"){
    REQUIRE(!player->is_valid_action(NoMove));
}

TEST_CASE("Roll Dice, Hasn't Rolled","[player]"){
    player->set_rolled_dice(false);
    REQUIRE(player->is_valid_action(RollDice));
}

TEST_CASE("Roll Dice, Has Rolled","[player]"){
    player->set_rolled_dice(true);
    REQUIRE(!player->is_valid_action(RollDice));
}

TEST_CASE("End Turn, Hasn't Rolled","[player]"){
    player->set_rolled_dice(false);
    REQUIRE(!player->is_valid_action(EndTurn));
}

TEST_CASE("End Turn, Has Rolled","[player]"){
    player->set_rolled_dice(true);
    REQUIRE(player->is_valid_action(EndTurn));
}

TEST_CASE("Place Road, Not Enough Resources","[player]"){
    REQUIRE(!player->is_valid_action(PlaceRoad));
}

TEST_CASE("Place Road, Enough Resources","[player]"){
    player->add_resources(vector<Resource>{Brick,Lumber});
    REQUIRE(player->is_valid_action(PlaceRoad));
}

TEST_CASE("Place Settlement, Not Enough Resources","[player]"){
    REQUIRE(!player->is_valid_action(PlaceSettlement));
}

TEST_CASE("Place Settlement, Enough Resources","[player]"){
    player->add_resources(vector<Resource>{Wool,Grain});
    REQUIRE(player->is_valid_action(PlaceSettlement));
}

TEST_CASE("Place City, Not Enough Resources","[player]"){
    REQUIRE(!player->is_valid_action(PlaceCity));
}

TEST_CASE("Place City, Enough Resources","[player]"){
    player->add_resources(vector<Resource>{Grain,Ore,Ore,Ore});
    REQUIRE(player->is_valid_action(PlaceCity));
}

TEST_CASE("Buy Dev Card, Not Enough Resources","[player]"){
    player->remove_resources(vector<Resource>{Brick,Lumber,Wool,Grain,Grain,Ore,Ore,Ore});
    REQUIRE(!player->is_valid_action(BuyDevCard));
}

TEST_CASE("Buy Dev Card, Enough Resources","[player]"){
    player->add_resources(vector<Resource>{Wool,Grain,Ore});
    REQUIRE(player->is_valid_action(BuyDevCard));
}

TEST_CASE("Buy Dev Card, No Dev Card","[player]"){
    REQUIRE(!player->is_valid_action(UseDevCard));
}

TEST_CASE("Buy Dev Card, Has Dev Card","[player]"){
    player->add_dev_card(Knight);
    REQUIRE(player->is_valid_action(UseDevCard));
}

TEST_CASE("Trade Offer, No Cards","[player]"){
    player->remove_resources(vector<Resource>{Wool,Grain,Ore});
    REQUIRE(!player->is_valid_action(TradeOffer));
}

TEST_CASE("Trade Offer, Has Cards","[player]"){
    player->add_resources(vector<Resource>{Wool});
    REQUIRE(player->is_valid_action(TradeOffer));
}

TEST_CASE("Trade Bank, Not Enough Cards","[player]"){
    REQUIRE(!player->is_valid_action(TradeBank));
}

TEST_CASE("Trade Bank, Has Enough Cards","[player]"){
    player->add_resources(vector<Resource>{Wool,Wool,Wool});
    REQUIRE(player->is_valid_action(TradeBank));
}

// Decision Funciton Tests
TEST_CASE("Place Road","[player]"){
    REQUIRE(false);
}

TEST_CASE("Place Settlement","[player]"){
    REQUIRE(false);
}

TEST_CASE("Place City","[player]"){
    REQUIRE(false);
}

TEST_CASE("Use Dev Card","[player]"){
    REQUIRE(false);
}

TEST_CASE("Trade Offer","[player]"){
    REQUIRE(false);
}

TEST_CASE("Trade Bank","[player]"){
    REQUIRE(false);
}

TEST_CASE("Move Robber","[player]"){
    REQUIRE(false);
}

TEST_CASE("","[player]"){
    REQUIRE(false);
}

int main(int argc, char* argv[]) {
    generator.seed(time(NULL));
    player = new Player(1,5,15,5,4,generator);
    Catch::Session session;
    int returnCode = session.applyCommandLine( argc, argv );
    if( returnCode != 0 )
        return returnCode;
    return session.run();
}