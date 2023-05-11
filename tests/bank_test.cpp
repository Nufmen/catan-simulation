#include <catch2/catch_all.hpp>
#include <bank.h>
#include <iostream>

Bank* bank;
vector<Development_Card> cards;
default_random_engine generator;

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
    Trade trade;
    REQUIRE(!bank->accept_trade_offer(trade, NoHarbor));
}

TEST_CASE("Empty Trade Generic Harbor","[bank]") {
    Trade trade;
    REQUIRE(!bank->accept_trade_offer(trade, GenericHarbor));
}

TEST_CASE("Empty Trade 2 for 1 Harbor","[bank]") {
    Trade trade;
    REQUIRE(!bank->accept_trade_offer(trade, BrickHarbor));
}

TEST_CASE("2 for 1 No Harbor","[bank]") {
    Trade trade;
    trade.offer = {Brick,Brick};
    trade.request = {Grain};
    REQUIRE(!bank->accept_trade_offer(trade, NoHarbor));
}

TEST_CASE("2 for 1 Generic Harbor","[bank]") {
    Trade trade;
    trade.offer = {Brick,Brick};
    trade.request = {Grain};

    REQUIRE(!bank->accept_trade_offer(trade, GenericHarbor));
}

TEST_CASE("2 for 1 Right Harbor Brick","[bank]") {
    Trade trade;
    trade.offer = {Brick,Brick};
    trade.request = {Grain};

    REQUIRE(bank->accept_trade_offer(trade, BrickHarbor));
}

TEST_CASE("2 for 1 Right Harbor Lumber","[bank]") {
    Trade trade;
    trade.offer = {Lumber,Lumber};
    trade.request = {Grain};

    REQUIRE(bank->accept_trade_offer(trade, LumberHarbor));
}

TEST_CASE("2 for 1 Right Harbor Wool","[bank]") {
    Trade trade;
    trade.offer = {Wool,Wool};
    trade.request = {Grain};

    REQUIRE(bank->accept_trade_offer(trade, WoolHarbor));
}

TEST_CASE("2 for 1 Right Harbor Ore","[bank]") {
    Trade trade;
    trade.offer = {Ore,Ore};
    trade.request = {Grain};

    REQUIRE(bank->accept_trade_offer(trade, OreHarbor));
}

TEST_CASE("2 for 1 Right Harbor Grain","[bank]") {
    Trade trade;
    trade.offer = {Grain,Grain};
    trade.request = {Brick};

    REQUIRE(bank->accept_trade_offer(trade, GrainHarbor));
}

TEST_CASE("2 for 1 Wrong Harbor Brick","[bank]") {
    Trade trade;
    trade.offer = {Lumber,Lumber};
    trade.request = {Grain};

    REQUIRE(!bank->accept_trade_offer(trade, BrickHarbor));
}

TEST_CASE("2 for 1 Wrong Harbor Lumber","[bank]") {
    Trade trade;
    trade.offer = {Ore,Ore};
    trade.request = {Grain};

    REQUIRE(!bank->accept_trade_offer(trade, LumberHarbor));
}

TEST_CASE("2 for 1 Wrong Harbor Wool","[bank]") {
    Trade trade;
    trade.offer = {Grain, Grain};
    trade.request = {Brick};

    REQUIRE(!bank->accept_trade_offer(trade, WoolHarbor));
}

TEST_CASE("2 for 1 Wrong Harbor Ore","[bank]") {
    Trade trade;
    trade.offer = {Wool,Wool};
    trade.request = {Grain};

    REQUIRE(!bank->accept_trade_offer(trade, OreHarbor));
}

TEST_CASE("2 for 1 Wrong Harbor Grain","[bank]") {
    Trade trade;
    trade.offer = {Brick,Brick};
    trade.request = {Grain};

    REQUIRE(!bank->accept_trade_offer(trade, GrainHarbor));
}

TEST_CASE("2 for 1 Resource Not Match","[bank]") {
    Trade trade;
    trade.offer = {Brick,Lumber};
    trade.request = {Grain};

    REQUIRE(!bank->accept_trade_offer(trade, BrickHarbor));
}

TEST_CASE("3 for 1 No Harbor","[bank]") {
    Trade trade;
    trade.offer = {Brick,Brick,Brick};
    trade.request = {Grain};

    REQUIRE(!bank->accept_trade_offer(trade, NoHarbor));
}

TEST_CASE("3 for 1 Generic Harbor","[bank]") {
    Trade trade;
    trade.offer = {Brick,Brick,Brick};
    trade.request = {Grain};

    REQUIRE(bank->accept_trade_offer(trade, GenericHarbor));
}

TEST_CASE("3 for 1 Matching Harbor","[bank]") {
    Trade trade;
    trade.offer = {Brick,Brick,Brick};
    trade.request = {Grain};

    REQUIRE(!bank->accept_trade_offer(trade, BrickHarbor));
}

TEST_CASE("3 for 1 Resource Not Match","[bank]") {
    Trade trade;
    trade.offer = {Brick,Lumber,Brick};
    trade.request = {Grain};

    REQUIRE(!bank->accept_trade_offer(trade, GenericHarbor));
}

TEST_CASE("4 for 1 No Harbor","[bank]") {
    Trade trade;
    trade.offer = {Brick,Brick,Brick,Brick};
    trade.request = {Grain};

    REQUIRE(bank->accept_trade_offer(trade, NoHarbor));
}

TEST_CASE("4 for 1 Generic Harbor","[bank]") {
    Trade trade;
    trade.offer = {Brick,Brick,Brick,Brick};
    trade.request = {Grain};

    REQUIRE(!bank->accept_trade_offer(trade, GenericHarbor));
}

TEST_CASE("4 for 1 Matching Harbor","[bank]") {
    Trade trade;
    trade.offer = {Brick,Brick,Brick,Brick};
    trade.request = {Grain};

    REQUIRE(!bank->accept_trade_offer(trade, BrickHarbor));
}

TEST_CASE("4 for 1 Resources Not Match","[bank]") {
    Trade trade;
    trade.offer = {Brick,Brick,Lumber,Brick};
    trade.request = {Grain};

    REQUIRE(!bank->accept_trade_offer(trade, NoHarbor));
}

int main(int argc, char* argv[]) {
    vector<int> c{14,5,2,2,2};
    generator.seed(time(0));
    bank = new Bank(5,19,c,generator);
    cards = *bank->get_dev_cards();
    Catch::Session session;
    int returnCode = session.applyCommandLine( argc, argv );
    if( returnCode != 0 )
        return returnCode;
    return session.run();
}