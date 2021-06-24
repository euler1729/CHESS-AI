#include "XMLGameParser.h"


bool parserIsTuple(const char *str, GameCommand *curr, int cell)
{
    if ((str == NULL) || ((str[0] != '<') || (str[strlen(str) - 1] != '>'))){
        return false;
    }
    if (strstr(str, ",") == NULL){
        return false;
    }

    int count = 0;
    int count2 = 0;
    int length = 0;

    length = strlen(str);

    for (int i = 0; i < length; ++i){
        if (str[i] != ','){
            ++count;
        }
        else{
            break;
        }
    }

    if (count != 2){
        curr->move[cell] = INVALID_POSITION_FLAG;}
    else{
        char c = str[1];
        curr->move[cell] = c - '0';}

    while (str[0] != ','){
        ++str;
    }

    for (int i = 0; i < length - count; ++i){
        if (str[i] != '>'){
            ++count2;}
        else{
            break;}
    }
    
    if (count2 != 2){
        curr->move[cell + 1] = INVALID_POSITION_FLAG;
    }
    else{
        char c = str[1];
        int num = c;
        num -= ASCII_OFFSET;
        curr->move[cell + 1] = num;
    }
    
    return true;
}
GameCommand parserGameParseLine(char *str)
{
    int flag = 0;             //The flag indicates that if the command supposed to have another segment and which one
    bool first_tup = false;   //
    bool second_tup = false;  // represent if the tuple that represents move is valid
    GameCommand curr_command; // represents the command

    for(int i=0; i<SP_MAX_LINE_LENGTH; ++i)
    {
        curr_command.path[i] = '\0';
    }
    curr_command.validArg = false;
    if(str ==NULL){
        curr_command.cmd = CH_GINVALID_LINE;
        return curr_command;
    }

    char* token;
    char line[SP_MAX_LINE_LENGTH+1];//represents the line
    strcpy(line, str);//cpy the given string
    line[strlen(str)] = '\0';
    token = strtok(line, " \t\r\n");// gets the first token seperated by the given delimiter
    curr_command.cmd = parserIsGameCommand(token);//Initializes the command
    token = strtok(NULL, " \t\r\n"); //gets the second token
    if(curr_command.cmd == CH_SAVE){
        flag = PATH_COMMANDS_FLAG_GAME;
    }
    if( (curr_command.cmd == CH_MOVE) || (curr_command.cmd==CH_GET_MOVES) ){
        flag = MOVES_COMMANDS_FLAG;
    }
    if(!flag){//not move or save
        if(token !=NULL){//have more tokens - invalid command
            curr_command.cmd = CH_GINVALID_LINE;
            return curr_command;
        }
        return curr_command;//valid command return 
    }



    else if( (token == NULL))//no more tokens
    {
        curr_command.cmd = CH_GINVALID_LINE;
    }



    else{

        if(flag == MOVES_COMMANDS_FLAG)//move token - supposed to have 2 tuples that represent the move
        {
            first_tup = parserIsTuple(token, &curr_command, 0); //get the source cell
            token = strtok(NULL, " \t\r\n"); // third token - supposed to be the string "to"
            if(curr_command.cmd==CH_GET_MOVES)
            {
                if( (!first_tup) || (token !=NULL) )
                {
                    curr_command.cmd = CH_GINVALID_LINE;
                }
                else{
                    curr_command.validArg = true;
                }
                return curr_command;
            }
            if( (!first_tup) || (token ==NULL) )
            {
                curr_command.cmd = CH_GINVALID_LINE;
                return curr_command;
            }
            if(strcmp(token,"to")==0)
            {
                token = strtok(NULL, " \t\r\n");//fourth token - suppossed to be the <x,y>
                second_tup = parserIsTuple(token, &curr_command, 2);//get the destination cell
                token = strtok(NULL, "\t\r\n"); // suppossed to be null - no more tokens
                if(token==NULL && first_tup && second_tup)
                {
                    curr_command.validArg = true;
                }
                else{
                    curr_command.cmd = CH_GINVALID_LINE;
                }
            }
            else{
                curr_command.cmd = CH_GINVALID_LINE;
            }

        }



        else if(flag == PATH_COMMANDS_FLAG_GAME)//load or save token - supposed to have a path
        {
            strcpy(curr_command.path, token);
            token = strtok(NULL, " \t\r\n");
            if(token ==NULL)//no more tokens after the column number
            {
                curr_command.validArg = true;
            }
            
        }


        
        else{
            curr_command.cmd = CH_GINVALID_LINE;
        }
    }
    // printf("->%d%d%d%d\n",curr_command.move[0],curr_command.move[1],curr_command.move[2],curr_command.move[3]);
    return curr_command;

}

GAME_COMMAND_NAME parserIsGameCommand(const char* str)
{
    GAME_COMMAND_NAME cmd;
    if(!str){
        cmd = CH_GINVALID_LINE;
        return cmd;
    }
    else if(strcmp(str, "move")==0){
        cmd = CH_MOVE;
    }
    else if(strcmp(str, "get_moves")==0){
        cmd = CH_GET_MOVES;
    }
    else if(strcmp(str, "save")==0){
        cmd = CH_SAVE;
    }
    else if(strcmp(str, "undo")==0){
        cmd = CH_UNDO;
    }
    else if(strcmp(str, "reset")==0){
        cmd = CH_RESET;
    }
    else if(strcmp(str, "quit")==0){
        cmd = CH_GQUIT;
    }
    else{
        cmd = CH_GINVALID_LINE;
    }

    return cmd;
}
