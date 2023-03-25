/*
commencé le 31 mai 2021
après que l'écran ait prouvé qu'il fonctionne et soit lisible
on va tester l'ensemble des boutons et surtout leur combinaisons
ceci fera partie du programme de débugage des nouveaux piccolo construits
il faudra en effet tester les pushbuttons et encodeur rotatifs à la source

dans un second temps, on ajoutera encodeurs rotatis et le slide
je prévois un peu de débogage pour trouver les bonnes valeurs de résistance du slider

après tout cela, je vais implémenter l'ancienne version de Piccolo (Tambour) dans ce nouveau cadre

5 juin : essai de remplacement des 220 ohm par des 10K : je réalise en fait que ça fonctionne SANS les résistances !
par contre, dès le début de fc des pushbuttons, arrêt des encodeurs !

*/

#include <Arduino.h>
#include <Encoder.h>
#include <affichage_functions.h>
// #include <DmxSimple.h>
#include <TeensyDMX.h>
#include <MIDI.h>
#include <bounce.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif

#include "SdFat.h"

MIDI_CREATE_INSTANCE(HardwareSerial, Serial3, MIDI3);

// DEFINITION De TOUS LES PINS : une bonne habitude à prendre si on pense qu'on gardera pas toujours le même support

// MIDI IN et OUT
int pinRx3 = 7;
int pinTx3 = 8;

// CREATION DU TIMER F8
IntervalTimer myTimerF8;
// CREATION DU TIMER Screen
IntervalTimer myTimerScreen;
// CREATION DU TIMER Screen
IntervalTimer myTimerEncoder;

long time2checkEncoder = 1000000; // soit 0,5 seconde
int multiplyRatio = 5;

bool readyToUpdateEncoder = true;

int oldHardwareA = -1;
int olderHardwareEncoderA = -1;
int differenceHardware = 0;
int absoluteDifferenceHardware = 0;
int stepSize = 1;
int gapOfStepsToAccelerate = 10;

long softwareValueEncoder = 0;
long oldSoftwareA = -1;
long olderSoftwareEncoderA = -1;
long differenceSoftware = 0;
long absoluteDifferenceSoftware = 0;
long minimumSoftwareValueEncoder = 0;
long maximumSoftwareValueEncoder = 10000;

int measuredHighest = 800;
int measuredLowest = 100;

namespace teensydmx = ::qindesign::teensydmx;
teensydmx::Sender dmxTx{Serial7};

String display = "" ; 
 

// DEFINITION DES PINS, ENCODERS et BOUTONS BLACK 03 - 2 juin 2021


// BOUTONS

const int pinButtonSave = 0 ; 
const int pinButtonLoad = 1 ;
const int pinButtonKarakt = 2;
const int pinButtonMemory = 3;
const int pinButtonTap = 4 ; 
const int pinButtonOK = 5 ;
const int pinButtonUndo = 6 ;
const int pinButtonPad8 = 7 ;

const int pinButtonPad7 = 8;
const int pinButtonPad6 = 9;
const int pinButtonPad5 = 10;
const int pinButtonDup2 = 11;
const int pinButtonDup1 = 12;
const int pinButtonMute = 13;

const int pinMidiOut1 = 14 ; 
const int pinMidiIn1 = 15;
const int pinButtonCtrl = 16;
const int pinMidiOut2 = 17;
const int pinOled1 = 18;
const int pinOled2 = 19;
const int pinButtonAlt = 20;
const int pinSlidePotentiometer = 21 ; 

// ENCODEURS ROTATIFS SIMPLES

const int pinRotaryUpA = 22 ;      // ROTARY UP
const int pinRotaryUpB = 23;

const int pinRotaryRightA = 24; // ROTARY RIGHT
const int pinRotaryRightB = 25;

const int pinRotaryLeftA = 26; // ROTARY LEFT
const int pinRotaryLeftB = 27;

// BOUTONS LA SUITE 

const int pinDMXin = 28 ;
const int pinDMXout = 29 ; 

