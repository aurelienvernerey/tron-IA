#include <iostream>
#include <string>
#include <vector>

#include "Board/Board"
#include "Tron/Tron"

#define BOARD_HEIGHT    22
#define BOARD_WIDTH     32

using namespace std;
using namespace Board;
using namespace Tron;

int main()
{
    TronBattle *game;
    short _playerNumber, myId;

    std::cin >> _playerNumber >> myId;

    game = new TronBattle(BOARD_WIDTH, BOARD_HEIGHT, _playerNumber);

    Tron::Computer2 myAi(game, &game->players[myId]);

    for (int lap = 0; true; ++lap) {
        if (lap) {
            std::cin >> _playerNumber >> myId;
        }

        std::cin >> (*game);

#ifdef DEBUG
        std::cerr << "my id: " << myId << std::endl;
        std::cerr << "pr: " << game->playerRemaining << "/" << _playerNumber << std::endl;
        std::cerr << "liberty: " << game->board->getLiberty(game->players[myId].lightCycle.back(), true) << std::endl;
        std::cerr << *game->board << std::endl;
#endif

        std::cout << myAi.getNextMovement() << std::endl;
    }

    return 0;
}
