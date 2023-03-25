/*



PPPPPPPPPPPPPPPPP     iiii                                                           lllllll                              444444444
P::::::::::::::::P   i::::i                                                          l:::::l                             4::::::::4
P::::::PPPPPP:::::P   iiii                                                           l:::::l                            4:::::::::4
PP:::::P     P:::::P                                                                 l:::::l                           4::::44::::4
  P::::P     P:::::Piiiiiii     cccccccccccccccc    cccccccccccccccc   ooooooooooo    l::::l    ooooooooooo           4::::4 4::::4      cccccccccccccccc
  P::::P     P:::::Pi:::::i   cc:::::::::::::::c  cc:::::::::::::::c oo:::::::::::oo  l::::l  oo:::::::::::oo        4::::4  4::::4    cc:::::::::::::::c
  P::::PPPPPP:::::P  i::::i  c:::::::::::::::::c c:::::::::::::::::co:::::::::::::::o l::::l o:::::::::::::::o      4::::4   4::::4   c:::::::::::::::::c
  P:::::::::::::PP   i::::i c:::::::cccccc:::::cc:::::::cccccc:::::co:::::ooooo:::::o l::::l o:::::ooooo:::::o     4::::444444::::444c:::::::cccccc:::::c
  P::::PPPPPPPPP     i::::i c::::::c     cccccccc::::::c     ccccccco::::o     o::::o l::::l o::::o     o::::o     4::::::::::::::::4c::::::c     ccccccc
  P::::P             i::::i c:::::c             c:::::c             o::::o     o::::o l::::l o::::o     o::::o     4444444444:::::444c:::::c
  P::::P             i::::i c:::::c             c:::::c             o::::o     o::::o l::::l o::::o     o::::o               4::::4  c:::::c
  P::::P             i::::i c::::::c     cccccccc::::::c     ccccccco::::o     o::::o l::::l o::::o     o::::o               4::::4  c::::::c     ccccccc
PP::::::PP          i::::::ic:::::::cccccc:::::cc:::::::cccccc:::::co:::::ooooo:::::ol::::::lo:::::ooooo:::::o               4::::4  c:::::::cccccc:::::c
P::::::::P          i::::::i c:::::::::::::::::c c:::::::::::::::::co:::::::::::::::ol::::::lo:::::::::::::::o             44::::::44 c:::::::::::::::::c
P::::::::P          i::::::i  cc:::::::::::::::c  cc:::::::::::::::c oo:::::::::::oo l::::::l oo:::::::::::oo              4::::::::4  cc:::::::::::::::c
PPPPPPPPPP          iiiiiiii    cccccccccccccccc    cccccccccccccccc   ooooooooooo   llllllll   ooooooooooo                4444444444    cccccccccccccccc

Copyright (C) 2016-2023 sylvain van iniitu - All rights reserved.
www.iniitu.net 
   This program is free software; you can redistribute it and/or
   modify it under the terms of the MIT License

*/

// TODO

// avant Versin RESIDENZ
// J'ai sacrifié GetNameTest pour Comb2 ! il faut restaurer ça pour Teensy !

// version qui effectue un pas en arrière concernant le CCMod
// alors que j'avais développé une version avec pointeurs, je retourne ici vers un array

// la valeur réelle est affichée en softwareValueEncoderRight
// celle ci est transmise à la transformation
// pour le random de Event, il faudra prévoir qu'il balance entre min et max de la transformation prévue
// vérifier l'affichage de ligne2 dans trasnformation 134 : choix de valeur min et valeur max de Event Transfo

// QUAND NOUVELLE VARIABLE INFLUENCANT LE JEU ET DEVANT ETRE SAUVEE SOUS KARAKT
// AJOUTER AU SAVE
// AJOUTER AU LOAD ( à partir de la ligne précédent )
// AJOUTER AU SAVE TO MEM
// AJOUTE AU LOAD FROM MEM

// 1
//  aller dans saveCurrentandMemKarakt() dans SaveLoadLocal.h
//  on rajoute en bas de cette fonction des _printToKarakt avec des chiffres consécutifs, ainsi que le nom de variable et description
//  faire un compile pour vérifier que OK

// 2
//  un peu plus bas, dans le même fichier, même fonction recopier les nouvelles lignes et modifier en fromKMem_int_printToKarakt
//  re-compiler

// 3
//  aller dans la fonction loadLocal2, même fichier
//  ajouters les nouvelles lignes et modifier en    int_loadFromKarakt(128, &keepStepzBUG );
//  re-compiler

// 5
//  recopier ces mêmes lignes EXACTEMENT sans rien modifer dans la fonction loadKarakt_from_KMem
//  re-compiler

// 4
//  aller ensuite dans le fichier karaktPile.h
//  trouver la fonction saveKarakt_to_KMem
//  y recopier les mêmes lignes juste modifée et en changeant le début par econome_int_saveToKMem(place,
//  sans le &
//  re-compiler

// procédures standard

#include <Arduino.h>
#include <MIDI.h>
#include <Encoder.h>
#include <EEPROM.h>
#include <bounce.h>
#include <affichage_functions.h>
#include <TeensyDMX.h>

// BLACK05 tentative de réinstauration de microSD en s'inspirant de
// https://forum.pjrc.com/threads/66326-Interference-between-SDFat-SDIO-and-I2S-on-T4-1?highlight=microSD+teensy4.1

#include <Wire.h>
#include <SPI.h>
#include "SdFat.h"

#define FILE_TRANSFER_BUFFER_SIZE 64
#define N_BUFFERS_IN_FILE 100

// MicroSD vars : SD_FAT_TYPE == 3
SdFs SDcard; // sd
FsFile dir;
// FsFile file;

// FsFile Wave0; // File on microSD card
FsFile myFile;
byte fileTransferBuffer[FILE_TRANSFER_BUFFER_SIZE] = {0};
uint32_t playbackFilePos = 0;
bool ready = false; // SD busy flag

// https://github.com/greiman/SdFs/blob/master/examples/TeensySdioDemo/TeensySdioDemo.ino
// https://github.com/greiman/SdFs/blob/master/examples/OpenNext/OpenNext.ino

// SD_FAT_TYPE = 1 for FAT16/FAT32, 2 for exFAT, 3 for FAT16/FAT32 and exFAT.
#define SD_FAT_TYPE 3
/*
  Change the value of SD_CS_PIN if you are using SPI and
  your hardware does not use the default value, SS.  
  Common values are:
  Arduino Ethernet shield: pin 4
  Sparkfun SD shield: pin 8
  Adafruit SD shields and modules: pin 10
*/

// SDCARD_SS_PIN is defined for the built-in SD on some boards.
#ifndef SDCARD_SS_PIN
const uint8_t SD_CS_PIN = SS;
#else  // SDCARD_SS_PIN
// Assume built-in SD is used.
const uint8_t SD_CS_PIN = SDCARD_SS_PIN;
#endif // SDCARD_SS_PIN

// Try to select the best SD card configuration.
#if HAS_SDIO_CLASS
#define SD_CONFIG SdioConfig(FIFO_SDIO)
#elif ENABLE_DEDICATED_SPI
#define SD_CONFIG SdSpiConfig(SD_CS_PIN, DEDICATED_SPI)
#else // HAS_SDIO_CLASS
#define SD_CONFIG SdSpiConfig(SD_CS_PIN, SHARED_SPI)
#endif // HAS_SDIO_CLASS

/*
#if SD_FAT_TYPE == 1
SdFat sd;
File dir;
File file;
#elif SD_FAT_TYPE == 2
SdExFat sd;
ExFile dir;
ExFile file;
#elif SD_FAT_TYPE == 3
SdFs sd;
FsFile dir;
FsFile file;
#else // SD_FAT_TYPE
#error invalid SD_FAT_TYPE
#endif // SD_FAT_TYPE
*/

//------------------------------------------------------------------------------
// Store error strings in flash to save RAM.
#define error(s) SDcard.errorHalt(&Serial, F(s))
//------------------------------------------------------------------------------

// END

MIDI_CREATE_INSTANCE(HardwareSerial, Serial3, MIDI3);

// DEFINITION De TOUS LES PINS : une bonne habitude à prendre si on pense qu'on gardera pas toujours le même support

// DELETE
// MIDI IN et OUT
// int pinRx3 = 7;
// int pinTx3 = 8;

// CREATION DU TIMER F8
IntervalTimer myTimerF8;
// CREATION DU TIMER Screen
IntervalTimer myTimerScreen;
// CREATION DU TIMER Screen
IntervalTimer myTimerEncoder;

int SizeTransfo = 613 - 1; // 22C01 :) 6 mars 2022
// c'est ajusté dans l'initialisation avec le nombre de transformations existantes !
// dans la fc void initialisationsDiverses(void) 

long time2checkEncoder = 1000000; // soit 1 seconde ?
long time2updateScreen = 5000;

bool readyToUpdateEncoder = true;

// values RIGHT

int multiplyRatioRight = 5;
int oldHardwareRight = -1;
int oldHardwareEncoderRight = -1;
int differenceHardwareRight = 0;
int absolutedifferenceHardwareRight = 0;
int stepSizeRight = 1;
int gapOfStepsToAccelerateRight = 5; // modifie la rapidité à accélérer

long softwareValueEncoderRight = 0;
long oldSoftwareRight = -1;
long olderSoftwareEncoderRight = -1;
long differenceSoftwareRight = 0;
long absoluteDifferenceSoftwareRight = 0;
long minimumSoftwareValueEncoderRight = 0;
long maximumSoftwareValueEncoderRight = 10000;
bool loopingValueRight = false;

// values modulated modulated RIGHT

long softwareValueEncoderRightModulated[8] = {100, 100, 100, 100, 100, 100, 0, 100}; //BLACK08
//
long oldSoftwareRightModulated[8] = {-1, -1, -1, -1, -1, -1, -1, -1};
long olderSoftwareEncoderRightModulated[8] = {-1, -1, -1, -1, -1, -1, -1, -1};
long differenceSoftwareRightModulated[8] = {0};
long absoluteDifferenceSoftwareRightModulated[8] = {0};
long minimumSoftwareValueEncoderRightModulated[8] = {0};
long maximumSoftwareValueEncoderRightModulated[8] = {10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000};
bool loopingValueRightModulated[8] = {false, false, false, false, false, false, false, false};

// values LEFT

int multiplyRatioLeft = 1; // çàd pas d'accélération
int oldHardwareLeft = 0;
int oldHardwareEncoderLeft = 0;
int differenceHardwareLeft = 0;
int absolutedifferenceHardwareLeft = 0;
int stepSizeLeft = 1;
int gapOfStepsToAccelerateLeft = 10;
long softwareValueEncoderLeft = 0;
long oldSoftwareLeft = 0;
long olderSoftwareEncoderLeft = 0;
long differenceSoftwareLeft = 0;
long absoluteDifferenceSoftwareLeft = 0;
long minimumSoftwareValueEncoderLeft = 0;
long maximumSoftwareValueEncoderLeft = SizeTransfo - 1;
bool loopingValueLeft = true;

// values UP

int multiplyRatioUp = 0;
int oldHardwareUp = 0;
int oldHardwareEncoderUp = 0;
int differenceHardwareUp = 0;
int absolutedifferenceHardwareUp = 0;
int stepSizeUp = 1;
int gapOfStepsToAccelerateUp = 10;
long softwareValueEncoderUp = 0;
long oldSoftwareUp = 0;
long olderSoftwareEncoderUp = 0;
long differenceSoftwareUp = 0;
long absoluteDifferenceSoftwareUp = 0;
long minimumSoftwareValueEncoderUp = 0;
long maximumSoftwareValueEncoderUp = 8 ; // type de keyboards mode 
long loopingValueUp = true;

int measuredHighest = 800;
int measuredLowest = 100;

namespace teensydmx = ::qindesign::teensydmx;
teensydmx::Sender dmxTx{Serial7};

String display = "";

// DEFINITION DES PINS, ENCODERS et BOUTONS BLACK 03 - 2 juin 2021

// BOUTONS

const int pinButtonSave = 0;
const int pinButtonLoad = 1;
const int pinButtonKarakt = 2;
const int pinButtonMemory = 3;
const int pinButtonTap = 4;
const int pinButtonOK = 5;
const int pinButtonUndo = 6;
const int pinButtonPad8 = 7;

const int pinButtonPad7 = 8;
const int pinButtonPad6 = 9;
const int pinButtonPad5 = 10;
const int pinButtonJSP = 11;
const int pinButtonExtra = 12;
const int pinButtonMute = 13;

const int pinMidiOut1 = 14;   // TX3
const int pinMidiIn1 = 15;    // RX3
const int pinButtonCtrl = 16; // ATTENTION EST AUSSI RX4 ! voir si pose souci ou pas
const int pinMidiOut2 = 17;   // TX4
const int pinOled1 = 18;
const int pinOled2 = 19;
const int pinButtonAlt = 20;
const int pinSlidePotentiometer = 21;

// ENCODEURS ROTATIFS SIMPLES

const int pinRotaryUpA = 23; // ROTARY UP
const int pinRotaryUpB = 22;

const int pinRotaryRightA = 25; // ROTARY RIGHT
const int pinRotaryRightB = 24;

const int pinRotaryLeftA = 27; // ROTARY LEFT
const int pinRotaryLeftB = 26;

// BOUTONS LA SUITE

const int pinDMXin = 28;
const int pinDMXout = 29;

const int pinButtonDel = 30;
const int pinCTX3 = 31;
const int pinButtonShift = 32;
const int pinButtonJump = 33;
const int pinButtonPad4 = 34;
const int pinButtonPad3 = 35;
const int pinButtonPad2 = 36;
const int pinButtonPad1 = 37;
const int pinButtonLayer4 = 38;
const int pinButtonLayer3 = 39;
const int pinButtonLayer2 = 40;
const int pinButtonLayer1 = 41;

// const int pinButtonSave = 57;

Encoder rotKnobUp(pinRotaryUpA, pinRotaryUpB);
Encoder rotKnobLeft(pinRotaryLeftA, pinRotaryLeftB);
Encoder rotKnobRight(pinRotaryRightA, pinRotaryRightB);

// new names Black005 - 16juillet2021

Bounce myButtonSave(pinButtonSave, 10);
Bounce myButtonLoad(pinButtonLoad, 10);
Bounce myButtonKarakt(pinButtonKarakt, 10);
Bounce myButtonMemory(pinButtonMemory, 10);
Bounce myButtonTap(pinButtonTap, 10);
Bounce myButtonOK(pinButtonOK, 10);
Bounce myButtonUndo(pinButtonUndo, 10);
Bounce myButtonPad8(pinButtonPad8, 10);

Bounce myButtonPad7(pinButtonPad7, 10);
Bounce myButtonPad6(pinButtonPad6, 10);
Bounce myButtonPad5(pinButtonPad5, 10);
Bounce myButtonJSP(pinButtonJSP, 10);
Bounce myButtonExtra(pinButtonExtra, 10);
Bounce myButtonMute(pinButtonMute, 10);

Bounce myButtonCtrl(pinButtonCtrl, 10);
Bounce myButtonAlt(pinButtonAlt, 10);
Bounce myButtonDel(pinButtonDel, 10);
Bounce myButtonShift(pinButtonShift, 10);
Bounce myButtonJump(pinButtonJump, 10);
Bounce myButtonPad4(pinButtonPad4, 10);
Bounce myButtonPad3(pinButtonPad3, 10);
Bounce myButtonPad2(pinButtonPad2, 10);
Bounce myButtonPad1(pinButtonPad1, 10);
Bounce myButtonLayer4(pinButtonLayer4, 10);
Bounce myButtonLayer3(pinButtonLayer3, 10);
Bounce myButtonLayer2(pinButtonLayer2, 10);
Bounce myButtonLayer1(pinButtonLayer1, 10);

/// *****************

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

// OLD PICCOLO
// ENCODEURS ROTATIFS + PUSHBUTTON
//
// int pinRotaryEncoder1A = 30; // COARSE + FINE
// int pinRotaryEncoder1B = 29;
// int pinPushRE1 = 31;
byte readPinPushR1 = HIGH;

long lastPushREVel[4] = {0};

const byte COARSE = 1;
const byte FINE = 2;
byte encoder1mode = COARSE;
long lastPushRE1 = 0;
int saveA, saveB = 0;

// int pinRotaryEncoder2A = 1; // VELOCITY 1
// int pinRotaryEncoder2B = 0;
// int pinPushRE2 = 2;
const byte ACTIVE = 1;
const byte MUTED = 2;
int encoderVelmode[4] = {ACTIVE};
long lastPushRE2 = 0;
int saveEnc2;

// int pinRotaryEncoder3A = 4; // VELOCITY 2
// int pinRotaryEncoder3B = 3;
// int pinPushRE3 = 5;

// int pinRotaryEncoder4A = 11; // VELOCITY 3
// int pinRotaryEncoder4B = 10;
// int pinPushRE4 = 12;

// int pinRotaryEncoder5A = 27; // VELOCITY 4
// int pinRotaryEncoder5B = 26;
// int pinPushRE5 = 28;

// int pinRotaryEncoder6A = 37; // LFO
// int pinRotaryEncoder6B = 36;
// int pinPushRE6 = 35;
const byte LFODUREE = 1;
const byte LFOAMPLITUDE = 2;
byte encoder6mode = LFODUREE;
long lastPushRE6 = 0;
int saveLFOA, saveLFOB = 0;
int LFOA, LFOB = 0;

// procédures "maison"
//
#include <variables.h>  // toutes les variables et constantes
#include <pileNotes.h>  // version héritée de Dende v7
#include <karaktPile.h> // version modifiée de pileNotes 2C22

#include <fonctions.h> // gères toutes les fonctions hors bouton OK
// #include <midiMatrix.h> // il est inclus ailleurs
#include <transfoNames.h> // gère le nom des transformations
// #include <saveEvent.h>    // gère le load et save concernant les Events
// #include <saveSeq.h>      // gère le load et save concernant les Sequences
// #include <saveCCMod.h>    // gère le load et save concernant les CCMod
// #include <saveCell.h>     // gère le load et save concernant les Cell
// TODO reste plus qu'à faire un save load GLOBAL

#include <saveLoadLocal.h>

#include <transfoProcess.h> // gère les transformations qd on appuie sur OK
#include <fcConditional.h>  // contient les fonctions de tests pour event conditionnels

#include <transfoAffichage.h> // gère l'affichage sur l'écran
#include <subParts.h>         // contient les parties stables de l'architecture du MAIN
// #include <permute.h>   // permutations  2C28 : intégré à fonctions

#include <harmonize.h> // nouvelles fonctions d'harmonizations et modif de triades 1C89
#include <dendeFc.h>   // fc reprises de Dende

#define SD_CS_PIN SS

// *************************************************************************************
// *************************************************************************************
// DEBUT DU SETUP
// *************************************************************************************
// *************************************************************************************

// ToDO
// Transport vers Due, Teensy ou ESP32  :
// Pile NoteON
// Pile NoteOFF
// Events illimités
// Vel2CC, Note2CC illimités
// Tout encapsuler, refaire en OOP
// uniformiser RotPrint
// qd uniquement local, variable locale
// Switch RECORD
// PushButton DELETE
//
// transformer le tableau Transfo en struct : BYTE, INT, INT, BYTE, STRING
// complètement refaire le rotary encoder en tenant compte de min, max, step
// tenir compte de la vélocité pour le rotary encoder
// le mieux est de créer nouvelle procédure pour le teensy, plus pour le MEGA
// il faut un encoder rotatif dynamique, voire 2, ou idéalement 3

// transformer les lignes très répétitives d'appel en début de transformation en une fonction
// bien mettre les variables strictement locales en local ( i,j , etc )

// je ne vois pas comment autrement faire pour le CREATE EVENT, comment sélectionner le nom d'une transformations
// pour la valeur de la fonction on la pompe de l'array habituel, une fois qu'on connait quelle fonction

// SET type de transfo : (CC) / Transformation / (modification de valeur)
// SET espace fixe en sec de  l'event - le tap final créera un t dans cet espace e où il est déclenché
// SET random espacement : le prochain event sera entre random( x , y ) - le t est alors entre 0 et x
// SET quelle valeur (0-1023) transformée en affichage habituel
// SET valeur fixe / inc / dec / up-down / down-down / up-up / rnd
// SET Layer concernée : 1 2 3 4 ALL combinaison RND actives

// vient de https://forum.pjrc.com/threads/66326-Interference-between-SDFat-SDIO-and-I2S-on-T4-1?highlight=microSD+teensy4.1

bool sdBusy()
{
    return ready ? SDcard.card()->isBusy() : false;
}

// END

