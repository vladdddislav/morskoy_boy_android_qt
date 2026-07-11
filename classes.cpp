
#include "classes.h"
#include <stdexcept>
#include <algorithm>
#include <cstdlib>
#include <cmath>

int Point::getX() const { return x; }
int Point::getY() const { return y; }

void Point::getPoint(int vvedX, int vvedY) {
    if(vvedX < 1 || vvedY < 1) {
        throw std::invalid_argument("Error. Point cannot be that small");
    }
    if(vvedX > 10 || vvedY > 10) {
        throw std::invalid_argument("Error. Point cannot be that big");
    }
    x = vvedX;
    y = vvedY;
}

bool Point::operator ==(const Point &other) const {
    if (x != other.x) {
        return false;
    } else if (y != other.y) {
        return false;
    } else {
        return true;
    }
}

bool comparePointsConst(const Point &a, const Point &b) {
    if(a.getX() != b.getX()) {
        return a.getX() < b.getX();
    }
    return a.getY() < b.getY();
}

bool Ship::operator==(const Ship& other) const {
    if (size != other.size) {
        return false;
    }
    if (lives != other.lives) {
        return false;
    }
    if (alive != other.alive) {
        return false;
    }
    if (!(coordinates == other.coordinates)) {
        return false;
    }
    int i = 0;
    while (i < coordinates.size()) {
        if (stateOfCoordinates[i] != other.stateOfCoordinates[i]) {
            return false;
        }
        i = i + 1;
    }
    return true;
}

void Ship::checkSize(int size) {
    if(size > 4 || size < 1) {
        throw std::invalid_argument("Error size of the ship. Try again");
    }
}

void Ship::sortCoordinates(vector<Point>& inpCoordinates) {
    if(inpCoordinates.size() == 1) {
        return;
    }
    if(inpCoordinates.size() < 1) {
        throw std::invalid_argument("Size cannot be less than 0");
    } else {
        std::sort(inpCoordinates.begin(), inpCoordinates.end(), comparePointsConst);
    }
}

void Ship::getCoordinates (vector<Point> inpCoordinates) {
    sortCoordinates(inpCoordinates);
    if(inpCoordinates.size() == 1) {
        coordinates = inpCoordinates;
    } else {
        int i = 1;
        if (inpCoordinates[i].getX() == inpCoordinates[i - 1].getX()) {
            while(i < inpCoordinates.size() && inpCoordinates[i].getX() == inpCoordinates[i - 1].getX()) {
                int dif = inpCoordinates[i].getY() - inpCoordinates[i - 1].getY();
                if(dif != 1) {
                    throw std::invalid_argument("Incorrect placement");
                }
                i = i + 1;
            }
            if(i == inpCoordinates.size()) {
                coordinates = inpCoordinates;
            }
        } else if(inpCoordinates[i].getY() == inpCoordinates[i - 1].getY()) {
            while(i < inpCoordinates.size() && (inpCoordinates[i].getY() == inpCoordinates[i - 1].getY())) {
                int dif = inpCoordinates[i].getX() - inpCoordinates[i - 1].getX();
                if(dif != 1) {
                    throw std::invalid_argument("Incorrect placement");
                }
                i = i + 1;
            }
            if(i == inpCoordinates.size()) {
                coordinates = inpCoordinates;
            }
        }
    }
    if(!(coordinates == inpCoordinates)) {
        throw std::invalid_argument("Error. Ships coordinates (x or y) must be const");
    }
}

void Ship::getShip(int vvedSize, vector<Point> vvedCoordinates) {
    getCoordinates(vvedCoordinates);
    checkSize(vvedSize);
    size = vvedSize;
    lives = size;
    alive = true;
    stateOfCoordinates.clear();
    int i = 0;
    while(i < coordinates.size()) {
        stateOfCoordinates.push_back(true);
        i = i + 1;
    }
}

bool Ship::isHit(Point a) {
    int i = 0;
    while (i < coordinates.size()) {
        if(coordinates[i].getX() == a.getX()) {
            if(coordinates[i].getY() == a.getY()) {
                return true;
            }
        }
        i = i + 1;
    }
    return false;
}

bool Ship::isKilled() {
    if(lives == 0) {
        return true;
    } else {
        return false;
    }
}

int Ship::getSimilarCoordinatePlace(Point a) {
    int i = 0;
    while (i < coordinates.size()) {
        if(a == coordinates[i]) {
            return i;
        }
        i = i + 1;
    }
    return -1;
}

