#include <stdlib.h>
#include "Scenario.h"

// return dimension of scenario
int scenarioSize(Scenario* d) { 
	return d->N; 
}

// does v correspond to a corridor site? 
int isCorridorScenario(Scenario* d, Site v) {
	int i = v.i;
	int j = v.j;
	if (i < 0 || j < 0 || i >= d->N || j >= d->N) return 0;
	return d->isCorridor[i][j];
}

// does v correspond to a room site?
int isRoomScenario(Scenario* d, Site v) {
	int i = v.i;
	int j = v.j;
	if (i < 0 || j < 0 || i >= d->N || j >= d->N) return 0;
	return d->isRoom[i][j];
}


// does v correspond to a wall site?
int isWallScenario(Scenario* d, Site v) {
	return (!isRoomScenario(d,v) && !isCorridorScenario(d,v));
}

// does v-w correspond to a legal move?
int isLegalMoveScenario(Scenario* d,Site v, Site w) {
	int i1 = v.i;
	int j1 = v.j;
	int i2 = w.i;
	int j2 = w.j;
	if (i1 < 0 || j1 < 0 || i1 >= d->N || j1 >= d->N) return 0;
	if (i2 < 0 || j2 < 0 || i2 >= d->N || j2 >= d->N) return 0;
	if (isWallScenario(d,v) || isWallScenario(d,w)) return 0;
	if (abs(i1 - i2) > 1)  return 0;
	if (abs(j1 - j2) > 1)  return 0;
	if (isRoomScenario(d,v) && isRoomScenario(d,w)) return 1;
	if (i1 == i2)               return 1;
	if (j1 == j2)               return 1;

	return 0;
}

int manhattanToSite(Site v, Site w) {
	int i1 = v.i;
	int j1 = v.j;
	int i2 = w.i;
	int j2 = w.j;
	return abs(i1 - i2) + abs(j1 - j2);
}

// does invoking site equal site w?
int equalsSite(Site v, Site w) {
	return (manhattanToSite(v,w) == 0);
}

int** _createIntArray(int N) {
	int** arr = NULL;
	arr = (int**)malloc(N * sizeof(int*));
	if (NULL != arr) {
		for (int i = 0; i < N; i++)
			arr[i] = (int*)malloc(N * sizeof(int));
	}
	return arr;
}

// initialize a new scenario based on the given board
Scenario* createScenario(char** board, int size) {
	if (board) {
		Scenario* d = (Scenario*)malloc(sizeof(struct _Scenario));
		d->isRoom = _createIntArray(size);
		d->isCorridor = _createIntArray(size);
		d->N = size;
		for (int v = 0; v < size; v++) {
			for (int w = 0; w < size; w++) {
				d->isRoom[v][w] = 0;
				d->isCorridor[v][w] = 0;
				if (board[v][w] == '.') d->isRoom[v][w] = 1;
				else if (board[v][w] == '+') d->isCorridor[v][w] = 1;
			}
		}
		return d;
	}
	return NULL;
}

// dispose allocated memory properly after use
void destroyScenario(Scenario* d) {
	if (d) {
		if (d->isRoom) {
			for (int i = 0; i < d->N; i++)
				free(d->isRoom[i]);
			free(d->isRoom);
		}
		if (d->isCorridor) {
			for (int i = 0; i < d->N; i++)
				free(d->isCorridor[i]);
			free(d->isCorridor);
		}
		free(d);
	}
}
