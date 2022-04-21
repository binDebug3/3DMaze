#ifndef MAZE_H
#define MAZE_H

#include "MazeInterface.h"

#include <string>
#include <sstream>

using namespace std;

class Maze : public MazeInterface {
private:
	enum CellValue { OPEN, BLOCKED, PATH, EXIT, TEMPORARY,
					 LEFT, RIGHT, UP, DOWN, IN, OUT };
	int height;
	int width;
	int layer;
	CellValue ***mazeArray;
	bool solvingMaze = false;

	//create 3D dynamic array of CellValues
	void defineArray(void) {
		mazeArray = new CellValue**[height];
		for (int i = 0; i < height; i++) {
			mazeArray[i] = new CellValue*[width];
			for (int j = 0; j < width; j++) {
				mazeArray[i][j] = new CellValue[layer];
				for (int k = 0; k < layer; k++) {
					mazeArray[i][j][k] = OPEN;
				}
			}
		}
	}
	//convert CellValue to a printable string character
	string eToChar(CellValue cell) const {
		if (cell == OPEN || cell == TEMPORARY) return "_";
		else if (cell == BLOCKED) return "X";
		else if (cell == PATH) return "2";
		else if (cell == LEFT) return "L";
		else if (cell == RIGHT) return "R";
		else if (cell == UP) return "U";
		else if (cell == DOWN) return "D";
		else if (cell == IN) return "I";
		else if (cell == OUT) return "O";
		return "E";
	}

public:
	Maze(void) {}
	Maze(const int height, const int width, const int layer) :
		height(height),
		width(width),
		layer(layer) {
		defineArray();
	}
	//delete dynamically created array
	~Maze(void) {
		for (int h = 0; h < height; h++) {
			for (int w = 0; w < width; w++) {
				delete[] mazeArray[h][w];
			}
			delete[] mazeArray[h];
		}
		delete[] mazeArray;
	}
	//identifier for the toString method to know which maze to print
	void setSolving(void) {
		solvingMaze = true;
	}

	// Set maze value: parm height, parm width, parm layer, parm value
	virtual void setValue(int height, int width, int layer, int value) {
		if (value)
			mazeArray[height][width][layer] = BLOCKED;
		else
			mazeArray[height][width][layer] = OPEN;
	}

	// Solve maze return true if solveable, else false
	virtual bool find_maze_path() { return this->find_maze_path(0, 0, 0); }
	virtual bool find_maze_path(int h, int w, int d);

	// Output maze (same order as input maze) and return string of 2D layers
	virtual string toString() const;
	friend ostream& operator<< (ostream& os, Maze& cube) {
		os << cube.toString();
		return os;
	}
};

#endif

bool Maze::find_maze_path(int h, int w, int d) {
	//base cases---------------------------------
	if ((h < 0) || (h >= height) || (w < 0) || (w >= width) || (d < 0) || (d >= layer))
		return false;				// Out of bounds, not path

	if (mazeArray[h][w][d] != OPEN)
		return false;				// Path blocked, not path

	mazeArray[h][w][d] = PATH;		// Assume path

	if ((h == height - 1) && (w == width - 1) && (d == layer - 1)) {
		mazeArray[h][w][d] = EXIT;
		return true;               // Exit, success!
	}
	//-------------------------------------------

	// Look for path left, right, up, down, out, in and set mazeArray (BONUS)
	if (find_maze_path(h + 0, w - 1, d + 0)) {
		mazeArray[h][w][d] = LEFT;
		return true;
	}
	if (find_maze_path(h + 0, w + 1, d + 0)) {
		mazeArray[h][w][d] = RIGHT;
		return true;
	}
	if (find_maze_path(h - 1, w + 0, d + 0)) {
		mazeArray[h][w][d] = UP;
		return true;
	}
	if (find_maze_path(h + 1, w + 0, d + 0)) {
		mazeArray[h][w][d] = DOWN;
		return true;
	}
	if (find_maze_path(h + 0, w + 0, d - 1)) {
		mazeArray[h][w][d] = OUT;
		return true;
	}
	if (find_maze_path(h + 0, w + 0, d + 1)) {
		mazeArray[h][w][d] = IN;
		return true;
	}

	mazeArray[h][w][d] = TEMPORARY;	// Not a path, but don’t visit again
	return false;
}

string Maze::toString() const {
	stringstream oss;
	CellValue exitCell = mazeArray[height - 1][width - 1][layer - 1];
	//if it was unable to find the exit
	if (exitCell != EXIT && solvingMaze) {
		oss << "No Solution Exists!" << endl;
		return oss.str();
	}
	//if this was the call for the solved maze
	if (solvingMaze)
		oss << "Solution: " << endl;
	//print formatted maze
	for (int d = 0; d < layer; d++) {
		oss << "Layer " << (d + 1) << endl;
		for (int h = 0; h < height; h++) {
			for (int w = 0; w < width; w++) {
				oss << eToChar(mazeArray[h][w][d]) << " ";
			}
			oss << endl;
		}
	}
	return oss.str();
}