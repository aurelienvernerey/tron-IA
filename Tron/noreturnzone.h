#ifndef NORETURNZONE_H
#define NORETURNZONE_H

#include "Board/coordinate.h"

namespace Tron
{
class NoReturnZone
{
public: // tmp
    Board::Coordinate entrance;
    int size;
public:
    NoReturnZone(Board::Coordinate _entrance, int _size);

    friend bool operator == (NoReturnZone const&, NoReturnZone const&);
    friend bool operator != (NoReturnZone const&, NoReturnZone const&);
    friend bool operator > (NoReturnZone const&, NoReturnZone const&);
    friend bool operator < (NoReturnZone const&, NoReturnZone const&);
    friend bool operator >= (NoReturnZone const&, NoReturnZone const&);
    friend bool operator <= (NoReturnZone const&, NoReturnZone const&);
};
} // namespace Tron

#endif // NORETURNZONE_H
