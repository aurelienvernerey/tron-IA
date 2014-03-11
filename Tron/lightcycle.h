#ifndef LIGHTCYCLE_H
#define LIGHTCYCLE_H

#include <vector>
#include <string>
#include <istream>
#include <ostream>
#include <sstream>
#include "Board/direction.h"
#include "Board/coordinate.h"
#include "Board/gameboard.h"
#include "tronbattlesquare.h"

namespace Tron
{
class LightCycle
{
public:
    short id;
    bool loose;
    bool justLoose;
    Board::GameBoard<TronBattleSquare>* board;
    std::vector<Board::Coordinate> lightCycle;
    Board::Direction orientation;
public:
    LightCycle(Board::GameBoard<TronBattleSquare>* _board);

    friend std::ostream& operator << (std::ostream&, LightCycle const&);
    friend std::istream& operator >> (std::istream&, LightCycle&);

    std::string toString () const;
    bool canMove () const;
};
} // namespace Tron

#endif // LIGHTCYCLE_H
