#include <iostream>
#include <opencv2/opencv.hpp>

// Standard includes
#include <stdlib.h>
#include <string>
#include <iomanip>
#include <iterator>
#include <algorithm>
#include <vector>
#include <fstream>

// Boost includes
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
 
using namespace std;
using namespace cv;
using namespace boost::filesystem;

//extern "C" unsigned char testImageM;
//extern "C" unsigned char testImageM_size;

extern "C" unsigned char image;
extern "C" unsigned char image_size;

extern "C" unsigned char image_rc;
extern "C" unsigned char image_rc_size;

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);   
    strftime(buf, sizeof(buf), "%X - %d/%m/%Y -> ", &tstruct);

    return buf;
}

const string frame_extension = ".png";

int extract_frame_number_from_mask(string filename)
{	
	string frame_number = filename.substr(5, 5);
	return atoi(frame_number.c_str());
}

std::string getImageType(int number)
{
    // find type
    int imgTypeInt = number%8;
    std::string imgTypeString;

    switch (imgTypeInt)
    {
        case 0:
            imgTypeString = "8U";
            break;
        case 1:
            imgTypeString = "8S";
            break;
        case 2:
            imgTypeString = "16U";
            break;
        case 3:
            imgTypeString = "16S";
            break;
        case 4:
            imgTypeString = "32S";
            break;
        case 5:
            imgTypeString = "32F";
            break;
        case 6:
            imgTypeString = "64F";
            break;
        default:
            break;
    }

    // find channel
    int channel = (number/8) + 1;

    std::stringstream type;
    type<<"CV_"<<imgTypeString<<"C"<<channel;	

    return type.str();
}

// take number image type number (from cv::Mat.type()), get OpenCV's enum string.
string getImgType(int imgTypeInt)
{
    int numImgTypes = 35; // 7 base types, with five channel options each (none or C1, ..., C4)

    int enum_ints[] =       {CV_8U,  CV_8UC1,  CV_8UC2,  CV_8UC3,  CV_8UC4,
                             CV_8S,  CV_8SC1,  CV_8SC2,  CV_8SC3,  CV_8SC4,
                             CV_16U, CV_16UC1, CV_16UC2, CV_16UC3, CV_16UC4,
                             CV_16S, CV_16SC1, CV_16SC2, CV_16SC3, CV_16SC4,
                             CV_32S, CV_32SC1, CV_32SC2, CV_32SC3, CV_32SC4,
                             CV_32F, CV_32FC1, CV_32FC2, CV_32FC3, CV_32FC4,
                             CV_64F, CV_64FC1, CV_64FC2, CV_64FC3, CV_64FC4};

    string enum_strings[] = {"CV_8U",  "CV_8UC1",  "CV_8UC2",  "CV_8UC3",  "CV_8UC4",
                             "CV_8S",  "CV_8SC1",  "CV_8SC2",  "CV_8SC3",  "CV_8SC4",
                             "CV_16U", "CV_16UC1", "CV_16UC2", "CV_16UC3", "CV_16UC4",
                             "CV_16S", "CV_16SC1", "CV_16SC2", "CV_16SC3", "CV_16SC4",
                             "CV_32S", "CV_32SC1", "CV_32SC2", "CV_32SC3", "CV_32SC4",
                             "CV_32F", "CV_32FC1", "CV_32FC2", "CV_32FC3", "CV_32FC4",
                             "CV_64F", "CV_64FC1", "CV_64FC2", "CV_64FC3", "CV_64FC4"};

    for(int i=0; i<numImgTypes; i++)
    {
        if(imgTypeInt == enum_ints[i]) return enum_strings[i];
    }
    return "unknown image type";
}

		
const int MIN_RANDOM_POLYGONS = 40;
const int MAX_RANDOM_POLYGONS = 50;
const int MIN_RANDOM_POINTS_PER_POLYGON = 8;
const int MAX_RANDOM_POINTS_PER_POLYGON = 14;
const int MIN_RANDOM_RADIUS = 30;
const int MAX_RANDOM_RADIUS = 50;
const int RANDOM_RADIUS_VARIATON = 5;
const double MAX_BLACK_PERCENTAGE = 30.00;
const double MIN_NON_BLACK_PERCENTAGE = 100.00 - MAX_BLACK_PERCENTAGE;
#define PI 3.14159265

