#ifndef TRONBATTLESQUARE_H
#define TRONBATTLESQUARE_H

#include "Board/square.h"

namespace Tron
{
class TronBattleSquare : public Board::Square
{
public:
    int playerId;
    bool isWall;
public:
    TronBattleSquare(int _playerId = -1, bool _isWall = false);

    operator bool();
    std::string toString () const;
};
} // namespace Tron

#endif // TRONBATTLESQUARE_H
