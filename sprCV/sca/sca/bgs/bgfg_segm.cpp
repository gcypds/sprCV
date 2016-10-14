
// minimalistic foreground-background segmentation sample, based off OpenCV's bgfg_segm sample

#include "BackgroundSubtractorSuBSENSE.h"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/background_segm.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <vector>
#include <iostream>
#include "Input_source.h"
#include "Tracking_STIP.h"
#include "../stip/ActionHOGLibs.h"

int bg_main() 
{
	Input_source In_src;

	int src;

	src = In_src.menu();
    
	cv::VideoCapture oVideoInput;

    cv::Mat oCurrInputFrame, oCurrSegmMask, oCurrReconstrBGImg;

	int n_vid, cat_sel, vid_id;

	bool flag_roi = false;		//true: Load roi, false: work with full frame
	bool flag_load = false;		//true: Load precomputed codebooks, false: Initialize from zero
	bool flag_saveROI = false;	//true: Save color, lbsp and stip for each roi, false: don't save

	//Dsiplay flag information 
	std::cout<<std::endl<<"Load ROI : "<<flag_roi<<" Load parameters : "<<flag_load<<" Save ROI info : "<<flag_saveROI<<std::endl<<std::endl;

	switch (src)
	{
		//Image source----------------
	case 1: In_src.Im_seq_sel();
			break;
		//Video source----------------
	case 2: In_src.Vid_sel();
			oVideoInput.open(In_src.vpath);
			oVideoInput >> oCurrInputFrame;
			oVideoInput.set(CV_CAP_PROP_POS_FRAMES,0);
			break;
		//WebCam source----------------
	case 3: In_src.WebCam_sel();
			oVideoInput.open(0);
			oVideoInput >> oCurrInputFrame;
			break;
	}

	//create visualization windows
    cv::namedWindow("input",cv::WINDOW_NORMAL);
    cv::namedWindow("segmentation mask",cv::WINDOW_NORMAL);
    //cv::namedWindow("reconstructed background",cv::WINDOW_NORMAL);

	for(int c=0; c<In_src.Im_seqs[0].size();c++)	//loop through video categories
	{
		//identify number of videos to test (depending the categories selected)
		if(In_src.Im_seqs[1][0]!=-1)
		{
			n_vid = In_src.Im_seqs[1].size();	//single category
		}
		else
		{
			n_vid = In_src.Im_seqs[2][c];		//multiple categories
		}

		cat_sel = In_src.Im_seqs[0][c];		//get category to test	

		for(int v=0; v<n_vid; v++)	//loop through videos
		{
			//get videos ids (depending the categories selected)
			if(In_src.Im_seqs[1][0]!=-1)
			{
				vid_id = In_src.Im_seqs[1][v];	
			}
			else
			{
				vid_id = v+1;
			}

			if(src==1)
			{
				In_src.Im_seq_info(cat_sel,vid_id);
				oCurrInputFrame = In_src.ReadIm(In_src.inifr);
				In_src.fr_idx = In_src.inifr;
			}

			//create BGS Subsense object
			BackgroundSubtractorSuBSENSE oBGSAlg;

			//copy loadvars flag
			oBGSAlg.loadvars = flag_load;

			//create Tracking_Stip object
			Tracking_STIP Track_X(oCurrInputFrame);

			//create stip object
			ActionHOG Stip_X;
			
			//check if open
			if(src==2)
			{
				if(!oVideoInput.isOpened() || oCurrInputFrame.empty()) {
					printf("Could not open video file at '%s'.\n",In_src.vpath);
					cv::waitKey();
					return -1;
				}
			}
			if(src==3)
			{
				if(!oVideoInput.isOpened() || oCurrInputFrame.empty()) {
					printf("Could not open default camera.\n");
					cv::waitKey();
					return -1;
				}
			}

			//Initialize Subsense variables
			oCurrSegmMask.create(oCurrInputFrame.size(),CV_8UC1);
			oCurrReconstrBGImg.create(oCurrInputFrame.size(),oCurrInputFrame.type());

			//Depending on flag_roi load ROI or not
			cv::Mat R;
			if(flag_roi){
				R = cv::imread("ROI.png",CV_8UC1);
			}
			else
			{
				R = cv::Mat(oCurrInputFrame.size(),CV_8UC1,cv::Scalar_<uchar>(255));
			}

			//Initialize Subsense
			oBGSAlg.initialize(oCurrInputFrame,R);

			//loop through video frames
			while(In_src.fr_idx<In_src.endfr) {
				
				if(src!=1)
				{
					oVideoInput >> oCurrInputFrame;
				}
				else
				{
					oCurrInputFrame = In_src.ReadIm(In_src.fr_idx);
					In_src.fr_idx++;
				}
				if(oCurrInputFrame.empty())
					break;

				//Process
				oBGSAlg(oCurrInputFrame,oCurrSegmMask);
				//oBGSAlg.getBackgroundImage(oCurrReconstrBGImg);

				//Visualization
				imshow("input",oCurrInputFrame);
				imshow("segmentation mask",oCurrSegmMask);
				//imshow("reconstructed background",oCurrReconstrBGImg);

				//Copy information to track object
				Track_X.Fmask = oCurrSegmMask;
				Track_X.Im_RGB = oCurrInputFrame;
				Track_X.Im_LBSP = oBGSAlg.LBSP_m;
				Track_X.fr_idx = In_src.fr_idx;
				strcpy(Track_X.vidname,In_src.vidname);

				//Get bbox coordinates of isolated foreground regions and store into registers
				Track_X.getRegions();

				//Save bbox color and LBSP image every 3 frames
				if((In_src.fr_idx % 3 == 0)&flag_saveROI){
					Track_X.Save_bbox();
				}

				//Copy information to ActionHOG object
				Stip_X.src = oCurrInputFrame;
				Stip_X.Rmask = Track_X.Rmask;
				strcpy(Stip_X.vidname ,In_src.vidname);
				Stip_X.fr_idx = In_src.fr_idx;
				Stip_X.Nbbox = Track_X.Nroi;

				//Compute STIP features
				Stip_X.comp();

				//Write keys and descriptors every 3 frames
				if((In_src.fr_idx % 3 == 0)&flag_saveROI){
					Stip_X.writeKeyDesc(In_src.fr_idx, Stip_X.dstKeysf);
				}

				//Copy STIP descriptors to track object
				Track_X.STIPs_roi = Stip_X.STIPs_roi;

				

				if(cv::waitKey(1)==27)
					break;
			}
			//Save subsense codebooks and parameters
			//oBGSAlg.saveVariables();
		}
	}
	return 0;
}

