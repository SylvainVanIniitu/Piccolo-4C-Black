//*********************************************************
// SCREEN
//*********************************************************

/*
 * Demo_NHD0420CW-Ax3_I2C.ino
 * // from http://www.nhdforum.newhavendisplay.com/index.php?topic=914.0
 *
 * Tutorial sketch for use of character OLED slim display family by Newhaven with Arduino Uno, using
 * only Wire (I2C) library.  Models: NHD0420CW-Ax3, NHD0220CW-Ax3, NHD0216CW-Ax3. Controller: US2066
 * in this example, the display is connected to Arduino via I2C interface.
 *
 * Displays on the OLED alternately a 4-line message and a sequence of character "block".
 * This sketch assumes the use of a 4x20 display; if different, modify the values of the two variables
 * ROW_N e COLUMN_N.
 * The sketch uses the minimum possible of Arduino's pins; if you intend to use also /RES line,
 * the related instructions are already present, it's sufficient to remove the comment markers.
 *
 * The circuit:
 * OLED pin 1 (Vss)          to Arduino pin ground
 * OLED pin 2 (VDD)          to Arduino pin 5V
 * OLED pin 3 (REGVDD)       to Arduino pin 5V
 * OLED pin 4 (SA0)          to Vss ground   (to assign I2C address 0x3D, connect to VDD 5V)
 * OLED pin 5 and 6          to Vss ground
 * OLED pin 7 (SCL)          to Arduino pin A5 (SCL); 10K pull-up resistor on OLED pin
 * OLED pin 8 and 9 (SDAin,SDAout) to Arduino pin A4 (SDA); 10K pull-up resistor on OLED pin
 * OLED pin 10 to 15         to Vss ground
 * OLED pin 16 (/RES)        to Arduino pin Reset or VDD 5V (or to Arduino pin D13, to control reset by sw)
 * OLED pin 17 (BS0)         to Vss ground
 * OLED pin 18 (BS1)         to VDD 5V
 * OLED pin 19 (BS2)         to Vss ground
 * OLED pin 20 (Vss)         to Vss ground
 *
 * Original example created by Newhaven Display International Inc.
 * Modified and adapted to Arduino Uno 15 Mar 2015 by Pasquale D'Antini
 * Modified 19 May 2015 by Pasquale D'Antini
 *
 * This example code is in the public domain.
 */

//*********************************************************
// I²C SCANNER 
//*********************************************************

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

//*********************************************************
// CONTRASTE
//*********************************************************

// CODE POUR CHANGER LE CONTRASTE, MAIS APRES CA JE NE SAIS PLUS RIEN ECRIRE DE NOUVEAU
// IL FAUT VOIR COMMENT EN SORTIR : LIRE DOSSIER 2066 en entier
/* 
  command(0x78);
  command(0x2A);
  command(0x79);

  command(0x81); //set contrast control
  command(0x01); //set contrast control LOW

  delay(2000);

  command(0x81); //set contrast control
  command(0xFF); //set contrast control HIGH 
*/

//*********************************************************
// DIVERS
//*********************************************************

/// http://www.nhdforum.newhavendisplay.com/index.php?topic=914.0
// WIRE set SDA SCL
// https://forum.pjrc.com/threads/59547-Teensy-4-0-use-of-2-different-i2c-bus
// http://nhdforum.newhavendisplay.com/index.php/board,2.0.html
// https://community.particle.io/t/newhaven-oled-display-on-i2c/29188/6




//*********************************************************
// INTERNAL PULLUP PULLDOWN 
//*********************************************************

/* Input Pullup / Pulldown / Keeper Resistors All digital pins have optional pullup, pulldown, or keeper resistors.
These are used to keep the pin at logic HIGH or logic LOW or the same logic level when it is not being actively driven 
by external circuity.Normally these resistors are used with pushbuttons & switches.The pinMode function with 
INPUT_PULLUP or INPUT_PULLDOWN must be used to configure these pins to input mode with the built - in resistor.
*/

//*********************************************************
// FIX ENCODER
//*********************************************************

// Il a fallu changer une partie de Encoder.h - Jan 2021 

// problèmes avec encoder.h
/*
https://github.com/PaulStoffregen/Encoder/issues/44
The problem is because the reg macros are not defined for the teensy4 (IMXRT1062) in direct_pin_red.h
Adding the pin defines fixes this issue.

#if defined(__IMXRT1062__)
#define IO_REG_TYPE			    uint32_t
#define PIN_TO_BASEREG(pin)             (portInputRegister(digitalPinToPort(pin)))
#define PIN_TO_BITMASK(pin)             (digitalPinToBitMask(pin))
#define DIRECT_PIN_READ(base, mask)     (((*(base)) & (mask)) ? 1 : 0)
*/
