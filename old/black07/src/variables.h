// QUAND NOUVELLE VARIABLE INFLUENCANT LE JEU ET DEVANT ETRE SAUVEE SOUS KARAKT
// AJOUTER AU SAVE
// AJOUTER AU LOAD ( à partir de la ligne précédent )
// AJOUTER AU SAVE TO MEM
// AJOUTE AU LOAD FROM MEM

/* en détail 

QUAND ON RAJOUTE DES VARIABLES Qu'on VEUT SAUVER DANS KARAKT
PROCEDURE A SUIVRE
aller dans saveCurrentandMemKarakt()
on rajoute en bas de cette fonction des _printToKarakt avec des chiffres consécutifs, ainsi que le nom de variable et description
faire un compile pour vérifier que OK
un peu plus bas, dans la même fonction, recopier les nouvelles lignes et modifier en fromKMem_int_printToKarakt
re-compiler
aller dans loadLocal
ajouters les nouvelles lignes et modifier en    int_loadFromKarakt(128, &keepStepzBUG ); (=1)
re-compiler
aller ensuite dans le fichier karaktPile.h
trouver la fonction saveKarakt_to_KMem
y recopier les lignes juste modifée et en changeant le début par econome_int_saveToKMem(place,
re-compiler
recopier ces mêmes lignes en (=1)  EXACTEMENT sans rien modifer dans la fonction loadKarakt_from_KMem saut 
re-compiler 

*/

#include <Arduino.h>
#include <MIDI.h>
// #include <LiquidCrystal.h>
#include <Encoder.h>
// #include <SPI.h>
#include "SdFat.h"
// #include "math.h"

// file system object
// SdFat sd;
//SdFile file;

// BLACK 05 **MOD 
// SdFatSdioEX sd;

// SdFat sd;

File myFile;

// store error strings in flash
// #define sdErrorMsg(msg) sd.errorPrint(F(msg));

// MIDI_CREATE_DEFAULT_INSTANCE();
//   Encoder rotKnob2(21,20);
//    Encoder rotKnob(19,18);
//
// on utilise l'un ou l'autre selon le sens de rotation
//  Encoder rotKnob2(21, 20);
// Encoder rotKnob(19, 18 );

// #include <avr/pgmspace.h>

// SD default chip select pin.
// const uint8_t chipSelect = SS;

#define SD_CS_PIN SS

// Grandes Constantes, variables en fc de la mémoire à libérer

// const byte nbNotesByLayer = 32; // définit lenombre max de notes par layer
// 1C97 n'a plus de sens.

const byte nbLayer = 4; // définit le nombre de layers
const byte NbCCModMax = 32;
byte const tailleMaxTableauSequence(64); // = nb de data max dans une séquence
byte const nbSequencesMax = 32;          // pour commencer max 3 séquences
byte const markovTailleMax = 32;
byte const nbValeursMaxPourCell = 32;
// byte const tailleCell = 4; // 2C02 était à 64, ridicule car le tableau ne comporte que 4 variables.  sera remplacé par struct, anyway
const byte NbEvenementMax = 32;
const byte nbLFOMax = 16; // ça me paraît bien pour commencer


// BLACK06 : boutons de modulation

bool buttonShiftPressed = false;   int binShift = B10000000 ;
bool buttonCtrlPressed = false ;   int binCtrl  = B01000000 ; 
bool buttonAltPressed = false;     int binAlt =   B00100000 ; 
bool buttonMutePressed = false;    int binMute =  B00010000 ; 
bool buttonSavePressed = false;    int binSave =  B00001000 ; 
bool buttonLoadPressed = false;    int binLoad =  B00000100 ; 
bool buttonJSPPressed = false;     int binJSP =   B00000010 ; 
bool buttonExtraPressed =false ;   int binExtra = B00000001 ; 

int synthesePressed = 0 ;

    //2C31 sequential postpone
    bool turnOnSequentialPostpone[nbLayer] = {false};
int seqPostpone[nbLayer] = {0}; 
// indique la séquence que le sequential postpone va utiliser pour le layer

// keyboard STEPZ 2C30 17 septembre 2020 comment ai-je pu avoir besoin de tant de temps pour y penser ?
bool keyboardStepping = true;
byte startingKbStep = 60 ;
byte keyboardStepz[18] = {0} ; // couvre en fait 16 stepz, largement suffisant ! 

// shutUP 
bool shutUP = false ; 

// Decalagem 
int pourcentageDecalagem = 10 ; 
int maximumDecalagem = 10 ; 

// Sustain Blocker 2C28
bool proxyBlockerON = false;
unsigned int proxyBlockLimit = 1000; // msec
// unsigned long proxyPitch = 300 ; 
byte proximityPitch = 7 ; 

// Sustain Blocker 2C28
bool sustainBlockerON = false ;
unsigned int sustainBlockLimit = 1000 ; // msec 

    // Delay SubFidelity 2C28 et Drop pour Kannon

    int randomSubFidelityPourDelais[nbLayer] = {100, 100, 100, 100};
int dropPourKannon[nbLayer] = {100, 100, 100, 100};

// progress midi player
unsigned long midiFile_totalNotes = 0;
unsigned long midiFile_progress = 0;

// 2C28
bool autoHarmonizeLivePlaying = false;
int gammeActuelleLive = 0;
int modeActuelleLive = 15; // DODECA
int typeAutoHarmonizeLivePlayingMode = 0;
// 0 = + ou - 1 ( fluctue )
// 1 = + 1
// 2 = -1
// 3 = aucun filtre
// 4 = bloque
// 5 = bloque inverse ( ne laisse ce qui n'en fait pas partie )

bool modifMidiSpeedON = false;

bool accentMinON = false;
bool accentMaxON = false;

// 2C27 Send CC isolé
int sendCC_CH = 0;
int sendCC_CC = 0;
int sendCC_Value = 0;

// Verbose Serial 2C27

bool verboseSerial_sendToSerial = false;
bool verboseSerial_sendToLCD = false;
bool verboseSerial_sendChannel = true;

bool verboseSerial_sendNotes = true;
bool verboseSerial_sendCC = true;

bool verboseSerial_valueCC = true;
bool verboseSerial_velocity = true;

bool verboseSerial_string = true;
bool verboseSerial_timeStamp = true ; 

// midi File Info 2C27
String midiFileInfo = "";

// polyZ
int numerateurTimeSignature = 4;
int denominateurTimeSignature = 4;
//
int currentMem = 0;
int currentKarakt = 0 ; 

// Tracker
bool trackerActif[nbLayer] = {false, false, false, false};
int trackerSeq[nbLayer] = {0, 0, 0, 0}; // définit la séquence qui est liée au tracker

/* //AJOUTER A KARAKT
int kannonRandomAppear[nbLayer] = {100, 100, 100, 100};
int kannonDestinationChanel[nbLayer] = {16, 16, 16, 16}; // 16 = idem ; 0 à 15 = channel spécifique
int kannonBitmap[nbLayer] = {255, 255, 255, 255};
int kannonSeqBitmap[nbLayer] = {0}; // si négatif, indique SEQ
 */

int repeatVariations[nbLayer] = {1, 1, 1, 1};
int compteurRepeatVariations[nbLayer] = {0};

//************************************
// METTRE A JOUR CE QUI EST AUDESSUS POUR KARAKT
//*****************************************

// process AE

const byte percentDisappear = 10;
const byte percentRedouble = 0;
const byte percentFlipFlop = 5;
const byte percentSwitch = 25;
const byte percentLoot = 5;

// on pourra plus tard modifier ça par fonctions

// GABBERITO 2
int gabberito2Interval = 20;
int gabberito2K = 1;
int gabberito2N = 20;

// FLUX
byte currentFluxMem = 0;
byte statusFluxRecIn = 0;
int limitFlux = 300;

// Re-MIDI 2020
bool calculateMIDIsizeBeforePlay = true;

// Accent
// CH
int accentMax = 100; // int car Karakt n'aime pas les bytes
int accentMin = 20;

// Gabberito
int gabberitoInterval = 20;

