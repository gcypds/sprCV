
// minimalistic foreground-background segmentation sample, based off OpenCV's bgfg_segm sample

#include "BackgroundRegions.h"
#include "BackgroundSubtractorSuBSENSE.h"

// Boost includes
#include <boost/filesystem.hpp>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/background_segm.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <time.h>
#include <vector>
#include <iostream>

using namespace std;
using namespace cv;
using namespace boost::filesystem;

float pminArea = 0.0003;

#define MAX_PRINTF_BUFFER_SIZE 200
char printf_buffer[MAX_PRINTF_BUFFER_SIZE] = {'\0'};
const string variables_pathname = "cbookf\\";

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);   
    strftime(buf, sizeof(buf), "%X - %d/%m/%Y -> ", &tstruct);

    return buf;
}

int main(int argc, char *argv[]) {

	if (argc < 5)
	{
		cout << currentDateTime()  << "Usage: BMGenerator.exe video_path flag_roi(1=true, 0=false) flag_load(1=true, 0=false) flag_saveROI(1=true, 0=false)\n";
		return 1;
	}

	cv::VideoCapture oVideoInput;

    cv::Mat oCurrInputFrame, oCurrSegmMask, oCurrReconstrBGImg;	

	oVideoInput.open(argv[1]);
	oVideoInput >> oCurrInputFrame;
	oVideoInput.set(CV_CAP_PROP_POS_FRAMES,0);	

	// create BGS Subsense object
	BackgroundSubtractorSuBSENSE oBGSAlg;

	// copy loadvars flag
	if (strcmp(argv[3], "1") == 0) {
		oBGSAlg.loadvars = true;
	} else {
		oBGSAlg.loadvars = false;
	}
			
	// check if open
	if(!oVideoInput.isOpened() || oCurrInputFrame.empty()) {
		printf("Could not open video file at '%s'.\n", argv[1]);
		cv::waitKey();
		return -1;
	}

	// Initialize Subsense variables
	oCurrSegmMask.create(oCurrInputFrame.size(),CV_8UC1);
	oCurrReconstrBGImg.create(oCurrInputFrame.size(),oCurrInputFrame.type());

	// Depending on flag_roi load ROI or not
	cv::Mat R;
	if (strcmp(argv[2], "1") == 0) {
		R = cv::imread("ROI.png",CV_8UC1);
	} else {
		R = cv::Mat(oCurrInputFrame.size(),CV_8UC1,cv::Scalar_<uchar>(255));
	}

	// Initialize Subsense
	path video_path (argv[1]);

	cout << "video_path filename: " << video_path.filename().replace_extension("").string() << endl;

	ostringstream codebook_paramters_path;

	codebook_paramters_path << video_path.remove_filename().string() << "\\" << video_path.filename().replace_extension("").string() << "_cparameters.dat";
	cout << "cparameters filename: " <<  codebook_paramters_path.str() << endl;

	oBGSAlg.saveCodebookParametersPath(codebook_paramters_path.str());
	oBGSAlg.initialize(oCurrInputFrame, R);

	// create visualization windows
    cv::namedWindow("input",cv::WINDOW_AUTOSIZE);
    cv::namedWindow("segmentation mask",cv::WINDOW_AUTOSIZE);

	int roi_counter = 0;
	int frame_counter = 0;
	// loop through video frames
	while(1) {
				
		std::cout << "frame counter = " << frame_counter++ << std::endl;
		oVideoInput >> oCurrInputFrame;

		if(oCurrInputFrame.empty())
			break;

		// Process
		oBGSAlg(oCurrInputFrame,oCurrSegmMask);
		// oBGSAlg.getBackgroundImage(oCurrReconstrBGImg);
		std::vector<F_state_struct> F_states;

		getRegions(oCurrSegmMask, F_states, pminArea);

		for(int i=0; i<F_states.size(); i++) {
			std::cout << "Drawing: " << F_states.at(i).min_x << " , " << F_states.at(i).min_y << " , " << F_states.at(i).max_x << " , " << F_states.at(i).max_y << std::endl;
			cv:Rect drawing_rectangle = Rect(Point(F_states.at(i).min_x, F_states.at(i).min_y), Point(F_states.at(i).max_x, F_states.at(i).max_y));
			
			Mat drawingROI;
			Mat drawingROIMask = oCurrSegmMask(drawing_rectangle);
			
			oCurrInputFrame(drawing_rectangle).copyTo(drawingROI, drawingROIMask);
			sprintf(printf_buffer, "rois\\drawing%04d.png", roi_counter++);
			imwrite(printf_buffer, drawingROI);
			//rectangle( oCurrInputFrame, Point(F_states.at(i).min_x, F_states.at(i).min_y), Point(F_states.at(i).max_x, F_states.at(i).max_y), Scalar( 0, 55, 255 ), +3, 4 );
		}

		//Background Subtraction Visualization
		imshow("input",oCurrInputFrame);
		imshow("segmentation mask",oCurrSegmMask);
		//imshow("reconstructed background",oCurrReconstrBGImg);								
				
		if(cv::waitKey(1)==27)
			break;
	}

	//Save subsense codebooks and parameters
	if (strcmp(argv[4], "1") == 0) {
		oBGSAlg.saveVariables();
	}
	
	return 0;
}

