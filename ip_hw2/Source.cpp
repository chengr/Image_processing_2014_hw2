#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <cv.h>
#include <iostream>
#include <stdio.h>

using namespace cv;
Mat src;
/** @function main */
int main(int argc, char** argv)
{
	
	/// Read the image 
	src = imread("len_full.jpg", 1 );
	/// Show your results
	namedWindow( "Orign", CV_WINDOW_AUTOSIZE );
	imshow( "Orign", src );
	//create array
	int row=src.rows;
	int col=src.cols;
	int **second;
	second = new int *[row];
	for(int i=0;i<row;i++)
		second[i]=new int [col];
	//
	for(int i=0;i<row;i++)
		for(int j=0;j<col;j++){
			Vec3b color;
			color[0]=mask33(i,j,0);
			color[1]=mask33(i,j,1);
			color[2]=mask33(i,j,2);
			src.at<Vec3b>(Point(i,j)) = color;
		}

	
 
	//delete array
	for (int i=0; i<row; i++)
		delete [] second[i];
	delete [] second;
	//imwrite( "S2_fc.jpg", src );
	waitKey(0);
	return 0;
}
//
int mask33(int x,int y,int i){
	int arr33[3][3]={{-1,-1,-1},
					 {-1, 8,-1},
					 {-1,-1,-1}};
	int lu=check(x-1,y-1,i);
	int u=check(x,y-1,i);
	int ru=check(x+1,y-1,i);
	int l=check(x-1,y,i);
	int m=check(x,y,i);
	int r=check(x+1,y,i);
	int ld=check(x-1,y+1,i);
	int d=check(x,y+1,i);
	int rd=check(x+1,y+1,i);

	int color_v=(arr33[0][0]*lu+arr33[0][1]*u+arr33[0][2]*ru+
				 arr33[1][0]*l+arr33[1][1]*m+arr33[1][2]*r+
				 arr33[2][0]*ld+arr33[2][1]*d+arr33[2][2]*rd
	)/9;

	return color_v;
}
//檢查是否出界
int check(int x,int y,int i){
	if(x>=0&&x<src.cols&&y>=0&&y<src.rows){
		Vec3b c=src.at<Vec3b>(Point(x,y));
		return 	c[i];
	}
	else
		return 255;
}
