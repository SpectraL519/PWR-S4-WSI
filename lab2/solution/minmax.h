#ifndef MINMAX_H
#define MINMAX_H

#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#include "game.h"

#define WIN 10000
#define LOSS 10000



static int get_evaluation (const int state[SIZE][SIZE]) {
	int evaluation = 0;
	char player_pairs = 0, player_triples = 0;
	char opponent_pairs = 0, opponent_triples = 0;

	// win combinations
	for (int i = 0; i < 28; i++) {
		char player = 0;
		char opponent = 0;

		for (int j = 0; j < 4; j++) {
			if (state[win[i][j][0]][win[i][j][1]] == PLAYER)
				player++;
			else if (state[win[i][j][0]][win[i][j][1]] == OPPONENT)
				opponent++;
		}

		// win
		if (player == 4) 
			return WIN;
		if (opponent == 4)
			return -WIN;

		if (player * opponent == 0) {
			evaluation += (player - opponent) * 5 + 1;

			if (player == 3)
				player_triples++;
			else if (player == 2)
				player_pairs++;
			else if (opponent == 3)
				opponent_triples++;
			else if (opponent == 2)
				opponent_pairs++; 
		}
	}

	if (player_triples * opponent_triples == 0)
		evaluation += (player_triples - opponent_triples) * 100;
	if (player_pairs * opponent_pairs == 0)
		evaluation += (player_pairs - opponent_pairs) * 25;

	// loss combinations
	for (int i = 0; i < 48; i++) {
		char player = 0;
		char opponent = 0;

		for (int j = 0; j < 3; j++) {
			if (state[loss[i][j][0]][loss[i][j][1]] == PLAYER)
				player++;
			else if (state[loss[i][j][0]][loss[i][j][1]] == OPPONENT)
				opponent++;
		}

		// loss
		if (player == 3) 
			return -LOSS;
		if (opponent == 3)
			return LOSS;

		if (player * opponent == 0)
			evaluation -= (player - opponent) * 3 + 1;
	}

	return evaluation;
}

static int min (const int a, const int b) {
	return (a < b) ? a : b;
}

static int max (const int a, const int b) {
	return (a > b) ? a : b;
}

typedef struct move_params {
	int position;
	int evalutaion;
} move_params;

static move_params minmax (int state[SIZE][SIZE], const int depth, int alpha, int beta, const bool max_player) {
	int player = max_player ? PLAYER : OPPONENT;

	if (depth <= 0 || 
		check_win(state, player) || check_win(state, 3 - player) ||
		check_loss(state, player) || check_loss(state, 3 - player)
	) return (move_params) {-1, get_evaluation(state)};

	if (max_player) {
		move_params best_move = {-1, INT_MIN};

		// for each child state
		for (int i = 0; i < SIZE; i++) {
			for (int j = 0; j < SIZE; j++) {
				if (state[i][j] == 0) {
					state[i][j] = player;
					struct move_params child_params = minmax(state, depth - 1, alpha, beta, false);
					state[i][j] = 0; // undo "child move"

					if (child_params.evalutaion > best_move.evalutaion) {
						best_move = (struct move_params) {get_position(i, j), child_params.evalutaion};
						alpha = max(child_params.evalutaion, alpha);
						if (beta <= alpha)
							return best_move; // prune
					}
				}
			}
		}

		return best_move;
	}

	// else (min_player)
	struct move_params best_move = {-1, INT_MAX};

	// for each child state
	for (int i = 0; i < SIZE; i++) {
		if (beta <= alpha)
			break;

		for (int j = 0; j < SIZE; j++) {
			if (state[i][j] == 0) {
				state[i][j] = player;
				struct move_params child_params = minmax(state, depth - 1, alpha, beta, true);
				state[i][j] = 0; // undo "child move"

				if (child_params.evalutaion < best_move.evalutaion) {
					best_move = (struct move_params) {get_position(i, j), child_params.evalutaion};
					beta = min(child_params.evalutaion, beta);
					if (beta <= alpha)
						return best_move; // prune
				}
			}
		}
	}

	return best_move;
}

int bot_movement () {
	struct move_params best_move = minmax(board, DEPTH, INT_MIN, INT_MAX, true);
	printf("Making move: %d (eval = %d)\n", best_move.position, best_move.evalutaion);
	make_move(best_move.position, PLAYER);
	return best_move.position;
}

#endif // MINMAX_H
