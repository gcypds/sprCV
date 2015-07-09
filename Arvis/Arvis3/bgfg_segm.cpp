
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
#include "ActionHOGLibs.h"
#include "People_Features.h"

#include <time.h>

// OpenCV includes
#include <cv.h>
#include <highgui.h>

// Standard includes
#include <sstream>
#include <iterator>
#include <algorithm>
#include <tuple>

// Boost includes
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

// Xerces includes
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMDocumentType.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationLS.hpp>
#include <xercesc/dom/DOMNodeIterator.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMText.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/XMLUni.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/framework/StdOutFormatTarget.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>


// XSD Codesynthesis generated stubs includes
#include "XML\video.hxx"

using namespace cv;
using namespace std;
using namespace boost::filesystem;
using namespace xercesc;
XERCES_CPP_NAMESPACE_USE
	using std::cerr;

float pminArea = 0.0003;

#define MAX_PRINTF_BUFFER_SIZE 200

char printf_buffer[MAX_PRINTF_BUFFER_SIZE] = {'\0'};
const string variables_pathname = "cbookf\\";

Input_source In_src;
const std::string currentDateTime() {
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);   
	strftime(buf, sizeof(buf), "%X - %d/%m/%Y -> ", &tstruct);

	return buf;
}


bool removeTrailingSpace(string filename) {
	string line;
	// open input file
	ifstream in(filename);
	if( !in.is_open())
	{
		cout << "Input file failed to open\n";
		return false;
	}

	// Remove trailing space
	getline(in,line);
	ofstream out("outfile.xml");

	while( getline(in,line) )
	{
		out << line << "\n";
		cout << line << endl;
	}	
	in.close();
	out.close();

	try {
		//cout << "Removing " << filename.c_str() << endl;
		// delete the original file
		//std::remove(filename.c_str());	
		boost::filesystem::remove(path(filename));

		//cout << "REMOVED! " << filename.c_str() << endl;

		//cout << "Renaming to " << filename.c_str() << endl;
		// rename old to new
		//std::rename("outfile.xml",filename.c_str());
		boost::filesystem::rename(path("outfile.xml"), path(filename));

		//cout << "RENAMED! " << filename.c_str() << endl;
	} catch (const filesystem_error& ex) {
		cout << currentDateTime()  << ex.what() << '\n';
	}

	return 0;
}


void SaveXMLInfo(::xsd::cxx::tree::sequence< ::frame > &f_s, ::xsd::cxx::tree::sequence< ::roi > r_s, int fr_idx)
{
	/*frame::roi_sequence o_s;

	for(int i=0; i<states.size(); i++)
	{
	roi o1;
	o1.weight(get<0>(states[i]));
	o1.h((get<1>(states[i]))[3]);
	o1.w((get<1>(states[i]))[2]);
	o1.x((get<1>(states[i]))[0]);
	o1.y((get<1>(states[i]))[1]);
	o_s.push_back(o1);
	}*/

	frame f;
	f.roi(r_s);
	f.no(fr_idx);

	f_s.push_back(f);
}

void serializeVideo(std::string outFilename, ::xsd::cxx::tree::sequence< ::frame > f_s, int alg) {
	try {

		video v;
		v.name(In_src.vidname);
		v.frame(f_s);
		if (alg==1)
			v.algorithm("alg1");
		if (alg==2)
			v.algorithm("alg2");
		if (alg==3)
			v.algorithm("alg3");

		XMLPlatformUtils::Initialize ();

		{
			// Choose a target.
			//
			auto_ptr<XMLFormatTarget> ft;		  

			ft = auto_ptr<XMLFormatTarget> (new LocalFileFormatTarget (outFilename.c_str()));

			xml_schema::namespace_infomap map;		  
			map[""].schema = "XML/video.xsd";
			video_ (*ft, v, map, "UTF-8");
		}

		XMLPlatformUtils::Terminate ();
	} catch (const xml_schema::exception& e) {
		std::cerr << e << std::endl;
	}
}

