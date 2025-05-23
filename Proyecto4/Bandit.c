#include <stdlib.h>
#include "Bandit.h"

/* implement here the Bandit game stragegy to escape from Police*/
Site moveBandit(Game* game) {
	/*REPLACE THIS WITH YOUR CODE TO AVOID MONSTER*/
	Site move;
	move.i = 0;
	move.j = 0;
	if (game) {
		BanditData* rg = game->banditData;
		Site police = getPoliceSite(game);
		Site bandit = getBanditSite(game);
		// take random legal move
		int n = 0;
		for (int i = 0; i < game->N; i++) {
			for (int j = 0; j < game->N; j++) {
				Site site;
				site.i = i;
				site.j = j;
				if (isLegalMoveScenario(game->scenario, bandit, site)) {
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
