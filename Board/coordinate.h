#ifndef COORDINATE_H
#define COORDINATE_H

#include <istream>
#include <ostream>
#include <sstream>
#include <string>
#include "direction.h"

namespace Board
{
class Coordinate
{
public:
    int x;
    int y;

public:
    Coordinate(int _x = 0, int _y = 0);
    Coordinate operator+ (Direction const&) const;
    Coordinate operator- (Direction const&) const;
    Coordinate& operator+= (Direction const&);
    Coordinate& operator-= (Direction const&);
    friend bool operator == (Coordinate const&, Coordinate const&);
    friend bool operator != (Coordinate const&, Coordinate const&);
    friend std::ostream& operator << (std::ostream&, Coordinate const&);
    friend std::istream& operator >> (std::istream&, Coordinate&);

    Direction getDirectionTo (Coordinate arrival);
    std::string toString () const;

private:
    Coordinate& getNextCoordinate (Direction const&) const;
    Coordinate& move (Direction const&);
};
} // namespace

#endif // COORDINATE_H
