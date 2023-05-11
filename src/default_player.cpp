#include "default_player.h"

Player_Move Default_Player::call_player_action() {
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
                state = 1;
                rolled_dice = false;
                return EndTurn;
            }
        default:
            state = 1;
            return call_player_action();
    }
    return NoMove;
}

int Default_Player::place_road(vector<int> options_) {
    if (options_.size() <=0)
        return -1;
    uniform_int_distribution<int> dist(0, options_.size()-1);
    return options_[dist(generator)];
}

int Default_Player::place_settlement(vector<int> options_) {
    if (options_.size() <=0)
        return -1;
    player_state->victory_points++;
    victory_points++;
    uniform_int_distribution<int> dist(0, options_.size()-1);
    return options_[dist(generator)];
}
    
int Default_Player::place_city(vector<int> options_) {
    if (options_.size() <=0)
        return -1;
    player_state->victory_points++;
    victory_points++;
    uniform_int_distribution<int> dist(0, options_.size()-1);
    return options_[dist(generator)];
}

Development_Card Default_Player::use_dev_card() {
    if (dev_cards.size() <= 0)
        return NoDevCard;
    player_state->dev_cards--;
    uniform_int_distribution<int> dist(0, dev_cards.size()-1);
    Development_Card card = dev_cards[dist(generator)];
    if (card == Knight)
        player_state->knights++;
    return card;
}

Trade Default_Player::trade_offer() {
    Trade trade = Trade();
    if (count(resources.begin(),resources.end(),0) == resources.size())
        return trade;
    uniform_int_distribution<int> dist(0, resources.size()-1);
    int id = dist(generator);
    int new_id = id;
    trade.offer.push_back((Resource)id);
    while(id == new_id){
        new_id = dist(generator);
    }
    trade.request.push_back((Resource)new_id);
    return trade;
}

Trade Default_Player::trade_bank() {
    Trade trade = Trade();
    uniform_int_distribution<int> dist(0, resources.size()-1);
    int r = 0;
    for(int i=0;i<trade_rate.size();i++){
        if(trade_rate[i] <= resources[i]){
            r = i;
            for(int j=0;j<trade_rate[i];j++){
                trade.offer.push_back((Resource)i);
            }
            while(i == r){
                r = dist(generator);
            }
            trade.request.push_back((Resource)r);
            if(count(harbors.begin(),harbors.end(),(Harbor)(i+1))>0){
                trade.harbor = (Harbor)(i+1);
                break;
            }
            if(count(harbors.begin(),harbors.end(),GenericHarbor)>0){
                trade.harbor = GenericHarbor;
                break;
            }
            trade.harbor = NoHarbor;
            break;
        }
    }
    return trade;
}

bool Default_Player::accept_trade(Trade trade) {
    if(trade.offer.size() > trade.request.size() && trade.request.size() > 0)
        return true;
    return false;
}

int Default_Player::select_trade(vector<int> players_) {
    if (players_.size() <=0) {
        return -1;
    }
    uniform_int_distribution<int> dist(0, players_.size()-1);
    return players_[dist(generator)];
}

int Default_Player::move_robber(vector<int> options_) {
    if (options_.size() <=0)
        return -1;
    uniform_int_distribution<int> dist(0, options_.size()-1);
    return options_[dist(generator)];
}

int Default_Player::steal_resource(vector<int> players_) {
    if(players_.size() <=0)
        return -1;
    uniform_int_distribution<int> dist(0, players_.size()-1);
    return players_[dist(generator)];
}

vector<Resource> Default_Player::discard_hand() {
    vector<Resource> resources_discard;
    for(int i=0;i<5;i++){
        for(int j=0;j<resources[i];j++){
            resources_discard.push_back((Resource)i);
        }
    }
    shuffle(resources_discard.begin(),resources_discard.end(),generator);
    for(int i=0;i<(int)(resources_discard.size()/2);i++){
        resources_discard.erase(resources_discard.begin());
    }
    return resources_discard;
}

vector<Resource> Default_Player::year_of_plenty() {
    vector<Resource> resource_choice{Brick,Lumber};
    return resource_choice;
}

Resource Default_Player::monopoly() {
    return Brick;
}