// CH Mute
int CHMute = 0; // un bit est 1 si muted

// STEPZBUG
bool keepStepzBUG = false;

// Verbose MIDI
bool verboseMIDI = false;
int verboseDelay(0);

// SAVE LOAD KARAKT
int karaktNumber;
int karaktLayer;
int karaktValue;

// VARIATIONS
bool variationLayer[nbLayer] = {false, false, false, false};
int percentDisparitionChanceVariation[nbLayer] = {50, 50, 50, 50};
int minimumNotesVariation[nbLayer] = {1, 1, 1, 1};
int maximumReduxVariation[nbLayer] = {3, 3, 3, 3};
bool modeMinimumVariation[nbLayer] = {true, true, true, true};
int modeVariation[nbLayer] = {1, 1, 1, 1};

// si false : mode maximumRedux
// si true : mode minimumNotes

// TAP BPM
bool tapBPM = false;
unsigned long beatLengthHistory[nbLayer] = {100, 100, 100, 100};

// DUAL BUFFER : devrait être réécrit en C++ pour cacher certaines misères

// const byte sizeX = 16; // indique le nombre de caractères en X
// const byte sizeY = 8;  // indique le nombre de caractères en Y
// const int tailleEcran = (int)sizeX * (int)sizeY;

// char bufferWrite[sizeX][sizeY] = {0};
// char bufferDisplay[sizeX][sizeY] = {0};

// byte cursorX = 0;
// byte cursorY = 0;

// TWOPLETS
int ecartTwoplets = 0;
int ecartTwoplets32 = 0;
bool msecTwoplets = true; // devient faux si c'est en 32èmes

// DENDE in PICCOLO
bool dende[nbLayer] = {false};
int dendeSec[nbLayer] = {0};
int dendeMSec[nbLayer] = {100, 100, 100, 100};
int dendeDivide[nbLayer] = {3, 3, 3, 3};
int dendeDrop[nbLayer] = {90, 90, 90, 90}; // DEBUG il faudra modifier, pour avoir 0 - 100 - 200
int dendeDown[nbLayer] = {36, 36, 36, 36}; // DEBUG il faudra modifier, pour avoir 0 - 100 - 200
int dendeLevels[nbLayer] = {3, 3, 3, 3};
int dendeDecay[nbLayer] = {0};
int dendeStart[nbLayer] = {1, 1, 1, 1};
int dendeEnd[nbLayer] = {36, 36, 36, 36};
int dendeRatio[nbLayer] = {100, 100, 100, 100};
int dendeStretch[nbLayer] = {100, 100, 100, 100};
int dendePitch[nbLayer] = {0, 0, 0, 0};
int dendeFidelity[nbLayer] = {100, 100, 100, 100};
int overLoadLimit = 100;
int dendeRetard[nbLayer] = {100, 100, 100, 100};
int dendeGamme[nbLayer] = {0};
int dendeMode[nbLayer] = {0};
int dendeDegres[nbLayer] = {0, 0, 0, 0};
int dendeExpand[nbLayer] = {100, 100, 100, 100};
int dendeArp1[nbLayer] = {0, 0, 0, 0};
int dendeArp2[nbLayer] = {0, 0, 0, 0};
int dendeNoteOff[nbLayer] = {1, 1, 1, 1}; // sera à préciser

int dendeSubFidelity[nbLayer] = {100, 100, 100, 100}; // TODO prévoir fc pour modifier
int repeaterNbDende[nbLayer] = {0};

int NoteChannel = 0;
int NotePitch = 0;
int NoteVelocity = 0;

const byte NOTE_ON = 0x90;
const byte NOTE_OFF = 0x80;

int overLoadCounter = 0;
// 2C22 int currentRetard = 0;

unsigned long initialEnd;

int _bitMapDelay[nbLayer][8] = {1}; // à initialiser .  si vaut 1 delay a lieu, si vaut 0 pas
// pour au-delà de 8 delay : %8 !
int dendeBitMapDelay[nbLayer] = {0}; // si négatif, SEQ

// CUT SLICE
byte cutSlice = 2;
byte KcutSlice = 1; // rapport k/n

// GABBERISM
int gabberismLevels = 3;
int gabberismDivisions = 2;
int gabberismDivRatio = 2;
int gabberismRepeats = 2;
int gabberismRepeatRatio = 2;

// STEPZ
byte stepVelocity = 100;
byte stepChannel = 0;
byte nbStepz = 8;    // BLACK6 correspond au max au nombre de boutons physiques !  
byte nbSubStepz = 3; // correspond au nombre de sous-divisions, triplets, quadruplets, etc pour un step
byte currentStepzNote = 60;
byte stopModeStepz = 0; // 0 = subStepz 1 = stepz 2=10msec 3=rien
byte nbStepzAffiches = 8;  // BLACK5
byte nbStepzFor1BPM = 1;   //
byte nbBPM_Stepz = 133;    // à comparer avec autre valeur de BPM dans le software
byte currentPageStepz = 0; // aura des valeurs plus grandes si multiple affichage
byte numberMaxPageStepz = 0;

// SPANArp
long spanArp[nbLayer] = {30, 30, 30, 30}; // msec
byte sortMode[nbLayer] = {0};
byte ascendantMode[nbLayer] = {0};

// F8 Clock
// int F8Clock = 0;
// bool itsTimeForF8 = false;
// bool readyToSendF8 = false;

// Screen Clock
// int ScreenClock = 0;
// bool itsTimeForScreen = false;
// bool readyToUpdateScreen = false;

// DRUNK
bool drunk[nbLayer] = {false};
int drunkMin[nbLayer] = {0};
int drunkMax[nbLayer] = {0};
int drunkStep[nbLayer] = {1, 1, 1, 1};
int currentDrunkValue[nbLayer] = {0};

// KANNON multi layer

bool kannonActif[nbLayer] = {false};
int kannonRevert[nbLayer] = {0}; // % de revert
int kannonReflet[nbLayer] = {0}; // % de reflet
int kannonPitch[nbLayer] = {0};
int kannonDelay[nbLayer] = {200, 200, 200, 200};
int kannonRandom[nbLayer] = {100, 100, 100, 100};
int kannonStretchRatio[nbLayer] = {16, 16, 16, 16};

//AJOUTER A KARAKT
int kannonRandomAppear[nbLayer] = {100, 100, 100, 100};
int kannonDestinationChanel[nbLayer] = {16, 16, 16, 16}; // 16 = idem ; 0 à 15 = channel spécifique
int kannonBitmap[nbLayer] = {255, 255, 255, 255};
int kannonSeqBitmap[nbLayer] = {0}; // si négatif, indique SEQ

// TAP REC IN

byte statusTapRecIn = 0;

// SET PITCH
bool setPitchForWholeLayer[nbLayer] = {false};
byte pitchForWholeLayer[nbLayer] = {0};

// OMIT END

bool omitEnd[nbLayer] = {false};
int omitEndValue[nbLayer] = {0, 0, 0, 0};

// DERAZER

bool derazerActif[nbLayer] = {false};
byte derazerSeqChoice[nbLayer] = {0};
bool derazerOnChannel[16] = {false};

// F8 Send

// byte nbBeatsPourLayer1 = 1;
// bool sendF8 = false;
// long dureeF8 = 625000 / 133; // bpm 133 d'office

// NEW Save Local

int watchDelay = 0; // mettre à 100 si on veut voir le déroulement de LOAD SAVE

String fileName = "DEFAULT.txt";
String fileNameSave = "DEFAULT.txt";

const String idBlockLayers = "LAYERS";
const String idRecordLayer_Start = " // LAYER START-----------------------------------------------------------------------";
const String idRecordLayer_End = " // LAYER END------------------------------------------------------------------------";
const String idRecordLayer_Number = " // LAYER NUMBER";
const String idRecordLayer_Length = " // LAYER LENGTH";

