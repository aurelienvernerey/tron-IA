#ifndef COMPUTER2_H
#define COMPUTER2_H

#include <queue>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include "computerbase.h"
#include "tronbattle.h"
#include "movementestimation.h"
#include "noreturnzone.h"

namespace Tron
{

class Computer2 : public ComputerBase
{
public:
    bool aloneInZone;
public:
    Computer2 (TronBattle* _game, LightCycle* _me);
    int ***playerDistances;
    int *distancesFromOponents;
    int maxLiberty;
    std::vector<Board::Direction> *directionsToOponents;


    Board::Direction getNextMovement ();
    std::vector<MovementEstimation> getPossibleMovement(LightCycle* player, std::vector<Board::Direction> directions, bool getOnlyBetterMovements = false, double tolerance = 0);
    std::vector<Board::Direction> getPossibleDirection (LightCycle *player);
    int calculateLiberty (Board::GameBoard<bool> *board, std::vector<Board::Coordinate> *positions, int playerId, /*int *distancesFromPlayer, */int waySize = 1, int liberty = 0);
    int getMovementLiberty (int playerId, Board::Coordinate departure, Board::Direction movement);
    void getOponentsLibertiesByMovements(std::vector<MovementEstimation> movementsToCheck, int *liberties);
    void removeInaccessibleSquare (Board::GameBoard<bool> *board, Board::Coordinate position);
    void getZonesOfNoReturn (Board::GameBoard<bool> *board, std::vector<Board::Coordinate> *positions, std::priority_queue<NoReturnZone> *noReturnZones);
    void initPlayerDistances ();
    void reinitPlayerDistances ();
    void deletePlayerDistances ();
};
} // namespace Tron

#endif // COMPUTER2_H
