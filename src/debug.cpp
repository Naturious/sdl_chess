#include "debug.h"

void show(char arr[8][8])
{
	for(int i = 0;i<8;i++)
	{
		for(int j = 0;j<8;j++)
		{
			std::cout << arr[i][j];
		}
		std::cout << std::endl;
	}
}