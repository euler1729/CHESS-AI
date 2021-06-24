#include "XMLSettingsParser.h"



bool parserIsInt(const char* str)
{
    if(str==NULL)
    {
        return false;
    }
    if(!isdigit(*str) || (isdigit(*str) && str[0]=='-') )// checks the first character -is it digit or minus
    {
        return false;
    }
    str++;
    for( ; *str != '\0'; ++str)//checks the rest of the chars if they are digits
    {
        if(!isdigit(*str)){
            return false;
        }
    }
    return true;
}
SET_COMMAND_NAME parserIsSetCommand(const char *str)
{
    SET_COMMAND_NAME cmd;
    if(!str){
        cmd = CH_INVALID_LINE;
        return cmd;
    }
    
    else if(!strcmp(str,"game_mode")){
        cmd = CH_GAME_MODE;
    }
    else if(!strcmp(str,"difficulty")){
        cmd = CH_DIFFICULTY;
    }
    else if(!strcmp(str,"user_color")){
        cmd = CH_USER_COLOR;
    }
    else if(!strcmp(str,"load")){
        cmd = CH_LOAD;
    }
    else if(!strcmp(str,"default")){
        cmd = CH_DEFAULT;
    }
    else if(!strcmp(str,"quit")){
        cmd = CH_QUIT;
    }
    else if(!strcmp(str,"start")){
        cmd = CH_START;
    }
    else if(!strcmp(str,"print_setting")){
        cmd = CH_PRINT_SETTING;
    }
    else{
        cmd = CH_INVALID_LINE;
    }
    return cmd;
}
SetCommand parserSetParseLine(char* str)
{
    int flag = 0;
    SetCommand curr_command; //represents the command 
    for(int i=0; i<SP_MAX_LINE_LENGTH; ++i)
    {
        curr_command.path[i] = '\0';
    }
    curr_command.validArg = false;
    curr_command.arg = -1;
    if(str == NULL){
        curr_command.cmd = CH_INVALID_LINE;
        return curr_command;
    }

    char* token;
    char line[SP_MAX_LINE_LENGTH+1];
    strcpy(line, str);//copy the given string
    line[strlen(str)] = '\0';
    token = strtok(line, " \t\r\n");//gets the first token seperated by the given delimiter 
    curr_command.cmd = parserIsSetCommand(token);//INitialize the command
    token = strtok(NULL, " \t\r\n");//gets the second token
    if( (curr_command.cmd == CH_GAME_MODE) || (curr_command.cmd == CH_DIFFICULTY) || (curr_command.cmd==CH_USER_COLOR) )//checks if it's a command with integer
    {
        flag = INT_COMMANDS_FLAG;
    }
    if(curr_command.cmd ==  CH_LOAD)
    {
        flag = PATH_COMMAND_FLAG;
    }
    if(!flag)//not load/game_mode/difficulty/user_color
    {
        if(token!=NULL)//have more tokens but invalid command
        {
            curr_command.cmd = CH_INVALID_LINE;
            return curr_command;
        }
        return curr_command;//valid command return 
    }

    else if(token ==NULL)
    {
        curr_command.cmd = CH_INVALID_LINE;
        return curr_command;
    }
    
    else{
        if(flag == INT_COMMANDS_FLAG)
        {
            if(parserIsInt(token))//checks if next token is integer
            {
                int arg = atoi(token); 
                token = strtok(NULL, " \t\r\n");
                if(token == NULL)// no more tokends afer the column number
                {
                    curr_command.validArg = true;
                    curr_command.arg = arg;
                }
                else{
                    curr_command.cmd = CH_INVALID_LINE;
                }
            }
        }
        else if(flag==PATH_COMMAND_FLAG)
        {
            strcpy(curr_command.path, token);
            token = strtok(NULL, " \t\r\n");
            if(token == NULL)
            {
                curr_command.validArg = true;
            }
            else{
                curr_command.cmd = CH_INVALID_LINE;
            }
        }
        else{
            curr_command.cmd = CH_INVALID_LINE;
        }
    }
    return curr_command;
}