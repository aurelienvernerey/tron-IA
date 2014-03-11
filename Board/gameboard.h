#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <ostream>
#include <sstream>
#include "Board/coordinate.h"
#include <vector>
#include <queue>

namespace Board
{
template<typename T>
class GameBoard
{
protected:
    T** content;
    int width;
    int height;
    int freeSquareRemaining;

public:
    GameBoard (int = 0, int = 0);
    GameBoard (GameBoard<T> const&);
    template<typename U>
    GameBoard (GameBoard<U> const&);
    ~GameBoard ();

    GameBoard<T>& operator = (GameBoard<T> const&);
    T operator () (int, int) const;
    T operator () (int, int, T);
    T operator () (Coordinate) const;
    T operator () (Coordinate, T);

    template<typename T2>
    friend std::ostream& operator << (std::ostream&, GameBoard<T2> const&);

    void setBorder (T value, int borderWidth = 1);
    std::string toString () const;
    GameBoard<bool> *toBooleanBoard ();
    template<typename U, typename V>
    void getBoardCopy (U **&newBoard, V **oldBoard, int _width, int _height);
    template<typename U>
    void setContent (U **_content, int _width, int _height, int _freeSquareRemaining);
    int getAdjacentLiberty (Coordinate position);
    int getLiberty (Coordinate position, int recursionLevel = 1, bool firstCanBeOccuped = false);
    std::vector<Coordinate> getWayTo (Coordinate departure, Coordinate arrival, int maxWaySize = 100);
    std::vector<Coordinate> getWayToNearestPoint (Coordinate departure, const std::vector<Coordinate> &arrivals, int maxWaySize = 100);
    int getDistanceBetween (Coordinate departure, Coordinate arrival, int **cache = NULL);
    void getDistanceToEveryWhere (Coordinate departure, int** cache);
private:
    void copyBoardContent (GameBoard<T> const&);
    int getRecursiveLiberty (bool** board, Coordinate position, int recursionLevel = 1, int actualRecursionLevel = 1);
    int calculateLiberty (bool** board, Coordinate position, bool firstCanBeOccuped = false, int liberty = 0);
    template<typename U>
    void deleteBoard (U** board, int _width, int _height);

