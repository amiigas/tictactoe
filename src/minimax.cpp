
#include "Game.h"

int Game::heuristicEvaluation(int** s, int player) {

    int result = 0;
    int currentStreak = 0;
    int bound = size-(streak-2);

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {

            if (s[i][j] == -player && !(i >= bound && j >= bound)) {
                currentStreak = 1;

                for (int k = 1; k < streak-1; ++k) {
                    if (i+(streak-2) < size) {
                        if (s[i+k][j] == -player) currentStreak++;
                    }
                }
                if (currentStreak == streak-1) {
                    result += -player*10;
                } else {
                    currentStreak = 1;
                }

                for (int k = 1; k < streak-1; ++k) {
                    if (i+(streak-2) < size && j+(streak-2) < size) {
                        if (s[i+k][j+k] == -player) currentStreak++;
                    }
                }
                if (currentStreak == streak-1) {
                    result += -player*5;
                } else {
                    currentStreak = 1;
                }

                for (int k = 1; k < streak-1; ++k) {
                    if (j+(streak-2) < size) {
                        if (s[i][j+k] == -player) currentStreak++;
                    }
                }
                if (currentStreak == streak-1) {
                    result += -player*10;
                } else {
                    currentStreak = 1;
                }

                for (int k = 1; k < streak-1; ++k) {
                    if (i-(streak-2) < size && i-(streak-2) >= 0 && j+(streak-2) < size) {
                        if (s[i-k][j+k] == -player) currentStreak++;
                    }
                }
                if (currentStreak == streak-1) result += -player*5;
            }
        }
    }

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {

            if (s[i][j] == player) {

                for (int k = 0; k < size; ++k) {

                    if (s[i][k] == 0 || s[j][k] == 0) {
                        result++;
                        break;
                    }
                }
            }

        }
    }

    return result;
}

int Game::minimax(int** s, int player, int alpha, int beta, int depth) {

    if (isOver(s,-player)) {
        return -player*1000;
    } else if (isDraw(s)) {
        return 0;
    } else if (depth == 0) {
        return heuristicEvaluation(s,-player);
    }

    int bestScore = -player*INT32_MAX;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {

            if (s[i][j] == 0) {

                s[i][j] = player;
                int score = minimax(s, -player, alpha, beta, depth - 1);
                s[i][j] = 0;

                if (player == 1) {
                    bestScore = std::max(score, bestScore);
                    alpha = std::max(alpha,score);
                    if (beta <= alpha) return bestScore;
                } else {
                    bestScore = std::min(score, bestScore);
                    beta = std::min(beta,score);
                    if (beta <= alpha) return bestScore;
                }
            }
        }
    }
    return bestScore;
}