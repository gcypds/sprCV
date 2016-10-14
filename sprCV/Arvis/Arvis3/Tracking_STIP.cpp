#include "Tracking_STIP.h"

//-------------------------------------------------------------------------------------
//Constructor
Tracking_STIP::Tracking_STIP(cv::Mat iniFr)
{
	im_r = iniFr.rows;
	im_c = iniFr.cols;

	thr_match = 15;

	ROI_srch_radius = float(im_r)*0.15;	//set as 15% the frame height (If the FPS is known set according it)

	STIP_srch_radius = float(im_r)*0.05; //set as 5% the frame height (If the FPS is known set according it)
	
	NStips_ROI = 50;

	NStips_match = 0.1; 

	NStips_join = 0.1;

	C_ROI_lrg = 0.05;

	STIP_updt_rate = int(ceil(float(NStips_ROI)*0.15));

	min_Stips = int(ceil(float(NStips_ROI)*0.1));

	w_alpha = 0.1;

	w_ini = 0.1;

	s_beta = 0.1;

	hw_thr = 0.2;

	Ppl = People_Features(iniFr);	//Initialize object to validate if CROI is a PROI

	ppl_svm_thr = 0.8;

	Nsrch = 50;

	srch_std = 0.1;

	lim_srch_dev = 0.20;

	shrink_updt_roi = 0.1;

	lost_lim = 10;

	//Copy color library
	for(int i=0; i<20; i++)
	{
		Color_Lib.push_back(Scalar(Color1[i][0],Color1[i][1],Color1[i][2]));
	}

}

//-------------------------------------------------------------------------------------
//Destructor
Tracking_STIP::~Tracking_STIP()
{
}


//-------------------------------------------------------------------------------------
//Initialize ROIs states and STIP models
//-------------------------------------------------------------------------------------
void Tracking_STIP::initROI(vector<P_tuple> P_states, vector<vector<vector<float>>> Q_STIPs, vector<vector<cv::KeyPoint>> kpts_fmask, bool ini)
{
	//Copy PROI states from P_tuple(see People_Features.h) to vector of vectors
	Q_states = tuple2vector(P_states);

	deque<M_STIP_tuple> WSTIP;
	int rdm_n;

	//Loop through Query ROIs
	for (int i=0; i<Q_STIPs.size(); i++)
	{	
		//If the QROI has stips to be modeled then store in MROI
		if(Q_STIPs[i].size()>=min_Stips)
		{
			//Initialize number of tracked frames M[6]
			Q_states[i].push_back(1);

			//Initialize number of Plost frames (tracked only by STIPS, not PROI found) M[7]
			Q_states[i].push_back(0);

			//Initialize number of lost frames M[8]
			Q_states[i].push_back(0);

			//Assign tracking color
			M_colors.push_back(Color_Lib[0]);

			//Delete color from color library
			Color_Lib.pop_front();

			//Initialize ROI Model states
			M_states.push_back(Q_states[i]);	

			//Initialize STIP vector
			WSTIP.clear();	

			//Available STIPs for Query ROI i
			int Q_STIPs_roi = Q_STIPs[i].size();	

			//If there are more STIPs than the required -> randomly subsample, else -> fill the rest with zeros
			if(Q_STIPs_roi>NStips_ROI)
			{
				for (int j=0; j<NStips_ROI; j++)
				{
					//Get random index
					rdm_n = rand() % Q_STIPs[i].size();	

					//Fill with random STIP from Query
					WSTIP.push_back(std::make_tuple(w_ini, kpts_fmask[i][rdm_n], Q_STIPs[i][rdm_n]));	

					//Delete used STIP
					Q_STIPs[i].erase(Q_STIPs[i].begin()+rdm_n);	
				}
			}
			else
			{
				//Loop through Query Stips
				for (int j=0; j<Q_STIPs_roi; j++)	
				{		
					//Fill vector with available STIPs
					WSTIP.push_back(std::make_tuple(w_ini, kpts_fmask[i][j], Q_STIPs[i][j]));	
				}
			}
			//Store STIPs in ROI model
			M_STIPs.push_back(WSTIP);	
		}
	}
}


