#include "Tracking_STIP.h"

//-------------------------------------------------------------------------------------
//Constructor
Tracking_STIP::Tracking_STIP(cv::Mat iniFr){
	
	im_r = iniFr.rows;
	im_c = iniFr.cols;


	//Parameters values
	pminArea = 0.0005;
	minArea = int(pminArea*float(im_r*im_c));  //Minimum area to consider a region

	thr_match = 10;

	ROI_srch_radius = float(im_r)*0.15;	//set as 15% the frame height (If the FPS is known set according it)

	STIP_srch_radius = float(im_r)*0.05; //set as 5% the frame height (If the FPS is known set according it)
	
	NStips_ROI = 50;

	NStips_match = 0.1;

	C_ROI_lrg = 0.1;

	STIP_updt_rate = int(ceil(float(NStips_ROI)*0.15));

	w_alpha = 0.1;

	w_ini = 0.1;

	M_STIPs_min = int(ceil(float(NStips_ROI)*0.3));

	s_beta = 0.1;

	hw_thr = 0.2;
}

//-------------------------------------------------------------------------------------
//Destructor
Tracking_STIP::~Tracking_STIP(){
}

//-------------------------------------------------------------------------------------
//Get bbox coordinates and store into registers
//-------------------------------------------------------------------------------------
void Tracking_STIP::getRegions(){
	
	Bwlabel(Fmask,Rmask);	//get regions

	int min_x, max_x, min_y, max_y, wid, heig, ctr_x, ctr_y;
	std::vector<int> Obj;
	Q_states.clear();

	for (int i=0; i<Nroi;i++){
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
		Obj.clear();
		Obj.push_back(min_x);
		Obj.push_back(min_y);
		Obj.push_back(wid);
		Obj.push_back(heig);
		Obj.push_back(ctr_x);
		Obj.push_back(ctr_y);
		Q_states.push_back(Obj);
	}
}

//-------------------------------------------------------------------------------------
//Initialize ROIs states and STIP models
//-------------------------------------------------------------------------------------
void Tracking_STIP::initROI(vector<vector<int>> Q_states, vector<vector<vector<float>>> Q_STIPs, bool ini){
	
	//If ini==true (Initialize tracking first time) initialize Model ROI STIPs and states using all the Query ROIs, 
	//if not use the Query ROIs that didnt match any Model ROI
	if(ini==true){
		Q_ROI_matched.assign(Q_STIPs.size(),false);	
	}
			
	deque<M_STIP_tuple> WSTIP;
	int rdm_n;

	//Loop through Query ROIs
	for (int i=0; i<Q_STIPs.size(); i++){	

		if(Q_ROI_matched[i]==false){

			M_states.push_back(Q_states[i]);	//Initialize ROI Model states

			WSTIP.clear();	//Initialize STIP vector

			int Q_STIPs_roi = Q_STIPs[i].size();	//available STIPs for Query ROI i

			//If there are more STIPs than the required -> randomly subsample, else -> fill the rest with zeros
			if(Q_STIPs_roi>NStips_ROI){

				for (int j=0; j<NStips_ROI; j++){

					rdm_n = rand() % Q_STIPs[i].size();	//get random index

					WSTIP.push_back(std::make_tuple(w_ini, kpts_fmask[i][rdm_n], Q_STIPs[i][rdm_n]));	//Fill with random STIP from Query

					Q_STIPs[i].erase(Q_STIPs[i].begin()+rdm_n);	//Delete used STIP
				}
			}
			else{
				for (int j=0; j<Q_STIPs_roi; j++){		//loop through Query Stips

					WSTIP.push_back(std::make_tuple(w_ini, kpts_fmask[i][j], Q_STIPs[i][j]));	//Fill vector with available STIPs
				}
			}
			M_STIPs.push_back(WSTIP);	//Store STIPs in ROI model
		}
	}
}


