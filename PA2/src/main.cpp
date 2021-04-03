// **************************************************************************
//  File       [main.cpp]
//  Author     [Wei-Chen Hsu]
//  Synopsis   [The main program of 2020 fall Algorithm PA2]
// **************************************************************************

#include "maximum_planar_subset.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

void help_message() {
  cout << "usage: ./bin/mps <input file name> <output file name>" << endl;
}

int main(int argc, char *argv[]) {

    if (argc != 3) {
      help_message();
      return 0;
    }

    //////////////////// read the file ////////////////////

    fstream fin(argv[1]);
    fstream fout;
    fout.open(argv[2], ios::out);
    int numOfPoints;      // the number of vertices (= 2N, numOfPoints <= 180000)
    fin >> numOfPoints;

    vector<int> chord;
	  chord.assign(numOfPoints, 0);      // chord, a vector of size = numOfPoints
  	int data1, data2;
	  for (int i = 0; i < numOfPoints / 2; i++) {
        fin >> data1 >> data2;
		    chord[data1] = data2;
		    chord[data2] = data1;
	  }

    //////////////////// the maximum planar subset part ////////////////////
  
	  //////////// initial two vectors to store the answer ////////////
	  vector<int> mps_startPoint;
	  vector<int> mps_endPoint;

  	//////////// call mps ////////////
	  mps(numOfPoints, chord, mps_startPoint, mps_endPoint);

	  ////////////////// write the result to the output file //////////////////

    fout << mps_startPoint.size() << endl;
    for (int i = 0; i <= mps_startPoint.size() - 2; i++) {
        fout << mps_startPoint[i] << " " << mps_endPoint[i] << endl;
    }
    // no need to move to a new line after fout the final line
    fout << mps_startPoint[mps_startPoint.size() - 1] << " " << mps_endPoint[mps_startPoint.size() - 1];

    fin.close();
    fout.close();
    return 0;
}


