//#include "XMLProcessing.h"
//
//using namespace cv;
//using namespace std;
//using namespace boost::filesystem;
//using namespace xercesc;
//XERCES_CPP_NAMESPACE_USE
//using std::cerr;
//
//float pminArea = 0.0003;
//
//#define MAX_PRINTF_BUFFER_SIZE 200
//
//char printf_buffer[MAX_PRINTF_BUFFER_SIZE] = {'\0'};
//const string variables_pathname = "cbookf\\";
//
//const std::string currentDateTime() {
//	time_t     now = time(0);
//	struct tm  tstruct;
//	char       buf[80];
//	tstruct = *localtime(&now);   
//	strftime(buf, sizeof(buf), "%X - %d/%m/%Y -> ", &tstruct);
//
//	return buf;
//}
//
//bool removeTrailingSpace(string filename) {
//	string line;
//	// open input file
//	ifstream in(filename);
//	if( !in.is_open())
//	{
//		cout << "Input file failed to open\n";
//		return false;
//	}
//
//	// Remove trailing space
//	getline(in,line);
//	ofstream out("outfile.xml");
//
//	while( getline(in,line) )
//	{
//		out << line << "\n";
//		cout << line << endl;
//	}	
//	in.close();
//	out.close();
//
//	try {
//		//cout << "Removing " << filename.c_str() << endl;
//		// delete the original file
//		//std::remove(filename.c_str());	
//		boost::filesystem::remove(path(filename));
//
//		//cout << "REMOVED! " << filename.c_str() << endl;
//
//		//cout << "Renaming to " << filename.c_str() << endl;
//		// rename old to new
//		//std::rename("outfile.xml",filename.c_str());
//		boost::filesystem::rename(path("outfile.xml"), path(filename));
//
//		//cout << "RENAMED! " << filename.c_str() << endl;
//	} catch (const filesystem_error& ex) {
//		cout << currentDateTime()  << ex.what() << '\n';
//	}
//
//	return 0;
//}
//
//
//void SaveXMLInfo(video::frame_sequence &f_s, frame::roi_sequence o_s, int fr_idx)
//{
//	/*frame::roi_sequence o_s;
//
//	for(int i=0; i<states.size(); i++)
//	{
//		roi o1;
//		o1.weight(get<0>(states[i]));
//		o1.h((get<1>(states[i]))[3]);
//		o1.w((get<1>(states[i]))[2]);
//		o1.x((get<1>(states[i]))[0]);
//		o1.y((get<1>(states[i]))[1]);
//		o_s.push_back(o1);
//	}*/
//
//	frame f;
//	f.roi(o_s);
//	f.no(fr_idx);
//
//	f_s.push_back(f);
//}
//
//void serializeVideo(std::string outFilename, video::frame_sequence f_s) {
//	try {
//
//		video v;
//		v.name("01111");
//		v.frame(f_s);
//		v.algorithm("alg2");
//
//		XMLPlatformUtils::Initialize ();
//
//		{
//			// Choose a target.
//			//
//			auto_ptr<XMLFormatTarget> ft;		  
//
//			ft = auto_ptr<XMLFormatTarget> (new LocalFileFormatTarget (outFilename.c_str()));
//
//			xml_schema::namespace_infomap map;		  
//			map[""].schema = "xml/video.xsd";
//			video_ (*ft, v, map, "UTF-8");
//		}
//
//		XMLPlatformUtils::Terminate ();
//	} catch (const xml_schema::exception& e) {
//		std::cerr << e << std::endl;
//	}
//}
//
//void deserializeVideo(std::string inFilename) {	
//	std::auto_ptr<video> v(NULL);
//
//	try {
//		ifstream ifs (inFilename);
//		auto_ptr<video> v = video_ (ifs);
//		ifs.close ();		
//
//		/*std::cout << "Video Name: " << v->name() << endl;
//		std::cout << "Video Algorithm: " << v->algorithm() << endl;
//
//		
//		std::cout << "v->frame()[0].roi()[0].x(): " << v->frame()[0].roi()[0].x() << endl;
//
//		for(video::frame_sequence::iterator f = v->frame().begin(); f != v->frame().end(); f++) {
//			std::cout << "\tFrame No: " << f->no() << endl;
//
//			for(frame::roi_sequence::iterator r = f->roi().begin(); r != f->roi().end(); r++) {
//				std::cout << "\t\tRoi No: " << r->no() << endl;
//				std::cout << "\t\tRoi x: " << r->x() << endl;
//				std::cout << "\t\tRoi y: " << r->y() << endl;
//				std::cout << "\t\tRoi w: " << r->w() << endl;
//				std::cout << "\t\tRoi h: " << r->h() << endl;
//				std::cout << "\t\tRoi weight: " << r->weight() << endl << endl;
//			}
//		}*/
//	} catch (const xml_schema::exception& e) {
//		std::cerr << e << std::endl;
//	}
//}