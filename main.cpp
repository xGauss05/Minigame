#include "manager.h"

int main(int argc, char* args[]) {
	Manager gameManager;
	if (!gameManager.init()) {
		return -1;
	}

	bool end = false;
	while (!end) {
		end = gameManager.update();
		gameManager.draw();
	}

	gameManager.release();
	return 0;
}