    std::vector<Coordinate> calculateWayToNearestPoint (bool** board, std::vector<std::vector<Coordinate> > *possibleWays, const std::vector<Coordinate> &arrivals, int maxWaySize = 100);
    int calculateDistanceTo (bool** board, std::vector<Coordinate> *positions, Coordinate arrival, int** cache = NULL, int waySize = 1);
};

template<typename T>
GameBoard<T>::GameBoard (int _width, int _height)
    :width(_width), height(_height), freeSquareRemaining(_width * _height), content(NULL)
{
    if (this->width > 0 && this->height > 0) {
        this->content = new T*[this->height];

        for (int y = 0; y < this->height; ++y) {
            this->content[y] = new T[this->width];

            for (int x = 0; x < this->width; ++x) {
                if (!this->content[y][x]) {
                    --this->freeSquareRemaining;
                }
            }
        }
    }
    else {
        this->width = 0;
        this->height = 0;
    }
}

template<typename T>
GameBoard<T>::GameBoard (GameBoard<T> const& obj)
    :content(NULL)
{
    this->copyBoardContent(obj);
}

template<typename T>
template<typename U>
GameBoard<T>::GameBoard (GameBoard<U> const& obj)
    :content(NULL)
{
    this->copyBoardContent(obj);
}

template<typename T>
GameBoard<T>::~GameBoard ()
{
    if (this->content) {
        this->deleteBoard(this->content, this->width, this->height);
    }
}

template<typename T>
template<typename U>
void GameBoard<T>::deleteBoard (U** board, int _width, int _height) {
    for (int y = 0; y < _height; ++y) {
        delete[] board[y];
    }

    delete[] board;
}

template<typename T>
GameBoard<T>& GameBoard<T>::operator = (GameBoard<T> const& obj)
{
    if (this->content) {
        this->~GameBoard();
    }

    this->copyBoardContent(obj);

    return *this;
}

template<typename T>
T GameBoard<T>::operator () (int x, int y) const
{
    return this->content[y][x];
}

template<typename T>
T GameBoard<T>::operator () (int x, int y, T value)
{
    if(this->content[y][x]  && !value) {
        --this->freeSquareRemaining;
    }
    else if (!this->content[y][x]  && value) {
        ++this->freeSquareRemaining;
    }

    this->content[y][x]  = value;

    return value;
}

template<typename T>
T GameBoard<T>::operator () (Coordinate coord) const
{
    return this->content[coord.y][coord.x];
}

template<typename T>
T GameBoard<T>::operator () (Coordinate coord, T value)
{
    return (*this)(coord.x, coord.y, value);
}

template<typename T2>
std::ostream& operator << (std::ostream& stream, GameBoard<T2> const& obj)
{
    stream << obj.toString();

    return stream;
}

template<typename T>
std::string GameBoard<T>::toString () const
{
    std::stringstream s;

    s << "square remaining: " << this->freeSquareRemaining << std::endl << std::endl;

    for (int y = 0; y < this->height; ++y) {
        for (int x = 0; x < this->width; ++x) {
            s << this->content[y][x] << " ";
        }
        s << std::endl;
    }

    return s.str();
}

template<typename T>
void GameBoard<T>::setBorder (T value, int borderWidth)
{
    for (int j = 0; j < this->height; ++j) {
        for (int i = 0; i < this->width; ++i) {
            if (j < borderWidth || j >= this->height - borderWidth ||
                    i < borderWidth ||  i >= this->width - borderWidth) {
                (*this)(i, j, value);
            }
        }
    }
}

template<typename T>
template<typename U>
void GameBoard<T>::setContent (U **_content, int _width, int _height, int _freeSquareRemaining)
{
    this->~GameBoard();
    this->width = _width;
    this->height = _height;
    this->freeSquareRemaining =  _freeSquareRemaining;
    if (_content) {
        this->getBoardCopy(this->content, _content, _width, _height);
    }
}

template<typename T>
GameBoard<bool>* GameBoard<T>::toBooleanBoard ()
{
    GameBoard<bool> *board = new GameBoard<bool>(0, 0);
    board->setContent(this->content, this->width, this->height, this->freeSquareRemaining);

    return board;
}

template<typename T>
void GameBoard<T>::copyBoardContent (const GameBoard<T> &obj)
{
    this->setContent(obj.content, obj.width, obj.height, obj.freeSquareRemaining);
}

template<typename T>
template<typename U, typename V>
void GameBoard<T>::getBoardCopy (U **&newBoard, V **oldBoard, int _width, int _height)
{
    newBoard = new U*[_height];

    for (int y = 0; y < _height; ++y) {
        newBoard[y] = new U[_width];

        for (int x = 0; x < _width; ++x) {
            newBoard[y][x] = static_cast<U> (oldBoard[y][x]);
        }
    }
}

template<typename T>
int GameBoard<T>::getAdjacentLiberty (Coordinate position)
{
    int liberty = 0;
    for (int i = 0; i < 4; ++i) {
        if ((*this)(position + i)) {
            ++liberty;
        }
    }

    return liberty;
}

template<typename T>
int GameBoard<T>::getLiberty (Coordinate position, int recursionLevel, bool firstCanBeOccuped) {
    if ((*this)(position) || firstCanBeOccuped) {
        bool** board;
        this->getBoardCopy(board, this->content, this->width, this->height);
        board[position.y][position.x] = false;
        int liberty = this->getRecursiveLiberty(board, position, recursionLevel);
        this->deleteBoard(board, this->width, this->height);
        return liberty;
    }

    return 0;
}

template<typename T>
int GameBoard<T>::getRecursiveLiberty (bool** board, Coordinate position, int recursionLevel, int actualRecursionLevel) {
    std::priority_queue<int> liberties;
    if (recursionLevel != actualRecursionLevel)  {
        for (int d = 0; d < 4; ++d) {
            Coordinate coord = position + d;
            if (board[coord.y][coord.x]) {
                bool** newBoard;
                this->getBoardCopy(newBoard, board, this->width, this->height);
                newBoard[coord.y][coord.x] = false;

                liberties.push(this->getRecursiveLiberty(newBoard, coord, recursionLevel, actualRecursionLevel + 1));
                this->deleteBoard(newBoard, this->width, this->height);
            }
        }
    }
    else {
        liberties.push(this->calculateLiberty(board, position, true) + actualRecursionLevel - 1);
    }

    if (liberties.empty())  {
        return actualRecursionLevel - 1;
    }

    return liberties.top();
}

template<typename T>
int GameBoard<T>::calculateLiberty (bool **board, Coordinate position, bool firstCanBeOccuped, int liberty) {
    if (board[position.y][position.x] || firstCanBeOccuped && liberty == 0) {
        ++liberty;
        board[position.y][position.x] = false;

        liberty = calculateLiberty(board, position + UP, firstCanBeOccuped, liberty);
        liberty = calculateLiberty(board, position + DOWN, firstCanBeOccuped, liberty);
        liberty = calculateLiberty(board, position + LEFT, firstCanBeOccuped, liberty);
        liberty = calculateLiberty(board, position + RIGHT, firstCanBeOccuped, liberty);
    }

    return liberty;
}

template<typename T>
std::vector<Coordinate> GameBoard<T>::calculateWayToNearestPoint (bool** board, std::vector<std::vector<Coordinate> > *possibleWays, const std::vector<Coordinate> &arrivals, int maxWaySize) {
    if (maxWaySize > 0) {
        std::vector<std::vector<Coordinate> > *nextPossibleWays = new std::vector<std::vector<Coordinate> >;

        for (int i = 0; i < possibleWays->size(); i++) {
            for (int direction = 0; direction < 4; direction++) {
                Coordinate coord = (*possibleWays)[i].back() + direction;

                for (int arrival = 0; arrival < arrivals.size(); arrival++) {
                    if(coord == arrivals[arrival]) {
                        std::vector<Coordinate> way = (*possibleWays)[i];
                        way.push_back(coord);
                        way.erase(way.begin());

                        delete nextPossibleWays;
                        delete possibleWays;

                        return way;
                    }
                }

                if (board[coord.y][coord.x]) {
                    board[coord.y][coord.x] = false;
                    nextPossibleWays->push_back((*possibleWays)[i]);
                    nextPossibleWays->back().push_back(coord);
                }
            }
        }

        delete possibleWays;

        if (!nextPossibleWays->empty()) {
            return this->calculateWayToNearestPoint(board, nextPossibleWays, arrivals, --maxWaySize);
        }

        delete nextPossibleWays;
    }

    std::vector<Coordinate> noWay;

    return noWay;
}

template<typename T>
int GameBoard<T>::calculateDistanceTo (bool** board, std::vector<Coordinate> *positions, Coordinate arrival, int **cache, int waySize) {
    std::vector<Coordinate> *nextPositions = new std::vector<Coordinate>;

    for (int i = 0; i < positions->size(); i++) {
        for (int direction = 0; direction < 4; direction++) {
            Coordinate coord = (*positions)[i] + direction;

            if (board[coord.y][coord.x]) {
                board[coord.y][coord.x] = false;
                nextPositions->push_back(coord);

                if (cache != NULL) {
                                cache[coord.y][coord.x] = waySize;
                            }
            }

            if(coord == arrival) {
                if (cache != NULL) {
                                cache[coord.y][coord.x] = waySize;
                            }
                delete nextPositions;
                delete positions;
                return waySize;
            }
        }
    }

    if (!nextPositions->empty()) {
        delete positions;
        return this->calculateDistanceTo(board, nextPositions, arrival, cache, waySize + 1);
    }

    delete nextPositions;
    delete positions;
    if (cache != NULL && arrival.y > 0 && arrival.y < this->height && arrival.x > 0 && arrival.x < this->width) {
        cache[arrival.y][arrival.x] = -1;
    }
    return -1;
}

template<typename T>
std::vector<Coordinate> GameBoard<T>::getWayTo (Coordinate departure, Coordinate arrival, int maxWaySize) {
    bool** board;
    this->getBoardCopy(board, this->content, this->width, this->height);
    std::vector<std::vector<Coordinate> > *possibleWays = new std::vector<std::vector<Coordinate> >;
    std::vector<Coordinate> way;
    way.push_back(departure);
    possibleWays->push_back(way);

    std::vector<Coordinate> arrivals;
    arrivals.push_back(arrival);

    std::vector<Coordinate> wayTo =  this->calculateWayToNearestPoint(board, possibleWays, arrivals, maxWaySize);
    this->deleteBoard(board, this->width, this->height);

    return wayTo;
}

template<typename T>
std::vector<Coordinate> GameBoard<T>::getWayToNearestPoint (Coordinate departure, const std::vector<Coordinate> &arrivals, int maxWaySize) {
    bool** board;
    this->getBoardCopy(board, this->content, this->width, this->height);
    std::vector<std::vector<Coordinate> > *possibleWays = new std::vector<std::vector<Coordinate> >;
    std::vector<Coordinate> way;
    way.push_back(departure);
    possibleWays->push_back(way);

    std::vector<Coordinate> nearestWay = this->calculateWayToNearestPoint(board, possibleWays, arrivals, maxWaySize);
    this->deleteBoard(board, this->width, this->height);

    return nearestWay;
}

template<typename T>
int GameBoard<T>::getDistanceBetween (Coordinate departure, Coordinate arrival, int** cache) {
    bool** board;
    this->getBoardCopy(board, this->content, this->width, this->height);
    std::vector<Coordinate> *positions = new std::vector<Coordinate>;
    positions->push_back(departure);

    int distance = this->calculateDistanceTo (board, positions, arrival, cache);
    this->deleteBoard(board, this->width, this->height);

    return distance;
}

template<typename T>
void GameBoard<T>::getDistanceToEveryWhere (Coordinate departure, int** cache) {
    bool** board;
    this->getBoardCopy(board, this->content, this->width, this->height);
    std::vector<Coordinate> *positions = new std::vector<Coordinate>;
    positions->push_back(departure);
    this->calculateDistanceTo (board, positions, Coordinate(-1, -1), cache);
    this->deleteBoard(board, this->width, this->height);
}
} // namespace Board

#endif // GAMEBOARD_H
