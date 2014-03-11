#ifndef SQUARE_H
#define SQUARE_H

#include "ostream"
#include "sstream"

namespace Board
{
class Square
{
public:
    Square();

    friend std::ostream& operator << (std::ostream&, Square const&);
    virtual operator bool() = 0;

    virtual std::string toString () const = 0;
};
} // namespace Board

#endif // SQUARE_H
