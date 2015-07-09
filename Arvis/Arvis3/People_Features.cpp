#include "People_Features.h"


People_Features::People_Features(){


}

People_Features::People_Features(Mat iniFr)
{
	//----------------------Initialize variables----------------------
	im_r = iniFr.rows;
	im_c = iniFr.cols;

	double seed = time(NULL);	//create seed
	srand (seed);				//set random

	//Cast precomputed variables from arrays to Mat (Size is truncated: (cols, rows))
	Map_W = Pointer2Mat(&Mat_W, Size(200,3780));

	Mu = Pointer2Mat(&Mu_zscore, Size(3780,1));

	Dev = Pointer2Mat(&Dev_zscore, Size(3780,1));

	Alpha = Pointer2Mat(&alpha, Size(2505,1));

	Support_vecs = Pointer2Mat(&SupportVectors, Size(200,2505));

	size_t beta_count = (&bias_size - &bias)/sizeof(float);

	memcpy(&beta, &bias, beta_count*sizeof(float));	

	size_t sigma_count = (&sigma_size - &sigma)/sizeof(float);

	memcpy(&sigmak, &sigma, sigma_count*sizeof(float));	

	//----------------------Initialize parameters----------------------
	pminArea = 0.0005;
	minArea = int(pminArea*float(im_r*im_c));  //Minimum area to consider a region
	
	std_people_map = 0.15;		//0.15 according the mapped width and height

	ppl_max_iter = 15;

	svm_thr = 1;

	Fhog1 = fopen("No_Person.txt","w+");

}

People_Features::~People_Features()
{
	fclose(Fhog1);
}

