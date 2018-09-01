//Code 21 InvisiMaze
//team welivetogether
#include "Adafruit_HT1632.h"
#define HT_DATA 2
#define HT_WR   3
#define HT_CS   4
#define HT_CS2  5

// joystick consts
#define UTRIG 50
#define RTRIG 950
#define DTRIG 950
#define LTRIG 50

// TONES  ==========================================
// Start by defining the relationship between 
//       note, period, &  frequency. 
#define  a     4545    // 220 Hz 
#define  b     4049    // 247 Hz 
#define  c     3830    // 261 Hz 
#define  d     3400    // 294 Hz 
#define  e     3038    // 329 Hz 
#define  f     2864    // 349 Hz 
#define  g     2550    // 392 Hz 
#define  A     2272    // 440 Hz 
#define  B     2028    // 493 Hz 
#define  C     1912    // 523 Hz
#define  D     1702    // 587 Hz
#define  E     1517    // 659 Hz

// Define a special note, 'R', to represent a rest
#define  R     0

// SETUP ============================================
// Set up speaker on a PWM pin (digital 9, 10 or 11)
int speakerOut = 10;

// player position consts
const char up ='u';
const char down ='d';
const char left ='l';
const char right ='r';
const char wait ='w';

unsigned long timer;

// maze size const
const int X_MAX =16;
const int Y_MAX =24;



Adafruit_HT1632LEDMatrix matrix = Adafruit_HT1632LEDMatrix(HT_DATA, HT_WR, HT_CS);

// Game Variables
int analX, analY; // Analog input vars
int playerX = 1, playerY = 1; //player current cords
char curDir, lastDir; // player direction u/d/l/r
bool buttonL, buttonR;
bool win = false;
// Button pins
int rightBut = 9, leftBut = 8;



// current maze
bool maze[X_MAX][Y_MAX];

// MAZES

