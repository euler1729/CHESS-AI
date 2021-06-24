#include "ConsoleMode.h"

void startGame()
{
    CH_Game* game = gameCreate(HISTORY_SIZE, DEFAULT_DIFFICULTY, DEFAULT_COLOR, DEFAULT_MODE);
    if(game==NULL){
        failGame(game,false);
    }
    settingState(game);
    if(game->mode == TWO_PLAYER_MODE)
    {
        twoPlayerMode(game);
    }
    else{
        onePlayerMode(game);
    }

}

void settingState(CH_Game* game)
{
    ASSERT(game!=NULL);
    SetCommand command;

    printf("Specify the game Setting or type 'start' to begin with default Setting:\n ");
    while (1)//get the setting command from the user
    {
        command = getNewSetCommand();
        // printf("->%d%d%d%d\n",command.cmd.move[0],command.move[1],command.move[2],command.move[3]);
        if(command.cmd == CH_GAME_MODE){
            gameMode(game,command.arg);
        }
        else if(command.cmd==CH_DIFFICULTY){
            gameDifficulty(game,command.arg);
        }
        else if(command.cmd == CH_USER_COLOR){
            gameColor(game, command.arg);
        }
        else if(command.cmd == CH_LOAD){
            gameLoad(game, command.path);
        }
        else if(command.cmd==CH_DEFAULT){
            gameDefault(game);
        }
        else if(command.cmd==CH_PRINT_SETTING){
            gamePrintSetting(game);
        }
        else if(command.cmd== CH_QUIT){
            gameQuit(game, true);
        }
        else if(command.cmd==CH_START){
            break;
        }
    }
}

SetCommand getNewSetCommand()
{
    SetCommand command;
    char line[SP_MAX_LINE_LENGTH];
    while(1)//reading as stdin until gets a valid command
    {
        fgets(line, SP_MAX_LINE_LENGTH, stdin);//gets input from user
        int len = strlen(line) - 1;
        if(line[len] == '\n'){//puts a null
            line[len] = '\0';
        }
        command = parserSetParseLine(line);//parsing the read string into command
        if(command.cmd != CH_INVALID_LINE){
            return command;
        }
        printf("ERROR: invalid command\n");
    }
}
void gameMode(CH_Game* game, int arg)
{
    ASSERT(game!=NULL);

    if(arg == DEFAULT_MODE){
        game->mode = arg;
        printf("Game is set to AI mode.\n");
    }
    else if(arg==TWO_PLAYER_MODE){
        game->mode = arg;
        printf("Game mode is set to HUMAN vs HUMAN mode.\n");
    }
    else{
        printf("Invalid game mode.\n");
    }
}
void gameDifficulty(CH_Game* game, int arg)
{
    ASSERT(game!=NULL);
    if(game->mode == TWO_PLAYER_MODE){// in human vs human there is no difficulty level
        printf("ERRPR: invalid command\n");
    }
    else{
        if(arg == EXPERT_LEVEL){
            printf("Please choose a level between 1 to 7:\n");
        }
        else if(arg<1 || arg>EXPERT_LEVEL){
            printf("Invalid Difficulty level. Please choose between 1 to 7:\n ");
        }
        else{
            game->difficulty = arg;
        }
    }
}
void gameColor(CH_Game* game, int arg)
{
    ASSERT(game!=NULL);

    if(game->mode == TWO_PLAYER_MODE){
        printf("In HUMAN vs HUMAN mode, setting up color is unneccessary.\n");
    }
    else{
        game->user_color = arg;
    }
}
void gameLoad(CH_Game* game, char* path)
{
    ASSERT(game != NULL);
    ASSERT(path!=NULL);

    CH_Game* loadGame;
    FILE* fxml = fopen(path, "r");
    
    if(!fxml){
        printf("ERROR: File doesn't exist or cannot be opened.\n");
    }
    else{
        loadGame = fileToGame(fxml);//load game from file
        if(loadGame==NULL){
            failGame(game,true);
        }
        gameAssign(loadGame, game);//copies the loaded game to the game that the console has
        gameDestroy(loadGame);
        if(ferror(fxml)){
            printf("ERROR: File doesn't exist or cannot be opened.\n");
        }
        else if(fclose(fxml)!=0 ){
            printf("ERROR: File doesn't exist or cannot be opened.\n");
        }
    }
}
void gameDefault(CH_Game* game)
{
    ASSERT(game!=NULL);
    game->mode = DEFAULT_MODE;
    game->difficulty = DEFAULT_DIFFICULTY;
    game->user_color = DEFAULT_COLOR;
}
void gamePrintSetting(CH_Game* game)
{
    ASSERT(game!=NULL);
    if(game->mode != DEFAULT_MODE){//default is AI mode whose numerical value is 1
        printf("SETTINGS:\n");
        printf("GAME_MODE: 2\n");
    }
    else{
        printf("SETTINGS:\n");
        printf("GAME_MODE: 1\n");
        printf("DIFFICULTY_LVL: %d\n", game->difficulty);
        if(game->user_color ==0){
            printf("USER_COlOR: BLACK\n");
        }
        else{
            printf("USER_COLOR: WHITE\n");
        }
    }
}
void gameQuit(CH_Game* game, bool check)
{
    ASSERT(game!=NULL);
    gameDestroy(game);
    if(check){
        printf("Exiting....\n");
    }
    exit(0);
}
void twoPlayerMode(CH_Game* game)
{
    ASSERT(game!=NULL);
    bool check = true;
    GameCommand command;
    
    while(1){
        command = getNewGameCommand(game, check);// gets new game command from user
        check = gameState(game, command);
        if(command.cmd == CH_SAVE){
            check = false;
        }
        if(check){
            statusCheck(game);//check for a winner or change in game status
        }
    }
}


