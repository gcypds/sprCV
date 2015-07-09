#include <stdio.h>
#include <opencv2/core/core.hpp>

using namespace cv;

typedef struct {
	int min_x, max_x, min_y, max_y, wid, heig, ctr_x, ctr_y;
} F_state_struct;

void getRegions(Mat F, std::vector<F_state_struct> &F_states, float pminArea);