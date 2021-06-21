#include "ChessGame.h"

CH_Game* gameCreate(int historySize, int diff, int color, int mode)
{
    if(historySize<=0){
        return NULL;
    }
    CH_Game *new = (CH_Game*)malloc(sizeof(CH_Game));
    ASSERT(new!=NULL);
    new->undo_hist = ArrayListCreate(historySize);
    if(!(new->undo_hist)){
        free(new);
        return NULL;
    }
    argInitialize(new);//Initialize game with default arguments
    boardInitialize(new);
    new->mode = mode;
    new->user_color = color;
    new->difficulty = diff;
    new->currentPlayer = CH_PLAYER_1;
    new->mv_cnt = 0;
    return new;
}

void argInitialize(CH_Game* game)
{
    ASSERT(game != NULL);
    game->white[PAWN_INDEX] = game->black[PAWN_INDEX] = PAWN_NUM;
    game->white[KNIGHT_INDEX] = game->white[BISHOP_INDEX] = game->white[ROOK_INDEX] = B_R_Kn_NUM;
    game->black[KNIGHT_INDEX] = game->black[BISHOP_INDEX] = game->black[ROOK_INDEX] = B_R_Kn_NUM;
    game->white[KING_INDEX] = game->white[QUEEN_INDEX] = K_Q_NUM;
    game->black[KING_INDEX] = game->black[QUEEN_INDEX] = K_Q_NUM;
    game->white_king[START_ROW_PLACE] = (GRID-GRID);
    game->black_king[START_ROW_PLACE] = (GRID-1);
    game->white_king[START_COL_PLACE] = (CELL_SIZE*CELL_SIZE);
    game->black_king[START_COL_PLACE] = (CELL_SIZE*CELL_SIZE);
    game->game_status = REGULAR;
    memset(game->piece_promotion, 0 , sizeof(game->piece_promotion));
}
void boardInitialize(CH_Game* game)
{
    ASSERT(game!=NULL);
    for(int i=1; i<GRID-1; ++i)
    {
        for(int j=0; j<GRID; ++j)
        {
            if(i==1){
                game->gameBoard[i][j] = PAWN_W;
            }
            else if(i==6){
                game->gameBoard[i][j] = PAWN_B;
            }
            else{
                game->gameBoard[i][j] = CH_EMPTY_ENTRY;
            }
        }
    }
    game->gameBoard[0][0] = ROOK_W; 
    game->gameBoard[0][1] = KNIGHT_W;
    game->gameBoard[0][2] = BISHOP_W; 
    game->gameBoard[0][3] = QUEEN_W; 
    game->gameBoard[0][4] = KING_W; 
    game->gameBoard[0][5] = BISHOP_W;
    game->gameBoard[0][6] = KNIGHT_W;
    game->gameBoard[0][7] = ROOK_W; 
    game->gameBoard[7][0] = ROOK_B;
    game->gameBoard[7][1] = KNIGHT_B;
    game->gameBoard[7][2] = BISHOP_B;
    game->gameBoard[7][3] = QUEEN_B;
    game->gameBoard[7][4] = KING_B;
    game->gameBoard[7][5] = BISHOP_B;
    game->gameBoard[7][6] = KNIGHT_B;
    game->gameBoard[7][7] = ROOK_B;
}

CH_Game* gameCopy(CH_Game* src)
{
    if(!src){
        return NULL;
    }
    CH_Game* gamecopy = (CH_Game*)malloc(sizeof(CH_Game));
    if(!gamecopy){
        return NULL;
    }
    gamecopy->undo_hist = ArrayListCopy(src->undo_hist);
    if(gamecopy->undo_hist == NULL){
        free(gamecopy);
        return NULL;
    }
    copyParameters(src, gamecopy);
    return gamecopy;
}
void copyParameters(CH_Game* src, CH_Game* gamecopy)
{
    ASSERT(src!=NULL);
    ASSERT(gamecopy != NULL);
    for(int i=0; i<GRID; ++i){
        for(int j=0; j<GRID; ++j){
            (gamecopy->gameBoard)[i][j] = (src->gameBoard)[i][j];
        }
    }
    for(int i=0; i<NUM_OF_PIECES; ++i){
        gamecopy->white[i] = src->white[i];
        gamecopy->black[i] = src->black[i];
    }

    gamecopy->currentPlayer = getCurrentPlayer(src);
    gamecopy->mode = src->mode;
    gamecopy->mv_cnt = src->mv_cnt;
    gamecopy->black_king[0] = src->black_king[0];
    gamecopy->black_king[1] = src->black_king[1];
    gamecopy->white_king[0] = src->white_king[0];
    gamecopy->white_king[1] = src->white_king[1];
    gamecopy->difficulty = src->difficulty;
    gamecopy->user_color = src->user_color;
    gamecopy->game_status = src->game_status;
    gamecopy->piece_promotion[0] = src->piece_promotion[0];
    gamecopy->piece_promotion[1] = src->piece_promotion[1];
    gamecopy->piece_promotion[2] = src->piece_promotion[2];
    gamecopy->piece_promotion[3] = src->piece_promotion[3];

}

char getCurrentPlayer(CH_Game* src)
{
    if(!src){
        return CH_EMPTY_ENTRY;
    }
    return src->currentPlayer;
}

void gameDestroy(CH_Game* src)
{
    if(src){
        ArrayListDestroy(src->undo_hist);
        free(src);
    }
}

