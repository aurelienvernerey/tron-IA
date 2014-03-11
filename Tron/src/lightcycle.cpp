#include "../lightcycle.h"

namespace Tron
{
LightCycle::LightCycle (Board::GameBoard<TronBattleSquare> *_board)
    : board(_board), loose(false)
{
}

std::ostream& operator << (std::ostream& stream, LightCycle const& obj)
{
    stream << obj.toString();

    return stream;
}

std::istream& operator >> (std::istream& stream, LightCycle& obj)
{
    Board::Coordinate initialPosition, actualPosition;

    stream >> initialPosition >> actualPosition;
    obj.justLoose = false;

    if (initialPosition.x == -1) {
        if (!obj.loose)  {
            obj.loose = true;
            obj.justLoose = true;

            for (unsigned int i = 0; i < obj.lightCycle.size(); ++i) {
                (*obj.board)(obj.lightCycle[i].x, obj.lightCycle[i].y, TronBattleSquare()); // tmp free square
            }
        }
    }
    else {
        if (obj.lightCycle.size() == 0 && initialPosition != actualPosition) {
            (*obj.board)(++initialPosition.x, ++initialPosition.y, TronBattleSquare(obj.id)); // tmp ocupe square
            obj.lightCycle.push_back(initialPosition);
        }

        (*obj.board)(++actualPosition.x, ++actualPosition.y, TronBattleSquare(obj.id)); // tmp ocupe square
        obj.lightCycle.push_back(actualPosition);
    }

    return stream;
}

std::string LightCycle::toString () const
{
    std::stringstream s;

    s << "player " << this->id << ": ";

    if (this->loose) {
        s << " LOOSED" << std::endl;
    }
    else {
        s << this->lightCycle.back() << " " << this->orientation << std::endl;
    }

    return s.str();
}

bool LightCycle::canMove () const
{
    for (int i = 0; i < 4; i++) {
        if ((*this->board)(this->lightCycle.back() + i)) {
            return true;
        }
    }

    return false;
}
} // namespace Tron
