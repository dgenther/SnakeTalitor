///////////////////////////////
//
//			SNAKETALITOR
//
///////////////////////////////
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h> 
#include <math.h>
#include <string.h>
#include <Windows.h>

#define WINDOW_X	1600
#define WINDOW_Y	900

#define FIELD_OFFSET_X 2
#define GAME_FIELD_X (WINDOW_X/32)*FIELD_OFFSET_X
#define GAME_FIELD_Y (WINDOW_X/32)
#define GAME_FIELD_SIZE (WINDOW_X - GAME_FIELD_X/(FIELD_OFFSET_X-GAME_FIELD_X) - (WINDOW_X)/2)


#define FOOD_SIZE 0.75
#define SNAKE_WIDTH (1.0/6.0)

#define START_X (int)(unitFieldSize/3+3)
#define START_Y (int)(unitFieldSize/2+3)
#define START_LENGTH 2
#define FONT_CHAR_SIZE 27

#define TRUE 1
#define FALSE 0
#define DIFFICULTY 5

#define UNIT_SIZE ((WINDOW_X/16)/(DIFFICULTY))
#define UNIT_FIELD_SIZE (GAME_FIELD_SIZE/UNIT_SIZE)

int diff = 1;												//Difficulty  
int diffOffset;												//Pretty much just determines the number of squares in the field
															//[1 to 100]  lol, anything above 20 is pretty much impossible, but looks cool
															
float gameSpeed = 1;										//Speed is also scaled with difficulty

int red,green,blue; 										//variables used to store color
int unitSize; 												//size of each square in pixels
int unitFieldSize;											//size of the field in square units
float textScale = 0.5;
float snakeWidth = 1.0/6.0;
int gameTime = 0;											//game time in seconds
int GameOn = TRUE;											//boolean to determine if game is currently on
int canChangeDirection = TRUE;								//boolean to determine if we've already given a turn command this movement clock cycle
int disableBodyCheck = FALSE;
int skipDeleteSegment = FALSE;

void reverse(char* str, int len) { 
    int i = 0, j = len - 1, temp; 
    while (i < j) { 
        temp = str[i]; 
        str[i] = str[j]; 
        str[j] = temp; 
        i++; 
        j--; 
    } 
} 
  
int intToStr(int x, char str[], int d) { 
    int i = 0; 
    while (x) { 
        str[i++] = (x % 10) + '0'; 
        x = x / 10; 
    } 
  
    // If number of digits required is more, then 
    // add 0s at the beginning 
    while (i < d) 
        str[i++] = '0'; 
  
    reverse(str, i); 
    str[i] = '\0'; 
    return i; 
} 
  
void ftoa(float n, char* res, int afterpoint) { 
    // Extract integer part 
    int ipart = (int)n; 
  
    // Extract floating part 
    float fpart = n - (float)ipart; 
  
    // convert integer part to string 
    int i = intToStr(ipart, res, 0); 
  
    // check for display option after point 
    if (afterpoint != 0) { 
        res[i] = '.'; // add dot 
  
        // Get the value of fraction part upto given no. 
        // of points after dot. The third parameter  
        // is needed to handle cases like 233.007 
        fpart = fpart * pow(10, afterpoint); 
  
        intToStr((int)fpart, res + i + 1, afterpoint); 
    } 
}

typedef struct {
	int row, col;
} Food;
Food *currentFood;

void changeColor(float r, float g, float b){
	red = r; green = g; blue = b; 
	glColor3f(red/255.0,green/255.0,blue/255.0);
}

typedef struct Snake Snake;

struct Snake {
	int row;
	int col;
	char direction;
	Snake *nextSeg;
};

Snake *currentSnake;

Snake *createSnake(int col, int row, char direction){
	
	Snake *new_snake = (Snake*)malloc(sizeof(Snake));
	if(!new_snake){
		printf("Memory Error during createSnake\n");
		return;
	}
	new_snake->col = col;
	new_snake->row = row;
	new_snake->direction = direction;
	new_snake->nextSeg = NULL;
	return new_snake;
}

void deleteSnake(Snake **snake){
	Snake *current = *snake;
	Snake *next;
	
	while(current != NULL){
		next = current->nextSeg;
		free(current);
		current = next;
	}
	*snake = NULL;
}

void generateRandCoords(int *x, int *y){
	switch(getDiffOffset()){
		case 0:				// 0 doesn't exist, but I'd rather have it default to
		case 1:				// 8x8 instead of 801x801
			*x = rand()%8;
			*y = rand()%8;
			break;
		case 2:
			*x = rand()%16;
			*y = rand()%16;
			break;
		case 3:
			*x = rand()%24;
			*y = rand()%24;
			break;
		case 4:
			*x = rand()%32;
			*y = rand()%32;
			break;
		case 5:
			*x = rand()%40;
			*y = rand()%40;
			break;
		case 6:
			*x = rand()%50;
			*y = rand()%50;
			break;
		case 7:
			*x = rand()%57;
			*y = rand()%57;
			break;
		case 8:
			*x = rand()%66;
			*y = rand()%66;
			break;
		case 9:
			*x = rand()%72;
			*y = rand()%72;
			break;
		case 10:
			*x = rand()%80;
			*y = rand()%80;
			break;
		case 11:
			*x = rand()%89;
			*y = rand()%89;
			break;
		case 12:
			*x = rand()%100;
			*y = rand()%100;
			break;
		case 13:
		case 14:
			*x = rand()%114;
			*y = rand()%114;
			break;
		case 15:
		case 16:
			*x = rand()%133;
			*y = rand()%133;
			break;
		case 17:
		case 18:
		case 19:
		case 20:
			*x = rand()%160;
			*y = rand()%160;
			break;
		case 21:
		case 22:
		case 23:
		case 24:
		case 25:
			*x = rand()%200;
			*y = rand()%200;
			break;
		case 26:
		case 27:
		case 28:
		case 29:
		case 30:
		case 31:
		case 32:
		case 33:
			*x = rand()%267;
			*y = rand()%267;
			break;
		case 34:
		case 35:
		case 36:
		case 37:
		case 38:
		case 39:
		case 40:
		case 41:
		case 42:
		case 43:
		case 44:
		case 45:
		case 46:
		case 47:
		case 48:
		case 49:
		case 50:
			*x = rand()%400;
			*y = rand()%400;
			break;
		default:
			*x = rand()%801;
			*y = rand()%801;
			break;
	}	
}

void createSegmentOf(Snake **snake){		// input:  currentSnake, maybe second snake late}
	int pos = 2;
	int k = 1;
	Snake *q,*p;
	Snake *newSeg = (Snake*)malloc(sizeof(Snake));
	if(!newSeg){
		printf("Memory Error during creation of new Segment\n");
		return;
	}
	newSeg->col = currentSnake->col;
	newSeg->row = currentSnake->row;
	newSeg->direction = currentSnake->direction;
	newSeg->nextSeg = currentSnake->nextSeg;
	p = *snake;
	if(p == NULL){
		newSeg->nextSeg = *snake;
		*snake = newSeg;
	}
	else{
		while( p != NULL && (k < pos) ){
			k++;
			q = p;
			p = p->nextSeg;
		}
		newSeg->nextSeg = q->nextSeg;
		q->nextSeg = newSeg;
	}
}

