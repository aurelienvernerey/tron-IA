#ifndef DIRECTION_H
#define DIRECTION_H

#include <istream>
#include <ostream>
#include <string>

namespace Board
{
const short UP      = 0;
const short RIGHT   = 1;
const short DOWN    = 2;
const short LEFT    = 3;
const short FIXED   = -1;

class Direction
{
private:
    short value;

public:
    Direction (short = FIXED);
    Direction (std::string);

    Direction operator - () const;
    Direction& operator += (Direction const&);
    Direction& operator -= (Direction const&);
    friend Direction operator + (Direction const&, Direction const&);
    friend Direction operator + (Direction const&, short const&);
    friend Direction operator + (short const&, Direction const&);
    friend Direction operator - (Direction const&, Direction const&);
    friend Direction operator - (Direction const&, short const&);
    friend Direction operator - (short const&, Direction const&);
    friend bool operator == (Direction const&, Direction const&);
    friend bool operator == (Direction const&, short const&);
    friend bool operator == (short const&, Direction const&);
    friend bool operator != (Direction const&, Direction const&);
    friend bool operator != (Direction const&, short const&);
    friend bool operator != (short const&, Direction const&);
    friend std::ostream& operator << (std::ostream&, Direction const&);
    friend std::istream& operator >> (std::istream&, Direction&);
    operator short (void);

    short getValue () const;
    std::string toString () const;
};
} // namespace Board
#endif // DIRECTION_H
