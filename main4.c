#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
int MAX_BOARD_SIZE = 100;
int MAX_LEVEL = 10;
typedef struct{
	
	int type;
	char symbol;
	int x;
	int y;
}BoardElement;

typedef struct{
	BoardElement* board[100][100];
	int obstacles;
	int enemies;
	int x;
	int y;
}Board;

typedef struct{
	int status;
	int id;
	Board* current_board;
}Level;


int calculate_percentage(int x , int y , int percent);
void init_level(Level* level, int x, int y, char selected_level);
void end_level(Level* level);
Board* create_board(int x, int y, int obstacles, int enemies);
void insert_enemies(Board* board);
void insert_obstacles(Board* board);
void insert_knight(Board* board);
int movement(char move ,int step,Board* current_board);
BoardElement* get_knight(Board* board); 
void print(Board* board);
BoardElement* create_element(int type);

int main (void)
{
	srand(time(NULL));
	int m,n,step,movement_knight,i,x,y;
	char selected_level,direction;
	printf ("Choose the LINES\n");
	scanf ("%d", &m);
	while (m > (MAX_BOARD_SIZE - MAX_LEVEL)){
		printf ("Max lines are 90\n");
		scanf ("%d", &m);
	}
	printf ("Choose the COLUMNS\n");
	scanf ("%d", &n);
	while (n > (MAX_BOARD_SIZE - MAX_LEVEL)){
		printf ("Max columns are 90\n");
		scanf ("%d", &n);
	}
	printf ("Choose the difficulty\n");
	printf ("(e)EASY , (m)Medium , (h)HARD\n");
	scanf(" %c", &selected_level);
	printf("________________________________________________________________________________\n");
	while ((selected_level != 'e') && (selected_level != 'm') && (selected_level != 'h')){	
		printf ("PLEASE TYPE: (e)EASY , (m)Medium , (h)HARD\n");
		scanf(" %c", &selected_level);		
	}
	
	for ( i = 0; i < MAX_LEVEL; i++){
		printf("new level %d\n",i);
		Level *gameLevel;
		Board *knight_board;
		 x = m + i;
		 y = n + i;
		init_level(gameLevel,x,y,selected_level);//Initiate level
		printf("Enter direction: (l)left, (r)right, (u)up, (d)down and then the steps\n");
		scanf("%c%d", &direction, &step);
		while ((direction != 'l') && (direction != 'r') && (direction != 'u') && (direction != 'd') || (step<0))
		{
			printf("PLEASE TYPE: (l)left, (r)right, (u)up, (d)down\n");
			printf("PLEASY A STEP WHICH IS A NUMBER OR A NUMBER HIGHER THAN -1\n");
			scanf("%c", &direction);
			
			scanf("%d", &step);
		
		}
		movement_knight=movement(direction,step,gameLevel->current_board);
		while(gameLevel->status != -1){
			end_level(gameLevel);
		}
	}
	return 0;
}

void init_level(Level* level, int x, int y, char selected_level){
	level->status = 1;
	int obstacle_count, enemy_count,k,i,j;
	char symbol;
	
	if(selected_level == 'e'){	
		obstacle_count = calculate_percentage(x,y,5);
		enemy_count = calculate_percentage(x,y,5);
	}else if(selected_level == 'm'){
		obstacle_count = calculate_percentage(x,y,5);
		enemy_count = calculate_percentage(x,y,10);
	}else{
		obstacle_count = calculate_percentage(x,y,10);
		enemy_count = calculate_percentage(x,y,10);
	}
	
	Board* new_board = create_board(x,y,obstacle_count,enemy_count);
	level->current_board = new_board; 
	for ( i = 0; i < x; i++) {
        for ( j = 0; j < y; j++) {	
           	new_board->board[i][j] = create_element(0);
        }
	}
	 
	
	insert_enemies(new_board);
	
	insert_obstacles(new_board);
	
	insert_knight(new_board);
	
	print(new_board);
}