void resetGame(){
	int x, y;
	char dir;
	gameTime = 0;
	GameOn = FALSE;

	deleteSnake(&currentSnake);

	generateRandCoords(&x, &y);
	if (x < unitFieldSize / 2){
		dir = 'r';
	}else {
		dir = 'l';
	}
	
	//printf("x = %d,      y = %d,    unitFieldSize = %d\n", x, y, unitFieldSize);
	

	
	currentSnake = createSnake(x, y, dir);
	createSegmentOf(&currentSnake);
	(dir == 'l') ? currentSnake->col-- : currentSnake->col++ ;
	
	

	generateRandCoords(&x, &y);
	currentFood->col = x;
	currentFood->row = y;

	GameOn = TRUE;
}

Food *createFood(){		//create food item at random location
	int x = 0;
	int y = 0;
	Snake *snake = currentSnake;
	
	generateRandCoords(&x, &y);
	while(snake != NULL){
		if(snake->col == x && snake->row == y){
			generateRandCoords(&x, &y);
			*snake = *currentSnake;
		} 
		snake=snake->nextSeg;
	}
	
	Food *new_food;

	new_food = malloc(sizeof(Food));
	new_food->col = x;
	new_food->row = y;


	return new_food;
}

int snakeLength(Snake *snake){
	int count=0;
	Snake *current = snake;
	while(current != NULL){
		count++;
		current = current->nextSeg;	
	}
	return count;
}

int deleteSnakeSegment(Snake **snake){
	int pos = snakeLength(*snake);
	int k=1;
	Snake *p,*q;
	p=*snake;
	if( *snake == NULL ){
		//printf("Snake Empty\n");
		return 1;
	}
	else if( pos == 1 ){
		*snake=(*snake)->nextSeg;
		free(p);
	}
	else{
		while( p!=NULL && k<pos ){
			k++;
			q = p;
			p = p->nextSeg;
		}
		if(p==NULL){
			//printf("Position does not exist\n");
		}
		else{
			q->nextSeg = p->nextSeg;
			free(p);
		}
	}
	return 0;
}