//-------------------------------------------------------------------------------------
//Search for each Model ROI a near Query ROI that matches it according to an STIP majority vote
//-------------------------------------------------------------------------------------
void Tracking_STIP::matchRegions(Mat frame, vector<vector<int>> Q_states, vector<vector<vector<float>>> Q_STIPs, vector<vector<cv::KeyPoint>> kpts_fmask)
{
	Im_RGB = frame;

	float dist;	
	int Nm;			//Number of matched STIPs
	int Nm_ref;		//Reference of matched STIPs
	int QROIm = 0;	//Query ROI match

	//Copy PROI states from P_tuple(see People_Features.h) to vector of vectors
	//Q_states = tuple2vector(P_states);

	deque<M_STIP_tuple> CSTIPs;
	vector<int> Cstates;	
	vector<int> M_matches_ref;				//reference Model matches vector (see M_matches description)
	Q_ROI_matched.assign(Q_STIPs.size(),false);		//Matched Query ROIs

	//Reinitialize Candidate ROI states and STIPs

	//cout<<endl<<"MROIs:"<<M_states.size();

	for(int i=0; i<M_states.size();i++)	//loop through Model ROIs
	{	
		Nm_ref = 0;		//Initialize reference number of matches

		//cout<<endl<<"MROI:"<<i;

		//test---------
		//if(i>4){
		//PlotKpts(M_STIPs[i], frame, "Mkpts", cv::Scalar(255,0,0));
		//cout<<" Qs"<<Q_states.size()<<" Ks"<<kpts_fmask.size();
		//}
		//---------

		for(int j=0; j<Q_states.size();j++)	//loop through Query ROIs
		{	
			//Distance between Model ROI [i] centroid and Query [j] ROI centroid
			dist = sqrt(pow(double(M_states[i][4]-Q_states[j][4]),2)+pow(double(M_states[i][5]-Q_states[j][5]),2));

			//test---------
			//if(i>4){
			//std::cout<<std::endl<<"M:"<<i<<" Q:"<<j;
			//DrawRects(M_states[i], Q_states[j], "B", "B");
			//cout<<" dist"<<dist<<endl;
			//PlotKpts(kpts_fmask[j], frame, "Qkpts", cv::Scalar(0,255,0));
			//}
			//---------

			//Check if the distance is lower than a fixed search radius
			if (dist<ROI_srch_radius)
			{
				//Match STIPs
				Nm = matchSTIPs(M_STIPs[i],Q_STIPs[j],kpts_fmask[j]);	

				//test---------
				//if(i>4){
				//cout<<" Nm_p"<<Nm<<endl;
				//}

				if (Nm>Nm_ref)
				{
					Nm_ref = Nm;	//Set new reference
					QROIm = j;		//Set new Query ROI match
					M_matches_ref = M_matches;	//Set new Query matches vector 
				}
			}
		}
		//cout<<" Nm:"<<Nm_ref<<"/"<<NStips_match*M_STIPs[i].size();
		
		//If at least a percentage NStips_match of STIPs matched, perform tracking, otherwise see if the MROI is lost
		if(Nm_ref>ceil(NStips_match*M_STIPs[i].size()))
		{
			//Assign true label to Query ROI QROIm as matched to one Model ROI 
			Q_ROI_matched[QROIm] = true;

			//Get Candidate ROI states 
			Cstates = getCandidateROI(kpts_fmask[QROIm], Q_STIPs[QROIm], Q_states[QROIm], M_matches_ref,i);	

			//Get Candidate ROI STIPs
			CSTIPs = get_CROI_STIPs(Cstates, M_matches_ref, kpts_fmask[QROIm], Q_STIPs[QROIm]);

			//Update Model ROI states and STIPs
			UpdtModelROI(i, Cstates, CSTIPs, M_matches, kpts_fmask[QROIm]);

			//cout<<" Mlost:"<<M_states[i][7];
		}
		else
		{
			//Increase number of lost frames
			M_states[i][8] = M_states[i][8]+1;

			//Check if the MROI has been lost than lost_lim frames to delete its information
			MROI_lost(i);
		}
	}
}

//-------------------------------------------------------------------------------------
//Match stip features from a query set
//-------------------------------------------------------------------------------------
int Tracking_STIP::matchSTIPs(deque<M_STIP_tuple> Model, vector<vector<float>> Query, vector<cv::KeyPoint> kpts_fmask)
{
	vector<int> Model_matches (Model.size(),-1);	//Indices of Query STIPs that match Model STIPs (if found any) 
	
	float difval;
	float refval;
	bool match_found;
	int Nmatches = 0;	//Number of matched STIPs
	int STIP_dist;

	for(int i=0; i<Model.size(); i++)
	{
		refval = 1000;			//initialize reference matching value
		match_found = false;	//initialize match flag

		for(int j=0; j<Query.size();j++)
		{
			//check if the current index j has already taken into Query_matches
			std::vector<int>::iterator it = std::find (Model_matches.begin(), Model_matches.end(), j);	
			
			if (it == Model_matches.end())	//If not taken calculate match
			{ 
				//distance between M_STIP i and Q_STIP j centroids
				STIP_dist = sqrt(pow(std::get<1>(Model[i]).pt.x-kpts_fmask[j].pt.x,2)+pow(std::get<1>(Model[i]).pt.y-kpts_fmask[j].pt.y,2));

				if(STIP_dist<STIP_srch_radius)	//if the distance is lower than a searching threshold calculate 2norm
				{	
					difval = Norm_vectors(std::get<2>(Model[i]), Query[j]);	//2norm between Query[i] and Model[j]

					if((difval<thr_match)&(difval<refval))	//Validate the matching between Query[i] and Model[j]
					{		
						refval = difval;		//assign new reference matching value
						Model_matches[i] = j;	//assign Model j index to Query i
						match_found = true;		//match found true
					}
				}
			}
		}
		if(match_found)
		{
			Nmatches++;		//Increase number of matched STIPs
		}
	}
	M_matches = Model_matches; //Make copy to class variable

	return Nmatches;
}