void setup()
{
    // NOUVEAU SETUP PICCOLO BLACK

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
    myTimerScreen.begin(updateScreen, time2updateScreen);
    // 15000 = 30 millisecondes, parfait pour la persistance rétinienne
    myTimerEncoder.begin(updateEncoder, time2checkEncoder); // = 1 seconde

    for (int i = 0; i < 60; i++)
    {
        if (i != 18 && i != 19 && (i < 22 || i > 27))
        //  if (i != 18 && i != 19  )

        {
            pinMode(i, INPUT);
            digitalWrite(i, LOW);
        }
    }

    Serial.begin(9600); // pour le debug, Serial simple

    dmxTx.set(1, 128);
    dmxTx.begin();

    // VIEUX SETUP PICCOLO
    MIDI3.begin(MIDI_CHANNEL_OMNI);
    // MIDI4.begin(MIDI_CHANNEL_OMNI);   // DEBUG à voir si n'interfère pas avec le pin 16 en RX4 = CTRL

    timeZero = micros();

    initialisationsDiverses();
    initializeAllSequences();

    // BLACK06 Serial3.begin(31250);
    //  Serial4.begin(31250); // me semble faire double emploi avec les initialisations en midi plus haut

    // partie TEST SD
    // copié de https://forum.pjrc.com/threads/66326-Interference-between-SDFat-SDIO-and-I2S-on-T4-1?highlight=microSD+teensy4.1

    SDcard.begin(SdioConfig(FIFO_SDIO));
    if (SDcard.remove("test.txt"))
    {
        oledWriteStringAtXY("remove OK", 0, 1);
        delay(500);
    }
    else
    {
        oledWriteStringAtXY("remove PROBLEM", 0, 1);
        delay(10000);
    }
    myFile = SDcard.open("test.txt", O_RDWR | O_CREAT);
    myFile.println("test data with Piccolo Black 08");
    myFile.close();

    // Check to see if the file exists:

    if (SDcard.exists("test.txt"))
    {
        oledWriteStringAtXY("test succesfull", 0, 1);
    }
    else
    {
        oledWriteStringAtXY("test failed", 0, 1);
    }

    delay(2000);

    if (SDcard.begin(SdioConfig(FIFO_SDIO)))
    {
        // printRot2(PiccoloDate + F(" NO SD"), 16, 200, 3);
        buffer_and_HARD_setCursor(0, 2);
        buffer_and_HARD_print(PiccoloDate + F(" SD OK"));
        SDCardPresente = true;
        measureSDFileCount();
        buffer_and_HARD_setCursor(0, 4);
        // buffer_and_HARD_print("files:" + String(rootFileCount));
        oledWriteStringAtXY("files:" + String(rootFileCount), 0, 3);
        //  delay(4000);
    }
    else
    {
        // printRot2(PiccoloDate + F(" SD OK"), 16, 200, 3);
        buffer_and_HARD_setCursor(0, 2);
        buffer_and_HARD_print(PiccoloDate + F(" NO SD"));
        SDCardPresente = false;
    }

    delay(2000);

    buffer_clearScreen_HARD();

    // SETUP DES PILES

    // Setup de la pile

    // laPile.channel = 0;
    // laPile.note = 0;
    // laPile.velocity = 0;
    // // laPile.repeatsEffectues = 0;
    // // laPile.nbF8avantJeu = 0 ;
    // laPile.nextPile = 0; // pointe nulle part - y a pas de case 0 dans la mémoire

    // pointeurInitial = &laPile;

    // pileNote pileLayer[nbLayer];
    // pileNote *pInitial[nbLayer];

    // initialisatoin des 4 layers de piles de notes

    for (int i = 0; i < nbTotalPiles; i++)
    {
        pileLayer[i].channel = 0;
        pileLayer[i].killnote = 0;
        pileLayer[i].layer = 0;
        pileLayer[i].startNote = 0;
        pileLayer[i].nextPile = 0;
        pileLayer[i].note = 0;
        pileLayer[i].type = 0;
        pileLayer[i].velocity = 0;
        pileLayer[i].thereIsAnEnd = false;
        pileLayer[i].timeToPlayON = 0;
        // pileLayer[i].timeToPlayOFF = 0;

        pInitial[i] = &pileLayer[i];
    }

    // initialisation des Markov
    for (int i = 0; i < 5; i++)
    {
        putNoteBinNoteA(markov[i], emptyNote);
        pMarkov[i] = &markov[i];
    }

    // 2C21
    // initialisation du batch

    batch.killnote = false;
    batch.layer = 1;
    batch.nextPile = 0;
    batch.timeToPlayON = 0;
    batch.transfoNb = 4;
    batch.transfoValue = 0;
    pBatch = &batch;

    // 2C20
    // initialisation des variations

    putNoteBinNoteA(variations, emptyNote);
    pVariations = &variations;

    // 2C22 Initialisation des Karakt Mem

    for (int i = 0; i < nbTotalKMem; i++)
    {
        karakt_Header[i].number = 0;
        karakt_Header[i].layer = 0;
        karakt_Header[i].valeur = 0;
        karakt_Header[i].nextPile = NULL;

        pKarakt_Header[i] = &karakt_Header[i];
    }

    // 2C26
    // initialisation des variations Lapidz et Dialyse

    putNoteBinNoteA(variationsLapidz, emptyNote);
    pVariationsLapidz = &variationsLapidz;

    putNoteBinNoteA(variationsDialyse, emptyNote);
    pVariationsDialyse = &variationsDialyse;

    // 2C28
    // initialisation de la pile SoftPass

    putNoteBinNoteA(softPassRedirect, emptyNote);
    pSoftPassRedirect = &softPassRedirect;

    // 2C28
    // initialisation de la pile sustainBlock

    putNoteBinNoteA(sustainBlock, emptyNote);
    pSustainBlock = &sustainBlock;

    // 2C28
    // initialisation de la pile proxyBlock

    putNoteBinNoteA(proxyBlock, emptyNote);
    pProxyBlock = &proxyBlock;

    // initialisation des piles servant de mémoire ( pour save, load, switch )

    // for (int i = 0; i < nbLayersMemory; i++)
    // {
    //     pileMemory[i].channel = 0;
    //     pileMemory[i].killnote = 0;
    //     pileMemory[i].layer = 0;
    //     pileMemory[i].startNote = 0;
    //     pileMemory[i].nextPile = 0;
    //     pileMemory[i].note = 0;
    //     pileMemory[i].type = 0;
    //     pileMemory[i].velocity = 0;
    //     pileMemory[i].data1 = 0;
    //     pileMemory[i].data2 = 0;
    //     pileMemory[i].thereIsAnEnd = false;
    //     pileMemory[i].timeToPlayON = 0;
    //     pileMemory[i].timeToPlayOFF = 0;

    //     pInitialMemory[i] = &pileMemory[i];
    // }

    emptyNote.channel = 0;
    emptyNote.killnote = 0;
    emptyNote.layer = 0;
    emptyNote.startNote = 0;
    emptyNote.nextPile = 0;
    emptyNote.note = 60;
    emptyNote.type = 0;
    emptyNote.velocity = 0;
    emptyNote.thereIsAnEnd = false;
    emptyNote.timeToPlayON = 0;

    emptyNote.pinchCH = 0 ;
    emptyNote.pinchCH_size = 1 ; 
    emptyNote.pinchCC = 0 ; 
    emptyNote.pinchCC_size = 1 ;
    emptyNote.pinchValue = 1 ; 

    // emptyNote.timeToPlayOFF = 0;

    // SETUP de la pile PLAY and STOP once, Dende-Style

    // Setup de la pile

    laPile.channel = 0;
    laPile.note = 0;
    laPile.velocity = 0;
    laPile.nextPile = 0; // pointe nulle part - y a pas de case 0 dans la mémoire

    pointeurInitial = &laPile;

    // DEBUG j'ajoute ici articiellement des notes pour tester la fonction
    // for (int i = 0; i < 50; i++)
    // {
    //     laPile.note = rand()%127 ;
    //     laPile.velocity = rand()%10;
    //     laPile.startNote = i ;
    //     laPile.endNote = i ;
    //     ZEROaddNewNotetoPile(pInitial[rand()%4],laPile);
    //     // addNewNotetoPileMSEC(pInitial[rand() % 4], rand() % 4, rand() % 127, rand() % 100, rand() % 4 * 1000, rand() % 4);
    // }

    // je dois apprendre comment faire surcharge d'opérateur '=' pour pileNote !

    saveKarakt_to_KMem(0); // on sauve les valeurs initiales dans la première mémoire

    // on fait ceci pour éviter que la machine perçoive un faux "mouvement" de bouton à l'initialisation
    // en faisant ceci, on commence avec un beau zéro
    oldHardwareEncoderRight = rotKnobRight.read() / 4;
    oldHardwareEncoderLeft = rotKnobLeft.read() / 4;
    oldHardwareEncoderUp = rotKnobUp.read() / 4;

    // direct_clearScreen();
    //    oledWriteStringAtXY("FULL SETUP OK",0,0);
    //    delay(3000);
}

unsigned int marginMap(int initial, byte percent, int lowTarget, int highTarget)
{
    // on part de valeurs allant de 0 à 1024 : analogRead
    // et on doit arriver aux valeurs target, en sachant qu'une marge des valeurs donne min et max

    int initialHighest = measuredHighest - ((long)percent * measuredHighest / 100);
    int initialLowest = measuredLowest + ((long)percent * measuredHighest / 100);

    if (initial >= initialHighest)
    {
        return highTarget;
    }
    if (initial <= initialLowest)
    {
        return lowTarget;
    }

    // unsigned int result = ((highTarget - lowTarget) * (initial - initialLowest)) / (initialHighest - initialLowest);
    unsigned int result = map(initial, initialLowest, initialHighest, lowTarget, highTarget);

    return result;
}

// *************************************************************************************
// *************************************************************************************
// DEBUT DE LA BOUCLE
// *************************************************************************************
// *************************************************************************************