int Ship::shot(Point b) {
    if(isHit(b)) {
        lives = lives - 1;
        int i = getSimilarCoordinatePlace(b);
        stateOfCoordinates[i] = false;
        if(isKilled()) {
            alive = false;
            return -1;
        }
        return 0;
    }
    return 1;
}

bool Ship::hasCoordinate(Point p) const {
    int i = 0;
    while (i < coordinates.size()) {
        if(coordinates[i].getX() == p.getX()) {
            if(coordinates[i].getY() == p.getY()) {
                return true;
            }
        }
        i = i + 1;
    }
    return false;
}

bool gameLogic::isClose(Ship korabl, bool isPlayers) {
    vector<Point> korablCells = korabl.getCoordinates();
    int i = 0;
    vector<Ship> ships;
    if(isPlayers) {
        ships = playerShips;
    } else {
        ships = enemyShips;
    }

    bool isProhibited = false;
    while(i < ships.size() && !isProhibited) {
        int j = 0;
        vector<Point> shipCells = ships[i].getCoordinates();
        while(j < korabl.getCoordinates().size() && !isProhibited) {
            int k = 0;
            while(k < ships[i].getCoordinates().size() && !isProhibited) {
                int korabl_x = korablCells[j].getX();
                int korabl_y = korablCells[j].getY();
                int ship_x = shipCells[k].getX();
                int ship_y = shipCells[k].getY();
                int x = abs(korabl_x - ship_x);
                int y = abs(korabl_y - ship_y);
                if(x <= 1 && y <= 1) {
                    isProhibited = true;
                }
                k = k + 1;
            }
            j = j + 1;
        }
        i = i + 1;
    }
    return isProhibited;
}

int gameLogic::findShipPos(Ship ship, bool isPlayers) {
    int i = 0;
    if(isPlayers) {
        while(i < playerShips.size()) {
            if(ship == playerShips[i]) {
                return i;
            }
            i =  i + 1;
        }
    } else {
        while(i < enemyShips.size()) {
            if(ship == enemyShips[i]) {
                return i;
            }
            i =  i + 1;
        }
    }
    return -1;
}

void gameLogic::outline(Ship ship, bool isPlayers) {
    int pos = findShipPos(ship, isPlayers);
    if(pos == -1) {
        throw std::invalid_argument("No such ship in the game");
    }
    vector<Point> currCoordinates;
    if (isPlayers) {
        currCoordinates = playerShips[pos].getCoordinates();
    } else {
        currCoordinates = enemyShips[pos].getCoordinates();
    }
    int minX = currCoordinates.front().getX() - 1;
    int maxX = currCoordinates.back().getX() - 1;
    int minY = currCoordinates.front().getY() - 1;
    int maxY = currCoordinates.back().getY() - 1;
    int leftBorderX   = std::max(0, minX - 1);
    int rightBorderX  = std::min(9, maxX + 1);
    int topBorderY    = std::max(0, minY - 1);
    int bottomBorderY = std::min(9, maxY + 1);
    int x = leftBorderX;
    while (x <= rightBorderX) {
        int y = topBorderY;
        while (y <= bottomBorderY) {
            Point checkPoint;
            checkPoint.getPoint(x + 1, y + 1);
            if (!ship.hasCoordinate(checkPoint)) {
                if (isPlayers) {
                    playerField[x][y] = miss;
                } else {
                    enemyField[x][y] = miss;
                }
            }
            y = y + 1;
        }
        x = x + 1;
    }
}

int gameLogic::quantityOfShips(bool isPlayers, int givenDeck) {
    if(givenDeck < 1 || givenDeck > 4) {
        throw std::invalid_argument("The ship cannot have such quantity of decks");
    }
    int i = 0;
    int count = 0;
    vector<Ship> currShips;
    if(isPlayers) {
        currShips = playerShips;
    } else {
        currShips = enemyShips;
    }
    while (i < currShips.size()) {
        if(currShips[i].getSize() == givenDeck) {
            count = count + 1;
        }
        i = i + 1;
    }
    return count;
}

vector<Ship> gameLogic::getPlayerShips() {
    return playerShips;
}

vector<Ship> gameLogic::getEnemyShips() {
    return enemyShips;
}

bool gameLogic::checkWin(bool isPlayers) {
    int count = 0;
    int i = 0;
    if(isPlayers) {
        while (i < playerShips.size()) {
            if(playerShips[i].isKilled()) {
                count = count + 1;
            }
            i = i + 1;
        }
        if(count == 10) {
            return true;
        }
        return false;
    } else {
        while (i < enemyShips.size()) {
            if(enemyShips[i].isKilled()) {
                count = count + 1;
            }
            i = i + 1;
        }
        if(count == 10) {
            return true;
        }
        return false;
    }
    return false;
}