const String idRecordNote_Note = "// NOTE VALUE";
const String idRecordNote_Channel = "// CHANNEL";
const String idRecordNote_Layer = "// LAYER";
const String idRecordNote_Velocity = "// VELOCITY";
const String idRecordNote_StartNote = "// START (msec)";
const String idRecordNote_EndNote = "// END (msec)";
const String idRecordNote_ThereIsAnEnd = "// FINITUDE";
const String idRecordNote_Type = "// TYPE (144=0x90 ; 128=0x80 ; 176=0xB0=CC)";

const String idBlockSequences = "SEQUENCES";
const String idRecordSequence_Start = " // SEQUENCE START*********************************************************************";
const String idRecordSequence_End = " // SEQUENCE  END*********************************************************************";
const String idRecordSequence_Number = " // SEQUENCE NUMBER";
const String idRecordSequence_Length = "// SEQUENCE  LENGTH";
const String idRecordSequence_modeSeq = "// mode 0 FWD    1 BACK    2 RND    3 A-R    4 JITTER    5 RND-FRG";
const String idRecordSequence_actif = "// Actif";
const String idRecordSequence_jitterMinUP = "// Jitter up min";
const String idRecordSequence_jitterMaxUP = "// Jitter up max";
const String idRecordSequence_jitterMinDOWN = "// Jitter down min";
const String idRecordSequence_jitterMaxDOWN = "// Jitter down max";
const String idRecordSequence_jitterStaticMin = "// Jitter static min";
const String idRecordSequence_jitterStaticMax = "// Jitter static max";
const String idRecordSequence_sizeRANDOMFRAG = "// Random Fragment";
const String idRecordSequence_repeatRANDOMFRAG = "// Random Fgt repeats";
const String idRecordSequence_compteLayer = "// Layer or Note";
const String idRecordSequence_action = "// Action  00 = CC    1 = Note     2 = Velocite    plus tard : 3 = SWING ";
const String idRecordSequence_CC = "// CC Number ";
const String idRecordSequence_channel = "// Channel number";
const String idRecordSequence_layer = "// Layer number (BINARY)";
const String idRecordSequence_values = "// Seq Value";

const String idBlockEvenements = "EVENTS";
const String idRecordEvenement_Start = " // EVENT START >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
const String idRecordEvenement_Number = " // EVENT NUMBER ";
const String idRecordEvenement_actif = "// Actif (1=YES)";
const String idRecordEvenement_type = "// Type : 0=Vide 1=CC 2=Transfo";
const String idRecordEvenement_intervalle = "// Intervalle ( x 0.1 sec )";
const String idRecordEvenement_moment = "// Moment after interval start";
const String idRecordEvenement_fidelite = "// Fidelity %";
const String idRecordEvenement_typeInterval = "// Type intervalle : 1 = constant 2 = random(a,b)  3=gaussian   4 = bi intervalle";
const String idRecordEvenement_which = "// if CC : WHICH ONE ; if Tf : WHICH ONE  ";
const String idRecordEvenement_valeur = "// VALUE of CC ; VALUE of Tf";
const String idRecordEvenement_shape = "// 1.Valeur Fixe 2.Inc 3.Dec 4.Rnd 5.Liste 6.liste Random  ";
const String idRecordEvenement_rangeUP_ = "// range up";
const String idRecordEvenement_rangeLO_ = "// range down";
const String idRecordEvenement_incSize = "// increase size";
const String idRecordEvenement_behaviorLimit = "// Behavior 1.change dir 2.other side 3.stay limit";
const String idRecordEvenement_channel = "// CHANNEL";
const String idRecordEvenement_layer = "// LAYER";
const String idRecordEvenement_eventGOON = "// Go ON ";
const String idRecordEvenement_conditionActive = "// CONDITION ACTIVE (1=YES)";
const String idRecordEvenement_whichTest = "// Which Test ( list in manual )";
const String idRecordEvenement_whichOperation = "// Which Operation 1% 2/ 3x 4%+ 5+ ";
const String idRecordEvenement_whichValue = "// Test Value";
const String idRecordEvenement_whichComparison = "// Which Comparison : 1< 2> 3= 4<>";
const String idRecordEvenement_whichReference = "// Which Reference";
const String idRecordEvenement_whichAction = "// 1.DO 2.BLOCK ";
const String idRecordEvenement_whichLayer2test = "// LAYER to TEST";
const String idRecordEvenement_modulo = "// compteur modulo N";
const String idRecordEvenement_moduloK = "//( compteur + K ) % n";
const String idRecordEvenement_moduloLayers = "// Layer pour modulo - b0000 à b1111 - 16 = ACTIVE";
const String idRecordEvenement_moduloMode = "// 1.ONLY- 2.ALL BUT ";
const String idRecordEvenement_echeance = "// Echeance ( au dela, pas d'Event)";
const String idRecordEvenement_latence = "// Latence ( en deca, pas d'Event)";
const String idRecordEvenement_fugace = "// Fugace (1=YES)";
const String idRecordEvenement_seqUsedAsList = "// SEQUENCE USED";
const String idRecordEvenement_End = " // EVENT END <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<";

const String idBlockCCMod = "CCMODS";
const String idRecordCCMod_Start = "// CCMOD START !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
const String idRecordCCMod_Number = "// CCMOD NUMBER";
const String idRecordCCMod_type = "// type : 0 = vide   1 = VELOCITY  2 = PITCH   3 = NOTE";
const String idRecordCCMod_sourceMin = "// Source min 0-127 pitch, velocity >127 pour durees";
const String idRecordCCMod_sourceMax = "// Source max";
const String idRecordCCMod_targetMin = "// Target min 0-127 valeur de CC";
const String idRecordCCMod_targetMax = "// Target max";
const String idRecordCCMod_borders = "// 0=limite 1=expand 2=No Send <min et >max   3=invert : envoie au-dela mais pas entre !";
const String idRecordCCMod_idCC = "// CC to which we send";
const String idRecordCCMod_channelCC = "// Channel";
const String idRecordCCMod_layerConcerne = "// concerned layers LAYER 1 to 4 - b0000 à b1111 - b10000 = 16 = ACTIVE";
const String idRecordCCMod_actif = "// Actif (1=YES)";
const String idRecordCCMod_End = "// CCMOD END !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";

const String idBlockCell = "CELLS";
const String idRecordCell_Start = "//CELL START //////////////////////////////////////////////";
const String idRecordCell_Number = "// CELL NUMBER";
const String idRecordCell_channelIn = "// CELL CHANNEL IN";
const String idRecordCell_noteIn = "// CELL NOTE IN";
const String idRecordCell_channelOut = "// CELL CHANNEL OUT";
const String idRecordCell_noteOut = "// CELL NOTE OUT ";
const String idRecordCell_End = "// CELL END //////////////////////////////////////////////";

const String idBlockMidiMatrix = "MIDIMATRIX";
const String idRecordMidiMatrix_Start = "// MIDIMATRIX START -------------------------------------";
const String idRecordMidiMatrix_Number = "// MIDIMATRIX NUMBER";
const String idRecordMidiMatrix_midiRedirect = "// CH->CH MATRIX --11 = CH1 + CH0";
const String idRecordMidiMatrix_End = "// MIDIMATRIX END -------------------------------------";

const String idBlockNoteRedirect = "NOTE REDIRECT";
const String idRecordNoteRedirect_Start = "// NOTE REDIRECT START -------------------------------------";
const String idRecordNoteRedirect_Number = "// NOTE REDIRECT NUMBER";
const String idRecordNoteRedirect_noteRedirect = "// Note -> Note ??";
const String idRecordNoteRedirect_End = F("// NOTE REDIRECT END -------------------------------------");

const String idBlockLFOs = "LFOs";
const String idRecordLFO_Start = " // LFO START >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
const String idRecordLFO_Number = " // LFO NUMBER ";

const String idBlockKarakt = "KARAKT";
const String idRecordKarakt_Start = " // KARAKT Start ---------------------------";
const String idRecordKarakt_End = " // KARAKT End ---------------------------";

const String idRecordKarakt_Number = " // KARAKT NUMBER ";

