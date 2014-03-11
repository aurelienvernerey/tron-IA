#ifndef MOVEMENTESTIMATION_H
#define MOVEMENTESTIMATION_H

#include "Board/direction.h"

namespace Tron
{
class MovementEstimation
{
public: // tmp
    Board::Direction direction;
    int liberty;
public:
    MovementEstimation(Board::Direction _direction = Board::FIXED, int _liberty = 0);

    friend bool operator == (MovementEstimation const&, MovementEstimation const&);
    friend bool operator != (MovementEstimation const&, MovementEstimation const&);
    friend bool operator > (MovementEstimation const&, MovementEstimation const&);
    friend bool operator < (MovementEstimation const&, MovementEstimation const&);
    friend bool operator >= (MovementEstimation const&, MovementEstimation const&);
    friend bool operator <= (MovementEstimation const&, MovementEstimation const&);
};
} // namespace Tron

#endif // MOVEMENTESTIMATION_H
