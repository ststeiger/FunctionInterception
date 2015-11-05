
#ifndef WAITCHAR_H
#define WAITCHAR_H



// http://stackoverflow.com/questions/1798511/how-to-avoid-press-enter-with-any-getchar
int WaitChar()
{
#if defined(_WIN32)
	//int c = getch(); // Deprecated
	int c = _getch();

	fflush(stdin);
	return c;
#elif defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
	int c;
	static struct termios oldt, newt;
	/*tcgetattr gets the parameters of the current terminal
	STDIN_FILENO will tell tcgetattr that it should write the settings
	of stdin to oldt*/
	tcgetattr(STDIN_FILENO, &oldt);
	/*now the settings will be copied*/
	newt = oldt;

	/*ICANON normally takes care that one line at a time will be processed
	that means it will return if it sees a "\n" or an EOF or an EOL*/
	newt.c_lflag &= ~(ICANON);

	/*Those new settings will be set to STDIN
	TCSANOW tells tcsetattr to change attributes immediately. */
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);

	c = getchar();

	// restore the old settings
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

	fflush(stdin);
	return c;
#else
	// system("/bin/stty raw");
	int c;
	printf("Note: Waitchar for OS not supported, waits for ENTER-key instead.\n");
	c = getchar();

	fflush(stdin);
	return c;
#endif

	return 0;
}




#endif // WAITCHAR_H 
