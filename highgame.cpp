#include <csignal>
#include <cmath>
#include "highgame.hpp"

#define DEFAULT_ITER 1000
// clearing screen 
void handler(int sigNum) {
  if (system("clear") == -1 || system("tput cvvis") == -1) {
    exit(-1);
  }
  exit(0);
}

// debugging purposes
void printM(vector<vector<int>> &M) {
  for (int j= 0; j< M.size(); j++) {
    for (int i = 0; i < M[j].size(); i++) {
      std::cout << M[j][i];
    }
    std::cout << "\n";
  }
}
int main(int argc, char *argv[]) {
  struct winsize w;

  // getting console window sizes
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  const int MAX_HEIGHT = w.ws_row;
  const int MAX_WIDTH = w.ws_col;

  if (argc != 3 && argc != 4) {
    std::cout << "./highgame <iterations> <delay> <optional-preset>\n";
    return 1;
  }

  signal(SIGINT, handler);
  int height = MAX_HEIGHT-1, 
      width = MAX_WIDTH-1,
      iter = std::stoi(argv[1]); 

  useconds_t delay = std::stof(argv[2])*pow(10,6);

  vector<vector<int>> life(height, vector<int> (width, 0));
  vector<vector<int>> prev(height+2, vector<int> (width+2, 0));

  if (argc == 4) {
    int preset = std::stoi(argv[3]);

    if (preset == 1) {
      if (height >= 10  && width >= 10) { 
        replicator(life);
      } else {
        std::cout << "Board too small for replicator\n";
        exit(1);
      }
    }
    else if (preset == 2) {
      if (height >= 20  && width >= 20) { 
        bomber(life);
      } else {
        std::cout << "Board too small for bomber\n";
        exit(1);
      }
    } else if (preset == 3) {
      if (height >= 35 && width >= 40) {
        oscillator96(life);
      } else {
        std::cout << "Board too small for oscillator96\n";
        exit(1);
      }
    }

  } else {
    generateMatrix(life);
  }

  int curs = system("tput civis");

  for (int i = 0; i < iter; i++) {
    copyPaddedMatrix(life,prev);
    compute(life,prev);
    writeScreen(life);
    clearScreen(width, height);
    usleep(delay);
  }
  moveDown(height);

  if (!curs) {
    curs = system("tput cvvis");
  }
  return curs;
}
