#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <vector>
#include "Board/Board"
#include "lightcycle.h"
#include "tronbattlesquare.h"

namespace Tron
{
class TronBattle
{
public:
    short playerRemaining;
    short playerNumber;
    std::vector<LightCycle> players;
    bool aPlayerJustLoose;
    //int freeSquareRemaining; //tmp
    Board::GameBoard<TronBattleSquare>* board;
public:
    TronBattle(int width, int height, int _playerNumber);

    friend std::ostream& operator << (std::ostream&, TronBattle const&);
    friend std::istream& operator >> (std::istream&, TronBattle&);

    std::string toString () const;
};
} // namespace Tron

#endif // GAME_H
