int mtrDir1 = 8;
int mtrRun1 = 9;
int mtrDir2 = 11;
int mtrRun2 = 10;
int l1 = 5;
int l2 = 6;
int l3 = 7;

int init_X = 0;
int init_y = 0;

int curr_orientation = 0;
int curr_X = 0;
int curr_Y = 0;

int target_X = 2;
int target_Y = 3;

int count = 0;

void setup() {
  pinMode(mtrDir1,OUTPUT);
  pinMode(mtrRun1,OUTPUT);
  pinMode(mtrDir2,OUTPUT);
  pinMode(mtrRun2,OUTPUT);
  pinMode(l1,INPUT);
  pinMode(l2,INPUT);
  pinMode(l3,INPUT);
  Serial.begin(9600);
}

void forward(){
  digitalWrite(mtrDir1, HIGH);
  digitalWrite(mtrDir2, HIGH);
  analogWrite(mtrRun1, 50);
  analogWrite(mtrRun2, 50); 
}
void back(){
  digitalWrite(mtrDir1, LOW);
  digitalWrite(mtrDir2, LOW);
  analogWrite(mtrRun2, 80);
  analogWrite(mtrRun1, 80); 
}

void stp(){
  digitalWrite(mtrDir1, HIGH);
  digitalWrite(mtrDir2, HIGH);
  analogWrite(mtrRun1, 0);
  analogWrite(mtrRun2, 0); 
}
void rotCCW(){
  digitalWrite(mtrDir1, HIGH);
  digitalWrite(mtrDir2, LOW);
  analogWrite(mtrRun1, 100);
  analogWrite(mtrRun2, 100); 
}
void rotCW(){
  digitalWrite(mtrDir1, LOW);
  digitalWrite(mtrDir2, HIGH);
  analogWrite(mtrRun1, 100);
  analogWrite(mtrRun2, 100); 
}

void leftTurn(){
  digitalWrite(mtrDir1, HIGH);
  digitalWrite(mtrDir2, HIGH);
  analogWrite(mtrRun1, 100);
  analogWrite(mtrRun2, 0); 
}

void rightTurn(){
  digitalWrite(mtrDir1, HIGH);
  digitalWrite(mtrDir2, HIGH);
  analogWrite(mtrRun1, 0);
  analogWrite(mtrRun2, 100); 
}

void smlLeftTurn(){
  digitalWrite(mtrDir1, HIGH);
  digitalWrite(mtrDir2, HIGH);
  analogWrite(mtrRun1, 60);
  analogWrite(mtrRun2, 10); 
}

void smlRightTurn(){
  digitalWrite(mtrDir1, HIGH);
  digitalWrite(mtrDir2, HIGH);
  analogWrite(mtrRun1, 10);
  analogWrite(mtrRun2, 60); 
}


void stateForward()
{
  if(digitalRead(l1) == 1 && digitalRead(l2) == 0 && digitalRead(l3) == 1)
    forward();
  else if(digitalRead(l1) == 1 && digitalRead(l2) == 1 && digitalRead(l3) == 0)
    rightTurn();
  else if(digitalRead(l1) == 1 && digitalRead(l2) == 0 && digitalRead(l3) == 0)
    smlRightTurn();
  else if(digitalRead(l1) == 0 && digitalRead(l2) == 1 && digitalRead(l3) == 1)
    leftTurn();
  else if(digitalRead(l1) == 0 && digitalRead(l2) == 0 && digitalRead(l3) == 1)
    smlLeftTurn();
  else if(digitalRead(l1) == 0 && digitalRead(l2) == 0 && digitalRead(l3) == 0)
  {
    stateUpdateForward();
    while(digitalRead(l1) == 0 && digitalRead(l2) == 0 && digitalRead(l3) == 0)
    {
      forward();
    }
  }
  else
    stp();
}

// i=0 => left; i=1 => right
void stateTurn(int i)
{
  //back();
  //delay(100);
  if(i == 0)
  {
    rotCCW();
    delay(500);
    while(!(digitalRead(l1) == 1 && digitalRead(l2) == 0 && digitalRead(l3) == 1))
      rotCCW();
    curr_orientation = curr_orientation + 1;
    if(curr_orientation > 3)
      curr_orientation = 0;
  }
  else
  {
    rotCW();
    delay(500);
    while(!(digitalRead(l1) == 1 && digitalRead(l2) == 0 && digitalRead(l3) == 1))
      rotCW();
    curr_orientation = curr_orientation - 1;
    if(curr_orientation < 0)
      curr_orientation = 4;
  }
}

int stateCheckBlock()
{
  while((digitalRead(l1) == 1 && digitalRead(l2) == 1 && digitalRead(l3) == 1) && count < 1000)
  {
    count++;
    stp();
  }
  if(count > 1000)
  {
    count = 0;
    return 1; 
  }
  else
  {
    count = 0;
    return 0; 
  }
}

void stateUpdateForward()
{
  if(curr_orientation == 0)
    curr_Y = curr_Y+1;
  else if(curr_orientation == 1)
    curr_X = curr_X+1;
  else if(curr_orientation == 2)
    curr_Y = curr_Y-1;
  else if(curr_orientation == 3)
    curr_X = curr_X-1;
}

void loop() 
{
  int i = 0;
  if(curr_X == target_X && curr_Y == target_Y)
  {
    stp();
    delay(5000);    
  }
  else{
    while((target_Y > curr_Y) && i == 0)
    {
      stateForward();
      i = stateCheckBlock();
    }
    if((target_X > curr_X) && !(digitalRead(l1) == 1 && digitalRead(l2) == 1 && digitalRead(l3) == 1))
    {
      stateTurn(0);
      while((target_X > curr_X) && !(digitalRead(l1) == 1 && digitalRead(l2) == 1 && digitalRead(l3) == 1))
      {
        stateForward();
      }
      stateTurn(1);
    }
  }
}

