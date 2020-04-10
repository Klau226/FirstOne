#include <stdio.h>
#include <stdlib.h>
int calculate_percetanges(int line , int column, char diffic);
int main (void)
{
	int m,n,upol;
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
		

	upol= calculate_percetanges(m,n,lvl);   /* TO KANO GIA NA UPOLOGISO TA OBSTACLES*/
	printf("%d",upol);    /* ALLA APO TI FENETAI  THA EXEI KANENA PROVLHMA ME TO 'e' 'm' 'h' */
		
		
	
	return 0;
}

int calculate_percetanges(int line , int column, char diffic)
{
	int x;
	x=line*column;
	if (diffic=='e')
	{
		x=x*(5/100);
	}
	else if (diffic=='m')
	{
		x=x*(10/100);
	}
	else if (diffic=='h')
	{
		x=x*(15/100);
	}
	
	return x;
}
	
   
		







	
