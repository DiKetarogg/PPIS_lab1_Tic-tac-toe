
//\brief Location in a 2D world.
struct Vector2 {
	long long x;
	long long y;
};

/*\brief Tic-Tac-Toe game.
*
*\see Strings
*\see Vector2
*
*/
template <unsigned long long SIZE>
class TicTacToe {
	public:
	///Enumerator for storing current turn.
	enum Turn {
	X,
	Y
	};
	
	/// Places X or Y in the current position if possible and switches turns if needed.
	char play();

	/// Sets the current position.
	///\param ch character argument.
	void set(char ch);

	/// Sets the current position.
	///\param ch character argument.
	///\param x long long argument.
	///\param y long long argument.
	void set (long long x, long long y, char ch);

	/// Checks if current location is valid for setting an X or a Y.
	bool checkValid();

	/// Checks the current location for a win sequence.
	bool checkWin();

	/// Replaces every character of grid with spaces.
	void clear();

	
	private:
	/// Stores current turn.
	Turn turn;
	/// Stores Xs and Ys locations.
	char* grid[];
	/// Stores the current pointer. Pointer is where an X or a Y is played. 
	Vector2 pointer;
};