void drawSnake(Snake *snake){
	int k = 1;					//counter for snake length
	float snakeWidthL;			//left side of snake
	float snakeWidthR; 			//right side of snake
	float snakeWidthU; 			//top side of snake
	float snakeWidthD;			//bottom side of snake
	float snakeUnit = unitSize * snakeWidth;
	int col = GAME_FIELD_X + unitSize * snake->col;
	int row = GAME_FIELD_Y + unitSize * snake->row;
	
	///////////////////////////////DRAW SNAKE HEAD
	changeColor(50,100,25);

	//Snake head facing to the right, starting in the top left part of square for snake neck and working around clockwise
	
	////////////////////////////////////////THIS SWITCH DRAWS THE PART THAT CONNECTS THE HEAD TO THE BODY AROUND CORNERS
	switch(snake->nextSeg->direction){

		case 'u':
			glBegin(GL_QUADS);
				glVertex2f(		col	+		snakeUnit*(1)		, 	row	+		snakeUnit*(3)		); // vertex 
				glVertex2f(		col	+		snakeUnit*(1)		, 	row + 		snakeUnit*(6) 		); // vertex 
				glVertex2f(		col	+		snakeUnit*(5)		, 	row + 		snakeUnit*(6) 		); // vertex 
				glVertex2f(		col	+		snakeUnit*(5)		, 	row	+		snakeUnit*(3)		); // vertex 
			glEnd();
			break;
		case 'd':
			glBegin(GL_QUADS);
				glVertex2f(		col	+		snakeUnit*(1)		, 	row 							); // vertex 
				glVertex2f(		col	+		snakeUnit*(1)		, 	row + 		snakeUnit*(3) 		); // vertex 
				glVertex2f(		col	+		snakeUnit*(5)		, 	row + 		snakeUnit*(3) 		); // vertex 
				glVertex2f(		col	+		snakeUnit*(5)		, 	row								); // vertex 
			glEnd();
			break;
		case 'l':
			glBegin(GL_QUADS);
				glVertex2f(		col	+ 		snakeUnit*(3)		, 	row +		snakeUnit*(1)		); // vertex 
				glVertex2f(		col	+ 		snakeUnit*(3)		, 	row + 		snakeUnit*(5) 		); // vertex 
				glVertex2f(		col	+		snakeUnit*(6)		, 	row + 		snakeUnit*(5) 		); // vertex 
				glVertex2f(		col	+		snakeUnit*(6)		, 	row	+		snakeUnit*(1)		); // vertex 
			glEnd();
			break;
		case 'r':			
			glBegin(GL_QUADS);
				glVertex2f(		col	+ 		snakeUnit*(0)		, 	row +		snakeUnit*(1)		); // vertex 
				glVertex2f(		col	+ 		snakeUnit*(0)		, 	row + 		snakeUnit*(5) 		); // vertex 
				glVertex2f(		col	+		snakeUnit*(3)		, 	row + 		snakeUnit*(5) 		); // vertex 
				glVertex2f(		col	+		snakeUnit*(3)		, 	row	+		snakeUnit*(1)		); // vertex 
			glEnd();
			break;
	}
	////////////////////////////////////////THIS SWITCH DRAWS THE HEAD
	switch(snake->direction){
		
		case 'u':
			glBegin(GL_QUADS);
			glVertex2f(		col	+		snakeUnit*(1)		, 	row + 		snakeUnit*(5) 			); // vertex 
			glVertex2f(		col	+		snakeUnit*(2)		, 	row + 		snakeUnit*(5.5) 		); // vertex 
			glVertex2f(		col	+		snakeUnit*(4)		, 	row + 		snakeUnit*(5.5) 		); // vertex 
			glVertex2f(		col	+		snakeUnit*(5)		, 	row + 		snakeUnit*(5) 			); // vertex 
			glEnd();
			
			glBegin(GL_TRIANGLE_STRIP);
			glVertex2f(		col + 		snakeUnit*(3)		, 	row + 		snakeUnit*(5.6)			); // vertex 
			glVertex2f(		col + 		snakeUnit*(1)		, 	row + 		snakeUnit*(6-1)			); // vertex 
			glVertex2f(		col + 		snakeUnit*(5)		, 	row + 		snakeUnit*(6-1)			); // vertex 
			glVertex2f(		col + 		snakeUnit*(0.5)		, 	row + 		snakeUnit*(6-2)			); // vertex 
			glVertex2f(		col + 		snakeUnit*(5.5)		, 	row + 		snakeUnit*(6-2)			); // vertex 
			glVertex2f(		col + 		snakeUnit*(0.4)		, 	row + 		snakeUnit*(6-2.75)		); // vertex 
			glVertex2f(		col + 		snakeUnit*(5.6)		, 	row + 		snakeUnit*(6-2.75)		); // vertex 
			glVertex2f(		col + 		snakeUnit*(0.6)		, 	row + 		snakeUnit*(6-3.25)		); // vertex 
			glVertex2f(		col + 		snakeUnit*(5.4)		, 	row + 		snakeUnit*(6-3.25)		); // vertex 
			glVertex2f(		col + 		snakeUnit*(2)		, 	row + 		snakeUnit*(6-5)			); // vertex 
			glVertex2f(		col + 		snakeUnit*(4)		, 	row + 		snakeUnit*(6-5)			); // vertex 
			glVertex2f(		col + 		snakeUnit*(2.75)	, 	row + 		snakeUnit*(6-5.5)		); // vertex 
			glVertex2f(		col + 		snakeUnit*(3.25)	, 	row + 		snakeUnit*(6-5.5)		); // vertex 
			glVertex2f(		col + 		snakeUnit*(3)		, 	row + 		snakeUnit*(6-5.55)		); // vertex 
			glEnd();
			
			changeColor(50,25,25);
			///////////////////EYES
			glBegin(GL_QUADS);
				glVertex2f(		col + 		snakeUnit*(1.6)		, 	row + 		snakeUnit*2.2		); // vertex 
				glVertex2f(		col + 		snakeUnit*(2)		, 	row + 		snakeUnit*2.2			); // vertex 
				glVertex2f(		col + 		snakeUnit*(1.8)		, 	row + 		snakeUnit*2.8		); // vertex  
				glVertex2f(		col + 		snakeUnit*(1.4)		, 	row + 		snakeUnit*2.8		); // vertex 
			glEnd();			
			
			glBegin(GL_QUADS);
				glVertex2f(		col + 		snakeUnit*(6-1.6)		, 	row + 		snakeUnit*(2.2)		); // vertex 
				glVertex2f(		col + 		snakeUnit*(6-2)			, 	row + 		snakeUnit*(2.2)			); // vertex 
				glVertex2f(		col + 		snakeUnit*(6-1.8)		, 	row + 		snakeUnit*(2.8)		); // vertex  
				glVertex2f(		col + 		snakeUnit*(6-1.4)		, 	row + 		snakeUnit*(2.8)		); // vertex 
			glEnd();			
			
			changeColor(0,0,0);
			glLineWidth(unitSize/30);
			
			glBegin(GL_LINES);
				glVertex2f(		col + 		snakeUnit*(2.2)		, 	row + 		snakeUnit*(2.5)		); // vertex 
				glVertex2f(		col + 		snakeUnit*(1)		, 	row + 		snakeUnit*(3)		); // vertex
			glEnd();

			glBegin(GL_LINES);
				glVertex2f(		col + 		snakeUnit*(6-2.2)		, 	row + 		snakeUnit*(2.5)		); // vertex 
				glVertex2f(		col + 		snakeUnit*(6-1)		, 	row + 		snakeUnit*(3)		); // vertex
			glEnd();
			break;
			
		case 'd':
			glBegin(GL_QUADS);
			glVertex2f(		col	+		snakeUnit*(1)		, 	row + 		snakeUnit*(1) 			); // vertex 
			glVertex2f(		col	+		snakeUnit*(2)		, 	row + 		snakeUnit*(0.5) 		); // vertex 
			glVertex2f(		col	+		snakeUnit*(4)		, 	row + 		snakeUnit*(0.5) 		); // vertex 
			glVertex2f(		col	+		snakeUnit*(5)		, 	row + 		snakeUnit*(1) 			); // vertex 
			glEnd();

			glBegin(GL_TRIANGLE_STRIP);
			glVertex2f(		col	+		snakeUnit*(3)		, 	row + 		snakeUnit*(0.4) 		); // vertex 
			glVertex2f(		col + 		snakeUnit*(6-1)		, 	row + 		snakeUnit*(1)			); // vertex 
			glVertex2f(		col + 		snakeUnit*(6-5)		, 	row + 		snakeUnit*(1)			); // vertex 
			glVertex2f(		col + 		snakeUnit*(6-0.5)	, 	row + 		snakeUnit*(2)			); // vertex 
			glVertex2f(		col + 		snakeUnit*(6-5.5)	, 	row + 		snakeUnit*(2)			); // vertex 
			glVertex2f(		col + 		snakeUnit*(6-0.4)	, 	row + 		snakeUnit*(2.75)		); // vertex 
			glVertex2f(		col + 		snakeUnit*(6-5.6)	, 	row + 		snakeUnit*(2.75)		); // vertex 
			glVertex2f(		col + 		snakeUnit*(6-0.6)	, 	row + 		snakeUnit*(3.25)		); // vertex 
			glVertex2f(		col + 		snakeUnit*(6-5.4)	, 	row + 		snakeUnit*(3.25)		); // vertex 
			glVertex2f(		col + 		snakeUnit*(6-2)		, 	row + 		snakeUnit*(5)			); // vertex 
			glVertex2f(		col + 		snakeUnit*(6-4)		, 	row + 		snakeUnit*(5)			); // vertex 
			glVertex2f(		col + 		snakeUnit*(6-2.75)	, 	row + 		snakeUnit*(5.5)			); // vertex 
			glVertex2f(		col + 		snakeUnit*(6-3.25)	, 	row + 		snakeUnit*(5.5)			); // vertex 
			glVertex2f(		col + 		snakeUnit*(6-3)		, 	row + 		snakeUnit*(5.55)		); // vertex 
			glEnd();
			
			changeColor(50,25,25);
			///////////////////EYES
			glBegin(GL_QUADS);
				glVertex2f(		col + 		snakeUnit*(1.6)		, 	row + 		snakeUnit*(6-2.2)	); // vertex 
				glVertex2f(		col + 		snakeUnit*(2)		, 	row + 		snakeUnit*(6-2.2)	); // vertex 
				glVertex2f(		col + 		snakeUnit*(1.8)		, 	row + 		snakeUnit*(6-2.8)	); // vertex  
				glVertex2f(		col + 		snakeUnit*(1.4)		, 	row + 		snakeUnit*(6-2.8)	); // vertex 
			glEnd();			
			
			glBegin(GL_QUADS);
				glVertex2f(		col + 		snakeUnit*(6-1.6)	, 	row + 		snakeUnit*(6-2.2)	); // vertex 
				glVertex2f(		col + 		snakeUnit*(6-2)		, 	row + 		snakeUnit*(6-2.2)	); // vertex 
				glVertex2f(		col + 		snakeUnit*(6-1.8)	, 	row + 		snakeUnit*(6-2.8)	); // vertex  
				glVertex2f(		col + 		snakeUnit*(6-1.4)	, 	row + 		snakeUnit*(2.8)		); // vertex 
			glEnd();		
			
			changeColor(0,0,0);
			glLineWidth(unitSize/30);
			
			glBegin(GL_LINES);
				glVertex2f(		col + 		snakeUnit*(2.2)		, 	row + 		snakeUnit*(6-2.5)	); // vertex 
				glVertex2f(		col + 		snakeUnit*(1)		, 	row + 		snakeUnit*(6-3)		); // vertex
			glEnd();

			glBegin(GL_LINES);
				glVertex2f(		col + 		snakeUnit*(6-2.2)	, 	row + 		snakeUnit*(6-2.5)	); // vertex 
				glVertex2f(		col + 		snakeUnit*(6-1)		, 	row + 		snakeUnit*(6-3)		); // vertex
			glEnd();
			break;
			
		case 'l':
			glBegin(GL_QUADS);
			glVertex2f(		col	+		snakeUnit*(5.5)		, 	row + 		snakeUnit*(2) 		); // vertex 
			glVertex2f(		col	+		snakeUnit*(5)		, 	row + 		snakeUnit*(1) 		); // vertex 
			glVertex2f(		col	+		snakeUnit*(5)		, 	row + 		snakeUnit*(5) 		); // vertex 
			glVertex2f(		col	+		snakeUnit*(5.5)		, 	row + 		snakeUnit*(4) 		); // vertex 
			glEnd();
			
			glBegin(GL_TRIANGLES);	
			glVertex2f(		col + 		snakeUnit*(6-1)		, 	row + 		snakeUnit*1 			); // vertex 
			glVertex2f(		col + 		snakeUnit*(6-1)		, 	row + 		snakeUnit*5				); // vertex 
			glVertex2f(		col + 		snakeUnit*(5.5)		, 	row + 		snakeUnit*3				); // vertex 
			glEnd();
			
			glBegin(GL_TRIANGLE_STRIP);			
			//glVertex2f(		col + 		snakeUnit*(6-0.5)	, 	row + 		snakeUnit*3 			); // vertex
			glVertex2f(		col + 		snakeUnit*(6-1)		, 	row + 		snakeUnit*1 			); // vertex 
			glVertex2f(		col + 		snakeUnit*(6-1)		, 	row + 		snakeUnit*5				); // vertex 
			glVertex2f(		col + 		snakeUnit*(6-2)		, 	row + 		snakeUnit*0.5			); // vertex 
			glVertex2f(		col + 		snakeUnit*(6-2)		, 	row + 		snakeUnit*5.5			); // vertex 
			glVertex2f(		col + 		snakeUnit*(6-2.75)	, 	row + 		snakeUnit*0.4			); // vertex 
			glVertex2f(		col + 		snakeUnit*(6-2.75)	, 	row + 		snakeUnit*5.6			); // vertex 
			glVertex2f(		col + 		snakeUnit*(6-3.25)	, 	row + 		snakeUnit*0.6			); // vertex 
			glVertex2f(		col + 		snakeUnit*(6-3.25)	, 	row + 		snakeUnit*5.4			); // vertex 
			glVertex2f(		col + 		snakeUnit*(6-5)		, 	row + 		snakeUnit*2				); // vertex 
			glVertex2f(		col + 		snakeUnit*(6-5)		, 	row + 		snakeUnit*4				); // vertex 
			glVertex2f(		col + 		snakeUnit*(6-5.5)	, 	row + 		snakeUnit*2.75			); // vertex 
			glVertex2f(		col + 		snakeUnit*(6-5.5)	, 	row + 		snakeUnit*3.25			); // vertex 
			glVertex2f(		col + 		snakeUnit*(6-5.55)	, 	row + 		snakeUnit*3				); // vertex 
			glEnd();
			
			changeColor(50,25,25);
			///////////////////EYES
			glBegin(GL_QUADS);
				glVertex2f(		col + 		snakeUnit*(2.2)		, 	row + 		snakeUnit*1.6		); // vertex 
				glVertex2f(		col + 		snakeUnit*(2.2)		, 	row + 		snakeUnit*2			); // vertex 
				glVertex2f(		col + 		snakeUnit*(2.8)		, 	row + 		snakeUnit*1.8		); // vertex  
				glVertex2f(		col + 		snakeUnit*(2.8)		, 	row + 		snakeUnit*1.4		); // vertex 
			glEnd();
			
			glBegin(GL_QUADS);
				glVertex2f(		col + 		snakeUnit*(2.2)		, 	row + 		snakeUnit*(6-1.6)		); // vertex 
				glVertex2f(		col + 		snakeUnit*(2.2)		, 	row + 		snakeUnit*(6-2)			); // vertex 
				glVertex2f(		col + 		snakeUnit*(2.8)		, 	row + 		snakeUnit*(6-1.8)		); // vertex 
				glVertex2f(		col + 		snakeUnit*(2.8)		, 	row + 		snakeUnit*(6-1.4)		); // vertex 
			glEnd();
			
			changeColor(0,0,0);
			glLineWidth(unitSize/30);
			glBegin(GL_LINES);
				glVertex2f(		col + 		snakeUnit*(2.5)		, 	row + 		snakeUnit*2.2		); // vertex 
				glVertex2f(		col + 		snakeUnit*(3)		, 	row + 		snakeUnit*1		); // vertex
			glEnd();
			
			changeColor(0,0,0);
			glLineWidth(unitSize/30);
			glBegin(GL_LINES);
				glVertex2f(		col + 		snakeUnit*(2.5)		, 	row + 		snakeUnit*(6-2.2)		); // vertex 
				glVertex2f(		col + 		snakeUnit*(3)		, 	row + 		snakeUnit*(6-1)		); // vertex
			glEnd();
			break;
			
		case 'r':
			glBegin(GL_QUADS);
			glVertex2f(		col	+		snakeUnit*(0.5)		, 	row + 		snakeUnit*(2) 		); // vertex 
			glVertex2f(		col	+		snakeUnit*(0.5)		, 	row + 		snakeUnit*(4) 		); // vertex 
			glVertex2f(		col	+		snakeUnit*(1)		, 	row + 		snakeUnit*(5) 		); // vertex 
			glVertex2f(		col	+		snakeUnit*(1)		, 	row + 		snakeUnit*(1) 		); // vertex 
			glEnd();

			glBegin(GL_TRIANGLE_STRIP);			
			glVertex2f(		col + 		snakeUnit		, 	row + 		snakeUnit*1 			); // vertex 
			glVertex2f(		col + 		snakeUnit		, 	row + 		snakeUnit*5				); // vertex 
			glVertex2f(		col + 		snakeUnit*2		, 	row + 		snakeUnit*0.5			); // vertex 
			glVertex2f(		col + 		snakeUnit*2		, 	row + 		snakeUnit*5.5			); // vertex 
			glVertex2f(		col + 		snakeUnit*2.75	, 	row + 		snakeUnit*0.4			); // vertex 
			glVertex2f(		col + 		snakeUnit*2.75	, 	row + 		snakeUnit*5.6			); // vertex 
			glVertex2f(		col + 		snakeUnit*3.25	, 	row + 		snakeUnit*0.6			); // vertex 
			glVertex2f(		col + 		snakeUnit*3.25	, 	row + 		snakeUnit*5.4			); // vertex 
			glVertex2f(		col + 		snakeUnit*5		, 	row + 		snakeUnit*2				); // vertex 
			glVertex2f(		col + 		snakeUnit*5		, 	row + 		snakeUnit*4				); // vertex 
			glVertex2f(		col + 		snakeUnit*5.5	, 	row + 		snakeUnit*2.75			); // vertex 
			glVertex2f(		col + 		snakeUnit*5.5	, 	row + 		snakeUnit*3.25			); // vertex 
			glVertex2f(		col + 		snakeUnit*5.55	, 	row + 		snakeUnit*3				); // vertex 
			glEnd();
			
			changeColor(50,25,25);
			///////////////////EYES
			glBegin(GL_TRIANGLE_STRIP);
			glVertex2f(		col + 		snakeUnit*(6-2.2)		, 	row + 		snakeUnit*1.6			); // vertex 
			glVertex2f(		col + 		snakeUnit*(6-2.4)		, 	row + 		snakeUnit*1.4			); // vertex 
			glVertex2f(		col + 		snakeUnit*(6-2.6)		, 	row + 		snakeUnit*1.8			); // vertex 
			glVertex2f(		col + 		snakeUnit*(6-2.8)		, 	row + 		snakeUnit*1.2			); // vertex 
			glEnd();
			
			glBegin(GL_TRIANGLE_STRIP);
			glVertex2f(		col + 		snakeUnit*(6-2.2)		, 	row + 		snakeUnit*1.6			); // vertex 
			glVertex2f(		col + 		snakeUnit*(6-2.4)		, 	row + 		snakeUnit*1.4			); // vertex 
			glVertex2f(		col + 		snakeUnit*(6-2.6)		, 	row + 		snakeUnit*1.8			); // vertex 
			glVertex2f(		col + 		snakeUnit*(6-2.8)		, 	row + 		snakeUnit*1.2			); // vertex 
			glEnd();
			
			changeColor(50,25,25);
			///////////////////EYES
			glBegin(GL_QUADS);
				glVertex2f(		col + 		snakeUnit*(6-2.2)		, 	row + 		snakeUnit*1.6		); // vertex 
				glVertex2f(		col + 		snakeUnit*(6-2.2)		, 	row + 		snakeUnit*2			); // vertex 
				glVertex2f(		col + 		snakeUnit*(6-2.8)		, 	row + 		snakeUnit*1.8		); // vertex  
				glVertex2f(		col + 		snakeUnit*(6-2.8)		, 	row + 		snakeUnit*1.4		); // vertex 
			glEnd();
			
			glBegin(GL_QUADS);
				glVertex2f(		col + 		snakeUnit*(6-2.2)		, 	row + 		snakeUnit*(6-1.6)		); // vertex 
				glVertex2f(		col + 		snakeUnit*(6-2.2)		, 	row + 		snakeUnit*(6-2)			); // vertex 
				glVertex2f(		col + 		snakeUnit*(6-2.8)		, 	row + 		snakeUnit*(6-1.8)		); // vertex 
				glVertex2f(		col + 		snakeUnit*(6-2.8)		, 	row + 		snakeUnit*(6-1.4)		); // vertex 
			glEnd();
			
			changeColor(0,0,0);
			glLineWidth(unitSize/30);
			glBegin(GL_LINES);
				glVertex2f(		col + 		snakeUnit*(6-2.5)		, 	row + 		snakeUnit*2.2		); // vertex 
				glVertex2f(		col + 		snakeUnit*(6-3)		, 	row + 		snakeUnit*1		); // vertex
			glEnd();
			
			changeColor(0,0,0);
			glLineWidth(unitSize/30);
			glBegin(GL_LINES);
				glVertex2f(		col + 		snakeUnit*(6-2.5)		, 	row + 		snakeUnit*(6-2.2)		); // vertex 
				glVertex2f(		col + 		snakeUnit*(6-3)		, 	row + 		snakeUnit*(6-1)		); // vertex
			glEnd();
			break;
			
		}
    snake = snake->nextSeg;
    k++;
    
    changeColor(50,100,25);
	///////////////////////////////DRAW BODY SEGMENTS
	while(k < snakeLength(currentSnake)){
		col = GAME_FIELD_X + unitSize * snake->col;
		row = GAME_FIELD_Y + unitSize * snake->row;
		if (snake->nextSeg != NULL){
			
			if ( (snake->nextSeg->direction == 'u' && snake->direction == 'l') || (snake->nextSeg->direction == 'r' && snake->direction == 'd')){
				glBegin(GL_TRIANGLE_STRIP);
					glVertex2f(		col + 		snakeUnit*(0)		, 	row + 		snakeUnit*(1)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(0)		, 	row + 		snakeUnit*(5)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(1)		, 	row + 		snakeUnit*(1.2)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(1/6)		, 	row + 		snakeUnit*(5)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(2)		, 	row + 		snakeUnit*(1.5)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(2/6)		, 	row + 		snakeUnit*(5)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(3)		, 	row + 		snakeUnit*(2)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(3/6)		, 	row + 		snakeUnit*(5)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(4)		, 	row + 		snakeUnit*(3)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(4/6)		, 	row + 		snakeUnit*(5)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(4.5)		, 	row + 		snakeUnit*(4)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(5/6)		, 	row + 		snakeUnit*(5)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(4.8)		, 	row + 		snakeUnit*(5)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(1)		, 	row + 		snakeUnit*(6)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(5)		, 	row + 		snakeUnit*(6)		); // vertex 
				glEnd();
			}
			
			if ( (snake->nextSeg->direction == 'u' && snake->direction == 'r') || (snake->nextSeg->direction == 'l' && snake->direction == 'd')){
				glBegin(GL_TRIANGLE_STRIP);
					glVertex2f(		col + 		snakeUnit*(6-0)		, 	row + 		snakeUnit*(1)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(6-0)		, 	row + 		snakeUnit*(5)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(6-1)		, 	row + 		snakeUnit*(1.2)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(6-1/6)	, 	row + 		snakeUnit*(5)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(6-2)		, 	row + 		snakeUnit*(1.5)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(6-2/6)	, 	row + 		snakeUnit*(5)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(6-3)		, 	row + 		snakeUnit*(2)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(6-3/6)	, 	row + 		snakeUnit*(5)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(6-4)		, 	row + 		snakeUnit*(3)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(6-4/6)	, 	row + 		snakeUnit*(5)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(6-4.5)	, 	row + 		snakeUnit*(4)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(6-5/6)	, 	row + 		snakeUnit*(5)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(6-4.8)	, 	row + 		snakeUnit*(5)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(6-1)		, 	row + 		snakeUnit*(6)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(6-5)		, 	row + 		snakeUnit*(6)		); // vertex 
				glEnd();
			}
			if ( (snake->nextSeg->direction == 'd' && snake->direction == 'r') || (snake->nextSeg->direction == 'l' && snake->direction == 'u') ){
				glBegin(GL_TRIANGLE_STRIP);
					glVertex2f(		col + 		snakeUnit*(6-0)		, 	row + 		snakeUnit*(6-1)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(6-0)		, 	row + 		snakeUnit*(6-5)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(6-1)		, 	row + 		snakeUnit*(6-1.2)	); // vertex 
					glVertex2f(		col + 		snakeUnit*(6-1/6)	, 	row + 		snakeUnit*(6-5)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(6-2)		, 	row + 		snakeUnit*(6-1.5)	); // vertex 
					glVertex2f(		col + 		snakeUnit*(6-2/6)	, 	row + 		snakeUnit*(6-5)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(6-3)		, 	row + 		snakeUnit*(6-2)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(6-3/6)	, 	row + 		snakeUnit*(6-5)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(6-4)		, 	row + 		snakeUnit*(6-3)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(6-4/6)	, 	row + 		snakeUnit*(6-5)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(6-4.5)	, 	row + 		snakeUnit*(6-4)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(6-5/6)	, 	row + 		snakeUnit*(6-5)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(6-4.8)	, 	row + 		snakeUnit*(6-5)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(6-1)		, 	row + 		snakeUnit*(6-6)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(6-5)		, 	row + 		snakeUnit*(6-6)		); // vertex 
				glEnd();
			}
			if ( (snake->nextSeg->direction == 'd' && snake->direction == 'l') || (snake->nextSeg->direction == 'r' && snake->direction == 'u') ){
				glBegin(GL_TRIANGLE_STRIP);
					glVertex2f(		col + 		snakeUnit*(0)		, 	row + 		snakeUnit*(6-1)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(0)		, 	row + 		snakeUnit*(6-5)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(1)		, 	row + 		snakeUnit*(6-1.2)	); // vertex 
					glVertex2f(		col + 		snakeUnit*(1/6)		, 	row + 		snakeUnit*(6-5)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(2)		, 	row + 		snakeUnit*(6-1.5)	); // vertex 
					glVertex2f(		col + 		snakeUnit*(2/6)		, 	row + 		snakeUnit*(6-5)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(3)		, 	row + 		snakeUnit*(6-2)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(3/6)		, 	row + 		snakeUnit*(6-5)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(4)		, 	row + 		snakeUnit*(6-3)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(4/6)		, 	row + 		snakeUnit*(6-5)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(4.5)		, 	row + 		snakeUnit*(6-4)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(5/6)		, 	row + 		snakeUnit*(6-5)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(4.8)		, 	row + 		snakeUnit*(6-5)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(1)		, 	row + 		snakeUnit*(6-6)		); // vertex 
					glVertex2f(		col + 		snakeUnit*(5)		, 	row + 		snakeUnit*(6-6)		); // vertex 
				glEnd();
			}
			if ( (snake->nextSeg->direction == 'd' && snake->direction == 'd') || (snake->nextSeg->direction == 'u' && snake->direction == 'u')){
				glBegin(GL_QUADS);
					glVertex2f(		col	+		snakeUnit*(1)		, 	row	+		snakeUnit*(0)		); // vertex 
					glVertex2f(		col	+		snakeUnit*(1)		, 	row + 		snakeUnit*(6) 		); // vertex 
					glVertex2f(		col	+		snakeUnit*(5)		, 	row + 		snakeUnit*(6) 		); // vertex 
					glVertex2f(		col	+		snakeUnit*(5)		, 	row	+		snakeUnit*(0)		); // vertex 
				glEnd();
				
				
			}
			if ( (snake->nextSeg->direction == 'l' && snake->direction == 'l') || (snake->nextSeg->direction == 'r' && snake->direction == 'r')){
				glBegin(GL_QUADS);
					glVertex2f(		col	+ 		snakeUnit*(0)		, 	row +		snakeUnit*(1)		); // vertex 
					glVertex2f(		col	+ 		snakeUnit*(0)		, 	row + 		snakeUnit*(5) 		); // vertex 
					glVertex2f(		col	+		snakeUnit*(6)		, 	row + 		snakeUnit*(5) 		); // vertex 
					glVertex2f(		col	+		snakeUnit*(6)		, 	row	+		snakeUnit*(1)		); // vertex 
				glEnd();				
			}
		}
		
		snake = snake->nextSeg;
		//printf("Length = %d\n", snakeLength(currentSnake));
		k++;
	}
	
	col = GAME_FIELD_X + unitSize * snake->col;
	row = GAME_FIELD_Y + unitSize * snake->row;
	
	///////////////////////////////DRAW TAIL
	switch(snake->direction){
		case 'u':
			glBegin(GL_TRIANGLE_STRIP);
			glVertex2f(		col	+		snakeUnit*(3)		, 	row + 		snakeUnit*(6) 		); // vertex 
			glVertex2f(		col	+		snakeUnit*(2.75)	, 	row + 		snakeUnit*(5.75) 	); // vertex 
			glVertex2f(		col	+		snakeUnit*(3.25)	, 	row + 		snakeUnit*(5.75) 	); // vertex 
			glVertex2f(		col	+		snakeUnit*(2.5)		, 	row + 		snakeUnit*(5) 		); // vertex 
			glVertex2f(		col	+		snakeUnit*(3.5)		, 	row + 		snakeUnit*(5) 		); // vertex 
			glVertex2f(		col	+		snakeUnit*(1.25)	, 	row + 		snakeUnit*(1) 		); // vertex
			glVertex2f(		col	+		snakeUnit*(4.75)	, 	row + 		snakeUnit*(1) 		); // vertex
			glVertex2f(		col	+		snakeUnit*(1)		, 	row + 		snakeUnit*(0) 		); // vertex
			glVertex2f(		col	+		snakeUnit*(5)		, 	row + 		snakeUnit*(0) 		); // vertex
			glEnd();
			break;
		case 'd':
			glBegin(GL_TRIANGLE_STRIP);
			glVertex2f(		col	+		snakeUnit*(3)		, 	row + 		snakeUnit*(0) 		); // vertex 
			glVertex2f(		col	+		snakeUnit*(2.75)	, 	row + 		snakeUnit*(0.25) 	); // vertex 
			glVertex2f(		col	+		snakeUnit*(3.25)	, 	row + 		snakeUnit*(0.25) 	); // vertex 
			glVertex2f(		col	+		snakeUnit*(2.5)		, 	row + 		snakeUnit*(1) 		); // vertex 
			glVertex2f(		col	+		snakeUnit*(3.5)		, 	row + 		snakeUnit*(1) 		); // vertex 
			glVertex2f(		col	+		snakeUnit*(1.25)	, 	row + 		snakeUnit*(5) 		); // vertex
			glVertex2f(		col	+		snakeUnit*(4.75)	, 	row + 		snakeUnit*(5) 		); // vertex
			glVertex2f(		col	+		snakeUnit*(1)		, 	row + 		snakeUnit*(6) 		); // vertex
			glVertex2f(		col	+		snakeUnit*(5)		, 	row + 		snakeUnit*(6) 		); // vertex
			glEnd();
			break;
		case 'l':
			glBegin(GL_TRIANGLE_STRIP);
			glVertex2f(		col	+		snakeUnit*(0)		, 	row + 		snakeUnit*(1) 			); // vertex 
			glVertex2f(		col	+		snakeUnit*(0)		, 	row + 		snakeUnit*(5) 			); // vertex 
			glVertex2f(		col	+		snakeUnit*(1)		, 	row + 		snakeUnit*(1.25) 		); // vertex 
			glVertex2f(		col	+		snakeUnit*(1)		, 	row + 		snakeUnit*(4.75) 		); // vertex 
			glVertex2f(		col	+		snakeUnit*(5)		, 	row + 		snakeUnit*(2.5) 		); // vertex 
			glVertex2f(		col	+		snakeUnit*(5)		, 	row + 		snakeUnit*(3.5) 		); // vertex
			glVertex2f(		col	+		snakeUnit*(5.75)	, 	row + 		snakeUnit*(2.75) 		); // vertex
			glVertex2f(		col	+		snakeUnit*(5.75)	, 	row + 		snakeUnit*(3.25) 		); // vertex
			glVertex2f(		col	+		snakeUnit*(6)		, 	row + 		snakeUnit*(3) 			); // vertex
			glEnd();
			break;
		case 'r':	
			glBegin(GL_TRIANGLE_STRIP);
			glVertex2f(		col	+		snakeUnit*(0)		, 	row + 		snakeUnit*(3) 			); // vertex 
			glVertex2f(		col	+		snakeUnit*(0.25)	, 	row + 		snakeUnit*(3.25) 		); // vertex 
			glVertex2f(		col	+		snakeUnit*(0.25)	, 	row + 		snakeUnit*(2.75) 		); // vertex 
			glVertex2f(		col	+		snakeUnit*(1)		, 	row + 		snakeUnit*(3.5) 		); // vertex 
			glVertex2f(		col	+		snakeUnit*(1)		, 	row + 		snakeUnit*(2.5) 		); // vertex 
			glVertex2f(		col	+		snakeUnit*(5)		, 	row + 		snakeUnit*(4.75) 		); // vertex
			glVertex2f(		col	+		snakeUnit*(5)		, 	row + 		snakeUnit*(1.25) 		); // vertex
			glVertex2f(		col	+		snakeUnit*(6)		, 	row + 		snakeUnit*(5) 			); // vertex
			glVertex2f(		col	+		snakeUnit*(6)		, 	row + 		snakeUnit*(1) 			); // vertex
			glEnd();
			break;
	}
	glutPostRedisplay();	
}

