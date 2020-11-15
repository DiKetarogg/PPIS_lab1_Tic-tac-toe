#include "TicTacToe.hpp"
#include <iostream>
#include <cstdlib>
unsigned int skipSpaces(const char *str, unsigned int i) {
	while (isspace(str[i]) && str[i])
		++i;
	return i;
}
unsigned int skipNumeric(const char *str, unsigned int i) {
	while (isdigit(str[i]) && str[i])
		++i;
	return i;
}
void print(char ch) {
	std::cout << ch;
}
int main () {
	char str[100];
	dl::TicTacToe game(10, 10, 10);
	long long times;
	while (true) {
		system("clear");
		game.printNear(10,10,print);
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
				case 'p':
				case 'P':
					game.play();
					i = 98;
			}
			times = 1;
		}
		
	}
}
