// #include <iostream>
// #include <stdlib.h>     /* srand, rand */
// #include <string.h>           /* strlen */
// #include <ctime>
// #include <windows.h>
// #include <conio.h>

// TODO
// Clean par suppression
// Clean par fluctuation
// Clean par reproduction

// Passage d'une gamme � une autre

// PROTOTYPES
bool isNoteInScaleMode(int z,int gammeActuelle,long  potVal3) ;
bool isNoteInTriadNumber(int z,int triadNumber ) ;
String GiveNameOfNote ( int NNN ) ;
void simplePartialSortZeroNotesv2(int layer ) ;

// https://www.basicmusictheory.com/c-sharp-major-scale

/* Unfortunately, most C++ compilers do not have any means of expressing binary numbers directly in source code.

A few allow the prefix 0b followed by a series of 0 and 1 digits, e.g. 0b11 == 3.
The C++ compiler used by Arduino does not support this syntax. However, starting in Arduino 0007,
the constants B0 through B11111111 are defined for your convenience.
*/

// const int nbDeModesConnus = 14;
// const unsigned long Scale[nbDeModesConnus] = { 2741L, 1453L, 2477L, 2733L, 661L, 1193L, 1717L, 2773L, 1749L, 1459L, 1709L, 1451L, 1387L, 1365L  } ;
// 1C89 mis dans variables

/*
0 ScaleMajor =  2741L;          // B101010110101  ;  // la note de d�part est le bit 0, qd espace d'un ton un 0 suit, puis un 1
// qd demi-ton, c'est un 1 qui suit directement ( suit dans le sens bit, c�d pr�c�de dans le sens lecture
// ce chiffre se lit donc : ton, ton, demi-ton, ton, ton, ton, demi-ton
1 ScaleMinorNatural =  1453L;   // 010110101101  ; // ton, demi-ton, ton, ton, demi-ton, ton, ton
2 ScaleMinorHarmonic = 2477L;   //  100110101101  ; // ton, demi-ton, ton, ton, demi-ton, 1 ton et demi, demi-ton
3 ScaleMinorMelodic =  2733L;   // 101010101101  ; // ton, demi-ton, ton, ton, ton, ton, demi-ton
4 ScalePentatonicMajor =  661L;      // 001010010101  ; // ton, ton, ton et demi, ton, ton et demi
5 ScalePentatonicMinor =  1193L;      // 010010101001  ; // ton et demi, ton, ton, ton et demi, ton
6 Mixolydian
7 Lydian
8 Lydian Dominant
9 Phrygian Dominant
10 Dorian
11 Phrygian
12 Locrian
13 Hexa  = 1365L / 10101010101

Janvier 2019 :
je refais tout le processus pour vérification
1. nom du mode
2. représentation des places des notes : 1 = notes, 0 = pas de notes.   l'espace entre les 1 décrit donc ton et demi-ton
3. inversion gauche-droite
4. écriture en binaire
5. équivalent décimal

MAJOR
101011 010101
101010110101
2741 OK

MIXOLYDIAN
101011 010110
011010110101
1717

LYDIAN
101010 110101
101011010101
2773

LYDIAN DOMINANT
101010 110110
011011010101
1749

PHRYGIAN DOMINANT
110011 011010
010110110011
1459

NATURAL MINOR
101101 011010
010110101101
1453 OK

DORIAN
101101 010110
011010101101
1709

HARMONIC MINOR
101101 011001
100110101101
2477 OK

MELODIC MINOR
101101 010101
101010101101
2733 OK

PHRYGIAN
110101 011010
010110101011
1451

LOCRIAN
110101 101010
010101101011
1387

DODECA  // 2C28 
111111 111111
111111111111
4095 

*/


// int activeLayer = 1 ;

// inline void wait_on_enter()
// {
//     std::string dummy;
//     std::cout << std::endl << "Enter to continue..." << std::endl;
//     std::getline(std::cin, dummy);
//     cout << endl << endl ;
// }

/////////////////////////////////////////////////////////////////////////////////////
// PILE VERSION : faite et vérifiée
// 1C94
/////////////////////////////////////////////////////////////////////////////////////

void newHarmonize( byte layer, int gammeActuelle  , long potVal3  )
/** \brief fonction qui va faire rentrer une melodymatrix dans une gamme donnée ( gammeActuelle ) et un un mode donné ( potVal3 )
*
* \param gammeActuelle décrit la note de base de la gamme.  0=C 1=C# 2=D, etc
* \param potVal3 décrit le mode.  0=major, etc.
* \return
*
*/

{
   // for (int i = 0 ; i < nbLayer ; i ++ )
    {
        // if ( isLayerActive(i ) )
        int i = layer ; 
  
         {
          unsigned int taille = mesureTaillePileComplete(pInitial[i]);
        
            for ( unsigned int j  = 0 ; j  < taille ; j  ++ )
            {
                pileNote tempNote;
                tempNote = ZEROgetNoteFromPile(pInitial[i], j);
                
                if ( tempNote.note != 0 && ( tempNote.type == 0x90 || tempNote.type == 0x80 ) && pitchableChannel[tempNote.channel] == true && pitchableLayer[tempNote.layer] == true )
                {
                    // d'abord il faut réduire la melody(note) à une note

                    int rootnote =   tempNote.note  % 12 ; // k devient la note entre 0 (C) et 11 (B)

                    // il faut ensuite v�rifier si cette note est comprise dans la gamme indiqu�e par PotVal
                    // pour une note de base ( C par d�faut mais un programme pourra changer )
                    //
                    //                              while  ( isNoteInScaleMode(rootnote,gammeActuelle,potVal3) == false  )
                    //                              {
                    //
                    //                                    melodyMatrix[i][j][MMnote] = melodyMatrix[i][j][MMnote] + (rand () % 2) - (rand () % 2) ;
                    //                                    // va augmenter ou réduire le pitch jusqu'à ce que colle à la gamme.
                    //                                    rootnote =   melodyMatrix[i][j][MMnote] % 12 ; // rootnote devient la note entre 0 (C) et 11 (B)
                    //
                    //                              }

                    // nouvelle m�thode car la pr�c�dente n'est pas time efficient
                    signed int ajoutPitch = 0 ;

                    ajoutPitch = (( rand () % 2 ) == 0 )?+1:-1 ;
                    // dans le premier cas on fera du plus 1, dans le second, du moins 1

                    while  ( isNoteInScaleMode(rootnote,gammeActuelle,potVal3) == false  )
                    {
                        tempNote.note = modifyPitchUniversal(tempNote,tempNote.note,ajoutPitch )  ;
                        // va augmenter ou réduire le pitch jusqu'à ce que colle à la gamme.
                        rootnote =   tempNote.note % 12 ; // rootnote devient la note entre 0 (C) et 11 (B)
                    }

                    ZEROSetNoteInPile(pInitial[i], j, tempNote);
                }
            }
        }
    }
}

