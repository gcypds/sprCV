/*
	HAGTA: Human Activities Ground Truth Analyzer.
	This software takes a path containing frames of a video, then plots reference ground truth and optionally test ground truth.
	Software preprocesses information in the following way:

	1. Get number of frames from ground truth and validate that number of frames in path is the same.
	2. Read each frame in path and according to frame id, search using XPath on XML.
	3. If there is a result from XPath query, paint a bounding box from XML info.
	4. If theres is a ground truth test file, search with XPath and do steps 2 and 3.

	Prerequisites:
	-> This software processes a ground truth for a frame sequence. The frames must be on a directory, enumerated with the name of the frame
	and without requiring any mask for the filename, natural sorting will be used as the order mecanism to process frames. All images must 
	be with the same image format and file extension.

	-> Requires:
		* OpenCV.
		* Boost.
		* XSD Codesynthesis.
		* Xerces.
		* XQilla.
*/

// OpenCV includes
#include <cv.h>
#include <highgui.h>

// Standard includes
#include <stdlib.h>
#include <string>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>

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

// XQilla includes
#include <xqilla/xqilla-dom3.hpp>

// XSD Codesynthesis generated stubs includes
#include "xml/video.hxx"

// Util includes
#include "util.h"

using namespace cv;
using namespace std;
using namespace boost::filesystem;
using namespace xercesc;
XERCES_CPP_NAMESPACE_USE			

using std::cerr;

#define MAX_XPATH_EXPRESSION_SIZE 300
#define MAX_FRAME_INDEX_FILENAME_SIZE 10
#define MAX_FRAME_NUMBER_STRING_SIZE 10
#define MAX_ROI_LABEL_STRING_SIZE 100
#define MAX_ROI_FRAME_FILENAME_SIZE 10

char frame_index_filename[MAX_FRAME_INDEX_FILENAME_SIZE] = {'\0'};
char frame_expression[MAX_XPATH_EXPRESSION_SIZE] = {'\0'};
char frame_number_string[MAX_FRAME_NUMBER_STRING_SIZE] = {'\0'};
char roi_label_string[MAX_ROI_LABEL_STRING_SIZE] = {'\0'};
char roi_frame_filename[MAX_ROI_FRAME_FILENAME_SIZE] = {'\0'};

void set_labels(cv::Mat& im, cv::Rect r, std::string label_top, std::string label_middle, std::string label_bottom, int roi_counter, Scalar color)
{
    int fontface = cv::FONT_HERSHEY_SIMPLEX;
    double scale = 0.5;
    int thickness = 2;
    int baseline = 0;
	int top_offset = 10;
	int bottom_offset = 10;

	cv::Size text_top = cv::getTextSize(label_middle, fontface, scale, thickness, &baseline);
    cv::Point pt_top(r.x + (r.width-text_top.width)/2, r.y - top_offset);    
    cv::putText(im, label_top, pt_top, fontface, scale, color, thickness, 8);

	cv::Size text_middle = cv::getTextSize(label_middle, fontface, scale, thickness, &baseline);
    cv::Point pt_middle(r.x + (r.width-text_middle.width)/2, r.y + (r.height+text_middle.height)/2);    
    cv::putText(im, label_middle, pt_middle, fontface, scale, color, thickness, 8);

	cv::Size text_bottom = cv::getTextSize(label_middle, fontface, scale, thickness, &baseline);
    cv::Point pt_bottom(r.x + (r.width-text_bottom.width)/2, r.y + (r.height - bottom_offset));    
    cv::putText(im, label_bottom, pt_bottom, fontface, scale, color, thickness, 8);
}

void showDetections(const vector<Point>& found, Mat& imageData) {
    size_t i, j;
    for (i = 0; i < found.size(); ++i) {
        Point r = found[i];
        // Rect_(_Tp _x, _Tp _y, _Tp _width, _Tp _height);
        rectangle(imageData, Rect(r.x+16, r.y+16, 32, 80), Scalar(64, 255, 64), 3);
    }
	imshow("Detections:",imageData);
	//waitKey();
}

int main(int argc, char *argv[])
{
	Mat frame;
    namedWindow("video", 1);
	VideoCapture cap("http://192.168.46.170/videostream.cgi?user=admin&pwd=&resolution=8&rate=0.mjpeg");
    while ( cap.isOpened() )
    {
        cap >> frame;
        if(frame.empty()) break;

		HOGDescriptor hog;
		vector<float> HOG_d = HOGDescriptor::getDefaultPeopleDetector();
		hog.setSVMDetector(HOG_d);

		vector<Point> found;

		bool label = false;

		hog.detect(frame, found, 0, Size(8,8), Size(0,0));

		if(found.size()>0)
			label = true;

		showDetections(found, frame);		

        imshow("video", frame);
        if(waitKey(30) >= 0) break;
    }

	return 0;
}