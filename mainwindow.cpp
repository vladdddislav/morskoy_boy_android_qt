
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);
    setFixedSize(800, 500);

    setupMenu();
    setupPlacement();
    setupGame();

    stackedWidget->addWidget(menuWidget);
    stackedWidget->addWidget(placementWidget);
    stackedWidget->addWidget(gameWidget);
    stackedWidget->setCurrentWidget(menuWidget);
}

void MainWindow::setupMenu() {
    menuWidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(menuWidget);

    QLabel *title = new QLabel("Морской бой", menuWidget);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 24px; font-weight: bold;");

    QPushButton *btnPvE = new QPushButton("Игра против компьютера", menuWidget);
    QPushButton *btnPvP = new QPushButton("Игра друг против друга", menuWidget);

    connect(btnPvE, &QPushButton::clicked, [this]() {
        isPvE = true;
        game.clear();
        startPlacement(true);
    });
    connect(btnPvP, &QPushButton::clicked, [this]() {
        isPvE = false;
        game.clear();
        startPlacement(true);
    });

    layout->addStretch();
    layout->addWidget(title);
    layout->addWidget(btnPvE);
    layout->addWidget(btnPvP);
    layout->addStretch();
}

void MainWindow::setupPlacement() {
    placementWidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(placementWidget);

    infoLabel = new QLabel("", placementWidget);
    infoLabel->setAlignment(Qt::AlignCenter);
    infoLabel->setStyleSheet("font-size: 16px;");

    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->setSpacing(2);

    int i = 0;
    while (i < 10) {
        int j = 0;
        while (j < 10) {
            QPushButton *btn = new QPushButton();
            btn->setFixedSize(35, 35);
            gridLayout->addWidget(btn, i, j);
            placementGrid[i][j] = btn;
            connect(btn, &QPushButton::clicked, [this, i, j]() {
                handlePlacementClick(i, j);
            });
            j = j + 1;
        }
        i = i + 1;
    }

    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->addStretch();
    hLayout->addLayout(gridLayout);
    hLayout->addStretch();

    layout->addWidget(infoLabel);
    layout->addLayout(hLayout);
}

void MainWindow::setupGame() {
    gameWidget = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(gameWidget);

    turnLabel = new QLabel("", gameWidget);
    turnLabel->setAlignment(Qt::AlignCenter);
    turnLabel->setStyleSheet("font-size: 18px; font-weight: bold;");
    mainLayout->addWidget(turnLabel);

    QHBoxLayout *boardsLayout = new QHBoxLayout();
    QGridLayout *g1 = new QGridLayout();
    QGridLayout *g2 = new QGridLayout();
    g1->setSpacing(2);
    g2->setSpacing(2);

    int i = 0;
    while (i < 10) {
        int j = 0;
        while (j < 10) {
            p1Grid[i][j] = new QPushButton();
            p1Grid[i][j]->setFixedSize(30, 30);
            g1->addWidget(p1Grid[i][j], i, j);
            connect(p1Grid[i][j], &QPushButton::clicked, [this, i, j]() {
                handleGameClick(true, i, j);
            });

            p2Grid[i][j] = new QPushButton();
            p2Grid[i][j]->setFixedSize(30, 30);
            g2->addWidget(p2Grid[i][j], i, j);
            connect(p2Grid[i][j], &QPushButton::clicked, [this, i, j]() {
                handleGameClick(false, i, j);
            });
            j = j + 1;
        }
        i = i + 1;
    }

    QVBoxLayout *leftLayout = new QVBoxLayout();
    p1TitleLabel = new QLabel("", gameWidget);
    leftLayout->addWidget(p1TitleLabel);
    leftLayout->addLayout(g1);

    QVBoxLayout *rightLayout = new QVBoxLayout();
    p2TitleLabel = new QLabel("", gameWidget);
    rightLayout->addWidget(p2TitleLabel);
    rightLayout->addLayout(g2);

    boardsLayout->addLayout(leftLayout);
    boardsLayout->addSpacing(50);
    boardsLayout->addLayout(rightLayout);

    mainLayout->addLayout(boardsLayout);
}

