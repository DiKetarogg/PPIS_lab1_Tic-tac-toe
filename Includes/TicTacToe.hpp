#pragma once
#include <stddef.h>
#include <sys/types.h>
namespace dl {
///\brief 2D contiguous heap allocated array.
template <class T>
struct Matrix {
	Matrix(): container(0), m_y(0){}
	Matrix(size_t x, size_t y):container(new T[x * y]), m_x(x), m_y(y) {}
	///\brief Returns a pointer to a specified 'row'.
	T* operator [] (size_t val) {
		return &(container[val * m_y * sizeof(T)]);
	}
	~Matrix() {
		delete[] container;
	}
	///\brief Resizes the matrix.

	/// Copies old data into a new location. If new size is smaller, data is trimmed.
	///\param x number of 1d arrays.
	///\param y size of the arrays.
	void resize(size_t x, size_t y) {
		if (x == 0 || y == 0) {
			delete[] container;
			m_x = 0;
			m_y = 0;
			container = 0;
			return;
		}
		Matrix<T> temp(x,y);
		size_t minX = temp.m_x < x ? temp.m_x : x;
		size_t minY = temp.m_y < y ? temp.m_y : y;
		for (size_t i = 0; i < minX; ++i)
			for (size_t j = 0; j < minY; ++j)
				temp[i][j] = (*this)[i][j];
		delete[] container;
		m_x = x;
		m_y = y;
		container = temp.container;
		temp.container = 0;
	}
	///\brief Resizes the matrix and fills it with data provided.

	///\param x number of 1d arrays.
	///\param y size of the arrays.
	///\param fill data to fill the matrix with.
	void resize(size_t x, size_t y, const T& fill) {
		if (x == 0 || y == 0) {
			delete[] container;
			m_x = 0;
			m_y = 0;
			container = 0;
			return;
		}
		delete[] container;
		Matrix<T> temp(x,y);
		for (size_t i = 0; i < x; ++i)
			for (size_t j = 0; j < y; ++j)
				temp[i][j] = fill;
		m_x = x;
		m_y = y;
		container = temp.container;
		temp.container = 0;
	}
	///\brief Returns the number of 1d arrays.
	size_t getX() {
		return m_x;
	}
	///\brief Returns the size of the 1d arrays.
	size_t getY() {
		return m_y;
	}
	private:
	///\brief Pointer to the data.
	T *container;
	///\brief Number of rows.
	size_t m_x;
	///\brief Size of rows.
	size_t m_y;
};
///\brief Reference to a memory cluster.

///Takes a pointer and uses memory in the specified location as a 2D array.
///\see Matrix
template <class T>
struct MatrixRef{
	MatrixRef(): container(0), m_x(0) {}
	MatrixRef(T* location, size_t x):container(location), m_x(x) {}
	///\brief Returns a pointer to a specified 'row'.
	T* operator [] (size_t val) {
		return &(container[val * m_x * sizeof(T)]);
	}
	///\brief Sets the memory locations.
	void set (T* location, size_t x) {
		container = location;
		m_x = x;
	}
	private:
	///\brief Pointer to the data.
	T *container;
	///\brief Number of rows.
	size_t m_x;
};
///\brief Coordinates in a 2D world.
struct Vector2 {
	Vector2() = default;
	Vector2(ssize_t x, ssize_t y): x(x), y(y) {}
	ssize_t x;
	ssize_t y;
};

/**\brief Tic-Tac-Toe game.
*
* Tic-Tac-Toe is a simple and fun game for 2 players, X and 0.
*It is played on a X by Y grid. X and Y could be set using resize function.
*Each player's goal is to make set number of Xs or 0s in a row.
*The number could be set using setWinCounter function.
*The win row could be horizontal, vertical or diagonal.
*If the row goes outside grid, it does count.
*
*\see Vector2
*\see Matrix
*\see MatrixRef
*
*/
class TicTacToe {
	public:
	TicTacToe(size_t x, size_t y, size_t win, char e = 32, char current = '#')
		:m_emptyChar(e),
		m_currentChar(current),
		m_turn('X'),
		m_win(win),
		m_x(x), m_halfX(x / 2),
		m_y(y), m_halfY(y / 2),
		m_grid(x,y),
		m_center(&m_grid[(x - 1) / 2][(y - 1) / 2], x),
		m_cur(0,0) {clear();set(current);}

	TicTacToe()
		:m_emptyChar(32),
		m_currentChar('#'),
		m_turn('X'),
		m_win(0),
		m_x(0), m_halfX(0),
		m_y(0),	m_halfY(0),
		m_grid(),
		m_center(),
		m_cur(0,0) {}

