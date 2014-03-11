#include "../movementestimation.h"

namespace Tron
{
MovementEstimation::MovementEstimation(Board::Direction _direction, int _liberty)
    :direction(_direction), liberty(_liberty)
{
}

bool operator == (MovementEstimation const& a, MovementEstimation const& b)
{
    return a.liberty == b.liberty;
}

bool operator != (MovementEstimation const& a, MovementEstimation const& b)
{
    return !(a == b);
}

bool operator > (MovementEstimation const&a , MovementEstimation const& b)
{
    return a.liberty > b.liberty;
}

bool operator < (MovementEstimation const&a , MovementEstimation const& b)
{
    return a.liberty < b.liberty;
}

bool operator >= (MovementEstimation const&a , MovementEstimation const& b)
{
    return !(a.liberty < b.liberty);
}

bool operator <= (MovementEstimation const&a , MovementEstimation const& b)
{
    return !(a.liberty > b.liberty);
}
} // namespace Tron
