#include "../direction.h"

namespace Board {

    Direction::Direction (short _value)
        : value(_value)
    {
        if (this->value > 3) {
            value %= 4;
        }
    }

    Direction::Direction (std::string value)
    {
        if (value == "UP" || value == "up") {
            this->value = UP;
        }
        else if (value == "RIGHT" || value == "right") {
            this->value = RIGHT;
        }
        else if (value == "DOWN" || value == "down") {
            this->value = DOWN;
        }
        else if (value == "LEFT" || value == "left") {
            this->value = LEFT;
        }
        else {
            this->value = FIXED;
        }
    }

    Direction Direction::operator - () const
    {
        Direction opposite = *this;

        opposite.value += 2;
        opposite.value %= 4;

        return opposite;
    }

    Direction& Direction::operator += (Direction const& a)
    {
        this->value += a.value;
        this->value %= 4;

        return *this;
    }

    Direction& Direction::operator -= (Direction const& a)
    {
        return (*this += (-a));
    }

    Direction operator + (Direction const& a, Direction const& b)
    {
        return Direction((a.value + b.value));
    }

    Direction operator + (Direction const& a, short const& b)
    {
        return a + (Direction) b;
    }

    Direction operator + (short const& a, Direction const& b)
    {
        return (Direction)a +  b;
    }

    Direction operator - (Direction const& a, Direction const& b)
    {
        return (a + (-b));
    }

    Direction operator - (Direction const& a, short const& b)
    {
        return a - (Direction) b;
    }

    Direction operator - (short const& a, Direction const& b)
    {
        return (Direction)a -  b;
    }

    bool operator == (Direction const& a, Direction const& b)
    {
        return (a.value == b.value);
    }

    bool operator == (Direction const& a, short const& b)
    {
        return (a == (Direction) b);
    }

    bool operator == (short const& a, Direction const& b)
    {
        return ((Direction) a == b);
    }

    bool operator != (Direction const& a, Direction const& b)
    {
        return !(a == b);
    }

    bool operator != (Direction const& a, short const& b)
    {
        return !(a == b);
    }

    bool operator != (short const& a, Direction const& b)
    {
        return !(a == b);
    }

    std::ostream& operator << (std::ostream& stream, Direction const& obj)
    {
        stream << obj.toString();
        return stream;
    }

    std::istream& operator >> (std::istream& stream, Direction &obj)
    {
        std::string  input;
        stream >> input;
        obj = Direction(input);

        return stream;
    }

    Direction::operator short (void)
    {
        return this->value;
    }

    short Direction::getValue () const
    {
        return this->value;
    }

    std::string Direction::toString () const
    {
        switch (this->value) {
        case UP:
            return "UP";
        case RIGHT:
            return "RIGHT";
        case DOWN:
            return "DOWN";
        case LEFT:
            return "LEFT";
        case FIXED:
        default:
            return "FIXED";
        }
    }
}