void drawLoserSnake(Snake *snake){
	if(GameOn == FALSE){
		changeColor(150,50,25);
		glRectf(
			GAME_FIELD_X + (snake->col		) * unitSize,
			GAME_FIELD_Y + (snake->row		) * unitSize, 
			GAME_FIELD_X + (snake->col + 1	) * unitSize, 
			GAME_FIELD_Y + (snake->row + 1	) * unitSize
		);
	}
}

void drawFood(Food *food){		//draw food onto field}
	
	//printf("Begin Draw Food\n");
	glColor3f(1,1,1);
	glRectf(
		GAME_FIELD_X + (food->col )		* unitSize + (unitSize * FOOD_SIZE),
		GAME_FIELD_Y + (food->row )		* unitSize + (unitSize * FOOD_SIZE), 
		GAME_FIELD_X + (food->col + 1)	* unitSize - (unitSize * FOOD_SIZE), 
		GAME_FIELD_Y + (food->row + 1)	* unitSize - (unitSize * FOOD_SIZE)
	);
	//printf("End Draw Food\n");
	glutPostRedisplay();
}

////////////////////////////////////////////FIX UPDATE SNAKE
void updateSnake(Snake *snake){
	
	Snake *tempSnake = (Snake*)malloc(sizeof(Snake));	
	
	do {
		tempSnake->col = 			snake->col;
		tempSnake->row = 			snake->row;
		tempSnake->direction = 		snake->direction;
		
		snake->nextSeg->col = 		tempSnake->col;
		snake->nextSeg->row = 		tempSnake->row;
		snake->nextSeg->direction = tempSnake->direction;

		snake = snake->nextSeg;
		
	} while(snake->nextSeg != NULL);
	
	free(tempSnake);
}
////////////////////////////////////////////////////////////EVENTUALLY ADD SECOND SNAKE PLAYER CONTROLS HERE
void specialButtons(int key, int x, int y){
	
	switch(key){
    	case GLUT_KEY_DOWN:												//down
        	if (currentSnake->direction != 'u'){

        		currentSnake->direction = 'd';
        	}
	        break;
	    case GLUT_KEY_RIGHT:													//right
        	if (currentSnake->direction != 'l'){

	        	currentSnake->direction = 'r';	
	        }
            break;
        case GLUT_KEY_LEFT:														//left
        	if (currentSnake->direction != 'r'){

	        	currentSnake->direction = 'l';	
        	}
            break;
        case GLUT_KEY_UP:															//up
        	if (currentSnake->direction != 'd'){

	        	currentSnake->direction = 'u';	
	    	}
	        break;
	}
	glutPostRedisplay();
}

