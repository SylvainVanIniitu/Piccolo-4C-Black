#include <midiMatrix.h>
#include <DMXroutines.h>   // PreRelease1

// prototype

void measureSDFileCount(void);

const float RadianToDegrees = 57.295779513;

// PROTOTYPES DE CERTAINES FONCTIONS
//
bool isNegativeSByte(byte const &thi);
bool isSilentSByte(byte const &thi);
int valueSByte(byte const &thi);
byte makeSByte(int thi, bool const &silent);
void Transformation(int TTT, int BBB);

long ultraMap(long depart, long sourceA, long sourceB, long targetA, long targetB, float divisionUltraMap);
signed int giveBipolarRandomForLayer(byte i);
byte dynamicHarmonize(byte note, byte layer);
int putNoteAinMarkovChainAndGetOneOutAsResult(int noteIn);
unsigned long dynamicTreatNote_pileNote(pileNote &tempNote, unsigned long lgAB);
unsigned long dynamicTreatNote_pileNote_SIMPLIFIED(pileNote &tempNote);

bool isLayerActive(byte layer);
byte limiterForPitch(int pitchInitial);
void fixStartEndNote(byte i, byte j, long potVal2 = 0);
void pileNote_fixStartEndNote(pileNote &celleCi, long potVal2 = 0);
void changePitchandCheckAlone(int i, int j, int direction);
bool isNoteInScaleMode(int z, int gamme, long mode);
void pileNote_actualizeTimeToPlay(pileNote &cetteNote);

void newHarmonize(byte layer, int gammeActuelle = 0, long potVal3 = 0);
int giveNextValue(sequenceStruct(&tab), long compteurThat);
void buffer_and_HARD_setCursor(byte x, byte y);
void buffer_and_HARD_print(String Texte);
int getValueStraightOrFromSequence(int value, int floor) ;

	// FONCTIONS EN TANT QUE TELLES

	bool isWinogradDeep(String &String1)
{
	// a rhythm is Winograd-deep if each distance, 1,2, .. n/2 has a unique number of occurences
	// toutes les distances doivent apparaître 1 fois et un nb différent de fois des autres
	String longString = nulString;
	; // utilisé pour Erdös, Winograd, Aksak et le shift
	longString = String1 + String1;
	byte n = String1.length();

	byte valeurs[100];

	for (byte i = 1; i <= n / 2; i++)
	{
		valeurs[i] = 0;
	}

	for (byte i = 1; i <= n; i++)
	{
		for (byte j = 1; j <= (n / 2); j++)
		{
			if ((longString.charAt(i - 1) == longString.charAt(i + j - 1)) && (longString.charAt(i - 1) != '0'))
			{
				valeurs[j]++;
				// il faudrait idéalement comparer avec usualB plutôt que '0'
			}
		}
	}

	bool WDeep = true;

	for (byte i = 1; i <= n / 2; i++)
	{

		//if ( valeurs[i] == 0 ) {
		//   WDeep = false ;
		// }
		for (byte j = i + 1; j <= n / 2; j++)
		{
			if ((valeurs[i] == valeurs[j]) || (valeurs[i] == 0))
			{
				WDeep = false;
			}
		}
	}

	return WDeep;
}

bool isErdosDeep(String &String2)
{
	// a rhtyms is Erdös-deep is it has k onsets and for every multiplicity 1,2, ... k-1 there is a nonzero arc-length distance determined by the points on the circle with exactly that multiplicity
	// les distances doivent apparaître un nb différent de fois
	// mais il ne faut pas toutes les distances
	String longString = nulString;
	; // utilisé pour Erdös, Winograd, Aksak et le shift
	longString = String2 + String2;
	byte n = String2.length();

	byte valeurs[100];

	for (byte i = 1; i <= n / 2; i++)
	{
		valeurs[i] = 0;
	}

	for (byte i = 1; i <= n; i++)
	{
		for (byte j = 1; j <= (n / 2); j++)
		{

			if (longString.charAt(i - 1) == longString.charAt(i + j - 1))
			{
				// Serial.println("IDEM");
				if (longString.charAt(i - 1) == '0')
				{
					// Serial.println(" BUT NUL");
				}
				else
				{
					valeurs[j]++;
				}
			}
		}
	}

	bool EDeep = true;

	for (byte i = 1; i <= n / 2; i++)
	{

		for (byte j = i + 1; j <= n / 2; j++)
		{
			if ((valeurs[i] == valeurs[j]) && (valeurs[i] != 0))
			{
				EDeep = false;
			}
		}
	}

	return EDeep;
}

bool isAksak(String &String3)
{
	// those meters that use units of duration 2 and 3, and no other durations.   must contain at least one duration of length 2 and at lesat one duration of lenght 3
	// authentic if m is prime
	// quasi-aksak if m is odd
	// pseudo-aksak if m is even
	String longString = nulString;
	; // utilisé pour Erdös, Winograd, Aksak et le shift
	longString = String3 + String3;
	byte n = String3.length();
	byte i, j;
	byte valeurs[100];

	for (i = 1; i <= n / 2; i++)
	{
		valeurs[i] = 0;
	}

	for (i = 1; i <= n; i++)
	{
		for (j = 1; j <= (n / 2) + 1; j++)
		{

			if (longString.charAt(i - 1) == longString.charAt(i + j - 1))
			{
				// Serial.println("IDEM");
				if (longString.charAt(i - 1) == '0')
				{
					// Serial.println(" BUT NUL");
				}
				else
				{
					valeurs[j]++;
					j = n / 2 + 1;
				}
			}
		}
	}

	bool Aksak = false;

	if ((valeurs[2] != 0) && (valeurs[3] != 0))
	{
		Aksak = true;
	}

	for (i = 1; i <= n / 2; i++)
	{
		if ((i != 2) && (i != 3) && (valeurs[i] != 0))
		{
			Aksak = false;
		}
	}
	return Aksak;
}

bool isOdd(int &cenombre)
{
	// détermine si est impair

	bool odd = ((cenombre % 2) == 0 ? false : true);

	return odd;
}

String StringShift(String &cettechaine, int &shift)
{

	byte n = cettechaine.length();
	String longString = nulString;
	; // utilisé pour Erdös, Winograd, Aksak et le shift
	longString = cettechaine;

	if (shift > 0)
	{
		// SHIFT 1 place vers la droite
		// cela veut dire : prendre le dernier caractère, en faire le premier, puis coller les n-1 caractères derrière
		for (int i = 1; i <= shift; i++)
		{
			longString = longString.substring(n - 1, n) + longString.substring(0, n - 1);
		}
	}

	if (shift < 0)
	{
		// SHIFT 1 place vers la gauche
		// cela signifie prendre le string du 2ème caractère au dernier, puis de coller derrière le premier

		for (int i = 1; i <= abs(shift); i++)
		{
			//longString = longString.substring(n-1,n) + longString.substring(0,n-1);
			longString = longString.substring(1, n) + longString.substring(0, 1);
		}
	}

	return longString;
}

String GatedString(byte k, byte m)
{
	// k définit la valeur qui sera affichée par le String
	// m décrit la longueur souaitée du String
	// ex : (2, 4) donnera 0010, tandis que (2,6) donnerait 000010, (12,6) serait 001100
	String GString = String(k, BIN);
	// si < m

	while (GString.length() < m)
	{
		GString = "0" + GString;
	}

	// si > m

	if (GString.length() > m)
	{
		GString = GString.substring(GString.length() - m, GString.length());
	}

	return GString;
}

unsigned int MaximumBinForString(String &St)
{
	byte n = St.length();
	unsigned int m = (unsigned int)(pow(2.0, n));
	// on ne doit pas faire de -1 car il travaille en float et visiblement arrondit à l'unité inférieur,
	// étrangement ....
	return m;
}

unsigned int MaximumBinForSteps(byte &n)
{

	unsigned int m = (unsigned int)(pow(2.0, n));
	// on ne doit pas faire de -1 car il travaille en float et visiblement arrondit à l'unité inférieur,
	// étrangement ....
	return m;
}

byte FindLowestNote(byte &i)
{ // va trouver la note la plus basse du layeri

	byte Lownote = 200; // on part d'une valeur impossible

	unsigned int taille = mesureTaillePileComplete(pInitial[i]);

	for (byte j = 0; j < taille; j++)
	{
		pileNote tempNote;
		tempNote = ZEROgetNoteFromPile(pInitial[i], j);

		// si valeur plus petite que LOW mais pas nulle alors LOW devient valeur

		if ((tempNote.note < Lownote) && (tempNote.note != 0))
		{
			Lownote = tempNote.note;
		}
	}
	// peut ^^etre faudra t il donner une valeur par défaut si est resté 200
	if (Lownote == 200)
	{
		Lownote = 0;
	}

	return Lownote;
}

byte FindHighestNote(byte &i)
{ // va trouver la note la plus haute du layeri

	byte Highnote = 0; // on part du plus bas !!! mais attention un layer vide renverra 0 !!!
					   // la lowest des active layer doit être la plus basse exceptée 0 !
					   //  bool found = false ;
	unsigned int taille = mesureTaillePileComplete(pInitial[i]);

	for (byte j = 0; j < taille; j++)
	{
		pileNote tempNote;
		tempNote = ZEROgetNoteFromPile(pInitial[i], j);

		if (tempNote.note > Highnote)
		{
			Highnote = tempNote.note;
			// found = true ;
		}
	}

	// peut ^^etre faudra t il donner une valeur par défaut si found = false !!!

	return Highnote;
}

byte FindLowestNoteOfActive()
{ // va trouver la note la plus basse du layeri
	byte allLowest = 200;

	// DEBUG NOTE **** pas bon, car parfois un layer est vide.   il retourne alors 200 c'set ridicule

	for (byte i = 0; i < nbLayer; i++)
	{

		if (isLayerActive(i))
		{
			if (FindLowestNote(i) != 0)
			{
				allLowest = ((allLowest > FindLowestNote(i)) ? FindLowestNote(i) : allLowest);
			}
		}
	}
	if (allLowest == 200)
	{
		allLowest = 0;
	}

	return allLowest;
}

byte FindHighestNoteOfActive()
{ // va trouver la note la plus basse du layeri
	byte allHighest = 0;
	// bool found = false ;

	for (byte i = 0; i < nbLayer; i++)
	{

		if (isLayerActive(i))
		{

			allHighest = ((allHighest < FindHighestNote(i)) ? FindHighestNote(i) : allHighest);
		}
	}
	return allHighest;
}

String Euclidian(byte B, byte A)
{ // première méthode, non récursive, basée sur le calque exact de la façon euclidienne.
	// variables pour le calcul des Strings euclidiens - première méthode, non récursive

	String usualA = F("1");
	String usualB = F("0");

	byte nbA, nbB, newnbA, newnbB;

	String contenuA = nulString, contenuB = nulString, newcontenuA = nulString, newcontenuB = nulString;

	String totalcontenuA = nulString;
	;

	byte tEuc;

	nbA = B;
	nbB = A - B;

	contenuA = usualA;
	contenuB = usualB;

	while (nbB >= 1)
	{

		if (nbA < nbB)
		{
			newnbA = nbA;
			newnbB = nbB - nbA;
			newcontenuB = contenuB;
		}
		else
		{
			newnbA = nbB;
			newnbB = nbA - nbB;
			newcontenuB = contenuA;
		}

		newcontenuA = contenuA + contenuB;

		nbA = newnbA;
		nbB = newnbB;
		// tailleA = newtailleA ;
		// tailleB = newtailleB ;
		contenuA = newcontenuA;
		contenuB = newcontenuB;
	}

	totalcontenuA = contenuA;

	for (tEuc = 1; tEuc < nbA; tEuc++)
	{
		totalcontenuA = totalcontenuA + contenuA;
	}

	for (tEuc = 1; tEuc < nbB; tEuc++)
	{
		totalcontenuA = totalcontenuA + contenuB;
	}

	return totalcontenuA;
}

byte lastnotebeforetime(byte thisnote, byte thischannel, byte thatlayer, unsigned long thisplace)
{
	// procédure qui va renvoyer la dernière note de valeur thisnote avant l'emplacement thisplace en msec
	// si il y a zéro avant, il faut trouver la dernière en fin de mélodie

	unsigned long highestfound = 0;
	byte lendroit = 0;

	// - - - -  - - - - ON - - - - - OFF - - - -
	//
	unsigned int taille = mesureTaillePileComplete(pInitial[thatlayer]);
	pileNote tempNote;

	for (byte jgg = 0; jgg < taille; jgg++)
	{
		tempNote = ZEROgetNoteFromPile(pInitial[thatlayer], jgg);

		if ((tempNote.note == thisnote) && (tempNote.channel == thischannel) && (tempNote.startNote < thisplace) && (tempNote.startNote > highestfound))
		{
			highestfound = tempNote.startNote;
			lendroit = jgg;
		}
	}

	// si on n'a trouvé aucune note correspondante ayant commencé avant ça veut dire qu'il faut prendre la note correspondante avec le MMTstartnote le plus élevé

	// - - -- - - - - - - OFF - - -- - -  ON - - - -

	if (lendroit == 0)
	{
		highestfound = 0;
		for (byte jgg = 0; jgg < taille; jgg++)
		{
			tempNote = ZEROgetNoteFromPile(pInitial[thatlayer], jgg);

			if ((tempNote.note == thisnote) && (tempNote.channel == thischannel) && (tempNote.startNote > highestfound))
			{
				highestfound = tempNote.startNote;
				lendroit = jgg;
			}
		}
	}

	return lendroit;
}

String createTHXname()
{
	String randString = nulString;
	randString = nulString + letters[random(0, 26)] + letters[random(0, 26)] + letters[random(0, 26)] + F("_") + letters[random(26, 36)] + letters[random(26, 36)] + letters[random(26, 36)] + letters[random(26, 36)] + F(".txt");

	return (randString);
}

void readfromFile(int n)
// lit le nb de bytes spécifiés et met les 2 derniers dans un DOUBLE BYTE !
// NOTE **** remplacer par une fonction qui donne valeur LONG
{
	mididouble = 0;

	if (n == 0)
	{
		return;
	}

	for (byte i = 0; i < n; i++)
	{
		midibyte = myFile.read();
		if (verboseMIDI)
		{
			Serial.print(String(midibyte, HEX));
			Serial.print(" ");
		}
		mididouble = (mididouble << 8) + midibyte;
	}

	if (verboseMIDI)
	{
		Serial.print(">");
		Serial.println(String(mididouble, HEX));
	}
}

void ReadVLVDelta()
{
	// VLVDelta = iVLVDelta;	  // 0;
	// DureeDelta = iDureeDelta; // 0;
	// mididouble = imididouble; // 0;

	VLVDelta = getValueStraightOrFromSequence(iVLVDelta, 0);
	DureeDelta = getValueStraightOrFromSequence(iDureeDelta,0 ) ;
	mididouble = getValueStraightOrFromSequence(imididouble,0); 
	// DEBUG attention on renvoie de l'INT à partir de LONG !!!

	int nowiShiftDelta = getValueStraightOrFromSequence(iShiftDelta, 0);
int nowiShiftDuree = getValueStraightOrFromSequence(iShiftDuree, 0);
int nowiDureeMask= getValueStraightOrFromSequence(iDureeMask, 0);
unsigned int nowiMask2 = getValueStraightOrFromSequence(iMask2, 0);
do
{
	VLVDelta = VLVDelta << nowiShiftDelta; // 8 ;
	readfromFile(1);
	VLVDelta = VLVDelta + mididouble; // cette procédure lit en continu ; en parallèle une autre calculera directement la valeur du VLV

	DureeDelta = DureeDelta << nowiShiftDuree;				 // 7 ;
	DureeDelta = DureeDelta + (mididouble & nowiDureeMask);	 // B01111111 ) ;
															 // } while (  ( mididouble & iMask2  ) == iMask2 && (myFile.available() == true) ) ; // tant que le bit 7 est 1 il faut continuer à lire.
	} while ((mididouble & nowiMask2) == nowiMask2);				 // tant que le bit 7 est 1 il faut continuer à lire.

	if (specialDebug)
	{
		Serial.print(F("["));
		Serial.print(VLVDelta, HEX);
		Serial.print(F("]"));
	}
}

void midiplayerdelay()
{

	/// delay ( long ((( DureeDelta * 60000  *  currentKnob2 ) / calculatedBPM ) / NbTicksQuarterNote) / 50  );
	//                                          100             120               25                  50

	if ((currentKnob2 > 10) && (DureeDelta != 0))
	{
		delay(long((float)DureeDelta * (float)currentKnob2) / (float)500);
		//  delay ( long ((( (float) DureeDelta * (float) 60000  *  (float) currentKnob2 ) / (float) calculatedBPM ) / (float) NbTicksQuarterNote) / (float) 50  );
	}
}

void verboseSerial(byte cmd, byte pitch, byte velocity)
{

	if (( verboseSerial_sendToSerial && verboseSerial_string ) == false)
	{

		if (verboseSerial_timeStamp)
		{
			Serial.print(nowMillis);
		}

		if (verboseSerial_OrderGiven )
		{
			Serial.print(cmd & B11110000);
		}

		if (verboseSerial_sendChannel)
		{
			Serial.print(cmd & B00001111);
		}

		if ((verboseSerial_sendNotes == true) && (cmd & B11110000) == 0x90)
		{
			Serial.print(pitch);
		}

		if ((verboseSerial_sendNotes == true) && (cmd & B11110000) == 0x80)
		{
			Serial.print(pitch);
		}

		if ((verboseSerial_sendCC == true) && (cmd & B11110000) == 0xB0)
		{
			Serial.print(pitch);
		}

		if ((verboseSerial_velocity == true) && (cmd & B11110000) == 0x90)
		{
			Serial.print(velocity);
		}

		if ((verboseSerial_valueCC == true) && (cmd & B11110000) == 0xB0)
		{
			Serial.print(velocity);
		}
	}

	if (( verboseSerial_sendToSerial && verboseSerial_string ) == true)
	{
		if (verboseSerial_timeStamp)
		{
			Serial.print(String(nowMillis));
			Serial.print(" ");
		}

		if (verboseSerial_OrderGiven)
		{
			Serial.print(String(cmd & B11110000));
			Serial.print(" ");
		}

		if (verboseSerial_sendChannel)
		{
			Serial.print(String(cmd & B00001111));
			Serial.print(" ");
		}

		if ((verboseSerial_sendNotes == true) && (cmd & B11110000) == 0x90)
		{
			Serial.print(String(pitch));
			Serial.print(" ");
		}

		if ((verboseSerial_sendNotes == true) && (cmd & B11110000) == 0x80)
		{
			Serial.print(String(pitch));
			Serial.print(" ");
		}

		if ((verboseSerial_sendCC == true) && (cmd & B11110000) == 0xB0)
		{
			Serial.print(String(pitch));
			Serial.print(" ");
		}

		if ((verboseSerial_velocity == true) && (cmd & B11110000) == 0x90)
		{
			Serial.print(String(velocity));
			Serial.print(" ");
		}

		if ((verboseSerial_valueCC == true) && (cmd & B11110000) == 0xB0)
		{
			Serial.print(String(velocity));
			Serial.print(" ");
		}

		Serial.println();
	}

	if (verboseSerial_sendToLCD)
	{
		buffer_and_HARD_setCursor(0, 5);

		if (verboseSerial_sendChannel)
		{
			buffer_and_HARD_print(String(cmd & B00001111));
		}

		if ((verboseSerial_sendNotes == true) && (cmd & B11110000) == 0x90)
		{
			buffer_and_HARD_print(String(pitch));
		}

		if ((verboseSerial_sendCC == true) && (cmd & B11110000) == 0xB0)
		{
			buffer_and_HARD_print(String(pitch));
		}

		if ((verboseSerial_velocity == true) && (cmd & B11110000) == 0x90)
		{
			buffer_and_HARD_print(String(velocity));
		}

		if ((verboseSerial_valueCC == true) && (cmd & B11110000) == 0xB0)
		{
			buffer_and_HARD_print(String(velocity));
		}
	}
}

void PlaynoteOn_(byte cmd, byte pitch, byte velocity)
{

	if (shutUP)
	{
		return;
	}

	byte cmd_isolee = cmd & B11110000;

	// 2C29 impossible !!!   ne fonctionne pas pour une commande combinée, car 1111 vaut 16 !
	//	if ((cmd & B00001111) > 15)
	//	{
	//		return;
	//	} // çàd concerne un channel > 16 = MUTED THINGS

if ( cmd_isolee == 0x10 ) // DMX
{
    byte channel = cmd & B00001111 ; 
	int intCH = ((( int) velocity ) << 8 ) + ( (int) channel )  ;
	int intNT = (int) pitch ;

	dmxTx.set(intCH, intNT);
	return ; 
}

	if (cmd_isolee == 0x90)
	{
		if (checkNoteAChannelBinProxyBlockPiles(pitch, cmd & B00001111) == true)
		{

			// OFF ed ?
			// YES
			// NO

			// unsigned long lastStart = getTimeValue_from_NoteAChannelBinProxyBlockPiles ( pitch, cmd & B00001111 ) ;

			// if ( ( nowMillis - lastStart ) < proxyPitch ) { pitch += proximityPitch  ; }

			deleteNoteAChannelBinProxyBlockPiles(pitch, cmd & B00001111);
			pileNote tempNote;
			tempNote.note = pitch;
			tempNote.noteReallyPlayed = pitch;
			tempNote.velocity = velocity;
			tempNote.channel = cmd & B00001111;
			tempNote.layer = 0;
			tempNote.killnote = false;
			tempNote.startNote = nowMillis;
			tempNote.endNote = tempNote.startNote; // TODO
			tempNote.thereIsAnEnd = false;		   // DIFFERE ICI DU SUSTAIN BLOCK !!!
			tempNote.type = 0x90;
			ZEROaddNewNotetoPile(pProxyBlock, tempNote);
		}
		else
		{
			pileNote tempNote;
			tempNote.note = pitch;
			tempNote.noteReallyPlayed = pitch;
			tempNote.velocity = velocity;
			tempNote.channel = cmd & B00001111;
			tempNote.layer = 0;
			tempNote.killnote = false;
			tempNote.startNote = nowMillis;
			tempNote.endNote = tempNote.startNote; // TODO
			tempNote.thereIsAnEnd = false;		   // DIFFERE ICI DU SUSTAIN BLOCK !!!
			tempNote.type = 0x90;
			ZEROaddNewNotetoPile(pProxyBlock, tempNote);
		}
	}

	if (cmd_isolee == 0x80)
	{
		if (checkNoteAChannelBinProxyBlockPiles(pitch, cmd & B00001111) == true)
		{
			updateNoteAChannelBinProxyBlockPiles(pitch, cmd & B00001111);
		}
		else
		{
			// BIZARRE : on a une note END sans START... inquiétant mais pas impossible dans ce monde polyrythmique... TODO DEBUG
		}
	}

	if (cmd_isolee == 0x90)
	{
		pileNote tempNote;
		tempNote.note = pitch;
		tempNote.noteReallyPlayed = pitch;
		tempNote.velocity = velocity;
		tempNote.channel = cmd & B00001111;
		tempNote.layer = 0;
		tempNote.killnote = false;
		tempNote.startNote = nowMillis;
		tempNote.endNote = tempNote.startNote; // TODO
		tempNote.thereIsAnEnd = true;
		tempNote.type = 0x90;
		ZEROaddNewNotetoPile(pSustainBlock, tempNote);
	}

	if (cmd_isolee == 0x80)
	{
		if (checkNoteAChannelBinSustainBlockPiles(pitch, cmd & B00001111) == true)
		{
			// unsigned long timeStart ;
			// timeStart = getTimeValue_from_NoteAChannelBinSustainBlockPiles(pitch, cmd & B00001111);
			// if ( ( sustainBlockerON == true ) && (( nowMillis - timeStart )  > sustainBlockLimit) )
			{
			}
			deleteNoteAChannelBinSustainBlockPiles(pitch, cmd & B00001111);
		}
	}

	Serial3.write(cmd);
	Serial3.write(pitch);
	Serial3.write(velocity);

	verboseSerial(cmd, pitch, velocity);

	if ( ( ( statusFluxRecIn == 1 ) || ( statusFluxRecIn == 2 ) ) && (cmd_isolee) == 0x90)
	// TODO DEBUG : il faut prévoir un enregistrement des note OFF aussi dans le flux !!!
	{
		int taille = mesureTaillePileComplete(pInitial[pFluxStart + currentFluxMem]);

		if (taille < limitFlux)
		{
			// on enregistre le flux NOTE ON
			pileNote tempNote;
			tempNote.note = pitch;
			tempNote.velocity = velocity;
			tempNote.channel = cmd & B00001111;
			tempNote.layer = 0;
			tempNote.killnote = false;
			tempNote.startNote = nowMillis;
			tempNote.endNote = tempNote.startNote - 1 ; // TODO
			tempNote.thereIsAnEnd = true;
			tempNote.type = 0x90;

			// devra sans doute être mis au point
			// pileNote_fixStartEndNote(tempNote, 0);
			ZEROaddNewNotetoPile(pInitial[pFluxStart + currentFluxMem], tempNote);
		}
		else
		{
			statusFluxRecIn = 0; // on n'enregistre pas trop
		}
	}

	if (statusFluxRecIn == 2 && (cmd_isolee) == 0x80)
	// AVEC NOTE OFF 
	{
		int taille = mesureTaillePileComplete(pInitial[pFluxStart + currentFluxMem]);

// on doit chercher la note dans cette pile qui a CH et NOTE idem, en 0x90, la dernière en temps

int highest = -1 ; // valeur au départ négative, si on trouve un note qui convient, prendra la valeur de la dernière ; 

		for ( int j = 0; j < taille; j++)
		{

			pileNote tempNote;
			tempNote = ZEROgetNoteFromPile(pInitial[pFluxStart + currentFluxMem], j);


			if (tempNote.channel == ( cmd & B00001111 ) && tempNote.note == pitch && tempNote.type == 0x90)  
			{
				highest = j ; 
			}
		}

		if ( highest != - 1 )
		{
			pileNote tempNote;
			tempNote = ZEROgetNoteFromPile(pInitial[pFluxStart + currentFluxMem], highest );
			tempNote.endNote = nowMillis; 
					ZEROSetNoteInPile(pInitial[pFluxStart + currentFluxMem], highest, tempNote);
		}

	}
}