float giveInclusion ( int gamme   , long mode   )
{
    int compteurIN = 0 ;
    int compteurTOT = 0 ;
    float resultat = 0.0 ;

    for (int i = 0 ; i < nbLayer ; i ++ )
    {
        if ( isLayerActive(i) )
        {

            unsigned int taille = mesureTaillePileComplete(pInitial[i]);
           
            for ( unsigned int j = 0 ; j < taille  ; j ++ )
            {

                pileNote tempNote;
                tempNote = ZEROgetNoteFromPile(pInitial[i], j);
               
                if ( tempNote.note != 0 )
                {
                    // d'abord il faut réduire la melody(note) � une note

                    int rootnote = tempNote.note % 12; // k devient la note entre 0 (C) et 11 (B)

                    // il faut ensuite v�rifier si cette note est comprise dans la gamme indiqu�e par PotVal
                    // pour une note de base ( C par d�faut mais un programme pourra changer )

                    if  ( isNoteInScaleMode(rootnote, gamme , mode ) )
                    {
                        compteurIN++ ;
                    }
                    compteurTOT++ ;
                }
            }
        }
    }

    if ( compteurTOT != 0 )
    {
        // cout << compteurIN << " notes sur " << compteurTOT << " sont dans la gamme\n" ;
        resultat =   ( (float) compteurIN * 100.0 ) / (float) compteurTOT   ;
    }
    else
    {
        //  cout << compteurIN << " notes sont dans la gamme\n" ;
        resultat =   0;
    }

    return resultat ;
}


float giveInclusionArray ( int tableau[], int gamme   ,  long mode   )
{
    // va tester un tableau qui comprend un check pour chaque note dodécaphonique
    // donne une valeur qui indique quel pourcentage de ce tableau est contenu dans la gamme visée, mode visé (potval3 )
    int compteurIN = 0 ;
    int compteurTOT = 0 ;
    float resultat = 0 ;

    for (int i = 0 ; i < nbLayer ; i ++ )
    {
        if ( isLayerActive(i ) )
        {
            for ( int j = 0 ; j < 12 ; j ++ )
            // 12 pour dodécaphonique
            {
                if ( tableau[j] != 0 )
                {
                    // d'abord il faut réduire la melody(note) � une note

                    int rootnote =  j % 12 ; // k devient la note entre 0 (C) et 11 (B)

                    // il faut ensuite v�rifier si cette note est comprise dans la gamme indiqu�e par PotVal
                    // pour une note de base ( C par d�faut mais un programme pourra changer )

                    if  ( isNoteInScaleMode(rootnote,gamme , mode ) )
                    {
                        compteurIN++ ;
                    }

                    compteurTOT++ ;
                }
            }
        }
    }

    if ( compteurTOT != 0 )
    {
        // cout << compteurIN << " notes sur " << compteurTOT << " sont dans la gamme\n" ;
        resultat =   ( (float) compteurIN / (float) compteurTOT ) * 100.0 ;
    }
    else
    {
        //  cout << compteurIN << " notes sont dans la gamme\n" ;
        resultat =   0;
    }

    return resultat ;
}

float giveCompletude (  int gamme  , long mode )
{
    int compteurIN = 0;
    int compteurTOT = 0 ;
    float resultat = 0 ;
    int intermediaire[12] = {0};

    for (int i = 0 ; i < nbLayer ; i ++ )
    {
        if ( isLayerActive(i ) ) {
 unsigned int taille = mesureTaillePileComplete(pInitial[i]) ;
						pileNote tempNote;
				
            for ( unsigned int j = 0 ; j < taille ; j ++ )
            {
               		tempNote = ZEROgetNoteFromPile(pInitial[i], j);
						
                if ( tempNote.note != 0 )
                {
                    // d'abord il faut réduire la melody(note) � une note

                    int rootnote =   tempNote.note % 12 ; // k devient la note entre 0 (C) et 11 (B)

                    // il faut ensuite v�rifier si cette note est comprise dans la gamme indiqu�e par PotVal
                    // pour une note de base ( C par d�faut mais un programme pourra changer )

                    if  ( isNoteInScaleMode(rootnote,gamme , mode ) )
                    {
                        intermediaire[rootnote] = 1 ;
                    }
                }
            }
        }
    }

    for ( int z = 0 ; z < 12 ; z++ )
    {
        if  ( isNoteInScaleMode(z,gamme , mode ) )
        {
            if ( intermediaire[z] != 0 )
            {
                compteurIN++ ;
                // cout << " O " ;
            }
            else
            {
                // cout << " - " ;
            }

            compteurTOT++ ;
        }
        else
        {
            //  cout << " - " ;
        }
    }

    // cout << endl ;

    if ( compteurTOT != 0 )
    {
        // cout << compteurIN << " des notes de la gamme ( sur " << compteurTOT << ") sont comprises \n" ;
        resultat =   ( (float) compteurIN / (float) compteurTOT ) * 100.0 ;
    }
    else
    {
        // cout << compteurIN << " notes de la gamme\n" ;
        resultat =   0;
    }

    return resultat ;
}


float giveCompletudeOfTriad (  int numberFromTriad )
{
    int compteurIN = 0 ;
    int compteurTOT = 0 ;
    float resultat = 0 ;
    int intermediaire[12] = {0};

    for (int i = 0 ; i < nbLayer ; i ++ )
    {
        if ( isLayerActive(i ) )
        {
             unsigned int taille = mesureTaillePileComplete(pInitial[i]) ;
						pileNote tempNote;
				
            for ( unsigned int j = 0 ; j < taille ; j ++ )
            {
                tempNote = ZEROgetNoteFromPile(pInitial[i], j);
					
                if ( tempNote.note != 0 )
                {
                    // d'abord il faut réduire la melody(note) � une note

                    int rootnote =   tempNote.note  % 12 ; // k devient la note entre 0 (C) et 11 (B)

                    // il faut ensuite v�rifier si cette note est comprise dans la gamme indiqu�e par PotVal
                    // pour une note de base ( C par d�faut mais un programme pourra changer )

                    if  ( isNoteInTriadNumber(rootnote,numberFromTriad) )
                    {
                        intermediaire[rootnote] = 1 ;
                    }
                }
            }
        }
    }

    for ( int z = 0 ; z < 12 ; z++ )
    {
        if  ( isNoteInTriadNumber(z,numberFromTriad) )
        {
            if ( intermediaire[z] != 0 )
            {
                compteurIN++ ;
                // cout << " O " ;
            }
            else
            {
                // cout << " - " ;
            }

            compteurTOT++ ;
        }
        else
        {
            //  cout << " - " ;
        }
    }

    // cout << endl ;

    if ( compteurTOT != 0 )
    {
        // cout << compteurIN << " des notes de la gamme ( sur " << compteurTOT << ") sont comprises \n" ;
        resultat =   ( (float) compteurIN / (float) compteurTOT ) * 100.0 ;
    }
    else
    {
        // cout << compteurIN << " notes de la gamme\n" ;
        resultat =   0;
    }

    return resultat ;
}

String giveMelodyStats ( void )
{
    int compteurIN = 0 ;
    // int compteurTOT = 0 ;
    // float resultat = 0 ;
    int intermediaire[12] = {0};

    for (int i = 0 ; i < nbLayer ; i ++ )
    {
        if ( isLayerActive(i ) ) {
unsigned int taille = mesureTaillePileComplete(pInitial[i]) ;
						pileNote tempNote;
						
            for ( unsigned int j = 0 ; j < taille ; j ++ )
            {
                tempNote = ZEROgetNoteFromPile(pInitial[i], j);
						
                if ( tempNote.note != 0 )
                {
                    // d'abord il faut réduire la melody(note) � une note

                    int rootnote =   tempNote.note % 12 ; // k devient la note entre 0 (C) et 11 (B)

                    // il faut ensuite v�rifier si cette note est comprise dans la gamme indiqu�e par PotVal
                    // pour une note de base ( C par d�faut mais un programme pourra changer )

                    intermediaire[rootnote] = 1 ;
                }
            }
        }
    }

    // cout << endl << "presence et absence() de notes dans cette melodie " << endl << endl  ;

    String affiche = nulString ;

    for ( int z = 0 ; z < 12 ; z++ )
    {

        if ( intermediaire[z] != 0 )
        {
            compteurIN++ ;
            // cout << " Present " ;
            // cout << " " << GiveNameOfNote(z);
            affiche += GiveNameOfNote(z);
        }
        else
        {
            // cout << " Absent " ;
            affiche += "(" ;
            affiche += GiveNameOfNote(z) ;
            affiche += ")";
        }
    }

    return affiche ;
}


