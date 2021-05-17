#include "XMLREADWRITE.h"

char *boardRowToLine(CH_Game *src, int row)
{
    assert(src != NULL);
    char *line = (char *)malloc(sizeof(char) * (GRID + 1));
    if (!line)
    {
        return NULL;
    }
    int j = 0;
    for (j = 0; j < GRID; j++)
    { //print gameboard rows
        line[j] = src->gameBoard[row][j];
    }
    line[j + 1] = '\0';
    return line;
}

void lineToInfo(CH_Game *game, char *line)
{
    assert(game != NULL);
    assert(line != NULL);

    int row = 0;
    char *ch = NULL;
    if (strstr(line, "game_mode"))
    { //if the line represent the game mode
        if (strstr(line, "2"))
        {
            game->mode = 2;
        }
    }
    if (strstr(line, "current_turn"))
    { //if the line represents current_turn
        if (strstr(line, "0"))
        {
            game->currentPlayer = CH_PLAYER_2;
        }
        else
        {
            game->currentPlayer = CH_PLAYER_1;
        }
    }
    if (strstr(line, "difficulty"))
    { //if the line represents user color
        if (strstr(line, "1"))
        {
            game->difficulty = 1;
        }
        else if (strstr(line, "2"))
        {
            game->difficulty = 2;
        }
        else if (strstr(line, "3"))
        {
            game->difficulty = 3;
        }
        else
        {
            game->difficulty = 4;
        }
    }
    if (strstr(line, "user_color"))
    { //if the line represents user color
        if (strstr(line, "0"))
        {
            game->user_color = 0;
        }
        else
        {
            game->user_color = 1;
        }
    }
    if (strstr(line, "row"))
    { //if the line represents row
        ch = strstr(line, "_");
        ++ch;
        row = (ch[0] - '0') - 1;
        ch = strstr(line, ">");
        ++ch;
        for (int i = 0; i < GRID; ++i)
        {
            game->gameBoard[row][i] = ch[i];
        }
    }
}

CH_Game *fileToGame(FILE *f)
{
    assert(f != NULL);
    char line[SP_MAX_LINE_LENGTH];
    CH_Game *game = gameCreate(HISTORY_SIZE, DEFAULT_DIFFICULTY, DEFAULT_COLOR, DEFAULT_MODE);
    char fig, player;
    if (!game)
    {
        return NULL;
    }
    for (int i = 0; i < NUM_OF_PIECES; ++i)
    {
        game->white[i] = 0;
        game->black[i] = 0;
    }

    while (fgets(line, SP_MAX_LINE_LENGTH, f) != NULL)
    {
        lineToInfo(game, line);
    }
    //Initializing other fields
    for (int i = 0; i < GRID; ++i)
    {
        for (int j = 0; j < GRID; ++j)
        {
            fig = game->gameBoard[i][j];
            if (fig >= 'A' && fig <= 'Z')
            {
                player = CH_PLAYER_1;
            }
            else
            {
                player = CH_PLAYER_2;
            }
            piecesUpdate(game, fig, true, player); //Updates the number of pieces of the specific player
            if (fig == KING_W)
            {
                game->white_king[0] = i;
                game->white_king[1] = j;
            }
        }
    }
    game->game_status = updateStatus(game);
    return game;
}

XML_MESSAGE gameToFile(CH_Game *src, FILE *xml)
{
    assert(xml != NULL);
    if (src == NULL)
    {
        return XML_FAILED;
    }
    fprintf(xml, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"); //opening line
    fprintf(xml, "<game>\n");                                     //print to file all relevant game date
    if (src->currentPlayer == CH_PLAYER_1)
    {
        fprintf(xml, "\t<current_turn>1</current_turn>\n");
    }
    else
    {
        fprintf(xml, "\t<current_turn>0</current_turn>\n");
    }
    fprintf(xml, "\t<game_mode>%d</game_mode>\n", src->mode);
    if (src->mode != TWO_PLAYER_MODE)
    {
        fprintf(xml, "\t<diffifcult>%d</difficulty>\n", src->difficulty);
        fprintf(xml, "\t<user_color>%d</user_color>\n", src->user_color);
    }
    fprintf(xml, "\t<board>\n");
    for (int i = GRID; i > 0; --i)
    {
        char *line = (char *)malloc(sizeof(char) * (GRID + 1));
        line = boardRowToLine(src, i - 1);
        if (line == NULL)
        {
            fclose(xml);
            return XML_ALLOC;
        }
        fprintf(xml, "\t\t<row_%d>%.8s</row_%d>\n", i, line, i);
        free(line);
    }
    fprintf(xml, "\t</board>\n");
    fprintf(xml, "</game>\n");
    return XML_SUCCESS; //printing ended successfully
}