CH_GAME_MESSAGE gamePrintBoard(CH_Game* src)
{
    if(!src){
        return CH_INVALID_ARGUMENT;
    }
    for(int i=GRID-1; i>=0; --i)
    {
        printf("%d| ",i+1);
        for(int j=0; j<GRID; ++j)
        {
            printf("%c ",src->gameBoard[i][j]);
        }
        printf("|\n");
    }
    printf("  ");
    for(int i=0; i<GRID; ++i){
        printf("--");
    }
    printf("-\n");
    printf("   ");
    for(int i=0; i<GRID; ++i){
        printf("%c ",'A'+i);
    }
    printf("\n");
    return CH_SUCCESS;

}
int setMove(CH_Game* src, int* move)
{
    elem* element;
    int res = 0;
    if( (!src) || (!move) ){
        printf("error: chessgame.c-line174\n");
        return 1;
    }
    for(int i=0; i<CELL_SIZE*CELL_SIZE; ++i)
    {
        if(move[i]<1 || move[i]>GRID)
        {
            printf("error: chessgame.c-line181\n");
            return 1;
        }
    }
    element = transfer(src, move);
    if(element==NULL){
        printf("error: chessgame.c-line187\n");
        return ALLOCATION_ERROR;
    }
    res = isValidMove(src,element);
    if(res==ALLOCATION_ERROR)
    {
        printf("error: chessgame.c-line193\n");
        free(element);
        return ALLOCATION_ERROR;
    }
    if(res != 1)
    {
        printf("error: chessgame.c-line199\n");
        free(element);
        return res;
    }
    doMove(src, element);
    src->game_status = updateStatus(src);
    free(element);
    return 0;
}
elem* transfer(CH_Game* src, int* move)
{
    ASSERT(move!=NULL);
    ASSERT(src!=NULL);
    elem* element;
    element = (elem*)malloc(2*sizeof(elem));
    if(!element){
        return NULL;
    }
    element->destinationRow = move[(CELL_SIZE*CELL_SIZE)-DESTINATION_ROW_PLACE]-1;
    element->destinationCol = move[(CELL_SIZE*CELL_SIZE)-START_COL_PLACE]-1;
    element->startRow = move[(CELL_SIZE*CELL_SIZE)-DESTINATION_COL_PLACE-1]-1;
    element->startCol = move[(CELL_SIZE*CELL_SIZE)-DESTINATION_COL_PLACE]-1;
    element->fig = src->gameBoard[element->startRow][element->startCol];
    element->player = getCurrentPlayer(src);
    element->gameStatus = statusToInt(src->game_status);
    return element;
}

int statusToInt(CH_STATUS st)
{
    if(st==CHECK){
        return CHECK_REP;
    }
    if(st==CHECKMATE){
        return CHECKMATE_REP;
    }
    if(st==TIE){
        return TIE_REP;
    }
    return REG_REP;
}
CH_STATUS intToStatus(int n)
{
    if(n==CHECK_REP)
    {
        return CHECK;
    }
    if(n==CHECKMATE_REP)
    {
        return CHECKMATE;
    }
    if(n==TIE_REP)
    {
        return TIE;
    }
    return REGULAR;
}
int isValidMove(CH_Game* src, elem* element)
{
    ASSERT(element!=NULL);
    ASSERT(src!=NULL);
    int row=0, col=0;
    if(element->fig ==CH_EMPTY_ENTRY)
    {
        return ILLEGAL_FIGURE;
    }
    if(!playerFig(src, element->startRow,element->startCol)){
        return ILLEGAL_MOVE;
    }
    if(!figureMove(src,element)){
        return ILLEGAL_MOVE;
    }
    if(!noOverlap(src,element))
    {
        return ILLEGAL_MOVE;
    }
    CH_Game* copy = gameCopy(src);
    if(copy == NULL)
    {
        return ALLOCATION_ERROR;
    }
    doMove(copy, element);
    switchPlayer(copy);
    if(copy->currentPlayer == CH_PLAYER_1)
    {
        row = copy->white_king[0];
        col = copy->white_king[1];
    }
    else
    {
        row = copy->black_king[0];
        col = copy->black_king[1];
    }
    if(pieceUnderAttack(copy, row, col))
    {
        //if the move puts the player in check 
        gameDestroy(copy);
        return ILLEGAL_MOVE;
    }
    gameDestroy(copy);
    return 1;
}
bool playerFig(CH_Game* src, int row, int col)
{
    ASSERT(src!=NULL);
    if(src->gameBoard[row][col]==CH_EMPTY_ENTRY){
        return false;
    }
    if(src->currentPlayer==CH_PLAYER_1){
        if(isupper(src->gameBoard[row][col])){
            return false;
        }
    }
    else{
        if(!isupper(src->gameBoard[row][col])){
            return false;
        }
    }
    return true;
}
bool figureMove(CH_Game* src, elem* element)
{
    ASSERT(src!=NULL);
    ASSERT(element!=NULL);

    switch(element->fig)
    {
        case PAWN_W:
        case PAWN_B:
                return pawnMove(src,element);
        case BISHOP_W:
        case BISHOP_B:
                return bishopMove(element);
        case ROOK_W:
        case ROOK_B:
                return rookMove(element);
        case KNIGHT_W:
        case KNIGHT_B:
                return knightMove(element);
        case QUEEN_W:
        case QUEEN_B:
                return queenMove(element);
        case KING_W:
        case KING_B:
                return kingMove(element);
    }
    return false;
}
bool pawnMove(CH_Game* src, elem* element)
{
    ASSERT(element!=NULL);
    ASSERT(src!=NULL);
    bool check = false;
    if(element->player ==CH_PLAYER_1)
    {
        if(element->startRow==1)//can move two steps
        {
            if(element->destinationRow<=1 || element->destinationRow>3){
                return false;
            }
            check = (element->destinationRow - element->startRow)==2;
        }
        else
        {
            if(element->destinationRow != element->startRow+1){
                return false;
            }
        }
    }
    else
    {
        if(element->startRow==6)
        {
            if(element->destinationRow>=6 || element->destinationRow<4){
                return false;
            }
            check = (element->startRow - element->destinationRow)==2;
        }
        else
        {
            if(element->destinationRow!=element->startRow-1){
                return false;
            }
        }
    }
    if(abs(element->destinationCol-element->startCol)==1)
    {
        //diagonal move is ok if capture
        if(!isOppPiece(src,element->destinationRow, element->destinationCol) || check)
        {
            return false;
        }
    }
    else if(element->destinationCol != element->startCol)
    {
        return false;
    }
    return true;
}
bool isOppPiece(CH_Game* src, int row, int col)
{
    ASSERT(src!= NULL);
    char piece = src->gameBoard[row][col];
    if(getCurrentPlayer(src) == CH_PLAYER_2)
    {
        return (piece == PAWN_W || piece == BISHOP_W || piece == ROOK_W || piece == KNIGHT_W || piece == QUEEN_W || piece == KING_W);
    }
    else
    {
        return (piece == PAWN_B || piece == BISHOP_B || piece == ROOK_B || piece == KNIGHT_B || piece == QUEEN_B || piece == KING_B);
    }
}
bool bishopMove(elem* element)
{
    ASSERT(element!=NULL);
    return isDiagonal(element);
}
bool isDiagonal(elem* element)
{
    ASSERT(element!=NULL);
    int col, row;
    if( (element->destinationCol-element->startCol)==0 && (element->destinationRow-element->startRow)==0)
    {
        return false;
    }
    row = element->destinationRow-element->startRow;
    col = element->destinationCol-element->startCol;
    col = abs(col);
    row = abs(row);
    return (row==col);
}

