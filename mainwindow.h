
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QLabel>
#include "classes.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

private:
    QStackedWidget *stackedWidget;
    QWidget *menuWidget;
    QWidget *placementWidget;
    QWidget *gameWidget;

    gameLogic game;
    Bot bot;
    bool isPvE;
    bool isPlayer1Turn;
    bool isPlacingPlayer1;

    std::vector<int> shipsToPlace;
    int currentShipIndex;
    std::vector<Point> currentPlacementPoints;

    QPushButton* placementGrid[10][10];
    QPushButton* p1Grid[10][10];
    QPushButton* p2Grid[10][10];
    QLabel* infoLabel;
    QLabel* turnLabel;
    QLabel* p1TitleLabel;
    QLabel* p2TitleLabel;

    void setupMenu();
    void setupPlacement();
    void setupGame();

    void startPlacement(bool p1);
    void updatePlacementGrid();
    void handlePlacementClick(int x, int y);

    void startGame();
    void updateGameGrids();
    void handleGameClick(bool clickedP1Grid, int x, int y);
    void doBotTurn();
    void endGame(const QString& winner);
    bool isKilled(bool isPlayer, int x, int y);
};

#endif