/* Klaudio Manofi
   AM: 2022201900119
   EMAIL: dit19119@uop.gr
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct{
	int number;
	char alphabet;
}final_destination;

final_destination* array(final_destination* board,int arr_col);


int main (void)
{
	int num;
	srand(time(NULL));
	final_destination* s_array; 	
	printf("Give number of rows/collumns for the matrix : ");
	scanf("%d", &num);
	final_destination* f_array = array(s_array,num);
	return 0;
}
	

final_destination* array(final_destination* board,int arr_col){
	int i,j;
	final_destination* new_board = (final_destination*)malloc(arr_col*sizeof(final_destination));
	printf("\n\n");
	printf("The matrix cells were filled in as follows:\n");	
	for ( i=0;i<arr_col;i++){
		for ( j=0;j<arr_col;j++){
			do{
			new_board->number = -26+rand()%56;
			}while(new_board->number < -26 || new_board->number > 26);
			if (new_board->number>0 ){
				new_board->alphabet = ('a'+ new_board->number)-1;
			}
			else if ( new_board->number<0){
				new_board->alphabet = ('A' - new_board->number)-1;
			}
			else{
				new_board->alphabet = '*';
			}
			printf(" %d%s%c ",new_board->number,"/",new_board->alphabet);	
		}
		printf(" \n ");
	}
	free(new_board);
	return new_board;
}


	







