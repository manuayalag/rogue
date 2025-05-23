#pragma once
#ifndef DEFINE_GAME_H
#define DEFINE_GAME_H
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "Scenario.h"
#define MAX_LINE_LENGTH 80
/*ADD HERE YOUR DATA STRUCTURES NEEDED FOR POLICE STRATEGY.
E.G. add a Adjacency Matrix for graph implementation*/
typedef struct _PoliceData {
	//add your needed structs/data containers for the Graph here.
	void* replaceThisWithYourDataContainer;
}PoliceData;

typedef struct _BanditData {
	//add your needed structs/data containers here.
	void* replaceThisWithYourDataContainer;
}BanditData;



typedef struct _Game {
	Scenario* scenario;
	char POLICE;
	char BANDIT;
	int N;
	Site policeSite;
	Site banditSite;
	PoliceData* policeData;
	BanditData* banditData;
}Game;

Game* createGame(char* fileName);
void destroyGame(Game* g);
Site getPoliceSite(Game* g);
Site getBanditSite(Game* g);
Scenario* getScenario(Game* g);
void printGame(Game* g);
void playGame(Game* g);

PoliceData* createPoliceData(Game* g);
void destroyPoliceData(PoliceData* r);

BanditData* createBanditData(Game* g);
void destroyBanditData(BanditData* r);

#endif