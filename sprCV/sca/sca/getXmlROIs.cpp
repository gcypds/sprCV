//// getXmlROIs include
//#include "getXmlROIs.h"
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
//
//// XQilla includes
//#include <xqilla/xqilla-dom3.hpp>
//
//// XSD Codesynthesis generated stubs includes
//#include "xml/frame_info.hxx"
//
//// Boost includes
//#include <boost/filesystem.hpp>
//#include <boost/algorithm/string.hpp>
//
//using std::cout;
//using std::cerr;
//using namespace cv;
//using namespace std;
//using namespace boost::filesystem;
//using namespace xercesc;
//
//#define MAX_FRAME_INDEX_FILENAME_SIZE 10
//
//DOMImplementation* xqillaImplementation;
//DOMLSParser* xmlParser;
//DOMConfiguration* dc_parser;
//string frame_index_path;
//int person_id;
//int first_frame_index_number;
//
//// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
//const std::string currentDateTime() {
//    time_t     now = time(0);
//    struct tm  tstruct;
//    char       buf[80];
//    tstruct = *localtime(&now);   
//    strftime(buf, sizeof(buf), "%X - %d/%m/%Y -> ", &tstruct);
//
//    return buf;
//}
//
//void initXmlROIs() {
//	xqillaImplementation = DOMImplementationRegistry::getDOMImplementation(X("XPath2 3.0"));
//	xmlParser = xqillaImplementation->createLSParser(DOMImplementationLS::MODE_SYNCHRONOUS, 0);
//	dc_parser = xmlParser->getDomConfig();
//	
//	dc_parser->setParameter(XMLUni::fgDOMNamespaces, true);
//	dc_parser->setParameter(XMLUni::fgXercesSchema, true);
//	dc_parser->setParameter(XMLUni::fgDOMValidate, true);	
//}
//
//int getProfileCode(string profileType) {
//	if(profileType == "DISORIENTED" || profileType == "INTERESTED") {
//		return 1;
//	} else if(profileType == "DISTRACTED") {
//		return 2;
//	} else if(profileType == "EXPLORING") {
//		return 3;
//	} else {
//		return 1;
//	}
//}
//
//void getXmlROIs(string video_path, int idx, vector<XmlROIsInfo> &xml_rois_info) {
//	static char frame_index_filename[MAX_FRAME_INDEX_FILENAME_SIZE] = {'\0'};
//
//	path video_dir (video_path);
//
//	if (!exists(video_dir)) {
//		cout << currentDateTime()  << "Path: " << video_dir << " doesn't exist!" << endl;
//		return;
//	}
//
//	if (!is_directory(video_dir)) {
//		cout << currentDateTime()  << video_dir << " is NOT a directory!" << endl;
//		return;
//	}
//
//	sprintf(frame_index_filename, "%d.xml\0", idx);
//	cout << currentDateTime()  << "frame_index filename: " << frame_index_filename << endl;
//
//	std::ostringstream frame_index_file_path;
//	frame_index_file_path << video_path << "\\" << frame_index_filename;
//	cout << currentDateTime()  << "parsing frame_index_file_path: " << frame_index_file_path.str() << endl;						
//
//	DOMDocument* frame_info_document = xmlParser->parseURI(frame_index_file_path.str().c_str());
//
//	const DOMXPathNSResolver* resolver = frame_info_document->createNSResolver(frame_info_document->getDocumentElement());
//
//	XQillaNSResolver* xqillaResolver = (XQillaNSResolver*)resolver;   
//	xqillaResolver->addNamespaceBinding(X("xs"), X("http://www.w3.org/2001/XMLSchema"));	
//	xqillaResolver->addNamespaceBinding(X("fn"), X("http://www.w3.org/2005/xpath-functions"));	
//
//	try {
//
//		std::ostringstream roi_xpath_expression;
//		roi_xpath_expression << "//roi[person_id[text()=\""<< person_id << "\"]]";
//		cout << currentDateTime()  << "roi_xpath_expression: " << roi_xpath_expression.str() << endl;
//
//		const DOMXPathExpression* parsedExpression = frame_info_document->createExpression(X(roi_xpath_expression.str().c_str()), resolver);
//
//		DOMXPathResult* iteratorResult = (DOMXPathResult*)parsedExpression->evaluate(frame_info_document->getDocumentElement(), DOMXPathResult::ITERATOR_RESULT_TYPE, 0);
//
//		int i = 0;
//
//		while(iteratorResult->iterateNext()) {
//			if(iteratorResult->isNode()) {
//				DOMNode* n (iteratorResult->getNodeValue ());
//
//				char * localName = XMLString::transcode(n->getLocalName());
//
//				cout << currentDateTime()  << "Localname: " << localName << endl;
//
//				DOMElement* resultElement = dynamic_cast<DOMElement*>(n);
//
//				if(strcmp( localName, "roi") == 0) {
//
//					roi* r = new roi (*resultElement);
//
//					XmlROIsInfo xmlROIsInfo;
//
//					xmlROIsInfo.coordinates = Rect(Point(r->bb_ul_x(),  r->bb_ul_y()), Point(r->bb_lr_x(), r->bb_lr_y()));
//					xmlROIsInfo.labelText = r->profile_type();
//					xmlROIsInfo.labelCode = getProfileCode(r->profile_type());
//
//					cout << currentDateTime()  << "roi info for person " << person_id << ": " << endl;
//					cout << currentDateTime()  << "bb_ul_x: " << r->bb_ul_x() << endl;
//					cout << currentDateTime()  << "bb_ul_y: " << r->bb_ul_y() << endl;
//					cout << currentDateTime()  << "bb_lr_x: " << r->bb_lr_x() << endl;
//					cout << currentDateTime()  << "bb_lr_y: " << r->bb_lr_y() << endl;
//
//					xml_rois_info.push_back(xmlROIsInfo);
//				}
//			}
//		}
//	}
//	catch(DOMXPathException &e) {
//		cerr << "DOMXPathException: " << UTF8(e.msg) << endl;
//		return;
//	}
//	catch(DOMException &e) {
//		cerr << "DOMException: " << UTF8(e.getMessage()) << endl;
//		return;
//	}	
//}