	///\brief Places X or 0 in the current position.

	/// Places X or 0 in the current position only if it is possible and switches turn if needed.
	/// Returns -1 if the move is illegal, 0 if move has been played and 1 if move has been played and lead to a victory.
	signed char play() {
		if (!checkValid())
			return -1;
		set(m_turn);
		switchTurn();
		signed char res = checkWin();
		markCurrent();
		return res;
	}

	///\brief Sets the current position.
	///Forcibly changes the current character without any checks.
	///\param ch character argument.
	void set(char ch) {
		if (m_x == 0 || m_y == 0)
			return;
		m_center[m_cur.x][m_cur.y] = ch;
	}

	///\brief Sets the current position.
	///Forcibly changes the current character without any checks.
	///\param x ssize_t argument.
	///\param y ssize_t argument.
	///\param ch character argument.
	void set(ssize_t x, ssize_t y, char ch) {
		if (m_x == 0 || m_y == 0)
			return;
		m_center[x][y] = ch;
	}
	///\brief Resizes the grid.

	///Resizes the play grid copying all the data stored previously.
	///If the new size is lower, the data will be trimmed.
	///\param x horizontal size.
	///\param y vertical size
	void resize(size_t x, size_t y) {
		m_grid.resize(x, y, m_emptyChar);
		if (x == 0 || y == 0) {
			m_cur.x = 0;
			m_cur.y = 0;
			m_x = 0;
			m_y = 0;
			m_halfX = 0;
			m_halfY = 0;
			m_center.set(0, 0);
			return;
		}
		m_x = x;
		m_y = y;
		m_halfX = x / 2;
		m_halfY = y / 2;
		m_center.set(&m_grid[(x - 1) / 2][(y - 1) / 2], x);
		m_cur.x = wrapX(m_cur.x);
		m_cur.y = wrapY(m_cur.y);
		markCurrent();
	}

	///\brief Checks if current location is valid for setting an X or a 0.
	bool checkValid() {
		if (m_x == 0 || m_y == 0)
			return false;
		if (m_center[m_cur.x][m_cur.y] == m_currentChar)
			return true;
		return false;
	}

	///\brief Checks the current location for a win sequence.
	bool checkWin() {
		if (m_x == 0 || m_y == 0)
			return false;
		char ch = m_center[m_cur.x][m_cur.y];
		//vertical check:
		size_t winCounter = 0;
		Vector2 pos = m_cur;
			while (m_center[wrapX(pos.x)][wrapY(pos.y)] == ch) {
			++winCounter;
			if (winCounter >= m_win)
				return true;
			++pos.x;
		}
		pos.x = m_cur.x - 1;
		while (m_center[wrapX(pos.x)][wrapY(pos.y)] == ch) {
			++winCounter;
			if (winCounter >= m_win)
				return true;
			--pos.x;
		}
		//horizontal check:
		winCounter = 0;
		pos.x = m_cur.x;
		while (m_center[wrapX(pos.x)][wrapY(pos.y)] == ch) {
			++winCounter;
			if (winCounter >= m_win)
				return true;
			++pos.y;
		}
		pos.y = m_cur.y - 1;
		while (m_center[wrapX(pos.x)][wrapY(pos.y)] == ch) {
			++winCounter;
			if (winCounter >= m_win)
				return true;
			--pos.y;
		}
		//diagonal ++ -- check:
		winCounter = 0;
		pos.y = m_cur.y;
		while (m_center[wrapX(pos.x)][wrapY(pos.y)] == ch) {
			++winCounter;
			if (winCounter >= m_win)
				return true;
			++pos.y;
			++pos.x;
		}
		pos.y = m_cur.y - 1;
		pos.x = m_cur.x - 1;
		while (m_center[wrapX(pos.x)][wrapY(pos.y)] == ch) {
			++winCounter;
			if (winCounter >= m_win)
				return true;
			--pos.y;
			--pos.x;
		}
		//diagonal +- -+ check:
		winCounter = 0;
		pos.y = m_cur.y;
		pos.x = m_cur.x;
		while (m_center[wrapX(pos.x)][wrapY(pos.y)] == ch) {
			++winCounter;
			if (winCounter >= m_win)
				return true;
			++pos.y;
			--pos.x;
		}
		pos.y = m_cur.y - 1;
		pos.x = m_cur.x + 1;
		while (m_center[wrapX(pos.x)][wrapY(pos.y)] == ch) {
			++winCounter;
			if (winCounter >= m_win)
				return true;
			--pos.y;
			++pos.x;
		}
		return false;
	}

