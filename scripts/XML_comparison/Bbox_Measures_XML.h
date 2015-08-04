#ifndef Bbox_Measures_XML_H
#define Bbox_Measures_XML_H

// XQilla includes
#include <xqilla/xqilla-dom3.hpp>

// XSD Codesynthesis generated stubs includes
#include "XML/video.hxx"

// Standard includes
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iterator>
#include <vector>
#include <tuple>
#include <cv.h>
#include <opencv\highgui.h>

using namespace std;
using namespace xercesc;
XERCES_CPP_NAMESPACE_USE

#define MAX_PRINTF_BUFFER_SIZE 200

typedef tuple<float, vector<int>> P_tuple;

class Bbox_Measures_XML{

public:

	//-------------------------------Main Functions-------------------------------

	Bbox_Measures_XML(char *vidname);

	~Bbox_Measures_XML();

	void Calculate_Measures(char *XML_GT, char *XML_ALG, float &Recall, float &Precision, float &F1);

	void XML_visualization(char *XML1, char *XML2 = NULL, char *XML3 = NULL, char *XML4 = NULL, char *XML5 = NULL);

	//---------------------------------Variables---------------------------------
	cv::VideoCapture oVideoInput;	//Video or image sequence for visualization

private:

	//-----------------------------Auxiliar Functions-----------------------------

	void ReadXML(string inFilename, unique_ptr<gtrois> &v);

	void Comparison_Values(vector<P_tuple> BBox1,vector<P_tuple> BBox2, vector<int> &C_values);

	int Intersect_Area(vector<int> r1, vector<int> r2);

	void ShowDetections(vector<vector<int>> states, char *plot);

	//--------------------------Variables and Parameters--------------------------
	
	cv::Mat Im_RGB;		//Current frame image (Visualization)
	char *Vidname;		//Videoname
	float area_thr;		//Overlaping parameter for supervised measures
};
#endif
