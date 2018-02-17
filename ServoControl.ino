  //Any coordinats used in this program presented as impulses for step motors provided by servo driver.
  //Each motor (left and right) controlled by servodriver
  //Arduino controller sends signal to both servo drivers
  //leftmove = left(true) + right(true)
  //rightmove = left(false) + right(false)
  //upMove = left(false) + right(true)
  //downMove = left(true) + right(false)
  //*******************constants******************
  boolean grabbingStage;
  boolean movingToSellStage;
  int manualMove = 2;
  int releaseStop = 8;
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
  int secondLevelY = 18100;
  int thirdLevelY = 14500; 
  int fourthLevelY = 10600; 
  int fifthLevelY = 6700; 
  int sixthLevelY = 2800; 
  //**********************************************
  boolean isGripperPlaced;
  boolean permissionSet;
  boolean manualMode;
  int counterLap = 0;
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
  int lowerLevel;
  int higherLevel;
  int partsCount = 2;
  //*****************************************************************************************************
void setup() {
  // put your setup code here, to run once:
  pinMode(clkLeft, OUTPUT);
  pinMode(clkRight, OUTPUT);
  pinMode(cwLeftMotor, OUTPUT);
  pinMode(cwRightMotor, OUTPUT);
  pinMode(releaseStop, OUTPUT);
  pinMode(manualMove, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
//  
    firstMove();
    secondMove();
    checkManualMove();
}

  //void main(){
   // firstMove(); //move to part griping position
    //secondMove(); //move to part placing position
  //}
  
  //waits for permission to move, if gots permission, then moves to grabbing point 
  // then sends command to FX5U that gripper has been placed
  void firstMove(){
    //permissionListener();
   // if(permissionSet == true){
      //isGripperPlaced = false;
      moveToGrabbingPos();
      delay(1000);
      //isGripperPlaced = true;
      //permissionSet = false;
    //}
  }
  //waits for permission to move, if gots permission, then moves to placing point 
  // then sends command to FX5U that gripper has been placed
  void secondMove(){
    //movePart(checkColor());
    movePart(36);
  }
//    permissionListener();
//    if(permissionSet == true){
//      currentPartsCount = checkColor();
//      isGripperPlaced = false;
//      movePart(currentPartsCount);
//      isGripperPlaced = true;
//      permissionSet = false;
//    }
//  }
 //Listents if movement allowed
  void permissionListener(){
    int partCount;
    if(digitalRead(isAllowedToMove) == true){
      permissionSet = true;                    //if allowed to move
    }
    else{
       //do nothing
    }
  }
    //checks color or type of the part and set level1 and level2
  int checkColor(){
    if(typeA == true){
      if(typeB == true){
        //sort next 
        // certain part count
        redPartCount++;
        return redPartCount + 10;      //1* - means red part and cell depends on *      
      }
      else{
          //sort next
          transparentPartCount++;
          return transparentPartCount + 20;  //2* - means transparent part and cell depends on *
        }
    }
    else{
      if(typeB == true){
          metalPartCount++;  
          return metalPartCount + 30;    //   3* - means metal part and cell depends on * digit 
        }
      else{
          //sort next
        }
    }
  }
  int setLevels(int color){
    if(color < 20){    //RED COLOR
        lowerLevel = firstLevelY;
        higherLevel = secondLevelY;
        return 10;
    }
    else if(color < 30){    //TRANSPARENT COLOR
        lowerLevel = thirdLevelY;
        higherLevel = fourthLevelY;
        return 20;
    }
    else{    //TRANSPARENT COLOR
        lowerLevel = fifthLevelY;
        higherLevel = sixthLevelY;
        return 30;
    }
  }
//moves certain part to certain position
 void movePart(int color){
   int partCount = color - setLevels(color);
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

//******************main methods for movement*****************************************
  void getBackToInitial(){
      moveLeft(x);
      moveUp(y);
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

//move each motor impulseCount times  and change global coordinates : x and y
  void moveMotors(int impulseCount){
    //digitalWrite(releaseStop, HIGH);
      for(int i = 0; i < impulseCount; i++){
          checkManualMove();
              if(permissionSet = true){
                  moveLeftMotor();
                  moveRightMotor();
                  changeCoord();
              }
          }
   }

// changes x and y on one depending on rotation direction  
// x and y are absolute coordinates
  void changeCoord(){
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
  //method check manual move needs in case if you need to stop program and adjust grip to required posotion
  void checkManualMove(){
    if(digitalRead(manualMove) == HIGH){        //if button pressed and while pressed stop everything and do nothing
        while(digitalRead(manualMove) == HIGH){
            //do nothing
        }
        if(digitalRead(releaseStop) == HIGH){    //when button released: if stop has been released, than turn it on
            digitalWrite(releaseStop, LOW);
        }
        else{                                    // else turn it off and wait until it button pressed again
          digitalWrite(releaseStop, HIGH);
          while(digitalRead(manualMove) == LOW){
              //do nothing
          }
          while(digitalRead(manualMove) == HIGH){
              //do nothing
          }
          delay(300);          
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
  
    

  
