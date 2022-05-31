float v0, v1, v2, v3;
float Rtest, Rrefer = 100;
bool waitsInput0 = true, waitsInput1 = true, waitsInput2 = true, waitsInput3 = true,diesis = true;
int C=0,D=1,E=2,F=3,G=4,A=5,B=6;
int tr_down = 290;
int tr_up = 600;
int inputPin = 4;
int tr_up_7 = 15;
int tr_middle_1 = 280;
int tr_middle_2 = 300;
int pinValue;

void setup() {
  Serial.begin(9600);
}
void loop() {
  pinValue = digitalRead(inputPin);
  v0 = analogRead(A0);
  v1 = analogRead(A1);
  v2 = analogRead(A2);
  v3 = analogRead(A3);

  if(pinValue == 1 && !diesis){
    Serial.print('t');
    diesis = true;
  }
  else if(diesis && pinValue==0){
    diesis = false;
    Serial.print('f');
    
  }
 
  if(v0>=tr_up && waitsInput0){
    Serial.print(C);
    waitsInput0 = false;
  }
  else if(v0<=tr_down && waitsInput0){
    Serial.print(D);
    waitsInput0 = false;
  }
  else if(!waitsInput0 && v0 > tr_down && v0 < tr_up){
    Serial.print('a');
    waitsInput0 = true;
  }

  if(v1>=tr_up && waitsInput1){
    Serial.print(E);
    waitsInput1 = false;
  }
  else if(v1<=tr_down && waitsInput1){
    Serial.print(F);
    waitsInput1 = false;
  }
  else if(!waitsInput1 && v1 > tr_down && v1 < tr_up) {
    Serial.print('b');
    waitsInput1 = true;
  }

  if(v2>=tr_up && waitsInput2){
    Serial.print(G);
    waitsInput2 = false;
  }
  else if(v2<=tr_down && waitsInput2){
    Serial.print(A);
    waitsInput2 = false;
  }
  else if(!waitsInput2 && v2 > tr_down && v2 < tr_up){
    Serial.print('c');
    waitsInput2 = true;
  }

  if(v3<=tr_up_7 && waitsInput3){
    Serial.print(B);
    waitsInput3 = false;
  }
  else if(v3 > tr_up_7 && !waitsInput3) {
    Serial.print('d');
    waitsInput3 = true;
  }

  delay(1);


}
