
// minimalistic foreground-background segmentation sample, based off OpenCV's bgfg_segm sample

#include "BackgroundRegions.h"
#include "BackgroundSubtractorSuBSENSE.h"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/background_segm.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <vector>
#include <iostream>

using namespace cv;

//Color library
const static uchar Color[20][3] = {{200,0,0},{0,0,200},{0,200,0},{200,200,0},{0,200,200},{200,0,200},
{0,80,255},{220,150,200},{0,250,100},{100,0,250},{100,250,0},{240,240,100},{180,250,20},{160,190,20},
{25,255,155},{180,130,70},{32,165,220},{120,150,255},{125,25,25},{105,180,180}};

vector<vector<int>> Xcroi;				//X coordinates of countours of each isolated region 
vector<vector<int>> Ycroi;				//Y coordinates of countours of each isolated region 
vector<vector<cv::Point>> contours;
vector<cv::Vec4i> hierarchy;
cv::Point pt;
int Nroi;			//Number of isolated regions in current foreground mask

//-------------------------------------------------------------------------------------
//Imagesc
//-------------------------------------------------------------------------------------
void Imagesc(cv::Mat src, int siz, char* Plotname)
{
	//Color Directory
	cv::Mat output(cv::Size(src.size.p[1],src.size.p[0]),CV_8UC3,CV_RGB(0,0,0));
	cv::Mat aux = src.clone();
	cv::Mat aux3;
	cv::Scalar v;
	for(int i=0; i<=siz; i++)
	{
		v.val[0]=i;
		compare(src,v,aux,cv::CMP_EQ);

		cv::Mat aux2[]= {aux*Color[i][0],aux*Color[i][1],aux*Color[i][2]};
		merge(aux2,3,aux3);
		output = aux3+output;
	}
	/*imshow(Plotname,output);*/
}

//-------------------------------------------------------------------------------------
//Erosion
//-------------------------------------------------------------------------------------
void Erosion(cv::Mat src, cv::Mat des)
{
	int erosion_elem = 2;
	int erosion_size = 1;
	int erosion_type;

	if( erosion_elem == 0 ){ erosion_type = cv::MORPH_RECT; }
	else if( erosion_elem == 1 ){ erosion_type = cv::MORPH_CROSS; }
	else if( erosion_elem == 2) { erosion_type = cv::MORPH_ELLIPSE; }

	cv::Mat element = cv::getStructuringElement( erosion_type,
		cv::Size( 2*erosion_size + 1, 2*erosion_size+1 ),
		cv::Point( erosion_size, erosion_size ) );
	/// Apply the erosion operation
	erode( src, des, element );
}

//-------------------------------------------------------------------------------------
//Label and extract isolated regions from foreground mask
//-------------------------------------------------------------------------------------
void Bwlabel(Mat Fmask, Mat &Out, int minArea){
	
	Mat cFmask = Fmask.clone();

	Erosion(cFmask ,cFmask);

	Mat A = cFmask.clone(); 

	findContours(A, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	Nroi = 0;

	//Variables to store contours coordinates of each isolated region 
	Xcroi.clear();
	Ycroi.clear();
	std::vector<int> X_temp;
	std::vector<int> Y_temp;

	for(int i=0; i<(int)contours.size(); i++ )
	{
		pt.x = contours[i][0].x;
		pt.y = contours[i][0].y;
		if((int)contours[i].size()>=minArea)
		{ 
			Nroi++;
			floodFill(cFmask,pt, cv::Scalar::all(Nroi));

			X_temp.clear();
			Y_temp.clear();

			//store contours coordinates
			for(int j=0; j<(int)contours[i].size();j++){
				X_temp.push_back(contours[i][j].x);
				Y_temp.push_back(contours[i][j].y);
			}
			Xcroi.push_back(X_temp);
			Ycroi.push_back(Y_temp);
		}
		else
		{
			floodFill(cFmask,pt, cv::Scalar::all(0));
		}
	}
	Imagesc(cFmask,Nroi,"regions");
	//cv::waitKey();
	Out = cFmask.clone();
}

//-------------------------------------------------------------------------------------
//Get bbox coordinates of F_ROIs and store into register
//-------------------------------------------------------------------------------------
void getRegions(Mat F, std::vector<F_state_struct> &F_states, float pminArea) {
	
	int minArea = int(pminArea*float(F.rows*F.cols));  //Minimum area to consider a region	

	Mat Rmask;	
	Bwlabel(F,Rmask, minArea);	//get regions

	F_state_struct region_data;

	Mat P, Ppos;
	
	for (int i=0; i<Nroi;i++){
		//Find the corners of the bounding boxes that cover each moving object
		region_data.min_x = *std::min_element(Xcroi[i].begin(),Xcroi[i].end());   
		region_data.max_x = *std::max_element(Xcroi[i].begin(),Xcroi[i].end());
		region_data.min_y = *std::min_element(Ycroi[i].begin(),Ycroi[i].end());
		region_data.max_y = *std::max_element(Ycroi[i].begin(),Ycroi[i].end());
		region_data.wid = region_data.max_x-region_data.min_x;
		region_data.heig = region_data.max_y-region_data.min_y;
		region_data.ctr_x = region_data.min_x+int(floor(double(region_data.wid)/2.));
		region_data.ctr_y = region_data.min_y+int(floor(double(region_data.heig)/2.));

		//Copy bbox coordinates into registers
		//F_states.clear();
		F_states.push_back(region_data);
 	}
}