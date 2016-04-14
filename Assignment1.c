/*
Author : Andre Vidal
ID num : 620077449
Assignment : 1 Part 2
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <regex.h>

int ROW = 9;//global variable for number of rows in spreadsheet
int COL = 9;//global variable for number of columns in spreadsheet

//structure representing a cell
struct cell{
	char col;//column eg. A,B,C
	int row;//row eg. 1,2,3
	int type;//type of data in cell
	char text[100];//text of the cell
	int value;//int of cell
};
////////////////////////

//Function Declarations
struct cell* Initialize(struct cell grid[COL][ROW],struct cell *current,char input[100]);
void DisplayGrid(struct cell grid[COL][ROW]);
char * stringGrid(struct cell grid[COL][ROW]);
void DisplayMenu();
struct cell SelectCell(struct cell grid[COL][ROW]);
struct cell* FindCell(struct cell grid[COL][ROW],char column,int row);
void AcceptInput(struct cell grid[COL][ROW],char column,int row,char input[100]);
void TestInput(struct cell grid[COL][ROW],char column,int row,char input[100]);
int Calculate(struct cell grid[COL][ROW],char column,int row,char input[100]);
void SetContent(struct cell grid[COL][ROW],char column,int row,int type,int val,char content[100]);
////////////////////////

/*
main(int argc, char *argv[]){
	struct cell grid[COL][ROW],currCell;
	int choice=0,col,row;
	char input[100],temp[10];
	
	Initialize(grid,&currCell,input);

	do{
		col = ((int)currCell.col)-65;//current column in grid
		row = currCell.row-1;//current row in grid

		system("clear");

		//display the current cell and its contents
		printf("\nCURRENT CELL: \t%c%d\nCONTENT : \t",currCell.col,currCell.row);

		if(grid[col][row].type == 0){printf("%s",grid[col][row].text);}
		if(grid[col][row].type == 1){printf("%d",grid[col][row].value);}

		DisplayGrid(grid);
		DisplayMenu();
		scanf("%s",temp);

		choice = (int)strtol(temp,(char**)NULL,10);
		switch(choice){
			case 1: currCell = SelectCell(grid);break;
			case 2: AcceptInput(grid,currCell.col,currCell.row,input);
						 break;
			case 3: break;
			default : break;		

		}
	}while(choice!=3);
}
*/

struct cell* Initialize(struct cell grid[COL][ROW], struct cell *current, char input[100]){
	int i,j;
	//asign the values of each cell's respective position in the grid
	for(i=0;i<COL;i++){
		for(j=0;j<ROW;j++){
			grid[i][j].col = ((char)65)+i;
			grid[i][j].row = j+1;
			grid[i][j].type = 0;
			strcpy(grid[i][j].text,"\0");//set text to null
			grid[i][j].value = 0.0;//set int value to 0		
		}	
	}
	//set default cell to first in the sheet
	current->col = 'A';
	current->row = 1;
	current->type = 0;
	strcpy(current->text,"\0");
	current->value = 0.0;

	//set initial input to null
	strcpy(input," ");	

	return *grid;
}

void DisplayGrid(struct cell grid[COL][ROW]){
	int i,j;
	
	//display the spreadsheet
	printf("\n\n    A       B       C       D       E       F       G       H       I   \n");
	printf(" _______ _______ _______ _______ _______ _______ _______ _______ _______\n");
	for(i=0;i<COL;i++){
		printf("|\t|\t|\t|\t|\t|\t|\t|\t|\t|\n");
		for(j=0;j<ROW;j++){
			if(grid[j][i].type == 0){
				if(strlen(grid[j][i].text)>6)
					printf("|%.3s...\t",grid[j][i].text);
				else
					printf("|%.6s\t",grid[j][i].text);
			}
			if(grid[j][i].type == 1)
				printf("|%3.2d\t",grid[j][i].value);
		}
		printf("|   %d\n|_______|_______|_______|_______|_______|_______|_______|_______|_______|\n",i+1);
	}	
}

char * stringGrid(struct cell grid[COL][ROW]){
	int i,j;
	char temp[100];
	char *table = (char *) malloc(2000);

	strcpy(table,"\n    A       B       C       D       E       F       G       H       I   \n");
	strcat(table," _______ _______ _______ _______ _______ _______ _______ _______ _______\n");
	for(i=0;i<COL;i++){
		strcat(table,"|\t|\t|\t|\t|\t|\t|\t|\t|\t|\n");
		for(j=0;j<ROW;j++){
			if(grid[j][i].type == 0){
				if(strlen(grid[j][i].text)>6)
					sprintf(table,"%s|%.3s...\t",table,grid[j][i].text);
				else
					sprintf(table,"%s|%.6s\t",table,grid[j][i].text);
			}
			if(grid[j][i].type == 1){
				if(grid[j][i].value > 99999)
					sprintf(table,"%s| #####\t",table);
				else
					sprintf(table,"%s|%.2d\t",table,grid[j][i].value);
			}
		}
		sprintf(table,"%s|   %d\n|_______|_______|_______|_______|_______|_______|_______|_______|_______|\n",table,i+1);
	
	}	
	return table;
}