/*
bool actif;          // rajouté en nov2018, sur le modèle de Sequence
byte type;              // 0 = vide 1 = CC 2 = transfo : on va développer 1 pour l'instant, 2 sera pour plus tard, peut être
byte shape;             // sin square Sample&Hold saw
byte fidelite;          // normalement 100, mais on pourra avoir dégradation du signal
unsigned long duration; // en millisecondes
unsigned long decalage;
int lowest; // je mets INT car je pense que ce ne sera pas que pour CC, mais aussi Transfo !!!
int highest;
int compteur; // indique la valeur actuelle
byte channel;     // channel concerné 0-15
int which;        // CC Value ou valeur de Transfo
byte typeMachine; // 0 = normal : CC en 3 byte ; faudra spécifique pour microSampler, TX81Z, WavePad
int valeur;       // valeur à jouer au moment où eventGOON arrive

byte layer;              // layer concerné 1-4 ou combinaison ou random ; ou bien layers actives.   par défaut : Actif ; sinon spécifique
unsigned long eventGOON; // indique à quel moment le prochain Evènement doit avoir lieu
byte modC;
byte modV;
long oldResidue;
*/

const String idRecordLFO_actif = "// Actif (1=YES)";
const String idRecordLFO_type = "// Type : 0=vide 1=CC 2=transfo";
const String idRecordLFO_shape = "// 0Flat 1Nada 2Sin 3Sqr 4S&H 5SwUpDn 6RmpUp 7RmpDn"; // TODO compléter
const String idRecordLFO_fidelite = "// Fidelity %";
const String idRecordLFO_duration = "// duration";
const String idRecordLFO_decalage = "// decalage ";
const String idRecordLFO_lowest = "// lowest";
const String idRecordLFO_highest = "// highest";
const String idRecordLFO_channel = "// channel ";
const String idRecordLFO_which = "// which ";
const String idRecordLFO_typeMachine = "// 0=Nl 1=TX81Z 2=Sherman etc"; // TODO compléter
const String idRecordLFO_layer = "// layers ";                          // layer concerné 1-4 ou combinaison ou random ; ou bien layers actives.   par défaut : Actif ; sinon spécifique
const String idRecordLFO_modC = "// modulo Compteur";
const String idRecordLFO_modV = "// modulo Value";
const String idRecordLFO_LFORandom = "// random";

const String idRecordLFO_End = " //LFO END <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<";

// NEW Global Markov

int tailleChaineMarkov(0);
bool markovNEWChainGlobal(false);
int currentTailleNEWMarkovTable{0};
int markovNEWTailleMax = 10;
bool participateNEWMarkov[nbLayer] = {false};
bool participateLAYERMarkov[nbLayer] = {false};
int currentSizeLayerMarkov[nbLayer]{0};
int markovLayerMax[nbLayer] = {10, 10, 10, 10};
// pourrait être ajusté par une nouvelle fonction

// PAN + DERAZER

bool panMicroSamplerOnChannel[16] = {false};
bool panMicroSamplerOnLayer[nbLayer] = {false};
int panMicroSampler[nbLayer] = {0};

bool pitchMicroSamplerOnChannel[16] = {false};
bool pitchMicroSamplerOnLayer[nbLayer] = {false};
int pitchMicroSampler[nbLayer] = {0};

//Barocco

bool baroccoLayer[nbLayer] = {false, false, false, false};
int baroccoChosenSeq[nbLayer] = {0, 0, 0, 0};
// byte nbDeDelais[nbLayer] = {4, 4, 4, 4};
int nbDeMsecPourBarocco[4] = {100, 100, 100, 100}; // doit être négatif pour les SEQ
int dropPourBarocco[nbLayer] = {100, 100, 100, 100};
// byte downPourDelais[nbLayer] = {30, 30, 30, 30};
int randomPourBarocco[nbLayer] = {100, 100, 100, 100};
int symHBarocco[nbLayer] = {100, 100, 100, 100};
int symVBarocco[nbLayer] = {100, 100, 100, 100};
int refletBarocco[nbLayer] = {100, 100, 100, 100};
int preReflet[nbLayer] = {0, 0, 0, 0};
int preH[nbLayer] = {0, 0, 0, 0};
int preV[nbLayer] = {0, 0, 0, 0};

// holidays & fin

bool recAutomation = false;
unsigned int automationCompteur = 0;

byte automationStep = 1;

bool dynamicSwing[nbLayer] = {false};
int valueDynamicSwing[nbLayer] = {0};
bool eraseRecording = false;

bool pitchableLayer[nbLayer] = {true, true, true, true};
// TODO regrouper dans une struct toutes les caractéristiques de layers ! durée, pitchable, fidelity, etc. !
bool pitchableChannel[16] = {true};

int sourceMem = 0; // indique la mémoire source

byte valueC60 = 60; // crée le C60, mais on peut en modifier la valeur

unsigned int nbMaxDeNotesParLayer[nbLayer] = {10000, 10000, 10000, 10000};
byte modeDeletionWhenExtraNotes[nbLayer] = {1, 1, 1, 1};

// 1 = none
// 2 = Delete Oldest
// 3 = delete random
// 4 = delete ClosestPitch

// INITIALISATION dans subpats
bool vel2Vol[16] = {false};

bool delayLayer[nbLayer] = {false, false, false, false};
int nbDeDelais[nbLayer] = {4, 4, 4, 4};
int nbDeMsecPourDelais[nbLayer] = {100, 100, 100, 100}; // signed pour pouvoir enregistrer SEQ
int dropPourDelais[nbLayer] = {100, 100, 100, 100};
int downPourDelais[nbLayer] = {90, 90, 90, 90}; // signed pour pouvoir enregistrer SEQ
int randomPourDelais[nbLayer] = {100, 100, 100, 100};

// byte modStartPourDelais[nbLayer] = { 0 } ;
// byte modEndPourDelais[nbLayer] = { 100,100,100,100 } ;
int bitMapDelay[nbLayer][8] = {1}; // à initialiser .  si vaut 1 delay a lieu, si vaut 0 pas
// pour au-delà de 8 delay : %8 !
int repeaterNb[nbLayer] = {0};     // si négatif, SEQ
int seqBitMapDelay[nbLayer] = {0}; // si négatif, SEQ

byte layerToInspect = 0;

// unsigned long nowMicros = 0;
// unsigned long nowMillis = 0;
// int iterationCompteur = 0;

unsigned long tapBegin, tapEnd, newTempo;
bool tempoStarted = false;

double timeZero = 0;

bool SDCardPresente = false;

#include "struct.h" // contient les définitions de structures pour ce projet

// LFO pour CC et autres

LFOstruct LFO[nbLFOMax]; // TODO penser à prévoir une intialisation

byte currentLFO = 0;
int repeaterNbLFO[nbLFOMax] = {0};
byte machineSpecific[16] = {0}; // si le channel est machine specific, prend une autre valeur que 0
byte level480 = 0 ;
byte machineSpecificChannel = 0 ;

// 1 = TX81Z
// 2 = Sherman
// 3 = MicroSampler
// DEBUG 2C29 est-ce que 16 se réfère au LFO ou au Channel ?

// compteur F8
int compteurF8 = 0;
const int nbNotesToCountForF8 = 96;
String affiche;
long currentCounterSpeed;
// 24 F8 par quarternote
// 96 F8 par note
// 96 * nbNotes
// unsigned int lgAB ;

long lastAmountF8Time = 0;
long BPM = 0;
//bool bufferReady = false ;
byte channel;
byte note;
byte velocity;
byte midiType;
//bool readF8Again = false ;

// TreatNote : protection contre récursion infinie
long treatNoteTime = 0;

// edu eduLight
bool eduLight = true;
unsigned int limitEduLight = 30;

// softMute
bool softMute = false;

// FunkyVel pour midi player
int FunkyVelValueMIDIPlayer = 0;

// Octaver
byte octaverPercent = 100;
byte octaverRatioNum = 1;
byte octaverRatioDen = 1;

// numerote transfo

bool numeroteTransfo = false;

// Fidelity Whole Layer