struct RandomPolygon { 
	Point centroid;
	int radius;
	vector<Point> random_coordinates;
};

void polar(double x, double y, double& r, double& theta)
{
	r = sqrt((pow(x,2))+(pow(y,2)));

    theta = atan(y/x);
	
	theta = (theta*180)/3.141592654;
}

bool comparePoints(Point a, Point b)
{
  return (a.x < b.x);
}

bool saveBinaryVariable(char * filename, void * variable, size_t variable_size) {
	ofstream ofs(filename, ios::binary);
	ofs.write((char *)variable, variable_size);
	ofs.close();
	return true;
}
 
class Student {
public:
	char   FullName[40];
	char   CompleteAddress[120];
	char   Gender;
	double Age;
	bool   LivesInASingleParentHome;
};

int main (int argc, const char * argv[])
{
	Student one;
	strcpy(one.FullName, "Ernestine Waller");
	strcpy(one.CompleteAddress, "824 Larson Drv, Silver Spring, MD 20910");
	one.Gender = 'F';
	one.Age = 16.50;
	one.LivesInASingleParentHome = true;	

	saveBinaryVariable("student.bin", &one, sizeof(one));

	path frames_path (argv[1]);

    /*VideoCapture cap(CV_CAP_ANY);
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 320);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 240);    
    if (!cap.isOpened())
        return -1;*/
 
    /*Mat img;
    HOGDescriptor hog;*/
	
    /*HOGDescriptor(Size win_size=Size(64, 128), Size block_size=Size(16, 16),
                  Size block_stride=Size(8, 8), Size cell_size=Size(8, 8),
                  int nbins=9, double win_sigma=DEFAULT_WIN_SIGMA,
                  double threshold_L2hys=0.2, bool gamma_correction=true,
                  int nlevels=DEFAULT_NLEVELS);*/

	
    /*HOGDescriptor hog(Size(48, 96), Size(16, 16),
                  Size(8, 8), Size(8, 8),
                  9, -1,
                  0.2, true,
                  64);
*/

	//HOGDescriptor hog;
	//hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
 //
 //   /*namedWindow("video capture", CV_WINDOW_AUTOSIZE);*/

	Mat maskImage = imread("G:\\data\\sebastianvilla139\\work\\survmantics\\dataset\\memoDB\\Santiago\\ROIsurv.png", CV_LOAD_IMAGE_GRAYSCALE);

	if( argc != 2 || !maskImage.data ) {
		printf( " No image data \n " );
		return -1;
	} else {
		cout << currentDateTime()  <<  "Mask Image type: " << getImgType(maskImage.type()) << endl;
		//Mat binaryMaskImage;
		//cvtColor( maskImage, binaryMaskImage, CV_32F);
		//cout << currentDateTime()  <<  "Binary Mask Image type: " << getImgType(binaryMaskImage.type()) << endl;

		//Show the results
		//namedWindow("Output", cv::WINDOW_AUTOSIZE);
		//imshow("Output", maskImage);
		//waitKey();
		
		int cols = maskImage.cols;
		int rows = maskImage.rows;

		cv::Size s = maskImage.size();
		rows = s.height;
		cols = s.width;

		char dump[5] = {0};		
		
		/*for(int i=0; i<rows; i++) {
			for(int j=0; j<cols; j++) {
				sprintf(dump, "%02d", maskImage.at<uchar>(i,j));
				cout << "Point("<<i << "," << j << ") = " << dump << endl;
			}
		}*/

		Mat img_bw;		
		threshold(maskImage, img_bw, 254, 1.00, THRESH_BINARY); //GRAY 2 Binary based on threshold

		/*for(int i=0; i<rows; i++) {
			for(int j=0; j<cols; j++) {
				sprintf(dump, "%d", img_bw.at<uchar>(i,j));
				cout << "Point("<<i << "," << j << ") = " << dump << endl;
			}
		}*/

		Mat testImage = imread("G:\\data\\sebastianvilla139\\work\\survmantics\\dataset\\memoDB\\Santiago\\image00001.png");

		cout << currentDateTime()  <<  "Test Image type: " << getImgType(testImage.type()) << endl;
		
		/*Mat testImageM2;		
		testImage.copyTo(testImageM2, img_bw);*/

		int image_size[3] = {0};
		memcpy(image_size, &image_rc, 3*sizeof(int));
		cout << "image_size = {" << image_size[0] << " , " << image_size[1] << "}" << endl;
		cout << "image_type = {" << getImageType(image_size[2]) << "}" << endl;

		Mat testImageM2 = Mat(image_size[0], image_size[1], image_size[2], &image);

		imshow("testImageM2", testImageM2);
		waitKey();

		//namedWindow("Multiplication", cv::WINDOW_AUTOSIZE);
		//imshow("Multiplication", testImageM);
		//waitKey();
		
		/* initialize random seed: */
		srand (time(NULL));

		/* generate secret number between 10 and 30: */
		int iSecret = rand() % 20 + 10;		

		// Draw a circle 
		/*circle( testImageM, Point( 200, 200 ), 10.0, Scalar( 0, 0, 255 ), 1, 8 );
		imshow("ImageCircle",testImageM);
		waitKey();*/

		vector<RandomPolygon> random_polygons;		

		// Generate random list of centroids, count from MIN_RANDOM_POLYGONS to MAX_RANDOM_POLYGONS
		int number_of_regions = rand() % (MAX_RANDOM_POLYGONS - MIN_RANDOM_POLYGONS) + MIN_RANDOM_POLYGONS;

		cout << currentDateTime()  <<  "number_of_regions: " << number_of_regions << endl;

		for (int i=0; i<number_of_regions; i++) {
			RandomPolygon randomPolygon;

			// Generate a circle using centroid coordinates, degrees in angles

			int xCord = rand() % cols;
			int yCord = rand() % rows;

			randomPolygon.centroid = Point( xCord, yCord );
			
			cout << currentDateTime()  <<  "Centroid("<< randomPolygon.centroid.x << "," << randomPolygon.centroid.y << ")" << endl;

			// Generate base random radius
			int base_random_radius = rand() % (MAX_RANDOM_RADIUS - MIN_RANDOM_RADIUS) + MIN_RANDOM_RADIUS;
			cout << currentDateTime()  <<  "base_random_radius: " << base_random_radius << endl;

			// Generate random list of polygon points, count from MIN_RANDOM_POINTS_PER_POLYGON to MAX_RANDOM_POINTS_PER_POLYGON
			int number_of_points = rand() % (MAX_RANDOM_POINTS_PER_POLYGON - MIN_RANDOM_POINTS_PER_POLYGON) + MIN_RANDOM_POINTS_PER_POLYGON;
			cout << currentDateTime()  <<  "number_of_points: " << number_of_points << endl;
			
			float points_angle = 360 / number_of_points;
			cout << currentDateTime()  <<  "points_angle: " << points_angle << endl;

			// For each random centroid point generate a list of random polygon points, count from 5 to 30.
			for (int j=0; j<number_of_points; j++) {
				// Generate base random radius variation
				int random_radius_variation = rand() % (2*RANDOM_RADIUS_VARIATON+1) - RANDOM_RADIUS_VARIATON;
				cout << currentDateTime()  <<  "random_radius_variation: " << random_radius_variation << endl;
				
				//int point_radius = base_random_radius + random_radius_variation;
				int point_radius = base_random_radius;
				cout << currentDateTime()  <<  "point_radius: " << point_radius << endl;
				
				float point_angle = points_angle*j*180/PI;
				cout << currentDateTime()  <<  "point_angle: " << point_angle << endl;

				int point_xCord = randomPolygon.centroid.x + point_radius*cos(point_angle);				
				int point_yCord = randomPolygon.centroid.y + point_radius*sin(point_angle);								

				cout << currentDateTime()  <<  "Point("<< point_xCord << "," << point_yCord << ")" << endl;

				randomPolygon.random_coordinates.push_back(Point((point_xCord <= cols)? point_xCord: cols, (point_yCord <= rows)? point_yCord: rows));
			}

			cout << currentDateTime()  <<  "Before sorting..." << endl;

			// Print vector before sorting
			for(int k=0; k<number_of_points; k++) {				
				cout << currentDateTime()  <<  "Point(" << randomPolygon.random_coordinates[k].x << "," << randomPolygon.random_coordinates[k].y << ")" << endl;
			}

			//// Sorting the points vector
			vector<Point> sortedPoints;

			//std::sort(randomPolygon.random_coordinates.begin(), randomPolygon.random_coordinates.end(), comparePoints);

			//cout << currentDateTime()  <<  "After sorting..." << endl;

			//// Print vector before sorting
			//for(int k=0; k<number_of_points; k++) {				
			//	cout << currentDateTime()  <<  "Point(" << randomPolygon.random_coordinates[k].x << "," << randomPolygon.random_coordinates[k].y << ")" << endl;
			//}			

			/*Point sortedPointsArray[MAX_RANDOM_POINTS_PER_POLYGON];
			std::copy(randomPolygon.random_coordinates.begin(), randomPolygon.random_coordinates.end(), sortedPointsArray);*/

			//vector<Point> ROI_Poly;
			//approxPolyDP(randomPolygon.random_coordinates, ROI_Poly, 1.0, true);			
			//convexHull(randomPolygon.random_coordinates, sortedPoints, false, false);

			cout << currentDateTime()  <<  "ConvexHull sorting..." << endl;

			// Print vector before sorting
			for(int k=0; k<sortedPoints.size(); k++) {				
				cout << currentDateTime()  <<  "Point(" << sortedPoints[k].x << "," << sortedPoints[k].y << ")" << endl;
			}

			//const Point* ppt[1] = { sortedPointsArray };
			//int npt[] = { number_of_points };
						
			//Point const* p3 = (Point const*) &sortedPointsArray; 			

			//fillPoly( testImageM, ppt, npt, 1, Scalar( 255, 255, 255 ), 8 );
			//fillConvexPoly(testImageM, &sortedPointsArray[0], number_of_points, 255, 8, 0);

			// Fill polygon white
			//fillConvexPoly(testImageM, &ROI_Poly[0], ROI_Poly.size(), 255, 8, 0);       
			/* ROI by creating mask for the parallelogram */
			Mat emptyMask = Mat::zeros( Size(testImage.cols, testImage.rows), CV_8UC1 );
			//imshow("ImageFilled",emptyMask);
			//waitKey();
			//fillConvexPoly(emptyMask, &sortedPoints[0], sortedPoints.size(), Scalar( 255, 255, 255 ), 8, 0);
			//imshow("ImageFilled",emptyMask);
			//waitKey();

			//Rect boundingRectangle = boundingRect(sortedPoints);

			int noPolygonsRectWidth = 64;
			int noPolygonsRectHeigth = 128;

			Rect boundingRectangle = Rect( Point(randomPolygon.centroid.x - noPolygonsRectWidth/2, randomPolygon.centroid.y - noPolygonsRectHeigth/2), Point(randomPolygon.centroid.x + noPolygonsRectWidth/2, randomPolygon.centroid.y + noPolygonsRectHeigth/2));			

			int upper_right_x = boundingRectangle.x;
			int upper_right_y = boundingRectangle.y;			

			int lower_left_x = boundingRectangle.x + boundingRectangle.width;
			int lower_left_y = boundingRectangle.y + boundingRectangle.height;			

			bool sizeAfected = false;

			if(upper_right_x < 0) {
				upper_right_x = 0;
				sizeAfected = true;
			}
		
			if(upper_right_y < 0) {
				upper_right_y = 0;
				sizeAfected = true;
			}	

			if(lower_left_x > testImage.size().width) {
				lower_left_x = testImage.size().width;
				sizeAfected = true;
			}

			if(lower_left_y > testImage.size().height) {
				lower_left_y = testImage.size().height;
				sizeAfected = true;
			}

			if(!sizeAfected) {

				Point upper_right_point = Point(upper_right_x, upper_right_y);
				Point lower_lef_point = Point(lower_left_x, lower_left_y);

				rectangle( emptyMask, upper_right_point, lower_lef_point, Scalar( 0, 55, 255 ), +1, 4 );
				//imshow("ImageFilled",emptyMask);
				//waitKey();

				Rect croppedMask = Rect(upper_right_point, lower_lef_point);

				cout << currentDateTime()  <<  "ImageSize(" << testImage.size().width << "," << testImage.size().height << ")" << endl;
				cout << currentDateTime()  <<  "BoundingRect(" << boundingRectangle.x << "," << boundingRectangle.y << "," << boundingRectangle.width << "," << boundingRectangle.height << ")" << endl;
				cout << currentDateTime()  <<  "CroppedMask(" << croppedMask.x << "," << croppedMask.y << "," << croppedMask.width << "," << croppedMask.height << ")" << endl;

				Mat peopleAreaMask = maskImage(croppedMask);
				//imshow("peopleAreaMask",peopleAreaMask);
				//waitKey();
			
				//Mat polygonMask = emptyMask(croppedMask);
				//imshow("polygonMask",polygonMask);
				//waitKey();

				Mat finalMask = peopleAreaMask;

				//bitwise_and(peopleAreaMask, polygonMask, finalMask);
				//imshow("finalMask",finalMask);
				//waitKey();
			
				double polygon_roi_size = finalMask.cols * finalMask.rows;
				double non_black_percent = ((double) cv::countNonZero(finalMask))/polygon_roi_size;
				cout << currentDateTime()  << "Non black percent: " << std::setprecision(2) << non_black_percent*100 << "%" << endl;

				if( non_black_percent*100 >= MIN_NON_BLACK_PERCENTAGE) {
					char nameBuffer[10] = {'\0'};
					sprintf(nameBuffer, "%05d.png\0", i);
					string randomDataPathname = "G:\\data\\sebastianvilla139\\work\\survmantics\\dataset\\memoDB\\Santiago\\RandomRectData\\image"+ string(nameBuffer);				

					Mat polygonROI;
					testImage(croppedMask).copyTo(polygonROI, finalMask);

					//Mat img;
					//Mat subMask;			
					//testImage.copyTo(img);
					//Mat roiImg;

					//roiImg = img(boundingRectangle); /* sliced image */
					//namedWindow("ROI", CV_WINDOW_AUTOSIZE);
					//imshow("ROI", polygonROI);				

					cout << currentDateTime()  << "Saving to: " << randomDataPathname << endl;
					imwrite(randomDataPathname, polygonROI);
				
					//Mat black;
					//inRange(polygonROI, Scalar(0, 0, 0), Scalar(0, 0, 0), black); // black			

					//waitKey();

					random_polygons.push_back(randomPolygon);
				}
			}				
		}

		void * input;

		cout << currentDateTime()  <<  "testImageM type: " << getImageType(testImageM2.type()) << endl;
		cout << currentDateTime()  <<  "type: " << testImageM2.type() << endl;
		
		int imageTypeInt = testImageM2.type()%8;
		string imageTypeStr;

		switch(imageTypeInt) {
			case 0:
				// "8U"
				imageTypeStr = "8U";
				input = (unsigned char*)(testImageM2.data);
				break;
			case 1:
				//"8S"
				imageTypeStr = "8S";
				input = (unsigned char*)(testImageM2.data);
				break;
			case 2:
				//"16U"
				imageTypeStr = "16U";
				input = (short*)(testImageM2.data);
				break;
			case 3:
				//"16S"
				imageTypeStr = "16S";
				input = (short*)(testImageM2.data);
				break;
			case 4:
				//"32S"
				imageTypeStr = "32S";
				input = (int*)(testImageM2.data);
				break;
			case 5:
				//"32F"
				imageTypeStr = "32F";
				input = (float*)(testImageM2.data);
				break;
			case 6:
				//"64F"
				imageTypeStr = "64F";
				input = (double*)(testImageM2.data);
				break;
			default:
				//"8U"
				imageTypeStr = "default";
				input = (unsigned char*)(testImageM2.data);
				break;
		}		

		cout << currentDateTime()  <<  "testImageM imageTypeInt: " << imageTypeInt << endl;
		cout << currentDateTime()  <<  "testImageM imageTypeStr: " << imageTypeStr << endl;

		// Get testImageM size in bytes
		// size_t sizeInBytes = testImageM2.step[0] * testImageM2.rows;
		size_t sizeInBytes = testImageM2.total() * testImageM2.elemSize();

		cout << currentDateTime()  <<  "testImageM sizeInBytes: " << sizeInBytes << endl;
		cout << currentDateTime()  <<  "testImageM sizeof: " << sizeof(input) << endl;
		cout << currentDateTime()  <<  "testImageM sizeof: & " << sizeof(&input) << endl;
		saveBinaryVariable("image.bin", input, sizeInBytes);		
		int image_rc[3] = {testImageM2.rows, testImageM2.cols, testImageM2.type()};
		saveBinaryVariable("image_rc.bin", image_rc, 3*sizeof(int));

		//int w=cols;
		//// Draw a circle 
		///** Create some points */
		//Point rook_points[1][20];
		//rook_points[0][0] = Point( w/4.0, 7*w/8.0 );
		//rook_points[0][1] = Point( 3*w/4.0, 7*w/8.0 );
		//rook_points[0][2] = Point( 3*w/4.0, 13*w/16.0 );
		//rook_points[0][3] = Point( 11*w/16.0, 13*w/16.0 );
		//rook_points[0][4] = Point( 19*w/32.0, 3*w/8.0 );
		//rook_points[0][5] = Point( 3*w/4.0, 3*w/8.0 );
		//rook_points[0][6] = Point( 3*w/4.0, w/8.0 );
		//rook_points[0][7] = Point( 26*w/40.0, w/8.0 );
		//rook_points[0][8] = Point( 26*w/40.0, w/4.0 );
		//rook_points[0][9] = Point( 22*w/40.0, w/4.0 );
		//rook_points[0][10] = Point( 22*w/40.0, w/8.0 );
		//rook_points[0][11] = Point( 18*w/40.0, w/8.0 );
		//rook_points[0][12] = Point( 18*w/40.0, w/4.0 );
		//rook_points[0][13] = Point( 14*w/40.0, w/4.0 );
		//rook_points[0][14] = Point( 14*w/40.0, w/8.0 );
		//rook_points[0][15] = Point( w/4.0, w/8.0 );
		//rook_points[0][16] = Point( w/4.0, 3*w/8.0 );
		//rook_points[0][17] = Point( 13*w/32.0, 3*w/8.0 );
		//rook_points[0][18] = Point( 5*w/16.0, 13*w/16.0 );
		//rook_points[0][19] = Point( w/4.0, 13*w/16.0) ;
 
		//const Point* ppt[1] = { rook_points[0] };
		//int npt[] = { 20 };
 
		//fillPoly( testImageM, ppt, npt, 1, Scalar( 255, 255, 255 ), 8 );
		//imshow("ImageFilled",testImageM);
		//waitKey();		
		
		/*Mat testImageA = testImage&img_bw;
		namedWindow("And", cv::WINDOW_AUTOSIZE);
		imshow("And", testImageA);
		waitKey();*/		
	}
	//
	//try {
	//	if (!exists(frames_path))
	//	{
	//		cout << currentDateTime()  << "File: " << frames_path << " doesn't exist!" << endl;
	//		return 1;
	//	}

	//	if (!is_directory(frames_path)) {
	//		cout << currentDateTime()  << "frames_path is NOT a directory!" << endl;
	//		return 1;
	//	} else {
	//		namedWindow("peopleDetector", CV_WINDOW_AUTOSIZE);

	//		cout << currentDateTime()  << "Processing " << frames_path << " ..." << endl;

	//		typedef vector<path> vec;
	//		vec v;

	//		copy(directory_iterator(frames_path), directory_iterator(), back_inserter(v));	

	//		for (vec::const_iterator it(v.begin()), it_end(v.end()); it != it_end; ++it) {				
	//				cout << currentDateTime()  << " filename: " << it->filename() << ", extension: " << it->extension() << endl;
	//			
	//				string file_extension = it->extension().string();

	//				if(strcmp(file_extension.c_str(), frame_extension.c_str())==0) {
	//					        /*cap >> img;*/

	//					/*       if (!img.data)
	//							continue;
	//					*/

	//							Mat img;							
	//							//cout << currentDateTime()  << "Showing frame: " << frame_number << endl;

	//							cout << currentDateTime() << "Reading " << it->string() << endl;
	//							img = imread(it->string());								

	//							if(!img.data)
	//							{
	//								cout << currentDateTime()  <<  "No image data" << endl;
	//							} else {				   
	//								//cout << currentDateTime()  <<  "Image data!" << endl;		

	//								Mat dst;
	//								
	//								// cout << currentDateTime()  <<  "Image type: " << getImgType(img.type()) << endl;

	//								// Change image type from 8UC1 to 32FC1
	//								img.convertTo(dst, CV_8UC1);

	//								// cout << currentDateTime()  <<  "Image type: " << getImgType(dst.type()) << endl;

	//								vector<Rect> found, found_filtered;
	//								hog.detectMultiScale(dst, found, 0, Size(8,8), Size(32,32), 1.05, 2);									
 //
	//								if(found.size()) {
	//									cout << currentDateTime()  <<  "PERSON FOUND! Count: " << found.size() << endl;

	//									size_t i, j;
	//									for (i=0; i<found.size(); i++) {
	//										Rect r = found[i];
	//										for (j=0; j<found.size(); j++)
	//											if (j!=i && (r & found[j])==r)
	//												break;
	//										if (j==found.size())
	//											found_filtered.push_back(r);
	//									}
	//									for (i=0; i<found_filtered.size(); i++) {
	//										Rect r = found_filtered[i];
	//											r.x += cvRound(r.width*0.1);
	//										r.width = cvRound(r.width*0.8);
	//										r.y += cvRound(r.height*0.06);
	//										r.height = cvRound(r.height*0.9);
	//										rectangle(dst, r.tl(), r.br(), cv::Scalar(0,255,0), 2);
	//									}
	//	
	//									imshow("peopleDetector", dst);

	//									/*if (waitKey(20) >= 0)
	//										break;*/
	//									waitKey();

	//								} else {
	//									cout << currentDateTime()  <<  "NO PERSON FOUND! " << endl;
	//								}									
	//							}								
	//				}
	//		}
	//	}
	//}
	//catch (const filesystem_error& ex)
	//{
	//	cout << currentDateTime() << ex.what() << '\n';
	//}

   /* while (true)
    {

	}*/
	getchar();
    return 0;
}