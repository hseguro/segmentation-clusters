#include <algorithm>
#include <iostream>
#include <vector>
#include <map>
#include <iterator>
#include <unordered_map>
#include <omp.h>

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
    void clear() {
      for (int i = 0; i < coords->size(); i++) {
        delete (*coords)[i];
      }

      delete coords;
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

void findCluster(CloudPoint *family, unordered_map<unsigned int, Cluster*> *cloud, int x, int y, int w, int h) {
  // #pragma omp for
  for (int i = 0; i < 8; i++) {
    int next_x = x + filters[i][0];
    int next_y = y + filters[i][1];

    if (next_x < 0 || next_y < 0 || next_x > w || next_y > h) {
      continue;
    }

    Cluster* tmp = (*cloud)[next_x + (next_y * w)];

    if (tmp == nullptr) {
      continue;
    }
    family->cluster = tmp;
    i = 8;
    continue;
  }
}

void findContours(vector<vector<uint>> *points, vector<vector<uint>> *contours) {
  omp_lock_t writelock;
  int width = (*points).size();
  int height = (*points)[0].size();

  unordered_map<unsigned int, Cluster*> cloud;

  vector<Cluster*> clusters;
  
  #pragma omp for
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      if ((*points)[x][y] == 1) {
        CloudPoint family;
        findCluster(&family, &cloud, x, y, width, height);
        int index = x + (y * width);
        if (family.cluster == nullptr) {
          Cluster *cluster = new Cluster();
          cloud[index] = cluster;
          clusters.insert(clusters.end(), cluster);
        } else {
          cloud[index] = family.cluster;
        }
        
        cloud[index]->coords->push_back(new BoxCoordinates(x, y));
      }
    }
  }

  #pragma omp for
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

    clusters[i]->clear();

    delete clusters[i];

    // cout << "cluster deleted" << endl;
  }

  // clusters.clear();
  // clusters.shrink_to_fit();
};