int fidelityWholeLayer[nbLayer] = {100, 100, 100, 100}; // ne surtout pas remplacer par {100}
bool isWholeLayerFidelityMuted[nbLayer] = {false, false, false, false};
// TODO DEBUG : on ne peut pas se fier à l'initialisation des valeurs d'array par une simple {} pour toutes les valeurs !

// Rotary Encoder 2

int rotaryStepSize = 1;
long moment1rotaryStep(0);
long moment2rotaryStep(0);
int value1Step(0);
int value2Step(0);
const int tailleSTEP = 10;  // indique par quel ratio est multiplié le step qd passe à vitesse supérieure
const int dureeSTEP = 2000; // définit combien de temps il faut entre le saut décrit par tailleSTEP pour passer à vitesse sup ou inf

// syncop BUG

bool KeepSyncopBUG = false;

// Modulo Event by Layer

byte status270(0);

// Hash

byte hashSegments = 4;
byte hashPercentRepeats = 0;
byte hashNbRepeatsMax = 1;
byte hashPercentPermute = 100;
byte hashPercentInvert = 0;
byte hashUnify = true;

// AutoCleanUp
byte rememberCleanUpChoice = 1;
bool autoCleanUp = false ;

//
byte ledOn[nbLayer] = {0};
// modif Accou

bool tapAccou(false);

// constantes pour la chaîne de Markov

// byte tailleChaineMarkov (0);

// bool markovChainGlobal(false);
// int markovTable[markovTailleMax + 1] = {0};
// byte currentTailleMarkovTable{0};

// TODO DEBUG on doit rajouter une limite définie à la chaîne de markov

// compteur pour carte SD
int rootFileCount = 0;

// nouveaux compteurs

// int counterEncoder(0);
// int softwareValueEncoderLeft(0);
// int counter2Encoder(0);
// int softwareValueEncoderRight(0);
int pageEncoder(0);
int pageEndUser(0);
int oldCounterLeft = 0;
int oldCounterRight = 0;
// int oldC2 = 0;

// modulo layer

byte status248(0);

int eclipseMode[nbLayer] = {0}; // 0 = pas de mute   1 = mute selected    2 = mute all except selected
int eclipseModulo(0);
int eclipseK(0);
int eclipseModuloTab[nbLayer] = {0};
int eclipseKTab[nbLayer] = {0};
bool modeConstipe = false; // NOTE DEBUG prévoir une transfo pour on/off

// compteur de layers

unsigned long layerCounter[nbLayer] = {0};
// unsigned int oldLayerCounter[nbLayer] = {0} ;

int ajoutLayerNote[nbLayer][nbSequencesMax] = {0};
int ajoutLayerVel[nbLayer][nbSequencesMax] = {0};
int totalAjoutLayerNote[nbLayer] = {0};
int totalAjoutLayerVel[nbLayer] = {0};

// comptuer de notes
unsigned long TotalNoteCounter = 0;

// dynramic harmonize

int dynamicHarmonizeMode(0);
// 0 = + ou - 1 ( fluctue )
// 1 = + 1
// 2 = -1
bool harmonizeLayer[nbLayer] = {false};

// optimisation de Strings

// String const nulString = "";

// inclusion de développement fait en mai 2018 pour jitter

// sequenceStruct tableauSequence  ;   // DEBUG : on ne peut pas utiliser un seul tableau pour une fc qui sera appelée par plein de valeurs différentes
// NOTE*** attention il faut initialiser les pointeurs

// byte tailleActuelleTableau(0);
// String versionEcrite("");

const byte modeFWD = 0;  //
const byte modeBACK = 1; //
const byte modeRANDOM = 2;
const byte modeALLERRETOUR = 3;
const byte modeJITTER = 4;
const byte modeRANDOMFRAG = 5;
const byte modePROGRESSION = 6;
const byte modeSPECTRUM = 7;
const byte modeBIPOLAR = 8;
const byte modeSIMPLEBIPOLAR = 9;
const byte modeREPEATER = 10;

// 	case 8 : affiche += F("9.bipolar A *c-d E*f-g H%deA") ; break ;
//     case 9 : affiche += F("10.bipolar A B C%deA");
// case 10 : affiche += F("11.n repeater"); break ;

// pas encore assez fort pour l'orientation objet, mais il faudra aller vers ça
// pour l'instant je vais utiliser des variables globales
// l'idée : les rendre privées et utiliser des fonctions SET

// data pour sequence

sequenceStruct sequence[nbSequencesMax];
sequenceStruct baroccoSequence;

// JEUDI int sequence[nbSequencesMax][tailleMaxTableauSequence] = {0} ;
// j'ai mis int mais idéalement serait signed byte !

// byte sequenceSize[nbSequencesMax] = {0} ;
// devient une partie de la struct sequenceStruct : .tailleSeq
// byte sequenceIndex[nbSequencesMax] = {0} ;

// byte sequenceComportement[nbSequencesMax][nbDonnesComportement] = {0} ;

byte currentSequence(0);

// unsigned int sequenceCounter (0) ;

// chaque donnée définira un peu mieux la séquence
// par exemple : FORWARD BACKWARD
// jitter 3 à 7 ( MIN MAX )
// peut être mieux avec un STRUCT que array, à voir...

// byte Status223(0); // indique le status d'enregistrement de Sentence
// 0 = en attente de OK
// 1 = a effacé les layers, mis à 32, prêt à enregistrer.  va indiquer d'attendre le début de layer
//

long lastRecordedNoteTime(0);

bool softPass(false);
bool busyWithMIDIAffichage(false); // Sera vrai pour MIDI OX et pour CELL
// qd ceci est true, le son doit être joué à travers l'affichage, car la partie RECORD est déroutée

byte targetVelociteMin(50);
byte targetVelociteMax(126);

String PiccoloVersion;
String PiccoloDate;

// variables pour Cell
// tableau qui contient jusque 15 valeurs initiales.   15 est le nombre de pads sur le WavePad
// chaque fois qu'une valeur sera consolidée, le nb de valeurs entrées est +1
// au delà on n'enregistre plus rien

byte nbValeursDansCell(0);
cellBijection tableauCell[nbValeursMaxPourCell] = {0};
// 2C02 remplacement de l'array par un struct, correction de la tailleCell qui était inutilement énorme ( 64 pour 4 ! )

// byte channelForCell(0) ;
byte channelInForCell(0);
byte channelOutForCell(0);
byte noteForCellArray(0);
bool initKick(true);
byte lastConsPlayed(0);
bool consToCons(true);
bool atLeastOneKick(false);
byte currentPad(0);
byte lastPad(0);
byte lastChannel(0);
bool boutonTurned(false);

// poof est une version plus simple du bipolar : un seule pole, une seule valeur = 1 msec et un max
// le min est 0

int poof[nbLayer] = {0};
bool turnOnPoof(false);

byte noteToKill(0);
bool killingNote(false);

// définition du bipolar randomisation pour chaque layer
// pole1 : unité en millisecondes : 0 à 1000 disons
// pole1 : min ( -10..+10 )
// pole1 : max ( +10..+10 )
// pole2 : unité en millisecondes
// pole2 : min
// pole3 : max
// fréquence sur 100 de pole1 par rapport à pole2 ( 100-fq )

int DureePole1[nbLayer] = {0};
int minPole1[nbLayer] = {0};
int maxPole1[nbLayer] = {0};
int DureePole2[nbLayer] = {0};
int minPole2[nbLayer] = {0};
int maxPole2[nbLayer] = {0};
int freqPole1[nbLayer] = {100, 100, 100, 100};

bool turnOnBipolarRandom(false);

bool midiThru = true; // 2C28 devient vrai d'office, car toujours utilisé ainsi
byte channelForPlay1Note(0);

bool specialDebug = false; // met le Serial à 115200 et envoie note et VLVDelta

//  version avec le struct de point hérité de Rotate
//  ajout d'une structure EVENT
//  - CC
//  - Transfo
//  - modif valeur
//  ensuite il faudra reformuler la gestion du playNote