bool isNoteInScaleMode(int z, int gamme , long  mode )
/** \brief indique si la note z appartient à la gammeActuelle, sur le mode potVal3
// c'est la fonction la plus utilisée de ce programme !
*
* \param
* \param
* \return
*
*/

{
    int rootnote =   gamme  % 12   ; // k devient la note entre 0 (C) et 11 (B)

    // il faut ensuite vérifier si cette note est comprise dans la gamme indiquée par PotVal
    // pour une note de base ( C par défaut mais un programme pourra changer )

    // int diff = ( z - rootnote ) ;
    // while ( diff < 0 )
    // {
    //     diff += 12 ;
    // }
    // diff = diff % 12 ;

int rootZ = z % 12 ;
int diff = rootZ - rootnote ;
if ( diff < 0 ) { diff += 12 ; }

    if  ( ( ( Scale[mode] >> ( diff ) ) &  1 ) == 1  )
    {
        return true ;
    }

    return false ;

}

bool isNoteInTriadNumber(int z,int triadNumber )
/** \brief indique si la note z appartient � la s�quence (triade probablement) décrite par le chiffre TriadNumber, qui la code en binaire
*
* \param
* \param
* \return
*
*/

{
    int rootnote =   0   ; // k devient la note entre 0 (C) et 11 (B)

    // il faut ensuite v�rifier si cette note est comprise dans la gamme indiqu�e par PotVal
    // pour une note de base ( C par d�faut mais un programme pourra changer )

    int diff = ( z - rootnote ) ;
    while ( diff < 0 )
    {
        diff += 12 ;
    }
    diff = diff % 12 ;

    if  ( ( ( triadNumber >> ( diff ) ) &  1 ) == 1  )
    {
        return true ;
    }

    return false ;

}

int giveDecalageOfNthNoteOfScaleMode(int N,int gammeActuelle,long  potVal3)
// donne le décalage en demi-tons de la nième note de la gammme
// par rapport à la note de départ de cette gamme
// ex : 0 pour C -> 0
// 5 pour C ->
{
    int compteurNth = 0 ;
    int compteurDecalage = 0 ;

    do
    {
        if  ( isNoteInScaleMode(gammeActuelle+compteurDecalage,gammeActuelle,potVal3))
        {
            compteurNth++ ;
        }

        compteurDecalage++ ;
    }
    while ( compteurNth < N ) ;


    compteurDecalage -= 1;
    return compteurDecalage ;
}

String giveStringOfScale (  int gammeActuelle, int potVal3  )
{
    String affiche = nulString ;

    for ( int z = 0 ; z < 12 ; z++ )
    {
        if ( isNoteInScaleMode(z,gammeActuelle,potVal3) )

        {
            affiche += " O " ;
        }
        else
        {
            affiche += " - " ;
        }
    }
    return affiche ;

}

/*
String GiveNameOfNote ( int NNN )
{
switch ( NNN % 12 )
{
case 0 :
return F("C";
break;
case 1:
return  F("C#";
break;
case 2:
return  "D";
break;
case 3:
return  "D#";
break;
case 4:
return  "E";
break;
case 5:
return  "F";
break;
case 6:
return  "F#";
break;
case 7:
return  "G";
break;
case 8:
return  "G#";
break;
case 9:
return  "A";
break;
case 10:
return  "A#";
break;
case 11:
return  "B";
break;
default :
return  " ";
break;
}
}
*/

String GiveNameOfMode ( int NNN )
{
    switch ( NNN   )
    {
        case 0 :
        return F("Major");
        break;
        case 1:
        return  F("MinorNatural");
        break;
        case 2:
        return  F("MinorHarmonic");
        break;
        case 3:
        return  F("MinorMelodic");
        break;
        case 4:
        return  F("PentatonicMajor");
        break;
        case 5:
        return  F("PentatonicMinor");
        break;
        case 6 :
        return F("Mixolydian");
        break;
        case 7:
        return  F("Lydian");
        break;
        case 8:
        return  F("Lydian Dominant");
        break;
        case 9:
        return  F("Phrygian Dominant ");
        break;
        case 10:
        return  F("Dorian");
        break;
        case 11:
        return  F("Phrygian");
        break;
        case 12:
        return  F("Locrian");
        break;
        case 13 :
        return F("Hexa") ;
        break ;
case 14 :
return F("Messiaen2") ;
break ;
case 15 : 
return F("Dodeca");
break ;

        default :
        return  F("?");
        break;
    }
}

String GiveNameOfModeShort(int NNN)  // 2C12 pour affichage multi layer 
{
    switch (NNN)
    {
    case 0:
        return F("Maj");
        break;
    case 1:
        return F("MinN");
        break;
    case 2:
        return F("MinH");
        break;
    case 3:
        return F("MinM");
        break;
    case 4:
        return F("5Maj");
        break;
    case 5:
        return F("5Min");
        break;
    case 6:
        return F("Mix");
        break;
    case 7:
        return F("Lyd");
        break;
    case 8:
        return F("LyD");
        break;
    case 9:
        return F("PhrD");
        break;
    case 10:
        return F("Dor");
        break;
    case 11:
        return F("Phr");
        break;
    case 12:
        return F("Loc");
        break;
    case 13:
        return F("Hex");
        break;
    case 14:
        return F("Mes2");
        break;
        case 15 : 
        return F("Dod");
        break ; 
        
    default:
        return F("?");
        break;
    }
}
/////////////////////////////////////////////////////////////////////////////////////
// Version Pile NOTE
// 1C95
/////////////////////////////////////////////////////////////////////////////////////

unsigned int getNbNotesInMelody ( int i   )
{
    // tout d'abord on place les notes vides à la fin
    // simplePartialSortZeroNotesv2(i) ;

    // int compteur = 0 ;
ZEROtreatPileComplete(pInitial[i]); 

    unsigned int taille = mesureTaillePileComplete(pInitial[i]);
    
    // for ( int j = 0 ; j < taille ; j ++ )
    // {
    //     pileNote tempNote;
    //     tempNote = ZEROgetNoteFromPile(pInitial[i], j);

    //     if ( tempNote.note != 0 )
    //     {
    //         compteur ++ ;
    //     }
    // }
    // on pourrait faire mieux : partir de la dernière note et descendre de 1 jusqu'à rencontrer une note

    return taille ;
}

String printMelody ( int j   )
{
    int iMax = 0    ;
    iMax = getNbNotesInMelody(j) ;
    String affiche = nulString ;

    for ( int i = 0 ; i < iMax ; i++ )
    {
        // unsigned int taille = mesureTaillePileComplete(pInitial[j]) ;
						pileNote tempNote;
						tempNote = ZEROgetNoteFromPile(pInitial[j], i);
						

        // cout << melodyMatrix[0][i][MMnote] << " " << GiveNameOfNote(melodyMatrix[0][i][MMnote]) << "\n" ;
        if ( tempNote.note != 0 )
        {
            affiche +=  GiveNameOfNote(tempNote.note) ;
            affiche += "[" ;
            affiche +=  String( tempNote.note ) ;
            affiche +=  "] " ;
        }
    }
    return affiche ;
}

