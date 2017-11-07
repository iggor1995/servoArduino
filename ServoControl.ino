  
  //Any coordinats used in this program presented as impulses for step motors provided by servo driver.
  //Each motor (left and right) controlled by servodriver
  //Arduino controller sends signal to both servo drivers
  //leftmove = left(true) + right(true)
  //rightmove = left(false) + right(false)
  //upMove = left(false) + right(true)
  //downMove = left(true) + right(false)
  //*******************constants******************
  int clkLeft = 9;
  int clkRight = 10; 
  int cwLeftMotor = 11;
  int cwRightMotor = 12;
  int maxPartsOnLevel = 8;
  int maxPartsInRow = 4;
  int motorSpeed = 50;
  int grabbingPartXCoord = 1500;
  int grabbingPartYCoord = 24000;
  int xFirstCellPos = 800;
  int xDelta = 2200;
  int firstLevelY = 21700;
  int secondLevelY = 18100;// = 
  int thirdLevelY = 14500;// = 
  int fourthLevelY = 10600;// = 
  int fifthLevelY = 6700;// = 
  int sixthLevelY = 2800;// = 
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
  int x = 0;
  int y = 0;
  // level1 and level2 and partsCount are changed due to type of part. initialized in checkColor() method
  int lowerLevel = firstLevelY;
  int higherLevel = secondLevelY;
  int partsCount = 2;
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
  movePart(2);
  moveUp(5000);
  movePart(5);
  moveUp(5000);
  movePart(10);
  moveUp(5000);
}
 //Listents if movement allowed
  void commandListener(){
    int partCount;
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
      yMoveToPos(lowerLevel);
    } 
    else{
      yMoveToPos(higherLevel);
      partCount = partCount - maxPartsInRow;
    }
    xMoveToPos(xFirstCellPos); // move to first sell int row
    if(partCount < maxPartsInRow){
      delta = xDelta * partCount;
    }
    else{
      delta = xDelta * (partCount - maxPartsInRow);
    }
    moveRight(delta);
  }
// moves grabber to grabbing pos, where part has been set
  void moveToGrabbingPos(){
    xMoveToPos(grabbingPartXCoord);
    yMoveToPos(grabbingPartYCoord);
  }
// move grabber to absolute position by x coordinates
  void xMoveToPos(int pos){
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
    int moveY;
    if(pos > y){
      moveY = pos - y;
      moveDown(moveY);
    }
    else{
      moveY = y - pos;
      moveUp(moveY);
    }    
  }
  //checks color or type of the part and set level1 and level2
  int checkColor(){
    if(typeA == true){
      if(typeB == true){
        //sort next 
      // certain part count     
      }
      else{
          //sort next
        }
    }
    else{
      if(typeB == true){
        //sort next
      }
      else{
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
    delay(10);
  }
// change direction to left and move
// xRel = relative coordinate changes  
  void moveRight(int xRel){
    changeDir(false, cwLeftMotor);
    changeDir(false, cwRightMotor);
    moveMotors(xRel);
    delay(10);
  }
// change direction to upward and move
// xRel = relative coordinate changes
  void moveUp(int xRel){
    changeDir(false, cwLeftMotor);
    changeDir(true, cwRightMotor);
    moveMotors(xRel);
    delay(10);
  }
// change direction to downward and move
// xRel = relative coordinate changes
  void moveDown(int xRel){
    changeDir(true, cwLeftMotor);
    changeDir(false, cwRightMotor);
    moveMotors(xRel);
    delay(10);
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
      else{  //left(true) right(false) Up Move
        y++;
      }
    }
    else{
      if(digitalRead(cwRightMotor) == true){  //left(false) right(true) Down Move
        y--;
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
  
    

  
