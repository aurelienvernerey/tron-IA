#include "../computer2.h"

namespace Tron // tmp modify this !!
{
Computer2::Computer2 (TronBattle* _game, LightCycle* _me)
    : ComputerBase(_game, _me), aloneInZone(false)
{
    srand(time(NULL));
    this->initPlayerDistances();
}

Board::Direction Computer2::getNextMovement ()
{
    //return 0;
    std::vector<Board::Direction> directions;
    Board::Direction dir;

    directions = this->getPossibleDirection(me);

    if (directions.empty()) {
        return Board::FIXED;
    }
    else if (directions.size() == 1) {
        return directions.back();
    }

    if (this->game->aPlayerJustLoose) {
        this->aloneInZone = true; // tmp to optimize

        for (int i = 0; i < this->game->playerNumber; ++i) {
            if (i != me->id && !this->game->players[i].loose) {
                int distancesFromPlayer = this->game->board->getDistanceBetween(me->lightCycle.back(), this->game->players[i].lightCycle.back());
                if (distancesFromPlayer != -1) {
                    this->aloneInZone = false;
                    break;
                }
            }
        }

    }

    Board::Direction movement;
    this->reinitPlayerDistances();
    for (int p = 0; p < this->game->players.size(); ++p) {
        if (p != me->id && !this->game->players[p].loose) { // tmp
            this->game->board->getDistanceToEveryWhere(this->game->players[p].lightCycle.back(), this->playerDistances[p]);
        }
    }
    double tolerance = (this->aloneInZone) ? 0 : 0.1;//0.04 //tmp (check allway all possibilities ????)
    std::vector<MovementEstimation> movements = this->getPossibleMovement(me, directions, true, tolerance);//tmp 4% of tolerance
    // Do something;
    short nearestPlayer = -1;
    bool alone = true;

    for (int i = 0; i < this->game->playerNumber; ++i) {
        if (i != me->id) {
            if (this->distancesFromOponents[i] != -1) {
                alone = false;

                if (nearestPlayer == -1 || this->distancesFromOponents[i] < this->distancesFromOponents[nearestPlayer]) {
                    nearestPlayer = i;
                }
            }
        }
    }

    this->aloneInZone = alone;

    if (this->aloneInZone) {
        int lowerLiberty = -1;
        std::vector<Board::Direction> movementsToKeep;

        for (int i = 0; i < movements.size(); ++i) {
            std::cerr << "max lib = " << this->maxLiberty << std::endl;
            if (movements[i].liberty == this->maxLiberty) {
                int adjacentLiberty = this->game->board->getAdjacentLiberty(me->lightCycle.back() + movements[i].direction);
                if (lowerLiberty == -1 || adjacentLiberty < lowerLiberty) {
                    lowerLiberty = adjacentLiberty;
                    movementsToKeep.clear();
                    movementsToKeep.push_back(movements[i].direction);
                }
                else if (adjacentLiberty == lowerLiberty) {
                    movementsToKeep.push_back(movements[i].direction);
                }
            }
        }

        if (movementsToKeep.size() == 1) {
            movement = movementsToKeep.back();//tmp
        }
        else {
            std::cerr << "ALONE MORE THAN ONE POSSIBILITY" << std::endl;
            movement = movementsToKeep.back();//tmp
        }
    }
    else {
        if (movements.size() == 1) {
            movement = movements.back().direction;
        }
        else {
            std::vector<Board::Direction> movementsToKeep;
            std::cerr <<"toto0" << std::endl;
            for (int i = 0; i < movements.size(); ++i) {
                movementsToKeep.push_back(movements[i].direction);
            }
            std::cerr <<"toto1" << std::endl;
            int oponentLiberties[4];
            this->getOponentsLibertiesByMovements(movements, oponentLiberties);

            int l = 0;

            for (int i = 0; i < movements.size(); ++i) {
                int ll = /*movements[i].liberty*/ - oponentLiberties[movements[i].direction];
                std::cerr << movements[i].direction << movements[i].liberty << " - " << oponentLiberties[movements[i].direction] << ": " << ll << std::endl;
                if (i == 0 || ll > l) {
                    l = ll;
                    movement = movements[i].direction;
                }
            }


            return movement;


            //return movementsToKeep.back();//*/
            /*movementsToKeep.clear();

            for (int i = 0; i < movements.size(); ++i) {
                for (int j = 0; j < this->directionsToOponents[nearestPlayer].size(); ++j) {
                    if (movements[i].direction == this->directionsToOponents[nearestPlayer][j]) {
                        movementsToKeep.push_back(movements[i].direction);
                        break;
                    }
                }
            }

            if (movementsToKeep.size() == 1) {
                movement = movementsToKeep.back();
                std::cerr << "move to the nearest player !" << std::endl;
            }
            else {
                std::cerr << "MORE THAN ONE POSSIBILITY" << std::endl;
                if (movementsToKeep.empty()) {//max 2 choices
                    movement = movements.back().direction;//tmp
                }
                else { //max 2 choices
                    movement = movementsToKeep.back();//tmp
                }
            }//*/
        }
    }

    return movement;
}

std::vector<Board::Direction> Computer2::getPossibleDirection (LightCycle *player)
{
    std::vector<Board::Direction> possibleDirections;

    for (int i = 0; i < 4; ++i) {
        if ((*this->game->board)(player->lightCycle.back() + i)) {
            possibleDirections.push_back(i);
            std::cerr << "a: " << Board::Direction(i)<<std::endl;
        }
    }

    return possibleDirections;
}

//tmp still need to be opimized !!!
std::vector<MovementEstimation> Computer2::getPossibleMovement (LightCycle* player, std::vector<Board::Direction> directions, bool getOnlyBetterMovements, double tolerance) {
    std::vector<MovementEstimation> possibleMovement;

    int _maxLiberty = 0;
    std::cerr <<  "PLAYER " << player->id << std::endl;

    for (int i = 0; i < directions.size(); ++i) {
        Board::Coordinate newPos = player->lightCycle.back() + directions[i];

        int liberty = this->getMovementLiberty(me->id, me->lightCycle.back(), directions[i]);

        _maxLiberty = (liberty > _maxLiberty) ? liberty : _maxLiberty;

        std::cerr <<  directions[i] << " -> liberty: " << liberty << std::endl;
        possibleMovement.push_back(MovementEstimation(directions[i], liberty));
    }

    if (player == me) {
        this->maxLiberty = _maxLiberty;
    }

    if (getOnlyBetterMovements) {
        std::vector<MovementEstimation> betterPossibleMovement;

        int libertyMin = (tolerance > 0 && tolerance <= 1) ? _maxLiberty * (1 - tolerance) : _maxLiberty;

        for (int i = 0; i < possibleMovement.size(); ++i) {
            if (possibleMovement[i].liberty >= libertyMin) {
                betterPossibleMovement.push_back(possibleMovement[i]);
            }
        }

        return betterPossibleMovement;
    }

    return possibleMovement;
}

void Computer2::getOponentsLibertiesByMovements(std::vector<MovementEstimation> movementsToCheck, int* liberties) { // tmp return
    std::vector<MovementEstimation> possibleMovement;

    int actualOponentLiberty[4];

    this->game->board->getDistanceToEveryWhere(me->lightCycle.back(), this->playerDistances[me->id]);

    for (int p = 0; p < this->game->playerNumber; ++p) {
        if (p != me->id && !this->game->players[p].loose && this->distancesFromOponents[p] != -1) {
            Board::GameBoard<bool> *board = this->game->board->toBooleanBoard();
            std::vector<Board::Coordinate> *positions = new std::vector<Board::Coordinate>;
            positions->push_back(this->game->players[p].lightCycle.back());
            actualOponentLiberty[p] = this->calculateLiberty (board, positions, p);
            delete board;
        }
    }

    for (int i = 0; i < movementsToCheck.size(); ++i) {

        Board::Coordinate newPos = me->lightCycle.back() + movementsToCheck[i].direction;

        this->game->board->getDistanceToEveryWhere(newPos, this->playerDistances[me->id]);
        liberties[movementsToCheck[i].direction] = 0;
        std::cerr << movementsToCheck[i].direction << std::endl;
        int pn = 0;

        //int liberty = this->getMovementLiberty(me->id, me->lightCycle.back(), directions[i]);

        for (int p = 0; p < this->game->playerNumber; ++p) {
            if (p != me->id && !this->game->players[p].loose && this->distancesFromOponents[p] != -1) {

                Board::GameBoard<bool> *board = this->game->board->toBooleanBoard();
                (*board)(newPos, false);
                std::vector<Board::Coordinate> *positions = new std::vector<Board::Coordinate>;
                positions->push_back(this->game->players[p].lightCycle.back());//tmp attention a qui a le tour

                int liberty = this->calculateLiberty (board, positions, p, 0);
                int libertyDifference = liberty - actualOponentLiberty[p];
                if (libertyDifference != 0) { // tmp percentage ???
                    liberties[movementsToCheck[i].direction] += libertyDifference;//liberty;
                    pn++;
                }
                    std::cerr << " player " << p << " have " << liberty << " liberties - " << actualOponentLiberty[p] << " -> " << (liberty - actualOponentLiberty[p]) <<  std::endl;
                delete board;
            }
        }

        if (pn > 0) {
            liberties[movementsToCheck[i].direction] /= pn;
        }
    }

    /*if (player == me) {
        this->maxLiberty = _maxLiberty;
    }

    if (getOnlyBetterMovements) {
        std::vector<MovementEstimation> betterPossibleMovement;

        int libertyMin = (tolerance > 0 && tolerance <= 1) ? _maxLiberty * (1 - tolerance) : _maxLiberty;

        for (int i = 0; i < possibleMovement.size(); ++i) {
            if (possibleMovement[i].liberty >= libertyMin) {
                betterPossibleMovement.push_back(possibleMovement[i]);
            }
        }

        return betterPossibleMovement;
    }

    return possibleMovement;*/
}//*/

int Computer2::calculateLiberty (Board::GameBoard<bool> *board, std::vector<Board::Coordinate> *positions, int playerId, /*int* distancesFromPlayer, */int waySize, int liberty) {
    std::vector<Board::Coordinate> *nextPositions = new std::vector<Board::Coordinate>;

    for (int i = 0; i < positions->size(); i++) {
        for (int direction = 0; direction < 4; direction++) {
            Board::Coordinate coord = (*positions)[i] + direction;
            if ((*board)(coord)) {
                bool isReachable = true;
                for (int p = 0; p < this->game->players.size(); ++p) {
                    if (p != playerId && !(this->game->players[p].loose) /*&& distancesFromPlayer[p] != -1 && waySize >= (distancesFromPlayer[p])*/) {
                        if (this->playerDistances[p][coord.y][coord.x] != -1 && this->playerDistances[p][coord.y][coord.x] <= waySize) {//myDistance) { // tmp
                            isReachable = false;
                            break;
                        }
                    }
                }
                if (isReachable) {
                    nextPositions->push_back(coord);
                    liberty++;
                    (*board)(coord, false);//tmp
                }
                //(*board)(coord, false);
            }
        }
    }

    if (!nextPositions->empty()) {
        delete positions;
        return this->calculateLiberty(board, nextPositions, playerId, /*distancesFromPlayer, */waySize + 1, liberty);
    }

    delete nextPositions;
    delete positions;
    return liberty;//tmp ??
}

int Computer2::getMovementLiberty (int playerId, Board::Coordinate departure, Board::Direction movement) {
    Board::GameBoard<bool> *board = this->game->board->toBooleanBoard();
    std::vector<Board::Coordinate> *positions = new std::vector<Board::Coordinate>;
    Board::Coordinate newPos = departure + movement;
    positions->push_back(newPos);

    int distancesFromPlayer[4];

    for (int i = 0; i < this->game->playerNumber; ++i) {
        if (i != me->id && !this->game->players[i].loose) {
            distancesFromPlayer[i] = this->game->board->getDistanceBetween(newPos, this->game->players[i].lightCycle.back());// / 2; // tmp
            if (distancesFromPlayer[i] != -1) {
                if (this->distancesFromOponents[i] != -1) {
                    if (this->distancesFromOponents[i] > distancesFromPlayer[i]) {
                        this->directionsToOponents[i].clear();
                        this->distancesFromOponents[i] = distancesFromPlayer[i];
                        this->directionsToOponents[i].push_back(movement);
                    }
                    else if (this->distancesFromOponents[i] == distancesFromPlayer[i]){
                        this->directionsToOponents[i].push_back(movement);
                    }
                }
                else {
                    this->distancesFromOponents[i] = distancesFromPlayer[i];
                    this->directionsToOponents[i].push_back(movement);
                }


            }
            distancesFromPlayer[i] /= 2;
        }
    }

    (*board)(newPos, false);
    int liberty = this->calculateLiberty (board, positions, playerId/*, distancesFromPlayer*/);

    if (this->aloneInZone) {
        for (int y = 0; y < 22; ++y) {
            for (int x = 0; x < 32; ++x) {
                if ((*this->game->board)(x, y)) {
                    if (!(*board)(x, y)) {
                        (*board)(x, y, true);
                    }
                    else {
                        (*board)(x, y, false);
                    }
                }
            }
        }

        this->removeInaccessibleSquare(board, newPos); // tmp do that only once !!
        liberty = board->getLiberty(newPos, 6, true);
    }
    else {
        /*for (int y = 0; y < 22; ++y) {
            for (int x = 0; x < 32; ++x) {
                if ((*this->game->board)(x, y)) {
                    if (!(*board)(x, y)) {
                        (*board)(x, y, true);
                    }
                    else {
                        (*board)(x, y, false);
                    }
                }
            }
        }

        //this->removeInaccessibleSquare(board, newPos); // tmp do that only once !!
        liberty = board->getLiberty(newPos, 2, true);//*/
    }
    delete board;

    return liberty;
}

void Computer2::removeInaccessibleSquare (Board::GameBoard<bool> *board, Board::Coordinate position)
{
    Board::GameBoard<bool> *boardTemp = board->toBooleanBoard();//tmp this->game->board
    (*boardTemp)(position, false);
    std::priority_queue<NoReturnZone> noReturnZones;
    std::vector<Board::Coordinate> *positions = new std::vector<Board::Coordinate>;
    positions->push_back(position);
    this->getZonesOfNoReturn(boardTemp, positions, &noReturnZones);

    if (noReturnZones.size() > 1) {
        noReturnZones.pop();
        do {
            NoReturnZone zone = noReturnZones.top();
            noReturnZones.pop();
            (*board)(zone.entrance, false);

        } while (!noReturnZones.empty());
    }
    delete boardTemp;
}

void Computer2::getZonesOfNoReturn (Board::GameBoard<bool> *board, std::vector<Board::Coordinate> *positions, std::priority_queue<NoReturnZone> *noReturnZones)
{
    std::vector<Board::Coordinate> *nextPositions = new std::vector<Board::Coordinate>;
    for (int i = 0; i < positions->size(); ++i) {
        for (int d = 0; d < 4; ++d) {
            Board::Coordinate newPos = (*positions)[i] + d;
            if ((*board)(newPos)) {
                (*board)(newPos, false);

                if (!(*this->game->board)(newPos + (d + Board::LEFT)) && !((*this->game->board)(newPos + (d + Board::RIGHT)) && (*this->game->board)(newPos + (d + Board::RIGHT) + (d + Board::DOWN))) ||
                        !(*this->game->board)(newPos + (d + Board::RIGHT)) && !(*this->game->board)(newPos + (d + Board::LEFT) + (d + Board::DOWN))) {
                    noReturnZones->push(NoReturnZone(newPos, board->getLiberty(newPos, 1, true)));
                }
                else {
                    nextPositions->push_back(newPos);
                }
            }
        }
    }

    delete positions;

    if (nextPositions->empty()) {
        delete nextPositions;
    }
    else {
        this->getZonesOfNoReturn(board, nextPositions, noReturnZones);
    }
}

void Computer2::initPlayerDistances ()
{
    this->playerDistances = new int**[this->game->playerNumber];
    this->distancesFromOponents = new int[this->game->playerNumber];
    this->directionsToOponents = new std::vector<Board::Direction>[this->game->playerNumber];
    for (int p = 0; p < this->game->playerNumber; ++p) {
        this->playerDistances[p] = new int*[22];
        for (int j = 0; j < 22; ++j) {
            this->playerDistances[p][j] = new int[32];
            for (int i = 0; i < 32; ++i) {
                this->playerDistances[p][j][i] = -1;
            }
        }

        this->distancesFromOponents[p] = -1;
    }
}

void Computer2::reinitPlayerDistances ()
{
    //this->playerDistances = new int**[this->game->playerNumber];
    //this->distancesFromOponents = new int[this->game->playerNumber];
    //this->directionsToOponents = new std::vector<Board::Direction>[this->game->playerNumber];
    for (int p = 0; p < this->game->playerNumber; ++p) {
        //this->playerDistances[p] = new int*[32];
        for (int j = 0; j < 22; ++j) {
            //this->playerDistances[p][i] = new int[22];
            for (int i = 0; i < 32; ++i) {
                this->playerDistances[p][j][i] = -1;
            }
        }

        this->distancesFromOponents[p] = -1;
        this->directionsToOponents[p].clear();
    }
}

void Computer2::deletePlayerDistances ()
{
    for (int p = 0; p < this->game->playerNumber; ++p) {
        for (int j = 0; j < 22; ++j) {
            delete[] this->playerDistances[p][j];
        }
        delete[] this->playerDistances[p];
    }
    delete[] this->playerDistances;
    delete[] this->distancesFromOponents;
    delete[] this->directionsToOponents;
}

}// namespace Tron