void buttons(unsigned char key, int x, int y){
	int i, j;
	
	//printf("canChangeDirection = %s\n", canChangeDirection ? "TRUE" : "FALSE" );
    switch(key){
        case 's':																//down
        	if (currentSnake->direction != 'u' && canChangeDirection){
        		currentSnake->direction = 'd';
        	}
	        break;
        case 'd':																//right
        	if (currentSnake->direction != 'l' && canChangeDirection){
	        	currentSnake->direction = 'r';	
	        }
            break;
        case 'a':																//left
        	if (currentSnake->direction != 'r' && canChangeDirection){
	        	currentSnake->direction = 'l';	
        	}
            break;
        case 'w':																//up
        	if (currentSnake->direction != 'd' && canChangeDirection){
	        	currentSnake->direction = 'u';	
	    	}
	        break;
		case 'y':
			resetGame();
			break;
		case '-':
			if (diff > 1){	
				diff--;
				resetGame();
			}
			break;
		case '=':
			if (diff < 19){	
				diff++;
				resetGame();
			}
			break;
		case '[':
			if (gameSpeed > 1){
				gameSpeed -= 1;
				resetGame();
			}
			break;
		case ']':
			if (gameSpeed < 5){
				gameSpeed += 1;
				resetGame();
			}
			break;
    }
    canChangeDirection = FALSE;
	glutPostRedisplay();
}

