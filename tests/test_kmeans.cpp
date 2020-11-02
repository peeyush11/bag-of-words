#include <gtest/gtest.h>
//#include <iostream>
#include <memory>
#include <opencv2/opencv.hpp>
#include <eigen3/Eigen/Dense>
#include "clustering/kmeans.hpp"
#include "clustering/cluster.hpp"

namespace igg {

TEST(KMeansTest, initClustering) {
  
  int K = 2;
  int pointId = 1;
  int iters = 100;
  std::vector<type::Point> point_set;

  Eigen::Matrix<double, 10, 2> data;

  data << 9, 9, 1, 1,
          -1, -1, 3, 3,
          10, 10, -2, -2,
          7, 8, 0.2, 0,
          -1, 0, 6, 10;

 for (int r = 0; r < data.rows(); r++)
 {
    type::Point point(pointId);
    for (int c = 0; c < data.cols(); c++)
    {
      point.SetData(data(r,c));
    }
    point_set.emplace_back(point);
    pointId++;
  }


 igg::kmeans kmeans(K, iters);
 
 kmeans.initCluster(point_set);
 kmeans.printClusterCentroid();
  
}

}