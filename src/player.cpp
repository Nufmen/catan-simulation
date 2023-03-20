#include "player.h"

Player::Player(int owner, int num_resources, int num_roads, int num_settlements, int num_cities, Game_State* game_state) {
    resources.resize(num_resources);
    fill(resources.begin(), resources.end(),0);
    victory_points = 0;
    roads = num_roads;
    settlements = num_settlements;
    cities = num_cities;
    game_state = game_state;
    generator.seed(time(0));
    uniform_int_distribution<> dice_(1,6);
    dice = dice_;
    owner = owner;
}

void Player::call_player_action() {

}

int Player::roll_dice() {
    int result = dice(generator) + dice(generator);
    return result;
}

void Player::end_turn() {

}

void Player::send_trade_offer(vector<Resource> offer, vector<Resource> request) {

}

void Player::accept_trade_offer() {

}

void Player::play_dev_card(Development_Card dev_card) {

}

void Player::buy_dev_card() {
    dev_cards.push_back(game_state->bank->draw_dev_card());
}

void Player::place_road(BoardCell* cell1, BoardCell* cell2) {
    game_state->board->addRoad(cell1, cell2, owner);
}

void Player::place_settlement(int row, int col) {
    game_state->board->addSettlement(row,col,owner);
}

void Player::place_city(int row, int col) {
    game_state->board->upgradeSettlement(row,col);
}