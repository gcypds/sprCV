///*
//	HAGTA: Human Activities Ground Truth Analyzer.
//	This software takes a path containing frames of a video, then plots reference ground truth and optionally test ground truth.
//	Software preprocesses information in the following way:
//
//	1. Get number of frames from ground truth and validate that number of frames in path is the same.
//	2. Read each frame in path and according to frame id, search using XPath on XML.
//	3. If there is a result from XPath query, paint a bounding box from XML info.
//	4. If theres is a ground truth test file, search with XPath and do steps 2 and 3.
//
//	Prerequisites:
//	-> This software processes a ground truth for a frame sequence. The frames must be on a directory, enumerated with the name of the frame
//	and without requiring any mask for the filename, natural sorting will be used as the order mecanism to process frames. All images must 
//	be with the same image format and file extension.
//
//	-> Requires:
//		* OpenCV.
//		* Boost.
//		* XSD Codesynthesis.
//		* Xerces.
//		* XQilla.
//*/
//
//// OpenCV includes
//#include <cv.h>
//#include <highgui.h>
//
//// Standard includes
//#include <stdlib.h>
//#include <string>
//#include <iostream>
//#include <iterator>
//#include <algorithm>
//#include <vector>
//
//// Boost includes
//#include <boost/filesystem.hpp>
//#include <boost/algorithm/string.hpp>
//
//// Xerces includes
//#include <xercesc/dom/DOM.hpp>
//#include <xercesc/dom/DOMDocument.hpp>
//#include <xercesc/dom/DOMDocumentType.hpp>
//#include <xercesc/dom/DOMElement.hpp>
//#include <xercesc/dom/DOMImplementation.hpp>
//#include <xercesc/dom/DOMImplementationLS.hpp>
//#include <xercesc/dom/DOMNodeIterator.hpp>
//#include <xercesc/dom/DOMNodeList.hpp>
//#include <xercesc/dom/DOMText.hpp>
//#include <xercesc/parsers/XercesDOMParser.hpp>
//#include <xercesc/util/XMLUni.hpp>
//#include <xercesc/util/XMLString.hpp>
//#include <xercesc/sax/HandlerBase.hpp>
//#include <xercesc/util/PlatformUtils.hpp>
//#include <xercesc/framework/StdOutFormatTarget.hpp>
//#include <xercesc/framework/LocalFileFormatTarget.hpp>
//
//// XQilla includes
//#include <xqilla/xqilla-dom3.hpp>
//
//// XSD Codesynthesis generated stubs includes
//#include "xml/group.hxx"
//#include "xml/frame_info.hxx"
//#include "xml/minion.hxx"
//
//// Xalan includes
//#include <xalanc/Include/PlatformDefinitions.hpp>
//#include <xalanc/XalanTransformer/XalanTransformer.hpp>
//#include <xalanc/XSLT/XSLTInputSource.hpp>
//#include <xalanc/XSLT/XSLTResultTarget.hpp>
//#include <xalanc/XalanDOM/XalanDOMString.hpp>
//
//// STIP includes
//#include "stip/ActionHOGLibs.h"
//
//// Util includes
//#include "util.h"
//
//using namespace cv;
//using namespace std;
//using namespace boost::filesystem;
//using namespace xercesc;
//XERCES_CPP_NAMESPACE_USE
//			
//XALAN_USING_XERCES(XMLPlatformUtils)
//XALAN_USING_XALAN(XalanTransformer)
//XALAN_USING_XALAN(XSLTInputSource)
//XALAN_USING_XALAN(XSLTResultTarget)
//XALAN_USING_XALAN(XalanDOMString)
//
//using std::cerr;
//
//#define MAX_XPATH_EXPRESSION_SIZE 300
//#define MAX_FRAME_INDEX_FILENAME_SIZE 10
//#define MAX_FRAME_NUMBER_STRING_SIZE 10
//#define MAX_ROI_LABEL_STRING_SIZE 100
//#define MAX_ROI_FRAME_FILENAME_SIZE 10
//
//const string frame_mask = "image%05d.png";
//const string frame_extension = ".png";
//char frame_index_filename[MAX_FRAME_INDEX_FILENAME_SIZE] = {'\0'};
//char frame_expression[MAX_XPATH_EXPRESSION_SIZE] = {'\0'};
//char frame_number_string[MAX_FRAME_NUMBER_STRING_SIZE] = {'\0'};
//char roi_label_string[MAX_ROI_LABEL_STRING_SIZE] = {'\0'};
//char roi_frame_filename[MAX_ROI_FRAME_FILENAME_SIZE] = {'\0'};
//
//int extract_frame_number_from_mask(string filename)
//{	
//	string frame_number = filename.substr(5, 5);
//	return atoi(frame_number.c_str());
//}
//
//void set_labels(cv::Mat& im, cv::Rect r, std::string label_top, std::string label_middle, std::string label_bottom, int roi_counter, Scalar color)
//{
//    int fontface = cv::FONT_HERSHEY_SIMPLEX;
//    double scale = 0.5;
//    int thickness = 2;
//    int baseline = 0;
//	int top_offset = 10;
//	int bottom_offset = 10;
//
//	cv::Size text_top = cv::getTextSize(label_middle, fontface, scale, thickness, &baseline);
//    cv::Point pt_top(r.x + (r.width-text_top.width)/2, r.y - top_offset);    
//    cv::putText(im, label_top, pt_top, fontface, scale, color, thickness, 8);
//
//	cv::Size text_middle = cv::getTextSize(label_middle, fontface, scale, thickness, &baseline);
//    cv::Point pt_middle(r.x + (r.width-text_middle.width)/2, r.y + (r.height+text_middle.height)/2);    
//    cv::putText(im, label_middle, pt_middle, fontface, scale, color, thickness, 8);
//
//	cv::Size text_bottom = cv::getTextSize(label_middle, fontface, scale, thickness, &baseline);
//    cv::Point pt_bottom(r.x + (r.width-text_bottom.width)/2, r.y + (r.height - bottom_offset));    
//    cv::putText(im, label_bottom, pt_bottom, fontface, scale, color, thickness, 8);
//}
//
//void compute_STIP(string video_path, string stip_output_file, string frame_index_dir, int person_id, int first_frame_index_number) {
//	// setup default parameters 
//	string detName("SURF"); 		// interest point detector (no dominant orientations)
//	string featChan("IMG_MHI_OPT"); // feature channels
//	bool vis = false; 				// no visualization
//
//	int imgNGrids = 3; // number of grids in x and y for image channel
//	int imgNBins = 8;  // number of bins to quantize gradient orientation for image channel
//
//	int mhiNGrids = 3; // number of grids in x and y for MHI channel
//	int mhiNBins = 8;  // number of bins to quantize gradient orientation for MHI channel
//	
//	int optNGrids = 3; // number of grids in x and y for optical flow channel
//	int optNBins = 8;  // number of bins to quantize gradient orientation for optical flow channel
//
//	//initialize ActionHOG
//	ActionHOG feat( detName, featChan,
//				    imgNGrids, imgNBins,
//				    mhiNGrids, mhiNBins,
//				    optNGrids, optNBins,
//				    vis, frame_index_dir, person_id, first_frame_index_number);
//
//	// check video file and feature file	
//	feat.check(video_path, stip_output_file);
//
//	// compute ActionHOG
//	feat.comp();
//}
//
//void serialize_group() {
//	std::istringstream agent_smith(
//	  "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>"
//	  "<minion xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
//	  "xsi:noNamespaceSchemaLocation=\"xml/minion.xsd\" loyalty=\"0.2\">"
//	  "<name>Agent Smith</name>"
//	  "<rank>Member of Minion Staff</rank>"
//	  "<serial>2</serial>"
//	  "</minion>");
//
//	std::auto_ptr<minion> m(NULL);
//
//	try {
//	  m = minion_(agent_smith);
//	} catch (const xml_schema::exception& e) {
//	  std::cerr << e << std::endl;
//	  return;
//	}
//
//	std::cout << "Name: " << m->name() << std::endl
//          << "Loyalty: " << m->loyalty() << std::endl
//          << "Rank: " << m->rank() << std::endl
//          << "Serial number: " << m->serial() << std::endl;
//
//	minion m2("Salacious Crumb", "Senior Lackey", 1, 0.9);	
//	minion_(std::cout, m2);
//	
//	XMLPlatformUtils::Initialize ();
//
//	{
//	  // Choose a target.
//	  //
//	  auto_ptr<XMLFormatTarget> ft;
//
//	  //ft = auto_ptr<XMLFormatTarget> (new StdOutFormatTarget ());
//
//	  ft = auto_ptr<XMLFormatTarget> (new LocalFileFormatTarget ("output_minion.xml"));
//
//	  xml_schema::namespace_infomap map;
//
//	  //map[""].schema = "xml/minion.xsd";  
//
//	  // Write it out.
//	  //
//	  minion_ (*ft, m2, map, "UTF-8", xml_schema::flags::no_xml_declaration | xml_schema::flags::dont_pretty_print);
//
//	  ostringstream ss;
//	  string s;
//	  const string& tmp (ss.str ());
//
//		if (!tmp.empty () && tmp[0] == '\n')
//		  s.assign (tmp, 1, tmp.size () - 1);
//		else
//		  s = tmp;
//	}
//
//	XMLPlatformUtils::Terminate ();
//
//}
//
//bool removeTrailingSpace(string filename) {
//	string line;
//    // open input file
//    ifstream in(filename);
//    if( !in.is_open())
//    {
//          cout << "Input file failed to open\n";
//          return false;
//    }
//    
//	// Remove trailing space
//	getline(in,line);
//	ofstream out("outfile.xml");
//
//    while( getline(in,line) )
//    {
//         out << line << "\n";
//		 cout << line << endl;
//    }	
//    in.close();
//	out.close();
//
//	try {
//		cout << "Removing " << filename.c_str() << endl;
//		// delete the original file
//		//std::remove(filename.c_str());	
//		boost::filesystem::remove(path(filename));
//
//		cout << "REMOVED! " << filename.c_str() << endl;
//
//		cout << "Renaming to " << filename.c_str() << endl;
//		// rename old to new
//		//std::rename("outfile.xml",filename.c_str());
//		boost::filesystem::rename(path("outfile.xml"), path(filename));
//
//		cout << "RENAMED! " << filename.c_str() << endl;
//	} catch (const filesystem_error& ex) {
//		cout << currentDateTime()  << ex.what() << '\n';
//	}
//
//    return 0;
//}
//
//void serializeGroup(string outFilename) {
//	//ifstream is;
//
//	//is.open("xml/group.xml", ios::in);
//
//	//if(is.is_open()) {
//	//	cout << "Is open!"  << endl;
//	//} else {
//	//	cout << "Is closed!"  << endl;
//	//}
//
//	///*std::istringstream agent_smith(
//	//  "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>"
//	//  "<minion xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
//	//  "xsi:noNamespaceSchemaLocation=\"xml/minion.xsd\" loyalty=\"0.2\">"
//	//  "<name>Agent Smith</name>"
//	//  "<rank>Member of Minion Staff</rank>"
//	//  "<serial>2</serial>"
//	//  "</minion>");*/
//
//	//std::auto_ptr<group> g(NULL);
//
//	//try {
//	//  //g = group_(agent_smith);
//	//	g = group_(is);
//	//} catch (const xml_schema::exception& e) {
//	//  std::cerr << e << std::endl;
//	//  return;
//	//}
//
//	//std::cout << "Group Description: " << g->description() << std::endl;	
//
//	//for (group::behaviour_sequence::iterator b = g->behaviour().begin() ; b != g->behaviour().end(); ++b) {
//	//	std::cout << "Behaviour Type: " << b->type();
//
//	//	for (behaviour::person_sequence::iterator p = b->person().begin() ; p != b->person().end(); ++p) {
//	//		std::cout << "Person Id: " << p->id();
//	//		std::cout << "Person Description: " << p->description();
//
//	//		for (person::profile_sequence::iterator pf = p->profile().begin() ; pf != p->profile().end(); ++pf) {
//	//			std::cout << "Profile Type: " << pf->type();
//
//	//			for (profile::frame_sequence::iterator f = pf->frame().begin() ; f != pf->frame().end(); ++f) {
//	//				std::cout << "Frame No: " << f->no();					
//
//	//				std::cout << "Frame BB ulx: " << f->bb().ul_x();
//	//				std::cout << "Frame BB uly: " << f->bb().ul_y();
//	//				std::cout << "Frame BB lrx: " << f->bb().lr_x();
//	//				std::cout << "Frame BB lry: " << f->bb().lr_y();
//	//				std::cout << "Frame BB or: " << f->bb().or_();
//	//				std::cout << "Frame BB gp_x: " << f->bb().gp_x();
//	//				std::cout << "Frame BB gp_y: " << f->bb().gp_y();
//	//				std::cout << "Frame BB gp_or: " << f->bb().gp_or();
//	//			}
//	//		}
//	//	}
//	//}
//
//	try {
//		bb bb;
//		bb.ul_x(300);
//		bb.ul_y(400);
//		bb.lr_x(500);
//		bb.lr_y(600);
//		bb.or_(1.1);
//		bb.gp_x(9.9);
//		bb.gp_y(4.4);
//		bb.gp_or(6.6);
//
//		frame f(bb);
//		f.no(33);
//
//		profile::frame_sequence f_s;
//		f_s.push_back(f);	
//		profile pf;
//		pf.frame(f_s);
//		pf.type("DISTRACTED");
//
//		person::profile_sequence pf_s;
//		pf_s.push_back(pf);
//		person p;
//		p.profile(pf_s);	
//		p.id(44);
//		p.description("test person");
//
//		behaviour::person_sequence p_s;
//		p_s.push_back(p);
//		behaviour b;
//		b.person(p_s);
//		b.type("CHATTING");
//	
//		group::behaviour_sequence b_s;
//		b_s.push_back(b);
//		group g;
//		g.behaviour(b_s);
//		
//		XMLPlatformUtils::Initialize ();
//
//		{
//		  // Choose a target.
//		  //
//		  auto_ptr<XMLFormatTarget> ft;
//
//		  //ft = auto_ptr<XMLFormatTarget> (new StdOutFormatTarget ());
//		  
//		  ft = auto_ptr<XMLFormatTarget> (new LocalFileFormatTarget (outFilename.c_str()));
//
//		  xml_schema::namespace_infomap map;
//
//		  //map[""].schema = "xml/minion.xsd";  
//
//		  // Write it out.
//		  //
//		  group_ (*ft, g, map, "UTF-8", xml_schema::flags::no_xml_declaration);
//		}
//
//		XMLPlatformUtils::Terminate ();
//	} catch (const xml_schema::exception& e) {
//	  std::cerr << e << std::endl;
//	  return;
//	}
//}
//
//int main(int argc, char *argv[])
//{
//	if (argc < 8)
//	{
//		cout << currentDateTime()  << "Usage: humanActivitiesGTA.exe gt_xml_path frames_path generate_index_flag(1=true, 0=false, default=0) draw_frames_flag(1=true, 0=false, default=1) generate_roi_files(1=true, 0=false, default=0) generate_roi_videos(1=true, 0=false, default=0) compute_stip_videos(1=true, 0=false, default=0)\n";
//		return 1;
//	}
//
//	//serialize_group();
//	string outFilename = "output_group.xml";
//	serializeGroup(outFilename);
//	removeTrailingSpace(outFilename);
//	
//	path gt_xml_path (argv[1]);
//	path frames_path (argv[2]);
//
//	try {
//		
//		XMLPlatformUtils::Initialize();
//		XalanTransformer::initialize();
//		XQillaPlatformUtils::initialize();
//
//	} catch (const XMLException& eXerces) {
//		cerr << "Error during Xerces-C initialisation.\n"
//			<< "Xerces exception message: "
//			<< UTF8(eXerces.getMessage()) << endl;
//		return 1;
//	}
//
//	try
//	{
//		if (!exists(gt_xml_path))
//		{
//			cout << currentDateTime()  << "File: " << gt_xml_path << " doesn't exist!" << endl;
//			return 1;
//		}
//
//		if (!exists(frames_path))
//		{
//			cout << currentDateTime()  << "File: " << frames_path << " doesn't exist!" << endl;
//			return 1;
//		}
//
//		if (!is_regular_file(gt_xml_path)) {
//			cout << currentDateTime()  << "gt_xml_path is NOT a regular file!" << endl;
//			return 1;
//		} else {
//			//cout << currentDateTime()  << "gt_xml_path " << " size is " << file_size(gt_xml_path) << endl;
//		}		
//
//		if (!is_directory(frames_path)) {
//			cout << currentDateTime()  << "frames_path is NOT a directory!" << endl;
//			return 1;
//		} else {
//
//			cout << currentDateTime()  << "Processing " << frames_path << " ..." << endl;
//
//			DOMImplementation* xqillaImplementation = DOMImplementationRegistry::getDOMImplementation(X("XPath2 3.0"));
//
//			DOMLSParser* xmlParser = xqillaImplementation->createLSParser(DOMImplementationLS::MODE_SYNCHRONOUS, 0);
//
//			DOMConfiguration* dc_parser = xmlParser->getDomConfig();
//
//			dc_parser->setParameter(XMLUni::fgDOMNamespaces, true);
//			dc_parser->setParameter(XMLUni::fgXercesSchema, true);
//			dc_parser->setParameter(XMLUni::fgDOMValidate, true);
//
//			DOMDocument* document = xmlParser->parseURI(argv[1]);
//
//			if(document == 0) {
//				cerr << "Document not found: " << argv[1] << endl;
//				return 1;
//			}    
//
//			//const DOMXPathNSResolver* resolver = document->createNSResolver(document->getDocumentElement());
//			//XQillaNSResolver* xqillaResolver = (XQillaNSResolver*)resolver;   
//			//xqillaResolver->addNamespaceBinding(X("xs"), X("http://www.w3.org/2001/XMLSchema"));		
//			//xqillaResolver->addNamespaceBinding(X("fn"), X("http://www.w3.org/2005/xpath-functions"));
//
//			// 3. Create a XalanTransformer
//			XalanTransformer theXalanTransformer;
//			
//			// 4. Prepare the input and output sources
//			XSLTInputSource xmlIn(argv[1]);
//			XSLTInputSource xslIn("frame_info.xsl");			
//
//			//cout << currentDateTime()  << frames_path << " is a directory containing:\n";
//
//			typedef vector<path> vec;
//			vec v;
//
//			copy(directory_iterator(frames_path), directory_iterator(), back_inserter(v));			
//
//			if (strcmp(argv[3], "1") == 0) {
//			
//				std::ostringstream frame_index_path;
//				frame_index_path << frames_path.string() << "\\frame_index" << "\0";
//				cout << currentDateTime()  << "frame_index_path: " << frame_index_path.str() << endl;
//
//				boost::filesystem::path frame_index_dir(frame_index_path.str());
//			
//				if(!exists(frame_index_dir)) {
//					if (boost::filesystem::create_directory(frame_index_dir)) {
//						cout << currentDateTime()  << "frame_index_dir created!" << endl;												
//					} else {
//						cout << currentDateTime()  << "Cannot create frame_index_dir!" << endl;
//						return 1;
//					}
//				}
//
//				for (vec::const_iterator it(v.begin()), it_end(v.end()); it != it_end; ++it)
//				{				
//					cout << currentDateTime()  << " filename: " << it->filename() << ", extension: " << it->extension() << endl;
//				
//					string file_extension = it->extension().string();
//
//					if(strcmp(file_extension.c_str(), frame_extension.c_str())==0) {
//						int frame_number = extract_frame_number_from_mask(it->filename().string());
//
//						cout << currentDateTime()  << " ------------------------------------------------------- " << endl;
//						cout << currentDateTime()  << " -------------- frame number: " << frame_number << " -------------- " << endl;
//						cout << currentDateTime()  << " ------------------------------------------------------- " << endl;
//
//						cout << currentDateTime()  << " Generating index for frame..." << endl;								
//
//						sprintf(frame_index_filename, "\\image%05d.xml\0", frame_number);
//
//						cout << currentDateTime()  << "frame_index filename: " << frame_index_filename << endl;
//
//						std::ostringstream frame_index_file_path;
//						frame_index_file_path << frame_index_dir.string() << frame_index_filename;
//						cout << currentDateTime()  << "frame_index_file_path: " << frame_index_file_path.str() << endl;
//
//						XSLTResultTarget xmlOut(frame_index_file_path.str().c_str());
//
//						sprintf(frame_number_string, "%d\0", frame_number);
//						cout << currentDateTime()  << "frame number string: " << frame_number_string << endl;						
//
//						const XalanDOMString key("frame_number");
//						const XalanDOMString expression(frame_number_string);
//
//						theXalanTransformer.setStylesheetParam(key, expression);	
//						theXalanTransformer.setIndent(4);
//
//						int theResult = theXalanTransformer.transform(xmlIn, xslIn, xmlOut);
//	
//						//cout << currentDateTime()  << "Result of Transformation is " << theResult << "\n";						
//					}
//				}
//			}
//
//			if (strcmp(argv[4], "1") == 0 || strcmp(argv[5], "1") == 0) {
//
//				std::ostringstream frame_index_path;
//				frame_index_path << frames_path.string() << "\\frame_index" << "\0";
//				//cout << currentDateTime()  << "frame_index_path: " << frame_index_path.str() << endl;
//
//				boost::filesystem::path frame_index_dir(frame_index_path.str());
//
//				for (vec::const_iterator it(v.begin()), it_end(v.end()); it != it_end; ++it)
//				{				
//					//cout << currentDateTime()  << " filename: " << it->filename() << ", extension: " << it->extension() << endl;
//				
//					string frame_filename = it->string();
//					string file_extension = it->extension().string();
//
//					if(strcmp(file_extension.c_str(), frame_extension.c_str())==0) {
//						int frame_number = extract_frame_number_from_mask(it->filename().string());
//
//						//cout << currentDateTime()  << " ------------------------------------------------------- " << endl;
//						//cout << currentDateTime()  << " -------------- frame number: " << frame_number << " -------------- " << endl;
//						//cout << currentDateTime()  << " ------------------------------------------------------- " << endl;
//
//						//cout << currentDateTime()  << " Reading index for frame... " << endl;						
//
//						sprintf(frame_index_filename, "\\%d.xml\0", frame_number);
//						//cout << currentDateTime()  << "frame_index filename: " << frame_index_filename << endl;
//
//						std::ostringstream frame_index_file_path;
//						frame_index_file_path << frame_index_path.str() << frame_index_filename;
//						//cout << currentDateTime()  << "parsing frame_index_file_path: " << frame_index_file_path.str() << endl;						
//
//						DOMDocument* frame_info_document = xmlParser->parseURI(frame_index_file_path.str().c_str());
//
//						auto_ptr<frame_info_t> fi (frame_info (*frame_info_document));
//
//						//cout << currentDateTime()  << endl << "frame_info id: " << fi->frame_id() << ", roi count: " << distance(fi->roi ().begin (), fi->roi ().end ()) << endl;
//
//						sprintf(frame_number_string, "%05d\0", frame_number);
//						//cout << currentDateTime()  << "frame number string: " << frame_number_string << endl;
//
//						Mat a;							
//						//cout << currentDateTime()  << "Showing frame: " << frame_number << endl;
//						a = imread(frame_filename);
//
//						if(!a.data)
//						{
//							cout << currentDateTime()  <<  "No image data" << endl;
//						} else {				   
//							//cout << currentDateTime()  <<  "Image data!" << endl;														
//
//							int roi_counter = 0;
//
//							if (strcmp(argv[5], "1") == 0) {
//								for (frame_info_t::roi_const_iterator ri (fi->roi ().begin ()); ri != fi->roi ().end (); ++ri)
//								{
//									roi_counter++;																								
//
//									sprintf(roi_label_string, "Person%d\0", ri->person_id());								
//
//									cv::Rect rect (Point(ri->bb_ul_x(), ri->bb_ul_y()), Point(ri->bb_lr_x(), ri->bb_lr_y()));
//
//									std::ostringstream full_roi_dir_path;
//									full_roi_dir_path << frames_path.string() << "\\roi\\" << roi_label_string << "\\" << ri->profile_type() << "\\frames\\full" << "\0";
//									//cout << currentDateTime()  << "full_roi_dir_path: " << full_roi_dir_path.str() << endl;
//
//									std::ostringstream bg_full_roi_dir_path;
//									bg_full_roi_dir_path << frames_path.string() << "\\roi\\" << roi_label_string << "\\" << ri->profile_type() << "\\frames\\bg" << "\0";
//									//cout << currentDateTime()  << "full_roi_dir_path: " << full_roi_dir_path.str() << endl;
//
//									boost::filesystem::path full_roi_dir(full_roi_dir_path.str());
//									boost::filesystem::path bg_full_roi_dir(bg_full_roi_dir_path.str());
//			
//									if(!exists(full_roi_dir)) {
//										if (boost::filesystem::create_directories(full_roi_dir)) {
//											//cout << currentDateTime()  << "full_roi_dir created!" << endl;												
//										} else {
//											//cout << currentDateTime()  << "Cannot create full_roi_dir!" << endl;
//											return 1;
//										}
//									}
//
//									if(!exists(bg_full_roi_dir)) {
//										if (boost::filesystem::create_directories(bg_full_roi_dir)) {
//											//cout << currentDateTime()  << "bg_full_roi_dir created!" << endl;												
//										} else {
//											//cout << currentDateTime()  << "Cannot create bg_full_roi_dir!" << endl;
//											return 1;
//										}
//									}
//								
//									sprintf(roi_frame_filename, "image%05d.png\0", frame_number);									
//
//									std::ostringstream full_roi_frame_path;
//									full_roi_frame_path << full_roi_dir.string() << "\\" << roi_frame_filename << "\0";
//									//cout << currentDateTime()  << "full_roi_frame_path: " << full_roi_frame_path.str() << endl;									
//
//									std::ostringstream bg_full_roi_frame_path;
//									bg_full_roi_frame_path << bg_full_roi_dir.string() << "\\" << roi_frame_filename << "\0";
//									//cout << currentDateTime()  << "full_roi_frame_path: " << full_roi_frame_path.str() << endl;
//
//									imwrite(bg_full_roi_frame_path.str() , a);
//
//									Mat mask = Mat::zeros(a.size(), CV_8UC1);								
//									mask(rect) = 255;
//									Mat full_roi;
//									a.copyTo(full_roi, mask);
//
//									imwrite(full_roi_frame_path.str(), full_roi);
//
//									std::ostringstream single_roi_dir_path;
//									single_roi_dir_path << frames_path.string() << "\\roi\\" << roi_label_string << "\\" << ri->profile_type() << "\\frames\\single" << "\0";
//									//cout << currentDateTime()  << "single_roi_dir_path: " << single_roi_dir_path.str() << endl;
//
//									boost::filesystem::path single_roi_dir(single_roi_dir_path.str());
//			
//									if(!exists(single_roi_dir)) {
//										if (boost::filesystem::create_directories(single_roi_dir)) {
//											//cout << currentDateTime()  << "single_roi_dir created!" << endl;												
//										} else {
//											//cout << currentDateTime()  << "Cannot create single_roi_dir!" << endl;
//											return 1;
//										}
//									}
//
//									std::ostringstream single_roi_frame_path;
//									single_roi_frame_path << single_roi_dir.string() << "\\" << roi_frame_filename << "\0";
//									//cout << currentDateTime()  << "single_roi_frame_path: " << single_roi_frame_path.str() << endl;
//
//									Mat single_roi = a(rect);
//
//									imwrite(single_roi_frame_path.str(), single_roi);
//								}
//
//								cout << currentDateTime()  << "Saved rois for frame " << frame_number << "..." << endl;
//
//								roi_counter = 0;
//							}							
//
//							if (strcmp(argv[4], "1") == 0) {
//								namedWindow("HAGTA", CV_WINDOW_AUTOSIZE);
//
//								for (frame_info_t::roi_const_iterator ri (fi->roi ().begin ()); ri != fi->roi ().end (); ++ri)
//								{
//									roi_counter++;	
//
//									cv::Rect rect (Point(ri->bb_ul_x(), ri->bb_ul_y()), Point(ri->bb_lr_x(), ri->bb_lr_y()));
//
//									Scalar roi_color (100+(roi_counter*50), 250-(roi_counter*50), roi_counter*50);
//
//									set_labels(a, rect, ri->profile_type(), roi_label_string, ri->person_description(), roi_counter, roi_color);
//
//									rectangle(
//										a, 
//										Point(ri->bb_ul_x(), ri->bb_ul_y()), 
//										Point(ri->bb_lr_x(), ri->bb_lr_y()), 
//										roi_color, 
//										2
//									);
//								}
//
//								sprintf(frame_number_string, "FRAME %05d\0", frame_number);
//								putText(a, frame_number_string , Point(5, 25), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255), 3);
//
//								imshow("HAGTA", a);
//								waitKey(1000/40);
//							}							
//						}
//					}
//				}							
//			}
//
//			if (strcmp(argv[6], "1") == 0 || strcmp(argv[7], "1") == 0) {
//				typedef vector<path> vec;				
//
//				string frame_index_dir_path = frames_path.string() + "\\frame_index";
//
//				path frame_index_dir(frame_index_dir_path);
//																	
//				int first_frame_index_number = 1;
//
//				vec v;
//
//				copy(directory_iterator(frame_index_dir), directory_iterator(), back_inserter(v));
//
//				sort(v.begin(), v.end(), numericCompare);
//
//				for (vec::const_iterator it (v.begin()); it != v.end(); ++it)
//				{										
//					string first_file = it->filename().string();
//
//					//cout << " first_file: " << first_file << endl;
//
//					unsigned found = first_file.find_last_of(".");
//
//					first_frame_index_number = atoi(first_file.substr(0, found).c_str());
//
//					cout << " first_frame_index_number: " << first_frame_index_number << endl;
//
//					break;
//				}
//
//				// Generate roi videos from roi frames
//				std::ostringstream roi_dir_path;
//				roi_dir_path << frames_path.string() << "\\roi" << "\0";
//				//cout << currentDateTime()  << "roi_dir_path: " << roi_dir_path.str() << endl;	
//
//				path roi_dir (roi_dir_path.str());
//
//				try
//				{
//					if (exists(roi_dir))
//					{						
//						if (is_directory(roi_dir))
//						{
//							//cout << currentDateTime()  << roi_dir << " is a directory containing:\n";							
//
//							vec roi_path_vector;
//
//							copy(directory_iterator(roi_dir), directory_iterator(), back_inserter(roi_path_vector));							
//							
//							for (vec::const_iterator it_roi(roi_path_vector.begin()), it_roi_end(roi_path_vector.end()); it_roi != it_roi_end; ++it_roi)
//							{
//								//cout << currentDateTime()  << "   " << *it_roi << '\n';								
//
//								vec person_path_vector;
//
//								path person_dir (*it_roi);
//
//								//cout << currentDateTime()  << person_dir << " is a directory containing:\n";
//
//								copy(directory_iterator(person_dir), directory_iterator(), back_inserter(person_path_vector));
//
//								if (strcmp(argv[6], "1") == 0) {
//									for (vec::const_iterator it_person(person_path_vector.begin()), it_person_end(person_path_vector.end()); it_person != it_person_end; ++it_person)
//									{
//										//cout << currentDateTime()  << "   " << *it_person << '\n';									
//
//										path profile_dir (*it_person);
//
//										// Generate videos according to standard structure of directories.
//
//										// Full rois video
//										vec full_roi_path_vector;
//										
//										path full_video_frames_pathname(profile_dir.string() + "\\frames\\full\\");										
//
//										string full_video_dir_pathname = profile_dir.string() + "\\video.avi";										
//
//										//cout << currentDateTime()  << "full_video_dir_pathname: " << full_video_dir_pathname << endl;
//
//										VideoWriter full_video_out(full_video_dir_pathname, CV_FOURCC('M','J','P','G'), 30, Size(512, 384));									
//
//										copy(directory_iterator(full_video_frames_pathname), directory_iterator(), back_inserter(full_roi_path_vector));
//
//										for (vec::const_iterator it_full_frame(full_roi_path_vector.begin()), it_full_frame_end(full_roi_path_vector.end()); it_full_frame != it_full_frame_end; ++it_full_frame)
//										{
//											//cout << currentDateTime()  << "   " << *it_full_frame << '\n';
//
//											path frame_filename_path (*it_full_frame);										
//
//											Mat a;
//											//cout << currentDateTime()  << "Reading " <<  frame_filename_path.string() << endl;
//											a = imread(frame_filename_path.string());
//
//											if(!a.data)
//											{
//												//cout << currentDateTime()  <<  "No image data" << endl;
//											} else {				   
//												//cout << currentDateTime()  <<  "Image data!" << endl;
//												full_video_out.write(a);
//											}
//										}
//
//										// bg_full rois video
//										vec bg_full_roi_path_vector;
//										
//										path bg_full_video_frames_pathname(profile_dir.string() + "\\frames\\bg\\");
//
//										string bg_full_video_dir_pathname = profile_dir.string() + "\\video_bg.avi";
//
//										//cout << currentDateTime()  << "bg_full_video_dir_pathname: " << bg_full_video_dir_pathname << endl;
//
//										VideoWriter bg_full_video_out(bg_full_video_dir_pathname, CV_FOURCC('M','J','P','G'), 30, Size(512, 384));									
//
//										copy(directory_iterator(bg_full_video_frames_pathname), directory_iterator(), back_inserter(bg_full_roi_path_vector));
//
//										for (vec::const_iterator it_bg_full_frame(bg_full_roi_path_vector.begin()), it_bg_full_frame_end(bg_full_roi_path_vector.end()); it_bg_full_frame != it_bg_full_frame_end; ++it_bg_full_frame)
//										{
//											//cout << currentDateTime()  << "   " << *it_bg_full_frame << '\n';
//
//											path frame_filename_path (*it_bg_full_frame);										
//
//											Mat a;
//											//cout << currentDateTime()  << "Reading " <<  frame_filename_path.string() << endl;
//											a = imread(frame_filename_path.string());
//
//											if(!a.data)
//											{
//												//cout << currentDateTime()  <<  "No image data" << endl;
//											} else {				   
//												//cout << currentDateTime()  <<  "Image data!" << endl;
//												bg_full_video_out.write(a);
//											}
//										}
//										
//										cout << currentDateTime()  << "Saved videos for " << person_dir.filename().string() << "\\" << profile_dir.filename().string() << "..." << endl;
//									}		
//								}								
//
//								if (strcmp(argv[7], "1") == 0) {									
//									for (vec::const_iterator it_person(person_path_vector.begin()), it_person_end(person_path_vector.end()); it_person != it_person_end; ++it_person)
//									{
//										//cout << currentDateTime()  << "   " << *it_person << '\n';									
//
//										path profile_dir (*it_person);
//
//										int person_id = std::stoi(person_dir.filename().string().substr(6));
//									
//										/*										
//										string full_video_dir_pathname = profile_dir.string() + "\\video.avi";
//
//										//cout << currentDateTime()  << "full_video_dir_pathname: " << full_video_dir_pathname << endl;																
//
//										VideoCapture cap(full_video_dir_pathname); // open the video file for reading
//
//										if ( !cap.isOpened() )  // if not success, exit program
//										{
//											 cout << currentDateTime()  << "Cannot open the video file" << endl;
//											 return -1;
//										}									
//
//										//int frame_count = cap.get(CV_CAP_PROP_FRAME_COUNT); //get the frames per seconds of the video
//										//cout << currentDateTime()  << "Frame count : " << frame_count << endl;
//
//										//double fps = cap.get(CV_CAP_PROP_FPS); //get the frames per seconds of the video
//										//cout << currentDateTime()  << "Frame per seconds : " << fps << endl;
//
//										// STIP
//										string stip_output_pathname = profile_dir.string() + "\\video_stip_" + profile_dir.filename().string() + "_" + frames_path.filename().string() + ".txt";
//
//										//cout << currentDateTime()  << "stip_full_video_pathname: " << full_video_dir_pathname << endl;
//										//cout << currentDateTime()  << "stip_output_pathname: " << stip_output_pathname << endl;
//
//										//cout << currentDateTime()  << "person_dir.filename().string(): " << person_dir.filename().string() << endl;
//										//cout << currentDateTime()  << "person_dir.filename().string().length(): " << person_dir.filename().string().length() << endl;
//										//cout << currentDateTime()  << "person_dir.filename().string().substr(5): " << person_dir.filename().string().substr(5) << endl;
//										//cout << currentDateTime()  << "person_dir.filename().string().substr(6): " << person_dir.filename().string().substr(6) << endl;
//										
//										cout << currentDateTime()  << "Processing STIP for person id: " << person_id << endl;										
//
//										compute_STIP(full_video_dir_pathname, stip_output_pathname, frame_index_dir_path, person_id, first_frame_index_number);
//										cout << currentDateTime()  << "STIP calculated for " << person_dir.filename().string() << "\\" << profile_dir.filename().string() << "..." << endl;
//										*/
//
//										string bg_full_video_dir_pathname = profile_dir.string() + "\\video_bg.avi";
//
//										//cout << currentDateTime()  << "bg_full_video_dir_pathname: " << bg_full_video_dir_pathname << endl;																
//
//										VideoCapture bg_cap(bg_full_video_dir_pathname); // open the video file for reading
//
//										if ( !bg_cap.isOpened() )  // if not success, exit program
//										{
//											 cout << currentDateTime()  << "Cannot open the video file" << endl;
//											 return -1;
//										}									
//
//										//int frame_count = cap.get(CV_CAP_PROP_FRAME_COUNT); //get the frames per seconds of the video
//										//cout << currentDateTime()  << "Frame count : " << frame_count << endl;
//
//										//double fps = cap.get(CV_CAP_PROP_FPS); //get the frames per seconds of the video
//										//cout << currentDateTime()  << "Frame per seconds : " << fps << endl;
//
//										// STIP
//										string bg_stip_output_pathname = profile_dir.string() + "\\video_bg_stip_" + profile_dir.filename().string() + "_" + frames_path.filename().string() + ".txt";
//
//										//cout << currentDateTime()  << "stip_bg_full_video_pathname: " << bg_full_video_dir_pathname << endl;
//										//cout << currentDateTime()  << "stip_output_pathname: " << stip_output_pathname << endl;
//
//										//cout << currentDateTime()  << "person_dir.filename().string(): " << person_dir.filename().string() << endl;
//										//cout << currentDateTime()  << "person_dir.filename().string().length(): " << person_dir.filename().string().length() << endl;
//										//cout << currentDateTime()  << "person_dir.filename().string().substr(5): " << person_dir.filename().string().substr(5) << endl;
//										//cout << currentDateTime()  << "person_dir.filename().string().substr(6): " << person_dir.filename().string().substr(6) << endl;
//										
//										cout << currentDateTime()  << "Processing bg STIP for person id: " << person_id << endl;										
//
//										compute_STIP(bg_full_video_dir_pathname, bg_stip_output_pathname, frame_index_dir_path, person_id, first_frame_index_number);
//										cout << currentDateTime()  << "bg STIP calculated for " << person_dir.filename().string() << "\\" << profile_dir.filename().string() << "..." << endl;
//									}
//								}
//							}
//						}
//						else 
//						{
//							cout << currentDateTime()  << roi_dir << " is not a directory\n";
//							return 1;
//						}
//					}
//					else {
//						cout << currentDateTime()  << roi_dir << " does not exist\n";
//					}
//				}
//				catch (const filesystem_error& ex)
//				{
//					cout << currentDateTime()  << ex.what() << '\n';
//				}
//			}	
//		}
//	}
//	catch (const filesystem_error& ex)
//	{
//		cout << currentDateTime()  << ex.what() << '\n';
//	}
//
//	XQillaPlatformUtils::terminate();	
//	XalanTransformer::terminate();
//	XMLPlatformUtils::Terminate();
//	XalanTransformer::ICUCleanUp();
//
//	return 0;
//}