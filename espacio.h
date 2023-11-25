#ifndef ESPACIO_H
#define ESPACIO_H

#include <QObject>
#include <QWidget>
#include <QTime>
#include <QTimer>

class Espacio : public QWidget
{
    Q_OBJECT
public:
    explicit Espacio(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent * Event) override;


    int scoreCount = 0;
    int coinCount = 0;
    bool weaknessOn = false;

    double powerUpTimer = 0;

    bool gameOver = false;
    bool gameWon = false;
    bool gamePaused = false;
    bool gameStarted = false;

    int pm_x = 10;
    int pm_y = 15;

    int g_x = 10;
    int g_y = 9;

signals:
    void testSignal();

public slots:

    bool pm_move(int key);
    void g_move();
    //bool canMove(int key);
    void ifCoin();
    void restart();

//signals:
    //void testSignal();

private:
    QColor wall_color;
    QColor coin_color;
    QColor cherry_color;
    QColor blank_color;
    QColor player_color;
    QColor ghost_color;

    char new_espacio[21][21] =        {{'.','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','.'},
                                     {'.','#','o','o','o','o','o','o','o','o','#','o','o','o','o','o','o','o','o','#','.'},
                                     {'.','#','o','#','#','o','#','#','#','o','#','o','#','#','#','o','#','#','o','#','.'},
                                     {'.','#','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o','#','.'},
                                     {'.','#','o','#','#','o','#','o','#','#','#','#','#','o','#','o','#','#','o','#','.'},
                                     {'.','#','o','o','o','o','#','o','o','o','#','o','o','o','#','o','o','o','o','#','.'},
                                     {'.','#','#','#','#','o','#','#','#','.','#','.','#','#','#','o','#','#','#','#','.'},
                                     {'.','.','.','.','#','o','#','.','.','.','.','.','.','.','#','o','#','.','.','.','.'},
                                     {'#','#','#','#','#','o','#','.','#','#','.','#','#','.','#','o','#','#','#','#','#'},
                                     {'.','.','.','.','.','o','.','.','#','.','.','.','#','.','.','o','.','.','.','.','.'},
                                     {'#','#','#','#','#','o','#','.','#','#','#','#','#','.','#','o','#','#','#','#','#'},
                                     {'.','.','.','.','#','o','#','.','.','.','0','.','.','.','#','o','#','.','.','.','.'},
                                     {'.','#','#','#','#','o','#','.','#','#','#','#','#','.','#','o','#','#','#','#','.'},
                                     {'.','#','o','o','o','o','o','o','o','o','#','o','o','o','o','o','o','o','o','#','.'},
                                     {'.','#','o','#','#','o','#','#','#','o','#','o','#','#','#','o','#','#','o','#','.'},
                                     {'.','#','o','o','#','o','o','o','o','o','o','o','o','o','o','o','#','o','o','#','.'},
                                     {'.','#','#','o','#','o','#','o','#','#','#','#','#','o','#','o','#','o','#','#','.'},
                                     {'.','#','o','o','o','o','#','o','o','o','#','o','o','o','#','o','o','o','o','#','.'},
                                     {'.','#','o','#','#','#','#','#','#','o','#','o','#','#','#','#','#','#','o','#','.'},
                                     {'.','#','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o','#','.'},
                                     {'.','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','.'}};

    char espacioAscii[21][21];
    int espacioWidth = 21;
    int espacioHeight = 21;

    const int espacioSize = espacioHeight * 42; //block size


    char g_direction = 'u'; // 'u', 'd', 'r', 'l'.
};

#endif // ESPACIO_H