//-------------------------------------------------------------------------------------
//From the Query ROI matched to a Model ROI find a Candidate ROI
//-------------------------------------------------------------------------------------
vector<int> Tracking_STIP::getCandidateROI(vector<cv::KeyPoint> kpts_fmask, vector<vector<float>> Q_STIPs, vector<int> Q_states, vector<int> M_matches_ref, int M_idx)
{
	vector<int> Cstates, Cstates_new;
	double svmval;

	//Get average MROI displacement according to matched STIPs
	vector<int> displ = getROI_avg_displacement(kpts_fmask, M_matches_ref, M_idx);

	//Displace MROI according to displ
	Cstates.push_back(M_states[M_idx][0]+displ[0]);
	Cstates.push_back(M_states[M_idx][1]+displ[1]);
	Cstates.push_back(M_states[M_idx][2]);
	Cstates.push_back(M_states[M_idx][3]);
	Cstates.push_back(M_states[M_idx][4]+displ[0]);
	Cstates.push_back(M_states[M_idx][5]+displ[1]);

	validate_states(Cstates);	//Validate displaced states

	//1st stage: Check for PROI in displaced ROI
	bool flagp =  checkROI4PROI(Cstates, Im_RGB, svmval);
	cout<<" Trck1:"<<flagp;

	if(!flagp)
	{
		//2nd stage: Check for PROI in random ROIs spread around displaced area
		flagp = searchPROI_aroundCROI(Cstates, Cstates_new, Im_RGB, M_idx);
		cout<<" Trck2:"<<flagp;

		if(flagp)
			Cstates = Cstates_new;
		else
		{
			cout<<" Trck3:"<<flagp;
			//If not PROI found in 1st nor 2nd stage assing to MROI CROI
			Cstates = get_CROI_from_STIPs(kpts_fmask, Q_STIPs, Q_states, M_matches_ref, M_idx);
		}
	}

	//If person found, save in Cstates[6]=1, save in Cstates[7]=0. Else, the other way around 
	if(flagp)
	{
		Cstates.push_back(1);
		Cstates.push_back(0);
	}
	else
	{
		Cstates.push_back(0);
		Cstates.push_back(1);
	}

	//DrawRects(M_states[M_idx], Cstates, "M&T", "M&T");
	return Cstates;
}

//-------------------------------------------------------------------------------------
//Sort Model STIPs according to their weights
//-------------------------------------------------------------------------------------
void Tracking_STIP::SortSTIPs(deque<M_STIP_tuple> M_STIPs)
{
		//Sort M_STIP_tuple according the weights
		std::sort(M_STIPs.begin(),M_STIPs.end(),
			[](const  M_STIP_tuple& a,
			const  M_STIP_tuple& b) -> bool
		{
			return std::get<0>(a) > std::get<0>(b);
		});
}

