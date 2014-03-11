#include "../computer0.h"

namespace Tron
{
Computer0::Computer0 (TronBattle* _game, LightCycle* _me)
    : ComputerBase(_game, _me)
{
}

Board::Direction Computer0::getNextMovement ()
{
    Board::Direction dir;
    int maxLiberty = 0;

    for (int i = 0; i < 4; ++i) {
        int liberty = this->game->board->getLiberty(me->lightCycle.back() + i);
        if (liberty > maxLiberty) {
            dir = i;
            maxLiberty = liberty;
        }
    }

    return dir;
}

Computer1::Computer1 (TronBattle* _game, LightCycle* _me)
    : ComputerBase(_game, _me), aloneInZone(false)
{
    srand(time(NULL));
}

Board::Direction Computer1::getNextMovement ()
{
    std::priority_queue<MovementEstimation> movements; // tmp use of priority queue usefull ?? limitfull??
    Board::Direction dir;

    this->getPossibleMovement(me, &movements, true);

    if (movements.empty()) {
        //std::cerr << "no possibilities" << std::endl;
        return Board::FIXED;
    }
    else if (movements.size() == 1) {
        //std::cerr << "only one possibility" << std::endl;
        return movements.top().direction;
    }

    // Do something;

    return movements.top().direction;
}

void Computer1::getPossibleMovement (LightCycle* player, std::priority_queue<MovementEstimation>* movements, bool getOnlyBetterMovements) {
    /*if (playerId == me->id) {
        this->modifyBoardPlayerLiberty(playerId, position, board);
    }
    this->modifyBoardPlayerNearLiberty(playerId, position, board);*/

    std::priority_queue<MovementEstimation> possibleMovement;

    for (int i = 0; i < 4; ++i) {
        int liberty = this->game->board->getLiberty(player->lightCycle.back() + i);
        if (liberty != 0) {
            possibleMovement.push(MovementEstimation(i, liberty));
        }
    }

    if (getOnlyBetterMovements) {
        int liberty = 0;

        while (possibleMovement.size() > 0 && possibleMovement.top().liberty >= liberty) {
            liberty = possibleMovement.top().liberty;
            movements->push(possibleMovement.top());
            possibleMovement.pop();
        }
    }
    else {
        *movements = possibleMovement;
    }
}

Board::Direction Computer1::getRandomNextMovement (std::priority_queue<MovementEstimation>* movements, bool useRand) {
    if ((*movements).size() > 0) {
        short movementId = (useRand) ? rand() % (*movements).size() : 0;

        for (int i = 0; i < movementId; ++i) {
            movements->pop();
        }

        return movements->top().direction;
    }

    return Board::FIXED;
}

/*std::vector<Board::Coordinate> arrivals;
arrivals.push_back(Board::Coordinate(1, 1));
arrivals.push_back(Board::Coordinate(1, 20));
arrivals.push_back(Board::Coordinate(30, 1));
arrivals.push_back(Board::Coordinate(30, 20));
for (int i = 0; i < this->game->playerNumber; ++i) {
    if (i != me->id) {
        arrivals.push_back(this->game->players[i].lightCycle.back());
    }
}
std::vector<Board::Coordinate> way = this->game->board->getWayToNearestPoint(this->me->lightCycle.back(), arrivals);
if (!way.empty() > 1) {
    Board::Direction dir2 = this->me->lightCycle.back().getDirectionTo(way[1]);
    if (dir2 != Board::FIXED) {
        return dir2;
    }
}*/

}// namespace Tron