CCModule CCModule1;
// pileCCMod  pileCCMod1 ;
// pileCCMod* CCModPointeurInitial ;

int taillePileCCMod(0);

event Evenement[NbEvenementMax]; // DEBUG 1C86 : était avant +1
// Ceci sera remplacé par une pile d'Events ! // NOTE DEBUG ****
// s'inspirer du travail fait avec PileNote ! -> à imprimer !
byte CurrentEvenement = 0;
unsigned long EventtimeINIT;
// byte RecallActiveLayers ; // sert à enregistrer activeLayers le temps d'une transfo pour Evenement

// int i, j, k ;
// String ligne = "" ;
// char character ;

// String midistring = "";
// String charstring = "";
//
unsigned long exact1stbyte, lastexact1stbyte, exact2ndbyte, mididouble, specialmididouble = 0;
byte midibyte = 0;
String midiname; // = turca.mid
// 2C22 bool parser = false; // si false : va jouer les notes, sinon va faire un display sur le monitor

// long threadlen, resolution, timedivision, SMPTE = 0 ;
// int chunklen[256] ;   // me paraît particulièrement stupide comme façon de compter la longueur.   pq pas une valeur à laquelle on ajoute les valeurs trouvées ? = 1 int !  ou alors pour trouver note rapide
unsigned int NbChunk;
unsigned int CurrentMIDIChunk;
unsigned long VLVDelta, DureeDelta;
// midicategory

// byte NbdeChannels ; // va calculer le nombre de channels utilisés
// int ChannelsUsed ; // bit n = 1 si le channel (n) est utilisé

bool EndofTrack;

unsigned long nbNotesOn, nbNotesOff, unknownRunningStatus;

unsigned long totalDureeDelta;
byte lastRunningStatus;
unsigned long value1000ticks;
int calculatedBPM;
unsigned long NbTicksQuarterNote;

/* LE point sur ce qu'il faut développer : nov 2017

qu'est-ce qui est prioritaire ?
1. evolutory
2. midi file read
3. automation
4. FX

Display Fidelity : ajouter F
display lenght : ajouter L
FindNbFilesOnSD : ajouter variable qui contient ce nb au début, test quand load, qd save.  sera nécesaire pour RandomEvolution
refaire le point sur RotKnob.   Valeur 0-1000 directement ? non, car faudrait scroller à mort.  il faut que le rotknob augmente d'une unité en fc du display en cours.
FIXED, bug quand fc load et que carte est vide !
crééer fc cleanup
passer click2 et 3 sur les rotary encoders ?

créer une double automation : enregistre un movemnt de potard sur la duée du layer.   nb de valeurs max ? lissées ?
créer un autre tableau avec des events : arrive à un moment, fréquence indépendante, types d'actiosn différentes

SAVE :
fc qui vérifie si un fichier existe déjà

EVENTS
développer un tableau avec 32 events : durée loop, moment dans le loop ( pour 2 loops idem, distinguer moments ), type : CC, MODIFICATIO, chgt valeur constante

remplacer les 2 boutons médiocres du côté par le PUSH des rotary encoders !

pour le développement du stochastic Dauby, avant l'implémentation des Event Layer
ajouter des modifications de valeur pour
la valeur random de split note, RndUp, Shrink+valeur du Shrink, decimate, deplace
à plus long terme:  il doit modifier des valeurs de VOICES, pas que VCED, mais aussi ACED, de I01 à I32
ensuite dans le mode Performance, il charge les voices et joue de plusieurs Voix, qu'il modifie progressivement
ajouter un autoharmonize ON OFF



à développer :
-> PROGMEM pour les valeurs statiques du tableau Transfo
-> construire un nouveau board stable et enboitable

3 les modifications de mélodie ( à développer longuement, pê avec Alice ?
/ RANDOMIZE MELODY /BAROCCO (ajoute ornementation ): utiliser les fc harmonize pour avoir notes OK

ajout d'un controlleur UC33 pour les durées

8 le système évolutionnaire (EEPROM) : un bouton enregistre un chouette résultat ; un autre permet de
sauter à une autre chouette évolution ; un troisième fait un mix de 2 evolutions = sera la version WALLACE

quand on rajoute un nouvel effet :
1/ d'abord le mettre en case en fin de programme, dans la procédure Transformation
2/ ensuite, mettre dans le tableau transfo une ligne de 4 chiffres, avec n0 de transformation, valeur minimale du potard, valeur maximale puis coder l'affichage
3/ ensuite dans le case lié à l'affichage, mettre le nom et s'occuper de ce qui est affiché

*/

char name[30];
int nameLen = 40;
int currentFileNumber = 1;

// Modifier l'emploi des STRINGS

// char *letters = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";  // 66
// j'ai pas compris avec ce *

String letters[40] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0"};

// String randString   = nulString ; ;
String fileString = nulString;
;

// File myFile;

// String notename = ""  ;  // variable qui contiendra le nom de la note après la fonction

// comment peut-on limiter la taille de String à 2 caractères ?

// https://www.basicmusictheory.com/c-sharp-major-scale

/* Unfortunately, most C++ compilers do not have any means of expressing binary numbers directly in source code.

A few allow the prefix 0b followed by a series of 0 and 1 digits, e.g. 0b11 == 3.
The C++ compiler used by Arduino does not support this syntax. However, starting in Arduino 0007,
the constants B0 through B11111111 are defined for your convenience.
*/

// const unsigned long Scale[6] = { 2741L, 1453L, 2477L, 2733L, 661L, 1193L } ;  // 1C89
const int nbDeModesConnus = 15;
const unsigned long Scale[nbDeModesConnus] = {2741L, 1453L, 2477L, 2733L, 661L, 1193L, 1717L, 2773L, 1749L, 1459L, 1709L, 1451L, 1387L, 1365L, 4095L};

/*
0 ScaleMajor =  2741L;          // B101010110101  ;  // la note de départ est le bit 0, qd espace d'un ton un 0 suit, puis un 1
// qd demi-ton, c'est un 1 qui suit directement ( suit dans le sens bit, càd précède dans le sens lecture
// ce chiffre se lit donc : ton, ton, demi-ton, ton, ton, ton, demi-ton
1 ScaleMinorNatural =  1453L;   // 010110101101  ; // ton, demi-ton, ton, ton, demi-ton, ton, ton
2 ScaleMinorHarmonic = 2477L;   //  100110101101  ; // ton, demi-ton, ton, ton, demi-ton, 1 ton et demi, demi-ton
3 ScaleMinorMelodic =  2733L;   // 101010101101  ; // ton, demi-ton, ton, ton, ton, ton, demi-ton
4 ScalePentatonicMajor =  661L;      // 001010010101  ; // ton, ton, ton et demi, ton, ton et demi
5 ScalePentatonicMinor =  1193L;      // 010010101001  ; // ton et demi, ton, ton, ton et demi, ton
*/

// APPRENDRE a RAJOUTER LES NOUVELLES VARIABLES A LA SUITE !!!

String String374 = nulString;
String ChoixString374 = nulString;

// affichage et création String pour variation

String String153 = nulString;
String ChoixString153 = nulString;
// = " abcdefghij<klmnopqrst<uvwxyz0123<456789ABCD<EFGHIJKLMN<OPQRSTUVWXYZ<" ;
// const byte tailleChoixString153 = 66 ;

int nSwing = 4; // par défaut

String String159 = nulString;
// = "---" ;

// const String flechegauche =  (String) ((char)(B01111111) ); // flèche vers gauche
// const String flechedroite = (String) ((char)(B01111110) ); // flèche vers droite
// non reconnus par Teensy et u8x8
const String flechegauche = "<";
const String flechedroite = ">";
const String barremilieu = (String)((char)(B01111100)); // barre droite
const String barreH = "H";

String ChoixString156 = nulString;
// = "-" + flechegauche + flechedroite + barremilieu + barreH + "x!" + "<" ;

//const byte tailleChoixString156 = 8 ; // nb de caractère différents dans les choix

// const byte taillePresetString156 = 14 ;

