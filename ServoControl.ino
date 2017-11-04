  
  //leftmove = left(true) + right(true)
  //rightmove = left(false) + right(false)
  //upMove = left(false) + right(true)
  //downMove = left(true) + right(false)
  int x;
  int y;
  int clkLeft = 9;
  int clkRight = 10; 
  int cwLeftMotor = 11;
  int cwRightMotor = 12;
  
void setup() {
  // put your setup code here, to run once:
  pinMode(clkLeft, OUTPUT);
  pinMode(clkRight, OUTPUT);
  pinMode(cwLeftMotor, OUTPUT);
  pinMode(cwRightMotor, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
    moveDown(3000);
    moveRight(1000);
    moveDown(500);
    moveRight(1200);
    getBackToInitial();
}
//******************main methods for movement**************
  void getBackToInitial(){
      moveLeft(x);
      moveUp(y);
  }
// change direction to down and move
// xRel = relative coordinate changes
  void moveLeft(int xRel){
    changeDir(true, cwLeftMotor);
    changeDir(true, cwRightMotor);
    move(xRel);
  }
// change direction to left and move
// xRel = relative coordinate changes  
  void moveRight(int xRel){
    changeDir(false, cwLeftMotor);
    changeDir(false, cwRightMotor);
    move(xRel);
  }
// change direction to upward and move
// xRel = relative coordinate changes
  void moveUp(int xRel){
    changeDir(false, cwLeftMotor);
    changeDir(true, cwRightMotor);
    move(xRel);
  }
// change direction to downward and move
// xRel = relative coordinate changes
  void moveDown(int xRel){
    changeDir(true, cwLeftMotor);
    changeDir(false, cwRightMotor);
    move(xRel);
  }
//***************************************************************  
//move each motor impulseCount times  and change global coordinates : x and y
  void move(int impulseCount){
    for(int i = 0; i < impulseCount; i++){
        moveLeftMotor();
        moveRightMotor();
        changeCoord();
    }
  }
// changes x and y on one depending on rotation direction   
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
    delayMicroseconds(50);
    digitalWrite(clkLeft, LOW);
    delayMicroseconds(50);
  }
  //send impulses to right motor
  void moveRightMotor(){
      digitalWrite(clkRight, HIGH);
      delayMicroseconds(50);
      digitalWrite(clkRight, LOW);
      delayMicroseconds(50);
  }
  
    

  