bool rookMove(elem* element)
{
    ASSERT(element!=NULL);
    if( (element->destinationCol-element->startCol)==0 && (element->destinationRow-element->startRow)==0)
    {
        return false;
    }
    return (!( (element->destinationCol!=element->startCol) && (element->destinationRow!=element->startRow)) );
}

bool knightMove(elem* element)
{
    ASSERT(element!=NULL);
    if( (abs(element->destinationCol-element->startCol)==2) && (abs(element->destinationRow-element->startRow)==1)  )
    {
        return true;
    }
    if( (abs(element->destinationCol-element->startCol)==1) && (abs(element->destinationRow-element->startRow)==2)  )
    {
        return true;
    }
    return false;
}
bool queenMove(elem* element)
{
    ASSERT(element!=NULL);
    return (rookMove(element) || bishopMove(element));
}
bool kingMove(elem* element)
{
    ASSERT(element != NULL);
    if(abs(element->destinationCol-element->startCol)==0 && abs(element->destinationRow-element->startRow)==0)
    {
        return false;
    }
    if(abs(element->destinationCol-element->startCol)>1 || abs(element->destinationRow-element->startRow)>1)
    {
        return false;
    }
    return true;
}

bool noOverlap( CH_Game* src, elem* element)
{
    ASSERT(src!=NULL);
    ASSERT(element!=NULL);

    switch(element->fig)
    {
	case PAWN_W:
	case PAWN_B:
		return pawnOverlap(src, element);
	case BISHOP_W:
	case BISHOP_B:
		return bishopOverlap(src, element);
	case ROOK_W:
	case ROOK_B:
		return rookOverlap(src, element);
	case KNIGHT_W:
	case KNIGHT_B:
	case KING_B:
	case KING_W:
		return kingOrKnightOverlap(src, element);
	case QUEEN_W:
	case QUEEN_B:
		return queenOverlap(src, element);       
    }
    return false;
}
bool pawnOverlap(CH_Game* src, elem* element)
{
    ASSERT(src!=NULL);
    ASSERT(element!=NULL);
    int check_row = 0;

    if(abs(element->destinationRow-element->startRow)==1)
    {
        if(element->startCol==element->destinationCol)
        {
            return isCellEmpty(src, element->destinationRow, element->startCol);
        }
        else
        {
            return isOppPiece(src,element->destinationRow,element->destinationCol);
        }
    }
    else
    {
        if(element->player == CH_PLAYER_1)
        {
            check_row = element->startRow+1;
        }
        else
        {
            check_row = element->startRow-1;
        }
        if(!isCellEmpty(src, check_row,element->startCol))
        {
            return false;
        }
        check_row = element->destinationRow;
        if(!isCellEmpty(src, check_row,element->startCol))
        {
            return false;
        }
    }
    return true;
}
bool isCellEmpty(CH_Game* src, int row, int col)
{
    ASSERT(src!=NULL);
    if(col<0 || col>(GRID-1) || row<0 || row>(GRID-1))
    {
        return false;
    }
    return (src->gameBoard[row][col] == CH_EMPTY_ENTRY);
}
bool bishopOverlap(CH_Game* src, elem* element)
{
    ASSERT(src != NULL);
    ASSERT(element != NULL);
    int row_check = 0;
    int col_check = 0;
    if(element->destinationRow > element->startRow)
    {
        //moving forward
        if( element->destinationCol> element->startCol)
        {
            //moving right diagonal
            row_check = element->startRow+1;
            col_check = element->startCol+1;
            while(row_check != element->destinationRow)
            {
                if (isCellEmpty(src, row_check, col_check) == false){
                    return false;
                }
                row_check +=1;
                col_check +=1;
            }
        }
        else
        {
            //moving left diagonal forward
            row_check = element->startRow + 1;
            col_check = element->startCol - 1;
            while(row_check!=element->destinationRow)
            {
                if(isCellEmpty(src,row_check, col_check) == false){
                    return false;
                }
                row_check += 1;
                col_check -= 1;
            }
        }
    }
    else
    {
        // moving down
        if(element->startCol > element->destinationCol)
        {
            // moving left diagonal
            row_check = element->startRow - 1;
            col_check = element->startCol - 1;
            while(row_check != element->destinationRow)
            {
                if(isCellEmpty(src,row_check, col_check)==false){
                    return false;
                }
                row_check -= 1;
                col_check -= 1;
            }
        }
        else
        {
            // moving right diagon
            row_check = element->startRow -1;
            col_check = element->startCol +1;
            while(row_check!= element->destinationRow)
            {
                if(isCellEmpty(src,row_check, col_check)==false){
                    return false;
                }
                row_check -= 1;
                col_check += 1;
            }
        }
    }
    return (isCellEmpty(src, element->destinationRow, element->destinationCol) || isOppPiece(src, element->destinationRow, element->destinationCol));
}
bool rookOverlap(CH_Game* src, elem* element)
{
    ASSERT(src!=NULL);
    ASSERT(element!=NULL);
    int row_check = 0, col_check = 0;
    if(element->destinationRow == element->startRow)
    {
        // moving horizontally
        if(element->destinationCol> element->startCol)
        {
            col_check = element->startCol + 1;
            while(col_check != element->destinationCol)
            {
                // moving right
                if(isCellEmpty(src, element->startRow, col_check)==false)
                {
                    return false;
                }
                col_check += 1;
            }
        }
        else
        {
            // moving left
            col_check = element->startCol -1;
            while(col_check != element->destinationCol)
            {
                if(isCellEmpty(src, element->startRow, col_check)==false)
                {
                    return false;
                }
                col_check -= 1;
            }
        }
    }
    else
    {
        // moving vertically
        if(element->destinationRow > element->startRow)
        {
            // moving up
            row_check = element->startRow +1;
            while (row_check != element->destinationRow)
            {
                if(isCellEmpty(src, row_check, element->startCol)==false)
                {
                    return false;
                }
                row_check += 1;
            }
            
        }
        else
        {
            // moving down
            row_check = element->startRow -1;
            while(row_check != element->destinationRow)
            {
                if(isCellEmpty(src, row_check, element->startCol)==false)
                {
                    return false;
                }
                row_check -= 1;
            }
        }
    }
    return (isCellEmpty(src, element->destinationRow, element->destinationCol) || isOppPiece(src, element->destinationRow, element->destinationCol));
}
bool queenOverlap(CH_Game* src, elem* element)
{
    ASSERT(element != NULL);
	ASSERT(src != NULL);
    if(isDiagonal(element))
    {
        return bishopOverlap(src, element);
    }
    else{
        return rookOverlap(src, element);
    }
}
bool kingOrKnightOverlap(CH_Game* src, elem* element)
{
    ASSERT(src!= NULL);
    ASSERT(element!=NULL);
    return (isCellEmpty(src, element->destinationRow, element->destinationCol) || isOppPiece(src, element->destinationRow, element->destinationCol));
}
void doMove(CH_Game* src, elem* element)
{
    ASSERT(src!=NULL);
    // int i = 3;
    element->prevFig = src->gameBoard[element->destinationRow][element->destinationCol];//save in the history list the captured piece
    if(element->player == CH_PLAYER_1 && element->fig == PAWN_W && element->destinationRow == 7){
        if(src->piece_promotion[3]){
            src->gameBoard[element->destinationRow][element->destinationCol] = QUEEN_W;
            src->white[4]++;
            element->fig = QUEEN_W;
            src->piece_promotion[3]--;
        }
        else if(src->piece_promotion[2]){
            src->gameBoard[element->destinationRow][element->destinationCol] = ROOK_W;
            src->white[3]++;
            element->fig = ROOK_W;
            src->piece_promotion[2]--;
        }
        else if(src->piece_promotion[1]){
            src->gameBoard[element->destinationRow][element->destinationCol] = BISHOP_W;
            src->white[2]++;
            element->fig = BISHOP_W;
            src->piece_promotion[1]--;
        }
        else if(src->piece_promotion[0]){
            src->gameBoard[element->destinationRow][element->destinationCol] = KNIGHT_W;
            src->white[1]++;
            element->fig = KNIGHT_B;
            src->piece_promotion[0]--;
        }
        else{
            src->gameBoard[element->destinationRow][element->destinationCol] = element->fig;//putting the piece int the deisred place
        }
    }
    else if(element->player == CH_PLAYER_2 && element->fig == PAWN_B && element->destinationRow == 0){
        if(src->piece_promotion[3]){
            src->gameBoard[element->destinationRow][element->destinationCol] = QUEEN_B;
            src->black[4]++;
            element->fig = QUEEN_B;
            src->piece_promotion[3]--;
        }
        else if(src->piece_promotion[2]){
            src->gameBoard[element->destinationRow][element->destinationCol] = ROOK_B;
            src->black[3]++;
            element->fig = ROOK_B;
            src->piece_promotion[2]--;
        }
        else if(src->piece_promotion[1]){
            src->gameBoard[element->destinationRow][element->destinationCol] = BISHOP_B;
            src->black[2]++;
            element->fig = BISHOP_B;
            src->piece_promotion[1]--;
        }
        else if(src->piece_promotion[0]){
            src->gameBoard[element->destinationRow][element->destinationCol] = KNIGHT_B;
            src->black[1]++;
            element->fig = KNIGHT_B;
            src->piece_promotion[0]--;
        }
        else{
            src->gameBoard[element->destinationRow][element->destinationCol] = element->fig;//putting the piece int the deisred place
        }
    }
    else{
        src->gameBoard[element->destinationRow][element->destinationCol] = element->fig;//putting the piece int the deisred place
    }

    src->gameBoard[element->startRow][element->startCol] = CH_EMPTY_ENTRY;//making the previous cell empty
    piecesUpdate(src, element->prevFig, false, getCurrentPlayer(src)); //Updates the piece vector
    kingUpdate(src, element->fig, element->destinationRow, element->destinationCol);//updates the king's position
    if(ArrayListIsFull(src->undo_hist))
    {
        ArrayListRemoveFirst(src->undo_hist);
    }
    ArrayListAddLast(src->undo_hist, element);//updates the history array that contains previous moves
    switchPlayer(src);

}
void piecesUpdate(CH_Game* src, char figure, bool add, char player)
{
    ASSERT(src!=NULL);
    int index = -1;
    switch(figure)
    {
        // the index in the pieces vector of the piece that was captures
        case PAWN_B:
        case PAWN_W:
            index = PAWN_INDEX;
            break;
        case KNIGHT_B:
        case KNIGHT_W:
            index = KNIGHT_INDEX;
            break;
        case BISHOP_B:
        case BISHOP_W:
            index = BISHOP_INDEX;
            break;
        case ROOK_B:
        case ROOK_W:
            index = ROOK_INDEX;
            break;
        case QUEEN_B:
        case QUEEN_W:
            index = QUEEN_INDEX;
            break;
        case KING_B:
        case KING_W:
            index = KING_INDEX;
            break;
    }
    if(index != -1)
    {//updates the piece vector according to the current player
        if(!add)
        {
            if(player == CH_PLAYER_2)
            {
                src->white[index]--;
                if(index<5 && index>0)
                 src->piece_promotion[index-1] ++;
            }
            else{
                src->black[index]--;
                if(index<5 && index>0)
                 src->piece_promotion[index-1] ++;
            }
        }
        else
        {
            if(player == CH_PLAYER_2)
            {
                src->white[index]++;
            }
            else{
                src->black[index]++;
            }
        }
    }
}
char playerPC(CH_Game* src)
{
    if(src->user_color == 0)
    {
        return CH_PLAYER_1;
    }
    return CH_PLAYER_2;
}