void PlaynoteOn2(byte cmd, byte pitch, byte velocity, byte layer, int dureeNote = 0)
{

	if (shutUP)
	{
		return;
	}

	/** 2.0 version du PlayNote où on passe aussi la valeur de layer et midi //
	// si layer vaut B10000000, alors il s'agit de Midi parallel
	*/
	// Serial3.write(cmd);
	// Serial3.write(pitch);
	// Serial3.write(velocity);
	//
	// version historique du PlayNote

	// avant de jouer une note on va devoir inspecter plusieurs String :
	// PatternONOFF 1-4
	// PatternONOFFChannel 1-16 pas pour l'instant
	// PatternOctaver 1-4
	// PatternCHDistribute 1-4
	// PatternVelocite 1-4
	// PatternDismorphophobia 1-4
	// DONE Array NoteFregoli 1-12 : envoie une note vers une autre
	// DONE Array NoteCotard 1-12 : ne joue pas une note
	//
	// qd joue ( ou aurait du jouer ) une note d'un layer : compteur du layer + 1
	// d'emblée on peut prévoir un compteur idem pour chaque channel ! +1
	//
	// on
	// patternSwing.substring(iAB % patternSwing.length(),iAB % patternSwing.length() + 1 ) ;
	// devient
	// patternxxx.substring(compteur % patternxxx.length(),compteur % patternxxx.length() + 1 ) ;

	// 1 isoler la commande et le channel

	byte cmd_isolee = cmd & B11110000;
	byte channel_isole = cmd & B00001111;

	if (cmd_isolee == 0x10) // DMX
	{
		 
	int intCH =( (( int) velocity ) << 8 ) + ( (int) channel )  ;
	int intNT = (int)pitch;

	dmxTx.set(intCH, intNT);
	return;
	}

	if (channel_isole > 15)
	{
		return;
	} // çàd concerne un channel > 16 = MUTED THINGS
	// NE SERA PLUS VRAI POUR LA VERSION à 2 MIDI OUT !!! TODO TODO TODO 

	// !!!!!!!!!!!!!!!!!!!!!!!!!!

	// UTILISE pour le Mute channel : CHMute >> 1 à 16 décrit si un ch est ON (0) ou MUTE (1)
	// on compare ici si la valeur du channel actuel est mute ou pas ; si oui on arrête les frais de suite
	if ((CHMute >> (byte)(channel_isole)&1) != 0)
	{
		return;
	}

	// TODO : peut être limiter les redirections Matrix Midi et CC Mod au 0x80 et 0x90 ?

// 22C01 6 mars 2022, en préparation de Django à l'An Vert 

	if (layer == B10000000)
	{ // çàd on joue une note du MidiParallel
int newPitch = (int) pitch + (int) transposeMIDIPlayer ;
pitch = limiter(newPitch,0,127); 
	}

		// if (FunkyVel == true && cmd_isolee == 0x90) // 2C27
	if ( cmd_isolee == 0x90) // SEPT 2025
	{
			signed int funk(0);

			signed int add = random(0, FunkyVelValue[layer]); // est la valeur absolue qui peut être ajoutée à une vélocité

			if (layer == B10000000)
			{ // çàd on joue une note du MidiParallel
				add = random(0, getValueStraightOrFromSequence(FunkyVelValueMIDIPlayer, 0));
			}

			if (random(2) == 0)
			{
				funk = (int)velocity + add;
			}
			else
			{
				funk = (int)velocity - add;
			}

			funk = limiter(funk, 0, 126); // va limiter entre 0 et 127 ;
			velocity = funk;
		}
		else
		{
			velocity = limiter(velocity, 0, 126);
		}

		for (byte j = 0; j < 16; j++)
		{
			if (isONmidiMatrix(midiRedirect[channel_isole], j))
			{
				{
					byte pitchRoot = pitch % 12; // on donne la valeur de note ( C, D, etc ) du pitch

					if (
						pitch > dynamicLOcut && pitch < dynamicHIcut && !isSilentSByte(noteRedirect[pitchRoot]))
					{
						if (CCModActif)
						{
							for (byte z = 0; z < NbCCModMax; z++)
							{
								// pileCCMod* CCModPtr ;
								// pileCCMod* NextCCModPtr ;

								// CCModPtr = CCModPointeurInitial ;
								// bool keepOn = false ;

								// do {

								if (CCModTableau[z].actif)
								{
									// ex: on a 00001100 : ca veut dire envoyer CC si layer actuel est 4 ou 3

									byte k = CCModTableau[z].layerConcerne;

									if (k == 16)
									{
										// est-ce que layer est dans activeLayer ?

										k = ((activeLayer >> layer) & B00000001);
									}
									else
									{
										// est-ce que layer est un layer compris dans CCModTableau[z].actif ?
										k = (k >> layer) & B00000001;
									}

									if (k == B00000001)
									{
										unsigned int x;
										byte y = 0;
										bool send = true;

										switch (CCModTableau[z].type)
										{
										case (0):
											x = 0;
											break;
										case (1):
											x = velocity;
											break;
										case (2):
											x = pitch;
											break;
										case (3):
											x = pitch % 12;
											break; // = une NOTE do ré mi fa
										case (4):
											x = dureeNote;
											break;
										default:
											x = 0;
											break;
										}
										//  byte type ; // 0 = vide   1 = VELOCITY  2 = PITCH   3 = NOTE   4= LG

										switch (CCModTableau[z].borders)
										{
										case (0): // limite
										{
											y = ultraMap(x, CCModTableau[z].sourceMin, CCModTableau[z].sourceMax, CCModTableau[z].targetMin, CCModTableau[z].targetMax, 127.0);
											y = limiter(y, 0, 127);
											send = true;
										};
										break;
										case (1): // expand
										{
											y = map(x, CCModTableau[z].sourceMin, CCModTableau[z].sourceMax, CCModTableau[z].targetMin, CCModTableau[z].targetMax);
											y = limiter(y, 0, 127);
											send = true;
										};
										break;
										case (2): // no send
										{
											y = map(x, CCModTableau[z].sourceMin, CCModTableau[z].sourceMax, CCModTableau[z].targetMin, CCModTableau[z].targetMax);
											y = limiter(y, 0, 127);
											send = true;
											if (y > CCModTableau[z].targetMax)
											{
												send = false;
											}
											if (y < CCModTableau[z].targetMin)
											{
												send = false;
											}
										};
										break;
										case (3): // black hole (invert)
										{
											y = map(x, CCModTableau[z].sourceMin, CCModTableau[z].sourceMax, CCModTableau[z].targetMin, CCModTableau[z].targetMax);
											y = limiter(y, 0, 127);
											send = false;
											if (y > CCModTableau[z].targetMax)
											{
												send = true;
											}
											if (y < CCModTableau[z].targetMin)
											{
												send = true;
											}
										};
										break;

										default: //
										{
											y = 0;
										};
										break;
										}

										// byte borders ; // 0 = limite    1 = expand     2 = No Send audelà et endeça des valeurs min et max   3 = invert : envoie au-delà mais pas entre !

										// On balance ici le CC

										if (send)
										{
											byte cmd = 0xB0 + CCModTableau[z].channelCC; // on encode bien un channel MACHINE
											byte arg2 = CCModTableau[z].idCC;
											byte arg3 = y;

											Serial3.write(cmd);
											Serial3.write(arg2);
											Serial3.write(arg3);

											verboseSerial(cmd, arg2, arg3);
										}
									}
								}

								// if ( (*CCModPtr).nextCCMod != 0 )
								// {
								// 	keepOn = true ;
								// 	CCModPtr = (*CCModPtr).nextCCMod ;
								// }
								// else
								// {
								// 	keepOn = false ;
								// }
								// }							while ( keepOn ) ;

								// OK commencer en début de pile CCMod
								// OK parcourir jusqu'à ce que next = 0
								// OK voir ce qui est le x, tel que y = f(x)  // pitch, note, Vélocité
								// OK créer fonction qui donne  y avec le map spécial
								// OK si le layer est actif pour CCMod ( mettre au début )
								// balancer ça en 0xB0 + channel
							}
						}

						// pitch = (byte) limiter(pitch+valueSByte(noteRedirect[pitchRoot]),0,127);
						pitch = (byte)(pitch + valueSByte(noteRedirect[pitchRoot]));
						// pitch = pitch & B01111111 ;
						pitch = limiterForPitch(pitch);

						// on vérifie si pour cette layer il y a un dynamicHarmonize ON
						// si oui, la valeur de pitch est modifée en conséquence

						// 2C25 APRES DES MOIS DE RECHERCHE J'AI ENFIN TROUVE ICI LE BIDULE QUI FAIT FOIRER LE PITCH ET FAIT DES NOTES SUSTAIN !!!!
						// Ceci ne peut se trouver ici, car on ne sait pas quelle sera la note OFF à jouer pour éteindre cette note-ci !
						// dans playNOte il ne peut y avoir aucune modif de pitch !!!

						// if (harmonizeLayer[layer] && pitchableLayer[layer] == true && pitchableChannel[channel_isole] == true)
						// {
						// 	pitch = dynamicHarmonize(pitch, layer); // seul lieu où cette fonction est utilisée.  à priori OK pour l'unpitchable
						// }

						// if (vel2Vol[layer] == true)   // 1C98
						if (vel2Vol[channel_isole] == true) // 1C98
						{
							// TODO DEBUG ne faut il pas vérifier layer et channel ?
							//  F0 42 30 7F 41 SN 00 18 00 DV 00 F7 Level 0..127(center 100)

							byte valueNote = pitch;
							byte valueVelocity = velocity;

							Serial3.write(0xF0);
							Serial3.write(0x42);
							Serial3.write(0x30);
							Serial3.write(0x7F);
							Serial3.write(0x41);
							Serial3.write(valueNote - (48 - 16));

							Serial3.write(0x00);
							Serial3.write(0x18);
							Serial3.write(0x00);
							Serial3.write(valueVelocity);
							Serial3.write(0x00);
							Serial3.write(0xF7);

							// OK pour les notes du clavier avec samples
							// la valeur de SN, de sample n'est pas égale à la valeur de note
							// SN est numéroté de 16 à 51, alors que les notes commencent à 48 !

							// buffer_and_HARD_setCursor(5,5);
							// buffer_and_HARD_print(valueVelocity);
						}

						if ((panMicroSamplerOnLayer[layer] == true) && (panMicroSamplerOnChannel[channel_isole] == true))
						{
							//  F0 42 30 7F 41 SN 00 18 00 DV 00 F7 Level 0..127(center 100)
							// F0 42 30 7F 41 SN 00 19 00 DV 00 F7 Pan 0..127 (center 63)
							byte valueNote = pitch;
							// byte valueVelocity = velocity;
							int modif;

							if (panMicroSampler[layer] >= 0)
							{
								modif = random(panMicroSampler[layer] + 1);

								if (random(2) == 0)
								{
									modif = -modif;
								}
							}
							else
							{
								modif = giveNextValue(sequence[-panMicroSampler[layer] - 1], sequence[-panMicroSampler[layer] - 1].compteur);
								sequence[-panMicroSampler[layer] - 1].compteur++;
							}

							Serial3.write(0xF0);
							Serial3.write(0x42);
							Serial3.write(0x30);
							Serial3.write(0x7F);
							Serial3.write(0x41);
							Serial3.write(valueNote - (48 - 16));

							Serial3.write(0x00);
							Serial3.write(0x19);
							Serial3.write(0x00);
							Serial3.write((byte)(63 + modif));
							Serial3.write(0x00);
							Serial3.write(0xF7);

							// OK pour les notes du clavier avec samples
							// la valeur de SN, de sample n'est pas égale à la valeur de note
							// SN est numéroté de 16 à 51, alors que les notes commencent à 48 !

							// buffer_and_HARD_setCursor(5,5);
							// buffer_and_HARD_print(valueVelocity);
						}

						if ((pitchMicroSamplerOnLayer[layer] == true) && (pitchMicroSamplerOnChannel[channel_isole] == true))
						{
							//  F0 42 30 7F 41 SN 00 18 00 DV 00 F7 Level 0..127(center 100)
							// F0 42 30 7F 41 SN 00 19 00 DV 00 F7 Pan 0..127 (center 63)
							// F0 42 30 7F 41 SN 00 1C 00 DV 00 F7 Tune 0..127 (center 63)
							int modif = 0;

							byte valueNote = pitch;
							// byte valueVelocity = velocity;
							if (pitchMicroSampler[layer] >= 0)
							{
								modif = random(pitchMicroSampler[layer] + 1);

								if (random(2) == 0)
								{
									modif = -modif;
								}
							}
							else
							{
								modif = giveNextValue(sequence[-pitchMicroSampler[layer] - 1], sequence[-pitchMicroSampler[layer] - 1].compteur);
								sequence[-pitchMicroSampler[layer] - 1].compteur++;
							}

							Serial3.write(0xF0);
							Serial3.write(0x42);
							Serial3.write(0x30);
							Serial3.write(0x7F);
							Serial3.write(0x41);
							Serial3.write(valueNote - (48 - 16));

							Serial3.write(0x00);
							Serial3.write(0x1B);
							Serial3.write(0x00);
							// F0 42 30 7F 41 SN 00 1B 00 DV DV F7 Semitone -24..24 (68 7F .. 18 00)
							if (modif >= 0)
							{
								Serial3.write((byte)(modif));
								// Serial3.write((byte)(63 + modif));
								Serial3.write(0x00);
							}
							else
							{
								Serial3.write((byte)(128 + modif));
								Serial3.write(0x7F);
							}
							Serial3.write(0xF7);

							// OK pour les notes du clavier avec samples
							// la valeur de SN, de sample n'est pas égale à la valeur de note
							// SN est numéroté de 16 à 51, alors que les notes commencent à 48 !

							// buffer_and_HARD_setCursor(5,5);
							// buffer_and_HARD_print(valueVelocity);
						}

						// 2C29 suppression du Derazer, trop specifique et ne marche d'ailleurs pas !!!!
						if (cmd_isolee == 0x90)
						{
							if (checkNoteAChannelBinProxyBlockPiles(pitch, cmd & B00001111) == true)
							{

								// OFF ed ?
								// YES
								// NO

								//	unsigned long lastStart = getTimeValue_from_NoteAChannelBinProxyBlockPiles(pitch, cmd & B00001111);
								// Serial.println(nowMillis-lastStart);

								// if ((nowMillis - lastStart) < proxyPitch)
								{
									//	pitch += proximityPitch ;
								}

								deleteNoteAChannelBinProxyBlockPiles(pitch, cmd & B00001111);
								pileNote tempNote;
								tempNote.note = pitch;
								tempNote.noteReallyPlayed = pitch;
								tempNote.velocity = velocity;
								tempNote.channel = channel_isole;
								tempNote.layer = 0;
								tempNote.killnote = false;
								tempNote.startNote = nowMillis;
								tempNote.endNote = tempNote.startNote; // TODO
								tempNote.thereIsAnEnd = false;		   // DIFFERE ICI DU SUSTAIN BLOCK !!!
								tempNote.type = 0x90;
								ZEROaddNewNotetoPile(pProxyBlock, tempNote);
							}
							else
							{
								pileNote tempNote;
								tempNote.note = pitch;
								tempNote.noteReallyPlayed = pitch;
								tempNote.velocity = velocity;
								tempNote.channel = channel_isole;
								tempNote.layer = 0;
								tempNote.killnote = false;
								tempNote.startNote = nowMillis;
								tempNote.endNote = tempNote.startNote; // TODO
								tempNote.thereIsAnEnd = false;		   // DIFFERE ICI DU SUSTAIN BLOCK !!!
								tempNote.type = 0x90;
								ZEROaddNewNotetoPile(pProxyBlock, tempNote);
							}
						}

						if (cmd_isolee == 0x80)
						{
							if (checkNoteAChannelBinProxyBlockPiles(pitch, cmd & B00001111) == true)
							{
								updateNoteAChannelBinProxyBlockPiles(pitch, cmd & B00001111);
							}
							else
							{
								// BIZARRE : on a une note END sans START... inquiétant mais pas impossible dans ce monde polyrythmique... TODO DEBUG
							}
						}

						if (cmd_isolee == 0x90)
						{
							pileNote tempNote;
							tempNote.note = pitch;
							tempNote.noteReallyPlayed = pitch;
							tempNote.velocity = velocity;
							tempNote.channel = channel_isole;
							tempNote.layer = 0;
							tempNote.killnote = false;
							tempNote.startNote = nowMillis;
							tempNote.endNote = tempNote.startNote; // TODO
							tempNote.thereIsAnEnd = true;
							tempNote.type = 0x90;
							ZEROaddNewNotetoPile(pSustainBlock, tempNote);
						}

						if (cmd_isolee == 0x80)
						{
							if (checkNoteAChannelBinSustainBlockPiles(pitch, cmd & B00001111) == true)
							{
								// unsigned long timeStart ;
								// timeStart = getTimeValue_from_NoteAChannelBinSustainBlockPiles(pitch, cmd & B00001111);
								// if ( ( sustainBlockerON == true ) && (( nowMillis - timeStart )  > sustainBlockLimit) )
								{
								}
								deleteNoteAChannelBinSustainBlockPiles(pitch, cmd & B00001111);
							}
						}

						Serial3.write(cmd_isolee + j);
						Serial3.write(pitch);
						Serial3.write(velocity);

						if (DMXautoGenerate == true)
						{
							generateDMX(0, cmd_isolee + j, pitch, velocity);
						}

						verboseSerial(cmd_isolee + j, pitch, velocity);

						if (((statusFluxRecIn == 1) || (statusFluxRecIn == 2)) && (cmd_isolee) == 0x90)
						{
							int taille = mesureTaillePileComplete(pInitial[pFluxStart + currentFluxMem]);

							if (taille < limitFlux)
							{
								// on enregistre le flux NOTE ON
								pileNote tempNote;
								tempNote.note = pitch;
								tempNote.velocity = velocity;
								tempNote.channel = j;
								tempNote.layer = 0;
								tempNote.killnote = false;
								tempNote.startNote = nowMillis;
								tempNote.endNote = tempNote.startNote + dureeNote;
								tempNote.thereIsAnEnd = true;
								tempNote.type = 0x90;

								// devra sans doute être mis au point
								// pileNote_fixStartEndNote(tempNote, 0);
								ZEROaddNewNotetoPile(pInitial[pFluxStart + currentFluxMem], tempNote);
							}
							else
							{
								statusFluxRecIn = 0; // on n'enregistre pas trop
							}
						}

						if (statusFluxRecIn == 2 && (cmd_isolee) == 0x80)
						// AVEC NOTE OFF
						{
							int taille = mesureTaillePileComplete(pInitial[pFluxStart + currentFluxMem]);

							// on doit chercher la note dans cette pile qui a CH et NOTE idem, en 0x90, la dernière en temps

							int highest = -1; // valeur au départ négative, si on trouve un note qui convient, prendra la valeur de la dernière ;

							for (  int j = 0; j < taille; j++)
							{

								pileNote tempNote;
								tempNote = ZEROgetNoteFromPile(pInitial[pFluxStart + currentFluxMem], j);

								if (tempNote.channel == (cmd & B00001111) && tempNote.note == pitch && tempNote.type == 0x90)
								{
									highest = j;
								}
							}

							if (highest != -1)
							{
								pileNote tempNote;
								tempNote = ZEROgetNoteFromPile(pInitial[pFluxStart + currentFluxMem], highest);
								tempNote.endNote = nowMillis;
								ZEROSetNoteInPile(pInitial[pFluxStart + currentFluxMem], highest, tempNote);
							}
						}

						TotalNoteCounter++;

						// TODO DEBUG il faudrait aussi un compteur de notes par layer, pour Eclipse Note
					}
				}
			}
		}
	}

// AJOUTER LE CC Mod
// à quel endroit ?  avant redirection MIDI ou après ?
// parcourir la liste des CCMod en commençant à la première
// voir si remplit les conditions : Layer ? Midi Parallel ? (info doit être passée dans PlayNote)
// si OK, prendre valeur initiale : note, vélocité, pitch
// la transformer en fc du type de courbe : map, ultramap, void, limit, etc
// si on n'a pas No SEND : on envoie un CC au channel, valeur ad hoc

// typedef struct CCModule CCModule ;
// struct CCModule {
//     byte type ; // 0 = vide   1 = VELOCITY  2 = PITCH   3 = NOTE
//     byte sourceMin ; // 0 à 127 : vélocité, pitch, note (0-12)
//     byte sourceMax ; // 0 à 127
//     byte targetMin ; // 0 à 127 : valeur du CC 0x Bn 103 Valeur
//     byte targetMax ; // 0 à 127 : valeur du CC 0x Bn 103 Valeur
//     byte borders ; // 0 = limite    1 = expand     2 = No Send audelà et endeça des valeurs min et max   3 = invert : envoie au-delà mais pas entre !
//     byte idCC ; // définit le CC auquel on envoie la valeur : ex : 103 pour EditA sur Behringer2024
//     byte channelCC ; // définit sur quel Channel n on doit envoyer l'info CC : 0x Bn 103 Valeur
//     byte layerConcerne ; // définit quel layer sont concernés : 00004321
//                           // attention : 0001 0000 = concerne le Midi parallel player
// } ;
//
// typedef struct pileCCMod pileCCMod ;
// struct pileCCMod {
//   CCModule CCMod ;
//   int numeroPile ;   // pourra disparaître
//   pileCCMod* nextCCMod ;
// } ; // j'ai pas oublié cette fois !
//
// int taillePileCCMod (0);

// ************************************************************************************************

// PLAY MIDI FILE  SEQUENCE

// à terme il faudrait remplacer playmidifile1, Definemidifile1 et LoadMIDInotesfromFile, qui partagent la même structure par une procédure unique
// en argument on pourrait mettre ce qu'on attend d'elle : jouer un morceau, en calculer la taille ou charger certaines notes
// à faire quand le midi parser sera tout à fait abouti ?

// ************************************************************************************************

////////////////////////////////////////////////////////////////////////////////////////////////////

// LOOOAAADDDD SEQUENCE

///////////////////////////////////////////////////////////////////////////////////////////////////

void LoadMIDINotesFromFile(unsigned int StartNote)
{ // Startnote indique la note à partir de laquelle on commence à enregistrer dans la mémoire
	// le nb de notes à enregistrer est défini par la fc 86 : midiLongueurFgt
	// cette fonction est basée sur le MidiPlay1, version basique du midi Play.

	// on va aller jusqu'au moment où nbNotesOn = Startnote avant de les enregistrer

	nbNotesOn = 0;
	nbNotesOff = 0;
	totalDureeDelta = 0;
	calculatedBPM = 120; // par défaut si n'est pas spécifié

	// open the file. note that only one file can be open at a time,
	// so you have to close this one before opening another.

	if (myFile)
	{
		myFile.close();
	}

	myFile = SDcard.open(midiname, FILE_READ);

	// OPEN the HEader

	readfromFile(4);
	readfromFile(4);
	// threadlen = mididouble;
	readfromFile(2);
	readfromFile(2);

	// NbChunk = int( mididouble ) ;

	// faire un premier passage qui repère les différents chunks et les explicite ?

	readfromFile(2);
	// timedivision = ( mididouble >> 15 );
	// if ( timedivision == 0 ) {
	//     //   NbTicksQuarterNote = mididouble ;
	//     // value1000ticks = long ( (float) 6000000 / ( (float) 4.0 * (float) 12.0 * (float) mididouble ) );
	// } // else {
	// SMPTE = (( mididouble >> 8 ) & B01111111 );
	// resolution = (( mididouble << 8 ) >> 8 ) ;
	// }

	//  readfromFile(threadlen - 6);

	//  CurrentMIDIChunk = 0;
	EndofTrack = false;
	FillUpEnoughNotes = false;
	bool firstreached = false;

	do
	{
		CurrentMIDIChunk++;
		readfromFile(4);
		readfromFile(4);

		do
		{
			ReadVLVDelta(); // renvoie info dans DureeDelta et VLVDelta

			totalDureeDelta = totalDureeDelta + DureeDelta;

			readfromFile(1);

			exact1stbyte = mididouble & B11111111;
			byte midicategory = (mididouble & B11110000);
			byte midichannel = (mididouble & B00001111);

			int nowNumRatioDeltaToMsec = getValueStraightOrFromSequence(NumRatioDeltaToMsec, 1);
			int nowDenomRatioDeltaToMsec = getValueStraightOrFromSequence(DenomRatioDeltaToMsec, 1);
			if ( nowDenomRatioDeltaToMsec == 0 ) { nowDenomRatioDeltaToMsec++ ; }
			float RatioDeltaToMsec = (float)nowNumRatioDeltaToMsec / (float)nowDenomRatioDeltaToMsec;

			switch (midicategory)
			{
			case (0x90):
			{
				// delay ( long ((( DureeDelta * 60000 ) / calculatedBPM ) / NbTicksQuarterNote) * (100 / 50) );

				nbNotesOn++;
				buffer_and_HARD_setCursor(0, 1);
				buffer_and_HARD_print(nbNotesOn);

				if (nbNotesOn > (StartNote + midiLongueurFgt))
				{
					FillUpEnoughNotes = true;
					buffer_and_HARD_setCursor(0, 0);
					buffer_and_HARD_print(F("FULL  "));
					// delay(1000);
				}

				readfromFile(2);

				// PlaynoteOn( exact1stbyte, byte (mididouble >> 8) , byte (mididouble &  B01111111) );
				if (nbNotesOn >= StartNote)
				{ // Si on a atteint la note de départ on enregistre dans le layer
					if (!firstreached)
					{
						buffer_and_HARD_setCursor(0, 0);
						buffer_and_HARD_print(F("FIRST  "));
						buffer_and_HARD_print(nbNotesOn);
						// delay(1000);
						startmiditime = (long)((float)totalDureeDelta * (float)RatioDeltaToMsec);
						// long (((( (float) totalDureeDelta * (float) 60000 ) / (float) calculatedBPM ) / (float) NbTicksQuarterNote) * ( (float) 100 / (float) 50) ) ;
						firstreached = true;
					}

					buffer_and_HARD_setCursor(0, 0);
					buffer_and_HARD_print(F("REC  "));

					// ********************************

					for (byte i = 0; i < nbLayer; i++)
					{
						if (isLayerActive(i))
						{
							// byte j = firstfreenote(i);
							pileNote tempNote;
							tempNote.note = byte(mididouble >> 8);
							tempNote.velocity = byte(mididouble & B01111111);

							switch (recordMode)
							{
							// il faudra adapter qd on instaurera chord !!!
							case 0:
								tempNote.channel = layerChannel[i]; // çàd les 4 premiers bits = le channel   les 4 suivants sont les données pour chord : 0 = solo
								break;
							case 1:
								tempNote.channel = midichannel; // çàd les 4 premiers bits = le channel   les 4 suivants sont les données pour chord : 0 = solo
								break;
							case 2:
								tempNote.channel = 0; // on change rien
								break;
							default:
								break;
							}
							tempNote.startNote = (long)((float)totalDureeDelta * (float)RatioDeltaToMsec) - (long)startmiditime;

							// GoONmidi = millis()+ (long)    (  (float) DureeDelta   *  (float) RatioDeltaToMsec ) ;
							// NOTE **** revoir ce calcul abracadabrant !!!

							// le temps de la note c'est celui cumulé moins celui au moment de la première valable.

							// vérifier si faut pas timenote DOUBLE
							tempNote.endNote = tempNote.startNote + 10; // sera ensuite précisé avec le note OFF
							tempNote.thereIsAnEnd = true;
							pileNote_fixStartEndNote(tempNote, 0);
							ZEROaddNewNotetoPile(pInitial[i], tempNote);
						}
					}
				}
				// PlaynoteOn( exact1stbyte, byte (mididouble >> 8) , byte (mididouble &  B01111111) );

				lastexact1stbyte = exact1stbyte;

				if (mididouble == 0)
				{
					nbNotesOn--;  // on supprime ce qu'on vient d'ajouter
					nbNotesOff++; // on a une note Off en plus
				}

				lastRunningStatus = 0x90; // on en aura besoin pour compter les notes On et Off

				// 0kkkkkkk
				// 0vvvvvvv
			}
			break;

			case (0x80):
			{
				// delay ( long ((( DureeDelta * 60000 ) / calculatedBPM ) / NbTicksQuarterNote) * (100 / 50) );

				nbNotesOff++;
				readfromFile(2);
				//PlaynoteOn(exact1stbyte, mididouble >> 8, mididouble &  B01111111);

				// il faut trouver la dernière note ajoutée correspondant à la note qui s'arrête !

				for (byte i = 0; i < nbLayer; i++)
				{
					if (isLayerActive(i))
					{
						pileNote tempNote;
						// trouver la dernière note ajoutée qui partage la même NOTE - PAS j = firstfreenote(i);
						// melodyMatrix[i][j][MMnote] = byte (mididouble >> 8) ;
						byte j = lastnotebeforetime(mididouble >> 8, midichannel, i, (long)((float)totalDureeDelta * (float)RatioDeltaToMsec) - (long)startmiditime); // renvoie le n° de la note à laquelle nous devons mettre fin
																																									  // NOTE **** revoir ce calcul abracadrant !!!!
																																									  //
																																									  // melodyMatrixTime[i][j][MMTstartnote] = long ((( totalDureeDelta * 60000 ) / calculatedBPM ) / NbTicksQuarterNote) * (100 / 50) )  - startmiditime ;
																																									  // le temps de la note c'est celui cumulé moins celui au moment de la première valable.
						tempNote = ZEROgetNoteFromPile(pInitial[i], j);

						tempNote.endNote = (long)((float)totalDureeDelta * (float)RatioDeltaToMsec) - startmiditime;
						pileNote_fixStartEndNote(tempNote, 0);
						ZEROaddNewNotetoPile(pInitial[i], tempNote);
					}
				}

				lastRunningStatus = 0x80; // on en aura besoin pour compter les notes On et Off
				lastexact1stbyte = exact1stbyte;
			}
			break;

			case (0xA0):
			{
				readfromFile(2);
			}
			break;

			case (0xB0):
			{
				readfromFile(2);
			}
			break;

			case (0xC0):
			{
				readfromFile(1);
			}
			break;

			case (0xD0):
			{
				readfromFile(1);
			}
			break;

			case (0xE0):
			{
				readfromFile(2);
			}
			break;

			case (0xF0):
			{
				exact1stbyte = mididouble;
				readfromFile(1);
				exact2ndbyte = mididouble;

				if (exact1stbyte == 0xFF)
				{
					switch (exact2ndbyte)
					{
					case (0x2F):
						EndofTrack = true;
						break;
					}
					readfromFile(1);
					readfromFile(mididouble); // va lire le nb de bytes précisés juste au dessus
					if (exact2ndbyte == 0x51)
					{
						calculatedBPM = (int)((float)60000000 / (float)mididouble);
					}
				}
			}
			break;

			default: // Running Status ???

				specialmididouble = mididouble;
				readfromFile(1);
				mididouble = (specialmididouble << 8) + mididouble;

				// si le dernier était un 0x90 alors on a NoteOn++, sauf si vélocité = 0 ;
				// si le dernier était un 0x80 alors on a NotesOff++

				switch (lastRunningStatus)
				{
				case (0x80):
					nbNotesOff++;
					//
					// On( lastexact1stbyte, byte (mididouble >> 8) , byte (mididouble &  B01111111) );

					for (byte i = 0; i < nbLayer; i++)
					{
						if (isLayerActive(i))
						{
							pileNote tempNote;
							// trouver la dernière note ajoutée qui partage la même NOTE - PAS j = firstfreenote(i);
							// melodyMatrix[i][j][MMnote] = byte (mididouble >> 8) ;
							byte j = lastnotebeforetime(mididouble >> 8, midichannel, i, (long)((float)totalDureeDelta * (float)RatioDeltaToMsec) - (long)startmiditime);
							//long (((( (float) totalDureeDelta * (float) 60000 ) / (float) calculatedBPM ) /(float) NbTicksQuarterNote) * ((float) 100 / (float) 50) )  - startmiditime); // renvoie le n° de la note à laquelle nous devons mettre fin

							// melodyMatrixTime[i][j][MMTstartnote] = long ((( totalDureeDelta * 60000 ) / calculatedBPM ) / NbTicksQuarterNote) * (100 / 50) )  - startmiditime ;
							// le temps de la note c'est celui cumulé moins celui au moment de la première valable.
							tempNote = ZEROgetNoteFromPile(pInitial[i], j);

							tempNote.endNote = (long)(((float)totalDureeDelta * (float)RatioDeltaToMsec) - (long)startmiditime);
							// long (((( (float) totalDureeDelta * (float) 60000 ) / (float) calculatedBPM ) / (float) NbTicksQuarterNote) * ((float) 100 / (float) 50) )  - startmiditime ;

							pileNote_fixStartEndNote(tempNote, 0);
							ZEROaddNewNotetoPile(pInitial[i], tempNote);
						}
					}

					break;
				case (0x90):
				{
					// PlaynoteOn( lastexact1stbyte, byte (mididouble >> 8) , byte (mididouble &  B01111111) );

					if (mididouble == 0)
					{
						nbNotesOff++;
					}
					else
					{
						nbNotesOn++;
					}

					if (nbNotesOn > (StartNote + midiLongueurFgt))
					{
						FillUpEnoughNotes = true;
						buffer_and_HARD_setCursor(0, 0);
						buffer_and_HARD_print(F("FULL  "));
						// delay(1000);
					}

					// *********************

					if (nbNotesOn >= StartNote)
					{ // Si on a atteint la note de départ on enregistre dans le layer
						if (!firstreached)
						{
							buffer_and_HARD_setCursor(0, 0);
							buffer_and_HARD_print(F("FIRST  "));
							buffer_and_HARD_print(nbNotesOn);
							// delay(1000);
							startmiditime = (long)((float)totalDureeDelta * (float)RatioDeltaToMsec);
							// long (((( (float) totalDureeDelta * (float) 60000 ) / (float) calculatedBPM ) /(float) NbTicksQuarterNote) * ((float)100 / (float)50) ) ;
							firstreached = true;
						}

						buffer_and_HARD_setCursor(0, 0);
						buffer_and_HARD_print(F("REC  "));

						for (byte i = 0; i < nbLayer; i++)
						{
							if (isLayerActive(i))
							{
								// byte j = firstfreenote(i);
								pileNote tempNote;

								tempNote.note = byte(mididouble >> 8);
								tempNote.velocity = byte(mididouble & B01111111);

								switch (recordMode)
								{
								// il faudra adapter qd on instaurera chord !!!
								case 0:
									tempNote.channel = layerChannel[i]; // çàd les 4 premiers bits = le channel   les 4 suivants sont les données pour chord : 0 = solo
									break;
								case 1:
									tempNote.channel = midichannel; // çàd les 4 premiers bits = le channel   les 4 suivants sont les données pour chord : 0 = solo
									break;
								case 2:
									tempNote.channel = 0; // on change rien
									break;
								default:
									break;
								}
								// melodyMatrix[i][j][MMdata2] = (melodyMatrix[i][j][MMdata2] ^ B00000011); // bit 0 et 1 sont 1 = played and silented pour que la note ne soit pas rejouée avant le tour suivant.
								tempNote.startNote = (long)(((float)totalDureeDelta * (float)RatioDeltaToMsec) - (long)startmiditime);
								// long (((( (float) totalDureeDelta * (float) 60000 ) / (float) calculatedBPM ) / (float) NbTicksQuarterNote) * ((float) 100 / (float) 50) )  - startmiditime ;

								// le temps de la note c'est celui cumulé moins celui au moment de la première valable.

								// vérifier si faut pas timenote DOUBLE
								tempNote.endNote = tempNote.startNote + 10; // sera ensuite précisé avec le note OFF
								tempNote.thereIsAnEnd = true;

								pileNote_fixStartEndNote(tempNote, 0);
								ZEROaddNewNotetoPile(pInitial[i], tempNote);
							}
						}
					}
				};
				break;
				default:
					unknownRunningStatus++;
				}
				break;
			}
		} while ((!EndofTrack) && (!FillUpEnoughNotes));

		EndofTrack = false;

	} while (myFile.available() && (!FillUpEnoughNotes));

	// close the file:
	myFile.close();
	measureSDFileCount();
}

// £££££££££££££££££££££££££££££££££££££££££
// DEFINE MIDI FILE and calculate length
// £££££££££££££££££££££££££££££££££££££