// const String preset156_1 = "" + '-' + flechedroite + '-' + flechedroite ;
// const String preset156_2 = "" + barremilieu + flechedroite + barremilieu + flechedroite ;

String preset156[] =

    {
        "custom",
        "-" + flechedroite + "-" + flechedroite,
        barremilieu + flechedroite + barremilieu + flechedroite,
        flechedroite + "-" + flechedroite + "-",
        flechedroite + barremilieu + flechedroite + barremilieu,
        barremilieu + barremilieu,
        flechedroite + flechegauche + flechedroite + flechegauche,
        flechegauche + flechedroite + flechegauche + flechedroite,
        flechedroite + barremilieu + flechegauche,
        "--" + flechedroite + "-" + flechedroite,
        barremilieu + flechedroite + barremilieu + flechedroite,
        "-x--x",
        barremilieu + "x" + barremilieu + barremilieu + "x",
        "-!-!xx!-x" // ce dernier sera remplacé par le custom !!!
                    // NOTE**** développer des patterns intéressants !!!
};

// NOTE **** si on change le nb de presets dans preset156, il faut chagner lt ableau Transfo 156 !
//

String patternSwing = preset156[0];
// String patternSwingCustom = nulString ;

byte typeFuse(0);
byte pcAB(0);

const long phiNum = 75025;
const int phiDen = 43368; // phi est approximé par phiNum/phiDen

// byte  diffnote,
// byte rootnote ,
int gammeActuelle[nbLayer] = {0}; // modif 2C12
int recordMode = 1;               // çàd enregistre par défaut la source avec son CHANNEL
byte RndUpScope = 1;
int DeplaceScope = 200;

// float fracRatio;
double realRatio;

byte ThruWhere = 100; // valeurs 0 à 15 enverront à terme vers le channel spécifié ( code 0 = CH midi 1 )
// si valeur = 100 : midi Thru software = OFF

// int derniereA  = LOW;
// int n = LOW;

bool  FunkyVel = false;
int FunkyVelValue[nbLayer] = {0};

// byte calcnote ;
// int compteur = 0;
const byte blocktime = 140; // indique le temps de blocage après avoir poussé un bouton.   si trop grand : réduit réactivité , si trop court un hit simple peut être compté pour 2 !
// bool pressed1, pressed2, pressed3 = false ; // indique qu'on a pressé sur le bouton - permet d'éviter l'utilisation de delay
bool automodification = false;  // si devient vrai, tous les automodificationtime, il y a aura modif automatique
bool Daubymodification = false; // si devient vrai, lance des modifications à chaque itération pour la TX81Z

// byte lowMIDIvalue, highMIDIvalue  ; // définit la plus haute et la plus basse valeur midi utilisée par un channel.   attention, ne dépend pas du layer, mais du channel.
// il en faudra donc à vrai dire un array de 16 !!!

byte activeLayer = 00000001;
byte muteLayer = 00000000;
// 0000 0001 = layer 0              // je dois relire méthode pour extraire bit d'un byte !
// 0000 0010 = layer 1              // y a-t-il moyen d'allumer 4 diodes de façon sélective à partir d'un seul digital ? ( allumer en fc du bit de activeLayer bien sûr )
// 0000 0011 = layer 0 et 1
// 0000 0100 = layer 2
// 0000 0101 = layer 2 et 0
// 0000 0110 = layer 2 et 1
// 0000 0111 = layer 2, 1 et 0
// 0000 1000 = layer 3
// 0000 1001 = layer 3 et 0
// 0000 1010 = layer 3 et1
// 0000 1011 = layer 3 et 0 et 1
// 0000 1100 = layer 3 et 2
// 0000 1101 = layer 3 et 2 et 0
// 0000 1110 = layer 3 et 2 et 1
// 0000 1111 = layer 3 et 2, 1 et 0

// 0001 0000 = concerne le midi.play parallel

unsigned long time1, time2, timenote, previousTime2 = 0;
unsigned long pressed1time(0), pressed2time(0), pressed3time(0), pressedE1time(0);
unsigned long pressedL1time(0), pressedL2time(0), pressedL3time(0), pressedL4time(0);
unsigned long pressedRE8time(0), pressedRE9time(0);
// moment de pression pour les boutons pousssoirs supplémentaires du bas et ceux de l'encodeur rotatif

unsigned long automodificationtime;

// const byte nbTotalPiles = 4 + 64 + 4 * 4;
// passage en LONG à partir de 1C92
unsigned long layerLength[4 + 64 + 4 * 4 + 10] = {1000, 2000, 3000, 4000}; // indiquera la durée de chaque layer, en msec    // si on veut des boucles de plus de 32 secondes on doit utiliser LONG pour les 3 variables suivante
// 2C22 jamais utilisé unsigned long layerDecalage[nbLayer] = {0};
// 2C25 le +10 sont les layers de Flux

unsigned long actualizedlayerLength;                      // utilisé pour calculer le layerlenght avec les ratios.
unsigned int ratioLength[nbLayer] = {100, 100, 100, 100}; // indique par combien de 100ème il faut multiplier les valeurs en lien avec le Varispeed : 100% = vitesse normale.   1% = 100 x plus lent ; 900 = 9x plus vite.
unsigned int ratioCurrent[nbLayer] = {100, 100, 100, 100};
unsigned int totalratioLength;
byte varispeedState = 1;                            // valeurs : 1 = OFF ; 2 = ON ; 3 = FIX ; 4 = MOD
byte layerChannel[nbLayer] = {0, 0, 0, 0};          // soit MIDI channel 1, 1,  1 et 1 !
int fidelityLayer_[nbLayer] = {100, 100, 100, 100}; // d'emblée on joue toutes les notes, mais on pourra réduire cette "fidélité" !

unsigned long derniermodulo[nbLayer] = {0};
unsigned long avantderniermodulo[nbLayer] = {0};

// byte jffn, notefinder = 0;     valeurs passées dans la fonction elle-même
// bool fulllayer = true;
//bool foundfirst = false;

// bool fairelongue = true;

//byte actionMatrix[5][32][4]       ;
//unsigned int actionMatrixTime[4][32][2] ;

// 1C96 fin du melodymatrix : une page qui se tourne
// et beaucoup de mémoire qui se libère !!

// byte melodyMatrix[nbLayer][nbNotesByLayer ][5]       ;  // 4 layers, 32 notes, 4 paramètres        // on utilisera du bitwise arithmetic pour compresser les données
// quelle est la durée max de int ???
// peut-on créer un array de différents types de variables ?  si oui, combiner les 2 tableaux

////const byte MMnote   = 0 ;
//const byte MMvelocite   = 1;
////const byte MMdata   = 2;  // [MMdata]>>4 & 1111 = chord
// 0 = solo  1 = maj   2 = min   3 = min7 etc    byte
// [MMdata]>>0 & 1111 = midi channel
// 0 = midi1   1 = midi2   2 = midi3, etc        byte
//const byte MMdata2   = 3 ; // [MMdata2]>>0 & 0001 = played
// 0 = non    1 = oui
// [MMdata2]>>1 & 0001 = silented
// O = non    1 = oui
// [MMdata2]>>2 & 0001 = just added : qd transformation où on créée une note, il ne faut pas qu'elle fasse
// juste après l'objet d'une transformation.
// il reste 5 bits disponibles dans le data2 !      peut-être pour MUTE ???

// byte channel = 0 , note = 0, velocity = 0 ; // pour lecture MIDI IN
// const byte MMnoteReallyPlayed = 4 ;

// signed int melodyMatrixTime[nbLayer][nbNotesByLayer][2] = {0} ;                                                        // était unsigned
// unsigned long timeToPlayMatrix[nbLayer][nbNotesByLayer][2] = {0} ; // 0 = calcul ON time   1 = calcul OFF time
// doit rentrer dans le notePile ! TODO DEBUG

unsigned long countertime1(0);
unsigned long meancounterspeed, counterspeed = 0;
const long nbTicsPourCounterSpeed = 10000;
bool timeToClear = true;

// const byte MMTstartnote = 0;
// const byte MMTendnote = 1;

