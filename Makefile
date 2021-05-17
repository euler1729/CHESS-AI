CC = gcc

OBJS =  Main.o ConsoleMode.o Search.o XMLREADWRITE.o XMLGameParser.o XMLSettingsParser.o ChessGame.o  ArrayList.o \
GuiMode.o GUIManager.o MainMenu.o LoadWindow.o SettingWindow.o Panel.o Cell.o Button.o GameWindow.o Sound.o  

EXEC = CHESSAI
COMP_FLAG = -std=c99 -Wall -Wextra -Werror -pedantic-errors

SDL_COMP_FLAG = -I/usr/local/lib/sdl_2.0.5/include/SDL2 -D_REENTRANT
SDL_LIB = -L/usr/local/lib/sdl_2.0.5/lib -Wl,-rpath,/usr/local/lib/sdl_2.0.5/lib -Wl,--enable-new-dtags -lSDL2 -lSDL2main -lm -lSDL_mixer


VPATH = GUI ENGINE

all: $(OBJS)
	$(CC) $(OBJS) $(SDL_LIB) -o CHESSAI
$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(SDL_LIB) -o $@

Main.o: Main.c  GuiMode.h 
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c ENGINE/$*.c
ConsoleMode.o: ConsoleMode.c Search.h XMLREADWRITE.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c ENGINE/$*.c
XMLREADWRITE.o: XMLREADWRITE.c ChessGame.h XMLSettingsParser.h XMLGameParser.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c ENGINE/$*.c
XMLGameParser.o: XMLGameParser.c
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c ENGINE/$*.c
XMLSettingsParser.o: XMLSettingsParser.c
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c ENGINE/$*.c
Search.o: Search.c ChessGame.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c ENGINE/$*.c
ChessGame.o: ChessGame.c ArrayList.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c ENGINE/$*.c
ArrayList.o: ENGINE/ArrayList.c
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c ENGINE/$*.c

GuiMode.o: GuiMode.c GUIManager.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c GUI/$*.c
GUIManager.o: GUIManager.c GameWindow.h SettingWindow.h LoadWindow.h MainMenu.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c GUI/$*.c
MainMenu.o: MainMenu.c Button.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c GUI/$*.c
SettingWindow.o: SettingWindow.c ENGINE/ConsoleMode.h Button.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c GUI/$*.c
LoadWindow.o: LoadWindow.c ENGINE/ConsoleMode.h Button.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c GUI/$*.c
GameWindow.o: GameWindow.c Panel.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c GUI/$*.c
Panel.o: Panel.c Cell.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c GUI/$*.c
Cell.o: Cell.c Button.h ENGINE/ConsoleMode.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c GUI/$*.c
Button.o: GUI/Button.c
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c GUI/$*.c
Sound.o: GUI/Sound.c
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c GUI/$*.c

clean:
	rm -f $(OBJS) $(EXEC)
