#include "FUNCS.h"
#include <pthread.h>
#include <stdio.h>

// Función para inicializar Pac-Man
void initPacMan(PacMan *pacman, int startX, int startY) {
    pacman->x = startX;
    pacman->y = startY;
    pacman->lives = 3;
    pacman->score = 0;
    pacman->symbol = '@';
    maze[pacman->x][pacman->y] = pacman->symbol;
    pthread_mutex_init(&pacman->lock, NULL);
}

// Función para verificar si el movimiento a realizar es válido.
int isValidMove(int newX, int newY) {
    // Asegurarse que los límites del laberinto no se sobrepasen.
    if (newX >= 0 && newX < 11 && newY >= 0 && newY < 31) {
        return (maze[newX][newY] != '\xDB');
    }
    return 0;
}

void pacManMovement(PacMan *pacman, char direction) {
    pthread_mutex_lock(&pacman->lock);

    int newX = pacman->x;
    int newY = pacman->y;

    switch (direction) {
        case 'w': newX--; break;    // Arriba
        case 's': newX++; break;    // Abajo
        case 'a': newY--; break;    // Izquierda
        case 'd': newY++; break;    // Derecha
        default:
            printf("Entrada no válida.\n");
            pthread_mutex_unlock(&pacman->lock);
            return;
    }

    pthread_mutex_lock(&mazeLock);

    if (isValidMove(newX, newY)) {
        // Verifica si Pac-Man colisiona con un fantasma
        if (maze[newX][newY] == '$') {  // Supone que el fantasma tiene el símbolo '$'
            pacman->lives--;  // Restar una vida a Pac-Man
            printf("¡Pac-Man ha sido atrapado por un fantasma! Vidas restantes: %d\n", pacman->lives);

            if (pacman->lives == 0) {
                printf("¡Pac-Man ha perdido todas sus vidas! Fin del juego.\n");
                exit(0);  // Termina el juego
            }
        } else {
            // Actualizar posición de Pac-Man en el laberinto
            if (maze[newX][newY] == '.') {
                pacman->score++;
            }

            maze[pacman->x][pacman->y] = ' ';  // Deja espacio vacío
            pacman->x = newX;
            pacman->y = newY;
            maze[pacman->x][pacman->y] = pacman->symbol;
        }
    }

    if (pacman->score == 115) {
        printf("¡Pac-Man ha recogido todas las píldoras! ¡Victoria!\n");
        exit(0);  // Termina el juego
    }

    pthread_mutex_unlock(&mazeLock);
    pthread_mutex_unlock(&pacman->lock);
}