void deserializeVideo(std::string inFilename) {	
	std::auto_ptr<video> v(NULL);

	try {
		ifstream ifs (inFilename);
		auto_ptr<video> v = video_ (ifs);
		ifs.close ();		

		/*std::cout << "Video Name: " << v->name() << endl;
		std::cout << "Video Algorithm: " << v->algorithm() << endl;


		std::cout << "v->frame()[0].roi()[0].x(): " << v->frame()[0].roi()[0].x() << endl;

		for(video::frame_sequence::iterator f = v->frame().begin(); f != v->frame().end(); f++) {
		std::cout << "\tFrame No: " << f->no() << endl;

		for(frame::roi_sequence::iterator r = f->roi().begin(); r != f->roi().end(); r++) {
		std::cout << "\t\tRoi No: " << r->no() << endl;
		std::cout << "\t\tRoi x: " << r->x() << endl;
		std::cout << "\t\tRoi y: " << r->y() << endl;
		std::cout << "\t\tRoi w: " << r->w() << endl;
		std::cout << "\t\tRoi h: " << r->h() << endl;
		std::cout << "\t\tRoi weight: " << r->weight() << endl << endl;
		}
		}*/
	} catch (const xml_schema::exception& e) {
		std::cerr << e << std::endl;
	}
}

