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
#include "People_Features.h"


//Color library
const static uchar Color1[20][3] = {{200,0,0},{0,0,200},{0,200,0},{200,200,0},{0,200,200},{200,0,200},
{0,80,255},{220,150,200},{0,250,100},{100,0,250},{100,250,0},{240,240,100},{180,250,20},{160,190,20},
{25,255,155},{180,130,70},{32,165,220},{120,150,255},{125,25,25},{105,180,180}};

using std::deque;
using std::vector;
using std::tuple;
using cv::Mat;
using cv::Scalar;
using std::cout;
using std::cin;
using std::endl;

typedef tuple<float, cv::KeyPoint, vector<float>> M_STIP_tuple; //holds a float weight and a STIP vector

typedef tuple<float, vector<int>> P_tuple; //holds a float weight and a P candidate states vector

class Tracking_STIP{
public:

	//-------------Main Functions-------------
	Tracking_STIP(cv::Mat iniFr);

	~Tracking_STIP();

	void initROI(vector<P_tuple> P_states, vector<vector<vector<float>>> Q_STIPs, vector<vector<cv::KeyPoint>> kpts_fmask, bool ini);

	void matchRegions(Mat frame, vector<vector<int>> Q_states, vector<vector<vector<float>>> Q_STIPs, vector<vector<cv::KeyPoint>> kpts_fmask);

	int matchSTIPs(deque<M_STIP_tuple> M_STIPs, vector<vector<float>> Q_STIPs, vector<cv::KeyPoint> kpts_fmask);

	vector<int> getCandidateROI(vector<cv::KeyPoint> kpts_fmask, vector<vector<float>> Q_STIPs, vector<int> Q_states, vector<int> M_matches_ref, int M_idx);

	vector<int> get_CROI_from_STIPs(vector<cv::KeyPoint> kpts_fmask, vector<vector<float>> Q_STIPs, vector<int> Q_states, vector<int> M_matches_ref, int M_idx);

	bool searchPROI_aroundCROI(vector<int> Cstates, vector<int> &Cstates_new, Mat Frame, int Midx);

	bool checkROI4PROI(vector<int> A, Mat Frame, double &val);
	
	vector<int> getROI_avg_displacement(vector<cv::KeyPoint> Q, vector<int> M, int M_idx);
	
	void SortSTIPs(deque<M_STIP_tuple> M_STIPs);

	deque<M_STIP_tuple> get_CROI_STIPs(vector<int> C_states, vector<int> M_matches_ref, vector<cv::KeyPoint> Q, vector<vector<float>> Q_STIPs);

	void UpdtModelROI(int M_idx, vector<int> C_states, deque<M_STIP_tuple> C_STIPs, vector<int> M_matches, vector<cv::KeyPoint> kpts_fmask);

	bool kpt_in_FROI(cv::KeyPoint a, Mat Fmask, float lrg_Fmask_srch);

	bool MROI_lost(int M_idx);

	void merge_MROInew_MROI(vector<P_tuple> &Pstates, vector<vector<vector<float>>> &Q_STIPs, vector<vector<cv::KeyPoint>> &kpts_fmask);

	//-----------Auxiliar Functions-----------

	void Imagesc(Mat src, int siz, char* Plotname);

	void Save_bbox();

	float Norm_vectors(vector<float> A, vector<float> B);

	void DrawRects(vector<int> A, vector<int> B, char* plot1, char* plot2, cv::Scalar S1 = cv::Scalar(255,0,0), cv::Scalar S2 = cv::Scalar(0,255,0));

	void Tracking_Plot(int fr_idx, char *vid_name);

	cv::Mat Mul3(cv::Mat A3, cv::Mat B1);

	vector<vector<int>> tuple2vector(vector<P_tuple> A);

	void PlotKpts(vector<cv::KeyPoint> A, Mat B, char *Pltname, cv::Scalar S1);

	void PlotKpts(deque<M_STIP_tuple> A, Mat B, char *Pltname, cv::Scalar S1);

	void validate_states(vector<int> &S);

	int Randn_int(double mean, double std, double low_lim, double high_lim);

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
	vector<vector<int>> M_states;			//States of Model ROIs
	vector<deque<M_STIP_tuple>> M_STIPs;	//STIPs tuples of Model ROIs 
	deque<Scalar> M_colors;				//MROIs colors used for tracking

private:

	//---------------Variables----------------
	int im_r, im_c;		//image number of rows and columns
	
	vector<vector<int>> Xcroi;				//X coordinates of countours of each isolated region 
	vector<vector<int>> Ycroi;				//Y coordinates of countours of each isolated region 
	vector<int> M_matches;					//For each Model STIP holds index of Query STIP that matches if not -1
	vector<bool> Q_ROI_matched;				//Query ROIs that matched Model ROIs, the Query ROIs that left are used to initialize new Model ROIs

	People_Features Ppl;					//Object that allows to validate if a tracked region (CROI) corresponds to a PROI

	deque<Scalar> Color_Lib;					//Color library used to plot Bbox tracking

	//---------------Parameters---------------
	
	float thr_match;		//Threshold to validate the match between 2 STIP descriptors 
	float ROI_srch_radius;	//Search radius to match a Model ROI with a Query ROI 
	float STIP_srch_radius;	//Search radius to match a Model STIP with a Query STIP
	float NStips_match;		//Percentage of NStips_ROI STIPs required to accept a match 
	float NStips_join;		//Percentage of NStips_ROI STIPs required to join a new detected PROI to a MROI 
	int NStips_ROI;			//Number of STIPs used to model a ROI
	int min_Stips;		//Minimum number of STIPs to consider a PROI
	float C_ROI_lrg;		//Percentage of Candidate ROI to enlarge its bounding box
	int STIP_updt_rate;		//Model STIPs updated from Candidate STIPs
	float w_alpha;			//STIP Weight updating parameter
	float w_ini;			//STIP Initial weight
	float s_beta;			//M_states updating rate
	float hw_thr;			//Percentage of change of size admited to match a CROI to a MROI
	float ppl_svm_thr;		//Threshold to accept a person from the Ppl class SVM
	int Nsrch;				//Number of PROI searches around a displaced CROI 
	float srch_std;			//Standard deviation to search a displaced PROI
	float lim_srch_dev;		//Limit for search according a percentage of CROI states
	float shrink_updt_roi;	//Percentage to shrink the CROI area to accept STIPs to incorporate into the model
	int lost_lim;			//Limit to decide if a MROI is lost (to delete it from memory)
};
#endif