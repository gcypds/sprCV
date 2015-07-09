// BackgroundSubtracion includes
#include "BackgroundRegions.h"
#include "BackgroundSubtractorSuBSENSE.h"

// OpenCV includes
#include <cv.h>
#include <highgui.h>

// Standard includes
#include <stdlib.h>
#include <string>
#include <iostream>
#include <sstream>
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
#include "XML/video.hxx"

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

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
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
		cout << "Removing " << filename.c_str() << endl;
		// delete the original file
		//std::remove(filename.c_str());	
		boost::filesystem::remove(path(filename));

		cout << "REMOVED! " << filename.c_str() << endl;

		cout << "Renaming to " << filename.c_str() << endl;
		// rename old to new
		//std::rename("outfile.xml",filename.c_str());
		boost::filesystem::rename(path("outfile.xml"), path(filename));

		cout << "RENAMED! " << filename.c_str() << endl;
	} catch (const filesystem_error& ex) {
		cout << currentDateTime()  << ex.what() << '\n';
	}

    return 0;
}

void serializeVideo(string outFilename) {
	try {

		frame::object_sequence o_s;

		object o1;
		o1.fish_species("Dascyllus Aruanus");
		o1.h(33);
		o1.w(34);
		o1.x(35);
		o1.y(36);
		o_s.push_back(o1);

		object o2;
		o2.fish_species("Plectrogly-Phidodon Dickii");
		o2.h(43);
		o2.w(44);
		o2.x(45);
		o2.y(46);
		o_s.push_back(o2);

		frame f;
		f.object(o_s);
		f.id(1);
	
		video::frame_sequence f_s;
		f_s.push_back(f);
		video v;
		v.id("0b21f0579d247c855e05405d3ed805c1#201205251240");
		v.frame(f_s);
		v.location("NXB4");
		v.camera(4);
		
		XMLPlatformUtils::Initialize ();

		{
		  // Choose a target.
		  //
		  auto_ptr<XMLFormatTarget> ft;

		  //ft = auto_ptr<XMLFormatTarget> (new StdOutFormatTarget ());
		  
		  ft = auto_ptr<XMLFormatTarget> (new LocalFileFormatTarget (outFilename.c_str()));

		  xml_schema::namespace_infomap map;

		  //map[""].schema = "xml/minion.xsd";  

		  // Write it out.
		  //
		  video_ (*ft, v, map, "UTF-8", xml_schema::flags::no_xml_declaration);
		}

		XMLPlatformUtils::Terminate ();
	} catch (const xml_schema::exception& e) {
	  std::cerr << e << std::endl;
	  return;
	}
}

