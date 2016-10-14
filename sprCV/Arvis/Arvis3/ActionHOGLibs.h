#ifndef ACTION_HOG_LIBS_H
#define ACTION_HOG_LIBS_H

#include <string>
#include "opencv2/core/core.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include <tuple>


using std::string;

using namespace cv;

//holds a float weight and a P candidate states vector
typedef std::tuple<float, vector<int>> P_tuple; 

// compute dense optical flow based on each patch or entire frame 
#define OPT_PATCH 
// minimum intensity difference to be considered as motion
const double MHI_DIFF_THRESH = 20.0;
// motion duration
const double MHI_DURATION = 20.0;
// minimum threshold to binarize before morphlogical operation
const double MHI_MORPH_THRESH = 100.0;
// minimum threshold for pixel intensity 
const double MHI_MIN_THRESH = 200.0;
// minimum threshold for pixel motion
const double OPT_MIN_THRESH = 1.0;
// maximum threshold for pixel motion
const double OPT_MAX_THRESH = 50.0;
// a frame duration (ms) in visualization
const int fdur = 20;

class ActionHOG {
public:
	ActionHOG();

	ActionHOG(Mat iniFrame);

	~ActionHOG();

	// compute ActionHOG
	int comp(Mat Frame, vector<P_tuple> PF_states, Mat Fmask);

	// compute ActionHOG2
	int comp(Mat Frame, vector<vector<int>> PF_states, Mat Fmask);

	// detect interest points based on a detector
	int detKeys(const Mat &img, vector<KeyPoint> &keys);

	// compute MHI
	int getMotionHistoryImage(int idx, const Mat &pre, const Mat &cur, Mat &mhi8U, Mat &mhi32F);

	// remove static points with insufficient motion
	int filterKeysByMotion(const vector<KeyPoint> &srcKeys, vector<KeyPoint> &dstKeys,
						   const Mat &mhi, const Mat &pre, const Mat &cur, const Mat &src);

	// compute HOG on IMG channel
	int getImageHOG(const Mat &img, const vector<KeyPoint> &keys, Mat &hog);

	// compute HOG on MHI channel
	int getMotionHistoryImageHOG(const Mat &mhi, const vector<KeyPoint> &keys, Mat &hog);

	// compute HOG on OPT channel
	int getOpticalFlowHOG(const Mat &pre, const Mat &cur, const vector<KeyPoint> &keys, Mat &hog);

	// check if keypoint is enclosed in a FROI
	bool kpt_in_FROI(cv::KeyPoint a, Mat Fmask, float lrg_Fmask_srch);

	// write points and descriptors
	int writeKeyDesc(const vector<KeyPoint> &keys);

	//current frame
	Mat src, pre, cur;

	// motion history images with different data types
	Mat mhi8U, mhi32F; 

	int fr_idx;

	char vidname[50];

	

	vector<KeyPoint> dstKeysf;

	//Stores color STIP descriptors for each foreground mask
	vector<vector<vector<float>>> STIPs_fmask;

	vector<vector<KeyPoint>> kpts_fmask;

private:

	// Filter STIPS with foreground masks
	void filter_Fmask(vector<P_tuple> PF_states, Mat Fmask);

	// Filter STIPS with foreground masks
	void filter_Fmask(vector<vector<int>> PF_states, Mat Fmask);

	// video file
	VideoCapture vid;

	// feature file
	FILE *fp;

	int Nbbox;	//Number of detected persons

	// detector and channels
	string det;
	string chan;

	// channel flag
	bool imgflag, mhiflag, optflag;

	// visualization flag
	bool vis;

	// number of spatial grids of each channel
	int imgngs;
	int mhings;
	int optngs;

	// number of gradient orientation bins of each channel
	int imgnbs;
	int mhinbs;
	int optnbs;

	// frame size
	int im_r;
	int im_c;
	int fr_p;

	float lrg_Fmask_srch;	//Percentage to enlarge FROIs to accept kpts inside them

	// descriptor dimensions
	int imgHOGDims, mhiHOGDims, optHOGDims;

	// descriptors of each frame on different channels
	Mat mhiHOG, optHOG, imgHOG;

	vector<KeyPoint> dstKeys;

	vector<int> kpts_roi;	
};

#endif