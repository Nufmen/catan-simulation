#ifndef CATAN_SIMULATION_BANK_H
#define CATAN_SIMULATION_BANK_H

#include <algorithm>
#include <random>
#include <vector>

#include "components.h"

using namespace std;

class Bank {
    int num_resources, resource_count;
    vector<int> resources;
    vector<Development_Card>* dev_cards;
    default_random_engine generator;

    public:
    Bank(int num_resources, int resource_count);

    void generate_dev_cards();

    bool accept_trade_offer(vector<Resource> offer, vector<Resource> request, Harbor port);

    Development_Card draw_dev_card();

    vector<Development_Card>* get_dev_cards() {return dev_cards;}

    int get_num_resources() {return num_resources;}

    int get_resource_count() {return resource_count;}
};

#endif