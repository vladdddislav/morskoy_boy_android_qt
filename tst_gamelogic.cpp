#include <QtTest>
#include <QCoreApplication>
#include "classes.h"

class TestBattleship : public QObject
{
    Q_OBJECT

private slots:
    void testPointCreationAndBounds();
    void testPointEquality();
    void testShipValidCreation();
    void testShipInvalidCreation();
    void testShipShooting();
    void testGameLogicPlacement();
    void testGameLogicInvalidPlacement();
    void testGameLogicShooting();
    void testBotRandomizeShips();
};

void TestBattleship::testPointCreationAndBounds()
{
    Point p;
    p.getPoint(5, 5);
    QCOMPARE(p.getX(), 5);
    QCOMPARE(p.getY(), 5);

    QVERIFY_EXCEPTION_THROWN(p.getPoint(0, 5), std::invalid_argument);
    QVERIFY_EXCEPTION_THROWN(p.getPoint(5, 11), std::invalid_argument);
}

void TestBattleship::testPointEquality()
{
    Point p1, p2, p3;
    p1.getPoint(3, 4);
    p2.getPoint(3, 4);
    p3.getPoint(4, 4);

    QVERIFY(p1 == p2);
    QVERIFY(!(p1 == p3));
}

void TestBattleship::testShipValidCreation()
{
    Ship s;
    Point p1, p2, p3;
    p1.getPoint(1, 1);
    p2.getPoint(1, 2);
    p3.getPoint(1, 3);

    s.getShip(3, {p1, p2, p3});

    QCOMPARE(s.getSize(), 3);
    QVERIFY(s.isAlive());
    QVERIFY(s.hasCoordinate(p2));
}

void TestBattleship::testShipInvalidCreation()
{
    Ship s;
    Point p1, p2;
    p1.getPoint(1, 1);
    p2.getPoint(2, 2);

    QVERIFY_EXCEPTION_THROWN(s.getShip(2, {p1, p2}), std::invalid_argument);

    Point p3, p4;
    p3.getPoint(1, 1);
    p4.getPoint(1, 3);

    QVERIFY_EXCEPTION_THROWN(s.getShip(2, {p3, p4}), std::invalid_argument);
}

void TestBattleship::testShipShooting()
{
    Ship s;
    Point p1, p2;
    p1.getPoint(5, 5);
    p2.getPoint(5, 6);
    s.getShip(2, {p1, p2});

    Point missPoint;
    missPoint.getPoint(1, 1);

    QCOMPARE(s.shot(missPoint), 1);
    QCOMPARE(s.shot(p1), 0);
    QVERIFY(!s.isKilled());
    QCOMPARE(s.shot(p2), -1);
    QVERIFY(s.isKilled());
    QVERIFY(!s.isAlive());
}

void TestBattleship::testGameLogicPlacement()
{
    gameLogic game;
    Point p1;
    p1.getPoint(1, 1);

    Ship s1;
    s1.getShip(1, {p1});

    game.shipPlacement(s1, true);
    QCOMPARE(game.getPlayerShips().size(), 1);
    QCOMPARE(game.getPlayerCell(0, 0), shipCell);
}

void TestBattleship::testGameLogicInvalidPlacement()
{
    gameLogic game;
    Point p1, p2;
    p1.getPoint(1, 1);
    p2.getPoint(2, 2);

    Ship s1, s2;
    s1.getShip(1, {p1});
    s2.getShip(1, {p2});

    game.shipPlacement(s1, true);

    QVERIFY_EXCEPTION_THROWN(game.shipPlacement(s2, true), std::invalid_argument);
}

void TestBattleship::testGameLogicShooting()
{
    gameLogic game;
    Point p1;
    p1.getPoint(2, 2);
    Ship s1;
    s1.getShip(1, {p1});

    game.shipPlacement(s1, false);

    Point missPoint;
    missPoint.getPoint(10, 10);

    bool isHit = game.move(true, missPoint);
    QVERIFY(!isHit);
    QCOMPARE(game.getEnemyCell(9, 9), miss);

    QVERIFY_EXCEPTION_THROWN(game.move(true, missPoint), std::invalid_argument);

    isHit = game.move(true, p1);
    QVERIFY(isHit);
    QCOMPARE(game.getEnemyCell(1, 1), hitCell);

    QCOMPARE(game.getEnemyCell(2, 2), miss);
}

void TestBattleship::testBotRandomizeShips()
{
    gameLogic game;
    Bot bot;

    bot.randomizeShips(game);

    QCOMPARE(game.getEnemyShips().size(), 10);

    QCOMPARE(game.quantityOfShips(false, 4), 1);
    QCOMPARE(game.quantityOfShips(false, 3), 2);
    QCOMPARE(game.quantityOfShips(false, 2), 3);
    QCOMPARE(game.quantityOfShips(false, 1), 4);
}

QTEST_MAIN(TestBattleship)

#include "tst_gamelogic.moc"