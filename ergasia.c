#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
int MAX_BOARD_SIZE = 100;/* TO MEGETHOS*/
int MAX_LEVEL = 10;/* TO MEGISTO LEVEL */
typedef struct{
	
	int type; /* 0= Cell,1 = Knight,2 = Enemy,3 = Obstacle*/ 
	char symbol; /* .= KENO, $=IPOTHS ...*/
}BoardElement;

typedef struct{
	BoardElement* board[100][100];
	int obstacles;
	int enemies;
	int x;
	int y;
}Board; /* EDW THA BPOTHITHI O PINAKAS GIA NA FTIAXTEI */

typedef struct{
	int status;
	int id;//Level 1, 2, 3 etc.
	Board* current_board; /*SE AUTHN EDW THN METABLHTH EXOUN MPEI OI STHLES MAS OI GRAMMES MAS TA EMPODIA KAI OI EXTHROI APO THN SUNARTHSH 
					        create_board */
}Level; /* H STRUCT AUTH UPOLIGHZEI OLO TO LEVEL KAI OLO TON PINAKA */

/*Functions*/
int calculate_percentage(int x , int y , int percent); /* YPULOGIZI TA OBSTACLES KAI ENEMIES */
void init_level(Level* level, int x, int y, char selected_level); /* APO EDW PERNANE OLES OI SUNARTHSEIS */
void end_level(Level* level); /* EDW EGINE ENA LATHOS STO LEVEL */
Board* create_board(int x, int y, int obstacles, int enemies); /* EDW FTIAXNOUME TON PINAKA */
void insert_enemies(Board* board);
void insert_obstacles(Board* board);
void insert_knight(Board* board);
void print(Board* board);
BoardElement* create_element(int type);

int main (void)
{
	srand(time(NULL));
	int m,n;
	char selected_level;
	printf ("Choose the LINES\n"); /*DIALEGEI GRAMMH O XRHSTHS*/
	scanf ("%d", &m); /*DIALEGEI GRAMMH O XRHSTHS*/
	while (m > (MAX_BOARD_SIZE - MAX_LEVEL)){ /* AFERH TO ORIO TOU PINAKA (MEGISTO MEGETHOS - MEGISTO LEVEL) */
		printf ("Max lines are 90\n"); /* AN EIMAI MEGALUTERO APO TO ORIO TOU PINAKA EPANAEKSETAZEI THN GRAMMH */
		scanf ("%d", &m);
	}
	printf ("Choose the COLUMNS\n"); /*DIALEGEI STHLH O XRHSTHS*/
	scanf ("%d", &n); /*DIALEGEI STHLH O XRHSTHS*/
	while (n > (MAX_BOARD_SIZE - MAX_LEVEL)){ /* AFERH TO ORIO TOU PINAKA (MEGISTO MEGETHOS - MEGISTO LEVEL) */
		printf ("Max columns are 90\n"); /* AN EIMAI MEGALUTERO APO TO ORIO TOU PINAKA EPANAEKSETAZEI THN GRAMMH */
		scanf ("%d", &n);
	}
	printf ("Choose the difficulty\n"); /* DUSKOLIA */
	printf ("(e)EASY , (m)Medium , (h)HARD\n");
	scanf(" %c", &selected_level); /* DIALEGH O XRHSTH THN DUSKOLIA */
	printf("________________________________________________________________________________\n");
	while ((selected_level != 'e') && (selected_level != 'm') && (selected_level != 'h')){	/* 'e'= easy, 'm' = medium, 'h' = hard*/
		printf ("PLEASE TYPE: (e)EASY , (m)Medium , (h)HARD\n");
		scanf(" %c", &selected_level);	/*O XRHSTHS PATAEI PIA DUSKOLIA THELEI */	
	}
	/*Run for 10 levels*/
	for (int i = 0; i < MAX_LEVEL; i++){ /* THA TREKSH GIA 10 LEVELS (TOSO EINAI TO MEGISTO LEVEL */
		printf("new level %d\n",i); 
		Level *gameLevel;//Create new level
		int x = m + i;//Increase x by 1 every time in order to increase the board size
		int y = n + i;//Increase y by 1 every time in order to increase the board size
		init_level(gameLevel,x,y,selected_level);//Initiate level
		while(gameLevel->status != -1){//Run while the level status != 'finished'
			end_level(gameLevel);
		}
	}
	return 0;
}
/* Function init_level creates the board and inserts each element on a random position in order for the level to begin */
void init_level(Level* level, int x, int y, char selected_level){
	level->status = 1; /* ARXIKOPOIEI TO LEVEL.STATUS (BLEPE STRUCT LEVEL) ME 1 (DHLADH TREXEI SWSTA) */
	int obstacle_count, enemy_count,k;
	char symbol;
	/*Based on the level the user has selected we calculate the percentage of obstacles and enemies*/
	if(selected_level == 'e'){	
		obstacle_count = calculate_percentage(x,y,5); /* YPOLOGIZI TA OBSTACLES STO EASY */
		enemy_count = calculate_percentage(x,y,5); /* YPOLOGIZI TA ENEMIES STO EASY */
	}else if(selected_level == 'm'){
		obstacle_count = calculate_percentage(x,y,5); /* YPOLOGIZI TA OBSTACLES STO MEDIUM */
		enemy_count = calculate_percentage(x,y,10); /* YPOLOGIZI TA ENEMIES STO MEDIUM */
	}else{
		obstacle_count = calculate_percentage(x,y,10); /* YPOLOGIZI TA OBSTACLES STO HARD */
		enemy_count = calculate_percentage(x,y,10); /* YPOLOGIZI TA ENEMIES STO EASY */
	}
	/*Create the board and initialize with empty cells*/
	Board* new_board = create_board(x,y,obstacle_count,enemy_count); /* KALEI THN SHNARTHSH POU THA MAS FTIAKSEI TON PIANAKA */ 
	level->current_board = new_board; 
	for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {	
           	new_board->board[i][j] = create_element(0);
        }
	}
	/* edw edw edw edw edw */ 
	//Insert enemies in random positions of the board;
	insert_enemies(new_board);
	//Insert obstacles in random positions of the board;
	insert_obstacles(new_board);
	//Insert knight in a random position;
	insert_knight(new_board);
	/*print the board*/
	print(new_board);
}