void drawField(){
	int i, j, k = 0;
	int x1 = 6, y1 = 6;
	int x2 = 2, y2 = 5;

	for (i = 0; i < unitFieldSize; i++){
		for (j = 0; j < unitFieldSize; j++){
		
			//BEGIN DRAWING CHECKERED BACKGROUND
			if (j%2 == k){
				glColor3f(0.08,0.08,0.08);
			}else {
				glColor3f(0.07,0.07,0.07);
			}
			glRectf(
				GAME_FIELD_X + (j	 ) * unitSize,
				GAME_FIELD_Y + (i	 ) * unitSize, 
				GAME_FIELD_X + (j + 1) * unitSize, 
				GAME_FIELD_Y + (i + 1) * unitSize
			);

			//END DRAWING CHECKERED BACKGROUND
			
		}
		k = k ? 0 : 1;
	}
	glutPostRedisplay();
}

int getDiffOffset(){
	int off;
	switch(diff){
		case 0:
		case 1:	
			off = 1;
			break;
		case 2:
			off = 2;
			break;
		case 3:
			off = 3;
			break;
		case 4:
			off = 4;
			break;
		case 5:
			off = 5;
			break;
		case 6:
			off = 6;
			break;
		case 7:
			off = 7;
			break;
		case 8:
			off = 8;
			break;
		case 9:
			off = 9;
			break;
		case 10:
			off = 10;
			break;
		case 11:
			off = 11;
			break;
		case 12:
			off = 12;
			break;
		case 13:
			off = 13;
			break;
		case 14:
			off = 15;
			break;
		case 15:
			off = 17;
			break;
		case 16:
			off = 21;
			break;
		case 17:
			off = 26;
			break;
		case 18:
			off = 34;
			break;
		case 19:
			off = 51;
			break;
		default:
			off = 51;
			break;
	}
	return off;
}

