// **************************************************************************
//  File       [maximum_planar_subset.h]
//  Author     [Wei-Chen Hsu]
//  Synopsis   [The header file for the maximum_planar_subset Class]
// **************************************************************************

#ifndef _MAXIMUM_PLANAR_SUBSET_H
#define _MAXIMUM_PLANAR_SUBSET_H

#include <vector>
#include<fstream>
#include <iostream>
#include <algorithm>    // for max() function

using namespace std;

void mps(int numOfPoints, vector<int> &chord, vector<int> &mps_startPoint, vector<int> &mps_endPoint);
void find_chords_in_mps (vector<int> &chord, vector<int> &mps_startPoint, vector<int> &mps_endPoint, vector<vector<bool>> &C, int i, int j);
int top_down_mps(vector<int>&, vector<vector<int>>&, vector<vector<bool>>&, int, int);
#endif