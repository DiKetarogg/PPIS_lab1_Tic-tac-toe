#pragma once
#include <stddef.h>
#include <sys/types.h>
namespace dl {
template <class T>
inline T abs(T arg) {
	return arg > 0 ? arg : -arg;
}

template <class T>
struct Matrix {
	Matrix(): container(0), m_y(0){}
	Matrix(size_t x, size_t y):container(new T[x * y]), m_x(x), m_y(y) {}
	T* operator [] (size_t val) {
		return &(container[val * m_y * sizeof(T)]);
	}
	~Matrix() {
		delete[] container;
	}
	void resize(size_t x, size_t y) {
		if (x == 0 || y == 0) {
			delete[] container;
			m_x = 0;
			m_y = 0;
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
	size_t getX() {
		return m_x;
	}
	size_t getY() {
		return m_y;
	}
	private:
	T *container;
	size_t m_x;
	size_t m_y;
};
template <class T>
struct MatrixRef{
	MatrixRef(): container(0), m_x(0) {}
	MatrixRef(T* location, size_t x):container(location), m_x(x) {}
	T* operator [] (size_t val) {
		return &(container[val * m_x * sizeof(T)]);
	}
	void set (T* location, size_t m_x) {
		container = location;
		m_x = m_x;
	}
	private:
	T *container;
	size_t m_x;
};
//\brief Location in a 2D world.
struct Vector2 {
	Vector2() = default;
	Vector2(ssize_t x, ssize_t y): x(x), y(y) {}
	ssize_t x;
	ssize_t y;
};

/*\brief Tic-Tac-Toe game.
*
*\see Vector2
*
*/
class TicTacToe {
	public:
	TicTacToe(size_t x, size_t y, size_t win, char e = 32, char current = '#')
		:m_emptyChar(e),
		m_currentChar(current),
		m_turn('x'),
		m_win(win),
		m_x(x), m_halfX(x / 2),
		m_y(y), m_halfY(y / 2),
		m_grid(x,y),
		m_center(&m_grid[x/2][y/2], x),
		m_cur(x/2,y/2) {set(current);}

	TicTacToe()
		:m_emptyChar(32),
		m_currentChar('#'),
		m_turn('x'),
		m_win(0),
		m_x(0), m_halfX(0),
		m_y(0),	m_halfY(0),
		m_grid(),
		m_center(),
		m_cur(0,0) {}

	//\brief Places X or Y in the current position.
	/// Places X or Y in the current position only if it is possible and switches turn if needed.
	/// Returns -1 if the move is illegal, 0 if move has been played and 1 if move has been played and lead to a victory.
	signed char play() {
		if (!checkValid())
			return -1;
		set(m_turn);
		switchTurn();
		return checkWin();
	}

	//\brief Sets the current position.
	///Forcibly changes the current character without any checks.
	///\param ch character argument.
	void set(char ch) {
		m_center[m_cur.x][m_cur.y] = ch;
	}

	//\brief Sets the current position.
	///Forcibly changes the current character without any checks.
	///\param ch character argument.
	///\param x ssize_t argument.
	///\param y ssize_t argument.
	void set(ssize_t x, ssize_t y, char ch) {
		m_center[x][y] = ch;
	}

	void resize(size_t x, size_t y) {
		m_grid.resize(x,y);
		m_center.set(&m_grid[x/2][y/2], x);
	}

	/// Checks if current location is valid for setting an X or a Y.
	bool checkValid() {
		if (m_center[m_cur.x][m_cur.y] == m_currentChar)
			return true;
		return false;
	}

	/// Checks the current location for a win sequence.
	bool checkWin() {
		char ch = getCurrentChar();
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
		pos.x = m_cur.x - 1;
		while (m_center[wrapX(pos.x)][wrapY(pos.y)] == ch) {
			++winCounter;
			if (winCounter >= m_win)
				return true;
			--pos.y;
			++pos.x;
		}
		return false;
	}

	/// Replaces every character of grid with spaces.
	void clear() {
		size_t size = m_x * m_y;
		for (size_t i = 0; i < size; ++i)
			m_grid[0][i] = m_emptyChar;
	}

	void switchTurn() {
		m_turn = (m_turn == 'y') * 'x' + (m_turn == 'x') * 'y';
	}

	char getCurrentChar() {
		char ch = m_center[m_cur.x][m_cur.y];
		switch (ch) {
			case 'x':
			return 'X';
			case 'y': 
			return 'Y';
			default:
			if (ch == m_currentChar)
				return m_emptyChar;
		}
		return ch;
	}
	void printNear(size_t x, size_t y, void (*print)(char), char lineBreak = '\n') {
		if (x == 0 || y == 0)
			return;
		ssize_t itX = m_cur.x - x / 2;
		ssize_t itY = m_cur.y - y / 2;
		for (size_t i = 0; i < x; ++i, ++itX) {
			for (size_t j = 0; j < y; ++j, ++itY)
				print(m_center[wrapX(itX)][wrapY(itY)]);
			print(lineBreak);
		}
	}
	void moveCurrent(ssize_t x, ssize_t y) {
		unmarkCurrent();	
		m_cur.x = wrapX(m_cur.x + x);
		m_cur.y = wrapY(m_cur.y + y);
		markCurrent();
	}
	void setCurrent(ssize_t x, ssize_t y) {
		unmarkCurrent();
		m_cur.x = wrapX(x);
		m_cur.y = wrapY(y);
		markCurrent();
	}
	Vector2 getCurrentPosition() {
		return m_cur;
	}
	ssize_t wrapX(ssize_t x) {
		x %= m_x;
		if (x > m_halfX)
			x -= m_x;
		return x;
	}
	ssize_t wrapY(ssize_t y) {
		y %= m_y;
		if (y > m_halfY)
			y -= m_y;
		return y;
	}
	protected:
	void unmarkCurrent() {
		char ch = m_center[m_cur.x][m_cur.y];
		switch (ch) {
			case 'x':
			set('X');
			break;
			case 'y': 
			set('Y');
			break;
			default:
			if (ch == m_currentChar)
				set(m_emptyChar);
		}
	}

	void markCurrent() {
		char ch = m_center[m_cur.x][m_cur.y];
		switch (ch) {
			case 'X':
			set('y');
			break;
			case 'Y': 
			set('y');
			break;
			default:
			if (ch == m_currentChar)
				set(m_currentChar);
		}
	}
	
	private:
	char m_emptyChar;
	char m_currentChar;
	/// Stores the current turn.
	char m_turn;
	size_t m_win;
	size_t m_x;
	ssize_t m_halfX;
	size_t m_y;
	ssize_t m_halfY;
	/// Stores Xs and Ys locations.
	Matrix<char> m_grid;
	//char* m_grid[];
	/// Pointer to the center of the grid.
	MatrixRef<char> m_center;
	//char* m_center[];
	/// Stores the current pointer. Pointer is where an X or a Y is played. 
	Vector2 m_cur;
};

}