//-------------------------------------------------------------------------------------
//Search for each Model ROI a near Query ROI that matches it according to an STIP majority vote
//-------------------------------------------------------------------------------------
void Tracking_STIP::matchRegions(vector<vector<int>> Q_states, vector<vector<vector<float>>> Q_STIPs){

	float dist;	
	int Nm;			//Number of matched STIPs
	int Nm_ref;		//Reference of matched STIPs
	int QROIm = 0;	//Query ROI match

	vector<int> M_matches_ref;						//reference Model matches vector (see M_matches description)
	Q_ROI_matched.assign(Q_STIPs.size(),false);		//Matched Query ROIs

	//Reinitialize Candidate ROI states and STIPs

	for(int i=0; i<M_states.size();i++){	//loop through Model ROIs

		Nm_ref = 0;		//Initialize reference number of matches

		for(int j=0; j<Q_states.size();j++){	//loop through Query ROIs

			//Distance between Model ROI [i] centroid and Query [j] ROI centroid
			dist = sqrt(pow(double(M_states[i][4]-Q_states[j][4]),2)+pow(double(M_states[i][5]-Q_states[j][5]),2));

			//Check if the distance is lower than a fixed search radius
			if (dist<ROI_srch_radius){

				//March STIPs
				Nm = matchSTIPs(M_STIPs[i],Q_STIPs[j],kpts_fmask[j]);	

				if (Nm>Nm_ref){

					Nm_ref = Nm;	//Set new reference
					QROIm = j;		//Set new Query ROI match
					M_matches_ref = M_matches;	//Set new Query matches vector 
				}
			}
		}
		std::cout<<std::endl<<"M:"<<i<<" Q:"<<QROIm<<" Mn:"<<M_STIPs[i].size()<<" Qn:"<<Q_STIPs[QROIm].size()<<" Nm:"<<Nm_ref;
		if(Nm_ref>NStips_match){

			//Assign true label to Query ROI QROIm as matched to one Model ROI 
			Q_ROI_matched[QROIm] = true;

			//DrawRects(M_states[i], Q_states[QROIm], "A", "B");

			//Get Candidate ROI states and STIPs
			getCandidateROI(kpts_fmask[QROIm], Q_STIPs[QROIm], Q_states[QROIm], M_matches_ref,i);	//Get Candidate ROI states and STIPs

			//Update Model ROI states and STIPs
			UpdtModelROI(i, C_states, C_STIPs, M_matches, kpts_fmask[QROIm]);
		}
	}
}

//-------------------------------------------------------------------------------------
//Match stip features from a query set
//-------------------------------------------------------------------------------------
int Tracking_STIP::matchSTIPs(deque<M_STIP_tuple> Model, vector<vector<float>> Query, vector<cv::KeyPoint> kpts_fmask){
	
	vector<int> Model_matches (Model.size(),-1);	//Indices of Query STIPs that match Model STIPs (if found any) 
	
	float difval;
	float refval;
	bool match_found;
	int Nmatches = 0;	//Number of matched STIPs
	int STIP_dist;

	for(int i=0; i<Model.size(); i++){

		refval = 1000;			//initialize reference matching value
		match_found = false;	//initialize match flag

		for(int j=0; j<Query.size();j++){

			//check if the current index j has already taken into Query_matches
			std::vector<int>::iterator it = std::find (Model_matches.begin(), Model_matches.end(), j);	
			if (it == Model_matches.end()){ //If not taken calculate match

				//distance between M_STIP i and Q_STIP j centroids
				STIP_dist = sqrt(pow(std::get<1>(Model[i]).pt.x-kpts_fmask[j].pt.x,2)+pow(std::get<1>(Model[i]).pt.y-kpts_fmask[j].pt.y,2));

				if(STIP_dist<STIP_srch_radius){	//if the distance is lower than a searching threshold calculate 2norm

					difval = Norm_vectors(std::get<2>(Model[i]), Query[j]);	//2norm between Query[i] and Model[j]

					if((difval<thr_match)&(difval<refval)){		//Validate the matching between Query[i] and Model[j]

						refval = difval;		//assign new reference matching value
						Model_matches[i] = j;	//assign Model j index to Query i
						match_found = true;		//match found true
					}
				}
			}
		}
		if(match_found){

			Nmatches++;		//Increase number of matched STIPs
		}
	}
	M_matches = Model_matches; //Make copy to class variable

	return Nmatches;
}

