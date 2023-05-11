#ifndef CATAN_SIMULATION_BANK_H
#define CATAN_SIMULATION_BANK_H

#include <algorithm>
#include <random>
#include <vector>

#include "components.h"

using namespace std;

class Bank {
    int num_resources, resource_count;
    vector<int> resources, dev_card_count;
    vector<Development_Card>* dev_cards;
    default_random_engine generator;

    void generate_dev_cards();

    public:
    Bank(int num_resources, int resource_count, vector<int> dev_card_count, default_random_engine generator);

    Development_Card draw_dev_card();

    bool accept_trade_offer(Trade trade, Harbor port);

    void add_resources(vector<Resource> resources);

    bool remove_resources(vector<Resource> resources);

    vector<Development_Card>* get_dev_cards() {return dev_cards;}

    int get_num_resources() {return num_resources;}

    int get_resource_count() {return resource_count;}

    int get_num_dev_cards() {return dev_cards->size();}

    string get_dev_card_state();
};

#endif