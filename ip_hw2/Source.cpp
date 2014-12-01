#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <cv.h>
#include <iostream>
#include <stdio.h>
using namespace std;
using namespace cv;
Mat src,img;

//
float **first0;//BGR
float **first1;
float **first2;

float **first20;//BGR
float **first21;
float **first22;

//
int mask33(int x,int y,int i);
int mask_blur(int x,int y,int i);
float mask_sobel(int x,int y,int i);
int check(int x,int y,int i);
int check2(int x,int y,int i);
/** @function main */
int main(int argc, char** argv)
{
	
	/// Read the image 
	src = imread("Koala.jpg", 1 );
	//img=src;
	/// Show your results
	namedWindow( "Orign", CV_WINDOW_AUTOSIZE );
	imshow( "Orign", src );
	//create array
	int row=src.rows;
	int col=src.cols;

	Mat img(row,col , CV_8UC3, Scalar(0,0,0));
	//存二階
	int **second0;//BGR
	int **second1;
	int **second2;
	second0 = new int *[col];
	second1 = new int *[col];
	second2 = new int *[col];
	for(int i=0;i<col;i++){
		second0[i]=new int [row];
		second1[i]=new int [row];
		second2[i]=new int [row];
	}
	//跑二階
	for(int i=0;i<col;i++)
		for(int j=0;j<row;j++){
			second0[i][j]=mask33(i,j,0);
			second1[i][j]=mask33(i,j,1);
			second2[i][j]=mask33(i,j,2);
		}
	//存一階

	first0 = new float *[col];
	first1 = new float *[col];
	first2 = new float *[col];
	for(int i=0;i<col;i++){
		first0[i]=new float [row];
		first1[i]=new float [row];
		first2[i]=new float [row];
	}
	//跑一階 sobel
	for(int i=0;i<col;i++)
		for(int j=0;j<row;j++){
			first0[i][j]=mask_sobel(i,j,0);
			first1[i][j]=mask_sobel(i,j,1);
			first2[i][j]=mask_sobel(i,j,2);
		}
	//
	first20 = new float *[col];
	first21 = new float *[col];
	first22 = new float *[col];
	for(int i=0;i<col;i++){
		first20[i]=new float [row];
		first21[i]=new float [row];
		first22[i]=new float [row];
	}
	//跑模糊
	for(int i=0;i<col;i++)
		for(int j=0;j<row;j++){
			first20[i][j]=mask_blur(i,j,0);
			first21[i][j]=mask_blur(i,j,1);
			first22[i][j]=mask_blur(i,j,2);
		}
	//RESULT
	for(int i=0;i<col;i++)
		for(int j=0;j<row;j++){
			Vec3b color;
			Vec3b cs=src.at<Vec3b>(Point(i,j));
			if(j==300){
			//cout<<"0:"<<second0[i][j]*abs(first20[i][j]/255)<<endl;
			//cout<<"1:"<<second1[i][j]*abs(first21[i][j]/255)<<endl;
			//cout<<"2:"<<second2[i][j]*abs(first22[i][j]/255)<<endl;
			}
			/*
			color[0]=cs[0]+(second0[i][j]*abs(first20[i][j]/255));
			color[1]=cs[1]+(second1[i][j]*abs(first21[i][j]/255));
			color[2]=cs[2]+(second2[i][j]*abs(first22[i][j]/255));
			*/
			
			/*
			color[0]=abs(first0[i][j]);
			color[1]=abs(first1[i][j]);
			color[2]=abs(first2[i][j]);
			*/

			
			color[0]=second0[i][j]*abs(first20[i][j]/255);
			color[1]=second1[i][j]*abs(first21[i][j]/255);
			color[2]=second2[i][j]*abs(first22[i][j]/255);
			

			/*
			color[0]=abs(first20[i][j]);
			color[1]=abs(first21[i][j]);
			color[2]=abs(first22[i][j]);
			*/
			img.at<Vec3b>(Point(i,j)) = color;
		}
	/// Show your results
	namedWindow( "Result", CV_WINDOW_AUTOSIZE );
	imshow( "Result", img );
	//delete array
	for (int i=0; i<col; i++){
		delete [] second0[i];
		delete [] second1[i];
		delete [] second2[i];
	}
	delete [] second0;
	delete [] second1;
	delete [] second2;
	for (int i=0; i<col; i++){
		delete [] first0[i];
		delete [] first1[i];
		delete [] first2[i];
		delete [] first20[i];
		delete [] first21[i];
		delete [] first22[i];
	}
	delete [] first0;
	delete [] first1;
	delete [] first2;
	delete [] first20;
	delete [] first21;
	delete [] first22;
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
	)/16;

	return color_v;
}
//
float mask_sobel(int x,int y,int i){
	int arr1[3][3]={{-1,-2,-1},
					{ 0, 0, 0},
					{ 1, 2, 1}};
	int arr2[3][3]={{-1, 0, 1},
					{-2, 0, 2},
					{-1, 0, 1}};
	int lu=check(x-1,y-1,i);
	int u=check(x,y-1,i);
	int ru=check(x+1,y-1,i);
	int l=check(x-1,y,i);
	int m=check(x,y,i);
	int r=check(x+1,y,i);
	int ld=check(x-1,y+1,i);
	int d=check(x,y+1,i);
	int rd=check(x+1,y+1,i);

	float color_v=(arr1[0][0]*lu+arr1[0][1]*u+arr1[0][2]*ru+
				 arr1[1][0]*l+arr1[1][1]*m+arr1[1][2]*r+
				 arr1[2][0]*ld+arr1[2][1]*d+arr1[2][2]*rd
	)/8;
	float color_v2=(arr2[0][0]*lu+arr2[0][1]*u+arr2[0][2]*ru+
				 arr2[1][0]*l+arr2[1][1]*m+arr2[1][2]*r+
				 arr2[2][0]*ld+arr2[2][1]*d+arr2[2][2]*rd
	)/8;

	return color_v+color_v2;
}
//
int mask_blur(int x,int y,int i){
	int arr33[3][3]={{1,2,1},
					 {2,4,2},
					 {1,2,1}};
	int lu=check2(x-1,y-1,i);
	int u=check2(x,y-1,i);
	int ru=check2(x+1,y-1,i);
	int l=check2(x-1,y,i);
	int m=check2(x,y,i);
	int r=check2(x+1,y,i);
	int ld=check2(x-1,y+1,i);
	int d=check2(x,y+1,i);
	int rd=check2(x+1,y+1,i);

	int color_v=(arr33[0][0]*lu+arr33[0][1]*u+arr33[0][2]*ru+
				 arr33[1][0]*l+arr33[1][1]*m+arr33[1][2]*r+
				 arr33[2][0]*ld+arr33[2][1]*d+arr33[2][2]*rd
	)/16;

	return color_v;
}
//檢查是否出界
int check(int x,int y,int i){
	if(x>=0&&x<src.cols&&y>=0&&y<src.rows){
		Vec3b c=src.at<Vec3b>(Point(x,y));
		return 	c[i];
	}
	else
		return 0;
}
//檢查陣列
int check2(int x,int y,int i){
	if(x>=0&&x<src.cols&&y>=0&&y<src.rows){
		if(i==0)
			return first0[x][y];
		else if(i==1)
			return 	first1[x][y];
		else
			return 	first2[x][y];
	}
	else
		return 0;
}
