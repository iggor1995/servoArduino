 //Any coordinats used in this program presented as impulses for step motors provided by servo driver.
  //Each motor (left and right) controlled by servodriver
  //Arduino controller sends signal to both servo drivers
  //leftmove = left(true) + right(true)
  //rightmove = left(false) + right(false)
  //upMove = left(false) + right(true)
  //downMove = left(true) + right(false)
  //*******************constants******************
 #include <SPI.h>
#include <Ethernet.h>
#include "Mudbus.h"

Mudbus Mb;
//Function codes 1(read coils), 3(read registers), 5(write coil), 6(write register)
//signed int Mb.R[0 to 125] and bool Mb.C[0 to 128] MB_N_R MB_N_C
//Port 502 (defined in Mudbus.h) MB_PORT

#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
{'1','2','3'},
{'4','5','6'},
{'7','8','9'},
{'*','0','#'}
};
byte rowPins[ROWS] = {28, 27, 26, 25};
byte colPins[COLS] = {24, 23, 22};
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

  
  
  
  
  
  
  
  boolean grabbingStage;
  boolean movingToSellStage;
  int gripIn = 0;
  int manualMove = 2;
  int releaseStop = 5;
  int clkLeft = 6;
  int clkRight = 7; 
  int cwLeftMotor = 8;
  int cwRightMotor = 9;
  int maxPartsOnLevel = 8;
  int maxPartsInRow = 4;
  int motorSpeed = 50;
  int grabbingPartXCoord = 1400;
  int grabbingPartYCoord = 24780;
  int xFirstCellPos = 600;
  int xDelta = 2275;
  int firstLevelY = 20500;
  int secondLevelY = 16600;
  int thirdLevelY = 12800; 
  int fourthLevelY = 9200; 
  int fifthLevelY = 5500; 
  int sixthLevelY = 1800; 
  //**********************************************
  int isGripperPlaced = 4;
  int redPartCount = 0;
  int transparentPartCount = 0;
  int metalPartCount = 0;
  int colorNumber = 0; // 10 - red | 20 - transparent | metal - 30
  //main var for listener
  //*****************from FX5U controller*******
  int isAllowedToMove = 1;
  int typeA = 2;
  int typeB = 3;
  int allowToMove = 1;
  //********************************************
  int x = 0;
  int y = 0;
  // level1 and level2 and partsCount are changed due to type of part. initialized in checkColor() method
  int lowerLevel;
  int higherLevel;
  int partCount;
  //*****************************************************************************************************
void setup() {
  // put your setup code here, to run once:
  pinMode(clkLeft, OUTPUT);
  pinMode(clkRight, OUTPUT);
  pinMode(cwLeftMotor, OUTPUT);
  pinMode(cwRightMotor, OUTPUT);
  pinMode(releaseStop, OUTPUT);
  pinMode(manualMove, INPUT);
  pinMode(typeA, INPUT);
  pinMode(typeB, INPUT);
  pinMode(isGripperPlaced, OUTPUT);
  pinMode(allowToMove, INPUT);
  pinMode(gripIn, INPUT);
//  Serial.begin(9600);
  uint8_t mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };
  uint8_t ip[] = { 192, 168, 1, 100 };
  uint8_t gateway[] = { 192, 168, 0, 4 };
  uint8_t subnet[] = { 255, 255, 255, 0 };
  Ethernet.begin(mac, ip, gateway, subnet);
  //Avoid pins 4,10,11,12,13 when using ethernet shield
  delay(5000);
}
int xg = 0;
int sts = 0;

void loop() {
//  digitalWrite(releaseStop, HIGH);
  mainMove();
}
  void mainMove(){
     firstMove();
   delay(2000);
   secondMove();
   delay(2500);
  }
  
  
  boolean checkIfManual(){
      if(Mb.C[21] == true){   //if manual mode
        manualMode();
      }
  }

  void manualMode(){
    while(Mb.C[21] == 1){
      Mb.Run();
      if(Mb.C[26] == 1){
         digitalWrite(releaseStop, HIGH);
         while(Mb.C[26] == 1){
          Mb.Run();
         }
      }
      if(Mb.C[22] == 1){
        digitalWrite(releaseStop, HIGH);
        while(Mb.C[22] == 1){ // moveUp
          moveUpManual(1);
          Mb.Run();
        }
        digitalWrite(releaseStop, LOW);
      }
      if(Mb.C[23] == 1){
        digitalWrite(releaseStop, HIGH);
        while(Mb.C[23] == 1){ // moveDown
          Mb.Run();
          moveDownManual(10);
        }
        digitalWrite(releaseStop, LOW);
      }
      if(Mb.C[24] == 1){
        digitalWrite(releaseStop, HIGH);
        while(Mb.C[24] == 1){ // moveLeft
          Mb.Run();
          moveLeftManual(10);
        }
        digitalWrite(releaseStop, LOW);
      }
      if(Mb.C[25] == 1){
        digitalWrite(releaseStop, HIGH);
        while(Mb.C[25] == 1){ // moveRight
          Mb.Run();
          moveRightManual(10);
        }
      }
      digitalWrite(releaseStop, LOW);
    }
  }
  //**************************************************
  // change direction to down and move
