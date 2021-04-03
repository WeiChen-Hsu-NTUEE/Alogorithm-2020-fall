// **************************************************************************
//  File       [maximum_planar_subset.cpp]
//  Author     [Wei-Chen Hsu]
//  Synopsis   [The implementation of the maximum planar subset Class]
// **************************************************************************

#include "maximum_planar_subset.h"

//////////////////// the Maxumum planar subset function ////////////////////

void mps(int numOfPoints, vector<int> &chord, vector<int> &mps_startPoint, vector<int> &mps_endPoint) {

    //////////////// initial an 2D vector M ////////////////
    //////// M[i][j] records the maximum number of chords in the subset (i, j) ////////
    //////// the final result we want is M[0][numOfPoints - 1] ////////
    vector<vector<int>> M(numOfPoints, vector<int> (numOfPoints, 200000));

    //////////// initial an 2D vector C ////////////
    //////// C[i][j] records whether the chord (i, j) is picked in the maximum planar subset ////////
    vector<vector<bool>> C(numOfPoints, vector<bool> (numOfPoints, 0));

    ////////top-down mps ////////
    top_down_mps (chord, M, C, 0, numOfPoints - 1);

    ///////////// trace back from C[0][numOfPoints - 1] to C[i][i + 1] to find the chords in the mps /////////////
    //////// using top-down method, so need to write a function ////////
    find_chords_in_mps (chord, mps_startPoint, mps_endPoint, C, 0, numOfPoints - 1);
}

int top_down_mps(vector<int>& chord, vector<vector<int>>& M, vector<vector<bool>>& C, int i, int j) {
    if (i >= j) return 0;
    if (M[i][j] != 200000) return M[i][j];
    int k = chord[j];
    // case 1
    if (k < i || j < k) {
        M[i][j] = top_down_mps(chord, M, C, i, j - 1);
    }
    // case2
    else if (k == i) {
        M[i][j] = top_down_mps (chord, M, C, i + 1, j - 1) + 1;
        C[i][j] = 1;
    }
    // case3
    else if (i < k && k < j) {
        int case1 = top_down_mps (chord, M, C, i, j - 1);
        int case2 = top_down_mps (chord, M, C, i, k - 1) + 1 + top_down_mps(chord, M, C, k + 1, j - 1);
        if (case2 > case1) {
            M[i][j] = case2;
            C[i][j] = 1;
        }
        else {
            M[i][j] = case1;
        }
    }
    return M[i][j];
}

///////////// trace back from C[0][numOfPoints - 1] to C[i][i + 1] to find the chords in the mps ///////////// (to-do)
//////// using top-down method, so need to write a function ////////

void find_chords_in_mps (vector<int> &chord, vector<int> &mps_startPoint, vector<int> &mps_endPoint, vector<vector<bool>> &C, int i, int j) {
    if (i < j) {      // find whether chord(i, j) is in the subset [i, j]
        if (C[i][j] == 0) {
            find_chords_in_mps (chord, mps_startPoint, mps_endPoint, C, i, j - 1);
        }
        else {
            int k = chord[j];
            if (k == i) {
                mps_startPoint.push_back(k);
                mps_endPoint.push_back(j);
                find_chords_in_mps (chord, mps_startPoint, mps_endPoint, C, i + 1, j - 1);
            }
            else if (i < k && k < j) {
                find_chords_in_mps (chord, mps_startPoint, mps_endPoint, C, i, k - 1);
                mps_startPoint.push_back(k);
                mps_endPoint.push_back(j);
                find_chords_in_mps (chord, mps_startPoint, mps_endPoint, C, k + 1, j - 1);
            }
        }
    }
}