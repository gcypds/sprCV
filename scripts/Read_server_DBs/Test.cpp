#include "Datasets_Menu.h"

void main()
{
	Datasets_Menu M;		
	
	M.Select_Dataset();		//Display menu and select video

	Mat F;
	
	for(;;)
	{
		M.Get_Frame(F);		//Get new frame from selected video
		
		if(F.empty())
			break;
			
		imshow(F);
		
		waitKey(15);
	}
}