//-------------------------------------------------------------------------------------
//Get bbox coordinates of F_ROIs and store into register
//-------------------------------------------------------------------------------------
void People_Features::Get_PROIs(Mat Frame, Mat Fmask)
{
	Im = Frame.clone();

	int Nroi;

	Nroi = Bwlabel(Fmask,Rmask);	//get regions

	int min_x, max_x, min_y, max_y, wid, heig, ctr_x, ctr_y;
	vector<int> Fs_states, P_aprox_size;
	vector<float> Sample;

	float svm_val, svm_val_A, svm_val_B;

	int max_iter, iter, Parea, Farea, disa;
		

	bool svm_l, svm_l_A, svm_l_B;

	//clear global PROI and FROI memory
	PF_states.clear();
	PF_states_A.clear();
	PF_states_B.clear();
	F_states.clear();

	for (int i=0; i<Nroi;i++)
	{
		//Find the corners of the bounding boxes that cover each moving object
		min_x = *std::min_element(Xcroi[i].begin(),Xcroi[i].end());   
		max_x = *std::max_element(Xcroi[i].begin(),Xcroi[i].end());
		min_y = *std::min_element(Ycroi[i].begin(),Ycroi[i].end());
		max_y = *std::max_element(Ycroi[i].begin(),Ycroi[i].end());
		wid = max_x-min_x;
		heig = max_y-min_y;
		ctr_x = min_x+int(floor(double(wid)/2.));
		ctr_y = min_y+int(floor(double(heig)/2.));

		//Copy bbox coordinates into registers
		Fs_states.clear();
		Fs_states.push_back(min_x);
		Fs_states.push_back(min_y);
		Fs_states.push_back(wid);
		Fs_states.push_back(heig);
		Fs_states.push_back(ctr_x);
		Fs_states.push_back(ctr_y);

		F_states.push_back(Fs_states);	//Store FROI states in global memory

		//visualization of FROIs bbox---------------------------------
		/*Point ul;
		Point lr;
		Mat Im2 = Im.clone();
		ul = Point(Fs_states[0],Fs_states[1]);
		lr = Point(Fs_states[0]+Fs_states[2],Fs_states[1]+Fs_states[3]);
		rectangle(Im2,ul,lr,Scalar(250,0,0),2);
		imshow("Rect1",Im2);
		waitKey();*/
		//------------------------------------------------------------

		PC_states.clear();
		PC_states_A.clear();
		PC_states_B.clear(); //prueba
		
		//Get the max number of iterations according to the FROI size and its relative position
		iter = 0;

		P_aprox_size = People_map(Fs_states[1], max_y);

		Parea = P_aprox_size[0]*P_aprox_size[1];
		Farea = Fs_states[2]*Fs_states[3];

		if(Parea>=Farea)
			max_iter = 1;
		else
			max_iter = floor(double(ppl_max_iter)*double(Farea)/double(Parea));

		//double T1=0, T2=0, T3=0;

		//Search for PROI en FROI. The search stops when  the max_iter is reached
		while(iter<=max_iter)
		{
			svm_l = false;
			svm_l_A = false;
			svm_l_B = false;
			
			//clock_t ini1 = clock();
			svm_val = People_Automatic(Fmask, Fs_states, Xcroi[i], Ycroi[i]);
			//T1 += ((double)clock() - ini1)/CLOCKS_PER_SEC;

			//clock_t ini2 = clock();
			svm_val_A = People_Aleatory(Fmask, Fs_states, Xcroi[i], Ycroi[i]);
			//T2 += ((double)clock() - ini2)/CLOCKS_PER_SEC;

			//clock_t ini3 = clock();
			svm_val_B = People_Background(Fmask);
			//T3 += ((double)clock() - ini3)/CLOCKS_PER_SEC;

			if(svm_val>svm_thr)
			{
				svm_l = true;

			}

			if(svm_val_A>svm_thr)
				svm_l_A = true;

			if(svm_val_B>svm_thr)
				svm_l_B = true;

			iter++;
			
			//cout<<"S:"<<svm_val<<" l:"<<svm_l<<" i:"<<iter<<"/"<<max_iter<<" fa:"<<Farea<<" pa:"<<Parea<<endl;

			//imshow("D1",Ppos);
			//waitKey();

			if(svm_l)
			{
				PC_states.push_back(make_tuple(svm_val,P_states));
			}
			if(svm_l_A)
			{
				PC_states_A.push_back(make_tuple(svm_val_A,P2_states));
			}
			if(svm_l_B)
			{
				PC_states_B.push_back(make_tuple(svm_val_B,P3_states));
			}

		}

 		disa=1;
		GroupPROI(PC_states,disa);
		disa=2;
		GroupPROI(PC_states_A,disa);
		disa=3;
		GroupPROI(PC_states_B,disa);
	}
	showDetections(PF_states);
	showDetections(PF_states_A);
	showDetections(PF_states_B);
	
	//Get_Pmask(PF_states, Fmask);
}
float People_Features::People_Automatic(Mat Fm,vector<int> Fs_states1,vector<int> Xcroi1,vector<int> Ycroi1){
	float svm_val;
			
	Mat P, Ppos, MSample;

	vector<float> Sample;

	P = ROIsearch(Fm, Fs_states1, Xcroi1, Ycroi1);

	Ppos = ImPreprocess(P);

	Sample = HOG_compute(Ppos);

	MSample = Projection(Sample);

	svm_val = SVM_Classify(MSample);


	return svm_val;
}

float People_Features::People_Background(Mat Fm){
	int wid, heig, min_h, max_h, min_w, max_w, c_x, c_y, v_w, v_h;
	min_w = 15;
	max_w = 100;
	min_h = 45;
	max_h=200;

	float svm_val;
			
	Mat P, Ppos, MSample;

	vector<float> Sample;

	c_x = rand() % Fm.cols + 1 ;
	c_y = rand() % Fm.rows + 1 ;
	v_h = max_h - min_h;
	heig = rand() %  v_h + min_h;
	v_w = max_w - min_w;
	wid = rand() % v_w + min_w;

	while ((c_x+wid)>Fm.cols)
		c_x=rand() % Fm.cols + 1 ;
	while ((c_y+heig)>Fm.rows)
		c_y=rand() % Fm.rows + 1 ;


	int ctr_x = c_x+int(floor(double(wid)/2.));
	int ctr_y = c_y+int(floor(double(heig)/2.));

	P3_states.clear();
	P3_states.push_back(c_x);
	P3_states.push_back(c_y);
	P3_states.push_back(wid);
	P3_states.push_back(heig);
	P3_states.push_back(ctr_x);
	P3_states.push_back(ctr_y);

	Mat At(Im,Rect(P3_states[0],P3_states[1],P3_states[2],P3_states[3]));

	P = At.clone();

	Ppos = ImPreprocess(P);

	Sample = HOG_compute(Ppos);

	MSample = Projection(Sample);

	svm_val = SVM_Classify(MSample);
	return svm_val;
}

