#include <catch2/catch_all.hpp>
#include <bank.h>
#include <iostream>

Bank* bank;
vector<Development_Card> cards;

TEST_CASE("Number of Dev Cards","[bank]") {
    REQUIRE(bank->get_num_dev_cards() == 25);
}

TEST_CASE("Number of Knights","[bank]") {
    int count = 0;
    for (int i=0;i<cards.size();i++) {
        if (cards[i] == Knight)
            count++;
    }
    REQUIRE(count == 14);
}

TEST_CASE("Number of Victory Points","[bank]") {
    int count = 0;
    for (int i=0;i<cards.size();i++) {
        if (cards[i] == VictoryPoint)
            count++;
    }
    REQUIRE(count == 5);
}

TEST_CASE("Number of Road Building","[bank]") {
    int count = 0;
    for (int i=0;i<cards.size();i++) {
        if (cards[i] == RoadBuilding)
            count++;
    }
    REQUIRE(count == 2);
}

TEST_CASE("Number of Year of Plenty","[bank]") {
    int count = 0;
    for (int i=0;i<cards.size();i++) {
        if (cards[i] == YearOfPlenty)
            count++;
    }
    REQUIRE(count == 2);
}

TEST_CASE("Number of Monopoly","[bank]") {
    int count = 0;
    for (int i=0;i<cards.size();i++) {
        if (cards[i] == Monopoly)
            count++;
    }
    REQUIRE(count == 2);
}

TEST_CASE("Draw Dev Card","[bank]") {
    bank->draw_dev_card();
    REQUIRE(bank->get_num_dev_cards() == 24);
}

TEST_CASE("Empty Trade No Harbor","[bank]") {
    vector<Resource> offer = {};
    vector<Resource> request = {};

    REQUIRE(!bank->accept_trade_offer(offer, request, NoHarbor));
}

TEST_CASE("Empty Trade Generic Harbor","[bank]") {
    vector<Resource> offer = {};
    vector<Resource> request = {};

    REQUIRE(!bank->accept_trade_offer(offer, request, GenericHarbor));
}

TEST_CASE("Empty Trade 2 for 1 Harbor","[bank]") {
    vector<Resource> offer = {};
    vector<Resource> request = {};

    REQUIRE(!bank->accept_trade_offer(offer, request, BrickHarbor));
}

TEST_CASE("2 for 1 No Harbor","[bank]") {
    vector<Resource> offer = {Brick,Brick};
    vector<Resource> request = {Grain};

    REQUIRE(!bank->accept_trade_offer(offer, request, NoHarbor));
}

TEST_CASE("2 for 1 Generic Harbor","[bank]") {
    vector<Resource> offer = {Brick,Brick};
    vector<Resource> request = {Grain};

    REQUIRE(!bank->accept_trade_offer(offer, request, GenericHarbor));
}

TEST_CASE("2 for 1 Right Harbor Brick","[bank]") {
    vector<Resource> offer = {Brick,Brick};
    vector<Resource> request = {Grain};

    REQUIRE(bank->accept_trade_offer(offer, request, BrickHarbor));
}

TEST_CASE("2 for 1 Right Harbor Lumber","[bank]") {
    vector<Resource> offer = {Lumber,Lumber};
    vector<Resource> request = {Grain};

    REQUIRE(bank->accept_trade_offer(offer, request, LumberHarbor));
}

TEST_CASE("2 for 1 Right Harbor Wool","[bank]") {
    vector<Resource> offer = {Wool,Wool};
    vector<Resource> request = {Grain};

    REQUIRE(bank->accept_trade_offer(offer, request, WoolHarbor));
}

TEST_CASE("2 for 1 Right Harbor Ore","[bank]") {
    vector<Resource> offer = {Ore,Ore};
    vector<Resource> request = {Grain};

    REQUIRE(bank->accept_trade_offer(offer, request, OreHarbor));
}

TEST_CASE("2 for 1 Right Harbor Grain","[bank]") {
    vector<Resource> offer = {Grain,Grain};
    vector<Resource> request = {Brick};

    REQUIRE(bank->accept_trade_offer(offer, request, GrainHarbor));
}