void MainWindow::startPlacement(bool p1) {
    isPlacingPlayer1 = p1;
    shipsToPlace = {4, 3, 3, 2, 2, 2, 1, 1, 1, 1};
    currentShipIndex = 0;
    currentPlacementPoints.clear();
    updatePlacementGrid();
    stackedWidget->setCurrentWidget(placementWidget);
}

void MainWindow::updatePlacementGrid() {
    QString msg = "";
    if (isPvE) {
        msg = "Игрок: ";
    } else {
        if (isPlacingPlayer1) {
            msg = "Игрок 1: ";
        } else {
            msg = "Игрок 2: ";
        }
    }

    msg = msg + QString("Расстановка корабля (%1-палубный)").arg(shipsToPlace[currentShipIndex]);
    infoLabel->setText(msg);

    int i = 0;
    while (i < 10) {
        int j = 0;
        while (j < 10) {
            cellState state;
            if (isPlacingPlayer1) {
                state = game.getPlayerCell(i, j);
            } else {
                state = game.getEnemyCell(i, j);
            }

            if (state == shipCell) {
                placementGrid[i][j]->setStyleSheet("background-color: gray;");
            } else {
                placementGrid[i][j]->setStyleSheet("background-color: lightblue;");
            }
            j = j + 1;
        }
        i = i + 1;
    }

    int k = 0;
    while (k < currentPlacementPoints.size()) {
        Point p = currentPlacementPoints[k];
        placementGrid[p.getX() - 1][p.getY() - 1]->setStyleSheet("background-color: yellow;");
        k = k + 1;
    }
}

void MainWindow::handlePlacementClick(int x, int y) {
    Point p;
    try {
        p.getPoint(x + 1, y + 1);
    } catch (...) {
        return;
    }

    bool isFound = false;
    int idx = 0;
    while (idx < currentPlacementPoints.size() && !isFound) {
        if (currentPlacementPoints[idx] == p) {
            isFound = true;
        }
        idx = idx + 1;
    }

    if (!isFound) {
        currentPlacementPoints.push_back(p);
        updatePlacementGrid();

        if (currentPlacementPoints.size() == (size_t)shipsToPlace[currentShipIndex]) {
            try {
                Ship s;
                s.getShip(shipsToPlace[currentShipIndex], currentPlacementPoints);
                game.shipPlacement(s, isPlacingPlayer1);
                currentPlacementPoints.clear();
                currentShipIndex = currentShipIndex + 1;
                updatePlacementGrid();

                if (currentShipIndex == 10) {
                    if (isPvE) {
                        bot.randomizeShips(game);
                        startGame();
                    } else {
                        if (isPlacingPlayer1) {
                            startPlacement(false);
                        } else {
                            startGame();
                        }
                    }
                }
            } catch (...) {
                currentPlacementPoints.clear();
                updatePlacementGrid();
            }
        }
    }
}

void MainWindow::startGame() {
    isPlayer1Turn = true;

    if (isPvE) {
        turnLabel->setText("Ваш ход!");
        p1TitleLabel->setText("Поле Игрока");
        p2TitleLabel->setText("Поле Компьютера");
    } else {
        turnLabel->setText("Ход Игрока 1");
        p1TitleLabel->setText("Поле Игрока 1");
        p2TitleLabel->setText("Поле Игрока 2");
    }

    updateGameGrids();
    stackedWidget->setCurrentWidget(gameWidget);
}

bool MainWindow::isKilled(bool isPlayer, int x, int y) {
    Point pt;
    pt.getPoint(x + 1, y + 1);

    std::vector<Ship> ships;
    if (isPlayer) {
        ships = game.getPlayerShips();
    } else {
        ships = game.getEnemyShips();
    }

    bool foundKilled = false;
    int idx = 0;
    while (idx < ships.size() && !foundKilled) {
        if (ships[idx].isKilled() && ships[idx].hasCoordinate(pt)) {
            foundKilled = true;
        }
        idx = idx + 1;
    }
    return foundKilled;
}

