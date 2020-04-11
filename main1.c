#include <stdio.h>
#include <stdlib.h>
int calculate_percentages(int line , int column , int percent);

typedef struct {
	int status;
} level;
typedef struct { /* OUTE EDW */
	int obstacle_count;
	int enemy_count;
} board;

int main (void)
{
	int m,n,obstacle_count,enemy_count,i,j;
	char lvl;
	level status=1;
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

	for(i=1;i<=10;i++){
		while (status==1){
			printf("The level is ok dog whats up man\n");
		};
			
			
	
		

  				
	return 0;
}

int calculate_percentages(int line , int column , int percent)
{
	int x,s;
	x=line*column;	
	s=x*percent/100;						
	return s;
}
	

		







	