// xRel = relative coordinate changes
  void moveLeftManual(int xRel){
    changeDir(true, cwLeftMotor);
    changeDir(true, cwRightMotor);
    moveMotors(xRel);
  }
// change direction to left and move
// xRel = relative coordinate changes  
  void moveRightManual(int xRel){
    changeDir(false, cwLeftMotor);
    changeDir(false, cwRightMotor);
    moveMotors(xRel);
  }
// change direction to upward and move
// xRel = relative coordinate changes
  void moveUpManual(int xRel){
    changeDir(false, cwLeftMotor);
    changeDir(true, cwRightMotor);
//     moveUp(1000);
    moveMotors(xRel);
  }
// change direction to downward and move
// xRel = relative coordinate changes
  void moveDownManual(int xRel){
    changeDir(true, cwLeftMotor);
    changeDir(false, cwRightMotor);
    moveMotors(xRel);
  }
//****************************************************************************************************

///////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
  //waits for permission to move, if gots permission, then moves to grabbing point 
  // then sends command to FX5U that gripper has been placed
  void firstMove(){
    digitalWrite(isGripperPlaced, LOW);
    while(digitalRead(allowToMove) != true){
          Mb.Run();
          checkIfManual();
    }
    Serial.println("firstMove out of cycle");
    delay(1000);
    digitalWrite(isGripperPlaced, LOW);
    if(digitalRead(allowToMove) == true){
      moveToGrabbingPos();
      Serial.println("fallow to move");
      delay(1000);
      digitalWrite(isGripperPlaced, HIGH);
      delay(2300);
      digitalWrite(isGripperPlaced, LOW);
    }
  }
  
  //waits for permission to move, if gots permission, then moves to placing point 
  // then sends command to FX5U that gripper has been placed
  void secondMove(){
          Serial.println("secind move");

    digitalWrite(isGripperPlaced, LOW);
    while(digitalRead(allowToMove) != true){
      //do nothing
    }
     digitalWrite(isGripperPlaced, LOW);
     if(digitalRead(allowToMove) == true){
        int colorNumber = checkColor();
        if(colorNumber != 0){
          movePart(colorNumber);
          digitalWrite(isGripperPlaced, HIGH);
        }
     }
     delay(1000);
     digitalWrite(isGripperPlaced, LOW);
  }
//////////////////////////////////////////////////////////////////////   
/////////////////////////////////////////////////////////////////////
//moves certain part to certain position
 void movePart(int color){
    int delta;
    setLevelsandPartCount(color); 
    if(partCount > 16){
      while(true){
            //do nothing            
          }
    }
    if(partCount < 17){
      if(partCount <= maxPartsOnLevel){
        yMoveToPos(lowerLevel);
      } 
      else{
        yMoveToPos(higherLevel);
        partCount = partCount - maxPartsOnLevel;
      }
      xMoveToPos(xFirstCellPos); // move to first sell int row
      if(partCount <= maxPartsInRow){
        delta = xDelta * (partCount - 1);
      }
      else{
        delta = xDelta * (partCount - maxPartsInRow - 1);
      }
      moveRight(delta);
    }
  }
  
   //checks color or type of the part and set level1 and level2
  int checkColor(){
    if(digitalRead(typeA) == HIGH){
      if(digitalRead(typeB) == HIGH){
        // sort next 
        // certain part count
        redPartCount++;
        colorNumber = 1;
        return colorNumber;      //1* - means red part and cell depends on *      
      }
      else{
          //sort next
          transparentPartCount++;
          colorNumber = 2;
          return colorNumber;  //2* - means transparent part and cell depends on *
        }
    }
    else{
      if(digitalRead(typeB) == HIGH){
          return 0;
        }
      else{
          //sort next
            metalPartCount++;
            colorNumber = 3;  
            return colorNumber;    //3* - means metal part and cell depends on * digit 
        }
    }
  }
  
  //method sets global level coordinates and global variable partCount
  //color - number of color : 1 - red; 2 - transparent; 3 - metal;
  void setLevelsandPartCount(int color){
    if(color == 1){    //RED COLOR
        lowerLevel = firstLevelY;
        higherLevel = secondLevelY;
        partCount = redPartCount;
    }
    else if(color == 2){    //TRANSPARENT COLOR
        lowerLevel = thirdLevelY;
        higherLevel = fourthLevelY;
        partCount = transparentPartCount;
    }
    else{    //METAL COLOR
        lowerLevel = fifthLevelY;
        higherLevel = sixthLevelY;
        partCount = metalPartCount;
    }
  }
  