void Definemidifile1(void)
{

	// on garde les variables en interne, on va juste mettre les infos dans un String à la fin !
	// byte usedChannel [16] = {0};  // pas utilisé, on va voir nb de Notes pour déterminer si utilisé
	int nbNotesByChannel[16] = {0};
	byte typeMidiFile = 0; // est soit 0, soit 1
	int nbChuncks = 0;
	// long cumulClicks = 0 ; // on va compter le cumul de clicks pour tenter d'ajuster vitesse

	// Open serial communications and wait for port to open:

	nbNotesOn = 0;
	nbNotesOff = 0;
	// NbdeChannels = 0 ;
	totalDureeDelta = 0;
	calculatedBPM = 120; // par défaut si n'est pas spécifié

	// open the file. note that only one file can be open at a time,
	// so you have to close this one before opening another.

	myFile.close();

	myFile = SDcard.open(midiname, FILE_READ);

	// OPEN the HEader

	// TODO 2C27 le typeMIdiFile doit être ici

	readfromFile(4);
	readfromFile(4);
	// threadlen = mididouble;
	readfromFile(2);
	readfromFile(2);

	// NbChunk = int( mididouble ) ;

	// faire un premier passage qui repère les différents chunks et les explicite ?

	readfromFile(2);
	// readfromFile(threadlen - 6);

	CurrentMIDIChunk = 0;
	EndofTrack = false;

	midiFileInfo = "Mid";
	do
	{
		CurrentMIDIChunk++;
		nbChuncks++;

		readfromFile(4);
		readfromFile(4);

		do
		{
			ReadVLVDelta(); // renvoie info dans DureeDelta et VLVDelta
			totalDureeDelta = totalDureeDelta + DureeDelta;

			if (myFile.available() == false)
			{
				return;
			}

			readfromFile(1);
			delay(1);

			exact1stbyte = mididouble & B11111111;
			byte midicategory = (mididouble & B11110000);
			byte midichannel = (mididouble & B00001111); // restauré en 2C27

			switch (midicategory)
			{
			case (0x80):
			{
				readfromFile(2);
				lastRunningStatus = 0x80; // on en aura besoin pour compter les notes On et Off
				lastexact1stbyte = exact1stbyte;
			}
			break;

			case (0x90):
			{
				nbNotesOn++;
				nbNotesByChannel[midichannel]++;

				readfromFile(2);
				lastexact1stbyte = exact1stbyte;

				if (mididouble == 0)
				{
					nbNotesOn--;  // on supprime ce qu'on vient d'ajouter
					nbNotesOff++; // on a une note Off en plus
				}

				lastRunningStatus = 0x90; // on en aura besoin pour compter les notes On et Off
			}
			break;

			case (0xA0):
			{
				readfromFile(2);
			}
			break;

			case (0xB0):
			{
				readfromFile(2);
			}
			break;

			case (0xC0):
			{
				readfromFile(1);
			}
			break;

			case (0xD0):
			{
				readfromFile(1);
			}
			break;

			case (0xE0):
			{
				readfromFile(2);
			}
			break;

			case (0xF0):
			{
				exact1stbyte = mididouble;
				readfromFile(1);
				exact2ndbyte = mididouble;

				if (exact1stbyte == 0xFF)
				{
					switch (exact2ndbyte)
					{
					case (0x2F):
						EndofTrack = true;
						break;
					}
					readfromFile(1);
					readfromFile(mididouble); // va lire le nb de bytes précisés juste au dessus
				}
			}
			break;

			default: // Running Status ???
				specialmididouble = mididouble;
				readfromFile(1);
				mididouble = (specialmididouble << 8) + mididouble;

				// si le dernier était un 0x90 alors on a NoteOn++, sauf si vélocité = 0 ;
				// si le dernier était un 0x80 alors on a NotesOff++

				switch (lastRunningStatus)
				{
				case (0x80):
					nbNotesOff++;
					break;
				case (0x90):
				{
					if (mididouble == 0)
					{
						nbNotesOff++;
					}
					else
					{
						nbNotesOn++;
						nbNotesByChannel[midichannel]++; //2C27
					}
				};
				break;
				default:
					unknownRunningStatus++;
				}
				break;
			}
		} while (!EndofTrack);

		EndofTrack = false;

	} while (myFile.available());

	/* NbTicksQuarterNote * BPM = le nb de Ticks par minute
	donc ( totaldureeDelta / ( NbTicksQuarterNote * BPM ) ) = le nb de minutes
	donc ( totaldureeDelta * 60 / ( NbTicksQuarterNote * BPM ) )= le nb de secondes

	si totaldureeDelta  représente  ( totaldureeDelta * 60 / ( NbTicksQuarterNote * BPM ) ) secondes
	alors totaldureeDelta  représente  ( totaldureeDelta * 60 / ( NbTicksQuarterNote * BPM ) ) * 1000 millisecondes
	et donc 1 delta vaut ( totaldureeDelta * 60 / ( NbTicksQuarterNote * BPM ) ) * 1000 millisecondes / totaldureeDelta
	soit 60 / ( NbTicksQuarterNote * BPM ) ) * 1000 millisecondes
	soit 60000 / ( NbTicksQuarterNote * calculatedBPM )
	*/

	// close the file:
	// tailleTotaleMIDI = nbNotesOn;
	midiFile_totalNotes = nbNotesOn;

	// 2C27

	midiFileInfo += String(typeMidiFile);
	midiFileInfo += " ";

	midiFileInfo += "Chunks:";
	midiFileInfo += String(nbChuncks);
	midiFileInfo += " ";

	for (int i = 0; i < 16; i++)
	{
		if (nbNotesByChannel[i] != 0)
		{
			midiFileInfo += "CH";
			midiFileInfo += String(i);
			midiFileInfo += ":";
			midiFileInfo += String(nbNotesByChannel[i]);
			midiFileInfo += " ";

			Serial.println(midiFileInfo);
		}
	}

	midiFileInfo += " total:";
	midiFileInfo += String(midiFile_totalNotes);

	Serial.println(midiFileInfo);

	myFile.close();
	measureSDFileCount();
}

// void GiveNameOfNote ( byte NNN ) {
//     switch ( NNN % 12 ) {
//         case 0 : notename = "C"; break;
//         case 1: notename = "C#"; break;
//         case 2: notename = "D"; break;
//         case 3: notename = "D#"; break;
//         case 4: notename = "E"; break;
//         case 5: notename = "F"; break;
//         case 6: notename = "F#"; break;
//         case 7: notename = "G"; break;
//         case 8: notename = "G#"; break;
//         case 9: notename = "A"; break;
//         case 10: notename = "A#"; break;
//         case 11: notename = "B"; break;
//         default : notename = " "; break;
//     }
// }

String GiveNameOfNote(byte NNN)
{
	switch (NNN % 12)
	{
	case 0:
		return F("C");
		break;
	case 1:
		return F("C#");
		break;
	case 2:
		return F("D");
		break;
	case 3:
		return F("D#");
		break;
	case 4:
		return F("E");
		break;
	case 5:
		return F("F");
		break;
	case 6:
		return F("F#");
		break;
	case 7:
		return F("G");
		break;
	case 8:
		return F("G#");
		break;
	case 9:
		return F("A");
		break;
	case 10:
		return F("A#");
		break;
	case 11:
		return F("B");
		break;
	default:
		return F(" ");
		break;
	}
}

String GiveFrenchNameOfNote(byte NNN)
{
	switch (NNN % 12)
	{
	case 0:
		return F("Do");
		break;
	case 1:
		return F("Do#");
		break;
	case 2:
		return F("Re");
		break;
	case 3:
		return F("Re#");
		break;
	case 4:
		return F("Mi");
		break;
	case 5:
		return F("Fa");
		break;
	case 6:
		return F("Fa#");
		break;
	case 7:
		return F("Sol");
		break;
	case 8:
		return F("Sol#");
		break;
	case 9:
		return F("La");
		break;
	case 10:
		return F("La#");
		break;
	case 11:
		return F("Si");
		break;
	default:
		return F(" ");
		break;
	}
}

int LineCallingTransfo(int AAA)
{ // renvoie la valeur x tel que Transfo[x][0] appelera la transfo AAA.  Cela permet de savoir où dans le tableau est la transformation, et ainsi les valeurs min et max de façon unifiée
	int thisone;
	thisone = 0;
	while (Transfo[thisone][0] != AAA)
	{
		thisone++;

		if (thisone > SizeTransfo)
		{
			return 0;
		}
		// précaution, vue boucle while
	}

	return thisone;
}

// create a random integer from 0 - 65535
unsigned int rng()
{
	static unsigned int yo = 0;
	yo += micros(); // seeded with changing number
	yo ^= yo << 2;
	yo ^= yo >> 7;
	yo ^= yo << 7;
	yo = yo / 655;
	return (yo);
}

String createNUMBERname(int numberfile)
{
	String randString = nulString;
	randString = String(numberfile);

	while (randString.length() < 4)
	{
		randString = "0" + randString;
	}

	randString = randString + F(".txt");
	return (randString);
}

String addONEtothisFile(String &File1)
{
	String smallString;
	String randString = nulString;

	smallString = File1.substring(0, 4);
	int firstval = smallString.toInt();
	firstval++;
	randString = createNUMBERname(firstval);

	return (randString);
}

bool FileExists(String &thisFile)
{

	if (SDcard.exists(thisFile))
	{
		return (true);
	}
	else
	{
		return (false);
	}

	// A ADAPTER POUR LE PASSAGE SD vers SdFAT
	return true;
}

void createANDopenFile(String &thisFile)
{
	// open a new file and immediately close it:

	myFile = SDcard.open(thisFile, FILE_WRITE);
}

void closeFile()
{
	// DEBUG je ne vois pas que ceci ferme
	myFile.close();
}

void deleteFile(String &thisFile)
{
	myFile.close(); // ferme le fichier actuel

	// File deleteFile;

	//	deleteFile = SDcard.open(thisFile, FILE_WRITE);
	//	deleteFile.remove();
	SDcard.remove(thisFile);

	// A ADAPTER POUR LE PASSAGE SD vers SdFAT
}

/////////////////////////////////////////////////////////////////////////////////////
// BLACK 08 - rue Hespel
// 28 juillet 2021        // réécrit de façon beaucoup moins énergivore pour la carte
////////////////////////////////////////////////////////////////////////////////////

void NameFileNumber(int a)
{
	if (SDCardPresente == false)
	{
		strcpy(name, "nada");
		return;
	}

	if (a < 0)
	{
		strcpy(name, "invalid nb");
		return;
	}

	if (playParallelMidiON == true)
	{
		strcpy(name, "Playing!!!");
		return;
	}

	// Open root directory
	if (!dir.open("/"))
	{
		error("dir.open failed");
	}

	// Open next file in root.  The volume working directory, vwd, is root.
	// Warning, openNext starts at the current position of  wd() so a
	// rewind may be neccessary in your application.
	// BLACK08 apparemment pas nécessaire ici

	int i = 1;

	if (a == 1)
	{
		myFile.openNext(&dir, O_READ);
		myFile.getName(name, 60);
		myFile.close();
		return;
	}

	myFile.close();

	while (i <= a && myFile.openNext(&dir, O_READ))
	{
		// file.printFileSize(&Serial);
		// Serial.write(' ');
		// file.printModifyDateTime(&Serial);
		// Serial.write(' ');
		myFile.printName(&Serial);

		if (myFile.isDir())
		{
			// Indicate a directory.
			Serial.write('/');
		}
		Serial.println();

		i++;
	}

	myFile.getName(name, 60);
	myFile.close();

	Serial.println("Done!");
}

void peekfromFile(int n)
{
	// midistring = nulString ; ;
	mididouble = 0;

	if (n == 0)
	{
		return;
	}
	for (int i = 0; i < n; i++)
	{
		midibyte = myFile.peek();

		// Pas juste car il n'avance pas d'une position !!!!!!!!!

		// midistring = midistring + String ( midibyte, HEX ) + " " ;
		mididouble = (mididouble << 8) + midibyte;
	}
}

void PlaynoteOnSampler(byte cmd, byte pitch, byte velocity)
{
	// On va envoyer sur le Behringer DSP2024P des codes CC permettant de jouer un sample pitché plutôt qu'une note
	// Serial3.write(cmd);
	// Serial3.write(pitch);
	// Serial3.write(velocity);

	// Dans un premier temps on présume que le Sampler est prêt
	// il faudra s'assurer quand on arme l'effet qu'on sélectionne le bon effet

	byte ExtractCH;
	byte ExtractCode;

	ExtractCH = cmd & B00001111;
	ExtractCode = cmd & B11110000;

	switch (ExtractCode)
	{
	case 0x90:
		// SET FWD ONCE
		//Serial3.write(0xB0 + ExtractCH); //
		//Serial3.write(106); //  = mode
		//Serial3.write(0x2);  //  = 1x FWD

		// CHANGE PITCH
		Serial3.write(0xB0 + ExtractCH); //
		Serial3.write(105);				 // = speed
		Serial3.write(pitch);			 //

		// PLAY
		Serial3.write(0xB0 + ExtractCH); //
		Serial3.write(104);				 // = PLAY
		Serial3.write(127);				 // = JOUE ?

		break;

	case 0x80:

		// STOP
		Serial3.write(0xB0 + ExtractCH); // CC Channel 1 = Midi 0
		Serial3.write(104);				 // = PLAY STOP
		Serial3.write(0x0);				 // = Valeur en A : 0 = STOP

		break;
	}
}

/* va modifier la valeur d'un effet du Behringer
typiquement répartir en 6 valeur, de A à F
l'idée est ici qu'à intervalle régulier (lié à la duée du layer et à l'emplacement au sein de celui-ci), une valeur A-F soit modifiée aléatoirement
à terme il faut se demander si intégrer cela à un layer est la meilleure idée, car on n'aura sans doute que 1 ou 2 modification par layer, pas 32
de plus c'est très lourd comme infrastructure
il faut que le player reconnaisse que cest pas une note
proposition : utiliser un autre array pour les CC modifs -> le player joue les notes, puis avec une architecture simplifiée, joue les CC

Serial3.write(0xB0); // CC Channel 1 = Midi 0
Serial3.write(103); // = Edit A
Serial3.write(0x0);  // = Valeur en A : 0 = STOP REC


Changer l'effet                            102 (0x66)
Paramètres de l'effet 57 = SAMPLER         valeur MIDI = 56 !!!

Edit A = STOP/REC      STOP                103 (0x67)
Edit B = STOP/PLAY     STOP                104 (0x68)
Edit C = Speed         100%  : 25à400      105 (0x69)
Edit D = Mode          FRWRD               106  (0x6A)
BACK FRWRD 1x 2x 3x 4x 5x 6x 7x 8x 9x 10x
0     1     2 3   4  5 6  7   8 9 10 11

Edit E = start point   0.0s                107
Edit F = stop point    5.08s               108
Edit G = Bass    -16 +16
Edit H = Treble  -16 +16

Mix                    50%                  111

*/

// typedef struct slotType slotType ;
// struct slotType {
//     byte x ;
//     byte y ;
// } ;
//
// slotType slot[5] ;

/*
void defineSlot(byte const &a, byte const &x, byte const &y)
{
	slot[a].x = x;
	slot[a].y = y;
}
*/

/*
void buffer_setCursor(byte x, byte y)
// dispose le curseur sur l'écran virtuel, actualisé progressivement
{
	cursorX = x;
	cursorY = y;
}

void buffer_and_HARD_setCursor(byte x, byte y)
// dispose le curseur sur l'écran virtuel ET en vrai !
{
	cursorX = x;
	cursorY = y;
	u8x8.setCursor(x, y); // LA SEULE REFERENCE à 8x8 à garder // plus tard remplacer par LCD ou wathever
}
*/

/*
void buffer_print(String Texte)
// écrit un texte sur l'écran virtuel, actualisé progressivement
{
	int positionBuffer = cursorY * sizeX + cursorX;

	byte longueur = Texte.length();

	for (int i = 0; i < longueur; i++)
	{
		bufferWrite[positionBuffer % sizeX][positionBuffer / sizeX] = (char)Texte[i];
		positionBuffer++;
	}

	cursorX = positionBuffer % sizeX;
	cursorY = positionBuffer / sizeX;
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
		u8x8.print(Texte[i]); // LE SEUL QUI DOIT RESTER INTACT - plus tard remplacer par LCD, etc...
		positionBuffer++;
	}

	cursorX = positionBuffer % sizeX;
	cursorY = positionBuffer / sizeX;
}
*/

/*
void buffer_clearScreen(void)
{

	for (byte i = 0; i < sizeX; i++)
	{
		for (byte j = 0; j < sizeY; j++)
		{
			//	bufferDisplay[i][j] = ' ';
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

void buffer_clearLine(byte A)
{
	for (byte j = 0; j < sizeY; j++)
	{
		bufferDisplay[A][j] = ' ';
	}
}

void buffer_clearLine_HARD(byte A)
{
	for (byte j = 0; j < sizeY; j++)
	{
		bufferDisplay[A][j] = '*';
		bufferWrite[A][j] = ' ';
		// va donc forcer un rewrite complet
	}
}

void printRot2(String Rtexte, byte Rcases, int Rspeed, byte slotIci)
{

	// v3.0 affiche un texte déroulant

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

	if ((nowMicros - lastAffichage[slotIci]) > 3000)
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

			// 2C19 buffer_and_HARD_setCursor(slot[slotIci].x, slot[slotIci].y);
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
				// 2C19 buffer_and_HARD_print(temp);
				buffer_print(temp);
			}
			else
			{
				buffer_print(Rtexte);

				for (int i = Rlen; i < Rcases; i++)
				{
					// buffer_and_HARD_print(" ");
					buffer_print(" ");
				}
			}
			Rpreviousl[slotIci] = l;
		}

		Rfirsttime[slotIci] = false;
	}
}
*/

// UPDATE FROM CONCPOLY19
point Stretch(point x, point a, float R) // Stretche un point a par rapport à un fixe x
{
	point z;
	z.x = (float)((float)R * ((float)a.x - (float)x.x)) + (float)x.x;
	z.y = (float)((float)R * ((float)a.y - (float)x.y)) + (float)x.y;
	return z;
}

float fixAngle(float angle)
{
	while (angle < 0)
	{
		angle = angle + 360;
	}
	return angle;
}

//UPDATE CONCPOLYG19

float absf(float a)
{
	if (a < 0)
	{
		return -a;
	}
	else
	{
		return a;
	}
}

float oldfixAngle(float angle)
{
	while (angle < 0)
	{
		angle = angle + 360;
	}
	return angle;
}

// UPDATE FROM CONCPOLYG19
float Longueur2(point x, point a)
{
	// calcule la longueur entre le point x1,x2 et a1,a2

	if (x.x == a.x)
	{
		return (absf(a.y - x.y));
	}

	if (x.y == a.y)
	{
		return (absf(a.x - x.x));
	}

	return sqrtf((float)pow((a.x - x.x), 2) + (float)pow((a.y - x.y), 2));
}

// UPDATE FROM CONCPOLYG19
float Angle360bis(point x, point a)
{
	// calcule l'angle entre l'horizontale et ces 2 points dans le sens horloger en partant de l'horizontale
	double alpha1(0);

	if ((Longueur2(a, x)) == 0.0)
	{
		alpha1 = 0;
		return alpha1;
	}

	//printf("\na.y-x.y %f ",a.y - x.y );
	//printf("\nLongueur (a,x) %f",Longueur2(a , x) );
	//printf("\n(float) ( a.y - x.y ) / (float) Longueur2(a , x) %f ",(float) ( a.y - x.y ) / (float) Longueur2(a , x));
	float value = (float)(a.y - x.y) / (float)Longueur2(a, x);
	// printf("\nvalue %f",value);
	if (value >= 1.0)
	{
		value = 1.0;
	}
	if (value <= -1.0)
	{
		value = -1.0;
	}
	// printf("\ncorrected value %f",value);
	// printf("\nasin value %f ",asin  ( value ));

	alpha1 = asin(value) * (float)RadianToDegrees;

	// printf("\nalpha1 %f",alpha1) ;

	if (a.x >= x.x)
	{
		if (a.y >= x.y) // quadrant supérieur droit
		{
			alpha1 = 360.0 - alpha1;
		}
		else
		{
			// quadrant inférieur droit
			alpha1 = -alpha1;
		}
	}
	else
	{
		if (a.y >= x.y) // quadrant supérieur gauche
		{
			alpha1 = 180.0 + alpha1;
		}
		else
		{
			// quadrant inférieur gauche
			alpha1 = 180.0 + alpha1;
		}
	}

	alpha1 = oldfixAngle(alpha1);

	// printf("\nfix alpha1 %f",alpha1) ;
	return alpha1;
}

point Rotate(point &x, point &y, float angle)
// UPDATED FROM CONCPOLYG19
{
	// rotation de y autour de x !
	// effectue une rotation du point y d'un certain angle en prenant comme centre de rotation le point x1,x2
	point z;
	// float LongSeg = Longueur(y.x, y.y, x.x, x.y);
	float LongSeg = Longueur2(y, x);

	// float alphadepart = Angle360(x.x, x.y, y.x, y.y);
	float alphadepart = Angle360bis(x, y);
	float alphaarrivee = oldfixAngle(fmod(alphadepart + angle, 360));

	z.y = (((float)x.y -
			(float)sinf((float)(alphaarrivee) / (float)RadianToDegrees) * (float)LongSeg));

	if (LongSeg != 0)
	{
		z.x = ((float)x.x + ((float)cosf((float)(alphaarrivee) / (float)RadianToDegrees) * (float)LongSeg));
	}
	else
	{
		z.x = x.x;
		// ajouté suite à des stack overflow ou nan ! pas sur que soit exact
	}

	return z;
}

byte firstfreeEvenement(void)
{ // procédure qui va renvoyer le premier évènement libre ;
	// si tout est full, il renvoie un chiffre au hasard ****NOTE ??? vraiment ???

	byte eventfinder = 0;
	bool fulllayer = true;
	bool foundfirst = false;

	for (byte j = 0; j < NbEvenementMax; j++)
	{
		// DEBUG on ne commmence pas à 0 car 0 sert à stocker les évènements en préparation
		// de 1 à NbEvenementMax on a les actifs

		if (Evenement[j].type == 0)
		{
			fulllayer = false;

			if (foundfirst == false)
			{
				eventfinder = j;
				foundfirst = true;
			}
		}
	}

	// NOTE **** ne faut-il pas remplacer comme firstfreenote par une procédure avec un Do While ?
	// Do While !foundfirst && j < 32

	if (fulllayer == true)
	{
		eventfinder = random(0, NbEvenementMax);
	}
	return eventfinder;
}

void EmptyEvenement(byte i)
{
	Evenement[i].actif = false;
	Evenement[i].type = 0; // 0 = vide  1=CC 2=Transfo
	// pour les Transfo modifiant valeur, elle devront voir le type défini
	Evenement[i].intervalle = 1000; // 1 = 0.1sec - sinon risque de déclencher un event super réptitif en ativant un event nul
	Evenement[i].moment = 0;		// dans les temps divisible par 300, agit après 100
	Transfo[LineCallingTransfo(131)][2] = (int)18000;
	// TODO DEBUG c'est moche cette façon de faire !
	Evenement[i].fidelite = 100; // %
	Evenement[i].which = 0;		 // définit quel CC va être modifié
	Evenement[i].valeur = 0;	 // définit la valeur initiale qui lui sera attribuée
	Evenement[i].shape = 1;		 // 1=fixe 2=inc 3=Dec 4=rnd 5=liste
	Evenement[i].rangeUP_ = 125;
	Evenement[i].rangeLO_ = 10;
	Evenement[i].incSize = 0;
	Evenement[i].behaviorLimit = 0;
	Evenement[i].channel = 0; //
	Evenement[i].layer = 16;  // tous layers actives - non pertinent pour un CC, car ne modifie pas de layers !
	Evenement[i].eventGOON = 0;

	Evenement[i].conditionActive = false;
	Evenement[i].whichTest = 0;
	Evenement[i].whichOperation = 0;
	Evenement[i].whichValue = 0;
	Evenement[i].whichComparison = 0;
	Evenement[i].whichReference = 0;
	Evenement[i].whichAction = 0;	  // 1 DO 2.BLOCK
	Evenement[i].launch = false;	  // utilisé pour ce qui est lié  à des notes ou layer modulo play
	Evenement[i].whichLayer2test = 0; //

	Evenement[i].modulo = 1;
	Evenement[i].moduloK = 0;
	Evenement[i].moduloLayers = 0;
	Evenement[i].moduloMode = 0;

	Evenement[i].compteur = 0;
	Evenement[i].echeance = 1000;	// decrit le nb de fois maximum qu'un evt peut avoir lieu
	Evenement[i].latence = 0;		// décrit à partir de quand l'event aura réellement lieu
	Evenement[i].fugace = false;	// si YES, qd compteur arrive à échéance, l'evt est supprimé
	Evenement[i].seqUsedAsList = 0; // TODO DEBUG : mettre àjour initialisation, delete et save

// était auparavant :

	// Evenement[i].type = Evenement[0].type ; // 0 = vide  1=CC 2=Transfo
	// // pour les Transfo modifiant valeur, elle devront voir le type défini
	// Evenement[i].intervalle  = Evenement[0].intervalle   ; // 1 = 0.1sec
	// Evenement[i].moment = Evenement[0].moment ; // dans les temps divisible par 300, agit après 100
	// Evenement[i].fidelite  = Evenement[0].fidelite  ; // %
	// Evenement[i].which  =  Evenement[0].which  ; // définit quel CC va être modifié
	// Evenement[i].valeur  =Evenement[0].valeur ; // définit la valeur initiale qui lui sera attribuée
	// Evenement[i].shape   =   Evenement[0].shape   ; // 1=fixe 2=inc 3=Dec 4=rnd 5=liste
	// Evenement[i].rangeUP_  = Evenement[0].rangeUP_   ;
	// Evenement[i].rangeLO_  = Evenement[0].rangeLO_   ;
	// Evenement[i].incSize  = Evenement[0].incSize  ;
	// Evenement[i].behaviorLimit  = Evenement[0].behaviorLimit  ;
	// Evenement[i].channel  = Evenement[0].channel  ; // à supposer que la machine recevant le CC soit en midi 5 (CH6)
	// Evenement[i].layer  = Evenement[0].layer   ;   // tous layers actives - non pertinent pour un CC, car ne modifie pas de layers !
	// Evenement[i].eventGOON = millis() - ( millis() % Evenement[i].intervalle ) + Evenement[i].intervalle + Evenement[i].moment ;
}

void EmptyLFO(byte i)
{
	LFO[i].actif = false;	 // rajouté en nov2018, sur le modèle de Sequence
	LFO[i].type = 0;		 // 0 = vide 1 = CC 2 = transfo : on va développer 1 pour l'instant, 2 sera pour plus tard, peut être
	LFO[i].shape = 2;		 // sin square Sample&Hold saw
	LFO[i].fidelite = 100;	 // normalement 100, mais on pourra avoir dégradation du signal
	LFO[i].duration = 10000; // en millisecondes
	LFO[i].decalage = 0;

	LFO[i].lowest = 0; // je mets INT car je pense que ce ne sera pas que pour CC, mais aussi Transfo !!!
	LFO[i].highest = 127;
	// LFO[i].stepValue = 1 ; // augment de n à chaque pas.
	LFO[i].compteur = 0; // indique la valeur actuelle
	// LFO[i].direction = 0 ; // 0 = UP  1 = DOWN

	LFO[i].channel = 0;		// channel concerné 0-15
	LFO[i].which = 0;		// CC Value ou valeur de Transfo
	LFO[i].typeMachine = 0; // 0 = normal : CC en 3 byte ; faudra spécifique pour microSampler, TX81Z, WavePad
	LFO[i].valeur = 0;		// valeur à jouer au moment où eventGOON arrive

	LFO[i].layer = 0;	  // layer concerné 1-4 ou combinaison ou random ; ou bien layers actives.   par défaut : Actif ; sinon spécifique
	LFO[i].eventGOON = 0; // indique à quel moment le prochain Evènement doit avoir lieu
	LFO[i].modC = 1;
	LFO[i].modV = 1;
	// LFO[i].residue = 0 ;
	LFO[i].oldResidue = 0;
	LFO[i].LFORandom = 0;
}

void copyLFOAintoLFOB(byte i, byte B)
{
	LFO[B].actif = LFO[i].actif;	   // rajouté en nov2018, sur le modèle de Sequence
	LFO[B].type = LFO[i].type;		   // 0 = vide 1 = CC 2 = transfo : on va développer 1 pour l'instant, 2 sera pour plus tard, peut être
	LFO[B].shape = LFO[i].shape;	   // sin square Sample&Hold saw
	LFO[B].fidelite = LFO[i].fidelite; // normalement 100, mais on pourra avoir dégradation du signal
	LFO[B].duration = LFO[i].duration; // en millisecondes
	LFO[B].decalage = LFO[i].decalage;

	LFO[B].lowest = LFO[i].lowest; // je mets INT car je pense que ce ne sera pas que pour CC, mais aussi Transfo !!!
	LFO[B].highest = LFO[i].highest;
	// LFO[B].stepValue = LFO[i].stepValue; // augment de n à chaque pas.
	LFO[B].compteur = LFO[i].compteur; // indique la valeur actuelle
	// LFO[B].direction = LFO[i].direction; // 0 = UP  1 = DOWN

	LFO[B].channel = LFO[i].channel;		 // channel concerné 0-15
	LFO[B].which = LFO[i].which;			 // CC Value ou valeur de Transfo
	LFO[B].typeMachine = LFO[i].typeMachine; // 0 = normal : CC en 3 byte ; faudra spécifique pour microSampler, TX81Z, WavePad
	LFO[B].valeur = LFO[i].valeur;			 // valeur à jouer au moment où eventGOON arrive

	LFO[B].layer = LFO[i].layer; // layer concerné 1-4 ou combinaison ou random ; ou bien layers actives.   par défaut : Actif ; sinon spécifique
	LFO[B].eventGOON = 0;		 // indique à quel moment le prochain Evènement doit avoir lieu
	LFO[B].modC = LFO[i].modC;
	LFO[B].modV = LFO[i].modV;
	// LFO[B].residue = 0;
	LFO[B].oldResidue = 0;
	LFO[B].LFORandom = LFO[i].LFORandom;
	;
}

void TreatNote_pileNote(byte i, byte j, unsigned long lgAB)
{

	if ((nowMillis - treatNoteTime) > 1000)
	{
		return;
	} // protection contre récursion infinie

	int tempA, tempB;
	byte iAB;

	String contenuA = nulString, contenuB = nulString, newcontenuA = nulString, newcontenuB = nulString;

	String totalcontenuA = nulString;
	;

	pileNote tempNote;

	tempNote = ZEROgetNoteFromPile(pInitial[i], j);
	pileNote_fixStartEndNote(tempNote, 0);

	ZEROSetNoteInPile(pInitial[i], j, tempNote);
	KeepDuree = getDuree(tempNote);

	//	iAB = melodyMatrixTime[i][j][MMTstartnote] / lgAB;
	iAB = tempNote.startNote / lgAB;

	// si iAB est pair, on ne fait rien
	// si iAB est impair :
	// les notes sont amenées à temps(iAB+1)-tempsiAB * pcAB / 100, ajouté à tempsiAB

	// NOTE **** ajouter des options pour choisir si
	// odd, even
	// si l'even doit être aligné ou laissé tel quel

	//         if ( isOdd(iAB) ) {
	//
	//           tempA = iAB * lgAB  ;        // donne le temps dans la grille parfaite avant startnote
	//           tempB = (iAB + 1) * lgAB ; // donne le temps dans la grille parfaite après startnote
	//           float ftempC = ((float)tempB - (float)tempA) * (float)pcAB / (float)100.0 ;
	//           long tempC = (long) tempA + (long) ftempC ;
	//
	//           melodyMatrixTime[i][j][MMTstartnote] = tempC ;
	//           melodyMatrixTime[i][j][MMTendnote] = melodyMatrixTime[i][j][MMTstartnote] + KeepDuree   ; ;
	// }

	// on a le iAB, càd le lieu dans le layer qui indique la division
	// on regarde dans le patternSwing, en fc du nSwing l'action à entreprendre
	// si -> : on décale ( déjà fait)
	// si <- : on décale, mais le % devient 100-%
	// si - : nada
	// si I : on met sur tempA

	String check;
	check = patternSwing.substring(iAB % patternSwing.length(), iAB % patternSwing.length() + 1);

	if (check == flechedroite)
	{
		tempA = iAB * lgAB;		  // donne le temps dans la grille parfaite avant startnote
		tempB = (iAB + 1) * lgAB; // donne le temps dans la grille parfaite après startnote
		float ftempC = ((float)tempB - (float)tempA) * (float)pcAB / (float)100.0;
		long tempC = (long)tempA + (long)ftempC;

		tempNote.startNote = tempC;
		tempNote.endNote = tempNote.startNote + KeepDuree;
		ZEROSetNoteInPile(pInitial[i], j, tempNote);
		;
	}

	if (check == flechegauche)
	{
		pcAB = 100 - pcAB;
		tempA = iAB * lgAB;		  // donne le temps dans la grille parfaite avant startnote
		tempB = (iAB + 1) * lgAB; // donne le temps dans la grille parfaite après startnote
		float ftempC = ((float)tempB - (float)tempA) * (float)pcAB / (float)100.0;
		long tempC = (long)tempA + (long)ftempC;

		tempNote.startNote = tempC;
		tempNote.endNote = tempNote.startNote + KeepDuree;
		ZEROSetNoteInPile(pInitial[i], j, tempNote);
		;
	}

	if (check == barremilieu)
	{
		tempA = iAB * lgAB; // donne le temps dans la grille parfaite avant startnote
		tempNote.startNote = tempA + 1;
		tempNote.endNote = tempNote.startNote + KeepDuree;
		ZEROSetNoteInPile(pInitial[i], j, tempNote);
		;
	}

	if (check == barreH)
	{
		tempB = (iAB + 1) * lgAB; // donne le temps dans la grille parfaite après startnote
		tempNote.startNote = tempB + 1;
		tempNote.endNote = tempNote.startNote + KeepDuree;
		ZEROSetNoteInPile(pInitial[i], j, tempNote);
		;
	}

	if (check == "!")
	{

		tempA = iAB * lgAB;
		tempB = (iAB + 1) * lgAB; // donne le temps dans la grille parfaite après startnote
		tempNote.startNote = (random(1) == 0 ? tempA - 2 : tempB + 2);
		// fait en somme passer la note soit juste de l'autre côté en bas, soit juste de l'autre côté en haut

		tempNote.endNote = tempNote.startNote + KeepDuree;
		;
		pileNote_fixStartEndNote(tempNote, 0);
		ZEROSetNoteInPile(pInitial[i], j, tempNote);
		TreatNote_pileNote(i, j, lgAB);
	}

	if (check == "x")
	{ // NOTE ***** A définir : va envoyer les notes vers le iAB précédent avec une valeur non "x"
		//   n = iAB
		//   si n = 0 : stop
		//   si n > 0 : m = n-1
		//   tester patternSwing.substring(iAB % patternSwing.length(),iAB % patternSwing.length() + 1 )
		//    si ="x"
		//      recursion
		//    sinon : on applique là la transfo exigée

		if (iAB != 0)
		{
			// d'abord on va déplacer la note à la même place dans la division précédente

			tempNote.startNote -= lgAB;
			tempNote.endNote -= lgAB;

			pileNote_fixStartEndNote(tempNote, 0);
			ZEROSetNoteInPile(pInitial[i], j, tempNote);
			TreatNote_pileNote(i, j, lgAB);

			// ensuite on doit revoir la situation pour cette note modifiée, par récursivité
		}
	}

	//	fixStartEndNote(i, j);
}

