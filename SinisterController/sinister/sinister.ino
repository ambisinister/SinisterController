#include "Nintendo.h"
/* Requires Nicohood
 * Sinister Controller - by Eryk Banatt
 * Loosely based upon SimpleControllers' work
 */
 
CGamecubeConsole GamecubeConsole(12);
Gamecube_Data_t d = defaultGamecubeData;

//This is needed
CGamecubeController GamecubeController1(7); 

//This is the pinout
const int A = 3;
const int B = 4;
const int X = 5;
const int Y = 6;
const int Z = 8;
const int START = 11;
const int R = 9;
const int L = 10;
const int Ax = A5;
const int Ay = A4;
const int Cx = A3;
const int Cy = A2;

//Value of big lightshield (TODO: test)
const int RLIGHTv = 74;

int lastxinput = 128; //previous poll's x axis inputs, for backdash fix


void setup()
{
  pinMode(A, INPUT_PULLUP);
  pinMode(B, INPUT_PULLUP);
  pinMode(X, INPUT_PULLUP);
  pinMode(Y, INPUT_PULLUP);
  pinMode(Z, INPUT_PULLUP);
  pinMode(START, INPUT_PULLUP);
  pinMode(R, INPUT_PULLUP);
  pinMode(L, INPUT_PULLUP);
  pinMode(Ax, INPUT_PULLUP);
  pinMode(Ay, INPUT_PULLUP);
  pinMode(Cx, INPUT_PULLUP);
  pinMode(Cy, INPUT_PULLUP);
  GamecubeController1.read();
}

void loop()
{
  //zeros out
  int pinA = 0;
  int pinB = 0;
  int pinX = 0;
  int pinY = 0;
  int pinZ = 0;
  int pinSTART = 0;
  int pinR = 0;
  int pinL = 0;
  int pinRlight = 0;
  int pinxAxisC = 128;
  int pinyAxisC = 128;
  int pinxAxis = 128;
  int pinyAxis = 128;
  
  /*
   * TODO:
   *  Shield Dropping
   */

  //digital buttons
  if (digitalRead(A) == LOW)pinA = 1;
  if (digitalRead(B) == LOW)pinB = 1;
  if (digitalRead(X) == LOW)pinX = 1;
  if (digitalRead(Y) == LOW)pinY = 1;
  if (digitalRead(Z) == LOW)pinZ = 1;
  if (digitalRead(START) == LOW)pinSTART = 1;
  if (digitalRead(R) == LOW)pinR = 1;
  if (digitalRead(L) == LOW)pinL = 1;
  if ((digitalRead(L) == LOW) && (digitalRead(R) == LOW)){
    pinR = 0;
    pinL = 0;
    pinRlight = RLIGHTv;
  }

  //analog
  
  //dead zones (TODO test these)
  int xpinraw = analogRead(Ax); //optimization (one pin read, rather than many)
  int xpinconv = xpinraw / 4;
  int ypinraw = analogRead(Ay);
  int ypinconv = ypinraw / 4;
  int xpinrawC = analogRead(Cx);
  int xpinconvC = xpinrawC / 4;
  int ypinrawC = analogRead(Cy);
  int ypinconvC = ypinrawC / 4;

  if (xpinconvC > 106 && xpinconvC < 170){
    pinxAxisC = 128;
  }
  else{
    pinxAxisC = xpinconvC;
  }
  if (ypinconvC > 106 && ypinconvC < 170){
    pinyAxisC = 128;
  }
  else{
    pinyAxisC = ypinconvC;
  }
  if (xpinconv > 106 && xpinconv < 170){
    pinxAxis = 128;
  }
  else if (((xpinconv <= 106 && xpinconv >= 65) || (xpinconv >= 170 && xpinconv <= 225))){ //test these values please
    
    /* Backdash fix:
     * if previous frame was in deadzone and this frame is in tilt zone, and no buttons are being pressed, return deadzone value
     */
     
    if (lastxinput > 86 && lastxinput < 170) {
      //if any button or C stick, do not drop input
      if(pinA == 1 || pinB == 1 || pinX == 1 || pinY == 1 || pinZ == 1 || pinR == 1 || pinL == 1){
        pinxAxis = xpinconv;
      }
      else{
        pinxAxis = 128;SS
      }
    }
    else{
      pinxAxis = xpinconv;
    }
  }
  else{
    pinxAxis = xpinconv;
  }
  if (ypinconv > 106 && ypinconv < 170){
    pinyAxis = 128;
  }
  else{
    pinyAxis = ypinconv;
  }

  lastxinput = xpinconv;


  //reports data
  d.report.a = pinA;
  d.report.b = pinB;
  d.report.x = pinX;
  d.report.y = pinY;
  d.report.z = pinZ;
  d.report.start = pinSTART;
  d.report.r = pinR;
  d.report.l = pinL;
  d.report.left = pinRlight;
  d.report.xAxis = pinxAxis;
  d.report.yAxis = 128;
  d.report.cxAxis = 128;
  d.report.cyAxis = 128;
  GamecubeConsole.write(d);

}
