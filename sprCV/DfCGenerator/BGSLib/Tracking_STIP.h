#ifndef Tracking_STIP_H
#define Tracking_STIP_H

#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/features2d/features2d.hpp"
#include <math.h>
#include <iterator>
#include <tuple>
#include <deque>


//Color library
const static uchar Color[20][3] = {{200,0,0},{0,0,200},{0,200,0},{200,200,0},{0,200,200},{200,0,200},
{0,80,255},{220,150,200},{0,250,100},{100,0,250},{100,250,0},{240,240,100},{180,250,20},{160,190,20},
{25,255,155},{180,130,70},{32,165,220},{120,150,255},{125,25,25},{105,180,180}};

using std::deque;
using std::vector;
using cv::Mat;

typedef std::tuple<float, cv::KeyPoint, vector<float>> M_STIP_tuple; //holds a float weight and a STIP vector

class Tracking_STIP{
public:

	//-------------Main Functions-------------
	Tracking_STIP(cv::Mat iniFr);

	~Tracking_STIP();

	void initROI(vector<vector<int>> Q_states, vector<vector<vector<float>>> Q_STIPs, bool ini);

	void getRegions();

	void matchRegions(vector<vector<int>> Q_states, vector<vector<vector<float>>> Q_STIPs);

	int matchSTIPs(deque<M_STIP_tuple> M_STIPs, vector<vector<float>> Q_STIPs, vector<cv::KeyPoint> kpts_fmask);

	void SortSTIPs(deque<M_STIP_tuple> M_STIPs);

	void getCandidateROI(vector<cv::KeyPoint> kpts_fmask, vector<vector<float>> Q_STIPs, vector<int> Q_states, vector<int> M_matches_ref, int M_idx);

	void UpdtModelROI(int M_idx, vector<int> C_states, vector<M_STIP_tuple> C_STIPs, vector<int> M_matches, vector<cv::KeyPoint> kpts_fmask);

	//-----------Auxiliar Functions-----------
	void Bwlabel(Mat src, Mat &des);

	void Erosion(Mat src, Mat des);

	void Imagesc(Mat src, int siz, char* Plotname);

	void Save_bbox();

	float Norm_vectors(vector<float> A, vector<float> B);

	void DrawRects(vector<int> A, vector<int> B, char* plot1, char* plot2);

	void Tracking_Plot();

	cv::Mat Mul3(cv::Mat A3, cv::Mat B1);

	//---------------Variables----------------
	Mat Im_RGB;			//Current RGB image
	Mat Im_LBSP;		//Current LBSP image
	Mat Fmask;			//Current foreground mask
	Mat Rmask;			//Current foreground mask with isolated regions
	int Nroi;			//Number of isolated regions in current foreground mask
	int fr_idx;			//Frame index
	char vidname[50];	//Video name

	vector<vector<vector<float>>> Q_STIPs;		//Color STIPs descriptor for each roi
	vector<vector<int>> Q_states;				//States of Query ROIs
	vector<vector<cv::KeyPoint>> kpts_fmask;	//Keypoints info for ROIs

	//---------------Parameters---------------
	float pminArea;			//Minimum percentage of the frame size to accept a valid ROI
	float thr_match;		//Threshold to validate the match between 2 STIP descriptors 
	float ROI_srch_radius;	//Search radius to match a Model ROI with a Query ROI 
	float STIP_srch_radius;	//Search radius to match a Model STIP with a Query STIP
	float NStips_match;		//Percentage of NStips_ROI STIPs required to accept a match 
	int NStips_ROI;			//Number of STIPs used to model a ROI
	float C_ROI_lrg;		//Percentage of Candidate ROI to enlarge its bounding box
	int STIP_updt_rate;		//Model STIPs updated from Candidate STIPs
	float w_alpha;			//STIP Weight updating parameter
	float w_ini;			//STIP Initial weight
	int M_STIPs_min;		//Minimum M_STIPs required to calculate C_STIPs
	float s_beta;			//M_states updating rate
	float hw_thr;

private:

	//---------------Variables----------------
	vector<vector<cv::Point>> contours;
	vector<cv::Vec4i> hierarchy;
	cv::Point pt;
	int minArea;

	int im_r, im_c;		//image number of rows and columns

	vector<vector<int>> Xcroi;				//X coordinates of countours of each isolated region 
	vector<vector<int>> Ycroi;				//Y coordinates of countours of each isolated region 
	vector<vector<int>> M_states;			//States of Model ROIs
	vector<deque<M_STIP_tuple>> M_STIPs;	//STIPs tuples of Model ROIs 
	vector<int> M_matches;					//For each Model STIP holds index of Query STIP that matches if not -1
	vector<int> C_states;					//States of Candidate ROI
	vector<M_STIP_tuple> C_STIPs;			//Color STIPs of Candidate ROI
	vector<bool> Q_ROI_matched;				//Query ROIs that matched Model ROIs, the Query ROIs that left are used to initialize new Model ROIs
};
#endif