// moves grabber to grabbing pos, where part has been set
  void moveToGrabbingPos(){
    xMoveToPos(grabbingPartXCoord);
    yMoveToPos(grabbingPartYCoord);
  }
//******************main methods for movement*****************************************
  void getBackToInitial(){
      xMoveToPos(0);
      yMoveToPos(0);
  }
// move grabber to absolute position by x coordinates
  void xMoveToPos(int pos){
    digitalWrite(isGripperPlaced, LOW);
    int moveX;
    if(pos > x){
      moveX = pos - x;
      moveRight(moveX);
    }
    if(pos < x){
      moveX = x - pos;
      moveLeft(moveX);
    }    
  }
  
// move grabber to absolute position by y coordinates
   void yMoveToPos(int pos){
    digitalWrite(isGripperPlaced, LOW);
    int moveY;
    if(pos > y){
      moveY = pos - y;
      moveDown(moveY);
    }
    else if(pos < y){
      moveY = y - pos;
      moveUp(moveY);
    }
    else{
        //do nothing
    }    
  }


// change direction to down and move
// xRel = relative coordinate changes
  void moveLeft(int xRel){
    digitalWrite(releaseStop, HIGH);
    delay(100);
    changeDir(true, cwLeftMotor);
    changeDir(true, cwRightMotor);
    moveMotors(xRel);
    delay(10);
    digitalWrite(releaseStop, LOW);
    delay(100);
  }
  //**************************************************
// change direction to left and move
// xRel = relative coordinate changes  
  void moveRight(int xRel){
    digitalWrite(releaseStop, HIGH);
    delay(100);
    changeDir(false, cwLeftMotor);
    changeDir(false, cwRightMotor);
    moveMotors(xRel);
    delay(10);
    digitalWrite(releaseStop, LOW);
    delay(100);
  }
// change direction to upward and move
// xRel = relative coordinate changes
  void moveUp(int xRel){
    digitalWrite(releaseStop, HIGH);
    delay(100);
    changeDir(false, cwLeftMotor);
    changeDir(true, cwRightMotor);
    moveMotors(xRel);
    delay(10);
    digitalWrite(releaseStop, LOW);
    delay(100);
  }
// change direction to downward and move
// xRel = relative coordinate changes
  void moveDown(int xRel){
    digitalWrite(releaseStop, HIGH);
    delay(100);
    changeDir(true, cwLeftMotor);
    changeDir(false, cwRightMotor);
    moveMotors(xRel);
    delay(10);
    digitalWrite(releaseStop, LOW);
    delay(100);
  }
//****************************************************************************************************

//move each motor [impulseCount] times  and change global coordinates : x and y
  void moveMotors(int impulseCount){
      for(int i = 0; i < impulseCount; i++){
                  moveLeftMotor();
                  moveRightMotor();
                  changeCoord();
          }
   }

// changes x and y on one depending on rotation direction  
// x and y are absolute coordinates
  void changeCoord(){
    Mb.Run();
    if(digitalRead(cwLeftMotor) == true){
      if(digitalRead(cwRightMotor) == true){  //left(true) right(true) Left Move
        if(x > 0){
          x--;
        }
      }
      else{  //left(true) right(false) Up Move
        y++;
      }
    }
    else{
      if(digitalRead(cwRightMotor) == true){  //left(false) right(true) Down Move
        if(y > 0){
          y--;
        }
      }
      else{  //left(false) right(false) Right Move
        x++;
      }
    }
  }
  
//********************************************initial methods**************************
// change rotating direction to certain motor
// motorNumber - address for clk+ output
  void changeDir(boolean isCwPlus, int motorNumber){
    Mb.Run();
    if(isCwPlus == true){
     digitalWrite(motorNumber, HIGH);
    }
    else{
      digitalWrite(motorNumber, LOW);
    }    
  }
  //send impulse to right motor  
  void moveLeftMotor(){
//    if(digitalRead(allowToMove) == true && digitalRead(gripIn) == true){
      Mb.Run();
      digitalWrite(clkLeft, HIGH);
      delayMicroseconds(motorSpeed);
      digitalWrite(clkLeft, LOW);
      delayMicroseconds(motorSpeed);
//    }
  }
  //send impulse to right motor
  void moveRightMotor(){
//      if(digitalRead(allowToMove)== true && digitalRead(gripIn) == true){
        Mb.Run();
        digitalWrite(clkRight, HIGH);
        delayMicroseconds(motorSpeed);
        digitalWrite(clkRight, LOW);
        delayMicroseconds(motorSpeed);
//      }
  }
  //***************************************************************************