const int pinButtonDel = 30 ;
const int pinCTX3 = 31 ; 
const int pinButtonShift = 32 ; 
const int pinButtonJump = 33 ;
 const int pinButtonPad4 = 34 ; 
 const int pinButtonPad3 = 35 ;
 const int pinButtonPad2 = 36 ; 
 const int pinButtonPad1 = 37 ;
 const int pinButtonLayer4 = 38 ; 
 const int pinButtonLayer3 = 39 ;
 const int pinButtonLayer2 = 40; 
 const int pinButtonLayer1 = 41 ; 

// const int pinButtonSave = 57;

Encoder rotKnobUp(pinRotaryUpA, pinRotaryUpB);
Encoder rotKnobLeft(pinRotaryLeftA , pinRotaryLeftB);
Encoder rotKnobRight(pinRotaryRightA , pinRotaryRightB);


/*
 Encoder rotKnobUp(A8, A9);
 Encoder rotKnobLeft(A10, A11);
 Encoder rotKnobRight(A12, A13);
*/

Bounce myButtonMemory(pinButtonMemory, 10);
Bounce myButtonSave(pinButtonSave, 10);
Bounce myButtonLoad(pinButtonLoad, 10);
Bounce myButtonKarakt(pinButtonKarakt, 10);

Bounce myButtonOK(pinButtonOK, 10);
Bounce myButtonTap(pinButtonTap, 10);
Bounce myButtonJump(pinButtonJump, 10);
Bounce myButtonUndo(pinButtonUndo, 10);

// procédures "maison"
//

void updateEncoder(void)
{
  readyToUpdateEncoder = true;
}

void addF8(void)
{
  readyToSendF8 = true;
}

void updateScreen(void)
{
  readyToUpdateScreen = true;
}