void MainWindow::updateGameGrids() {
    QString stEmpty = "background-color: lightblue;";
    QString stShip = "background-color: gray;";
    QString stMiss = "background-color: black;";
    QString stHit = "background-color: orange;";
    QString stKill = "background-color: red;";

    int i = 0;
    while (i < 10) {
        int j = 0;
        while (j < 10) {

            cellState s1 = game.getPlayerCell(i, j);
            if (s1 == miss) {
                p1Grid[i][j]->setStyleSheet(stMiss);
            } else if (s1 == hitCell) {
                if (isKilled(true, i, j)) {
                    p1Grid[i][j]->setStyleSheet(stKill);
                } else {
                    p1Grid[i][j]->setStyleSheet(stHit);
                }
            } else if (s1 == shipCell) {
                if (isPvE) {
                    p1Grid[i][j]->setStyleSheet(stShip);
                } else {
                    p1Grid[i][j]->setStyleSheet(stEmpty);
                }
            } else {
                p1Grid[i][j]->setStyleSheet(stEmpty);
            }

            cellState s2 = game.getEnemyCell(i, j);
            if (s2 == miss) {
                p2Grid[i][j]->setStyleSheet(stMiss);
            } else if (s2 == hitCell) {
                if (isKilled(false, i, j)) {
                    p2Grid[i][j]->setStyleSheet(stKill);
                } else {
                    p2Grid[i][j]->setStyleSheet(stHit);
                }
            } else if (s2 == shipCell) {
                p2Grid[i][j]->setStyleSheet(stEmpty);
            } else {
                p2Grid[i][j]->setStyleSheet(stEmpty);
            }
            j = j + 1;
        }
        i = i + 1;
    }

    if(!isPvE) {
        if (isPlayer1Turn) {
            turnLabel->setText("Ход Игрока 1");
        } else {
            turnLabel->setText("Ход Игрока 2");
        }
    }
}

void MainWindow::handleGameClick(bool clickedP1Grid, int x, int y) {
    Point pt;
    try {
        pt.getPoint(x + 1, y + 1);
    } catch (...) {
        return;
    }

    try {
        if (isPvE) {
            if (!clickedP1Grid) {
                bool hit = game.move(true, pt);
                updateGameGrids();
                if (game.checkWin(false)) {
                    endGame("Игрок победил!");
                } else if (!hit) {
                    doBotTurn();
                }
            }
        } else {
            if (isPlayer1Turn && !clickedP1Grid) {
                bool hit = game.move(true, pt);
                updateGameGrids();
                if (game.checkWin(false)) {
                    endGame("Игрок 1 победил!");
                } else if (!hit) {
                    isPlayer1Turn = false;
                }
            } else if (!isPlayer1Turn && clickedP1Grid) {
                bool hit = game.move(false, pt);
                updateGameGrids();
                if (game.checkWin(true)) {
                    endGame("Игрок 2 победил!");
                } else if (!hit) {
                    isPlayer1Turn = true;
                }
            }
            updateGameGrids();
        }
    } catch (...) {}
}

void MainWindow::doBotTurn() {
    bool botTurn = true;
    while (botTurn && !game.checkWin(true)) {
        Point pt = bot.makeMove(game);
        if (game.getPlayerCell(pt.getX() - 1, pt.getY() - 1) == miss) {
            botTurn = false;
        }
    }
    updateGameGrids();
    if (game.checkWin(true)) {
        endGame("Компьютер победил!");
    }
}

void MainWindow::endGame(const QString& winner) {
    QMessageBox::information(this, "Конец игры", winner);
    stackedWidget->setCurrentWidget(menuWidget);
}