Board* create_board(int x, int y, int obstacles, int enemies){
	Board *new_board = (Board*)malloc(sizeof(Board)); /*EDW THA ANAFERESAI ME NEW_BOARD KAI OXI ME CREATE_BOARD */
	BoardElement *tmp_arr = (BoardElement *)malloc(x * y * sizeof(BoardElement)); /* DHMIOURGEI ENAN KAINOURGIO PINAKA DUNAMIKA */
	memcpy(new_board->board, tmp_arr, sizeof(tmp_arr)); /* KOLLAEI THN tmp_arr ME THN new_board */
	new_board->obstacles = obstacles; /* EKXWREI TA EMPODIA  MESA STHN STRUCT LEVEL GIA NA ISXUEI GIA OLO TO PROGRAMMA MAS */
	new_board->enemies = enemies;  /* EKXWREI TOUS EXTHROUS MESA STHN STRUCT LEVEL GIA NA ISXUEI GIA OLO TO PROGRAMMA MAS */
	new_board->x = x; /* EKXWREI THN GRAMMH MESA STHN STRUCT LEVEL GIA NA ISXUEI GIA OLO TO PROGRAMMA MAS */
	new_board->y = y; /* EKXWREI THN  STHLH MESA STHN STRUCT LEVEL GIA NA ISXUEI GIA OLO TO PROGRAMMA MAS */
	free(tmp_arr);
	return new_board;
}
BoardElement* create_element(int type){
	BoardElement *new_element = (BoardElement*)malloc(sizeof(BoardElement));
	if(type == 0){
		new_element->symbol = '.';
		new_element->type = 0;
	}else if (type == 1){
		new_element->symbol = '$';
		new_element->type = 1;
	}else if (type == 2){
		new_element->symbol = (rand()%3)+1;
		new_element->symbol = new_element->symbol+'0';  
		new_element->type = 2;
	}else{
		new_element->symbol = '#';
		new_element->type = 3;
	}
	return new_element;
}
void insert_enemies(Board* board){
	int prevX,prevY;
	for (int i = 0; i < board->enemies; i++)
	{
		/*edw edw edw edw edw*/
		BoardElement* new_enemy = create_element(2); /* KALEI THN SUNARTHSH create_element ME TO 2 GIA ENEMYS */
		int foundEmptySpace = 0;
		while(!foundEmptySpace){
			if(prevX){
				int tmpX[2] = {prevX-1, prevX+1}; /* GIA NA MHN EINAI DIASPARTOI OI ENEMYS PREPEI NA TOUS PERIORISOUME SE ENA MEROS OLOUS*/
				int randX = tmpX[rand()%1];
				
			}
			else{
				int randX = rand()%board->x;
			
				
			}
			if(prevY){
				int tmpY[2] = {prevY-1, prevY+1};
				int randY = tmpY[rand()%1];
				
			}
			else{
				int randY = rand()%board->y;
			}
			BoardElement* tmp_element = board->board[randX][randY];
			if(board->board[randX][randY]){
				if(board->board[randX][randY]->type == 0){
					board->board[randX][randY] = new_enemy;
					free(tmp_element);
					foundEmptySpace = 1;

				}
			}
		
		}
	}
}
void insert_obstacles(Board* board){
	for (int i = 0; i < board->obstacles; i++) /* KANEI EPANALHPSH GIA OSA EMPODIA YPARXOUN */
	{
		BoardElement* new_obstacle = create_element(3); /* KALEI THN SUNARTHSH create_element ME type=3 
																GIA NA EKXWRHSH STON PINAKA TA EMPODIA ME symbol=# */

		int foundEmptySpace = 0; /*ARXIKA FTIAXNOUME MIA METABLHTH foundemtyspace GIA NA YPOLOGISOUME STHN SUNEXEIA 
									EAN AUTO TO SUGKEKRIMENO KELI EXEI DESMEUTEI APO ALLO EMPODIO,EXTHRO,IPPOTH */

		while(!foundEmptySpace){ /* EPANALHPSH POU ELEGXEI EAN TO TO foundepmtyspace EINAI =0 EAN EINAI =0
									TOTE THA MPEI MESA STHN EPANALHPSH.EAN EINAI =1 TOTE DEN THA MPEI STHN 
									EPANALHPSH(DHLADH EKEINO TO SYGKEKRIMENO KELI EXEI XRHSIMOPOIHTEI KAI DEN MPOROUME
									NA TOU BALOUME KATI */
											
			int randX = rand() % board->x; /*KANEI RAND TA X GIA NA BREI ENA MEROS GIA NA BALEI TO EMPODIO */
			int randY = rand() % board->y; /*KANEI RAND TA Y GIA NA BREI ENA MEROS GIA NA BALEI TO EMPODIO */
			BoardElement* tmp_element = board->board[randX][randY]; /*EKXWREI TON PINAKA board ME DIASTASEIS randX kai randY */
			if(board->board[randX][randY]->type == 0){ /* EAN TO type EINAI =0 DHLADH KENO BALTO TO EMPODIO MESA STON PINAKA */
				board->board[randX][randY] = new_obstacle;	/* BAZEI MESA STON PINAKA TO EMPODIO */
				free(tmp_element);	/* APODESMEUI TON PINAkaA tmp_element */
				foundEmptySpace = 1; /* EKXWREI STO foundemtyspace=1 GIA NA MHN MPORESOUME KAI BALOUME STO SUGKEKRIMENO KELI
										KAPION EXTHRO IPPOTH H KAI EMPODIO */
			}
		}
	}
}
void insert_knight(Board* board){
	BoardElement* knight = create_element(1);
	int foundEmptySpace = 0;
	while(!foundEmptySpace){
		int randX = rand() % board->x;
		int randY = rand() % board->y;
		BoardElement* tmp_element = board->board[randX][randY];
		if(board->board[randX][randY]->type == 0){
			board->board[randX][randY] = knight;
			free(tmp_element);
			foundEmptySpace = 1;
		}
	}
}
void print(Board* board){ /* EMFANIZEI TON PINAKA MAS MEXRI EDW */
	for (int i = 0; i < board->x; i++) {
        for (int j = 0; j < board->y; j++) {	
			printf("%c ", board->board[i][j]->symbol);
        }
		printf("\n");
	}
}
void end_level(Level* level){ /* EAN TO level->status= -1 TOTE STAMATEI OLO TO PROGRAMMA BLEPE SYNARTHSH init_level */
	level->status = -1;
	free(level->current_board); /* APODESMEUI OLO TON PINAKA POU EXOUME FTIAKSEI */
}
int calculate_percentage(int x , int y , int percent){
	int total,res;
	total= x*y;
	res = total*percent/100;
	return res;
}
