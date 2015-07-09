#include "Input_source.h"



//-------------------------------------------------------------------------------------
//Constructor
Input_source::Input_source(){
}

//-------------------------------------------------------------------------------------
//Destructor
Input_source::~Input_source(){
}

//-------------------------------------------------------------------------------------
//Menu to select input source
int Input_source::menu() {
		std::cout<<"Subsense Background Subtraction Algorithm"<<std::endl;
		std::cout<<std::endl<<"Select input source: "<<std::endl;
		std::cout<<"1) Image sequence"<<std::endl;
		std::cout<<"2) Video"<<std::endl;
		std::cout<<"3) WebCam"<<std::endl;
	int src = 2; //Default input source: Video
	std::cin>>src;
	return src;
}


//-------------------------------------------------------------------------------------
//User image sequence selection:

//Returns a vector containing two vectors, in the first one are stored the categories 
//selected by the user and in the second are the videos

void Input_source::Im_seq_sel()
{
	int cat_sel;	//selected category identifier
	std::vector<int> cats_ids;	//selected categories identifiers (if choosen to run a whole dataset)

	//categories
	std::cout<<"Select video category (Change Detection 2014):"<<std::endl;
	std::cout<<"1) badWeather"<<std::endl;
	std::cout<<"2) baseline"<<std::endl;
	std::cout<<"3) cameraJitter"<<std::endl;
	std::cout<<"4) dynamicBackground"<<std::endl;
	std::cout<<"5) intermittentObjectMotion"<<std::endl;
	std::cout<<"6) lowFrameRate"<<std::endl;
	std::cout<<"7) nightVideos"<<std::endl;
	std::cout<<"8) PTZ"<<std::endl;
	std::cout<<"9) shadow"<<std::endl;
	std::cout<<"10) thermal"<<std::endl;
	std::cout<<"11) turbulence"<<std::endl;
	std::cout<<"12) survmantics"<<std::endl;
	std::cout<<"13) other datasets"<<std::endl;
	std::cout<<"14) All categories from Change Detection 2014"<<std::endl;
	std::cout<<"15) All datasets"<<std::endl;
	std::cin>>cat_sel;	//get selected category

	int vid_id;				//selected video identifier
	std::vector<int> vids_ids;	//selected videos identifiers (if choosen to run a whole category)

	std::vector<int> vids_cat;		//number of videos for each category

	for (int c=1; c<13; c++)
	{
		switch(c) //store number of videos per category
		{
		case 1: vids_cat.push_back(4);
		case 2: vids_cat.push_back(4);
		case 3: vids_cat.push_back(4);
		case 4: vids_cat.push_back(6);
		case 5: vids_cat.push_back(6);
		case 6: vids_cat.push_back(4);
		case 7: vids_cat.push_back(6);
		case 8: vids_cat.push_back(4);
		case 9: vids_cat.push_back(6);
		case 10: vids_cat.push_back(5);
		case 11: vids_cat.push_back(4);
		case 12: vids_cat.push_back(10);
		case 13: vids_cat.push_back(9);
		}
	}


	switch(cat_sel)	//switch through categories	
	{	
	case 1:	std::cout<<"Select video from category: badWeather"<<std::endl;
		std::cout<<"1) blizzard"<<std::endl;
		std::cout<<"2) skating"<<std::endl;
		std::cout<<"3) snowFall"<<std::endl;
		std::cout<<"4) wetSnow"<<std::endl;
		std::cout<<"5) All videos"<<std::endl;
		std::cin>>vid_id;			//get selected video
		cats_ids.push_back(1);	//category identifier

		switch(vid_id)	//switch through videos
		{		
		case 1: vids_ids.push_back(1);	//video identifier
			break;
		case 2: vids_ids.push_back(2);	//video identifier
			break;
		case 3: vids_ids.push_back(3);	//video identifier
			break;
		case 4: vids_ids.push_back(4);	//video identifier
			break;
		default:
		case 5: for(int v=1; v<(vids_cat[cat_sel]+1); v++)
				{
					vids_ids.push_back(v);	//video identifier
				}
				break;
		}
		break;
	default:
	case 2:	std::cout<<"Select video from category: baseline"<<std::endl;
		std::cout<<"1) highway"<<std::endl;
		std::cout<<"2) office"<<std::endl;
		std::cout<<"3) pedestrians"<<std::endl;
		std::cout<<"4) PETS2006"<<std::endl;
		std::cout<<"5) All videos"<<std::endl;
		std::cin>>vid_id;			//get selected video
		cats_ids.push_back(2);	//category identifier

		switch(vid_id)	//switch through videos
		{		
		case 1: vids_ids.push_back(1);	//video identifier
			break;
		case 2: vids_ids.push_back(2);	//video identifier
			break;
		case 3: vids_ids.push_back(3);	//video identifier
			break;
		case 4: vids_ids.push_back(4);	//video identifier
			break;
		default:
		case 5: for(int v=1; v<(vids_cat[cat_sel]+1); v++)
				{
					vids_ids.push_back(v);	//video identifier
				}
				break;
		} 
		break;
	case 3:	std::cout<<"Select video from category: cameraJitter"<<std::endl;
		std::cout<<"1) badminton"<<std::endl;
		std::cout<<"2) boulevard"<<std::endl;
		std::cout<<"3) sidewalk"<<std::endl;
		std::cout<<"4) traffic"<<std::endl;
		std::cout<<"5) All videos"<<std::endl;
		std::cin>>vid_id;			//get selected video
		cats_ids.push_back(3);	//category identifier

		switch(vid_id)	//switch through videos
		{		
		case 1: vids_ids.push_back(1);	//video identifier
			break;
		case 2: vids_ids.push_back(2);	//video identifier
			break;
		case 3: vids_ids.push_back(3);	//video identifier
			break;
		case 4: vids_ids.push_back(4);	//video identifier
			break;
		default:
		case 5: for(int v=1; v<(vids_cat[cat_sel]+1); v++)
				{
					vids_ids.push_back(v);	//video identifier
				}
				break;
		}
		break;
	case 4:	std::cout<<"Select video from category: dynamicBackground"<<std::endl;
		std::cout<<"1) boats"<<std::endl;
		std::cout<<"2) canoe"<<std::endl;
		std::cout<<"3) fall"<<std::endl;
		std::cout<<"4) fountain01"<<std::endl;
		std::cout<<"5) fountain02"<<std::endl;
		std::cout<<"6) overpass"<<std::endl;
		std::cout<<"7) All videos"<<std::endl;
		std::cin>>vid_id;			//get selected video
		cats_ids.push_back(4);	//category identifier

		switch(vid_id)	//switch through videos
		{		
		case 1: vids_ids.push_back(1);	//video identifier
			break;
		case 2: vids_ids.push_back(2);	//video identifier
			break;
		case 3: vids_ids.push_back(3);	//video identifier
			break;
		case 4: vids_ids.push_back(4);	//video identifier
			break;
		case 5: vids_ids.push_back(5);	//video identifier
			break;
		case 6: vids_ids.push_back(6);	//video identifier
			break;
		default:
		case 7: for(int v=1; v<(vids_cat[cat_sel]+1); v++)
				{
					vids_ids.push_back(v);	//video identifier
				}
				break;
		}
		break;
	case 5:	std::cout<<"Select video from category: intermittentObjectMotion"<<std::endl;
		std::cout<<"1) abbandonedBox"<<std::endl;
		std::cout<<"2) parking"<<std::endl;
		std::cout<<"3) sofa"<<std::endl;
		std::cout<<"4) streetLight"<<std::endl;
		std::cout<<"5) tramstop"<<std::endl;
		std::cout<<"6) winterDriveway"<<std::endl;
		std::cout<<"7) All videos"<<std::endl;
		std::cin>>vid_id;			//get selected video
		cats_ids.push_back(5);	//category identifier

		switch(vid_id)	//switch through videos
		{		
		case 1: vids_ids.push_back(1);	//video identifier
			break;
		case 2: vids_ids.push_back(2);	//video identifier
			break;
		case 3: vids_ids.push_back(3);	//video identifier
			break;
		case 4: vids_ids.push_back(4);	//video identifier
			break;
		case 5: vids_ids.push_back(5);	//video identifier
			break;
		case 6: vids_ids.push_back(6);	//video identifier
			break;
		default:
		case 7: for(int v=1; v<(vids_cat[cat_sel]+1); v++)
				{
					vids_ids.push_back(v);	//video identifier
				}
				break;
		}
		break;
	case 6:	std::cout<<"Select video from category: lowFramerate"<<std::endl;
		std::cout<<"1) port_0_17fps"<<std::endl;
		std::cout<<"2) tramCrossroad_1fps"<<std::endl;
		std::cout<<"3) tunnelExit_0_35fps"<<std::endl;
		std::cout<<"4) turnpike_0_5fps"<<std::endl;
		std::cout<<"5) All videos"<<std::endl;
		std::cin>>vid_id;			//get selected video
		cats_ids.push_back(6);	//category identifier

		switch(vid_id)	//switch through videos
		{		
		case 1: vids_ids.push_back(1);	//video identifier
			break;
		case 2: vids_ids.push_back(2);	//video identifier
			break;
		case 3: vids_ids.push_back(3);	//video identifier
			break;
		case 4: vids_ids.push_back(4);	//video identifier
			break;
		default:
		case 5: for(int v=1; v<(vids_cat[cat_sel]+1); v++)
				{
					vids_ids.push_back(v);	//video identifier
				}
				break;
		}
		break;
	case 7:	std::cout<<"Select video from category: nightVideos"<<std::endl;
		std::cout<<"1) bridgeEntry"<<std::endl;
		std::cout<<"2) bussyBoulevard"<<std::endl;
		std::cout<<"3) fluiHighway"<<std::endl;
		std::cout<<"4) streetCornerAtNight"<<std::endl;
		std::cout<<"5) tramStation"<<std::endl;
		std::cout<<"6) winterStreet"<<std::endl;
		std::cout<<"7) All videos"<<std::endl;
		std::cin>>vid_id;			//get selected video
		cats_ids.push_back(7);	//category identifier

		switch(vid_id)	//switch through videos
		{		
		case 1: vids_ids.push_back(1);	//video identifier
			break;
		case 2: vids_ids.push_back(2);	//video identifier
			break;
		case 3: vids_ids.push_back(3);	//video identifier
			break;
		case 4: vids_ids.push_back(4);	//video identifier
			break;
		case 5: vids_ids.push_back(5);	//video identifier
			break;
		case 6: vids_ids.push_back(6);	//video identifier
			break;
		default:
		case 7: for(int v=1; v<(vids_cat[cat_sel]+1); v++)
				{
					vids_ids.push_back(v);	//video identifier
				}
				break;
		}
		break;
	case 8:	std::cout<<"Select video from category: PTZ"<<std::endl;
		std::cout<<"1) continuosPan"<<std::endl;
		std::cout<<"2) intermittentPan"<<std::endl;
		std::cout<<"3) twoPositionPTZCam"<<std::endl;
		std::cout<<"4) zoomInZoomOut"<<std::endl;
		std::cout<<"5) All videos"<<std::endl;
		std::cin>>vid_id;			//get selected video
		cats_ids.push_back(8);	//category identifier

		switch(vid_id)	//switch through videos
		{		
		case 1: vids_ids.push_back(1);	//video identifier
			break;
		case 2: vids_ids.push_back(2);	//video identifier
			break;
		case 3: vids_ids.push_back(3);	//video identifier
			break;
		case 4: vids_ids.push_back(4);	//video identifier
			break;
		default:
		case 5: for(int v=1; v<(vids_cat[cat_sel]+1); v++)
				{
					vids_ids.push_back(v);	//video identifier
				}
				break;
		}
		break;
	case 9:	std::cout<<"Select video from category: shadow"<<std::endl;
		std::cout<<"1) backdoor"<<std::endl;
		std::cout<<"2) bungalows"<<std::endl;
		std::cout<<"3) busStation"<<std::endl;
		std::cout<<"4) copyMachine"<<std::endl;
		std::cout<<"5) cubicle"<<std::endl;
		std::cout<<"6) peopleInShade"<<std::endl;
		std::cout<<"7) All videos"<<std::endl;
		std::cin>>vid_id;			//get selected video
		cats_ids.push_back(9);	//category identifier

		switch(vid_id)	//switch through videos
		{		
		case 1: vids_ids.push_back(1);	//video identifier
			break;
		case 2: vids_ids.push_back(2);	//video identifier
			break;
		case 3: vids_ids.push_back(3);	//video identifier
			break;
		case 4: vids_ids.push_back(4);	//video identifier
			break;
		case 5: vids_ids.push_back(5);	//video identifier
			break;
		case 6: vids_ids.push_back(6);	//video identifier
			break;
		default:
		case 7: for(int v=1; v<(vids_cat[cat_sel]+1); v++)
				{
					vids_ids.push_back(v);	//video identifier
				}
				break;
		}
		break;
	case 10: std::cout<<"Select video from category: thermal"<<std::endl;
		std::cout<<"1) corridor"<<std::endl;
		std::cout<<"2) diningRoom"<<std::endl;
		std::cout<<"3) lakeSide"<<std::endl;
		std::cout<<"4) library"<<std::endl;
		std::cout<<"5) park"<<std::endl;
		std::cout<<"6) All videos"<<std::endl;
		std::cin>>vid_id;			//get selected video
		cats_ids.push_back(10);	//category identifier

		switch(vid_id)	//switch through videos
		{		
		case 1: vids_ids.push_back(1);	//video identifier
			break;
		case 2: vids_ids.push_back(2);	//video identifier
			break;
		case 3: vids_ids.push_back(3);	//video identifier
			break;
		case 4: vids_ids.push_back(4);	//video identifier
			break;
		case 5: vids_ids.push_back(5);	//video identifier
			break;
		default:
		case 6: for(int v=1; v<(vids_cat[cat_sel]+1); v++)
				{
					vids_ids.push_back(v);	//video identifier
				}
				break;
		}
		break;
	case 11: std::cout<<"Select video from category: turbulence"<<std::endl;
		std::cout<<"1) turbulence0"<<std::endl;
		std::cout<<"2) turbulence1"<<std::endl;
		std::cout<<"3) turbulence2"<<std::endl;
		std::cout<<"4) turbulence3"<<std::endl;
		std::cout<<"5) All videos"<<std::endl;
		std::cin>>vid_id;			//get selected video
		cats_ids.push_back(11);	//category identifier

		switch(vid_id)	//switch through videos
		{		
		case 1: vids_ids.push_back(1);	//video identifier
			break;
		case 2: vids_ids.push_back(2);	//video identifier
			break;
		case 3: vids_ids.push_back(3);	//video identifier
			break;
		case 4: vids_ids.push_back(4);	//video identifier
			break;
		default:
		case 5: for(int v=1; v<(vids_cat[cat_sel]+1); v++)
				{
					vids_ids.push_back(v);	//video identifier
				}
				break;
		}
		break;
	case 12: std::cout<<"Select video from category: survmantics"<<std::endl;
		std::cout<<"1) 00001"<<std::endl;
		std::cout<<"2) 00587"<<std::endl;
		std::cout<<"3) 00945"<<std::endl;
		std::cout<<"4) 04500"<<std::endl;
		std::cout<<"5) 22499"<<std::endl;
		std::cout<<"6) 37270"<<std::endl;
		std::cout<<"7) 48194"<<std::endl;
		std::cout<<"8) 54202"<<std::endl;
		std::cout<<"9) 62012"<<std::endl;
		std::cout<<"10) 70083"<<std::endl;
		std::cout<<"11) 80120"<<std::endl;
		std::cout<<"12) All videos"<<std::endl;
		std::cin>>vid_id;			//get selected video
		cats_ids.push_back(12);	//category identifier

		switch(vid_id)	//switch through videos
		{		
		case 1: vids_ids.push_back(vid_id);	//video identifier
			break;
		case 2: vids_ids.push_back(vid_id);	//video identifier
			break;
		case 3: vids_ids.push_back(vid_id);	//video identifier
			break;
		case 4: vids_ids.push_back(vid_id);	//video identifier
			break;
		case 5: vids_ids.push_back(vid_id);	//video identifier
			break;
		case 6: vids_ids.push_back(vid_id);	//video identifier
			break;
		case 7: vids_ids.push_back(vid_id);	//video identifier
			break;
		case 8: vids_ids.push_back(vid_id);	//video identifier
			break;
		case 9: vids_ids.push_back(vid_id);	//video identifier
			break;
		case 10: vids_ids.push_back(vid_id);	//video identifier
			break;
		case 11: vids_ids.push_back(vid_id);	//video identifier
			break;
		default:
		case 12: for(int v=1; v<(vids_cat[cat_sel]+1); v++)
				{
					vids_ids.push_back(v);	//video identifier
				}
				break;
		}
		break;
	case 13: std::cout<<"Select video from category: others"<<std::endl;
		std::cout<<"1) ShoppingMall"<<std::endl;
		std::cout<<"2) LeftBag"<<std::endl;
		std::cout<<"3) Hall"<<std::endl;
		std::cout<<"4) WaterSurface"<<std::endl;
		std::cout<<"5) Bootstrap"<<std::endl;
		std::cout<<"6) DariaWalk"<<std::endl;
		std::cout<<"7) MosheWalk"<<std::endl;
		std::cout<<"8) Limp"<<std::endl;
		std::cout<<"9) KneesUp"<<std::endl;
		std::cout<<"10) All videos"<<std::endl;
		std::cin>>vid_id;			//get selected video
		cats_ids.push_back(13);	//category identifier

		switch(vid_id)	//switch through videos
		{		
		case 1: vids_ids.push_back(1);	//video identifier
			break;
		case 2: vids_ids.push_back(2);	//video identifier
			break;
		case 3: vids_ids.push_back(3);	//video identifier
			break;
		case 4: vids_ids.push_back(4);	//video identifier
			break;
		case 5: vids_ids.push_back(5);	//video identifier
			break;
		case 6: vids_ids.push_back(6);	//video identifier
			break;
		case 7: vids_ids.push_back(7);	//video identifier
			break;
		case 8: vids_ids.push_back(8);	//video identifier
			break;
		case 9: vids_ids.push_back(8);	//video identifier
			break;
		default:
		case 10: for(int v=1; v<(vids_cat[cat_sel]+1); v++)
				{
					vids_ids.push_back(v);	//video identifier
				}
				break;
		}
		break;
	case 14: for(int c=1; c<11; c++)	
			 {
				 if(c!=3)	//avoid camera jitter category (excesive computational load given the amount of movement)
					cats_ids.push_back(c);		//category identifier
			 }
			 vids_ids.push_back(-1);		//'-1' as a flag to identify that all videos of several caregories are going to be used
			 break;
	case 15: for(int c=1; c<13; c++)	
			 {
				 if(c!=3)	//avoid camera jitter category	(excesive computational load given the amount of movement)
					cats_ids.push_back(c);		//category identifier
			 }
			 vids_ids.push_back(-1);		//'-1' as a flag to identify that all videos of several caregories are going to be used
			 break;
	}

	Im_seqs.push_back(cats_ids);	//store categories identifiers
	Im_seqs.push_back(vids_ids);	//store videos identifiers
	Im_seqs.push_back(vids_cat);	//store number of videos per category
}