//-------------------------------------------------------------------------------------
//Search a PROI to match MROI around the coordinates given by C_states
//-------------------------------------------------------------------------------------
bool Tracking_STIP::searchPROI_aroundCROI(vector<int> Cstates, vector<int> &Cstates_new, Mat Frame, int Midx)
{
	vector<int> Cstates_q;
	bool PROIfound, PROIfoundT = false;
	double refval = 0, svmval;

	//--------------------------First search: Variate width and height-------------------------------
	int rand_wid, rand_heig, wid_dif, heig_dif, ctr_x, ctr_y;

	double low_lim_w = ((Cstates[2]-Cstates[2]*lim_srch_dev)<15)?15:Cstates[2]-Cstates[2]*lim_srch_dev;
	double high_lim_w = ((Cstates[2]+Cstates[2]*lim_srch_dev)>100)?100:Cstates[2]+Cstates[2]*lim_srch_dev;
	double low_lim_h = ((Cstates[3]-Cstates[3]*lim_srch_dev)<45)?45:Cstates[3]-Cstates[3]*lim_srch_dev;
	double high_lim_h = ((Cstates[3]+Cstates[3]*lim_srch_dev)<190)?190:Cstates[3]+Cstates[3]*lim_srch_dev;

	for(int i=0; i<floor(double(Nsrch)/2); i++)
	{
		rand_wid = Randn_int(Cstates[2], Cstates[2]*srch_std, low_lim_w, high_lim_w);
		rand_heig = Randn_int(Cstates[3], Cstates[3]*srch_std, low_lim_h, high_lim_h);

		wid_dif = floor(double(rand_wid-Cstates[2])/2);
		heig_dif = floor(double(rand_heig-Cstates[3])/2);

		//Set Cstates_new
		Cstates_q.clear();
		Cstates_q.push_back(Cstates[0]+wid_dif);
		Cstates_q.push_back(Cstates[1]+heig_dif);
		Cstates_q.push_back(rand_wid);
		Cstates_q.push_back(rand_heig);

		ctr_x = Cstates_q[0]+int(floor(double(rand_wid)/2.));
		ctr_y = Cstates_q[1]+int(floor(double(rand_heig)/2.));

		Cstates_q.push_back(ctr_x);
		Cstates_q.push_back(ctr_y);

		//Validate states
		validate_states(Cstates_q);	

		//Check for PROI
		PROIfound = checkROI4PROI(Cstates_q, Im_RGB, svmval);

		//cout<<" s1:"<<svmval;
		//DrawRects(M_states[Midx], Cstates_q, "M&P1", "M&P1");

		if(PROIfound&&(svmval>refval))
		{
			Cstates_new = Cstates_q;
			refval = svmval;
			PROIfoundT = true;
		}
	}
	if(!PROIfoundT)
	{
		//--------------------------Second search: Variate all states-----------------------------
		double low_lim_x, low_lim_y, high_lim_x, high_lim_y;
		int rand_cx, rand_cy;

		for(int i=0; i<floor(double(Nsrch)/2); i++)
		{
			rand_wid = Randn_int(Cstates[2], Cstates[2]*srch_std, low_lim_w, high_lim_w);
			rand_heig = Randn_int(Cstates[3], Cstates[3]*srch_std, low_lim_h, high_lim_h);

			low_lim_x = ((Cstates[0]-rand_wid*lim_srch_dev)<0) ? 0 : Cstates[0]-rand_wid*lim_srch_dev;
			high_lim_x = ((Cstates[0]+rand_wid*lim_srch_dev)>(im_c-rand_wid-1)) ? (im_c-rand_wid-1) : Cstates[0]+rand_wid*lim_srch_dev;
			low_lim_y = ((Cstates[1]-rand_heig*lim_srch_dev)<0) ? 0 : Cstates[1]-rand_heig*lim_srch_dev;
			high_lim_y = ((Cstates[1]+rand_heig*lim_srch_dev)>(im_r-rand_heig-1)) ? (im_r-rand_heig-1) : Cstates[1]+rand_heig*lim_srch_dev;

			rand_cx = Randn_int(Cstates[0], Cstates[2]*srch_std, low_lim_x, high_lim_x);
			rand_cy = Randn_int(Cstates[1], Cstates[3]*srch_std, low_lim_y, high_lim_y);

			//Set Cstates_new
			Cstates_q.clear();
			Cstates_q.push_back(rand_cx);
			Cstates_q.push_back(rand_cy);
			Cstates_q.push_back(rand_wid);
			Cstates_q.push_back(rand_heig);

			ctr_x = Cstates_q[0]+int(floor(double(rand_wid)/2.));
			ctr_y = Cstates_q[1]+int(floor(double(rand_heig)/2.));

			Cstates_q.push_back(ctr_x);
			Cstates_q.push_back(ctr_y);

			//Validate states
			validate_states(Cstates_q);	

			//Check for PROI
			PROIfound = checkROI4PROI(Cstates_q, Im_RGB, svmval);

			//cout<<" s2:"<<svmval;
			//DrawRects(M_states[Midx], Cstates_q, "M&P2", "M&P2");

			if(PROIfound&&(svmval>refval))
			{
				Cstates_new = Cstates_q;
				refval = svmval;
				PROIfoundT = true;
			}
		}
	}
	return PROIfoundT;
}

