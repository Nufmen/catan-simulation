#include <catch2/catch_all.hpp>
#include <bank.h>
#include <iostream>

TEST_CASE("Generate Dev Cards","[bank]"){
    Bank bank = Bank(5,19);
    bank.generate_dev_cards();
    bool card_check = true;
    vector<Development_Card> cards = *bank.get_dev_cards();
    vector<int> dev_count(5), num_cards{14,5,2,2,2};
    for (int i=0; i<cards.size();i++) {
        dev_count[cards[i]]++;
    }
    REQUIRE(dev_count == num_cards);
}

TEST_CASE("Accept Trade Offer","[bank]"){
    Bank bank = Bank(5,19);
    vector<Resource> offer = {};
    vector<Resource> request = {};

    REQUIRE(!bank.accept_trade_offer(offer, request, NoHarbor));
    REQUIRE(!bank.accept_trade_offer(offer, request, GenericHarbor));
    REQUIRE(!bank.accept_trade_offer(offer, request, BrickHarbor));

    offer = {Brick, Brick};
    request = {Grain, Grain};
    REQUIRE(!bank.accept_trade_offer(offer, request, NoHarbor));
    REQUIRE(!bank.accept_trade_offer(offer, request, GenericHarbor));
    REQUIRE(!bank.accept_trade_offer(offer, request, BrickHarbor));

    request = {Grain};
    REQUIRE(!bank.accept_trade_offer(offer, request, NoHarbor));
    REQUIRE(!bank.accept_trade_offer(offer, request, GenericHarbor));
    REQUIRE(bank.accept_trade_offer(offer, request, BrickHarbor));

    offer = {Brick, Brick, Brick};
    REQUIRE(!bank.accept_trade_offer(offer, request, NoHarbor));
    REQUIRE(bank.accept_trade_offer(offer, request, GenericHarbor));
    REQUIRE(!bank.accept_trade_offer(offer, request, BrickHarbor));

    offer = {Brick, Brick, Brick, Brick};
    REQUIRE(bank.accept_trade_offer(offer, request, NoHarbor));
    REQUIRE(!bank.accept_trade_offer(offer, request, GenericHarbor));
    REQUIRE(!bank.accept_trade_offer(offer, request, BrickHarbor));

    offer = {Brick, Brick, Lumber, Wool};
    REQUIRE(!bank.accept_trade_offer(offer, request, NoHarbor));
    REQUIRE(!bank.accept_trade_offer(offer, request, GenericHarbor));
    REQUIRE(!bank.accept_trade_offer(offer, request, BrickHarbor));

    offer = {Grain, Grain, Grain, Grain};
    REQUIRE(!bank.accept_trade_offer(offer, request, NoHarbor));
    REQUIRE(!bank.accept_trade_offer(offer, request, GenericHarbor));
    REQUIRE(!bank.accept_trade_offer(offer, request, BrickHarbor));

    offer = {Ore, Ore};
    REQUIRE(!bank.accept_trade_offer(offer, request, BrickHarbor));
    REQUIRE(bank.accept_trade_offer(offer, request, OreHarbor));

    offer = {Lumber, Lumber};
    cout << request[0] << " " << offer[0] << endl;
    REQUIRE(!bank.accept_trade_offer(offer, request, BrickHarbor));
    REQUIRE(bank.accept_trade_offer(offer, request, LumberHarbor));

    offer = {Wool, Wool};
    REQUIRE(!bank.accept_trade_offer(offer, request, BrickHarbor));
    REQUIRE(bank.accept_trade_offer(offer, request, WoolHarbor));

    offer = {Grain, Grain};
    request = {Brick};
    REQUIRE(!bank.accept_trade_offer(offer, request, BrickHarbor));
    REQUIRE(bank.accept_trade_offer(offer, request, GrainHarbor));
}

int main(int argc, char* argv[]) {
    Catch::Session session;
    int returnCode = session.applyCommandLine( argc, argv );
    if( returnCode != 0 )
        return returnCode;
    return session.run();
}