	///\brief Replaces every character of grid with empty char (space by default).
	void clear() {
		if (m_x == 0 || m_y == 0)
			return;
		size_t size = m_x * m_y;
		for (size_t i = 0; i < size; ++i)
			m_grid[0][i] = m_emptyChar;
		m_cur.x = wrapX(m_cur.x);
		m_cur.y = wrapY(m_cur.y);
		markCurrent();
	}
	///\brief Switches X/0 turn.
	void switchTurn() {
		m_turn = (m_turn == '0') * 'X' + (m_turn == 'X') * '0';
	}
	///\brief Retrieves data from the current location while converting from the 'selected' character.
	char getCurrentChar() {
		if (m_x == 0 || m_y == 0)
			return 0;
		char ch = m_center[m_cur.x][m_cur.y];
		switch (ch) {
			case 'x':
			return 'X';
			case 'o': 
			return '0';
			default:
			if (ch == m_currentChar)
				return m_emptyChar;
		}
		return ch;
	}
	///\brief Returns character corresponding to the current turn.
	char getTurn() {
		return m_turn;
	}
	///\brief Prints specified number of characters around the current location.
	///\param x horizontal diameter.
	///\param y vertical diameter.
	///\param print function for printing a character.
	///\param lineBreak character to break a line.
	void printNearCurrent(size_t x, size_t y, void (*print)(char), char lineBreak = '\n') {
		if (x == 0 || y == 0)
			return;
		if (m_x == 0 || m_y == 0)
			return;
		ssize_t startX = m_cur.x - (x - 1) / 2;
		ssize_t itX = startX;
		ssize_t itY = m_cur.y + y / 2;
		for (size_t i = 0; i < y; ++i, --itY) {
			for (size_t j = 1; j < x; ++j, ++itX)
				print(m_center[wrapX(itX)][wrapY(itY)]);
			print(lineBreak);
			itX = startX;
		}
	}
	///\brief Prints specified number of characters around the current location.

	/// Each character is separated by a specified character. This is needed to get a more appealing output.
	///Without separators a square would look like a rectangle.
	///\param x horizontal diameter.
	///\param y vertical diameter.
	///\param print function for printing a character.
	///\param separator character to separate characters on the same line.
	///\param lineBreak character to break a line.
	void printNearCurrentSep(size_t x, size_t y, void (*print)(char), char separator = 32, char lineBreak = '\n') {
		if (x == 0 || y == 0)
			return;
		if (m_x == 0 || m_y == 0)
			return;
		ssize_t startX = m_cur.x - (x - 1) / 2;
		ssize_t itX = startX;
		ssize_t itY = m_cur.y + y / 2;
		for (size_t i = 0; i < y; ++i, --itY) {
			print(m_center[wrapX(itX)][wrapY(itY)]);
			++itX;
			for (size_t j = 1; j < x; ++j, ++itX) {
				print(separator);
				print(m_center[wrapX(itX)][wrapY(itY)]);
			}
			print(lineBreak);
			itX = startX;
		}
	}
	///\brief Prints specified number of characters around the specified location.

	///\param x horizontal diameter.
	///\param y vertical diameter.
	///\param itX horizontal center of the area.
	///\param itY vertical center of the area.
	///\param print function for printing a character.
	///\param lineBreak character to break a line.
	void printNear(size_t x, size_t y, ssize_t itX, ssize_t itY,
			void (*print)(char), char lineBreak = '\n') {
		if (x == 0 || y == 0)
			return;
		if (m_x == 0 || m_y == 0)
			return;
		ssize_t startX = itX - (x - 1) / 2;
		itX = startX;
		itY += y / 2;
		for (size_t i = 0; i < y; ++i, --itY) {
			for (size_t j = 1; j < x; ++j, ++itX)
				print(m_center[wrapX(itX)][wrapY(itY)]);
			print(lineBreak);
			itX = startX;
		}
	}
	///\brief Prints specified number of characters around the specified location.