String romanNumber(int num)
{
    String result ;

    switch (num)
    {
        case 1 :
        result = F("I") ;
        break ;
        case 2 :
        result = F("II") ;
        break ;
        case 3 :
        result = F("III") ;
        break ;
        case 4 :
        result = F("IV") ;
        break ;
        case 5 :
        result = F("V") ;
        break ;
        case 6 :
        result = F("VI") ;
        break ;
        case 7 :
        result = F("VII") ;
        break ;
        case 8 :
        result = F("VIII") ;
        break ;
        case 9 :
        result = F("IX") ;
        break ;
        case 10 :
        result = F("XI") ;
        break ;
        default :
        result = String(num);
        // itoa(num,result,10) ;
        break ;
    }

    return result ;
}


int getChiffrePourVerifierTriade ( int degreeJ, int gamme, int mode )
{

    int chiffrePourVerifierTriade = 0 ;

    for ( int k = 0 ; k < 3 ; k ++ )   // on teste ici 1st, 3rd, 5th
    {
        int dec = giveDecalageOfNthNoteOfScaleMode(degreeJ+k*2,gamme,mode) ; // teste donc 1ST, 3RD, 5TH
        // cout << GiveNameOfNote(choixGamme+dec) ;

        chiffrePourVerifierTriade = chiffrePourVerifierTriade | ( 1 << ((gamme+dec )%12) ) ;
    }
    return chiffrePourVerifierTriade ;
}

int getPlaceNoteAdansTriadeDegreBgammeCmodeD ( int noteA, int degreeJ, int gamme, int mode )
{
    for ( int k = 0 ; k < 10 ; k ++ )
    // TODO DEBUG pas clair ce que ce 10 signifie. : je crois qu'on cherche jusqu'au 1 major3 pfect5, 7th 9th < 10
    {
        int dec = giveDecalageOfNthNoteOfScaleMode(degreeJ+k*2,gamme,mode) ;
        int noteResultante = gamme+dec ;

        if ( noteResultante%12 == noteA%12 )
        {
            return ( k+1 ) ;
        }
    }

    return 0 ;

}

int getNotePlaceAdansTriadeDegreBgammeCmodeDoctaveE ( int placeA, int degreeJ, int gamme, int mode, int octave )
{
    int dec = giveDecalageOfNthNoteOfScaleMode(degreeJ+(placeA-1)*2,gamme,mode) ;
    int noteResultante = ( gamme + dec + ( 12 * octave  )) ;

    return noteResultante ;
}

int getOctave( int note , int gamme )
{
    int octave = ( ( note - ( gamme%12) ) / 12 ) ;
    return octave ;
}

int getPlaceNoteAdansGammeCmodeD ( int noteA, int gamme, int mode )
{
    for ( int k = 0 ; k < 12 ; k ++ )
    // TODO DEBUG pas clair ce que ce 10 signifie. : je crois qu'on cherche jusqu'au 1 major3 pfect5, 7th 9th < 10
    {
        int dec = giveDecalageOfNthNoteOfScaleMode(k,gamme,mode) ;
        int noteResultante = gamme+dec ;

        if ( noteResultante%12 == noteA%12 )
        {
            return ( k+1 ) ;
        }
    }

    return 0 ;

}

int getNotePlaceAdansGammeCmodeDoctaveE ( int placeA, int gamme, int mode, int octave )
{
    int dec = giveDecalageOfNthNoteOfScaleMode(placeA,gamme,mode) ;
    int noteResultante = ( (gamme%12) + dec + ( 12 * octave  )) ;

    return noteResultante ;
}

/////////////////////////////////////////////////////////////////////////////////////
// Version Pile NOTE
// 1C95  
/////////////////////////////////////////////////////////////////////////////////////


int transposeAllFromDegreeAtoDegreeB ( byte layer, int degreeA, int degreeB, int gamme, int mode )
// cette fonction va transposer toutes les notes appartenant au degr� normal A vers le degr� normal B  ( 1st, 3rd, 5th )

{
    // on calcule le nombre correspondant au degr� A pour gamme et mode
    // on calcule le nombre pour degr� B

    int chiffreA = getChiffrePourVerifierTriade(degreeA,gamme,mode) ;
    //      int chiffreB = getChiffrePourVerifierTriade(degreeB,gamme,mode) ;
    int nbTransposee = 0 ;

    // passer en revue les notes
    // cette note appartient-elle au degr� A ?
    // si oui, quelle est sa position au sein de la triade ? ( 1st, 3rd, 5th )
    // quelle est la position de la ( 1st, 3rd, 5th ) au sein de la triade B ?

  //  for (int i = 0 ; i < nbLayer ; i ++ )
    {
        // if ( isLayerActive(i ) ) 
        int i = layer ; 
        {

            int jMax = 0    ;
            jMax = getNbNotesInMelody(i) ;

            for ( int j  = 0 ; j  < jMax ; j  ++ )
            {

                  pileNote tempNote;
                tempNote = ZEROgetNoteFromPile(pInitial[i], j);
                 
                if ( tempNote.note != 0 && pitchableChannel[tempNote.channel] == true && pitchableLayer[tempNote.layer] == true )
                {
                    // d'abord il faut réduire la melody(note) � une note

                    int rootnote =   tempNote.note  % 12 ; // k devient la note entre 0 (C) et 11 (B)
                    int octaveBase = getOctave(tempNote.note , gamme ) ;

                    // si la note est dans la Triade, on doit la transposer
                    // pour �a je dois savoir sa place dans la triade
                    // et savoir la note correspondante � cette place dans la triade degre B

                    if ( isNoteInTriadNumber(rootnote,chiffreA))
                    {
                        //cout << "T" ;
                        int place  = 0 ;
                        place = getPlaceNoteAdansTriadeDegreBgammeCmodeD(rootnote,degreeA,gamme,mode) ;
                        int newNote = 0 ;
                        //cout << place ;
                        newNote = getNotePlaceAdansTriadeDegreBgammeCmodeDoctaveE(place,degreeB,gamme,mode,octaveBase) ;

                        tempNote.note = modifyPitchUniversal(tempNote,newNote)  ;
                        nbTransposee++ ;
                        ZEROSetNoteInPile(pInitial[i],j,tempNote); 
                    }
                    else
                    {

                        //cout << "-" ;
                    }
                    //cout << "\t" ;

                }
            }
        }
    }

    return nbTransposee ;

}

/////////////////////////////////////////////////////////////////////////////////////
// Version Pile NOTE
// 1C95
/////////////////////////////////////////////////////////////////////////////////////

int getTforNote(int i, int j, int tMax   )
// va indiquer la plus longue s�quence de notes sans r�p�tition � partir de cette note
{
    int t ;

    for ( t = 1 ; t < tMax ; t++ )
    {
        //cout << "T" << t ;
        for ( int k = 0 ; k < t ; k++ )
        {
            //cout << "K" << k ;

            pileNote tempNote;
            tempNote = ZEROgetNoteFromPile(pInitial[i], (j + k ) % 10);

            pileNote tempNote2;
            tempNote2 = ZEROgetNoteFromPile(pInitial[i], (j + t ) % 10);

            //cout << "L" << l ;
            // cout << endl << (j+k)%10 << " vs " << (j+t) % 10 ;
            if ( tempNote.note % 12 == tempNote2.note % 12 )
            {
                // cout << " IDEM " ;
                // cout << " MAX EST " << t-1 << endl << " " ;
                return ( t ) ;
            }
            else
            {
                // cout <<" DIFF " ;
            }
        }
    }

    return tMax ;
}

