  
  //Any coordinats used in this program presented as impulses for step motors provided by servo driver.
  //Each motor (left and right) controlled by servodriver
  //Arduino controller sends signal to both servo drivers
  //leftmove = left(true) + right(true)
  //rightmove = left(false) + right(false)
  //upMove = left(false) + right(true)
  //downMove = left(true) + right(false)
  //*******************constants****************
  int clkLeft = 9;
  int clkRight = 10; 
  int cwLeftMotor = 11;
  int cwRightMotor = 12;
  int maxPartsOnLevel = 8;
  int maxPartsInRow = 4;
  int motorSpeed = 50;
  int grabingPartXCoord;// = ;
  int grabbingPartYCoord;// = ;
  int xFirstCellPos;// = ;
  int xDelta;// = ;
  int firstLevelY;// = ;
  int secondLevelY;// = 
  int thirdLevelY;// = 
  int fourthLevelY;// = 
  int fifthLevelY;// = 
  int sixthLevelY;// = 
  //**********************************************
  int redPartCount;
  int transparentPartCount;
  int metalPartCount;
  //main var for listener
  //*****************from FX5U controller*******
  int isAllowedToMove;
  int typeA;
  int typeB;
  //********************************************
  int x;
  int y;
  // level1 and level2 and partsCount are changed due to type of part. initialized in checkColor() method
  int lowerLevel;
  int higherLevel;
  int partsCount;
  //*****************************************************************************************************
void setup() {
  // put your setup code here, to run once:
  pinMode(clkLeft, OUTPUT);
  pinMode(clkRight, OUTPUT);
  pinMode(cwLeftMotor, OUTPUT);
  pinMode(cwRightMotor, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
}
 //Listents if movement allowed
  void commandListener(){
    if(digitalRead(isAllowedToMove) == true){
      moveToGrabbingPos();
      partCount = checkColor();
      movePart(partCount);
    }
    else{
       //do nothing
    }
  }
//moves certain part to certain position
 void movePart(int partCount){
    int delta;
    if(partCount < maxPartsOnLevel){
      void yMoveToPos(lowerLevel);
    } 
    else{
      void yMoveToPos(higherLevel);
    }
    xMoveToPos(xFirstCellPos); // move to first sell int row
    if(partCount < maxPartsInRow){
      delta = xDelta * redPartCount;
    }
    else{
      delta = xDelta * (redPartCount - maxPartsInRow);
    }
    moveRigt(delta);
  }
// moves grabber to grabbing pos, where part has been set
  void moveToGrabbingPos(){
    xMoveToPos(grabingPartXCoord);
    yMoveToPos(grabingPartYCoord);
  }
// move grabber to absolute position by x coordinates
  void xMoveToPos(int pos){
    int moveX;
    if(pos > x){
      moveX = pos - x;
      moveLeft(moveX)
    }
    if(pos < x){
      moveX = x - pos;
      moveRight(moveX)
    }    
  }
// move grabber to absolute position by y coordinates
   void yMoveToPos(int pos){
    int moveY;
    if(pos > y){
      moveY = pos - y;
      moveDown(moveY)
    }
    if(pos < y){
      moveY = y - pos;
      moveUp(moveY)
    }    
  }
  //checks color or type of the part and set level1 and level2
  int checkColor(){
    if(typeA == true){
      if(typeB == true){
        //sort next 
   return // certain part count     
      }
      else(){
          //sort next
        }
    }
    else{
      if(typeB == true){
        //sort next
      }
      else(){
          //sort next
        }
    }
  }

//******************main methods for movement*****************************************
  void getBackToInitial(){
      moveLeft(x);
      moveUp(y);
  }
// change direction to down and move
// xRel = relative coordinate changes
  void moveLeft(int xRel){
    changeDir(true, cwLeftMotor);
    changeDir(true, cwRightMotor);
    moveMotors(xRel);
  }
// change direction to left and move
// xRel = relative coordinate changes  
  void moveRight(int xRel){
    changeDir(false, cwLeftMotor);
    changeDir(false, cwRightMotor);
    moveMotors(xRel);
  }
// change direction to upward and move
// xRel = relative coordinate changes
  void moveUp(int xRel){
    changeDir(false, cwLeftMotor);
    changeDir(true, cwRightMotor);
    moveMotors(xRel);
  }
// change direction to downward and move
// xRel = relative coordinate changes
  void moveDown(int xRel){
    changeDir(true, cwLeftMotor);
    changeDir(false, cwRightMotor);
    moveMotors(xRel);
  }
//****************************************************************************************************

//move each motor impulseCount times  and change global coordinates : x and y
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
    if(digitalRead(cwLeftMotor) == true){
      if(digitalRead(cwRightMotor) == true){  //left(true) right(true) Left Move
        x--;
      }
      if(digitalRead(cwRightMotor) == false){  //left(true) right(false) Up Move
        y++;
      }
    }
    else{
      if(digitalRead(cwRightMotor) == true){  //left(false) right(true) Down Move
        y--;
      }
      if(digitalRead(cwRightMotor) == false){  //left(false) right(false) Right Move
        x++;
      }
    }
  }
//********************************************initial methods**************************
// change rotating direction to certain motor
// motorNumber - address for clk+ output
  void changeDir(boolean isCwPlus, int motorNumber){
    if(isCwPlus == true){
     digitalWrite(motorNumber, HIGH);
    }
    else{
      digitalWrite(motorNumber, LOW);
    }    
  }
  //send impulses to right motor  
  void moveLeftMotor(){
    digitalWrite(clkLeft, HIGH);
    delayMicroseconds(motorSpeed);
    digitalWrite(clkLeft, LOW);
    delayMicroseconds(motorSpeed);
  }
  //send impulses to right motor
  void moveRightMotor(){
      digitalWrite(clkRight, HIGH);
      delayMicroseconds(motorSpeed);
      digitalWrite(clkRight, LOW);
      delayMicroseconds(motorSpeed);
  }
  //***************************************************************************
  
    

  