//-------------------------------------------------------------------------------------
//From the Query ROI matched to a Model ROI find a Candidate ROI
//-------------------------------------------------------------------------------------
vector<int> Tracking_STIP::get_CROI_from_STIPs(vector<cv::KeyPoint> kpts_fmask, vector<vector<float>> Q_STIPs, vector<int> C, vector<int> M_matches_ref, int M_idx)
{
	vector<int> Cstates;		//REVISAR ESTA PARTE!!!!!!!!!!!!!!!!!

	//Check if the matched Query ROI size is similar to the Model ROI size to assign it
	bool cond_w = abs(M_states[M_idx][2]-C[2])<(int)ceil((float)M_states[M_idx][2]*hw_thr);
	bool cond_h = abs(M_states[M_idx][3]-C[3])<(int)ceil((float)M_states[M_idx][3]*hw_thr);

	if(cond_w&cond_h)
	{
		Cstates = C;
		//std::cout<<"f: Q";
	}
	else{ //If the conditions above are not fulfilled calculate Candidate STIPs according to Q_STIPs coordinates that matched

		//std::cout<<"f: C";
		//Vectors to store the x and y coordinates of the matched Query STIPs
		vector<int> Kpts_x;
		vector<int> Kpts_y;


		//Loop through Model matches vector
		for(int i=0; i<M_matches_ref.size(); i++)
		{
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
		min_x = min_x - int(ceil(C_ROI_lrg*wid));
		min_y = min_y - int(ceil(C_ROI_lrg*heig));
		wid = wid + int(ceil(2*C_ROI_lrg*wid));
		heig = heig + int(ceil(2*C_ROI_lrg*heig));

		Cstates.push_back(min_x);
		Cstates.push_back(min_y);
		Cstates.push_back(wid);
		Cstates.push_back(heig);

		validate_states(Cstates);	//validate states

		ctr_x = min_x+int(floor(double(Cstates[2])/2.));
		ctr_y = min_y+int(floor(double(Cstates[3])/2.));

		Cstates.push_back(ctr_x);
		Cstates.push_back(ctr_y);
	}

	return Cstates;
}

//-------------------------------------------------------------------------------------
//Check if a ROI states correspond to a PROI
//-------------------------------------------------------------------------------------
bool Tracking_STIP::checkROI4PROI(vector<int> A, Mat Frame, double &val){

	vector<float> Sample;
	Mat MSample;
	float svm_val;
	bool ppl_lbl = false;

	//Extract ROI
	Mat CROI(Frame,Rect(A[0],A[1],A[2],A[3]));

	//Preprocess CROI to fit SVM samples
	Mat pCROI = Ppl.ImPreprocess(CROI);

	//Compute HOG features
	Sample = Ppl.HOG_compute(pCROI);

	//Project sample to low feature space
	MSample = Ppl.Projection(Sample);

	//Enter Projected sample into SVM to check for person
	svm_val = Ppl.SVM_Classify(MSample);

	//Get SVM value
	val = svm_val;

	if(svm_val>ppl_svm_thr)
		ppl_lbl = true;

	return ppl_lbl;
}

//-------------------------------------------------------------------------------------
//From matched STIPs get the average displacement of MROI M_idx
//-------------------------------------------------------------------------------------
vector<int> Tracking_STIP::getROI_avg_displacement(vector<cv::KeyPoint> Q, vector<int> M, int M_idx)
{
	double dispx = 0, dispy = 0;
	int s = 0;
	vector<int> displ;

	vector<cv::KeyPoint> TestV;

	for(int i=0; i<M.size(); i++)
	{
		if(M[i]!=-1)
		{
			s++;
			dispx = double(Q[M[i]].pt.x-std::get<1>(M_STIPs[M_idx][i]).pt.x) + dispx;
			dispy = double(Q[M[i]].pt.y-std::get<1>(M_STIPs[M_idx][i]).pt.y) + dispy;
			TestV.push_back(Q[M[i]]);
		}
	}
	displ.push_back(int(floor(dispx/s)));
	displ.push_back(int(floor(dispy/s)));

	//PlotKpts(TestV, Im_RGB, "KptsM", cv::Scalar(200,0,100));

	return displ;
}

//-------------------------------------------------------------------------------------
//From CROI states get new STIPs
//-------------------------------------------------------------------------------------
deque<M_STIP_tuple> Tracking_STIP::get_CROI_STIPs(vector<int> C_states, vector<int> M_matches_ref, vector<cv::KeyPoint> Q, vector<vector<float>> Q_STIPs)
{
	deque<M_STIP_tuple> C_STIPs;
	vector<int> Cstates_aux;

	//Get region from Fmask according to C_states and shrink accoding to shrink_updt_roi
	int x, y, wid, heig;

	wid = floor((1-shrink_updt_roi)*double(C_states[2]));
	heig = floor((1-shrink_updt_roi)*double(C_states[3]));
	x = C_states[0]+floor(double(shrink_updt_roi*C_states[2])/2);
	y = C_states[1]+floor(double(shrink_updt_roi*C_states[3])/2);

	Cstates_aux.push_back(x);
	Cstates_aux.push_back(y);
	Cstates_aux.push_back(wid);
	Cstates_aux.push_back(heig);

	//Validate states
	validate_states(Cstates_aux);

	Mat A = Mat::zeros(im_r,im_c,CV_8UC1);
	Mat roi(A,Rect(Cstates_aux[0],Cstates_aux[1],Cstates_aux[2],Cstates_aux[3]));
	roi = Scalar(255);

	//For each STIP that didn´t match see if it is inside the fmask enclosed in C_states
	for(int i=0; i<Q.size(); i++)
	{
		if(kpt_in_FROI(Q[i], A, 0))	
		{
			//Store in C_STIPs only the ones that didn't match any M_STIP but are within C_states,
			std::vector<int>::iterator it = std::find (M_matches_ref.begin(), M_matches_ref.end(), i);	

			if (it == M_matches_ref.end())
			{
				//Make candidate STIP with Query STIP i information and initial weight w_ini
				C_STIPs.push_back(std::make_tuple(w_ini, Q[i], Q_STIPs[i]));
			}
		}
	}
	return C_STIPs;
}

//-------------------------------------------------------------------------------------
//Update Model ROI M_idx states STIPs according to Candidate ROI
//-------------------------------------------------------------------------------------
void Tracking_STIP::UpdtModelROI(int M_idx, vector<int> C_states, deque<M_STIP_tuple> C_STIPs, vector<int> M_matches, vector<cv::KeyPoint> kpts_fmask)
{
	//Update states----------------------------
	M_states[M_idx][2] = s_beta*C_states[2]+(1-s_beta)*M_states[M_idx][2];
	M_states[M_idx][3] = s_beta*C_states[3]+(1-s_beta)*M_states[M_idx][3];
	M_states[M_idx][4] = C_states[4];
	M_states[M_idx][5] = C_states[5];
	M_states[M_idx][0] = C_states[4]-int(floor(float(M_states[M_idx][2])/2.));
	M_states[M_idx][1] = C_states[5]-int(floor(float(M_states[M_idx][3])/2.));
	M_states[M_idx][6] = M_states[M_idx][6]+C_states[6];
	M_states[M_idx][7] = (M_states[M_idx][7]+C_states[7])*C_states[7];
	M_states[M_idx][8] = 0;
		
	//Get region from Fmask according to C_states and shrink accoding to shrink_updt_roi
		//Validate states
	validate_states(M_states[M_idx]);

	Mat A = Mat::zeros(im_r,im_c,CV_8UC1);
	Mat roi(A,Rect(M_states[M_idx][0],M_states[M_idx][1],M_states[M_idx][2],M_states[M_idx][3]));
	roi = Scalar(255);

	//imshow("Test",A);
	//waitKey();

	//Update STIPs weights and coordinates------
	for(int i=0; i<M_STIPs[M_idx].size();i++)
	{
		if(M_matches[i]!=-1)
		{
			if(kpt_in_FROI(kpts_fmask[M_matches[i]], A, 0))	
			{
				//weights
				std::get<0>(M_STIPs[M_idx][i]) = std::get<0>(M_STIPs[M_idx][i])+w_alpha;

				//coordinates
				std::get<1>(M_STIPs[M_idx][i]) = kpts_fmask[M_matches[i]];
			}
		}
	}
	//PlotKpts(M_STIPs[M_idx], Im_RGB, "KptsC", cv::Scalar(255,0,0));

	//Update STIPs----------------------------

	//Sort Model ROI M_idx STIPs according its weights
	SortSTIPs(M_STIPs[M_idx]);

	int STIPs_replaced = 0, rdm_n;

	//std::cout<<" C:"<<C_STIPs.size()<<std::endl;

	//PlotKpts(C_STIPs, Im_RGB, "Cstips", cv::Scalar(255,0,0));

	while(C_STIPs.size()>0)
	{
		rdm_n = rand() % C_STIPs.size();	//get random index

		//If the Model has all the NStips_ROI STIPs, randomly replace STIP_updt_rate STIPs with the lowest weight with Candidate STIPs
		if(M_STIPs[M_idx].size()==NStips_ROI)
		{
			if(STIPs_replaced<=STIP_updt_rate)
			{
				M_STIPs[M_idx].pop_back();					//delete Model STIP with the lowest weight
				M_STIPs[M_idx].push_front(C_STIPs[rdm_n]);	//store Candidate STIP
				C_STIPs.erase(C_STIPs.begin()+rdm_n);		//delete used Candidate STIP
				STIPs_replaced++;		//Increment number of STIPs replaced
			}
			else
			{
				break;
			}			
		}
		else	//Else finish filling the Model NStips_ROI STIPs
		{	
			M_STIPs[M_idx].push_front(C_STIPs[rdm_n]);	//store Candidate STIP
			C_STIPs.erase(C_STIPs.begin()+rdm_n);		//delete used Candidate STIP
		}
	}
	//PlotKpts(M_STIPs[M_idx], Im_RGB, "KptsM2", cv::Scalar(0,0,255));
}

//-------------------------------------------------------------------------------------
//Decide if a MROI is lost to delete it from memory
//-------------------------------------------------------------------------------------
bool Tracking_STIP::MROI_lost(int M_idx)
{
	bool fl_lost = false;

	//Check if the MROI has been lost more than lost_lim frames
	if(M_states[M_idx][8]>lost_lim)
	{
		//Set as available the MROI trackingcolor
		Color_Lib.push_back(M_colors[M_idx]);

		//Delete MROI states
		M_states.erase(M_states.begin()+M_idx);

		//Delete MROI STIPs
		M_STIPs.erase(M_STIPs.begin()+M_idx);

		//Delete MROI Color
		M_colors.erase(M_colors.begin()+M_idx);

		fl_lost = true;
	}

	return fl_lost;
}

//-------------------------------------------------------------------------------------
//Check if a new detected region PROI corresponds to a previous detected region MROI
//-------------------------------------------------------------------------------------
void Tracking_STIP::merge_MROInew_MROI(vector<P_tuple> &Pstates, vector<vector<vector<float>>> &PSTIPs, vector<vector<cv::KeyPoint>> &kpts_fmask)
{
	double dist;
	int Nm;			//Number of matched STIPs
	int Nm_ref;		//Reference of matched STIPs
	int MROIm = 0;	//Query ROI match
	int i;
	int thr_Nm_ref;

	vector<P_tuple> PNstates;

	for(int j=0; j<M_states.size();j++)
	{
		Nm_ref = 0;
		i = 0;

		while(i<Pstates.size())
		{
			//If there are STIPs for PROI i, check for merge, otherwise dont
			if(PSTIPs[i].size()>=min_Stips)
			{
				//Distance between Model ROI [i] centroid and Query [j] ROI centroid
				dist = sqrt(pow(double(M_states[j][4]-std::get<1>(Pstates[i])[4]),2)+pow(double(M_states[j][5]-std::get<1>(Pstates[i])[5]),2));

				//DrawRects(M_states[j], get<1>(Pstates[i]), "testA", "testA");

				//cout<<endl<<" Pr"<<i<<" Mr"<<j<<" dist"<<dist<<"/"<<ROI_srch_radius;

				if(dist<(ROI_srch_radius*0.5))
				{
					//PlotKpts(kpts_fmask[i], Im_RGB, "KptsP", cv::Scalar(255,0,0));
					//PlotKpts(M_STIPs[j], Im_RGB, "KptsM", cv::Scalar(0,255,0));

					//Match STIPs
					Nm = matchSTIPs(M_STIPs[j],PSTIPs[i],kpts_fmask[i]);	

					//cout<<" Ma"<<Nm;

					if (Nm>=Nm_ref)
					{
						Nm_ref = Nm;	//Set new reference
						MROIm = i;		//Set new Query ROI match
					}
				}
				i++;
			}
			else 
			{
				//Erase PROI i information
				Pstates.erase(Pstates.begin()+i);
				PSTIPs.erase(PSTIPs.begin()+i);
				kpts_fmask.erase(kpts_fmask.begin()+i);
			}
		}
		thr_Nm_ref = ceil(NStips_join*double(M_STIPs[j].size()));
		//cout<<" Mg"<<Nm_ref<<"/"<<thr_Nm_ref;

		//If at least a percentage NStips_match of STIPs matched, perform tracking, otherwise see if the MROI is lost
		if(Nm_ref>=thr_Nm_ref)
		{
			//Assign to MROI MROIm the states of PROI i
			M_states[j][0] = std::get<1>(Pstates[MROIm])[0];
			M_states[j][1] = std::get<1>(Pstates[MROIm])[1];
			M_states[j][2] = std::get<1>(Pstates[MROIm])[2];
			M_states[j][3] = std::get<1>(Pstates[MROIm])[3];
			M_states[j][4] = std::get<1>(Pstates[MROIm])[4];
			M_states[j][5] = std::get<1>(Pstates[MROIm])[5];
			M_states[j][6] = 1;
			M_states[j][7] = 0;
			M_states[j][8] = 0;

			//Erase PROI i information
			Pstates.erase(Pstates.begin()+MROIm);
			PSTIPs.erase(PSTIPs.begin()+MROIm);
			kpts_fmask.erase(kpts_fmask.begin()+MROIm);
		}
	}
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

		cv::Mat aux2[]= {aux*Color1[i][0],aux*Color1[i][1],aux*Color1[i][2]};
		merge(aux2,3,aux3);
		output = aux3+output;
	}
	imshow(Plotname,output);
}

