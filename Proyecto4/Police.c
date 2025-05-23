#include <stdlib.h>
#include "Police.h"


/* implement here the Police game stragegy, i.e. use shortest path to bandit*/
Site movePolice(Game* game) {
	/*REPLACE THIS WITH YOUR CODE TO GET SHORTEST PATH TO ROGUE*/
	Site move;
	move.i = 0;
	move.j = 0;
	if (game) {
		PoliceData* md = game->policeData;
		Site police = getPoliceSite(game);
		Site bandit = getBanditSite(game);

		// take random legal move
		int n = 0;
		for (int i = 0; i < game->N; i++) {
			for (int j = 0; j < game->N; j++) {
				//Site site = new Site(i, j);
				Site site;
				site.i = i;
				site.j = j;
				if (isLegalMoveScenario(game->scenario, police, site)) {
					n++;
					if (rand() % 1000 <= 1000 / n) {
						move.i = site.i;
						move.j = site.j;
					}
				}
			}
		}
	}
	return move;
}