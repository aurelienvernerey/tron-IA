#ifndef COMPUTERBASE_H
#define COMPUTERBASE_H

#include "Board/direction.h"
#include "tronbattle.h"
#include "lightcycle.h"

namespace Tron
{
class ComputerBase
{
protected:
    TronBattle* game;
    LightCycle* me;
public:
    ComputerBase (TronBattle* _game, LightCycle* _me);

    virtual Board::Direction getNextMovement () = 0;
};
} // namespace Tron

#endif // COMPUTERBASE_H
