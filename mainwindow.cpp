#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "field.h"
#include <QTimer>
#include <QKeyEvent>
#include <QtDebug>

// Constructor de la clase MainWindow
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Configuración de la interfaz de usuario
    ui->setupUi(this);

    // Creación de un temporizador para manejar el juego
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerTick()));

    // Configuración del enfoque de los botones
    this->ui->Start->setFocusPolicy(Qt::NoFocus);
    this->ui->Pause->setFocusPolicy(Qt::NoFocus);
    this->ui->Quit->setFocusPolicy(Qt::NoFocus);

    // Creación del campo de juego y agregarlo al diseño vertical
    field = new Field;
    ui->verticalLayout->addWidget(field);
}

// Destructor de la clase MainWindow
MainWindow::~MainWindow()
{
    delete ui;
    delete timer;
}

// Método invocado en cada tick del temporizador
void MainWindow::timerTick() {
    // Si el juego está pausado, no hacer nada
    if (field->gamePaused) return;

    // Verificar si se ha alcanzado el límite de monedas para ganar el juego
    if (field->coinCount > 150) onGameWin();

    // Actualizar el tiempo y la interfaz de usuario
    time += 1;
    ui->time->setNum(time / 10.0);
    update();

    // Verificar y recoger monedas o cerezas
    this->field->ifCoin();

    // Actualizar la etiqueta de puntuación
    ui->label->setNum(this->field->scoreCount);

    // Manejar el tiempo de debilidad del jugador
    if (this->field->weaknessOn == true) {
        this->field->powerUpTimer += 0.1;
        if (this->field->powerUpTimer >= 10.0) {
            this->field->weaknessOn = false;
        } else {
            update();
        }
    }

    // Mover al jugador (Pacman) en la dirección especificada
    if (this->field->pm_move(nextKey)) {
        currentKey = nextKey;
    } else {
        this->field->pm_move(currentKey);
    }

    // Mover al fantasma en intervalos regulares
    if (time * 10 % 3 == 0) {
        field->g_move();
    }

    // Verificar colisión entre el jugador y el fantasma
    if (field->pm_x == field->g_x && field->pm_y == field->g_y) {
        // Si el jugador está en modo débil, reiniciar la posición del fantasma y otorgar puntos
        if (field->weaknessOn) {
            field->g_x = 10;
            field->g_y = 9;
            field->scoreCount += 200;
            field->weaknessOn = false;
        } else {
            // Si no, terminar el juego
            endGame();
        }
    }
}

// Manejar el evento de presionar una tecla
void MainWindow::keyPressEvent(QKeyEvent *event) {
    nextKey = event->key();
}

// Método para finalizar el juego
void MainWindow::endGame() {
    field->gameOver = true;
    timer->stop();
    update();
}

// Método para cerrar el juego
void MainWindow::quitGame() {
    this->~MainWindow();
}

// Método invocado al ganar el juego
void MainWindow::onGameWin() {
    field->gameWon = true;
    timer->stop();
    update();
}

// Método para pausar o reanudar el juego
void MainWindow::pauseGame() {
    if (field->gamePaused) {
        ui->Pause->setText("Pause");
        field->gamePaused = false;
    } else {
        ui->Pause->setText("Unpause");
        field->gamePaused = true;
    }
}

// Método para reiniciar el juego
void MainWindow::resetGame() {
    field->restart();
    timer->stop();
    currentKey = 0;
    nextKey = 0;
}

// Manejar el clic en el botón de pausa
void MainWindow::on_Pause_clicked() {
    pauseGame();
}

// Manejar el clic en el botón de salir
void MainWindow::on_Quit_clicked() {
    quitGame();
}

// Manejar el clic en el botón de inicio/reinicio
void MainWindow::on_Start_clicked() {
    if (field->gameStarted) {
        // Si el juego está en curso, detener y reiniciar
        field->gameStarted = false;
        resetGame();
        ui->Start->setText("Start");
    } else {
        // Si no, iniciar el juego
        field->gameStarted = true;
        ui->Start->setText("Reset");
        timer->start(150); // Tiempo especificado en ms
    }
}
