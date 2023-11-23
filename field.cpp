#include "field.h"
#include <QTime>
#include <QTimer>
#include <QtWidgets>
#include "mainwindow.h"
#include <iostream>
#include <queue>
#include <vector>
#include <utility>
#include <QtDebug>

using namespace std;

// Definición de la clase Field

// Constructor de la clase Field
Field::Field(QWidget *parent) : QWidget(parent)
{
    // Inicialización de colores para los elementos del campo
    wall_color = QColor(0, 0, 255, 255);
    coin_color = QColor(223, 201, 56, 255);
    cherry_color = QColor(255, 0, 0, 255);
    blank_color = QColor(0, 0, 0, 255);
    player_color = QColor(255, 255, 0, 255);
    ghost_color = QColor(255, 102, 255, 255);
    restart();
}

// Método para reiniciar el estado del campo de juego
void Field::restart() {
    memcpy(fieldAscii, new_field, 21 * 21);
    fieldWidth = 21;
    fieldHeight = 21;
    pm_x = 10;
    pm_y = 15;
    g_x = 10;
    g_y = 9;
    scoreCount = 0;
    coinCount = 0;
    weaknessOn = false;
    powerUpTimer = 0;
    gameOver = false;
    gamePaused = false;
    gameWon = false;
}

// Método para manejar el evento de pintura del campo
void Field::paintEvent(QPaintEvent *Event) {
    QPainter painter(this);
    painter.fillRect(0, 0, 630, 630, wall_color);

    for (int i = 0; i < fieldWidth; ++i) {
        for (int j = 0; j < fieldHeight; ++j) {
            switch (fieldAscii[j][i]) {
            case '#':
                painter.fillRect(i * 42, j * 42, 42, 42, wall_color);
                break;

            case 'o':
                painter.fillRect(i * 42, j * 42, 42, 42, blank_color);
                painter.setBrush(coin_color);
                painter.drawEllipse(i * 42 + 12, j * 42 + 12, 18, 18);
                break;

            case '.':
                painter.fillRect(i * 42, j * 42, 42, 42, blank_color);
                break;

            case '0':
                painter.fillRect(i * 42, j * 42, 42, 42, blank_color);
                painter.setBrush(cherry_color);
                painter.drawEllipse(i * 42 + 10, j * 42 + 10, 22, 22);
                break;
            }
        }
    }

    // Dibujar al jugador (Pacman)
    if (int(powerUpTimer * 10) % 2 != 0) {
        painter.fillRect(pm_x * 42, pm_y * 42, 42, 42, Qt::white);
    } else {
        painter.fillRect(pm_x * 42, pm_y * 42, 42, 42, player_color);
    }

    // Dibujar al fantasma
    painter.fillRect(g_x * 42, g_y * 42, 42, 42, blank_color);
    painter.setBrush(ghost_color);
    painter.drawEllipse(g_x * 42 + 6, g_y * 42 + 6, 30, 30);

    // Mostrar mensajes de Game Over o You Won si es el caso
    if (gameOver) {
        QFont font("Arial", 48);
        painter.setFont(font);
        QRectF rect{0, 0, double(fieldSize), double(fieldSize)};
        painter.setPen(Qt::white);
        painter.drawText(rect, Qt::AlignHCenter | Qt::AlignVCenter, "Game Over");
    } else if (gameWon) {
        QFont font("Arial", 48);
        painter.setFont(font);
        QRectF rect{0, 0, double(fieldSize), double(fieldSize)};
        painter.setPen(Qt::white);
        painter.drawText(rect, Qt::AlignHCenter | Qt::AlignVCenter, "You Won!");
    }
}

// Método para verificar si hay una moneda y agregarla al contador
void Field::ifCoin() {
    if (fieldAscii[pm_y][pm_x] == 'o') {
        fieldAscii[pm_y][pm_x] = '.';
        scoreCount++;
        coinCount++;
    } else if (fieldAscii[pm_y][pm_x] == '0') {
        fieldAscii[pm_y][pm_x] = '.';
        this->weaknessOn = true;
    }
}