bool fail[X_MAX][Y_MAX] =
{
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,1,0,1,0,1,1,1,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0},
  {0,1,0,1,0,1,0,1,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,1,0,0,1,0,1,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,1,0,0,1,0,1,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,1,0,0,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,1,1,0,0,1,0,1,1,1,0,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,1,0,1,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,0},
  {0,0,0,0,0,1,0,1,0,1,0,1,1,0,0,1,0,1,0,0,0,0,0,0},
  {0,0,0,0,0,1,0,1,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,0},
  {0,0,0,0,0,1,1,0,0,1,0,1,1,1,0,1,1,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
  };


 bool hard1[X_MAX][Y_MAX] =
{
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
  {0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,1,1,1,1,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,1,1,1,1,0,0,0},
  {0,0,0,1,1,1,1,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0},
  {0,0,0,1,1,1,1,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0},
  {0,0,0,1,1,0,0,1,1,1,1,1,1,0,0,1,1,0,0,1,1,0,0,0},
  {0,0,0,1,1,0,0,1,1,1,1,1,1,0,0,1,1,0,0,1,1,0,0,0},
  {0,0,0,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,1,1,0,0,0},
  {0,0,0,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,1,1,0,0,0},
  {0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0},
  {0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0},
  {0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,0},
  {0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
  };


  // music stuff





void setMaze(bool newMaze[X_MAX][Y_MAX])
{
    for(int y = 0; y < Y_MAX; y++)
  {
    for(int x = 0; x < X_MAX; x++){
      maze[x][y] = newMaze[x][y];
    }
  }
}


void printMaze()
{
  
  
  for(int y = 0; y < Y_MAX; y++)
  {
    for(int x = 0; x < X_MAX; x++){
      matrix.drawPixel(y,x,!maze[x][y]);
    }
  }
  for(int i = 0; i < 5; i++){
    matrix.drawPixel(playerX, playerY, 1);
    matrix.drawPixel(playerX+1, playerY, 1);
    matrix.drawPixel(playerX, playerY+1, 1);
    matrix.drawPixel(playerX+1, playerY+1, 1);
    matrix.writeScreen();
    delay(250);
    matrix.drawPixel(playerX, playerY, 0);
    matrix.drawPixel(playerX+1, playerY, 0);
    matrix.drawPixel(playerX, playerY+1, 0);
    matrix.drawPixel(playerX+1, playerY+1, 0);
    matrix.writeScreen();
    delay(250);
  } 
    for(int y = 0; y < Y_MAX; y++)
  {
    for(int x = 0; x < X_MAX; x++){
      matrix.drawPixel(y,x,0);
    }
  }
  matrix.drawPixel(playerX, playerY, 1);
  matrix.drawPixel(playerX+1, playerY, 1);
  matrix.drawPixel(playerX, playerY+1, 1);
  matrix.drawPixel(playerX+1, playerY+1, 1);
  matrix.writeScreen();
}

void getInput()
{
   analX = analogRead(A1);
   analY = analogRead(A0);
   buttonL = digitalRead(leftBut);
   buttonR = digitalRead(rightBut);
}

void updatePosition()
{
   lastDir = curDir;
   //move pixel
   if (analX > RTRIG) curDir = right;
   else if (analX < LTRIG) curDir = left;
   else if (analY > DTRIG) curDir = down;
   else if (analY < UTRIG) curDir = up;
   else curDir = wait;
   
   if(curDir != wait) // if new direction, erase old position from matrix
   {
    matrix.drawPixel(playerX, playerY, 0);
    matrix.drawPixel(playerX+1, playerY, 0);
    matrix.drawPixel(playerX, playerY+1, 0);
    matrix.drawPixel(playerX+1, playerY+1, 0);
    if ((curDir == right)&&(lastDir != right)) playerX+=2;
    else if ((curDir == left)&&(lastDir != left)) playerX-=2;
    else if ((curDir == down)&&(lastDir != down)) playerY-=2;
    else if ((curDir == up)&&(lastDir != up)) playerY+=2;
   } 

    // Bounds Checking
    if(!maze[playerY][playerX] || playerX < 1 || playerY < 1 || playerY >= 15 || playerX >= 23)
      triggerFail();

   //redraw player because he probably got overwritten
    matrix.drawPixel(playerX, playerY, 1);
    matrix.drawPixel(playerX+1, playerY, 1);
    matrix.drawPixel(playerX, playerY+1, 1);
    matrix.drawPixel(playerX+1, playerY+1, 1);

    //CHECK WIN
    if(playerX == 21 && playerY == 13){
      win = 1; // thats a win boi
      // play win noise please
    } 
}

void triggerFail()
{
  //play sound
  //print fail
  matrix.clearScreen();
  loseSounds();
  for(int y = 0; y < Y_MAX; y++)
  {
    for(int x = 0; x < X_MAX; x++){
      matrix.drawPixel(y,x,fail[x][y]);
    }
  }
  matrix.writeScreen();
  delay(500);
  for(int x = 0; x < X_MAX; x++)
  {
    for(int y = 0; y < Y_MAX; y++){
      matrix.drawPixel(y,x,!maze[x][y]);
      matrix.writeScreen();
    }
    delay(30);
  }
  playerX = playerY = 1;
  printMaze();
}


void setup() {
  Serial.begin(9600);
  matrix.begin(ADA_HT1632_COMMON_16NMOS);
  pinMode(rightBut, INPUT);
  pinMode(leftBut, INPUT);
  pinMode(speakerOut, OUTPUT);
  buttonR = false;
  setMaze(hard1);
  printMaze();
}


int melody1[] = {  e,  f,  e,  c,  a,  A,   R };
int beats1[]  = { 16, 16, 16, 16, 64, 64, 128 };

int melody2[] = {  e,  g,  A,  C,  e,  g,  A,   R };
int beats2[]  = { 16, 16, 16, 16, 48, 48, 96, 128 }; 

int MAX_COUNT1 = sizeof(melody1) / 2;

// Set overall tempo
long tempo = 7500;
int pause = 1000;
int rest_count = 100; //<-BLETCHEROUS HACK; See NOTES

// Initialize core variables
int tone_ = 0;
int beat = 0;
long duration  = 0;

// PLAY TONE  ==============================================
// Pulse the speaker to play a tone for a particular duration
void playTone() {
  long elapsed_time = 0;
  if (tone_ > 0) { // if this isn't a Rest beat, while the tone has 
    //  played less long than 'duration', pulse speaker HIGH and LOW
    while (elapsed_time < duration) {
      digitalWrite(speakerOut,HIGH);
      delayMicroseconds(tone_ / 2);
      digitalWrite(speakerOut, LOW);
      delayMicroseconds(tone_ / 2);
      elapsed_time += (tone_);
    } 
  }
  else { // Rest beat; loop times delay
    for (int j = 0; j < rest_count; j++) { // See NOTE on rest_count
      delayMicroseconds(duration);  
    }                                
  }                                 
}

void loseSounds() {
  for (int i=0; i<MAX_COUNT1; i++) {
  tone_ = melody1[i];
  beat = beats1[i];
  duration = beat * tempo; // Set up timing
  playTone();
  delayMicroseconds(pause);
  }
}

void winSounds() {
  for (int i=0; i<MAX_COUNT1; i++) {
  tone_ = melody2[i];
  beat = beats2[i];
  duration = beat * tempo; // Set up timing
  playTone();
  delayMicroseconds(pause);
  }
}



void loop() {
 while(1){
  timer = millis()/10^6;
  while(!win){
   getInput();
   if(buttonR){ printMaze(); }
   updatePosition();
   matrix.writeScreen();
   delay(30);
  }
  matrix.clearScreen();
  winSounds();
  
  int timer2 = -1* timer - millis()/10^6;
  Serial.println(timer2);
  matrix.setCursor(0, 0);
  matrix.print(timer2/-100);
  matrix.writeScreen();
  delay(10000);
  matrix.clearScreen();
  while(1)
  {
    delay(30);
    //diddle diddle diddle 
    int quick_maffs = 2+2-1;
  }
  }
  // i do not like this hardware;
}


