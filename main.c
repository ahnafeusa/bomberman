#include <stdio.h>
#include <stdlib.h>
#include <time.h>

float randomNum(int n, int m, int neg);
void displayGame(int rows, int cols, float (*a)[cols], float totalScore, int lives, float score, int bombs);
void initializeGame(int rows, int cols , float (*a)[cols]);
int randomNum1(int n, int m);
void exitGame(float score, time_t then, int flag, char* playerName);
int updateBoard(int rows, int cols, float (*a)[cols], float (*b)[cols], int xLocation, int yLocation,int boost);
float calculateScore(int rows, int cols, float (*a)[cols], float (*b)[cols], int xLocation, int yLocation,int boost);
void logScore(char* playerName, float score, time_t duration);
float boardScore(int rows, int cols, float (*a)[cols]);
void displayTopScores(int n);

int main(int argc, char **argv) {
  //making sure the right number of inputs are given. And output error message if not.
  if((argc < 4)||(argc > 4)){
    printf("Usage ./main <playername> <int> <int>\n");
    fprintf(stderr, "Wrong Input\n");
    exit(EXIT_FAILURE);
  }

  //storing the command line arguments into variables so it's easier to work with
  char *playerName = argv[1];
  int rows = atoi(argv[2]);
  int cols = atoi(argv[3]);

  //checking if the input values are with in range
  if((rows<10)||(cols<10)){
    printf("Please enter values of atleast 10\n");
    fprintf(stderr, "Wrong Input\n");
    exit(EXIT_FAILURE);
  }

  //intializing the 2D arrays
  float board[rows][cols];
  float board1[rows][cols];

  int boardSize=rows*cols;

  //initializing the score, lives and bombs
  float score = 0.0;
  float totalScore = 0.0;
  int lives = 3;
  int bombs = boardSize*0.05;
  int boost = 0;
  time_t then;
  then = time(NULL);

  //Preparing board1 with the value 100 so that it is distinguishable and we can print 'X' for it
  int i,j=0;
  for (i = 0; i < rows; i++) {
    for (j = 0; j < cols; j++) {
      board1[i][j]=100.0;
    }
  }

  //Filling board with random values
  initializeGame(rows, cols, board);

  //Printing the raw board values so its easier for TA to mark
  for(i = 0; i<rows; i++){ 
    printf("\n"); 
    for(j = 0; j<cols; j++) {
      if(board[i][j]==69.00){
        printf("  $  ");
      }
      else if(board[i][j]==-100.00){
        printf("  *  ");
      }else{
        printf("%.2f", board[i][j]);
      }
    }
  }
  printf("\n\n");

  //displaying the coverd board1
  displayGame(rows, cols, board1, totalScore, lives, score, bombs);


  //Putting the next part in an infinite while loop so that the player can enter the game multiple times 
  while(1){
    //declaring variables
    char userInput[30];
    int userInput1;
    int xInput,yInput;

    //checking if the number of bombs have been depleted and ending the game if it is
    if(bombs<=0){
      exitGame(score, then, 2, playerName);
    }

    //checking if user want to end game
    printf("\nEnter any key to play game, 'H' to see highscores and 'Q' to exit game: ");
    scanf("%s", userInput);
    if(*userInput=='Q'){
      //exiting game
      exitGame(score, then, 0, playerName);
    }
    else if(*userInput=='H'){
      printf("\nHow many highscores do you want to see: ");
      scanf("%d", &userInput1);
      displayTopScores(userInput1);

    }

    //getting bomb position from user
    printf("\nEnter bomb position(x, y): ");
    scanf("%d %d", &xInput, &yInput);

    //updating the score
    score+=calculateScore(rows, cols, board, board1, xInput, yInput, boost);
    //updating the covered board to 
    boost=updateBoard(rows, cols, board, board1, xInput, yInput, boost);
    
    totalScore=boardScore(rows, cols, board1);
    if(score<0){
      lives-=1;
      score=0.0;
    }
    bombs-=1;


    displayGame(rows, cols, board1, totalScore, lives, score, bombs);
    for (i = 0; i < rows; i++) {
      for (j = 0; j < cols; j++) {
        if(board1[i][j]==-100.0){
          exitGame(score, then, 1, playerName);
        }
      }
    }
  }
  return 0;
}

float boardScore(int rows, int cols, float (*a)[cols]){
  int i,j;
  float sum;
  for (i = 0; i < rows; i++) {
    for (j = 0; j < cols; j++) {
      if((a[i][j]<15.0)&&(a[i][j]>-15.0)){
        sum+= a[i][j];
      }
    }
  }
  return sum;
}
int updateBoard(int rows, int cols, float (*a)[cols], float (*b)[cols], int xLocation, int yLocation,int boost){
  int i,j;
  int newBoost = 0;
  int layers=0;
  if(boost>0){
    if(boost==1){
      layers=1;
    }
    else if(boost>1){
      layers=1;
      layers= layers << (boost-1);
    }
  }
  for(i=xLocation-1-layers; i<=xLocation+1+layers; i++){
    for(j=yLocation-1-layers; j<=yLocation+1+layers; j++){
      if((i<0)||(i>rows-1)||(j<0)||(j>cols-1)){
        continue;
      }
      b[i][j]=a[i][j];
      if(a[i][j]==69.0){
        newBoost += 1;
      }

    }
  }
  if(newBoost>0){
    printf("Bang!! your bomb range is increased by %d folds\n", newBoost+1);
  }
  return newBoost;
}

