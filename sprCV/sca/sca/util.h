#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <boost/filesystem.hpp>
#include <cv.h>
#include <highgui.h>
#include <vector>

using namespace cv;
using namespace std;
using namespace boost::filesystem;

const char *get_filename_ext(const char *filename);

IplImage* skipNFrames(CvCapture* capture, int n);

bool compareNat(const path& a, const path& b);

bool compare_filenames(path a, path b);

bool numericCompare(const path &s0p, const path &s1p);

const std::string currentDateTime();

#endif