//-------------------------------------------------------------------------------------
//From the Query ROI matched to a Model ROI find a Candidate ROI
//-------------------------------------------------------------------------------------
void Tracking_STIP::getCandidateROI(vector<cv::KeyPoint> kpts_fmask, vector<vector<float>> Q_STIPs, vector<int> Q_states, vector<int> M_matches_ref, int M_idx){

	C_states.clear();
	C_STIPs.clear();

	//Check if the matched Query ROI size is similar to the Model ROI size to assign it
	bool cond_w = abs(M_states[M_idx][2]-Q_states[2])<(int)ceil((float)M_states[M_idx][2]*hw_thr);
	bool cond_h = abs(M_states[M_idx][3]-Q_states[3])<(int)ceil((float)M_states[M_idx][3]*hw_thr);
	bool cond_s = ((M_states[M_idx][2]-Q_states[2])+(M_states[M_idx][3]-Q_states[3]))>0;

	//Check if the M_STIPs has too few features to describe the ROI
	bool cond_min = M_STIPs[M_idx].size()<M_STIPs_min;

	std::cout<<std::endl<<"Mw:"<<M_states[M_idx][2]<<" Mh:"<<M_states[M_idx][3]<<" Qw:"<<Q_states[2]<<" Qh:"<<Q_states[3]<<" Cw:"<<cond_w<<" Ch:"<<cond_h;

	if(cond_w&cond_h){
		C_states = Q_states;

		for(int i=0; i<kpts_fmask.size(); i++){
					//Make candidate STIP with Query STIP i information and initial weight w_ini
					C_STIPs.push_back(std::make_tuple(w_ini, kpts_fmask[i], Q_STIPs[i]));
		}
		std::cout<<"f: Q";
	}
	else{ //If the conditions above are not fulfilled calculate Candidate STIPs according to Q_STIPs coordinates that matched

		std::cout<<"f: C";
		//Vectors to store the x and y coordinates of the matched Query STIPs
		vector<int> Kpts_x;
		vector<int> Kpts_y;

		//Loop through Model matches vector
		for(int i=0; i<M_matches_ref.size(); i++){

			//If for Model ROI i there is a Query STIP match, store the Query Stip coordinates
			if(M_matches_ref[i]!=-1){
				Kpts_x.push_back(kpts_fmask[M_matches_ref[i]].pt.x);
				Kpts_y.push_back(kpts_fmask[M_matches_ref[i]].pt.y);
			}
		}

		int min_x, max_x, min_y, max_y, wid, heig, aux, ctr_x, ctr_y;

		//Find Candidate ROI states according to matched Query STIP coordinates---------
		min_x = *std::min_element(Kpts_x.begin(),Kpts_x.end());   
		max_x = *std::max_element(Kpts_x.begin(),Kpts_x.end());
		min_y = *std::min_element(Kpts_y.begin(),Kpts_y.end());
		max_y = *std::max_element(Kpts_y.begin(),Kpts_y.end());
		wid = max_x-min_x;
		heig = max_y-min_y;

		//Enlarge Candidate ROI bounding box---

		//min_x enlargement
		aux = min_x - int(ceil(C_ROI_lrg*wid));
		if(aux>=0)
			min_x = aux;
		else
			min_x = 0;

		//min_y enlargement
		aux = min_y - int(ceil(C_ROI_lrg*heig));
		if(aux>=0)
			min_y = aux;
		else
			min_y = 0;

		//wid and max_x enlargement
		aux = wid + int(ceil(2*C_ROI_lrg*wid));
		if((aux+min_x)<im_c)
			wid = aux;
		else
			wid = im_c-min_x-1;
		max_x = min_x+wid;

		//heig and max_y enlargement
		aux = heig + int(ceil(2*C_ROI_lrg*heig));
		if((aux+min_y)<im_r)
			heig = aux;
		else
			heig = im_r-min_y-1;
		max_y = min_y+heig;

		ctr_x = min_x+int(floor(double(wid)/2.));
		ctr_y = min_y+int(floor(double(heig)/2.));

		C_states.push_back(min_x);
		C_states.push_back(min_y);
		C_states.push_back(wid);
		C_states.push_back(heig);
		C_states.push_back(ctr_x);
		C_states.push_back(ctr_y);


		//Find STIPs from Query ROI that are within Candidate ROI states---------

		for(int i=0; i<kpts_fmask.size(); i++){

			if((kpts_fmask[i].pt.x>min_x)&(kpts_fmask[i].pt.x<max_x)&(kpts_fmask[i].pt.y>min_y)&(kpts_fmask[i].pt.y<max_y)){

				//Store in C_STIPs only the ones that didn't match any M_STIP but are within C_states,
				//for the M_STIPs that matched their weights are incremented
				std::vector<int>::iterator it = std::find (M_matches.begin(), M_matches.end(), i);	

				if (it == M_matches.end()){
					//Make candidate STIP with Query STIP i information and initial weight w_ini
					C_STIPs.push_back(std::make_tuple(w_ini, kpts_fmask[i], Q_STIPs[i]));
				}
			}
		}
	}
	//DrawRects(M_states[M_idx], C_states, "A", "C");
}

