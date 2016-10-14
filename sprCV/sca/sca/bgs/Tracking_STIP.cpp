#include "Tracking_STIP.h"

//-------------------------------------------------------------------------------------
//Constructor
Tracking_STIP::Tracking_STIP(cv::Mat iniFr){
	
	pminArea = 0.0005;
	minArea = int(pminArea*double(iniFr.cols*iniFr.rows));  //Minimum area to consider a region

	thr_match = 2.5;
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

	int min_x, max_x, min_y, max_y, wid, heig;
	std::vector<int> Obj;
	Bbox.clear();

	for (int i=0; i<Nroi;i++){
		//Find the corners of the bounding boxes that cover each moving object
		min_x = *std::min_element(Xcroi[i].begin(),Xcroi[i].end());   
		max_x = *std::max_element(Xcroi[i].begin(),Xcroi[i].end());
		min_y = *std::min_element(Ycroi[i].begin(),Ycroi[i].end());
		max_y = *std::max_element(Ycroi[i].begin(),Ycroi[i].end());
		wid = max_x-min_x;
		heig = max_y-min_y;

		//Copy bbox coordinates into registers
		Obj.clear();
		Obj.push_back(min_x);
		Obj.push_back(min_y);
		Obj.push_back(wid);
		Obj.push_back(heig);
		Bbox.push_back(Obj);
	}
}

//-------------------------------------------------------------------------------------
//Get bbox coordinates and store into registers
//-------------------------------------------------------------------------------------
void Tracking_STIP::matchRegions(){
	
	
}

//-------------------------------------------------------------------------------------
//Match stip features from a query set
//-------------------------------------------------------------------------------------
void Tracking_STIP::matchSTIPs(vector<vector<float>> Query, vector<vector<float>> Model){
	
	vector<int> Query_matches (Query.size(),0);	//Indices of Model that match Query STIPs (if found any) 

	float difval, refval;

	for(int i=0; i<Query.size(); i++){

		refval = 1000;	//initialize reference matching value

		for(int j=0; j<Model.size();j++){

			//check if the current index j has already taken into Query_matches
			std::vector<int>::iterator it = std::find (Query_matches.begin(), Query_matches.end(), j);	
			
			if (it != Query_matches.end()){

			}
			else{
				difval = Gkernel_vectors(Query[i], Model[j]);	//2norm between Query[i] and Model[j]

				if((difval<thr_match)&(difval<refval)){		//Validate the matching between Query[i] and Model[j]

					refval = difval;	//assign new reference matching value

					Query_matches[i] = j;	//assign Model j index to Query i
				}
			}
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
		cv::Mat Bb_RGB(F_RGB,cv::Rect(Bbox[i][0],Bbox[i][1],Bbox[i][2],Bbox[i][3]));
		cv::Mat Bb_LBSP(F_LBSP,cv::Rect(Bbox[i][0],Bbox[i][1],Bbox[i][2],Bbox[i][3]));
		
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
float Tracking_STIP::Gkernel_vectors(vector<float> A, vector<float> B)
{
	float normv = 0, gkernel;

	for(int i=0; i<A.size(); i++){
		normv = pow(A[i]-B[i],2)+normv;
	}
	sqrt(normv);

	gkernel = exp((normv*normv));

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


