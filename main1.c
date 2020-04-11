#include <stdio.h>
#include <stdlib.h>
int calculate_percentages(int line , int column , int percent);

typedef struct{ /* DEN KSERW AN EINAI SWSTO*/
	int m;
	int n;
} dimensions;

typedef struct { /* OUTE EDW */
	char lvl;
	dimensions array;
	int obstacle_count;
	int enemy_count;
} final_board;

int main (void)
{
	int m,n,obstacle_count,enemy_count;
	char lvl;
	int **array,i,j;
	printf("________________________________________________________________________________\n");
	printf ("Choose your preffered dimensions\n");
	printf ("Choose the LINE\n");
	scanf ("%d", &m);
	printf ("Choose the COLUMN\n");
	scanf ("%d", &n);
	printf("________________________________________________________________________________\n");
	array=(int**)malloc(m*sizeof(int*));   /* EKANA TON PINAKA DUNAMIKO */
	for(i=0;i<m;i++){
			array[i]=(int*)malloc(n*sizeof(int));
	}
	printf ("Choose the difficulty\n");
	printf ("(e)EASY , (m)Medium , (h)HARD\n");
	scanf("%c", &lvl);
	while (lvl != ('e') && lvl != ('m') && lvl != ('h')){	
			printf ("PLEASE TYPE: (e)EASY , (m)Medium , (h)HARD\n");
			scanf("%c", &lvl);	
	}
	if (lvl == 'e'){
		obstacle_count =calculate_percentages(m,n,5);
		enemy_count =calculate_percentages(m,n,5);
	}
	else if (lvl == 'm'){
		obstacle_count =calculate_percentages(m,n,5);
		enemy_count =calculate_percentages(m,n,10);
	}
	else{
		obstacle_count =calculate_percentages(m,n,10);
		enemy_count =calculate_percentages(m,n,10);
	}
	printf ("\nThe obstacles will be :");	
	printf("%d\n",obstacle_count);    	
	printf ("\nThe enemys will be :");	
	printf("%d\n",enemy_count);    				
	return 0;
}

int calculate_percentages(int line , int column , int percent)
{
	int x,s;
	x=line*column;	
	s=x*percent/100;						
	return s;
}
	

		







	