//-------------------------------------------------------------------------------------
//Sort Model STIPs according to their weights
//-------------------------------------------------------------------------------------
void Tracking_STIP::SortSTIPs(deque<M_STIP_tuple> M_STIPs){
	
		//Sort M_STIP_tuple according the weights
		std::sort(M_STIPs.begin(),M_STIPs.end(),
			[](const  M_STIP_tuple& a,
			const  M_STIP_tuple& b) -> bool
		{
			return std::get<0>(a) > std::get<0>(b);
		});
}

//-------------------------------------------------------------------------------------
//Update Model ROI M_idx states STIPs according to Candidate ROI
//-------------------------------------------------------------------------------------
void Tracking_STIP::UpdtModelROI(int M_idx, vector<int> C_states, vector<M_STIP_tuple> C_STIPs, vector<int> M_matches, vector<cv::KeyPoint> kpts_fmask){

	//Update states----------------------------
	M_states[M_idx][2] = s_beta*C_states[2]+(1-s_beta)*M_states[M_idx][2];
	M_states[M_idx][3] = s_beta*C_states[3]+(1-s_beta)*M_states[M_idx][3];
	M_states[M_idx][4] = C_states[4];
	M_states[M_idx][5] = C_states[5];
	M_states[M_idx][0] = C_states[4]-int(floor(float(M_states[M_idx][2])/2.));
	M_states[M_idx][1] = C_states[5]-int(floor(float(M_states[M_idx][3])/2.));


	//Update STIPs weights and coordinates------
	for(int i=0; i<M_STIPs[M_idx].size();i++){
		if(M_matches[i]!=-1){
			//weights
			std::get<0>(M_STIPs[M_idx][i]) = std::get<0>(M_STIPs[M_idx][i])+w_alpha;

			//coordinates
			std::get<1>(M_STIPs[M_idx][i]) = kpts_fmask[M_matches[i]];
		}
	}

	//Update STIPs----------------------------

	//Sort Model ROI M_idx STIPs according its weights
	SortSTIPs(M_STIPs[M_idx]);

	int STIPs_replaced = 0, rdm_n;

	std::cout<<" C:"<<C_STIPs.size()<<std::endl;

	while(C_STIPs.size()>0){

		rdm_n = rand() % C_STIPs.size();	//get random index

		//If the Model has all the NStips_ROI STIPs, randomly replace STIP_updt_rate STIPs with the lowest weight with Candidate STIPs
		if(M_STIPs[M_idx].size()==NStips_ROI){

			if(STIPs_replaced<=STIP_updt_rate){
				
				M_STIPs[M_idx].pop_back();					//delete Model STIP with the lowest weight
				M_STIPs[M_idx].push_front(C_STIPs[rdm_n]);	//store Candidate STIP
				C_STIPs.erase(C_STIPs.begin()+rdm_n);		//delete used Candidate STIP
				STIPs_replaced++;		//Increment number of STIPs replaced
			}
			else{
				break;
			}			
		}
		else{	//Else finish filling the Model NStips_ROI STIPs
			M_STIPs[M_idx].push_front(C_STIPs[rdm_n]);	//store Candidate STIP
			C_STIPs.erase(C_STIPs.begin()+rdm_n);		//delete used Candidate STIP
		}
	}
}
//-------------------------------------------------------------------------------------
//Label and extract isolated regions from foreground mask
//-------------------------------------------------------------------------------------
void Tracking_STIP::Bwlabel(cv::Mat Fmask, cv::Mat &Out){
	
	cv::Mat cFmask = Fmask.clone();

	Erosion(cFmask ,cFmask);

	cv::Mat A = cFmask.clone(); 

	cv::findContours(A, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	Nroi = 0;

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
}

//-------------------------------------------------------------------------------------
//Erosion
//-------------------------------------------------------------------------------------
void Tracking_STIP::Erosion(cv::Mat src, cv::Mat des)
{
	int erosion_elem = 2;
	int erosion_size = 1;
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
void Tracking_STIP::Imagesc(cv::Mat src, int siz, char* Plotname)
{
	//Color Directory
	cv::Mat output(cv::Size(src.size.p[1],src.size.p[0]),CV_8UC3,CV_RGB(0,0,0));
	cv::Mat aux = src.clone();
	cv::Mat aux3;
	cv::Scalar v;
	for(int i=0; i<=siz; i++)
	{
		v.val[0]=i;
		compare(src,v,aux,cv::CMP_EQ);

		cv::Mat aux2[]= {aux*Color[i][0],aux*Color[i][1],aux*Color[i][2]};
		merge(aux2,3,aux3);
		output = aux3+output;
	}
	imshow(Plotname,output);
}

//-------------------------------------------------------------------------------------
//Save bounding box images of current frame
//-------------------------------------------------------------------------------------
void Tracking_STIP::Save_bbox(){

	char save_path[200], Nbbox[5], fr[10];

	itoa(fr_idx,fr,10);

	cv::Mat F_RGB, F_LBSP, Fmask_16;

	Fmask.convertTo(Fmask_16,CV_16UC1);

	F_RGB = Mul3(Im_RGB,Fmask/255.);
	F_LBSP = Mul3(Im_LBSP,Fmask_16/255.);

	for(int i=0; i<Nroi;i++)
	{
		//Extract bbox
		cv::Mat Bb_RGB(F_RGB,cv::Rect(Q_states[i][0],Q_states[i][1],Q_states[i][2],Q_states[i][3]));
		cv::Mat Bb_LBSP(F_LBSP,cv::Rect(Q_states[i][0],Q_states[i][1],Q_states[i][2],Q_states[i][3]));
		
		itoa(i,Nbbox,10);

		//make path to save color images
		strcpy(save_path,"C:\\Users\\Santiago\\Desktop\\People_Dbase\\Color\\");
		strcat(save_path,vidname);
		strcat(save_path,"_");
		strcat(save_path,fr);
		strcat(save_path,"_");
		strcat(save_path,Nbbox);
		strcat(save_path,".png");

		cv::imwrite(save_path,Bb_RGB);

		//make path to save LBSP images
		strcpy(save_path,"C:\\Users\\Santiago\\Desktop\\People_Dbase\\LBSP\\");
		strcat(save_path,vidname);
		strcat(save_path,"_");
		strcat(save_path,fr);
		strcat(save_path,"_");
		strcat(save_path,Nbbox);
		strcat(save_path,".png");

		cv::imwrite(save_path,Bb_LBSP);
	}
}

//-------------------------------------------------------------------------------------
//Calculate the 2-norm of the difference of vectors <A-B>
//-------------------------------------------------------------------------------------
float Tracking_STIP::Norm_vectors(vector<float> A, vector<float> B)
{
	float normv = 0;

	for(int i=0; i<A.size(); i++){
		normv = pow(A[i]-B[i],2)+normv;
	}
	sqrt(normv);

	return normv;
}


//-------------------------------------------------------------------------------------
//Multiplication of 3-channel array by a 1-channel array
//-------------------------------------------------------------------------------------
cv::Mat Tracking_STIP::Mul3(cv::Mat A3, cv::Mat B1)
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
//Draw rectangle A and B in orginal image
//-------------------------------------------------------------------------------------
void Tracking_STIP::DrawRects(vector<int> A, vector<int> B, char* plot1, char* plot2){

	Mat Im1 = Im_RGB.clone();
	cv::rectangle(Im1,cv::Point(A[0],A[1]),cv::Point(A[0]+A[2],A[1]+A[3]),cv::Scalar(255,0,0),2);

	Mat Im2 = Im_RGB.clone();
	cv::rectangle(Im2,cv::Point(B[0],B[1]),cv::Point(B[0]+B[2],B[1]+B[3]),cv::Scalar(255,0,0),2);

	imshow(plot1,Im1);
	imshow(plot2,Im2);
	cv::waitKey();
}

//-------------------------------------------------------------------------------------
//Plot rectangles for the Model ROIs tracked
//-------------------------------------------------------------------------------------
void Tracking_STIP::Tracking_Plot(){

	Mat Im1 = Im_RGB.clone();

	cv::Point ul;
	cv::Point lr;

	for(int i=0;i<M_states.size();i++){

		ul = cv::Point(M_states[i][0],M_states[i][1]);
		lr = cv::Point(M_states[i][0]+M_states[i][2],M_states[i][1]+M_states[i][3]);

		cv::rectangle(Im1,ul,lr,cv::Scalar(Color[i][0],Color[i][1],Color[i][2]),2);
	}

	imshow("Tracked Regions:", Im1);
	cv::waitKey();
}

