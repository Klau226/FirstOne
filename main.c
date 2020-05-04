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
	int money_spent;
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
void print_money(Level* level);
BoardElement* create_element(int type);
int moveKnight(Board* current_board, char direction, int cell_count);
BoardElement* getKnight(Board* board);
int x_has_obstacles(Board* board, int current_position, int new_position, int current_y);
int y_has_obstacles(Board* board,int current_x, int current_position, int new_position);
int punch(Level* level);
int slash(Level* level);
int axe(Level* level);
int arrow(Level* level);
int decrease_enemy(BoardElement *enemy);
int move_enemy(BoardElement* board[100][100], BoardElement* enemy, char direction);


int total_money_spent = 0;
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
				char nextCommand = '-';
				while ( nextCommand == '-' ){
					int cell_count = -1;
					int command_complete = 0;
					printf ("Make your move(s):\n(l)left, r(right), (u)up, (d)down, f(Punch), a(Axe), s(Slash), b(Arrow)\n");
					scanf(" %c", &nextCommand);
					switch (nextCommand){
						case 'l':
							printf ("Number of cells to move:\n");
							scanf(" %d", &cell_count);
							command_complete = moveKnight(gameLevel->current_board,nextCommand,cell_count);
							if(!command_complete){
								printf("Invalid position, please choose a new one\n");
							}
							break;
						case 'r':
							printf ("Number of cells to move:\n");
							scanf(" %d", &cell_count);
							command_complete = moveKnight(gameLevel->current_board,nextCommand,cell_count);
							if(!command_complete){
								printf("Invalid position, please choose a new one\n");
							}
							break;
						case 'u':
							printf ("Number of cells to move:\n");
							scanf(" %d", &cell_count);
							command_complete = moveKnight(gameLevel->current_board,nextCommand,cell_count);
							if(!command_complete){
								printf("Invalid position, please choose a new one\n");
							}
							break;
						case 'd':
							printf ("Number of cells to move:\n");
							scanf(" %d", &cell_count);
							command_complete = moveKnight(gameLevel->current_board,nextCommand,cell_count);
							if(!command_complete){
								printf("Invalid position, please choose a new one\n");
							}
							break;
						case 'f':
							command_complete = punch(gameLevel);
							if(command_complete){
								print_board(gameLevel->current_board);
								print_money(gameLevel);
							}else{
								printf("Could not attack\n");
							}
							break;
						case 'a':
							command_complete = axe(gameLevel);
							if(command_complete){
								print_board(gameLevel->current_board);
								print_money(gameLevel);
							}else{
								printf("Could not attack\n");
							}
							break;
						case 's':
							command_complete = slash(gameLevel);
							if(command_complete){
								print_board(gameLevel->current_board);
								print_money(gameLevel);
							}else{
								printf("Could not attack\n");
							}
							break;
						case 'b':
							command_complete = arrow(gameLevel);
							if(command_complete){
								print_board(gameLevel->current_board);
								print_money(gameLevel);
							}else{
								printf("Could not attack\n");
							}
							break;
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
	level->money_spent = 0;
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
	for (int i = 0; i < board->enemies; i++){	
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
			knight->direction = 'u';
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
void print_money(Level* level){
	printf("\n");
	printf("Level money spent: %d \n", level->money_spent);
	printf("Game money spent: %d \n", total_money_spent);
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
			printf("-------------------\n");
			printf("Path has obstacles!\n");
			printf("-------------------\n");
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
			printf("-------------------\n");
			printf("Path has obstacles!\n");
			printf("-------------------\n");
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
			printf("-------------------\n");
			printf("Path has obstacles!\n");
			printf("-------------------\n");		}
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
			printf("-------------------\n");
			printf("Path has obstacles!\n");
			printf("-------------------\n");		}
	}
	if(moved){
		knight->direction = direction;
		printf("%c",knight->direction);
		print_board(current_board);
	}
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
int y_has_obstacles(Board* board,int current_x, int current_position, int new_position){
	int response = 0;
	if(current_position > new_position ){
		for (int i = new_position; i < current_position; i++) {
			if(board->board[current_x][i]->type != 0){
				response = 1;
			}
		}
	}else{
		for (int i = current_position+1; i < new_position+1; i++) {
			if(board->board[current_x][i]->type != 0){
				response = 1;
			}
		}
	}
	return response;
}
int punch(Level* level){
	BoardElement *knight = getKnight(level->current_board);
	int currentX = knight->x;
	int currentY = knight->y;
	int success = 0;
	if(knight->direction == 'l'){
		if(level->current_board->board[knight->x][knight->y-1]){
			if(level->current_board->board[knight->x][knight->y-1]->type == 2){
				int is_alive = decrease_enemy(level->current_board->board[knight->x][knight->y-1]);
				if(is_alive){
					success = move_enemy(level->current_board->board,level->current_board->board[knight->x][knight->y-1],knight->direction);
				}else{
					success = 1;
				}
			}
		}
	}else if(knight->direction == 'r'){
		if(level->current_board->board[knight->x][knight->y+1]){
			if(level->current_board->board[knight->x][knight->y+1]->type == 2){
				int is_alive = decrease_enemy(level->current_board->board[knight->x][knight->y+1]);
				if(is_alive){
					success = move_enemy(level->current_board->board,level->current_board->board[knight->x][knight->y+1],knight->direction);
				}else{
					success = 1;
				}
			}
		}
	}if(knight->direction == 'u'){
		if(level->current_board->board[knight->x-1][knight->y]){
			if(level->current_board->board[knight->x-1][knight->y]->type == 2){
				int is_alive = decrease_enemy(level->current_board->board[knight->x-1][knight->y]);
				if(is_alive){
					success = move_enemy(level->current_board->board,level->current_board->board[knight->x-1][knight->y],knight->direction);
				}else{
					success = 1;
				}
			}
		}
	}else{
		if(level->current_board->board[knight->x+1][knight->y]){
			if(level->current_board->board[knight->x+1][knight->y]->type == 2){
				int is_alive = decrease_enemy(level->current_board->board[knight->x+1][knight->y]);
				if(is_alive){
					success = move_enemy(level->current_board->board,level->current_board->board[knight->x+1][knight->y],knight->direction);
				}else{
					success = 1;
				}
			}
		}
	}
	if(success){
		level->money_spent += 50;
		total_money_spent += 50;
	}
	return success;
}
int axe(Level* level){
	BoardElement *knight = getKnight(level->current_board);
	int currentX = knight->x;
	int currentY = knight->y;
	int success = 0;
	if(knight->direction == 'l'){
		if(level->current_board->board[knight->x][knight->y-2]){
			if(level->current_board->board[knight->x][knight->y-2]->type == 2){
				int is_alive = decrease_enemy(level->current_board->board[knight->x][knight->y-2]);
				if(is_alive){
					success = move_enemy(level->current_board->board,level->current_board->board[knight->x][knight->y-2],knight->direction);
				}else{
					success = 1;
				}
			}
		}
		if(level->current_board->board[knight->x][knight->y-1]){
			if(level->current_board->board[knight->x][knight->y-1]->type == 2){
				int is_alive = decrease_enemy(level->current_board->board[knight->x][knight->y-1]);
				if(is_alive){
					success = move_enemy(level->current_board->board,level->current_board->board[knight->x][knight->y-1],knight->direction);
				}else{
					success = 1;
				}
			}
		}
	}else if(knight->direction == 'r'){
		if(level->current_board->board[knight->x][knight->y+2]){
			if(level->current_board->board[knight->x][knight->y+2]->type == 2){
				int is_alive = decrease_enemy(level->current_board->board[knight->x][knight->y+2]);
				if(is_alive){
					success = move_enemy(level->current_board->board,level->current_board->board[knight->x][knight->y+2],knight->direction);
				}else{
					success = 1;
				}
			}
		}
		if(level->current_board->board[knight->x][knight->y+1]){
			if(level->current_board->board[knight->x][knight->y+1]->type == 2){
				int is_alive = decrease_enemy(level->current_board->board[knight->x][knight->y+1]);
				if(is_alive){
					success = move_enemy(level->current_board->board,level->current_board->board[knight->x][knight->y+1],knight->direction);
				}else{
					success = 1;
				}
			}
		}
		
	}if(knight->direction == 'u'){
		if(level->current_board->board[knight->x-2][knight->y]){
			if(level->current_board->board[knight->x-2][knight->y]->type == 2){
				int is_alive = decrease_enemy(level->current_board->board[knight->x-2][knight->y]);
				if(is_alive){
					success = move_enemy(level->current_board->board,level->current_board->board[knight->x-2][knight->y],knight->direction);
				}else{
					success = 1;
				}
			}
		}
		if(level->current_board->board[knight->x-1][knight->y]){
			if(level->current_board->board[knight->x-1][knight->y]->type == 2){
				int is_alive = decrease_enemy(level->current_board->board[knight->x-1][knight->y]);
				if(is_alive){
					success = move_enemy(level->current_board->board,level->current_board->board[knight->x-1][knight->y],knight->direction);
				}else{
					success = 1;
				}
			}
		}
	}else{
		if(level->current_board->board[knight->x+2][knight->y]->type == 2){
			int is_alive = decrease_enemy(level->current_board->board[knight->x+2][knight->y]);
			if(is_alive){
				success = move_enemy(level->current_board->board,level->current_board->board[knight->x+2][knight->y],knight->direction);
			}else{
				success = 1;
			}
		}
		if(level->current_board->board[knight->x+1][knight->y]->type == 2){
			int is_alive = decrease_enemy(level->current_board->board[knight->x+1][knight->y]);
			if(is_alive){
				success = move_enemy(level->current_board->board,level->current_board->board[knight->x+1][knight->y],knight->direction);
			}else{
				success = 1;
			}
		}
	}
	if(success){
		level->money_spent += 70;
		total_money_spent += 70;
	}
	return success;
}
int slash(Level* level){
	BoardElement *knight = getKnight(level->current_board);
	int currentX = knight->x;
	int currentY = knight->y;
	int success = 0;
	if(knight->direction == 'l'){
		if(level->current_board->board[knight->x][knight->y-3]){
			if(level->current_board->board[knight->x][knight->y-3]->type == 2){
				int is_alive = decrease_enemy(level->current_board->board[knight->x][knight->y-3]);
				if(is_alive){
					success = move_enemy(level->current_board->board,level->current_board->board[knight->x][knight->y-3],knight->direction);
				}else{
					success = 1;
				}
			}
		}
		if(level->current_board->board[knight->x][knight->y-2]){
			if(level->current_board->board[knight->x][knight->y-2]->type == 2){
				int is_alive = decrease_enemy(level->current_board->board[knight->x][knight->y-2]);
				if(is_alive){
					success = move_enemy(level->current_board->board,level->current_board->board[knight->x][knight->y-2],knight->direction);
				}else{
					success = 1;
				}
			}
		}
		if(level->current_board->board[knight->x][knight->y-1]){
			if(level->current_board->board[knight->x][knight->y-1]->type == 2){
				int is_alive = decrease_enemy(level->current_board->board[knight->x][knight->y-1]);
				if(is_alive){
					success = move_enemy(level->current_board->board,level->current_board->board[knight->x][knight->y-1],knight->direction);
				}else{
					success = 1;
				}
			}
		}
	}else if(knight->direction == 'r'){
		if(level->current_board->board[knight->x][knight->y+3]){
			if(level->current_board->board[knight->x][knight->y+3]->type == 2){
				int is_alive = decrease_enemy(level->current_board->board[knight->x][knight->y+3]);
				if(is_alive){
					success = move_enemy(level->current_board->board,level->current_board->board[knight->x][knight->y+3],knight->direction);
				}else{
					success = 1;
				}
			}
		}
		if(level->current_board->board[knight->x][knight->y+2]){
			if(level->current_board->board[knight->x][knight->y+2]->type == 2){
				int is_alive = decrease_enemy(level->current_board->board[knight->x][knight->y+2]);
				if(is_alive){
					success = move_enemy(level->current_board->board,level->current_board->board[knight->x][knight->y+2],knight->direction);
				}else{
					success = 1;
				}
			}
		}
		if(level->current_board->board[knight->x][knight->y+1]){
			if(level->current_board->board[knight->x][knight->y+1]->type == 2){
				int is_alive = decrease_enemy(level->current_board->board[knight->x][knight->y+1]);
				if(is_alive){
					success = move_enemy(level->current_board->board,level->current_board->board[knight->x][knight->y+1],knight->direction);
				}else{
					success = 1;
				}
			}
		}
		
	}if(knight->direction == 'u'){
		if(level->current_board->board[knight->x-3][knight->y]){
			if(level->current_board->board[knight->x-3][knight->y]->type == 2){
				int is_alive = decrease_enemy(level->current_board->board[knight->x-3][knight->y]);
				if(is_alive){
					success = move_enemy(level->current_board->board,level->current_board->board[knight->x-3][knight->y],knight->direction);
				}else{
					success = 1;
				}
			}
		}
		if(level->current_board->board[knight->x-2][knight->y]){
			if(level->current_board->board[knight->x-2][knight->y]->type == 2){
				int is_alive = decrease_enemy(level->current_board->board[knight->x-2][knight->y]);
				if(is_alive){
					success = move_enemy(level->current_board->board,level->current_board->board[knight->x-2][knight->y],knight->direction);
				}else{
					success = 1;
				}
			}
		}
		if(level->current_board->board[knight->x-1][knight->y]){
			if(level->current_board->board[knight->x-1][knight->y]->type == 2){
				int is_alive = decrease_enemy(level->current_board->board[knight->x-1][knight->y]);
				if(is_alive){
					success = move_enemy(level->current_board->board,level->current_board->board[knight->x-1][knight->y],knight->direction);
				}else{
					success = 1;
				}
			}
		}
	}else{
		if(level->current_board->board[knight->x+3][knight->y]->type == 3){
			int is_alive = decrease_enemy(level->current_board->board[knight->x+3][knight->y]);
			if(is_alive){
				success = move_enemy(level->current_board->board,level->current_board->board[knight->x+3][knight->y],knight->direction);
			}else{
				success = 1;
			}
		}
		if(level->current_board->board[knight->x+2][knight->y]->type == 2){
			int is_alive = decrease_enemy(level->current_board->board[knight->x+2][knight->y]);
			if(is_alive){
				success = move_enemy(level->current_board->board,level->current_board->board[knight->x+2][knight->y],knight->direction);
			}else{
				success = 1;
			}
		}
		if(level->current_board->board[knight->x+1][knight->y]->type == 2){
			int is_alive = decrease_enemy(level->current_board->board[knight->x+1][knight->y]);
			if(is_alive){
				success = move_enemy(level->current_board->board,level->current_board->board[knight->x+1][knight->y],knight->direction);
			}else{
				success = 1;
			}
		}
	}
	if(success){
		level->money_spent += 80;
		total_money_spent += 80;
	}
	return success;
}
int arrow(Level* level){
	BoardElement *knight = getKnight(level->current_board);
	int currentX = knight->x;
	int currentY = knight->y;
	int success = 0;
	if(knight->direction == 'l'){
		if(level->current_board->board[knight->x][knight->y-2]){
			if(level->current_board->board[knight->x][knight->y-2]->type == 2){
				int is_alive = decrease_enemy(level->current_board->board[knight->x][knight->y-2]);
				if(is_alive){
					success = move_enemy(level->current_board->board,level->current_board->board[knight->x][knight->y-2],knight->direction);
				}else{
					success = 1;
				}
			}
		}
	}else if(knight->direction == 'r'){
		if(level->current_board->board[knight->x][knight->y+2]){
			if(level->current_board->board[knight->x][knight->y+2]->type == 2){
				int is_alive = decrease_enemy(level->current_board->board[knight->x][knight->y+2]);
				if(is_alive){
					success = move_enemy(level->current_board->board,level->current_board->board[knight->x][knight->y+2],knight->direction);
				}else{
					success = 1;
				}
			}
		}
	}if(knight->direction == 'u'){
		if(level->current_board->board[knight->x-2][knight->y]){
			if(level->current_board->board[knight->x-2][knight->y]->type == 2){
				int is_alive = decrease_enemy(level->current_board->board[knight->x-2][knight->y]);
				if(is_alive){
					success = move_enemy(level->current_board->board,level->current_board->board[knight->x-2][knight->y],knight->direction);
				}else{
					success = 1;
				}
			}
		}
	}else{
		if(level->current_board->board[knight->x+2][knight->y]){
			if(level->current_board->board[knight->x+2][knight->y]->type == 2){
				int is_alive = decrease_enemy(level->current_board->board[knight->x+2][knight->y]);
				if(is_alive){
					success = move_enemy(level->current_board->board,level->current_board->board[knight->x+2][knight->y],knight->direction);
				}else{
					success = 1;
				}
			}
		}
	}
	if(success){
		level->money_spent += 60;
		total_money_spent += 60;
	}
	return success;
}
int decrease_enemy(BoardElement *enemy){
	int response = 0;
	if(enemy->symbol == '3'){
		enemy->symbol = '2';
		response = 2;
	}else if( enemy->symbol == '2'){
		enemy->symbol = '1';
		response = 1;
	}else{
		enemy->type = 0;
		enemy->symbol = '.';
		response = 0;
	}
	return response;
}
int move_enemy(BoardElement *board[100][100], BoardElement *enemy,char direction){
	int response = 0;
	if(direction == 'l'){
		//Ean to keli sta aristera tou enemy yparxei h einai sthn akrh tou pinaka
		if(board[enemy->x][enemy->y-1]){
			//Ean to keli sta aristera tou enemy einai adeio
			if(board[enemy->x][enemy->y-1]->type == 0){
				//Vale to keli aristera tou enemy mesa se mia proswrinh metavlhth
				BoardElement *tmp_element = board[enemy->x][enemy->y-1];
				//Sthn thesh pou htan to adeio keli metakinhse ton enemy
				board[enemy->x][enemy->y-1] = enemy;
				//Sthn thesh pou htan o enemy phgaine to adeio keli pou to kratame se mia proswrinh metavlhth
				board[enemy->x][enemy->y] = tmp_element;
				//Kane update ton enemy kai pes tou to kainourio sou y einai to y-1 giati ton phgame kata ena keli aristera
				enemy->y = enemy->y-1;
				//Kane update to adeio keli kai pes tou to kainourio sou y einai y+1 giati ton phgame kata ena keli deksia
				tmp_element->y = tmp_element->y+1; 
				response = 1;
			}else{
				response = 1;
			}
		}else{
			response = 1;
		}
	}else if(direction == 'r'){
		if(board[enemy->x][enemy->y+1]){
			if(board[enemy->x][enemy->y+1]->type == 0){
				BoardElement *tmp_element = board[enemy->x][enemy->y+1];
				board[enemy->x][enemy->y+1] = enemy;
				board[enemy->x][enemy->y] = tmp_element;
				enemy->y = enemy->y+1;
				tmp_element->y = tmp_element->y-1; 
				response = 1;
			}else{
				response = 1;
			}
		}else{
			response = 1;
		}
	}else if(direction == 'u'){
		if(board[enemy->x-1][enemy->y]){
			if(board[enemy->x-1][enemy->y]->type == 0){
				BoardElement *tmp_element = board[enemy->x-1][enemy->y];
				board[enemy->x-1][enemy->y] = enemy;
				board[enemy->x][enemy->y] = tmp_element;
				enemy->x = enemy->x-1;
				tmp_element->x = tmp_element->x+1; 
				response = 1;
			}else{
				response = 1;
			}
		}else{
			response = 1;
		}
	}else{
		if(board[enemy->x+1][enemy->y]){
			if(board[enemy->x+1][enemy->y]->type == 0){
				BoardElement *tmp_element = board[enemy->x+1][enemy->y];
				board[enemy->x+1][enemy->y] = enemy;
				board[enemy->x][enemy->y] = tmp_element;
				enemy->x = enemy->x+1;
				tmp_element->x = tmp_element->x-1; 
				response = 1;
			}else{
				response = 1;
			}
		}else{
			response = 1;
		}
	}
	return response;
}
int calculate_percentage(int x , int y , int percent){
	int total,res;
	total= x*y;
	res = total*percent/100;
	return res;
}
