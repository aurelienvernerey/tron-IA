#include "../tronbattlesquare.h"

namespace Tron
{
TronBattleSquare::TronBattleSquare(int _playerID, bool _isWall)
    : playerId(_playerID), isWall(_isWall)
{

}

TronBattleSquare::operator bool ()
{
    return this->playerId == -1 && !this->isWall;
}

std::string TronBattleSquare::toString () const
{
    std::stringstream s;

    if (this->isWall) {
        s << "*";
    }
    else if (this->playerId != -1){
        s << this->playerId;
    }
    else {
        s << " ";
    }

    return s.str();
}
} // namespace Tron