void DisplayMenu(){
	printf("\n[1] Select Cell");
	printf("\n[2] Input Data (Note: Formulas are of form SUM,AVG or RNG)");
	printf("\n[3] Save");
	printf("\nChoice: ");
}
/*
struct cell SelectCell(struct cell grid[COL][ROW]){
	struct cell temp;
	char cellname[2];

	printf("\nChoose a cell: ");//prompt for a cell by column and row
	scanf("%s",cellname);
	return *FindCell(grid,toupper(cellname[0]),(int)cellname[1]-48);
	//find and return the cell with the corresponding row and column
}
*/

struct cell* FindCell(struct cell grid[COL][ROW],char column,int row){
	int i,j;
	struct cell temp;

	for(i=0;i<COL;i++){
		for(j=0;j<ROW;j++){
			if(grid[i][j].col == column && grid[i][j].row == row)//if the row and column match the search key
				return &grid[i][j];
		}
	}
	//return &temp;
}

void AcceptInput(struct cell grid[COL][ROW],char column,int row,char input[100]){

	printf("Input Data: ");
	scanf("%s",input);//Accept input here
	TestInput(grid,column,row,input);//have function to check for the type of input entered
}

void TestInput(struct cell grid[COL][ROW],char column,int row,char input[100]){
	int answer = 0;
	int col;
	regex_t regex;
	int regexint;
	col = ((int)column)-65;

	regexint = regcomp(&regex, "[0-9]",0);//CURRENTLY ONLY ACCEPTS INTEGERS
	regexint = regexec(&regex,input,0,NULL,0);
	
	if(input[0] == '=')//function starts with '='
	{
		answer = Calculate(grid,column,row,input);

		SetContent(grid,column,row,1,answer,"\0");
	}
	else if(!regexint){//if content is int
		answer = (int)strtol(input,(char**)NULL,10);
		SetContent(grid,column,row,1,answer,"\0");	
	}
	else{ //if content is string/text
		SetContent(grid,column,row,0,answer,input);
	}
			
}

int Calculate(struct cell grid[COL][ROW],char column,int row,char input[100]){
	
	char opt[3];
	int col,i;
	int answer=0,div=1,largest,smallest;
	strcpy(opt,"\0");
	col = ((int)column)-65;	
	
	//SUM FORMULA CALCULATION
	if((input[1] == 's' || input[1] == 'S') && (input[2] == 'U' || input[2] == 'u') && (input[3] =='M' || input[3] =='m')){
		if(input[5] == input[8]){
			for(i=((int)input[6]-49);i<=((int)input[9]-49);i++){
				answer += grid[col][i].value;
			}	
		}
		if(input[6] == input[9]){
			for(i=((int)input[5]-65);i<=((int)input[8]-65);i++){
				answer += grid[i][row-1].value;
			}	
		}
		return answer;
	}
	//AVERAGE FORMULA CALCULATION
	if((input[1] == 'a' || input[1] == 'A') && (input[2] == 'v' || input[2] == 'V') && (input[3] =='g' || input[3] =='G')){
		if(input[5] == input[8]){
			for(i=((int)input[6]-49);i<=((int)input[9]-49);i++){
				answer += grid[col][i].value;
			}
			div = ((int)input[9]-49)+1;	
		}
		if(input[6] == input[9]){
			for(i=((int)input[5]-65);i<=((int)input[8]-65);i++){
				answer += grid[i][row-1].value;
			}	
			div = ((int)input[8]-65)+1;
		}
		return (answer/div);	
	}
	//RANGE FORMULA CALCULATION
	if((input[1] == 'r' || input[1] == 'R') && (input[2] == 'n' || input[2] == 'N') && (input[3] =='g' || input[3] =='G')){
		if(input[5] == input[8]){
			largest = grid[col][((int)input[6]-49)].value;
			smallest = grid[col][((int)input[6]-49)].value;
			for(i=((int)input[6]-49);i<=((int)input[9]-49);i++){
				if(largest < grid[col][i].value)
					largest = grid[col][i].value;
				if(smallest > grid[col][i].value)
					smallest = grid[col][i].value;
			}	
		}
		if(input[6] == input[9]){
			largest = grid[((int)input[5]-65)][row-1].value;
			smallest = grid[((int)input[5]-65)][row-1].value;
			for(i=((int)input[5]-65);i<=((int)input[8]-65);i++){
				if(largest < grid[i][row-1].value)
					largest = grid[i][row-1].value;
				if(smallest > grid[i][row-1].value)
					smallest = grid[i][row-1].value;
			}	
		}
		return largest-smallest;
	}
}

void SetContent(struct cell grid[COL][ROW],char column,int row,int type,int val,char input[100]){
	int col;
	col = ((int)column)-65;
	
	grid[col][row-1].type = type;//set new data type of cell
	strcpy(grid[col][row-1].text,input);
	grid[col][row-1].value = val;
}

void saveWorksheet(struct cell grid[COL][ROW]){
	int i,j;
	FILE *fp;
	char filename[] = "test.txt";
	fp=fopen(filename,"w");

	if(fp != NULL){
		fwrite(grid,sizeof(struct cell),COL*ROW,fp);
	}	

	fclose(fp);
}

void openWorksheet(struct cell grid[COL][ROW]){
	int i,j,blah;
	FILE *fp;
	char filename[] = "test.txt";
	fp=fopen(filename,"r");

	if(fp != NULL){
		fread(grid,sizeof(struct cell),COL*ROW,fp);
	}

	fclose(fp);
}