# catan-simulation

Installation:
1. Download/Clone Repository
2. Navigate to the build directory.
3. run cmake .. && make 

Run ./simulaiton to run test example.


These are the 14 funcitons that need to be imlemented with a description of what needs to be returned. A full example is available in src/default_player.cpp.

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

Board Config:
O,O,P2,P2,B,P3,P3,B,B,O,O
O,P1,B,B,B,B,B,B,P4,P4,O
B,P1,B,B,B,B,B,B,B,B,P5
B,P9,B,B,B,B,B,B,B,B,P5
O,P9,B,B,B,B,B,B,P6,P6,O
O,O,P8,P8,B,P7,P7,B,B,O,O
    
Data Config:
numRows:6
numCols:11
numTiles:19
numPlayers:4
maxPlayers:4
oceanSpot:O
portSpot:P
buildingSpot:B
tileOrder:[0,3,7,12,16,17,18,15,11,6,2,1,4,8,13,14,10,5,9]
tileCount:[0,1,3,4,4,4,3]
tileNumbers:[5,2,6,3,8,10,9,12,11,4,8,10,9,4,5,6,3,11]
devCardCount:[14,5,2,2,2]
portCount:[4,1,1,1,1,1,0]
    
The program uses the board config to build the board. Ocean spots are ignored, buildings and ports are included in the board. Ports with matching numbers are considered connected to the same port. The data config file holds the values that change between different maps. If you would like to create one, you will need to update the variables here to match the map. Tile order chages the number placement order for a set cycle. Count lists says how many of each type there is based off the index of the enum located in components.h. Tile numbers are the number set for the board in order of how they are placed in the tile order. 
