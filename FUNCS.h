#ifndef FUNCS_H
#define FUNCS_H

#include <pthread.h>

// Definición de la estructura PacMan
typedef struct {
    int x;
    int y;
    int lives;
    int score;
    char symbol;
    pthread_mutex_t lock;
} PacMan;

typedef struct {
    int x;
    int y;
    int lives;
    char symbol;
    char prevChar;
    pthread_mutex_t lock;
} Ghost;

extern pthread_mutex_t mazeLock;

// Prototipos de las funciones
void initPacMan(PacMan *pacman, int startX, int startY);
void initGhost(Ghost *ghost, int startX, int startY);
int isValidMove(int newX, int newY);
void pacManMovement(PacMan *pacman, char direction);
extern char maze[11][31]; // Declaración del laberinto como variable externa
void printMaze(PacMan *pacman);
void ghostMovement(Ghost *ghost, PacMan *pacman);
#endif
