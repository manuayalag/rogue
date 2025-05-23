#pragma once
//#include <stdlib.h>
#ifndef DEFINE_SCENARIO_H
#define DEFINE_SCENARIO_H
typedef struct _Scenario {
	int** isRoom;// is v-w a room site?
	int** isCorridor;// is v-w a corridor site?
	int N;// dimension of scenario

}Scenario;

typedef struct _Site {
	int i;
	int j;
}Site;


// return dimension of scenario
int scenarioSize(Scenario* d);

// does v correspond to a corridor site? 
int isCorridorScenario(Scenario* d, Site v);

// does v correspond to a room site?
int isRoomScenario(Scenario* d, Site v);


// does v correspond to a wall site?
int isWallScenario(Scenario* d, Site v);
// does v-w correspond to a legal move?
int isLegalMoveScenario(Scenario* d, Site v, Site w);



int manhattanToSite(Site v, Site w);

// does invoking site equal site w?
int equalsSite(Site v, Site w);

// initialize a new scenario based on the given board
Scenario* createScenario(char** board, int size);

#endif