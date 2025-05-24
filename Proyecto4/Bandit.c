#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "Bandit.h"

static const int di8[] = { -1, -1,  0, 1, 1,  1,  0, -1 };  // N, NE, E, SE, S, SO, O, NO
static const int dj8[] = { 0,  1,  1, 1, 0, -1, -1, -1 };
static const int di4[] = { -1, 0, 1, 0 }; // N, E, S, O
static const int dj4[] = { 0, 1, 0, -1 };

// Verifica si un sitio es una entrada a corredor (corridor entrance)
bool isCorridorEntrance(Game* game, Site site) {
    if (!isCorridorScenario(game->scenario, site)) return false;
    int N = game->N;
    for (int d = 0; d < 8; d++) {
        int ni = site.i + di8[d];
        int nj = site.j + dj8[d];
        if (ni >= 0 && ni < N && nj >= 0 && nj < N) {
            Site neighbor = { ni, nj };
            if (isRoomScenario(game->scenario, neighbor)) {
                return true;
            }
        }
    }
    return false;
}

// BFS para encontrar el primer paso hacia el objetivo
Site bfsToTarget(Game* game, Site start, Site target) {
    int N = game->N;

    bool** visited = (bool**)malloc(N * sizeof(bool*));
    Site** prev = (Site**)malloc(N * sizeof(Site*));
    for (int i = 0; i < N; ++i) {
        visited[i] = (bool*)calloc(N, sizeof(bool));
        prev[i] = (Site*)malloc(N * sizeof(Site));
    }

    typedef struct { int i, j; } QueueNode;
    QueueNode* queue = (QueueNode*)malloc(N * N * sizeof(QueueNode));
    int front = 0, back = 0;

    queue[back++] = (QueueNode){ start.i, start.j };
    visited[start.i][start.j] = true;

    while (front < back) {
        QueueNode curr = queue[front++];
        Site currSite = { curr.i, curr.j };

        int directions = isCorridorScenario(game->scenario, currSite) ? 4 : 8;
        const int* di = directions == 4 ? di4 : di8;
        const int* dj = directions == 4 ? dj4 : dj8;

        for (int d = 0; d < directions; d++) {
            int ni = curr.i + di[d];
            int nj = curr.j + dj[d];
            if (ni < 0 || ni >= N || nj < 0 || nj >= N) continue;
            if (visited[ni][nj]) continue;
            Site neighbor = { ni, nj };
            if (!isLegalMoveScenario(game->scenario, currSite, neighbor)) continue;
            visited[ni][nj] = true;
            prev[ni][nj] = currSite;
            queue[back++] = (QueueNode){ ni, nj };
            if (ni == target.i && nj == target.j) {
                Site step = target;
                while (!(prev[step.i][step.j].i == start.i && prev[step.i][step.j].j == start.j)) {
                    step = prev[step.i][step.j];
                }

                for (int i = 0; i < N; ++i) {
                    free(visited[i]);
                    free(prev[i]);
                }
                free(visited);
                free(prev);
                free(queue);

                return step;
            }
        }
    }

    for (int i = 0; i < N; ++i) {
        free(visited[i]);
        free(prev[i]);
    }
    free(visited);
    free(prev);
    free(queue);

    return start;
}

Site corredorAdyacente(Game* game, Site actual) {
    Scenario* sc = getScenario(game);
    Site police = getPoliceSite(game);
    Site best = { -1, -1 };
    int  bestDist = -1;
    int  N = scenarioSize(sc);

    for (int d = 0; d < 4; d++) {
        Site vecino = {
            actual.i + di4[d],
            actual.j + dj4[d]
        };
        // dentro de límites
        if (vecino.i < 0 || vecino.i >= N ||
            vecino.j < 0 || vecino.j >= N)
            continue;
        // debe ser corredor y mov legal
        if (!isCorridorScenario(sc, vecino) ||
            !isLegalMoveScenario(sc, actual, vecino))
            continue;

        // mide cuán lejos está del policía
        int dist = manhattanToSite(vecino, police);
        if (dist > bestDist) {
            bestDist = dist;
            best = vecino;
        }
    }

    return best;
}

Site moveBandit(Game* game) {
    Site bandit = getBanditSite(game);
    Site police = getPoliceSite(game);
    int N = game->N;

    // PRIORIDAD: Si hay un corredor adyacente que aleje del policía, ir hacia él
    Site adyacente = corredorAdyacente(game, bandit);
    if (adyacente.i != -1 && adyacente.j != -1) {
        int distActual = manhattanToSite(bandit, police);
        int distAdyacente = manhattanToSite(adyacente, police);
        if (distAdyacente > distActual) {
            return adyacente;
        }
        // Si no aleja, NO se mueve al corredor, sigue con el resto de la lógica
    }

    // Si está en un corredor, moverse en 4 direcciones, eligiendo la más alejada del policía (evitar retroceder si hay empate)
    if (isCorridorScenario(game->scenario, bandit)) {
        Site bestMoves[4];
        int bestCount = 0;
        int maxDist = -1;
        for (int d = 0; d < 4; d++) {
            int ni = bandit.i + di4[d];
            int nj = bandit.j + dj4[d];
            if (ni < 0 || ni >= N || nj < 0 || nj >= N) continue;
            Site neighbor = { ni, nj };
            if (!isLegalMoveScenario(game->scenario, bandit, neighbor)) continue;
            int dist = manhattanToSite(neighbor, police);
            if (dist > maxDist) {
                maxDist = dist;
                bestMoves[0] = neighbor;
                bestCount = 1;
            } else if (dist == maxDist) {
                bestMoves[bestCount++] = neighbor;
            }
        }
        // Evitar retroceder si hay más de una opción
        if (bestCount > 1) {
            for (int i = 0; i < bestCount; ++i) {
                // No volver a la casilla anterior si es posible
                if (!(bestMoves[i].i == bandit.i && bestMoves[i].j == bandit.j)) {
                    return bestMoves[i];
                }
            }
        }
        if (bestCount > 0) {
            // Si solo hay una opción, tomarla
            return bestMoves[0];
        }
        // Si no hay movimiento legal, quedarse en el mismo lugar
        return bandit;
    }

    // Buscar la mejor entrada a corredor (más alejada del policía)
    Site bestEntrance = bandit;
    int bestDist = -1;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            Site candidate = { i, j };
            if (isCorridorEntrance(game, candidate)) {
                int dist = manhattanToSite(candidate, police);
                if (dist > bestDist) {
                    bestDist = dist;
                    bestEntrance = candidate;
                }
            }
        }
    }

    if (!(bestEntrance.i == bandit.i && bestEntrance.j == bandit.j)) {
        return bfsToTarget(game, bandit, bestEntrance);
    }

    // Si no hay entradas a corredores, alejarse lo más posible del policía (8 direcciones)
    Site bestMove = bandit;
    int maxDist = manhattanToSite(bandit, police);
    for (int d = 0; d < 8; d++) {
        int ni = bandit.i + di8[d];
        int nj = bandit.j + dj8[d];
        if (ni < 0 || ni >= N || nj < 0 || nj >= N) continue;
        Site neighbor = { ni, nj };
        if (!isLegalMoveScenario(game->scenario, bandit, neighbor)) continue;
        int dist = manhattanToSite(neighbor, police);
        if (dist > maxDist) {
            maxDist = dist;
            bestMove = neighbor;
        }
    }
    return bestMove;
}
