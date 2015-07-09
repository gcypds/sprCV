#ifndef People_Features_H
#define People_Features_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <ctime>
#include <cstring>
#include <tuple>
#include <time.h>	//for testing


#define PI 3.14159265 

using namespace cv;
using namespace std;

typedef tuple<float, vector<int>> P_tuple; //holds a float weight and a P candidate states vector

//----------------------------Load precomputed values-----------------------------

extern "C" unsigned char Mat_W;
extern "C" unsigned char Mat_W_size;

extern "C" unsigned char alpha;
extern "C" unsigned char alpha_size;

extern "C" unsigned char bias;
extern "C" unsigned char bias_size;

extern "C" unsigned char sigma;
extern "C" unsigned char sigma_size;

extern "C" unsigned char Mu_zscore;
extern "C" unsigned char Mu_zscore_size;

extern "C" unsigned char Dev_zscore;
extern "C" unsigned char Dev_zscore_size;

extern "C" unsigned char SupportVectors;
extern "C" unsigned char SupportVectors_size;


//Color library
const static uchar Color[20][3] = {{200,0,0},{0,0,200},{0,200,0},{200,200,0},{0,200,200},{200,0,200},
{0,80,255},{220,150,200},{0,250,100},{100,0,250},{100,250,0},{240,240,100},{180,250,20},{160,190,20},
{25,255,155},{180,130,70},{32,165,220},{120,150,255},{125,25,25},{105,180,180}};


//----------------------------------Main Class-----------------------------------
class People_Features{

public:

	//--------------------------------Main Functions--------------------------------
	float People_Automatic(Mat Fm, vector<int> Fs_states1, vector<int> Xcroi1, vector<int> Ycroi1);
	
	float People_Aleatory(Mat fm, vector<int> Fs_states1, vector<int> Xcroi1, vector<int> Ycroi1);

	float People_Background(Mat Fm);
	
	People_Features();

	People_Features(Mat iniFr);

	~People_Features();

	void Get_PROIs(Mat Frame, Mat Fmask);
	
	Mat ROIsearch(Mat F, vector<int> F_states, vector<int> Xc, vector<int> Yc);

	Mat ImPreprocess(Mat Im);

	vector<float> HOG_compute(Mat Im);//pregunta

	Mat Projection(vector<float> Sample);

	float SVM_Classify(Mat A);

	void SVM_ClassifyCV_Multi(Mat A, vector<vector<int>> &StatesCV);

	void GroupPROI(vector<P_tuple> P_states_f,int disa);

	bool SVM_ClassifyCV(Mat A);

	//------------------------------Auxiliar Functions-------------------------------

	int Bwlabel(Mat Fmask, Mat &Out);

	void Erosion(Mat src, Mat des, int erosion_size=1);

	int Randn_int(int mean, double std, double low_lim, double high_lim);

	int Row_coord(vector<int> F_states);
	
	int Col_coord(vector<int> Yc, vector<int> Xc, int cy, int wid, vector<int> F_states);

	vector<int> People_map(int cy, int max_y);

	vector<int> find_vals_vec(vector<int> A, vector<int> B, int cy, int wid);

	float GKernel_Mats(Mat A, Mat B);

	Mat Pointer2Mat(unsigned char *A, Size A_size);

	Mat Vector2Mat(vector<float> A);

	vector<float> Mat2Vector(Mat A);

	Mat Mul3(Mat A3, Mat B1);

	//--------------------------------Test Functions--------------------------------

	void label_save_Sample(Mat A, vector<float> B);

	void Imagesc(cv::Mat src, int siz, char* Plotname);

	void showDetections(vector<P_tuple> PF_states);

	void showDetections(const vector<Rect>& found, Mat& imageData);

	void writeHOG(Mat A);
	
	void writeHOG(vector<float> A);

	//-----------------------------------Variables-----------------------------------

	Mat Im;
	Mat PIm;
	Mat Fmask;			//Current foreground mask
	vector<vector<int>> F_states;	//States of Query ROIs
	vector<vector<int>> F_s;
	vector<vector<int>> F_s_A;
	vector<vector<int>> F_s_B;
	vector<float> W;
	vector<float> W_A;
	vector<float> W_B;


	//Mat Pmask;
	vector<P_tuple> PF_states;	//Resulting array. Holds the svm weights and the states of all detected PROIs
	vector<P_tuple> PF_states_A;
	vector<P_tuple> PF_states_B;
private:


	//-----------------------------------Variables-----------------------------------
	FILE *Fhog1, *Fhog2;

	Mat Rmask;			//Current foreground mask with isolated regions
	int PNroi;			//Number of isolated regions in current foreground mask
	int minArea;		//Minimum area to accept a ROI according to pminArea parameter
	int im_r, im_c;		//image number of rows and columns

	vector<vector<int>> Xcroi;				//X coordinates of countours of each isolated region 
	vector<vector<int>> Ycroi;				//Y coordinates of countours of each isolated region 
	vector<vector<cv::Point>> contours;
	vector<cv::Vec4i> hierarchy;
	cv::Point pt;

	vector<int> P_states;
	vector<int> P2_states;
	vector<int> P3_states;
	vector<P_tuple> PC_states;
	vector<P_tuple> PC_states_A;
	vector<P_tuple> PC_states_B;
	

	//----------------------------------Parameters------------------------------------

	float pminArea;	//Minimum percentage of the frame size to accept a valid ROI
	
	Mat Map_W;		//Mapping matrix to low dimensional space

	float std_people_map;	//deviation for the people map

	Mat Mu;			//Mean for zcore new sample
	Mat Dev;		//Standard deviation for zcore new variable

	int ppl_max_iter;	//Maximum iteration to search a PROI in a FROI

	float svm_thr;

	//SVM parameters
	Mat Support_vecs;		//Support vectors
	Mat Alpha;				//Alpha
	float beta;				//Beta
	float sigmak;			//Sigma

	Mat Atest;

};
#endif