GameCommand getNewGameCommand(CH_Game* game, bool to_print)
{
    GameCommand command;
    char line[SP_MAX_LINE_LENGTH];

    if(to_print){
        gamePrintBoard(game);
    }
    while(1)
    {
        printf("Enter move- %s: ",color(game->currentPlayer));
        fgets(line, SP_MAX_LINE_LENGTH, stdin);//gets user input
        int len = strlen(line)-1;
        if(line[len] == '\n'){
            line[len] = '\0';
        }
        printf("%s\n",line);
        command = parserGameParseLine(line);
        if(command.cmd != CH_GINVALID_LINE){
            return command;
        }
        printf("ERROR: Invalid Command.\n");
    }
}
char *color(char ch){
    if(ch=='F'){
        return "white";
    }
    else{
        return "black";
    }
}
bool gameState(CH_Game* game, GameCommand command)
{
    ASSERT(game!=NULL);
    int move[CELL_SIZE*CELL_SIZE];
    for(int i=0; i<CELL_SIZE*CELL_SIZE; ++i){
        move[i] = command.move[i];
    }
    if(command.cmd==CH_MOVE){
        return gameMove(game,move);
    }
    else if(command.cmd ==CH_GET_MOVES){
        return gameGetMoves(game, move);
    }
    else if(command.cmd==CH_SAVE){
        return gameSave(game, command.path);
    }
    else if(command.cmd==CH_UNDO){
        return gameUndoMove(game);
    }
    else if(command.cmd==CH_RESET){
        gameReset(game);
    }
    else if(command.cmd==CH_GQUIT){
        gameQuit(game, true);
    }
    return true;
}

bool gameMove(CH_Game* game, int* move)
{
    ASSERT(game!= NULL);
    ASSERT(move!=NULL);
    int result = setMove(game,move);
    if(result == ALLOCATION_ERROR){
        failGame(game, true);
    }
    if(result==POSITION_INVALID){
        printf("Invalid position on the board\n");
        return false;
    }
    else if(result == ILLEGAL_FIGURE){
        printf("The specified position does not contain your piece.\n");
        return false;
    }
    else if(result == ILLEGAL_MOVE){
        printf("Illegal Move.\n");
        return false;
    }
    return true;
    
}

bool gameGetMoves(CH_Game* game, int* move)
{
    ASSERT(game!=NULL);
    ASSERT(move!=NULL);
    if(game->mode == TWO_PLAYER_MODE ||(game->difficulty > DEFAULT_DIFFICULTY)){
        printf("ERROR: invalid command\n");
        return false;
    }
    if(getMovesBonus(game, move[START_ROW_PLACE],move[START_COL_PLACE],NULL,false)==CH_INVALID_ARGUMENT)
    {//gets the move acoording to move give
        failGame(game, true);
    }
    return true;
}
bool gameSave(CH_Game* game, char* path){
    ASSERT(game!=NULL);
    ASSERT(path!=NULL);
    XML_MESSAGE message;
    FILE* fxml = fopen(path, "w");
    if(fxml==NULL){
        printf("File cannot be created or modified\n");
        return false;
    }
    else{
        message = gameToFile(game,fxml);
    }
    if(message==XML_ALLOC){
        fclose(fxml);
        failGame(game, true);
    }
    if( (message==XML_FAILED) || ferror(fxml)){
        printf("File cannot be created or modified\n");
        return false;
    }
    if(fclose(fxml)!=0){
        printf("File cannot be created or modified\n");
        return false;
    }
    return true;
}

