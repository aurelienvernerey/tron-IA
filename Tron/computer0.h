#ifndef COMPUTER0_H
#define COMPUTER0_H

#include <queue>
#include <stdlib.h>
#include <time.h>
#include "computerbase.h"
#include "tronbattle.h"
#include "movementestimation.h"

namespace Tron
{
class Computer0 : public ComputerBase
{
public:
    Computer0 (TronBattle* _game, LightCycle* _me);

    Board::Direction getNextMovement ();
};

class Computer1 : public ComputerBase
{
public:
    bool aloneInZone; // tmp
public:
    Computer1 (TronBattle* _game, LightCycle* _me);

    Board::Direction getNextMovement ();
    Board::Direction getRandomNextMovement (std::priority_queue<MovementEstimation>* movements, bool useRand = true);
    void getPossibleMovement (LightCycle *player, std::priority_queue<MovementEstimation>* movements, bool getOnlyBetterMovements = false);
};
} // namespace Tron

#endif // COMPUTER0_H