float People_Features::People_Aleatory(Mat Fm, vector<int> Fs_states1, vector<int> Xcroi1, vector<int> Ycroi1){
	int wid, heig, min_w, max_w, min_h, max_h, max_y, c_y, c_x, v_x, v_w, v_h;

	//minimum and maximum height and width values for a person
	max_y=Fs_states1[1]+Fs_states1[3];
	min_w = 15;
	max_w = 100;
	min_h = 45;
	max_h = (200<(im_r-Fs_states1[1]-4))?200:max_y-Fs_states1[1];
	max_h = (max_h<min_h)?50:max_h;

	float svm_val;
			
	Mat Ppos, MSample;

	vector<float> Sample;

	if (Fs_states1[2]<=min_w)
	{
		wid = Fs_states1[2];
		c_x = Fs_states1[0];
	}
	else
	{
		v_x = Fs_states1[2]-min_w;
		c_x = rand() % v_x +  Fs_states1[0];
		v_w = max_w - min_w;
		wid = rand() % v_w + min_w;
		int kk = 0;
		while (c_x+wid>Fs_states1[2]+Fs_states1[0])
		{
			kk++;
			c_x = rand() % v_x +  Fs_states1[0];
			wid = rand() % v_w + min_w;
			if(kk>5)
			{
				wid = Fs_states1[2]+Fs_states1[0]-c_x;
			}
		}
	}
	if (Fs_states1[3]<=min_h)
	{
		heig = Fs_states1[3];
		c_y = Fs_states1[1];
	}
	else
	{
		v_h= max_h - min_h;
		c_y = rand() % min_h +  Fs_states1[1];
		heig = rand() % v_h + min_h;
		int kl = 0;
		while (c_y+heig>Fs_states1[1]+Fs_states1[3])
		{
			c_y = rand() % min_h +  Fs_states1[1];
			heig = rand() % v_h + min_h;
			kl++;
			if(kl>5)
			{
				heig = Fs_states1[1]+Fs_states1[3]-c_y;
			}
		}
	}
		
	//P = ROIsearch(Fm, Fs_states1, Xcroi1, Ycroi1);
	int ctr_x = c_x+int(floor(double(wid)/2.));
	int ctr_y = c_y+int(floor(double(heig)/2.));

	P2_states.clear();
	P2_states.push_back(c_x);
	P2_states.push_back(c_y);
	P2_states.push_back(wid);
	P2_states.push_back(heig);
	P2_states.push_back(ctr_x);
	P2_states.push_back(ctr_y);
	
	Mat At(Im,Rect(P2_states[0],P2_states[1],P2_states[2],P2_states[3]));

	Mat P = At.clone();

	Ppos = ImPreprocess(P);

	Sample = HOG_compute(Ppos);

	MSample = Projection(Sample);

	svm_val = SVM_Classify(MSample);

	return svm_val;
}