int  printContiguity ( int degreeA,   int gamme, int mode, int tMax )
// cette fonction va indiquer le degr� de contiguit� au sein de la m�lodie, indiquant donc des notes appartenant � un degr� d'une gamme, mode donné
// exemple : si A# D F pour le degr� 3 en G Phrygian
// T Max indique si on cherche jusque 2, 3, ou 4, etc ( 4 et 5 plus tard )
{

    int SumContiguity = 0 ;

    //      int chiffreA = getChiffrePourVerifierTriade(degreeA,gamme,mode) ;

    for (int i = 0 ; i < nbLayer ; i ++ )
    {
        if ( isLayerActive(i ) ) {

            int jMax = 0    ;
            jMax = getNbNotesInMelody(i) ;

pileNote tempNote;
					
            for ( int j  = 0 ; j  < jMax ; j  ++ )
            {
                	tempNote = ZEROgetNoteFromPile(pInitial[i], j);
						
                if ( tempNote.note != 0 )
                {
                    // int rootnote =   melodyMatrix[i][j][MMnote] % 12 ; // k devient la note entre 0 (C) et 11 (B)

                    if (true )
                    // ( isNoteInTriadNumber(rootnote,chiffreA))
                    {
                        int longest = getTforNote( i, j, 32 ) ;
                        //cout << longest ;
                        SumContiguity += longest ;
                    }
                    else
                    {
                        //cout << "-" ;
                    }
                    //cout << "\t" ;
                }
            }
        }
    }

    return SumContiguity ;
}


int getTforNoteInTriad(int i, int j, int tMax  , int chiffreTriad )
// va indiquer la plus longue s�quence de notes sans r�p�tition � partir de cette note
{
    int t ;

    for ( t = 1 ; t < tMax ; t++ )
    {
        //cout << "T" << t ;
        for ( int k = 0 ; k < t ; k++ )
        {
            //cout << "K" << k ;


            //cout << "L" << l ;
            // cout << endl << (j+k)%10 << " vs " << (j+t) % 10 ;

            // int rootnote =   melodyMatrix[i][(j+t)%10][MMnote] % 12 ;
int rootnote = 0 ;
pileNote tempNote;
tempNote = ZEROgetNoteFromPile(pInitial[i], (j + t) % 10);
rootnote = tempNote.note % 12 ; 

int rootnote2 = 0 ; 
pileNote tempNote2 ;
tempNote2 = ZEROgetNoteFromPile(pInitial[i], (j + k) % 10);
rootnote = tempNote.note % 12;

if ( rootnote2 == rootnote || isNoteInTriadNumber(rootnote, chiffreTriad) == false)

{
    // cout << " IDEM " ;
    // cout << " MAX EST " << t-1 << endl << " " ;
    return (t);
            }
            else
            {
                // cout <<" DIFF " ;
            }
        }
    }

    return tMax ;
}

int printContiguityAndInTriad ( int degreeA,   int gamme, int mode, int tMax )
// cette fonction va indiquer le degr� de contiguit� au sein de la m�lodie, indiquant donc des notes appartenant � un degr� d'une gamme, mode donné
// exemple : si A# D F pour le degr� 3 en G Phrygian
// T Max indique si on cherche jusque 2, 3, ou 4, etc ( 4 et 5 plus tard )
{

    int SumContiguity = 0 ;

    int chiffreA = getChiffrePourVerifierTriade(degreeA,gamme,mode) ;

    for (int i = 0 ; i < nbLayer ; i ++ )
    {
        if ( isLayerActive(i ) ) {

            int jMax = 0    ;
            jMax = getNbNotesInMelody(i) ;

pileNote tempNote;
						
            for ( int j  = 0 ; j  < jMax ; j  ++ )
            {
                tempNote = ZEROgetNoteFromPile(pInitial[i], j);
						
                if ( tempNote.note != 0 )
                {
                    int rootnote =   tempNote.note % 12 ; // k devient la note entre 0 (C) et 11 (B)

                    if  ( isNoteInTriadNumber(rootnote,chiffreA))
                    {
                        int longest = getTforNoteInTriad ( i, j, 32, chiffreA ) ;
                        //cout << longest ;
                        SumContiguity += longest ;
                    }
                    else
                    {
                        //cout << "-" ;
                    }
                    //cout << "\t" ;
                }
            }
        }
    }
    return SumContiguity ;
}

/////////////////////////////////////////////////////////////////////////////////////
// Version Pile NOTE
// 1C94 : n'a plus de raison d'être.   vidé
/////////////////////////////////////////////////////////////////////////////////////

