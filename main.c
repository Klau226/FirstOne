#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
	int status;
	int id;//Level 1, 2, 3 etc.
}Level;
typedef struct{
	int board[0][0];
	int obstacles;
	int enemies;	
}Board;
typedef struct{
	char symbol;
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
		printf ("%d", i);
		int x = m + i;
		int y = n + i;
		init_level(gameLevel,x,y,selected_level);
		while(gameLevel->status != -1){
			end_level(gameLevel);
		}
	}
	return 0;
}
void init_level(Level* level, int x, int y, char selected_level){
	level->status = 1;
	int obstacle_count, enemy_count;
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
	Board* s = create_board(x,y,obstacle_count,enemy_count);
	printf("%d",s->enemies);
}
Board* create_board(int x, int y, int obstacles, int enemies){
	Board *s;
	s->board[x][y];
	s->enemies = enemies;
	s->obstacles = obstacles;
	return s;
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