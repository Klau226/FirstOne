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
	int prevX,prevY,i,foundEmptySpace;
	int tmpX[2]={prevX-1,prevX+1};
	int tmpY[2]={prevY-1,prevY+1};
	int randX,randY=0;
	for ( i = 0; i < board->enemies; i++)
	{
		
		BoardElement* new_enemy = create_element(2);
		foundEmptySpace = 0;
		while(!foundEmptySpace){
			
			if (prevX){
				int tmpX[2]={prevX-1,prevX+1};
				 randX=tmpX[rand()%+1];
				
			}
			else {
				 randX=rand()%board->x;
			}
			if (prevY){
				int tmpY[2]={prevY-1,prevY+1};
				randY=tmpY[rand()%+1];
			
			}
			else{
				 randY=rand()%board->y;
			}
				BoardElement* tmp_element = board->board[randX][randY];
				if(board->board[randX][randY]){
					if(board->board[randX][randY]->type == 0){
						board->board[randX][randY] = new_enemy;
						new_enemy->x = randX;
						new_enemy->y = randY;
						free(tmp_element);
						prevX = randX;
						prevY = randY;
						foundEmptySpace = 1;
					
					}
				}
		}
	}
}	
void insert_obstacles(Board* board){
	int i,foundEmptySpace;
	int randX,randY;
	for ( i = 0; i < board->obstacles; i++)
	{
		BoardElement* new_obstacle = create_element(3);
		foundEmptySpace = 0;
		while(!foundEmptySpace){
			randX = rand() % board->x;
			randY = rand() % board->y;
			BoardElement* tmp_element = board->board[randX][randY];
			if(board->board[randX][randY]->type == 0){
				board->board[randX][randY] = new_obstacle;
				new_obstacle->x = randX;
				new_obstacle->y = randY;
				free(tmp_element);
				foundEmptySpace = 1;
			}
		}
	}
}
void insert_knight(Board* board){
	int foundEmptySpace,randX,randY;
	BoardElement* knight = create_element(1);
	foundEmptySpace = 0;
	while(!foundEmptySpace){
		randX = rand() % board->x;
		randY = rand() % board->y;
		BoardElement* tmp_element = board->board[randX][randY];
		if(board->board[randX][randY]->type == 0){
			board->board[randX][randY] = knight;
			knight->x = randX;
			knight->y = randY;
			free(tmp_element);
			foundEmptySpace = 1;
		}
	}
}
void print(Board* board){
	int z,k,i,j;
	printf("     ");
	for ( z=0;z < board->y;z++){
		printf("%3d", z+1);
	}
	printf("\n");
	
	for ( k=0;k < board->y+board->y+3;k++){	
		printf(" _");
	}
	printf("\n");
		for ( i = 0; i < board->x; i++) {
			printf("%3d |",i+1);
			for ( j=0;j < board->y; j++){
				printf("%3c", board->board[i][j]->symbol);
			}
			printf("\n");
}
}
int movement(char move ,int step,Board* current_board){
	int oldX,oldY,newX,newXL,newYU,newYD;
	int swapX,swapY;
	BoardElement *knight = get_knight(current_board);
	oldX = knight->x;
	oldY = knight->y;
	printf("Knight x: %d \n",knight->x);
	printf("Knight y: %d \n",knight->y); 
	if (move =='r'){
			newX = knight->x + step;
			if (current_board->board[newX][oldY]->type == 0){
				BoardElement* tmp = current_board->board[newX][oldY];
				
				current_board->board[newX][oldY] = current_board->board[oldX][oldY];
				
				current_board->board[oldX][oldY] = tmp;
				
				current_board->board[newX][oldY]->x = newX;
				
				tmp->x = oldX;
				
				free(tmp);
			}
	}
	else if (move == 'l'){
		 newXL = knight->x - step;
		if (current_board->board[newXL][oldY]->type == 0){
			BoardElement* tmpl = current_board->board[newXL][oldY];
			current_board->board[newXL][oldY] = current_board->board[oldX][oldY];
			current_board->board[oldX][oldY] = tmpl;
			current_board->board[newXL][oldY]->x = newXL;
			tmpl->x = oldX;
			free(tmpl);
		}
	}
	else if (move == 'u'){
		newYU = knight->y - step;
		if (current_board->board[oldX][newYU]->type == 0){
			BoardElement* tmpu = current_board->board[oldX][newYU];
			current_board->board[oldX][newYU] = current_board->board[oldX][oldY];
			current_board->board[oldX][oldY] = tmpu;
			current_board->board[oldX][newYU]->y = newYU;
			tmpu->y = newYU;
			free(tmpu);
		}
	}
	else{
		newYD = knight->y + step;
		if (current_board->board[oldX][newYD]->type == 0){
		BoardElement* tmpd = current_board->board[oldX][newYD];
		current_board->board[oldX][newYD] = current_board->board[oldX][newYD];
		current_board->board[oldX][oldY] = tmpd;
		current_board->board[oldX][newYD]->y = newYD;
		tmpd->y = newYD;
		free(tmpd);
		}
	}
	

}
BoardElement* get_knight(Board* board){
	int i,j;
	for( i=0;i<board->x;i++){
		for ( j=0;j<board->y;j++){
			if(board->board[i][j]->type==1){
				return board->board[i][j]; 
			}
		}
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
