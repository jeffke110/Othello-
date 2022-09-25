//============================================================================
// Name        : Othello.cpp
// Author      : Jeffrey Kedda
// TA		   : Lauren Olson
// Date        : 2-26-20
// Copyright   : Your copyright notice
// Description : Othello the game
//============================================================================


#include <iostream>
#include <time.h>
#include <math.h>
#include <stdlib.h>
using namespace std;

struct GameBoard {
	char **board; // the othello board
	int size; //for the size of the board (it's a square, so size is one dimension)
	char p; //for current player ('X' or 'O')
	int totalct; // for number of used squares (so that when the board is full, the game stops)
};

void selectionSort(char *arr, int len, int outer);
int Innersort(char *arr, int len, int inner, int placement);

void getSize(int &size);
void makeBoard(GameBoard *game, int ct, bool flag);
void printBoard(GameBoard *game, int ct);
void makeSpace(GameBoard *game, int inner);
void makerow(GameBoard *game, int inner, int outer);
void printrow(GameBoard *game, int inner, int outer);
void printStyleRow(GameBoard *game,int ct);

bool compplacepiece(GameBoard * game);
void findbestspot(GameBoard *game, int ct, int * BestArray, int &Temp);
void findbestspotRow(GameBoard *game, int inner, int outer,  int * BestArray, int &Temp);
int checkAround(int i, int j, GameBoard *game);

int checkTopLeft(int row, int column, GameBoard *game, char flipPiece, char endPiece, int &counter);
int checkTopMiddle(int row, int column, GameBoard *game, char flipPiece, char endPiece, int &topLeftcount);
int checkTopRight(int row, int column, GameBoard *game, char flipPiece, char endPiece,  int &counter);
int checkLeft(int row, int column, GameBoard *game, char flipPiece, char endPiece,  int &counter);
int checkRight(int row, int column, GameBoard *game, char flipPiece, char endPiece,  int &counter);
int checkBottomLeft(int row, int column, GameBoard *game, char flipPiece, char endPiece, int &counter);
int checkBottomMiddle(int row, int column, GameBoard *game, char flipPiece, char endPiece,  int &counter);
int checkBottomRight(int row, int column, GameBoard *game, char flipPiece, char endPiece,  int &counter);

void flipTopLeft(int row, int column, GameBoard *game, char flipPiece, char endPiece);
void flipTopMiddle(int row, int column, GameBoard *game, char flipPiece, char endPiece);
void flipTopRight(int row, int column, GameBoard *game, char flipPiece, char endPiece);
void flipLeft(int row, int column, GameBoard *game, char flipPiece, char endPiece);
void flipRight(int row, int column, GameBoard *game, char flipPiece, char endPiece);
void flipBottemLeft(int row, int column, GameBoard *game, char flipPiece, char endPiece);
void flipBottemMiddle(int row, int column, GameBoard *game, char flipPiece, char endPiece);
void flipBottomRight(int row, int column, GameBoard *game, char flipPiece, char endPiece);

void startGame(GameBoard *game);
void playGame(bool fp1,bool fp2,GameBoard *game,bool whoplaysfirstflag);

char ckwin(GameBoard *game) ;
void ckwinRow(GameBoard *game, int inner, int outer, int &X, int &O);
char chkwin2(GameBoard *game, int ct, int &X, int &O);

