
#ifndef CLASSES_H
#define CLASSES_H
#include <string>
#include <vector>

using namespace std;

class Point {
private:
    int x;
    int y;
public:
    int getX() const;
    int getY() const;
    void getPoint(int vvedX, int vvedY);
    bool operator ==(const Point& other) const;
};

bool comparePointsConst(const Point& a, const Point& b);

class Ship {
private:
    int size;
    int lives;
    bool alive;
    vector <Point> coordinates;
    vector <bool> stateOfCoordinates;
    void checkSize(int size);
    void sortCoordinates(vector<Point>& inpCoordinates);
    void getCoordinates(vector<Point> inpCoordinates);
    bool isHit(Point a);
    int getSimilarCoordinatePlace(Point a);
public:
    bool operator==(const Ship& other) const;
    void getShip(int vvedSize, vector<Point> vvedCoordinates);
    int shot(Point b);
    bool isKilled();
    bool isAlive() { return alive; }
    int getSize() { return size; }
    const vector<Point>& getCoordinates() const { return coordinates; }
    const vector<bool>& getStateOfCoordinates() const { return stateOfCoordinates; }
    bool hasCoordinate(Point p) const;
};

enum cellState {
    empty,
    miss,
    shipCell,
    hitCell
};

class gameLogic {
private:
    vector<Ship> playerShips;
    vector<Ship> enemyShips;
    cellState playerField [10][10] = {};
    cellState enemyField[10][10] = {};
    bool isClose(Ship korabl, bool isPlayers);
public:
    int findShipPos(Ship ship, bool isPlayers);
    void outline(Ship ship, bool isPlayers);
    int quantityOfShips(bool isPlayers, int givenDeck);
    vector<Ship> getPlayerShips();
    vector<Ship> getEnemyShips();
    bool checkWin(bool isPlayers);
    void shipPlacement(Ship korabl, bool isPlayers);
    bool move(bool isPlayers, Point chosenPoint);

    cellState getPlayerCell(int x, int y) { return playerField[x][y]; }
    cellState getEnemyCell(int x, int y) { return enemyField[x][y]; }
    void clear() {
        playerShips.clear();
        enemyShips.clear();
        int i = 0;
        while (i < 10) {
            int j = 0;
            while (j < 10) {
                playerField[i][j] = {};
                enemyField[i][j] = {};
                j = j + 1;
            }
            i = i + 1;
        }
    }
};

class Bot {
public:
    Bot();
    void randomizeShips(gameLogic &game);
    Point makeMove(gameLogic &game);
};

#endif