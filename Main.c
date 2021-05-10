
#include "GuiMode.h"
#include "ConsoleMode.h"

int main(int argc, char *args[])
{

	assert((argc == 1) || (argc == 2));
	if (!strcmp(args[1], "-g"))
	{
		startGuiMode();
		// printf("error\n");
	}
	else
	{
		startGame();
	}

	return 0;
}
