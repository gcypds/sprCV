#include <iostream>
#include <cstring>

//extern "C" unsigned char _binary_W_bin_start;
//extern "C" unsigned char _binary_W_bin_end;

extern "C" unsigned char Mat_W;
extern "C" unsigned char Mat_W_size;

float Wa[3780][165];

int main()
{
	//size_t total_elements = (&_binary_W_bin_end - &_binary_W_bin_start)/sizeof(float);
	size_t total_elements = (&Mat_W_size - &Mat_W)/sizeof(float);

	printf("Total elements: %d\n", total_elements);

	//unsigned char *pW = &_binary_W_bin_start;
	unsigned char *pW = &Mat_W;
	
	//memset(&W, 0.00, total_elements*sizeof(float));
	memcpy(&Wa, pW, total_elements*sizeof(float));

	printf("Element[3333][33]: %0.4f\n", Wa[3332][32]);
	printf("Element[1][1]: %0.4f\n", Wa[0][0]);

	printf("Element[1][2]: %0.4f\n", Wa[0][1]);
	printf("Element[1][3]: %0.4f\n", Wa[0][2]);
	printf("Element[1][4]: %0.4f\n", Wa[0][3]);
	
	printf("Element[2][1]: %0.4f\n", Wa[1][0]);
	printf("Element[2][2]: %0.4f\n", Wa[1][1]);
	printf("Element[2][3]: %0.4f\n", Wa[1][2]);

	printf("Element[3][1]: %0.4f\n", Wa[2][0]);
	printf("Element[3][2]: %0.4f\n", Wa[2][1]);
	printf("Element[3][3]: %0.4f\n", Wa[2][2]);

	printf("Element[4][1]: %0.4f\n", Wa[3][0]);
	printf("Element[4][2]: %0.4f\n", Wa[3][1]);
	printf("Element[4][3]: %0.4f\n", Wa[3][2]);

	/*unsigned char *pW = &_binary_W_bin_start; 
	printf("%f\n", pW - &_binary_W_bin_start, *pW);

	while(pW < &_binary_W_bin_end)
	{
		printf("%d: %02X\n", pW - &_binary_W_bin_start, *pW);
		pW++;
	}*/
	//printf("size: %d\n", &_binary_W_bin_size);
	getchar();

	return 0;
}