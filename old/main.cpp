/*
 * Tutorial sketch for use of character OLED slim display family by Newhaven with Teensy 4.1, using
 * only Wire (I2C) library.  Models: NHD0420CW-Ax3, NHD0220CW-Ax3, NHD0216CW-Ax3. Controller: US2066
 * in this example, the display is connected to Teensy 4.1 via I2C interface.
 *
 * Displays any string on the OLED  
 * This sketch assumes the use of a 4x20 display; if different, modify the values of the two variables
 * ROW_N e COLUMN_N.
 * The sketch uses the minimum possible of Teensy's pins; if you intend to use also /RES line,
 * please check the original version
 * 
 * Original example created by Newhaven Display International Inc.
 * Modified and adapted to Arduino Uno 15 Mar 2015 by Pasquale D'Antini
 * Modified 19 May 2015 by Pasquale D'Antini
 * Modified 7 jan 2021 by Sylvain van iniitu ( www.i
 *
 * This example code is in the public domain.
 */

#include <Arduino.h>
#include <Wire.h>

const byte ROW_N = 4;     // Number of display rows
const byte COLUMN_N = 20; // Number of display columns

//const byte RES = 13;                // Arduino's pin assigned to the Reset line (optional, can be always high)

const byte SLAVE2W = 0x3C; // Display I2C address, in 7-bit form: 0x3C if SA0=LOW, 0x3D if SA0=HIGH

byte new_line[4] = {0x80, 0xA0, 0xC0, 0xE0}; // DDRAM address for each line of the display
byte rows = 0x08;                            // Display mode: 1/3 lines or 2/4 lines; default 2/4 (0x08)
byte tx_packet[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// Packet to be transmitted (max 20 bytes)
// _______________________________________________________________________________________

void send_packet(byte x);

void command(byte c) // SUBROUTINE: PREPARES THE TRANSMISSION OF A COMMAND
{
    tx_packet[0] = 0x00; // Control Byte; C0_bit=0, D/C_bit=0 -> following Data Byte contains command
    tx_packet[1] = c;    // Data Byte: the command to be executed by the display
    send_packet(2);      // Transmits the two bytes
}
// _______________________________________________________________________________________

void data(byte d) // SUBROUTINE: PREPARES THE TRANSMISSION OF A BYTE OF DATA
{
    tx_packet[0] = 0x40; // Control Byte; C0_bit=0, D/C_bit=1 -> following Data Byte contains data
    tx_packet[1] = d;    // Data Byte: the character to be displayed
    send_packet(2);      // Transmits the two bytes
}
// _______________________________________________________________________________________

void send_packet(byte x) // SUBROUTINE: SEND TO THE DISPLAY THE x BYTES STORED IN tx_packet
{
    byte ix = 0; // Bytes index

    Wire.beginTransmission(SLAVE2W); // Begin the transmission via I2C to the display with the given address
    for (ix = 0; ix < x; ix++)       // One byte at a time,
    {
        Wire.write(tx_packet[ix]); //  queue bytes for transmission
    }
    Wire.endTransmission(); // Transmits the bytes that were queued
}
// _______________________________________________________________________________________

void oledWriteCharXY(byte i, byte x, byte y)
{
    command(new_line[y] + x); // moves the cursor
    data(i);                  //  fills DDRAM with char, and updates cursor
}

void writeStringAtXY(String S, byte x, byte y)
{
    byte j;
    for (unsigned int i = 0; i < S.length(); i++)
    {
        j = S[i];
        oledWriteCharXY(j, x + i, y);
    }
}

void setup(void) // INITIAL SETUP
{
    delayMicroseconds(200); // Waits 200 us for stabilization purpose
    Wire.begin();           // Initiate the Wire library and join the I2C bus as a master
    Wire.setSCL(19);
    Wire.setSDA(18);

    delay(10); // Waits 10 ms for stabilization purpose

    if (ROW_N == 2 || ROW_N == 4)
        rows = 0x08; // Display mode: 2/4 lines
    else
        rows = 0x00; // Display mode: 1/3 lines

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
 
    Serial.begin(9600);
}
// _______________________________________________________________________________________

void loop(void) // MAIN PROGRAM
{

    writeStringAtXY("Hello", 1, 0);
}