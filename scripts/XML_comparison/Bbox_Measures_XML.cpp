#include "Bbox_Measures_XML.h"


Bbox_Measures_XML::Bbox_Measures_XML(char *vidname)
{
	area_thr = 0.5;

	Vidname = vidname;
}

Bbox_Measures_XML::~Bbox_Measures_XML()
{

}

//-------------------------------------------------------------------------------------
//Calculates the Recall, Precision, F1 metrics when comparing two XML files
//-------------------------------------------------------------------------------------
void Bbox_Measures_XML::Calculate_Measures(char *XML_GT, char *XML_ALG, float &Recall, float &Precision, float &F1)
{
	unique_ptr<gtrois> GT;
	unique_ptr<gtrois> ALG;

	//Read XML files
	ReadXML(XML_GT, GT);
	ReadXML(XML_ALG, ALG);

	//Vectors to store ROIs per frame
	vector<P_tuple> V1;
	vector<P_tuple> V2;
	
	//Aux vector for states 
	vector<int> states;

	//Vector to store Tp, Fp, Fn
	vector<int> Cvalues(3,0);

	//Iterate through video frames
	for(size_t i=0; i<GT->frame().size(); i++)
	{		
		//Iterate through ROIS of Alg1
		frame f1 = GT->frame().at(i);

		V1.clear();
		for(frame::roi_sequence::iterator r = f1.roi().begin(); r != f1.roi().end(); r++)
		{
			states.clear();
			states.push_back(r->x().get());
			states.push_back(r->y().get());
			states.push_back(r->w().get());
			states.push_back(r->h().get());

			//Store ROIs per frame 
			V1.push_back(std::make_tuple(r->weight().get(),states));
		}

		//Iterate through ROIS of Alg2
		frame f2 = ALG->frame().at(i);
				
		V2.clear();
		for(frame::roi_sequence::iterator r = f2.roi().begin(); r != f2.roi().end(); r++) 
		{
			states.clear();
			states.push_back(r->x().get());
			states.push_back(r->y().get());
			states.push_back(r->w().get());
			states.push_back(r->h().get());

			//Store ROIs per frame 
			V2.push_back(std::make_tuple(r->weight().get(),states));
		}

		//Get Tp, Fp, Fn comparing against ground truth, stored in Cvalues vector
		Comparison_Values(V1, V2, Cvalues);
	}
	Recall = float(Cvalues[0])/float(Cvalues[0]+Cvalues[2]);
	Precision = float(Cvalues[0])/float(Cvalues[0]+Cvalues[1]);
	F1 = (2*Recall*Precision)/(Recall+Precision);
}

//-------------------------------------------------------------------------------------
//Visualize the bounding boxes stored in up to five XML files
//-------------------------------------------------------------------------------------
void Bbox_Measures_XML::XML_visualization(char *XML1, char *XML2, char *XML3, char *XML4, char *XML5)
{
	static const int max_algs = 5;
	int algs = 1;

	unique_ptr<gtrois> ALGS[max_algs];
	unique_ptr<gtrois> ALG;
	
	//Read XML files
	ReadXML(XML1, ALG);
	ALGS[0] = std::move(ALG);

	if(XML2!=NULL)
	{
		ReadXML(XML2, ALG);
		ALGS[1] = std::move(ALG);
		algs++;
	}
	if(XML3!=NULL)
	{
		ReadXML(XML3, ALG);
		ALGS[2] = std::move(ALG);
		algs++;
	}
	if(XML4!=NULL)
	{
		ReadXML(XML4, ALG);
		ALGS[3] = std::move(ALG);
		algs++;
	}
	if(XML5!=NULL)
	{
		ReadXML(XML5, ALG);
		ALGS[4] = std::move(ALG);
		algs++;
	}

	//Aux vector for states 
	vector<int> aux;

	//vector of the ROIs states per frame
	vector<vector<int>> states;

	//Read Image info
	char alg_id[10], plot_id[15];
	
	//Iterate through video frames
	for(size_t i=0; i<ALGS[0]->frame().size(); i++)
	{		
		//Read Image
		oVideoInput>>Im_RGB;

		//Iterate through XML files
		for(size_t j=0; j<algs; j++)
		{
			//Iterate through ROIS of Alg1
			frame f1 = ALGS[j]->frame().at(i);
			
			states.clear();
			for(frame::roi_sequence::iterator r = f1.roi().begin(); r != f1.roi().end(); r++)
			{
				aux.clear();
				aux.push_back(r->x().get());
				aux.push_back(r->y().get());
				aux.push_back(r->w().get());
				aux.push_back(r->h().get());

				//Store ROIs per frame 
				states.push_back(aux);
			}
			//Visualization
			itoa(j,alg_id,10);
			strcpy_s(plot_id,"Alg");
			strcat_s(plot_id,alg_id);
			ShowDetections(states, plot_id);
			cv::waitKey();
		}
	}
}

//-------------------------------------------------------------------------------------
//Reads XML file
//-------------------------------------------------------------------------------------
void Bbox_Measures_XML::ReadXML(string inFilename, unique_ptr<gtrois> &v) 
{	
	try 
	{
		ifstream ifs (inFilename);
		v = gtrois_ (ifs);
		ifs.close ();		
	} catch (const xml_schema::exception& e) 
	{
		std::cerr << e << std::endl;
		return;
	}
}

