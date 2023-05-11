#include <iostream>
#include "bank.h"

using namespace std;

Bank::Bank(int num_resources_, int resource_count_, vector<int> dev_card_count_, default_random_engine generator_) {
    num_resources = num_resources_;
    resource_count = resource_count_;
    dev_card_count = dev_card_count_;
    resources.resize(num_resources);
    fill(resources.begin(), resources.end(),resource_count);
    generator = generator_;
    generate_dev_cards();
}

void Bank::generate_dev_cards() {
    dev_cards = new vector<Development_Card>;

    for(int i=0; i<dev_card_count.size();i++){
        for(int j=0;j<dev_card_count[i];j++) {
            dev_cards->push_back((Development_Card)i);
        }
    }

    shuffle(begin(*dev_cards),end(*dev_cards),generator);
}

Development_Card Bank::draw_dev_card() {
    Development_Card out = dev_cards->back();
    dev_cards->pop_back();
    return out;
}

bool Bank::accept_trade_offer(Trade trade, Harbor port) {
    if (trade.request.size() != 1 || trade.offer[0] == trade.request[0])
        return false;
    Resource check;
    switch(port) {
        case NoHarbor:
            if(trade.offer.size() != 4)
                return false;
            check = trade.offer[0];
            for(Resource r: trade.offer)
                if(r != check)
                    return false;
            return true;
        case GenericHarbor:
            if(trade.offer.size() != 3)
                return false;
            check = trade.offer[0];
            for(Resource r: trade.offer)
                if(r != check)
                    return false;
            return true;
        case BrickHarbor:
            if (trade.offer == vector<Resource>{Brick, Brick})
                return true;
            return false;
        case LumberHarbor:
            if (trade.offer == vector<Resource>{Lumber, Lumber})
                return true;
            return false;
        case WoolHarbor:
            if (trade.offer == vector<Resource>{Wool, Wool})
                return true;
            return false;
        case GrainHarbor:
            if (trade.offer == vector<Resource>{Grain, Grain})
                    return true;
            return false;
        case OreHarbor:
            if (trade.offer == vector<Resource>{Ore, Ore})
                    return true;
            return false;
        default:
            throw;
    }
    return false;
}

void Bank::add_resources(vector<Resource> resources_) {
    for(int i=0;i<resources_.size();i++)
        resources[(int) resources_[i]]++;
}

bool Bank::remove_resources(vector<Resource> resources_) {
    vector<int> original_count = resources;
    for(int i=0;i<resources_.size();i++)
        if (resources[(int) resources_[i]]>0){
            resources[(int) resources_[i]]--;
        } else {
            resources = original_count;
            return false;
        }
    return true;
}

string Bank::get_dev_card_state() {
    string output = "Dev Card State:\n";
    for(int i=0;i<dev_cards->size();i++){
        switch(dev_cards->at(i)){
            case Knight:
                output += "K";
                break;
            case VictoryPoint:
                output += "V";
                break;
            case RoadBuilding:
                output += "R";
                break;
            case YearOfPlenty:
                output += "Y";
                break;
            case Monopoly:
                output += "M";
                break;
            default:
                output += "X";
                break;
        }
    }
    return output + "\n";
}