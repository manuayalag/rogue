// ----------------------
// File: Police.c
// ----------------------
#include "Police.h"
#include "Game.h"
#include "Scenario.h"
#include <stdlib.h>
#include <stdbool.h>

// Desplazamientos ortogonales (4-vecinos)
static const int di4[] = { -1,  0,  1,  0 };
static const int dj4[] = { 0,  1,  0, -1 };
// Desplazamientos 8-vecinos
static const int di8[] = { -1, -1,  0,  1,  1,  1,  0, -1 };
static const int dj8[] = { 0,  1,  1,  1,  0, -1, -1, -1 };

Site movePolice(Game* game) {
    Scenario* s = getScenario(game);
    int        N = scenarioSize(s);
    Site       police = getPoliceSite(game);
    Site       bandit = getBanditSite(game);
    Site       move = police;  // fallback: quedarse en el mismo lugar

    // Matrices dinámicas para BFS
    bool** visited = malloc(N * sizeof(bool*));
    Site** prev = malloc(N * sizeof(Site*));
    for (int i = 0; i < N; i++) {
        visited[i] = calloc(N, sizeof(bool));
        prev[i] = malloc(N * sizeof(Site));
    }

    // Cola de nodos para BFS
    typedef struct { int i, j; } Node;
    Node* queue = malloc(N * N * sizeof(Node));
    int front = 0, back = 0;

    // Inicializa BFS desde la posición del policía
    visited[police.i][police.j] = true;
    queue[back++] = (Node){ police.i, police.j };

    // Ejecuta BFS hasta hallar la casilla del bandido
    while (front < back && !visited[bandit.i][bandit.j]) {
        Node cur = queue[front++];
        Site curSite = { cur.i, cur.j };

        // Elige 4 u 8 direcciones según tipo de casilla
        bool room = isRoomScenario(s, curSite);
        int dirs = room ? 8 : 4;
        const int* di = room ? di8 : di4;
        const int* dj = room ? dj8 : dj4;

        for (int d = 0; d < dirs; d++) {
            int ni = cur.i + di[d];
            int nj = cur.j + dj[d];
            if (ni < 0 || ni >= N || nj < 0 || nj >= N)
                continue;
            if (visited[ni][nj])
                continue;
            Site nxt = { ni, nj };
            if (!isLegalMoveScenario(s, curSite, nxt))
                continue;

            visited[ni][nj] = true;
            prev[ni][nj] = curSite;
            queue[back++] = (Node){ ni, nj };
        }
    }

    // Si encontramos al bandido, reconstruye el primer paso
    if (visited[bandit.i][bandit.j]) {
        Site step = bandit;
        // Retrocede hasta dar con la casilla justo después de 'police'
        while (!(prev[step.i][step.j].i == police.i &&
            prev[step.i][step.j].j == police.j)) {
            step = prev[step.i][step.j];
        }
        move = step;
    }

    // Liberar memoria
    for (int i = 0; i < N; i++) {
        free(visited[i]);
        free(prev[i]);
    }
    free(visited);
    free(prev);
    free(queue);

    return move;
}