//-------------------------------------------------------------------------------------
//User video selection:

void Input_source::Vid_sel(){
	strcpy(vpath,"D:\\B.flv");
	
	//For video fix the category and the image sequence as '1'
	
	sq.push_back(1);
	cat.push_back(1);
	Im_seqs.push_back(sq);
	Im_seqs.push_back(cat);
	endfr = 2;
	fr_idx = 0;
}

//-------------------------------------------------------------------------------------
//User WebCam selection:

void Input_source::WebCam_sel(){

	//For video fix the category and the image sequence as '1'
	
	sq.push_back(1);
	cat.push_back(1);
	Im_seqs.push_back(sq);
	Im_seqs.push_back(cat);
	endfr = 2;
	fr_idx = 0;
}


//-------------------------------------------------------------------------------------
//Get video information:

//according to the video selection made by the user, this function reads all the info.
//regarding to the input video:

//imgpath	= image root path 
//gtpath	= groundtruth path
//vidname	= video name 
//ext		= image format 
//gtext		= groundtruth image format 
//inifr		= initial frame id
//endfr		= final frame id
//save_path = path to save segmented images

void Input_source::Im_seq_info(int cat_sel, int vid_id)
{
	//videos and groud truths root paths
	strcpy(imgpath,"C:\\Users\\Santiago\\Dropbox\\Grupo Investigacion-Analisis de Movimiento\\Segmentacion\\");
	strcpy(gtpath,"C:\\Users\\Santiago\\Dropbox\\Grupo Investigacion-Analisis de Movimiento\\Segmentacion\\");
	
	//save segmented images root path
	strcpy(save_path,imgpath);

	switch(cat_sel)	//switch through categories	
	{	
	case 1:				
		switch(vid_id)	//switch through videos
		{		
		case 1: strcat(imgpath,"dataset\\badWeather\\blizzard\\input\\in00");
			strcat(gtpath,"dataset\\badWeather\\blizzard\\groundtruth\\gt00");
			strcpy(vidname,"blizzard");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 7000;
			strcat(save_path,"dataset\\badWeather\\blizzard\\SegIm");
			break;
		case 2: strcat(imgpath,"dataset\\badWeather\\skating\\input\\in00");
			strcat(gtpath,"dataset\\badWeather\\skating\\groundtruth\\gt00");
			strcpy(vidname,"skating");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 3900;
			strcat(save_path,"dataset\\badWeather\\skating\\SegIm");
			break;
		case 3: strcat(imgpath,"dataset\\badWeather\\snowFall\\input\\in00");
			strcat(gtpath,"dataset\\badWeather\\snowFall\\groundtruth\\gt00");
			strcpy(vidname,"snowFall");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 6500;
			strcat(save_path,"dataset\\badWeather\\snowFall\\SegIm");
			break;
		case 4: strcat(imgpath,"dataset\\badWeather\\wetSnow\\input\\in00");
			strcat(gtpath,"dataset\\badWeather\\wetSnow\\groundtruth\\gt00");
			strcpy(vidname,"wetSnow");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 3500;
			strcat(save_path,"dataset\\badWeather\\wetSnow\\SegIm");
			break;
		}
		break;
	case 2:				
		switch(vid_id)	//switch through videos
		{		
		case 1: strcat(imgpath,"dataset\\baseline\\highway\\input\\in00");
			strcat(gtpath,"dataset\\baseline\\highway\\groundtruth\\gt00");
			strcpy(vidname,"highway");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 1700;
			strcat(save_path,"dataset\\baseline\\highway\\SegIm");
			break;
		case 2: strcat(imgpath,"dataset\\baseline\\office\\input\\in00");
			strcat(gtpath,"dataset\\baseline\\office\\groundtruth\\gt00");
			strcpy(vidname,"office");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 2050;
			strcat(save_path,"dataset\\baseline\\office\\SegIm");
			break;
		case 3: strcat(imgpath,"dataset\\baseline\\pedestrians\\input\\in00");
			strcat(gtpath,"dataset\\baseline\\pedestrians\\groundtruth\\gt00");
			strcpy(vidname,"pedestrians");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 1099;
			strcat(save_path,"dataset\\baseline\\pedestrians\\SegIm");
			break;
		case 4: strcat(imgpath,"dataset\\baseline\\PETS2006\\input\\in00");
			strcat(gtpath,"dataset\\baseline\\PETS2006\\groundtruth\\gt00");
			strcpy(vidname,"PETS2006");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 1200;
			strcat(save_path,"dataset\\baseline\\PETS2006\\SegIm");
			break;
		}
		break;
	case 3:				
		switch(vid_id)	//switch through videos
		{		
		case 1: strcat(imgpath,"dataset\\cameraJitter\\badminton\\input\\in00");
			strcat(gtpath,"dataset\\cameraJitter\\badminton\\groundtruth\\gt00");
			strcpy(vidname,"badminton");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 1150;
			strcat(save_path,"dataset\\cameraJitter\\badminton\\SegIm");
			break;
		case 2: strcat(imgpath,"dataset\\cameraJitter\\boulevard\\input\\in00");
			strcat(gtpath,"dataset\\cameraJitter\\boulevard\\groundtruth\\gt00");
			strcpy(vidname,"boulevard");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 2500;
			strcat(save_path,"dataset\\cameraJitter\\boulevard\\SegIm");
			break;
		case 3: strcat(imgpath,"dataset\\cameraJitter\\sidewalk\\input\\in00");
			strcat(gtpath,"dataset\\cameraJitter\\sidewalk\\groundtruth\\gt00");
			strcpy(vidname,"sidewalk");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 1200;
			strcat(save_path,"dataset\\cameraJitter\\sidewalk\\SegIm");
			break;
		case 4: strcat(imgpath,"dataset\\cameraJitter\\traffic\\input\\in00");
			strcat(gtpath,"dataset\\cameraJitter\\traffic\\groundtruth\\gt00");
			strcpy(vidname,"traffic");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 1570;
			strcat(save_path,"dataset\\cameraJitter\\traffic\\SegIm");
			break;
		}
		break;
	case 4:				
		switch(vid_id)	//switch through videos
		{		
		case 1: strcat(imgpath,"dataset\\dynamicBackground\\boats\\input\\in00");
			strcat(gtpath,"dataset\\dynamicBackground\\boats\\groundtruth\\gt00");
			strcpy(vidname,"boats");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 7999;
			strcat(save_path,"dataset\\dynamicBackground\\boats\\SegIm");
			break;
		case 2: strcat(imgpath,"dataset\\dynamicBackground\\canoe\\input\\in00");
			strcat(gtpath,"dataset\\dynamicBackground\\canoe\\groundtruth\\gt00");
			strcpy(vidname,"canoe");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 1189;
			strcat(save_path,"dataset\\dynamicBackground\\canoe\\SegIm");
			break;
		case 3: strcat(imgpath,"dataset\\dynamicBackground\\fall\\input\\in00");
			strcat(gtpath,"dataset\\dynamicBackground\\fall\\groundtruth\\gt00");
			strcpy(vidname,"fall");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 4000;
			strcat(save_path,"dataset\\dynamicBackground\\fall\\SegIm");
			break;
		case 4: strcat(imgpath,"dataset\\dynamicBackground\\fountain01\\input\\in00");
			strcat(gtpath,"dataset\\dynamicBackground\\fountain01\\groundtruth\\gt00");
			strcpy(vidname,"fountain01");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 1184;
			strcat(save_path,"dataset\\dynamicBackground\\fountain01\\SegIm");
			break;
		case 5: strcat(imgpath,"dataset\\dynamicBackground\\fountain02\\input\\in00");
			strcat(gtpath,"dataset\\dynamicBackground\\fountain02\\groundtruth\\gt00");
			strcpy(vidname,"fountain02");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 1499;
			strcat(save_path,"dataset\\dynamicBackground\\fountain02\\SegIm");
			break;
		case 6: strcat(imgpath,"dataset\\dynamicBackground\\overpass\\input\\in00");
			strcat(gtpath,"dataset\\dynamicBackground\\overpass\\groundtruth\\gt00");
			strcpy(vidname,"overpass");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 3000;
			strcat(save_path,"dataset\\dynamicBackground\\overpass\\SegIm");
			break;
		}
		break;
	case 5:				
		switch(vid_id)	//switch through videos
		{		
		case 1: strcat(imgpath,"dataset\\intermittentObjectMotion\\abandonedBox\\input\\in00");
			strcat(gtpath,"dataset\\intermittentObjectMotion\\abandonedBox\\groundtruth\\gt00");
			strcpy(vidname,"boats");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 4500;
			strcat(save_path,"dataset\\intermittentObjectMotion\\abandonedBox\\SegIm");
			break;
		case 2: strcat(imgpath,"dataset\\intermittentObjectMotion\\parking\\input\\in00");
			strcat(gtpath,"dataset\\intermittentObjectMotion\\parking\\groundtruth\\gt00");
			strcpy(vidname,"parking");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 2500;
			strcat(save_path,"dataset\\intermittentObjectMotion\\parking\\SegIm");
			break;
		case 3: strcat(imgpath,"dataset\\intermittentObjectMotion\\sofa\\input\\in00");
			strcat(gtpath,"dataset\\intermittentObjectMotion\\sofa\\groundtruth\\gt00");
			strcpy(vidname,"sofa");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 2750;
			strcat(save_path,"dataset\\intermittentObjectMotion\\sofa\\SegIm");
			break;
		case 4: strcat(imgpath,"dataset\\intermittentObjectMotion\\streetLight\\input\\in00");
			strcat(gtpath,"dataset\\intermittentObjectMotion\\streetLight\\groundtruth\\gt00");
			strcpy(vidname,"streetLight");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 3200;
			strcat(save_path,"dataset\\intermittentObjectMotion\\streetLight\\SegIm");
			break;
		case 5: strcat(imgpath,"dataset\\intermittentObjectMotion\\tramStop\\input\\in00");
			strcat(gtpath,"dataset\\intermittentObjectMotion\\tramStop\\groundtruth\\gt00");
			strcpy(vidname,"tramStop");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 3200;
			strcat(save_path,"dataset\\intermittentObjectMotion\\tramStop\\SegIm");
			break;
		case 6: strcat(imgpath,"dataset\\intermittentObjectMotion\\winterDriveway\\input\\in00");
			strcat(gtpath,"dataset\\intermittentObjectMotion\\winterDriveway\\groundtruth\\gt00");
			strcpy(vidname,"winterDriveway");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 2500;
			strcat(save_path,"dataset\\intermittentObjectMotion\\winterDriveway\\SegIm");
			break;
		}
		break;
	case 6:				
		switch(vid_id)	//switch through videos
		{		
		case 1: strcat(imgpath,"dataset\\lowFramerate\\port_0_17fps\\input\\in00");
			strcat(gtpath,"dataset\\lowFramerate\\port_0_17fps\\groundtruth\\gt00");
			strcpy(vidname,"port_0_17fps");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 3000;
			strcat(save_path,"dataset\\lowFramerate\\port_0_17fps\\SegIm");
			break;
		case 2: strcat(imgpath,"dataset\\lowFramerate\\tramCrossroad_1fps\\input\\in00");
			strcat(gtpath,"dataset\\lowFramerate\\tramCrossroad_1fps\\groundtruth\\gt00");
			strcpy(vidname,"tramCrossroad_1fps");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 900;
			strcat(save_path,"dataset\\lowFramerate\\tramCrossroad_1fps\\SegIm");
			break;
		case 3: strcat(imgpath,"dataset\\lowFramerate\\tunnelExit_0_35fps\\input\\in00");
			strcat(gtpath,"dataset\\lowFramerate\\tunnelExit_0_35fps\\groundtruth\\gt00");
			strcpy(vidname,"tunnelExit_0_35fps");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 4000;
			strcat(save_path,"dataset\\lowFramerate\\tunnelExit_0_35fps\\SegIm");
			break;
		case 4: strcat(imgpath,"dataset\\lowFramerate\\turnpike_0_5fps\\input\\in00");
			strcat(gtpath,"dataset\\lowFramerate\\turnpike_0_5fps\\groundtruth\\gt00");
			strcpy(vidname,"turnpike_0_5fps");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 1500;
			strcat(save_path,"dataset\\lowFramerate\\turnpike_0_5fps\\SegIm");
			break;
		}
		break;
	case 7:				
		switch(vid_id)	//switch through videos
		{		
		case 1: strcat(imgpath,"dataset\\nightVideos\\bridgeEntry\\input\\in00");
			strcat(gtpath,"dataset\\nightVideos\\bridgeEntry\\groundtruth\\gt00");
			strcpy(vidname,"bridgeEntry");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 2500;
			strcat(save_path,"dataset\\nightVideos\\bridgeEntry\\SegIm");
			break;
		case 2: strcat(imgpath,"dataset\\nightVideos\\busyBoulvard\\input\\in00");
			strcat(gtpath,"dataset\\nightVideos\\busyBoulvard\\groundtruth\\gt00");
			strcpy(vidname,"busyBoulvard");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 2760;
			strcat(save_path,"dataset\\nightVideos\\busyBoulvard\\SegIm");
			break;
		case 3: strcat(imgpath,"dataset\\nightVideos\\fluidHighway\\input\\in00");
			strcat(gtpath,"dataset\\nightVideos\\fluidHighway\\groundtruth\\gt00");
			strcpy(vidname,"fluidHighway");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 1364;
			strcat(save_path,"dataset\\nightVideos\\fluidHighway\\SegIm");
			break;
		case 4: strcat(imgpath,"dataset\\nightVideos\\streetCornerAtNight\\input\\in00");
			strcat(gtpath,"dataset\\nightVideos\\streetCornerAtNight\\groundtruth\\gt00");
			strcpy(vidname,"streetCornerAtNight");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 5200;
			strcat(save_path,"dataset\\nightVideos\\streetCornerAtNight\\SegIm");
			break;
		case 5: strcat(imgpath,"dataset\\nightVideos\\tramStation\\input\\in00");
			strcat(gtpath,"dataset\\nightVideos\\tramStation\\groundtruth\\gt00");
			strcpy(vidname,"tramStation");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 3000;
			strcat(save_path,"dataset\\nightVideos\\tramStation\\SegIm");
			break;
		case 6: strcat(imgpath,"dataset\\nightVideos\\winterStreet\\input\\in00");
			strcat(gtpath,"dataset\\nightVideos\\winterStreet\\groundtruth\\gt00");
			strcpy(vidname,"winterStreet");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 1785;
			strcat(save_path,"dataset\\nightVideos\\winterStreet\\SegIm");
			break;
		}
		break;
	case 8:				
		switch(vid_id)	//switch through videos
		{		
		case 1: strcat(imgpath,"dataset\\PTZ\\continuousPan\\input\\in00");
			strcat(gtpath,"dataset\\PTZ\\continuousPan\\groundtruth\\gt00");
			strcpy(vidname,"continuousPan");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 1700;
			strcat(save_path,"dataset\\PTZ\\continuousPan\\SegIm");
			break;
		case 2: strcat(imgpath,"dataset\\PTZ\\intermittentPan\\input\\in00");
			strcat(gtpath,"dataset\\PTZ\\intermittentPan\\groundtruth\\gt00");
			strcpy(vidname,"intermittentPan");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 3500;
			strcat(save_path,"dataset\\PTZ\\intermittentPan\\SegIm");
			break;
		case 3: strcat(imgpath,"dataset\\PTZ\\twoPositionPTZCam\\input\\in00");
			strcat(gtpath,"dataset\\PTZ\\twoPositionPTZCam\\groundtruth\\gt00");
			strcpy(vidname,"twoPositionPTZCam");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 2300;
			strcat(save_path,"dataset\\PTZ\\twoPositionPTZCam\\SegIm");
			break;
		case 4: strcat(imgpath,"dataset\\PTZ\\zoomInZoomOut\\input\\in00");
			strcat(gtpath,"dataset\\PTZ\\zoomInZoomOut\\groundtruth\\gt00");
			strcpy(vidname,"zoomInZoomOut");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 1130;
			strcat(save_path,"dataset\\PTZ\\zoomInZoomOut\\SegIm");
			break;
		}
		break;
	case 9:				
		switch(vid_id)	//switch through videos
		{		
		case 1: strcat(imgpath,"dataset\\shadow\\backdoor\\input\\in00");
			strcat(gtpath,"dataset\\shadow\\backdoor\\groundtruth\\gt00");
			strcpy(vidname,"backdoor");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 2000;
			strcat(save_path,"dataset\\shadow\\backdoor\\SegIm");
			break;
		case 2: strcat(imgpath,"dataset\\shadow\\bungalows\\input\\in00");
			strcat(gtpath,"dataset\\shadow\\bungalows\\groundtruth\\gt00");
			strcpy(vidname,"bungalows");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 1700;
			strcat(save_path,"dataset\\shadow\\bungalows\\SegIm");
			break;
		case 3: strcat(imgpath,"dataset\\shadow\\busStation\\input\\in00");
			strcat(gtpath,"dataset\\shadow\\busStation\\groundtruth\\gt00");
			strcpy(vidname,"busStation");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 1250;
			strcat(save_path,"dataset\\shadow\\busStation\\SegIm");
			break;
		case 4: strcat(imgpath,"dataset\\shadow\\copyMachine\\input\\in00");
			strcat(gtpath,"dataset\\shadow\\copyMachine\\groundtruth\\gt00");
			strcpy(vidname,"copyMachine");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 3400;
			strcat(save_path,"dataset\\shadow\\copyMachine\\SegIm");
			break;
		case 5: strcat(imgpath,"dataset\\shadow\\cubicle\\input\\in00");
			strcat(gtpath,"dataset\\shadow\\cubicle\\groundtruth\\gt00");
			strcpy(vidname,"cubicle");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 7400;
			strcat(save_path,"dataset\\shadow\\cubicle\\SegIm");
			break;
		case 6: strcat(imgpath,"dataset\\shadow\\peopleInShade\\input\\in00");
			strcat(gtpath,"dataset\\shadow\\peopleInShade\\groundtruth\\gt00");
			strcpy(vidname,"peopleInShade");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 1199;
			strcat(save_path,"dataset\\shadow\\peopleInShade\\SegIm");
			break;
		}
		break;
	case 10:				
		switch(vid_id)	//switch through videos
		{		
		case 1: strcat(imgpath,"dataset\\thermal\\corridor\\input\\in00");
			strcat(gtpath,"dataset\\thermal\\corridor\\groundtruth\\gt00");
			strcpy(vidname,"corridor");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 5400;
			strcat(save_path,"dataset\\thermal\\corridor\\SegIm");
			break;
		case 2: strcat(imgpath,"dataset\\thermal\\diningRoom\\input\\in00");
			strcat(gtpath,"dataset\\thermal\\diningRoom\\groundtruth\\gt00");
			strcpy(vidname,"diningRoom");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 3700;
			strcat(save_path,"dataset\\thermal\\diningRoom\\SegIm");
			break;
		case 3: strcat(imgpath,"dataset\\thermal\\lakeSide\\input\\in00");
			strcat(gtpath,"dataset\\thermal\\lakeSide\\groundtruth\\gt00");
			strcpy(vidname,"lakeSide");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 6500;
			strcat(save_path,"dataset\\thermal\\lakeSide\\SegIm");
			break;
		case 4: strcat(imgpath,"dataset\\thermal\\library\\input\\in00");
			strcat(gtpath,"dataset\\thermal\\library\\groundtruth\\gt00");
			strcpy(vidname,"library");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 4900;
			strcat(save_path,"dataset\\thermal\\library\\SegIm");
			break;
		case 5: strcat(imgpath,"dataset\\thermal\\park\\input\\in00");
			strcat(gtpath,"dataset\\thermal\\park\\groundtruth\\gt00");
			strcpy(vidname,"park");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 600;
			strcat(save_path,"dataset\\thermal\\park\\SegIm");
			break;
		}
		break;
	case 11:				
		switch(vid_id)	//switch through videos
		{		
		case 1: strcat(imgpath,"dataset\\turbulence\\turbulence0\\input\\in00");
			strcat(gtpath,"dataset\\turbulence\\turbulence0\\groundtruth\\gt00");
			strcpy(vidname,"turbulence0");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 5000;
			strcat(save_path,"dataset\\turbulence\\turbulence0\\SegIm");
			break;
		case 2: strcat(imgpath,"dataset\\turbulence\\turbulence1\\input\\in00");
			strcat(gtpath,"dataset\\turbulence\\turbulence1\\groundtruth\\gt00");
			strcpy(vidname,"turbulence1");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 4000;
			strcat(save_path,"dataset\\turbulence\\turbulence1\\SegIm");
			break;
		case 3: strcat(imgpath,"dataset\\turbulence\\turbulence2\\input\\in00");
			strcat(gtpath,"dataset\\turbulence\\turbulence2\\groundtruth\\gt00");
			strcpy(vidname,"turbulence2");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 4500;
			strcat(save_path,"dataset\\turbulence\\turbulence2\\SegIm");
			break;
		case 4: strcat(imgpath,"dataset\\turbulence\\turbulence3\\input\\in00");
			strcat(gtpath,"dataset\\turbulence\\turbulence3\\groundtruth\\gt00");
			strcpy(vidname,"turbulence3");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 2200;
			strcat(save_path,"dataset\\turbulence\\turbulence3\\SegIm");
			break;
		}
		break;
	case 12:				
		switch(vid_id)	//switch through videos
		{		
		case 1: strcpy(imgpath,"D:\\Challenge_Surveillance\\00001\\image0");
			strcat(gtpath,"dataset\\turbulence\\turbulence0\\groundtruth\\gt00");
			strcpy(vidname,"survmantics00001");
			strcpy(ext,"png");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 822;
			strcat(save_path,"D:\\Challenge_Surveillance\\00001\\SegIm");
			break;
		case 2: strcpy(imgpath,"D:\\Challenge_Surveillance\\00587\\image0");
			strcat(gtpath,"dataset\\turbulence\\turbulence0\\groundtruth\\gt00");
			strcpy(vidname,"survmantics00587");
			strcpy(ext,"png");
			strcpy(gtext,"png");
			inifr = 174;
			endfr = 887;
			strcat(save_path,"D:\\Challenge_Surveillance\\00587\\SegIm");
			break;
		case 3: strcpy(imgpath,"D:\\Challenge_Surveillance\\00945\\image0");
			strcat(gtpath,"dataset\\turbulence\\turbulence0\\groundtruth\\gt00");
			strcpy(vidname,"survmantics00945");
			strcpy(ext,"png");
			strcpy(gtext,"png");
			inifr = 945;
			endfr = 1158;
			strcat(save_path,"D:\\Challenge_Surveillance\\00945\\SegIm");
			break;
		case 4: strcpy(imgpath,"D:\\Challenge_Surveillance\\04500\\image0");
			strcat(gtpath,"dataset\\turbulence\\turbulence0\\groundtruth\\gt00");
			strcpy(vidname,"survmantics04500");
			strcpy(ext,"png");
			strcpy(gtext,"png");
			inifr = 4500;
			endfr = 4719;
			strcat(save_path,"D:\\Challenge_Surveillance\\04500\\SegIm");
			break;
		case 5: strcpy(imgpath,"D:\\Challenge_Surveillance\\22499\\image2");
			strcat(gtpath,"dataset\\turbulence\\turbulence0\\groundtruth\\gt00");
			strcpy(vidname,"survmantics22499");
			strcpy(ext,"png");
			strcpy(gtext,"png");
			inifr = 2499;
			endfr = 2841;
			strcat(save_path,"D:\\Challenge_Surveillance\\22499\\SegIm");
			break;
		case 6: strcpy(imgpath,"D:\\Challenge_Surveillance\\37270\\image3");
			strcat(gtpath,"dataset\\turbulence\\turbulence0\\groundtruth\\gt00");
			strcpy(vidname,"survmantics37270");
			strcpy(ext,"png");
			strcpy(gtext,"png");
			inifr = 7270;
			endfr = 7472;
			strcat(save_path,"D:\\Challenge_Surveillance\\37270\\SegIm");
			break;
		case 7: strcpy(imgpath,"D:\\Challenge_Surveillance\\48194\\image4");
			strcat(gtpath,"dataset\\turbulence\\turbulence0\\groundtruth\\gt00");
			strcpy(vidname,"survmantics48194");
			strcpy(ext,"png");
			strcpy(gtext,"png");
			inifr = 8194;
			endfr = 8411;
			strcat(save_path,"D:\\Challenge_Surveillance\\48194\\SegIm");
			break;
		case 8: strcpy(imgpath,"D:\\Challenge_Surveillance\\54202\\image5");
			strcat(gtpath,"dataset\\turbulence\\turbulence0\\groundtruth\\gt00");
			strcpy(vidname,"survmantics54202");
			strcpy(ext,"png");
			strcpy(gtext,"png");
			inifr = 4202;
			endfr = 4580;
			strcat(save_path,"D:\\Challenge_Surveillance\\54202\\SegIm");
			break;
		case 9: strcpy(imgpath,"D:\\Challenge_Surveillance\\62012\\image6");
			strcat(gtpath,"dataset\\turbulence\\turbulence0\\groundtruth\\gt00");
			strcpy(vidname,"survmantics62012");
			strcpy(ext,"png");
			strcpy(gtext,"png");
			inifr = 2012;
			endfr = 2233;
			strcat(save_path,"D:\\Challenge_Surveillance\\62012\\SegIm");
			break;
		case 10: strcpy(imgpath,"D:\\Challenge_Surveillance\\70083\\image7");
			strcat(gtpath,"dataset\\turbulence\\turbulence0\\groundtruth\\gt00");
			strcpy(vidname,"survmantics70083");
			strcpy(ext,"png");
			strcpy(gtext,"png");
			inifr = 83;
			endfr = 343;
			strcat(save_path,"D:\\Challenge_Surveillance\\70083\\SegIm");
			break;
		case 11: strcpy(imgpath,"D:\\Challenge_Surveillance\\80120\\image8");
			strcat(gtpath,"dataset\\turbulence\\turbulence0\\groundtruth\\gt00");
			strcpy(vidname,"survmantics80120");
			strcpy(ext,"png");
			strcpy(gtext,"png");
			inifr = 120;
			endfr = 455;
			strcat(save_path,"D:\\Challenge_Surveillance\\80120\\SegIm");
			break;
		}
		break;
	case 13:				
		switch(vid_id)	//switch through videos
		{		
		case 1: strcat(imgpath,"Bases de Datos Imagenes\\ShoppingMall\\ShoppingMall");
			strcat(gtpath,"Bases de Datos Imagenes\\ShoppingMall\\Ground_truth\\gt_new_ShoppingMall");
			strcpy(vidname,"ShoppingMall");
			strcpy(ext,"bmp");
			strcpy(gtext,"bmp");
			inifr = 1005;
			endfr = 2150;
			strcat(save_path,"Bases de Datos Imagenes\\ShoppingMall\\SegIm");
			break;
		case 2: strcat(imgpath,"Bases de Datos Imagenes\\Left_packages_videos\\LeftBag\\JPEGS\\LeftBag");
			strcat(gtpath,"Bases de Datos Imagenes\\Left_packages_videos\\LeftBag\\JPEGS\\Ground_truth\\gt_new_LeftBag");
			strcpy(vidname,"LeftBag");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1;
			endfr = 996;
			strcat(save_path,"Bases de Datos Imagenes\\Left_packages_videos\\LeftBag\\SegIm");
			break;
		case 3: strcat(imgpath,"Bases de Datos Imagenes\\Hall\\airport");
			strcat(gtpath,"Bases de Datos Imagenes\\Hall\\Ground_truth\\gt_new_airport");
			strcpy(vidname,"Hall");
			strcpy(ext,"bmp");
			strcpy(gtext,"bmp");
			inifr = 1000;
			endfr = 4440;
			strcat(save_path,"Bases de Datos Imagenes\\Hall\\SegIm");
			break;
		case 4: strcat(imgpath,"Bases de Datos Imagenes\\WaterSurface\\WaterSurface");
			strcat(gtpath,"Bases de Datos Imagenes\\WaterSurface\\Ground_truth\\gt_new_WaterSurface");
			strcpy(vidname,"WaterSurface");
			strcpy(ext,"bmp");
			strcpy(gtext,"bmp");
			inifr = 1000;
			endfr = 1632;
			strcat(save_path,"Bases de Datos Imagenes\\WaterSurface\\SegIm");
			break;
		case 5: strcat(imgpath,"Bases de Datos Imagenes\\Bootstrap\\b0");
			strcat(gtpath,"Bases de Datos Imagenes\\Bootstrap\\Ground_truth\\gt_new_b0");
			strcpy(vidname,"Bootstrap");
			strcpy(ext,"bmp");
			strcpy(gtext,"bmp");
			inifr = 1;
			endfr = 2800;
			strcat(save_path,"Bases de Datos Imagenes\\Bootstrap\\SegIm");
			break;
		case 6: strcat(imgpath,"Bases de Datos Imagenes\\Activity_recognition\\Walk\\daria_walk\\daria_walk00");
			strcat(gtpath,"Bases de Datos Imagenes\\Activity_recognition\\Walk\\daria_walk\\Ground_truth\\gt_daria_walk");
			strcpy(vidname,"dariaWalk");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1001;
			endfr = 1084;
			strcat(save_path,"Bases de Datos Imagenes\\Activity_recognition\\Walk\\daria_walk\\SegIm");
			break;
		case 7: strcat(imgpath,"Bases de Datos Imagenes\\Activity_recognition\\Walk\\moshe_walk\\moshe_walk00");
			strcat(gtpath,"Bases de Datos Imagenes\\Activity_recognition\\Walk\\moshe_walk\\Ground_truth\\gt_moshe_walk");
			strcpy(vidname,"moshe_walk");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1001;
			endfr = 1078;
			strcat(save_path,"Bases de Datos Imagenes\\Activity_recognition\\Walk\\moshe_walk\\SegIm");
			break;
		case 8: strcat(imgpath,"Bases de Datos Imagenes\\Activity_recognition\\original_limp\\original_limp00");
			strcat(gtpath,"Bases de Datos Imagenes\\Activity_recognition\\original_limp\\Ground_truth\\gt_robust_limp");
			strcpy(vidname,"robustLimp");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1001;
			endfr = 1106;
			strcat(save_path,"Bases de Datos Imagenes\\Activity_recognition\\original_limp\\SegIm");
			break;
		case 9: strcat(imgpath,"Bases de Datos Imagenes\\Activity_recognition\\original_kneesup\\original_kneesup00");
			strcat(gtpath,"Bases de Datos Imagenes\\Activity_recognition\\original_kneesup\\Ground_truth\\gt_robust_kneesup");
			strcpy(vidname,"kneesup");
			strcpy(ext,"jpg");
			strcpy(gtext,"png");
			inifr = 1001;
			endfr = 1103;
			strcat(save_path,"Bases de Datos Imagenes\\Activity_recognition\\original_kneesup\\SegIm");
			break;
		}
		break;
	}
}


//-------------------------------------------------------------------------------------
//Read image from path:

//reads an image specified by the root path 'imgpath', the functions adds the frame id 
//according to 'i' and fills with zeros to complete a 4 digit number, afterwards the 
//image format 'ext' is added, then the image is converted to float and stored in 'Im'

cv::Mat Input_source::ReadIm(int i)
{
	itoa(i,fr,10);
	char fpath[270];

	strcpy(fpath,imgpath);	//copy root path 

	//Fill with zeros
	if(i<10)
	{
		strcat(fpath,"000");
	}
	else
	{
		if(i<100)
		{
			strcat(fpath,"00");
		}
		else
		{
			if(i<1000)
			{
				strcat(fpath,"0");
			}
		}
	}

	//copy video information
	strcat(fpath,fr);
	strcat(fpath,".");
	strcat(fpath,ext);
	std::cout<<std::endl<<fpath;
	cv::Mat Im = cv::imread(fpath,CV_LOAD_IMAGE_COLOR);
	if (Im.data)
	{
		//imshow("Image:",Im);
	}
	else
	{
		printf("Could not load image file: %s\n",fpath);
	}
	return Im;
}