int main() 
{


	//XMLProcessing XML;

	video::frame_sequence F_S, F_S_A, F_S_B;

	frame::roi_sequence o_s, o_s_A, o_s_B;

	char Out[50], Out_A[50],Out_B[50];

	int src = In_src.menu();

	int	F=0;

	cv::VideoCapture oVideoInput;//,FmaskImage;

	cv::Mat oCurrInputFrame, oCurrSegmMask, oCurrReconstrBGImg, FmaskMat;

	int n_vid, cat_sel, vid_id, alg;

	bool flag_roi = false;		//true: Load roi, false: work with full frame
	bool flag_load = true;		//true: Load precomputed codebooks, false: Initialize from zero
	bool flag_saveROI = false;	//true: Save color, lbsp and stip for each roi, false: don't save
	bool seg = false; 
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
	//cv::namedWindow("input",cv::WINDOW_NORMAL);
	//cv::namedWindow("segmentation mask",cv::WINDOW_NORMAL);
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

			//create People detection object
			People_Features Pfeat_X(oCurrInputFrame);


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

			vector<vector<int>> Fstates;

			//loop through video frames
			if (seg)
			{
				
				while(In_src.fr_idx<In_src.endfr) 
				{
					F++;
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

					//Background Subtraction Visualization
					imshow("input",oCurrInputFrame);
					imshow("segmentation mask",oCurrSegmMask);
					
						//Get PROIs from all Fmask
						Pfeat_X.Get_PROIs(oCurrInputFrame, oCurrSegmMask);			
												
						o_s.clear();
						for(int i=0; i<Pfeat_X.PF_states.size(); i++)
						{
							roi o1;
							o1.weight(get<0>(Pfeat_X.PF_states[i]));
							o1.h((get<1>(Pfeat_X.PF_states[i]))[3]);
							o1.w((get<1>(Pfeat_X.PF_states[i]))[2]);
							o1.x((get<1>(Pfeat_X.PF_states[i]))[0]);
							o1.y((get<1>(Pfeat_X.PF_states[i]))[1]);
							o_s.push_back(o1);
						}

						o_s_A.clear();
						for(int i=0; i<Pfeat_X.PF_states_A.size(); i++)
						{
							roi o2;
							o2.weight(get<0>(Pfeat_X.PF_states_A[i]));
							o2.h((get<1>(Pfeat_X.PF_states_A[i]))[3]);
							o2.w((get<1>(Pfeat_X.PF_states_A[i]))[2]);
							o2.x((get<1>(Pfeat_X.PF_states_A[i]))[0]);
							o2.y((get<1>(Pfeat_X.PF_states_A[i]))[1]);
							o_s_A.push_back(o2);
						}

						o_s_B.clear();
						for(int i=0; i<Pfeat_X.PF_states_B.size(); i++)
						{
							roi o3;
							o3.weight(get<0>(Pfeat_X.PF_states_B[i]));
							o3.h((get<1>(Pfeat_X.PF_states_B[i]))[3]);
							o3.w((get<1>(Pfeat_X.PF_states_B[i]))[2]);
							o3.x((get<1>(Pfeat_X.PF_states_B[i]))[0]);
							o3.y((get<1>(Pfeat_X.PF_states_B[i]))[1]);
							o_s_B.push_back(o3);
						}


						SaveXMLInfo(F_S, o_s, F);
						SaveXMLInfo(F_S_A, o_s_A, F);
						SaveXMLInfo(F_S_B, o_s_B, F);
						cv::waitKey(1);
				}

				strcpy(Out,In_src.vidname);
				strcpy(Out_A,In_src.vidname);
				strcpy(Out_B,In_src.vidname);
				strcat(Out,"alg1.xml");
				strcat(Out_A,"alg2.xml");
				strcat(Out_B,"alg3.xml");

				alg=1;
				serializeVideo(Out,F_S,alg);
				alg=2;
				serializeVideo(Out_A,F_S_A,alg);
				alg=3;
				serializeVideo(Out_B,F_S_B,alg);
			}
			else
			{
				int fr = 0;
				char pathfmask[200], chfr[5];
				

				while(In_src.fr_idx<In_src.endfr) 
				{
					F++;
					//FmaskImage.open("C:\\Users\\carlos\\Dropbox\\survmantics00587\\F0%04d.png");
					//Fmask >> FmaskMat;
					if(src!=1)
					{
						oVideoInput >> oCurrInputFrame;						
					}
					else
					{
						oCurrInputFrame = In_src.ReadIm(In_src.fr_idx);
						In_src.fr_idx++;
						strcpy_s(pathfmask,In_src.imgpathf);
						itoa(fr,chfr,10);
						strcat_s(pathfmask,chfr);
						strcat_s(pathfmask,".png");
						FmaskMat = imread(pathfmask,CV_8UC1);
						fr++;
					}
					if(oCurrInputFrame.empty())
						break;

					vector<vector<int>> CV_det;
					Pfeat_X.SVM_ClassifyCV_Multi(oCurrInputFrame, CV_det);

					o_s.clear();
					for(int i=0; i<CV_det.size(); i++)
					{
						roi o1;
						o1.h(CV_det[i][3]);
						o1.w(CV_det[i][2]);
						o1.x(CV_det[i][0]);
						o1.y(CV_det[i][1]);
						o_s.push_back(o1);
					}

					//Get PROIs from  all Fmask

					/*clock_t ini = clock();
					Pfeat_X.Get_PROIs(oCurrInputFrame, FmaskMat);
					printf("tiempo de calculo: %f seg \n", ((double)clock() - ini)/CLOCKS_PER_SEC);*/
					
					/*
					o_s.clear();
					for(int i=0; i<Pfeat_X.PF_states.size(); i++)
					{
						roi o1;
						o1.weight(get<0>(Pfeat_X.PF_states[i]));
						o1.h((get<1>(Pfeat_X.PF_states[i]))[3]);
						o1.w((get<1>(Pfeat_X.PF_states[i]))[2]);
						o1.x((get<1>(Pfeat_X.PF_states[i]))[0]);
						o1.y((get<1>(Pfeat_X.PF_states[i]))[1]);
						o_s.push_back(o1);
					}

					o_s_A.clear();
					for(int i=0; i<Pfeat_X.PF_states_A.size(); i++)
					{
						roi o2;
						o2.weight(get<0>(Pfeat_X.PF_states_A[i]));
						o2.h((get<1>(Pfeat_X.PF_states_A[i]))[3]);
						o2.w((get<1>(Pfeat_X.PF_states_A[i]))[2]);
						o2.x((get<1>(Pfeat_X.PF_states_A[i]))[0]);
						o2.y((get<1>(Pfeat_X.PF_states_A[i]))[1]);
						o_s_A.push_back(o2);
					}

					o_s_B.clear();
					for(int i=0; i<Pfeat_X.PF_states_B.size(); i++)
					{
						roi o3;
						o3.weight(get<0>(Pfeat_X.PF_states_B[i]));
						o3.h((get<1>(Pfeat_X.PF_states_B[i]))[3]);
						o3.w((get<1>(Pfeat_X.PF_states_B[i]))[2]);
						o3.x((get<1>(Pfeat_X.PF_states_B[i]))[0]);
						o3.y((get<1>(Pfeat_X.PF_states_B[i]))[1]);
						o_s_B.push_back(o3);
					}
*/

					SaveXMLInfo(F_S, o_s, F);
					//SaveXMLInfo(F_S_A, o_s_A, F);
					//SaveXMLInfo(F_S_B, o_s_B, F);

					if(cv::waitKey(1)==27)
						break;
				}
				strcpy(Out,In_src.vidname);
				//strcpy(Out_A,In_src.vidname);
				//strcpy(Out_B,In_src.vidname);
				strcat(Out,"algCV.xml");
				//strcat(Out_A,"alg2.xml");
				//strcat(Out_B,"alg3.xml");

				alg=1;
				serializeVideo(Out,F_S,alg);
				/*alg=2;
				serializeVideo(Out_A,F_S_A,alg);
				alg=3;
				serializeVideo(Out_B,F_S_B,alg);*/

				

			}
			//Save subsense codebooks and parameters
			//oBGSAlg.saveVariables();
		}
	}
	return 0;
}

