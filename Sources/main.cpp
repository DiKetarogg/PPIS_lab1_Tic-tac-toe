#include "TicTacToe.hpp"
#include <iostream>
#include <string>
#include <cctype>
unsigned int skipSpaces(char *str, unsigned int i) {
	while (isspace(str[i]) && str[i])
		++i;
	return i;
}
unsigned int skipNumeric(char *str, unsigned int i) {
	while (isdigit(str[i]) && str[i])
		++i;
	return i;
}
int main () {
	char str[100];
	TicTacToe(100,100) game;
	unsigned long long times;
	while (true) {
		std::cin.get(str,100);
		for (unsigned int i = 0; i < sizeof(str); ++i) {
			i = skipSpace(str,i);
			times = stoul(&str[i]);
			i = skipNumeric(str, i);
			if (times == 0)
				times = 1;
			switch(str[i]) {
				case 'w':
				break;
				case 'a':
				break;
				case 's':
				break;
				case 'd':
				break;
				case 'W':
				break;
				case 'A':
				break;
				case 'S':
				break;
				case 'D':
				break;
			}
			times = 1;
		}
	}
}
