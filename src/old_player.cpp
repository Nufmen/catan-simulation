#include "old_player.h"

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
    state = 1;
    rolled_dice = false;
    played_dev_card = false;
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

bool Player::is_valid_action(Player_Move move_) {
    switch(move_) {
        case RollDice:
            cout << resources[0] << " " << resources[1] << " " << resources[2] << " " << resources[3] << " " << resources[4] << endl;
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

int Player::place_road(vector<int> options_) {
    if (options_.size() <=0)
        return -1;
    uniform_int_distribution<int> dist(0, options_.size()-1);
    return options_[dist(generator)];
}

int Player::place_settlement(vector<int> options_) {
    if (options_.size() <=0)
        return -1;
    player_state->victory_points++;
    victory_points++;
    uniform_int_distribution<int> dist(0, options_.size()-1);
    return options_[dist(generator)];
}
    
int Player::place_city(vector<int> options_) {
    if (options_.size() <=0)
        return -1;
    player_state->victory_points++;
    victory_points++;
    uniform_int_distribution<int> dist(0, options_.size()-1);
    return options_[dist(generator)];
}

Development_Card Player::use_dev_card() {
    if (dev_cards.size() <= 0)
        return NoDevCard;
    player_state->dev_cards--;
    uniform_int_distribution<int> dist(0, dev_cards.size()-1);
    Development_Card card = dev_cards[dist(generator)];
    if (card == Knight)
        player_state->knights++;
    return card;
}

Trade Player::trade_offer() {
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

Trade Player::trade_bank() {
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

bool Player::accept_trade(Trade trade) {
    if(trade.offer.size() > trade.request.size() && trade.request.size() > 0)
        return true;
    return false;
}

int Player::select_trade(vector<int> players_) {
    if (players_.size() <=0) {
        return -1;
    }
    uniform_int_distribution<int> dist(0, players_.size()-1);
    return players_[dist(generator)];
}

int Player::move_robber(vector<int> options_) {
    if (options_.size() <=0)
        return -1;
    uniform_int_distribution<int> dist(0, options_.size()-1);
    return options_[dist(generator)];
}

void Player::add_dev_card(Development_Card card_) {
    if (card_ == VictoryPoint) {
        victory_points++;
        return;
    }
    dev_cards.push_back(card_);
}

void Player::remove_dev_card(Development_Card card_) {
    auto itr = find(dev_cards.begin(),dev_cards.end(),card_);
    if(itr==dev_cards.end())
    return;
    auto index=distance(dev_cards.begin(), itr);
    dev_cards.erase(dev_cards.begin() + index);
}

void Player::add_resources(vector<Resource> resources_) {
    for(int i=0;i<resources_.size();i++)
        resources[(int) resources_[i]]++;
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

void Player::remove_resources(vector<Resource> resources_) {
    for(int i=0;i<resources_.size();i++)
        if (resources[(int) resources_[i]]>0)
            resources[(int) resources_[i]]--;
}

int Player::steal_resource(vector<int> players_) {
    if(players_.size() <=0)
        return -1;
    uniform_int_distribution<int> dist(0, players_.size()-1);
    return players_[dist(generator)];
}

vector<Resource> Player::year_of_plenty() {
    vector<Resource> resource_choice{Brick,Lumber};
    return resource_choice;
}

Resource Player::monopoly() {
    return Brick;
}

vector<Resource> Player::discard_hand() {
    vector<Resource> resources;
    return resources;
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

