// Je reprends ici les fonctions d'affichage de Piccolo T2 pour les mener vers Piccolo Black 
// objectifs : 
// perfection d'exécution
// optimisation de rapidité
// encapsulation

/*
* cette partie est basée sur : 
 * Demo_NHD0420CW-Ax3_I2C.ino
 * // from http://www.nhdforum.newhavendisplay.com/index.php?topic=914.0
 */

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

typedef struct slotType slotType;
struct slotType
{
    byte x;
    byte y;
};

// CONSTANTES

String const nulString = "";

// VARIABLES

// F8 Send

byte nbBeatsPourLayer1 = 1;
bool sendF8 = false;
long dureeF8 = 625000 / 133; // bpm 133 d'office

String memorytexte = nulString;
byte previousl;
unsigned long tempoinit;
bool firsttime;
byte waitROT = 6; // décrit le nombre de fois que le temps de défilement est attendu en début et fin de scroll


unsigned long nowMicros = 0;
unsigned long nowMillis = 0;

// F8 Clock
// int F8Clock = 0;
bool itsTimeForF8 = false;
bool readyToSendF8 = false;

// Screen Clock
// int ScreenClock = 0;
bool itsTimeForScreen = false;
bool readyToUpdateScreen = false;

// printRot2
byte const printRotSlots(10);
String Rmemorytexte[printRotSlots] = nulString;
byte Rpreviousl[printRotSlots]{(0)};
unsigned long Rtempoinit[printRotSlots]{(0)};
bool Rfirsttime[printRotSlots];
long lastAffichage[printRotSlots] = {0};
// optimisation de l'affichage, max 30x/seconde
// contient en micros le moment du dernier affichage d'un slot

slotType slot[printRotSlots + 1];

const byte sizeX = 20 ; // indique le nombre de caractères en X
const byte sizeY = 4 ;  // indique le nombre de caractères en Y
const int tailleEcran = (int)sizeX * (int)sizeY;

char bufferWrite[sizeX][sizeY] = {0};
char bufferDisplay[sizeX][sizeY] = {0};

byte cursorX = 0;
byte cursorY = 0;


// DEFINITIONS : pour futur header 


// FONCTIONS

// nouvelles fonctions pour oLED 4x20

void oled4x20SetCursor(  byte x, byte y)
{
    y = y % 4 ; 
    command(new_line[y] + x); // déplace le curseur à l'endroit souhaité
}

void oledWriteCharXY(byte i, byte x, byte y)
{
    bufferDisplay[x][y] = i;
    bufferWrite[x][y] = i;
    command(new_line[y] + x); // déplace le curseur à l'endroit souhaité
    data(i);                  //  remplit la DDRAM avec un carctère, il semble que le curseur augment automatiquement
}

void oledWriteStringAtXY(String S, byte x, byte y)
{
    byte j;
    for (unsigned int i = 0; i < S.length(); i++)
    {
        j = S[i];
        oledWriteCharXY(j, x + i, y);
    }
}

void defineSlot(byte a, byte x_, byte y_)
{
    slot[a].x = x_;
    slot[a].y = y_;
}

void buffer_setCursor(byte x, byte y)
{
    cursorX = x;
    cursorY = y;
}

void buffer_and_HARD_setCursor(byte x, byte y)
// dispose le curseur sur l'écran virtuel ET en vrai !
{
    cursorX = x;
    cursorY = y;
    oled4x20SetCursor(x, y);  
}

void buffer_print(String Texte)
{
    int positionBuffer = cursorY * sizeX + cursorX;
    positionBuffer = positionBuffer % tailleEcran;

    byte longueur = Texte.length();

    for (int i = 0; i < longueur; i++)
    {
        bufferWrite[positionBuffer % sizeX][positionBuffer / sizeX] = (char)Texte[i];
        positionBuffer++;
        positionBuffer = positionBuffer % tailleEcran;
    }

    cursorX = positionBuffer % sizeX;
    cursorY = positionBuffer / sizeX;
}

void buffer_print_atXY(String Texte, byte x, byte y)
{
    x = x % sizeX ;
    y = y % sizeY ; 
    buffer_setCursor(x,y);
    buffer_print(Texte); 
}

void buffer_eraseN_atXY(byte n, byte x, byte y)
// dispose le curseur sur l'écran virtuel ET en vrai !
{
    int positionBuffer = y * sizeX + x ;

    byte longueur = n ;

    for (int i = 0; i < longueur; i++)
    {
        bufferWrite[positionBuffer % sizeX][positionBuffer / sizeX] = ' ';
 //       bufferDisplay[positionBuffer % sizeX][positionBuffer / sizeX] = ' ';
        // u8x8.print(Texte[i]); // LE SEUL QUI DOIT RESTER INTACT - plus tard remplacer par LCD, etc...
   //     oledWriteCharXY(Texte[i], positionBuffer % sizeX, positionBuffer / sizeX);
        positionBuffer++;
        positionBuffer = positionBuffer % tailleEcran;
    }
}

