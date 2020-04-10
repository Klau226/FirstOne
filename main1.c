#include <stdio.h>
#include <stdlib.h>
int calculate_percetanges(int line , int column, char difficulty);
int main (void)
{
	int m,n,obstacle_count,enemy_count;
	char lvl;
	printf("________________________________________________________________________________\n");
	printf ("Choose your preffered dimensions\n");
	printf ("Choose the LINE\n");
	scanf ("%d", &m);
	printf ("Choose the COLUMN\n");
	scanf ("%d", &n);
	printf("________________________________________________________________________________\n");
	printf ("Choose the difficulty\n");
	printf ("(e)EASY , (m)Medium , (h)HARD\n");
	scanf("%c", &lvl);
	while (lvl != ('e') && lvl != ('m') && lvl != ('h'))
	{	
			printf ("PLEASE TYPE: (e)EASY , (m)Medium , (h)HARD\n");
			scanf("%c", &lvl);
			
	}
		

	obstacle_count= calculate_percetanges(m,n,lvl); /* TO KANO GIA NA UPOLOGISO TA OBSTACLES*/
  	printf ("\nThe obstacles will be :");	
	printf("%d\n",obstacle_count);    
	enemy_count= calculate_percetanges(m,n,lvl);
	printf ("\nThe enemys will be :");	
	printf("%d\n",enemy_count);    	
		
	
	return 0;
}

int calculate_percetanges(int line , int column, char difficulty)
{
	int x;
	x=line*column;
	if (difficulty=='e')
	{
		x=x*5/100;
	}
	else if (difficulty=='m')
	{
		x=x*10/100;
	}
	else if (difficulty=='h')
	{
		x=x*15/100;
	}
	
	return x;
}
	
   
		







	
