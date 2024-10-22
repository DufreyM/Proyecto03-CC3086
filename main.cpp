// main.cpp
#include <iostream>
#include <vector>
#include <ncurses.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <Map.h>

using namespace std;

int level = 1;
int GHOST_WAIT = 750000;
bool gameEnded = false;
sem_t mapWatcher;  // Controla el ciclo del juego

vector<vector<char>> mapa = {
    {'\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB'},
    {'\xDB', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '\xDB', '\xDB', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '\xDB'},
    {'\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB'},
    {'\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB'},
    {'\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB'},
    {'\xDB', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '\xDB'},
    {'\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB'},
    {'\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB'},
    {'\xDB', '.', '.', '.', '.', '.', '.', '\xDB', '\xDB', '.', '.', '.', '.', '\xDB', '\xDB', '.', '.', '.', '.', '\xDB', '\xDB', '.', '.', '.', '.', '.', '.', '\xDB'},
    {'\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', ' ', '\xDB', '\xDB', ' ', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB'},
    {' ', ' ', ' ', ' ', ' ', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', ' ', '\xDB', '\xDB', ' ', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', '\xDB', '.', '\xDB', '\xDB', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\xDB', '\xDB', '.', '\xDB', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', '\xDB', '.', '\xDB', '\xDB', ' ', '\xDB', '\xDB', '\xDB', '-', '-', '\xDB', '\xDB', '\xDB', ' ', '\xDB', '\xDB', '.', '\xDB', ' ', ' ', ' ', ' ', ' '},
    {'\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', ' ', '\xDB', '\xDB', ' ', ' ', ' ', ' ', '\xDB', '\xDB', ' ', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB'},
    {'\xDB', ' ', ' ', ' ', ' ', ' ', '.', ' ', ' ', ' ', '\xDB', '\xDB', '1', '2', '3', '4', '\xDB', '\xDB', ' ', ' ', ' ', '.', ' ', ' ', ' ', ' ', ' ', '\xDB'},
    {'\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', ' ', '\xDB', '\xDB', ' ', ' ', ' ', ' ', '\xDB', '\xDB', ' ', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB'},
    {' ', ' ', ' ', ' ', ' ', '\xDB', '.', '\xDB', '\xDB', ' ', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', ' ', '\xDB', '\xDB', '.', '\xDB', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', '\xDB', '.', '\xDB', '\xDB', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\xDB', '\xDB', '.', '\xDB', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', '\xDB', '.', '\xDB', '\xDB', ' ', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', ' ', '\xDB', '\xDB', '.', '\xDB', ' ', ' ', ' ', ' ', ' '},
    {'\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', ' ', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', ' ', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB'},
    {'\xDB', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '\xDB', '\xDB', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '\xDB'},
    {'\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB'},
    {'\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB'},
    {'\xDB', '.', '.', '.', '\xDB', '\xDB', '.', '.', '.', '.', '.', '.', '.', '@', '.', '.', '.', '.', '.', '.', '.', '.', '\xDB', '\xDB', '.', '.', '.', '\xDB'},
    {'\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '.', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '.', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB'},
    {'\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '.', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '.', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB'},
    {'\xDB', '.', '.', '.', '.', '.', '.', '\xDB', '\xDB', '.', '.', '.', '.', '\xDB', '\xDB', '.', '.', '.', '.', '\xDB', '\xDB', '.', '.', '.', '.', '.', '.', '\xDB'},
    {'\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB'},
    {'\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB', '\xDB', '.', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '.', '\xDB'},
    {'\xDB', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '\xDB'},
    {'\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB', '\xDB'}
};

Map* gameMap = nullptr;  // Usamos puntero para un control de memoria explícito

void initializeNcurses() {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);
}

void* pacManRoutine(void* arg) {
    int keyInput;
    gameMap->mostrarMapa();  // Mostrar el mapa inicial
    keyInput = getch();  // Leer entrada del usuario

    while (keyInput != 113 && keyInput != 99) {

        sem_wait(&mapWatcher);
        switch (keyInput) {
            case KEY_UP: gameMap->movePacman(-1, 0); break;
            case KEY_DOWN: gameMap->movePacman(1, 0); break;
            case KEY_LEFT: gameMap->movePacman(0, -1); break;
            case KEY_RIGHT: gameMap->movePacman(0, 1); break;
        }
        if(gameMap->getPacMan().getLives() <= 0) {
            gameMap->gameOver();
            sem_post(&mapWatcher);
        }
        if(gameMap->getPacMan().getScore() >= (2400 * level)) {
            gameMap->newLevel(level);
            sem_post(&mapWatcher);
            gameEnded = true;
        }
        sem_post(&mapWatcher);
        keyInput = getch();
    }
    endwin();
    return nullptr;
}

void* blinkyRoutine(void* arg) {
    refresh();
    while (1) {
        sem_wait(&mapWatcher);

        gameMap->moveBlinky();
   
        sem_post(&mapWatcher);

        if (gameEnded) {
            pthread_exit(NULL);
        }
        usleep(GHOST_WAIT); // Controla la velocidad del movimiento
    }
}

void* pinkyRoutine(void* arg) {
    refresh();
    while (1) {
        sem_wait(&mapWatcher);

        gameMap->movePinky();
   
        sem_post(&mapWatcher);

        if (gameEnded) {
            pthread_exit(NULL);
        }
        usleep(GHOST_WAIT); // Controla la velocidad del movimiento
    }
}

void* inkyRoutine(void* arg) {
    refresh();
    while (1) {
        sem_wait(&mapWatcher);

        gameMap->moveInky();
   
        sem_post(&mapWatcher);

        if (gameEnded) {
            pthread_exit(NULL);
        }
        usleep(GHOST_WAIT); // Controla la velocidad del movimiento
    }
}

void* clydeRoutine(void* arg) {
    refresh();
    while (1) {
        sem_wait(&mapWatcher);

        gameMap->moveClyde();
   
        sem_post(&mapWatcher);

        if (gameEnded) {
            pthread_exit(NULL);
        }
        usleep(GHOST_WAIT); // Controla la velocidad del movimiento
    }
}

int main() {
    pthread_t user,ghostThreads[1];

    sem_init(&mapWatcher, 0, 1);

    // Inicialización de ncurses
    initializeNcurses();

    while(1){
        // Crear el mapa dinámicamente para gestionar mejor la memoria
        gameMap = new Map(mapa);

		clear();

		pthread_create(&user, NULL, pacManRoutine, NULL);
		
        refresh();
		pthread_create(&ghostThreads[0], NULL, blinkyRoutine, NULL);
		usleep(3000000);

        refresh();
		pthread_create(&ghostThreads[1], NULL, pinkyRoutine, NULL);
		usleep(3000000);

        refresh();
		pthread_create(&ghostThreads[2], NULL, inkyRoutine, NULL);
		usleep(3000000);

        refresh();
		pthread_create(&ghostThreads[3], NULL, clydeRoutine, NULL);

		pthread_join(user,NULL);
		for (int i = 0; i < 1; ++i){
			pthread_join(ghostThreads[i], NULL);
		}

		gameEnded = false;
		GHOST_WAIT *= 0.5;
		level++;
	}

    // Limpieza de recursos
    delete gameMap;
    // Finaliza ncurses
    endwin();

    return 0;
}