#include <game.h>
#include <default_player.h>

string DATA_CONFIG_FILE = "../config_files/catan_data.txt";
string BOARD_CONFIG_FILE = "../config_files/catan_board.csv";

Game* game;
vector<Player*> players;

int main(int argc, char* argv[]) {
    for(int i=0;i<4;i++) {
        players.push_back(new Default_Player(i+1,5,15,5,4));
    }
    game = new Game(DATA_CONFIG_FILE,BOARD_CONFIG_FILE,players);
    game->simulation("test_log.txt");
    return 0;
}