/*for (int y = 1; y < 21; ++y) {
    for (int x = 1; x < 31; ++x) {
        if ((*board)(x, y)) {
            int myDistance = this->game->board->getDistanceBetween(newPos, Board::Coordinate(x, y));
            for (int p = 0; p < this->game->playerNumber; ++p) {
                if (p != me->id) {
                    LightCycle *player = &this->game->players[p];
                    if (!player->loose) {
                        int playerDistance = this->game->board->getDistanceBetween(player->lightCycle.back(), Board::Coordinate(x, y));
                        if (playerDistance != -1 && playerDistance <= myDistance) {
                            (*board)(x, y, false);
                            break;
                        }
                    }
                }
            }
        }
    }
}

for (int y = 0; y < 22; ++y) {
    for (int x = 0; x < 32; ++x) {
        if (x == newPos.x && y == newPos.y) {
            std::cerr << "n ";
        }
        else if ((*this->game->board)(x, y)) {
            if ((*board)(x, y)) {
                std::cerr << "  ";
            }
            else {
                std::cerr << "x ";
            }
        }
        else {
            if ((*this->game->board)(x, y).isWall) {
                std::cerr << "* ";
            }
            else {
                std::cerr << (*this->game->board)(x, y).playerId << " ";
            }
        }
    }
    std::cerr << std::endl;
}

int liberty = board->getLiberty(newPos, true);//*/
