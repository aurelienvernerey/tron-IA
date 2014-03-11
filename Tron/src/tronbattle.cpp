#include "../tronbattle.h"

namespace Tron
{
TronBattle::TronBattle(int width, int height, int _playerNumber)
    : playerNumber(_playerNumber), playerRemaining(_playerNumber)
{
    this->board = new Board::GameBoard<TronBattleSquare>(width, height);
    this->board->setBorder(TronBattleSquare(NULL, true));

    for (int playerId = 0; playerId < this->playerNumber; playerId++) {
        LightCycle aPlayer(this->board);
        aPlayer.id = playerId;
        this->players.push_back(aPlayer);
    }
}

std::ostream& operator << (std::ostream& stream, TronBattle const& obj)
{
    stream << obj.toString();

    return stream;
}

std::istream& operator >> (std::istream& stream, TronBattle& obj)
{
    obj.aPlayerJustLoose = false;
    for (int playerId = 0; playerId < obj.playerNumber; ++playerId) {
        std::cin >> obj.players[playerId];
        if (obj.players[playerId].justLoose) {
            obj.aPlayerJustLoose = true;//tmp usefull ??
            --obj.playerRemaining;
        }
    }
    return stream;
}

std::string TronBattle::toString () const
{
    std::stringstream s;

    s << "Default_String"; // tmp

    return s.str();
}
} // namespace Tron
