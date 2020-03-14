#include <algorithm>
#include <iostream>
#include <vector>
#include <map>
#include <iterator>

using namespace std;

int filters[8][2] = {
  {-1, -1},
  {0, -1},
  {1, -1},
  {-1, 0},
  {1, 0},
  {-1, 1},
  {0, 1},
  {1, 1}
};

class BoxCoordinates {
  public:
    int x;
    int y;
    BoxCoordinates(int ex, int ey) {
      x = ex;
      y = ey;
    }
};

class Cluster {
  public:
    vector<BoxCoordinates*> *coords;
    Cluster() {
      coords = new vector<BoxCoordinates*>();
    }
};

struct CloudPoint {
  Cluster *cluster = nullptr;
};

void generatePoints(vector<vector<uint>> *dstPoints, int x, int y) {
  vector<vector<uint>> _points_(x);

  for (int i = 0; i < x; i++) {
    _points_[i] = vector<uint>(y);
    for (int j = 0; j < y; j++) {
      _points_[i][j] = rand() % 2;
    }
  }

  *dstPoints = _points_;
}

bool sortByX(BoxCoordinates *i, BoxCoordinates *j) {
  return (i->x < j->x);
}

bool sortByY(BoxCoordinates *i, BoxCoordinates *j) {
  return (i->y < j->y);
}

void findCluster(CloudPoint *family, vector<vector<Cluster*>> *cloud, int x, int y) {
  #pragma omp for
  for (int i = 0; i < 8; i++) {
    int next_x = x + filters[i][0];
    int next_y = y + filters[i][1];

    if (next_x > (*cloud).size() - 1 || next_x < 0 || next_y > ((*cloud)[0].size() - 1) || next_y < 0) {
      continue;
    }

    if ((*cloud)[next_x][next_y] == nullptr) {
      continue;
    }
    family->cluster = (*cloud)[next_x][next_y];
    i = 8;
    continue;
  }
}

void findContours(vector<vector<uint>> *points, vector<vector<uint>> *contours) {
  omp_lock_t writelock;
  int width = (*points).size();
  int height = (*points)[0].size();

  vector<vector<Cluster*>> cloud(width);

  #pragma omp for
  for (int x = 0; x < width; x++) {
    cloud[x] = vector<Cluster*>(height);
  }

  vector<Cluster*> clusters;
  
  #pragma omp for
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      if ((*points)[x][y] == 1) {
        CloudPoint family;
        findCluster(&family, &cloud, x, y);
        if (family.cluster == nullptr) {
          Cluster *cluster = new Cluster();
          cloud[x][y] = cluster;
          clusters.insert(clusters.end(), cluster);
        } else {
          cloud[x][y] = family.cluster;
        }
        cloud[x][y]->coords->push_back(new BoxCoordinates(x, y));
      }
    }
  }

  for (int i = 0; i < clusters.size(); i++) {
    Cluster cluster = (*clusters[i]);

    sort(cluster.coords->begin(), cluster.coords->end(), sortByX);

    int min_x = (*cluster.coords->front()).x;
    int max_x = (*cluster.coords->back()).x;

    sort(cluster.coords->begin(), cluster.coords->end(), sortByY);

    int min_y = (*cluster.coords->front()).y;
    int max_y = (*cluster.coords->back()).y;

    contours->push_back(vector<uint> {
      (uint)min_x,
      (uint)min_y,
      (uint)max_x,
      (uint)max_y,
    });
  }

  cloud.clear();
  cloud.shrink_to_fit();
  clusters.clear();
  clusters.shrink_to_fit();
};
