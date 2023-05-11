# catan-simulation

Installation:
1. Download/Clone Repository
2. Navigate to the build directory.
3. run cmake .. && make 
4. 




Player_Move call_player_action()
- returns action from Player_Move.

int place_road(vector<int> options)
- returns selected road id from options.

int place_settlement(vector<int> options)
- returns selected settlement id from options.
    
int place_city(vector<int> options)
- returns selescted city option from options.

Development_Card use_dev_card()
- Returns selected development card from player dev_cards.

Trade trade_offer()
- Returns Trade structure with specific trade offer.

Trade trade_bank()
- Returns Trade structure with bank trade offer.

bool accept_trade(Trade trade)
- Returns true if trade is accepted, false if rejected.

int select_trade(vector<int> players)
- returns player id of who to trade with.

int move_robber(vector<int> options)
- returns tile id of next robber placement

int steal_resource(vector<int> players)
- returns player id of player to steal from.

vector<Resource> discard_hand()
- returns resource list of cards to discard from hand (half rounded down).

vector<Resource> year_of_plenty()
- returns resource list of cards to recieve from bank (exactly 2).

Resource monopoly()
- returns resource you would like to take from all other players.