void setup(void) // INITIAL SETUP
{

  //   pinMode(RES, OUTPUT);            // Initializes Arduino pin for the Reset line (optional)
  //   digitalWrite(RES, HIGH);         // Sets HIGH the Reset line of the display (optional, can be always high)
  delayMicroseconds(200); // Waits 200 us for stabilization purpose
  Wire.begin();           // Initiate the Wire library and join the I2C bus as a master
  Wire.setSCL(19);
  Wire.setSDA(18);

  delay(10); // Waits 10 ms for stabilization purpose

  if (ROW_N == 2 || ROW_N == 4)
    rows = 0x08; // Display mode: 2/4 lines
  else
    rows = 0x00; // Display mode: 1/3 lines

  // ADDIND I2C Scanner
  /*
{
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for (address = 1; address < 127; address++)
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");

      nDevices++;
    }
    else if (error == 4)
    {
      Serial.print("Unknow error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");

}
*/

  command(0x22 | rows); // Function set: extended command set (RE=1), lines #
  command(0x71);        // Function selection A:

  //  data(0x5C);           //  enable internal Vdd regulator at 5V I/O mode (def. value) (0x00 for disable, 2.8V I/O)
  data(0x00); //  enable internal Vdd regulator at 5V I/O mode (def. value) (0x00 for disable, 2.8V I/O)

  command(0x20 | rows); // Function set: fundamental command set (RE=0) (exit from extended command set), lines #
  command(0x08);        // Display ON/OFF control: display off, cursor off, blink off (default values)
  command(0x22 | rows); // Function set: extended command set (RE=1), lines #

  command(0x79); // OLED characterization: OLED command set enabled (SD=1)
  command(0xD5); // Set display clock divide ratio/oscillator frequency:
  command(0x70); //  divide ratio=1, frequency=7 (default values)
  command(0x78); // OLED characterization: OLED command set disabled (SD=0) (exit from OLED command set)

  if (ROW_N > 2)
    command(0x09); // Extended function set (RE=1): 5-dot font, B/W inverting disabled (def. val.), 3/4 lines
  else
    command(0x08); // Extended function set (RE=1): 5-dot font, B/W inverting disabled (def. val.), 1/2 lines

  command(0x06);        // Entry Mode set - COM/SEG direction: COM0->COM31, SEG99->SEG0 (BDC=1, BDS=0)
  command(0x72);        // Function selection B:
  data(0x0A);           //  ROM/CGRAM selection: ROM C, CGROM=250, CGRAM=6 (ROM=10, OPR=10)
  command(0x79);        // OLED characterization: OLED command set enabled (SD=1)
  command(0xDA);        // Set SEG pins hardware configuration:
  command(0x10);        //  alternative odd/even SEG pin, disable SEG left/right remap (default values)
  command(0xDC);        // Function selection C:
  command(0x00);        //  internal VSL, GPIO input disable
  command(0x81);        // Set contrast control:
  command(0x10);        //  contrast value
  command(0xD9);        // Set phase length:
  command(0xF1);        //  phase2=15, phase1=1 (default: 0x78)
  command(0xDB);        // Set VCOMH deselect level:
  command(0x40);        //  VCOMH deselect level=1 x Vcc (default: 0x20=0,77 x Vcc)
  command(0x78);        // OLED characterization: OLED command set disabled (SD=0) (exit from OLED command set)
  command(0x20 | rows); // Function set: fundamental command set (RE=0) (exit from extended command set), lines #
  command(0x01);        // Clear display
  delay(2);             // After a clear display, a minimum pause of 1-2 ms is required
  command(0x80);        // Set DDRAM address 0x00 in address counter (cursor home) (default value)
  command(0x0C);        // Display ON/OFF control: display ON, cursor off, blink off
  delay(250);           // Waits 250 ms for stabilization purpose after display on

  if (ROW_N == 2)
    new_line[1] = 0xC0; // DDRAM address for each line of the display (only for 2-line mode)

  myTimerF8.begin(addF8, 10000);
  myTimerScreen.begin(updateScreen, 15000); // = 30 millisecondes, parfait pour la persistance rétinienne
  myTimerEncoder.begin(updateEncoder, time2checkEncoder ); // = 1 seconde

  defineSlot(1, 0, 0);  // la première zone d'affichage sera 0,0
  defineSlot(2, 5, 2);  // la deuxième correspond à l'affichage des effets
  defineSlot(3, 0, 1);  // la troisième pour les valeurs des transfos
  defineSlot(4, 10, 3); // pour les transformations
  defineSlot(5, 0, 3);  // pour les MUTE et STEPZ
  defineSlot(6, 0, 3);  // pour le déroulement

//  pinMode(A0, INPUT);

/*
  pinMode(pinButtonMemory, INPUT);
  digitalWrite(pinButtonMemory, LOW);
  pinMode(pinButtonSave, INPUT);
  digitalWrite(pinButtonSave, LOW);
  pinMode(pinButtonLoad, INPUT);
  digitalWrite(pinButtonLoad, LOW);
  pinMode(pinButtonKarakt, INPUT);
  digitalWrite(pinButtonKarakt, LOW);

  pinMode(pinButtonOK, INPUT);
  digitalWrite(pinButtonOK, LOW);
  pinMode(pinButtonTap, INPUT);
  digitalWrite(pinButtonTap, LOW);
  pinMode(pinButtonJump, INPUT);
  digitalWrite(pinButtonJump, LOW);
  pinMode(pinButtonUndo, INPUT);
  digitalWrite(pinButtonUndo, LOW);
*/


for ( int i = 0 ; i < 60 ; i++ )
{
   if ( i != 18 && i != 19 && ( i < 22 || i > 27 ))
  //  if (i != 18 && i != 19  )

    {
      pinMode(i, INPUT);
      digitalWrite(i, LOW);
  }
}

  Serial.begin(9600);

  dmxTx.set(1, 128);
  dmxTx.begin();

}
// _______________________________________________________________________________________

