#include "player.h"
#include "game_state.h"

Player::Player(int owner_, int num_resources, int num_roads, int num_settlements, int num_cities, default_random_engine generator_) {
    resources.resize(num_resources);
    fill(resources.begin(), resources.end(),0);
    victory_points = 0;
    roads = num_roads;
    settlements = num_settlements;
    cities = num_cities;
    owner = owner_;
    generator = generator_;
    state = 0;
    rolled_dice = false;
    trade_rate = vector<int>{4,4,4,4,4};
}

Player_Move Player::call_player_action() {
    switch(state) {
        case 0:
            if(is_valid_action(UseDevCard)) {
                state = 1;
                return UseDevCard;
            }
        case 1:
            if(is_valid_action(RollDice)) {
                state = 2;
                rolled_dice = true;
                return RollDice;
            }
        case 2:
            if(is_valid_action(TradeOffer)) {
                state = 3;
                return TradeOffer;
            }
        case 3:
            if(is_valid_action(TradeBank)) {
                state = 4;
                return TradeBank;
            }
        case 4:
            if(is_valid_action(UseDevCard)) {
                state = 5;
                return UseDevCard;
            }
        case 5:
            if(is_valid_action(PlaceCity)) {
                state = 6;
                return PlaceCity;
            }
        case 6:
            if(is_valid_action(PlaceSettlement)) {
                state = 7;
                return PlaceSettlement;
            }
        case 7:
            if(is_valid_action(PlaceRoad)) {
                state = 8;
                return PlaceRoad;
            }
        case 8:
            if(is_valid_action(PlaceRoad)) {
                state = 9;
                return PlaceRoad;
            }
        case 9:
            if(is_valid_action(BuyDevCard)) {
                state = 10;
                return BuyDevCard;
            }
        case 10:
            if(is_valid_action(EndTurn)) {
                state = 0;
                return EndTurn;
            }
        default:
            state = 0;
            return call_player_action();
    }
    return NoMove;
}

bool Player::is_valid_action(Player_Move move_) {
    switch(move_) {
        case RollDice:
            return !rolled_dice;
        case EndTurn:
            return rolled_dice;
        case PlaceRoad:
            return resources[(int) Brick] >= 1 
            && resources[(int) Lumber] >= 1;
        case PlaceSettlement:
            return resources[(int) Brick] >= 1 
            && resources[(int) Lumber] >= 1
            && resources[(int) Wool] >= 1
            && resources[(int) Grain] >= 1;
        case PlaceCity:
            return resources[(int) Grain] >= 2
            && resources[(int) Ore] >= 3;
        case BuyDevCard:
            return resources[(int) Wool] >= 1
            && resources[(int) Grain] >= 1
            && resources[(int) Ore] >= 1;
        case UseDevCard:
            return dev_cards.size() >= 1;
        case TradeOffer:
            for(int r:resources)
                if(r > 0)
                    return true;
            return false;
        case TradeBank:
            for(int i=0;i<resources.size();i++)
                if(resources[i]>=trade_rate[i])
                    return true;
            return false;
        default:
            return false;
    }
    return true;
}

Road_Data Player::place_road() {
    Road_Data road = Road_Data();
    return road;
}

Coordinate_Data Player::place_settlement() {
    Coordinate_Data coords = Coordinate_Data();
    return coords;
}
    
Coordinate_Data Player::place_city() {
    Coordinate_Data coords = Coordinate_Data();
    return coords;
}

Development_Card Player::use_dev_card() {
    return NoDevCard;
}

Trade Player::trade_offer() {
    Trade trade = Trade();
    return trade;
}

Trade Player::trade_bank() {
    Trade trade = Trade();
    return trade;
}

bool Player::accept_trade(Trade trade) {
    return false;
}

Coordinate_Data Player::move_robber() {
    Coordinate_Data coords = Coordinate_Data();
    return coords;
}

void Player::add_resources(vector<Resource> resources_) {
    for(int i=0;i<resources_.size();i++)
        resources[(int) resources_[i]]++;
}

void Player::remove_resources(vector<Resource> resources_) {
    for(int i=0;i<resources_.size();i++)
        if (resources[(int) resources_[i]]>0)
            resources[(int) resources_[i]]--;
}