void gameLogic::shipPlacement(Ship korabl, bool isPlayers) {
    if(isClose(korabl, isPlayers)) {
        throw std::invalid_argument("Ships cannot be placed that close");
    } else {
        vector<int> deckVec = {4, 3, 2, 1};
        int size = korabl.getSize();
        int currQ = quantityOfShips(isPlayers, size);
        if(currQ == deckVec[size -1]) {
            throw std::invalid_argument("Impossble to add that ship. Too much of such ones");
        }
        if(isPlayers) {
            int i = 0;
            while(i < size) {
                Point currPoint = korabl.getCoordinates()[i];
                playerField[(currPoint.getX() - 1)][(currPoint.getY() - 1)] = shipCell;
                i = i + 1;
            }
            playerShips.push_back(korabl);
        } else {
            int i = 0;
            while(i < size) {
                Point currPoint = korabl.getCoordinates()[i];
                enemyField[(currPoint.getX() - 1)][(currPoint.getY() - 1)] = shipCell;
                i = i + 1;
            }
            enemyShips.push_back(korabl);
        }
    }
}

bool gameLogic::move(bool isPlayers, Point chosenPoint) {
    int x = chosenPoint.getX() - 1;
    int y = chosenPoint.getY() - 1;
    if(isPlayers) {
        int state = enemyField[x][y];
        if(state == 3 || state == 1) {
            throw std::invalid_argument("You cannot fire in the cell which was already fired");
        } else if (state == 0) {
            enemyField[x][y] = miss;
            return false;
        } else if(state == 2) {
            enemyField[x][y] = hitCell;
            int i = 0;
            while(i < enemyShips.size()) {
                if(enemyShips[i].hasCoordinate(chosenPoint)) {
                    enemyShips[i].shot(chosenPoint);
                    if(enemyShips[i].isKilled()) {
                        outline(enemyShips[i], false);
                    }
                }
                i = i + 1;
            }
            return true;
        }
    } else {
        int state = playerField[x][y];
        if(state == 3 || state == 1) {
            throw std::invalid_argument("You cannot fire in the cell which was already fired");
        } else if (state == 0) {
            playerField[x][y] = miss;
            return false;
        } else if(state == 2) {
            playerField[x][y] = hitCell;
            int i = 0;
            while(i < playerShips.size()) {
                if(playerShips[i].hasCoordinate(chosenPoint)) {
                    playerShips[i].shot(chosenPoint);
                    if(playerShips[i].isKilled()) {
                        outline(playerShips[i], true);
                    }
                }
                i = i + 1;
            }
            return true;
        }
    }
    return false;
}

Bot::Bot() {}

void Bot::randomizeShips(gameLogic &game) {
    int shipSizes[10] = {4, 3, 3, 2, 2, 2, 1, 1, 1, 1};
    int i = 0;
    while (i < 10) {
        int currentSize = shipSizes[i];
        bool placed = false;
        int attempts = 0;
        while (!placed && attempts < 1000) {
            int dir = std::rand() % 2;
            int startX = (dir == 0) ? (std::rand() % (10 - currentSize + 1)) + 1 : (std::rand() % 10) + 1;
            int startY = (dir == 1) ? (std::rand() % (10 - currentSize + 1)) + 1 : (std::rand() % 10) + 1;
            std::vector<Point> points;
            int p = 0;
            while (p < currentSize) {
                Point pt;
                pt.getPoint(dir == 0 ? startX + p : startX, dir == 1 ? startY + p : startY);
                points.push_back(pt);
                p = p + 1;
            }
            try {
                Ship newShip;
                newShip.getShip(currentSize, points);
                game.shipPlacement(newShip, false);
                placed = true;
            } catch (...) {
                attempts = attempts + 1;
            }
        }
        i = i + 1;
    }
}

Point Bot::makeMove(gameLogic &game) {
    bool validShot = false;
    Point shotPoint;
    int attempts = 0;
    while (!validShot && attempts < 1000) {
        shotPoint.getPoint((std::rand() % 10) + 1, (std::rand() % 10) + 1);
        try {
            game.move(false, shotPoint);
            validShot = true;
        } catch (...) {
            attempts = attempts + 1;
        }
    }
    return shotPoint;
}