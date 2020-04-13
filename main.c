#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct{
	int status;
	int id;//Level 1, 2, 3 etc.
}Level;
typedef struct{
	int board[10][10];
	int obstacles;
	int enemies;
	int x;
	int y;
}Board;
typedef struct{
	int symbol;
}Enemy;
typedef struct{
	char symbol;
}Obstacle;
typedef struct{
	char symbol;
}Knight;

int calculate_percentage(int x , int y , int percent);
void init_level(Level* level, int x, int y, char selected_level);
void end_level(Level* level);
Board* create_board(int x, int y, int obstacles, int enemies);
Enemy* create_enemy( char symbol ,Board* new_board );
int insert_enemy(Board* board,Enemy* enemy);

int main (void)
{
	int m,n;
	char selected_level;
	printf ("Choose the LINES\n");
	scanf ("%d", &m);
	printf ("Choose the COLUMNS\n");
	scanf ("%d", &n);
	printf ("Choose the difficulty\n");
	printf ("(e)EASY , (m)Medium , (h)HARD\n");
	scanf(" %c", &selected_level);
	printf("________________________________________________________________________________\n");
	while ((selected_level != 'e') && (selected_level != 'm') && (selected_level != 'h')){	
		printf ("PLEASE TYPE: (e)EASY , (m)Medium , (h)HARD\n");
		scanf(" %c", &selected_level);		
	}
	for (int i = 0; i < 10; i++){
		Level *gameLevel;
		int x = m + i;
		int y = n + i;
		init_level(gameLevel,x,y,selected_level);
		while(gameLevel->status != -1){
			//end_level(gameLevel);
		}
	}
	return 0;
}
void init_level(Level* level, int x, int y, char selected_level){
	level->status = 1;
	int obstacle_count, enemy_count,k;
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
	srand(time(NULL));
	for (int i = 0; i < enemy_count; i++){
		Enemy* new_enemy = create_enemy(symbol,new_board);
		if(new_enemy){
			int inserted = insert_enemy(new_board,new_enemy);
			printf("%d",inserted);
		}
	}
	/*
	printf("The matrix elements are:\n");
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {	
           	printf("*");
        }
        printf("\n");
   }
   */
}
Board* create_board(int x, int y, int obstacles, int enemies){
	Board *new_board = (Board*)malloc(sizeof(Board));
	int *tmp_arr = (int *)malloc(x * y * sizeof(int)); 
	memcpy(new_board->board, tmp_arr, sizeof(new_board->board));
	new_board->obstacles = obstacles;
	new_board->enemies = enemies;
	new_board->x = x;
	new_board->y = y;
	return new_board;
}
Enemy* create_enemy( char symbol , Board* new_board){
	Enemy *new_enemy = (Enemy*)malloc(sizeof(Enemy));
	new_enemy->symbol = rand()%4;
	if(new_enemy->symbol == 0){
		new_enemy->symbol++;
	}
	return new_enemy;
}
int insert_enemy(Board* board,Enemy* enemy){
	int randX = rand()% board->x;
	int randY = rand()%board->y;
	board->board[randX][randY] = enemy->symbol;
	printf("here");
	printf("%d",board->board[randX][randY]);
	return 1;
}

void end_level(Level* level){
	level->status = -1;
}
int calculate_percentage(int x , int y , int percent){
	int total,res;
	total= x*y;
	res = total*percent/100;
	return res;
}