// Método para mover al jugador en una dirección especificada
bool Field::pm_move(int key) {
    int x, y;
    switch (key) {
    case Qt::Key_Up:
        y = (pm_y - 1 + fieldHeight) % fieldHeight;
        x = pm_x;
        if (fieldAscii[y][x] != '#') {
            pm_y = (pm_y - 1 + fieldHeight) % fieldHeight;
            return true;
        }
        break;

    case Qt::Key_Down:
        y = (pm_y + 1) % fieldHeight;
        x = pm_x;
        if (fieldAscii[y][x] != '#') {
            pm_y = (pm_y + 1) % fieldHeight;
            return true;
        }
        break;

    case Qt::Key_Left:
        y = pm_y;
        x = (pm_x - 1 + fieldWidth) % fieldWidth;
        if (fieldAscii[y][x] != '#') {
            pm_x = (pm_x - 1 + fieldWidth) % fieldWidth;
            return true;
        }
        break;

    case Qt::Key_Right:
        y = pm_y;
        x = (pm_x + 1) % fieldWidth;
        if (fieldAscii[y][x] != '#') {
            pm_x = (pm_x + 1) % fieldWidth;
            return true;
        }
        break;
    }
    return false;
}

// Método para mover al fantasma hacia el jugador
void Field::g_move() {
    int x = g_x;
    int y = g_y;
    int cur_x;
    int cur_y;
    queue<pair<int, int>> order;
    int matrix[fieldHeight][fieldWidth];

    // Inicializar la matriz de búsqueda de camino
    for (int i = 0; i < fieldHeight; i++) {
        for (int j = 0; j < fieldWidth; j++) {
            matrix[i][j] = -1;
        }
    }
    matrix[y][x] = 0;

    // Buscar el camino más corto hacia el jugador
    while (1) {
        // UP
        cur_y = (y - 1 + fieldHeight) % fieldHeight;
        cur_x = x;
        if (fieldAscii[cur_y][cur_x] != '#' && matrix[cur_y][cur_x] == -1) {
            matrix[cur_y][cur_x] = matrix[y][x] + 1;
            order.push(make_pair(cur_y, cur_x));
        }

        // DOWN
        cur_y = (y + 1) % fieldHeight;
        cur_x = x;
        if (fieldAscii[cur_y][cur_x] != '#' && matrix[cur_y][cur_x] == -1) {
            matrix[cur_y][cur_x] = matrix[y][x] + 1;
            order.push(make_pair(cur_y, cur_x));
        }

        // RIGHT
        cur_y = y;
        cur_x = (x + 1) % fieldWidth;
        if (fieldAscii[cur_y][cur_x] != '#' && matrix[cur_y][cur_x] == -1) {
            matrix[cur_y][cur_x] = matrix[y][x] + 1;
            order.push(make_pair(cur_y, cur_x));
        }

        // LEFT
        cur_y = y;
        cur_x = (x - 1 + fieldWidth) % fieldWidth;
        if (fieldAscii[cur_y][cur_x] != '#' && matrix[cur_y][cur_x] == -1) {
            matrix[cur_y][cur_x] = matrix[y][x] + 1;
            order.push(make_pair(cur_y, cur_x));
        }

        // Actualizar las coordenadas actuales
        y = order.front().first;
        x = order.front().second;
        order.pop();

        // Si se llega al jugador, romper el bucle
        if (x == pm_x && y == pm_y) {
            break;
        }
    }

    // Retroceder por el camino más corto
    while (matrix[y][x] != 1) {
        // UP
        cur_y = (y - 1 + fieldHeight) % fieldHeight;
        cur_x = x;
        if (matrix[cur_y][cur_x] == matrix[y][x] - 1) {
            x = cur_x;
            y = cur_y;
            continue;
        }

        // DOWN
        cur_y = (y + 1) % fieldHeight;
        cur_x = x;
        if (matrix[cur_y][cur_x] == matrix[y][x] - 1) {
            x = cur_x;
            y = cur_y;
            continue;
        }

        // RIGHT
        cur_y = y;
        cur_x = (x + 1) % fieldWidth;
        if (matrix[cur_y][cur_x] == matrix[y][x] - 1) {
            x = cur_x;
            y = cur_y;
            continue;
        }

        // LEFT
        cur_y = y;
        cur_x = (x - 1 + fieldWidth) % fieldWidth;
        if (matrix[cur_y][cur_x] == matrix[y][x] - 1) {
            x = cur_x;
            y = cur_y;
            continue;
        }
    }

    // Actualizar las coordenadas del fantasma
    g_y = y;
    g_x = x;
}