void kingUpdate(CH_Game* src, char fig, int row, int col )
{
    ASSERT(src!=NULL);
    if(fig == KING_W)
    {
        src->white_king[0] = row;
        src->white_king[1] = col;
    }
    else if(fig == KING_B)
    {
        src->black_king[0] = row;
        src->black_king[1] = col;
    }
}
void switchPlayer(CH_Game* src)
{
    ASSERT(src!=NULL);
    if((getCurrentPlayer(src))==CH_PLAYER_1)
    {
        src->currentPlayer = CH_PLAYER_2;
    }
    else{
        src->currentPlayer = CH_PLAYER_1;
    }
}

bool pieceUnderAttack(CH_Game* src, int row, int col)
{
    ASSERT(src!=NULL);
    elem* element = NULL;
    int move[4] = {0};
    int pos[2] = {0};//represents the kings position of current player
    pos[0] = row;
    pos[1] = col;
    for(int i=0; i<GRID; ++i)
    {
        for(int j=0; j<GRID; ++j)
        {
            if(isOppPiece(src,i,j))
            {
                // check if the figure is opponents figure
                move[START_ROW_PLACE] = i+1;
                move[START_COL_PLACE] = j+1;
                move[DESTINATION_ROW_PLACE] = pos[0] + 1;
                move[DESTINATION_COL_PLACE] = pos[1] + 1;
                switchPlayer(src);
                element = transfer(src, move);//try to capture the move
                if(element==NULL){
                    failGame(src,true);
                }
                if(figureMove(src,element) && noOverlap(src,element))
                {
                   //if valid move
                    free(element);
                    switchPlayer(src);
                    return true;
                }
                switchPlayer(src);
                free(element);
            }
        }
    }
    return false;
}
void failGame(CH_Game* game, bool destroy)
{
    printf("ERROR: malloc has failed\n");
    if(destroy){
        gameDestroy(game);
    }
    exit(0);
}
CH_STATUS updateStatus(CH_Game* src)
{
    if(isCheck(src))
    {
        if(isCheckMate(src)){
            return CHECKMATE;
        }
        return CHECK;
    }
    else
    {
        if(isTie(src)){
            return TIE;
        }
        else if(src->mv_cnt>=100 && src->mode==DEFAULT_MODE){
            return TIE;
        }
        else if(src->mv_cnt>=100 && src->mode == TWO_PLAYER_MODE){
            return TIE;
        }
    }
    return REGULAR;
}
bool isTie(CH_Game *src)
{
	ASSERT(src != NULL);
	return noPossibleMoves(src); // true if no moves
}
bool isCheck(CH_Game* src)
{
    bool check1, check2;
    int row = 0, col = 0;
    if(src->currentPlayer == CH_PLAYER_1)
    {
        // white king position
        row = src->white_king[0];
        col = src->white_king[1];
    }
    else
    {
        row = src->black_king[0];
        col = src->black_king[1];
    }
    check1 = pieceUnderAttack(src, row, col);// check if current players king is under attack
    if(check1){
        return check1;
    }
    //check if other player king is under attack
    switchPlayer(src);
    if(src->currentPlayer == CH_PLAYER_1)
    {
        // white king position
        row = src->white_king[0];
        col = src->white_king[1];
    }
    else
    {
        row = src->black_king[0];
        col = src->black_king[1];
    }
    check2 = pieceUnderAttack(src, row, col);// check if other players king is under attack
    switchPlayer(src);
    return check2;
}
bool isCheckMate(CH_Game* src)
{
    return noPossibleMoves(src);
}
bool noPossibleMoves(CH_Game* src)
{
    ASSERT(src!=NULL);
    int **arr = NULL;
    int num_of_moves = 0;
    for(int i=0; i<GRID; ++i)
    {
        for(int j=0; j<GRID; ++j)
        {
            if(playerFig(src,i,j))
            {
                arr = possibleMoves(src,i,j);//gets the piece's possible moves
                if(arr==NULL){
                    failGame(src,true);
                }
                num_of_moves = numOfMoves(src->gameBoard[i][j]);
                if(arr[0][0] != -1)//if true then there is a possible move
                {
                    freeArray(arr, num_of_moves);
                    return false;
                }
                freeArray(arr, num_of_moves);
            }
        }
    }
    return true;

}
int **possibleMoves(CH_Game *src, int row, int col)
{
	ASSERT(src != NULL);
	switch (src->gameBoard[row][col])
	{ 
        //Checks if the move given doesn't overlap other pieces
	    case PAWN_W:
	    case PAWN_B:
	    	return possiblePawnMoves(src, row, col);
	    case BISHOP_W:
	    case BISHOP_B:
	    	return possibleBishopMoves(src, row, col);
	    case ROOK_W:
	    case ROOK_B:
	    	return possibleRookMoves(src, row, col);
	    case KNIGHT_W:
	    case KNIGHT_B:
	    	return possibleKnightMoves(src, row, col);
	    case QUEEN_W:
	    case QUEEN_B:
	    	return possibleQueenMoves(src, row, col);
	    case KING_W:
	    case KING_B:
		    return possibleKingMoves(src, row, col);
	}
	return NULL;
}
int **possiblePawnMoves(CH_Game* src, int row, int col)
{
    int index = 0;
    bool add_move = false;
    int ** arr = arrayInit(PAWN_MOVES);// Creates an array of all possible moves
    if(!arr){
        return NULL;
    }
    if(src->currentPlayer == CH_PLAYER_1)
    {
        add_move = addToMoveArray(src, row, col, row + 2, col, arr, index); //move two steps forwards
		if (add_move)
			index++;
		add_move = addToMoveArray(src, row, col, row + 1, col, arr, index); //move one step forward
		if (add_move)
			index++;
		add_move = addToMoveArray(src, row, col, row + 1, col + 1, arr, index); //diagonal right
		if (add_move)
			index++;
		add_move = addToMoveArray(src, row, col, row + 1, col - 1, arr, index); //diagonal left
		if (add_move)
			index++;
    }
    else
	{
		add_move = addToMoveArray(src, row, col, row - 2, col, arr, index); //move two steps forwards
		if (add_move)
			index++;
		add_move = addToMoveArray(src, row, col, row - 1, col, arr, index); //move one step forward
		if (add_move)
			index++;
		add_move = addToMoveArray(src, row, col, row - 1, col + 1, arr, index); //diagonal right
		if (add_move)
			index++;
		add_move = addToMoveArray(src, row, col, row - 1, col - 1, arr, index); //diagonal left
		if (add_move)
			index++;
	}
    return arr;
}
int **arrayInit(int size)
{
    int **arr = (int **)malloc(size*sizeof(int*));
    if(!arr){
        return NULL;
    }
    for(int i=0; i<size; ++i)
    {
        arr[i] = (int*)malloc(CELL_SIZE*sizeof(int));
        if(!arr[i])
        {
            freeArray(arr,i);
            return NULL;
        }
        arr[i][0] = -1;
        arr[i][1] = -1; 
    }
    return arr;
}
bool addToMoveArray(CH_Game* src, int row, int col, int i, int j, int **arr, int index)
{
    ASSERT(src!=NULL);
    ASSERT(arr!=NULL);
    elem* element = NULL;
    int res, move[CELL_SIZE*CELL_SIZE] = {0};
    if (row < 0 || row > (GRID - 1) || col < 0 || col > (GRID - 1) || i < 0 || i > (GRID - 1) || j < 0 || j > (GRID - 1) || (row == i && col == j))
	{
        return false;
    }
    move[START_ROW_PLACE] = row+1;
    move[START_COL_PLACE] = col+1;
    move[DESTINATION_ROW_PLACE] = i+1;
    move[DESTINATION_COL_PLACE] = j+1;
    element = transfer(src, move);
    if(element == NULL){
        failGame(src,true);
    }
    res = isValidMove(src, element);
    if(res == ALLOCATION_ERROR)
    {
        free(element);
        failGame(src, true);
    }
    if(res==1)
    {
        //adds to move array if the move is valid
        arr[index][0] = i;
        arr[index][1] = j;
        free(element);
        return true;
    }
    free(element);
    return false;
}
void freeArray(int **arr, int size)
{
	for (int i = 0; i < size; i++)
    {
        free(arr[i]);
    }
	free(arr);
}