	///Each character is separated by a specified character. This is needed to get a more appealing output.
	///Without separators a square would look like a rectangle.
	///\param x horizontal diameter.
	///\param y vertical diameter.
	///\param itX horizontal center of the area.
	///\param itY vertical center of the area.
	///\param print function for printing a character.
	///\param separator character to separate characters on the same line.
	///\param lineBreak character to break a line.
	void printNearSep(size_t x, size_t y, ssize_t itX, ssize_t itY,
			void (*print)(char), char separator = 32, char lineBreak = '\n') {
		if (x == 0 || y == 0)
			return;
		if (m_x == 0 || m_y == 0)
			return;
		ssize_t startX = itX - (x - 1) / 2;
		itX = startX;
		itY += y / 2;
		for (size_t i = 0; i < y; ++i, --itY) {
			print(m_center[wrapX(itX)][wrapY(itY)]);
			++itX;
			for (size_t j = 1; j < x; ++j, ++itX) {
				print(separator);
				print(m_center[wrapX(itX)][wrapY(itY)]);
			}
			print(lineBreak);
			itX = startX;
		}
	}
	///\brief Moves the current location.
	///\param x how far to move horizontally.
	///\param y how far to move vertically.
	void moveCurrent(ssize_t x, ssize_t y) {
		if (m_x == 0 || m_y == 0)
			return;
		unmarkCurrent();	
		m_cur.x = wrapX(m_cur.x + x);
		m_cur.y = wrapY(m_cur.y + y);
		markCurrent();
	}
	///\brief Sets the current location.
	///\param x new horizontal coordinate.
	///\param y new vertical coordinate.
	void setCurrent(ssize_t x, ssize_t y) {
		if (m_x == 0 || m_y == 0)
			return;
		unmarkCurrent();
		m_cur.x = wrapX(x);
		m_cur.y = wrapY(y);
		markCurrent();
	}
	///\brief Sets the number of Xs or 0s in a row required for a win.
	///param win win counter.
	void setWinCounter(size_t win) {
		m_win = win;
	}	
	///\brief Returns the number of Xs or 0s in a row required for a win.
	size_t getWinCounter() {
		return m_win;
	}
	///\brief Returns coordinates of the current location.
	///\see Vector2
	Vector2 getCurrentPosition() {
		return m_cur;
	}
	///\brief Returns horizontal size of te grid.
	size_t getSizeX() {
		return m_x;
	}
	/// Returns vertical size of the grid. 
	size_t getSizeY() {
		return m_y;
	}
	///\brief Returns horizontal coordinate that is inbounds of the game grid.
	///\param x horizontal coordinate that could be out of bounds.
	ssize_t wrapX(ssize_t x) {
		if (m_x == 0 || m_y == 0)
			return 0;
		if (x < 0) {
			x += m_x;
			if (x < 0)
				--x;
		}
		x %= m_x;
		if (x > m_halfX)
			x -= m_x;
		return x;
	}
	///\brief Returns vertical coordinate that is inbounds of the game grid.
	///\param y vertical coordinate that could be out of bounds.
	ssize_t wrapY(ssize_t y) {
		if (m_x == 0 || m_y == 0)
			return 0;
		if (y < 0) {
			y += m_y;
			if (y < 0)
				--y;
		}
		y %= m_y;
		if (y > m_halfY)
			y -= m_y;
		return y;
	}
	protected:
	///\brief Changes character in the current location from 'selected' to 'unselected'.
	void unmarkCurrent() {
		if (m_x == 0 || m_y == 0)
			return;
		char ch = m_center[m_cur.x][m_cur.y];
		switch (ch) {
			case 'x':
			set('X');
			break;
			case 'o': 
			set('0');
			break;
			default:
			if (ch == m_currentChar)
				set(m_emptyChar);
		}
	}
	///\brief Changes character in the current location from 'unselected' to 'selected'.
	void markCurrent() {
		if (m_x == 0 || m_y == 0)
			return;
		char ch = m_center[m_cur.x][m_cur.y];
		switch (ch) {
			case 'X':
			set('x');
			break;
			case '0': 
			set('o');
			break;
			default:
			if (ch == m_emptyChar)
				set(m_currentChar);
		}
	}
	
	private:
	///\brief Stores character to represent an empty space.
	char m_emptyChar;
	///\brief Stores character to represent the current location.
	char m_currentChar;
	///\brief Stores the current turn.
	char m_turn;
	///\brief Stores a number of Xs or 0s in a row required for a win.
	size_t m_win;
	///\brief Stores horizontal size.
	size_t m_x;
	///\brief Stores half of the horizontal size.
	ssize_t m_halfX;
	///\brief Stores vertical size.
	size_t m_y;
	///\brief Stores half of the vertical size.
	ssize_t m_halfY;
	///\brief Stores Xs and 0s locations.
	///\see Matrix
	Matrix<char> m_grid;
	///\brief Pointer to the center of the grid.
	///\see MatrixRed
	MatrixRef<char> m_center;
	///\brief Stores the current pointer. Pointer is where an X or a 0 is played. 
	///\see Vector2
	Vector2 m_cur;
};

}
