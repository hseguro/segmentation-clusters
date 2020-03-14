#include <stdio.h>
#include <iostream>
#include <chrono>
#include <omp.h>
#include "utils.h"

#define RST  "\x1B[0m"
#define KRED "\x1B[31m"

using namespace std;

void printer_points(vector<vector<uint>> *points) {
  if (points->size() == 0) {
    cout << "empty vector" << endl;
    return;
  }
  cout << "x: " << (*points).size() << endl;
  cout << "y: " << (*points)[0].size() << endl;
  for (int x = 0; x < (*points).size(); x++) {
    for (int y = 0; y < (*points)[0].size(); y++) {
      if ((*points)[x][y] == 1) {
        cout << KRED;
      }
      cout << (*points)[x][y] << " " << RST;

      if (y == (*points)[0].size() - 1) {
        cout << endl;
      }
    }
  }

  cout << RST << endl;
}

vector<vector<uint>> points8x8 = {
  {1, 1, 0, 0, 0, 0, 0, 0},
  {1, 1, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 1, 0, 0},
  {0, 0, 0, 0, 1, 1, 1, 0},
  {0, 1, 0, 0, 0, 1, 0, 0},
  {1, 0, 0, 1, 0, 0, 0, 1},
};

vector<vector<uint>> points16x16 = {
  {1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
  {1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
  {1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
  {1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
  {1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
  {1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
  {1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
  {1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
  {1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
  {1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
  {1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
  {1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
  {1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
  {1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
  {1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
  {1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
};

void calc8x8() {
  vector<vector<uint>> contours;

  printer_points(&points8x8);

  auto start = chrono::steady_clock::now();
  findContours(&points8x8, &contours);
  auto end = chrono::steady_clock::now();

  cout << "clusters: " << contours.size() << endl;

  cout << "Time: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " us" << endl;
  cout << "Time: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;

  printer_points(&contours);
}

void calc16x16() {
  vector<vector<uint>> contours;

  printer_points(&points16x16);

  auto start = chrono::steady_clock::now();
  findContours(&points16x16, &contours);
  auto end = chrono::steady_clock::now();

  cout << "clusters: " << contours.size() << endl;

  cout << "Time: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " us" << endl;
  cout << "Time: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;

  printer_points(&contours);
}

void calcNxN(int x, int y) {
  vector<vector<uint>> contours;

  vector<vector<uint>> points;

  generatePoints(&points, x, y);

  auto start = chrono::steady_clock::now();
  findContours(&points, &contours);
  auto end = chrono::steady_clock::now();
  cout << "size: " << x << "x" << y << endl;
  cout << "clusters: " << contours.size() << endl;
  cout << "Time: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " us" << endl;
  cout << "Time: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;

  points.clear();
  points.shrink_to_fit();

  contours.clear();
  contours.shrink_to_fit();
}

int main() {
  calc8x8();
  calc16x16();
  calcNxN(1024, 1024);
  return 0;
}