#include "Search.h"

int *miniMax(CH_Game *currentGame, unsigned int maxDepth)
{
    ASSERT(currentGame != NULL);

    int *move = (int *)malloc(CELL_SIZE * CELL_SIZE * sizeof(int));
    ASSERT(move != NULL);
    //makes a copy of the game
    CH_Game *copy = gameCopy(currentGame);
    if (!copy)
    {
        return NULL;
    }
    //Creates root node
    Node *node = nodeCreate(copy);
    if (!node)
    {
        gameDestroy(copy);
        return NULL;
    }

    if (!move)
    {
        gameDestroy(copy);
        nodeDestroy(node);
        return NULL;
    }
    miniMaxRec(node, 0, maxDepth, getCurrentPlayer(copy) == playerPC(copy), INT_MIN, INT_MAX);
    if (node->best_move[0] == ALLOC_ERROR_FLAG)
    {
        gameDestroy(copy);
        nodeDestroy(node);
        free(move);
        return NULL;
    }
    for (int i = 0; i < CELL_SIZE * CELL_SIZE; ++i)
    {
        move[i] = node->best_move[i];
    }

    gameDestroy(copy);
    nodeDestroy(node);
    return move;
}

int miniMaxRec(Node *src, int level, int maxDepth, bool maximize, int alpha, int beta)
{
    ASSERT(src != NULL);
    int val = 0;
    int move[CELL_SIZE * CELL_SIZE] = {0};
    int size = 0;
    bool updated = false; //best move hasn't been updated
    elem *element;
    int **arr = NULL;
    int check = callerWin(src->game_copy); //check for winner
    if (check != 0)
    {                 // if check!=0 that means there is a winner
        return check; //returns the max/min value depending on the player
    }
    if (level == maxDepth)
    {
        return scoreFunction(src->game_copy);
    }
    for (int i = 0; i < GRID; ++i)
    {
        for (int j = 0; j < GRID; ++j)
        {
            if (playerFig(src->game_copy, i, j))
            {
                arr = possibleMoves(src->game_copy, i, j);
                if (arr == NULL)
                {
                    src->best_move[START_ROW_PLACE] = ALLOC_ERROR_FLAG;
                    return ALLOC_ERROR_VALUE;
                }

                //maximum number of moves possible of the piece
                size = numOfMoves(src->game_copy->gameBoard[i][j]); 
                for (int k = 0; (k < size) && (alpha < beta); ++k)
                {
                    if (arr[k][0] == NO_MOVES_FLAG)
                    {
                        break;
                    }
                    move[START_ROW_PLACE] = i + 1;
                    move[START_COL_PLACE] = j + 1;
                    move[DESTINATION_ROW_PLACE] = arr[k][0] + 1;
                    move[DESTINATION_COL_PLACE] = arr[k][1] + 1;
                    element = transfer(src->game_copy, move);
                    if (element == NULL)
                    {
                        freeArray(arr, size);
                        src->best_move[START_ROW_PLACE] = ALLOC_ERROR_FLAG;
                        return ALLOC_ERROR_VALUE;
                    }
                    doMove(src->game_copy, element);
                    src->game_copy->game_status = updateStatus(src->game_copy);
                    val = miniMaxRec(src, level + 1, maxDepth, !maximize, alpha, beta);
                    if (src->best_move[START_ROW_PLACE] == ALLOC_ERROR_FLAG)
                    {
                        freeArray(arr, size);
                        free(element);
                        return ALLOC_ERROR_VALUE;
                    }

                    if (level == 0)
                    {
                        val = value(alpha, val, true);
                        if ((val != alpha) || (!updated))
                        {
                            src->best_move[START_ROW_PLACE] = element->startRow;
                            src->best_move[START_COL_PLACE] = element->startCol;
                            src->best_move[DESTINATION_ROW_PLACE] = element->destinationRow;
                            src->best_move[DESTINATION_COL_PLACE] = element->destinationCol;
                            updated = true;
                        }
                    }
                    if (maximize)
                    {
                        alpha = value(alpha, val, maximize);
                    }
                    else
                    {
                        beta = value(beta, val, maximize);
                    }
                    free(element);
                    undoPrevMove(src->game_copy);
                }
                freeArray(arr, size);
            }
        }
    }
    if (maximize)
    {
        return alpha;
    }
    return beta;
}
int callerWin(CH_Game *src)
{
    ASSERT(src != NULL);
    if (checkWinner(src) == playerPC(src))
    {
        return INT_MAX;
    }
    else if ((checkWinner(src) != '\0') && (checkWinner(src) != CH_TIE))
    {
        return INT_MIN;
    }
    return 0;
}
int scoreFunction(CH_Game *src)
{
    ASSERT(src != NULL);
    int vec[NUM_OF_PIECES] = VEC;
    int value = callerWin(src);
    if (value != 0)
    {
        return value;
    }
    if (playerPC(src) == CH_PLAYER_1)
    {
        for (int i = 0; i < NUM_OF_PIECES; ++i) //computes the score according to the weight vector
        {
            value += (src->white[i] - src->black[i]) * vec[i];
        }
    }
    else
    {
        for (int i = 0; i < NUM_OF_PIECES; ++i) //computes the score according to the weight vector
        {
            value += (src->black[i] - src->white[i]) * vec[i];
        }
    }
    return value;
}
int value(int alpha, int beta, bool maximize)
{
    if (maximize)
    {
        return max(alpha, beta);
    }
    return min(alpha, beta);
}
Node *nodeCreate(CH_Game *src)
{
    if (!src)
    {
        return NULL;
    }

    Node *node = (Node *)malloc(sizeof(Node));

    if (!node)
    {
        return NULL;
    }
    node->game_copy = src;
    for (int i = 0; i < CELL_SIZE * CELL_SIZE; ++i)
    {
        node->best_move[i] = NO_MOVES_FLAG;
    }
    node->player = src->currentPlayer;
    return node;
}
Node *nodeCopy(Node *src)
{
    if (!src)
    {
        return NULL;
    }
    Node *nd = nodeCreate(src->game_copy);
    if (!nd)
    {
        return NULL;
    }
    for (int i = 0; i < CELL_SIZE * CELL_SIZE; ++i)
    {
        nd->best_move[i] = src->best_move[i];
    }
    nd->player = src->player;
    return nd;
}
void nodeDestroy(Node *src)
{
    if (src != NULL)
    {
        free(src);
    }
}