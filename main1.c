#include <stdio.h>
#include <stdlib.h>
int OBS(int line , int column, char DIF);
int main (void)
{
	int m,n,upol;
	char LVL;
	printf("________________________________________________________________________________\n");
	printf ("Choose your preffered dimensions\n");
	printf ("Choose the LINE\n");
	scanf ("%d", &m);
	printf ("Choose the COLUMN\n");
	scanf ("%d", &n);
	printf("________________________________________________________________________________\n");
	printf ("Choose the difficulty\n");
	printf ("(e)EASY , (m)Medium , (h)HARD\n");
	scanf("%c", &LVL);
	while (LVL != ('e') && LVL != ('m') && LVL != ('h'))
	{	
			printf ("PLEASE TYPE: (e)EASY , (m)Medium , (h)HARD\n");
			scanf("%c", &LVL);
			
	}
	upol= OBS(m,n,LVL);   /* TO KANO GIA NA UPOLOGISO TA OBSTACLES*/
	printf("%d",upol);    /* ALLA APO TI FENETAI  THA EXEI KANENA PROVLHMA ME TO 'e' 'm' 'h' */
		
		
	
	return 0;
}

int OBS(int line , int column, char DIF)
{
	int FINALOBS,X;
	X=line*column;
	if (DIF='e')
	{
		X=X*(5/100);
	}
	else if (DIF='m')
	{
		X=X*(10/100);
	}
	else if (DIF='h')
	{
		X=X*(15/100);
	}
	
	return X;
}
	
   
		







	