int transposeOneRandomTriad ( byte layer , int degreeA, int degreeB,  int gamme, int mode, int triadSize  )
// cette fonction va indiquer le degr� de contiguit� au sein de la m�lodie, indiquant donc des notes appartenant � un degr� d'une gamme, mode donné
// exemple : si A# D F pour le degr� 3 en G Phrygian
// T Max indique si on cherche jusque 2, 3, ou 4, etc ( 4 et 5 plus tard )
{

    //      int tMax = 3 ;

    int chiffreA = getChiffrePourVerifierTriade(degreeA,gamme,mode) ;
    //   int chiffreB = getChiffrePourVerifierTriade(degreeB,gamme,mode) ;
    int nbTransposee = 0 ;

    //    int SumContiguity = 0 ;

    //      int chiffreA = getChiffrePourVerifierTriade(degreeA,gamme,mode) ;

    // for (int i = 0 ; i < nbLayer ; i ++ )
    {
      int i = layer ; 
       // if ( isLayerActive(i ) ) 
        {


            int contiguityTable [32] ;
            int highestContiguity = 0 ;
            int nbTriads = 0 ;

            int jMax = 0    ;
            jMax = getNbNotesInMelody(i) ;

            for ( int j  = 0 ; j  < jMax ; j  ++ )
            {

                 pileNote tempNote;
                tempNote = ZEROgetNoteFromPile(pInitial[i], j);
                ZEROSetNoteInPile(pInitial[i], j, tempNote);

                if ( tempNote.note != 0 )
                {
                    int rootnote = tempNote.note % 12; // k devient la note entre 0 (C) et 11 (B)

                    if  ( isNoteInTriadNumber(rootnote,chiffreA))
                    {
                        int longest = getTforNoteInTriad ( i, j, 32, chiffreA ) ;
                        if ( longest >= triadSize )
                        {
                            nbTriads++ ;
                        }
                        contiguityTable[j] = longest ;
                        if ( longest > highestContiguity )
                        {
                            highestContiguity = longest ;
                        }
                    }
                    else
                    {
                        contiguityTable[j] = 0 ;
                    }
                }
            }

            // on a maintenant un tableau qui reprend les contiguit�s pour chaque note de la m�lodie
            // mais on sait aussi quelle est la plus grande
            // et aussi le nombre de triades.

            //cout << endl << "highest = " << highestContiguity << endl ;
            //cout << "nb de triades = " << nbTriads << endl ;

            //cout << endl << "detail des contiguites " << endl ;

            for ( int j  = 0 ; j  < 10 ; j  ++ )
            {
                //cout << contiguityTable[j] << "\t" ;
            }

            //cout << endl ;

            if ( nbTriads == 0 )
            {
                return 0 ;
            }

            // on choisit une triade

            int chosenTriad = 0 ;
            // chosenTriad = ( rand() % nbTriads ) + 1 ;
    chosenTriad = random(1,nbTriads+1 ) ;

            // on rep�re quelle est la note qui est chosenTriad.

            nbTriads = 0 ;

            int look = 0 ;
            do
            {
                if ( contiguityTable[look] >= triadSize )
                {
                    nbTriads++ ;
                }
                look++ ;
            }
            while ( nbTriads < chosenTriad ) ;
            look-- ;

            //cout << endl << "found chosen triad " << chosenTriad << " at " << look << endl ;

pileNote tempNote;
						tempNote = ZEROgetNoteFromPile(pInitial[i], look%10);
            int octaveBase = getOctave(tempNote.note , gamme ) ;

            for ( int j  = look ; j  < look+triadSize ; j  ++ )
            {

tempNote = ZEROgetNoteFromPile(pInitial[i], j%10);
// DEBUG pas tout à fait clair pq on peut aller si loin dans la mélodie.   quid si est plus petite ?

                int rootnote =   tempNote.note % 12 ; // k devient la note entre 0 (C) et 11 (B)
                // TODO DEBUG ce modulo 10 repr�sente la longueur de la m�lodie !

                // si la note est dans la Triade, on doit la transposer
                // pour �a je dois savoir sa place dans la triade
                // et savoir la note correspondante � cette place dans la triade degre B

                if (isNoteInTriadNumber(rootnote, chiffreA) && pitchableChannel[tempNote.channel] == true && pitchableLayer[tempNote.layer] == true )
                {
                    //cout << "T" ;
                    int place  = 0 ;
                    place = getPlaceNoteAdansTriadeDegreBgammeCmodeD(rootnote,degreeA,gamme,mode) ;
                    int newNote = 0 ;
                    //cout << place ;
                    newNote = getNotePlaceAdansTriadeDegreBgammeCmodeDoctaveE(place,degreeB,gamme,mode,octaveBase) ;

                    tempNote.note = modifyPitchUniversal(tempNote,newNote) ; 
                    ZEROSetNoteInPile(pInitial[i],j%10,tempNote) ; 

                    // TODO DEBUG ce modulo 10 repr�sente la longueur de la m�lodie !

                    nbTransposee++ ;
                }
                else
                {
                    //cout << "-" ;
                }
                //cout << "\t" ;
            }
        }
    }

    return nbTransposee ;
}

/////////////////////////////////////////////////////////////////////////////////////
// Version Pile NOTE
// 1C95
/////////////////////////////////////////////////////////////////////////////////////

int transposeOneRandomSpacedTriad ( byte layer, int degreeA, int degreeB,  int gamme, int mode, int triadSize    )
// pour cette transposition les notes ne doivent pas �tre contigues
// T Max indique si on cherche jusque 2, 3, ou 4, etc ( 4 et 5 plus tard )
{

    //int tMax = 3 ;

    int chiffreA = getChiffrePourVerifierTriade(degreeA,gamme,mode) ;
    //  int chiffreB = getChiffrePourVerifierTriade(degreeB,gamme,mode) ;
    int nbTransposee = 0 ;

    //int SumContiguity = 0 ;

    //      int chiffreA = getChiffrePourVerifierTriade(degreeA,gamme,mode) ;

    // for (int i = 0 ; i < nbLayer ; i ++ )
    {
        int i = layer ; 
       // if ( isLayerActive(i ) ) 
        {

            int placeDansTriadeTable [32] = {0};
            // on va ici utiliser la table de contiguit� pour une autre fonction
            // dans cette table on indiquera quelle position la note [] a dans la triade
            // ex : [1] [0]aucune [3] [5]
            // ensuite, on compte combien de 1, combien de 3, combien de 5
            // ensuite, on choisit 1x 1, 1x 3, 1x 5
            // et on les transpose

            //int highestContiguity = 0 ;
            //int nbTriads = 0 ;

            int jMax = 0    ;
            jMax = getNbNotesInMelody(i) ;

            for ( int j  = 0 ; j  < jMax ; j  ++ )
            {

                pileNote tempNote;
                tempNote = ZEROgetNoteFromPile(pInitial[i], j);
               
                if ( tempNote.note != 0 )
                {
                    int rootnote =   tempNote.note % 12 ; // k devient la note entre 0 (C) et 11 (B)

                    if  ( isNoteInTriadNumber(rootnote,chiffreA))
                    {
                        int place  = 0 ;
                        place = getPlaceNoteAdansTriadeDegreBgammeCmodeD(rootnote,degreeA,gamme,mode) ;
                        placeDansTriadeTable[j] = place ;
                        //cout << place << "\t" ;
                    }
                    else
                    {
                        placeDansTriadeTable[j] = 0 ;
                        //cout << "-\t" ;
                    }
                }
            }

            // on compte combien de 1, 2, 3 ( = 1st, 3rd, 5th )

            int count [12] = {0} ;

            for ( int j  = 0 ; j  < 32 ; j  ++ )
            {
                count[placeDansTriadeTable[j]]++ ;
            }

            // affichage

            for ( int j  = 0 ; j  < 7 ; j  ++ )
            {
                //cout << "NbDe" << j << " : " << count[j] << "\t" ;
            }

            //cout << endl ;

            // je sais maintenant combien il y a de 1, 2, 3
            // si >= 1, je limite � 1
            // ensuite total count[1] count[2] et count[3] pour savoir si duo ou triplet
            // ensuite si oK, choisir un random
            // enfin, transposer

            int countRedux[12] = {0} ;

            //cout << endl << "affichage des redux " << endl ;
            for (int m = 0 ; m < 12 ; m++ )
            {
                countRedux[m] = count[m]>=1?1:0 ;
                //cout << countRedux[m] << "\t" ;
            }

            if ( countRedux[1]+countRedux[2]+countRedux[3] < triadSize )
            {
                //cout << endl << "pas de triade" << endl ;
                return 0 ;
            }

            // ceci signifierait qu'il n'y a pas au moins 2 ou 3 notes dans la m�lodie pour former une triade de 2 ou 3 notes

            int countChoice[12] = {0} ;

            //cout << endl << "affichage des choix " << endl ;
            for (int m = 0 ; m < 12 ; m++ )
            {
                if ( countRedux[m] > 0 )
                {
                    // countChoice[m] = ( rand() % count[m] ) + 1 ;

countChoice[m] = random(1, count[m]  + 1 ) ;


                    //cout << countChoice[m] << "\t" ;
                    // on choisit ici la quanti�me note va �tre s�lectionn�e pour former duo ou triade
                }
            }

            // on doit maintenant trouver la place de la ni�me note en question

            int transpose [12] = {0} ;

            for ( int k = 1 ; k <= 3 ; k++ )
            {
                // on va chercher la place de countChoice[1,2 et 3]
                int countFound = 0 ;
                int look = 0 ;
                do
                {
                    if ( placeDansTriadeTable[look] == k )
                    {
                        countFound++ ;
                    }
                    look++ ;
                }
                while ( countFound < countChoice[k]) ;
                look-- ;

                //cout << endl << countChoice[k] << "eme note " << k << " trouvee a la place " << look   ;
                transpose[k] = look ;
            }
            //cout << endl ;

            pileNote tempNote;
            tempNote = ZEROgetNoteFromPile(pInitial[i], transpose[1]  );

            int octaveBase = getOctave( tempNote.note , gamme ) ;

            for ( int j  = 1 ; j  < 12 ; j  ++ )
            {

                if ( count[j] > 0 )
                {
                    pileNote tempNote;
                    tempNote = ZEROgetNoteFromPile(pInitial[i], transpose[j] );

                    int rootnote =   tempNote.note % 12 ; // k devient la note entre 0 (C) et 11 (B)
                    // TODO DEBUG ce modulo 10 repr�sente la longueur de la m�lodie !
                    // int octaveBase = getOctave(melodyMatrix[i][transpose[j]][MMnote]) ;

                    if (isNoteInTriadNumber(rootnote, chiffreA) && pitchableChannel[tempNote.channel] == true && pitchableLayer[tempNote.layer] == true )
                    {
                        //cout << "T" ;
                        int place  = 0 ;
                        place = getPlaceNoteAdansTriadeDegreBgammeCmodeD(rootnote,degreeA,gamme,mode) ;
                        int newNote = 0 ;
                        //cout << place ;
                        newNote = getNotePlaceAdansTriadeDegreBgammeCmodeDoctaveE(place,degreeB,gamme,mode,octaveBase) ;

                        tempNote.note = modifyPitchUniversal(tempNote,newNote) ;
                        ZEROSetNoteInPile(pInitial[i], transpose[j], tempNote);

                        nbTransposee++ ;
                    }
                    else
                    {
                        //cout << "-" ;
                    }
                    //cout << "\t" ;
                }
            }
        }
    }

    return nbTransposee ;
}