void gameReset(CH_Game* game){
	ASSERT(game!=NULL);
	gameDestroy(game);
	printf("Restarting...\n");
	startGame(); //start new game
}
bool gameUndoMove(CH_Game* game)
{
    ASSERT(game!= NULL);
    CH_GAME_MESSAGE status;
    
    if(game->mode==TWO_PLAYER_MODE){
        printf("Undo Feature is not available in 2 players mode.\n");
        return false;
    }
    if(game->difficulty>3){
        printf("Undo unavailable for Moderate and Hard level.\n");
        return false;
    }
    if(game->undo_hist->actualSize<1){
        printf("Empty history, move cannot be undone.\n");
        return false;
    }
    // elem* element;
    // element = ArrayListGetLast(game->undo_hist);
    status = undoPrevMove(game);
    if(status != CH_SUCCESS){
        return false;
    }
    game->mv_cnt -=1;
    // printf("Undo move for player %s: <%c%d> -> <%c%d>\n",color(game->currentPlayer),element->destinationCol+ASCII_OFFSET+1,element->destinationRow+1,element->startCol+ASCII_OFFSET+1,element->startRow+1);
    return true;
}
void statusCheck(CH_Game* game)
{
    ASSERT(game!=NULL);
    if(checkWinner(game)!='\0')
    {
        if(game->game_status ==CHECKMATE)
        {
            switchPlayer(game);
            printf("CHECKMATE! %s player wins the game!\n",color(game->currentPlayer));
            gameQuit(game,false);
        }
        else{
            printf("Game TIED!\n");
            gameQuit(game, false);
        }
    }
    if(game->game_status==CHECK){
        printf("Check: %s KING is in CHECK!\n",color(game->currentPlayer));
    }
}
void onePlayerMode(CH_Game* game)
{
    ASSERT(game!=NULL);
    bool check = true;
    bool to_print = (game->user_color==1);
    GameCommand command;
    while(1)
    {
        if(game->currentPlayer==playerPC(game))
        {
            computerMove(game);//make a computer move
            to_print = true;
            if(checkWinner(game) != '\0'){
                if(game->game_status == CHECKMATE){
                    switchPlayer(game);
                    printf("CHECKMATE! %s has won the game!\n",color(game->currentPlayer));
                    gameQuit(game, false);
                }
                else{
                    printf("Match TIED!\n");
					gameQuit(game,false);
                }
            }
            if(game->game_status==CHECK){
                printf("%s is in CHECK!\n",color(game->currentPlayer));
            }
				
        }
        else{
            command = getNewGameCommand(game, to_print&&check);
            // printf("->%d%d%d%d\n",command.move[0],command.move[1],command.move[2],command.move[3]);
            check = gameState(game, command);
            if(command.cmd==CH_GET_MOVES){
                check = false;
            }
            if(command.cmd==CH_SAVE){
                to_print = false;
                check = false;
            }
            if(check!=false){
                statusCheck(game);//check for winner or change in game status
            }
        }
    }
}
void computerMove(CH_Game* game)
{
    ASSERT(game!=NULL);

    int res;
    int* move = (int*) malloc(CELL_SIZE*CELL_SIZE*sizeof(int));
    move = miniMax(game,game->difficulty);//get the best move from PC
    

    elem* element;

    if(!move){
        failGame(game,true);
    }
    for(int i=0; i<CELL_SIZE*CELL_SIZE; ++i){
        ++move[i];
    }
    
    res = setMove(game,move);
    
    free(move);
    if(res == ALLOCATION_ERROR){
        failGame(game,true);
    }
    
    element = ArrayListGetLast(game->undo_hist);//update history
    
    if(!element){
        printf("Allocatona error\n");
        return;
    }
    // printf("%d%d->%d%d\n",element->startRow+1, element->startCol+1,element->destinationRow+1, element->destinationCol+1);

 	printf("Computer: move %s at <%d,%c> to <%d,%c>\n",figure(element->fig),element->startRow+1,toChar(element->startCol),element->destinationRow+1,toChar(element->destinationCol)); //print the move
    // printf("PC\n");
}
char* figure(char ch)
{
	switch(ch)
    { //Checks which piece it is
	    case PAWN_W:
	    case PAWN_B: // pawn
	    	return "pawn";
	    case BISHOP_W:
	    case BISHOP_B: // bishop
	    	return "bishop";
	    case ROOK_W:
	    case ROOK_B: // rook
	    	return "rook";
	    case KNIGHT_W:
	    case KNIGHT_B: // knight
	    	return "knight";
	    case QUEEN_W:
	    case QUEEN_B: // queen
	    	return "queen";
	    case KING_W:
	    case KING_B: // king
	    	return "king";
	}
	return NULL;
}
