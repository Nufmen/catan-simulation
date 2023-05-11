#include "player.h"

Player::Player(int owner_, int num_resources, int num_roads, int num_settlements, int num_cities) {
    resources.resize(num_resources);
    fill(resources.begin(), resources.end(),0);
    player_state = new Player_State;
    player_state->dev_cards = 0;
    player_state->knights = 0;
    player_state->num_cards = 0;
    player_state->road_length = 0;
    player_state->victory_points = 0;
    victory_points = 0;
    roads = num_roads;
    settlements = num_settlements;
    cities = num_cities;
    owner = owner_;
    generator.seed(time(NULL));
    rolled_dice = false;
    played_dev_card = false;
    trade_rate = vector<int>{4,4,4,4,4};
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
                if(resources[i]>=trade_rate[i]) {
                    return true;
                }
            return false;
        default:
            return false;
    }
    return false;
}

bool Player::has_resources(vector<Resource> resources_) {
    vector<int> temp_resources = resources;
    for(Resource r: resources_) {
        temp_resources[(int)r]--;
        if(temp_resources[(int)r]<0)
            return false;
    }
    return true;
}

void Player::add_dev_card(Development_Card card_) {
    if (card_ == VictoryPoint) {
        victory_points++;
        return;
    }
    dev_cards.push_back(card_);
    player_state->dev_cards = dev_cards.size();
}

void Player::remove_dev_card(Development_Card card_) {
    auto itr = find(dev_cards.begin(),dev_cards.end(),card_);
    if(itr==dev_cards.end())
    return;
    auto index=distance(dev_cards.begin(), itr);
    dev_cards.erase(dev_cards.begin() + index);
    player_state->dev_cards = dev_cards.size();
}

void Player::add_resources(vector<Resource> resources_) {
    for(int i=0;i<resources_.size();i++)
        resources[(int) resources_[i]]++;

    player_state->num_cards = get_num_resources();
}

void Player::remove_resources(vector<Resource> resources_) {
    for(int i=0;i<resources_.size();i++)
        if (resources[(int) resources_[i]]>0)
            resources[(int) resources_[i]]--;
    
    player_state->num_cards = get_num_resources();
}

void Player::add_harbor(Harbor harbor_) {
    if (harbor_ == NoHarbor)
        return;
    harbors.push_back(harbor_);
    if(harbor_ == GenericHarbor){
        for(int i=0;i<trade_rate.size();i++){
            if(trade_rate[i] > 3) {
                trade_rate[i] = 3;
            }
        }
        return;
    }
    trade_rate[(int)harbor_ -1] = 2;
}

int Player::get_num_resources() {
    int out = 0;
    for(int i=0;i<5;i++) {
        out += resources[i];
    }
    return out;
}