/////////////////////////////////////////////////////////////////////////////////////
// Version Pile NOTE
// 1C95
/////////////////////////////////////////////////////////////////////////////////////

int unflattenTriad ( byte layer, int degreeA, int degreeB,  int gamme, int mode  )

// cette fonction va transposer toutes les notes appartenant au degr� normal A vers le degr� normal B  ( 1st, 3rd, 5th )

{
    // on calcule le nombre correspondant au degr� A pour gamme et mode
    // on calcule le nombre pour degr� B

    int chiffreA = getChiffrePourVerifierTriade(degreeA,gamme,mode) ;
    //      int chiffreB = getChiffrePourVerifierTriade(degreeB,gamme,mode) ;
    int nbTransposee = 0 ;

    // passer en revue les notes
    // cette note appartient-elle au degr� A ?
    // si oui, quelle est sa position au sein de la triade ? ( 1st, 3rd, 5th )
    // quelle est la position de la ( 1st, 3rd, 5th ) au sein de la triade B ?

    // for (int i = 0 ; i < nbLayer ; i ++ )
    {
int i = layer ; 
        // if ( isLayerActive(i ) ) 
        {


            bool ascendante = true ;

            ascendante=random(0,2) ==0?true:false ;

            if ( ascendante )
            {
                //cout << endl << "ASCENDANT" << endl  ;
            }
            else
            {

                //cout << endl << "DESCENDANT" << endl ;
            }

            int jMax = 0    ;
            jMax = getNbNotesInMelody(i) ;

            for ( int j  = 0 ; j  < jMax ; j  ++ )
            {

                pileNote tempNote;
                tempNote = ZEROgetNoteFromPile(pInitial[i], j);

                if ( tempNote.note != 0 )
                {
                    // d'abord il faut réduire la melody(note) � une note

                    int rootnote =   tempNote.note % 12 ; // k devient la note entre 0 (C) et 11 (B)
                    int octaveBase = getOctave( tempNote.note , gamme ) ;

                    // si la note est dans la Triade, on doit la transposer
                    // pour �a je dois savoir sa place dans la triade
                    // et savoir la note correspondante � cette place dans la triade degre B

                    if (isNoteInTriadNumber(rootnote, chiffreA) && pitchableChannel[tempNote.channel] == true && pitchableLayer[tempNote.layer] == true)
                    {
                        //cout << "T" ;
                        int place  = 0 ;
                        // place = getPlaceNoteAdansTriadeDegreBgammeCmodeD(rootnote,degreeA,gamme,mode) ;
                        if ( ascendante )
                        {
                            place = (nbTransposee % 3) + 1  ; // ceci pour la version ASCENDANTE
                        }
                        else
                        {

                            place = 3 - (nbTransposee % 3) ; // ceci pour la version DESCENDANTE
                        }
                        int newNote = 0 ;
                        //cout << place ;
                        newNote = getNotePlaceAdansTriadeDegreBgammeCmodeDoctaveE(place,degreeB,gamme,mode,octaveBase) ;

                        tempNote.note = modifyPitchUniversal(tempNote,newNote) ; 
                        ZEROSetNoteInPile(pInitial[i], j, tempNote);

                        nbTransposee++ ;
                    }
                    else
                    {

                        //cout << "-" ;
                    }
                    //cout << "\t" ;

                }
            }
        }
    }

    return nbTransposee ;

}

/////////////////////////////////////////////////////////////////////////////////////
// Version Pile NOTE
// 1C95
/////////////////////////////////////////////////////////////////////////////////////

void randomizeMelody (int k  )
{

    for ( int i = 0 ; i < nbLayer ; i ++ )
    {
        unsigned int taille = mesureTaillePileComplete(pInitial[i]);

        for ( unsigned int j = 0 ; j < taille ; j++ )
        {
            pileNote tempNote;
            tempNote = ZEROgetNoteFromPile(pInitial[i], j);
           
            tempNote.note  = 60 + random(0,12) ;
            ZEROSetNoteInPile(pInitial[i], j, tempNote);
    }
}
}



int nbOfNotesInModeNumber( int triadNumber )
{
    int compteur = 0 ;

    for ( int i = 0 ; i < 12 ; i++ )
    {
        if  ( ( ( triadNumber >> ( i ) ) &  1 ) == 1  )
        {
            compteur++ ;
        }
    }

    return compteur ;

}

int getSymmetricNoteOfAThroughAxeBQuartTon(int a, int b )
// ATTENTION on doit indiquer l'axe B en demi-valeurs de notes.   1 demi-ton = 2
{

    if ( a == 0 ) { return 0 ; }

    // on fait tourner l'axe jusqu'à ce qu'il coïncide avec 0 ou un %0
    // la différence entre les deux est n
    // on calcule B - n
    // on calcule 0 - cette valeur
    // on reporte cette valeur en faisant tourner en sens inverse

    int absoluteB ;
    absoluteB = b ;

    while (  absoluteB >= 24 )
    {
        absoluteB -= 24 ;
    }
    while (  absoluteB < 0 )
    {
        absoluteB += 24 ;
    }

    // cout << "axe absolu " << absoluteB << "/2 " << endl ;

    int y = a*2 ; // on va tout calculer en quarts de tons au final

    int yprime ;
    yprime = y - absoluteB ;

    int nprime ;
    nprime = ( y / 24 ) * 24 ; // arrondit à 0, 12, 24, 36, 48

    int diff ;
    diff = yprime - nprime ;

    int xprime ;
    xprime = nprime - diff ;

    int x ;
    x = xprime + absoluteB ;

    //cout << "n prime " << nprime << endl ;
    //cout << "y=" << y << " y'=" << yprime << " x'=" << xprime << " x=" << x << endl ;
    //cout << "REAL y=" << y/2 << " y'=" << yprime/2 << " x'=" << xprime/2 << " x=" << x/2 << endl ;

    return (x/2) ;
}

