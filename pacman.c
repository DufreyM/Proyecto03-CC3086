#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Definición de la estructura de Pac-Man
typedef struct {
    int x; // Posición en el eje X
    int y; // Posición en el eje Y
    int lives; // Vidas
    int score; // Puntos
    char symbol; // ASCII @
    pthread_mutex_t lock; // Mutex para manejar concurrencia en el acceso a Pac-Man
} PacMan;

// Inicializar Pac-Man en una posición específica
void initPacMan(PacMan *pacman, int startX, int startY) {
    pacman->x = startX;
    pacman->y = startY;
    pacman->lives = 3; 
    pacman->score = 0;
    pacman->symbol = '@';
    pthread_mutex_init(&pacman->lock, NULL);
}

// Definir límites del laberinto
#define MAZE_WIDTH 10
#define MAZE_HEIGHT 10

// Función que verifica si la nueva posición está dentro de los límites
int isValidMove(int newX, int newY) {
    if (newX >= 0 && newX < MAZE_WIDTH && newY >= 0 && newY < MAZE_HEIGHT) {
        return 1; 
    }
    return 0; //Si ya llego al tope del laberinto
}

// Función para mover a Pac-Man según la entrada del jugador
void pacManMovement(PacMan *pacman, char direction) {
    pthread_mutex_lock(&pacman->lock); // Bloquear el acceso a Pac-Man

    int newX = pacman->x;
    int newY = pacman->y;

    //Switch con la dirección
    switch (direction) {
        case 'w': //arriba
            newY++;
            break;
        case 's': //abajo
            newY--;
            break;
        case 'a': //izquierda
            newX--;
            break;
        case 'd': //derecha
            newX++;
            break;
        default:
            printf("Entrada no válida.\n");
            break;
    }

    // Verificar si el movimiento es válido
    if (isValidMove(newX, newY)) {
        pacman->x = newX;
        pacman->y = newY;
        printf("Pac-Man se ha movido a la posición (%d, %d)\n", pacman->x, pacman->y);
    } else {
        printf("Movimiento no permitido, fuera de los límites del laberinto.\n");
    }

    pthread_mutex_unlock(&pacman->lock); 
}

int main() {
    PacMan pacman;
    initPacMan(&pacman, 5, 5); //Pacman en el centro

    char input;
    while (pacman.lives > 0) {
        printf("Ingrese la dirección de movimiento (w: arriba, s: abajo, a: izquierda, d: derecha): ");
        scanf(" %c", &input);
        pacManMovement(&pacman, input);

    }
    pthread_mutex_destroy(&pacman.lock);
    return 0;
}
