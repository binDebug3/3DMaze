#include "Maze.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#ifdef _MSC_VER
#define _CRTDBG_MAP_ALLOC  
#include <crtdbg.h>
#define VS_MEM_CHECK _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#else
#define VS_MEM_CHECK
#endif

using namespace std;

int main(int argc, char* argv[]) {
	VS_MEM_CHECK

	//open files
	if (argc < 3) {
		cerr << "Please provide name of input and output files";
		return 1;
	}
	ifstream in(argv[1]);
	if (!in.is_open()) {
		cout << "Uh oh there was an inputFile error..." << endl;
		return 2;
	}
	ofstream out(argv[2]);
	if (!out.is_open()) {
		cout << "Uh oh there was an outputFile error..." << endl;
		return 3;
	}

	//set and input size values
	int height = 0, width = 0, layer = 0, value = 0;
	in >> height;
	in >> width;
	in >> layer;

	//create a maze
	Maze maze(height, width, layer);
	for (int d = 0; d < layer; d++) {
		for (int h = 0; h < height; h++) {
			for (int w = 0; w < width; w++) {
				if (in >> value)
					maze.setValue(h, w, d, value);
			}
		}
	}
	//print unsolved maze
	out << "Solve Maze: " << endl << maze << endl;
	//then solve and print the solved maze
	maze.setSolving();
	maze.find_maze_path(0, 0, 0);
	out << maze;

	in.close();
	out.close();
	return 0;
}