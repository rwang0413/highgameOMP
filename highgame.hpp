#include <vector>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <omp.h>

using namespace std;

void compute(vector<vector<int>> &curLife, vector<vector<int>> &prevLife);
void copyPaddedMatrix(vector<vector<int>> &curLife, vector<vector<int>> &prevLife); 
void generateMatrix(vector<vector<int>> &M);
void clearScreen(int x, int y);
void writeScreen(vector<vector<int>> &M);
void moveDown(int y);
void moveUp(int y); 
void replicator(vector<vector<int>> &M);

void replicator(vector<vector<int>> &M) {
  if (M.size() >= 10  && M[0].size() >= 10) {
    int y = M.size()/2;
    int x = M[0].size()/2;

    M[y][x] = 1;
    M[y+1][x-1] = 1;
    M[y-1][x+1] = 1;
    M[y-1][x+2] = 1;
    M[y-1][x+3] = 1;
    M[y][x+3] = 1;
    M[y+1][x+3] = 1;
    M[y+2][x+2] = 1;
    M[y+3][x+1] = 1;
    M[y+3][x] = 1;
    M[y+3][x-1] = 1;
    M[y+2][x-1] = 1;
    M[y+1][x-1] = 1;
  }
}

void compute(vector<vector<int>> &curLife , vector<vector<int>> &prevLife) {
  int neighbors;
  
  #pragma omp parallel for collapse(2) private(neighbors)
  for (int j = 1; j < prevLife.size()-1; j++) {
    for (int i = 1; i < prevLife[0].size()-1; i++) {
      neighbors = prevLife[j-1][i-1] + prevLife[j-1][i] + prevLife[j-1][i+1] 
                + prevLife[j][i-1] +  prevLife[j][i+1]
                + prevLife[j+1][i-1] + prevLife[j+1][i] + prevLife[j+1][i+1];

      if (prevLife[j][i] == 1 && neighbors == 2 || neighbors == 3) {
        curLife[j-1][i-1] = 1;
      } else if (prevLife[j][i] == 0 && neighbors == 6 || neighbors == 3) {
        curLife[j-1][i-1] = 1;
      } else {
        curLife[j-1][i-1] = 0;
      }
    }
  }

}

void copyPaddedMatrix(vector<vector<int>> &curLife, vector<vector<int>> &prevLife) {
  int height = curLife.size();
  int width = curLife[0].size();

  #pragma omp parallel for collapse(2)
  for (int j = 1; j < height; j++) {
    for (int i = 1; i < width; i++) {
      prevLife[j][i] = curLife[j-1][i-1];
    }
  }

  // Copying edges for periodic boundary conditions
  for (int j = 1; j < height+1; j++) {
    prevLife[j][0] = curLife[j-1][width-1];
    prevLife[j][width+1] =  curLife[j-1][0];
  }

  for (int i = 1; i < width+1; i++) {
    prevLife[0][i] = curLife[height-1][i-1];
    prevLife[height+1][i] = curLife[0][i-1];
  }
}

// generates a random matrix 
void generateMatrix(vector<vector<int>> &M) {
  srand(time(NULL));
  for (int j = 0; j < M.size(); j++) {
    for (int i = 0; i < M[0].size(); i++) {
      if (rand()%100 > 80) {
        M[j][i] = 1;
      } else {
        M[j][i] = 0;
      }
    }
  }
}

void clearScreen(int x, int y) {
  for (int j = 0; j < y; j++) {
    for (int i = 0; i < x; i++) {
      cout << "\b";
    }
    moveUp(1);
  }
}

// Writes the contents of vector life to stdout
void writeScreen(vector<vector<int>> &M) {
  int count = 0;
  int height = M.size(), width = M[0].size();

  for (int j = 0; j < height; j++) {
    for (int i = 0; i < width; i++) {
      if (M[j][i] == 1) {
        cout << "■";
        count++;
      } 
      else {
        cout << " ";
      }
    }
    cout << "\n";
  }
  cout << flush;
}

// Moves cursor down
void moveDown(int y) {
  for (int i = 0; i < y; i++) {
    cout << "\n";
  }
}

// Moves cursor up 
void moveUp(int y) {
  for (int i = 0; i < y; i++) {
    cout << "\x1b[A";
  }
}