int noteWrapper(int a)
{
	int b = limiter(a, 0, 127);
	// limiter limie aux valeurs, INCLUES
	return b;

	// plus tard développer des choses similaires à BOUNCE
	// NOTE *****
}

void checkSkipChunckAndCalcGoOn()
{
	unsigned int nowCutOffDureeDelta = getValueStraightOrFromSequence(CutOffDureeDelta, 0);
	
	if (DureeDelta > nowCutOffDureeDelta)
	{
		DureeDelta = getValueStraightOrFromSequence(BringToDureeDelta,0) ;
	}

	// 22C01 nouveau loCut et hiCut pour durée delta

	if ( (int) DureeDelta < loCutMIDI)
	{
		DureeDelta = getValueStraightOrFromSequence(loBringToMIDI, 0);
	}

	if ( (int) DureeDelta > hiCutMIDI)
	{
		DureeDelta = getValueStraightOrFromSequence(hiBringToMIDI, 0);
	}

	// quid interaction des 2 ? lo et hi...   ici on fait le lo d'abord puis le hi, ce qui peut créer une fenêtre
	

	// 2C25 peut être faut il d'emblée un skip chunck si c'est le premier ?

	if (SkipChunck)
	{
		DureeDelta = 0;
		GoONmidi = 0;
	}
	else
	{
		int nowNumRatioDeltaToMsec = getValueStraightOrFromSequence(NumRatioDeltaToMsec, 1);
		int nowDenomRatioDeltaToMsec = getValueStraightOrFromSequence(DenomRatioDeltaToMsec, 1);
		if (nowDenomRatioDeltaToMsec == 0)
		{
			nowDenomRatioDeltaToMsec++;
		}
		float RatioDeltaToMsec = (float)nowNumRatioDeltaToMsec / (float)nowDenomRatioDeltaToMsec;

		GoONmidi = nowMillis + (unsigned long)((float)DureeDelta * (float)RatioDeltaToMsec);
	}
}

fract closestFraction(int a, int b, int maxSteps, bool tryAllSteps)
{
	// si on met false en tryAllSteps, il donne juste la valeur pour n/maxSteps ( p.ex. 3/16, 4/16, 8/16
	// TODO DEBUG l'algorithme ne me semble pas très efficace en temps !
	float currentClosest;
	float currentTry;
	fract result;
	byte iclosest(0), jclosest(1);

	if (tryAllSteps)
	{
		for (byte i = 0; i < (maxSteps + 1); i++)
		{
			for (byte j = 1; j < (maxSteps + 1); j++)
			{
				currentClosest = sq(((float)iclosest * (float)b / (float)jclosest) - (float)a);
				currentTry = sq(((float)i * (float)b / (float)j) - (float)a);
				if (currentTry < currentClosest)
				{
					iclosest = i;
					jclosest = j;
				}
			}
		}
	}
	else
	{
		byte j = maxSteps;
		jclosest = j;
		for (byte i = 0; i < (maxSteps + 1); i++)
		{
			currentClosest = sq(((float)iclosest * (float)b / (float)jclosest) - (float)a);
			currentTry = sq(((float)i * (float)b / (float)j) - (float)a);
			if (currentTry < currentClosest)
			{
				iclosest = i;
				jclosest = j;
			}
		}
	}

	result.imprecision = (((float)iclosest * (float)b / (float)jclosest) - (float)a);
	result.num = iclosest;
	result.den = jclosest;

	return result;
}

byte firstActiveLayer(void)
{
	for (byte i = 0; i < nbLayer; i++)
	{
		if (isLayerActive(i))
		{
			return i;
		}
	}

	return 50; // si on a 50 ça veut dire qu'aucun layer n'est sélectionné car aucun n'est 1
}

