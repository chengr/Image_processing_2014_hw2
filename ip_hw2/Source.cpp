#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <cv.h>
#include <iostream>
#include <stdio.h>

using namespace cv;

/** @function main */
int main(int argc, char** argv)
{
  Mat src, src_gray,src2;

  /// Read the image 
  src = imread("D2.jpg", 1 );

  /// Show your results
  namedWindow( "Orign", CV_WINDOW_AUTOSIZE );
  imshow( "Orign", src );
  
  imwrite( "S2_fc.jpg", src );
  waitKey(0);
  return 0;
}