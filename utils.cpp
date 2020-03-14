#include "utils.h"

// void generatePoints(int x, int y, Cloud *ptrPoints) {
//   Cloud _points_(x);

//   for (int i = 0; i < x; i++) {
//     _points_[i] = vector<bool>(y);
//     for (int j = 0; j < x; j++) {
//       _points_[i][j] = rand() % 2;
//     }
//   }

//   *ptrPoints = _points_;
// }

// void printer_points(Cloud points) {
//   if (points.size() == 0) {
//     cout << "empty vector" << endl;
//     return;
//   }
//   cout << "x: " << points.size() << endl;
//   cout << "y: " << points[0].size() << endl;
//   for (int x = 0; x < points.size(); x++) {
//     for (int y = 0; y < points[0].size(); y++) {
//       cout << points[x][y] << " ";

//       if (y == points.size() - 1) {
//         cout << endl;
//       }
//     }
//   }

//   cout << endl;
// }





// void findContours(Cloud points, Cloud *contours) {
//   int width = points.size() - 1;
//   int height = points[0].size() - 1;

//   vector<vector<CloudPoint>> cloud(width);
//   for (int i = 0; i < width; i++) {
//     cloud[i] = vector<CloudPoint>(height);
//   }

//   vector<Node> clusters;

//   for (int x = 0; x < width; x++) {
//     for (int y = 0; y < height; y++) {
//       if (points[x][y] == 1) {

//         // filtered.insert(filtered.begin(), {i, j});
//       }
//     }
//   }

//   // vector<Node> clusters;

//   // for (int i = 0; i < filtered.size(); i++) {
//   // }

//   // *contours = filtered;
// }