int main(int argc, char *argv[])
{
	if (argc < 5)
	{
		cout << currentDateTime()  << "Usage: GTTagging.exe video_path flag_roi(1=true, 0=false) flag_load(1=true, 0=false) flag_saveROI(1=true, 0=false)\n";
		return 1;
	}

	/*serialize_video();*/
	string outFilename = "output_video.xml";
	serializeVideo(outFilename);
	removeTrailingSpace(outFilename);

	path video_path (argv[1]);

	string video_name = video_path.filename().replace_extension("").string();

	cout << "video_path filename: " << video_name << endl;

	string base_name = video_path.filename().replace_extension("").string();
	cout << "base_name: " << base_name << endl;

	string base_path = video_path.remove_filename().string();
	cout << "base_path: " << base_path << endl;	

	ostringstream codebook_paramters_path;

	codebook_paramters_path << base_path << "\\" << base_name << "_cparameters.dat";
	cout << "cparameters filename: " <<  codebook_paramters_path.str() << endl;	

	ostringstream xml_path;

	xml_path << base_path << "\\" << base_name << ".xml";
	cout << "xml filename: " <<  xml_path.str() << endl;

	path gt_xml_path (xml_path.str());
	
	try {
		
		XMLPlatformUtils::Initialize();	
		XQillaPlatformUtils::initialize();

	} catch (const XMLException& eXerces) {
		cerr << "Error during Xerces-C initialisation.\n"
			<< "Xerces exception message: "
			<< UTF8(eXerces.getMessage()) << endl;
		return 1;
	}

	try
	{
		if (!exists(gt_xml_path))
		{
			cout << currentDateTime()  << "File: " << gt_xml_path << " doesn't exist!" << endl;
			return 1;
		}

		if (!exists(video_path))
		{
			cout << currentDateTime()  << "File: " << video_path << " doesn't exist!" << endl;
			return 1;
		}		

		cout << currentDateTime()  << "Processing " << video_path << " ..." << endl;

		DOMImplementation* xqillaImplementation = DOMImplementationRegistry::getDOMImplementation(X("XPath2 3.0"));

		DOMLSParser* xmlParser = xqillaImplementation->createLSParser(DOMImplementationLS::MODE_SYNCHRONOUS, 0);

		DOMConfiguration* dc_parser = xmlParser->getDomConfig();

		dc_parser->setParameter(XMLUni::fgDOMNamespaces, true);
		dc_parser->setParameter(XMLUni::fgXercesSchema, true);
		dc_parser->setParameter(XMLUni::fgDOMValidate, true);

		DOMDocument* document = xmlParser->parseURI(xml_path.str().c_str());

		if(document == 0) {
			cerr << "Document not found: " << xml_path.str().c_str() << endl;
			return 1;
		}

		const DOMXPathNSResolver* resolver = document->createNSResolver(document->getDocumentElement());

		XQillaNSResolver* xqillaResolver = (XQillaNSResolver*)resolver;
		xqillaResolver->addNamespaceBinding(X("xs"), X("http://www.w3.org/2001/XMLSchema"));	
		xqillaResolver->addNamespaceBinding(X("fn"), X("http://www.w3.org/2005/xpath-functions"));			

		// Initialize Subsense
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
			cout << currentDateTime()  << "Pre-loading codebook ..." << endl;
		} else {
			oBGSAlg.loadvars = false;
			cout << currentDateTime()  << "Building codebook ..." << endl;
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

		oBGSAlg.saveCodebookParametersPath(codebook_paramters_path.str());
		oBGSAlg.initialize(oCurrInputFrame, R);

		// create visualization windows
		/*cv::namedWindow("input",cv::WINDOW_AUTOSIZE);
		cv::namedWindow("segmentation mask",cv::WINDOW_AUTOSIZE);*/

		int xml_roi_counter = 0;
		int bgs_roi_counter = 0;
		
		int frame_counter = 0;
		//loop through video frames
		while(1) {							
			try {
				std::cout << "frame counter = " << frame_counter << std::endl;
				oVideoInput >> oCurrInputFrame;

				if(oCurrInputFrame.empty())
					break;

				//Process
				oBGSAlg(oCurrInputFrame,oCurrSegmMask);
				//oBGSAlg.getBackgroundImage(oCurrReconstrBGImg);
				std::vector<F_state_struct> F_states;

				// TODO Add function to static lib.
				getRegions(oCurrSegmMask, F_states, pminArea);

				std::ostringstream object_xpath_expression;
				object_xpath_expression << "//object[../@id=\""<< frame_counter++ << "\"]";
				cout << currentDateTime()  << "object_xpath_expression: " << object_xpath_expression.str() << endl;

				const DOMXPathExpression* parsedExpression = document->createExpression(X(object_xpath_expression.str().c_str()), resolver);

				DOMXPathResult* iteratorResult = (DOMXPathResult*)parsedExpression->evaluate(document->getDocumentElement(), DOMXPathResult::ITERATOR_RESULT_TYPE, 0);

				/*ostringstream xml_rois_path;

				xml_rois_path << base_path << "\\" << base_name << "_xml_rois";
				cout << "xml_rois_path filename: " <<  xml_rois_path.str() << endl;

				path xml_rois_dir (xml_rois_path.str());				

				if(!exists(xml_rois_dir)) {
					if (create_directories(xml_rois_dir)) {
						cout << currentDateTime()  << "xml_rois_dir created!" << endl;												
					} else {
						cout << currentDateTime()  << "Cannot create xml_rois_dir!" << endl;
						return 1;
					}
				}*/

				ostringstream bgs_rois_path;

				bgs_rois_path << base_path << "\\" << base_name << "_bgs_rois";
				cout << "bgs_rois_path filename: " <<  bgs_rois_path.str() << endl;

				path bgs_rois_dir (bgs_rois_path.str());

				if(!exists(bgs_rois_dir)) {
					if (create_directories(bgs_rois_dir)) {
						cout << currentDateTime()  << "bgs_rois_dir created!" << endl;												
					} else {
						cout << currentDateTime()  << "Cannot create bgs_rois_dir!" << endl;
						return 1;
					}
				}

				int i = 0;

				while(iteratorResult->iterateNext()) {
					if(iteratorResult->isNode()) {
						DOMNode* n (iteratorResult->getNodeValue ());

						char * localName = XMLString::transcode(n->getLocalName());

						//cout << currentDateTime()  << "Localname: " << localName << endl;

						DOMElement* resultElement = dynamic_cast<DOMElement*>(n);

						if(strcmp( localName, "object") == 0) {

							object* o = new object (*resultElement);
							
							int w = o->w().get();
							int h = o->h().get();
							int x = o->x().get();
							int y = o->y().get();
							
							cout << currentDateTime()  << "fish_species: " << o->fish_species().get() << endl;
							cout << currentDateTime()  << "h: " << h << endl;
							cout << currentDateTime()  << "w: " << w << endl;
							cout << currentDateTime()  << "x: " << x << endl;
							cout << currentDateTime()  << "y: " << y << endl;

							try
							{
								ostringstream fish_specie_path;

								fish_specie_path << base_path << "\\" << o->fish_species().get();
								cout << "fish_specie_path filename: " <<  fish_specie_path.str() << endl;

								path fish_specie_dir (fish_specie_path.str());

								if(!exists(fish_specie_dir)) {
									if (create_directories(fish_specie_dir)) {
										cout << currentDateTime()  << "bgs_rois_dir created!" << endl;												
									} else {
										cout << currentDateTime()  << "Cannot create fish_specie_dir!" << endl;
										return 1;
									}
								}

								std::cout << "XML ROI: " << x << " , " << y << " , " << x+w << " , " << y+h << std::endl;
								Rect xml_roi_rectangle = Rect(Point(x, y), Point(x+w, y+h));

								Mat xmlROI, xmlROI_bg;								
								Mat xmlROIMask = oCurrSegmMask(xml_roi_rectangle);

								xml_roi_counter++;
								oCurrInputFrame(xml_roi_rectangle).copyTo(xmlROI_bg);
								sprintf(printf_buffer, "%s\\%s_roi%04d_bg.png", fish_specie_path.str().c_str(), base_name, xml_roi_counter);

								cout << currentDateTime()  << "Saving to: " << printf_buffer << endl;
								imwrite(printf_buffer, xmlROI_bg);
								cout << currentDateTime()  << "Saved..." << endl;

								oCurrInputFrame(xml_roi_rectangle).copyTo(xmlROI, xmlROIMask);
								sprintf(printf_buffer, "%s\\%s_roi%04d.png\0", fish_specie_path.str().c_str(), base_name, xml_roi_counter);

								cout << currentDateTime()  << "Saving to: " << printf_buffer << endl;
								imwrite(printf_buffer, xmlROI);
								cout << currentDateTime()  << "Saved..." << endl;

								//rectangle( oCurrInputFrame, Point(x, y), Point(x+w, y+h), Scalar( 255, 9, 0 ), +3, 4 );
							}
							catch (exception& e)
							{
								cout << "Error: " <<  e.what() << '\n';
							}
						}
					}
				}

				for(int i=0; i<F_states.size(); i++) {
					std::cout << "BGS ROI: " << F_states.at(i).min_x << " , " << F_states.at(i).min_y << " , " << F_states.at(i).max_x << " , " << F_states.at(i).max_y << std::endl;
					Rect bgs_roi_rectangle = Rect(Point(F_states.at(i).min_x, F_states.at(i).min_y), Point(F_states.at(i).max_x, F_states.at(i).max_y));
			
					Mat bgsROI, bgsROI_bg;
					Mat bgsROIMask = oCurrSegmMask(bgs_roi_rectangle);
			
					bgs_roi_counter++;
					
					oCurrInputFrame(bgs_roi_rectangle).copyTo(bgsROI_bg);
					sprintf(printf_buffer, "%s\\roi%04d_bg.png", bgs_rois_path.str().c_str(), bgs_roi_counter);

					cout << currentDateTime()  << "Saving to: " << printf_buffer << endl;
					imwrite(printf_buffer, bgsROI_bg);
					cout << currentDateTime()  << "Saved..." << endl;

					oCurrInputFrame(bgs_roi_rectangle).copyTo(bgsROI, bgsROIMask);
					sprintf(printf_buffer, "%s\\roi%04d.png\0", bgs_rois_path.str().c_str(), bgs_roi_counter);

					cout << currentDateTime()  << "Saving to: " << printf_buffer << endl;
					imwrite(printf_buffer, bgsROI);
					cout << currentDateTime()  << "Saved..." << endl;

					//rectangle( oCurrInputFrame, Point(F_states.at(i).min_x, F_states.at(i).min_y), Point(F_states.at(i).max_x, F_states.at(i).max_y), Scalar( 0, 55, 255 ), +3, 4 );
				}

				//Background Subtraction Visualization
				/*imshow("input",oCurrInputFrame);
				imshow("segmentation mask",oCurrSegmMask);*/
				//imshow("reconstructed background",oCurrReconstrBGImg);								
				
				/*if(cv::waitKey(1)==27)
					break;*/

				//Save subsense codebooks and parameters
				if (strcmp(argv[4], "1") == 0) {
					oBGSAlg.saveVariables();
				}
			} catch(DOMXPathException &e) {
				cerr << "DOMXPathException: " << UTF8(e.msg) << endl;
				return 1;
			} catch(DOMException &e) {
				cerr << "DOMException: " << UTF8(e.getMessage()) << endl;
				return 1;
			}						
		}		
	} catch (const filesystem_error& ex) {
		cout << currentDateTime()  << ex.what() << '\n';
	}

	XQillaPlatformUtils::terminate();
	XMLPlatformUtils::Terminate();

	return 0;
}
