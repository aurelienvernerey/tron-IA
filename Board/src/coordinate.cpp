#include "../coordinate.h"

namespace Board {

    Coordinate::Coordinate (int _x, int _y)
        : x(_x), y(_y)
    {
    }

    Coordinate Coordinate::operator+ (Direction const& direction) const
    {
        return getNextCoordinate(direction);
    }

    Coordinate Coordinate::operator- (Direction const& direction) const
    {
        return getNextCoordinate(-direction);
    }

    Coordinate& Coordinate::operator+= (Direction const& direction)
    {
        return this->move(direction);
    }

    Coordinate& Coordinate::operator-= (Direction const& direction)
    {
        return this->move(-direction);
    }

    bool operator == (Coordinate const& a, Coordinate const& b)
    {
        return (a.x == b.x && a.y == b.y);
    }

    bool operator != (Coordinate const& a, Coordinate const& b)
    {
        return !(a == b);
    }

    std::ostream& operator << (std::ostream& stream, Coordinate const& obj)
    {
        stream << obj.toString();
        return stream;
    }

    std::istream& operator >> (std::istream& stream, Coordinate &obj)
    {
        stream >> obj.x >> obj.y;

        return stream;
    }

    std::string Coordinate::toString () const
    {
        std::stringstream s;
        s << "{" << this->x << "," << this->y << "}";

        return s.str();
    }

    Direction Coordinate::getDirectionTo (Coordinate arrival) {
        for (int direction = 0; direction < 4; direction++) {
            if(arrival == (*this) + direction) {
                return direction;
            }
        }

        return FIXED;
    }

    Coordinate& Coordinate::getNextCoordinate (Direction const& direction) const
    {
        Coordinate newCoordinate(this->x, this->y);

        return newCoordinate.move(direction);
    }

    Coordinate& Coordinate::move (Direction const& direction)
    {
        switch (direction.getValue()) {
        case UP:
            --this->y;
            break;
        case RIGHT:
            ++this->x;
            break;
        case DOWN:
            ++this->y;
            break;
        case LEFT:
            --this->x;
            break;
        }

        return *this;
    }
}
