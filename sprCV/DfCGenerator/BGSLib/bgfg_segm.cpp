
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

float pminArea = 0.0003;

#define MAX_PRINTF_BUFFER_SIZE 200
char printf_buffer[MAX_PRINTF_BUFFER_SIZE] = {'\0'};
const string variables_pathname = "cbookf\\";

int main() {
	cv::VideoCapture oVideoInput;

    cv::Mat oCurrInputFrame, oCurrSegmMask, oCurrReconstrBGImg;

	int n_vid, cat_sel, vid_id;

	bool flag_roi = false;		//true: Load roi, false: work with full frame
	bool flag_load = true;		//true: Load precomputed codebooks, false: Initialize from zero
	bool flag_saveROI = true;	//true: Save color, lbsp and stip for each roi, false: don't save

	// Display flag information 
	std::cout<<std::endl<<"Load ROI : "<<flag_roi<<" Load parameters : "<<flag_load<<" Save ROI info : "<<flag_saveROI<<std::endl<<std::endl;	

	char vpath[] = "G:\\data\\sebastianvilla139\\work\\fishclef\\dataset\\videos\\video3.flv";

	oVideoInput.open(vpath);
	oVideoInput >> oCurrInputFrame;
	oVideoInput.set(CV_CAP_PROP_POS_FRAMES,0);

	// cv::namedWindow("reconstructed background",cv::WINDOW_NORMAL);

	// create BGS Subsense object
	BackgroundSubtractorSuBSENSE oBGSAlg;

	// copy loadvars flag
	oBGSAlg.loadvars = flag_load;			
			
	// check if open
	if(!oVideoInput.isOpened() || oCurrInputFrame.empty()) {
		printf("Could not open video file at '%s'.\n", vpath);
		cv::waitKey();
		return -1;
	}

	// Initialize Subsense variables
	oCurrSegmMask.create(oCurrInputFrame.size(),CV_8UC1);
	oCurrReconstrBGImg.create(oCurrInputFrame.size(),oCurrInputFrame.type());

	// Depending on flag_roi load ROI or not
	cv::Mat R;
	if(flag_roi) {
		R = cv::imread("ROI.png",CV_8UC1);
	} else {
		R = cv::Mat(oCurrInputFrame.size(),CV_8UC1,cv::Scalar_<uchar>(255));
	}

	// Initialize Subsense
	oBGSAlg.initialize(oCurrInputFrame,R);

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
	oBGSAlg.saveVariables();
	return 0;
}