void loop()
{
    nowMicros = micros();
    nowMillis = nowMicros / 1000;

    // ***************************************************************************
    // LECTURE DES SENSEURS
    // ***************************************************************************

    // lecture encodeurs rotatifs

    //  myButtonOK.update();
    //  if (myButtonOK.risingEdge())
    {
        //  oledWriteStringAtXY("OK", 1, 0);
        //    printRot2("OK", 9, 400, 5);
        //  delay(5000);
    }

    // myButtonMemory.update();
    // if (myButtonMemory.risingEdge())
    {
        //        oledWriteStringAtXY("Memory", 1, 0);
        // printRot2("Memory", 9, 400, 5);
        // delay(5000);
    }

    myButtonSave.update();
    myButtonLoad.update();
    myButtonKarakt.update();
    myButtonMemory.update();
    myButtonTap.update();
    myButtonOK.update();
    myButtonUndo.update();
    myButtonPad8.update();

    myButtonPad7.update();
    myButtonPad6.update();
    myButtonPad5.update();
    myButtonJSP.update();
    myButtonExtra.update();
    myButtonMute.update();

    myButtonCtrl.update();
    myButtonAlt.update();
    myButtonDel.update();
    myButtonShift.update();
    myButtonJump.update();
    myButtonPad4.update();
    myButtonPad3.update();
    myButtonPad2.update();
    myButtonPad1.update();
    myButtonLayer4.update();
    myButtonLayer3.update();
    myButtonLayer2.update();
    myButtonLayer1.update();

    // Lecture RIGHT

    int32_t a = rotKnobRight.read() / 4;

    if (a != oldHardwareRight && synthesePressed == 0) // çàd pas de modulation
    {
        differenceHardwareRight = a - oldHardwareRight;

        softwareValueEncoderRight = softwareValueEncoderRight + differenceHardwareRight * stepSizeRight;

        if ((softwareValueEncoderRight < minimumSoftwareValueEncoderRight && loopingValueRight == false) || (softwareValueEncoderRight > maximumSoftwareValueEncoderRight && loopingValueRight == true))

        {
            softwareValueEncoderRight = minimumSoftwareValueEncoderRight;
            stepSizeRight = 1;
        }

        if ((softwareValueEncoderRight > maximumSoftwareValueEncoderRight && loopingValueRight == false) || (softwareValueEncoderRight < minimumSoftwareValueEncoderRight && loopingValueRight == true))

        {
            softwareValueEncoderRight = maximumSoftwareValueEncoderRight;
            stepSizeRight = 1;
        }

        oldHardwareRight = a;
        oldSoftwareRight = softwareValueEncoderRight;
    }

    // Lecture modulated RIGHT = BLACK08

    if (a != oldHardwareRight && synthesePressed == binCtrl) // çàd CTRL + encoder
    {
        differenceHardwareRight = a - oldHardwareRight;

        softwareValueEncoderRightModulated[6] = softwareValueEncoderRightModulated[6] + differenceHardwareRight * stepSizeRight;

        if ((softwareValueEncoderRightModulated[6] < minimumSoftwareValueEncoderRightModulated[6] && loopingValueRightModulated[6] == false) || (softwareValueEncoderRightModulated[6] > maximumSoftwareValueEncoderRightModulated[6] && loopingValueRightModulated[6] == true))
        {
            softwareValueEncoderRightModulated[6] = minimumSoftwareValueEncoderRightModulated[6];
            stepSizeRight = 1;
        }

        if ((softwareValueEncoderRightModulated[6] > maximumSoftwareValueEncoderRightModulated[6] && loopingValueRightModulated[6] == false) || (softwareValueEncoderRightModulated[6] < minimumSoftwareValueEncoderRightModulated[6] && loopingValueRightModulated[6] == true))
        {
            softwareValueEncoderRightModulated[6] = maximumSoftwareValueEncoderRightModulated[6];
            stepSizeRight = 1;
        }

        oldHardwareRight = a;
        oldSoftwareRightModulated[6] = softwareValueEncoderRightModulated[6];
    }

    if (a != oldHardwareRight && synthesePressed == binAlt) // çàd ALT + encoder
    {
        differenceHardwareRight = a - oldHardwareRight;

        softwareValueEncoderRightModulated[5] = softwareValueEncoderRightModulated[5] + differenceHardwareRight * stepSizeRight;

        if ((softwareValueEncoderRightModulated[5] < minimumSoftwareValueEncoderRightModulated[5] && loopingValueRightModulated[5] == false) || (softwareValueEncoderRightModulated[5] > maximumSoftwareValueEncoderRightModulated[5] && loopingValueRightModulated[5] == true))
        {
            softwareValueEncoderRightModulated[5] = minimumSoftwareValueEncoderRightModulated[5];
            stepSizeRight = 1;
        }

        if ((softwareValueEncoderRightModulated[5] > maximumSoftwareValueEncoderRightModulated[5] && loopingValueRightModulated[5] == false) || (softwareValueEncoderRightModulated[5] < minimumSoftwareValueEncoderRightModulated[5] && loopingValueRightModulated[5] == true))
        {
            softwareValueEncoderRightModulated[5] = maximumSoftwareValueEncoderRightModulated[5];
            stepSizeRight = 1;
        }

        oldHardwareRight = a;
        oldSoftwareRightModulated[5] = softwareValueEncoderRightModulated[5];
    }

    // BLACK09

    if (a != oldHardwareRight && synthesePressed == binShift ) // çàd Shift  + encoder
    // Va déterminer la page affichée
    {
        differenceHardwareRight = a - oldHardwareRight;

        softwareValueEncoderRightModulated[7] = softwareValueEncoderRightModulated[7] + differenceHardwareRight * stepSizeRight;

        if ((softwareValueEncoderRightModulated[7] < minimumSoftwareValueEncoderRightModulated[7] && loopingValueRightModulated[5] == false) || (softwareValueEncoderRightModulated[5] > maximumSoftwareValueEncoderRightModulated[5] && loopingValueRightModulated[5] == true))
        {
            softwareValueEncoderRightModulated[7] = minimumSoftwareValueEncoderRightModulated[7];
            stepSizeRight = 1;
        }

        if ((softwareValueEncoderRightModulated[7] > maximumSoftwareValueEncoderRightModulated[7] && loopingValueRightModulated[5] == false) || (softwareValueEncoderRightModulated[5] < minimumSoftwareValueEncoderRightModulated[5] && loopingValueRightModulated[5] == true))
        {
            softwareValueEncoderRightModulated[7] = maximumSoftwareValueEncoderRightModulated[7];
            stepSizeRight = 1;
        }

        oldHardwareRight = a;
        oldSoftwareRightModulated[7] = softwareValueEncoderRightModulated[7];

        pageEndUser = softwareValueEncoderRightModulated[7];
        currentPageStepz = pageEndUser;

        if (pageEndUser > numberMaxPageStepz)
        {
            currentPageStepz = numberMaxPageStepz;
            pageEndUser = currentPageStepz ;
            softwareValueEncoderRightModulated[7] = currentPageStepz ;
        }

        if (pageEndUser < 0)
        {
            currentPageStepz = 0;
            pageEndUser = currentPageStepz;
            softwareValueEncoderRightModulated[7] = currentPageStepz;
        }

    }

    // Lecture LEFT

    a = rotKnobLeft.read() / 4;

    if (a != oldHardwareLeft)
    {
        differenceHardwareLeft = a - oldHardwareLeft;

        softwareValueEncoderLeft = softwareValueEncoderLeft + differenceHardwareLeft * stepSizeLeft;

        // BLACK06 la plupart du temps, qd on bouge l'encodeur gauche
        // on change de transformation
        // et il faut donc modifier les min et max pour l'encodeur de droite

        minimumSoftwareValueEncoderRight = Transfo[softwareValueEncoderLeft][1];
        maximumSoftwareValueEncoderRight = Transfo[softwareValueEncoderLeft][2];

        // TODO BLACK08 ne faut-il pas ici rappeler l'ancienne valeur de Right ?

        if ((softwareValueEncoderLeft < minimumSoftwareValueEncoderLeft && loopingValueLeft == false) || (softwareValueEncoderLeft > maximumSoftwareValueEncoderLeft && loopingValueLeft == true))

        {
            softwareValueEncoderLeft = minimumSoftwareValueEncoderLeft;
            stepSizeLeft = 1;
        }

        if ((softwareValueEncoderLeft > maximumSoftwareValueEncoderLeft && loopingValueLeft == false) || (softwareValueEncoderLeft < minimumSoftwareValueEncoderLeft && loopingValueLeft == true))

        {
            softwareValueEncoderLeft = maximumSoftwareValueEncoderLeft;
            stepSizeLeft = 1;
        }

        oldHardwareLeft = a;
        oldSoftwareLeft = softwareValueEncoderLeft;
    }

    //   oledWriteStringAtXY(String(a) + " ", 7, 0);
    //   oledWriteStringAtXY(String(softwareValueEncoderLeft) + " ", 7, 1);

    // Lecture UP

    a = rotKnobUp.read() / 4;

    if (a != oldHardwareUp)
    {
        differenceHardwareUp = a - oldHardwareUp;

        softwareValueEncoderUp = softwareValueEncoderUp + differenceHardwareUp * stepSizeUp;

        if ((softwareValueEncoderUp < minimumSoftwareValueEncoderUp && loopingValueUp == false) || (softwareValueEncoderUp > maximumSoftwareValueEncoderUp && loopingValueUp == true))

        {
            softwareValueEncoderUp = minimumSoftwareValueEncoderUp;
            stepSizeUp = 1;
        }

        if ((softwareValueEncoderUp > maximumSoftwareValueEncoderUp && loopingValueUp == false) || (softwareValueEncoderUp < minimumSoftwareValueEncoderUp && loopingValueUp == true))

        {
            softwareValueEncoderUp = maximumSoftwareValueEncoderUp;
            stepSizeUp = 1;
        }

        oldHardwareUp = a;
        oldSoftwareUp = softwareValueEncoderUp;

        // insérer ici les changements de type de keybaord

       //  if (absoluteDifferenceSoftwareUp >= 1)
        {
            // cela signifie qu'on a tourné le bouton UP
            // on nettoie
            affiche = "";
            // printRot2(affiche, 16, 400, 3);
            // buffer_clearScreen();
            keyboardMode = softwareValueEncoderUp;
            buffer_print_atXY(keyboardMode, 18, 0);

            if (keyboardMode == 0)
            {
                softPass = true;
                midiThru = true;
                MIDI3.turnThruOn(midi::Thru::Full);
            }
            else
            {
                softPass = false;
                midiThru = false;
                MIDI3.turnThruOff();
            }
        }

    }

    // oledWriteStringAtXY(String(a) + " ", 14, 0);
    // oledWriteStringAtXY(String(softwareValueEncoderUp) + " ", 14, 1);

    // BLACK 05 : ce qui compte SOFTWAREVALUEENCOERRIGHT LEFT et UP

    // compléter avec un writeChar, SANS string du tout
    // writeInt

    // NOTE**** on doit peut être donner un timing pour lire les digitalRead ?
    // DEBUG : il lit 2 fois trop de switches . pour Teensy on devrait développer un interrupt qui modifie la valeur quand les 4 switchs sont changés

    //  if ( Transfo[softwareValueEncoderLeft][0] != 424 ) { checkMuteButtons(); } else { checkStepzButtons(); }

    if (Transfo[softwareValueEncoderLeft][0] == 424 || Transfo[softwareValueEncoderLeft][0] == 512) // StepZ LapidZ
    {
        // keyboardMode = 1 ;
        checkStepzButtons();
    }
    else if (Transfo[softwareValueEncoderLeft][0] == 441) // PuntZ
    {
        // keyboardMode = 1;
        checkPuntzButtons();
    }
    else if (Transfo[softwareValueEncoderLeft][0] == 548) // polyZ
    {
        //  keyboardMode = 1;
        checkPolyZButtons();
    }
    else
    {
        //  keyboardMode = 0;
        checkPadsButtons(); // BLACK08 : les padz servent de shortcut qd pas utilisés par stepz
    }

    if ((record || sendCCtoTX81Z || softPass) && (busyWithMIDIAffichage == false))
    // TODO DEBUG problème qd on a synth1 IN , synth2 OUT, Record et SoftPass et Cell : il ne passe pas la valeur
    {
        if (MIDI3.read(0))
        {
            timenote = nowMillis;

            // byte channel = MIDI.getChannel();    // donne le channel, pas le codage du channel ! CH1 = 0 pour machine
            // channel = channel-1 ;
            // byte note = MIDI.getData1();
            // byte velocity = MIDI.getData2();

            // do
            {
                midiType = MIDI3.getType();

                if (midiType == 0xF8)
                {
                    compteurF8++;

                    if (compteurF8 >= (nbNotesToCountForF8))
                    {
                        BPM = 240000 / (nowMillis - lastAmountF8Time);
                        compteurF8 = 0;
                        lastAmountF8Time = nowMillis;
                    }

                    //     // 1 min = 60000 msec
                    //     // disons 1 BPM de 133
                    //     // 1 note = 60000 / 133 -> 24 * 4 F8
                    //     // duré 1 F8 = ( 60000 / BPM ) / 96
                    //     // BPM = 60000 / 96 * durée 1F8
                    //     // BPM = 60000 * nbNotesToCountForF8 / 96 * duréePourCompterCesNF8
                }
                else
                {
                    channel = MIDI3.getChannel() - 1; // donne le channel, pas le codage du channel ! CH1 = 0 pour machine
                    note = MIDI3.getData1();
                    velocity = MIDI3.getData2();
                }
            }
            // while (midiType == 0xF8 && MIDI3.read(0));

            // TODO DEBUG je n'aime pas cette boucle qui tourne tant qu'il y a du F8 - ça doit retarder le jeu de certaines notes
            // si le sketch est assez rapide, il faut suppprimer cela

            switch (midiType)
            {
            case midi::ControlChange:
            {
                if (sendCCtoTX81Z)
                {
                    convertData1and2toTX81ZModif(note, velocity, channel);
                }

                if (sendCCtoSHERMAN)
                {
                    convertData1and2toSHERMAN(note, velocity, 15);
                }

                // else  2C18

                {
                    // insérer ici un transfert du CC reçu !
                    /// TODO DEBUG
                    // en fonction de paramètres bien sûr
                    // insérer un mode SHERMAN
                    // et plein de modes pour les copains :)

                    if (recAutomation == true)
                    {
                        automationCompteur++;
                        if (automationCompteur % automationStep == 0)
                        // permet d'enregistrer un peu moins de CC
                        {
                            for (byte i = 0; i < nbLayer; i++)
                            {
                                if (isLayerActive(i))
                                {
                                    // ici on doit savoir si on enregistre le CC dans les layer actifs
                                    // on vers un layer spécifié

                                    // commençons avec layer actif
                                    // on a channel, note, velocity.   B0  n°CC  valeur

                                    pileNote newOneCC;

                                    newOneCC.killnote = false;
                                    newOneCC.layer = i;

                                    newOneCC.startNote = (timenote) % layerLength[i];
                                    newOneCC.endNote = 0; // ne sera pas utilisé pour CC !
                                    newOneCC.thereIsAnEnd = false;

                                    newOneCC.nextPile = 0;
                                    newOneCC.note = note; // en fait ici N° de CC !
                                    // newOneCC.timeToPlayOFF = 0; // TODO
                                    newOneCC.timeToPlayON = 0; // TODO
                                    newOneCC.type = 0xB0;
                                    newOneCC.velocity = velocity;
                                    newOneCC.channel = channel;

                                    pileNote_actualizeTimeToPlay(newOneCC);
                                    ZEROaddNewNotetoPile(pInitial[i], newOneCC);
                                }
                            }
                        }
                    }
                }
            }
            break;

            // 2C20 pour le Nord Pitch Bend
            case midi::PitchBend:
            {
                {
                    if (recAutomation == true)
                    {
                        automationCompteur++;
                        if (automationCompteur % automationStep == 0)
                        // permet d'enregistrer un peu moins de CC
                        {
                            for (byte i = 0; i < nbLayer; i++)
                            {
                                if (isLayerActive(i))
                                {
                                    // ici on doit savoir si on enregistre le CC dans les layer actifs
                                    // on vers un layer spécifié

                                    // commençons avec layer actif
                                    // on a channel, note, velocity.   B0  n°CC  valeur

                                    pileNote newOneCC;

                                    newOneCC.killnote = false;
                                    newOneCC.layer = i;

                                    newOneCC.startNote = (timenote) % layerLength[i];
                                    newOneCC.endNote = 0; // ne sera pas utilisé pour CC !
                                    newOneCC.thereIsAnEnd = false;

                                    newOneCC.nextPile = 0;
                                    newOneCC.note = note; // en fait ici N° de CC !
                                    // newOneCC.timeToPlayOFF = 0; // TODO
                                    newOneCC.timeToPlayON = 0; // TODO
                                    newOneCC.type = 224;       // PitchBend
                                    newOneCC.velocity = velocity;
                                    newOneCC.channel = channel;

                                    pileNote_actualizeTimeToPlay(newOneCC);
                                    ZEROaddNewNotetoPile(pInitial[i], newOneCC);
                                }
                            }
                        }
                    }
                }
            }
            break;

            case midi::NoteOn:
            {
                int initialNote = note;

                if (autoHarmonizeLivePlaying == true)
                {
                    note = dynamicHarmonizeLive(note); // attention ! si gammes et modes différents et si dynHarmonize +-,
                    // on aura des valeurs différentes, mais c'est le jeu !
                    // si la note est annulée, elle devient 0 !
                }

                /*
case 0:            affiche += (F("1.classic notes"));
 case 1:            affiche += (F("2.stepz=WH vel=BL"));
      case 2:            affiche += (F("3.padz=WH vel=BL"));
        case 3:     affiche += (F("4.Mem Layer"));
           case 4: affiche += (F("5.Mem Karakt"))
        case 5:            affiche += (F("6.launch CC string"));
        case 6:     affiche += (F("7.one shot Layer "));
        case 7:    affiche += (F("8.one shot Karakt"));
           */

                if (keyboardMode == 1) // Stepz avec white stepz et black velocity
                {
                    // on va attribuer une valeur de Step à différentes notes
                    note = initialNote % startingKbStep; // par défaut 60, penser plus tard à pouvoir ajuster, surtout si d'autres touches font shortcut

                    byte stepResult = 100;

                    // D'abord une répartition à l'attention des blanches
                    if (note >= 0 && note < 5)
                    {
                        stepResult = note / 2;
                    }
                    if (note >= 5 && note < 17)
                    {
                        stepResult = note / 2 + 1;
                    }
                    if (note >= 17 && note < 28)
                    {
                        stepResult = note / 2 + 2;
                    }

                    // Qd c'est une noire on va moduler la vélocité, pas créer une note
                    if (note == 1 || note == 3 || note == 6 || note == 8 || note == 10 || note == 13 || note == 15 || note == 18 || note == 20 || note == 22 || note == 25 || note == 27)
                    {
                        stepVelocity = (byte)((int)((int)126 * (int)note / (int)27));
                        stepResult = 100; // on annule le fait que ça crée une note
                    }

                    if (stepResult != 100)
                    {
                        keyboardStepz[stepResult] = 1; // çàd on a appuyé sur la touche keyboard comme Stepz
                    }

                    // et on finit par l'annuler
                    note = 0;
                }

                else if (keyboardMode == 2) // Padz avec white CH et black Velocité
                {
                }

                else if (keyboardMode == 3) // Memory Layer
                {
                     note = initialNote % startingKbStep; // par défaut 60, penser plus tard à pouvoir ajuster, surtout si d'autres touches font shortcut
                    // Load 341
                    // Save 340
                    byte compteur = 0;

                    for (byte i = 0; i < nbLayer; i++)
                    {
                        if (isLayerActive(i))
                        {
                            // contrôler que mémoire existe !
                             
                            int memoireVisee = 4 + note + compteur ;
//Serial.print("memoire visee = ");
//Serial.print(memoireVisee); 

                            unsigned int taille = mesureTaillePileComplete(pInitial[memoireVisee]);
//Serial.print(" taille=");
//Serial.println(taille); 

                            if (taille > 0)
                            {
                               // Serial.print("loading ");
                               // Serial.println(memoireVisee);
                                layerLength[i] = layerLength[note + compteur + 4];
                                // doit précéder la copie, pour que les calculs soient OK
                                ZEROcopyMemoryAtoLayerB( note + compteur, i);
                                // BLACK09 grosse nuance ici : 
                                // note+compteur+4 pour layerLenght
                                // et note+compteur pour le Copy !!!
                                ZEROupdatePileTimeToPlayMODEDIRECT(i);
                            }

                            compteur++;
                        }
                    }
                }
                else if (keyboardMode == 4) // Memory Karakt
                {
                    int memoireVisee = initialNote % startingKbStep; // par défaut 60, penser plus tard à pouvoir ajuster, surtout si d'autres touches font shortcut
                                                                     // Load 510
                                                                     // Save 509
                    Serial.print("loading ");
                    Serial.println(memoireVisee);

                    loadKarakt_from_KMem(memoireVisee);
                    currentKarakt = memoireVisee ;
                }
                else if (keyboardMode == 5) // Launch CC String
                {
                }
                else if (keyboardMode == 6) // One shot layer
                {
                }
                else if (keyboardMode == 7) // One shot Karakt
                {
                }
                else if (keyboardMode == 8) // Memory + Karakt
                {
                    // int noteModulo = note % 12; 

                    if ( noteIsWhite(note) )
                    {
                        // BLANCHE = memory
                        // note = initialNote % startingKbStep; // par défaut 60, penser plus tard à pouvoir ajuster, surtout si d'autres touches font shortcut

                        note = nombreDeBlanchesEntre(initialNote, startingKbStep);
                        Serial.println(note);

                        byte compteur = 0;

                        for (byte i = 0; i < nbLayer; i++)
                        {
                            if (isLayerActive(i))
                            {
                                // contrôler que mémoire existe !
                                int memoireVisee =  note + compteur + 4 ;
                                unsigned int taille = mesureTaillePileComplete(pInitial[memoireVisee]);

                                if (taille > 0)
                                {
                                    layerLength[i] = layerLength[note + compteur + 4];
                                    ZEROcopyMemoryAtoLayerB(note + compteur, i);
                                    // BLACK09 grosse nuance ici :
                                    // note+compteur+4 pour layerLenght
                                    // et note+compteur pour le Copy !!!
                                    ZEROupdatePileTimeToPlayMODEDIRECT(i);
                                }
                                compteur++;
                            }
                        }
                    }
                    else
                    {
                        int memoireVisee = nombreDeNoiresEntre(initialNote, startingKbStep);
                        Serial.println(memoireVisee);

                       // byte compteur = 0;

                        Serial.print("loading ");
                        Serial.println(memoireVisee);

                        loadKarakt_from_KMem(memoireVisee);
                        currentKarakt = memoireVisee;
                    }
                }

                else // BLACK09

                    if (keyboardMode == 0 && note != 0)
                {
                    // DEBUG pour Cell il faut quand même connaître le CH également ?
                    if (isNoteCellConsolidated(channel, note)) // 1C87
                    {
                        byte place = givePlaceInTableauForNoteCellConsolidated(channel, note);
                        note = tableauCell[place].noteOut;
                        channel = tableauCell[place].channelOut; // parce que juste après il fait -1 ;
                        if (softPass)
                        {
                            PlaynoteOn_(0x90 + channel, note, velocity);

                            // 2C28 si on enrgsitre sur plusieurs layers, va donc jouer cette note plusieurs fois !!!
                        }
                    }
                    else
                    {
                        if (softPass)
                        {
                            pileNote softPassNote; // on veut juste conserver CHANNEL, NOTE et REAL PLAYED !!!
                            softPassNote.note = initialNote;
                            softPassNote.noteReallyPlayed = note;
                            softPassNote.channelReallyPlayed = channel;
                            softPassNote.channel = channel;
                            softPassNote.thereIsAnEnd = false;
                            softPassNote.nextPile = 0;
                            softPassNote.timeToPlayON = 0; // TODO
                            softPassNote.type = 0x90;
                            softPassNote.velocity = velocity;
                            ZEROaddNewNotetoPile(pSoftPassRedirect, softPassNote);

                            PlaynoteOn_(0x90 + channel, note, velocity);
                            // 2C28 si on enrgsitre sur plusieurs layers, va donc jouer cette note plusieurs fois !!!
                        }
                    }

                    // turnOnLEDNoteIn();

                    for (byte i = 0; i < nbLayer; i++)
                    {
                        //byte k =  ( ( activeLayer >> i ) & B00000001 );    // le bit de droite est 1 si layer 0 est ON ; le 2ème bit est 1 si layer 1 est ON, etc...

                        //if ( k == B00000001 )
                        if (isLayerActive(i))
                        {
                            // byte j = firstfreenote(i);
                            // TODO pourra être supprimé
                            lastRecordedNoteTime = timenote;

                            // if ( isNoteCellConsolidated(note) )
                            // {
                            //     byte place = givePlaceInTableauForNoteCellConsolidated(note);
                            //     note =  tableauCell[place][2] ;
                            //     channel =  tableauCell[place][1]   ; // parce que juste après il fait -1 ;
                            //     if (softPass)
                            //     { PlaynoteOn_(0x90+channel , note, velocity) ; }
                            // } else
                            // {
                            //     if ( softPass) { PlaynoteOn_(0x90+channel , note, velocity) ; }
                            // }
                            if (eraseRecording == true)
                            {
                                // on doit trouver les notes note et channel dans la pile et les effacer

                                unsigned int taille = mesureTaillePileComplete(pInitial[i]);
                                pileNote tempNote;

                                for (unsigned int j = 0; j < taille; j++)
                                {
                                    tempNote = ZEROgetNoteFromPile(pInitial[i], j);

                                    if (tempNote.note == note && tempNote.channel == channel)
                                    {
                                        tempNote.note = 0;
                                        tempNote.killnote = true;
                                        tempNote.velocity = 0;
                                        // va modifier le pitch sauf si note appartenant à layer ou channel unpitchable
                                        ZEROSetNoteInPile(pInitial[i], j, tempNote);
                                    }
                                }
                            }
                            else if (record)
                            {
                                // 1C92 TOUT SE JOUE ICI !
                                // 1. définir le contenu d'une note
                                // 2. ajouter cette note à la pile Ad HOc ( en fait 1 suffirait ! )

                                pileNote newOne;

                                newOne.killnote = false;
                                newOne.layer = i;

                                newOne.startNote = (timenote) % layerLength[i];

                                //if ( newOne.startNote > 1 )
                                {
                                    // newOne.endNote = newOne.startNote - 1 ;
                                }
                                // else
                                {
                                    newOne.endNote = 0;
                                }

                                // TODO DEBUG : me semble très moche.   j'aimerais mieux un flag disant : j'ai pas d'endNote pour l'instant que cette approximation dangereuse
                                newOne.thereIsAnEnd = false;

                                newOne.nextPile = 0;
                                newOne.note = note;
                                // newOne.timeToPlayOFF = 0; // TODO
                                newOne.timeToPlayON = 0; // TODO
                                newOne.type = 0x90;
                                newOne.velocity = velocity;

                                switch (recordMode)
                                {
                                // il faudra adapter qd on instaurera chord !!!
                                case 0:

                                    newOne.channel = layerChannel[i];
                                    newOne.channelReallyPlayed = layerChannel[i];
                                    break;
                                case 1:

                                    newOne.channel = channel;
                                    newOne.channelReallyPlayed = channel;
                                    break;
                                case 2:

                                    newOne.channel = channel;
                                    newOne.channelReallyPlayed = channel;
                                    break;
                                default:
                                {
                                    newOne.channel = channel;
                                    newOne.channelReallyPlayed = channel;
                                }
                                break;
                                }

                                pileNote_actualizeTimeToPlay(newOne);
                                if (velocity != 0)
                                {
                                    ZEROaddNewNotetoPile(pInitial[i], newOne);
                                }

                                // *****************************************************
                                // vérification des tailles maximales de layer
                                // *****************************************************
                                // 1 = none
                                // 2 = Delete Oldest
                                // 3 = delete random
                                // 4 = delete ClosestPitch

                                if (modeDeletionWhenExtraNotes[i] != 1)
                                {
                                    unsigned int taille = mesureTaillePileComplete(pInitial[i]);
                                    if (taille > nbMaxDeNotesParLayer[i])
                                    {
                                        if (modeDeletionWhenExtraNotes[i] == 2)
                                        {
                                            // DELETE OLDEST
                                            ZEROdeleteNoteFromPile(pInitial[i], 0);
                                        }

                                        if (modeDeletionWhenExtraNotes[i] == 3)
                                        {
                                            // DELETE RANDOM
                                            ZEROdeleteNoteFromPile(pInitial[i], random(taille));
                                            // je mets ici (taille) et pas (taille+1) pour éviter d'effacer la NOUVELLE note !
                                        }

                                        if (modeDeletionWhenExtraNotes[i] == 4)
                                        {
                                            // DELETE CLOSEST IN PITCH
                                            unsigned int noteToKillOverflow = 0;
                                            noteToKillOverflow = ZEROFindClosestPitch(pInitial[i], newOne.channel, newOne.note);
                                            ZEROdeleteNoteFromPile(pInitial[i], noteToKillOverflow);
                                        }
                                    }
                                }
                            }

                        } // fin du if layer(i) active
                    }
                } // fin note != 0

                // turnOffLEDNoteIn();

                // TODO DEBUG il faut intégrer le fait qu'il ya parfois un play Note ON vélocité Zéro qui veut dire note Off
            }
            break;

            case midi::NoteOff:
            {

                if (keyboardMode == 0)
                {

                    // 2C28 sortie de la boucle

                    if (isNoteCellConsolidated(channel, note))
                    {
                        byte place = givePlaceInTableauForNoteCellConsolidated(channel, note);
                        note = tableauCell[place].noteOut;
                        channel = tableauCell[place].channelOut; // parce que juste après il fait -1 ;
                        if (softPass)
                        {
                            PlaynoteOn_(0x80 + channel, note, velocity);
                        }
                    }
                    else
                    {
                        if (softPass)
                        {

                            if (checkNoteAChannelBinSoftPassPiles(note, channel) == true)
                            {
                                int newNote = getRedirectValue_from_NoteAChannelBinSoftPassPiles(note, channel);
                                // et on efface la valeur dans la pile
                                deleteNoteAChannelBinSoftPassPiles(note, channel);
                                note = newNote;
                            }

                            PlaynoteOn_(0x80 + channel, note, 0);
                        }
                    }

                    for (byte i = 0; i < nbLayer; i++)
                    {
                        // byte k =  ( ( activeLayer >> i ) & B00000001 );    // le bit de droite est 1 si layer 0 est ON ; le 2ème bit est 1 si layer 1 est ON, etc...

                        // if ( k == B00000001 ) {
                        if (isLayerActive(i))
                        {
                            {
                                // AVANT 2C21 Couronne
                                /*
     if (record)
                        { // jusque 2C21
                          
                            // TODO DEBUG
                            // on doit ici trouver la note qui est ON juste auparavant
                            // 1. trouver cette note
                            // soit la dernière dont startnote < momentNote
                            // soit la dernière dont startnote > momentNote
                            // 2. mettre à jour la fin noteEnd
                            // 3. mettre à jour l'actualize time to play

                            unsigned int taille = mesureTaillePileComplete(pInitial[i]);

                            // cherchons d'abord un start plus petit que le end
                            bool foundSmaller = false;
                            bool foundBigger = false;
                            unsigned int noteNumberSmaller = 0;
                            unsigned int noteNumberBigger = 0;
                            unsigned long biggestSmaller = 0;
                            unsigned int biggestBigger = 0;

                            unsigned long momentNote = (timenote) % layerLength[i];
                            // int k = 0;

                            for (unsigned int j = 0; j < taille; j++)
                            {
                                pileNote tempNote;
                                tempNote = ZEROgetNoteFromPile(pInitial[i], j);

                                if (tempNote.note == note && tempNote.channel == channel && tempNote.startNote < momentNote)
                                {
                                    foundSmaller = true;
                                    if (tempNote.startNote > biggestSmaller)
                                    {
                                        biggestSmaller = tempNote.startNote;
                                        noteNumberSmaller = j;
                                    }
                                }

                                //2C20

                                // foundBigger = true;
                                if (tempNote.note == note && tempNote.channel == channel && tempNote.startNote >= momentNote && tempNote.startNote > biggestBigger)
                                {
                                    foundBigger = true;
                                    biggestBigger = tempNote.startNote;
                                    noteNumberBigger = j;
                                }
                            }

                            if (foundSmaller)
                            {
                                pileNote thisNote;
                                thisNote = ZEROgetNoteFromPile(pInitial[i], noteNumberSmaller);
                                thisNote.endNote = momentNote;
                                thisNote.thereIsAnEnd = true;
                                ZEROSetNoteInPile(pInitial[i], noteNumberSmaller, thisNote);
                                ZEROupdateNoteTimeToPlayMODEDIRECT(thisNote);
                            }
                            else if (foundBigger)
                            {
                                pileNote thisNote;
                                thisNote = ZEROgetNoteFromPile(pInitial[i], noteNumberBigger);
                                thisNote.endNote = momentNote;
                                thisNote.thereIsAnEnd = true;
                                // pileNote_fixStartEndNote(thisNote, 0);
                                ZEROSetNoteInPile(pInitial[i], noteNumberBigger, thisNote);
                                ZEROupdateNoteTimeToPlayMODEDIRECT(thisNote);
                            }
                        }
*/
                            }
                            // Nouvelle proposition 2C21 Couronne
                            if (record)
                            {
                                unsigned int taille = mesureTaillePileComplete(pInitial[i]);

                                bool foundBefore = false;
                                bool foundAfter = false;
                                unsigned long lastT = 0;
                                unsigned int lastTNoteNb = 0;

                                unsigned long momentNote = (timenote) % layerLength[i];
                                // int k = 0;

                                for (unsigned int j = 0; j < taille; j++)
                                {
                                    pileNote tempNote;
                                    tempNote = ZEROgetNoteFromPile(pInitial[i], j);

                                    if (tempNote.note == note && tempNote.channel == channel)
                                    {
                                        if (foundBefore == false && tempNote.startNote < momentNote)
                                        {
                                            foundBefore = true;
                                            lastT = tempNote.startNote;
                                            lastTNoteNb = j;
                                        }

                                        if (foundBefore == true && tempNote.startNote > lastT && tempNote.startNote < momentNote)
                                        {
                                            lastT = tempNote.startNote;
                                            lastTNoteNb = j;
                                        }

                                        if (foundBefore == false && tempNote.startNote > lastT && tempNote.startNote > momentNote)
                                        {
                                            lastT = tempNote.startNote;
                                            lastTNoteNb = j;
                                            foundAfter = true;
                                        }
                                    }
                                }

                                if (foundBefore || foundAfter)
                                {
                                    pileNote thisNote;
                                    thisNote = ZEROgetNoteFromPile(pInitial[i], lastTNoteNb);
                                    thisNote.endNote = momentNote;
                                    thisNote.thereIsAnEnd = true;
                                    ZEROSetNoteInPile(pInitial[i], lastTNoteNb, thisNote);
                                    ZEROupdateNoteTimeToPlayMODEDIRECT(thisNote);
                                }
                                else
                                {
                                    // ajout de note type 0x80 à la pile !!! TODO DEBUG
                                }
                            }
                        }
                    }
                }
            }
            break;

            default:
            {
            } // tous les CC, PC, F8 tombent ici
            break;
            }
        }
    }

    // partie gestion des boutons

    //MODULATIONS
    checkModulationButtons(); // doit rester avant les autres, car encode dans synthesePressed
    // la valeur des modulateurs pressés

    //NON MODULATIONS, ACTIONS
    checkTapTempoButton();

    checkJumpButton();

    checkDeleteButton(); // si OK : delete les layers actives
    // contient une vérification pour la transformation 424 : en mode STEPZ c'est la note qui est effacée uniquement

    checkLayersButtons(); // vérifie les boutons du bas pour le layer select

    checkOK_TransformationButton();

    checkMemoryButton();

    checkKaraktButton();

    // UNDO
    // PADS

    // Black07 uniformiser la dénomination des Checks : Check (type) Button(s)

    //     checkMuteButtons(); // disparaît dans une modalité de CheckButtonLayers ! BLACK06

    /* ébauche de la partie Evolution
    qd switchState5 : enregistrer la mélodie actuelle
    çàd melodyMatrix et melodyMatrixTime dnas l'EEPROM
    à la première place libre : séquentiellement toutes les valeurs
    avoir un pointeur qui indique la première place libre
    qd switchState6 : rappel d'une mélodie enregistrée
    random (nb de mélodies enregistrées)
    et lire à partir du début pour remplir les matrix
    qd switchState7 : effacer la dernière mélodie sélectionnée :
    pointer là et déplacer les datas suivant de la durée du bloc
    qd switchState8 : création d'une mélodie mutante à partir de 2
    enregistrements random du pool
    on peut presque déjà créer tout cela avec des procédures où
    la partie EEPROM serait discrètement laissée vide
    */

    // PICCOLO BLACK : gestion encodeur rotatif

    // TIMER UPDATE ENCODER
    bool changeEncoder = false;

    noInterrupts();
    changeEncoder = readyToUpdateEncoder;
    readyToUpdateEncoder = false;
    interrupts();

    if (changeEncoder == true) // cela veut dire que le timer a décidé qu'il fallait vérifier si l'encodeur a bougé
    // on va donc savoir de combien il a bougé durant le temps spécifié par timeToCheckEncoder ( initialement 1 seconde )
    {
        // RIGHT

        differenceHardwareRight = oldHardwareRight - oldHardwareEncoderRight;
        absolutedifferenceHardwareRight = abs(differenceHardwareRight);

        differenceSoftwareRight = oldSoftwareRight - olderSoftwareEncoderRight;
        absoluteDifferenceSoftwareRight = abs(differenceSoftwareRight);

        if (absoluteDifferenceSoftwareRight > (gapOfStepsToAccelerateRight * stepSizeRight))
        {
            stepSizeRight = stepSizeRight * multiplyRatioRight;
            buffer_print_atXY("*", 19, 0);

            if (stepSizeRight > maximumSoftwareValueEncoderRight)
            {
                stepSizeRight = maximumSoftwareValueEncoderRight;
            }
        }

        if (absoluteDifferenceSoftwareRight < (stepSizeRight))
        {
            stepSizeRight = stepSizeRight / multiplyRatioRight;

            if (stepSizeRight <= 1)
            {
                stepSizeRight = 1;
                buffer_print_atXY(" ", 19, 0);
            }
        }

        oldHardwareEncoderRight = oldHardwareRight;
        olderSoftwareEncoderRight = oldSoftwareRight;

        // LEFT

        differenceHardwareLeft = oldHardwareLeft - oldHardwareEncoderLeft;
        absolutedifferenceHardwareLeft = abs(differenceHardwareLeft);

        differenceSoftwareLeft = oldSoftwareLeft - olderSoftwareEncoderLeft;
        absoluteDifferenceSoftwareLeft = abs(differenceSoftwareLeft);

        if (absoluteDifferenceSoftwareLeft >= 1)
        {
            // cela signifie qu'on a tourné le bouton de gauche
            // on nettoie
            affiche = "";
            printRot2(affiche, 16, 400, 3);
            // buffer_clearScreen();
        }

        if (absoluteDifferenceSoftwareLeft > (gapOfStepsToAccelerateLeft * stepSizeLeft))
        {
            stepSizeLeft = stepSizeLeft * multiplyRatioLeft;
            if (stepSizeLeft > maximumSoftwareValueEncoderLeft)
            {
                stepSizeLeft = maximumSoftwareValueEncoderLeft;
            }
        }

        if (absoluteDifferenceSoftwareLeft < (stepSizeLeft))
        {
            stepSizeLeft = stepSizeLeft / multiplyRatioLeft;

            if (stepSizeLeft < 1)
            {
                stepSizeLeft = 1;
            }
        }

        oldHardwareEncoderLeft = oldHardwareLeft;
        olderSoftwareEncoderLeft = oldSoftwareLeft;

        // UP

        differenceHardwareUp = oldHardwareUp - oldHardwareEncoderUp;
        absolutedifferenceHardwareUp = abs(differenceHardwareUp);

        differenceSoftwareUp = oldSoftwareUp - olderSoftwareEncoderUp;
        absoluteDifferenceSoftwareUp = abs(differenceSoftwareUp);
/* 
        if (absoluteDifferenceSoftwareUp >= 1)
        {
            // cela signifie qu'on a tourné le bouton UP
            // on nettoie
            affiche = "";
           // printRot2(affiche, 16, 400, 3);
            // buffer_clearScreen();
            keyboardMode = softwareValueEncoderUp ;
 buffer_print_atXY(keyboardMode, 18, 0);

            if (keyboardMode == 0)
            {
                softPass = true;
                midiThru = true;
                MIDI3.turnThruOn(midi::Thru::Full);
            }
            else
            {
                softPass = false;
                midiThru = false;
                MIDI3.turnThruOff();
            }


        } */

/*
        if (absoluteDifferenceSoftwareUp > (gapOfStepsToAccelerateUp * stepSizeUp))
        {
            stepSizeUp = stepSizeUp * multiplyRatioUp;
            if (stepSizeUp > maximumSoftwareValueEncoderUp)
            {
                stepSizeUp = maximumSoftwareValueEncoderUp;
            }
        }

        if (absoluteDifferenceSoftwareUp < (stepSizeUp))
        {
            stepSizeUp = stepSizeUp / multiplyRatioUp;

            if (stepSizeUp < 1)
            {
                stepSizeUp = 1;
            }
        }
*/

        oldHardwareEncoderUp = oldHardwareUp;
        olderSoftwareEncoderUp = oldSoftwareUp;
    }

    // OLD PICCOLO

    softwareValueEncoderLeft = softwareValueEncoderLeft;





    if (softwareValueEncoderLeft != oldCounterLeft)
    {
        oldCounterLeft = softwareValueEncoderLeft;
        softwareValueEncoderRight = Transfo[softwareValueEncoderLeft][4];
        oldCounterRight = softwareValueEncoderRight;

        if (softwareValueEncoderLeft == 424) // STEPZ : on initialise Ctrl+Enc Right pour CH
        {
            oldCH = stepChannel + 1;
            softwareValueEncoderRightModulated[6] = stepChannel + 1;
            maximumSoftwareValueEncoderRightModulated[6] = 15;
            maximumSoftwareValueEncoderRightModulated[6] = 0;
        }
    }

    // TODO DEBUG prblème de conservation de la valeur qd on scrolle très rapidement !

    // Define Which Event
    if (Transfo[softwareValueEncoderLeft][0] == 126 && Evenement[CurrentEvenement].type == 2)
    {
        if (softwareValueEncoderRight > SizeTransfo)
        {
            // softwareValueEncoderRight = SizeTransfo ;
            // counter2Encoder = softwareValueEncoderRight * 4  ;
            // rotKnob2.write(counter2Encoder ) ;
            setCounterRight(SizeTransfo);
            // oldC2 = softwareValueEncoderRight ;
            //  oldC2 = SizeTransfo; // 1C88
        }

        if (softwareValueEncoderRight < 0)
        {
            // softwareValueEncoderRight = 0 ;
            // counter2Encoder = softwareValueEncoderRight * 4  ;
            // rotKnob2.write(counter2Encoder ) ;
            setCounterRight(0);
            // oldC2 = softwareValueEncoderRight ;
            //    oldC2 = 0; // 1C88
        }
    }

    else

        // Define Value, RAngeUP, RangeLO pour Event

        if (
            (
                Transfo[softwareValueEncoderLeft][0] == 134 || Transfo[softwareValueEncoderLeft][0] == 136 || Transfo[softwareValueEncoderLeft][0] == 137) &&
            Evenement[CurrentEvenement].type == 2)
    {

        if (softwareValueEncoderRight > Transfo[LineCallingTransfo(Evenement[CurrentEvenement].which)][2])
        {
            // softwareValueEncoderRight =Transfo[LineCallingTransfo(Evenement[CurrentEvenement].which)][2];
            // counter2Encoder = softwareValueEncoderRight * 4  ;
            // rotKnob2.write(counter2Encoder ) ;
            setCounterRight(Transfo[LineCallingTransfo(Evenement[CurrentEvenement].which)][2]);
            // oldC2 = softwareValueEncoderRight;
        }

        if (softwareValueEncoderRight < Transfo[LineCallingTransfo(Evenement[CurrentEvenement].which)][1])
        {
            // softwareValueEncoderRight = Transfo[LineCallingTransfo(Evenement[CurrentEvenement].which)][1] ;
            // counter2Encoder = softwareValueEncoderRight * 4  ;
            // rotKnob2.write(counter2Encoder ) ;
            setCounterRight(Transfo[LineCallingTransfo(Evenement[CurrentEvenement].which)][1]);
            //  oldC2 = softwareValueEncoderRight;
        }
    }

    else // affichage liés à la carte SD

        if (
            Transfo[softwareValueEncoderLeft][0] == 87 || Transfo[softwareValueEncoderLeft][0] == 82 || Transfo[softwareValueEncoderLeft][0] == 282 || Transfo[softwareValueEncoderLeft][0] == 286 || Transfo[softwareValueEncoderLeft][0] == 288 || Transfo[softwareValueEncoderLeft][0] == 290 || Transfo[softwareValueEncoderLeft][0] == 291 || Transfo[softwareValueEncoderLeft][0] == 292 || Transfo[softwareValueEncoderLeft][0] == 93 || Transfo[softwareValueEncoderLeft][0] == 84 || Transfo[softwareValueEncoderLeft][0] == 373 || Transfo[softwareValueEncoderLeft][0] == 508)
    {

        if (softwareValueEncoderRight > rootFileCount)
        {
            // softwareValueEncoderRight = rootFileCount ;
            // counter2Encoder = softwareValueEncoderRight * 4  ;
            // rotKnob2.write(counter2Encoder ) ;
            setCounterRight(rootFileCount);
            //  oldC2 = rootFileCount;
        }

        if (softwareValueEncoderRight < 1)
        {
            // softwareValueEncoderRight = 1 ;
            // counter2Encoder = softwareValueEncoderRight * 4  ;
            // rotKnob2.write(counter2Encoder ) ;
            setCounterRight(1);
            //   oldC2 = 1;
        }
    }

    /* BLACK 06 
    else

    {
        if (softwareValueEncoderRight > Transfo[softwareValueEncoderLeft][2])
        {
            // softwareValueEncoderRight = Transfo[softwareValueEncoderLeft][2] ;
            // counter2Encoder = softwareValueEncoderRight * 4  ;
            // rotKnob2.write(counter2Encoder ) ;
            setCounterRight(Transfo[softwareValueEncoderLeft][2]);
           // oldC2 = softwareValueEncoderRight;
        }

        if (softwareValueEncoderRight < Transfo[softwareValueEncoderLeft][1])
        {
            // softwareValueEncoderRight = Transfo[softwareValueEncoderLeft][1] ;
            // counter2Encoder = softwareValueEncoderRight * 4  ;
            // rotKnob2.write(counter2Encoder ) ;
            setCounterRight(Transfo[softwareValueEncoderLeft][1]);
          //  oldC2 = softwareValueEncoderRight;
        }
    }
*/

    // la transformation affichée c'était Counter-1
    // ça devient CounterEndUser ( 0 pour la première transformation )

    // temporairement on garde un counter2
    // counter = softwareValueEncoderLeft ;
    // counter2 = map(softwareValueEncoderRight,Transfo[softwareValueEncoderLeft][1],Transfo[softwareValueEncoderLeft][2],0,1024) ;

    if ((Transfo[softwareValueEncoderLeft][0]) != 45)
    {
        // on indique ici les transfos qui quand on change de valeur doivent réinitialiser // 45 c'est le varispeed
        varispeedState = 1;
    }

    ///// Gestion ici de tout l'affichage sur l'écran principal

    // ici si le menu a changé, reprendre l'ancienne valeur de cet affichage
    // TODO DEBUG
    // on peut sauver la valeur dans Transfo[][4], qui pourrait être ajouté

    Transfo[softwareValueEncoderLeft][4] = softwareValueEncoderRight;

    //********************************************************
    // partie AFFICHAGE DISPLAY
    //********************************************************

    affichageLine1();
    affichageLine2(Transfo[softwareValueEncoderLeft][0]);
    showMutedStatus();
    showActiveStatus();

    //********************************************************
    // partie player des NOTES
    //********************************************************

    // il joue d'abord la pile ONCE ( qui vient de DENDE )

    playAndStopFromONCEPile();

    // il va ici jouer les notes qu'il faut jouer une seule fois ( comme des délays )
    // mais aussi leurs notes OFF
    // et les note OFF de façon générale

    playFromBATCHPile();

    // Petit ménages de précaution à faire de temps en temps

    if (counterspeed % 100 == 0)
    {
        treatPileCompleteONCE(pointeurInitial); // effectue le ménage : supprime les notes dans la pile
        treatPileCompleteBATCH();

        for (byte i = 0; i < nbLayer; i++)
        {
            ZEROtreatPileComplete(pInitial[i]);
        }
        // BLACK09 - en voyant que le DEL de layers ne fonctionnait pas,
        // ceci ne me semble pas inutile, de temps  à autre !

        // précautions

        if (stepSizeRight < 1)
        {
            stepSizeRight = 1;
        }
        if (stepSizeLeft < 1)
        {
            stepSizeLeft = 1;
        }
        if (stepSizeUp < 1)
        {
            stepSizeUp = 1;
        }
        if (stepSizeRight > maximumSoftwareValueEncoderRight)
        {
            stepSizeRight = maximumSoftwareValueEncoderRight;
        }
        if (stepSizeLeft > maximumSoftwareValueEncoderLeft)
        {
            stepSizeLeft = maximumSoftwareValueEncoderLeft;
        }
        if (stepSizeUp > maximumSoftwareValueEncoderUp)
        {
            stepSizeUp = maximumSoftwareValueEncoderUp;
        }
    }
    // toutes les 10 itérations, fait le ménage

    // pour mettre une note sur cette pile : addNewNotetoPileONCE(pointeurInitial, NoteChannel, newPitch, newVelocity, moment_, NOTE_ON );

    // il passe en revue l'ensemble des notes pour voir s'il y en a une à jouer ou pas

    // 1C98 on va séparer le jeu des notes du jeu des CC !

    //********************************************************
    // partie player des notes 
    //********************************************************

    for (byte i = 0; i < nbLayer; i++)
    {
        //
        // byte z  =  ( ( muteLayer >> i ) & B00000001 );    // le bit de droite est 1 si layer 0 est MUTE ; le 2ème bit est 1 si layer 1 est MUTE, etc...
        //
        if (!(modeConstipe == true && (isLayerMuted(i) == true || isLayerEclipsed(i) == true || isWholeLayerFidelityMuted[i] == true)))
        {

            // fracRatio =  float ( ( float(ratioCurrent[i]) / 100 ) * ( float(ratioLength[i]) / 100 ) ) ;
            realRatio = (float)(((float)ratioCurrent[i] * (float)ratioLength[i]) / (float)100);
            // actualizedlayerLength =  int ( long ( ( layerLength[i] * realRatio) / 100 ) );
            actualizedlayerLength = int((float)(((float)layerLength[i] * (float)realRatio) / (float)100));

            // utilisation de la séquence pour une durée de barocco sur toute la layer
            // pq ce choix par layer plutôt que par note ?
            // à tester et à méditer TODO
            long ajoutViaSeq = 0;

            if (baroccoLayer[i] && nbDeMsecPourBarocco[i] < 0)
            {
                ajoutViaSeq = giveNextValue(sequence[-nbDeMsecPourBarocco[i] - 1], sequence[-nbDeMsecPourBarocco[i] - 1].compteur);
                sequence[-nbDeMsecPourBarocco[i] - 1].compteur++;
            }

            pileNote *playPtr = pInitial[i];

            if ((*playPtr).nextPile != 0)
            {
                do
                {
                    playPtr = (*playPtr).nextPile;

                    // for (byte j = 0; j < nbNotesByLayer; j++)
                  //  {
                        if ((*playPtr).note > 0 && ((*playPtr).type == 0x90) && (*playPtr).channel < 16)

                        {
                            // if ((((*playPtr).data2) & B00000001) == 0)
                            { // = played ?  0 = non   1 = oui

                                if (timeToPlay((*playPtr)))
                                // FC Qui à la fois dit s'il faut jouer ou pas,
                                // si OUI, préparer déjà le prochain time to play
                                {

                                    if ((isLayerMuted(i) == false && isLayerEclipsed(i) == false && isWholeLayerFidelityMuted[i] == false))
                                    {

                                        if (mustKillNote((*playPtr)) == false)
                                        {

int tempFi = getValueStraightOrFromSequence(fidelityLayer_[i], 0 );

                                            if ((int)random(100) < tempFi )

                                            { // à remplacer par une fonction plus rapide !!!
                                                // byte k = byte((*playPtr).data1 & B00001111); // valeur MIDI du channel ( les 4 autres bits = chord,etc. )
                                                // attention k utilisé un peu plus haut pour autre chose !
                                                byte k = (*playPtr).channel;

                                                (*playPtr).noteReallyPlayed = (*playPtr).note;       // valable pour ce qui est unpitchable ;
                                                (*playPtr).channelReallyPlayed = (*playPtr).channel; // valable pour ce qui est unpitchable ;

                                                (*playPtr).noteReallyPlayed = modifyPitchUniversal((*playPtr), (*playPtr).note, totalAjoutLayerNote[i]);

                                                int modifiedVelocite = (*playPtr).velocity + totalAjoutLayerVel[i];

                                                int ajout;

                                                for (byte t = 0; t < nbSequencesMax; t++)
                                                {

                                                    if (sequence[t].actif == true && sequence[t].compteLayer == false && sequence[t].tailleSeq > 0)
                                                    {
                                                        byte layerActivePourSeq = 0;

                                                        if (sequence[t].layer != 16)
                                                        {
                                                            layerActivePourSeq = ((sequence[t].layer >> i) & B00000001);
                                                        }
                                                        else
                                                        {
                                                            layerActivePourSeq = ((activeLayer >> i) & B00000001);
                                                        }

                                                        if (layerActivePourSeq == 1 && sequence[t].action != 4)
                                                        {
                                                            ajout = giveNextValue(sequence[t], sequence[t].compteur);
                                                            // le compteur est bien incrémenté plus bas !
                                                            // on trouve la description des différentes Séquences dans AFFICHAGE TF 239 !!!

                                                            if (sequence[t].action == 1 )
                                                            {
                                                                ajout = noteWrapper(ajout); // limite entre 0 et 127
                                                                // envoyer un CC
                                                                PlaynoteOn_(
                                                                    0xB0 + sequence[t].channel,
                                                                    sequence[t].CC,
                                                                    ajout);
                                                            }

                                                            if (sequence[t].action == 0 ) // ADD PITCH
                                                            {
                                                                (*playPtr).noteReallyPlayed = modifyPitchUniversal((*playPtr), (*playPtr).noteReallyPlayed, ajout);
                                                            }

                                                            if (sequence[t].action == 2 || sequence[t].action == 10)
                                                            {
                                                                // modifier la vélocité

                                                                modifiedVelocite = modifiedVelocite + ajout; // ajout peut être négatif
                                                                modifiedVelocite = noteWrapper(modifiedVelocite);
                                                            }

                                                            if (sequence[t].action == 3)
                                                            {
                                                                // SET Pitch
                                                                (*playPtr).noteReallyPlayed = ajout;
                                                            }

                                                            // TODO  SWING ( soon )
                                                            // 4 = Event-related
                                                            if (sequence[t].action == 5)
                                                            {
                                                                // modifier la longueur de note
                                                                // TODO DEBUG : horrible, car modifie réellement la durée de la note.
                                                                (*playPtr).endNote = (*playPtr).startNote + ajout;
                                                                pileNote_fixStartEndNote((*playPtr), 0);
                                                            }

                                                            if (sequence[t].action == 9)
                                                            {
                                                                // modifier la vélocité

                                                                modifiedVelocite = ajout; // ajout peut être négatif
                                                                modifiedVelocite = noteWrapper(modifiedVelocite);
                                                            }
                                                            if (sequence[t].action == 10)
                                                            {
                                                                // modifier la vélocité

                                                                modifiedVelocite += ajout; // ajout peut être négatif
                                                                modifiedVelocite = noteWrapper(modifiedVelocite);
                                                            }

                                                            if (sequence[t].action == 11)
                                                            {
                                                                modifiedVelocite = (int)((float)modifiedVelocite * (float)ajout / (float)100); // ajout peut être négatif
                                                                modifiedVelocite = noteWrapper(modifiedVelocite);
                                                            }

                                                            if (sequence[t].action == 12)
                                                            {
                                                                // modifier la longueur de note
                                                                // TODO DEBUG : horrible, car modifie réellement la durée de la note.
                                                                (*playPtr).endNote = (*playPtr).startNote + ajout;
                                                                pileNote_fixStartEndNote((*playPtr), 0);
                                                            }

                                                            if (sequence[t].action == 13)
                                                            {
                                                                (*playPtr).endNote = (*playPtr).endNote + ajout;
                                                                pileNote_fixStartEndNote((*playPtr), 0);
                                                            }

                                                            if (sequence[t].action == 14)
                                                            {
                                                                unsigned long dureeN = getDuree((*playPtr));
                                                                (*playPtr).endNote = (*playPtr).startNote + (unsigned long)dureeN * (unsigned long)ajout / (unsigned long)100;
                                                                pileNote_fixStartEndNote((*playPtr), 0);
                                                            }

                                                            if (sequence[t].action == 15) // ADD DEGREE
                                                            {
                                                                int differance = 0;
                                                                differance = noteADegreesAboveNoteBinGammeCmodeD(ajout, (*playPtr).noteReallyPlayed, gammeActuelle[i], modeActuelle[i]) - (*playPtr).noteReallyPlayed;
                                                                (*playPtr).noteReallyPlayed = modifyPitchUniversal((*playPtr), (*playPtr).noteReallyPlayed, differance); //2C22
                                                            }

                                                            if (sequence[t].action == 16) // ADD DEGREE RANDOM
                                                            {
                                                                int differance = 0;
                                                                int realAjout = random(-ajout, ajout + 1);
                                                                differance = noteADegreesAboveNoteBinGammeCmodeD(realAjout, (*playPtr).noteReallyPlayed, gammeActuelle[i], modeActuelle[i]) - (*playPtr).noteReallyPlayed;
                                                                (*playPtr).noteReallyPlayed = modifyPitchUniversal((*playPtr), (*playPtr).noteReallyPlayed, differance); //2C22
                                                            }

                                                            if (sequence[t].action == 17)
                                                            {
                                                                int realAjout = random(-ajout, ajout + 1);
                                                                modifiedVelocite = modifiedVelocite + realAjout; // ajout peut être négatif
                                                                modifiedVelocite = noteWrapper(modifiedVelocite);
                                                            }

                                                            if (sequence[t].action == 18)
                                                            {
                                                                if (ajout == 0)
                                                                {
                                                                    modifiedVelocite = 0; // = KILL NOTE !
                                                                }
                                                                else
                                                                {
                                                                    // rien ; // ajout peut être négatif
                                                                }
                                                                modifiedVelocite = noteWrapper(modifiedVelocite);
                                                            }

                                                            if (sequence[t].action == 19) // ADD PITCH RND
                                                            {
                                                                int realAjout = random(-ajout, ajout + 1);
                                                                (*playPtr).noteReallyPlayed = modifyPitchUniversal((*playPtr), (*playPtr).noteReallyPlayed, realAjout);
                                                            }

                                                            if (sequence[t].action == 20) // SET CH
                                                            {
                                                                // int realAjout = random(-ajout, ajout + 1);
                                                                (*playPtr).channelReallyPlayed = ajout % 16;
                                                                // TODO à revoir si on passen en 32CH sur 2 MIDI OUT !
                                                            }

                                                            if (sequence[t].action == 21) // ADD CH
                                                            {
                                                                // int realAjout = random(-ajout, ajout + 1);
                                                                (*playPtr).channelReallyPlayed = ((*playPtr).channelReallyPlayed + ajout) % 16;
                                                                // bon, le modulo 16 est peu élégant, mais oh combien efficace !
                                                                // à revoir si on passe en 32CH sur 2 MIDI OUT !
                                                            }

                                                            if (sequence[t].action == 22) // BUG CH : erreur entre CH et notes
                                                            {
                                                                (*playPtr).channelReallyPlayed = ((*playPtr).noteReallyPlayed + ajout) % 16;
                                                            }

                                                            sequence[t].compteur++;

                                                        } // fin du layer adéquat
                                                    }     // fin du séquence active

                                                } // fin du for t - parcours des séquences

                                                
                                                /* DISMISSE 2C22 
    //                                            if (markovChainGlobal == true)
                                                {
                                                    (*playPtr).noteReallyPlayed = putNoteAinMarkovChainAndGetOneOutAsResult((*playPtr).noteReallyPlayed);
                                                }
*/

                                                if (setPitchForWholeLayer[i] == true)
                                                {
                                                    (*playPtr).noteReallyPlayed = pitchForWholeLayer[i];
                                                }

                                                if (((CHSendToSampler >> k) & B0001) == 0)
                                                {
                                                    // pour le cas très précis du MicroSampler on a une procédure qui permet de déclarer vel 2 Vol
                                                    // çàd que les valeurs de vélocité sont transformés en modification de volume

                                                    unsigned long dureeNote;

                                                    // if ((*playPtr).startNote < (*playPtr).endNote)
                                                    // {
                                                    //     dureeNote = (*playPtr).endNote - (*playPtr).startNote;
                                                    // }
                                                    // else
                                                    // {
                                                    //     dureeNote = layerLength[i] - (*playPtr).startNote + (*playPtr).endNote;
                                                    // }

                                                    pileNote willBePlayed;
                                                    putNoteBinNoteA(willBePlayed, (*playPtr));
                                                    willBePlayed.velocity = modifiedVelocite;

                                                    if (participateLAYERMarkov[i] == true)
                                                    {
                                                        willBePlayed.velocity = modifiedVelocite;
                                                        willBePlayed = ZEROLayerMarkov(willBePlayed, i);
                                                    }

                                                    if (participateNEWMarkov[i] == true)
                                                    {
                                                        willBePlayed.velocity = modifiedVelocite;
                                                        willBePlayed = ZEROGeneralMarkov(willBePlayed);
                                                    }

                                                    if (thereIsCHAccent(willBePlayed.channel, i) == true)
                                                    {
                                                        // si le total accent est branché pour le layer en question
                                                        // on vérifie si la note est dans une zone accentuée
                                                        // si oui, valeur accentuée
                                                        // si non, valeur non accentuée
                                                        // = version BASIQUE
                                                        if (noteStartingAtAisinAccentZoneCHBLayerC(willBePlayed.startNote, willBePlayed.channel, i) == true)
                                                        {
                                                            willBePlayed.velocity = accentMax;
                                                        }
                                                        else
                                                        {
                                                            willBePlayed.velocity = accentMin;
                                                        }
                                                    }

                                                    if (thereIsTotalAccent(i) == true)
                                                    {
                                                        if (noteStartingAtAisinTotalAccentZoneLayerC(willBePlayed.startNote, i) == true)
                                                        {
                                                            willBePlayed.velocity = accentMax;
                                                        }
                                                        else
                                                        {
                                                            willBePlayed.velocity = accentMin;
                                                        }
                                                    }

                                                    dureeNote = getDuree(willBePlayed);
                                                    float realRatio;
                                                    realRatio = (float)(((float)ratioCurrent[i] * (float)ratioLength[i]) / (float)100.0);
                                                    dureeNote = (unsigned long)((float)dureeNote * (float)realRatio / (float)100);

                                                    if (harmonizeLayer[i] == true && pitchableLayer[i] == true && pitchableChannel[willBePlayed.channel] == true)
                                                    {
                                                        willBePlayed.noteReallyPlayed = dynamicHarmonize(willBePlayed.noteReallyPlayed, i); // seul lieu où cette fonction est utilisée.  à priori OK pour l'unpitchable
                                                    }
                                                    // 2C25 la fin du bug interminable de notes sustain ? ????

                                                    if ((checkNoteAChannelBinProxyBlockPiles(willBePlayed.noteReallyPlayed, willBePlayed.channel) == true) && (proxyBlockerON == true))
                                                    {
                                                        unsigned long lastStart = getTimeValue_from_NoteAChannelBinProxyBlockPiles(willBePlayed.noteReallyPlayed, willBePlayed.channel);

                                                        unsigned long nowProxyBlockLimit = abs(getValueStraightOrFromSequence(proxyBlockLimit, 1));

                                                        if ((nowMillis - lastStart) < nowProxyBlockLimit)
                                                        {
                                                            int proximity = getValueStraightOrFromSequence(proximityPitch, -36); // BLACK08
                                                                                                                                 //  willBePlayed.noteReallyPlayed += proximityPitch;
                                                            willBePlayed.noteReallyPlayed += proximity;
                                                        }
                                                    }

                                                    PlaynoteOn2(0x90 + willBePlayed.channelReallyPlayed,
                                                                willBePlayed.noteReallyPlayed,
                                                                willBePlayed.velocity,
                                                                i,
                                                                getDuree(willBePlayed));

                                                    // ici on effectue l'action faisant la syncopation ou pas au cas où deux layers joueraient la même note, de façon décalée
                                                    // NOTE ON             NOTE OFF
                                                    //        NOTE ON                   NOTE OFF
                                                    // syncopation :
                                                    // NOTE ON  NOTE ON    NOTE OFF     -
                                                    // non syncopation :
                                                    // NOTE ON  NOTE ON    -            NOTE OFF

                                                    // cela équivaut à chercher dans la pile Note Off (0x80) un note ayant même valeur de CH et de .note  ( pas forcément de layer )

                                                    if (KeepSyncopBUG == false)
                                                    {
                                                        // procédure qui va parcourir la pile de notes 0x80 et supprimer les note OFF de .channel et .note spécifié

                                                        // killNoteOffFromPileONCE(pointeurInitial, willBePlayed.channel, willBePlayed.noteReallyPlayed);

                                                        // me semble problématique avec  Dende et Delay qui peuvent intervenir entre deux...

                                                        treatPileCompleteONCE(pointeurInitial);
                                                    }

                                                    // calcul du moment OFF

                                                    addNewNotetoPileONCE(pointeurInitial, willBePlayed.channelReallyPlayed, willBePlayed.noteReallyPlayed,
                                                                         willBePlayed.velocity, nowMillis + dureeNote, 0x80, i);

                                                    // TODO DEBUG : il faudra incorporer ceci à PlayNOte2
                                                    // car il envoie vers plusieurs note via MIDI dispatch

                                                    // Rajoute à la pile ONCE le moment pour note OFF
                                                    // dans second temps il faudra vérifier qu'il n'y ait pas d'overlap de 2 notes : càd la pile contient un CH et Note idem, qu'il faut alors supprimer, s'il est entre maintenant et note off (forcément)

                                                    // calcul maintenant d'éventuel DELAIS
                                                    modifiedVelocite = willBePlayed.velocity;
                                                    int saveVelocity = modifiedVelocite;

                                                    int nowDropPourDelais = getValueStraightOrFromSequence(dropPourDelais[i], 0);

                                                    if (delayLayer[i] == true)
                                                    {

                                                        int nowRandomPourDelais = getValueStraightOrFromSequence(randomPourDelais[i], 0);

                                                        if ((int)random(100) < nowRandomPourDelais)
                                                        {
                                                            byte reducedVelocite;
                                                            byte firstReducedVelocite;
                                                            reducedVelocite = limiter((int)((float)modifiedVelocite * (float)nowDropPourDelais / (float)100), 0, 127);
                                                            firstReducedVelocite = reducedVelocite;
                                                            long timeStart = nowMillis;

                                                            int realNbDeDelais = 0;

                                                            if (nbDeDelais[i] >= 0)
                                                            {
                                                                realNbDeDelais = nbDeDelais[i];
                                                            }
                                                            else
                                                            {
                                                                realNbDeDelais = giveNextValue(sequence[-nbDeDelais[i] - 1], sequence[-nbDeDelais[i] - 1].compteur);
                                                                sequence[-nbDeDelais[i] - 1].compteur++;
                                                            }

                                                            // float ratioDecay = 1 ;
                                                            float deltaVelocite = 0;
                                                            if (realNbDeDelais > 1)
                                                            {

                                                                deltaVelocite = (((float)nowDropPourDelais - (float)downPourDelais[i]) * (float)modifiedVelocite) / ((float)100);
                                                                deltaVelocite = deltaVelocite / (float)(realNbDeDelais - 1);
                                                            }

                                                            long ajoutDelayViaSeq = 0;

                                                            int nowNbDeMsecPourDelais = getValueStraightOrFromSequence(nbDeMsecPourDelais[i], -1000);

ajoutDelayViaSeq = nowNbDeMsecPourDelais ; // BLACK 10 

                                                            for (byte p = 0; p < realNbDeDelais; p++)
                                                            {

timeStart += nowNbDeMsecPourDelais ; 

                                                                if (downPourDelais[i] < 0)
                                                                {
                                                                    int valuePercent = giveNextValue(sequence[-downPourDelais[i] - 1], sequence[-downPourDelais[i] - 1].compteur);
                                                                    sequence[-downPourDelais[i] - 1].compteur++;
                                                                    reducedVelocite = limiter((int)((float)modifiedVelocite * (float)valuePercent / (float)100), 0, 127);
                                                                }

                                                                // 2C28 on fait un dynamicHarmonize sur les délais pitchés également

                                                                if (harmonizeLayer[i] == true && pitchableLayer[i] == true && pitchableChannel[willBePlayed.channel] == true)
                                                                {
                                                                    willBePlayed.noteReallyPlayed = dynamicHarmonize(willBePlayed.noteReallyPlayed, i); // seul lieu où cette fonction est utilisée.  à priori OK pour l'unpitchable
                                                                }

                                                                int nowRandomSubFidelityPourDelais = getValueStraightOrFromSequence(randomSubFidelityPourDelais[i], 0);

                                                                if (nbDeMsecPourDelais[i] > 0 )
{
                                                                    if (bitMapDelay[i][p % 8] == 1 && (rand() % 100 < nowRandomSubFidelityPourDelais)) //2C28
                                                                    {
                                                                        addNewNotetoPileONCE(pointeurInitial, willBePlayed.channelReallyPlayed, willBePlayed.noteReallyPlayed,
                                                                                             reducedVelocite, timeStart, 0x90, i);
                                                                        addNewNotetoPileONCE(pointeurInitial, willBePlayed.channelReallyPlayed, willBePlayed.noteReallyPlayed,
                                                                                             0, timeStart + dureeNote, 0x80, i);
                                                                    }
}
else   // délais négatifs = prémonitions 
{
    if (bitMapDelay[i][p % 8] == 1 && (rand() % 100 < nowRandomSubFidelityPourDelais)) //2C28
    {
        addNewNotetoPileONCE(pointeurInitial, willBePlayed.channelReallyPlayed, willBePlayed.noteReallyPlayed,
                             reducedVelocite, timeStart + layerLength[i], 0x90, i);
        addNewNotetoPileONCE(pointeurInitial, willBePlayed.channelReallyPlayed, willBePlayed.noteReallyPlayed,
                             0, timeStart + dureeNote + layerLength[i] , 0x80, i);
    }
}
                                                                float reducedVelociteFL = (float)firstReducedVelocite - (float)((float)(p + 1) * (float)deltaVelocite);
                                                                reducedVelocite = limiter((int)reducedVelociteFL, 0, 127);
                                                            }
                                                        }
                                                    }

                                                    if (dende[i] == true)
                                                    {
                                                        int tempDendeFidelity = getValueStraightOrFromSequence(dendeFidelity[i], 0);

                                                        if ((int)random(100) < tempDendeFidelity)
                                                        {

                                                            int realNbDeDivide = 0;

                                                            if (dendeDivide[i] >= 0)
                                                            {
                                                                realNbDeDivide = dendeDivide[i];
                                                            }
                                                            else
                                                            {
                                                                // TODO
                                                                realNbDeDivide = giveNextValue(sequence[-dendeDivide[i] - 1], sequence[-dendeDivide[i] - 1].compteur);
                                                                sequence[-dendeDivide[i] - 1].compteur++;
                                                            }

                                                            // int realNbDeBitmap = B11111111;

                                                            if (dendeBitMapDelay[i] >= 0)
                                                            {
                                                                //  realNbDeBitmap = dendeBitMapDelay[i];
                                                            }
                                                            else
                                                            {
                                                                // TODO
                                                                int realNbDeBitmap = giveNextValue(sequence[-dendeBitMapDelay[i] - 1], sequence[-dendeBitMapDelay[i] - 1].compteur);
                                                                sequence[-dendeBitMapDelay[i] - 1].compteur++;

                                                                int nbCases = 8;

                                                                int t = realNbDeBitmap;

                                                                for (int j = 0; j < nbCases; j++)
                                                                {
                                                                    int k = 0;
                                                                    k = (t >> j) & 1;
                                                                    _bitMapDelay[i][7 - j] = k;
                                                                }
                                                            }

                                                            NoteChannel = willBePlayed.channelReallyPlayed;
                                                            NoteVelocity = willBePlayed.velocity;
                                                            NotePitch = willBePlayed.noteReallyPlayed;

                                                            if (repeaterNbDende[i] < 0)
                                                            {
                                                                int valueRepeater = giveNextValue(sequence[-repeaterNbDende[i] - 1], sequence[-repeaterNbDende[i] - 1].compteur);

                                                                // unsigned long newDelay = 0;
                                                                dendeSec[i] = (long)layerLength[i] * (long)valueRepeater / (long)32000;
                                                                dendeMSec[i] = ((long)layerLength[i] * (long)valueRepeater / (long)32) % 1000;

                                                                sequence[-potVal2 - 1].compteur++;
                                                            }

                                                            //    unsigned long duree_ = dendeSec[i] * 1000 + dendeMSec[i];
                                                            unsigned long duree_ = getValueStraightOrFromSequence(dendeSec[i], 0) * 1000 +
                                                                                   getValueStraightOrFromSequence(dendeMSec[i], 0); // BLACK08 inclusion Seq

                                                            unsigned long start_ = micros();
                                                            unsigned long end_ = start_ + duree_ * 1000;
                                                            initialEnd = end_; // utile pour le mode 2 du Fractal Delay

                                                            overLoadCounter = 0; // on l'initialise avant de lancer une fractalisation,
                                                                                 // pour limiter celle-ci et éviter un freeze
                                                            // float fDivide = (float)dendeDivide[i];
                                                            float fDivide = (float)realNbDeDivide;

                                                            // addNewNotetoPileONCE(pointeurInitial, NoteChannel, NotePitch, NoteVelocity, nowMillis + 100, NOTE_ON);
                                                            // juste pour entendre si fonctionne

                                                            int tempDendeLevels = getValueStraightOrFromSequence(dendeLevels[i], 1);
                                                            int tempDendeDecay = getValueStraightOrFromSequence(dendeDecay[i], 1);

                                                            FractalDelay(
                                                                start_,
                                                                end_,
                                                                fDivide,
                                                                NoteVelocity,
                                                                NotePitch,
                                                                tempDendeLevels,
                                                                tempDendeDecay,
                                                                i);
                                                        }
                                                    }
                                                    else
                                                    {
                                                    }

                                                    if (baroccoLayer[i] == true && sequence[baroccoChosenSeq[i]].tailleSeq > 0)
                                                    {
                                                        int nowRandomBarocco = getValueStraightOrFromSequence(randomPourBarocco[i], 0);
                                                        if ((int)random(100) < nowRandomBarocco)
                                                        {
                                                            int reducedVelocite;
                                                            int nowDropPourBarocco = getValueStraightOrFromSequence(dropPourBarocco[i], 0);

                                                            reducedVelocite = limiter((int)((float)saveVelocity * ((float)nowDropPourBarocco) / (float)100), 0, 127);
                                                            long timeStart = nowMillis;

                                                            // la séquence utilisée est :
                                                            // baroccoChosenSeq[i] ;
                                                            // sequence[baroccoChosenSeq[i]].tailleSeq
                                                            // sequence[baroccoChosenSeq[i]].tableauSeq[n] ;

                                                            // préparation de la séquence baroque en tant que telle

                                                            // version non destructive
                                                            int milieuBarocco = 0;
                                                            bool noReflet = true;
                                                            bool symH = true;
                                                            bool symV = true;
                                                            int total = 0;
                                                            int tailleSeqLocal = sequence[baroccoChosenSeq[i]].tailleSeq;

                                                            int nowRefletBarocco = getValueStraightOrFromSequence(refletBarocco[i], 0);
                                                            if ((int)random(100) < nowRefletBarocco)
                                                            {
                                                                noReflet = false;
                                                            }

                                                            int nowSymHBarocco = getValueStraightOrFromSequence(symHBarocco[i], 0);
                                                            if ((int)random(100) < nowSymHBarocco)
                                                            {
                                                                symH = false;
                                                            }

                                                            int nowSymVBarocco = getValueStraightOrFromSequence(symVBarocco[i], 0);
                                                            if ((int)random(100) < nowSymVBarocco)
                                                            {
                                                                symV = false;
                                                            }

                                                            int nowPreReflet = getValueStraightOrFromSequence(preReflet[i], 0);
                                                            if ((int)random(100) < nowPreReflet)
                                                            {
                                                                for (byte p = 0; p < tailleSeqLocal; p++)
                                                                {
                                                                    baroccoSequence.tableauSeq[p] = -1 * sequence[baroccoChosenSeq[i]].tableauSeq[p];
                                                                }
                                                            }
                                                            else
                                                            {
                                                                for (byte p = 0; p < tailleSeqLocal; p++)
                                                                {
                                                                    baroccoSequence.tableauSeq[p] = sequence[baroccoChosenSeq[i]].tableauSeq[p];
                                                                }
                                                            }

                                                            int nowPreH = getValueStraightOrFromSequence(preH[i], 0);
                                                            if ((int)random(100) < nowPreH)
                                                            {
                                                                int temp[tailleMaxTableauSequence] = {0};

                                                                for (byte p = 0; p < tailleSeqLocal; p++)
                                                                {
                                                                    temp[p] = baroccoSequence.tableauSeq[p];
                                                                }

                                                                for (byte p = 0; p < tailleSeqLocal; p++)
                                                                {
                                                                    baroccoSequence.tableauSeq[p] = temp[tailleSeqLocal - p - 1];
                                                                }
                                                            }

                                                            if (noReflet && symV)
                                                            {
                                                                baroccoSequence.tailleSeq = tailleSeqLocal * 2;
                                                                milieuBarocco = tailleSeqLocal;
                                                            }
                                                            else
                                                            {
                                                                // DE L AUTRE COTE DE L HORIZONTALE
                                                                baroccoSequence.tailleSeq = tailleSeqLocal * 2 + 1;
                                                                // le plus 1 est pour la chute
                                                                milieuBarocco = tailleSeqLocal;
                                                            }

                                                            if (symH)
                                                            {
                                                                // SYMETRIQUE HORIZONTALEMENT

                                                                for (byte p = 0; p < tailleSeqLocal; p++)
                                                                {
                                                                    // baroccoSequence.tableauSeq[p] = sequence[baroccoChosenSeq[i]].tableauSeq[p];
                                                                    total += sequence[baroccoChosenSeq[i]].tableauSeq[p];
                                                                    if (symV)
                                                                    {
                                                                        baroccoSequence.tableauSeq[baroccoSequence.tailleSeq - p - 1] = -baroccoSequence.tableauSeq[p];
                                                                    }
                                                                    else
                                                                    {
                                                                        baroccoSequence.tableauSeq[baroccoSequence.tailleSeq - p - 1] = baroccoSequence.tableauSeq[p];
                                                                    }
                                                                }
                                                            }
                                                            else
                                                            {
                                                                // ASSYMETRIQUE HORIZONTALEMENT

                                                                for (byte p = 0; p < tailleSeqLocal; p++)
                                                                {
                                                                    // baroccoSequence.tableauSeq[p] = sequence[baroccoChosenSeq[i]].tableauSeq[p];
                                                                    total += sequence[baroccoChosenSeq[i]].tableauSeq[p];
                                                                    if (symV)
                                                                    {
                                                                        baroccoSequence.tableauSeq[milieuBarocco + p] = -baroccoSequence.tableauSeq[p];
                                                                    }
                                                                    else
                                                                    {
                                                                        baroccoSequence.tableauSeq[milieuBarocco + p] = baroccoSequence.tableauSeq[p];
                                                                    }
                                                                }
                                                            }

                                                            if (noReflet == false)
                                                            {
                                                                baroccoSequence.tableauSeq[milieuBarocco] = -total;

                                                                if (symV == false)
                                                                {
                                                                    baroccoSequence.tableauSeq[milieuBarocco] = -2 * total;
                                                                }
                                                            }

                                                            if (noReflet == true)
                                                            {

                                                                if (symV == false)
                                                                {
                                                                    baroccoSequence.tableauSeq[milieuBarocco] = -total;
                                                                }
                                                                else
                                                                {
                                                                    baroccoSequence.tableauSeq[milieuBarocco] = 0;
                                                                }
                                                            }

                                                            int noteBaroque = willBePlayed.noteReallyPlayed;

                                                            // TODO si on veut passer à ulisation des Seq pour Barocco non sur layer mais sur note, il faudrait mettre ici les conditions
                                                            // qui définissent ajoutViaSeq

                                                            for (byte p = 0; p < baroccoSequence.tailleSeq; p++)
                                                            {

                                                                if (nbDeMsecPourBarocco[i] >= 0)
                                                                {
                                                                    timeStart += (long)nbDeMsecPourBarocco[i];
                                                                }
                                                                else
                                                                {
                                                                    timeStart += (long)ajoutViaSeq;
                                                                }

                                                                noteBaroque = noteADegreesAboveNoteBinGammeCmodeD(baroccoSequence.tableauSeq[p], noteBaroque, gammeActuelle[i], modeActuelle[i]);

                                                                addNewNotetoPileONCE(pointeurInitial, willBePlayed.channelReallyPlayed, noteBaroque,
                                                                                     reducedVelocite, timeStart, 0x90, i);
                                                                addNewNotetoPileONCE(pointeurInitial, willBePlayed.channelReallyPlayed, noteBaroque,
                                                                                     0, timeStart + dureeNote, 0x80, i);
                                                            }
                                                        }
                                                    }

                                                    //  (*playPtr).timeToPlayON = nowMillis - ( nowMillis %  layerLength[i]) + layerLength[i] + (*playPtr).startNote ;
                                                    // DEBUG pour trouver où est l'erreur
                                                }
                                                else
                                                {
                                                    PlaynoteOnSampler(0x90 + k, (*playPtr).noteReallyPlayed, (*playPtr).velocity);
                                                }
                                            }

                                            // 1C92
                                            // (*playPtr).data2 = (*playPtr).data2 ^ B00000001; // we turn the first bit ON = played

                                        } // fin du mustKillNote
                                    }     // fin du modeNon Constipé

                                    // (*playPtr).data2 = (*playPtr).data2 ^ B00000001; // we turn the first bit ON = played

                                } // fin du Time To Play

                            } // fin du played silented

                            // if ( melodyMatrix[i][j][MMnote] > 0 ) {
                            //   if ( ( (melodyMatrix[i][j][MMdata2] >> 1 ) & B00000001 )  == 0 ) {    // déjà silented ?    0 = non   1 = oui
                        }

                        if ( ((*playPtr).type == 0x10) )
                        {
                            if (timeToPlay((*playPtr)))
                            {
                                if ((isLayerMuted(i) == false && isLayerEclipsed(i) == false && isWholeLayerFidelityMuted[i] == false))
                                {
                                    int tempFi = getValueStraightOrFromSequence(fidelityLayer_[i], 0 );

                                        if ((int)random(100) < tempFi )
                                        { // à remplacer par une fonction plus rapide !!!
                                            // byte k = byte((*playPtr).data1 & B00001111); // valeur MIDI du channel ( les 4 autres bits = chord,etc. )
                                            // attention k utilisé un peu plus haut pour autre chose !
                                           
                                            int intCH = (((int)(*playPtr).velocity) << 8) + ((int)(*playPtr).channel);
                                            int intNT = (int) (*playPtr).note;

                                            dmxTx.set(intCH, intNT);

                                        }
                                    } // end for OLD j
                                } // fin du DO WHILE
                        }




                
                    
                       }   while ((*playPtr).nextPile != 0)          ;

                            } // fin du IF NEXT != 0

        } // end for if layer mute

        derniermodulo[i] = (nowMicros % (actualizedlayerLength * 1000)); // modif 2C02

        // si mode lumière Edu
        // si dernimodulo < 100 : ledOn = 1
        // si derniermodulo > 100 : ledOn = 0

        // iterationCompteur++; // TODO sert-il réellement à qqch ?

        // TODO DEBUG annulé temporairement pour ne pas ralentir l'exécution
        // doit être remis sous forme de LED, impérativement
        if (true == false)
        {
            if (eduLight == true)
            {

                byte k = ((muteLayer >> i) & B00000001); // le bit de droite est 1 si layer 0 est MUTE ; le 2ème bit est 1 si layer 1 est MUTE, etc...
                if (k == B00000001)
                {
                    buffer_setCursor(10 + i, 5);
                    buffer_print("M");
                    if (ledOn[i] == 1)
                    {
                        // digitalWrite(32+i, LOW); // de sorte que i =0 pour layer 1 -> 32 .. 33 .. 34 .. 35

                        ledOn[i] = 0;
                    }
                }
                else
                {
                    if ((derniermodulo[i] < limitEduLight) && (ledOn[i] == 0))
                    {
                        ledOn[i] = 1;
                        // digitalWrite(32+i, HIGH); // de sorte que i =0 pour layer 1 -> 32 .. 33 .. 34 .. 35
                        buffer_setCursor(10 + i, 5);
                        buffer_print("*");
                    }

                    if ((derniermodulo[i] > limitEduLight) && (ledOn[i] == 1))
                    {
                        {
                            ledOn[i] = 0;
                            //    digitalWrite(32+i, LOW); // de sorte que i =0 pour layer 1 -> 32 .. 33 .. 34 .. 35
                            buffer_setCursor(10 + i, 5);
                            buffer_print(" ");
                        }
                        // TODO DEBUG : excessif car va envoyer cette info plein de fois
                    }
                }
            }

            // iterationCompteur = 0;
        }

        //************************************************
        // BOUCLAGE DE LAYER
        //************************************************

        if (derniermodulo[i] < avantderniermodulo[i])
        {

            // TRACKER

            if (trackerActif[i])
            {
                int seqChosen_forTracker;
                seqChosen_forTracker = trackerSeq[i];
                int trackerMemoryToLoad = giveNextValue(sequence[seqChosen_forTracker], sequence[seqChosen_forTracker].compteur);
                sequence[seqChosen_forTracker].compteur++;

                layerLength[i] = layerLength[trackerMemoryToLoad - 1 + 4];
                // doit précéder la copie, pour que les calculs soient OK
                ZEROcopyMemoryAtoLayerB(trackerMemoryToLoad - 1, i);
                ZEROupdatePileTimeToPlayMODEDIRECT(i);
            }

            // BLACK09

            if (trackerKaraktActif[i])
            {
                int seqChosen_forTracker;
                seqChosen_forTracker = trackerKaraktSeq[i];
                int trackerMemoryToLoad = giveNextValue(sequence[seqChosen_forTracker], sequence[seqChosen_forTracker].compteur);
                sequence[seqChosen_forTracker].compteur++;

                if (trackerMemoryToLoad != 0)
                {
                    loadKarakt_from_KMem(trackerMemoryToLoad - 1);
                }

                // ça veut dire que SEQ 1 0 0 2 : va charger Karakt1, puis rien, rien, puis Karakt 2
                // très intéressant si 2 seq asychrones courrent en parallèle !
                // enfin, je crois, ou plutôt j'imagine

                // ZEROupdatePileTimeToPlayMODEDIRECT(i);
            }

            // TODO DEBUG : utilisé pour ne pas ralentir l'affichage, doit être remplacé par version LED
            if (true == false)
            {
                if (eduLight == false)
                {
                    byte k = ((muteLayer >> i) & B00000001); // le bit de droite est 1 si layer 0 est MUTE ; le 2ème bit est 1 si layer 1 est MUTE, etc...
                    if (k == B00000001)
                    {
                        buffer_setCursor(10 + i, 5);
                        buffer_print("M");
                        if (ledOn[i] == 1)
                        {
                            // digitalWrite(32+i, LOW); // de sorte que i =0 pour layer 1 -> 32 .. 33 .. 34 .. 35

                            ledOn[i] = 0;
                        }
                    }
                    else
                    {

                        // allumer la LED !
                        if (ledOn[i] == 0)
                        {
                            // digitalWrite(32+i, HIGH); // de sorte que i =0 pour layer 1 -> 32 .. 33 .. 34 .. 35
                            buffer_setCursor(10 + i, 5);
                            buffer_print("*");
                            ledOn[i] = 1;
                        }
                        else
                        {
                            // digitalWrite(32+i, LOW); // de sorte que i =0 pour layer 1 -> 32 .. 33 .. 34 .. 35
                            buffer_setCursor(10 + i, 5);
                            buffer_print(" ");
                            ledOn[i] = 0;
                        }
                    }
                }
            }

            // 2C20 VARIATIONS

            // SI Variations ON
            // alors calcul du nombre d notes dans le layer = t
            // parcours de 0 à t
            // si t déjà présente dans VariationMem -> Nada
            // sinon, ajouter note(t) avec channel à VaritionMem
            // après, calcul de tt, nombre de notes dans VariationMem
            // tant que (rnd Variation > %) && nbNotes dans VariationsMem > minmimum
            // delete une note au hasard
            // OU tant que (rnd Variation >%) && nbNotes > ( t - reduxMax )
            // delete une note au hasard
            // repasser en revue Layer, de 0 à t
            // si note appartien à VariationsMem -> CH = CH % 16
            // sinon, CH+16
            // bien indiquer dans le playNote que si CH > 16, pas de playNote.

            if (variationLayer[i] == true)
            {
                // SI Variations ON

                ZEROdeleteWholePile(pVariations); // 2C26

                // on vérfie qu'il a assez de fois répété la même variation
                if (compteurRepeatVariations[i] >= repeatVariations[i])
                {                                    // si s'est répété assez de fois
                    compteurRepeatVariations[i] = 1; // on réinitialise
                                                     // Serial.println();
                                                     //  Serial.print(compteurRepeatVariations[i]);
                                                     //  Serial.print(":");

                    // alors calcul du nombre d notes dans le layer = t

                    unsigned int taille = mesureTaillePileComplete(pInitial[i]);
                    //  Serial.print("nb=");
                    //  Serial.print(taille);

                    // parcours de 0 à t

                    for (unsigned int j = 0; j < taille; j++)
                    {
                        pileNote tempNote;
                        tempNote = ZEROgetNoteFromPile(pInitial[i], j);

                        // si t déjà présente dans VariationMem -> Nada
                        // sinon, ajouter note(t) avec channel à VaritionMem

                        // 2C26 on précise que c'est pour notes
                        if (tempNote.type != 0x90 || checkNoteAChannelBinVariationsPiles(tempNote.note, tempNote.channel) == true)
                        {
                            // NADA
                        }
                        else
                        {
                            // Ajout NOte à Variations
                            ZEROaddNewNotetoPile(pVariations, tempNote);
                            //  Serial.print("+");
                            // Serial.print(tempNote.note);
                        }

                    } // la pile Variations devrait contenir toutes les notes différentes
                      //  Serial.println();

                    int tailleVariations = mesureTaillePileComplete(pVariations);
                    int tailleVariationsInitiale = tailleVariations;
                    //  Serial.print("T");
                    //  Serial.print(tailleVariations);
                    //  Serial.print(" ");

                    // après, calcul de tt, nombre de notes dans VariationMem

                    // byte percentDisparitionChanceVariation[nbLayer] = {0, 0, 0, 0};
                    // byte minimumNotesVariation[nbLayer] = {1, 1, 1, 1};
                    // byte maximumReduxVariation[nbLayer] = {1, 1, 1, 1};
                    // bool modeMinimumVariation[nbLayer] = {true, true, true, true};

                    // tant que (rnd Variation > %) && nbNotes dans VariationsMem > minmimum
                    // delete une note au hasard
                    // OU tant que (rnd Variation >%) && nbNotes > ( t - reduxMax )
                    // delete une note au hasard

                    if (modeMinimumVariation[i] == true)
                    {
                        if (modeVariation[i] == 0) // CNT
                        {
                            while ((int)random(100) < percentDisparitionChanceVariation[i] && tailleVariations > minimumNotesVariation[i])
                            {
                                int chooseOneToDelete = random(tailleVariations);
                                //     Serial.print("D");
                                //     Serial.print(chooseOneToDelete);

                                ZEROdeleteNoteFromPile(pVariations, chooseOneToDelete);
                                tailleVariations = mesureTaillePileComplete(pVariations);
                                //    Serial.print("T");
                                //    Serial.print(tailleVariations);
                            }
                            //  Serial.println();
                        }
                        else // modeVariations == 1 DNG
                        {
                            int compteurVariations = 0;
                            while (compteurVariations < tailleVariationsInitiale && (int)tailleVariations > minimumNotesVariation[i])
                            {
                                compteurVariations++;
                                if ((int)random(100) < percentDisparitionChanceVariation[i])
                                {
                                    int chooseOneToDelete = random(tailleVariations);
                                    //  Serial.print("D");
                                    //   Serial.print(chooseOneToDelete);

                                    ZEROdeleteNoteFromPile(pVariations, chooseOneToDelete);
                                    tailleVariations = mesureTaillePileComplete(pVariations);
                                    //   Serial.print("T");
                                    //   Serial.print(tailleVariations);
                                }
                            }
                        }
                    }
                    else
                    { // modeMinimumVariation == false
                        if (modeVariation[i] == 0)
                        {
                            while ((int)random(100) < percentDisparitionChanceVariation[i] && tailleVariations > (tailleVariationsInitiale - maximumReduxVariation[i]))
                            {
                                int chooseOneToDelete = random(tailleVariations);
                                ZEROdeleteNoteFromPile(pVariations, chooseOneToDelete);
                                tailleVariations = mesureTaillePileComplete(pVariations);
                            }
                        }
                        else
                        {
                            byte compteurVariations = 0;
                            while (compteurVariations < tailleVariationsInitiale && tailleVariations > (tailleVariationsInitiale - maximumReduxVariation[i]))
                            {
                                compteurVariations++;
                                if ((int)random(100) < percentDisparitionChanceVariation[i])
                                {
                                    int chooseOneToDelete = random(tailleVariations);
                                    ZEROdeleteNoteFromPile(pVariations, chooseOneToDelete);
                                    tailleVariations = mesureTaillePileComplete(pVariations);
                                }
                            }
                        }
                    }

                    // repasser en revue Layer, de 0 à t

                    taille = mesureTaillePileComplete(pInitial[i]);

                    // parcours de 0 à t

                    for (unsigned int j = 0; j < taille; j++)

                    {
                        pileNote tempNote;
                        tempNote = ZEROgetNoteFromPile(pInitial[i], j);

                        // si note appartien à VariationsMem -> CH = CH % 16
                        // sinon, CH+16

                        if (tempNote.type == 0x90)
                        {
                            if (checkNoteAChannelBinVariationsPiles(tempNote.note, tempNote.channel) == true)
                            {
                                tempNote.channel = (tempNote.channel % 16); // çàd on active la note
                                ZEROSetNoteInPile(pInitial[i], j, tempNote);
                                //  Serial.print("Active");
                                //   Serial.print(tempNote.note);
                            }
                            else
                            {
                                tempNote.channel = (tempNote.channel % 16) + 16; // çàd on la désactive
                                ZEROSetNoteInPile(pInitial[i], j, tempNote);
                                //      Serial.print("Mute");
                                //     Serial.print(tempNote.note);
                            }
                        }
                    }

                    // bien indiquer dans le playNote que si CH > 16, pas de playNote.

                } // si repeat uniquement atteint
                else
                {
                    compteurRepeatVariations[i]++; // on ajoute 1 pour indiquer qu'on n'effectue pas le changement cette fois
                    // mais seulement qd on aura répété suffisamment de fois
                }
            }
            else
            { // Variations == FAUX
                // désactivation des valeurs > 16
                int taille = mesureTaillePileComplete(pInitial[i]);

                // parcours de 0 à t
                for (int j = 0; j < taille; j++)
                {
                    pileNote tempNote;
                    tempNote = ZEROgetNoteFromPile(pInitial[i], j);

                    // si note appartien à VariationsMem -> CH = CH % 16
                    // sinon, CH+16

                    tempNote.channel = (tempNote.channel % 16); // çàd on active la note
                    ZEROSetNoteInPile(pInitial[i], j, tempNote);
                }
            }

            // on va déterminer si on jouera la prochaine layer ou pas

            int nowFidelityWholeLayer = getValueStraightOrFromSequence(fidelityWholeLayer[i], 0);

            if ((int)random(100) < nowFidelityWholeLayer)
            {
                isWholeLayerFidelityMuted[i] = false;
            }
            else
            {
                isWholeLayerFidelityMuted[i] = true;
            }

            // PARTIE KANNON 2C02

            ZEROupdatePileTimeToPlayMODEDIRECT(i);

            int nowKannonRandom = getValueStraightOrFromSequence(kannonRandom[i], 0);

            if (kannonActif[i] == true && isLayerMuted(i) == false && (int)random(100) < nowKannonRandom)
            {

                unsigned int taille = mesureTaillePileComplete(pInitial[i]);

                int kaPitch = 0;

                /* BLACK 08, pour appliquer SEQ de plus en plus partout !
                if (kannonPitch[i] > -36)
                {
                    kaPitch = kannonPitch[i];
                    // çàd les valeurs normales du pitch, de -36 à +36
                }
                else
                {
                    int seqChosen;
                    seqChosen = -kannonPitch[i] - 37; // -37 devient 0 ; -38 devient 1 ; -39 devient 2 ; etc...
                    kaPitch = giveNextValue(sequence[seqChosen], sequence[seqChosen].compteur);
                    sequence[seqChosen].compteur++;
                }
                */

                kaPitch = getValueStraightOrFromSequence(kannonPitch[i], -36); // BLACK08

                if (kannonSeqBitmap[i] < 0)
                {
                    // cela signifie qu'on a une séquence pour indiquer la valeur de bitmap

                    int t = giveNextValue(sequence[-kannonSeqBitmap[i] - 1], sequence[-kannonSeqBitmap[i] - 1].compteur);

                    int nbCases = 8;
                    // int nombre = plusGrandNombreAvecAbits(nbCases);

                    for (int j = 0; j < nbCases; j++)
                    {
                        int k = 0;
                        k = (t >> j) & 1;
                        // bitMapDelay[i][7 - j] = k;
                        setBit(kannonBitmap[i], 7 - j, k);
                    }

                    sequence[-kannonSeqBitmap[i] - 1].compteur++;
                }

                int high = highestPitch(i);
                int low = lowestPitch(i);

                for (unsigned int j = 0; j < taille; j++)
                {
                    pileNote tempNote;
                    tempNote = ZEROgetNoteFromPile(pInitial[i], j);

                    // unsigned long dureeNote = getDuree(tempNote);

                    // unsigned long startKannonNote = nowMillis + (unsigned long) ( (float) (tempNote.timeToPlayON - nowMillis ) * kannonStretchRatio[i] ) +kannonDelay[i] ;

                    // unsigned long endKannonNote = startKannonNote + dureeNote ;

                    unsigned long startKannonLayer = nowMillis + kannonDelay[i];
                    unsigned long endKannonLayer = startKannonLayer + (unsigned long)((float)((float)layerLength[i] * (float)kannonStretchRatio[i] / (float)16));

                    unsigned long startKannonNote;
                    unsigned long endKannonNote;

                    int nowKannonRevert = getValueStraightOrFromSequence(kannonRevert[i], 0);

                    if ((int)random(100) < nowKannonRevert)
                    {
                        startKannonNote = endKannonLayer - (unsigned long)((float)((float)tempNote.endNote * (float)kannonStretchRatio[i] / (float)16));
                        endKannonNote = endKannonLayer - (unsigned long)((float)((float)tempNote.startNote * (float)kannonStretchRatio[i] / (float)16));
                    }
                    else
                    {
                        startKannonNote = startKannonLayer + (unsigned long)((float)((float)tempNote.startNote * (float)kannonStretchRatio[i] / (float)16));
                        endKannonNote = startKannonLayer + (unsigned long)((float)((float)tempNote.endNote * (float)kannonStretchRatio[i] / (float)16));
                    }

                    int pitch = tempNote.note;

                    int nowKannonReflet = getValueStraightOrFromSequence(kannonReflet[i], 0);

                    if ((int)random(100) < nowKannonReflet)
                    {
                        int delta = high - pitch;
                        pitch = low + delta;
                    }
                    else
                    {
                        // NADA
                    }

                    // 2C26 modif CH

                    int nowKannonDestination = getValueStraightOrFromSequence(kannonDestinationChanel[i], 0);

                    if (nowKannonDestination == 16)
                    {
                        // on ne change rien au channel
                    }
                    else
                    {
                        // on envoie le Kannon sur un autre channel
                        tempNote.channel = nowKannonDestination;
                    }

                    int reducedVelocite;
                    int nowDropPourKannon = getValueStraightOrFromSequence(dropPourKannon[i], 0);

                    reducedVelocite = limiter((int)((float)tempNote.velocity * ((float)nowDropPourKannon) / (float)100), 0, 127);

                    // if (bitMapDelay[i][p % 8] == 1)
                    // TODO remplacer par GetBit
                    if (((kannonBitmap[i] >> (j % 8)) & 1) == 1)
                    // DEBUG ou 7 - j%8 ?
                    {
                        int nowKannonRandomAppear = getValueStraightOrFromSequence(kannonRandomAppear[i], 0);

                        if ((int)random(100) < nowKannonRandomAppear)
                        {
                            addNewNotetoPileONCE(pointeurInitial, tempNote.channel, pitch + kaPitch,
                                                 reducedVelocite, startKannonNote, 0x90, i); // 2C28 drop velocity
                            addNewNotetoPileONCE(pointeurInitial, tempNote.channel, pitch + kaPitch,
                                                 0, endKannonNote, 0x80, i);
                        }
                    }
                }
            }

    // BLACK 10 : premonition 
            if (premonitionON[i] == true && isLayerMuted(i) == false && (int)random(100) > 0 )
{ // TOP

    unsigned int taille = mesureTaillePileComplete(pInitial[i]);

    int nowDelay = 0;

   nowDelay = getValueStraightOrFromSequence(premonitionValue[i], 1 ); // BLACK10

    for (unsigned int j = 0; j < taille; j++)
    {
        pileNote tempNote;
        tempNote = ZEROgetNoteFromPile(pInitial[i], j);

        int reducedVelocite;
        int nowDropPourKannon = getValueStraightOrFromSequence(dropPourPremonition[i], 0);

        reducedVelocite = limiter((int)((float)tempNote.velocity * ((float)nowDropPourKannon) / (float)100), 0, 127);

            int nowKannonRandomAppear = getValueStraightOrFromSequence(premonitionRandomAppear[i], 0);

            if ((int)random(100) < nowKannonRandomAppear)
            {
                addNewNotetoPileONCE(pointeurInitial, tempNote.channel, tempNote.note ,
                                     reducedVelocite, nowMillis + tempNote.startNote + layerLength[i] - nowDelay , 0x90, i); // 2C28 drop velocity
                addNewNotetoPileONCE(pointeurInitial, tempNote.channel, tempNote.note,
                                     0, nowMillis +  tempNote.endNote + layerLength[i] - nowDelay, 0x80, i);
            }
    }
    } // DOWN
                layerCounter[i]++;

            for (byte r = 0; r < NbEvenementMax; r++) // MODIF 1C86
            {
                // TODO devra être remplacé par une pile d'Evènements !

                if (Evenement[r].actif == true)
                { // vérifie que pas vide
                    if (Evenement[r].typeInterval == 4)
                    {
                        // si on est dans le type interval, il faut vérifier que l'Event t s'applique à la layer i
                        // Evenement[r].launch = false;

                        byte k = ((Evenement[r].moduloLayers >> i) & B00000001);

                        if (k == B00000001)
                        {

                            if ((layerCounter[i] + Evenement[r].moduloK) % (Evenement[r].modulo) == 0)
                            // && Evenement[r].moduloMode == 1

                            {
                                Evenement[r].launch = true;
                            }

                            // if (( ( layerCounter[i] + Evenement[r].moduloK ) % ( Evenement[r].modulo ) ) != 0
                            // && Evenement[r].moduloMode == 2 )
                            //
                            // { Evenement[r].launch = true ; }
                        }
                    }
                }
            } // fin de r - NbEvenementMax

            // TODO DEBUG
            // insérer ici le fait de mettre Evenement[i].launch ON si le décompte est lié à counter layer ou notes.
            // on met ON et l'EventPlayer mettra OFF

            // Sequence si comptage par layer

            for (byte t = 0; t < nbSequencesMax; t++)
            {

                if (sequence[t].actif == true && sequence[t].compteLayer == true && sequence[t].tailleSeq > 0)
                {

                    byte layerActivePourSeq = 0;

                    if (sequence[t].layer != 16)
                    {
                        layerActivePourSeq = ((sequence[t].layer >> i) & B00000001);
                    }
                    else
                    {
                        layerActivePourSeq = ((activeLayer >> i) & B00000001);
                    }

                    if (layerActivePourSeq == 1 && sequence[t].action != 4)
                    {

                        if (sequence[t].action == 1 )
                        {
                            int ajoutLayer = giveNextValue(sequence[t], sequence[t].compteur);
                            ajoutLayer = noteWrapper(ajoutLayer); // limite entre 0 et 127
                            // envoyer un CC
                            PlaynoteOn_(0xB0 + sequence[t].channel,
                                        sequence[t].CC,
                                        ajoutLayer);
                        }

                        if (sequence[t].action == 0)
                        {
                            ajoutLayerNote[i][t] = giveNextValue(sequence[t], sequence[t].compteur);
                        }
                        else
                        {
                            ajoutLayerNote[i][t] = 0;
                        }

                        if (sequence[t].action == 2)
                        {
                            // modifier la vélocité
                            ajoutLayerVel[i][t] = giveNextValue(sequence[t], sequence[t].compteur);
                        }
                        else
                        {
                            ajoutLayerVel[i][t] = 0;
                        }

                        if (sequence[t].action == 3)
                        {
                            // SET Pitch for whole layer !
                            setPitchForWholeLayer[i] = true;
                            pitchForWholeLayer[i] = giveNextValue(sequence[t], sequence[t].compteur);
                            // (*playPtr).noteReallyPlayed = ajout;
                        }
                        else
                        {
                            setPitchForWholeLayer[i] = false;
                        }

                        if (sequence[t].action == 5)
                        {
                            int ajout = giveNextValue(sequence[t], sequence[t].compteur);
                            // modifier la longueur de note

                            unsigned int taille = mesureTaillePileComplete(pInitial[i]);

                            for (byte j = 0; j < taille; j++)
                            {
                                pileNote tempNote;
                                tempNote = ZEROgetNoteFromPile(pInitial[i], j);
                                tempNote.endNote = tempNote.startNote + ajout;
                                pileNote_fixStartEndNote(tempNote, 0);
                                ZEROSetNoteInPile(pInitial[i], j, tempNote);
                                // TODO DEBUG bénéficierait d'une procédure plus rapide qui fasse POINT NOTE, dans la pile puis modifie la valeur d'emblée,
                                // sans devoir tout charger et tout replacer.

                                // melodyMatrixTime[i][j][MMTendnote] = melodyMatrixTime[i][j][MMTstartnote] + ajout;
                                // fixStartEndNote(i, j);
                                // TODO DEBUG me paraît problématique car modifie la valeur de durée de la note
                                // il faut que ce soit dynamique.
                            }
                        }

                        if (sequence[t].action == 6)
                        // SET LAYER LENGTH
                        {
                            int neoLongueur = giveNextValue(sequence[t], sequence[t].compteur);
                            layerLength[i] = checkLayer(neoLongueur);
                        }

                        if (sequence[t].action == 7)

                        {
                            int neoLongueur = giveNextValue(sequence[t], sequence[t].compteur);
                            layerLength[i] += neoLongueur;
                            layerLength[i] = checkLayer(layerLength[i]);
                        }

                        if (sequence[t].action == 8)

                        {
                            int neoLongueur = giveNextValue(sequence[t], sequence[t].compteur);
                            layerLength[i] = layerLength[i] + (unsigned long)neoLongueur * (unsigned long)layerLength[i] / (unsigned long)100;
                            layerLength[i] = checkLayer(layerLength[i]);
                        }

                        if (sequence[t].action == 12)
                        // NOTE LENGTH SET
                        {
                            int neoNoteL = giveNextValue(sequence[t], sequence[t].compteur);
                            // modifier la longueur de note

                            unsigned int taille = mesureTaillePileComplete(pInitial[i]);

                            for (byte j = 0; j < taille; j++)
                            {
                                pileNote tempNote;
                                tempNote = ZEROgetNoteFromPile(pInitial[i], j);
                                tempNote.endNote = tempNote.startNote + neoNoteL;
                                pileNote_fixStartEndNote(tempNote, 0);
                                ZEROSetNoteInPile(pInitial[i], j, tempNote);
                                // TODO DEBUG bénéficierait d'une procédure plus rapide qui fasse POINT NOTE, dans la pile puis modifie la valeur d'emblée,
                                // sans devoir tout charger et tout replacer.

                                // melodyMatrixTime[i][j][MMTendnote] = melodyMatrixTime[i][j][MMTstartnote] + ajout;
                                // fixStartEndNote(i, j);
                                // TODO DEBUG me paraît problématique car modifie la valeur de durée de la note
                                // il faut que ce soit dynamique.
                            }
                        }

                        if (sequence[t].action == 15) // ADD DEGREE
                        {
                            // int differance = 0;
                            // differance = noteADegreesAboveNoteBinGammeCmodeD(ajout, (*playPtr).noteReallyPlayed, gammeActuelle[i], modeActuelle[i]) - (*playPtr).noteReallyPlayed;
                            // (*playPtr).noteReallyPlayed = modifyPitchUniversal((*playPtr), (*playPtr).noteReallyPlayed, ajout);
                        }

                        if (sequence[t].action == 16) // ADD DEGREE RANDOM
                        {
                            // int differance = 0;
                            // int realAjout = random(-ajout, ajout + 1);
                            // differance = noteADegreesAboveNoteBinGammeCmodeD(realAjout, (*playPtr).noteReallyPlayed, gammeActuelle[i], modeActuelle[i]) - (*playPtr).noteReallyPlayed;
                            // (*playPtr).noteReallyPlayed = modifyPitchUniversal((*playPtr), (*playPtr).noteReallyPlayed, realAjout);
                        }

                        if (sequence[t].action == 19) // ADD PITCH RND
                        {
                            int ajout = giveNextValue(sequence[t], sequence[t].compteur);
                            int realAjout = random(-ajout, ajout + 1);
                            ajoutLayerNote[i][t] = realAjout;
                        }

                        if (sequence[t].action == 17)
                        {
                            // modifier la vélocité
                            int ajout = giveNextValue(sequence[t], sequence[t].compteur);
                            //    int realAjout = random(-ajout, ajout + 1);
                            // int     modifiedVelocite = modifiedVelocite + ajout; // ajout peut être négatif
                            int modifiedVelocite = ajoutLayerVel[i][t] + ajout; // ajout peut être négatif
                            modifiedVelocite = noteWrapper(modifiedVelocite);
                            ajoutLayerVel[i][t] = modifiedVelocite;
                        }

                        if (sequence[t].action == 9)
                        {

                            // modifiedVelocite = ajout; // ajout peut être négatif
                            // modifiedVelocite = noteWrapper(modifiedVelocite);
                        }

                        if (sequence[t].action == 18)
                        {
                            // if (ajout == 0)
                            // {
                            //     modifiedVelocit = 0; // = KILL NOTE !
                            // }
                            // else
                            // {
                            //     // rien ; // ajout peut être négatif
                            // }
                            // modifiedVelocite = noteWrapper(modifiedVelocite);
                        }

                        if (sequence[t].action == 11)
                        {
                            // modifiedVelocite = (int)((float)modifiedVelocite * (float)ajout / (float)100); // ajout peut être négatif
                            // modifiedVelocite = noteWrapper(modifiedVelocite);
                        }

                        if (sequence[t].action == 3)
                        {
                            // // SET Pitch
                            // (*playPtr).noteReallyPlayed = ajout;
                        }

                        // TODO  SWING ( soon )
                        // 4 = Event-related
                        if (sequence[t].action == 5)
                        {
                            // modifier la longueur de note
                            // TODO DEBUG : horrible, car modifie réellement la durée de la note.
                            // (*playPtr).endNote = (*playPtr).startNote + ajout;
                            // pileNote_fixStartEndNote((*playPtr), 0);
                        }

                        if (sequence[t].action == 12)
                        {
                            // modifier la longueur de note
                            // TODO DEBUG : horrible, car modifie réellement la durée de la note.
                            // (*playPtr).endNote = (*playPtr).startNote + ajout;
                            // pileNote_fixStartEndNote((*playPtr), 0);
                        }

                        // JE N'IMPLEMENTE PAS 20 et 21 : Set CH et Add CH, pour l'instant
                        // TODO

                        sequence[t].compteur++;

                    } // fin du layer adéquat

                } // fin du séquence active
                else
                {
                    ajoutLayerNote[i][t] = 0;
                    ajoutLayerVel[i][t] = 0;
                }

                if (sequence[t].actif == true && sequence[t].compteLayer == false)
                {
                    setPitchForWholeLayer[i] = false;
                }
            } // fin du for t - parcours des séquences

            // Fin de Séquence si comptage par layer
            // on doit maintenant calculer les nouveaux totaux par layer

            if (repeaterNb[i] < 0 && delayLayer[i] == true)
            {
                // cela signifie qu'on a une séquence pour indiquer la valeur de repeat
                int valueRepeater = giveNextValue(sequence[-repeaterNb[i] - 1], sequence[-repeaterNb[i] - 1].compteur);
                sequence[-repeaterNb[i] - 1].compteur++;

                // le délais est défini en 16e de layerLength
                unsigned long newDelay = 0;
                newDelay = (long)layerLength[i] * (long)valueRepeater / (long)32;
                nbDeMsecPourDelais[i] = (int)newDelay;

                dropPourDelais[i] = 100;
                downPourDelais[i] = 100;
            }

            for (int lfoRep = 0; lfoRep < nbLFOMax; lfoRep++)
            {
                if (repeaterNbLFO[lfoRep] < 0 && LFO[lfoRep].actif == true)
                {
                    // cela signifie qu'on a une séquence pour indiquer la valeur de repeat
                    int valueRepeater = giveNextValue(sequence[-repeaterNb[i] - 1], sequence[-repeaterNb[i] - 1].compteur);
                    sequence[-repeaterNbLFO[i] - 1].compteur++;

                    // le délais est défini en 32e de layerLength
                    unsigned long newDelay = 0;
                    newDelay = (long)layerLength[i] * (long)valueRepeater / (long)32;
                    LFO[lfoRep].duration = (unsigned long)newDelay;
                }
            }

            if (seqBitMapDelay[i] < 0 && delayLayer[i] == true)
            {
                // cela signifie qu'on a une séquence pour indiquer la valeur de bitmap

                int t = giveNextValue(sequence[-seqBitMapDelay[i] - 1], sequence[-seqBitMapDelay[i] - 1].compteur);

                int nbCases = 8;
                // int nombre = plusGrandNombreAvecAbits(nbCases);

                for (int j = 0; j < nbCases; j++)
                {
                    int k = 0;
                    k = (t >> j) & 1;
                    bitMapDelay[i][7 - j] = k;
                }

                sequence[-seqBitMapDelay[i] - 1].compteur++;
            }

            totalAjoutLayerNote[i] = 0;
            totalAjoutLayerVel[i] = 0;

            for (byte t = 0; t < nbSequencesMax; t++)
            {
                totalAjoutLayerNote[i] += ajoutLayerNote[i][t];
                totalAjoutLayerVel[i] += ajoutLayerVel[i][t];
            }

            // étiendre la LED
            // digitalWrite(32+i, LOW ); // de sorte que i =0 pour layer 1 -> 33 .. 34 .. 35 .. 36

        } // end du modulo plus petit que celui d'avant = fin de bouclage de layer

        avantderniermodulo[i] = derniermodulo[i];

    } // end for i

    //********************************************************
    // partie player des CC
    //********************************************************

    for (byte i = 0; i < nbLayer; i++)
    {
        if (!((isLayerMuted(i) == true || isLayerEclipsed(i) == true || isWholeLayerFidelityMuted[i] == true)))
        {
            realRatio = (float)(((float)ratioCurrent[i] * (float)ratioLength[i]) / (float)100);
            actualizedlayerLength = int((float)(((float)layerLength[i] * (float)realRatio) / (float)100));

            pileNote *playPtr = pInitial[i];

            if ((*playPtr).nextPile != 0)
            {
                do
                {
                    playPtr = (*playPtr).nextPile;
                    {
                        if ((*playPtr).type == 0xB0 || (*playPtr).type == 0xE0)
                        // ne s'intéresse ici qu'aux CC et Bend !
                        {
                            if (timeToPlay((*playPtr)))
                            // FC Qui à la fois dit s'il faut jouer ou pas,
                            // si OUI, préparer déjà le prochain time to play
                            {
                                // if ((isLayerMuted(i) == false && isLayerEclipsed(i) == false && isWholeLayerFidelityMuted[i] == false))

                                // if (random(100) <= fidelityLayer_[i])
                                // à remplacer par une fonction plus rapide !!!
                                // byte k = byte((*playPtr).data1 & B00001111); // valeur MIDI du channel ( les 4 autres bits = chord,etc. )
                                // attention k utilisé un peu plus haut pour autre chose !
                                byte k = (*playPtr).channel;

                                // int ajout;
                                // for (byte t = 0; t < nbSequencesMax; t++)
                                // fin du for t - parcours des séquences

                                // DEBUG : k semble tantôt exprimer un CHANNEL, tantot un numéro de LFO !!!

// BLACK 10 : on ajoute des SEQ pour CC fixés dans les layers

                                int ajout;
                                int modifiedVelocite = (*playPtr).velocity ; 

                                for (byte t = 0; t < nbSequencesMax; t++)
                                {
                                    if (sequence[t].actif == true && sequence[t].compteLayer == false && sequence[t].tailleSeq > 0)
                                    {
                                        byte layerActivePourSeq = 0;

                                        if (sequence[t].layer != 16)
                                        {
                                            layerActivePourSeq = ((sequence[t].layer >> i) & B00000001);
                                        }
                                        else
                                        {
                                            layerActivePourSeq = ((activeLayer >> i) & B00000001);
                                        }

                                        if (layerActivePourSeq == 1 && sequence[t].action != 4)
                                        {
                                            ajout = giveNextValue(sequence[t], sequence[t].compteur);
                                            // le compteur est bien incrémenté plus bas !
                                            // on trouve la description des différentes Séquences dans AFFICHAGE TF 239 !!!

// 23 SET    24 ADD    25 RND  
                                            if (sequence[t].action == 23)
                                            {
                                                // SET CC 
                                                modifiedVelocite = ajout;
                                            }

                                            if (sequence[t].action == 24  ) // ADD CC 
                                            {
                                                modifiedVelocite = modifiedVelocite + ajout; // ajout peut être négatif
                                                modifiedVelocite = noteWrapper(modifiedVelocite);
                                            }

                                            if (sequence[t].action == 25) // CC % 
                                            {
                                                modifiedVelocite = (int)((float)modifiedVelocite * (float)ajout / (float)100); // ajout peut être négatif
                                                modifiedVelocite = noteWrapper(modifiedVelocite);
                                            }

                                            if (sequence[t].action == 26 ) // CC RND 
                                            {
                                                int realAjout = random(-ajout, ajout + 1);
                                                modifiedVelocite = modifiedVelocite + realAjout;
                                                modifiedVelocite = noteWrapper(modifiedVelocite);
                                            }

                                                sequence[t].compteur++;

                                            } // fin du layer adéquat
                                    }     // fin du séquence active

                                } // fin du for t - parcours des séquences


                                if (machineSpecific[k] == 0)
                                {
                                    PlaynoteOn_((*playPtr).type + k,
                                                (*playPtr).note, modifiedVelocite );
                                }

                                if (machineSpecific[k] == 1) //TX81Z
                                {
                                    // PlaynoteOn_(0xB0 + k, (*playPtr).note, (*playPtr).velocity);
                                    convertData1and2toTX81ZModif((*playPtr).note, modifiedVelocite, k);
                                }

                                if (machineSpecific[k] == 2) // SHERMAN
                                {
                                    // PlaynoteOn_(0xB0 + k, (*playPtr).note, (*playPtr).velocity);
                                    convertData1and2toSHERMAN((*playPtr).note, modifiedVelocite, 15); // toujours 16
                                }

                                // fin du modeNon Constipé
                            } // fin du Time To Play
                              // fin du played silented
                        }
                    } // end for OLD j
                }     // fin du DO WHILE
                while ((*playPtr).nextPile != 0);
            } // fin du IF NEXT != 0
        }     // end for if layer mute
    }         // end for i

    //********************************************************
    // partie player des PINCH MULTI - 22C01
    // 10 mars 2022 
    //********************************************************

    for (byte i = 0; i < nbLayer; i++)
    {
        if (!((isLayerMuted(i) == true || isLayerEclipsed(i) == true || isWholeLayerFidelityMuted[i] == true)))
        {
            pileNote *playPtr = pInitial[i];

            if ((*playPtr).nextPile != 0)
            {
                do
                {
                    playPtr = (*playPtr).nextPile;
                    {
                        if ((*playPtr).type == 0x01)
                        // ne s'intéresse ici qu'aux PINCH MULTI
                        {
                            if (timeToPlay((*playPtr)))
                            {
                                Serial.print("*");
                                (*playPtr).timeToPlayON = nowMillis - (nowMillis % layerLength[i]) + layerLength[i] + (*playPtr).startNote ;

                                byte k = (*playPtr).channel;

                                int localPinchCHsize = getValueStraightOrFromSequence((*playPtr).pinchCH_size, 1);
                                for (int z = 0; z < localPinchCHsize; z++)
                                {
                                    int localPinchCH = getValueStraightOrFromSequence((*playPtr).pinchCH, 1);
                                    int localPinchCCsize = getValueStraightOrFromSequence((*playPtr).pinchCC_size, 1);
                                    for (int y = 0; y < localPinchCCsize; y++)
                                    {
                                        int localPinchCC = getValueStraightOrFromSequence((*playPtr).pinchCC, 0);

                                        int localPinchValue = getValueStraightOrFromSequence((*playPtr).pinchValue, 0);

                                        PlaynoteOn_(0xB0 + localPinchCH - 1, localPinchCC, localPinchValue);
                                    //    Serial.print(localPinchCH); Serial.print("-");
                                     //   Serial.print(localPinchCC);
                                     //   Serial.print("-");
                                     //   Serial.print(localPinchValue);
                                     //   Serial.print("-");
                                     //   Serial.println("//"); 
                                    }
                                }
                            }
                        }
                    } // end for OLD j
                }     // fin du DO WHILE
                while ((*playPtr).nextPile != 0);
            } // fin du IF NEXT != 0
        }     // end for if layer mute
    }         // end for i

    // 2C20 Fin de la DaubyModification (Thinner)

    // PARTIE MIDI PARALLEL Player
    // ****************************

    // si playParallelMidiON :
    // Si ParallelMIDION et
    // Si Millis de début > GoON : il est temps de lire une action midi et de l'exécuter
    //
    //   ( ReadMidiAction
    //   PLayMidiAction
    //   Read Next VLV )
    //     tant que VLV vaut zéro
    //     après : on fixe GoON à millis()de début + durée calculée du VLV
    //
    // DEBUT MIDI PLAYER PARALLEL

    parallelMidiPlayer(); // se trouve dans SubParts.h

    ///////////////////////////////////// FIN MIDI PLAYER PARALLEL

    // ****************************
    // PARTIE EVENT Player
    // développée à partir du 21/02/2018
    // ****************************

    // Si Millis de début > GoON : il est temps de lire une action midi et de l'exécuter

    EventtimeINIT = nowMillis;

    for (byte i = 0; i < NbEvenementMax; i++) // modif 1C86
    {
        // TODO devra être remplacé par une pile d'Evènements !

        if (Evenement[i].actif == true)
        { // vérifie que pas vide

            if (((Evenement[i].typeInterval != 4) && (EventtimeINIT >= Evenement[i].eventGOON)) || ((Evenement[i].typeInterval == 4) && (Evenement[i].launch == true)))
            {

                Evenement[i].launch = false; // TODO compléter le launche avec un travail sur l'analyse du layerCounter

                // TODO insérer ici tout le travail Conditionnel
                // structure générale : RésultatFonction (whichTest) opérateur valeur1 comparaison réference
                // ResultFc <- selon whichTest et la fonction
                // ResultFc = Resultat(Fc(Evenement.[whichTest]))
                // Transfo1 = ResultFc.  application opérateur avec valeur1
                // case Evenement[].whichOperation : 1 : ResultFc = ResultFc / Evenement[].whichValue, etc.
                // en fonction de whichComparison, compare Transfo1 avec Référence
                // case Evenement[]whichComparison : 1 : if ResultFc > Evenement[]whicReference => decide = TRUE
                // on a TRUE ou FALSE
                // on regarde ensuite si on veut BLCOK ou GO
                // if Evenement[].action = GO & decide = TRUE : actionAFaire = GO
                // if                    = BLOCK         TRUE :                DONT GO

                // on prend pour principe qu'il faut lancer
                // le travail sur condition coonsistera à dire quand il ne faut PAS lancer
                bool caPasse = true;
                bool fautTransfo = true;

                if (Evenement[i].conditionActive == true)
                {

                    long Resultat1 = 0;

                    if (Evenement[i].whichLayer2test != 16)
                    {
                        Resultat1 = resultatTest(Evenement[i].whichTest, Evenement[i].whichLayer2test);
                    }
                    else
                    {
                        Resultat1 = resultatTest(Evenement[i].whichTest, activeLayer);
                    }

                    switch (Evenement[i].whichOperation)
                    {
                    case 1:
                    {
                        Resultat1 = Resultat1 % Evenement[i].whichValue;
                    }
                    break; // modulo
                    case 2:
                    {
                        Resultat1 = Resultat1 / Evenement[i].whichValue;
                    }
                    break; // divisé
                    case 3:
                    {
                        Resultat1 = Resultat1 * Evenement[i].whichValue;
                    }
                    break; // x
                    case 4:
                    {
                        Resultat1 = (Resultat1 + 0) % Evenement[i].whichValue;
                    }
                    break; // %+
                    case 5:
                    {
                        Resultat1 = Resultat1 + Evenement[i].whichValue;
                    }
                    break; // +
                    // TODO nothing
                    default:
                    {
                    }
                    break;
                    }

                    switch (Evenement[i].whichComparison)
                    {
                    case 1:
                    {
                        if (Resultat1 < Evenement[i].whichReference)
                        {
                            caPasse = true;
                        }
                        else
                        {
                            caPasse = false;
                        }
                        break;
                    } // <
                    case 2:
                    {
                        if (Resultat1 > Evenement[i].whichReference)
                        {
                            caPasse = true;
                        }
                        else
                        {
                            caPasse = false;
                        }
                        break;
                    } // >
                    case 3:
                    {
                        if (Resultat1 == Evenement[i].whichReference)
                        {
                            caPasse = true;
                        }
                        else
                        {
                            caPasse = false;
                        }
                        break;
                    } // =
                    case 4:
                    {
                        if (Resultat1 != Evenement[i].whichReference)
                        {
                            caPasse = true;
                        }
                        else
                        {
                            caPasse = false;
                        }
                        break;
                    } // !=
                    default:
                    {
                        caPasse = true;
                    };
                    break;
                    }

                    if (caPasse == true && Evenement[i].whichAction == 1)
                    {
                        fautTransfo = true;
                    } // 1 DO   2 BLOCK
                    if (caPasse == true && Evenement[i].whichAction == 2)
                    {
                        fautTransfo = false;
                    } // 1 DO   2 BLOCK
                    if (caPasse == false)
                    {
                        fautTransfo = false;
                    } // 1 DO   2 BLOCK
                }

                if (fautTransfo == true)
                {
                    // alors on déclenche l'évènement en question
                    // // 1. si modification de CC
                    if (Evenement[i].type == 1) // 0 = vide  1=CC 2=Transfo
                    {
                        if (random(100) < Evenement[i].fidelite)
                        {

                            Evenement[i].compteur++;

                            if (
                                (
                                    Evenement[i].fugace == true &&
                                    Evenement[i].compteur < Evenement[i].echeance &&
                                    Evenement[i].compteur > Evenement[i].latence) ||
                                (Evenement[i].fugace == false &&
                                 Evenement[i].compteur > Evenement[i].latence))

                            {
                                // CC : 3 choses : B0 avec channel, nb, valeur

                                buffer_setCursor(0, 2);
                                buffer_print(String(i + 1));
                                buffer_print("=");
                                buffer_print(String(Evenement[i].compteur));
                                buffer_print("-");

                                PlaynoteOn_(0xB0 + Evenement[i].channel, Evenement[i].which, limiter(Evenement[i].valeur, 0, 127));
                                // REMINDER et la partie random, où est-elle passée ?
                                // elle est déterminée dans .valeur pour la prochaine fois !
                            }
                        }
                    }

                    // 2. si déclenchement d'une transformation
                    if (Evenement[i].type == 2) // 0 = vide  1=CC 2=Transfo
                    {
                        if (random(100) < Evenement[i].fidelite)
                        {

                            Evenement[i].compteur++;
                            if (
                                (
                                    Evenement[i].fugace == true &&
                                    Evenement[i].compteur < Evenement[i].echeance &&
                                    Evenement[i].compteur > Evenement[i].latence) ||
                                (Evenement[i].fugace == false &&
                                 Evenement[i].compteur > Evenement[i].latence))

                            {
                                // void Transformation(byte TTT , int BBB) {
                                //  potVal2 = map(BBB, 0, 1023, Transfo[LineCallingTransfo(1)][1] , Transfo[LineCallingTransfo(1)][2] );   // on ne peut pas se référer au tableau Transfo car l'ordre peut changer.  il faut que la procédure reçoive min et max en envoi si on veut pouvoir changer ça du tableau !

                                // lecture du potard ( rotary encoder en fait, qui simule 0-1023 )
                                // maintien de valeur 0-1023 choisie à partir de tableau Transfo LO-HI
                                // c'est cette valeur qui est transmise à la Transformation, et là identiquement retrnasformée
                                // donc ici au moment de fixer Valeur on va utiliser le même affichage que quand on lance Transformation

                                // Si Event actif sur layers spécifiques :
                                // 1. enregistrer activeLayer ; modifier layers pour Event ; remettre en place activeLayer
                                // Si Event actif sur les layers actifs : ne rien changer du tout
                                byte RecallActiveLayers;

                                RecallActiveLayers = activeLayer;

                                if (Evenement[i].layer != 16)
                                { // 16  signifie : agir sur les Active Layers
                                    // 0 = 0000   15 = 1111
                                    activeLayer = Evenement[i].layer;
                                }

                                buffer_setCursor(0, 2);
                                buffer_print(String(i + 1));
                                buffer_print("=");
                                buffer_print(String(Evenement[i].compteur));
                                buffer_print("-");

                                Transformation(Evenement[i].which, Evenement[i].valeur);

                                if (Evenement[i].which != 31)
                                {
                                    activeLayer = RecallActiveLayers;
                                }
                                // en effet, la seule transformation pour laquelle on ne doit pas restaurer les layers de départ
                                // c'est la modification qui choisit des layers au hasard ( ou séquentiel )
                            }
                        }
                    }

                    // puis on fixe de nouvelles valeurs
                    // TODO si le fidelité n'a pas fait jouer la valeur, il faut pas changer la valeur !!!!
                    // créer un mode où change et un où pas ! DEBUG ****

                    switch (Evenement[i].shape)
                    {

                    case 1:
                    {
                    }
                    break; // fixe : c'est simple on fout rien

                    case 2:
                    { // inc
                        Evenement[i].valeur += Evenement[i].incSize;
                        if (Evenement[i].valeur > Evenement[i].rangeUP_)
                        {
                            switch (Evenement[i].behaviorLimit)
                            {
                            case 1:
                                Evenement[i].valeur = Evenement[i].rangeUP_;
                                Evenement[i].shape = 3;
                                break;
                            case 2:
                                Evenement[i].valeur = Evenement[i].rangeLO_;
                                Evenement[i].shape = 2;
                                break;
                            case 3:
                                Evenement[i].valeur = Evenement[i].rangeUP_;
                                Evenement[i].shape = 2;
                                break;
                            default:
                                break;

                                // 1 = Change direction
                                // 2 = qd dépasse vers haut ou bas, réapparait de l'autre côté
                                // 3 = reste à la limite
                            }
                        }
                    }
                    break;

                    case 3:
                    { // dec

                        Evenement[i].valeur -= Evenement[i].incSize;
                        if (Evenement[i].valeur < Evenement[i].rangeLO_)
                        {
                            switch (Evenement[i].behaviorLimit)
                            {
                            case 1:
                                Evenement[i].valeur = Evenement[i].rangeLO_;
                                Evenement[i].shape = 2;
                                break;
                            case 2:
                                Evenement[i].valeur = Evenement[i].rangeUP_;
                                Evenement[i].shape = 3;
                                break;
                            case 3:
                                Evenement[i].valeur = Evenement[i].rangeLO_;
                                Evenement[i].shape = 3;
                                break;
                            default:
                                break;

                                // 1 = Change direction
                                // 2 = qd dépasse vers haut ou bas, réapparait de l'autre côté
                                // 3 = reste à la limite
                            }
                        }
                    }
                    break;

                    case 4:
                        Evenement[i].valeur = random(Evenement[i].rangeLO_, Evenement[i].rangeUP_);

                        break;

                    //  rnd
                    case 5:
                    { // liste sequential
                        int valeurSeq = 0;

                        // for ( byte t = 0 ; t < nbSequencesMax ; t++ )
                        // {
                        // version simplifiée de lecteur de Sequence

                        if (sequence[Evenement[i].seqUsedAsList].tailleSeq > 0)
                        {

                            valeurSeq = giveNextValue(sequence[Evenement[i].seqUsedAsList], sequence[Evenement[i].seqUsedAsList].compteur);

                            // on ne va pas vérifier que l'action est bien Event linked

                            // if ( sequence[t].action == 0 )
                            // {

                            sequence[Evenement[i].seqUsedAsList].compteur++;

                        } // fin du séquence active
                        //} // fin du for t - parcours des séquences

                        Evenement[i].valeur = valeurSeq;
                    };
                    break;

                    default:
                        break;
                    }

                    // et un nouvel .eventGOON
                    Evenement[i].eventGOON =
                        EventtimeINIT - ((long)EventtimeINIT % ((long)Evenement[i].intervalle * (long)100)) + ((long)Evenement[i].intervalle * (long)100) + ((long)Evenement[i].moment * (long)100);

                    // TODO DEBUG : il faut ici compléter avec les Intervalles RANDOM, GAUSS et BIPOLAR
                } // fin de FautTransfo
            }

            // Evenement[i].type = 1 ; // 0 = vide  1=CC 2=Transfo
            // pour les Transfo modifiant valeur, elle devront voir le type défini
            // Evenement[i].intervalle  = 150  ; // 1 = 0.1sec
            // Evenement[i].moment = 50 ; // dans les temps divisible par 300, agit après 100
            // Evenement[i].fidelite  = 100  ; // %
            // Evenement[i].which  =  17 ; // définit quel CC va être modifié
            // Evenement[i].valeur  = 55  ; // définit la valeur initiale qui lui sera attribuée
            // Evenement[i].shape   = 4  ; // 1=fixe 2=inc 3=Dec 4=rnd 5=liste sequential 6 = liste random
            // Evenement[i].rangeUP  = 100  ;
            // Evenement[i].rangeLO  = 10  ;
            // Evenement[i].incSize  = 0  ;
            // Evenement[i].behaviorLimit  = 0  ;
            // Evenement[i].channel  = 5 ; // à supposer que la machine recevant le CC soit en midi 5 (CH6)
            // Evenement[i].layer  =  0  ;   // tous layers actives - non pertinent pour un CC, car ne modifie pas de layers !
            // Evenement[i].eventGOON = millis() - ( millis() % Evenement[i].intervalle ) + Evenement[i].intervalle + Evenement[i].moment ;
        }
    }

    ///////////////////////////////////// FIN EVENT PLAYER

    // ****************************
    // PARTIE LFO Player
    // développée à partir d'octobre 2019 - implémentation 27 oct 2019, dans 2C14Sorted
    // ****************************

    // Si Millis de début > GoON : il est temps de lire une action midi et de l'exécuter

    for (byte i = 0; i < nbLFOMax; i++) // modif 2C14
    {
        // TODO devra être remplacé par une pile d'Evènements !

        if (LFO[i].actif == true)
        { // vérifie que pas vide

            if (nowMillis >= LFO[i].eventGOON)
            {
                // if (fautTransfo == true)
                {
                    LFO[i].compteur++;
                    int realLFOWhich = 0;

                    if (LFO[i].which >= 0)
                    {
                        realLFOWhich = LFO[i].which;
                    }
                    else
                    {
                        realLFOWhich = giveNextValue(sequence[-LFO[i].which - 1], sequence[-LFO[i].which - 1].compteur);
                        sequence[-LFO[i].which - 1].compteur++;
                    }

                    // pour EVENT, ici prenaient places les actions
                    // dans le cas qui nous intéresse, soit un envoi de CC

                    // alors on déclenche l'évènement en question
                    // // 1. si modification de CC
                    if (LFO[i].type == 1) // 0 = vide  1=CC 2=Transfo
                    {
                        if (
                            (LFO[i].compteur % LFO[i].modC == 0) &&
                            (LFO[i].valeur % LFO[i].modV == 0) &&
                            (random(100) < LFO[i].fidelite))

                        {

                            // 2C27
                            // ajout d'un random
                            int rdm;
                            if (LFO[i].LFORandom != 0)
                            {
                                rdm = random(0, LFO[i].LFORandom);
                                if (random(0, 2) == 0)
                                {
                                    rdm = -rdm;
                                }
                                LFO[i].valeur = limiter(LFO[i].valeur + rdm, 0, 127);
                            }

                            // CC : 3 choses : B0 avec channel, nb, valeur
                            // penser à limiter
                            switch (LFO[i].typeMachine)
                            // DEBUG
                            // double emploi avec
                            // machineSpecific[LFO[i].channel] !!!
                            {

                            case 0:
                            {
                                PlaynoteOn_(0xB0 + LFO[i].channel, realLFOWhich, LFO[i].valeur);
                            }
                            break;

                            case 1: // TX81Z
                            {
                                /*
                                if (realLFOWhich < 100) // 2C28
                                {
                                    // VCED
                                    Serial3.write(0xF0); // Exclusive
                                    Serial3.write(0x43); // Yamaha
                                    Serial3.write(0x10 + LFO[i].channel);
                                    Serial3.write(0x12); // VCED
                                    Serial3.write(realLFOWhich);
                                    Serial3.write(LFO[i].valeur);
                                    Serial3.write(0xF7);
                                }
                                else
                                {
                                    // ACED
                                    Serial3.write(0xF0);
                                    Serial3.write(0x43);
                                    Serial3.write(0x10 + LFO[i].channel);
                                    Serial3.write(0x13);               // ACED
                                    Serial3.write(realLFOWhich - 100); // car on encode de 0 à 100, mais ACED -> 0
                                    Serial3.write(LFO[i].valeur);
                                    Serial3.write(0xF7);
                                }
                                */
                                // 2C29
                                convertData1and2toTX81ZModif(realLFOWhich, LFO[i].valeur, LFO[i].channel);
                                verboseSerial(0xB0 + LFO[i].channel, realLFOWhich, LFO[i].valeur);
                                // on triche un peu pour savoir plus ou moins ce qu'on envoie au TX81Z !
                            }
                            break;

                            case 2:
                            {
                                convertData1and2toSHERMAN(realLFOWhich, LFO[i].valeur);
                            }
                            break;
                            }

                            // TODO devra être remplacé par sendCC(machineSpecific)
                        }
                    }

                    // 2. si déclenchement d'une transformation
                    if (LFO[i].type == 2) // 0 = vide  1=CC 2=Transfo
                    {
                        if (random(100) < LFO[i].fidelite)
                        {
                            {
                                byte RecallActiveLayers;

                                RecallActiveLayers = activeLayer;

                                if (LFO[i].layer != 16)
                                { // 16  signifie : agir sur les Active Layers
                                    // 0 = 0000   15 = 1111
                                    activeLayer = B1111;
                                    // Evenement[i].layer;
                                    // TODO DEBUG : il faut préciser quelles layers !
                                }

                                Transformation(realLFOWhich, LFO[i].valeur);

                                activeLayer = RecallActiveLayers;
                            }
                        }
                    }

                    // puis on fixe de nouvelles valeurs
                    // TODO si le fidelité n'a pas fait jouer la valeur, il faut pas changer la valeur !!!!
                    // créer un mode où change et un où pas ! DEBUG ****

                    // état des lieux : un CC vient d'être envoyé, au moment nowMillis ou presque
                    // on sait où le compteur en est, quelle est la direction actuelle
                    // on doit maintenant prévoir le prochain moment où il faudra balancer qqch

                    switch (LFO[i].shape)
                    {

                    case 1:
                    {
                    }
                    break; // fixe : c'est simple on fout rien

                    case 2:
                    { // SIN

                        int nbPoints = 2 * (LFO[i].highest - LFO[i].lowest) + 1;
                        int whereItsAt = LFO[i].compteur % (nbPoints - 1);

                        // buffer_and_HARD_setCursor(0, 6);
                        // buffer_and_HARD_print(whereItsAt);
                        // buffer_and_HARD_print("/");
                        // buffer_and_HARD_print(nbPoints);

                        // Serial.print(whereItsAt);
                        // Serial.print("\t");
                        // Serial.print(nbPoints);
                        // Serial.print("\t");

                        // DEBUG il y a quelque chose d'inélégant dans cette procédure

                        int newValue;
                        int mid = ((nbPoints - 1) / 2);

                        if (whereItsAt <= mid)
                        {
                            newValue = LFO[i].lowest + whereItsAt;
                        }
                        else
                        {
                            //                 newValue = LFO[i].highest - (whereItsAt - (nbPoints / 2));
                            int revert = nbPoints - whereItsAt - 1;
                            // newValue = LFO[i].lowest + (nbPoints - 1 - whereItsAt);
                            newValue = LFO[i].lowest + revert;
                        }

                        LFO[i].valeur = newValue;
                        // Serial.print(newValue);
                        // Serial.print("\t");

                        float valueSpan = LFO[i].highest - LFO[i].lowest;
                        // float mappedI = (float)(((((float)newValue - (float)LFO[i].lowest) / (float)valueSpan) * (float)2) - 1);
                        float mappedI = 2.0 * ((float)(newValue - LFO[i].lowest) / (float)valueSpan) - 1.0;
                        // fait un map de -1 à +1

                        // Serial.print(mappedI);
                        // Serial.print("\t");

                        if (mappedI < -1.0)
                        {
                            mappedI = -1.0;
                        }
                        if (mappedI > 1.0)
                        {
                            mappedI = 1.0;
                        }

                        float result = asin((float)mappedI);

                        // Serial.print(result);
                        // Serial.print("\t");

                        long M = millis();
                        // Serial.print(M);
                        // Serial.print("\t");

                        long residue = (M % LFO[i].duration);
                        // Serial.print( residue);
                        // Serial.print("\t");

                        unsigned long timeMin = millis() - (millis() % LFO[i].duration);
                        // Serial.print(timeMin);
                        // Serial.print("\t");
                        // DEBUG on ne peut pas utiliser des modulos avec des unsigned long, si ?
                        unsigned long timeMax = timeMin + LFO[i].duration;
                        // Serial.print(timeMax);
                        // Serial.print("\t");
                        unsigned long timeMid = timeMin + (LFO[i].duration / 2);
                        // Serial.print(timeMid);
                        // Serial.print("\t");

                        if (whereItsAt == 0)
                        {
                            if (residue > LFO[i].oldResidue)
                            {
                                LFO[i].eventGOON = timeMax;
                            }
                            else
                            {
                                LFO[i].eventGOON = timeMin;
                            }
                        }
                        else if (whereItsAt <= ((nbPoints - 1) / 2))
                        {
                            LFO[i].eventGOON = piToTime(result, (float)timeMin, (float)timeMid);
                            // buffer_and_HARD_print("UP ");
                        }
                        else
                        {
                            float first = piToTime(result, (float)timeMin, (float)timeMid);
                            float last = timeMid + (timeMid - first);
                            // float last = timeMax - (first - timeMin);
                            LFO[i].eventGOON = (unsigned long)last;
                            // buffer_and_HARD_print("DO ");
                        }

                        LFO[i].oldResidue = residue;

                        // Serial.print(LFO[i].eventGOON);
                        // Serial.println("\t");
                    }
                    break;

                    case 3:
                    { // SQR

                        // int nbPoints = 2 * (LFO[i].highest - LFO[i].lowest) + 1;
                        int whereItsAt = LFO[i].compteur % 2;

                        // Serial.print(whereItsAt);
                        // Serial.print("\t");

                        int newValue;

                        if (whereItsAt == 0)
                        {
                            newValue = LFO[i].lowest;
                        }
                        else
                        {
                            newValue = LFO[i].highest;
                        }

                        LFO[i].valeur = newValue;
                        // Serial.print(newValue);
                        // Serial.print("\t");

                        // long M = millis();
                        // Serial.print(M);
                        // Serial.print("\t");

                        // long N = (M % LFO[i].duration);
                        // Serial.print(N);
                        // Serial.print("\t");

                        unsigned long timeMin = millis() - (millis() % LFO[i].duration);
                        // Serial.print(timeMin);
                        // Serial.print("\t");
                        // DEBUG on ne peut pas utiliser des modulos avec des unsigned long, si ?
                        unsigned long timeMax = timeMin + LFO[i].duration;
                        // Serial.print(timeMax);
                        // Serial.print("\t");
                        unsigned long timeMid = timeMin + (LFO[i].duration / 2);
                        // Serial.print(timeMid);
                        // Serial.print("\t");

                        if (whereItsAt == 0)
                        {
                            LFO[i].eventGOON = timeMid;
                        }
                        else
                        {
                            LFO[i].eventGOON = timeMax;
                        }

                        // Serial.print(LFO[i].eventGOON);
                        // Serial.println("\t");
                    }
                    break;

                    case 4:
                    { // Sample And Hold

                        // int nbPoints = 2 * (LFO[i].highest - LFO[i].lowest) + 1;
                        int whereItsAt = LFO[i].compteur % 2;

                        // Serial.print(whereItsAt);
                        // Serial.print("\t");

                        int newValue;

                        newValue = random(LFO[i].lowest, LFO[i].highest + 1);

                        LFO[i].valeur = newValue;
                        // Serial.print(newValue);
                        // Serial.print("\t");

                        // long M = millis();
                        // Serial.print(M);
                        // Serial.print("\t");

                        // long N = (M % LFO[i].duration);
                        // Serial.print(N);
                        // Serial.print("\t");

                        unsigned long timeMin = millis() - (millis() % LFO[i].duration);
                        // Serial.print(timeMin);
                        // Serial.print("\t");
                        // DEBUG on ne peut pas utiliser des modulos avec des unsigned long, si ?
                        unsigned long timeMax = timeMin + LFO[i].duration;
                        // Serial.print(timeMax);
                        // Serial.print("\t");
                        unsigned long timeMid = timeMin + (LFO[i].duration / 2);
                        // Serial.print(timeMid);
                        // Serial.print("\t");

                        if (whereItsAt == 0)
                        {
                            LFO[i].eventGOON = timeMid;
                        }
                        else
                        {
                            LFO[i].eventGOON = timeMax;
                        }

                        // Serial.print(LFO[i].eventGOON);
                        //  Serial.println("\t");
                    }
                    break;

                    case 5:
                    { // SAW UP & DOWN

                        int nbPoints = 2 * (LFO[i].highest - LFO[i].lowest) + 1;
                        int whereItsAt = LFO[i].compteur % (nbPoints - 1);

                        // Serial.print(whereItsAt);
                        // Serial.print("\t");
                        // Serial.print(nbPoints);
                        // Serial.print("\t");

                        int newValue;
                        int mid = ((nbPoints - 1) / 2);

                        if (whereItsAt <= mid)
                        {
                            newValue = LFO[i].lowest + whereItsAt;
                        }
                        else
                        {
                            int revert = nbPoints - whereItsAt - 1;
                            newValue = LFO[i].lowest + revert;
                        }

                        LFO[i].valeur = newValue;
                        //   Serial.print(newValue);
                        //   Serial.print("\t");

                        float valueSpan = LFO[i].highest - LFO[i].lowest;
                        // float mappedI = (float)(((((float)newValue - (float)LFO[i].lowest) / (float)valueSpan) * (float)2) - 1);
                        float mappedI = 2.0 * ((float)(newValue - LFO[i].lowest) / (float)valueSpan) - 1.0;
                        // fait un map de -1 à +1

                        // Serial.print(mappedI);
                        // Serial.print("\t");

                        if (mappedI < -1.0)
                        {
                            mappedI = -1.0;
                        }
                        if (mappedI > 1.0)
                        {
                            mappedI = 1.0;
                        }

                        float result = mappedI;

                        // Serial.print(result);
                        // Serial.print("\t");

                        long M = millis();
                        // Serial.print(M);
                        // Serial.print("\t");

                        long residue = (M % LFO[i].duration);
                        // Serial.print(residue );
                        // Serial.print("\t");

                        double timeMin = millis() - (millis() % LFO[i].duration);
                        // Serial.print(timeMin);
                        // Serial.print("\t");
                        // DEBUG on ne peut pas utiliser des modulos avec des unsigned long, si ?
                        double timeMax = timeMin + LFO[i].duration;
                        // Serial.print(timeMax);
                        // Serial.print("\t");
                        double timeMid = timeMin + (LFO[i].duration / 2);
                        // Serial.print(timeMid);
                        // Serial.print("\t");

                        if (whereItsAt == 0)
                        {

                            if (residue > LFO[i].oldResidue)
                            {
                                LFO[i].eventGOON = timeMax;
                            }
                            else
                            {
                                LFO[i].eventGOON = timeMin;
                            }
                        }
                        else if (whereItsAt <= ((nbPoints - 1) / 2))
                        {
                            LFO[i].eventGOON = mapF(result, -1, 1, (float)timeMin, (float)timeMid);
                            // buffer_and_HARD_print("UP ");
                        }
                        else
                        {
                            float first = mapF(result, -1, 1, (float)timeMin, (float)timeMid);
                            float last = timeMid + (timeMid - first);
                            LFO[i].eventGOON = (unsigned long)last;
                        }

                        LFO[i].oldResidue = residue;

                        //     Serial.print(LFO[i].eventGOON);
                        //  Serial.println("\t");
                    }
                    break;

                    case 6:
                    { // RAMP UP , adapté de SIN

                        int nbPoints = (LFO[i].highest - LFO[i].lowest) + 1;
                        int whereItsAt = LFO[i].compteur % (nbPoints);

                        //Serial.print(whereItsAt);
                        // Serial.print("\t");
                        // Serial.print(nbPoints);
                        // Serial.print("\t");

                        int newValue;

                        newValue = LFO[i].lowest + whereItsAt;

                        LFO[i].valeur = newValue;

                        //   Serial.print(newValue);
                        //  Serial.print("\t");

                        // float valueSpan = LFO[i].highest - LFO[i].lowest;
                        // float mappedI = (float)(((((float)newValue - (float)LFO[i].lowest) / (float)valueSpan) * (float)2) - 1);
                        // float mappedI = 2.0 * ((float)(newValue - LFO[i].lowest) / (float)valueSpan) - 1.0;
                        // // fait un map de -1 à +1

                        // // Serial.print(mappedI);
                        // // Serial.print("\t");

                        // if (mappedI < -1.0)
                        // {
                        //     mappedI = -1.0;
                        // }
                        // if (mappedI > 1.0)
                        // {
                        //     mappedI = 1.0;
                        // }

                        // float result = mappedI;

                        // Serial.print(result);
                        // Serial.print("\t");

                        long M = millis();
                        // Serial.print(M);
                        // Serial.print("\t");

                        long residue = (M % LFO[i].duration);
                        //  Serial.print(residue );
                        //  Serial.print("\t");

                        double timeMin = millis() - (millis() % LFO[i].duration);
                        // Serial.print(timeMin);
                        // Serial.print("\t");
                        // DEBUG on ne peut pas utiliser des modulos avec des unsigned long, si ?
                        double timeMax = timeMin + LFO[i].duration;
                        // Serial.print(timeMax);
                        // Serial.print("\t");
                        // double timeMid = timeMin + (LFO[i].duration / 2);
                        // Serial.print(timeMid);
                        // Serial.print("\t");

                        if (whereItsAt == 0)
                        {

                            if (residue > LFO[i].oldResidue)
                            {
                                LFO[i].eventGOON = timeMax;
                            }
                            else
                            {
                                LFO[i].eventGOON = timeMin;
                            }
                        }
                        else
                        {
                            // LFO[i].eventGOON = mapF(result, -1, 1, (float)timeMin, (float)timeMax);
                            LFO[i].eventGOON = mapF(whereItsAt, 0, nbPoints, (float)timeMin, (float)timeMax);

                            // buffer_and_HARD_print("UP ");
                        }

                        LFO[i].oldResidue = residue;

                        //   Serial.print(LFO[i].eventGOON);
                        //   Serial.println("\t");
                    }
                    break;

                    case 7:
                    { // RAMP DOWN , adapté de RAMP UP

                        int nbPoints = (LFO[i].highest - LFO[i].lowest) + 1;
                        int whereItsAt = (nbPoints - (LFO[i].compteur % (nbPoints))) % nbPoints;
                        int realWhereItsAt = LFO[i].compteur % (nbPoints);
                        // WOW.   façon d'éviter si ==4, =0

                        //                                     Serial.print(whereItsAt);
                        // Serial.print("\t");
                        //  Serial.print(nbPoints);
                        //  Serial.print("\t");

                        int newValue;

                        newValue = LFO[i].lowest + whereItsAt;

                        LFO[i].valeur = newValue;

                        //  Serial.print(newValue);
                        //  Serial.print("\t");

                        // float valueSpan = LFO[i].highest - LFO[i].lowest;
                        // float mappedI = (float)(((((float)newValue - (float)LFO[i].lowest) / (float)valueSpan) * (float)2) - 1);
                        // float mappedI = 2.0 * ((float)(newValue - LFO[i].lowest) / (float)valueSpan) - 1.0;
                        // float mappedI = 2.0 * ((float)( newValue - LFO[i].lowest   ) / (float)valueSpan) - 1.0;
                        // fait un map de -1 à +1

                        // Serial.print(mappedI);
                        // Serial.print("\t");

                        // if (mappedI < -1.0)
                        // {
                        //     mappedI = -1.0;
                        // }
                        // if (mappedI > 1.0)
                        // {
                        //     mappedI = 1.0;
                        // }

                        // float result = mappedI;

                        // Serial.print(result);
                        // Serial.print("\t");

                        long M = millis();
                        // Serial.print(M);
                        //  Serial.print("\t");

                        long residue = (M % LFO[i].duration);
                        //   Serial.print(residue);
                        //  Serial.print("\t");

                        unsigned long timeMin = millis() - (millis() % LFO[i].duration);
                        // Serial.print(timeMin);
                        // Serial.print("\t");
                        // DEBUG on ne peut pas utiliser des modulos avec des unsigned long, si ?
                        unsigned long timeMax = timeMin + LFO[i].duration;
                        // Serial.print(timeMax);
                        // Serial.print("\t");
                        // double timeMid = timeMin + (LFO[i].duration / 2);
                        // Serial.print(timeMid);
                        // Serial.print("\t");

                        if (whereItsAt == 0)
                        {
                            if (residue > LFO[i].oldResidue)
                            {
                                LFO[i].eventGOON = timeMax;
                            }
                            else
                            {
                                LFO[i].eventGOON = timeMin;
                            }
                        }
                        else
                        {
                            LFO[i].eventGOON = mapF(realWhereItsAt, 0, nbPoints, (float)timeMin, (float)timeMax);
                            // buffer_and_HARD_print("UP ");
                        }

                        LFO[i].oldResidue = residue;

                        // Serial.print(LFO[i].eventGOON);
                        //  Serial.println("\t");
                    }
                    break;

                    default:
                        break;
                    }

                    // et un nouvel .eventGOON
                } // fin de FautTransfo
            }
        }
    }

    ///////////////////////////////////// FIN LFO PLAYER

    // 2C20 Automodification est obsoléte, supprimé

    // si le temps passe au-delà de la durée d'un layer, le modulo sera plus petit que le précédent
    // il faut alors remettre tous les compteurs played silented à zéro

    counterspeed++;

    if (counterspeed >= nbTicsPourCounterSpeed)
    {
        // long tempMillis = millis();
        // currentCounterSpeed = ( nbTicsPourCounterSpeed * 1000 ) / ( tempMillis - countertime1 ) ;

        long tempMicros = nowMicros;
        long dureeIterations = (tempMicros - countertime1) / nbTicsPourCounterSpeed;
        currentCounterSpeed = dureeIterations;

        counterspeed = 0;

        countertime1 = tempMicros;

        long Newmeancounterspeed = (meancounterspeed + currentCounterSpeed) / 2;
        meancounterspeed = Newmeancounterspeed;
    }

    if (
        Transfo[softwareValueEncoderLeft][0] != 30 && Transfo[softwareValueEncoderLeft][0] != 14 && Transfo[softwareValueEncoderLeft][0] != 246 && Transfo[softwareValueEncoderLeft][0] != 46 && Transfo[softwareValueEncoderLeft][0] != 48 && Transfo[softwareValueEncoderLeft][0] != 14)
    {
        showActiveStatus();
    }

    // 2C28 SUSTAIN BLOCKER

    unsigned int tailleSustain = mesureTaillePileComplete(pSustainBlock);

    if (tailleSustain == 0)
    {
    }
    else
    {
        pileNote tempNote;

        for (unsigned int j = 0; j < tailleSustain; j++)
        { // va parcourir la mélodie pointée ( Layer 1 à 4 : potVal3 )

            tempNote = ZEROgetNoteFromPile(pSustainBlock, j);

            unsigned long nowSustainBlockLimit = abs(getValueStraightOrFromSequence(sustainBlockLimit, 1));

            if ((sustainBlockerON == true) && ((nowMillis - tempNote.startNote) > (unsigned long)nowSustainBlockLimit))
            {
                //   Serial.println(nowMillis);
                //   Serial.println(tempNote.startNote);
                // Serial.println("------");

                // deleteNoteAChannelBinSustainBlockPiles(tempNote.note, tempNote.channel );
                PlaynoteOn_(0x80 + tempNote.channel, tempNote.note, 0);

                tailleSustain = mesureTaillePileComplete(pSustainBlock);
                // j = 0 ;
            }

            if ((sustainBlockerON == false) && ((nowMillis - tempNote.startNote) > (unsigned long)nowSustainBlockLimit))
            {
                //   Serial.println(nowMillis);
                //   Serial.println(tempNote.startNote);
                // Serial.println("------");

                deleteNoteAChannelBinSustainBlockPiles(tempNote.note, tempNote.channel);
                //  PlaynoteOn_(0x80 + tempNote.channel, tempNote.note, 0);

                tailleSustain = mesureTaillePileComplete(pSustainBlock);
                // j = 0 ;
            }
        }
    }

    // 2C28 PROXY BLOCKER

    {
        unsigned int tailleSustain = mesureTaillePileComplete(pProxyBlock);
        if (tailleSustain == 0)
        {
        }
        else
        {
            pileNote tempNote;

            for (unsigned int j = 0; j < tailleSustain; j++)
            { // va parcourir la mélodie pointée ( Layer 1 à 4 : potVal3 )

                tempNote = ZEROgetNoteFromPile(pProxyBlock, j);

                // pour gagner une opération, on peut calculer horizon = nowMillis - proxyBlockLimit
                // après on n'a plus qu'une comparaison à faire, plus de calcul

                if (((nowMillis - tempNote.startNote) > (unsigned long)30000))
                {
                    deleteNoteAChannelBinProxyBlockPiles(tempNote.note, tempNote.channel);

                    tailleSustain = mesureTaillePileComplete(pSustainBlock);
                }
            }
        }
    }

    // nettoyage

    for (byte i = 0; i < nbLayer; i++)
    {
        // *****************************************************
        // vérification des tailles maximales de layer
        // *****************************************************
        // 1 = none
        // 2 = Delete Oldest
        // 3 = delete random
        // 4 = delete ClosestPit      treatPileComplete(pInitial[i]); // on en profite pour un peu faire le ménage,0
        // TODO ? A chaque itération ??? me semble beaucoup !

        if (modeDeletionWhenExtraNotes[i] != 1)
        {
            unsigned int taille;

            // do {
            taille = mesureTaillePileComplete(pInitial[i]);

            if (taille > nbMaxDeNotesParLayer[i])
            {
                if (modeDeletionWhenExtraNotes[i] == 2)
                {
                    // DELETE OLDEST
                    ZEROdeleteNoteFromPile(pInitial[i], 0);
                }

                if (modeDeletionWhenExtraNotes[i] == 3)
                {
                    // DELETE RANDOM
                    ZEROdeleteNoteFromPile(pInitial[i], random(taille + 1));
                }

                if (modeDeletionWhenExtraNotes[i] == 4)
                {
                    // DELETE CLOSEST IN PITCH normalement, mais ici on ne sait pas !
                    // DONC
                    // DELETE RANDOM
                    ZEROdeleteNoteFromPile(pInitial[i], random(taille + 1));
                }
            }

            // } while ( taille > nbMaxDeNotesParLayer[i] ) ;
        }
    }

    oldCounterLeft = softwareValueEncoderLeft;
    oldCounterRight = softwareValueEncoderRight;

    // NEW PICCOLO

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
                bufferDisplay[newX][newY] = bufferWrite[newX][newY];
                t = tailleEcran; // on met fin à la routine, càd une mise à jour à la fois
            }
        }
    }

    long valueFader;

    valueFader = analogRead(A0);

    if (valueFader < measuredLowest)
    {
        measuredLowest = valueFader;
    }
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
 */

    valueFader = marginMap(valueFader, 10, 0, 127);

    valueFader = 127 - valueFader;

    // printRot2("Fader=" + String(valueFader) + "--", 9, 400, 5);

    if (valueFader > 60)
    {
        display = "Haut";
    }
    else
    {
        display = "Tournez Tournai";
    }
 

    ///////////////////////////////////////////////////////
    /// FIN DE LA BOUCLE MAJEURE
    ///////////////////////////////////////////////////////
}
