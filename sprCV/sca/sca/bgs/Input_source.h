#ifndef Input_source_H
#define Input_source_H

#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>


class Input_source{
public:

	//-----------Functions-----------
	Input_source();

	~Input_source();

	int menu();

	void Im_seq_sel();

	void Vid_sel();

	void WebCam_sel();

	void Im_seq_info(int cat_sel, int vid_id);

	cv::Mat ReadIm(int i);

	//-----------Variables-----------

	std::vector<std::vector<int>> Im_seqs;	

	char imgpath[200], gtpath[200], save_path[200], vidname[50], ext[5], gtext[5], vpath[200];

	int inifr, endfr, fr_idx;

	std::vector<int> sq;

	std::vector<int> cat;

private:

	char fr[5];

};
#endif