//-------------------------------------------------------------------------------------
//Find in F_ROIs P_ROIs that correspond to a person 
//-------------------------------------------------------------------------------------
Mat People_Features::ROIsearch(Mat F, vector<int> F_states, vector<int> Xc, vector<int> Yc)
{
	int cy, cx, wid, heig;

	vector<int> tamF, tamP;
	
	//cout<<endl<<"x"<<F_states[0]<<" y"<<F_states[1]<<" w"<<F_states[2]<<" h"<<F_states[3]<<endl;
	
	//check if the size of F_ROI is greater than the size according to the people_map,
	//if greater search P_ROIs in it, if lower assign P_ROI with F_ROI

	tamF = People_map(F_states[1], F_states[1]+F_states[3]);	

	if(tamF[1]>F_states[3])//check height
	{	
		heig = F_states[3];
		cy = F_states[1];
	}
	else
	{
		cy = Row_coord(F_states);	//randomly get upper left P_ROI coordinate
		tamP = People_map(cy, F_states[1]+F_states[3]);	
		heig = tamP[1];
	}

	//cout<<"y"<<cy<<" h"<<heig<<endl;

	if(tamF[0]>F_states[2])//check width
	{ 
		wid = F_states[2];
		cx = F_states[0];
	}
	else
	{
		tamP = People_map(cy, F_states[1]+F_states[3]);	
		wid = tamP[0];
		cx = Col_coord(Yc, Xc, cy, tamP[0], F_states);
	}

	//cout<<"x"<<cx<<" w"<<wid<<endl;

	//validate width
	if(cx+wid>im_c)
		wid = im_c-cx-1;

	//validate height
	if(cy+heig>im_r)
		heig = im_r-cy-1;

	//Centroid coordinates
	int ctr_x = cx+int(floor(double(wid)/2.));
	int ctr_y = cy+int(floor(double(heig)/2.));


	P_states.clear();

	P_states.push_back(cx);			//left x coordinate
	P_states.push_back(cy);			//top y coordinate
	P_states.push_back(wid);		//width
	P_states.push_back(heig);		//height
	P_states.push_back(ctr_x);		//centroid x coordinate
	P_states.push_back(ctr_y);		//centroid y coordinate

	//Extract ROI
	Mat At(Im,Rect(P_states[0],P_states[1],P_states[2],P_states[3]));
	


	Mat P = At.clone();
		
	//visualization
	/*Point ul;
	Point lr;

	Mat Im1 = Im.clone();

	ul = Point(P_states[0],P_states[1]);
	lr = Point(P_states[0]+P_states[2],P_states[1]+P_states[3]);

	rectangle(Im1,ul,lr,Scalar(250,0,0),2);

	imshow("Rect2",Im1);*/

	return P;
}

//-------------------------------------------------------------------------------------
//Preprocess input ROI candidate image to characterize with HOG by rescaling
//-------------------------------------------------------------------------------------
Mat People_Features::ImPreprocess(Mat A)
{
	resize(A,PIm,Size(64,128));

	return PIm;
}

//-------------------------------------------------------------------------------------
//Compute HOG features for preprocessed image ROI
//-------------------------------------------------------------------------------------
vector<float> People_Features::HOG_compute(Mat A)
{
	HOGDescriptor hog;

	vector<float> dvals;
	vector<Point> locs;

	hog.compute(A,dvals,Size(8,8),Size(0,0),locs);

	return dvals;
}

//-------------------------------------------------------------------------------------
//Normalize and project new sample to low dimensional space using maping matrix Map_W
//-------------------------------------------------------------------------------------
Mat People_Features::Projection(vector<float> Sample)
{
	Mat Sample_mat, Pj_Sample_mat;

	Sample_mat = Vector2Mat(Sample);	//Convert std vector to Mat

	Sample_mat = Sample_mat-Mu;			//Centralize subtracting the mean

	divide(Sample_mat,Dev,Sample_mat);	//Normaliza dividing by standard deviation

	Pj_Sample_mat = Sample_mat*Map_W;		//Sample projection using Mat per Mat multiplication

	//Pj_Sample = Mat2Vector(Pj_Sample_mat);	//Convert Mat to vector to return

	return Pj_Sample_mat;
}

//-------------------------------------------------------------------------------------
//Evaluate new Sample into SVM classifier to get label
//-------------------------------------------------------------------------------------
float People_Features::SVM_Classify(Mat A)
{
	float Gkernel, sumSVM = 0;

	for(int i=0; i<Support_vecs.rows; i++){

		Gkernel = GKernel_Mats(A, Support_vecs.row(i));

		sumSVM = Alpha.at<float>(0,i)*Gkernel + sumSVM;
	}
	
	sumSVM = sumSVM+beta;

	return sumSVM;
}

//-------------------------------------------------------------------------------------
//Evaluate new Sample into OpenCV SVM classifier to get label
//-------------------------------------------------------------------------------------
bool People_Features::SVM_ClassifyCV(Mat A)
{
	HOGDescriptor hog;
	vector<float> HOG_d = HOGDescriptor::getDefaultPeopleDetector();
	hog.setSVMDetector(HOG_d);

	vector<Point> found;

	bool label = false;

	hog.detect(A, found, 0, Size(8,8), Size(0,0));

	if(found.size()>0)
		label = true;

	//showDetections(found,A);
	//waitKey(100);

	return label;
}

