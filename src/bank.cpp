#include <iostream>
#include "bank.h"

using namespace std;

Bank::Bank(int num_resources_, int resource_count_) {
    num_resources = num_resources_;
    resource_count = resource_count_;
    resources.resize(num_resources);
    fill(resources.begin(), resources.end(),resource_count);
    generator.seed(time(0));
}

void Bank::generate_dev_cards() {
    vector<int> c{14,5,2,2,2};
    dev_cards = new vector<Development_Card>;

    for(int i=0; i<c.size();i++){
        for(int j=0;j<c[i];j++) {
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

bool Bank::accept_trade_offer(vector<Resource> offer, vector<Resource> request, Harbor port) {
    if (request.size() != 1 || offer[0] == request[0])
        return false;
    Resource check;
    switch(port) {
        case NoHarbor:
            if(offer.size() != 4)
                return false;
            check = offer[0];
            for(Resource r: offer)
                if(r != check)
                    return false;
            return true;
        case GenericHarbor:
            if(offer.size() != 3)
                return false;
            check = offer[0];
            for(Resource r: offer)
                if(r != check)
                    return false;
            return true;
        case BrickHarbor:
            if (offer == vector<Resource>{Brick, Brick})
                return true;
            return false;
        case LumberHarbor:
            if (offer == vector<Resource>{Lumber, Lumber})
                return true;
            return false;
        case WoolHarbor:
            if (offer == vector<Resource>{Wool, Wool})
                return true;
            return false;
        case GrainHarbor:
            if (offer == vector<Resource>{Grain, Grain})
                    return true;
            return false;
        case OreHarbor:
            if (offer == vector<Resource>{Ore, Ore})
                    return true;
            return false;
        default:
            throw;
    }
    return false;
}