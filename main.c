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
	int x;//Own x position on array
	int y;//Own y position on array
	char direction;
}BoardElement;
/**	Struct Board represents the Game Board
 *  A GameBoard has an array of BoardElements
*/
typedef struct{
	BoardElement* board[100][100];
	int obstacles;
	int enemies;
	int coordinates;
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
int init_level(Level* level, int x, int y, char selected_level);
void end_level(Level* level);
Board* create_board(int x, int y, int obstacles, int enemies);
void insert_enemies(Board* board);
void insert_obstacles(Board* board);
void insert_knight(Board* board);
void print_board(Board* board);
BoardElement* create_element(int type);
int moveKnight(Board* current_board, char direction, int cell_count);
BoardElement* getKnight(Board* board);
int x_has_obstacles(Board* board, int current_position, int new_position, int current_y);
int y_has_obstacles(Board* board,int knight_x, int current_position, int new_position);

int main (void)
{
	int user_y,user_x;
	char selected_level;
	printf ("Choose the COLUMNS\n");
	scanf ("%d", &user_y);
	while (user_y > (MAX_BOARD_SIZE - MAX_LEVEL)){
		printf ("Max lines are 90\n");
		scanf ("%d", &user_y);
	}
	printf ("Choose the LINES\n");
	scanf ("%d", &user_x);
	while (user_x > (MAX_BOARD_SIZE - MAX_LEVEL)){
		printf ("Max columns are 90\n");
		scanf ("%d", &user_x);
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
		printf("new level %d\n",i+1);
		Level *gameLevel;//Create new level
		int x = user_x + i;//Increase x by 1 every time in order to increase the board size
		int y = user_y + i;//Increase y by 1 every time in order to increase the board size
		int initiated = init_level(gameLevel,x,y,selected_level);//Initiate level
		if(initiated){
			while(gameLevel->status != -1){//Run while the level status != 'finished'
				char nextDirection = '-';
				int cell_count = -1;
				while ( (nextDirection == '-') && (cell_count == -1) ){
					printf ("Enter direction: (l)left, r(right), (u)up, (d)down\n");
					scanf(" %c", &nextDirection);
					printf ("Number of cells to move:\n");
					scanf(" %d", &cell_count);
					int moved = moveKnight(gameLevel->current_board,nextDirection,cell_count);
					if(!moved){
						printf("Invalid position, please choose a new one\n");
					}
				}
				//end_level(gameLevel);
			}
		}
	}
	return 0;
}
/* Function init_level creates the board and inserts each element on a random position in order for the level to begin */
int init_level(Level* level, int x, int y, char selected_level){
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
	srand(time(0)); 
	//Insert enemies in random positions of the board;
	insert_enemies(new_board);
	//Insert obstacles in random positions of the board;
	insert_obstacles(new_board);
	//Insert knight in a random position;
	insert_knight(new_board);
	/*print the board*/
	print_board(new_board);

	return 1;
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
	int prevX,prevY;
	int randX,randY=0;
	for (int i = 0; i < board->enemies; i++)
	{	
		BoardElement* new_enemy = create_element(2);
		int foundEmptySpace = 0;
		while(!foundEmptySpace){
			srand(time(0));
			if(prevX){
				int tmpX[2] = {prevX-1, prevX+1};
				randX = tmpX[rand()%1];
			}else{
				randX = rand()%board->x;
			}
			if(prevY){
				int tmpY[2] = {prevY-1, prevY+1};
				randY = tmpY[rand()%1];
			}else{
				randY = rand()%board->y;
			}
			BoardElement* tmp_element = board->board[randX][randY];
			if(board->board[randX][randY]){
				if(board->board[randX][randY]->type == 0){
					board->board[randX][randY] = new_enemy;
					free(tmp_element);
					prevX = randX;
					prevY = randY;
					new_enemy->x = randX;
					new_enemy->y = randY;
					foundEmptySpace = 1;
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
				new_obstacle->x = randX;
				new_obstacle->y = randY;
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
			knight->x = randX;
			knight->y = randY;
			free(tmp_element);
			foundEmptySpace = 1;
		}
	}
}
void print_board(Board* board){
	printf("    ");
	for(int z = 0; z < board->y; z++){
		if( (z+1) > 9){
			printf("%d",z+1);
		}else{
			printf("%d ",z+1);
		}
	}
	printf("\n");
	for(int z = 0; z < board->y+2; z++){
		printf("_ ");
	}
	printf("\n");
	for (int i = 0; i < board->x; i++) {
		int row  = i+1;
		if(row > 9){
			printf("%d| ",i+1);
		}else{
			printf("%d | ",i+1);
		}
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
BoardElement* getKnight(Board* board){
	BoardElement* element;
	for (int i = 0; i < board->x; i++) {
        for (int j = 0; j < board->y; j++) {
			if(board->board[i][j]->type == 1){
				return board->board[i][j];
			}
        }
	}
	return element;
}
int moveKnight(Board* current_board, char direction, int cell_count){
	BoardElement *knight = getKnight(current_board);
	int currentX = knight->x;
	int currentY = knight->y;
	int moved;
	if(direction == 'l'){
		int newY = knight->y - cell_count;
		if(!y_has_obstacles(current_board,knight->x,knight->y,newY)){
			BoardElement *tmp_element = current_board->board[currentX][newY];
			int canMove = 0;
			if(tmp_element->type == 0){
				current_board->board[currentX][newY] = knight;
				current_board->board[currentX][newY]->y = newY;
				current_board->board[currentX][currentY] = tmp_element;
				current_board->board[currentX][currentY]->x = currentX;
				current_board->board[currentX][currentY]->y = currentY;
				moved = 1;
			}else{
				moved = 0;
			}
		}else{
			moved = 0;
		}
	}else if(direction == 'r'){
		int newY = knight->y + cell_count;
		if(!y_has_obstacles(current_board,knight->x,knight->y,newY)){
			BoardElement *tmp_element = current_board->board[currentX][newY];
			int canMove = 0;
			if(tmp_element->type == 0){
				current_board->board[currentX][newY] = knight;
				current_board->board[currentX][newY]->y = newY;
				current_board->board[currentX][currentY] = tmp_element;
				current_board->board[currentX][currentY]->x = currentX;
				current_board->board[currentX][currentY]->y = currentY;
				moved = 1;
			}else{
				moved = 0;
			}
		}else{
			moved = 0;
			printf("Path has obstacles!\n");
		}
	}else if(direction == 'u'){
		int newX = knight->x - cell_count;
		if(!x_has_obstacles(current_board,knight->x,newX,knight->y)){
			BoardElement *tmp_element = current_board->board[newX][currentY];
			int canMove = 0;
			if(tmp_element->type == 0){
				current_board->board[newX][currentY] = knight;
				current_board->board[newX][currentY]->x = newX;
				current_board->board[currentX][currentY] = tmp_element;
				current_board->board[currentX][currentY]->x = currentX;
				current_board->board[currentX][currentY]->y = currentY;
				moved = 1;
			}else{
				moved = 0;
			}
		}else{
			moved = 0;
			printf("Path has obstacles!\n");
		}
	}else if(direction == 'd'){
		int newX = knight->x + cell_count;
		if(!x_has_obstacles(current_board,knight->x,newX,knight->y)){
			BoardElement *tmp_element = current_board->board[newX][currentY];
			if(tmp_element->type == 0){
				current_board->board[newX][currentY] = knight;
				current_board->board[newX][currentY]->x = newX;
				current_board->board[currentX][currentY] = tmp_element;
				current_board->board[currentX][currentY]->x = currentX;
				current_board->board[currentX][currentY]->y = currentY;
				moved = 1;
			}else{
				moved = 0;
			}
		}else{
			moved = 0;
			printf("Path has obstacles!\n");
		}
	}
	print_board(current_board);
}
int x_has_obstacles(Board* board, int current_position, int new_position, int current_y){
	int response = 0;
	if(current_position > new_position ){
		for (int i = new_position; i < current_position; i++) {
			if(board->board[i][current_y]->type != 0){
				response = 1;
			}
		}
	}else{
		for (int i = current_position+1; i < new_position+1; i++) {
			if(board->board[i][current_y]->type != 0){
				response = 1;
			}
		}
	}
	return response;
}
int y_has_obstacles(Board* board,int knight_x, int current_position, int new_position){
	int response = 0;
	if(current_position > new_position ){
		for (int i = 0; i < board->x; i++) {
			for (int j = current_position+1; j > new_position; j--) {	
				if(knight_x == i){
					printf("%c ", board->board[i][j]->symbol);
				}
			}
			printf("\n");
		}
		printf("\n");
		//H praksh prepei na einai -
	}else{
		for (int i = 0; i < board->x; i++) {
			for (int j = new_position; j < current_position; j++) {	
				if(knight_x == i){
					printf("%c ", board->board[i][j]->symbol);
				}
			}
			printf("\n");
		}
		printf("\n");
		//H praksh prepei na einai +
	}
}
int calculate_percentage(int x , int y , int percent){
	int total,res;
	total= x*y;
	res = total*percent/100;
	return res;
}
