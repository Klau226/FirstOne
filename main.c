#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main (void)
{

	int i,j,m,n;
	int **array;
	char q;
	
	printf("Choose the size of your game (!!!WARNING!!! The size should be >... !!!WARNING!!!)");
	scanf("%d", &m);
	scanf("%d", &n);
	srand(time(NULL));
	array=(int**)malloc(m*sizeof(int*));
	for (i=0;i<m;i++)
	{
		array[i]=(int*)malloc(n*sizeof(int));
	}
	
	for (i=0;i<m;i++)
	{
		for (j=0;j<n;j++)
		{
		
			q=rand();
			array[i][j]=q;
		}
	}
	
	for (i=0;i<m;i++)
	{
		for (j=0;j<n;j++)
		{
			printf("%d",array[i][j]);
		}
		printf("\n");
	}
		





	return 0;
	
}
	
