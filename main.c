#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
int MAX_BOARD_SIZE = 100;
int MAX_LEVEL = 10;
typedef struct{
	/* 0= Cell,1 = Knight,2 = Enemy,3 = Obstacle*/
	int type;
	char symbol;
}BoardElement;
/**	Struct Board represents the Game Board
 *  A GameBoard has an array of BoardElements
*/
typedef struct{
	BoardElement* board[100][100];
	int obstacles;
	int enemies;
	int x;
	int y;
}Board;
/**	Struct BoardElement represents a board element
 *  A BoardELement can be whatever we like, Bellow you can see available types
*/
typedef struct{
	int status;
	int id;//Level 1, 2, 3 etc.
	Board* current_board;
}Level;

/*Functions*/
int calculate_percentage(int x , int y , int percent);
void init_level(Level* level, int x, int y, char selected_level);
void end_level(Level* level);
Board* create_board(int x, int y, int obstacles, int enemies);
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
	/*Run for 10 levels*/
	for (int i = 0; i < MAX_LEVEL; i++){
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
	level->status = 1;
	int obstacle_count, enemy_count,k;
	char symbol;
	/*Based on the level the user has selected we calculate the percentage of obstacles and enemies*/
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
	/*Create the board and initialize with empty cells*/
	Board* new_board = create_board(x,y,obstacle_count,enemy_count);
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
	Board *new_board = (Board*)malloc(sizeof(Board));
	BoardElement *tmp_arr = (BoardElement *)malloc(x * y * sizeof(BoardElement)); 
	memcpy(new_board->board, tmp_arr, sizeof(tmp_arr));
	new_board->obstacles = obstacles;
	new_board->enemies = enemies;
	new_board->x = x;
	new_board->y = y;
	free(tmp_arr);
	return new_board;
}
BoardElement* create_element(int type){
	BoardElement *new_element = (BoardElement*)malloc(sizeof(BoardElement));
	if(type == 0){
		new_element->symbol = '*';
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
		BoardElement* new_enemy = create_element(2);
		int foundEmptySpace = 0;
		while(!foundEmptySpace){
			if (!prevX){
				int tmpX[2]={prevX-1,prevX+1};
			}
			else if(!prevY){
				int tmpY[2]={prevY-1,prevY+1};
			}	
			else{
	
				int randX = rand()%board->x;
				int randY = rand()%board->y;
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
}
void insert_obstacles(Board* board){
	for (int i = 0; i < board->obstacles; i++)
	{
		BoardElement* new_obstacle = create_element(3);
		int foundEmptySpace = 0;
		while(!foundEmptySpace){
			int randX = rand() % board->x;
			int randY = rand() % board->y;
			BoardElement* tmp_element = board->board[randX][randY];
			if(board->board[randX][randY]->type == 0){
				board->board[randX][randY] = new_obstacle;
				free(tmp_element);
				foundEmptySpace = 1;
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
void print(Board* board){
	for (int i = 0; i < board->x; i++) {
        for (int j = 0; j < board->y; j++) {	
			printf("%c ", board->board[i][j]->symbol);
        }
		printf("\n");
	}
}
void end_level(Level* level){
	level->status = -1;
	free(level->current_board);
}
int calculate_percentage(int x , int y , int percent){
	int total,res;
	total= x*y;
	res = total*percent/100;
	return res;
}
	
