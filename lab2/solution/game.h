#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdbool.h>

#define SIZE 5



int board[SIZE][SIZE];
int PLAYER = -1;
int OPPONENT = -1;
int DEPTH = -1;

const int win[28][4][2] = { 
	{ {0,0}, {0,1}, {0,2}, {0,3} },
	{ {1,0}, {1,1}, {1,2}, {1,3} },
	{ {2,0}, {2,1}, {2,2}, {2,3} },
	{ {3,0}, {3,1}, {3,2}, {3,3} },
	{ {4,0}, {4,1}, {4,2}, {4,3} },
	{ {0,1}, {0,2}, {0,3}, {0,4} },
	{ {1,1}, {1,2}, {1,3}, {1,4} },
	{ {2,1}, {2,2}, {2,3}, {2,4} },
	{ {3,1}, {3,2}, {3,3}, {3,4} },
	{ {4,1}, {4,2}, {4,3}, {4,4} },
	{ {0,0}, {1,0}, {2,0}, {3,0} },
	{ {0,1}, {1,1}, {2,1}, {3,1} },
	{ {0,2}, {1,2}, {2,2}, {3,2} },
	{ {0,3}, {1,3}, {2,3}, {3,3} },
	{ {0,4}, {1,4}, {2,4}, {3,4} },
	{ {1,0}, {2,0}, {3,0}, {4,0} },
	{ {1,1}, {2,1}, {3,1}, {4,1} },
	{ {1,2}, {2,2}, {3,2}, {4,2} },
	{ {1,3}, {2,3}, {3,3}, {4,3} },
	{ {1,4}, {2,4}, {3,4}, {4,4} },
	{ {0,1}, {1,2}, {2,3}, {3,4} },
	{ {0,0}, {1,1}, {2,2}, {3,3} },
	{ {1,1}, {2,2}, {3,3}, {4,4} },
	{ {1,0}, {2,1}, {3,2}, {4,3} },
	{ {0,3}, {1,2}, {2,1}, {3,0} },
	{ {0,4}, {1,3}, {2,2}, {3,1} },
	{ {1,3}, {2,2}, {3,1}, {4,0} },
	{ {1,4}, {2,3}, {3,2}, {4,1} }
};

const int loss[48][3][2] = {
	{ {0,0}, {0,1}, {0,2} }, { {0,1}, {0,2}, {0,3} }, { {0,2}, {0,3}, {0,4} }, 
	{ {1,0}, {1,1}, {1,2} }, { {1,1}, {1,2}, {1,3} }, { {1,2}, {1,3}, {1,4} }, 
	{ {2,0}, {2,1}, {2,2} }, { {2,1}, {2,2}, {2,3} }, { {2,2}, {2,3}, {2,4} }, 
	{ {3,0}, {3,1}, {3,2} }, { {3,1}, {3,2}, {3,3} }, { {3,2}, {3,3}, {3,4} }, 
	{ {4,0}, {4,1}, {4,2} }, { {4,1}, {4,2}, {4,3} }, { {4,2}, {4,3}, {4,4} }, 
	{ {0,0}, {1,0}, {2,0} }, { {1,0}, {2,0}, {3,0} }, { {2,0}, {3,0}, {4,0} }, 
	{ {0,1}, {1,1}, {2,1} }, { {1,1}, {2,1}, {3,1} }, { {2,1}, {3,1}, {4,1} }, 
	{ {0,2}, {1,2}, {2,2} }, { {1,2}, {2,2}, {3,2} }, { {2,2}, {3,2}, {4,2} }, 
	{ {0,3}, {1,3}, {2,3} }, { {1,3}, {2,3}, {3,3} }, { {2,3}, {3,3}, {4,3} }, 
	{ {0,4}, {1,4}, {2,4} }, { {1,4}, {2,4}, {3,4} }, { {2,4}, {3,4}, {4,4} }, 
	{ {0,2}, {1,3}, {2,4} }, { {0,1}, {1,2}, {2,3} }, { {1,2}, {2,3}, {3,4} }, 
	{ {0,0}, {1,1}, {2,2} }, { {1,1}, {2,2}, {3,3} }, { {2,2}, {3,3}, {4,4} }, 
	{ {1,0}, {2,1}, {3,2} }, { {2,1}, {3,2}, {4,3} }, { {2,0}, {3,1}, {4,2} }, 
	{ {0,2}, {1,1}, {2,0} }, { {0,3}, {1,2}, {2,1} }, { {1,2}, {2,1}, {3,0} }, 
	{ {0,4}, {1,3}, {2,2} }, { {1,3}, {2,2}, {3,1} }, { {2,2}, {3,1}, {4,0} }, 
	{ {1,4}, {2,3}, {3,2} }, { {2,3}, {3,2}, {4,1} }, { {2,4}, {3,3}, {4,2} }
};



void init_board() {
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
			board[i][j] = 0;
}

void show_board() {
	printf("  1 2 3 4 5\n");
	for (int i = 0; i < 5; i++) {
		printf("%d",i+1);
		for (int j = 0; j < 5; j++)
			switch(board[i][j]) {
				case 0: { printf(" -"); break; }
				case 1: { printf(" X"); break; }
				case 2: { printf(" O"); break; }
			}
		printf("\n");
	}
	printf("\n");
}

int get_position (int row, int column) {
	return (row + 1) * 10 + column + 1;
}

bool set_game_params (const int player, const int depth) {
	if (player != 1 && player != 2) {
		printf("Error: Invaliv value of 'player' - must be 1 or 2, was %d\n", depth);
		return false;
	}

	if (depth < 1 || depth > 10) {
        printf("Error: Invaliv value of 'depth' - must be in range 1 - 10, was %d\n", depth);
        return false;
    }

	PLAYER = player;
	OPPONENT = 3 - PLAYER;
	DEPTH = depth;

	return true;
}

bool check_win(const int state[SIZE][SIZE], const int player) {
	for (int i = 0; i < 28; i++)
		if ( 
			(state[win[i][0][0]][win[i][0][1]] == player) && 
			(state[win[i][1][0]][win[i][1][1]] == player) && 
			(state[win[i][2][0]][win[i][2][1]] == player) && 
			(state[win[i][3][0]][win[i][3][1]] == player) 
		) return true;
	return false;
}

bool check_loss(const int state[SIZE][SIZE], const int player) {
	for (int i = 0; i < 48; i++)
		if (
			(state[loss[i][0][0]][loss[i][0][1]] == player) && 
			(state[loss[i][1][0]][loss[i][1][1]] == player) && 
			(state[loss[i][2][0]][loss[i][2][1]] == player) 
		) return true;
	return false;
}

bool make_move (const int move, const int player) {
	int row = (move / 10) - 1;
	int col = (move % 10) - 1;

	if ((row < 0) || (row > 4) || (col < 0) || (col > 4)) 
		return false; 

	if (board[row][col] != 0) 
		return false;

	board[row][col] = player;
	return true;
}

#endif // GAME_H