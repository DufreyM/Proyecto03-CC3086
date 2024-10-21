// main.cpp
#include <iostream>
#include <cstdlib>
#include <vector>
#include <ncurses.h>  // ncurses para manejo de pantalla y teclado
#include <Map.h>
#include <pthread.h>
#include <unistd.h>
#include <random>

using namespace std;

pthread_mutex_t gameMutex;
pthread_cond_t gameCondition;

bool gameRunning = true;  // Controla el ciclo del juego
vector<vector<char>> mapa = {
    {'\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB'},
    {'\xDB', '\xDB', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '\xDB', '\xDB', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '\xDB', '\xDB'},
    {'\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB'},
    {'\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB'},
    {'\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB'},
    {'\xDB', '\xDB', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '\xDB', '\xDB'},
    {'\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB'},
    {'\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB'},
    {'\xDB', '\xDB', '.', '.', '.', '.', '.', '.', '\xDB', '\xDB', '.', '.', '.', '.', '\xDB', '\xDB', '.', '.', '.', '.', '\xDB', '\xDB', '.', '.', '.', '.', '.', '.', '\xDB', '\xDB'},
    {'\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', ' ', '\xDB', '\xDB', ' ', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB'},
    {' ', ' ', ' ', ' ', ' ', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', ' ', '\xDB', '\xDB', ' ', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', '\xDB', '\xDB', '.', '\xDB', '\xDB', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\xDB', '\xDB', '.', '\xDB', '\xDB', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', '\xDB', '\xDB', '.', '\xDB', '\xDB', ' ', '\xDB', '\xDB', '\xDB', '-', '-', '\xDB', '\xDB', '\xDB', ' ', '\xDB', '\xDB', '.', '\xDB', '\xDB', ' ', ' ', ' ', ' ', ' '},
    {'\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', ' ', '\xDB', '\xDB', ' ', ' ', ' ', ' ', '\xDB', '\xDB', ' ', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB'},
    {'\xDB', '\xDB', ' ', ' ', ' ', ' ', ' ', '.', ' ', ' ', ' ', '\xDB', '\xDB', '1', '2', '3', '4', '\xDB', '\xDB', ' ', ' ', ' ', '.', ' ', ' ', ' ', ' ', ' ', '\xDB', '\xDB'},
    {'\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', ' ', '\xDB', '\xDB', ' ', ' ', ' ', ' ', '\xDB', '\xDB', ' ', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB'},
    {' ', ' ', ' ', ' ', ' ', '\xDB', '\xDB', '.', '\xDB', '\xDB', ' ', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', ' ', '\xDB', '\xDB', '.', '\xDB', '\xDB', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', '\xDB', '\xDB', '.', '\xDB', '\xDB', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\xDB', '\xDB', '.', '\xDB', '\xDB', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', '\xDB', '\xDB', '.', '\xDB', '\xDB', ' ', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', ' ', '\xDB', '\xDB', '.', '\xDB', '\xDB', ' ', ' ', ' ', ' ', ' '},
    {'\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', ' ', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', ' ', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB'},
    {'\xDB', '\xDB', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '\xDB', '\xDB', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '\xDB', '\xDB'},
    {'\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB'},
    {'\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB'},
    {'\xDB', '\xDB', '.', '.', '.', '\xDB', '\xDB', '.', '.', '.', '.', '.', '.', '@', '.', '.', '.', '.', '.', '.', '.', '.', '.', '\xDB', '\xDB', '.', '.', '.', '\xDB', '\xDB'},
    {'\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '.', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '.', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB'},
    {'\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '.', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '.', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB'},
    {'\xDB', '\xDB', '.', '.', '.', '.', '.', '.', '\xDB', '\xDB', '.', '.', '.', '.', '\xDB', '\xDB', '.', '.', '.', '.', '\xDB', '\xDB', '.', '.', '.', '.', '.', '.', '\xDB', '\xDB'},
    {'\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB'},
    {'\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB'},
    {'\xDB', '\xDB', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '\xDB', '\xDB'},
    {'\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB'}
};
Map gameMap = *(new Map(mapa));


void initializeNcurses() {
    initscr();              // Inicializa ncurses
    noecho();               // No mostrar la entrada del usuario
    cbreak();               // Desactiva el buffer de línea
    keypad(stdscr, TRUE);   // Habilita las teclas especiales (como las flechas)
    curs_set(0);            // Oculta el cursor
}

void* pacManThread(void* arg) {
    while (gameRunning) {
        int input = getch();  // Leer entrada del usuario

        pthread_mutex_lock(&gameMutex);
        switch (input) {
            case KEY_UP: gameMap.movePacman(-1,0); break;
            case KEY_DOWN: gameMap.movePacman(1,0); break;
            case KEY_LEFT: gameMap.movePacman(0,-1); break;
            case KEY_RIGHT: gameMap.movePacman(0,1); break;
            case 'q': gameRunning = false; break;
        }

        pthread_mutex_unlock(&gameMutex);

        usleep(100000);  // Controla la velocidad del movimiento
    }

    return nullptr;
}

void* blinkyThread(void* arg) {
    random_device rd;  // Semilla aleatoria del sistema
    mt19937 gen(rd()); // Mersenne Twister como generador

    uniform_int_distribution<int> distrib(-1, 1);

    while (gameRunning) {
        pthread_mutex_lock(&gameMutex);

        gameMap.moveBlinky(distrib(gen), distrib(gen));

        pthread_mutex_unlock(&gameMutex);

        usleep(1000000);  // Controla la velocidad del movimiento
    }

    return nullptr;
}

void* gameThread(void* arg) {
    while (gameRunning) {

        pthread_mutex_lock(&gameMutex);

        if(gameMap.getPacMan().getLives() == 0) {
            gameMap.gameOver();
            int input = getch();  // Leer entrada del usuario
            if (input == 'e') {
                gameRunning = false;
            }
        }

        pthread_mutex_unlock(&gameMutex);
    }

    return nullptr;
}

int main() {
    // Inicialización de ncurses
    initializeNcurses();

    pthread_t thread1, thread2, thread3;
    pthread_mutex_init(&gameMutex, NULL);
    pthread_cond_init(&gameCondition, NULL);

    gameMap.mostrarMapa();  // Muestra el mapa inicial

    pthread_create(&thread1, NULL, pacManThread, NULL);
    pthread_create(&thread2, NULL, blinkyThread, NULL);
    pthread_create(&thread3, NULL, gameThread, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);

    clear();

    pthread_mutex_destroy(&gameMutex); // Destruir el mutex

    // Finaliza ncurses
    endwin();
    return 0;
}