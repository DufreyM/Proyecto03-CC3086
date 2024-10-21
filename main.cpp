#include "FUNCS.h" // Incluye las declaraciones
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h> 
#include <ncurses.h>


// Función para mostrar el menú y elegir el modo de juego
int showMenu() {
    int choice;
    printf("Seleccione el modo de juego:\n");
    printf("1. Un jugador (Pac-Man vs IA Fantasma)\n");
    printf("2. Dos jugadores (Jugador 1 controla Pac-Man, Jugador 2 controla Fantasma)\n");
    printf("Ingrese su opción (1 o 2): ");
    scanf("%d", &choice);
    return choice;
}

// Función del hilo para Pac-Man
void* pacmanThread(void* arg) {
    PacMan *pacman = (PacMan*) arg;
    int input;
    
    while (pacman->lives > 0) {
        input = getch();  // Capturar la entrada sin esperar "Enter"
        
        switch (input) {
            case 'w': case 's': case 'a': case 'd':
                pacManMovement(pacman, input);
                break;
            default:
                break;
        }
        printMaze(pacman);
        usleep(100000);  // Para evitar sobrecargar la CPU
    }

    return NULL;
}

// Función del hilo para el fantasma controlado por IA
void* ghostThreadAI(void* arg) {
    Ghost *ghost = ((Ghost*)(((void**)arg)[0]));
    PacMan *pacman = ((PacMan*)(((void**)arg)[1]));
    
    while (ghost->lives > 0) {
        ghostMovement(ghost, pacman);  // Movimiento automático del fantasma
        printMaze(NULL);
        sleep(1);  // El fantasma se mueve cada 1 segundo
    }

    return NULL;
}

// Función del hilo para el fantasma controlado por el jugador 2
void* ghostThreadPlayer(void* arg) {
    Ghost *ghost = (Ghost*) arg;
    int input;

    while (ghost->lives > 0) {
        // Limpiar la línea anterior antes de mostrar la nueva entrada
        pthread_mutex_lock(&mazeLock);
        mvprintw(13, 0, "Jugador 2 - Ingrese la dirección de movimiento del fantasma (i: arriba, k: abajo, j: izquierda, l: derecha): ");
        refresh();
        pthread_mutex_unlock(&mazeLock);

        // Capturar entrada del jugador 2 sin necesidad de presionar Enter
        input = getch();

        pthread_mutex_lock(&mazeLock);

        int newX = ghost->x;
        int newY = ghost->y;

        switch (input) {
            case 'i': newX--; break;    // Arriba
            case 'k': newX++; break;    // Abajo
            case 'j': newY--; break;    // Izquierda
            case 'l': newY++; break;    // Derecha
            default:
                mvprintw(14, 0, "Entrada no válida.");
                refresh();
                pthread_mutex_unlock(&mazeLock);
                continue;
        }

        if (isValidMove(newX, newY)) {
            maze[ghost->x][ghost->y] = ghost->prevChar;  // Restaurar carácter anterior
            ghost->x = newX;
            ghost->y = newY;
            ghost->prevChar = maze[newX][newY];  // Guardar lo que había en la nueva posición
            maze[ghost->x][ghost->y] = ghost->symbol;  // Colocar al fantasma
        }

        pthread_mutex_unlock(&mazeLock);
        printMaze(NULL);  // Mostrar laberinto después del movimiento
    }

    return NULL;
}


int main() {
    initscr();  // Iniciar ncurses
    noecho();   // No mostrar las teclas presionadas
    curs_set(FALSE);  // Ocultar el cursor

    srand(time(NULL));

    PacMan pacman;
    Ghost ghost1;
    
    initPacMan(&pacman, 1, 1);  // Inicializar Pac-Man
    initGhost(&ghost1, 1, 25);  // Inicializar fantasma

    printMaze(&pacman);

    pthread_t pacman_tid, ghost_tid;

    pthread_create(&pacman_tid, NULL, pacmanThread, &pacman);
    void *args[] = {&ghost1, &pacman};
    pthread_create(&ghost_tid, NULL, ghostThreadAI, args);

    pthread_join(pacman_tid, NULL);
    pthread_join(ghost_tid, NULL);

    endwin();  // Finalizar ncurses

    pthread_mutex_destroy(&mazeLock);
    pthread_mutex_destroy(&pacman.lock);
    pthread_mutex_destroy(&ghost1.lock);

    return 0;
}