// ce qui suit pourrait être mis dans PROGMEM

// les valeurs sup doivent être commme pour random( ) + 1
// UNIFORMISER TABLEAU ET TRNASFO en faisant : une procédure va voir dans le tableau à quelle x correspond une valeur Transfo[x] tel que Transfo[x][1] = TTT
// de la va chercher min = Transfo[x][1] et max = Transfo[x][2]
// plus simple : il faut transmettre la valeur affichée ! vers la fonction.
// NON car cela empêcherait la transformation aléatoire, pour laquelle on envoie une valeur de potard entre 0 et 1023

// byte boutonNoir1, boutonNoir2, boutonNoir3 = 0 ; // indique si bouton poussoir noir a été pressé
bool RECtiming;     // indique si est en train d'enregistrer la durée
bool record = true; // indique si on est en train d'enregistrer une mélodie. 2C28 TRUE d'office

// valeurs pour Create Ladder
// Ceci devrait être encapsulé : la fonction transformation qui modifie ces valeurs les
// modifie dans une classe ou une méthode, pas dans des variables générales
// NOTE OOP pour l'avenir !

byte ladStart = 60;
byte ladNbSteps = 5;
byte ladStep = 1;
byte ladIncStep = 0;

int ladLength = 100;
int ladPause = 100;
int ladIncLength = 0;
int ladIncPause = 0;

// NOTE DEBUG pour gagner place on peut inactiver LAdder ?
//
int potVal, initPotVal2, potVal2, potVal3; // lecture des potards qui indiqueront le choix de transformation et 1 valeur modifiable
// potVal3 est utilisé quand potVal2 est lu linéairement puis transformé en des valeurs non continues
// int Wobble ; // utilisé pour définir une valeur intermédiaire

// byte midichannel ; // utilisé par le midiplayer pour savoir dans quel channel il faut jouer
int currentKnob2; // utilisé par le midiplayer pour la valeur du Knob2.

int midiLocationFgt = 1;
byte midiLongueurFgt = 32;
// long tailleTotaleMIDI; // indiquera le nb de notes du fichier MIDI - sera utile surtout pour la fc Load Random Sequence
// 2C28 : remplacé par midiFile_totalNotes
bool FillUpEnoughNotes;
long startmiditime; // décrit le moment en msec où la séquence MIDI commence dans le fichier

int CurrentMIDIlocation; // définit l'endroit où on a lu la dernière fois dans le fichier MIDI.  on le garde pour faire RELOAD éventuel !
// int TailleTotaleMIDI ; // décrit en notes la taille d'un fichier.   est déterminé si on fait un PLAY ou un DEFINE midi file
/// A FAIRE 8888!!!!!

bool SqueezeOn = false;
bool KeepLength = false;

unsigned int oldlayerLength;
unsigned int KeepDuree;

// Création de rythme Euclidien

byte NoteNecklace = 60;
byte kNecklace = 5;
byte mNecklace = 8;
int LGNecklace = 100;   // longeur des notes dans le Necklace
int ShiftNecklace = 0;  // Shift dans la séquence
byte VelNecklace = 110; // Vélocité du necklace

// Channel To Behringer DSP2024P Virtualizer

int CHSendToSampler; // les bits indiquent si le channel doit être envoyé au sampler : ex SendToSampler >> 3 == 1 alors on fait que que jouer note, on envoie des CC au channel 3 pour jouer un sample
int CHMuteNote;      // idem pour indiquer que le channel ne doit pas jouer de notes.   intéressant si on veut exclusivement faire jouer le sampler sur un channel

// variables utilisées par le GATED multiple

byte NbMultiplications = 4;
String MultiplicationsString = nulString;
; // il faut trouver comment on définit la taille max d'un String !

// printRot
//
// String memorytexte = nulString;
;
// byte previousl;
// unsigned long tempoinit;
// bool firsttime;
// byte waitROT = 6; // décrit le nombre de fois que le temps de défilement est attendu en début et fin de scroll

// printRot2
// byte const printRotSlots(7);
// String Rmemorytexte[printRotSlots] = nulString;
// byte Rpreviousl[printRotSlots]{(0)};
// unsigned long Rtempoinit[printRotSlots]{(0)};
// bool Rfirsttime[printRotSlots];
// long lastAffichage[printRotSlots] = {0};
// optimisation de l'affichage, max 30x/seconde
// contient en micros le moment du dernier affichage d'un slot

// slotType slot[printRotSlots + 1];

// autoharmonize
bool autoHarmonize = false;
int modeActuelle[nbLayer] = {0}; // mémorise le mode actuelle - modif 2C12

//complexSqueeze
bool complexSqueeze = false;

// VLV Glitch : reprend les constantes habituellement utilisées par VLVRead pour permettre de les faire glitcher.
// initialement elles sont évidemment correctes.
// il faut créer des fonctions permettant de les faire varier.

long iVLVDelta = 0;
long iDureeDelta = 0;
long imididouble = 0;
byte iShiftDelta = 8;
byte iShiftDuree = 7;
byte iDureeMask = B01111111;
byte iMask2 = B10000000;

unsigned int CutOffDureeDelta = 1000; // NOTE **** devra peut-être être changée pour ne pas avoir de cutoff qd on veut perfection
int BringToDureeDelta = 0;
float RatioDeltaToMsec = 4.0;
int NumRatioDeltaToMsec = 100;
int DenomRatioDeltaToMsec = 100;

bool playParallelMidiON = false;
bool MUSTreadHeader = true;

int currentKnob2FIXED(500);
long parallelMIDItimeINIT(0);
long GoONmidi(0);

// 4 switches for MUTE & more
// NOTE**** n'a plus été utilisé, mais pourrait le devenir si on veut attribuer des fcs à UP et LO
// bool switch1UP ;
// bool  switch1DOWN ; ;
// bool  switch2UP   ;
// bool  switch2DOWN  ;
// bool  switch3UP   ;
// bool  switch3DOWN  ;
// bool  switch4UP   ;
// bool  switch4DOWN  ;

bool switches[8]; // valeur des différents switches  0 et 1 sont pour le premier : UP et DOWN ; 2 et 3 pour le 2nd ; etc

// phasage ;

int kPhasage = 1;
int nPhasage = 32;

point Centre;
byte CentreX = 50;
byte CentreY = 63;

float RapportPitchLayer;
bool dontRotateLength = false;

String NameTransfo = nulString;
;

unsigned int midiRedirect[16] = {0}; // contient la matrice de redirection des CH midi
// peut être à sauver/charger .   peut être en crééer plusieurs pour faire des switchs rapides
// NOTE¨**** pour plus tard !

byte targetMidiMatrix(0);
bool SkipChunck(false);

int LGnotMax(100);

byte dynamicLOcut(2);
byte dynamicHIcut(127);

bool sendCCtoTX81Z = false;
bool sendCCtoSHERMAN = false;

unsigned int mapHTargetMax(1000), mapHTargetMin(0), mapHSourceMax(1000), mapHSourceMin(0);

int noteRedirect[12]{0}; // contient la matrice de pitch des notes
// ex : -12 = -2 octaves    +5 = vers sa quinte    0 = note inchangée
// le bit 7 indique si négatif
// le bit 6 indique si silencieux
// il y a des fonctions pour conversion :
//
// bool isNegativeSByte( byte const& thi  ) {
// bool isSilentSByte( byte const& thi  ) {
// int valueSByte( byte const& thi  ) {
// byte makeSByte( int  thi , bool const& silent ) {

byte typeBreathe(0);

// pour l'affichage des fractions

fract fraction;
bool showFract = true; // si true va montrer tous les temps en fractions !

byte noteToWorkOn(0); // la note sur laquelle on va travailler pour Fregoli et Cotard

float divisionUltraMap(20.0);

// Pile de CCMod
int currentCCMod(0);
// int highestCCMod (1);

CCModule CCModTableau[NbCCModMax]; // modif 1C86 : était avant +1

// bool DEBUG (false);

bool CCModActif(false);

#include <TransfoArray.h>

// -------------------------------------
