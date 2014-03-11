#include "../noreturnzone.h"

namespace Tron
{
NoReturnZone::NoReturnZone(Board::Coordinate _entrance, int _size)
    :entrance(_entrance), size(_size)
{
}

bool operator == (NoReturnZone const& a, NoReturnZone const& b)
{
    return a.size == b.size;
}

bool operator != (NoReturnZone const& a, NoReturnZone const& b)
{
    return !(a == b);
}

bool operator > (NoReturnZone const&a , NoReturnZone const& b)
{
    return a.size > b.size;
}

bool operator < (NoReturnZone const&a , NoReturnZone const& b)
{
    return a.size < b.size;
}

bool operator >= (NoReturnZone const&a , NoReturnZone const& b)
{
    return !(a.size < b.size);
}

bool operator <= (NoReturnZone const&a , NoReturnZone const& b)
{
    return !(a.size > b.size);
}
} // namespace Tron
