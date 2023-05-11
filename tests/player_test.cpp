#include <catch2/catch_all.hpp>
#include <player.h>
#include <default_player.h>
#include <board.h>
#include <string>

using namespace std;

Player* player;
default_random_engine generator;

// Valid Acitons

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
/*
// Decision Funciton Tests
TEST_CASE("Place Road","[player]"){
    player = new Default_Player(1,5,15,5,4);
    vector<int> roads{3,14,25};
    int id = player->place_road(roads);
    REQUIRE(count(roads.begin(),roads.end(),id)>0);
}

TEST_CASE("Place Road No Options","[player]"){
    vector<int> roads{};
    REQUIRE(player->place_road(roads) == -1);
}

TEST_CASE("Place Settlement","[player]"){
    vector<int> settlements{12,34,45};
    int id = player->place_settlement(settlements);
    REQUIRE(count(settlements.begin(),settlements.end(),id)>0);
}

TEST_CASE("Place Settlement No Options","[player]"){
    vector<int> settlements{};
    REQUIRE(player->place_road(settlements) == -1);
}

TEST_CASE("Place City","[player]"){
    vector<int> cities{23,74,25,64};
    int id = player->place_city(cities);
    REQUIRE(count(cities.begin(),cities.end(),id)>0);
}

TEST_CASE("Place City No Options","[player]"){
    vector<int> cities{};
    REQUIRE(player->place_road(cities) == -1);
}

TEST_CASE("Use Dev Card None","[player]"){
    REQUIRE(player->use_dev_card() == NoDevCard);
}

TEST_CASE("Use Dev Card One","[player]"){
    player->add_dev_card(Monopoly);
    REQUIRE(player->use_dev_card() == Monopoly);
}

TEST_CASE("Use Dev Card Multiple","[player]"){
    player->add_dev_card(Knight);
    player->add_dev_card(VictoryPoint);
    player->add_dev_card(RoadBuilding);
    player->add_dev_card(YearOfPlenty);
    player->add_dev_card(Monopoly);
    REQUIRE(player->use_dev_card() != NoDevCard);
}

TEST_CASE("Trade Offer No Hand","[player]"){
    REQUIRE(player->trade_offer().offer.size() == 0);
}

TEST_CASE("Trade Offer One Card In Hand","[player]"){
    vector<Resource> resources{Grain};
    player->add_resources(resources);
    REQUIRE(player->trade_offer().offer.size() == 1);
}

TEST_CASE("Trade Offer Multiple Card In Hand","[player]"){
    vector<Resource> resources{Ore,Wool};
    player->add_resources(resources);
    REQUIRE(player->trade_offer().offer.size() >= 1);
}

TEST_CASE("Trade Bank No Hand","[player]"){
    vector<Resource> resources{Grain,Ore,Wool};
    player->remove_resources(resources);
    REQUIRE(player->trade_bank().offer.size() == 0);
}

TEST_CASE("Trade Bank No 2 Cards In Hand","[player]"){
    vector<Resource> resources{Brick,Lumber,Grain,Ore,Wool};
    player->add_resources(resources);
    REQUIRE(player->trade_bank().offer.size() == 0);
}

TEST_CASE("Trade Bank Multiple Cards In Hand","[player]"){
    vector<Resource> resources{Brick,Lumber,Grain,Ore,Wool};
    player->add_resources(resources);
    player->set_trade_rate(Brick,2);
    REQUIRE(player->trade_bank().offer.size() > 1);
    REQUIRE(player->trade_bank().request.size() == 1);
}

TEST_CASE("Accept Trade No Trade","[player]"){
    Trade trade;
    REQUIRE(!player->accept_trade(trade));
}

TEST_CASE("Accept Trade No Offer","[player]"){
    Trade trade;
    trade.request.push_back(Grain);
    REQUIRE(!player->accept_trade(trade));
}

TEST_CASE("Accept Trade No Request","[player]"){
    Trade trade;
    trade.offer.push_back(Grain);
    REQUIRE(!player->accept_trade(trade));
}

TEST_CASE("Move Robber No Options","[player]"){
    vector<int> tiles{};
    REQUIRE(player->move_robber(tiles) == -1);
}

TEST_CASE("Move Robber Within Range","[player]"){
    vector<int> tiles{0,1,2,3,4,5,6,7,8,9,10};
    vector<int> copy;
    uniform_int_distribution<int> dist(0, tiles.size()-1);
    int id = -1;
    bool pass = true;
    for(int i=0;i<1000;i++){
        copy = tiles;
        id = dist(generator);
        swap(copy[id],copy.back());
        copy.pop_back();
        if(player->move_robber(copy) > tiles[tiles.size()-1]) {
            pass = false;
            break;
        }
    }
    REQUIRE(pass);
}

TEST_CASE("Move Robber Exclude Current Tile","[player]"){
    vector<int> tiles{0,1,2,3,4,5,6,7,8,9,10};
    vector<int> copy;
    uniform_int_distribution<int> dist(0, tiles.size()-1);
    int id = -1;
    bool pass = true;
    for(int i=0;i<1000;i++){
        copy = tiles;
        id = dist(generator);
        swap(copy[id],copy.back());
        copy.pop_back();
        if(player->move_robber(copy) == id) {
            pass = false;
            break;
        }
    }
    REQUIRE(pass);
}

TEST_CASE("Remove Cards from Empty Hand","[player]"){
    player = new Default_Player(1,5,15,5,4);
    vector<Resource> resources{Brick,Lumber,Grain,Ore,Wool};
    player->remove_resources(resources);
    REQUIRE(player->get_resource_count(Brick) == 0);
}*/

int main(int argc, char* argv[]) {
    player = new Default_Player(1,5,15,5,4);
    Catch::Session session;
    int returnCode = session.applyCommandLine( argc, argv );
    if( returnCode != 0 )
        return returnCode;
    return session.run();
}