//-------------------------------------------------------------------------------------
//Save bounding box images of current frame
//-------------------------------------------------------------------------------------
void Tracking_STIP::Save_bbox()
{
	char save_path[200], Nbbox[5], fr[10];

	itoa(fr_idx,fr,10);

	cv::Mat F_RGB, F_LBSP, Fmask_16;

	Fmask.convertTo(Fmask_16,CV_16UC1);

	F_RGB = Mul3(Im_RGB,Fmask/255.);
	F_LBSP = Mul3(Im_LBSP,Fmask_16/255.);

	for(int i=0; i<Nroi;i++)
	{
		//Extract bbox
		cv::Mat Bb_RGB(Im_RGB,cv::Rect(Q_states[i][0],Q_states[i][1],Q_states[i][2],Q_states[i][3]));
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

	for(int i=0; i<A.size(); i++)
	{
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
//Look if a keypoint is enclosed in a FROI by checking if the four cardinal points given 
//by the keypoint centroid and radius are inside the FROI
//-------------------------------------------------------------------------------------
bool Tracking_STIP::kpt_in_FROI(cv::KeyPoint a, Mat Fmask, float lrg_Fmask_srch)
{
	int cx, cy;

	bool flagin_l, flagin_r, flagin_u, flagin_d, flagin = false;

	if(a.size>1)
	{
		//left
		cx = a.pt.x-floor((1-lrg_Fmask_srch)*a.size/2);
		cy = a.pt.y;
		if(cx<0)
			cx = 0;
		flagin_l = (Fmask.at<uchar>(Point(cx,cy))==255);

		//right
		cx = a.pt.x+floor((1-lrg_Fmask_srch)*a.size/2);
		cy = a.pt.y;
		if(cx>=im_c)
			cx = im_c+1;
		flagin_r = (Fmask.at<uchar>(Point(cx,cy))==255);

		//up
		cx = a.pt.x;
		cy = a.pt.y-floor((1-lrg_Fmask_srch)*a.size/2);
		if(cy<0)
			cy = 0;
		flagin_u = (Fmask.at<uchar>(Point(cx,cy))==255);

		//down
		cx = a.pt.x;
		cy = a.pt.y+floor((1-lrg_Fmask_srch)*a.size/2);
		if(cy>=im_r)
			cy = im_r+1;
		flagin_d = (Fmask.at<uchar>(Point(cx,cy))==255);

		flagin = flagin_l&&flagin_r&&flagin_u&&flagin_d;
	}

	return flagin;
}

//-------------------------------------------------------------------------------------
//Draw rectangle A and B in orginal image
//-------------------------------------------------------------------------------------
void Tracking_STIP::DrawRects(vector<int> A, vector<int> B, char* plot1, char* plot2, cv::Scalar S1, cv::Scalar S2)
{
	Mat Im1 = Im_RGB.clone();
	cv::rectangle(Im1,cv::Point(A[0],A[1]),cv::Point(A[0]+A[2],A[1]+A[3]),S1,2);

	cv::rectangle(Im1,cv::Point(B[0],B[1]),cv::Point(B[0]+B[2],B[1]+B[3]),S2,2);

	if(A.size()>4&&B.size()>4)
	{
		cv::circle(Im1,cv::Point(A[4],A[5]),2,S1,2);
		cv::circle(Im1,cv::Point(B[4],B[5]),2,S2,2);
	}


	imshow(plot1,Im1);
	cv::waitKey();
}

//-------------------------------------------------------------------------------------
//Plot rectangles for the Model ROIs tracked
//-------------------------------------------------------------------------------------
void Tracking_STIP::Tracking_Plot(int fr_idx, char *vid_name)
{
	Mat Im1 = Im_RGB.clone();

	cv::Point ul;
	cv::Point lr;

	char id[5];

	for(int i=0;i<M_states.size();i++)
	{
		ul = cv::Point(M_states[i][0],M_states[i][1]);
		lr = cv::Point(M_states[i][0]+M_states[i][2],M_states[i][1]+M_states[i][3]);

		cv::rectangle(Im1,ul,lr,M_colors[i],2);

		itoa(i,id,10);

		putText(Im1,id,Point(ul.x+5,ul.y+10),FONT_HERSHEY_SIMPLEX,0.5,M_colors[i],1.5);
	}

	imshow("Tracked Regions:", Im1);


	char vid_save[100], fr_i[5];
	strcpy_s(vid_save,"C:\\Users\\Santiago\\Desktop\\Frame_Video\\");
	strcat_s(vid_save,vid_name);
	strcat_s(vid_save,"_");
	itoa(fr_idx,fr_i,10);
	strcat_s(vid_save,fr_i);
	strcat_s(vid_save,".jpg");
	imwrite(vid_save,Im1);

	//waitKey();
}


//-------------------------------------------------------------------------------------
//Copy PROI states from P_tuple(see People_Features.h) to vector of vectors
//-------------------------------------------------------------------------------------
vector<vector<int>> Tracking_STIP::tuple2vector(vector<P_tuple> A)
{
	vector<vector<int>> B; 

	for(int i=0; i<A.size(); i++)
	{
		B.push_back(std::get<1>(A[i]));
	}

	return B;
}

//-------------------------------------------------------------------------------------
//Plot keypoints in image
//-------------------------------------------------------------------------------------
void Tracking_STIP::PlotKpts(vector<cv::KeyPoint> A, Mat Frame, char *Pltname, cv::Scalar S1)
{
	Mat B = Frame.clone();

	for(int i=0; i<A.size(); i++)
	{
		circle(B, A[i].pt, (int)A[i].size/2, S1, 2);
	}

	imshow(Pltname, B);
	waitKey();
}

//-------------------------------------------------------------------------------------
//Plot keypoints in image
//-------------------------------------------------------------------------------------
void Tracking_STIP::PlotKpts(deque<M_STIP_tuple> A, Mat Frame, char *Pltname, cv::Scalar S1)
{
	Mat B = Frame.clone();

	for(int i=0; i<A.size(); i++)
	{
		circle(B, std::get<1>(A[i]).pt, (int)std::get<1>(A[i]).size/2, S1, 2);
	}

	imshow(Pltname, B);
	waitKey();
}

//-------------------------------------------------------------------------------------
//Validate if generated states are inside the image
//-------------------------------------------------------------------------------------
void Tracking_STIP::validate_states(vector<int> &S)
{
	if(S[0]<0)
	{
		S[2] = S[2]-S[0];
		S[0] = 0;
	}

	if(S[1]<0)
	{
		S[3] = S[3]-S[0];
		S[1] = 0;
	}

	if((S[0]+S[2])>im_c)
		S[2] = im_c-S[0]-1;

	if((S[1]+S[3])>im_r)
		S[3] = im_r-S[1]-1;
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

int Tracking_STIP::Randn_int(double mean, double std, double low_lim, double high_lim)
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
		}
	}
	return int(floor(rn));
}

		