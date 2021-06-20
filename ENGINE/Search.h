#ifndef SEARCH_H_
#define SEARCH_H_

#include "ChessGame.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>


#define max(a,b) ((a)>=(b)? (a) : (b));
#define min(a,b) ((a)<=(b)? (a) : (b));

#define VEC {1,3,3,5,9,100} //weight vector
#define NO_MOVES_FLAG -1
#define ALLOC_ERROR_FLAG -2
#define ALLOC_ERROR_VALUE 15000

//represents the node int the MiniMax tree
typedef struct{
    int best_move[CELL_SIZE*CELL_SIZE];//the best col to choose for the current node according to minimax algo
    char player;//the player
    CH_Game* game_copy; // copy the game with the moves represented by the node
}Node;




/**
 * Creates the root of the minimax tree and calls the recursion function
 * due to the desired level which is represented by the MAXDEPTH argument
 * 
 * @param currentGame - The source game
 * @param maxDepth - The level of the game - represents the maximum Depth of the minimax algorithm
 * @return 
 * null - if the game is NULL or there was an allocation error
 * suggested move - otherwise
 * assertin error if currentGame==NULL
 */
extern int*  miniMax(CH_Game* currentGame, unsigned int maxDepth);
/**
 * @brief The recursive function that computes the best move according to the minimax
 * @param currentGame - The source game
 * @param level - Tree level
 * @param maxDepth
 * @param maximize - maximization/minimization flag
 * @param alpha - computer side
 * @param beta
 * @return
 * best move for the minimax algo
 * assertion error if src == NULL
 */
extern int miniMaxRec(Node* src, int level, int maxDepth, bool maximize, int alpha, int beta );
/**
 * @brief Creates a new node for the minimax tree. The tree contains copy of the game, which it get from
 * its' parent node in the minimax tree or if it's the root node - a copy of the current game
 * The node also contains the player and the best move for the current node
 * @param currentGame - The game with the hypothetical move
 * @return 
 * NULL if either a memory allocation failure occurs
 * Otherwise , a new node instant is returned
 */
extern Node* nodeCreate(CH_Game* src);
/**
 * @brief Frees all memory allocation associated with a given node.
 * if src==NULL the function does nothing
 * @param src- the Game represented by the node
 * @return 
 */
extern void nodeDestroy(Node* src);
/**
 * @brief Creates a copy of the given node
 * @param src
 * @return
 * NULL if either src is NULL or a memory allocation failure.
 * Otherwise, an new copy of the source node is returned
 */
extern Node* nodeCopy(Node* src);
/**
 * @brief Computes the scoring according to the weight vector defined
 * @param arr - the array that represents the games' span calculation
 * @return 
 * INT_MIN - if the player wins
 * INT_MAX - if the computer wins
 * else returns the sum of the weight vector
 */
extern int scoreFunction(CH_Game* src);
/**
 * @brief Checks does the player win who called the function
 * @param src - the root node
 * @return 
 * INT_MIN - if player wins
 * INT_MAX - if computer wins
 * 0 - otherwise
 */
extern int callerWin(CH_Game* src);
/**
 * @brief Decides whether the recursive functions depth currently compute the maximum or the minimum
 * @param alpha- try to maximize
 * @param beta - try to minimize
 * @param maximize - if the maximize flag is true it will compute maximization otherwise minimization
 * @return 
 * max(alpha,beta) if maximize == true;
 * min(alpha,beta) if maximize == false;
 */
extern int value(int alpha, int beta, bool maximize);


#endif