//-------------------------------------------------------------------------------------
//Gets Tp, Fp, Fn from the intersection/union of a set of bounding boxes of the same frame
//-------------------------------------------------------------------------------------
void Bbox_Measures_XML::Comparison_Values(vector<P_tuple> BBox1,vector<P_tuple> BBox2, vector<int> &C_values)
{
	int Tp = 0, Fp = 0, Fn = 0;
	int int_area, uni_area;
	float ref_area, cur_area;
	int k, mk;

	vector<int> States1, States2;

	// se recorre todos los bounding box para determinar si hay intersección
	for (int i=0;i<BBox1.size();i++)
	{
		//Get BBox1 states
		States1 = std::get<1>(BBox1[i]);

		ref_area = 0;

		k = 0, mk = 0;

		//validate states
		if(States1[2]>10&&States1[3]>10)
		{
			while (k<BBox2.size())
			{
				//Get BBox1 states
				States2 = std::get<1>(BBox2[k]);

				//validate states
				if(States2[2]>10&&States2[3]>10)
				{
					//Calculate intercected area 
					int_area = Intersect_Area(States1, States2);

					mk = 0;

					//Caculate bonded area
					uni_area = States1[2]*States1[3]+States2[2]*States2[3]-int_area;

					cur_area = float(int_area)/float(uni_area);

					if (cur_area>ref_area)
					{
						ref_area = cur_area;
						mk = k;
					}
					k++;
				}
				else
				{
					BBox2.erase(BBox2.begin()+k);
				}
			}
			//Check if intersected area is greater than threshold
			if(ref_area>area_thr)
			{
				Tp++;
				BBox2.erase(BBox2.begin()+mk);
				k--;
			}
			else
			{
				Fn++;
			}
		}
	}
	Fp = BBox2.size();

	//Store in return variable
	C_values[0] += Tp;
	C_values[1] += Fp;
	C_values[2] += Fn;
}

//-------------------------------------------------------------------------------------
//Returns the intersected area of the rectangles specified by position vectors A and B.
//-------------------------------------------------------------------------------------
int Bbox_Measures_XML::Intersect_Area(vector<int> r1, vector<int> r2)
{
	int area = 0; //initial area of the inner rectangle

	//--stores the information of r1 as (min_x, min_y, max_x, max_y)
	r1[2] = r1[0]+r1[2];
	r1[3] = r1[1]+r1[3];

	//--stores the information of r2 as (min_x, min_y, max_x, max_y)
	r2[2] = r2[0]+r2[2];
	r2[3] = r2[1]+r2[3];

	//--stores coordinates of the inner rectangle as (min_x, min_y, max_x, max_y)
	vector<int> rect_int; 
	rect_int = r2;

	bool in = false; //checks for interception between r1 and r2

	for(int k=0; k<2; k++)
	{
		//--stores the coordinates of the vertex of r1
		vector<cv::Point> r1_pts;
		r1_pts.push_back(cv::Point(r1[0],r1[1]));			  
		r1_pts.push_back(cv::Point(r1[2],r1[1]));
		r1_pts.push_back(cv::Point(r1[0],r1[3]));
		r1_pts.push_back(cv::Point(r1[2],r1[3]));

		for(int i=0; i<4; i++)
		{
			if(Inner_point(r1_pts[i], r2))
			{
				in = true;
				if(i==0)
				{
					rect_int[0] = r1[0];
					rect_int[1] = r1[1];
				}
				if(i==1)
				{
					rect_int[2] = r1[2];
					rect_int[1] = r1[1];
				}
				if(i==2)
				{
					rect_int[0] = r1[0];
					rect_int[3] = r1[3];
				}
				if(i==3)
				{
					rect_int[2] = r1[2];
					rect_int[3] = r1[3];
				}
			}
		}
		if(in) 
		{
			area = (rect_int[2]-rect_int[0])*(rect_int[3]-rect_int[1]); //calculate area
			k=2;
		}
		else
		{
			r1.swap(r2);
			rect_int = r1;
		}
	}

	return area;
}

//-------------------------------------------------------------------------------------
//Check if a point is inside a rectangle (cx,cy,width,height)
//-------------------------------------------------------------------------------------
bool Bbox_Measures_XML::Inner_point(cv::Point v, vector<int> r)
{
	bool flag_in = false;

	if((v.x>=r[0])&(v.x<=r[2])&(v.y>=r[1])&(v.y<=r[3]))
		flag_in = true;

	return flag_in;
}

//-------------------------------------------------------------------------------------
//Show BBoxes
//-------------------------------------------------------------------------------------
void Bbox_Measures_XML::ShowDetections(vector<vector<int>> states, char *plot)
{
	cv::Point ul;
	cv::Point lr;

	cv::Mat Im1 = Im_RGB.clone();

	for(int k=0; k<states.size();k++)
	{
		ul = cv::Point(states[k][0],states[k][1]);
		lr = cv::Point(states[k][0]+states[k][2],states[k][1]+states[k][3]);

		rectangle(Im1,ul,lr,cv::Scalar(0,255,0),2);
	}
	cv::imshow(plot,Im1);
}