bool isNegativeSByte(byte const &thi)
{
	/**
	* renvoie true si le SignedSilentByte est négatif
	* @param [name] [description]
	*/
	if (((thi >> 7) & B0001) == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool isSilentSByte(byte const &thi)
{
	/**
	* renvoie true si le SignedSilentByte est silencieux
	* @param [name] [description]
	*/
	if (((thi >> 6) & B0001) == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int valueSByte(byte const &thi)
{
	/**
	* renvoie la valeur en + ou - du SignedSilentByte : cette valeur va de +63 à -63
	* en pratique on l'utilisera surtout de +24 à -24
	* @param thi [description]
	*/
	int that = (thi & B00111111);
	if (isNegativeSByte(thi))
	{
		that = -that;
	}
	return that;
}

byte makeSByte(int thi, bool const &silent)
{
	/**
	* crée un SignedSilentByte à partir d'un integer, où on indique s'il doit être silencieux ou pas
	* @param [name] [description]
	*/

	byte a;
	if (thi > 63)
	{
		thi = 63;
	}
	if (thi < -63)
	{
		thi = -63;
	}
	a = abs(thi);
	if (thi < 0)
	{
		a = a | B10000000;
	}
	if (silent)
	{
		a = a | B01000000;
	}
	return a;
}

void silenceNoteRedirect(byte i)
{
	noteRedirect[i] = makeSByte(valueSByte(noteRedirect[i]), true);
}

String giveSign(int a)
{

	String temp;
	temp = String(a);
	if (a > 0)
	{
		temp = "+" + temp;
	}
	return temp;
}

/////////////////////////////////////////////////////////////////////////////////////
// KANNIBAL 2C19
// MODIF DU RECEIVE CHANNEL à la relecture du manuel TX81Z
// Ajout de channel
/////////////////////////////////////////////////////////////////////////////////////

void convertData1and2toTX81ZModif_OLD(byte note, byte velocity, byte channel = 0)
{

	// note = n° du CC
	// velocity = valeur du CC
	byte paramTX81Z;
	byte dataTX81Z;
	byte lig;

	lig = (((note - 10) / 8) + 1) * 13;

	if ((note < 33) && (note > 9))
	{
		switch ((note - 10) % 8)
		{
		case 0:
			paramTX81Z = 0 + lig; // AR
			dataTX81Z = map(velocity, 0, 127, 0, 32);
			break;
		case 1:
			paramTX81Z = 1 + lig; // D1R
			dataTX81Z = map(velocity, 0, 127, 0, 32);
			break;
		case 2:
			paramTX81Z = 2 + lig; // D2R
			dataTX81Z = map(velocity, 0, 127, 0, 32);
			break;
		case 3:
			paramTX81Z = 3 + lig; // RR
			dataTX81Z = map(velocity, 0, 127, 0, 32);
			break;
		case 4:
			paramTX81Z = 4 + lig; // D1L
			dataTX81Z = map(velocity, 0, 127, 0, 32);
			break;
		case 5:
			paramTX81Z = 11 + lig; // CRS
			dataTX81Z = map(velocity, 0, 127, 0, 63);
			break;
		case 6:
			paramTX81Z = 12 + lig; // DET
			dataTX81Z = map(velocity, 0, 127, 0, 6);
			break;
		case 7:
			paramTX81Z = 10 + lig; // OUT
			dataTX81Z = map(velocity, 0, 127, 0, 99);
			break;

			/* case 8 : dataTX81Z = random(0, 1);
			case 9 : dataTX81Z = random(0, 7);
			case 10 : dataTX81Z = random(0, 99);
			case 11 : dataTX81Z = random(0, 63);
			case 12 : dataTX81Z = random(0, 6);

			case 52 : dataTX81Z = random(0, 7);
			case 53 : dataTX81Z = random(0, 7);
			case 54 : dataTX81Z = random(0, 99);
			case 55 : dataTX81Z = random(0, 99);
			case 56 : dataTX81Z = random(0, 99);
			case 57 : dataTX81Z = random(0, 99);
			case 58 : dataTX81Z = random(0, 1);
			case 59 : dataTX81Z = random(0, 3);
			case 60 : dataTX81Z = random(0, 7);
			case 61 : dataTX81Z = random(0, 3);
			case 62 : dataTX81Z = random(0, 48);
			*/
		}

		Serial3.write(0xF0);		   // Exclusive
		Serial3.write(0x43);		   // Yamaha ID
		Serial3.write(0x10 + channel); // 1n : n = basic receive channel
		Serial3.write(0x12);		   // VCED
		Serial3.write(paramTX81Z);
		Serial3.write(dataTX81Z);
		Serial3.write(0xF7);
	}

	if (note == 1)
	{
		// ALG
		Serial3.write(0xF0);
		Serial3.write(0x43);
		Serial3.write(0x10 + channel);
		Serial3.write(0x12);
		Serial3.write(52);
		dataTX81Z = map(velocity, 0, 127, 0, 6);
		Serial3.write(dataTX81Z);
		Serial3.write(0xF7);
	}

	if (note == 2)
	{
		// Portamento Time
		Serial3.write(0xF0);
		Serial3.write(0x43);
		Serial3.write(0x10 + channel);
		Serial3.write(0x12);
		Serial3.write(66);
		dataTX81Z = map(velocity, 0, 127, 0, 99);
		Serial3.write(dataTX81Z);
		Serial3.write(0xF7);
	}

	if (note == 3)
	{
		// Operator Waveform 1
		Serial3.write(0xF0);
		Serial3.write(0x43);
		Serial3.write(0x10 + channel); // ACED, pas VCED    DEBUG devrait être 0 pour channel 0 !
		Serial3.write(0x13);		   // ACED
		Serial3.write(18);
		dataTX81Z = map(velocity, 0, 127, 0, 8);
		Serial3.write(dataTX81Z);
		Serial3.write(0xF7);
	}

	if (note == 4)
	{
		// Operator Waveform 1
		Serial3.write(0xF0);
		Serial3.write(0x43);
		Serial3.write(0x10 + channel); // ACED, pas VCED   DEBUG devrait être 0 pour channel 0 !
		Serial3.write(0x13);		   // ACED
		Serial3.write(13);
		dataTX81Z = map(velocity, 0, 127, 0, 8);
		Serial3.write(dataTX81Z);
		Serial3.write(0xF7);
	}

	if (note == 5)
	{
		// Operator Waveform 1
		Serial3.write(0xF0);
		Serial3.write(0x43);
		Serial3.write(0x10 + channel); // ACED, pas VCED  DEBUG devrait être 0 pour channel 0 !
		Serial3.write(0x13);
		Serial3.write(8);
		dataTX81Z = map(velocity, 0, 127, 0, 8);
		Serial3.write(dataTX81Z);
		Serial3.write(0xF7);
	}

	if (note == 6)
	{
		// Operator Waveform 1
		Serial3.write(0xF0);
		Serial3.write(0x43);
		Serial3.write(0x10 + channel); // ACED, pas VCED    DEBUG devrait être 0 pour channel 0 !
		Serial3.write(0x13);
		Serial3.write(3);
		dataTX81Z = map(velocity, 0, 127, 0, 8);
		Serial3.write(dataTX81Z);
		Serial3.write(0xF7);
	}
}

/////////////////////////////////////////////////////////////////////////////////////
// KANNIBAL 2C19
// MODIF DU RECEIVE CHANNEL à la relecture du manuel TX81Z
// Ajout de
// l'ancienne version était utile pour un envoi d'info d'un pad de CC standard
// celle-ci est supposée être polyvalente, envoi de LFO
// de plus j'ai appris entre-temps à configurer mon pad UC33 !!!
/////////////////////////////////////////////////////////////////////////////////////

void convertData1and2toTX81ZModif(byte note, byte velocity, byte channel = 0)
{

	if (note < 100)
	{
		// VCED
		byte OP;
		byte subOP;
		byte sub = velocity;

		if (note < 52)
		{
			OP = note / 13;
			OP = 4 - OP;

			subOP = note % 13;

			switch (subOP)
			{
			case 0:
				sub = map(sub, 0, 127, 0, 31);
				break;
			case 1:
				sub = map(sub, 0, 127, 0, 31);
				break;
			case 2:
				sub = map(sub, 0, 127, 0, 31);
				break;
			case 3:
				sub = map(sub, 0, 127, 1, 15);
				break;
			case 4:
				sub = map(sub, 0, 127, 0, 15);
				break;
			case 5:
				sub = map(sub, 0, 127, 0, 99);
				break;
			case 6:
				sub = map(sub, 0, 127, 0, 3);
				break;
			case 7:
				sub = map(sub, 0, 127, 0, 7);
				break;
			case 8:
				sub = map(sub, 0, 127, 0, 1);
				break;
			case 9:
				sub = map(sub, 0, 127, 0, 7);
				break;
			case 10:
				sub = map(sub, 0, 127, 0, 99);
				break;
			case 11:
				sub = map(sub, 0, 127, 0, 63);
				break;
			case 12:
				sub = map(sub, 0, 127, 0, 6);
				break;
			}
		}

		switch (note)
		{
		case 52:
			sub = map(sub, 0, 127, 0, 7);
			break;
		case 53:
			sub = map(sub, 0, 127, 0, 7);
			break;
		case 54:
			sub = map(sub, 0, 127, 0, 99);
			break;
		case 55:
			sub = map(sub, 0, 127, 0, 99);
			break;
		case 56:
			sub = map(sub, 0, 127, 0, 99);
			break;
		case 57:
			sub = map(sub, 0, 127, 0, 99);
			break;
		case 58:
			sub = map(sub, 0, 127, 0, 1);
			break;
		case 59:
			sub = map(sub, 0, 127, 0, 3);
			break;
		case 60:
			sub = map(sub, 0, 127, 0, 7);
			break;
		case 61:
			sub = map(sub, 0, 127, 0, 3);
			break;
		case 62:
			sub = map(sub, 0, 127, 0, 48);
			break;
		case 63:
			sub = map(sub, 0, 127, 0, 1);
			break;
		case 64:
			sub = map(sub, 0, 127, 0, 12);
			break;
		case 65:
			sub = map(sub, 0, 127, 0, 1);
			break;
		case 66:
			sub = map(sub, 0, 127, 0, 99);
			break;
		case 67:
			sub = map(sub, 0, 127, 0, 99);
			break;
		case 68:
			sub = map(sub, 0, 127, 0, 1);
			break;
		case 69:
			sub = map(sub, 0, 127, 0, 1);
			break;
		case 70:
			sub = map(sub, 0, 127, 0, 1);
			break;
		case 71:
			sub = map(sub, 0, 127, 0, 99);
			break;
		case 72:
			sub = map(sub, 0, 127, 0, 99);
			break;
		case 73:
			sub = map(sub, 0, 127, 0, 99);
			break;
		case 74:
			sub = map(sub, 0, 127, 0, 99);
			break;
		case 75:
			sub = map(sub, 0, 127, 0, 99);
			break;
		case 76:
			sub = map(sub, 0, 127, 0, 99);
			break;
		case 93:
			sub = map(sub, 0, 127, 0, 15);
			break;
		default:
			break;
		}

		Serial3.write(0xF0);		   // Exclusive
		Serial3.write(0x43);		   // Yamaha ID
		Serial3.write(0x10 + channel); // 1n : n = basic receive channel
		Serial3.write(0x12);		   // VCED
		Serial3.write(note);
		Serial3.write(sub);
		Serial3.write(0xF7);
	}
	else
	{
		// ACED

		note = note - 100;

		byte OP;
		byte subOP;
		byte sub = velocity;

		if (note < 20)
		{
			OP = note / 5;
			OP = 4 - OP;

			subOP = note % 5;

			switch (subOP)
			{
			case 0:
				sub = map(sub, 0, 127, 0, 1);
				break;
			case 1:
				sub = map(sub, 0, 127, 1, 7);
				break;
			case 2:
				sub = map(sub, 0, 127, 0, 15);
				break;
			case 3:
				sub = map(sub, 0, 127, 0, 7);
				break;
			case 4:
				sub = map(sub, 0, 127, 0, 3);
				break;
			}
		}

		switch (note)
		{
		case 20:
			sub = map(sub, 0, 127, 0, 7);
			break;
		case 21:
			sub = map(sub, 0, 127, 0, 99);
			break;
		case 22:
			sub = map(sub, 0, 127, 0, 99);
			break;
		default:
			break;
		}

		Serial3.write(0xF0);		   // Exclusive
		Serial3.write(0x43);		   // Yamaha ID
		Serial3.write(0x10 + channel); // 1n : n = basic receive channel
		Serial3.write(0x13);		   // ACED
		Serial3.write(note);
		Serial3.write(sub);
		Serial3.write(0xF7);
	}
}

/////////////////////////////////////////////////////////////////////////////////////
// KANNIBAL 2C19
// adaptation du TX81Z vers SHERMAN
/////////////////////////////////////////////////////////////////////////////////////

void convertData1and2toSHERMAN(byte note, int velocity, byte channel = 0)
{

	// note = n° du CC
	// velocity = valeur du CC

	switch (note)
	{
	case 0:
		break;
	case 1:
		// affiche += "INPUT"; OK
		Serial3.write((byte)0xBF);
		Serial3.write((byte)0x07);
		Serial3.write((byte)velocity);
		break;
	case 2:
		// affiche += "FREQ1"; OK
		Serial3.write((byte)0xEF);
		// Serial3.write((byte)0x07);   // low
		// Serial3.write((byte)velocity);   // middle ?   4096
		// Serial3.write((int16_t)velocity);   // DEBUG !!!! attention, on envoie ici sur 2 bytes
		Serial3.write((byte)(velocity >> 8));
		Serial3.write((byte)(velocity & B11111111));

		break;
	case 3:
		// affiche += "FM1"; OK
		Serial3.write((byte)0xBF);
		Serial3.write((byte)0x04);
		Serial3.write((byte)velocity);
		break;
	case 4:
		// affiche += "RESO1"; OK
		Serial3.write((byte)0xDF);
		Serial3.write((byte)velocity);
		break;
	case 5:
		// affiche += "ATTACK1"; OK
		Serial3.write((byte)0xBF);
		Serial3.write((byte)0x05);
		Serial3.write((byte)velocity);
		break;
	case 6:
		// affiche += "DECAY1"; OK
		Serial3.write((byte)0xBF);
		Serial3.write((byte)0x10);
		Serial3.write((byte)velocity);
		break;
	case 7:
		// affiche += "RELEASE1"; OK
		Serial3.write((byte)0xBF);
		Serial3.write((byte)0x11);
		Serial3.write((byte)velocity);
		break;
	case 8:
		// affiche += "FREQ2"; OK
		Serial3.write((byte)0xBF);
		Serial3.write((byte)0x01);
		Serial3.write((byte)velocity);
		break;
	case 9:
		// affiche += "AM2"; OK
		Serial3.write((byte)0xBF);
		Serial3.write((byte)0x0B);
		Serial3.write((byte)velocity);
		break;
	case 10:
		// affiche += "RESO2"; OK
		Serial3.write((byte)0xBF);
		Serial3.write((byte)0x02);
		Serial3.write((byte)velocity);
		break;
	case 11:
		// affiche += "ATTACK2"; OK
		Serial3.write((byte)0xBF);
		Serial3.write((byte)0x12);
		Serial3.write((byte)velocity);
		break;
	case 12:
		// affiche += "RELEASE2"; OK
		Serial3.write((byte)0xBF);
		Serial3.write((byte)0x13);
		Serial3.write((byte)velocity);
		break;
	default:
		// affiche += "???";
		break;
	}
}

void daubyProcedures()
{

	/* pour le développement du stochastic Dauby, avant l'implémentation des Event Layer
	ajouter des modifications de valeur pour
	la valeur random de split note, RndUp, Shrink+valeur du Shrink, decimate, deplace
	à plus long terme:  il doit modifier des valeurs de VOICES, pas que VCED, mais aussi ACED, de I01 à I32
	ensuite dans le mode Performance, il charge les voices et joue de plusieurs Voix, qu'il modifie progressivement
	ajouter un autoharmonize ON OFF
	*/

	//   if ( random(10) == 2 ) {      // Modif TX81Z valeur
	//     Transformation(75, random(10) ) ;
	//   } // on pourrait passer en argument le nb de modif subies

	if (random(10) == 2)
	{ // Split note
		Transformation(76, 100);
	} // 100 sur 1023 deviendra environ 10%

	if (random(10) == 2)
	{ // RndUp
		Transformation(77, 100);
	} //

	if (random(30) == 2)
	{ // Shrink
		Transformation(37, random(1000));
	} //

	if (random(20) == 2)
	{ // decimate
		Transformation(10, 40);
	} //

	if (random(20) == 2)
	{ // deplace %
		Transformation(79, 100);
	} //

	if (random(5) == 2)
	{ //  modification du choix de layer
		Transformation(31, random(1000));
	} //
}

void randomizeNoteRedirect()
{
	bool noteDone[12]{(false)};
	// NOTE **** remplacer par calculs sur un int avec >> i

	for (byte i = 0; i < 12; i++)
	{
		byte chosen(0);
		// d'abord trouver une valeur qui n'a pas encore été choisie
		do
		{
			chosen = random(0, 12);
		} while (noteDone[chosen] == true);

		noteDone[chosen] = true;

		// ensuite calculer la différence entre la valeur normale 0, 1, 2, 3 et ce qu'il faut pour atteindre la nouvelle valeur

		// bool currentStatus = isSilentSByte(noteRedirect[noteToWorkOn] );
		bool currentStatus = false; // on ne garde pas silencieuses les notes qui le sont
		noteRedirect[i] = makeSByte((int)((int)chosen - (int)i), currentStatus);
	}
}

long ultraMap(long depart, long sourceA, long sourceB, long targetA, long targetB, float divisionUltraMap)
{
	/**
	* version 3.0
	* @param [name] [description]
	*/
	// les valeurs 5% inférieur de depart vaudront targetA
	// les valeurs 5% supérieur de depart vaudront targetB
	// les valeurs intermédiaires auront un map classique vers targetA targetB
	//     // // ATTENTION : pour 5% de marge, on a divisionUltraMap = 20
	// pour une marge très petite, il faut metttre un divisionUltraMap très GRAND
	// si on met divisionUltraMap = 1, on a une onde carrée !
	// version 3.0 : on pas le DivisionUltraMap en données, car on peut avoir souhaits différents
	// NOTE : mis long pour la valeur de xsize 99999 , sinon int irait très bien
	//
	long calculA, calculB;

	if (divisionUltraMap != 0.0)
	{
		calculA = sourceA + ((sourceB - sourceA) / divisionUltraMap);
		calculB = sourceB - ((sourceB - sourceA) / divisionUltraMap);
	}
	else
	{
		calculA = sourceA;
		calculB = sourceB;
	}

	if (depart < calculA)
	{
		return targetA;
	}
	if (depart > calculB)
	{
		return targetB;
	}

	return map(depart, calculA, calculB, targetA, targetB);
}

void actualizeTimeToPlay(byte i, byte j)
// DEBUG utilisé juste 1 fois dans Affichage, fonction 204
// peut-être superflu...
{
	pileNote tempNote;
	tempNote = ZEROgetNoteFromPile(pInitial[i], j);
	pileNote_actualizeTimeToPlay(tempNote);
	ZEROSetNoteInPile(pInitial[i], j, tempNote);
}

void pileNote_actualizeTimeToPlay(pileNote &cetteNote)
{

	// unsigned long nowMillis = nowMillis;
	float realRatio;
	unsigned long actualizedlayerLength;

	byte i = cetteNote.layer;

	unsigned long localStartNote;
	localStartNote = cetteNote.startNote;

	int nowDrunkMin = getValueStraightOrFromSequence(drunkMin[i], -1000);
	int nowDrunkMax = getValueStraightOrFromSequence(drunkMax[i], -1000);
	int nowDrunkStep = getValueStraightOrFromSequence(drunkStep[i], 0);

	if (dynamicSwing[cetteNote.layer] == true && cetteNote.type != 0xB0)
	{
		localStartNote = dynamicTreatNote_pileNote_SIMPLIFIED(cetteNote);
	}

	realRatio = (float)(((float)ratioCurrent[i] * (float)ratioLength[i]) / (float)100.0);
	actualizedlayerLength = int((float)(((float)layerLength[i] * (float)realRatio) / (float)100));

	// Serial.println(actualizedlayerLength) ;

	unsigned long debutThisSegment = nowMillis - ((nowMillis) % actualizedlayerLength);
	unsigned long endThisSegment = debutThisSegment + actualizedlayerLength;

	// T2C16 : modification : si la valeur calculée est pas encore passée, dans ce segment, sinon dans segment suivant !
	// normalement on appelle cette fonction une fois que la note a été jouée, dans le segment actuel !
	// on calcule donc la valeur dans le prochain segment !
	// pose problème avec le poof : on peut avoir une note jouée une première fois, avant le moment initialement prévu
	// si on recalcule, on risque une deuxième note dans le même segment !

	//	cetteNote.timeToPlayON = debutThisSegment + (unsigned long)(((float)localStartNote * (float)realRatio) / (float)100);

	// if ( cetteNote.timeToPlayON < nowMillis )
	{
		cetteNote.timeToPlayON = endThisSegment + (unsigned long)(((float)localStartNote * (float)realRatio) / (float)100);
	}

	// if (cetteNote.thereIsAnEnd)
	{
		// cetteNote.timeToPlayOFF = endThisSegment + (unsigned long)(((float)cetteNote.endNote * (float)realRatio) / (float)100);
		// TODO DEBUG
		// TESTER SI IL Y A DEJA Un ENDNOTE TODO DEBUG
	}

	// TODO DEBUG :
	// problème avec l'actualistion : elle doit se passer en 2 fois
	// uen fois pour le Note ON, une autre fois pour le Note OFF
	// or les valeurs peuvent être différentes, ce qui est un problème
	// il faut donc enregistrer la valeur du Poof avec le Note ON et mettre à jour le Note OFF avec cette valeur mémorisée

	// on doit quand même faire ceci au cas où on aurait affaire à une machine qui n'envoie que des note ON et pas de OFF
	if (turnOnBipolarRandom && cetteNote.type != 0xB0)
	{
		signed int poof_;
		poof_ = giveBipolarRandomForLayer(i);
		cetteNote.timeToPlayON += poof_;
		// cetteNote.timeToPlayOFF += poof_;
	}

	if (turnOnPoof && cetteNote.type != 0xB0)
	{
		int nowPoof = getValueStraightOrFromSequence(poof[i], 0);
		signed int poof_;
		poof_ = random(nowPoof);
		if (random(2) == 0)
		{
			poof_ = -poof_;
		}
		cetteNote.timeToPlayON += poof_;
		// cetteNote.timeToPlayOFF += poof_;
	}

	//2C31 Sequential Postpone
	if (turnOnSequentialPostpone[i] == true && cetteNote.type != 0xB0)
	{
		signed int poof_;
		poof_ = giveNextValue(sequence[seqPostpone[i]], sequence[seqPostpone[i]].compteur);
		sequence[seqPostpone[i]].compteur++;
		//	Serial.print( poof_ ) ;
		//	Serial.print(" ");
		cetteNote.timeToPlayON += poof_;
		// cetteNote.timeToPlayOFF += poof_;
	}

	if (drunk[i] && cetteNote.type != 0xB0)
	{
		signed int poof_;
		poof_ = currentDrunkValue[i];

		if (random(0, 2) == 0)
		{
			currentDrunkValue[i] += nowDrunkStep;
		}
		else
		{
			currentDrunkValue[i] -= nowDrunkStep;
		}
		if (currentDrunkValue[i] > nowDrunkMax)
		{
			currentDrunkValue[i] = nowDrunkMax;
		}
		if (currentDrunkValue[i] < nowDrunkMin)
		{
			currentDrunkValue[i] = nowDrunkMin;
		}

		cetteNote.timeToPlayON += poof_;
		// cetteNote.timeToPlayOFF += poof_;
	}

	// Serial.print("=");

	// return actualizedlayerLength;
}

bool timeToPlay(pileNote &thisOne)
// la boucle principale teste ceci : si OK va jouer la note
// mais ceci étant dit, on prépare ici le moment suivant où elle sera jouée
// il faut donc dans le cadre du live swing, tenir compte de celui-ci !
{

	// unsigned long nowMillis = nowMillis;

	// 2C21
	if (nowMillis > thisOne.timeToPlayON)
	{
		// dans un premier temps on va faire les calcul de swing si nécessaire
		// ensuite on passe aux calculs de compression de vitesse

		// on prend la valeur .start et .end
		// et on voit vers où cela est modifié par le swing

		unsigned long localStartNote;
		localStartNote = thisOne.startNote;

		byte i = thisOne.layer;

		int nowDrunkMin = getValueStraightOrFromSequence(drunkMin[i], -1000);
		int nowDrunkMax = getValueStraightOrFromSequence(drunkMax[i], -1000);
		int nowDrunkStep = getValueStraightOrFromSequence(drunkStep[i], 0);

		if (dynamicSwing[i] == true && thisOne.type != 0xB0)
		{
			localStartNote = dynamicTreatNote_pileNote_SIMPLIFIED(thisOne);
		}

		float realRatio;
		unsigned int actualizedlayerLength;

		realRatio = (float)(((float)ratioCurrent[i] * (float)ratioLength[i]) / (float)100.0);
		actualizedlayerLength = int((float)(((float)layerLength[i] * (float)realRatio) / (float)100));

		// Serial.println(actualizedlayerLength) ;

		// TODO DEBUG peut être à décaler avec decalageLayer ?
		unsigned long debutThisSegment = nowMillis - ((nowMillis) % actualizedlayerLength);
		unsigned long endThisSegment = debutThisSegment + actualizedlayerLength;

		thisOne.timeToPlayON = endThisSegment + (unsigned long)(((float)localStartNote * (float)realRatio) / (float)100);

		if (turnOnBipolarRandom && thisOne.type != 0xB0)
		{
			signed int poof_;
			poof_ = giveBipolarRandomForLayer(i);
			thisOne.timeToPlayON += (unsigned long)poof_;
			//	thisOne.timeToPlayOFF += (unsigned long)poof_; // TODO supprimer : n'est pas vraiment effectif ici
			// le calcul de la fin de note a lieu quand une note est vraiment jouée, dans la boucle principale,
			// en comptant le vrai début et ajouter la durée de note
		}

		if (turnOnPoof && thisOne.type != 0xB0)
		{
			int nowPoof = getValueStraightOrFromSequence(poof[i], 0);
			signed int poof_;
			poof_ = random(nowPoof);
			if (random(2) == 0)
			{
				poof_ = -poof_;
			}
			thisOne.timeToPlayON += (unsigned long)poof_;
			//	thisOne.timeToPlayOFF += (unsigned long)poof_; // TODO supprimer
		}

		//2C31 Sequential Postpone
		if (turnOnSequentialPostpone[i] == true && thisOne.type != 0xB0)
		{
			signed int poof_;
			poof_ = giveNextValue(sequence[seqPostpone[i]], sequence[seqPostpone[i]].compteur);
			sequence[seqPostpone[i]].compteur++;

			thisOne.timeToPlayON += poof_;
			// cetteNote.timeToPlayOFF += poof_;
		}

		if (drunk[i] && thisOne.type != 0xB0)
		{
			signed int poof_;
			poof_ = currentDrunkValue[i];

			if (random(0, 2) == 0)
			{
				currentDrunkValue[i] += nowDrunkStep;
			}
			else
			{
				currentDrunkValue[i] -= nowDrunkStep;
			}
			if (currentDrunkValue[i] > nowDrunkMax)
			{
				currentDrunkValue[i] = nowDrunkMax;
			}
			if (currentDrunkValue[i] < nowDrunkMin)
			{
				currentDrunkValue[i] = nowDrunkMin;
			}

			thisOne.timeToPlayON += poof_;
			// cetteNote.timeToPlayOFF += poof_;
		}

		return true;
	}

	return false;
}

int giveBipolarRandomForLayer(byte i)
{

	int nowDureePole1 = getValueStraightOrFromSequence(DureePole1[i], 0);
int nowDureePole2 = getValueStraightOrFromSequence(DureePole2[i], 0);
int nowMinPole1 = getValueStraightOrFromSequence(minPole1[i], -10);
int nowMaxPole1 = getValueStraightOrFromSequence(maxPole1[i], -10);
int nowMinPole2 = getValueStraightOrFromSequence(minPole2[i], -10);
int nowMaxPole2 = getValueStraightOrFromSequence(maxPole2[i], -10);
int nowFreqPole1 = getValueStraightOrFromSequence(freqPole1[i], 0);
int duree;

if ((int)random(100) < nowFreqPole1)
{
	// on attribue une durée en fc des critères pole1
	// DEBUG vérifier que random fonctionne avec des chiffres négatifs
	int a = random(nowMinPole1, nowMaxPole1 + 1);
	duree = a * nowDureePole1;
	}
	else
	{
		// on attribue une durée en fc des critères pole2
		int a = random(nowMinPole2, nowMaxPole2 + 1);
		duree = a * nowDureePole2;
	}

	return duree;
}

byte FindLowestVelocite(byte i)
{ // va trouver la note la plus basse du layeri

	byte Lownote = 200; // on part d'une valeur impossible

	unsigned int taille = mesureTaillePileComplete(pInitial[i]);
	pileNote tempNote;

	for (byte j = 0; j < taille; j++)
	{
		tempNote = ZEROgetNoteFromPile(pInitial[i], j);

		// si valeur plus petite que LOW mais pas nulle alors LOW devient valeur

		if ((tempNote.velocity < Lownote) && (tempNote.velocity != 0))
		{
			Lownote = tempNote.velocity;
		}
	}
	// peut ^^etre faudra t il donner une valeur par défaut si est resté 200

	return Lownote;
}

byte FindHighestVelocite(byte i)
{ // va trouver la note la plus haute du layeri

	byte Highnote = 0; // on part du plus bas !!! mais attention un layer vide renverra 0 !!!
	// la lowest des active layer doit être la plus basse exceptée 0 !
	//  bool found = false ;

	unsigned int taille = mesureTaillePileComplete(pInitial[i]);
	pileNote tempNote;

	for (byte j = 0; j < taille; j++)
	{
		tempNote = ZEROgetNoteFromPile(pInitial[i], j);

		if (tempNote.velocity > Highnote)
		{
			Highnote = tempNote.velocity;
			// found = true ;
		}
	}

	// peut ^^etre faudra t il donner une valeur par défaut si found = false !!!

	return Highnote;
}

byte FindLowestVelocityOfActive(void)
{ // va trouver la note la plus basse du layeri
	byte allLowest = 200;

	// DEBUG NOTE **** pas bon, car parfois un layer est vide.   il retourne alors 200 c'set ridicule

	for (byte i = 0; i < nbLayer; i++)
	{

		if (isLayerActive(i))
		{
			byte lowest = FindLowestVelocite(i);
			allLowest = ((allLowest > lowest) ? lowest : allLowest);
		}
	}
	return allLowest;
}

byte FindHighestVelocityOfActive(void)
{ // va trouver la note la plus basse du layeri
	byte allHighest = 0;
	// bool found = false ;

	for (byte i = 0; i < 4; i++)
	{

		if (isLayerActive(i))
		{
			byte highest = FindHighestVelocite(i);
			allHighest = ((allHighest < highest) ? highest : allHighest);
		}
	}
	return allHighest;
}

// bool mustKillNote( pileNote thisNote )
// {

// i = thisNote.layer ;

// 	if (isLayerActive(i))
// 	{
// 		if (killingNote == true &&
// 			thisNote.note == noteToKill)
// 		{
// 			return true;
// 		}
// 	}

// 	return false;
// }

bool mustKillNote(pileNote &thisNote)
{
	if (killingNote != true)
	{
		return false;
	}
	if (thisNote.note != noteToKill)
	{
		return false;
	}
	byte i = thisNote.layer;
	if (isLayerActive(i) == false)
	{
		return false;
	}
	return true;
}

// réécrit en mettant le moins probable en premier test !

bool isNoteCellConsolidated(byte channel, byte n)
{
	for (byte i = 0; i < nbValeursDansCell; i++)
	{
		if (tableauCell[i].noteIn == n && tableauCell[i].channelIn == channel)
		{
			return true;
		}
	}
	return false;
}

byte giveChannelOUTForNoteCellConsolidated(byte channel, byte n)
{
	for (byte i = 0; i < nbValeursDansCell; i++)
	{
		if (tableauCell[i].noteIn == n && tableauCell[i].channelIn == channel)
		{
			return tableauCell[i].channelOut;
		}
	}
	return 0;
}

byte giveNoteOUTForNoteCellConsolidated(byte channel, byte n)
{
	for (byte i = 0; i < nbValeursDansCell; i++)
	{
		if (tableauCell[i].noteIn == n && tableauCell[i].channelIn == channel)
		{
			return tableauCell[i].noteOut;
		}
	}
	return 0;
}

byte givePlaceInTableauForNoteCellConsolidated(byte channel, byte n)
{
	for (byte i = 0; i < nbValeursDansCell; i++)
	{
		if (tableauCell[i].noteIn == n && tableauCell[i].channelIn == channel)
		{
			return i;
		}
	}
	return 0;
}

String transformSequenceEnString(sequenceStruct &tab)
{
	String versionTemporaire = F("(");

	for (byte i = 0; i < tab.tailleSeq; i++)
	{
		if (tab.tableauSeq[i] > 0)
		{
			versionTemporaire += F("+");
			versionTemporaire += String(tab.tableauSeq[i]);
		}
		else
		{
			versionTemporaire += String(tab.tableauSeq[i]);
		}

		if (i != (tab.tailleSeq - 1))
		{
			versionTemporaire += F(",");
		}
	}
	versionTemporaire += F(")");
	return versionTemporaire;
}

void fillSequenceRandom(sequenceStruct &tab, int minF, int maxF)
{

	for (byte i = 0; i < tab.tailleSeq; i++)
	{
		tab.tableauSeq[i] = random(minF, maxF);
	}
}

void fillSequenceLinear(sequenceStruct &tab, int start)
{

	for (byte i = 0; i < tab.tailleSeq; i++)
	{
		tab.tableauSeq[i] = start + i;
	}
}

void addValue2Sequence(sequenceStruct &tab, int value)
{
	byte taille = tab.tailleSeq;
	tab.tableauSeq[taille] = value;
	tab.tailleSeq++;
}

void removeValue2Sequence(sequenceStruct &tab)
{
	if (tab.tailleSeq != 0)
	{
		tab.tailleSeq--;
	};
}

void shiftRightSequence(sequenceStruct &tab)
{
	byte taille = tab.tailleSeq;

	// [2][45][66]
	// exemple de format Sequence

	int temp;

	temp = tab.tableauSeq[taille - 1];

	for (byte i = tab.tailleSeq; i > 1; i--)
	{
		tab.tableauSeq[i - 1] = tab.tableauSeq[i - 2];
	}

	tab.tableauSeq[0] = temp;
}

void shiftLeftSequence(sequenceStruct &tab)
{
	byte taille = tab.tailleSeq;

	// [2][45][66]
	// exemple de format Sequence

	int temp;

	temp = tab.tableauSeq[0];

	for (byte i = 0; i < (tab.tailleSeq - 1); i++)
	{
		tab.tableauSeq[i] = tab.tableauSeq[i + 1];
	}

	tab.tableauSeq[taille - 1] = temp;
}

void scrambleSequence(sequenceStruct &tab)
{

	byte taille = tab.tailleSeq;

	// [2][45][66]
	// exemple de format Sequence

	int temp;
	byte a, b;

	if (tab.tailleSeq > 1)
	{
		for (byte i = 0; i < tab.tailleSeq * 2; i++)
		{
			a = random(0, taille);
			b = random(0, taille);

			temp = tab.tableauSeq[a];
			tab.tableauSeq[a] = tab.tableauSeq[b];
			tab.tableauSeq[b] = temp;
		}
	}
}

void revertSequence(sequenceStruct &tab)
{

	byte taille = tab.tailleSeq;
	sequenceStruct tableauSequenceTemp;

	// [2][45][66]
	// exemple de format Sequence

	if (tab.tailleSeq > 1)
	{
		for (byte i = 0; i < (tab.tailleSeq); i++)
		{
			tableauSequenceTemp.tableauSeq[i] = tab.tableauSeq[taille - i - 1];
		}

		for (byte i = 0; i < (tab.tailleSeq); i++)
		{
			tab.tableauSeq[i] = tableauSequenceTemp.tableauSeq[i];
		}
	}
}

void mirrorSequence(sequenceStruct &tab)
{

	// byte taille = tab.tailleSeq;
	int valeurMax(0), valeurMin(0);

	// [2][45][66]
	// exemple de format Sequence

	if (tab.tailleSeq > 1)
	{

		// d'abord trouvers les min et max
		valeurMax = tab.tableauSeq[0];
		valeurMin = tab.tableauSeq[0];

		for (byte i = 0; i < tab.tailleSeq; i++)
		{
			if (tab.tableauSeq[i] > valeurMax)
			{
				valeurMax = tab.tableauSeq[i];
			}
			if (tab.tableauSeq[i] < valeurMin)
			{
				valeurMin = tab.tableauSeq[i];
			}
		}

		for (byte i = 0; i < tab.tailleSeq; i++)
		{
			tab.tableauSeq[i] = valeurMax - (tab.tableauSeq[i] - valeurMin);
		}
	}
}

void doubleSequence(sequenceStruct &tab)
{
	// [2][45][66]
	// exemple de format Sequence
	byte oldTaille = tab.tailleSeq;
	byte newTaille = 2 * oldTaille;
	if (newTaille > tailleMaxTableauSequence)
	{
		newTaille = tailleMaxTableauSequence;
	}
	tab.tailleSeq = newTaille;

	for (byte i = 0; i < oldTaille; i++)
	{
		if (oldTaille + 1 + i < tailleMaxTableauSequence)
		{ // on veille à ne pas remplir un array au delà de sa taille
			tab.tableauSeq[oldTaille + i] = tab.tableauSeq[i];
		}
	}
}

void plusminSequence(sequenceStruct &tab)
{

	// byte taille = tab.tailleSeq;

	// [2][45][66]
	// exemple de format Sequence

	if (tab.tailleSeq > 1)
	{

		// d'abord trouvers les min et max

		for (byte i = 0; i < tab.tailleSeq; i++)
		{
			tab.tableauSeq[i] = -tab.tableauSeq[i];
		}
	}
}

// int giveNextValue(  int (&tab)[tailleMaxTableau] , long compteurThat, byte mode ) {

int tailleBetween2Numbers(int a, int b) // fonction utilisée par GiveNextValue : donne distance entre 2 chiffres, sauf si IDEM
// si ils sont idntiques, donnera 1, pour préserver l'apparition de 2x le même chiffre dans une séquence

{
	int c = b - a;

	c = abs(c);

	if (c == 0)
	{
		c++;
	}

	return c;
}

int giveNextValue(sequenceStruct(&tab), long compteurThat)
{
	// on repère sur le compteur à quelle valeur du tableau on fera référence
	// le nb de note est donné par tab[0]
	// on fait donc compteur % tab[0] pour avoir découpage : 0, 1        0, 1, 2, .. nb notes-1
	// si FWD -> on donne valeur du modulo + 1
	// si BACK -> on donnne valeur de taille - modulo
	// si FWDBACK -> on doit calculer un modulo par ( tab[0]*2 ) si le résultat <= tab[0] alors calcul FWD, sinon calcul BACK
	// si RANDOM -> on donne une valeur random parmi le tableau
	// si JITTER -> on devrait savoir à quelle fréquence le jitter change de sens
	// case 6:	affiche += F("7.progression a->b->c");
	// case 7:	affiche += F("8.rnd spectum a-b");
	// case 8:	affiche += F("9.bipolar A *c-d E*f-g H%deA");
	// case 9:	affiche += F("10.bipolar A B C%deA");
	// case 10:	affiche += F("11.n repeater");
	// 11   -> 12.Markov <-A/100 ->B/100 self/100


	int decoupe;
	int taille = tab.tailleSeq;
	int mode = tab.modeSeq;

	if (taille == 0)
	{
		return 0;
	}

	if (taille == 1)
	{
		return tab.tableauSeq[0];
	}

	if (taille > 1)
	{

		decoupe = (int)((long)compteurThat % (long)taille);

		if (mode == modeFWD)
		{
			return tab.tableauSeq[decoupe];
		}
		else if (mode == modeBACK)
		{
			return tab.tableauSeq[taille - decoupe - 1];
		}
		else if (mode == modeRANDOM)
		{
			return tab.tableauSeq[random(0, taille)];
		}
		else if (mode == modeALLERRETOUR)
		{
			/* [3][45][66][22][31]
				// exemple de format Sequence

				// on veut avoir 45 66 22 66 45 66 22 66 45 etc
				// on veut avoir 45 66 22 66 / 45 66 22 66 / 45 etc
				// decoupeAR vaut 0  1  2  3    0  1  2  3
				// c'est dans le fond une séquence de longueur : (taille *2 ) - 2 car la première et dernière note ne sont pas répétées
				// on calcule un decoupeAR
				// pour le decoupeAR < taille, on joue tab[decoupeAR+1] ( = modeFWD )
				// pour le decoupeAR >= taille, on joue tab[(taille *2 ) - 2-decoupeAR+1]

				// [45][66][22][31]
				// [45][66][22][31][22][66]/[45][66][22][31][22][66]/[45][66][22][31][22][66]
				// DecoupeAR
				//   0  1   2   3   4    5    0  1   2   3   4    5   0  1   2   3   4    5
				//   x   x  x   x              x  x    x x              x  x   x x
				2    1                   2    1
				*/

			byte tailleAR = (taille * 2) - 2;
			byte decoupeAR = (byte)(compteurThat % tailleAR);

			if (decoupeAR < taille)
			{
				return tab.tableauSeq[decoupeAR];
			}
			else
			{
				return tab.tableauSeq[tailleAR - decoupeAR];
			}
		}
		else if (mode == modeJITTER)
		{

			if (tab.jitterWhereTo < 0)
			{
				// Serial.println("WOW problem !!! WhereTo négatif ou nul ");
				while (tab.jitterWhereTo < 0)
				{
					tab.jitterWhereTo += taille;
				};
				tab.jitterWhereTo = abs(tab.jitterWhereTo % taille);
			}

			if (tab.jitterWhereNow < 0)
			{
				// Serial.println("WOW problem !!! WhereNow négatif !");
				tab.jitterWhereNow = 0; // 2C03 ???
			}

			if (tab.beingStatic)
			{
				//Serial.println("Being Static ");
				tab.jitterStaticCompteur++;

				if (tab.jitterStaticCompteur >= tab.jitterStaticToReach)
				{
					//Serial.println("Leaving Static ");
					tab.beingStatic = false;
					tab.jitterRichting = -tab.jitterRichting; // on change de sens
					//Serial.println("changing direction ");

					if (tab.jitterRichting > 0)
					{
						tab.jitterWhereTo += (random(tab.jitterMinUP, tab.jitterMaxUP + 1));
					}
					else
					{
						tab.jitterWhereTo -= (random(tab.jitterMinDOWN, tab.jitterMaxDOWN + 1));
					}
					// Serial.println("1.Going to : ");
					//  Serial.println(tab.jitterWhereTo);

					while (tab.jitterWhereTo < 0)
					{
						tab.jitterWhereTo += taille;
					};
					// Serial.println("2.Going to : ");
					// Serial.println(tab.jitterWhereTo);

					tab.jitterWhereTo = tab.jitterWhereTo % taille;
					// Serial.println("3.Going to : ");
					// Serial.println(tab.jitterWhereTo);
				}
			}

			else // çàd NON STATIQUE, est en train de bouger
			{
				tab.jitterWhereNow += tab.jitterRichting;

				// Serial.println("1.Now  : ");
				// Serial.println(tab.jitterWhereNow);

				while (tab.jitterWhereNow < 0)
				{
					tab.jitterWhereNow += taille;
				};
				//  Serial.println("2.Now  : ");
				//  Serial.println(tab.jitterWhereNow);

				tab.jitterWhereNow = tab.jitterWhereNow % taille;
				//  Serial.println("3.Now  : ");
				//  Serial.println(tab.jitterWhereNow);

				if (tab.jitterWhereNow == tab.jitterWhereTo)
				{
					// ça signifie qu'on est arrivé sur la cible
					// soit on passe en statique, soit on inverse
					//  Serial.println("Reached Goal");

					tab.jitterStaticToReach = random(tab.jitterStaticMin, tab.jitterStaticMax + 1);

					if (tab.jitterStaticToReach > 0)
					{
						//    Serial.println("Switched to Static" );

						tab.jitterStaticCompteur = 0;
						tab.beingStatic = true;
					}
					else
					{
						//  Serial.println("Not being Static" );

						tab.beingStatic = false;
						tab.jitterRichting = -tab.jitterRichting; // on change de sens
						// Serial.println("changing direction " );

						if (tab.jitterRichting > 0)
						{
							tab.jitterWhereTo += (random(tab.jitterMinUP, tab.jitterMaxUP + 1));
						}
						else
						{
							tab.jitterWhereTo -= (random(tab.jitterMinDOWN, tab.jitterMaxDOWN + 1));
						}

						//  Serial.println("1.Going to : ");
						//   Serial.println(tab.jitterWhereTo);

						while (tab.jitterWhereTo < 0)
						{
							tab.jitterWhereTo += taille;
						};
						//   Serial.println("2.Going to : ");
						//   Serial.println(tab.jitterWhereTo);

						tab.jitterWhereTo = abs(tab.jitterWhereTo % taille);
						//   Serial.println("3.Going to : ");
						//    Serial.println(tab.jitterWhereTo);
					}
				}
			}

			return (tab.tableauSeq[tab.jitterWhereNow]);
		}
		else if (mode == modeRANDOMFRAG)
		{
			// pour avoir un tableau non partagé par différentes séquences on doit le recalculer
			// préférable au fait de l'enregistrer pour chaque séquence ( 30 digits par séquence ! )

			int subSequence[tab.sizeRANDOMFRAG];

			if ((compteurThat % tab.sizeRANDOMFRAG) == 0)
			{

				if (tab.repeatRANDOMFRAG >= tab.toReachRANDOMFRAG)
				{
					tab.repeatRANDOMFRAG = 0;
				}

				if (tab.repeatRANDOMFRAG == 0)
				{
					// on initialise un nouveau fragment
					byte n = random(0, taille); // point de départ entre 1 et la dernière valeur du TAb
					tab.keepN = n;
				}

				tab.repeatRANDOMFRAG++;
			}

			for (byte i = 0; i < tab.sizeRANDOMFRAG; i++)
			{
				byte m;
				m = tab.keepN + i;
				m = m % taille;
				subSequence[i] = tab.tableauSeq[m];
			}

			return subSequence[(compteurThat % tab.sizeRANDOMFRAG)]; // redonner ce résultat, sans passer par la création d'un tableauSequenceRand !

		} // fin RandomFRAG
		else if (mode == modePROGRESSION)
		{ // ex [1,10,3] devient [1 2 3 4 5 6 7 8 9 10 9 8 7 6 5 4 3]
			// d'abord calculer la taille du tableau nécessaire
			// puis l'étendre
			// on part d'un array qui contient des valeurs négatives et positives, dont on connait la taille
			// tab1[10]
			// tab1Size
			// on veut l'étendre vers un autre array

			// tab1[] remplacé par tab.tableauSeq[]
			int tab1Size = tab.tailleSeq;

			int step = 1; // TODO on doit réfléchir à comment le modifier pour une séquence - à priori menu supplémentaire ?

			int delta = 0;

			for (int i = 0; i < tab1Size - 1; i++)

			{
				delta = delta + tailleBetween2Numbers(tab.tableauSeq[i], tab.tableauSeq[i + 1]) / step;
			}

			int tab2Size = delta + 1;
			int tab2[tab2Size] = {0};

			delta = 0;

			// ---------- ON PEAUFINE

			for (int i = 0; i < tab1Size - 1; i++)

			{
				tab2[delta] = tab.tableauSeq[i];
				int ajout = 0;

				if (tab.tableauSeq[i + 1] > tab.tableauSeq[i])
				{
					ajout = step;
				}
				else
				{
					ajout = -step;
				}

				for (int j = 0; j < tailleBetween2Numbers(tab.tableauSeq[i], tab.tableauSeq[i + 1]) / step; j++)
				{
					tab2[delta + j] = tab2[delta] + j * ajout;
				}

				delta = delta + tailleBetween2Numbers(tab.tableauSeq[i], tab.tableauSeq[i + 1]) / step;
			}

			decoupe = (byte)(compteurThat % tab2Size);
			return tab2[decoupe]; // on renvoie la valeur du tableau2, version expandue du tabSeq !
		}
		else if (mode == modeSPECTRUM)
		{ // on part du même concept que progression sauf qu'ici on choisira au hasard

			// tab1[] remplacé par tab.tableauSeq[]
			int tab1Size = tab.tailleSeq;

			int step = 1; // TODO on doit réfléchir à comment le modifier pour une séquence - à priori menu supplémentaire ?

			int delta = 0;

			for (int i = 0; i < tab1Size - 1; i++)

			{
				delta = delta + tailleBetween2Numbers(tab.tableauSeq[i], tab.tableauSeq[i + 1]) / step;
			}

			int tab2Size = delta + 1;
			int tab2[tab2Size] = {0};

			delta = 0;

			// ---------- ON PEAUFINE

			for (int i = 0; i < tab1Size - 1; i++)

			{
				tab2[delta] = tab.tableauSeq[i];
				int ajout = 0;

				if (tab.tableauSeq[i + 1] > tab.tableauSeq[i])
				{
					ajout = step;
				}
				else
				{
					ajout = -step;
				}

				for (int j = 0; j < tailleBetween2Numbers(tab.tableauSeq[i], tab.tableauSeq[i + 1]) / step; j++)
				{
					tab2[delta + j] = tab2[delta] + j * ajout;
				}

				delta = delta + tailleBetween2Numbers(tab.tableauSeq[i], tab.tableauSeq[i + 1]) / step;
			}

			// decoupe = (byte)(compteurThat % tab2Size);
			return tab2[random(0, tab2Size)]; // on renvoie la valeur du tableau2, version expandue du tabSeq !
		}
		else if (mode == modeBIPOLAR)
		// A *c-d E*f-g H%deA
		{
			if (tab.tailleSeq < 6)
			{
				printRot2("NOT ENUF NUMB", 16, 300, 3);
				// delay(1000);
				return 0;
			}

			// A *c-d E*f-g H%deA
			int duree;
			if (random(0, 100) < tab.tableauSeq[6])
			{
				// on attribue une durée en fc des critères pole1
				// DEBUG vérifier que random fonctionne avec des chiffres négatifs
				int a = random(tab.tableauSeq[1], tab.tableauSeq[2] + 1);
				duree = a * tab.tableauSeq[0];
			}
			else
			{
				// on attribue une durée en fc des critères pole2
				int a = random(tab.tableauSeq[4], tab.tableauSeq[5] + 1);
				duree = a * tab.tableauSeq[3];
			}

			return duree;
		}
		else if (mode == modeSIMPLEBIPOLAR ) // 10.bipolar A B C%deA");
		{
// TODO 
		}
		else if (mode == modeREPEATER ) // 11. n repeater
		{
// TODO 
		}
		else if (mode == modeMARKOV ) // 12.Markov <-A/100 ->B/100 self/100");
		{


// tab.tailleSeq < 6)
// tab.tableauSeq[6]
// pour savoir où on est actuellement je vais utilser .jitterNow 

// 0. déterminer si la séquence est suffisante, il faut au moins 4 chiffres
// 1. déterminer où on est
// 2. déterminer si on va vers arrière, idem, ou avant 
// 3. déterminer cette valeur, modifier le jitterwhereNow

// 0.
if (tab.tailleSeq < 6)
{
	printRot2("NOT ENUF NUMB", 16, 300, 3);
	// delay(1000);
	return 0;
}

// 1.
// int position = tab.jitterWhereNow ;

// 2.
// byte direction ; // 0 = vers plus bas 1 = idem 2 = plus haut dans le tableau

int taille = tab.tailleSeq ;
int a = tab.tableauSeq[taille - 3]; 
int b = tab.tableauSeq[taille - 2];
int c = tab.tableauSeq[taille - 1];
int sum = a+b+c ;
int choix = random(sum); 

if ( choix < a )
{
	// direction = 0 ;  // vers gauche
	tab.jitterWhereNow = (tab.jitterWhereNow - 1) % (tab.tailleSeq - 3 ) ;
}
else
if ( choix < ( a+b ))
{
	// direction = 2 ; // vers droite
	tab.jitterWhereNow = (tab.jitterWhereNow + 1) % (tab.tailleSeq - 3);
}
else 
{ // direction = 1 ; // idem
	tab.jitterWhereNow = tab.jitterWhereNow % (tab.tailleSeq - 3) ; // juste par prudence pour modulo
}

 
	return tab.tableauSeq[tab.jitterWhereNow];
}


	} // fin Taille > 1

	return 0;

} // fin GiveNextValue

String giveStringOfSequence(byte s)
{
	String affiche = nulString;

	affiche = +F("Seq");
	affiche += String(s + 1);
	affiche += "=[";

	int i(0);

	while (i < sequence[s].tailleSeq)
	{
		String ajout = String((int)sequence[s].tableauSeq[i]);

		if (sequence[s].tableauSeq[i] < floorSeq )
		{
			ajout = "Seq" + String ( -sequence[s].tableauSeq[i] + floorSeq  ) ;  
		}

			if (sequence[s].tableauSeq[i] >= 0)
			{
				ajout = "+" + ajout;
			}
		if (i != sequence[s].tailleSeq - 1)
		{
			ajout = ajout + F(",");
		}
		affiche += ajout;
		i++;
	}

	affiche += F("]");

	return affiche;
}

String giveShortStringOfSequence(byte s)
{
	String affiche = nulString;

	affiche = +F("Seq");
	affiche += String(s + 1);
	affiche += F(" ");

	return affiche;
}

String giveStringOfEvent(byte s)
{
	String affiche = nulString;

	affiche = +F("Evt");
	affiche += String(s + 1); // 1C86 c'est ici que l'affichage diffère de l'encodage
	affiche += F(" ");
	//affiche += "=[" ;

	// byte i (0) ;

	// while ( i < sequence[s].tailleSeq  ) {
	// 	String ajout = String( (int) sequence[s].tableauSeq[i]);
	// 	if ( sequence[s].tableauSeq[i] >= 0 ) { ajout = "+" + ajout ; }
	// 	if ( i != sequence[s].tailleSeq -1 ) {
	// 		ajout = ajout + F(",") ;
	// 	}
	// 	affiche += ajout ;
	// 	i++ ;
	// }
	//affiche+= "a detailler" ;
	// TODO DEBUG : detail here characteristics of an event

	//affiche += F("]");

	return affiche;
}

String giveStringOfCCMod(byte s)
{
	String affiche = nulString;

	affiche = +F("CCMod");
	affiche += String(s + 1); // 1C86 c'est ici que l'affichage diffère de l'encodage
	affiche += F(" ");

	return affiche;
}

byte dynamicHarmonize(byte note, byte layer)
{

	byte rootnote = note % 12; // k devient la note entre 0 (C) et 11 (B)

	// il faut ensuite vérifier si cette note est comprise dans la gamme indiquée par PotVal pour une note de base ( C par défaut mais un programme pourra changer )

	/* if ( rootnote > gammeActuelle ) {                 // y a-t-il ABS en arduino ?  (byte) abs ( (int) rootnote - (int) gammeActuelle )
		diffnote = rootnote - gammeActuelle ; }
		else {
		diffnote = gammeActuelle - rootnote ; }
		*/
	byte diffnote = (byte)abs((int)((int)rootnote - (int)gammeActuelle[layer]));

	while (((Scale[modeActuelle[layer]] >> diffnote) & B0001) == 0)
	{
		if (dynamicHarmonizeMode == 0)
		{
			note = note + random(-1, 2); // va augmenter ou réduire le pitch jusqu'à ce que colle à la gamme.
			if (note > 127)
			{
				note = 125;
			};
			if (note == 0)
			{
				note = 4;
			}
		}

		if (dynamicHarmonizeMode == 1)
		{
			note = note + 1; // va augmenter ou réduire le pitch jusqu'à ce que colle à la gamme.
			if (note > 127)
			{
				note = 115;
			};
		}

		if (dynamicHarmonizeMode == 2)
		{
			note = note - 1; // va augmenter ou réduire le pitch jusqu'à ce que colle à la gamme.
			if (note == 0)
			{
				note = 12;
			}
		}
		rootnote = note % 12; // rootnote devient la note entre 0 (C) et 11 (B)
		diffnote = (byte)abs(((int)rootnote - (int)gammeActuelle[layer]));
	}

	return note;
}

byte dynamicHarmonizeLive(byte note) // 2C28 basé sur dynamicHarmonize
{
	// 0 = + ou - 1 ( fluctue )
	// 1 = + 1
	// 2 = -1
	// 3 = aucun filtre
	// 4 = bloque
	// 5 = bloque inverse ( ne laisse ce qui n'en fait pas partie )

	if (typeAutoHarmonizeLivePlayingMode == 3)
	{
		return (note);
	}

	if (typeAutoHarmonizeLivePlayingMode == 4)
	{
		// bloquer ce qui est hors de gamme
		if (isNoteInScaleMode(note, gammeActuelleLive, modeActuelleLive))
		{
			return (note);
		}
		else
		{
			return (0);
		}
	}

	if (typeAutoHarmonizeLivePlayingMode == 5)
	{
		// bloquer ce qui est dans la gamme
		if (isNoteInScaleMode(note, gammeActuelleLive, modeActuelleLive))
		{
			return (0);
		}
		else
		{
			return (note);
		}
	}

	byte rootnote = note % 12; // k devient la note entre 0 (C) et 11 (B)

	// il faut ensuite vérifier si cette note est comprise dans la gamme indiquée par PotVal pour une note de base ( C par défaut mais un programme pourra changer )

	/* if ( rootnote > gammeActuelle ) {                 // y a-t-il ABS en arduino ?  (byte) abs ( (int) rootnote - (int) gammeActuelle )
		diffnote = rootnote - gammeActuelle ; }
		else {
		diffnote = gammeActuelle - rootnote ; }
		*/
	byte diffnote = (byte)abs((int)((int)rootnote - (int)gammeActuelleLive));

	while (((Scale[modeActuelleLive] >> diffnote) & B0001) == 0)
	{
		// 0 = + ou - 1 ( fluctue )
		// 1 = + 1
		// 2 = -1
		// 3 = aucun filtre
		// 4 = bloque
		// 5 = bloque inverse ( ne laisse ce qui n'en fait pas partie )

		if (typeAutoHarmonizeLivePlayingMode == 0)
		{
			note = note + random(-1, 2); // va augmenter ou réduire le pitch jusqu'à ce que colle à la gamme.
			if (note > 127)
			{
				note = 125;
			};
			if (note == 0)
			{
				note = 4;
			}
		}

		if (typeAutoHarmonizeLivePlayingMode == 1)
		{
			note = note + 1; // va augmenter ou réduire le pitch jusqu'à ce que colle à la gamme.
			if (note > 127)
			{
				note = 115;
			};
		}

		if (typeAutoHarmonizeLivePlayingMode == 2)
		{
			note = note - 1; // va augmenter ou réduire le pitch jusqu'à ce que colle à la gamme.
			if (note == 0)
			{
				note = 12;
			}
		}

		if (typeAutoHarmonizeLivePlayingMode == 0)
		{
			note = note + random(-1, 2); // va augmenter ou réduire le pitch jusqu'à ce que colle à la gamme.
			if (note > 127)
			{
				note = 125;
			};
			if (note == 0)
			{
				note = 4;
			}
		}

		rootnote = note % 12; // rootnote devient la note entre 0 (C) et 11 (B)
		diffnote = (byte)abs(((int)rootnote - (int)gammeActuelleLive));
	}

	return note;
}

bool isLayerEclipsed(byte layer)
{

	if (eclipseMode[layer] == 0)
	{
		return false;
	}

	long a = layerCounter[layer];
	long b = (a + eclipseKTab[layer]) % eclipseModuloTab[layer];

	// byte eclipseMode [ nbLayer ] (0) ; // 0 = pas de mute   1 = mute selected    2 = mute all except selected

	if (eclipseMode[layer] == 1 && b == 0)
	{
		return true;
	}

	if (eclipseMode[layer] == 2 && b != 0)
	{
		return true;
	}

	return false;
}

bool isLayerMuted(byte layer)
{

	byte z = ((muteLayer >> layer) & B00000001);
	if (z == 1)
	{
		return true;
	}
	return false;
}

bool isLayerActive(byte layer)
{

	byte k = ((activeLayer >> layer) & B00000001);

	if (k == B00000001)
	{
		return true;
	}

	return false;
}

/*
int putNoteAinMarkovChainAndGetOneOutAsResult(int noteIn)
{

	if (currentTailleMarkovTable < markovTailleMax)
	{
		markovTable[currentTailleMarkovTable] = noteIn;
		currentTailleMarkovTable++;

		byte r = random(0, currentTailleMarkovTable);

		return markovTable[r]; // et on le garde car ce n'est pas encore complet
	}
	else
	{
		markovTable[currentTailleMarkovTable] = noteIn; // la note est donc ajoutée dans la dernière case
		// currentTailleMarkovTable++ ;  mais on n'augmente plus la taille

		byte r = random(0, currentTailleMarkovTable);
		int temp = markovTable[r]; // c'est la valeur qu'on va renvoyer

		// mais on va surtout l'effacer et décaler toutes les valeurs jusqu'à la fin du tableau
		for (byte i = r; i < markovTailleMax; i++)
		{
			markovTable[i] = markovTable[i + 1];
		}

		return temp; // et on le garde car ce n'est pas encore complet
	}
}
 */

void fixStartEndNote(byte i, byte j, long potVal2)
{
	pileNote tempNote;
	tempNote = ZEROgetNoteFromPile(pInitial[i], j);
	pileNote_fixStartEndNote(tempNote);
	ZEROSetNoteInPile(pInitial[i], j, tempNote);
}

// MODIF 2C21

void pileNote_fixStartEndNote(pileNote &celleCi, long potVal2)
{
	// la troisième valeur, potVal2, indique une valeur qu'on ajoute à Start et End
	// la plupart du temps cela vaudra zéro

	signed long temporaryStartNote;

	temporaryStartNote = (signed long)celleCi.startNote + (signed long)potVal2;
	byte i = celleCi.layer;

	/*
	if (temporaryStartNote > (long)layerLength[i])
	{
		do
		{
			temporaryStartNote = temporaryStartNote - (long)layerLength[i];
		} while (temporaryStartNote > (long)layerLength[i]);

		// DEBUG NOTE !!! attention, le melodymatrix est passé en unsigned int !
		// !!!! ceci n'est donc plus valable !!!
	// 	celleCi.startNote = (unsigned long)temporaryStartNote; // 2C21
	}
*/

	while (temporaryStartNote > (signed long)layerLength[i])
	{
		temporaryStartNote = temporaryStartNote - (signed long)layerLength[i];
	}

	/*
	if (temporaryStartNote < 0)
	{
		do
		{
			temporaryStartNote = (long)layerLength[i] + temporaryStartNote;
		} while (temporaryStartNote < 0);
	}
*/
	while (temporaryStartNote < 0)
	{
		temporaryStartNote = (signed long)layerLength[i] + temporaryStartNote;
	}

	celleCi.startNote = (unsigned long)temporaryStartNote;

	signed long temporaryEndNote;

	temporaryEndNote = (signed long)celleCi.endNote + (signed long)potVal2;
	/*
	if (temporaryEndNote > (long)layerLength[i])
	{
		do
		{
			temporaryEndNote = temporaryEndNote - (long)layerLength[i];
		} while (temporaryEndNote > (long)layerLength[i]);

		celleCi.endNote = (long)temporaryEndNote;
	}

	if (temporaryEndNote < 0)
	{
		do
		{
			temporaryEndNote = (long)layerLength[i] + temporaryEndNote;
		} while (temporaryEndNote < 0);
		celleCi.endNote = (long)temporaryEndNote;
	}
*/

	while (temporaryEndNote > (signed long)layerLength[i])
	{
		temporaryEndNote = temporaryEndNote - (signed long)layerLength[i];
	}

	while (temporaryEndNote < 0)
	{
		{
			temporaryEndNote = (signed long)layerLength[i] + temporaryEndNote;
		}
	}

	celleCi.endNote = (unsigned long)temporaryEndNote;
}

String getNameTest(int potVal2)
{
	String affiche = nulString;

	//1C89 enlevait ceci - 1C99 le réinstaure
	switch (potVal2)
	{
	case 1:
		affiche += F("1.---");
		break; // pour 1 layer
	case 2:
		affiche += F("2.Nb notes ");
		break;
	case 3:
		affiche += F("3.Highest pitch");
		break;
	case 4:
		affiche += F("4.Lowest pitch");
		break;
	case 5:
		affiche += F("5.Moy pitch ");
		break;
	case 6:
		affiche += F("6.---");
		break;
	case 7:
		affiche += F("7.LGNotMin ");
		break;
	case 8:
		affiche += F("8.LGNotMax ");
		break;
	case 9:
		affiche += F("9.LGNotMoy ");
		break;
	case 10:
		affiche += F("10.Layer counter ");
		break;
	case 11:
		affiche += F("11.notes counter ");
		break;
	case 12:
		affiche += F("12.Time (sec)");
		break;
	case 13:
		affiche += F("13.Time (min)");
		break;
	case 14:
		affiche += F("14.Slope");
		break;
	case 15:
		affiche += F("15.NtSpaceMin");
		break;
	case 16:
		affiche += F("16.NtSpaceMax");
		break;
	case 17:
		affiche += F("17.NtSpaceMoy ");
		break;
	case 18:
		affiche += F("18.demiTonsMin");
		break;
	case 19:
		affiche += F("19.demiTonsMax");
		break;
	case 20:
		affiche += F("20.demiTonsMoy");
		break;
	case 21:
		affiche += F("21.---");
		break;
	case 22:
		affiche += F("22.VelMin");
		break;
	case 23:
		affiche += F("23.VelMax");
		break;
	case 24:
		affiche += F("24.VelMoy");
		break;
	case 25:
		affiche += F("25.NbChannels");
		break;
	case 26:
		affiche += F("26.DureeLayerMin");
		break;
	case 27:
		affiche += F("27.DureeLayerMax");
		break;
	case 28:
		affiche += F("28.DureeLayerMoy");
		break;
	case 29:
		affiche += F("29.Nb notes total ");
		break;
	case 30:
		affiche += F("30. MinPitchSpan");
		break;
	case 31:
		affiche += F("31. MaxPitchSpan");
		break;
	case 32:
		affiche += F("32. MoyPitchSpan");
		break;

	default:
		break;
	}
	return affiche;
}

void resetMidiPanic()
{
	// record = false ;
	/*
		OTE OFF message, otherwise the note will continue to sound forever.
		There are cases where you want to reset whatever notes are playing. There are basically 4 ways to do this. Some synthesizer do not accept them all, so it is interesting to offer the different possibilities.
		1 - Using MIDI controller 123
		If you send a MIDI controller 123 on one MIDI channel, the synthesizer will stop all notes playing on that channel. To reset all MIDI channels, send that message for each channel. Please note that some synthesizer do not respond to this message.
		2 -MIDI Reset message
		This is a one status byte message 0xFF, without data bytes. It should reset the synthesizer to its power-on default, so it also stops all notes playing. Use this message sparingly, as it reset the full synthesizer, not only the notes playing.
		3 - MIDI NOTE OFF
		You can also send, for each channel (0 to 15) and each note pitch (0 to 127) one MIDI NOTE OFF message. This is the total solution, but requires a lot of MIDI messages to be sent, which may have some reaction time according to the MIDI hardware system you use.
		4 - MIDI NOTE OFF - Optimized
		In this case, use a table to keep track of the NOTE ON and OFF messages you send for each channel. A buffer of 128 bytes for each channel, representing the number of NOTE ON messages sent to that note,should be incremented by a NOTE ON and decremented by a NOTE OFF. Then, when you want to reset every note, simply go through that table and send a NOTE OFF to each note that is still playing.
		*/
	// va utiliser toutes les méthodes connues pour arrêter les notes suspendues
	for (byte i = 0; i < 16; i++)
	{
		for (byte j = 0; j <= 127; j++)
		{
			PlaynoteOn_(0x80 + i, j, 0);
			delay(1);
		}
	}
}

int extractNumRatio(int valeur, byte maxNum, byte maxDen)
{
	int temp = 0;
	temp = (valeur % maxNum) + 1;
	return temp;
}

int extractDenRatio(int valeur, byte maxNum, byte maxDen)
{
	byte temp = 0;
	temp = (valeur / maxNum) + 1;
	return temp;
}

String afficheRatio(int valeur, byte maxNum, byte maxDen, bool includeNegatif = false)
{
	String petit = nulString;
	if (includeNegatif == false)
	{
		petit = String(extractNumRatio(valeur, maxNum, maxDen)) + F("/") + String(extractDenRatio(valeur, maxNum, maxDen));
	}
	else
	{
		int totalSpan = 2 * maxNum * maxDen;
		int middle = totalSpan / 2;
		if (valeur < middle)
		{
			petit = F("-");
			petit += String(extractNumRatio(middle - valeur, maxNum, maxDen));
			petit += F("/");
			petit += String(extractDenRatio(middle - valeur, maxNum, maxDen));
		}
		else
		{
			petit = F("+");
			petit += String(extractNumRatio(valeur - middle, maxNum, maxDen));
			petit += F("/");
			petit += String(extractDenRatio(valeur - middle, maxNum, maxDen));
		}
	}
	return petit;
}

int extractNumRatioPlusMinus(int valeur, byte maxNum, byte maxDen)
{

	int temp = 0;
	int totalSpan = 2 * maxNum * maxDen;
	int middle = totalSpan / 2;

	if (valeur < middle)
	{
		temp = -extractNumRatio(middle - valeur, maxNum, maxDen);
	}
	else
	{
		temp = extractNumRatio(valeur - middle, maxNum, maxDen);
	}

	return temp;
}

int extractDenRatioPlusMinus(int valeur, byte maxNum, byte maxDen)
{
	byte temp = 0;
	int totalSpan = 2 * maxNum * maxDen;
	int middle = totalSpan / 2;

	if (valeur < middle)
	{
		temp = extractDenRatio(middle - valeur, maxNum, maxDen);
	}
	else
	{
		temp = extractDenRatio(valeur - middle, maxNum, maxDen);
	}

	return temp;
}

void fuseIdenticalNotes(byte i)
{
	if (isLayerActive(i))
	{
		unsigned int taille = mesureTaillePileComplete(pInitial[i]);

		for (byte j = 0; j < taille; j++)
		{

			pileNote tempNote;
			tempNote = ZEROgetNoteFromPile(pInitial[i], j);

			if (tempNote.note != 0)
			{

				for (byte l = (j + 1); l < taille; l++)
				{
					pileNote tempNote2;
					tempNote2 = ZEROgetNoteFromPile(pInitial[i], l);

					if (tempNote.note == tempNote2.note)
					{

						if ((abs((long)tempNote.startNote - (long)tempNote2.startNote) < 10) && (abs((int)tempNote.endNote - (int)tempNote2.endNote) < 10))
						{
							putNoteBinNoteA(tempNote2, emptyNote);
							tempNote2.killnote = true;
							ZEROSetNoteInPile(pInitial[i], l, tempNote2);
						}
					}
				}
			}
		}
	}
}

byte limiterForPitch(int pitchInitial)
/**
 * @brief limite une valeur entre 0 et 127, peu importe la nature, mais sur un mode NOTE ( %12 ) 
 * 
 */
{
	int note = pitchInitial;

	if (pitchInitial > 127) // 0111 1111
	{
		// d'abord trouver la note
		note = pitchInitial % 12;
		// ensuite on ramène à la dernière note qui a cette valeur
		note = 108 + note;
		if (note > 127)
		{
			note = note - 12;
		}
	}

	if (pitchInitial <= 0) // 0111 1111
	{
		// d'abord trouver la note
		note = pitchInitial % 12;
		// ensuite on ramène à la dernière note qui a cette valeur
		note = 0 + note;
		if (note <= 0)
		// 1C93 j'ai mis = car si c'est un C0 il ne sera pas entendu et considéré comme nul
		{
			note = note + 12;
		}
	}

	byte note2 = (byte)note;
	return note2;
}

void setCounterLeft(int const &a)
{
	/*
	softwareValueEncoderLeft = a;
	counterEncoder = softwareValueEncoderLeft * 8;
	rotKnobLeft.write(counterEncoder);
	*/
	softwareValueEncoderLeft = a;
}

void setCounterRight(int const &a)
{
	/*
	softwareValueEncoderRight = a;
	counter2Encoder = softwareValueEncoderRight * 8;
	rotKnobRight.write(counter2Encoder);
	*/
	softwareValueEncoderRight = a;
}

/////////////////////////////////////////////////////////////////////////////////////
// Version Pile NOTE
// 1C95
/////////////////////////////////////////////////////////////////////////////////////

int howManyNoteAinLayerB(int note, byte layer)
{
	if (note <= 0)
	{
		return 0;
	}
	if (note > 127)
	{
		return 0;
	}

	int compteur = 0;

	unsigned int taille = mesureTaillePileComplete(pInitial[layer]);

	for (unsigned int j = 0; j < taille; j++)
	{
		pileNote tempNote;
		tempNote = ZEROgetNoteFromPile(pInitial[layer], j);

		if (tempNote.note == note)
		{
			compteur++;
		}
	}
	return compteur;
}

/////////////////////////////////////////////////////////////////////////////////////
// Version Pile NOTE
// 1C95
/////////////////////////////////////////////////////////////////////////////////////

int howManyNotesAinLayersOtherThanB(int note, byte layer)
{
	if (note <= 0)
	{
		return 0;
	}
	if (note > 127)
	{
		return 0;
	}

	int compteur = 0;
	for (int i = 0; i < nbLayer; i++)
	{
		if (i != layer)
		{
			compteur += howManyNoteAinLayerB(note, i);
		}
	}
	return compteur;
}

/////////////////////////////////////////////////////////////////////////////////////
// Version Pile NOTE
// 1C95
/////////////////////////////////////////////////////////////////////////////////////

void Comb2(void)
{

	unsigned int taille[nbLayer];
	unsigned int tailleMax = 0;

	for (int i = 0; i < nbLayer; i++)
	{
		taille[i] = mesureTaillePileComplete(pInitial[i]);
		if (taille[i] > tailleMax)
		{
			tailleMax = taille[i];
		}
	}

	if (potVal2 == 0)
	{
		potVal2 = 1;
	}

	for (unsigned int j = 0; j < tailleMax; j++)
	{
		for (int i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i) && (j < taille[i]))
			{
				pileNote tempNote;
				tempNote = ZEROgetNoteFromPile(pInitial[i], j);

				int noote = tempNote.note;

				if (noote != 0)
				{
					int direction = 0;
					direction = random(2) == 0 ? 1 : -1; // choisit soit +1, soit -1

					// doit continuer à monter si :
					// la note n'est pas ZERO ET
					// il y a une autre note idem sur une autre layer
					// OU il y a aucune note sur autre layer qui soit n+- potVal2
					// OU si la note n'est pas dans la gamme

					while (
						(
							(noote != 0) && ((howManyNotesAinLayersOtherThanB(noote, i) > 0) || (howManyNotesAinLayersOtherThanB(noote + potVal2, i) > 0) || (howManyNotesAinLayersOtherThanB(noote - potVal2, i) > 0) || (isNoteInScaleMode(noote, gammeActuelle[i], modeActuelle[i]) == false))))

					{
						// noote  = melodyMatrix[i][j][MMnote] + potVal2  ;
						noote = noote + direction * potVal2;
						// envoie donc vers + ou - potVal2 tant qu'il y a des notes idem sur autre layer
						// à même pitch ou à potVal2 intervalles
						if (noote > 127)
						{
							noote = 0;
						}
						if (noote < 0)
						{
							noote = 0;
						}
						// remplacer ceci par qqch de plus classe TODO : la note similaire idem la plus haute ?
					}

					if (noote > 127)
					{
						noote = 0;
					}
					if (noote < 0)
					{
						noote = 0;
					}

					tempNote.note = noote;

					ZEROSetNoteInPile(pInitial[i], j, tempNote);
				}
			}
		}
	}
}

void turnOnLEDNoteIn()
{
	// digitalWrite(5, HIGH);
	// à réinstaurer pour version nouvelle du prototype
}

void turnOffLEDNoteIn()
{
	// digitalWrite(5, HIGH);
	// à réinstaurer pour version nouvelle du prototype
}

void showMutedStatus()
{
	affiche = nulString;

	for (byte i = 0; i < nbLayer; i++)
	{
		byte k = ((muteLayer >> i) & B00000001); // le bit de droite est 1 si layer 0 est MUTE ; le 2ème bit est 1 si layer 1 est MUTE, etc...

		if (k == B00000001)
		{
			// buffer_and_HARD_setCursor(10 + i, 5);
			// buffer_and_HARD_print("M");
			affiche += "M";
		}
		else
		{
			// buffer_and_HARD_setCursor(10 + i, 5);
			// buffer_and_HARD_print("-");
			affiche += "-";
		}
	}

	buffer_setCursor(0, 2);
	buffer_print(affiche);

	// printRot2(affiche, 5, 400, 5);
	//	delay(100);
}

void modifyPitchADD_(pileNote &cetteNote, signed int modification)
// cette fonction va modifier la valeur de note directement !
// on peut imaginer qu'elle renvoie 0 ou 1 pour indiquer si overflow ou qqch du style, mais pas prioritaire
{
	if (pitchableChannel[cetteNote.channel] == true && pitchableLayer[cetteNote.layer] == true)
	{
		signed int newValue = (signed int)cetteNote.note;
		newValue += (signed int)modification;
		newValue = limiterForPitch(newValue);
		cetteNote.note = newValue;
	}
}

void modifyPitchSET_(pileNote &cetteNote, signed int modification)
// cette fonction va modifier la valeur de note directement !
// on peut imaginer qu'elle renvoie 0 ou 1 pour indiquer si overflow ou qqch du style, mais pas prioritaire
{
	if (pitchableChannel[cetteNote.channel] == true && pitchableLayer[cetteNote.layer] == true)
	{
		signed int newValue = (signed int)modification;
		newValue = limiterForPitch(newValue);
		cetteNote.note = newValue;
	}
}

void modifyPitchADDRealPlayed_(pileNote &cetteNote, signed int modification)
// cette fonction va modifier la valeur de note directement !
// on peut imaginer qu'elle renvoie 0 ou 1 pour indiquer si overflow ou qqch du style, mais pas prioritaire
{
	if (pitchableChannel[cetteNote.channel] == true && pitchableLayer[cetteNote.layer] == true)
	{
		signed int newValue = (signed int)cetteNote.noteReallyPlayed;
		newValue += (signed int)modification;
		newValue = limiterForPitch(newValue);
		cetteNote.noteReallyPlayed = newValue;
	}
}

void modifyPitchSETRealPlayed_(pileNote &cetteNote, signed int modification)
// cette fonction va modifier la valeur de note directement !
// on peut imaginer qu'elle renvoie 0 ou 1 pour indiquer si overflow ou qqch du style, mais pas prioritaire
{
	if (pitchableChannel[cetteNote.channel] == true && pitchableLayer[cetteNote.layer] == true)
	{
		signed int newValue = (signed int)modification;
		newValue = limiterForPitch(newValue);
		cetteNote.noteReallyPlayed = newValue;
	}
}

byte modifyPitchUniversal(pileNote &cetteNote, signed int newValue, signed int addition = 0)
/**
 * @brief ne fonctionne qu'avec des 0x90, pour modifier le pitch 
 * 
 */
{
	signed int Value;

	if (pitchableChannel[cetteNote.channel] == true && pitchableLayer[cetteNote.layer] == true && cetteNote.type == 0x90)
	// 2C24
	{
		Value = (signed int)newValue;
		Value += (signed int)addition;
		Value = limiterForPitch(Value);
	}
	else
	{
		Value = cetteNote.note; // si non pitchable on doit toujours revenir à la valeur initiale
	}

	return (byte)(Value);
}

unsigned long dynamicTreatNote_pileNote(pileNote &tempNote, unsigned long lgAB)
{
	int tempA, tempB;
	byte iAB;

	String contenuA = nulString, contenuB = nulString, newcontenuA = nulString, newcontenuB = nulString;

	String totalcontenuA = nulString;

	iAB = tempNote.startNote / lgAB;

	String check;
	check = patternSwing.substring(iAB % patternSwing.length(), iAB % patternSwing.length() + 1);

	if (check == flechedroite)
	{
		tempA = iAB * lgAB;		  // donne le temps dans la grille parfaite avant startnote
		tempB = (iAB + 1) * lgAB; // donne le temps dans la grille parfaite après startnote
		float ftempC = ((float)tempB - (float)tempA) * (float)pcAB / (float)100.0;
		long tempC = (long)tempA + (long)ftempC;

		return tempC;
	}

	if (check == flechegauche)
	{
		pcAB = 100 - pcAB;
		tempA = iAB * lgAB;		  // donne le temps dans la grille parfaite avant startnote
		tempB = (iAB + 1) * lgAB; // donne le temps dans la grille parfaite après startnote
		float ftempC = ((float)tempB - (float)tempA) * (float)pcAB / (float)100.0;
		long tempC = (long)tempA + (long)ftempC;

		return tempC;
	}

	if (check == barremilieu)
	{
		tempA = iAB * lgAB; // donne le temps dans la grille parfaite avant startnote
		return tempA + 1;
	}

	if (check == barreH)
	{
		tempB = (iAB + 1) * lgAB; // donne le temps dans la grille parfaite après startnote
		return tempB + 1;
	}

	if (check == "!")
	{

		tempA = iAB * lgAB;
		tempB = (iAB + 1) * lgAB; // donne le temps dans la grille parfaite après startnote
		long tempC = (random(1) == 0 ? tempA - 2 : tempB + 2);
		// fait en somme passer la note soit juste de l'autre côté en bas, soit juste de l'autre côté en haut
		return tempC;
	}

	if (check == "x")
	{ // NOTE ***** A définir : va envoyer les notes vers le iAB précédent avec une valeur non "x"
		//   n = iAB
		//   si n = 0 : stop
		//   si n > 0 : m = n-1
		//   tester patternSwing.substring(iAB % patternSwing.length(),iAB % patternSwing.length() + 1 )
		//    si ="x"
		//      recursion
		//    sinon : on applique là la transfo exigée

		// if (iAB != 0)
		{
			// d'abord on va déplacer la note à la même place dans la division précédente

			// long tempC -= lgAB;
			return 0; // TODO on ne fait pas tout comme le static SWING !

			// ensuite on doit revoir la situation pour cette note modifiée, par récursivité
		}
	}

	return 0;
}

unsigned long dynamicTreatNote_pileNote_SIMPLIFIED(pileNote &tempNote)
// Version simplifiée, qui ne fait pas appel à des string, car trop lent pour à chaque itération calculer cela !
// se base sur le nb de Swing, mais fait d'office | >
{
	unsigned long tempA, tempB;
	unsigned long iAB;

	byte i = tempNote.layer;

	unsigned long lgAB = layerLength[i] / nSwing;
	iAB = tempNote.startNote / lgAB;
int nowValueDynamicSwing = getValueStraightOrFromSequence( valueDynamicSwing[i], 0 );

	// on pourrait calculer ici, sans devoir passer lgAB en valeur
	// car on connait .layer et duree, ainsi que nSwing

	if (iAB % 2 == 1)
	{
		tempA = iAB * lgAB;		  // donne le temps dans la grille parfaite avant startnote
		tempB = (iAB + 1) * lgAB; // donne le temps dans la grille parfaite après startnote
		float ftempC = ((float)tempB - (float)tempA) * (float) nowValueDynamicSwing / (float)100.0;
		long tempC = (long)tempA + (long)ftempC;

		return tempC;
	}
	else
	{
		tempA = iAB * lgAB; // donne le temps dans la grille parfaite avant startnote
		return tempA + 1;
	}

	return 0;
}

void ZEROupdatePileTimeToPlayMODEDIRECT(int a)
// met à jour la pile A
{
	unsigned int taille = mesureTaillePileComplete(pInitial[a]);

	for (unsigned int j = 0; j < taille; j++)
	{ // va parcourir la mélodie pointée ( Layer 1 à 4 : potVal3 )
		pileNote cetteNote;
		cetteNote = ZEROgetNoteFromPile(pInitial[a], j);

		pileNote_fixStartEndNote(cetteNote, 0);

		// unsigned long nowMillis = nowMillis;
		float realRatio;
		unsigned long actualizedlayerLength;

		byte i = cetteNote.layer;

		int nowDrunkMin = getValueStraightOrFromSequence(drunkMin[i], -1000);
		int nowDrunkMax = getValueStraightOrFromSequence(drunkMax[i], -1000);
		int nowDrunkStep = getValueStraightOrFromSequence(drunkStep[i], 0);

		unsigned long localStartNote;
		localStartNote = cetteNote.startNote;

		if (dynamicSwing[i] == true)
		{
			localStartNote = dynamicTreatNote_pileNote_SIMPLIFIED(cetteNote);
		}

		realRatio = (float)(((float)ratioCurrent[i] * (float)ratioLength[i]) / (float)100.0);
		actualizedlayerLength = int((float)(((float)layerLength[i] * (float)realRatio) / (float)100));

		// Serial.println(actualizedlayerLength) ;

		unsigned long debutThisSegment = nowMillis - ((nowMillis) % actualizedlayerLength);
		unsigned long endThisSegment = debutThisSegment + actualizedlayerLength;

		if (ratioCurrent[i] != 100)
		{
			if (localStartNote < (nowMillis % actualizedlayerLength))
			{
				cetteNote.timeToPlayON = endThisSegment + (unsigned long)(((float)localStartNote * (float)realRatio) / (float)100);
			}
			else
			{
				cetteNote.timeToPlayON = debutThisSegment + (unsigned long)(((float)localStartNote * (float)realRatio) / (float)100);
			}
		}
		else
		{
			if (localStartNote < (nowMillis % actualizedlayerLength))
			{
				cetteNote.timeToPlayON = endThisSegment + cetteNote.startNote;
			}
			else
			{
				cetteNote.timeToPlayON = debutThisSegment + cetteNote.startNote;
			}
		}

		// if (cetteNote.thereIsAnEnd)
		{
			// cetteNote.timeToPlayOFF = endThisSegment + (unsigned long)(((float)cetteNote.endNote * (float)realRatio) / (float)100);
			// TODO DEBUG
			// TESTER SI IL Y A DEJA Un ENDNOTE TODO DEBUG
		}

		// TODO DEBUG :
		// problème avec l'actualistion : elle doit se passer en 2 fois
		// uen fois pour le Note ON, une autre fois pour le Note OFF
		// or les valeurs peuvent être différentes, ce qui est un problème
		// il faut donc enregistrer la valeur du Poof avec le Note ON et mettre à jour le Note OFF avec cette valeur mémorisée

		// on doit quand même faire ceci au cas où on aurait affaire à une machine qui n'envoie que des note ON et pas de OFF
		if (turnOnBipolarRandom)
		{
			signed int poof_;
			poof_ = giveBipolarRandomForLayer(i);
			cetteNote.timeToPlayON += poof_;
			// cetteNote.timeToPlayOFF += poof_;
		}

		if (turnOnPoof)
		{
			int nowPoof = getValueStraightOrFromSequence(poof[i], 0);
			signed int poof_;
			poof_ = random(nowPoof);
			if (random(2) == 0)
			{
				poof_ = -poof_;
			}
			cetteNote.timeToPlayON += poof_;
			// cetteNote.timeToPlayOFF += poof_;
		}

		//2C31 Sequential Postpone
		if (turnOnSequentialPostpone[i] == true && cetteNote.type != 0xB0)
		{
			signed int poof_;
			poof_ = giveNextValue(sequence[seqPostpone[i]], sequence[seqPostpone[i]].compteur);
			sequence[seqPostpone[i]].compteur++;

			cetteNote.timeToPlayON += poof_;
			// cetteNote.timeToPlayOFF += poof_;
		}

		if (drunk[i] && cetteNote.type != 0xB0)
		{
			signed int poof_;
			poof_ = currentDrunkValue[i];

			if (random(0, 2) == 0)
			{
				currentDrunkValue[i] += nowDrunkStep;
			}
			else
			{
				currentDrunkValue[i] -= nowDrunkStep;
			}
			if (currentDrunkValue[i] > nowDrunkMax)
			{
				currentDrunkValue[i] = nowDrunkMax;
			}
			if (currentDrunkValue[i] < nowDrunkMin)
			{
				currentDrunkValue[i] = nowDrunkMin;
			}

			cetteNote.timeToPlayON += poof_;
			// cetteNote.timeToPlayOFF += poof_;
		}

		pileNote_fixStartEndNote(cetteNote, 0);
		// ZEROaddNewNotetoPile(pInitial[b], tempNote);
		ZEROSetNoteInPile(pInitial[a], j, cetteNote);
	}
}

void ZEROupdateNoteTimeToPlayMODEDIRECT(pileNote &cetteNote)
// met à jour la pile A
{
	{ // va parcourir la mélodie pointée ( Layer 1 à 4 : potVal3 )

		pileNote_fixStartEndNote(cetteNote, 0);

		// unsigned long nowMillis = nowMillis;
		float realRatio;
		unsigned long actualizedlayerLength;

		byte i = cetteNote.layer;

		int nowDrunkMin = getValueStraightOrFromSequence(drunkMin[i], -1000);
		int nowDrunkMax = getValueStraightOrFromSequence(drunkMax[i], -1000);
		int nowDrunkStep = getValueStraightOrFromSequence(drunkStep[i], 0);

		unsigned long localStartNote;
		localStartNote = cetteNote.startNote;

		if (dynamicSwing[i] == true)
		{
			localStartNote = dynamicTreatNote_pileNote_SIMPLIFIED(cetteNote);
		}

		realRatio = (float)(((float)ratioCurrent[i] * (float)ratioLength[i]) / (float)100.0);
		actualizedlayerLength = int((float)(((float)layerLength[i] * (float)realRatio) / (float)100));

		// Serial.println(actualizedlayerLength) ;

		unsigned long debutThisSegment = nowMillis - ((nowMillis) % actualizedlayerLength);
		unsigned long endThisSegment = debutThisSegment + actualizedlayerLength;

		if (localStartNote < (nowMillis % actualizedlayerLength))
		{
			cetteNote.timeToPlayON = endThisSegment + (unsigned long)(((float)localStartNote * (float)realRatio) / (float)100);
		}
		else
		{
			cetteNote.timeToPlayON = debutThisSegment + (unsigned long)(((float)localStartNote * (float)realRatio) / (float)100);
		}
		// if (cetteNote.thereIsAnEnd)
		{
			// cetteNote.timeToPlayOFF = endThisSegment + (unsigned long)(((float)cetteNote.endNote * (float)realRatio) / (float)100);
			// TODO DEBUG
			// TESTER SI IL Y A DEJA Un ENDNOTE TODO DEBUG
		}

		// TODO DEBUG :
		// problème avec l'actualistion : elle doit se passer en 2 fois
		// uen fois pour le Note ON, une autre fois pour le Note OFF
		// or les valeurs peuvent être différentes, ce qui est un problème
		// il faut donc enregistrer la valeur du Poof avec le Note ON et mettre à jour le Note OFF avec cette valeur mémorisée

		// on doit quand même faire ceci au cas où on aurait affaire à une machine qui n'envoie que des note ON et pas de OFF
		if (turnOnBipolarRandom)
		{
			cetteNote.timeToPlayON = debutThisSegment + (unsigned long)(((float)localStartNote * (float)realRatio) / (float)100);

			signed int poof_;
			poof_ = giveBipolarRandomForLayer(i);
			cetteNote.timeToPlayON += poof_;
			// cetteNote.timeToPlayOFF += poof_;
		}

		if (turnOnPoof)
		{
			cetteNote.timeToPlayON = debutThisSegment + (unsigned long)(((float)localStartNote * (float)realRatio) / (float)100);

			int nowPoof = getValueStraightOrFromSequence(poof[i], 0);
			signed int poof_;
			poof_ = random(nowPoof);
			if (random(2) == 0)
			{
				poof_ = -poof_;
			}
			cetteNote.timeToPlayON += poof_;
			// cetteNote.timeToPlayOFF += poof_;
		}

		//2C31 Sequential Postpone
		if (turnOnSequentialPostpone[i] == true && cetteNote.type != 0xB0)
		{
			signed int poof_;
			poof_ = giveNextValue(sequence[seqPostpone[i]], sequence[seqPostpone[i]].compteur);
			sequence[seqPostpone[i]].compteur++;

			cetteNote.timeToPlayON += poof_;
			// cetteNote.timeToPlayOFF += poof_;
		}

		if (drunk[i] && cetteNote.type != 0xB0)
		{
			cetteNote.timeToPlayON = debutThisSegment + (unsigned long)(((float)localStartNote * (float)realRatio) / (float)100);

			signed int poof_;
			poof_ = currentDrunkValue[i];

			if (random(0, 2) == 0)
			{
				currentDrunkValue[i] += nowDrunkStep;
			}
			else
			{
				currentDrunkValue[i] -= nowDrunkStep;
			}
			if (currentDrunkValue[i] > nowDrunkMax)
			{
				currentDrunkValue[i] = nowDrunkMax;
			}
			if (currentDrunkValue[i] < nowDrunkMin)
			{
				currentDrunkValue[i] = nowDrunkMin;
			}

			cetteNote.timeToPlayON += poof_;
			// cetteNote.timeToPlayOFF += poof_;
		}

		pileNote_fixStartEndNote(cetteNote, 0);
		// ZEROaddNewNotetoPile(pInitial[b], tempNote);
		// ZEROSetNoteInPile(pInitial[a], j, cetteNote);
	}
}

pileNote ZEROGeneralMarkov(pileNote &cetteNote)
// variation sur la fonction Markov précédente
// ici, on met une note dans le markov général, qui combine certains layers
// en retour on reçoit une note.
// la version précédente gérait les CH, Notes, Vélocité n'importe comment..
// en retournant juste une valuer de note !
// à terme il faudra la supprimer
{

	if (currentTailleNEWMarkovTable < markovNEWTailleMax)
	{
		// markovTable[currentTailleMarkovTable] = noteIn;
		// on ajoute la note à Markov Général
		ZEROaddNewNotetoPile(pMarkov[0], cetteNote);

		//	currentTailleMarkovTable++;
		currentTailleNEWMarkovTable = mesureTaillePileComplete(pMarkov[0]);

		byte r = random(0, currentTailleNEWMarkovTable);

		pileNote returnNote;
		returnNote = ZEROgetNoteFromPile(pMarkov[0], r);

		return returnNote; // et on le garde car ce n'est pas encore complet
						   // on retourne donc ici avec CH, NOTE, VELOCITE
						   // on pourrait imaginer un filte :
						   // keep CH or not
						   // keep Note or not
						   // keep Velocite or not
	}
	else
	{
		// on ajoute la note à Markov Général
		ZEROaddNewNotetoPile(pMarkov[0], cetteNote);
		currentTailleNEWMarkovTable = mesureTaillePileComplete(pMarkov[0]);

		byte r = random(0, currentTailleNEWMarkovTable);

		pileNote returnNote;
		returnNote = ZEROgetNoteFromPile(pMarkov[0], r);

		ZEROdeleteNoteFromPile(pMarkov[0], r);

		return returnNote;
	}
}

pileNote ZEROLayerMarkov(pileNote &cetteNote, int layer)
// variation sur la fonction Markov précédente
// ici, on met une note dans le markov général, qui combine certains layers
// en retour on reçoit une note.
// la version précédente gérait les CH, Notes, Vélocité n'importe comment..
// en retournant juste une valuer de note !
// à terme il faudra la supprimer
{

	if (currentSizeLayerMarkov[layer] < markovLayerMax[layer])
	{
		ZEROaddNewNotetoPile(pMarkov[1 + layer], cetteNote);
		currentSizeLayerMarkov[layer] = mesureTaillePileComplete(pMarkov[1 + layer]);

		byte r = random(0, currentSizeLayerMarkov[layer]);

		pileNote returnNote;
		returnNote = ZEROgetNoteFromPile(pMarkov[1 + layer], r);

		return returnNote; // et on le garde car ce n'est pas encore complet
						   // on retourne donc ici avec CH, NOTE, VELOCITE
						   // on pourrait imaginer un filte :
						   // keep CH or not
						   // keep Note or not
						   // keep Velocite or not
	}
	else
	{
		// on ajoute la note à Markov Général
		ZEROaddNewNotetoPile(pMarkov[1 + layer], cetteNote);
		currentSizeLayerMarkov[layer] = mesureTaillePileComplete(pMarkov[1 + layer]);

		byte r = random(0, currentSizeLayerMarkov[layer]);

		pileNote returnNote;
		returnNote = ZEROgetNoteFromPile(pMarkov[1 + layer], r);

		ZEROdeleteNoteFromPile(pMarkov[1 + layer], r);

		return returnNote;
	}
}

void cleanLigne(String &ligne_)
// va effacer les tabulations, espaces, et supprimer ce qui suit // en commentaire
{
	//	Serial.println("process>"+ligne_);
	//Serial.print("++A++"+ligne_);

	ligne_.replace("\t", "");
	//	Serial.print("++B++"+ligne_);

	ligne_.replace("\n", "");
	//	Serial.print("++C++"+ligne_);

	ligne_.replace(" ", "");
	//	Serial.print("++D++"+ligne_);

	ligne_.trim();
	//	Serial.print("++E++"+ligne_);

	int trouve;
	trouve = ligne_.indexOf("//");
	//	Serial.print("++F++"+ligne_);

	if (trouve != -1)
	{
		ligne_ = ligne_.substring(0, trouve);
	}
	//	Serial.println("++G++"+ligne_);

	// return ligne_ ;
}

void miniCleanLigne(String &ligne)
// va effacer les tabulations, espaces
{
	ligne.replace("\t", "");
	ligne.replace("\n", "");
	ligne.replace("\r", "");
	ligne.replace(" ", "");
	// return ligne ;
}

void reduxProcessLigne(String &ligne)
{
	// on va procéder aux modificatons à l'autechre
	// byte percentDisappear = 5;
	// byte percentRedouble = 5;
	// byte percentFlipFlop = 5;
	// byte percentSwitch = 5;
	// byte percentLoot = 5;

	// suppression de lettres
	int longueur = ligne.length();
	for (int i = 0; i < longueur; i++)
	{
		if (random(100) < percentDisappear)
		{
			// on supprime
			String ligne2;
			ligne2 = ligne.substring(0, i) + ligne.substring(i + 1);
			ligne = ligne2;
			longueur--;
		}
	}

	// redoublement de lettres
	longueur = ligne.length();
	for (int i = 0; i < longueur; i++)
	{
		if (random(100) < percentRedouble)
		{
			// on redouble
			String ligne2;
			ligne2 = ligne.substring(0, i + 1) + ligne.substring(i);
			ligne = ligne2;
			longueur++;
		}
	}

	// switch de lettres
	longueur = ligne.length();
	for (int i = 0; i < longueur; i++)
	{
		if (random(100) < percentSwitch)
		{
			// on échange deux lettres
			String ligne3;
			int a = random(0, longueur);
			ligne3 = ligne[a];

			String ligne4;
			ligne4 = ligne[i];

			ligne[i] = ligne3[0];
			ligne[a] = ligne4[0];
		}
	}

	// flip flop de lettres
	// loot hacker style
	Serial.println();
	Serial.println(ligne);
}

String getBracketsFromLigne(String &ligne)
// va retenir ce qui se trouve entre []
{
	//ligne.replace("\t", "");
	//ligne.replace("\n", "");
	//ligne.replace(" ", "");
	//ligne.trim();
	// int trouve;
	int trouve1 = ligne.indexOf("[");
	int trouve2 = ligne.indexOf("]");
	return ligne.substring(trouve1 + 1, trouve2);
}

String getParenthesesFromLigne(String &ligne)
// va retenir ce qui se trouve entre []
{
	//ligne.replace("\t", "");
	//	ligne.replace("\n", "");
	//	ligne.replace(" ", "");
	//	ligne.trim();
	// int trouve;
	int trouve1 = ligne.indexOf("(");
	int trouve2 = ligne.indexOf(")");
	return ligne.substring(trouve1 + 1, trouve2);
}

void deleteCCMod(int currentCCMod)
{
	CCModTableau[currentCCMod].type = 0;
	CCModTableau[currentCCMod].sourceMin = 0;
	CCModTableau[currentCCMod].sourceMax = 127;
	CCModTableau[currentCCMod].targetMin = 0;
	CCModTableau[currentCCMod].targetMax = 127;
	CCModTableau[currentCCMod].borders = 0;
	CCModTableau[currentCCMod].idCC = 10; // au hasard
	CCModTableau[currentCCMod].channelCC = 0;
	CCModTableau[currentCCMod].layerConcerne = 1;
	CCModTableau[currentCCMod].actif = false;
}

void deleteAllCCMod()
{
	for (int i = 0; i < NbCCModMax; i++)
	{
		deleteCCMod(i);
	}
}

int plusGrandNombreAvecAbits(int a)
{
	int sum = 0;
	for (int i = 0; i < a; i++)
	{
		sum += pow(2, i);
		// sum += ( 1 << i );   TODO
	}
	return sum;
}

// FONCTIONS LIEES A IMPLICIT

// unsigned long distanceEntre2Start (pileNote &celleCi, pileNote &celleLa )
unsigned long distanceEntre2StartSimple(unsigned long A, unsigned long B, unsigned long LLength)

{
	// unsigned long A = celleCi.startNote ;
	// unsigned long B = celleLa.startNote ;
	unsigned long C = 0; // résultat
	unsigned long D = 0; // résultat anti horaire

	// calcul direction horaire
	if (B > A)
	{
		C = B - A;
	}
	else
	{
		C = A - B;
	}

	// calcul anti-horaire
	if (B > A)
	{
		// D = layerLength[celleLa.layer] - B + A ;
		D = LLength - B + A;
	}
	else
	{
		// D = layerLength[celleCi.layer] - A + B ;
		D = LLength - A + B;
	}

	if (C < D)
	{
		return C;
	}
	else
	{
		return D;
	}
}

void implicitLayerUntilOne(int layeri)
{
	bool firstHigher = true;

	// on place les notes dans le tableau1 : le startNote
	// on calcule la taille du tableau1
	unsigned int taille = mesureTaillePileComplete(pInitial[layeri]);
	long tableau1[taille];

	for (unsigned int j = 0; j < taille; j++)
	{
		pileNote tempNote;
		tempNote = ZEROgetNoteFromPile(pInitial[layeri], j);
		tableau1[j] = tempNote.startNote;
	}

	unsigned int taille1 = taille; // nombre de notes dans le tableau1

	// on va tester les nombres de divisions possibles

	bool foundFirstSumSquare = false;
	long long minimalSumSquare = 0;

	for (int i = 2; i < 256; i++) // parcourt le nb de divisions possibles
	{
		unsigned long tableau2[i];

		// on calcule la durée d'un bloc pour cette valeur

		unsigned long block = layerLength[layeri] / (long)i;

		// on va tester les différents décalages possibles

		long long sumSquare = 0;

		for (unsigned long j = 0; j <= block; j++) // parcourt pour chaque division les décalages possibles
		{

			sumSquare = 0;

			// calcul des valeurs du tableau2
			for (int k = 0; k < i; k++)
			{
				tableau2[k] = k * block + j;
				if (tableau2[k] > layerLength[layeri])
				{
					tableau2[k] -= layerLength[layeri];
				}
			}

			// on a maintenant tableau1 avec les notes de départ et tableau2 avec la comparaison
			// on va trouver la note la plus proche de tableau1 dans tableau2 et faire une somme

			sumSquare = 0;

			for (unsigned int k = 0; k < taille1; k++) // pour chaque note..
			{
				long smallest = 0;
				bool found = false;
				long distance = 0;

				for (int l = 0; l < i; l++) // ..je regarde sa proximité avec un contenu de tableau2
				{
					distance = distanceEntre2StartSimple(tableau1[k], tableau2[l], layerLength[layeri]);

					if (distance < smallest || found == false)
					{
						smallest = distance;
						found = true;
					}
				}

				sumSquare = sumSquare + smallest * smallest;
			}

			if (sumSquare <= minimalSumSquare || foundFirstSumSquare == false)
			{
				minimalSumSquare = sumSquare;
				foundFirstSumSquare = true;
				// cout << "SMALLEST SUMSQUARE " << minimalSumSquare << endl;

				// affichage du tableau2
				for (int k = 0; k < i; k++)
				{
					//	cout << tableau2[k] << " ";
				}

				// on doit ici préciser pour ce block combien de notes du layer original se retrouvent
				// sur chacun des éléments du block
				// on connait I,J

				int tableau3[i] = {0}; // il va servir à compter combien de fois une note du tableau repère est chargée

				for (unsigned int k = 0; k < taille1; k++) // pour chaque note..
				{
					long smallest = 0;
					bool found = false;
					long distance = 0;
					int chosenL = 0;

					for (int l = 0; l < i; l++) // ..je regarde sa proximité avec un contenu de tableau2
					{
						// comparaison

						distance = distanceEntre2StartSimple(tableau1[k], tableau2[l], layerLength[layeri]);

						if (distance <= smallest || found == false)
						{
							smallest = distance;
							found = true;
							chosenL = l;
						}
					}

					tableau3[chosenL]++;
				}
				int nbBlockHigherThanOne = 0;
				int Full = 0;
				int Empty = 0;

				// cout << "REPARTITION " << endl;

				// affichage du tableau3

				for (int k = 0; k < i; k++)
				{
					// cout << tableau3[k] << " ";
					if (tableau3[k] > 1)
					{
						nbBlockHigherThanOne++;
					}

					if (tableau3[k] > 0)
					{
						Full++;
					}
					else
					{
						Empty++;
					}
				}

				if (nbBlockHigherThanOne == 0 && firstHigher == true)
				{
					firstHigher = false;
					// cout << "PREMIER BLOC DE REPARTITION 1" << endl;
					buffer_and_HARD_setCursor(5, 5);
					buffer_and_HARD_print("**");
					buffer_and_HARD_print(i);
					buffer_and_HARD_print("**");

					// comment repartir d'ici pour transformer le layer i
					// en layer influencé par ces valeurs ?

					for (unsigned int k = 0; k < taille1; k++) // pour chaque note..
					{
						long smallest = 0;
						bool found = false;
						long distance = 0;
						int chosenL = 0;

						for (int l = 0; l < i; l++) // ..je regarde sa proximité avec un contenu de tableau2
						{
							// comparaison

							distance = distanceEntre2StartSimple(tableau1[k], tableau2[l], layerLength[layeri]);

							if (distance <= smallest || found == false)
							{
								smallest = distance;
								found = true;
								chosenL = l;
							}
						}

						// je prends la note k dans le layer
						// je calcule sa duree
						// son start devient la valeur de tableau2[chosenL]
						// son end devient start + duree

						pileNote tempNote;
						tempNote = ZEROgetNoteFromPile(pInitial[layeri], k);
						long duree = getDuree(tempNote);
						tempNote.startNote = tableau2[chosenL];
						tempNote.endNote = tempNote.startNote + duree;
						ZEROSetNoteInPile(pInitial[layeri], k, tempNote);
					}

					return;
				}

				// calcul du ratio 1 sur 0

				if (abs(Full - Empty) <= taille1 && nbBlockHigherThanOne == 0)
				{
					//	cout << "GOOD RATIO" << endl;
				}

				// wait_on_enter() ;
			}

		} // FIN DE J : décalage

	} // FIN DE I : différentes découpes

	// si on arrive ici ça signifie échec : on va faire un implicit sur 256, forcé !
	buffer_and_HARD_setCursor(5, 5);
	buffer_and_HARD_print("**");
	buffer_and_HARD_print("FORCE256");
	buffer_and_HARD_print("**");

	int i = 256;
	int j = 0;

	unsigned long block = layerLength[layeri] / (long)i;
	unsigned long tableau2[i];

	// calcul des valeurs du tableau2
	for (int k = 0; k < i; k++)
	{
		tableau2[k] = k * block + j;
		if (tableau2[k] > layerLength[layeri])
		{
			tableau2[k] -= layerLength[layeri];
		}
	}

	for (unsigned int k = 0; k < taille1; k++) // pour chaque note..
	{
		long smallest = 0;
		bool found = false;
		long distance = 0;
		int chosenL = 0;

		for (int l = 0; l < i; l++) // ..je regarde sa proximité avec un contenu de tableau2
		{
			// comparaison

			distance = distanceEntre2StartSimple(tableau1[k], tableau2[l], layerLength[layeri]);

			if (distance <= smallest || found == false)
			{
				smallest = distance;
				found = true;
				chosenL = l;
			}
		}

		// je prends la note k dans le layer
		// je calcule sa duree
		// son start devient la valeur de tableau2[chosenL]
		// son end devient start + duree

		pileNote tempNote;
		tempNote = ZEROgetNoteFromPile(pInitial[layeri], k);
		long duree = getDuree(tempNote);
		tempNote.startNote = tableau2[chosenL];
		tempNote.endNote = tempNote.startNote + duree;
		ZEROSetNoteInPile(pInitial[layeri], k, tempNote);
	}

	return;
}

void implicitLayerBeats(int layeri, int minimum, int maximum)
// va répartir les notes d'un layer entre minimum et maximum beats, vers la répartition la plus proche
{
	//	bool firstHigher = true;

	// on place les notes dans le tableau1 : le startNote
	// on calcule la taille du tableau1
	unsigned int taille = mesureTaillePileComplete(pInitial[layeri]);
	long tableau1[taille];

	for (unsigned int j = 0; j < taille; j++)
	{
		pileNote tempNote;
		tempNote = ZEROgetNoteFromPile(pInitial[layeri], j);
		tableau1[j] = tempNote.startNote;
	}

	unsigned int taille1 = taille; // nombre de notes dans le tableau1

	// on va tester les nombres de divisions possibles

	bool foundFirstSumSquare = false;
	long long minimalSumSquare = 0;
	int smallestI = 0;
	int smallestJ = 0;

	for (int i = minimum; i <= maximum; i++) // parcourt le nb de divisions possibles
	{
		unsigned long tableau2[i];

		// on calcule la durée d'un bloc pour cette valeur

		unsigned long block = layerLength[layeri] / (long)i;

		// on va tester les différents décalages possibles

		long long sumSquare = 0;

		for (unsigned long j = 0; j <= block; j++) // parcourt pour chaque division les décalages possibles
		{

			sumSquare = 0;

			// calcul des valeurs du tableau2
			for (int k = 0; k < i; k++)
			{
				tableau2[k] = k * block + j;
				if (tableau2[k] > layerLength[layeri])
				{
					tableau2[k] -= layerLength[layeri];
				}
			}

			// on a maintenant tableau1 avec les notes de départ et tableau2 avec la comparaison
			// on va trouver la note la plus proche de tableau1 dans tableau2 et faire une somme

			sumSquare = 0;

			for (unsigned int k = 0; k < taille1; k++) // pour chaque note..
			{
				long smallest = 0;
				bool found = false;
				long distance = 0;

				for (int l = 0; l < i; l++) // ..je regarde sa proximité avec un contenu de tableau2
				{
					distance = distanceEntre2StartSimple(tableau1[k], tableau2[l], layerLength[layeri]);

					if (distance < smallest || found == false)
					{
						smallest = distance;
						found = true;
					}
				}

				sumSquare = sumSquare + smallest * smallest;
			}

			if (sumSquare <= minimalSumSquare || foundFirstSumSquare == false)
			{
				minimalSumSquare = sumSquare;
				foundFirstSumSquare = true;

				// on connait I,J
				// on va s'en souvenir pour appliquer la transfo après avoir tout parcouru
				smallestI = i;
				smallestJ = j;
			}

		} // FIN DE J : décalage

	} // FIN DE I : différentes découpes

	// si on arrive ici ça signifie échec : on va faire un implicit sur 256, forcé !
	buffer_and_HARD_setCursor(5, 5);
	buffer_and_HARD_print("**");
	buffer_and_HARD_print(smallestI);
	buffer_and_HARD_print("**");

	// comment repartir d'ici pour transformer le layer i
	// en layer influencé par ces valeurs ?

	int i = smallestI;
	int j = smallestJ;

	unsigned long block = layerLength[layeri] / (long)i;
	unsigned long tableau2[i];

	// calcul des valeurs du tableau2
	for (int k = 0; k < i; k++)
	{
		tableau2[k] = k * block + j;
		if (tableau2[k] > layerLength[layeri])
		{
			tableau2[k] -= layerLength[layeri];
		}
	}

	for (unsigned int k = 0; k < taille1; k++) // pour chaque note..
	{
		long smallest = 0;
		bool found = false;
		long distance = 0;
		int chosenL = 0;

		for (int l = 0; l < i; l++) // ..je regarde sa proximité avec un contenu de tableau2
		{
			// comparaison

			distance = distanceEntre2StartSimple(tableau1[k], tableau2[l], layerLength[layeri]);

			if (distance <= smallest || found == false)
			{
				smallest = distance;
				found = true;
				chosenL = l;
			}
		}

		// je prends la note k dans le layer
		// je calcule sa duree
		// son start devient la valeur de tableau2[chosenL]
		// son end devient start + duree

		pileNote tempNote;
		tempNote = ZEROgetNoteFromPile(pInitial[layeri], k);
		long duree = getDuree(tempNote);
		tempNote.startNote = tableau2[chosenL];
		tempNote.endNote = tempNote.startNote + duree;
		ZEROSetNoteInPile(pInitial[layeri], k, tempNote);
	}

	return;
}

void swingplicitLayerBeats(int layeri, int minimum, int maximum, int percent = 0)
// va répartir les notes d'un layer entre minimum et maximum beats, vers la répartition la plus proche
{
	//	bool firstHigher = true;

	// on place les notes dans le tableau1 : le startNote
	// on calcule la taille du tableau1
	unsigned int taille = mesureTaillePileComplete(pInitial[layeri]);
	long tableau1[taille];

	for (unsigned int j = 0; j < taille; j++)
	{
		pileNote tempNote;
		tempNote = ZEROgetNoteFromPile(pInitial[layeri], j);
		tableau1[j] = tempNote.startNote;
	}

	unsigned int taille1 = taille; // nombre de notes dans le tableau1

	// on va tester les nombres de divisions possibles

	bool foundFirstSumSquare = false;
	long long minimalSumSquare = 0;
	int smallestI = 0, smallestJ = 0, smallestZ = 0;

	for (int i = minimum; i <= maximum; i++) // parcourt le nb de divisions possibles
	{
		unsigned long tableau2[i * 2]; // * 2 car il y a le beat et son swing

		// on calcule la durée d'un bloc pour cette valeur

		unsigned long block = layerLength[layeri] / (long)i;

		// on va tester les différents décalages possibles

		long long sumSquare = 0;

		for (unsigned long j = 0; j <= block; j++) // parcourt pour chaque division les décalages possibles
		{

			unsigned long minZ, maxZ;

			if (percent == 0)
			{
				minZ = 0;
				maxZ = block;
			}
			else
			{
				minZ = (unsigned long)((float)((float)block * (float)percent / (float)100));
				maxZ = minZ;
				buffer_and_HARD_setCursor(5, 4);
				buffer_and_HARD_print(minZ);
			}

			for (unsigned long z = minZ; z <= maxZ; z++) // parcourt pour chaque beat et décalage tous les swing possibles ( msec )
			{

				sumSquare = 0;

				// calcul des valeurs du tableau2
				for (int k = 0; k < i; k++)
				{
					tableau2[k] = k * block + j;		 // beat
					tableau2[i + k] = k * block + j + z; // swing

					if (tableau2[k] > layerLength[layeri])
					{
						tableau2[k] -= layerLength[layeri];
					}

					if (tableau2[i + k] > layerLength[layeri])
					{
						tableau2[i + k] -= layerLength[layeri];
					}
				}

				// on a maintenant tableau1 avec les notes de départ et tableau2 avec la comparaison
				// on va trouver la note la plus proche de tableau1 dans tableau2 et faire une somme

				sumSquare = 0;

				for (unsigned int k = 0; k < taille1; k++) // pour chaque note..
				{
					long smallest = 0;
					bool found = false;
					long distance = 0;

					for (int l = 0; l < 2 * i; l++) // ..je regarde sa proximité avec un contenu de tableau2
					{
						distance = distanceEntre2StartSimple(tableau1[k], tableau2[l], layerLength[layeri]);

						if (distance < smallest || found == false)
						{
							smallest = distance;
							found = true;
						}
					}

					sumSquare = sumSquare + smallest * smallest;

					if (foundFirstSumSquare == true && sumSquare > minimalSumSquare)
					{
						k = taille1; // va interrompre la boucle
					}
				}

				if (sumSquare <= minimalSumSquare || foundFirstSumSquare == false)
				{
					minimalSumSquare = sumSquare;
					foundFirstSumSquare = true;

					// on connait I,J
					// on va s'en souvenir pour appliquer la transfo après avoir tout parcouru
					smallestI = i;
					smallestJ = j;
					smallestZ = z;
				}

			} // FIN DE Z : swing

		} // FIN DE J : décalage

	} // FIN DE I : différentes découpes

	// si on arrive ici ça signifie échec : on va faire un implicit sur 256, forcé !

	// comment repartir d'ici pour transformer le layer i
	// en layer influencé par ces valeurs ?

	int i = smallestI;
	int j = smallestJ;
	int z = smallestZ;

	unsigned long block = layerLength[layeri] / (long)i;
	unsigned long tableau2[2 * i];

	buffer_and_HARD_setCursor(0, 5);
	buffer_and_HARD_print("*");
	buffer_and_HARD_print(smallestI);
	buffer_and_HARD_print("-");
	buffer_and_HARD_print(block);
	buffer_and_HARD_print("-");
	buffer_and_HARD_print(smallestJ);
	buffer_and_HARD_print("-");
	buffer_and_HARD_print(smallestZ);
	buffer_and_HARD_print("-");
	buffer_and_HARD_print("*");
	// delay(1000);

	// calcul des valeurs du tableau2
	for (int k = 0; k < i; k++)
	{
		tableau2[k] = k * block + j;		 // beat
		tableau2[i + k] = k * block + j + z; // swing

		if (tableau2[k] > layerLength[layeri])
		{
			tableau2[k] -= layerLength[layeri];
		}

		if (tableau2[i + k] > layerLength[layeri])
		{
			tableau2[i + k] -= layerLength[layeri];
		}
	}

	for (unsigned int k = 0; k < taille1; k++) // pour chaque note..
	{
		long smallest = 0;
		bool found = false;
		long distance = 0;
		int chosenL = 0;

		for (int l = 0; l < 2 * i; l++) // ..je regarde sa proximité avec un contenu de tableau2
		{
			// comparaison

			distance = distanceEntre2StartSimple(tableau1[k], tableau2[l], layerLength[layeri]);

			if (distance <= smallest || found == false)
			{
				smallest = distance;
				found = true;
				chosenL = l;
			}
		}

		// je prends la note k dans le layer
		// je calcule sa duree
		// son start devient la valeur de tableau2[chosenL]
		// son end devient start + duree

		pileNote tempNote;
		tempNote = ZEROgetNoteFromPile(pInitial[layeri], k);
		long duree = getDuree(tempNote);
		tempNote.startNote = tableau2[chosenL];
		tempNote.endNote = tempNote.startNote + duree;
		ZEROSetNoteInPile(pInitial[layeri], k, tempNote);
	}

	return;
}

void switch2NotesInPile(pileNote *adresseOriginalPilePtr, int noteA, int noteB)
// programmé le 25 octobre 2019 en une soirée
// laborieux à cause du premier élément de pointeur, qui n'est pas une note
// j'oubliais de faire pointer celui-ci vers A ou B quand il suivait juste après
// c'est un print des valeurs qui m'a mis sur la piste
// toujours VOIR
// l'intérêt est d'avoir une procédure extrêmement rapide pour manipuler l'ordre dans la pile
// il y a très peu de modif d'information, 2 ou 3 valeurs qui changent, plutôt que des dizaines de bytes si tout le champ pileNote était transféré
// ceci sera utile pour diverses arpégiation, mais aussi pour les kompakteurs
{
	if (adresseOriginalPilePtr == 0)
	{
		return;
	}

	if (noteB == noteA)
	{
		return;
	}

	pileNote *adresseParcoursPilePtr = adresseOriginalPilePtr;

	pileNote *preA = 0;
	pileNote *A = 0;
	// pileNote *postA = 0 ;
	pileNote *preB = 0;
	pileNote *B = 0;
	// pileNote *postB = 0 ;

	int compteur = 0;

	// u8x8.clearLine(6);

	do
	{
		adresseParcoursPilePtr = (*adresseParcoursPilePtr).nextPile;

		if (compteur == (noteA - 1))
		{
			preA = adresseParcoursPilePtr;
		}

		if (compteur == noteA)
		{
			A = adresseParcoursPilePtr;
		}

		if (compteur == (noteB - 1))
		{
			preB = adresseParcoursPilePtr;
		}

		if (compteur == noteB)
		{
			B = adresseParcoursPilePtr;
		}

		compteur++;
	} while ((*adresseParcoursPilePtr).nextPile != 0);

	// on a normalement en une seule passe trouvé toutes les adresses nécessaires

	if (noteB == (noteA + 1))
	{
		(*A).nextPile = (*B).nextPile;
		(*B).nextPile = A;
		if (noteA != 0)
		{
			(*preA).nextPile = B;
		}
		else
		{
			(*adresseOriginalPilePtr).nextPile = B;
		}

		return;
	}

	if (noteA == (noteB + 1))
	{
		(*B).nextPile = (*A).nextPile;
		(*A).nextPile = B;
		if (noteB != 0)
		{
			(*preB).nextPile = A;
		}
		else
		{
			(*adresseOriginalPilePtr).nextPile = A;
		}

		return;
	}

	// CAS CLASSIQUE

	pileNote *C;
	C = (*A).nextPile;
	(*A).nextPile = (*B).nextPile;
	(*B).nextPile = C;

	if (noteA != 0)
	{
		(*preA).nextPile = B;
	}
	else
	{
		(*adresseOriginalPilePtr).nextPile = B;
	}

	if (noteB != 0)
	{
		(*preB).nextPile = A;
	}
	else
	{
		(*adresseOriginalPilePtr).nextPile = A;
	}

	return;
}

void insertNoteBBeforeAInPile(pileNote *adresseOriginalPilePtr, int noteA, int noteB)
// programmé le 25 octobre 2019 en une soirée
// laborieux à cause du premier élément de pointeur, qui n'est pas une note
// j'oubliais de faire pointer celui-ci vers A ou B quand il suivait juste après
// c'est un print des valeurs qui m'a mis sur la piste
// toujours VOIR
// l'intérêt est d'avoir une procédure extrêmement rapide pour manipuler l'ordre dans la pile
// il y a très peu de modif d'information, 2 ou 3 valeurs qui changent, plutôt que des dizaines de bytes si tout le champ pileNote était transféré
// ceci sera utile pour diverses arpégiation, mais aussi pour les kompakteurs
{
	if (adresseOriginalPilePtr == 0)
	{
		return;
	}

	if (noteB == noteA)
	{
		return;
	}

	pileNote *adresseParcoursPilePtr = adresseOriginalPilePtr;

	pileNote *preA = 0;
	pileNote *A = 0;
	// pileNote *postA = 0 ;
	pileNote *preB = 0;
	pileNote *B = 0;
	// pileNote *postB = 0 ;

	int compteur = 0;

	// u8x8.clearLine(6);

	do
	{
		adresseParcoursPilePtr = (*adresseParcoursPilePtr).nextPile;

		if (compteur == (noteA - 1))
		{
			preA = adresseParcoursPilePtr;
		}

		if (compteur == noteA)
		{
			A = adresseParcoursPilePtr;
		}

		if (compteur == (noteB - 1))
		{
			preB = adresseParcoursPilePtr;
		}

		if (compteur == noteB)
		{
			B = adresseParcoursPilePtr;
		}

		compteur++;
	} while ((*adresseParcoursPilePtr).nextPile != 0);

	// on a normalement en une seule passe trouvé toutes les adresses nécessaires

	// on a preA, A, preB, B
	// si noteA ou noteB = 0, alors pre = (*adresseOriginalPilePtr)

	if (noteA != 0)
	{
		(*preA).nextPile = B;
	}
	else
	{
		(*adresseOriginalPilePtr).nextPile = B;
	}

	if (noteB != 0)
	{
		(*preB).nextPile = (*B).nextPile;
	}
	else
	{
		(*adresseOriginalPilePtr).nextPile = (*B).nextPile;
	}

	(*B).nextPile = A;

	return;
}

long getValueFrom(pileNote *adresseParcours2PilePtr, int modeSort)
{
	long value = 0;

	switch (modeSort)
	{
	case 0:
		value = (*adresseParcours2PilePtr).note;
		break;
	case 1:
		value = (*adresseParcours2PilePtr).velocity;
		break;
	case 2:
		value = (*adresseParcours2PilePtr).startNote;
		break;
	case 3:
		value = (*adresseParcours2PilePtr).endNote;
		break;
	case 4:
		value = getDuree(*adresseParcours2PilePtr);
		break;
	default:
		value = 0;
		break;
	}

	return value;
}

void sortPile(pileNote *adresseOriginalPilePtr, byte modeSort, byte ascendant)
// ascendant = 0 si ascendant
// = 1 si descendant

{
	if (adresseOriginalPilePtr == 0)
	{
		return;
	}

	pileNote *adresseParcoursPilePtr = adresseOriginalPilePtr;

	// pileNote *preA = 0;
	// pileNote *A = 0;
	// // pileNote *postA = 0 ;
	// pileNote *preB = 0;
	// pileNote *B = 0;
	// pileNote *postB = 0 ;

	int j = 1;

	// on doit parcourir de la note 2 ( 1 dans array ) à finale
	adresseParcoursPilePtr = (*adresseParcoursPilePtr).nextPile;
	//  pointe la valeur de note 0array 1ère

	if ((*adresseParcoursPilePtr).nextPile == 0)
	{
		// çàd il y a une seule note : rien à trier
		return;
	}

	adresseParcoursPilePtr = (*adresseParcoursPilePtr).nextPile;

	// J = 2 à taille

	while (adresseParcoursPilePtr != 0)
	{
		// commence par valoir valeur de note 1(array) (2ème)

		long key = 0;

		// valeur à tester :

		key = getValueFrom(adresseParcoursPilePtr, modeSort);

		// if (modeSort == 0)
		// {
		// 	// on va tester le pitch ASC
		// 	key = (*adresseParcoursPilePtr).note;
		// }

		// u8x8.clearLine(5);
		buffer_clearLine_HARD(5);
		buffer_and_HARD_setCursor(0, 5);
		buffer_and_HARD_print(j);
		buffer_and_HARD_print("=");
		buffer_and_HARD_print(key);
		// delay(1000);

		// on va parcourir les notes qui précèdent j
		// si on trouve une valeur plus grande, on place la note J juste avant

		int i = 0;

		pileNote *Next = (*adresseParcoursPilePtr).nextPile;

		pileNote *adresseParcours2PilePtr = adresseOriginalPilePtr;
		// on doit parcourir de la note 1 ( 0 dans array ) à j-1

		do
		{
			adresseParcours2PilePtr = (*adresseParcours2PilePtr).nextPile;
			//  pointe la valeur de note 0array 1ère

			long key2 = 0;

			// valeur à tester :
			// TODO remplacer par une fc qui fait donne valeur en fc de choix et qui utilise pointeur réel
			// key2 = giveValue(pointeur,VALEUR ) ;
			// if (modeSort == 0)
			// {
			// 	// on va tester le pitch ASC
			// 	key2 = (*adresseParcours2PilePtr).note;
			// }

			key2 = getValueFrom(adresseParcours2PilePtr, modeSort);

			// u8x8.clearLine(5);
			buffer_and_HARD_setCursor(10, 5);
			buffer_and_HARD_print(i);
			buffer_and_HARD_print("=");
			buffer_and_HARD_print(key2);
			// delay(1000);

			if (ascendant == 0 && key2 > key)
			// if ( false == true )
			{
				// u8x8.clearLine(6);
				buffer_clearLine_HARD(6);
				buffer_and_HARD_setCursor(10, 6);
				buffer_and_HARD_print("INS");
				// delay(1000);
				// u8x8.clearLine(6);
				buffer_clearLine_HARD(6);

				insertNoteBBeforeAInPile(adresseOriginalPilePtr, i, j);

				// u8x8.clearLine(6);
				buffer_clearLine_HARD(6);
				buffer_and_HARD_setCursor(10, 6);
				buffer_and_HARD_print(">");
				//	delay(1000);
				// u8x8.clearLine(6);
				buffer_clearLine_HARD(6);

				// ne modifie pas l'emplacement en mémoire, par contre le suivant de B ne sera plus celui qu'il était !
				// B = Next ;

				i = j; // va arrêter la boucle car i n'est plus plus petit que j
			}

			if (ascendant == 1 && key2 < key)
			// if ( false == true )
			{
				// u8x8.clearLine(6);
				buffer_clearLine_HARD(6);
				buffer_and_HARD_setCursor(10, 6);
				buffer_and_HARD_print("INS");
				// 	delay(1000);
				// u8x8.clearLine(6);
				buffer_clearLine_HARD(6);

				insertNoteBBeforeAInPile(adresseOriginalPilePtr, i, j);

				// u8x8.clearLine(6);
				buffer_clearLine_HARD(6);
				buffer_and_HARD_setCursor(10, 6);
				buffer_and_HARD_print("<");
				//	delay(1000);
				//				u8x8.clearLine(6);
				buffer_clearLine_HARD(6);

				// ne modifie pas l'emplacement en mémoire, par contre le suivant de B ne sera plus celui qu'il était !
				// B = Next ;

				i = j; // va arrêter la boucle car i n'est plus plus petit que j
			}

			i++;
		} while (i < j);

		//	u8x8.clearLine(6);
		buffer_clearLine_HARD(6);
		buffer_and_HARD_setCursor(10, 6);
		buffer_and_HARD_print("NEX");
		//	delay(1000);
		//	u8x8.clearLine(6);
		buffer_clearLine_HARD(6);

		adresseParcoursPilePtr = Next;

		j++;
	}

	return;
}

void sortPile_CLEAN(pileNote *adresseOriginalPilePtr, byte modeSort, byte ascendant)
// ascendant = 0 si ascendant
// = 1 si descendant

{
	if (adresseOriginalPilePtr == 0)
	{
		return;
	}

	pileNote *adresseParcoursPilePtr = adresseOriginalPilePtr;

	int j = 1;

	adresseParcoursPilePtr = (*adresseParcoursPilePtr).nextPile;

	if ((*adresseParcoursPilePtr).nextPile == 0)
	{
		return;
	}

	adresseParcoursPilePtr = (*adresseParcoursPilePtr).nextPile;

	while (adresseParcoursPilePtr != 0)
	{
		long key = 0;

		key = getValueFrom(adresseParcoursPilePtr, modeSort);

		int i = 0;

		pileNote *Next = (*adresseParcoursPilePtr).nextPile;
		pileNote *adresseParcours2PilePtr = adresseOriginalPilePtr;

		do
		{
			adresseParcours2PilePtr = (*adresseParcours2PilePtr).nextPile;

			long key2 = 0;
			key2 = getValueFrom(adresseParcours2PilePtr, modeSort);

			if (ascendant == 0 && key2 > key)
			{
				insertNoteBBeforeAInPile(adresseOriginalPilePtr, i, j);
				i = j;
			}

			if (ascendant == 1 && key2 < key)
			{
				insertNoteBBeforeAInPile(adresseOriginalPilePtr, i, j);
				i = j;
			}

			i++;
		} while (i < j);

		adresseParcoursPilePtr = Next;

		j++;
	}

	return;
}

// const float PI = 3.14157 ;  existe déjà dans le programme

float mapF(float init, float a, float b, float c, float d)
{
	float rapport = (d - c) / (b - a);
	float res = (((init - a) * rapport) + c);
	return res;
}

unsigned long piToTime(float piValue, float timeMin, float timeMax)
{
	// float timeSpan = timeMax - timeMin ;
	//  cout << endl << "timespan " << timeSpan << endl ;

	// on fait un mapping entre -PI/2 et PI/2
	//    // non, entre -PI et PI et timeMin et timeMax
	// NON NON, entre -pi/2 et pi/2

	//    float result ;
	//
	//    cout << "piValue devient " << piValue + PI << endl ;
	//    cout << "mapping O-2PI vers 0-" << timeSpan << endl ;
	//
	//    result = (float)((( (float) piValue + (float) PI )/ (float)2*(float) PI ) * (float) timeSpan ) ;
	//cout << "-PI<" << piValue << "<+PI " << timeMin <<"<" << result << "<" << timeMax << endl ;

	if (piValue >= 1.57)
	{
		return timeMax;
	}
	if (piValue <= -1.57)
	{
		return timeMin;
	}

	float approx = mapF(piValue, -PI / 2, PI / 2, timeMin, timeMax);
	if (approx >= timeMax)
	{
		approx = timeMax;
	}
	if (approx <= timeMin)
	{
		approx = timeMin;
	}

	return ((unsigned long)(approx));
}

bool isPrime(long long n)
// repris de Euler10 et amÃ©liorÃ© !
{
	if (n <= 1)
	{
		return false;
	}

	if (n == 2)
	{
		return true;
	}

	if (n == 3)
	{
		return true;
	}

	for (long long i = 2; i < (sqrt(n) + 1); i++)
	{
		if (n % i == 0)
		{
			return false;
		}
	}

	return true;
}

bool layerLengthAisIdemAtLeastAnotherActiveOne(byte n)
{

	for (byte k = 0; k < nbLayer; k++)
	{
		if ((k != n) && (layerLength[k] == layerLength[n]) && (isLayerActive(k)))
		{
			return true;
		}
	}

	return false;
}

unsigned long checkLayer(signed long layer)
{
	if (layer < 1)
	{
		layer = 1;
	}
	return layer;
}

void setBit(int &modif, int place, int valeur)
/**
 * @brief modifie la valeur du bit à la place, en lui attribuant valeur ( 0 ou 1 )
 * 
 */
{
	if (place < 0)
	{
		return;
	}
	if (place > 7)
	{
		return;
	}
	if (valeur < 0)
	{
		return;
	}
	if (valeur > 1)
	{
		return;
	}

	int k = 1 << place;
	int a = modif;
	if (valeur == 0)
	{
		a = a & !k;
	}
	if (valeur == 1)
	{
		a = a | k;
	}
	modif = a;
}

// fonction qui donne current Stepz pour PolyZ
int giveStepzForPolyZSubStepz(int i)
{
	int nbCasesTotal = numerateurTimeSignature * denominateurTimeSignature;
	int pos = (nowMillis % layerLength[i]) / (layerLength[i] / nbCasesTotal);
	// pos indique dans quel subStepz on est

	// il faut maintenant le convetir en Stepz
	int nbStepz = 0;
	for (int j = 0; j < pos; j++)
	{
		if ((j % numerateurTimeSignature == 0) || (j % denominateurTimeSignature == 0))
		{
			nbStepz++;
		}
		else
		{
		}
	}
	if (nbStepz == 0)
	{
		return 0;
	}
	else
	{
		return (nbStepz - 1);
	};
}

// Anciennement permute.h

void swapChar(char *s, int a, int b)
{
	char temp = s[a];
	s[a] = s[b];
	s[b] = temp;
}

void swapInt(int *s, int a, int b)
{
	int temp = s[a];
	s[a] = s[b];
	s[b] = temp;
}

int permuteInt(int *str, int len)
{
	int key = len - 1;
	int newkey = len - 1;
	while ((key > 0) && (str[key] <= str[key - 1]))
		key--;
	key--;
	if (key < 0)
		return 0;
	newkey = len - 1;
	while ((newkey > key) && (str[newkey] <= str[key]))
	{
		newkey--;
	}
	swapInt(str, key, newkey);
	len--;
	key++;
	while (len > key)
	{
		swapInt(str, len, key);
		key++;
		len--;
	}
	return 1;
}

int antiPermuteInt(int *str, int len)
{
	int key = len - 1;
	int newkey = len - 1;
	while ((key > 0) && (str[key] >= str[key - 1]))
		key--;
	key--;
	if (key < 0)
		return 0;
	newkey = len - 1;
	while ((newkey > key) && (str[newkey] >= str[key]))
	{
		newkey--;
	}
	swapInt(str, key, newkey);
	len--;
	key++;
	while (len > key)
	{
		swapInt(str, len, key);
		key++;
		len--;
	}
	return 1;
}

/////////////////////////////////////////////////////////////////////////////////////
// 2C29 Conway - 20 juillet 2020
// adaptation des Johnson réalisés avec Mathieu Lilin l'année passée
// folder Tom
/////////////////////////////////////////////////////////////////////////////////////

void transformJohnson1UP(byte n)
{

	if (sequence[n].tailleSeq > tailleMaxTableauSequence / 2)
	{
		return;
	}

	for (int i = 0; i < sequence[n].tailleSeq; i++)
	{
		sequence[n].tableauSeq[i + sequence[n].tailleSeq] = sequence[n].tableauSeq[i];
	}

	sequence[n].tailleSeq = 2 * sequence[n].tailleSeq;
	sequence[n].tableauSeq[sequence[n].tailleSeq - 1]++;
}

void transformJohnson1DOWN(byte n)
{
	if (sequence[n].tailleSeq < 2)
	{
		return;
	}

	if (sequence[n].tailleSeq % 2 == 1)
	{
		return;
	}

	sequence[n].tailleSeq = sequence[n].tailleSeq / 2;
}

void transformJohnson2UP(byte n)
{

	if (sequence[n].tailleSeq > tailleMaxTableauSequence / 2)
	{
		return;
	}

	// trouver le plus grand nombre
	int M = 0;
	for (int i = 0; i < sequence[n].tailleSeq; i++)
	{
		if (sequence[n].tableauSeq[i] > M)
		{
			M = sequence[n].tableauSeq[i];
		}
	}

	for (int i = 0; i < sequence[n].tailleSeq; i++)
	{
		sequence[n].tableauSeq[i + sequence[n].tailleSeq + 1] = sequence[n].tableauSeq[i];
	}

	sequence[n].tableauSeq[sequence[n].tailleSeq] = M + 1;
	sequence[n].tailleSeq = 2 * sequence[n].tailleSeq + 1;
}

void transformJohnson2DOWN(byte n)
{
	if (sequence[n].tailleSeq <= 1)
	{
		return;
	}

	if (sequence[n].tailleSeq % 2 == 0)
	{
		return;
	}

	sequence[n].tailleSeq = (sequence[n].tailleSeq - 1) / 2;
}

void transformJohnson3UP(byte n)
{

	if (sequence[n].tailleSeq == 1)
	{
		sequence[n].tableauSeq[1] = sequence[n].tableauSeq[0];
		sequence[n].tailleSeq = 2;
		return;
	}

	if (sequence[n].tailleSeq > tailleMaxTableauSequence / 2)
	{
		return;
	}

	for (int i = sequence[n].tailleSeq - 1; i >= sequence[n].tailleSeq / 2; i--)
	{
		sequence[n].tableauSeq[i + sequence[n].tailleSeq] = sequence[n].tableauSeq[i];
		sequence[n].tableauSeq[i + sequence[n].tailleSeq / 2] = sequence[n].tableauSeq[i];
	}

	for (int i = 0; i < sequence[n].tailleSeq / 2; i++)
	{
		sequence[n].tableauSeq[i + sequence[n].tailleSeq / 2] = sequence[n].tableauSeq[i];
	}

	sequence[n].tailleSeq *= 2;
	sequence[n].tableauSeq[sequence[n].tailleSeq / 2 - 1]++;
	sequence[n].tableauSeq[sequence[n].tailleSeq / 2]++;
}

void transformJohnson3DOWN(byte n)
{
	if (sequence[n].tailleSeq < 2)
	{
		return;
	}

	if (sequence[n].tailleSeq % 2 == 1)
	{
		return;
	}

	sequence[n].tailleSeq = sequence[n].tailleSeq / 2;

	for (int i = sequence[n].tailleSeq / 2; i < sequence[n].tailleSeq; i++)
	{
		sequence[n].tableauSeq[i] = sequence[n].tableauSeq[i + sequence[n].tailleSeq];
	}
}

void transformJohnson4UP(byte n)
{

	if (sequence[n].tailleSeq > tailleMaxTableauSequence / 2)
	{
		return;
	}

	// trouver le plus grand nombre
	int M = 0;
	for (int i = 0; i < sequence[n].tailleSeq; i++)
	{
		if (sequence[n].tableauSeq[i] > M)
		{
			M = sequence[n].tableauSeq[i];
		}
	}

	// J'ai eu du fil à retordre ici, en effet je commmençais par transformer le début de la séquence
	// et puis la fin, oubliant que la transfo du début modifiait les valeurs de fin !!!!

	for (int i = sequence[n].tailleSeq - 1; i >= sequence[n].tailleSeq / 2; i--)
	{
		sequence[n].tableauSeq[i * 2 + 1] = M + 1;
		sequence[n].tableauSeq[i * 2] = sequence[n].tableauSeq[i];
	}

	for (int i = sequence[n].tailleSeq / 2 - 1; i >= 0; i--)
	{
		sequence[n].tableauSeq[i * 2 + 1] = sequence[n].tableauSeq[i];
		sequence[n].tableauSeq[i * 2] = M + 1;
	}

	sequence[n].tailleSeq = 2 * sequence[n].tailleSeq;
}

void transformJohnson4DOWN(byte n)
{
	if (sequence[n].tailleSeq <= 2)
	{
		return;
	}

	if (sequence[n].tailleSeq % 2 == 1)
	{
		return;
	}

	for (int i = 0; i < sequence[n].tailleSeq / 2; i++)
	{
		if (i < sequence[n].tailleSeq / 4)
		{
			sequence[n].tableauSeq[i] = sequence[n].tableauSeq[i * 2 + 1];
		}
		else
		{
			sequence[n].tableauSeq[i] = sequence[n].tableauSeq[i * 2];
		}
	}
	sequence[n].tailleSeq = (sequence[n].tailleSeq) / 2;
}

void transformJohnson5UP(byte n)
{

	if (sequence[n].tailleSeq > tailleMaxTableauSequence / 2)
	{
		return;
	}

	// trouver le plus grand nombre
	int M = 0;
	for (int i = 0; i < sequence[n].tailleSeq; i++)
	{
		if (sequence[n].tableauSeq[i] > M)
		{
			M = sequence[n].tableauSeq[i];
		}
	}

	for (int i = sequence[n].tailleSeq; i >= 0; i--)
	{
		sequence[n].tableauSeq[i * 2] = sequence[n].tableauSeq[i];
		sequence[n].tableauSeq[i * 2 + 1] = M + 1;
	}

	sequence[n].tailleSeq = 2 * sequence[n].tailleSeq;
}

void transformJohnson5DOWN(byte n)
{
	if (sequence[n].tailleSeq <= 2)
	{
		return;
	}

	if (sequence[n].tailleSeq % 2 == 1)
	{
		return;
	}

	for (int i = 0; i < sequence[n].tailleSeq / 2; i++)
	{
		sequence[n].tableauSeq[i] = sequence[n].tableauSeq[i * 2];
	}

	sequence[n].tailleSeq = (sequence[n].tailleSeq) / 2;
}

void transformJohnson6UP(byte n)
{

	if (sequence[n].tailleSeq > tailleMaxTableauSequence / 2)
	{
		return;
	}

	// trouver le plus grand nombre
	int M = 0;
	for (int i = 0; i < sequence[n].tailleSeq; i++)
	{
		if (sequence[n].tableauSeq[i] > M)
		{
			M = sequence[n].tableauSeq[i];
		}
	}

	int demiTaille = sequence[n].tailleSeq / 2;

	for (int i = 0; i < demiTaille; i++)
	{
		sequence[n].tableauSeq[sequence[n].tailleSeq * 2 - 1 - i] = sequence[n].tableauSeq[sequence[n].tailleSeq - 1 - i];
	}

	for (int i = 0; i < demiTaille; i++)
	{
		sequence[n].tableauSeq[sequence[n].tailleSeq * 2 - 1 - demiTaille - i] = sequence[n].tableauSeq[sequence[n].tailleSeq - 1 - demiTaille / 2 - i];
	}
	sequence[n].tableauSeq[sequence[n].tailleSeq] = M + 1;

	for (int i = 0; i < demiTaille; i++)
	{
		sequence[n].tableauSeq[sequence[n].tailleSeq - 1 - i] = sequence[n].tableauSeq[sequence[n].tailleSeq - 1 - demiTaille / 2 - i];
	}
	sequence[n].tableauSeq[sequence[n].tailleSeq - 1] = M + 1;

	sequence[n].tailleSeq = 2 * sequence[n].tailleSeq;
}

void transformJohnson6DOWN(byte n)
{
	if (sequence[n].tailleSeq <= 2)
	{
		return;
	}

	if (sequence[n].tailleSeq % 2 == 1)
	{
		return;
	}

	sequence[n].tableauSeq[sequence[n].tailleSeq / 2 - 1] = sequence[n].tableauSeq[sequence[n].tailleSeq - 1];

	sequence[n].tailleSeq = (sequence[n].tailleSeq) / 2;
}

void transformJohnson7UP(byte n)
{

	if (sequence[n].tailleSeq > tailleMaxTableauSequence / 2)
	{
		return;
	}

	// trouver le plus grand nombre
	int M = 0;
	for (int i = 0; i < sequence[n].tailleSeq; i++)
	{
		if (sequence[n].tableauSeq[i] > M)
		{
			M = sequence[n].tableauSeq[i];
		}
	}

	int demiTaille = sequence[n].tailleSeq / 2;

	for (int i = 0; i < demiTaille; i++)
	{
		sequence[n].tableauSeq[sequence[n].tailleSeq * 2 - 1 - i] = sequence[n].tableauSeq[sequence[n].tailleSeq - 1 - i];
	}

	for (int i = 0; i < demiTaille; i++)
	{
		sequence[n].tableauSeq[sequence[n].tailleSeq * 2 - 1 - demiTaille - i] = sequence[n].tableauSeq[sequence[n].tailleSeq - 1 - demiTaille / 2 - i];
	}
	sequence[n].tableauSeq[sequence[n].tailleSeq] = M + 1;

	for (int i = 0; i < demiTaille; i++)
	{
		sequence[n].tableauSeq[sequence[n].tailleSeq - 1 - i] = sequence[n].tableauSeq[sequence[n].tailleSeq - 1 - demiTaille / 2 - i];
	}
	sequence[n].tableauSeq[sequence[n].tailleSeq - 1] = M + 1;

	if (sequence[n].tailleSeq > 8)
	{
		sequence[n].tableauSeq[demiTaille / 2 - 1] = M + 1;
		sequence[n].tableauSeq[demiTaille / 2 * 3] = M + 1;
	}

	sequence[n].tailleSeq = 2 * sequence[n].tailleSeq;
}

void jumpToTransfo(int Tf)
{
	softwareValueEncoderLeft = LineCallingTransfo(Tf);

	minimumSoftwareValueEncoderRight = Transfo[softwareValueEncoderLeft][1];
	maximumSoftwareValueEncoderRight = Transfo[softwareValueEncoderLeft][2];

	oldCounterLeft = softwareValueEncoderLeft;
	softwareValueEncoderRight = Transfo[softwareValueEncoderLeft][4];
	oldCounterRight = softwareValueEncoderRight;
}

int getValueStraightOrFromSequence(int value, int floor)
{
	if (value >= floor) // BLACK08 >= me paraîtrait plus logique
	{
		return value;
	}
	else
	{
		int seqChosen;
		seqChosen = -value + floor - 1;
		// pour un floor à -36 :
		// -37 devient 37 - 36 - 1, soit 0 ;
		// -38 devient 38 - 36 - 1, soit 1 ;
		// -39 devient 39 - 36 - 1, soit 2 ;
		//  etc...
		int temp = giveNextValue(sequence[seqChosen], sequence[seqChosen].compteur);
		sequence[seqChosen].compteur++;
		return temp;
	}
}

String afficheValueStraightOrFromSequence(int value, String blurb, int floor)
{
	String affiche = nulString; // BLACK 10 trouvé que affiche était déclaré non en local !!!!
	if (value >= floor)
	{
		affiche = String(value) + blurb;
	}
	else
	{
		affiche = giveStringOfSequence(-value + floor - 1);
		// 0 -> 0 - 1 - 1
	}
	return affiche;
}

void afficheBoiteARythme (byte i, byte stepChannel, byte currentStepzNote )
{
	unsigned int nbCases = nbStepz * nbSubStepz;
	unsigned long duree1subStep = layerLength[i] / (unsigned long)(nbCases);

	// on peut dès lors déterminer combien de notes dans chaque subStep
	// puis les regrouper par Step _ - = # 11 52

	byte tableauSubStepz[nbCases] = {0};
	byte tableauStepz[nbStepz] = {0};

	unsigned int taille = mesureTaillePileComplete(pInitial[i]);
	pileNote tempNote;

	// Attention, la note 0 est l'ACCENT !

	for (unsigned int j = 0; j < taille; j++)
	{ //
		tempNote = ZEROgetNoteFromPile(pInitial[i], j);
		if (tempNote.note == currentStepzNote && tempNote.channel == stepChannel)
		// Normalement valable aussi pour ACCENT
		{
			tableauSubStepz[tempNote.startNote / duree1subStep] = 1;
		}

		if (currentStepzNote == 128 && tempNote.note == 128) // cas particulier du TOTAL ACCENT
		{
			tableauSubStepz[tempNote.startNote / duree1subStep] = 1;
		}
	}

	for (unsigned int j = 0; j < nbStepz; j++)
	{
		for (unsigned k = 0; k < nbSubStepz; k++)
		{
			tableauStepz[j] = tableauStepz[j] + (tableauSubStepz[j * nbSubStepz + k] << k);
		}
	}

	affiche = nulString;
	affiche = String(currentPageStepz);
	affiche += ":";

	for (unsigned int j = 0; j < nbStepzAffiches; j++)
	{
		if (currentPageStepz * nbStepzAffiches + j < nbStepz)
		{
			affiche += String((tableauStepz[currentPageStepz * nbStepzAffiches + j]));
		}
	}

	printRot2(affiche, 16, 400, 7);
	// TODO DEBUG l'affichage n'est pas très pertinent quand plusieurs layers sont sélectionnés

	// faire affichage par Stepz avec nbNotes impliquées soit 111 -> 3 , soit 111 -> 8

	// affichage du déroulement du layerActive
	unsigned int pos = (nowMillis % layerLength[i]) / (layerLength[i] / nbStepz);
	affiche = nulString;
	affiche = String(currentPageStepz);
	affiche += ":";

	for (unsigned int j = 0; j < nbStepzAffiches; j++)
	{
		if ((int)currentPageStepz * (int)nbStepzAffiches + (int)j == (int)pos)
		{
			affiche += ".";
		}
		else
		{
			affiche += " ";
		}
	}

	printRot2(affiche, 16, 400, 6);
}
bool noteIsWhite(int noteDonnee)
{
	noteDonnee = noteDonnee % 12 ; 

if ( noteDonnee < 5 && noteDonnee%2 == 0 )
{
	return true ; 
}

if ( noteDonnee >=5 && noteDonnee%2 == 1 )
{
	return true ; 
}

return false ; 
}

int nombreDeBlanchesEntre( int noteInitiale , int floorNote  )
{

	if ( noteIsWhite(noteInitiale) == false ) { return 0 ; }

if ( noteInitiale < floorNote ) { return 0 ; }

int noteModulo = ( noteInitiale - floorNote ) % 12 ;
int add = 0 ;
int multip = (( noteInitiale - floorNote ) / 12 )  ; 

if ( noteModulo < 5 )
{
	add = noteModulo / 2  ; 
}
else
{
	add = ( noteModulo+1 ) / 2 ; 
}

return ( add + multip * 7 );  // BLACK09  

}



int nombreDeNoiresEntre(int noteInitiale, int floorNote)
{

if  ( noteIsWhite(noteInitiale) ) {return 0 ; }

	if (noteInitiale < floorNote)
	{
		return 0;
	}

	int noteModulo = (noteInitiale - floorNote) % 12;
	int add = 0;
	int multip = ((noteInitiale - floorNote) / 12);

	if (noteModulo < 5)
	{
		add = ( noteModulo - 1 ) / 2;
	}
	else
	{
		add = (noteModulo - 2 ) / 2;
	}

	return (add + multip * 7); // BLACK09
}

void simplifie(int &num, int &denom)
// fonction qui simplifie une fraction 
{
	int a = 2;
	int reste = 0;

	do
	{
		do
		{
			reste = num % a; // si le numérateur est divisible par un nombre
			if (reste == 0)
			{
				reste = denom % a; // et le dénominateur aussi

				if (reste == 0)
				{
					num = num / a; // alors on les simplifie
					denom = denom / a;
				}
			}
		} while (reste == 0);			  // autant de fois que parfait pour ce nombre
		a++;							  // sinon on passe au  nombre suivant ( je n'ai pas poussé jusqu'à faire liste des premiers !
	} while ((a <= num) && (a <= denom)); // et tout ça tant que le chiffre est plus petit ou égal au plus petit des deux
}

int chooseWithRatio(int a, int b = 0, int c = 0, int d = 0, int e = 0, int f = 0, int g = 0, int h = 0, int i = 0 , int j = 0 )
// permet de faire un choix à partir des ratios exposés 
// ex : choix 0 = 2 ; choix 1  = 1 ; choix 2 = 1
// chance d'avoir choix 0 = 2 / 4   choix 1 = 1/4    choix 2 = 1/4
// me semble plus pertinent que des pourcentages
{
int total = a+b+c+d+e+f+g+h+i+j ; 
int choix = rand()%total ; 
int sum = a ; 
if ( choix < sum ) { return 0 ; }
sum = sum + b;  
if ( choix < sum )  { return 1 ; }
sum = sum + c ; 
if ( choix < sum ) { return 2 ; }
sum = sum + d;  
if ( choix < sum )  { return 3 ; }
sum = sum + e ; 
if ( choix < sum ) { return 4 ; }
sum = sum + f;  
if ( choix < sum )  { return 5 ; }
sum = sum + g ; 
if ( choix < sum ) { return 6 ; }
sum = sum + h;  
if ( choix < sum )  { return 7 ; }
return 0 ; 

}

// NARAYANA 

void deleteEnfants()
{
	for (int i = 0; i < sizeTroupeauMax; i++)
	{
		Enfants[i][0] = 0; // âge de la vache ( 1 = nouveau-né ; 2, 3, 4 ... adulte    0 = unborn, dead )
		Enfants[i][1] = 0; // pitch : équivalent en fait à la génération
		Enfants[i][2] = 0; // génération
	}
}

void enfantsBecomeParents()
{
	for (int i = 0; i < sizeTroupeauMax; i++)
	{

		Parents[i][0] = Enfants[i][0];
		Parents[i][1] = Enfants[i][1];
		Parents[i][2] = Enfants[i][2];
	}
}

void showGenerationParents()
{
	for (int i = 0; i < sizeTroupeauMax; i++)
	{
		if (Parents[i][0] != 0)
		{
			if (Parents[i][0] > 3)
			{
			//	cout << "O";
			}
			else
			{
			//	cout << ".";
			}
		}
	}

//	cout << endl
//		 << endl;

	for (int i = 0; i < sizeTroupeauMax; i++)
	{
		if (Parents[i][0] != 0)
		{
	//		cout << Parents[i][1] << " ";
		}
	}
}

void buildNewGeneration()
{
	int indexParents = 0;
	int indexEnfants = 0;

	enfantsBecomeParents();
	deleteEnfants();

	for (int i = 0; i < sizeTroupeauMax; i++)
	{
		indexParents = i;

		if ((rand() % 100) < mortaliteFortuite)
		{
		}
		else if (Parents[indexParents][0] == 0)
		{
		}
		else if (Parents[indexParents][0] < (maturite - 1))
		{
			Enfants[indexEnfants][0] = Parents[indexParents][0] + 1;
			Enfants[indexEnfants][1] = Parents[indexParents][1];
			Enfants[indexEnfants][2] = Parents[indexParents][2];
			indexEnfants++;
		}
		// else if (  Parents[indexParents][0] >= 3 && Parents[indexParents][0] < 10 )
		else if ((Parents[indexParents][0] >= (maturite - 1)) && (Parents[indexParents][0] < menopause) && (Parents[indexParents][0] < mort))
		{
			Enfants[indexEnfants][0] = Parents[indexParents][0] + 1;
			Enfants[indexEnfants][1] = Parents[indexParents][1];
			Enfants[indexEnfants][2] = Parents[indexParents][2];
			indexEnfants++;

			for (int j = 0; j < taillePortee; j++)
			{
				Enfants[indexEnfants][0] = 1; // nouveau né
				Enfants[indexEnfants][2] = Parents[indexParents][2] + 1;
				Enfants[indexEnfants][1] = Parents[indexParents][1] - scale[Enfants[indexEnfants][2] % tailleScale ];
				indexEnfants++;
			}
		}
		else if ((Parents[indexParents][0] >= (maturite - 1)) && (Parents[indexParents][0] >= menopause) && (Parents[indexParents][0] < mort))
		{
			Enfants[indexEnfants][0] = Parents[indexParents][0] + 1;
			Enfants[indexEnfants][1] = Parents[indexParents][1];
			Enfants[indexEnfants][2] = Parents[indexParents][2];
			indexEnfants++;
		}
		else if ((Parents[indexParents][0] >= (maturite - 1))
				 // && (  Parents[indexParents][0] >=  menopause    )
				 && (Parents[indexParents][0] >= mort))
		{
			Enfants[indexEnfants][0] = 0;
			Enfants[indexEnfants][1] = 0;
			Enfants[indexEnfants][2] = Parents[indexParents][2];
			// indexEnfants++ ; // on pourrait en fait ne rien faire : ne pas augmenter l'index efface d'emblée
		}
	}
	enfantsBecomeParents();
}