unsigned int marginMap ( unsigned int initial, byte percent, unsigned int lowTarget, unsigned int highTarget )
{
// on part de valeurs allant de 0 à 1024 : analogRead 
// et on doit arriver aux valeurs target, en sachant qu'une marge des valeurs donne min et max

long initialHighest = measuredHighest - ((long)percent * measuredHighest / 100);
long initialLowest = measuredLowest + ((long)percent * measuredHighest / 100);

if ( initial >= initialHighest ) {return highTarget ; }
if ( initial <= initialLowest ) { return lowTarget ; }

// unsigned int result = ((highTarget - lowTarget) * (initial - initialLowest)) / (initialHighest - initialLowest);
unsigned int result = map(initial, initialLowest, initialHighest, lowTarget, highTarget);

return result ; 
}

void loop(void) // MAIN PROGRAM
{


myButtonOK.update();
 if (myButtonOK.risingEdge() )
 {
   writeStringAtXY("OK", 1, 0);
   printRot2("OK", 9, 400, 5);
 //  delay(5000);
 }

 myButtonMemory.update();
 if (myButtonMemory.risingEdge())
 {
   writeStringAtXY("Memory", 1, 0);
   printRot2("Memory", 9, 400, 5);
  // delay(5000);
 }

  /*
  int stepSize = 1;
  int gapOfStepsToAccelerate = 10;

  long softwareValueEncoder = 0;
  long OldSoftwareValueEncoder = 0;
  long OlderSoftwareValueEncoder = 0;
  long minimumSoftwareValueEncoder = 0;
  long maximumSoftwareValueEncoder = 10000;
*/

  

  
  int a = rotKnobRight.read() / 4;

  if (a != oldHardwareA)
  {
    differenceHardware = a - oldHardwareA;

    softwareValueEncoder = softwareValueEncoder + differenceHardware * stepSize;

    if (softwareValueEncoder < minimumSoftwareValueEncoder)
    {
      softwareValueEncoder = minimumSoftwareValueEncoder;
      stepSize = 1;
    }

    if (softwareValueEncoder > maximumSoftwareValueEncoder)
    {
      softwareValueEncoder = maximumSoftwareValueEncoder;
      stepSize = 1;
    }

    oldHardwareA = a;
    oldSoftwareA = softwareValueEncoder ; 
  }

  writeStringAtXY(String(a) + " ", 1, 0);
  writeStringAtXY(String(softwareValueEncoder) + " ", 1, 1);



  // compléter avec un writeChar, SANS string du tout
  // writeInt

  // travailler l'encoder dynamique

  // TIMER UPDATE ENCODER
  bool changeEncoder = false;

  noInterrupts();
  changeEncoder = readyToUpdateEncoder;
  readyToUpdateEncoder = false;
  interrupts();

  if (changeEncoder == true) // cela veut dire que le timer a décidé qu'il fallait vérifier si l'encodeur a bougé 
  // on va donc savoir de combien il a bougé durant le temps spécifié par timeToCheckEncoder ( initialement 1 seconde )
  {
    differenceHardware = oldHardwareA - olderHardwareEncoderA;
    absoluteDifferenceHardware = abs(differenceHardware);
    writeStringAtXY(String(differenceHardware) + "   ", 8, 0);

    differenceSoftware = oldSoftwareA - olderSoftwareEncoderA;
    absoluteDifferenceSoftware = abs(differenceSoftware);
    writeStringAtXY(String(differenceSoftware) + "   ", 8, 1);

    if (absoluteDifferenceSoftware > (gapOfStepsToAccelerate * stepSize))
    {
      stepSize = stepSize * multiplyRatio ; 
      if  (stepSize > maximumSoftwareValueEncoder ) { stepSize = maximumSoftwareValueEncoder ; }
    }

    if (absoluteDifferenceSoftware < (stepSize)   )
    {
      stepSize = stepSize / multiplyRatio ;

      if ( stepSize < 1 ) {stepSize = 1 ; }
    }

    writeStringAtXY(String(stepSize) + "   ", 12, 1);

    olderHardwareEncoderA = oldHardwareA;
    olderSoftwareEncoderA = oldSoftwareA;
  }
 
  // TIMER F8
  bool copyF8 = false;

  noInterrupts();
  copyF8 = readyToSendF8;
  readyToSendF8 = false;
  interrupts();

  if (copyF8 == true && sendF8 == true)
  {
    Serial3.write((byte)0xF8);
  }

  // TIMER UPDATE SCREEN
  bool copyScreen = false;

  noInterrupts();
  copyScreen = readyToUpdateScreen;
  readyToUpdateScreen = false;
  interrupts();

  if (copyScreen == true)
  {
    // vérification de l'écran et mise à jour

    // choix d'un numéro entre min et max
    // int choixBuffer = random(0, tailleEcran );  // choisit un point de départ
    int choixBuffer = 0; // point de départ nul

    for (int t = 0; t < tailleEcran; t++)
    {
      int u = (choixBuffer + t) % tailleEcran;
      int newX = u % sizeX;
      int newY = u / sizeX;

      if (bufferWrite[newX][newY] != bufferDisplay[newX][newY])
      {
        buffer_and_HARD_setCursor(newX, newY);
        buffer_and_HARD_print(bufferWrite[newX][newY]);
        //buffer_and_HARD_setCursor(5, 5);
        //buffer_and_HARD_print('*');

        bufferDisplay[newX][newY] = bufferWrite[newX][newY];
        t = tailleEcran; // on met fin à la routine, càd une mise à jour à la fois
      }
    }
  }

printRot2(display,6,200,2);
printRot2(display, 10, 400, 4);

long valueFader ;

valueFader =  analogRead(A0)   ;

if ( valueFader < measuredLowest ) { measuredLowest = valueFader ; }
if (valueFader > measuredHighest)
{
  measuredHighest = valueFader;
}

/* 
Serial.print(valueFader);
Serial.print(" [");
Serial.print(measuredLowest);
Serial.print("-");
Serial.print(measuredHighest);
Serial.print("] ");

Serial.print(String(rotKnobUp.read()) + "/" + String(rotKnobLeft.read()) + "/" + String(rotKnobRight.read()));
Serial.println();
 */ 

// if ( true == false )
for ( int i = 0 ; i < 50 ; i++ )
{
  int a = digitalRead(i);
//   Serial.print(a); 
  if ( i%5 == 0 )
  { 
   // Serial.print(" "); 
    }
}
// Serial.print("\r"); 
Serial.println(); 

int a_ = rotKnobLeft.read();
int b_ = rotKnobRight.read();
int c_ = rotKnobUp.read();

Serial.print(a_); Serial.print(" + ");

Serial.print(b_);
Serial.print(" + ");
Serial.print(c_);
Serial.println (" // ");

valueFader = marginMap(valueFader,10,0,127);

valueFader = 127 - valueFader ; 

// printRot2("Fader=" + String(valueFader) + "--", 9, 400, 5);

if ( valueFader > 60 ) 
{ display = "Haut" ; }
else
{ display = "Tournez Tournai" ; }

nowMicros = micros();
nowMillis = nowMicros / 1000;

// DmxSimple.write(1, rand()%100 );

dmxTx.set(2, rand() % 255 );
dmxTx.set(5, rand() % 255);
dmxTx.set(6, rand() % 255);
dmxTx.set(7, rand() % 255);
dmxTx.set(8, rand() % 255);
dmxTx.set(9, rand() % 255);
// delay(5000);

dmxTx.set(4, 0);
// delay(5000);

}

/*
buffer_setCursor(byte x, byte y)
buffer_and_HARD_setCursor(byte x, byte y)
buffer_print(String Texte)
buffer_and_HARD_print(String Texte)
 buffer_clearScreen
 buffer_clearScreen_HARD(void
 buffer_clearLine(byte A)
 buffer_clearLine_HARD(byte A)

 cursorX = x;
	cursorY = y;

  bufferWrite
  bufferDisplay


}


*/