int **possibleBishopMoves(CH_Game *src, int row, int col)
{
	int i = 0, j = 0, index = 0;
	bool add_move = false;
	int **arr = arrayInit(BISHOP_MOVES); //creates an array of all possible moves
	
    if (!arr){
        return NULL;
    }
		
	    i = row - 1;
	    j = col - 1;
	//checking all 13 possible moves of the knight if they are valid
	while ((i >= 0 && i <= (GRID - 1)) && (j >= 0 && j <= (GRID - 1))) //right backwards diagonal
	{
		add_move = addToMoveArray(src, row, col, i, j, arr, index);
		if (add_move)
        {
            index++;
        }
		i--;
		j--;
	}
	    i = row - 1;
	    j = col + 1;
	while ((i >= 0 && i <= (GRID - 1)) && (j >= 0 && j <= (GRID - 1))) //left backwards diagonal
	{
		add_move = addToMoveArray(src, row, col, i, j, arr, index);
		if (add_move)
        {
			index++;
        }
		i--;
		j++;
	}
	    i = row + 1;
	    j = col - 1;
	while ((i >= 0 && i <= (GRID - 1)) && (j >= 0 && j <= (GRID - 1))) //left forward diagonal
	{
		add_move = addToMoveArray(src, row, col, i, j, arr, index);
		if (add_move){
            index++;
        }
		i++;
		j--;
	}
	    i = row + 1;
	    j = col + 1;
	while ((i >= 0 && i <= (GRID - 1)) && (j >= 0 && j <= (GRID - 1))) //left forward diagonal
	{
		add_move = addToMoveArray(src, row, col, i, j, arr, index);
		if (add_move){
			index++;
        }
		i++;
		j++;
	}
	return arr;
}
int **possibleRookMoves(CH_Game *src, int row, int col)
{
	int index = 0;
	bool add_move = false;
	int **arr = arrayInit(ROOK_MOVES); //creates an array of all possible moves
	
    if (!arr){
		return NULL;
    }
	//checking all 14 possible moves of the knight if they are valid
	for (int i = row + 1; i < GRID; ++i)
	{ 
        //moving forward
		add_move = addToMoveArray(src, row, col, i, col, arr, index);
		if (add_move){
            index++;
        }
	}
	for (int i = row - 1; i >= 0; --i)
	{ 
        //moving backwards
		add_move = addToMoveArray(src, row, col, i, col, arr, index);
		if (add_move){
            index++;
        }
	}
	for (int j = col + 1; j < GRID; ++j)
	{ 
        //moving right
		add_move = addToMoveArray(src, row, col, row, j, arr, index);
		if (add_move){
            index++;
        }
	}
	for (int j = col - 1; j >= 0; --j)
	{ 
        //moving left
		add_move = addToMoveArray(src, row, col, row, j, arr, index);
		if (add_move){
            index++;
        }
	}
	return arr;
}
int **possibleKnightMoves(CH_Game *src, int row, int col)
{
	int index = 0;
	int **arr = arrayInit(KNIGHT_MOVES); //creates an array of all possible moves
	if (!arr){
        return NULL;
    }
		
	index = 0;
	bool add_move = false;

	//checking all 8 possible moves of the knight if they are valid
	add_move = addToMoveArray(src, row, col, row + 2, col + 1, arr, index);
	if(add_move)
    {
        index++;
    }
		
	add_move = addToMoveArray(src, row, col, row + 1, col + 2, arr, index);
	if(add_move)
    {
        index++;
    }
	add_move = addToMoveArray(src, row, col, row + 2, col - 1, arr, index);
	if(add_move)
    {
        index++;
    }
	add_move = addToMoveArray(src, row, col, row + 1, col - 2, arr, index);
	if(add_move)
    {
        index++;
    }
	add_move = addToMoveArray(src, row, col, row - 2, col - 1, arr, index);
	if(add_move)
    {
        index++;
    }
	add_move = addToMoveArray(src, row, col, row - 1, col - 2, arr, index);
	if(add_move)
    {
        index++;
    }
	add_move = addToMoveArray(src, row, col, row - 2, col + 1, arr, index);
	if(add_move)
    {
        index++;
    }
	add_move = addToMoveArray(src, row, col, row - 1, col + 2, arr, index);
	if(add_move)
    {
        index++;
    }
	return arr;
}
int **possibleQueenMoves(CH_Game *src, int row, int col)
{
	int index = 0;
	int **rookarr = NULL;
	int **bishoparr = NULL;
	int **arr = arrayInit(QUEEN_MOVES); //creates an array of all possible moves
	
    if (!arr){
        return NULL;
    }
		
	//queen moves:bishop moves+rook moves
	rookarr = possibleRookMoves(src, row, col);
	bishoparr = possibleBishopMoves(src, row, col);

	if((!rookarr) || (!bishoparr))
	{
		freeArray(arr, QUEEN_MOVES);
		if ((!rookarr) && (!bishoparr))
		{
			freeArray(rookarr, ROOK_MOVES);
			freeArray(bishoparr, BISHOP_MOVES);
		}
		else if ((!bishoparr))
			freeArray(bishoparr, BISHOP_MOVES);
		else if ((!rookarr))
			freeArray(rookarr, ROOK_MOVES);
		return NULL;
	}
	for(int i = 0; i < ROOK_MOVES; ++i)
	{ 
        //add possible rook moves to array
		if (rookarr[i][0] != -1)
		{
			arr[index][0] = rookarr[i][0];
			arr[index][1] = rookarr[i][1];
			index++;
		}
		free(rookarr[i]);
	}
	for(int i = 0; i < BISHOP_MOVES; ++i)
	{ 
        //add possible bishop moves to array
		if (bishoparr[i][0] != -1)
		{
			arr[index][0] = bishoparr[i][0];
			arr[index][1] = bishoparr[i][1];
			index++;
		}
		free(bishoparr[i]);
	}
	free(rookarr);
	free(bishoparr);
	return arr;
}
int **possibleKingMoves(CH_Game *src, int row, int col)
{
	int index = 0;
	bool add_move = false;
	int **arr = arrayInit(KING_MOVES); //creates an array of all possible moves
	
    if (!arr){
        return NULL;
    }
		
	for(int i = row - 1; i <= row + 1; ++i)
	{ 
        //the king has 8 moves options
		for (int j = col - 1; j <= col + 1; ++j)
		{
			if ((i >= 0 && i <= (GRID - 1)) && (j >= 0 && j <= (GRID - 1)))
			{
				add_move = addToMoveArray(src, row, col, i, j, arr, index); //checks if the move valid
				if (add_move){
                    index++; //returns true if the move valid
                }	
			}
		}
	}
	return arr;
}
int numOfMoves(char ch)
{
	switch (ch)
	{ 
        //the number of piece moves
	    case PAWN_B:
	    case PAWN_W:
	    	return PAWN_MOVES;
	    case BISHOP_B:
	    case BISHOP_W:
	    	return BISHOP_MOVES;
	    case ROOK_B:
	    case ROOK_W:
	    	return ROOK_MOVES;
	    case KNIGHT_W:
	    case KNIGHT_B:
	    	return KNIGHT_MOVES;
	    case QUEEN_W:
	    case QUEEN_B:
	    	return QUEEN_MOVES;
	    case KING_W:
	    case KING_B:
	    	return KING_MOVES;
	}
	return 0;
}

