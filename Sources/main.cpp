#include "TicTacToe.hpp"
#include <iostream>
#include <cstdlib>
unsigned int skipSpaces(const char *str, unsigned int i) {
	while (isspace(str[i]) && str[i])
		++i;
	return i;
}
unsigned int skipNumeric(const char *str, unsigned int i) {
	while ((isdigit(str[i]) || str[i] == '-' || str[i] == '+') && str[i])
		++i;
	return i;
}
void extract2 (const char *str, unsigned int& i, long long& arg1, long long& arg2) {
		++i;
		if (i < 99)
			arg1 = atol(&str[i]);
		i = skipSpaces(str, i);
		i = skipNumeric(str, i);
		++i;
		if (i < 99)
			arg2 = atol(&str[i]);
}
void print(char ch) {
	std::cout << ch;
}
int main () {
	char str[100];
	dl::TicTacToe game(10, 10, 10);
	long long times;
	long long arg1;
	long long arg2;
	long long lockX;
	long long lockY;
	long long viewX = 10;
	long long viewY = 10;
	unsigned int winX = 0;
	unsigned int win0 = 0;
	bool lockScreen = false;
	while (true) {
		system("clear");
		if (lockScreen)
			game.printNearSep(viewX, viewY, lockX, lockY, print);
		else
			game.printNearCurrentSep(viewX, viewY, print);
		std::cout << "Current move: " << game.getTurn() << "\tScore X: " << winX << "\tScore 0: "<< win0 << '\n'
			<<"Inline win sequence: " << game.getWinCounter() 
			<< "\tCoordinates: " << game.getCurrentPosition().x << ':' << game.getCurrentPosition().y << '\n';
		std::cin.get(str,100);
		std::cin.clear();
		std::cin.ignore();
		for (unsigned int i = 0; str[i]; ++i) {
			i = skipSpaces(str, i);
			times = atol(&str[i]);
			i = skipNumeric(str, i);
			if (times == 0)
				times = 1;
			switch(str[i]) {
				case 'w':
					game.moveCurrent(0, times);
				break;
				case 'a':
					game.moveCurrent(-times, 0);
				break;
				case 's':
					game.moveCurrent(0, -times);
				break;
				case 'd':
					game.moveCurrent(times, 0);
				break;
				case 'W':
					game.moveCurrent(0, times * 10);
				break;
				case 'A':
					game.moveCurrent(-times * 10, 0);
				break;
				case 'S':
					game.moveCurrent(0, -times * 10);
				break;
				case 'D':
					game.moveCurrent(times * 10, 0);
				break;
				case 'g':
				case 'G':
					extract2(str, i, arg1, arg2);
					game.setCurrent(arg1, arg2);
					arg1 = 0;
					arg2 = 0;
				break;
				case 'l':
				case 'L':
					if (lockScreen)
						lockScreen = false;
					else {
						lockScreen = true;
						lockX = game.getCurrentPosition().x;
						lockY = game.getCurrentPosition().y;
					}
					
				break;
				case 'p':
				case 'P':
					if (game.play() == 1) {
						switch(game.getTurn()) {
							case 'X':
								++win0;
							break;
							case '0':
								++winX;
							break;
						}
						game.clear();
					}
					i = 98;
				break;
				case 'r':
				case 'R':
					extract2(str, i, arg1, arg2);
					game.resize(arg1, arg2);
					arg1 = 0;
					arg2 = 0;
				break;
				case 'i':
				case 'I':
					game.setWinCounter(times);
				break;
				case 'n':
				case 'N':
					game.clear();
				break;
				case 'v':
				case 'V':
					extract2(str, i, arg1, arg2);
					viewX = arg1;
					viewY = arg2;
					arg1 = 0;
					arg2 = 0;
				break;
				case 'q':
				case 'Q':
					return 0;
			}
			times = 1;
		}
		
	}
}