//-------------------------------------------------------------------------------------
//Evaluate new Sample into OpenCV SVM classifier to get label
//-------------------------------------------------------------------------------------
void People_Features::SVM_ClassifyCV_Multi(Mat A, vector<vector<int>> &StatesCV){

	HOGDescriptor hog;
	vector<float> HOG_d = HOGDescriptor::getDefaultPeopleDetector();
	hog.setSVMDetector(HOG_d);

	vector<Rect> found;

	bool label = false;

	hog.detectMultiScale(A, found, 0, Size(8,8), Size(0,0));

	vector<int> aux;
	for(int i=0; i<found.size(); i++)
	{
		aux.clear();
		aux.push_back(found[i].x);
		aux.push_back(found[i].y);
		aux.push_back(found[i].width);
		aux.push_back(found[i].height);

		StatesCV.push_back(aux);
	}

	showDetections(found,A);
	waitKey(100);
}

//-------------------------------------------------------------------------------------
//Given a set of candidate PROIs, merge the close ones and pick the ones with the highest values
//-------------------------------------------------------------------------------------
void People_Features::GroupPROI(vector<P_tuple> P_states_c, int disa )
{
	float dist;

    int i = 0, lim = P_states_c.size(), j;
	
	while(i<lim)
	{
		j = i+1;

		while(j<lim)
		{
			//test
			/*Point ul;
			Point lr;

			Mat Im1 = Im.clone();

			ul = Point(get<1>(P_states_c[i])[0],get<1>(P_states_c[i])[1]);
			lr = Point(get<1>(P_states_c[i])[0]+get<1>(P_states_c[i])[2],get<1>(P_states_c[i])[1]+get<1>(P_states_c[i])[3]);

			rectangle(Im1,ul,lr,Scalar(250,0,0),2);

			ul = Point(get<1>(P_states_c[j])[0],get<1>(P_states_c[j])[1]);
			lr = Point(get<1>(P_states_c[j])[0]+get<1>(P_states_c[j])[2],get<1>(P_states_c[j])[1]+get<1>(P_states_c[j])[3]);

			rectangle(Im1,ul,lr,Scalar(0,255,0),2);

			imshow("Rect1",Im1);

			waitKey(10);*/

			//Distance between PROI candidates i and j 
 			dist = sqrt(pow(double(get<1>(P_states_c[i])[4]-get<1>(P_states_c[j])[4]),2)+pow(double(get<1>(P_states_c[j])[5]-get<1>(P_states_c[j])[5]),2));

			//If PROI i and j are close, delete the PROI with the lowest svm value
			if(dist<15)
			{
				if(get<0>(P_states_c[j])>get<0>(P_states_c[i]))
				{
					P_states_c.erase(P_states_c.begin()+i);
					lim = P_states_c.size();
					i--;
					break;
				}
				else
				{
					P_states_c.erase(P_states_c.begin()+j);
					lim = P_states_c.size();
					j--;
				}
			}
			j++;
		}
		i++;
	}

	/*Point ul;
	Point lr;
	Mat Im1 = Im.clone();*/
	for(int k=0; k<P_states_c.size();k++)
	{
		//copy detected PROI to global memory
		if (disa==1)
			PF_states.push_back(P_states_c[k]);
		if (disa==2)
			PF_states_A.push_back(P_states_c[k]);
		if (disa==3)
			PF_states_B.push_back(P_states_c[k]);

		/*ul = Point(get<1>(P_states_c[k])[0],get<1>(P_states_c[k])[1]);
		lr = Point(get<1>(P_states_c[k])[0]+get<1>(P_states_c[k])[2],get<1>(P_states_c[k])[1]+get<1>(P_states_c[k])[3]);
		rectangle(Im1,ul,lr,Scalar(0,255,0),2);
		imshow("People Det",Im1);
		waitKey();*/
		//cout<<endl<<"SVM_score:"<<get<0>(P_states_c[k]);
	}
	
}

