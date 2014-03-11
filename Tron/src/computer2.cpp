#include "../computer2.h"

namespace Tron
{
Computer2::Computer2 (TronBattle* _game, LightCycle* _me)
    : ComputerBase(_game, _me), aloneInZone(false)
{
    srand(time(NULL));
    this->initPlayerDistances();
}

Board::Direction Computer2::getNextMovement ()
{
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
        this->aloneInZone = true;

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
        if (p != me->id && !this->game->players[p].loose) {
            this->game->board->getDistanceToEveryWhere(this->game->players[p].lightCycle.back(), this->playerDistances[p]);
        }
    }
    std::vector<MovementEstimation> movements = this->getPossibleMovement(me, directions, true, 0.04);//4% of tolerance
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

        movement = movementsToKeep.back();
    }
    else {
        if (movements.size() == 1) {
            movement = movements.back().direction;
        }
        else {
            std::vector<MovementEstimation> movementsToKeep;
            for (int i = 0; i < movements.size(); ++i) {
                for (int j = 0; j < this->directionsToOponents[nearestPlayer].size(); ++j) {
                    if (movements[i].direction == this->directionsToOponents[nearestPlayer][j]) {
                        movementsToKeep.push_back(movements[i]);
                        break;
                    }
                }
            }

            if (movementsToKeep.size() == 1) {
                movement = movementsToKeep.back().direction;
            }
            else {
                if (movementsToKeep.empty()) {
                    movement = movements.back().direction;
                }
                else {
                    if (movementsToKeep.size() == 2 && movementsToKeep[0].liberty == movementsToKeep[1].liberty) {
                        std::vector<Board::Coordinate> way = this->game->board->getWayTo(me->lightCycle.back(), this->game->players[nearestPlayer].lightCycle.back());

                        for (int i = 1; i < way.size(); ++i) {
                            Board::Direction wayDirection = (way[i - 1].getDirectionTo(way[i]));

                            if (movementsToKeep[0].direction ==  -wayDirection) {
                                movement = movementsToKeep[1].direction;
                                break;
                            }
                            else if (movementsToKeep[1].direction ==  -wayDirection) {
                                movement = movementsToKeep[0].direction;
                                break;
                            }
                        }

                        if (movement == Board::FIXED) {
                            movement = movementsToKeep.back().direction;
                        }
                    }
                    else {
                        movement = movementsToKeep.back().direction;
                    }
                }
            }
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
        }
    }

    return possibleDirections;
}

std::vector<MovementEstimation> Computer2::getPossibleMovement (LightCycle* player, std::vector<Board::Direction> directions, bool getOnlyBetterMovements, double tolerance) {
    std::vector<MovementEstimation> possibleMovement;

    this->maxLiberty = 0;

    for (int i = 0; i < directions.size(); ++i) {
        Board::GameBoard<bool> *board = this->game->board->toBooleanBoard();
        Board::Coordinate newPos = player->lightCycle.back() + directions[i];
        int liberty = this->getMovementLiberty(me->id, me->lightCycle.back(), directions[i]);

        this->maxLiberty = (liberty > this->maxLiberty) ? liberty : this->maxLiberty;
        possibleMovement.push_back(MovementEstimation(directions[i], liberty));
        delete board;
    }

    if (getOnlyBetterMovements) {
        std::vector<MovementEstimation> betterPossibleMovement;
        int libertyMin = (tolerance > 0 && tolerance <= 1) ? this->maxLiberty * (1 - tolerance) : this->maxLiberty;

        for (int i = 0; i < possibleMovement.size(); ++i) {
            if (possibleMovement[i].liberty >= libertyMin) {
                betterPossibleMovement.push_back(possibleMovement[i]);
            }
        }

        return betterPossibleMovement;
    }

    return possibleMovement;
}

int Computer2::calculateLiberty (Board::GameBoard<bool> *board, std::vector<Board::Coordinate> *positions, int playerId, int waySize, int liberty) {
    std::vector<Board::Coordinate> *nextPositions = new std::vector<Board::Coordinate>;

    for (int i = 0; i < positions->size(); i++) {
        for (int direction = 0; direction < 4; direction++) {
            Board::Coordinate coord = (*positions)[i] + direction;
            if ((*board)(coord)) {
                bool isReachable = true;
                for (int p = 0; p < this->game->players.size(); ++p) {
                    if (p != playerId && !(this->game->players[p].loose)) {
                        if (this->playerDistances[p][coord.y][coord.x] != -1 && this->playerDistances[p][coord.y][coord.x] <= waySize) {
                            isReachable = false;
                            break;
                        }
                    }
                }
                if (isReachable) {
                    nextPositions->push_back(coord);
                    liberty++;
                    (*board)(coord, false);
                }
            }
        }
    }

    if (!nextPositions->empty()) {
        delete positions;
        return this->calculateLiberty(board, nextPositions, playerId, waySize + 1, liberty);
    }

    delete nextPositions;
    delete positions;
    return liberty;
}

int Computer2::getMovementLiberty (int playerId, Board::Coordinate departure, Board::Direction movement) {
    Board::GameBoard<bool> *board = this->game->board->toBooleanBoard();
    std::vector<Board::Coordinate> *positions = new std::vector<Board::Coordinate>;
    Board::Coordinate newPos = departure + movement;
    positions->push_back(newPos);

    int distancesFromPlayer[4];

    for (int i = 0; i < this->game->playerNumber; ++i) {
        if (i != me->id && !this->game->players[i].loose) {
            distancesFromPlayer[i] = this->game->board->getDistanceBetween(newPos, this->game->players[i].lightCycle.back());
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
    int liberty = this->calculateLiberty (board, positions, playerId);

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

        this->removeInaccessibleSquare(board, newPos);
        liberty = board->getLiberty(newPos, 6, true);
    }
    delete board;

    return liberty;
}

void Computer2::removeInaccessibleSquare (Board::GameBoard<bool> *board, Board::Coordinate position)
{
    Board::GameBoard<bool> *boardTemp = board->toBooleanBoard();
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
    for (int p = 0; p < this->game->playerNumber; ++p) {
        for (int j = 0; j < 22; ++j) {
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