bool gameAssign(CH_Game *src, CH_Game *gamecopy)
{

    ASSERT(src!=NULL);
	ASSERT(gamecopy!=NULL);

	copyParams(src->undo_hist, gamecopy->undo_hist); //copies undo hist params
	copyParameters(src, gamecopy);					 //copies params from src to gamecopy
	return true;
}
CH_GAME_MESSAGE undoPrevMove(CH_Game *src)
{
	int scol = 0, srow = 0, dcol = 0, drow = 0;

	if (!src){
        return CH_INVALID_ARGUMENT;
    }
	if (ArrayListIsEmpty(src->undo_hist)) //checks if the move can be done
	{
        return CH_NO_HISTORY;
    }	
	scol = ArrayListGetLast(src->undo_hist)->startCol; //where to move back the figure (col)
	srow = ArrayListGetLast(src->undo_hist)->startRow; //where to move back the figure (row)
	dcol = ArrayListGetLast(src->undo_hist)->destinationCol;	 //where to place the figure that was captured (col)
	drow = ArrayListGetLast(src->undo_hist)->destinationRow;	 //where to place the figure that was captured (row)
	
    kingUpdate(src, ArrayListGetLast(src->undo_hist)->fig, ArrayListGetLast(src->undo_hist)->startRow,
    ArrayListGetLast(src->undo_hist)->startCol);				  //update's king's place
	
    src->gameBoard[srow][scol] = ArrayListGetLast(src->undo_hist)->fig;	  //moving back the figure
	src->gameBoard[drow][dcol] = ArrayListGetLast(src->undo_hist)->prevFig; //restore the captured figure
	src->game_status = intToStatus(ArrayListGetLast(src->undo_hist)->gameStatus);
	
    switchPlayer(src); //switch player
	piecesUpdate(src, ArrayListGetLast(src->undo_hist)->prevFig, true, getCurrentPlayer(src));
	ArrayListRemoveLast(src->undo_hist); //remove from history list;
	
    return CH_SUCCESS;
}
char checkWinner(CH_Game *src)
{
	ASSERT(src != NULL);
    
	if (src->game_status == CHECKMATE)
	{											  
		if (CH_PLAYER_1 == getCurrentPlayer(src)) //the loser is the one who's king cannot be saved
		{
            return CH_PLAYER_2;
        }	
		else{
            return CH_PLAYER_1;
        }
			
	}
	if (src->game_status == TIE){
        return CH_TIE;
    }		
	return '\0'; //continue playing
}
int comp(const void *arg1, const void *arg2)
{
	//compare function for qsort
	const int *a = *(const int **)arg1;
    const int *b = *(const int **)arg2;
	if (a[0] < b[0])
	{	
        return -1;
    }
	if (a[0] > b[0])
	{
        return 1;
    }	
	if (a[1] < b[1]){
        return -1;
    }
		
	else{
        return 1;
    }
		
}
CH_GAME_MESSAGE getMovesBonus(CH_Game *src, int row, int col, char **board, bool gui)
{
	ASSERT(src != NULL);
	if (gui){
        ASSERT(board != NULL);
    }
		
	char fig;
	row--;
	col--; //makes indexes zero dependent
	int **arr = NULL, num_of_moves = 0;
	CH_Game *copy = NULL;

	if (row < 0 || row > GRID - 1 || col < 0 || col > GRID - 1)
	{ 
		printf("Invalid position on the board\n");
		return CH_SUCCESS;
	}
	
    if ((isCellEmpty(src, row, col)) || (isOppPiece(src, row, col)))
	{ 
        //checks whether there is a fig piece
		if (src->currentPlayer == CH_PLAYER_1)
		{
            	printf("The specified position does not contain white player piece\n");
        }
        else
		{	
            printf("The specified position does not contain black player piece\n");
        }
        return CH_SUCCESS;
	}

	arr = possibleMoves(src, row, col);

	if (arr == NULL)
	{
        return CH_INVALID_ARGUMENT;
    }
    if (arr[0][0] == -1)
	{
		printf("\n"); // no possible moves
		return CH_SUCCESS;
	}
	
    num_of_moves = numOfMoves(src->gameBoard[row][col]); //get num of fig moves
	qsort(arr, num_of_moves, sizeof(int *), comp);		 //sort the fig moves
	copy = gameCopy(src);
	
    if (copy == NULL)
	{
		freeArray(arr, num_of_moves);
		return CH_INVALID_ARGUMENT;
	}

	for(int i = 0; i < num_of_moves; ++i)
	{
		if(arr[i][0] != -1)
		{
			fig = copy->gameBoard[arr[i][0]][arr[i][1]];
			copy->gameBoard[arr[i][0]][arr[i][1]] = copy->gameBoard[row][col]; //make the move
			copy->gameBoard[row][col] = CH_EMPTY_ENTRY;
			printf("<%d,%c>", arr[i][0] + 1, arr[i][1] + 65);
			if(gui)
			{
                board[arr[i][0]][arr[i][1]] = '0'; //can move to this cell
            }	
			if(pieceUnderAttack(copy, arr[i][0], arr[i][1]))
			{ 
                //checks if the piece is threatened
				printf("*");
				if (gui)
                {
                    board[arr[i][0]][arr[i][1]] = '1'; // piece can be attacked
                }
			}
			if (isOppPiece(src, arr[i][0], arr[i][1]))
			{ //check if the piece eats other fig
				printf("^");
				if (gui)
				{
					if (board[arr[i][0]][arr[i][1]] != '1') // can eat opp piece
					{
                        board[arr[i][0]][arr[i][1]] = '2';
                    }	
				}
			}
			printf("\n");
			copy->gameBoard[row][col] = copy->gameBoard[arr[i][0]][arr[i][1]]; //undo the move
			copy->gameBoard[arr[i][0]][arr[i][1]] = fig;
		}
	}
	freeArray(arr, num_of_moves);
	gameDestroy(copy);
	return CH_SUCCESS;
}



