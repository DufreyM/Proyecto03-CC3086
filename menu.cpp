#include "FUNCS.h" 
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h> 

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