TEST_CASE("2 for 1 Wrong Harbor Brick","[bank]") {
    vector<Resource> offer = {Lumber,Lumber};
    vector<Resource> request = {Grain};

    REQUIRE(!bank->accept_trade_offer(offer, request, BrickHarbor));
}

TEST_CASE("2 for 1 Wrong Harbor Lumber","[bank]") {
    vector<Resource> offer = {Ore,Ore};
    vector<Resource> request = {Grain};

    REQUIRE(!bank->accept_trade_offer(offer, request, LumberHarbor));
}

TEST_CASE("2 for 1 Wrong Harbor Wool","[bank]") {
    vector<Resource> offer = {Grain, Grain};
    vector<Resource> request = {Brick};

    REQUIRE(!bank->accept_trade_offer(offer, request, WoolHarbor));
}

TEST_CASE("2 for 1 Wrong Harbor Ore","[bank]") {
    vector<Resource> offer = {Wool,Wool};
    vector<Resource> request = {Grain};

    REQUIRE(!bank->accept_trade_offer(offer, request, OreHarbor));
}

TEST_CASE("2 for 1 Wrong Harbor Grain","[bank]") {
    vector<Resource> offer = {Brick,Brick};
    vector<Resource> request = {Grain};

    REQUIRE(!bank->accept_trade_offer(offer, request, GrainHarbor));
}

TEST_CASE("2 for 1 Resource Not Match","[bank]") {
    vector<Resource> offer = {Brick,Lumber};
    vector<Resource> request = {Grain};

    REQUIRE(!bank->accept_trade_offer(offer, request, BrickHarbor));
}

TEST_CASE("3 for 1 No Harbor","[bank]") {
    vector<Resource> offer = {Brick,Brick,Brick};
    vector<Resource> request = {Grain};

    REQUIRE(!bank->accept_trade_offer(offer, request, NoHarbor));
}

TEST_CASE("3 for 1 Generic Harbor","[bank]") {
    vector<Resource> offer = {Brick,Brick,Brick};
    vector<Resource> request = {Grain};

    REQUIRE(bank->accept_trade_offer(offer, request, GenericHarbor));
}

TEST_CASE("3 for 1 Matching Harbor","[bank]") {
    vector<Resource> offer = {Brick,Brick,Brick};
    vector<Resource> request = {Grain};

    REQUIRE(!bank->accept_trade_offer(offer, request, BrickHarbor));
}

TEST_CASE("3 for 1 Resource Not Match","[bank]") {
    vector<Resource> offer = {Brick,Lumber,Brick};
    vector<Resource> request = {Grain};

    REQUIRE(!bank->accept_trade_offer(offer, request, GenericHarbor));
}

TEST_CASE("4 for 1 No Harbor","[bank]") {
    vector<Resource> offer = {Brick,Brick,Brick,Brick};
    vector<Resource> request = {Grain};

    REQUIRE(bank->accept_trade_offer(offer, request, NoHarbor));
}

TEST_CASE("4 for 1 Generic Harbor","[bank]") {
    vector<Resource> offer = {Brick,Brick,Brick,Brick};
    vector<Resource> request = {Grain};

    REQUIRE(!bank->accept_trade_offer(offer, request, GenericHarbor));
}

TEST_CASE("4 for 1 Matching Harbor","[bank]") {
    vector<Resource> offer = {Brick,Brick,Brick,Brick};
    vector<Resource> request = {Grain};

    REQUIRE(!bank->accept_trade_offer(offer, request, BrickHarbor));
}

TEST_CASE("4 for 1 Resources Not Match","[bank]") {
    vector<Resource> offer = {Brick,Brick,Lumber,Brick};
    vector<Resource> request = {Grain};

    REQUIRE(!bank->accept_trade_offer(offer, request, NoHarbor));
}

int main(int argc, char* argv[]) {
    vector<int> c{14,5,2,2,2};
    bank = new Bank(5,19,c);
    cards = *bank->get_dev_cards();
    Catch::Session session;
    int returnCode = session.applyCommandLine( argc, argv );
    if( returnCode != 0 )
        return returnCode;
    return session.run();
}