int main() {
	char one[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g'};
	char two[] = {'g', 'f', 'e', 'd', 'c', 'b', 'a'}; //backwards
	char three[] = {'g'};							  //one
	char four[] = {'g', 'b', 'a', 'f', 'e', 'c', 'd'};//random

	cout << "Problem 0: (test cases)" << endl;
	selectionSort(one,  7, 0);
	selectionSort(two,  7, 0);
	selectionSort(three, 1, 0);
	selectionSort(four , 7, 0);

	for(int i = 0; i < 7; i++){
		cout << one[i];
	}
		cout << endl;
	for(int i = 0; i < 7; i++){
		cout << two[i];
	}
	cout << endl;
	for(int i = 0; i < 1; i++){
		cout << three[i];
	}
	cout << endl;
	for(int i = 0; i < 7; i++){
		cout << four[i];
	}
	cout << endl;

	cout << "OTHELLO --------------------------------------------------------" << endl;

	srand(time(NULL));
	int size = 0;
	getSize(size); // gets the size for the game board. Note we only do this once for this game.
	GameBoard *game = new GameBoard; // places a gameBoard struct object on the heap.
	game->size = size; //sets the gameboard's size
	game->totalct = 4; // the 4 middle squares are taken
	cout << "Size is " << game->size << endl;
	makeBoard(game,0,true);
	printBoard(game, 0); // for testing purposes

	startGame(game);
	return 0;
}



//---------------------------------------------------------------------------------------------------------------------------------------------------

/*
NAME			 : RESelection (Problem 1)
INPUT PARAMETERS : char *arr, int len, int outer
OUTPUT PARAMETERS: nothing
DESCRIPTION      : This is for Problem 1, it recusively does insertion sort
*/
void selectionSort(char *arr, int len, int outer){
	if(outer < len ){
		//cout << outer << endl;
		int placement = outer;
		placement = Innersort(arr, len, outer + 1, placement);
		char tmp = arr[placement];
		arr[placement] = arr[outer];
		arr[outer] = tmp;
		outer++;
		selectionSort(arr, len, outer);
	}

}
/*
NAME			 : Innersort (Problem 1)
INPUT PARAMETERS : char *arr, int len, int inner, int placement
OUTPUT PARAMETERS: int
DESCRIPTION      : This is for Problem 1, it recusively does insertion sort for each row
*/
int Innersort(char *arr, int len, int inner, int placement){
	if(inner == len){
		return placement;
	}else{
		//cout << inner << endl;
		if(arr[inner] < arr[placement]){
			placement = inner;
		}
		inner++;
		return Innersort(arr, len, inner , placement);
	}

}


//---------------------------------------------------------------------------------------------------------------------------------------------------
//OTHELLO

/*
NAME			 : makeboard
INPUT PARAMETERS : GameBoard *game, int ct, bool flag
OUTPUT PARAMETERS: nothing
DESCRIPTION      : creates the board for othello, adds O,X to the four middle spots
*/
void  makeBoard(GameBoard *game, int ct, bool flag){
	int length = game->size;
	if(flag == true){
		game->board = new char*[length]; //allocating for each row
		makeSpace(game, 0);
	}
	if(ct < length ){
		makerow(game, 0 , ct);
		ct++;
		makeBoard(game, ct,false);
	}
	//finds the middle four spots
	int topLeft[] = {length/2 - 1, length/2 - 1};
	int topRight[] = {length/2 - 1, length/2};
	int bottomeLeft[] = {length/2, length/2 - 1};
	int bottomRight[] = {length/2, length/2};

	game->board[topLeft[0]][topLeft[1]] = 'X';
	game->board[topRight[0]][topRight[1]] = 'O';
	game->board[bottomeLeft[0]][bottomeLeft[1]] = 'O';
	game->board[bottomRight[0]][bottomRight[1]] = 'X';
}


/*
NAME			 : makeSpace (HELPER)
INPUT PARAMETERS : GameBoard *game, int inner
OUTPUT PARAMETERS: nothing
DESCRIPTION      : creates the board rows on the heap for othello
*/
void makeSpace(GameBoard *game, int inner){
	int length = game->size;
	if(inner == length){
	}else{
		game->board[inner] = new char[length];
		inner++;
		makeSpace(game, inner);
	}
}


/*
NAME			 : makerow (HELPER)
INPUT PARAMETERS : GameBoard *game, int inner, int outer
OUTPUT PARAMETERS: nothing
DESCRIPTION      : assigns each square '-' on the othello board
*/
void makerow(GameBoard *game, int inner, int outer){
	int length = game->size;
	if(inner == length){
	}else{
		game->board[outer][inner] = '-';
		inner++;
		makerow(game, inner, outer);
	}
}

/*
NAME			 : getSize
INPUT PARAMETERS : int &size
OUTPUT PARAMETERS: nothing
DESCRIPTION      : Pass by reference, changes int to a random number between between 7 and 20 (20 not included)
Didn't know the range so made it 7 to 20 because any smaller woudln't make sense to me.
*/
void getSize(int &size){
	srand(time(NULL));
	size = rand()%13 + 7;
	if(size%2 != 0){
		size++; //adds one if it is odd
	}
}


////// CHKWIN IS AT THE BOTTEM!!!! //////////////
////// CHKWIN IS AT THE BOTTEM!!!! //////////////
////// CHKWIN IS AT THE BOTTEM!!!! //////////////
////// CHKWIN IS AT THE BOTTEM!!!! //////////////

/*
NAME			 : printBoard
INPUT PARAMETERS : GameBoard *game, int ct
OUTPUT PARAMETERS: nothing
DESCRIPTION      : Prints the board
*/
void printBoard(GameBoard *game, int ct){
	int length = game->size;
	if(ct == 0 ){
		cout << "\t";
		printStyleRow(game, 0);
		cout << endl;
	}
	if(ct < length ){
		cout << ct << "\t"; //left most column for debugging
		printrow(game, 0, ct);
		cout << endl;
		ct++;
		printBoard(game, ct);
	}
}

/*
NAME			 : printStyleRow  (HELPER)
INPUT PARAMETERS : GameBoard *game, int ct
OUTPUT PARAMETERS: nothing
DESCRIPTION      : Prints the top row of the board that helps debugging
*/
void printStyleRow(GameBoard *game,int ct){
	if(ct < game->size ){
		cout << ct << "\t"; //top most row for debugging
		ct++;
		printStyleRow(game,ct);
	}

}


/*
NAME			 : printrow  (HELPER)
INPUT PARAMETERS : GameBoard *game, int inner, int outer
OUTPUT PARAMETERS: nothing
DESCRIPTION      : Prints each element of the row
*/
void printrow(GameBoard *game, int inner, int outer){
	int length = game->size;
		if(inner == length){
		}else{
			cout << game->board[outer][inner] << "\t";
			inner++;
			printrow(game, inner, outer);
		}
}


/*
NAME			 : placepieceperson
INPUT PARAMETERS : GameBoard *game
OUTPUT PARAMETERS: boolean
DESCRIPTION      : Takes in coordinates from the player, Checks whether the choice legal or not,
then flips pieces, returns false if there are no more spaces or a wrong space picked. If the player picks a correct coordinate,
returns true;
*/
bool placepieceperson(GameBoard *game){
	cout << "Player with: " << game->p << endl;
	char playerPiece = game->p;
	char flipPiece;
	char endPiece;
	if(playerPiece == 'X'){
		flipPiece = 'O';
		endPiece = 'X';
	}else{
		flipPiece = 'X';
		endPiece = 'O';
	}

	//-----------------------------------------------------------------------
	//choosing square
	int x, y;
	cout << "Choose x coordinate (between 0 and " << game->size << + ") : " << endl;
	cin >> x;
	if(x > game->size || x < 0){ //checks the bounds
		cout << "(Out of Bounds)" << endl;
		return false;
	}
	cout << "Choose y coordinate (between 0 and " << game->size << + ") : " << endl;
	cin >> y;
	if(y > game->size  || y < 0){
		cout << "(Out of Bounds)" << endl;
		return false;
	}
	//-----------------------------------------------------------------------
	//checks whether spot is legal
	int BestArray;
	BestArray = checkAround(x, y, game);
	//-----------------------------------------------------------------------
	//if there are 0 pieces to flip, return false
	if(BestArray == 0){
		cout << "(Spot Can't Be Chosen)" << endl;
		return false;

	}else{
	//-----------------------------------------------------------------------
	//Flip
		int TopLeft = 0;
		int TopMiddle = 0;
		int TopRight = 0;
		int Left = 0;
		int Right = 0;
		int BottemLeft = 0;
		int Bottem = 0;
		int BottemRight = 0;
		checkTopLeft(x, y, game, flipPiece, endPiece, TopLeft);
		checkTopMiddle(x, y, game, flipPiece, endPiece, TopMiddle);
		checkTopRight(x, y, game, flipPiece, endPiece, TopRight);
		checkLeft(x, y, game, flipPiece, endPiece, Left);
		checkRight(x, y, game, flipPiece, endPiece, Right);
		checkBottomLeft(x, y, game, flipPiece, endPiece, BottemLeft);
		checkBottomMiddle(x, y, game, flipPiece, endPiece, Bottem);
		checkBottomRight(x, y, game, flipPiece, endPiece, BottemRight);
		if(TopLeft > 0){
			flipTopLeft(x, y,  game,  flipPiece,  endPiece);
		}
		if(TopMiddle > 0){
			flipTopMiddle(x, y,  game,  flipPiece,  endPiece);
		}
		if(TopRight> 0){
			flipTopRight(x, y,  game,  flipPiece,  endPiece);
		}
		if(Left > 0){
			flipLeft(x, y,  game,  flipPiece,  endPiece);
		}
		if( Right> 0){
			flipRight(x, y,  game,  flipPiece,  endPiece);
		}
		if(BottemLeft > 0){
			flipBottemLeft(x, y,  game,  flipPiece,  endPiece);
		}
		if(Bottem > 0){
			flipBottemMiddle(x, y,  game,  flipPiece,  endPiece);
		}
		if(BottemRight > 0){
			flipBottomRight(x, y,  game,  flipPiece,  endPiece);
		}

		return true;
	}
}


/*
NAME			 : compplacepiece
INPUT PARAMETERS : GameBoard *game
OUTPUT PARAMETERS: boolean
DESCRIPTION      : The computer finds the spot that flips the most pieces, returns true if it can, else false if it
can't find a legal spot
*/
bool compplacepiece(GameBoard * game){
	int * BestArray = new int[3];
	char playerPiece = game->p;
	char flipPiece;
	char endPiece;
	if(playerPiece == 'X'){
		flipPiece = 'O';
		endPiece = 'X';
	}else{
		flipPiece = 'X';
		endPiece = 'O';
	}
	//-----------------------------------------------------------------------
	//Finds best spot
	int Temp = 0;
	BestArray[0] = 0;
	BestArray[1] = 0;
	BestArray[2] = 0;
	findbestspot(game, 0, BestArray, Temp);
	//-----------------------------------------------------------------------
	if(BestArray[0] == 0){
		cout << "(Spot Can't Be Chosen)" << endl;
		delete[] BestArray;
		return false;
	}else{
	//-----------------------------------------------------------------------
		cout << "COMP " << playerPiece << " chose: ( "<< BestArray[1] << ", "<< BestArray[2] << " ) "<<  endl;
		//Flip
		int TopLeft = 0;
		int TopMiddle = 0;
		int TopRight = 0;
		int Left = 0;
		int Right = 0;
		int BottemLeft = 0;
		int Bottem = 0;
		int BottemRight = 0;
		checkTopLeft(BestArray[1], BestArray[2], game, flipPiece, endPiece, TopLeft);
		checkTopMiddle(BestArray[1], BestArray[2], game, flipPiece, endPiece, TopMiddle);
		checkTopRight(BestArray[1], BestArray[2], game, flipPiece, endPiece, TopRight);
		checkLeft(BestArray[1], BestArray[2], game, flipPiece, endPiece, Left);
		checkRight(BestArray[1], BestArray[2], game, flipPiece, endPiece, Right);
		checkBottomLeft(BestArray[1], BestArray[2], game, flipPiece, endPiece, BottemLeft);
		checkBottomMiddle(BestArray[1], BestArray[2], game, flipPiece, endPiece, Bottem);
		checkBottomRight(BestArray[1], BestArray[2], game, flipPiece, endPiece, BottemRight);
		if(TopLeft > 0){
			flipTopLeft(BestArray[1], BestArray[2],  game,  flipPiece,  endPiece);
		}
		if(TopMiddle > 0){
			flipTopMiddle(BestArray[1], BestArray[2],  game,  flipPiece,  endPiece);
		}
		if(TopRight> 0){
			flipTopRight(BestArray[1], BestArray[2],  game,  flipPiece,  endPiece);
		}
		if(Left > 0){
			flipLeft(BestArray[1], BestArray[2],  game,  flipPiece,  endPiece);
		}
		if(Right > 0){
			flipRight(BestArray[1], BestArray[2],  game,  flipPiece,  endPiece);
		}
		if(BottemLeft > 0){
			flipBottemLeft(BestArray[1], BestArray[2],  game,  flipPiece,  endPiece);
		}
		if(Bottem > 0){
			flipBottemMiddle(BestArray[1], BestArray[2],  game,  flipPiece,  endPiece);
		}
		if(BottemRight > 0){
			flipBottomRight(BestArray[1], BestArray[2],  game,  flipPiece,  endPiece);
		}
		delete[] BestArray;
		return true;
	}
}

/*
NAME			 : findbestspot (HELPER)
INPUT PARAMETERS : GameBoard *game, int ct, int * BestArray, int &Temp
OUTPUT PARAMETERS: nothing
DESCRIPTION      : finds the best spot
*/
void findbestspot(GameBoard *game, int ct, int * BestArray, int &Temp){
	int length = game->size;
	if (ct < length) {
		findbestspotRow(game, 0, ct, BestArray, Temp);
		ct++;
		findbestspot(game, ct, BestArray, Temp);
	}
}

/*
NAME			 : findbestspotRow (HELPER)
INPUT PARAMETERS : GameBoard *game, int inner, int outer,  int * BestArray, int &Temp
OUTPUT PARAMETERS: nothing
DESCRIPTION      : finds the best spot this goes through each element and compares to the best
*/
void findbestspotRow(GameBoard *game, int inner, int outer,  int * BestArray, int &Temp){
	int length = game->size;
	if (inner == length) {
	} else {
		if (game->board[outer][inner] == '-') { //doesn't check already picked spots
			Temp = checkAround(outer, inner, game);
			if (Temp > BestArray[0]) {
				BestArray[0] = Temp;
				BestArray[1] = outer;
				BestArray[2] = inner;
				cout << "COMP " << game->p << " updated best: ( "<< BestArray[1] << ", "<< BestArray[2] << " ) "<<  endl;
			}
		}
		inner++;
		findbestspotRow(game, inner, outer, BestArray, Temp);
	}
}


/*
NAME			 : checkAround (HELPER)
INPUT PARAMETERS : int i, int j, GameBoard *game
OUTPUT PARAMETERS: nothing
DESCRIPTION      : checks around each spot around a element
*/
int checkAround(int i, int j, GameBoard *game){
	char playerPiece = game->p;
	char flipPiece;
	char endPiece;
	int counter = 0;
	if(playerPiece == 'X'){
		flipPiece = 'O';
		endPiece = 'X';
	}else{
		flipPiece = 'X';
		endPiece = 'O';
	}
	int TopLeft = 0;
	int TopMiddle = 0;
	int TopRight = 0;
	int Left = 0;
	int Right = 0;
	int BottemLeft = 0;
	int Bottem = 0;
	int BottemRight = 0;
	checkTopLeft(i, j, game, flipPiece, endPiece, TopLeft);
	checkTopMiddle(i, j, game, flipPiece, endPiece, TopMiddle);
	checkTopRight(i, j, game, flipPiece, endPiece, TopRight);
	checkLeft(i, j, game, flipPiece, endPiece, Left);
	checkRight(i, j, game, flipPiece, endPiece, Right);
	checkBottomLeft(i, j, game, flipPiece, endPiece, BottemLeft);
	checkBottomMiddle(i, j, game, flipPiece, endPiece, Bottem);
	checkBottomRight(i, j, game, flipPiece, endPiece, BottemRight);
	counter = TopLeft + TopMiddle +  TopRight + Left +  Right  + BottemLeft  + Bottem + BottemRight;
	return counter;
}

/*
NAME			 : checkTopLeft (HELPER)
INPUT PARAMETERS : int row, int column, GameBoard *game, char flipPiece, char endPiece, int &counter
OUTPUT PARAMETERS: nothing
DESCRIPTION      : checks Top left of the element
*/
int checkTopLeft(int row, int column, GameBoard *game, char flipPiece, char endPiece, int &counter){ //direction 1	//------------------------------------------------------
	row--;
	column--;
	//------------------------------------------------------
	if(row >= 0 && column >= 0  && game->board[row][column] == flipPiece){ //top left
		counter++;
		checkTopLeft(row, column, game, flipPiece, endPiece, counter);
	//------------------------------------------------------
	}else if(row < 0 || column < 0){
		counter = 0;
	}else if(game->board[row][column] != endPiece){
		counter = 0;
	}
	return counter;

}

/*
NAME			 : checkTopMiddle (HELPER)
INPUT PARAMETERS : int row, int column, GameBoard *game, char flipPiece, char endPiece, int &counter
OUTPUT PARAMETERS: nothing
DESCRIPTION      : checks Top Middle of the element
*/
int checkTopMiddle(int row, int column,  GameBoard *game, char flipPiece, char endPiece,int &counter){ //direction 2
	//------------------------------------------------------
	row--;
	//------------------------------------------------------
	if(row >= 0 && column >= 0 && game->board[row][column] == flipPiece){ //top left
		counter++;
		checkTopMiddle(row, column, game, flipPiece, endPiece, counter);
	//------------------------------------------------------
	}else if(row < 0 || column < 0){
		counter = 0;
	}else if(game->board[row][column] != endPiece){
		counter = 0;
	}
	return counter;
}

/*
NAME			 : checkTopRight (HELPER)
INPUT PARAMETERS : int row, int column, GameBoard *game, char flipPiece, char endPiece, int &counter
OUTPUT PARAMETERS: nothing
DESCRIPTION      : checks Top Right of the element
*/
int checkTopRight(int row, int column,  GameBoard *game, char flipPiece, char endPiece, int &counter){ //direction 3
	//------------------------------------------------------
	row--;
	column++;
	//------------------------------------------------------
	if(row >= 0 && column < game->size && game->board[row][column] == flipPiece){ //top right //Change
		counter++;
		checkTopRight(row, column, game, flipPiece, endPiece, counter); //Change
	//------------------------------------------------------
	}else if(row < 0 || column > game->size){ //change
		counter = 0;
	}else if(game->board[row][column] != endPiece){
		counter = 0;
	}
	return counter;
}

/*
NAME			 : checkLeft (HELPER)
INPUT PARAMETERS : int row, int column, GameBoard *game, char flipPiece, char endPiece, int &counter
OUTPUT PARAMETERS: nothing
DESCRIPTION      : checks left of the element
*/
int checkLeft(int row, int column,  GameBoard *game, char flipPiece, char endPiece, int &counter){ //direction 4
	//------------------------------------------------------
	column--;
	//------------------------------------------------------
	if(column >= 0  && game->board[row][column] == flipPiece){ //top right //Change
		counter++;
		checkLeft(row, column, game, flipPiece, endPiece, counter); //Change
	//------------------------------------------------------
	}else if(column < 0){ //change
		counter = 0;
	}else if(game->board[row][column] != endPiece){
		counter = 0;
	}
	return counter;
}

/*
NAME			 : checkRight (HELPER)
INPUT PARAMETERS : int row, int column, GameBoard *game, char flipPiece, char endPiece, int &counter
OUTPUT PARAMETERS: nothing
DESCRIPTION      : checks Right of the element
*/
int checkRight(int row, int column, GameBoard *game, char flipPiece, char endPiece, int &counter){ //direction 5
	//------------------------------------------------------
	column++;//change
	//------------------------------------------------------
	if(column < game->size  && game->board[row][column] == flipPiece){ //top right //Change
		counter++;
		checkRight(row, column, game, flipPiece, endPiece, counter); //Change
	//------------------------------------------------------
	}else if(column >= game->size){ //change
		counter = 0;
	}else if(game->board[row][column] != endPiece){
		counter = 0;
	}
	return counter;
}


/*
NAME			 : checkBottomLeft (HELPER)
INPUT PARAMETERS : int row, int column, GameBoard *game, char flipPiece, char endPiece, int &counter
OUTPUT PARAMETERS: nothing
DESCRIPTION      : checks BottemLeft of the element
*/
int checkBottomLeft(int row, int column, GameBoard *game, char flipPiece, char endPiece,  int &counter){ //direction 6
	//i + 1 < length && j - 1 >= 0
	//------------------------------------------------------
	row++;
	column--;//change
	//------------------------------------------------------
	if(row < game->size && column >= 0 && game->board[row][column] == flipPiece){ //top right //Change
		counter++;
		checkBottomLeft(row, column, game, flipPiece, endPiece, counter); //Change
	//------------------------------------------------------
	}else if(row > game->size - 1|| column < 0){ //change
		counter = 0;
	}else if(game->board[row][column] != endPiece){
		counter = 0;
	}
	return counter;
}

/*
NAME			 : checkBottemMiddle (HELPER)
INPUT PARAMETERS : int row, int column, GameBoard *game, char flipPiece, char endPiece, int &counter
OUTPUT PARAMETERS: nothing
DESCRIPTION      : checks BottemMiddle of the element
*/
int checkBottomMiddle(int row, int column, GameBoard *game, char flipPiece, char endPiece, int &counter){ //direction 7
	//i + 1 < length && j >= 0
	//------------------------------------------------------
	row++;
	//------------------------------------------------------
	if(row < game->size && game->board[row][column] == flipPiece){ //top right //Change
		counter++;
		checkBottomMiddle(row, column, game, flipPiece, endPiece, counter); //Change
	//------------------------------------------------------
	}else if(row >= game->size){ //change
		counter = 0;
	}else if(game->board[row][column] != endPiece){
		counter = 0;
	}
	return counter;
}


/*
NAME			 : checkBottomRight (HELPER)
INPUT PARAMETERS : int row, int column, GameBoard *game, char flipPiece, char endPiece, int &counter
OUTPUT PARAMETERS: nothing
DESCRIPTION      : checks Bottem Right of the element
*/
int checkBottomRight(int row, int column, GameBoard *game, char flipPiece, char endPiece, int &counter){ //direction 8
	//i + 1 < length && j + 1 < length
	//------------------------------------------------------
	row++;
	column++;
	//------------------------------------------------------
	if(row < game->size  && column < game->size  && game->board[row][column] == flipPiece){ //top right //Change
		counter++;
		checkBottomRight(row, column, game, flipPiece, endPiece, counter); //Change
	//------------------------------------------------------
	}else if(row > game->size -1  || column > game->size - 1){ //change
		counter = 0;
	}else if(game->board[row][column] != endPiece){
		counter = 0;
	}
	return counter;
}


/*
NAME			 : flipTopLeft (HELPER)
INPUT PARAMETERS : int row, int column, GameBoard *game, char flipPiece, char endPiece
OUTPUT PARAMETERS: nothing
DESCRIPTION      : flips Top Left of the element
*/
void flipTopLeft(int row, int column, GameBoard *game, char flipPiece, char endPiece){ //direction 1
	game->board[row][column] = endPiece;
	row--;
	column--;
	//------------------------------------------------------
	if(row >= 0 && column >= 0 && game->board[row][column] == flipPiece){ //top lef
		game->board[row][column] = endPiece;
		cout << "changing ( " << row  << ", " << column <<" ) to "<< endPiece << endl;
		flipTopLeft(row, column, game, flipPiece, endPiece);
	//------------------------------------------------------
	}
}

/*
NAME			 : flipTopMiddle (HELPER)
INPUT PARAMETERS : int row, int column, GameBoard *game, char flipPiece, char endPiece
OUTPUT PARAMETERS: nothing
DESCRIPTION      : flips Top Middle of the element
*/
void flipTopMiddle(int row, int column, GameBoard *game, char flipPiece, char endPiece){ //direction 2
	game->board[row][column] = endPiece;
	//------------------------------------------------------
	row--;
	//------------------------------------------------------
	if(row >= 0 && column >= 0 && game->board[row][column] == flipPiece){ //top left
		game->board[row][column] = endPiece;
		cout << "changing ( " << row  << ", " << column <<" ) to "<< endPiece << endl;
		flipTopMiddle(row, column, game, flipPiece, endPiece);
	//------------------------------------------------------
	}
}


/*
NAME			 : flipTopRight (HELPER)
INPUT PARAMETERS : int row, int column, GameBoard *game, char flipPiece, char endPiece
OUTPUT PARAMETERS: nothing
DESCRIPTION      : flips Top Right of the element
*/
void flipTopRight(int row, int column, GameBoard *game, char flipPiece, char endPiece){ //direction 3
	game->board[row][column] = endPiece;
	//------------------------------------------------------
	row--;
	column++;
	//------------------------------------------------------
	if(row >= 0 && column < game->size && game->board[row][column] == flipPiece){ //top right //Change
		game->board[row][column] = endPiece;
		cout << "changing ( " << row  << ", " << column <<" ) to "<< endPiece << endl;
		flipTopRight(row, column, game, flipPiece, endPiece);
	//------------------------------------------------------
	}
}

/*
NAME			 : flipLeft (HELPER)
INPUT PARAMETERS : int row, int column, GameBoard *game, char flipPiece, char endPiece
OUTPUT PARAMETERS: nothing
DESCRIPTION      : flips Left of the element
*/
void flipLeft(int row, int column, GameBoard *game, char flipPiece, char endPiece){ //direction 4
	game->board[row][column] = endPiece;
	//------------------------------------------------------
	column--;
	//------------------------------------------------------
	if(column >= 0  && game->board[row][column] == flipPiece){ //top right //Change
		game->board[row][column] = endPiece;
		cout << "changing ( " << row  << ", " << column <<" ) to "<< endPiece << endl;
		flipLeft(row, column, game, flipPiece, endPiece);
	//------------------------------------------------------
	}
}

/*
NAME			 : flipRight (HELPER)
INPUT PARAMETERS : int row, int column, GameBoard *game, char flipPiece, char endPiece
OUTPUT PARAMETERS: nothing
DESCRIPTION      : flips Right of the element
*/
void flipRight(int row, int column, GameBoard *game, char flipPiece, char endPiece){ //direction 5
	game->board[row][column] = endPiece;
	//------------------------------------------------------
	column++;//change
	//------------------------------------------------------
	if(column < game->size  && game->board[row][column] == flipPiece){ //top right //Change
		game->board[row][column] = endPiece;
		cout << "changing ( " << row  << ", " << column <<" ) to "<< endPiece << endl;
		flipRight(row, column, game, flipPiece, endPiece);
	//------------------------------------------------------
	}
}

/*
NAME			 : flipBottemLeft (HELPER)
INPUT PARAMETERS : int row, int column, GameBoard *game, char flipPiece, char endPiece
OUTPUT PARAMETERS: nothing
DESCRIPTION      : flips Bottem Left of the element
*/
void flipBottemLeft(int row, int column, GameBoard *game, char flipPiece, char endPiece){ //direction 6
	game->board[row][column] = endPiece;
	//------------------------------------------------------
	row++;
	column--;//change
	//------------------------------------------------------
	if(row < game->size && column >= 0 && game->board[row][column] == flipPiece){ //top right //Change
		game->board[row][column] = endPiece;
		cout << "changing ( " << row  << ", " << column <<" ) to "<< endPiece << endl;
		flipBottemLeft(row, column, game, flipPiece, endPiece);
	//------------------------------------------------------
	}
}

/*
NAME			 : flipBottemMiddle (HELPER)
INPUT PARAMETERS : int row, int column, GameBoard *game, char flipPiece, char endPiece
OUTPUT PARAMETERS: nothing
DESCRIPTION      : flips Bottem Middle of the element
*/
void flipBottemMiddle(int row, int column, GameBoard *game, char flipPiece, char endPiece){ //direction 7
	game->board[row][column] = endPiece;
	//------------------------------------------------------
	row++;
	//------------------------------------------------------
	if(row < game->size && game->board[row][column] == flipPiece){ //top right //Change
		game->board[row][column] = endPiece;
		cout << "changing ( " << row  << ", " << column <<" ) to "<< endPiece << endl;
		flipBottemMiddle(row, column, game, flipPiece, endPiece);
	//------------------------------------------------------
	}
}

/*
NAME			 : flipBottomRight (HELPER)
INPUT PARAMETERS : int row, int column, GameBoard *game, char flipPiece, char endPiece
OUTPUT PARAMETERS: nothing
DESCRIPTION      : flips Bottem Right of the element
*/
void flipBottomRight(int row, int column, GameBoard *game, char flipPiece, char endPiece){ //direction 8
	game->board[row][column] = endPiece;
	//------------------------------------------------------
	row++;
	column++;
	//------------------------------------------------------
	if(row < game->size && column < game->size && game->board[row][column] == flipPiece){ //top right //Change
		game->board[row][column] = endPiece;
		cout << "changing ( " << row  << ", " << column <<" ) to "<< endPiece << endl;
		flipBottomRight(row, column, game, flipPiece, endPiece);
	//------------------------------------------------------
	}
}


/*
NAME			 : playGame
INPUT PARAMETERS : bool fp1, bool fp2, GameBoard *game, bool whoplaysfirstflag
OUTPUT PARAMETERS: nothing
DESCRIPTION      : plays the game
*/
void playGame(bool fp1, bool fp2, GameBoard *game, bool whoplaysfirstflag) {
	bool s1 = true;
	bool s2 = true;
	if (fp1 && fp2 && (s1 || s2)) {
		if (whoplaysfirstflag) {
			game->p = 'X';
			s1 = placepieceperson(game);
			printBoard(game, 0);
			game->p = 'O';
			s2 = placepieceperson(game);
			printBoard(game, 0);
		} else {
			game->p = 'O';
			s1 = placepieceperson(game);
			printBoard(game, 0);
			game->p = 'X';
			s2 = placepieceperson(game);
			printBoard(game, 0);
		}
	} else if (fp1 == false && fp2 == false && (s1 || s2)) {
		game->p = 'X';
		s1 = compplacepiece(game);
		printBoard(game, 0);
		game->p = 'O';
		s2 = compplacepiece(game);
		printBoard(game, 0);
	} else {
		if (whoplaysfirstflag) {
			game->p = 'X';
			s1 = placepieceperson(game);
			printBoard(game, 0);
			game->p = 'O';
			s2 = compplacepiece(game);
			printBoard(game, 0);
		} else {
			game->p = 'X';
			s1 = compplacepiece(game);
			printBoard(game, 0);
			game->p = 'O';
			s2 = placepieceperson(game);
			printBoard(game, 0);
		}
	}
	if (s1 == false && s2 == false || game->totalct == game->size * game->size) {
		cout << "Game over" << endl;
		printBoard(game, 0);
		return;
	}
	playGame(fp1, fp2, game, whoplaysfirstflag);
}

/*
NAME			 : startGame
INPUT PARAMETERS : GameBoard *game
OUTPUT PARAMETERS: nothing
DESCRIPTION      : starts the game
*/
void startGame(GameBoard *game) {
	cout << "How many players? (0,1, or 2)" << endl;
	int numplayers;
	cin >> numplayers;
	if (numplayers == 0) {
		playGame(false, false, game, true);
	} else if (numplayers == 1) {
		bool whoplaysfirstflag;
		rand() % 2 == 0 ? whoplaysfirstflag = true : whoplaysfirstflag = false;
		playGame(true, false, game, whoplaysfirstflag);
	} else
		playGame(true, true, game, true);

	char w = ckwin(game);
	if (w != 'T') {
		cout << w << " WON!!!! " << endl;
	} else {
		cout << "Tie game. " << endl;
	}
	string s;
	cout << "Play again? (Y or N)" << endl;
	cin >> s;
	if (s == "Y") {
		game->totalct = 4;
		makeBoard(game, 0, false);
		cout << "STARTING OVER" << endl;
		printBoard(game, 0);
		startGame(game);
	} else {
		cout << "THANKS FOR PLAYING!" << endl;
	}
}

/*
NAME			 : ckwin
INPUT PARAMETERS : GameBoard *game
OUTPUT PARAMETERS: char
DESCRIPTION      : checks the amount of pieces on the board, returns the winner (returns 'T' for a tie)
*/
char ckwin(GameBoard *game) {
	int X = 0;
	int O = 0;
	return chkwin2(game, 0, X, O);

}

/*
NAME			 : ckwinRow
INPUT PARAMETERS : GameBoard *game, int ct, int &X, int &O
OUTPUT PARAMETERS: nothing
DESCRIPTION      : checks the amount of pieces on the board and goes through each element.
*/
void ckwinRow(GameBoard *game, int inner, int outer, int &X, int &O) {
	int length = game->size;
	if (inner == length) {
	} else {
		if(game->board[outer][inner] == 'X'){
			X++;
		}else if(game->board[outer][inner] == 'O'){
			O++;
		}
		inner++;
		ckwinRow(game, inner, outer, X, O);
	}
}


/*
NAME			 : ckwin2
INPUT PARAMETERS : GameBoard *game, int ct, int &X, int &O
OUTPUT PARAMETERS: char
DESCRIPTION      : checks the amount of pieces on the board (helper function) returns who won, "T" if a tie
*/
char chkwin2(GameBoard *game, int ct, int &X, int &O){
	if (ct < game->size) {
			ckwinRow(game, 0, ct, X, O);
			ct++;
			chkwin2(game, ct, X, O);
		}

		if(X == O){
			return 'T';
		}
		if(X > O){
			return 'X';
		}
		if(O > X){
			return 'O';
		}

}




