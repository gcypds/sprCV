#include "Bbox_Measures_XML.h"

int main()
{
	//-----------------------------Paths for reading XML---------------------------
	char path[200], pathA1[200], pathA2[200], pathA3[200], pathA4[200], 
		pathA5[200], vidname[50];

	strcpy_s(path, "D:\\People_Detection\\XML_results\\");	//Main path to XMLs
	strcpy_s(vidname,"survmantics04500");					//Video root name

	strcpy_s(pathA1,path);
	strcat_s(pathA1,vidname);
	strcat_s(pathA1,"alg1.xml");	//XML algorithm 1

	strcpy_s(pathA2,path);
	strcat_s(pathA2,vidname);
	strcat_s(pathA2,"alg2.xml");	//XML algorithm 2

	strcpy_s(pathA3,path);
	strcat_s(pathA3,vidname);
	strcat_s(pathA3,"alg3.xml");	//XML algorithm 3

	strcpy_s(pathA4,path);
	strcat_s(pathA4,vidname);
	strcat_s(pathA4,"algCV.xml");	//XML algorithm 4

	strcpy_s(pathA5,path);
	strcat_s(pathA5,vidname);
	strcat_s(pathA5,"GT.xml");		//XML Ground-truth
	
	//------------------Create object for the XML bbox comparison-----------------
	Bbox_Measures_XML BBoxM(vidname);

	//-------------------------Visualization of all XMLs--------------------------
	
	//Read image sequence for visualization
	char Impath[200], fr_id[10];
	strcpy_s(Impath,"D:\\People_Detection\\Videos\\");	//Path to videos
	strcat_s(Impath,vidname);
	strcat_s(Impath,"\\image04500.png");				//Initial frame name
	cv::VideoCapture sequence(Impath);
	BBoxM.oVideoInput = sequence;						//Copy sequence to object

	//Visualization function
	BBoxM.XML_visualization(pathA1, pathA2, pathA3, pathA4, pathA5);

	//-------XML comparison against ground-truth with supervised measures---------
	float Recall, Precision, F1;
	BBoxM.Calculate_Measures(pathA4, pathA5, Recall, Precision, F1); 
	cout<<"Recall:"<<Recall<<" Precision:"<<Precision<<" F1:"<<F1<<endl;
	system("pause");

	return 0;
}