void buffer_and_HARD_print(String Texte)
// écrit un texte sur l'écran virtuel, actualisé progressivement
// mais va aussi l'écrire réellement sur l'écran
{
    int positionBuffer = cursorY * sizeX + cursorX;

    byte longueur = Texte.length();

    for (int i = 0; i < longueur; i++)
    {
        bufferWrite[positionBuffer % sizeX][positionBuffer / sizeX] = (char)Texte[i];
        bufferDisplay[positionBuffer % sizeX][positionBuffer / sizeX] = bufferWrite[positionBuffer % sizeX][positionBuffer / sizeX];
        // u8x8.print(Texte[i]); // LE SEUL QUI DOIT RESTER INTACT - plus tard remplacer par LCD, etc...
        oledWriteCharXY(Texte[i],positionBuffer % sizeX,positionBuffer / sizeX); 
        positionBuffer++;
        positionBuffer = positionBuffer % tailleEcran ; 
    }

    cursorX = positionBuffer % sizeX;
    cursorY = positionBuffer / sizeX;
}


void buffer_clearScreen(void)
{

    for (byte i = 0; i < sizeX; i++)
    {
        for (byte j = 0; j < sizeY; j++)
        {
            bufferWrite[i][j] = ' ';
        }
    }
}

void buffer_clearScreen_HARD(void)
{

    for (byte i = 0; i < sizeX; i++)
    {
        for (byte j = 0; j < sizeY; j++)
        {
            bufferDisplay[i][j] = '*';
            bufferWrite[i][j] = ' ';
            // va donc forcer un rewrite complet
        }
    }
}

void direct_clearScreen(void) // effaçage direct, au niveau matériel 
{

    for (byte i = 0; i < sizeX; i++)
    {
        for (byte j = 0; j < sizeY; j++)
        {
           //  bufferDisplay[i][j] = ' ';
           // bufferWrite[i][j] = ' ';
            // oled4x20SetCursor(i,j); // pas nécessaire car compris dans ligne suivante 
            oledWriteCharXY(' ',i,j); 
        }
    }
}

void buffer_clearLine(byte A)
{
    A = A % sizeY ; 
    for (byte j = 0; j < sizeX; j++)
    {
        bufferDisplay[j][A] = ' ';
    }
}

void buffer_clearLine_HARD(byte A)
{
    A = A % sizeY ; 
    for (byte j = 0; j < sizeX; j++)
    {
        bufferDisplay[j][A] = '*';
        bufferWrite[j][A] = ' ';
        // va donc forcer un rewrite complet
    }
}

void printRot2(String Rtexte, byte Rcases, int Rspeed, byte slotIci)
{
    /**
	* v3.0 affiche un texte déroulant
	*/
    // attention on ne peut avoir qu'un texte rotatif à la fois !!!
    // pour plusieurs messages on peut créer un indice dans les variables trnasmies
    // avec un layer poru les previousl([1][2][3],etc) et les memorytexte aussi
    // on peut maintenant utiliser 4 slots dont l'emplacement est défini par la procédure defineSlot

    // cette fonction clé devrait être optimisée ! elle est une des plus souvent appelées !
    // idées :
    // - passer le STring en référence
    // - insérer pour chaque Slot une mémoire du moment où la fonction a été appelée la dernière fois
    // il ne sert en effet à rien de rafraichir réellement plus de 30x/ seconde
    // il faut par contre que la toute dernière valeur reçue soit quand même affichée

    if ((nowMicros - lastAffichage[slotIci]) > 30000)
    {
        lastAffichage[slotIci] = nowMicros;

        byte Rlen = Rtexte.length();

        if ((Rfirsttime[slotIci] == true) || (Rmemorytexte[slotIci] != Rtexte))

        {
            Rfirsttime[slotIci] = true;
            Rmemorytexte[slotIci] = Rtexte;
            Rtempoinit[slotIci] = nowMillis;
        }
        else
        {
            Rfirsttime[slotIci] = false;
            if (Rlen < Rcases)
            {
                return;
            }
        }

        int divider = Rlen - Rcases + waitROT + waitROT;

        byte l = (int)((float)(nowMillis - Rtempoinit[slotIci]) / (float)Rspeed) % (divider);
        // byte l = (unsigned int) (  (  millis() -  Rtempoinit[slotIci] ) / (float) Rspeed )  % ( divider )  ;

if ((l != Rpreviousl[slotIci]) || (Rfirsttime[slotIci] == true))
{

    buffer_eraseN_atXY(Rcases, slot[slotIci].x, slot[slotIci].y);

    // 2C19 u8x8.setCursor(slot[slotIci].x, slot[slotIci].y);
    buffer_setCursor(slot[slotIci].x, slot[slotIci].y);

    if (l < waitROT)
    {
        l = 0;
    }
    else
    {
        l = l - (waitROT - 1);
    }
    // if ( l > (texte.length()  - cases )) { l = l - 1; }
    // vérifier que case n'est pas plus grand que texte !!!!

    if ((int)l > ((int)Rlen - (int)Rcases))
    {
        l = Rlen - Rcases;
    }

    if (Rcases < Rlen)
    {
        String temp;
        temp = Rtexte.substring(l, l + Rcases);
        // 2C19 u8x8.print(temp);
        buffer_print(temp);
    }
    else
    {
        buffer_print(Rtexte);

        for (int i = Rlen; i < Rcases; i++)
        {
            // u8x8.print(" ");
            buffer_print(" ");
        }
    }
    Rpreviousl[slotIci] = l;
        }

        Rfirsttime[slotIci] = false;
    }
}
