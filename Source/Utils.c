#include <stdio.h>
#include <stdlib.h>
#include "Utils.h"
#include "Movement.h"

int timeSinceTurn(int turnNumber, int targetTurn){
	if (turnNumber >= 0){
		return turn % (targetTurn+turnNumber) == 0;
	} else {
		return 0;
	}
}
