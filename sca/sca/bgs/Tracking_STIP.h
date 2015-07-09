#ifndef Tracking_STIP_H
#define Tracking_STIP_H

#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>
#include <iterator>

//Color library
const static double Color[20][3] = {{200/255.,0,0},{0,0,200./255.},{0,200./255.,0},{200/255.,200/255.,0},{0,200/255.,200/255.},{200/255.,0,200/255.},
{0,80/255.,255/255.},{220/255.,150/255.,200/255.},{0,250/255.,100/255.},{100/255.,0,250/255.},{100/255.,250/255.,0},
{240/255.,240/255.,100/255.},{180/255.,250/255.,20/255.},{160/255.,190/255.,20/255.},{25/255.,255/255.,155/255.},{180/255.,130/255.,70/255.},
{32/255.,165/255.,220/255.},{120/255.,150/255.,255/255.},{125/255.,25/255.,25/255.},{105/255.,180/255.,180/255.}};

using std::vector;
using cv::Mat;

class Tracking_STIP{
public:

	//-------------Main Functions-------------
	Tracking_STIP(cv::Mat iniFr);

	~Tracking_STIP();

	void getRegions();

	void matchRegions();

	void matchSTIPs(vector<vector<float>> Query, vector<vector<float>> Model);

	//-----------Auxiliar Functions-----------
	void Bwlabel(Mat src, Mat &des);

	void Erosion(Mat src, Mat des);

	void Imagesc(Mat src, int siz, char* Plotname);

	void Save_bbox();
	
	float Tracking_STIP::Gkernel_vectors(vector<float> A, vector<float> B);

	cv::Mat Mul3(cv::Mat A3, cv::Mat B1);

	//---------------Variables----------------
	Mat Im_RGB;		//Current RGB image
	Mat Im_LBSP;	//Current LBSP image
	Mat Fmask;		//Current foreground mask
	Mat Rmask;		//Current foreground mask with isolated regions
	int Nroi;			//Number of isolated regions in current foreground mask
	int fr_idx;			//Frame index
	char vidname[50];	//Video name
	vector<vector<vector<float>>> STIPs_roi; //Color STIPs descriptor for each roi

	//---------------Parameters---------------
	double pminArea;	//Minimum percentage of the frame size to accept a valid ROI
	float thr_match;	//Threshold to validate the match between 2 STIP descriptors 

private:

	//---------------Variables----------------
	vector<vector<cv::Point>> contours;
	vector<cv::Vec4i> hierarchy;
	cv::Point pt;
	int minArea;
	vector<vector<int>> Xcroi;			//X coordinates of countours of each isolated region 
	vector<vector<int>> Ycroi;			//Y coordinates of countours of each isolated region 
	vector<vector<int>> Bbox;			//Current detected bounding boxes
	vector<vector<int>> Bbox_states;	//States of detected bounding boxes
};
#endif