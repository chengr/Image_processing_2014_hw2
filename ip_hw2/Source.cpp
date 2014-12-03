#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <cv.h>
#include <iostream>
#include <stdio.h>
using namespace std;
using namespace cv;
Mat src,img, gray_image;

//
int **first0;//BGR
float **first1;
float **first2;

int **first20;//BGR
float **first21;
float **first22;

//
int mask33(int x,int y,int i);
int mask_blur(int x,int y,int i);
int mask_sobel(int x,int y,int i);
int check(int x,int y,int i);
int checks(int x,int y,int i);
int check2(int x,int y,int i);
/** @function main */
int main(int argc, char** argv)
{
	String ins="F16";
	/// Read the image 
	src = imread(ins+".jpg", 1 );
	//img=src;
	/// Show your results
	//namedWindow( "Orign", CV_WINDOW_AUTOSIZE );
	//imshow( "Orign", src );
	cvtColor( src, gray_image, CV_RGB2GRAY );
	//create array
	int row=src.rows;
	int col=src.cols;
	//存二階
	int **second0;//BGR
	second0 = new int *[col];
	for(int i=0;i<col;i++){
		second0[i]=new int [row];
	}
	//跑二階
	for(int i=0;i<col;i++)
		for(int j=0;j<row;j++){
			second0[i][j]=mask33(i,j,0);
		}
	//存一階
	first0 = new int *[col];
	for(int i=0;i<col;i++){
		first0[i]=new int [row];
	}
	//跑一階 sobel
	for(int i=0;i<col;i++)
		for(int j=0;j<row;j++){
			first0[i][j]=mask_sobel(i,j,0);
		}
	//
	first20 = new int *[col];
	for(int i=0;i<col;i++){
		first20[i]=new int [row];
	}
	//跑模糊
	for(int i=0;i<col;i++)
		for(int j=0;j<row;j++){
			first20[i][j]=mask_blur(i,j,0);
		}
	namedWindow( "gray", CV_WINDOW_AUTOSIZE );
	imshow( "gray", gray_image );
	imwrite( ins+"_gray.jpg", gray_image );
	cout<<"COL:"<<col<<endl;
	cout<<"ROW:"<<row<<endl;
	//RESULT
	for(int i=0;i<col;i++)
		for(int j=0;j<row;j++){
			uchar color;
			uchar cs=gray_image.at<uchar>(Point(i,j));
			float val=255;
			if(cs+(int)((float)second0[i][j]*((float)first20[i][j]/val))>255){
				cout<<"x:"<<i<<"y:"<<j<<endl;
			}
			
			/*
			color=cs+(second0[i][j]*(first20[i][j]/val));
			*/
			//color=(int)((float)second0[i][j]*((float)first20[i][j]/val));

			color=cs+(int)((float)second0[i][j]*((float)first20[i][j]/val));//result
			//color=(int)((float)second0[i][j]*((float)first20[i][j]/val));//細節
			//color=(int)((float)first20[i][j]);//模糊後
			//color=(int)((float)first0[i][j]);//模糊前
			//color=(int)(second0[i][j]);//銳化
			//color=(int)((float)first0[i][j]/val);//模糊前
			
			if(color>255)
				color=255;
			if(color<0)
				color=0;
			gray_image.at<uchar>(Point(i,j))= color;
		}
	/// Show your results
	namedWindow( "Result", CV_WINDOW_AUTOSIZE );
	imshow( "Result", gray_image );
	imwrite( ins+"_result.jpg", gray_image );
	//delete array
	for (int i=0; i<col; i++){
		delete [] second0[i];
	}
	delete [] second0;
	for (int i=0; i<col; i++){
		delete [] first0[i];
		delete [] first20[i];
	}
	delete [] first0;
	delete [] first20;
	//imwrite( "S2_fc.jpg", src );
	waitKey(0);
	return 0;
}
//
int mask33(int x,int y,int i){
	int arr33[3][3]={{-1,-1,-1},
					 {-1, 8,-1},
					 {-1,-1,-1}};
	int lu=checks(x-1,y-1,i);
	int u=checks(x,y-1,i);
	int ru=checks(x+1,y-1,i);
	int l=checks(x-1,y,i);
	int m=checks(x,y,i);
	int r=checks(x+1,y,i);
	int ld=checks(x-1,y+1,i);
	int d=checks(x,y+1,i);
	int rd=checks(x+1,y+1,i);

	float color_v=(arr33[0][0]*lu+arr33[0][1]*u+arr33[0][2]*ru+
				 arr33[1][0]*l+arr33[1][1]*m+arr33[1][2]*r+
				 arr33[2][0]*ld+arr33[2][1]*d+arr33[2][2]*rd
	)/9;

	return color_v;
}
//
int mask_sobel(int x,int y,int i){
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
	);
	float color_v2=(arr2[0][0]*lu+arr2[0][1]*u+arr2[0][2]*ru+
				 arr2[1][0]*l+arr2[1][1]*m+arr2[1][2]*r+
				 arr2[2][0]*ld+arr2[2][1]*d+arr2[2][2]*rd
	);
	int tp=(int)sqrt(color_v*color_v+color_v2*color_v2);
	return tp<=255?tp:255;
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
		uchar c=gray_image.at<uchar>(y,x);
		//cout<<"C:"<<c<<endl;
		return 	c;
	}
	else
		return 0;
}
int checks(int x,int y,int i){
	if(x>=0&&x<src.cols&&y>=0&&y<src.rows){
		uchar c=gray_image.at<uchar>(y,x);
		return 	c;
	}
	else
		return 0;
}
//檢查陣列
int check2(int x,int y,int i){
	if(x>=0&&x<src.cols&&y>=0&&y<src.rows){
			return first0[x][y];
	}
	else
		return 0;
}