void bodyHitCheck(Snake *snake){
	Snake *head = (Snake*)malloc(sizeof(Snake));	
	*head = *snake;
	snake = snake->nextSeg;
	while(snake != NULL){
		
		if (head->col == snake->col && head->row == snake->row){
			printf("WE'VE HIT OURSELF\n");
		}
		
		GameOn = (head->col == snake->col && head->row == snake->row) ? FALSE : GameOn;
		
		snake = snake->nextSeg;
	}
}

void wallHitCheck(Snake *snake){
	GameOn = (currentSnake->row > unitFieldSize-1) ? FALSE : GameOn;
	GameOn = (currentSnake->row < 0) 			   ? FALSE : GameOn;
	GameOn = (currentSnake->col > unitFieldSize-1) ? FALSE : GameOn;
	GameOn = (currentSnake->col < 0) 			   ? FALSE : GameOn;
}

void foodHitCheck(Snake *snake){
	int x, y;
	if (currentFood->col == snake->col && currentFood->row == snake->row){
			printf("WE'VE HIT A FOOD!\n");
			skipDeleteSegment = TRUE;
			generateRandCoords(&x, &y);
			currentFood->col = x;
			currentFood->row = y;
	}
}

void drawText(unsigned char* string, float x, float y, float scale){
	const char *c;
	
	float snakeUnit = 100 * 1/6;
	float xOffset = GAME_FIELD_X + GAME_FIELD_SIZE + snakeUnit*3;
	float yOffset = GAME_FIELD_Y + snakeUnit*3;
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);
	
	glPushMatrix();
    glTranslated(xOffset + x * snakeUnit , yOffset + y * snakeUnit, 0);
    glScaled(1 * textScale * scale, (-1) * textScale * scale, 0); 
	for (c = string; *c != '\0' ; c++){
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
	}
	glPopMatrix();
}