//-------------------------------------------------------------------------------------
//Label and extract isolated regions from foreground mask
//-------------------------------------------------------------------------------------
int People_Features::Bwlabel(Mat Fmask, Mat &Out){
	
	Mat cFmask = Fmask.clone();

	Erosion(cFmask ,cFmask);

	Mat A = cFmask.clone(); 

	findContours(A, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	int Nroi = 0;

	//Variables to store contours coordinates of each isolated region 
	Xcroi.clear();
	Ycroi.clear();
	std::vector<int> X_temp;
	std::vector<int> Y_temp;

	for(int i=0; i<contours.size(); i++ )
	{
		pt.x = contours[i][0].x;
		pt.y = contours[i][0].y;
		if(contours[i].size()>=minArea)
		{ 
			Nroi++;
			floodFill(cFmask,pt, cv::Scalar::all(Nroi));

			X_temp.clear();
			Y_temp.clear();

			//store contours coordinates
			for(int j=0; j<contours[i].size();j++){
				X_temp.push_back(contours[i][j].x);
				Y_temp.push_back(contours[i][j].y);
			}
			Xcroi.push_back(X_temp);
			Ycroi.push_back(Y_temp);
		}
		else
		{
			floodFill(cFmask,pt, cv::Scalar::all(0));
		}
 	}
	//Imagesc(cFmask,Nroi,"test2");
	//cv::waitKey();
	Out = cFmask.clone();

	return Nroi;
}

//-------------------------------------------------------------------------------------
//Erosion
//-------------------------------------------------------------------------------------
void People_Features::Erosion(cv::Mat src, cv::Mat des, int erosion_size)
{
	int erosion_elem = 2;
	int erosion_type;

	if( erosion_elem == 0 ){ erosion_type = cv::MORPH_RECT; }
	else if( erosion_elem == 1 ){ erosion_type = cv::MORPH_CROSS; }
	else if( erosion_elem == 2) { erosion_type = cv::MORPH_ELLIPSE; }

	cv::Mat element = cv::getStructuringElement( erosion_type,
	cv::Size( 2*erosion_size + 1, 2*erosion_size+1 ),
	cv::Point( erosion_size, erosion_size ) );
	/// Apply the erosion operation
	erode( src, des, element );
}

//-------------------------------------------------------------------------------------
//Imagesc
//-------------------------------------------------------------------------------------
void People_Features::Imagesc(cv::Mat src, int siz, char* Plotname)
{
	//Color Directory
	cv::Mat output(src.rows,src.cols,CV_8UC3,CV_RGB(0,0,0));
	cv::Mat aux = src.clone();
	cv::Mat aux3;
	cv::Scalar v;
	for(int i=0; i<=siz; i++)
	{
		v.val[0]=i;
		compare(src,v,aux,cv::CMP_EQ);
		
		aux = aux/255;

		cv::Mat aux2[]= {aux*Color[i][0],aux*Color[i][1],aux*Color[i][2]};
		merge(aux2,3,aux3);
		output = aux3+output;
	}
	imshow(Plotname,output);
}

//-------------------------------------------------------------------------------------
//Given a F_ROI, randomly get the upper left row coordinate of P_ROI
//-------------------------------------------------------------------------------------
int People_Features::Row_coord(vector<int> F_states)
{
	int lim_inf, lim_sup, max_y, min_h_inf, cy;
	double b, m;

	min_h_inf = 100;	//minimum height for a person entering or leaving the scene a the bottom
	b = 23.5;			//linear equation parameter for height
	m = 0.9;			//linear equation parameter for height

	lim_sup = F_states[1];	//upper limit for random row search within F_ROI

	max_y = F_states[1]+F_states[3];	//lower limit of F_ROI
	
	if(max_y>=(im_r-5))
	{		//Find lower limit for random search according to max_y
		lim_inf = min_h_inf;	//set lower limit to min_h_inf
	}
	else{
		lim_inf = (int)ceil((max_y-b)/(1+m));
	}

	if (lim_inf<=lim_sup)
		lim_inf = lim_sup+1;

	cy = rand() % (lim_inf-lim_sup) + lim_sup;	//generate random 

	return cy;
}

//-------------------------------------------------------------------------------------
//Given a F_ROI and the mapped width, randomly get the upper left column coordinate of P_ROI
//-------------------------------------------------------------------------------------
int People_Features::Col_coord(vector<int> Yc, vector<int> Xc, int cy, int wid, vector<int> F_states)
{
	int cx, cx_min, cx_max;

	if(wid>=F_states[2])
		wid = F_states[2]-1;

	cx_min = F_states[0]-F_states[2]*std_people_map;
	
	if(cx_min<0)
		cx_min = 0;

	cx_max = F_states[0]+(1+std_people_map)*F_states[2]-wid;

	if(cx_max>im_c)
		cx_max = im_c-1;

	if(cx_min>=cx_max)
		cx_min = cx_max-1;
	
	cx = rand() % (cx_max-cx_min) + cx_min;

	return cx;
}

//-------------------------------------------------------------------------------------
//Given a row coordinate get the average height and width for a person in that coordinate
//-------------------------------------------------------------------------------------
vector<int> People_Features::People_map(int cy, int max_y)
{
	//object size map equations: width= mw*y+bw; height= (cy<chp)?mhu+y+bhu:mhl+y+bhl
	float mw, bw, mhu, bhu, mhl, bhl, chp;
	mw = 0.3;
	bw = 10;
	mhu = 0.9;
	bhu = 23.5;
	mhl = -0.92;
	bhl = 360;
	chp = 190;

	int wid, heig, min_w, max_w, min_h, max_h;

	//minimum and maximum height and width values for a person
	min_w = 15;
	max_w = 100;
	min_h = 45;
	max_h = (200<(im_r-cy-4))?200:max_y-cy;
	max_h = (max_h<min_h)?50:max_h;

	vector<int> siz;

	wid = mw*cy+bw;	//average width for cy coordinate

	wid = Randn_int(wid, wid*std_people_map, min_w, max_w);	//random width around average for cy 

	siz.push_back((int)wid);

	//average height for cy coordinate
	if(cy<chp)
	{
		heig = mhu*cy+bhu;
	}
	else
	{
		heig = mhl*cy+bhl;
	}

	heig = Randn_int(heig, heig*std_people_map, min_h, max_h);	//random height around average for cy 

	siz.push_back((int)heig);

	return siz;
}

//-------------------------------------------------------------------------------------
//Random positive number generator from normal distribution:

//Generates two random number in [0 1] from a uniform distribution using as seed the time, 
//afterwards, the Box-Muller is used to get a random number 'rn' from a normal distribution,
//'rn' must be grater than 'm_lim'

//Inputs:
//mean = Normal distribution mean
//std = Normal distribution std
//low_lim = low limit for random number
//high_lim	= high limit for a random number

//Output:
//rn = random number from normal distribution

int People_Features::Randn_int(int mean, double std, double low_lim, double high_lim)
{
	double rdm_u1, rdm_u2;
	double rn = low_lim;

	//Validate limits
	low_lim = low_lim >= high_lim ? high_lim-1: low_lim;

	int k = 0;
	while((rn<=low_lim)||(rn>=high_lim))
	{
		k++;
		rdm_u1 = (rand()%1000);		//get random number from uniform distribution
		rdm_u1 = rdm_u1/1000;

		rdm_u2 = (rand()%1000);		//get random number from uniform distribution
		rdm_u2 = rdm_u2/1000;

		if((rdm_u1>0)&(rdm_u2>0))
		{
			rn = sqrt(-2*log(rdm_u1))*sin(2*PI*rdm_u2);	//Box-Muller random number 
			rn = (rn*std)+mean;
		}
		if(k>5)		//avoid infinite loops trying to surpass minimum limit
		{
			if(rn<low_lim)
				rn = low_lim+1;
			else
				rn = high_lim-1;
			break;
		}
	}
	return int(floor(rn));
}

//-------------------------------------------------------------------------------------
//Given a row coordinate and a width find the minimum and maximum value to search a column
//coordinate
//-------------------------------------------------------------------------------------
vector<int> People_Features::find_vals_vec(vector<int> A, vector<int> B, int cy, int wid)
{
	vector<int> cx;

	int refmin=1000, refmax=0;

	bool found = false;

	for(int i=0; i<A.size(); i++)
	{
		if(A[i]==cy)
		{
			if(B[i]<refmin)
				refmin = B[i];
			if(B[i]>refmax)
				refmax = B[i];

			found = true;
		}
	}

	if(!found)
	{
		refmin = B[0];
		refmax = B[5];
	}

	refmin = refmin-(int)ceil(wid*0.2);
	refmax = refmax-(int)ceil(wid*0.8);

	cx.push_back(refmin);
	cx.push_back(refmax);

	return cx;
}


//-------------------------------------------------------------------------------------
//Calculate the Gaussian Kernel of the difference of Mats A-B 
//-------------------------------------------------------------------------------------
float People_Features::GKernel_Mats(Mat A, Mat B)
{
	float normv, Gkernel;

	Scalar norm;

	Mat C = A-B;

	multiply(C,C,C);

	norm = sum(C);

	normv = norm.val[0];

	Gkernel = exp(-normv/(2*pow(sigmak,2)));
	
	return Gkernel;
}

//-------------------------------------------------------------------------------------
//Convert uchar pointer array to opencv Mat 
//-------------------------------------------------------------------------------------
Mat People_Features::Pointer2Mat(unsigned char *A, Size A_size)
{
	Mat B(A_size,CV_32FC1, A);

	//cout<<" B:"<<B.at<float>(0,99);
	
	return B;
}

//-------------------------------------------------------------------------------------
//Convert std vector to opencv Mat to compute Mat per Mat multiplication
//-------------------------------------------------------------------------------------
Mat People_Features::Vector2Mat(vector<float> A)
{
	Mat B(1,A.size(),CV_32FC1);

	if(A.size()>0)
	{
		for(int i=0; i<A.size(); i++)
		{
			B.at<float>(0,i) = A[i];
		}
	}
	else
		cout<<"Sample size must be >0";

	return B;
}

//-------------------------------------------------------------------------------------
//Convert opencv Mat to std vector to evaluate projected sample into the SVM
//-------------------------------------------------------------------------------------
vector<float> People_Features::Mat2Vector(Mat A)
{
	vector<float> B;

	if(A.cols>0)
	{
		for(int i=0; i<A.cols; i++)
		{
			B.push_back(A.at<float>(1,i));
		}
	}
	else
		cout<<"Sample size must be >0";

	return B;
}

//-------------------------------------------------------------------------------------
//Multiplication of 3-channel array by a 1-channel array
//-------------------------------------------------------------------------------------
Mat People_Features::Mul3(Mat A3, Mat B1)
{
	cv::Mat Temp[3], D3;
	split(A3,Temp);
	for(int i=0; i<3; i++)
	{
		cv::multiply(Temp[i],B1,Temp[i]);
	}
	cv::merge(Temp,3,D3);
	return D3;
}

//-------------------------------------------------------------------------------------
//Write HOG features into txt
//-------------------------------------------------------------------------------------
void People_Features::writeHOG(Mat A)
{
	Fhog2 = fopen("SM.txt","w+");

	for(int i=0; i<A.cols; i++)
	{

		fprintf(Fhog2,"%f ",A.at<float>(0,i));
	}
	fprintf(Fhog2,"\n");
	fclose(Fhog2);
}

//-------------------------------------------------------------------------------------
//Write HOG features into txt
//-------------------------------------------------------------------------------------
void People_Features::writeHOG(vector<float> A)
{
	for(int i=0; i<A.size(); i++)
	{
		fprintf(Fhog1,"%f ",A[i]);
	}
	fprintf(Fhog1,"\n");
	
}


//-------------------------------------------------------------------------------------
//Show bounding box of detected PROIs
//-------------------------------------------------------------------------------------
void People_Features::showDetections(vector<P_tuple> PF_states)
{
	Point ul;
	Point lr;

	Mat Im1 = Im.clone();

	for(int k=0; k<PF_states.size();k++)
	{
		ul = Point(get<1>(PF_states[k])[0],get<1>(PF_states[k])[1]);
		lr = Point(get<1>(PF_states[k])[0]+get<1>(PF_states[k])[2],get<1>(PF_states[k])[1]+get<1>(PF_states[k])[3]);

		rectangle(Im1,ul,lr,Scalar(0,255,0),2);
	}
	imshow("People Det",Im1);
	waitKey(1);
}

//-------------------------------------------------------------------------------------
//Show bounding box of detected person
//-------------------------------------------------------------------------------------
void People_Features::showDetections(const vector<Rect>& found, Mat& imageData) {
    size_t i, j;
    
	for (i = 0; i < found.size(); ++i) {
        // Rect_(_Tp _x, _Tp _y, _Tp _width, _Tp _height);
        rectangle(imageData, found[i], Scalar(64, 255, 64), 3);
    }
	imshow("Detections:",imageData);
	//waitKey();
}