float calculateScore(int rows, int cols, float (*a)[cols], float (*b)[cols], int xLocation, int yLocation,int boost){
  int i,j;
  float sum = 0.0;
  int layers=0;
  if(boost>0){
    if(boost==1){
      layers=1;
    }
    else if(boost>1){
      layers=1;
      layers= layers << (boost-1);
    }
  }
  for(i=xLocation-1-layers; i<=xLocation+1+layers; i++){
    for(j=yLocation-1-layers; j<=yLocation+1+layers; j++){
      if((i<0)||(i>rows-1)||(j<0)||(j>cols-1)){
        continue;
      }
      if(b[i][j]==100.00){
        if((a[i][j]<15.0)&&(a[i][j]>-15.0)){
          sum+= a[i][j];
        }
      }
    }
  }
  return sum;
}

float randomNum(int n, int m, int neg){
  float num = rand()&15;
  float num1 = ((float)rand())/RAND_MAX;
  num += num1-0.5;

  if(neg==0){
    return num;
  }else{
  return -num;
  }
}

int randomNum1(int n, int m){
  int num = rand()%m;
  while((num<n)){
    num = rand()%m;
  }
  return num;
}

void displayGame(int rows, int cols, float (*a)[cols], float totalScore, int lives, float score, int bombs){
  printf("Total uncovered score of %.2f points\n", totalScore);
  int i,j;
  for(i = 0; i<rows; i++){ 
    printf("\n"); 
    for(j = 0; j<cols; j++) {
      if(a[i][j]==100.0){
        printf("X");
      }
      else if(a[i][j]==-100.0){
        printf("*");
      }
      else if(a[i][j]==69.0){
        printf("$");
      }
      else if(a[i][j]>0){
        printf("+");
      }
      else if(a[i][j]<0){
        printf("-");
      }
    } 
  }
  printf("\n");

  printf("Lives: %d\n", lives);
  printf("Score: %.2f\n", score);
  printf("Bombs: %d\n", bombs);
    
}


void initializeGame(int rows, int cols ,float (*a)[cols]){
  srand(time(NULL));
  int i,j;
  int MIN = 0;
  int MAX = 15;
  int neg = 0;
  int nCount = 0;
  int eCount = 0;
  int dice = 0;
  int totalTiles=rows*cols;

  for (i = 0; i < rows; i++) {
    for (j = 0; j < cols; j++) {
      dice=randomNum1(0, 100);
      if((dice>=80)&&(dice<=99)){
        if(eCount==9){
          a[i][j]=-100.00;
          eCount++;
        }else{
        a[i][j] = 69.0;
        eCount++;
        }
      }
      else if(dice<40){
        a[i][j] = randomNum(MIN, MAX, 1);
        nCount++;
      }else{
        a[i][j] = randomNum(MIN, MAX, 0);
      }
    }
  }

  float fortyPercent = (float)totalTiles*0.4;
  float nPercent = (float)(nCount/(float)totalTiles)*100;
  if(nPercent>fortyPercent){
    printf("\n\nTotal Negative Numbers %d/%d = %.2f%% more than 40%%\n", nCount, totalTiles, (nPercent-fortyPercent));
  }else{
    printf("\n\nTotal Negative Numbers %d/%d = %.2f%% less than 40%%\n", nCount, totalTiles, (fortyPercent-nPercent));
  }

  
  
}

void exitGame(float score, time_t then, int flag, char* playerName){
  //flags
  //Quit game   0
  //Win         1
  //Lose        2
  time_t now;
  now = time(NULL);
  int timeElapsed = now - then;
  if(flag==0){
    printf("Game quit.\n");
  }
  else if(flag==1){
    printf("YOU WON\n");
  }else{
    printf("You lost :/\n");
  }

  logScore(playerName, score, timeElapsed);

  printf("Thank you for playing the game ! ^-^\n");
  exit(0);
}

void logScore(char* playerName, float score, time_t duration){
  FILE *fptr;
  fptr = fopen("scores.log", "a+");
  int seconds = duration;
  if(fptr == NULL)
  {
    printf("Error opening file!\n");   
    exit(1);             
  }
  fprintf(fptr, "%s;%d;%6.2f\n", playerName, seconds, score);
  fclose(fptr);
}

void displayTopScores(int n){
  int i;
  int j;
  int count;
  float temp;
  char name[10];
  int time;
  float score;
  float scores[10];
  printf("works22");
  FILE *fptr;
  fptr = fopen("scores.log", "r");

  if(fptr == NULL)
  {
    printf("Error opening file!\n");   
    exit(1);             
  }

  char singleLine[30];
  printf("works11");

  while(!feof(fptr)){
    fgets(singleLine, 30, fptr);
    fscanf(fptr,"%s;%d;%f", name, &time, &score);
    scores[i]=score;
    i++;
  }
  
  printf("works");

  for(i=0; i < 50; i++){
		for(j=i + 1; j < 50; j++){
			if(scores[i] > scores[j]){
				temp = scores[i];
				scores[i] = scores[j];
				scores[j] = temp;
			}	
		}
	}
  printf("works2");
  for(i=0; i<n; i++){
    while(!feof(fptr)){
      fgets(singleLine, 30, fptr);
      fscanf(fptr,"%s;%d;%f", name, &time, &score);
      if(score==scores[i]){
        printf("Player: %s Duration(s): %d Score: %.2f", name, time, score);
      }
      
    }
  }
  fclose(fptr);
}