void drawControls(){
	
	int letterUnit = 3;

	glLineWidth(2);
	changeColor(255,255,255);
	
	changeColor(255,255,255);
	
	drawText("RESET", 	letterUnit*8.5, letterUnit*3, .4);
	drawText("|", 		letterUnit*9.1, letterUnit*3.8, .4);
	drawText("Y", 		letterUnit*9, letterUnit*4.6, .4);


	drawText("UP", 		letterUnit*5.75, letterUnit*3.4, .4);
	drawText("|", 		letterUnit*6.1, letterUnit*4.2, .4);
	drawText("W", 		letterUnit*6, letterUnit*5, .4);

	drawText("S", 		letterUnit*6, letterUnit*7, .4);
	drawText("|", 		letterUnit*6.1, letterUnit*7.8, .4);
	drawText("DOWN", 	letterUnit*5.5, letterUnit*8.6, .4);


	drawText("LEFT", 	letterUnit*2, letterUnit*6, .4);
	drawText("-", 		letterUnit*4, letterUnit*6, .4);
	drawText("A", 		letterUnit*5, letterUnit*6, .4);
	

	drawText("D", 		letterUnit*7, letterUnit*6, .4);
	drawText("-", 		letterUnit*8, letterUnit*6, .4);
	drawText("RIGHT", 	letterUnit*9, letterUnit*6, .4);
	
	drawText("SPEED", 	letterUnit*2, letterUnit*3, .3);
	
	drawText("[", 	letterUnit*1, letterUnit*3, .2);
	drawText("]", 	letterUnit*4, letterUnit*3, .2);
	
	drawText("DIFFICULTY", 	letterUnit*1.75, letterUnit*4, .3);
	
	drawText("-", 	letterUnit*1, letterUnit*4, .2);
	drawText("=", 	letterUnit*4, letterUnit*4, .2);
}

void drawDataField(){
	
	int letterUnit = 3;
	char *lengthTimeString, *timeString, *diffString, *speedString;
	
	lengthTimeString = 	(char*)malloc(7*sizeof(char));
	timeString = 	 	(char*)malloc(7*sizeof(char));
	diffString = 	 	(char*)malloc(7*sizeof(char));
	speedString = 	 	(char*)malloc(7*sizeof(char));
	
	ftoa(snakeLength(currentSnake), lengthTimeString, 	0);
	ftoa(gameTime, 					timeString, 		0);
	ftoa(diff, 						diffString, 		0);
	ftoa(gameSpeed, 				speedString, 		0);
	if (gameTime == 0) strcpy(timeString, "0");
	
	glLineWidth(2);
	changeColor(255,255,255);
	
	drawText("SNAKETALITOR", 	letterUnit*2,	letterUnit,			0.8);
	drawText("LENGTH: ", 		letterUnit*2,	letterUnit*10,		0.8);
	drawText(lengthTimeString, 	letterUnit*10,	letterUnit*10,		0.8);
	drawText("TIME: ", 			letterUnit*2,	letterUnit*11.4,	0.8);
	drawText(timeString, 		letterUnit*10,	letterUnit*11.4,	0.8);
	drawText("DIFFICULTY: ", 	letterUnit*2,	letterUnit*12.8,	0.8);
	drawText(diffString, 		letterUnit*10,	letterUnit*12.8,	0.8);
	drawText("SPEED: ", 		letterUnit*2,	letterUnit*14.2,	0.8);
	drawText(speedString, 		letterUnit*10,	letterUnit*14.2,	0.8);
	
	drawControls();
}

void display(){
	int width;
	int diffOffset;
	glClear(GL_COLOR_BUFFER_BIT);

	diffOffset = getDiffOffset();

	unitSize = (((WINDOW_X / 16) / (diffOffset)));
	unitFieldSize = (GAME_FIELD_SIZE/unitSize);
	//printf("%d		%d			%d\n", diff, unitSize, unitFieldSize);

	wallHitCheck(currentSnake);
	
	drawField();
	drawFood(currentFood);
	drawSnake(currentSnake);
	drawLoserSnake(currentSnake);
	
	drawDataField();
	
	if (!GameOn){
		//GameOver
	}
	
	glFlush();
	glutSwapBuffers();
}

void gameTimer(){
	gameTime += 1;
	glutPostRedisplay();
	glutTimerFunc(1000, gameTimer, 0);
}

void moveTimer(){
	
	if(GameOn == TRUE){
		createSegmentOf(&currentSnake);
		
		if (skipDeleteSegment != TRUE){
			deleteSnakeSegment(&currentSnake);
		}
		skipDeleteSegment = FALSE;
		
		switch(currentSnake->direction){

			case 'u':		//UP
				currentSnake->row--;
				break;
			case 'd':		//DOWN
				currentSnake->row++;
				break;
			case 'l':		//LEFT
				currentSnake->col--;
				break;
			case 'r':		//RIGHT
				currentSnake->col++;
				break;
		}
		
	}
	canChangeDirection = TRUE;
	if(disableBodyCheck == FALSE){
		bodyHitCheck(currentSnake);
	}
	disableBodyCheck = FALSE;
	foodHitCheck(currentSnake);
	
	
	glutPostRedisplay();
	glutTimerFunc(500/(diff*gameSpeed), moveTimer, 0);
}

void init(){
	glClearColor(12.0/255.0,35.0/255.0,42.0/255.0,0);
	gluOrtho2D(0,WINDOW_X,WINDOW_Y,0);
}

int main(int argc, char** argv){
	int i, x, y;
	srand(time(NULL));
	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(WINDOW_X,WINDOW_Y);
	glutCreateWindow("Mort's Game");
	init();
	
	
	currentFood = createFood();
	
	generateRandCoords(&x, &y);
	currentSnake = createSnake(x, y, 'r');							//create Snake head
	
	createSegmentOf(&currentSnake);											//create first segment, or not, I'm not your mom.

	currentSnake->col++;	//start movement to the right so that we don't hit ourselves
	
	glutDisplayFunc(display);
	glutKeyboardFunc(buttons);
	glutSpecialFunc(specialButtons);
	glutTimerFunc(1000, gameTimer, 0);
	glutTimerFunc(500/(diff*gameSpeed), moveTimer, 0);
	glutMainLoop();
	return 0;
}