void findSymmetries( int gammeActuelle )
{
    int noteInit = 60 + gammeActuelle ;

    for ( int i = 0 ; i < nbDeModesConnus ; i++ )
    {
        // on va essayer tous les axes, qui valent n/2
        // à priori on essaie tout jusque 24, mais jusque 12 devrait suffire ( car le reste est symmétrique )
        //cout << endl << endl << GiveNameOfMode(i)   ;

        for ( int n = 0 ; n < 12 ; n++ )
        {
            bool estSymetrie = true ;


            for ( int k = 1 ; k <= nbOfNotesInModeNumber( Scale[i]) ; k++ )
            {

                int dec = 0 ;
                dec = giveDecalageOfNthNoteOfScaleMode(k,gammeActuelle,i) ; // à priori la gamme ne devrait pas intervenir, c'est le mode qui détermine tout

                dec = noteInit + dec ;

                int symmetricNote ;
                symmetricNote = getSymmetricNoteOfAThroughAxeBQuartTon(dec,n) ;

                // cout << "note " << dec << " -> " << symmetricNote << endl ;
                if ( isNoteInScaleMode(symmetricNote,gammeActuelle,i) )
                {
                    // cout << "OK" << endl ;
                }
                else
                {
                    // cout << "NOT OK" << endl ;
                    estSymetrie = false ;
                }

            }

            if ( estSymetrie )
            {
                //cout << endl << "axe " << n << "/2 "  ;

                //  cout << "EST AXE " << endl ;
            }

            // pour cet axe, on va tester toutes les notes du mode et de gamme.
            // la question est donc la suivante :
            // on prend la 1ère note de gamme
            // on calcule sa symmétrique selon l'axe n
            // cette nouvelle note appartient-elle à la gamme ?
            // si NON, stop
            // si OUI, passer à la note suivante
            // si OUI jusqu'au bout, la gamme accepte cette symétrie

            // on prend la 1ère note de gamme
            // on calcule sa symmétrique selon l'axe


            // cette nouvelle note appartient-elle à la gamme ?
            // si NON, stop
            // si OUI, passer à la note suivante
            // si OUI jusqu'au bout, la gamme accepte cette symétrie


        }

        // wait_on_enter() ;
    }
    // cout << endl ;

}


int getSymmetricAxe( int gammeActuelle, int ModeActuel )
{
    int noteInit = 60 + ( gammeActuelle % 12 ) ;
    int i = ModeActuel ;

    for ( int n = 0 ; n < 12 ; n++ )
    {
        bool estSymetrie = true ;

        for ( int k = 1 ; k <= nbOfNotesInModeNumber( Scale[i]) ; k++ )
        {

            int dec = 0 ;
            dec = giveDecalageOfNthNoteOfScaleMode(k,gammeActuelle,i) ; // à priori la gamme ne devrait pas intervenir, c'est le mode qui détermine tout

            dec = noteInit + dec ;

            int symmetricNote ;
            symmetricNote = getSymmetricNoteOfAThroughAxeBQuartTon(dec,n) ;

            // cout << "note " << dec << " -> " << symmetricNote << endl ;
            if ( isNoteInScaleMode(symmetricNote,gammeActuelle,i) )
            {
                // cout << "OK" << endl ;
            }
            else
            {
                // cout << "NOT OK" << endl ;
                estSymetrie = false ;
            }

        }

        if ( estSymetrie )
        {
            return n ;  // TODO DEBUG dans ce cas-ci il retourne le premier axe trouvé.  les rares gammes multi-axes ne font pas
            // l'objet de multiples modifications .
            // on peut imaginer une transfo qui force la symétrisation selon un axe !
        }

        // pour cet axe, on va tester toutes les notes du mode et de gamme.
        // la question est donc la suivante :
        // on prend la 1ère note de gamme
        // on calcule sa symmétrique selon l'axe n
        // cette nouvelle note appartient-elle à la gamme ?
        // si NON, stop
        // si OUI, passer à la note suivante
        // si OUI jusqu'au bout, la gamme accepte cette symétrie

        // on prend la 1ère note de gamme
        // on calcule sa symmétrique selon l'axe


        // cette nouvelle note appartient-elle à la gamme ?
        // si NON, stop
        // si OUI, passer à la note suivante
        // si OUI jusqu'au bout, la gamme accepte cette symétrie
    }

    return 0 ; // par défaut.  il faut auparavant tester s'il a un axe

}

bool hasSymmetricAxe( int gammeActuelle, int ModeActuel )
{
    int noteInit = 60 + ( gammeActuelle % 12 ) ;
    int i = ModeActuel ;

    for ( int n = 0 ; n < 12 ; n++ )
    {
        bool estSymetrie = true ;

        for ( int k = 1 ; k <= nbOfNotesInModeNumber( Scale[i]) ; k++ )
        {

            int dec = 0 ;
            dec = giveDecalageOfNthNoteOfScaleMode(k,gammeActuelle,i) ; // à priori la gamme ne devrait pas intervenir, c'est le mode qui détermine tout

            dec = noteInit + dec ;

            int symmetricNote ;
            symmetricNote = getSymmetricNoteOfAThroughAxeBQuartTon(dec,n) ;

            // cout << "note " << dec << " -> " << symmetricNote << endl ;
            if ( isNoteInScaleMode(symmetricNote,gammeActuelle,i) )
            {
                // cout << "OK" << endl ;
            }
            else
            {
                // cout << "NOT OK" << endl ;
                estSymetrie = false ;
            }

        }

        if ( estSymetrie )
        {
            return true ;
        }

        // pour cet axe, on va tester toutes les notes du mode et de gamme.
        // la question est donc la suivante :
        // on prend la 1ère note de gamme
        // on calcule sa symmétrique selon l'axe n
        // cette nouvelle note appartient-elle à la gamme ?
        // si NON, stop
        // si OUI, passer à la note suivante
        // si OUI jusqu'au bout, la gamme accepte cette symétrie

        // on prend la 1ère note de gamme
        // on calcule sa symmétrique selon l'axe


        // cette nouvelle note appartient-elle à la gamme ?
        // si NON, stop
        // si OUI, passer à la note suivante
        // si OUI jusqu'au bout, la gamme accepte cette symétrie
    }

    return false ; // par défaut.  il faut auparavant tester s'il a un axe

}
 
int noteADegreesAboveNoteBinGammeCmodeD(int aDegrees, int noteB, int gammeC, int modeD)
{
    if (aDegrees == 0)
    {
        return noteB;
    }

    int compteurDegres = 0;
    int noteStart = noteB;
    int increment = 1;
    if (aDegrees < 0)
    {
        increment = -1;
        aDegrees = -aDegrees;
    }

    do
    {
        noteStart = noteStart + increment;
        if (isNoteInScaleMode(noteStart, gammeC, modeD))
        {
            compteurDegres++;
        }
    } while (compteurDegres < aDegrees);
    return noteStart;
}

// String afficheLongueMelodie ()
// {
//     String affiche = nulString ;
//     for ( int i = 0 ; i < 1 ; i ++ )
//     {
//         for ( int j = 0 ; j < nbNotesByLayer ; j++ )
//         {
//             affiche += String(melodyMatrix[i][j][MMnote]) ;
//             affiche +=   "-" ;
//         }
//     }
//     return affiche ;
// }
