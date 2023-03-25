
// 1 C92
// TEMPLATE DES MODIFICATIONS SIMPLES
/*

			for (byte i = 0 ; i < nbLayer  ; i++ ) {

				if ( isLayerActive(i) ) {

					for ( byte j = 0 ; j < nbNotesByLayer   ; j++ ) {

						pileNote tempNote;
						tempNote = ZEROgetNoteFromPile(pInitial[i], j);

						if ( tempNote.note != 0 ) {
							
							//MODIFICATIONS
							 
								tempNote.note =  newValue  ;

							ZEROSetNoteInPile(pInitial[i], j, tempNote);
							}
							}
							}

CORE : 

						pileNote tempNote;
						tempNote = ZEROgetNoteFromPile(pInitial[i], j);
						ZEROSetNoteInPile(pInitial[i], j, tempNote);

ou 
initializeNote(tempNote) ; 
ZEROaddNewNotetoPile(pInitial[i],tempNote) ; 


*/

/* il faudra faire un tableau pour les transformations
celui-ci reprendra : Numéro à l'écran, Numéro de la procédure dans le programme, Nom, valeur min du potentiomètre, valeur max
de la sorte ce tableau sera celui qui sera modifié, pas toutes les procédures
plus facile aussi si certaines fonctions sont supprimées ou pour les réarranger
*/

// #include <pointers.h>
// # include <harmonize.h>

// #include "SdFat.h"

// PROTOTYPE d'une fc qui se trouve dans fcConditional.h

byte meanPitch_MultipleLayer(byte thoseLayer);
int antiPermuteInt(int *str, int len); // se trouve dans permute.h
int permuteInt(int *str, int len);	   // se trouve dans permute.h
void simplePartialSortZeroNotesv2(int layer);
bool hasSymmetricAxe(int gammeActuelle, int ModeActuel);
int getSymmetricAxe(int gammeActuelle, int ModeActuel);
int getSymmetricNoteOfAThroughAxeBQuartTon(int a, int b);
float giveInclusion(int gammeActuelle = 0, long potVal3 = 0);
String GiveNameOfMode(int NNN);
int transposeAllFromDegreeAtoDegreeB(byte layer, int degreeA, int degreeB, int gamme, int mode);
int transposeOneRandomTriad(byte layer, int degreeA, int degreeB, int gamme, int mode, int triadSize = 3);
int transposeOneRandomSpacedTriad(byte layer, int degreeA, int degreeB, int gamme, int mode, int triadSize = 3);
int unflattenTriad(byte layer, int degreeA, int degreeB, int gamme, int mode);
int getNotePlaceAdansGammeCmodeDoctaveE(int placeA, int gamme, int mode, int octave);
int getPlaceNoteAdansGammeCmodeD(int noteA, int gamme, int mode);
void initializeSequence(byte t);
unsigned int nbStartNotesDifferents(byte i);								   // dans fcConditionnal
unsigned long shortestNonNulStartDistance(byte i);							   // dans fcConditionnal
unsigned int nbNotesAtDistance(byte i, unsigned long distance);				   // dans fc Conditionnal
unsigned int giveNbOfNthNoteAtDistance(byte i, int n, unsigned long distance); // dans fc Conditional
void addF8(void);
int noteADegreesAboveNoteBinGammeCmodeD(int aDegrees, int noteB, int gammeC, int modeD); // dans harmonize.h
void verboseFullPile(int i);															 // dans SubParts
void verbosePrintln(String phrase);

// #include <saveLoadLocal.h>

void Transformation(int TTT, int BBB)
{

	// byte i,j,k ; // il faut à tout prix supprimer ceci et mettre les variables locales en local ! NOTE****
	int potVal2, potVal3;

	// TODO on ne doit plus faire le mapping, la valeur est déjà calculée auparavant !
	potVal2 = BBB;
	potVal3 = BBB;

	// TODO supprimer les endroits où on le fait équivaleur à Counter2EndUser ?
	//
	switch (TTT)
	{

		/////////////////////////////////////////////////////////////////////////////////////
		// PILE VERSION : faite et vérifiée
		/////////////////////////////////////////////////////////////////////////////////////

	case 0:
	{
	}
	break;

	case 1: // Continuous Random Melody
	{
		// il faut déplacer la lecture analogRead, le calcul du potVal2 et 3 vers la partie affichage.   fonction avec 2 valeurs : TTT et PotValX );
		// potVal2 = map(BBB, 0, 1023, Transfo[LineCallingTransfo(TTT)][1] , Transfo[LineCallingTransfo(TTT)][2] );   // on ne peut pas se référer au tableau Transfo car l'ordre peut changer.  il faut que la procédure reçoive min et max en envoi si on veut pouvoir changer ça du tableau !

		// doit-on lire le potVal dans la procédure ou avant ?

		if (potVal2 == 0)
		{
			potVal2++;
		}

		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				for (byte j = 0; j < potVal2; j++)
				{

					pileNote tempNote;
					initializeNote(tempNote);

					tempNote.note = random(60, 84);
					tempNote.velocity = random(1, 127);
					tempNote.layer = i;
					tempNote.channel = layerChannel[i];
					tempNote.type = 0x90; // Note ON
					float dureeDiv = (float)layerLength[i] / (float)potVal2;
					tempNote.startNote = (unsigned long)(((float)dureeDiv) * (float)(j));
					tempNote.endNote = (unsigned long)(((float)dureeDiv) * (float)(j + 1));
					tempNote.killnote = false;
					tempNote.thereIsAnEnd = true;

					pileNote_actualizeTimeToPlay(tempNote);
					ZEROaddNewNotetoPile(pInitial[i], tempNote);

					// ici on a prévu une mélodie bien carrée : division en nb de notes, mélodie continue sans overlap
					// avec switch on pourrait prévoir d'autres choses, random, bref, etc.
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// PILE VERSION : faite et vérifiée /mod 2C21
		/////////////////////////////////////////////////////////////////////////////////////

	case 2: //   HAMMMER
	{
		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				for (byte j = 0; j < potVal2; j++)
				{
					pileNote tempNote;
					initializeNote(tempNote);

					tempNote.note = 60;
					if (random(4) == 1)
					{
						tempNote.note = random(60, 74); // autre note sur le WavePad
					}
					tempNote.velocity = random(50, 100);
					tempNote.layer = i;
					tempNote.channel = layerChannel[i];
					tempNote.type = 0x90;
					float dureeDiv = (float)layerLength[i] / (float)potVal2;
					tempNote.startNote = (unsigned long)(((float)dureeDiv) * (float)(j));
					tempNote.endNote = tempNote.startNote + 10;
					tempNote.killnote = false;
					tempNote.thereIsAnEnd = true;

					pileNote_actualizeTimeToPlay(tempNote);
					ZEROaddNewNotetoPile(pInitial[i], tempNote);

					tempNote.note = 71; // HiHat sur le WAVEPAD
					tempNote.velocity = random(50, 100);
					tempNote.startNote = (unsigned long)((((float)layerLength[i] / (float)potVal2) * (float)(j)) + ((float)layerLength[i] / (float)(2 * potVal2)));
					tempNote.endNote = tempNote.startNote + 10;

					pileNote_actualizeTimeToPlay(tempNote);
					ZEROaddNewNotetoPile(pInitial[i], tempNote);
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// PILE VERSION : faite et vérifiée /mod2C21
		/////////////////////////////////////////////////////////////////////////////////////

	case 3: //   HAMMMER FUNK
	{
		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				for (byte j = 0; j < potVal2; j++)
				{

					pileNote tempNote;
					initializeNote(tempNote);

					tempNote.note = random(60, 74); //  WavePad
					tempNote.velocity = random(50, 100);
					tempNote.layer = i;
					tempNote.channel = layerChannel[i];
					tempNote.type = 0x90;
					// float dureeDiv = (float)layerLength[i] / (float)potVal2;
					tempNote.startNote = (unsigned long)(((float)layerLength[i] / (float)potVal2) * (float)(j) + random(100));
					tempNote.endNote = tempNote.startNote + 10;
					tempNote.killnote = false;
					tempNote.thereIsAnEnd = true;

					pileNote_actualizeTimeToPlay(tempNote);
					ZEROaddNewNotetoPile(pInitial[i], tempNote);

					tempNote.note = 71; // HiHat sur le WAVEPAD
					tempNote.velocity = random(50, 100);
					tempNote.startNote = (unsigned long)((((float)layerLength[i] / (float)potVal2) * (float)(j)) + ((float)layerLength[i] / (2 * (float)potVal2)) + random(100));
					tempNote.endNote = tempNote.startNote + 10;

					pileNote_actualizeTimeToPlay(tempNote);
					ZEROaddNewNotetoPile(pInitial[i], tempNote);
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// PILE VERSION : faite et vérifiée  mod2C21
		/////////////////////////////////////////////////////////////////////////////////////

	case 4: // TRANSPOSE
	{
		// potVal2 = BBB ;    // essayer ainsi et voir si 12 est mieux que 24 ( à priori c'est suffisant )

		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{

					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					if (tempNote.note != 0 && tempNote.type == 0x90) // 2C21
					{
						tempNote.note = modifyPitchUniversal(tempNote, tempNote.note, potVal2);
						// va modifier le pitch sauf si note appartenant à layer ou channel unpitchable
						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// PILE VERSION : faite et vérifiée
		/////////////////////////////////////////////////////////////////////////////////////

	case 5: // DELETE
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				ZEROdeleteWholePile(pInitial[i]);
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// PILE VERSION : faite et vérifiée //mod2C21
		/////////////////////////////////////////////////////////////////////////////////////

	case 6: // create C60, sur la première note d'une layer.
	{

		switch (potVal2)
		{
		case 1:
			potVal3 = 1;
			break;
		case 2:
			potVal3 = 10;
			break;
		case 3:
			potVal3 = 100;
			break;
		case 4:
			potVal3 = 1000;
			break;
		case 5:
			potVal3 = 10000; // soit 10 secondes
			break;
		}

		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				pileNote tempNote;
				initializeNote(tempNote);

				tempNote.note = valueC60; // C60
				tempNote.velocity = random(50, 100);
				tempNote.layer = i;
				tempNote.channel = layerChannel[i];
				tempNote.type = 0x90;

				// float dureeDiv = (float)layerLength[i] / (float)potVal2;
				tempNote.startNote = nowMillis % layerLength[i];
				tempNote.endNote = tempNote.startNote + potVal3;
				tempNote.killnote = false;
				tempNote.thereIsAnEnd = true;

				pileNote_actualizeTimeToPlay(tempNote);
				ZEROaddNewNotetoPile(pInitial[i], tempNote);

				PlaynoteOn_(0x90 + tempNote.channel, tempNote.note, tempNote.velocity);
				addNewNotetoPileONCE(pointeurInitial, tempNote.channel, tempNote.note,
									 tempNote.velocity, nowMillis + potVal3, 0x80, i);

				// fixStartEndNote(i,ll) ;
			}
		}
	}
	break;

		// le 7 est plus loin, pour être avec les autres valeurs de COPY

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		/////////////////////////////////////////////////////////////////////////////////////

	case 8: // EQUALLENGTH
	{

		// on choisit de quelle layer on veut copier les infos, vers les layers actives

		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				layerLength[i] = layerLength[potVal2 - 1];
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// PILE VERSION : faite et vérifiée
		// Grosse modification 2C21
		/////////////////////////////////////////////////////////////////////////////////////

	case 9: // DEPLACE
	{
		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{

					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					/*
					signed long temporaryStartNote;
					temporaryStartNote = tempNote.startNote + potVal2;

					if (temporaryStartNote > (signed int)layerLength[i])
					{
						do
						{
							temporaryStartNote = temporaryStartNote - layerLength[i];
						} while (temporaryStartNote > (signed int)layerLength[i]);
					}

					if (temporaryStartNote < 0)
					{
						do
						{
							temporaryStartNote = layerLength[i] + temporaryStartNote;
						} while (temporaryStartNote < 0);
					}

					tempNote.startNote = temporaryStartNote;

					signed long temporaryEndNote;
					temporaryEndNote = tempNote.endNote + potVal2;

					if (temporaryEndNote > (signed int)layerLength[i])
					{
						do
						{
							temporaryEndNote = temporaryEndNote - layerLength[i];
						} while (temporaryEndNote > (signed int)layerLength[i]);
					}

					if (temporaryEndNote < 0)
					{
						do
						{
							temporaryEndNote = layerLength[i] + temporaryEndNote;
						} while (temporaryEndNote < 0);
					}

					tempNote.endNote = temporaryEndNote;
*/

					if (tempNote.type == 0x90)
					{
						pileNote_fixStartEndNote(tempNote, potVal2);
					}

					ZEROSetNoteInPile(pInitial[i], j, tempNote);
				}
			}
		}
	}

	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// PILE VERSION : faite et vérifiée
		// mod 2C21 : choix de garder decimate quel que soit le type 0x90 ou CC
		/////////////////////////////////////////////////////////////////////////////////////

	case 10: // DECIMATE
	{
		// on va décimer entre 1  etv100%

		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				bool goON(false);

				// on a ici exemple de la strucutre ultra-rapide !

				pileNote *pilePtr;
				pilePtr = pInitial[i];

				if ((*pilePtr).nextPile != 0)
				{
					pilePtr = (*pilePtr).nextPile;

					if (pilePtr == 0)
					{
					}
					else
					{
						do
						{
							if (random(0, 100) < potVal2)
							{
								(*pilePtr).note = 0;
								(*pilePtr).velocity = 0;
								(*pilePtr).killnote = true;
							}

							if ((*pilePtr).nextPile != 0)
							{
								pilePtr = (*pilePtr).nextPile;
								goON = true;
							}
							else
							{
								goON = false;
							}
						}

						while (goON);
					}
				}
			}

			ZEROtreatPileComplete(pInitial[i]); // on en profite pour un peu faire le ménage, suppression des notes 0 et des killnote
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL : modif de constantes en unsigned long
		/////////////////////////////////////////////////////////////////////////////////////

	case 11: // MULTIPLIER de durée layer = MULTIPLY
	{
		unsigned long calcultime; // doit parfois être négatif !!!!

		// potVal3  = map(BBB, 0, 1023, 1 , 8);
		// potVal2  = map(BBB, (potVal3 - 1) * 1023 / 7, (potVal3) * 1023 / 7, 1, 8);

		potVal3 = (BBB / 7) + 1;
		potVal2 = (BBB % 7) + 1;

		// quotient =    potVal2 / potVal3 ;

		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{
				calcultime = (unsigned long)layerLength[i] * (unsigned long)potVal2;
				calcultime = calcultime / (unsigned long)potVal3;
				layerLength[i] = (unsigned long)calcultime;
				// TODO il faudrait peut être s'assure que ça ne déborde pas : ex 1000000000 -> 31000 msec ?
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// VERSION PILE
		// 1C93 ai trouvé un bug terrible : calcul d'endnote à partir de startnote !!!
		// 2C21 unsigned long pour start et endnote
		/////////////////////////////////////////////////////////////////////////////////////

	case 12: // MULTIPLIER de mélodie = MELTIPLY
	{
		unsigned long calcultime; // doit parfois être négatif !!!! 1C93 DEBUG ???

		potVal3 = (BBB / 7) + 1;
		potVal2 = (BBB % 7) + 1;

		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				calcultime = (unsigned long)layerLength[i] * (unsigned long)potVal2;
				calcultime = calcultime / (unsigned long)potVal3;
				layerLength[i] = (unsigned long)calcultime;

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{
					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					tempNote.startNote = (unsigned long)((float)tempNote.startNote * ((float)potVal2 / (float)potVal3));
					tempNote.endNote = (unsigned long)((float)tempNote.endNote * ((float)potVal2 / (float)potVal3));
					pileNote_fixStartEndNote(tempNote, 0); // 2C21
					ZEROSetNoteInPile(pInitial[i], j, tempNote);
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// VERSION PILE
		// 1C93
		/////////////////////////////////////////////////////////////////////////////////////

	case 13: // CHANNEL Changer
	{

		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				layerChannel[i] = potVal2 - 1; // moins 1 car on encode en valeur MACHINE, pas ENDUSER du channel

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{
					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					tempNote.channel = layerChannel[i];

					ZEROSetNoteInPile(pInitial[i], j, tempNote);
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// VERSION PILE
		// 1C93  : déplacement plus loin car perturbe le jeu ( affichge intensif )
		/////////////////////////////////////////////////////////////////////////////////////

	case 14: // TIME+Channel Info    la transfo ne fait rien du tout, c'est juste l'affichage qui est intéressant.
	{
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// VERSION PILE
		// 1C93  :  gros débuggage, pour cette fonction qui utilise des Transfo imbriquées
		// devrait être plus essayé, peut être ?
		/////////////////////////////////////////////////////////////////////////////////////

	case 15:
		// MACRO POLYRYTHM  = combinaison qui d'emblée créée un beau polyrythme à partir d'un channel
		//     // inactivée pour l'instant
		//
		//
		//     potVal2 = map(BBB, 0, 1023, 1 , 5);   // on lit le n° du channel
		//
		//     // Transformation(7, BBB);
		//     // Transformation(8, BBB); // y aura-t-il problème avec les i et j ?
		//
		//
		//     for (byte i15 = 0 ; i < 4 ; i++ ) {
		//
		//     byte k15 =  ( ( activeLayer >> i15 ) & B00000001 );    // le bit de droite est 1 si layer 0 est ON ; le 2ème bit est 1 si layer 1 est ON, etc...
		//     if ( k15 == B00000001 ) {
		//
		//
		//     Transformation(11, random(0, 1023)); // il faut passer les valeurs dans la fonction, pas en lecture !!! ne marchera pas pour l'instant
		//     layerLength[i15] = layerLength[i15] + random(10); // chouilla qui rend irrégulier
		//
		//   }
		// }
		//
		/*
		combinaison des transformations 7, 8 et 11
		7 copie un layer dans un autre
		8 rend la taille égale
		11 modifie la taille de ce layer

		*/
		{
			for (byte i = 0; i < nbLayer; i++)
			{

				if (isLayerActive(i) && (i != BBB - 1))
				{
					byte rememberLayer = activeLayer;
					activeLayer = (B0001 << i);
					// Transformation(5,0) ; // efface les layers actifs
					Transformation(8, BBB); // rend la taille des layers actifs idem à layer sélectionné
					activeLayer = rememberLayer;
				}
			}

			for (byte i = 0; i < nbLayer; i++)
			{
				if (isLayerActive(i))
				{
					byte rememberLayer = activeLayer;
					activeLayer = (B0001 << i);
					if ((i != (BBB - 1)))
					{
						Transformation(7, BBB); // copie dans layers actif le contenu du layer sélectionné
					}
					Transformation(11, random(2, 49)); // choisit au hasard une modification de la durée des layers sélectionnés ( idem )

					activeLayer = rememberLayer;
				}
				// j'ai ci-dessus utilisé un truc pour que la transfo s'applique sur une layer à la fois
			}
		}
		break;

		/////////////////////////////////////////////////////////////////////////////////////
		// VERSION PILE
		// 1C93  modif vers Pile
		// 2C21
		/////////////////////////////////////////////////////////////////////////////////////

	case 16: // POPULATE
	{

		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{

					pileNote tempNote;

					if (tempNote.note != 0 && tempNote.type == 0x90)
					{

						if (random(0, 100) < potVal2)
						{

							byte l = random(0, taille + 1); // l est la note qui va être créée
							tempNote = ZEROgetNoteFromPile(pInitial[i], l);
							// on garde la note qu'on vient de reprendre, on la modifie et on l'ajoute à la pile  !
							tempNote.velocity = random(50, 110);
							long duree = getDuree(tempNote);
							tempNote.startNote = random(0, layerLength[i]);
							tempNote.endNote = tempNote.startNote + duree;
							tempNote.type = 0x90;
							pileNote_fixStartEndNote(tempNote, 0);

							ZEROaddNewNotetoPile(pInitial[i], tempNote);
							// et pas un setNote, ici on rajoute la note à la pile !
						}
					}
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// VERSION PILE
		// échange les notes de position : modif de startNote et endNote réciproques
		// 1C93  modif vers Pile : j'ai gardé le changement de start et end bien que je pense
		// que le changement de note (pitch) eut été suffisant.
		/////////////////////////////////////////////////////////////////////////////////////

	case 17: // FLIP LOC
	{
		//		unsigned int tempotime, tempotime2, tempotime3;
		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{

					if (random(0, 100) < potVal2)
					{

						byte l = random(0, taille + 1); // l est la note qui va être flippée avec j

						pileNote tempNote;
						tempNote = ZEROgetNoteFromPile(pInitial[i], j);

						pileNote tempNote2;
						tempNote2 = ZEROgetNoteFromPile(pInitial[i], l);

						unsigned long start = tempNote.startNote;
						unsigned long end = tempNote.endNote;

						tempNote.startNote = tempNote2.startNote;
						tempNote.endNote = tempNote2.endNote;

						tempNote2.startNote = start;
						tempNote2.endNote = end;

						pileNote_fixStartEndNote(tempNote, 0);
						pileNote_fixStartEndNote(tempNote2, 0);

						ZEROSetNoteInPile(pInitial[i], j, tempNote);
						ZEROSetNoteInPile(pInitial[i], l, tempNote2);
					}
				}
			}
		}
	}
	break;

	case 18: // GOLDEN RATIO
	{

		// on doit commencer par les classer je crois
		Transformation(73, 0); // trie les notes par débuts

		long calcultime; // doit parfois être négatif !!!!

		unsigned int tempotime, tempotime2, tempotime3;

		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (byte j = 2; j < taille; j++)
				{
					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);
					pileNote tempNote2;
					tempNote2 = ZEROgetNoteFromPile(pInitial[i], j - 1);
					pileNote tempNote3;
					tempNote3 = ZEROgetNoteFromPile(pInitial[i], j - 2);

					if (random(0, 100) < potVal2)
					{

						tempotime = tempNote2.startNote - tempNote3.startNote;
						// on suppose ici que notes sont chronologiques, pas toujours vrai...
						// sauf depuis que la transformation 73 a été rajoutée en début de fonction

						tempotime2 = tempNote.startNote - tempNote2.startNote;
						// donne espace entre début de note j-1 et début j

						tempotime3 = getDuree(tempNote);
						// donne durée de note j
						// tempotime3 = melodyMatrixTime[i][j][MMTstartnote] - melodyMatrixTime[i][j][MMTendnote] ;
						/*
							const long phiNum   = 75025 ;
							const int phiDen   = 43368 ; // phi est approximé par phiNum/phiDen
							*/

						if (tempotime < tempotime2)
						{
							calcultime = (long)((float)tempotime * (float)phiNum / (float)phiDen);
						}
						else
						{
							calcultime = (long)((float)tempotime * (float)phiDen / (float)phiNum);
						}

						tempNote.startNote = tempNote2.startNote + (unsigned int)calcultime;
						tempNote.endNote = tempNote.startNote + (int)tempotime3;
						pileNote_fixStartEndNote(tempNote, 0);
						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// PILE VERSION : faite et vérifiée
		/////////////////////////////////////////////////////////////////////////////////////

	case 19: // SKETCH
		// fait doublon par rapport à 6+338
		// à terme évaluer ce qui est préférable, vite changer de note ou vite changer de durée.
		{
			// le potard indique qu'elle note on va tapper
			for (byte i = 0; i < nbLayer; i++)
			{

				if (isLayerActive(i))
				{

					pileNote tempNote;
					initializeNote(tempNote);

					tempNote.note = potVal2;
					tempNote.velocity = random(60, 110);
					tempNote.layer = i;
					tempNote.channel = layerChannel[i];

					// float dureeDiv = (float)layerLength[i] / (float)potVal2;
					tempNote.startNote = nowMillis % layerLength[i];
					tempNote.endNote = tempNote.startNote + 20; // create C60 permet de faire mieux en modifiant la durée
					tempNote.killnote = false;
					tempNote.thereIsAnEnd = true;

					pileNote_actualizeTimeToPlay(tempNote);
					ZEROaddNewNotetoPile(pInitial[i], tempNote);

					PlaynoteOn_(0x90 + tempNote.channel, tempNote.note, tempNote.velocity);

					addNewNotetoPileONCE(pointeurInitial, tempNote.channel, tempNote.note,
										 tempNote.velocity, nowMillis + 20, 0x80, i);

					// fixStartEndNote(i,ll) ;
				}
			}
		}
		break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Universel
		/////////////////////////////////////////////////////////////////////////////////////

	case 20: // (UN)MUTE
	{
		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				// si unmute devient mute
				// si mute devient unmute

				muteLayer = muteLayer ^ (1 << i); // si le bit est 0 deviendra 1 et s'il est 1 deviendra 0
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// PILE VERSION : faite et vérifiée
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 21: // REVERSE    random : inverse une note au hasard
	{
		unsigned long tempotime;

		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{
					// if  ( melodyMatrix[i][j][MMnote] != 0 ) {
					if (random(0, 100) < potVal2)
					{
						pileNote tempNote;
						tempNote = ZEROgetNoteFromPile(pInitial[i], j);

						tempotime = tempNote.startNote;
						tempNote.startNote = layerLength[i] - tempNote.endNote;
						tempNote.endNote = layerLength[i] - tempotime;

						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// PILE VERSION : faite et vérifiée
		// 1C95
		/////////////////////////////////////////////////////////////////////////////////////

	case 22: // PFUNK
	{
		long calcultime; // doit parfois être négatif !!!!

		unsigned long tempotime;

		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{
					// if  ( melodyMatrix[i][j][MMnote] != 0 ) {
					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					tempotime = getDuree(tempNote);

					calcultime = (long)((float)tempotime * (float)potVal2) / (float)100L;
					// on a un pourcentage de la durée des notes

					calcultime = random(0, (int)calcultime);
					if (random(2 == 0))
					{
						calcultime = -calcultime;
					}

					tempNote.startNote = tempNote.startNote + calcultime;
					tempNote.endNote = tempNote.endNote + calcultime;

					pileNote_fixStartEndNote(tempNote);
					ZEROSetNoteInPile(pInitial[i], j, tempNote);
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// PILE VERSION : faite et vérifiée
		/////////////////////////////////////////////////////////////////////////////////////

	case 23: // AUTOFX
	{
		// le potard indique combien de secondes avant auto-modifcation

		automodification = !automodification;
		automodificationtime = potVal2 * 1000L;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// PILE VERSION : faite et vérifiée
		/////////////////////////////////////////////////////////////////////////////////////

	case 24: // RECORD
	{

		// potVal2 = map(BBB, 0, 1023, 1 , 600);   // plus tard choisir entre un mode écrasant qd > 32 notes, delete d'emblée tout ou bien s'arrête à 32 notes

		record = !record;
		while (Serial3.available()) // BUG trouvé à la version 1C99 ! était resté Serial.available
		{
			Serial3.read(); // on vide le buffer
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// PILE VERSION : faite et vérifiée
		/////////////////////////////////////////////////////////////////////////////////////

	case 25: // MIDI-OX
	{

		// potVal2 = map(BBB, 0, 1023, 1 , 600);   // plus tard choisir entre un mode écrasant qd > 32 notes, delete d'emblée tout ou bien s'arrête à 32 notes
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// PILE VERSION : faite et vérifiée
		/////////////////////////////////////////////////////////////////////////////////////

	case 26: // SPEED
	{
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// PILE VERSION : faite et vérifiée
		/////////////////////////////////////////////////////////////////////////////////////

	case 7: // COPY FROM     = COPY+CH
	{

		// potVal3 = BBB ;    //
		// on choisit de quelle layer on veut copier les infos, vers les layers actives

		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i) && (i != (potVal3 - 1)))
			{

				ZEROdeleteWholePile(pInitial[i]);
				// pour éviter d'ajouter une note idem à sa propre layer !

				unsigned int taille = mesureTaillePileComplete(pInitial[potVal3 - 1]);

				for (unsigned int j = 0; j < taille; j++)
				{

					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[potVal3 - 1], j);

					tempNote.layer = i;

					pileNote_fixStartEndNote(tempNote, 0);
					ZEROaddNewNotetoPile(pInitial[i], tempNote);
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// PILE VERSION : faite et vérifiée
		/////////////////////////////////////////////////////////////////////////////////////

	case 27: // COPY FROM + CH FORCE     = COPY\CH
	{

		// potVal3 = BBB ;    //
		// on choisit de quelle layer on veut copier les infos, vers les layers actives

		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i) && (i != (potVal3 - 1)))
			{
				// pour éviter d'ajouter une note idem à sa propre layer !
				ZEROdeleteWholePile(pInitial[i]);
				unsigned int taille = mesureTaillePileComplete(pInitial[potVal3 - 1]);

				for (unsigned int j = 0; j < taille; j++)
				{

					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[potVal3 - 1], j);

					tempNote.layer = (layerChannel[i]);
					tempNote.channel = (layerChannel[i]);
					pileNote_fixStartEndNote(tempNote, 0);
					ZEROaddNewNotetoPile(pInitial[i], tempNote);
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// PILE VERSION : faite et vérifiée
		/////////////////////////////////////////////////////////////////////////////////////

	case 28: // ADD FROM
	{

		// potVal3 = BBB ;    //
		// on choisit de quelle layer on veut copier les infos, vers les layers actives

		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i) && (i != (potVal3 - 1)))
			{

				// ZEROdeleteWholePile(pInitial[i]);
				// pour éviter d'ajouter une note idem à sa propre layer !

				unsigned int taille = mesureTaillePileComplete(pInitial[potVal3 - 1]);

				for (unsigned int j = 0; j < taille; j++)
				{

					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[potVal3 - 1], j);
					// tempNote.data2 = 0;
					tempNote.layer = i;
					tempNote.channel = i;
					pileNote_fixStartEndNote(tempNote, 0);
					ZEROaddNewNotetoPile(pInitial[i], tempNote);
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// PILE VERSION : faite et vérifiée
		/////////////////////////////////////////////////////////////////////////////////////

	case 29: // ADD FROM    \ CH
	{

		// potVal3 = BBB ;    //
		// on choisit de quelle layer on veut copier les infos, vers les layers actives

		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i) && (i != (potVal3 - 1)))
			{
				// pour éviter d'ajouter une note idem à sa propre layer !
				ZEROdeleteWholePile(pInitial[i]);
				unsigned int taille = mesureTaillePileComplete(pInitial[potVal3 - 1]);

				for (unsigned int j = 0; j < taille; j++)
				{

					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[potVal3 - 1], j);
					// tempNote.data2 = 0;
					tempNote.layer = (layerChannel[i]);
					tempNote.channel = (layerChannel[i]);
					pileNote_fixStartEndNote(tempNote, 0);
					ZEROaddNewNotetoPile(pInitial[i], tempNote);
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 30: // EYE CANDY
	{
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 31: // ACTIVE LAYER CHOOSER    via le menu, de sorte qu'il puisse se produire quand on passe en mode automatique
	{
		activeLayer = potVal3;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// PILE VERSION : faite et vérifiée
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 32: // FIRSTFREENOTE display
	{
	}
	break;

	case 33: // SCHIZOPHRENIA = une transfo assez destructrice, car méchammant random..
	{

		// par rapport à l'appel de cette procédure linecallingtransfo, on pourrait :
		// 1. mettre une première ligne qui l'appelle 1x, avec une valeur, et réutliser cette valeur ( évite 1 second appel )
		// 2. encapsuler la définition de PotVal3 en encapsulant le map ( ) dans une fonction appellée.

		// potVal3 = BBB ;     // on choisit de quelle layer on veut copier les infos, vers les layers actives

		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{ // va parcourir la mélodie pointée ( Layer 1 à 4 : potVal3 )

					if (random(0, 100) < potVal3)
					{
						pileNote tempNote;
						tempNote = ZEROgetNoteFromPile(pInitial[i], j);

						pileNote tempNote2;

						byte i2 = random(0, 4);
						unsigned int taille2 = mesureTaillePileComplete(pInitial[i2]);
						unsigned j2 = random(0, taille2);
						// choisit donc une autre note au hasard parmi tous les layers
						tempNote2 = ZEROgetNoteFromPile(pInitial[i2], j2);
						tempNote.note = tempNote2.note;

						i2 = random(0, 4);
						j2 = random(0, mesureTaillePileComplete(pInitial[i2]));
						tempNote2 = ZEROgetNoteFromPile(pInitial[i2], j2);
						tempNote.velocity = tempNote2.velocity;

						i2 = random(0, 4);
						j2 = random(0, mesureTaillePileComplete(pInitial[i2]));
						tempNote2 = ZEROgetNoteFromPile(pInitial[i2], j2);
						tempNote.startNote = tempNote2.startNote;

						i2 = random(0, 4);
						j2 = random(0, mesureTaillePileComplete(pInitial[i2]));
						tempNote2 = ZEROgetNoteFromPile(pInitial[i2], j2);
						tempNote.endNote = tempNote2.endNote;

						i2 = random(0, 4);
						j2 = random(0, mesureTaillePileComplete(pInitial[i2]));
						tempNote2 = ZEROgetNoteFromPile(pInitial[i2], j2);
						tempNote.channel = tempNote2.channel;

						i2 = random(0, 4);
						j2 = random(0, mesureTaillePileComplete(pInitial[i2]));
						tempNote2 = ZEROgetNoteFromPile(pInitial[i2], j2);
						tempNote.layer = tempNote2.layer;

						pileNote_fixStartEndNote(tempNote);
						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
				}
			}
		}
	}
	break;

	case 34: // SCHIZOPHRENIA2 = une transfo très destructrice
	{
		potVal3 = BBB; // on choisit de quelle layer on veut copier les infos, vers les layers actives

		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{ // va parcourir la mélodie pointée ( Layer 1 à 4 : potVal3 )

					if (random(0, 100) < potVal3)
					{

						// long a , b ;
						while (random(10) != 0)
						{
							{
								pileNote tempNote;
								tempNote = ZEROgetNoteFromPile(pInitial[i], j);

								pileNote tempNote2;

								byte i2 = random(0, 4);

								unsigned int taille2 = mesureTaillePileComplete(pInitial[i2]);
								unsigned j2 = random(0, taille2);
								// choisit donc une autre note au hasard parmi tous les layers
								tempNote2 = ZEROgetNoteFromPile(pInitial[i2], j2);
								tempNote.note = tempNote2.note;

								i2 = random(0, 4);
								j2 = random(0, mesureTaillePileComplete(pInitial[i2]));
								tempNote2 = ZEROgetNoteFromPile(pInitial[i2], j2);
								tempNote.velocity = tempNote2.velocity;

								i2 = random(0, 4);
								j2 = random(0, mesureTaillePileComplete(pInitial[i2]));
								tempNote2 = ZEROgetNoteFromPile(pInitial[i2], j2);
								tempNote.startNote = tempNote2.startNote;

								i2 = random(0, 4);
								j2 = random(0, mesureTaillePileComplete(pInitial[i2]));
								tempNote2 = ZEROgetNoteFromPile(pInitial[i2], j2);
								tempNote.endNote = tempNote2.endNote;

								i2 = random(0, 4);
								j2 = random(0, mesureTaillePileComplete(pInitial[i2]));
								tempNote2 = ZEROgetNoteFromPile(pInitial[i2], j2);
								tempNote.channel = tempNote2.channel;

								i2 = random(0, 4);
								j2 = random(0, mesureTaillePileComplete(pInitial[i2]));
								tempNote2 = ZEROgetNoteFromPile(pInitial[i2], j2);
								tempNote.layer = tempNote2.layer;

								pileNote_fixStartEndNote(tempNote);
								ZEROSetNoteInPile(pInitial[i], j, tempNote);
							}
						}
					}
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// PILE VERSION : faite et vérifiée
		// 1C94 : belle erreur de conception dans le calcul de Start et EndNote  ! clarity of mind ...
		/////////////////////////////////////////////////////////////////////////////////////

	case 35: // MIRROR
	{
		// potVal3 = BBB ;     // potval devient un pourcentage qui dit à quel endroit du layer on doit répliquer les notes.
		unsigned long foldtime;
		long calcultime; // doit parfois être négatif !!!!

		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				calcultime = (((long)layerLength[i] * (long)potVal2) / (long)100);
				foldtime = (signed int)(calcultime); // on doit pouvoir se passer de la variable foldtime en fait !

				// en faisant cela il faut vérifier toutes les layers où il y a des multiplications : n'écrase-t-on pas la valeur avec un contenant incapable de contenir une valeur aussi grande ?
				// REVOIR TOUTES LES TRANSFO avec POURCENTAGES !

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{ // va parcourir la mélodie pointée ( Layer 1 à 4 : potVal3 )

					// d'abord on fait le ménage : on supprime les notes commençant après foldtime
					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					if (tempNote.note > foldtime)
					{
						tempNote.note = 0;
						tempNote.velocity = 0;
						tempNote.killnote = true;

						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
				}

				ZEROtreatPileComplete(pInitial[i]);

				// ensuite les notes qui commencent avant et finissent après ont juste modif de endtime

				taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)

				{ //
					// 					unsigned int taille = mesureTaillePileComplete(pInitial[i]);

					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					if ((tempNote.note != 0) && (tempNote.startNote < foldtime) && (tempNote.endNote > foldtime))
					{

						tempNote.endNote = foldtime + (foldtime - tempNote.startNote);
						// melodyMatrixTime[i][j][MMTendnote] = constrain ( melodyMatrixTime[i][j][MMTendnote], 0, layerLength[i] )  ;
						pileNote_fixStartEndNote(tempNote, 0);
						ZEROSetNoteInPile(pInitial[i], j, tempNote);

						// 						fixStartEndNote(i, j);
					}
					else
						// enfin les notes qui commencent avant et finissent avant -> nouvelle note.

						if ((tempNote.note != 0) && (tempNote.startNote < foldtime) && (tempNote.endNote < foldtime))
					{
						pileNote tempNote2;
						putNoteBinNoteA(tempNote2, tempNote);

						// // byte j_35 = firstfreenote(i);
						// melodyMatrix[i][j_35][MMnote] = melodyMatrix[i][j][MMnote];
						// melodyMatrix[i][j_35][MMvelocite] = melodyMatrix[i][j][MMvelocite];
						// melodyMatrix[i][j_35][MMdata] = melodyMatrix[i][j][MMdata];
						// melodyMatrix[i][j_35][MMdata2] = melodyMatrix[i][j_35][MMdata2] ^ B00000011; // à voir, peut être les faire 011 au début ??

						calcultime = (long)tempNote.endNote;
						tempNote2.endNote = (unsigned long)((long)foldtime + ((long)foldtime - (long)tempNote.startNote));
						// melodyMatrixTime[i][j_35][MMTendnote] = constrain ( melodyMatrixTime[i][j_35][MMTendnote], 0, layerLength[i] )  ;

						tempNote2.startNote = (unsigned long)((long)foldtime + ((long)foldtime - (long)tempNote.endNote));
						// melodyMatrixTime[i][j_35][MMTstartnote] = constrain ( melodyMatrixTime[i][j_35][MMTstartnote], 0, layerLength[i] )  ;
						pileNote_fixStartEndNote(tempNote2, 0);
						ZEROaddNewNotetoPile(pInitial[i], tempNote2);
					}
				}
			}
		}
	}

	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// PILE VERSION : faite et vérifiée
		// 1C94 adapté pour les pitch trop bas et trop hauts
		/////////////////////////////////////////////////////////////////////////////////////

	case 36: // REFLET
	{
		// potVal3 = BBB ;     // on choisit la note qui sert de ligne d'eau ! ex C60

		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{ // va parcourir la mélodie pointée ( Layer 1 à 4 : potVal3 )
					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					int newPitch;
					newPitch = (int)potVal3 + ((int)potVal3 - (int)tempNote.note);
					// if ( newPitch    > 126 ) { newPitch = 126 ; }
					// if ( newPitch   < 1 ) { newPitch = 1 ; }
					// newPitch = limiter(newPitch, 1, 127);
					// newPitch = limiterForPitch(newPitch);
					// tempNote.note = (byte)newPitch;

					tempNote.note = modifyPitchUniversal(tempNote, newPitch, 0);
					ZEROSetNoteInPile(pInitial[i], j, tempNote);

					// 					melodyMatrix[i][j][MMnote] = (byte)(newPitch);
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// PILE VERSION : faite et vérifiée
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 37: // SHRINK EXPAND de PRécision           // à voir s'il faut en faire un qui modifie les notes aussi
	{
		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				// calcultime = (long) layerLength[i] * (long) potVal3 ;
				// calcultime = (long) ( (float) calcultime / (float) 100 ) ;
				// layerLength[i] = layerLength[i] + (int) calcultime ;

				layerLength[i] = layerLength[i] + (long)(((float)layerLength[i] * (float)potVal3) / (float)100);
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// PILE VERSION : faite et vérifiée
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 38: // REVERSE
	{
		// potVal3 = map(BBB, 0, 1023, Transfo[LineCallingTransfo(38)][1] , Transfo[LineCallingTransfo(38)][2]);   // je ne sais pas ce qui pourrait moduler la transformation
		unsigned long calcultime; // doit parfois être négatif !!!! ??? 1C94

		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{ // va parcourir la mélodie pointée ( Layer 1 à 4 : potVal3 )
					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);
					pileNote_fixStartEndNote(tempNote, 0);

					calcultime = tempNote.endNote;
					tempNote.endNote = (unsigned long)layerLength[i] - (unsigned long)tempNote.startNote;
					tempNote.startNote = (unsigned long)layerLength[i] - (unsigned long)calcultime;

					ZEROSetNoteInPile(pInitial[i], j, tempNote);
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// PILE VERSION : faite et vérifiée
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 39: // VELOCITY FIX
	{
		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{ // va parcourir la mélodie pointée ( Layer 1 à 4 : potVal3 )

					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					if (potVal3 < 127)
					{
						tempNote.velocity = potVal3;
					}
					else
					{
						tempNote.velocity = random(1, 127);
					}

					ZEROSetNoteInPile(pInitial[i], j, tempNote);
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// PILE VERSION : faite et vérifiée
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 40: // Define MOde Actuelle
	{
		potVal3 = BBB; // 0 à 5

		/* 0 ScaleMajor =  2741L;          // B101010110101  ;  //   ton, ton, demi-ton, ton, ton, ton, demi-ton
			1 ScaleMinorNatural =  1453L;   // 010110101101  ; // ton, demi-ton, ton, ton, demi-ton, ton, ton
			2 ScaleMinorHarmonic = 2477L;   //  100110101101  ; // ton, demi-ton, ton, ton, demi-ton, 1 ton et demi, demi-ton
			3 ScaleMinorMelodic =  2733L;   // 101010101101  ; // ton, demi-ton, ton, ton, ton, ton, demi-ton
			4 ScalePentatonicMajor =  661L;      // 001010010101  ; // ton, ton, ton et demi, ton, ton et demi
			5 ScalePentatonicMinor =  1193L;
			*/

		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				modeActuelle[i] = potVal3; // on doit le retenir pour AutoHarmonize
				newHarmonize(i, gammeActuelle[i], modeActuelle[i]);
			}
		}

		/*
			for (byte i_40 = 0 ; i_40 < nbLayer  ; i_40 ++ ) {

			if ( isLayerActive(i_40) ) {

			for ( byte j_40 = 0 ; j_40 < nbNotesByLayer  ; j_40 ++ ) {     // va parcourir la mélodie pointée ( Layer 1 à 4 : potVal3 )

			// d'abord il faut réduire la melody(note) à une note

			byte rootnote =   melodyMatrix[i_40][j_40][MMnote] % 12 ; // k devient la note entre 0 (C) et 11 (B)

			// il faut ensuite vérifier si cette note est comprise dans la gamme indiquée par PotVal pour une note de base ( C par défaut mais un programme pourra changer )

			// if ( rootnote > gammeActuelle ) {                 // y a-t-il ABS en arduino ?  ////(byte) abs ( (int) rootnote - (int) gammeActuelle )
			//diffnote = rootnote - gammeActuelle ; }
			//else {
			/// diffnote = gammeActuelle - rootnote ; }


			byte diffnote = (byte) abs ( (int) ( (int) rootnote - (int) gammeActuelle )) ;

			while  ( ( ( Scale[potVal3] >> diffnote ) & B0001 ) == 0  ) {

			if ( random(2) == 0 ) {
			melodyMatrix[i_40][j_40][MMnote] = melodyMatrix[i_40][j_40][MMnote] + 1 ; // va augmenter ou réduire le pitch jusqu'à ce que colle à la gamme.
		} else {
		melodyMatrix[i_40][j_40][MMnote] = melodyMatrix[i_40][j_40][MMnote] - 1; // va augmenter ou réduire le pitch jusqu'à ce que colle à la gamme.
	}

	rootnote =   melodyMatrix[i_40][j_40][MMnote] % 12 ; // rootnote devient la note entre 0 (C) et 11 (B)
	diffnote = (byte) abs (( (int) rootnote - (int) gammeActuelle )) ;
}
}
}
}
*/
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 41: // ROOT NOTE Chooser
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				gammeActuelle[i] = potVal3;
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 42: // RECORD MODE
	{
		recordMode = potVal3;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 43: // PING PONG ( delay )
	{

		// potVal3 = BBB ;     // 0 à 127 puis de 127 à 160 = RANDOM

		// il faudra passer toutes les fc en revues : celles appelées en sous-routine par d'autres doivent avoir
		// leurs propres variables ( y compris potVal3, i, j, k )

		for (byte i = 0; i < nbLayer; i++)
		{

			ZEROtreatPileComplete(pInitial[i]);

			if (isLayerActive(i))
			{

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				// clean-up, on met le bit 2 à 0

				// for ( unsigned int j = 0 ;  j < taille  ; j++)
				// { // va parcourir la mélodie pointée ( Layer 1 à 4 : potVal3 )
				// 	melodyMatrix[i][j][MMdata2] = (melodyMatrix[i][j][MMdata2] & B11111011);
				// }

				// vaut-il mieux faire 1 ligne avec les 2 ?
				// à tester dans un sketch où on augmente les valeurs

				// taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)

				{ // va parcourir la mélodie pointée ( Layer 1 à 4 : potVal3 )
					// if ((melodyMatrix[i][j][MMdata2] >> 2 & 0001) != 1)
					{
						pileNote tempNote;
						tempNote = ZEROgetNoteFromPile(pInitial[i], j);

						// byte t43 = firstfreenote(i);
						pileNote tempNote2;

						putNoteBinNoteA(tempNote2, tempNote);

						// melodyMatrix[i][t43][MMnote] = melodyMatrix[i][j][MMnote];
						// melodyMatrix[i][t43][MMvelocite] = melodyMatrix[i][j][MMvelocite];
						// on pourrait imaginer un delay avec une velocité réduite, mais faudrait 2 potards

						// melodyMatrix[i][t43][MMdata] = melodyMatrix[i][j][MMdata];

						// melodyMatrix[i][t43][MMdata2] = (melodyMatrix[i][j][MMdata2] ^ B00000111);
						// bit 0 et 1 sont 1 = played and silented pour que la note ne soit pas rejouée avant le tour suivant.
						// bit 2 sont 1 car c'est une note qui vient d'être créé, elle ne doit pas être re-ping-pong-ée
						tempNote2.startNote += potVal3;
						tempNote2.endNote += potVal3;

						pileNote_fixStartEndNote(tempNote2, 0);
						ZEROaddNewNotetoPile(pInitial[i], tempNote2);
						// melodyMatrixTime[i][t43][MMTstartnote] = melodyMatrixTime[i][j][MMTstartnote] + potVal3;
						// melodyMatrixTime[i][t43][MMTendnote] = melodyMatrixTime[i][j][MMTendnote] + potVal3;
					}
					// insérer plus tard une routine cleanup pour les durées trop longues qui font déborder
				}
				// clean-up, on met le bit 2 à 0

				//   taille = mesureTaillePileComplete(pInitial[i]) ;

				// 				for ( unsigned int j = 0 ;  j < taille  ; j++)
				// 				{ // va parcourir la mélodie pointée ( Layer 1 à 4 : potVal3 )
				// 					melodyMatrix[i][j][MMdata2] = (melodyMatrix[i][j][MMdata2] & B11111011);
				// 				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 44: // MORSE ( 1 seule note, répétée, avec soit durée 1 ou durée 2, espace 1 ou 2 )
	{

		// insérer un DELETE auparavant ???

		bool fairelongue;

		// potVal3 = BBB ;     // 0 à 127 puis de 127 à 160 = RANDOM
		// potVal3 va indiquer le nb de notes souhaitées, de 2 à 32
		// s'il y a potVal3 notes,
		// alors il y a potVal3 + 1 espaces ( pauses )
		// soit un total de potVal3 + potVal3 + 1 durées ( notes + pauses ): ce nb sera n
		// ces durées vont êtres réparties en Longues et Courtes : m1 longues ; n-m1 courtes ( = m2 )
		// le ratioLC définit le quotient entre durée note longue et notes courtes : à priori 2, mais peut être modifié
		// la durée totale du layer est = m1 x durée longue + m2 x durée courte
		//                              = m1 x ratioLC x durée courte + m2 x durée courte
		//                              = ( m1 x ratio LC + (n-m1) ) x durée courte : on trouve ainsi durée courte

		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				ZEROdeleteWholePile(pInitial[i]);

				byte n = (potVal3 + potVal3 + 1);
				byte m1 = random(0, n + 1); // nb de longues durées ( notes ou pauses )

				// si onveut que la mélodie soit ininterrompue il faut enlever ce +1 et retravailler le début
				// pour qu'il n'y ait pas d'espace initial

				byte m2 = n - m1;		// nb de courtes durées ( notes ou pauses )
				const byte ratioLC = 2; // définit à quel point une longue est un multiple de courte ( 2 ou 3 ? )
				unsigned long duree_courte = (unsigned long)((float)layerLength[i] / (float)((m1 * ratioLC) + m2));

				byte compteur_m1 = 0;
				byte compteur_m2 = 0;

				// unsigned int taille = mesureTaillePileComplete(pInitial[i]) ;

				for (unsigned int j = 0; j < n; j++)
				{
					// si on tire 0 et qu'il faut encore des longues : alors note longue, compteur m1+1
					// si on tire 1 et qu'il faut encore des courtes : alors note courte, compteur m2+1
					// si on tire 0 et qu'il faut durée longue : alors durée longue; compteur m1+1
					// si on tire 1 et qu'il faut durée courte : alors durée courte, compteur m2+1

					pileNote tempNote;
					// tempNote = ZEROgetNoteFromPile(pInitial[i], j);
					initializeNote(tempNote);
					tempNote.note = 60;		// le fameux C
					tempNote.velocity = 60; // arbitraire

					tempNote.channel = layerChannel[i];
					// tempNote.data2 = 0;
					tempNote.layer = i;

					// première partie, on s'occupe du moment du début, càd de la pause qui précède la note !

					if (random(2) == 0)
					{
						fairelongue = true;
					}

					if ((fairelongue == true) & (compteur_m1 < m1))
					{
						if (j == 0)
						{
							tempNote.startNote = ratioLC * duree_courte;
						}
						else
						{
							pileNote tempNote2;
							tempNote2 = ZEROgetNoteFromPile(pInitial[i], j - 1);

							tempNote.endNote = tempNote2.endNote + ratioLC * duree_courte;
						}

						compteur_m1++;
					}

					if ((fairelongue == false) & (compteur_m2 < m2))
					{
						if (j == 0)
						{
							tempNote.startNote = duree_courte;
						}
						else
						{
							pileNote tempNote2;
							tempNote2 = ZEROgetNoteFromPile(pInitial[i], j - 1);

							tempNote.startNote = tempNote2.endNote + duree_courte;
						}

						compteur_m2++;
					}

					// deuxième partie on s'occupe de la durée de la note

					if (random(2) == 0)
					{
						fairelongue = true;
					}

					if ((fairelongue) & (compteur_m1 < m1))
					{

						tempNote.endNote = tempNote.startNote + (ratioLC * duree_courte);
						compteur_m1++;
					}

					if ((fairelongue == false) & (compteur_m2 < m2))
					{

						tempNote.endNote = tempNote.startNote + duree_courte;
						compteur_m2++;
					}

					ZEROaddNewNotetoPile(pInitial[i], tempNote);

					// insérer plus tard une routine cleanup pour les durées trop longues qui font déborder
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 45: // VARISPEED
	{
		// potVal3 = BBB ;     // 30 à 300

		// BBB est la valeur passée en fonction, elle contiendra counter2 : 0 - 1024
		// il faudra passer toutes les fc en revues : celles appelées en sous-routine par d'autres doivent avoir

		// si varispeedState = 1  : un click signifie TURN ON le varispeed  : la valeur ratioCurrent devient influencée par counter2, soit BBB
		// si varispeedState = 2 : un click signifie le ratioLength devient ratioLength x ratioCurrent et ratioCurrent = 100 et counter2 devient 100
		// si varispeedState = 3 : un click signifie qu'on modifie la layerLength ; ratioLength devient 100 ; ratioCurrent = 100

		switch (varispeedState)
		{
		case 1:
		{
			for (byte i = 0; i < nbLayer; i++)
			{
				if (isLayerActive(i))
				{
					ratioCurrent[i] = potVal3;
				}
			} // actualisé ici, mais il faut aussi l'actualiser dans le player !!!
			varispeedState++;
		};
		break;

		case 2:
		{
			for (byte i = 0; i < nbLayer; i++)
			{
				if (isLayerActive(i))
				{

					ratioLength[i] = (int)((((float)ratioLength[i] * (float)ratioCurrent[i]) / (float)100));
					ratioCurrent[i] = 100;
				}
			}

			softwareValueEncoderRight = 100;

			// pour avoir un counter2 stable à 100 il faut modifier le rotKnob2
			// counter2 = ( (  rotKnob2.read() * 4 ) - 3   );     // peut-être mettre un ratio pour moduler cela ( hi precision contre lo )
			// 100 = ( x * 4 ) - 3   103 = x * 4     x = 103/4 = 26?    // pas tout à fait précis, peut-être adapter
			// rotKnob2.write(26);
			// TODO  DEBUG WTF ?
			varispeedState++;
		};
		break;

		case 3:
		{
			for (byte i = 0; i < nbLayer; i++)
			{
				if (isLayerActive(i))
				{
					realRatio = (float)((((float)ratioCurrent[i] * (float)ratioLength[i]) / (float)100));
					layerLength[i] = (unsigned long)((((float)layerLength[i] * (float)realRatio) / (float)100));
					// TODO : ne faut-il pas aussi modifier les notes ?

					unsigned int taille = mesureTaillePileComplete(pInitial[i]);

					for (unsigned int j = 0; j < taille; j++)
					{
						pileNote tempNote;
						tempNote = ZEROgetNoteFromPile(pInitial[i], j);

						// melodyMatrix[i][j][MMdata2] = (melodyMatrix[i][j][MMdata2] ^ B00000100);
						// melodyMatrix[i][j][MMdata2] = (melodyMatrix[i][j][MMdata2] ^ B00000011); // bit 0 et 1 sont 1 = played and silented pour que la note ne soit pas rejouée avant le tour suivant.

						// bit 0 et 1 étaient intialement 1 = played and silented pour que la note ne soit pas rejouée avant le tour suivant.
						tempNote.startNote = (((float)tempNote.startNote * (float)realRatio) / (float)100);
						tempNote.endNote = (((float)tempNote.endNote * (float)realRatio) / (float)100);

						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
				}
				ratioLength[i] = 100;
				ratioCurrent[i] = 100;
			}

			softwareValueEncoderRight = 100;

			// pour avoir un counter2 stable à 100 il faut modifier le rotKnob2
			// counter2 = ( (  rotKnob2.read() * 4 ) - 3   );     // peut-être mettre un ratio pour moduler cela ( hi precision contre lo )
			// 100 = ( x * 4 ) - 3   103 = x * 4     x = 103/4 = 26?    // pas tout à fait précis, peut-être adapter
			// rotKnob2.write(26);
			varispeedState++;
		};
		break;
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 46: // Ratio+Channel Info    la transfo ne fait rien du tout, c'est juste l'affichage qui est intéressant.
	{
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 47: // FIDELITY
	{
		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				// vaut-il mieux faire 1 ligne avec les 2 ?
				// à tester dans un sketch où on augmente les valeurs

				fidelityLayer_[i] = potVal3;
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 48: // Fidelity+Channel Info    la transfo ne fait rien du tout, c'est juste l'affichage qui est intéressant.
	{
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// PILE VERSION : faite et vérifiée
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 49: // FLIP Not  : différent de FLIP Loc : celui-ci changeait de place la note, avec sa durée,
		// ici on change juste la valeur MIDI, sans changer la durée

		{
			// potVal3 = BBB ;     //

			for (byte i = 0; i < nbLayer; i++)
			{

				if (isLayerActive(i))
				{

					unsigned int taille = mesureTaillePileComplete(pInitial[i]);

					for (unsigned int j = 0; j < taille; j++)
					{

						pileNote tempNote;
						tempNote = ZEROgetNoteFromPile(pInitial[i], j);

						if ((tempNote.note != 0) && (random(0, 100) < potVal3))
						{
							byte l;
							byte q;

							byte petitcompteur = 0;
							pileNote tempNote2;

							do
							{
								l = random(0, taille); // l est la note qui va être flippée avec j

								tempNote2 = ZEROgetNoteFromPile(pInitial[i], l);

								q = tempNote2.note; // flip de la note
								petitcompteur++;
							} while (q == 0 || petitcompteur < taille);

							tempNote2.note = tempNote.note;
							tempNote.note = q;

							q = tempNote2.velocity; // flip de la vélocité
							tempNote2.velocity = tempNote.velocity;
							tempNote.velocity = q;
							ZEROSetNoteInPile(pInitial[i], j, tempNote);
							ZEROSetNoteInPile(pInitial[i], l, tempNote2);
						}
					}
				}
			}
		}
		break;

		/////////////////////////////////////////////////////////////////////////////////////
		// PILE VERSION : faite et vérifiée
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 50: // TABULA RASA
	{
		/* on propose plusieurs Reset du Piccolo :
	pyramide : l'ancien standard :
	1 - CH 1 - 1000 ; 2 - CH2 - 2000 ; 3 - CH3 - 3000 ; 4 - CH4 - 4000
	Square
	1 - CH 1 - 1000 ; 2 - CH1 - 1000 ; 3 - CH1 - 1000 ; 4 - CH1 - 1000
	Random
	Complex = polyrythmic
	LONG : 32 sec

	*/

		for (byte i = 0; i < nbLayer; i++)
		{ // layers
			layerChannel[i] = 0;

			ZEROdeleteWholePile(pInitial[i]);

			ratioLength[i] = 100; // indique par combien de 100ème il faut multiplier les valeurs en lien avec le Varispeed : 100% = vitesse normale.   1% = 100 x plus lent ; 900 = 9x plus vite.
			ratioCurrent[i] = 100;
			varispeedState = 1;		 // valeurs : 1 = OFF ; 2 = ON ; 3 = FIX ; 4 = MOD
			fidelityLayer_[i] = 100; // d'emblée on joue toutes les notes, mais on pourra réduire cette "fidélité" !
			derniermodulo[i] = 0;
			avantderniermodulo[i] = 0;
		}

		// gammeActuelle = 0;
		// recordMode = 0 ;
		automodification = false; // si devient vrai, tous les automodificationtime, il y a aura modif automatique
		activeLayer = 00000001;
		muteLayer = 00000000;

		switch (potVal2)
		{
		case (1): // pyramide, l'ancien standard
		{
			for (byte i = 0; i < nbLayer; i++)
			{

				layerLength[i] = (i + 1) * 1000; // indiquera la durée de chaque layer, en msec    // si on veut des boucles de plus de 32 secondes on doit utiliser LONG pour les 3 variables suivante

			} // soit MIDI channel 1, 1,  1 et 1 !
		}
		break;
		case (2): // square
		{
			for (byte i = 0; i < nbLayer; i++)
			{

				layerLength[i] = 1000; // indiquera la durée de chaque layer, en msec    // si on veut des boucles de plus de 32 secondes on doit utiliser LONG pour les 3 variables suivante
									   //layerChannel[i] =  0  ;
			}						   // soit MIDI channel 1, 1,  1 et 1 !
		}
		break;
		case (3): // random
		{
			for (byte i = 0; i < nbLayer; i++)
			{

				layerLength[i] = random(100, 3000); // indiquera la durée de chaque layer, en msec    // si on veut des boucles de plus de 32 secondes on doit utiliser LONG pour les 3 variables suivante
													//layerChannel[i] =  0 ; // soit MIDI channel 1, 1,  1 et 1 !
			}
		}
		break;
		case (4): // complex
		{
			for (byte i = 0; i < nbLayer; i++)
			{
				layerLength[i] = 1000 * random(1, 7) / random(1, 7);
				//layerChannel[i] = 0 ;
			} // soit MIDI channel 1, 1,  1 et 1 !
		}
		break;
		case (5): // LONG IL FAUT LE RAJOUTER a la LISTE
		{
			for (byte i = 0; i < nbLayer; i++)
			{
				layerLength[i] = 32000;
				//layerChannel[i] = 0 ;
			} // soit MIDI channel 1, 1,  1 et 1 !
		}
		break;

		case (6): // RAVE 133 BPM
		{
			for (byte i = 0; i < nbLayer; i++)
			{
				layerLength[i] = 451;
				//layerChannel[i] = 0 ;
			} // soit MIDI channel 1, 1,  1 et 1 !
		}
		case (7): // TECHNOID : Layer1 = 1600msec ; Layer 2 et 3 = 400 msec ; Layer 4 = 100 msec
		{
			//for ( byte i = 0 ; i < nbLayer  ; i++ ) {

			//	layerChannel[i] = 0 ;
			//} // soit MIDI channel 1, 1,  1 et 1 !
			layerLength[0] = 1600;
			layerLength[1] = 400;
			layerLength[2] = 400;
			layerLength[3] = 100;
		}
		break;

		case (8): // 5/5/3
		{
			//for ( byte  i = 0 ; i < nbLayer  ; i++ ) {

			//	layerChannel[i] = 0 ;
			//} // soit MIDI channel 1, 1,  1 et 1 !
			layerLength[0] = 1500;
			layerLength[1] = 300;
			layerLength[2] = 300;
			layerLength[3] = 100;
		}
		break;

		case (9): //  3/5/3
		{
			//for ( byte  i = 0 ; i < nbLayer  ; i++ ) {
			//	layerChannel[i] = 0 ;
			//} // soit MIDI channel 1, 1,  1 et 1 !
			layerLength[0] = 900;
			layerLength[1] = 300;
			layerLength[2] = 300;
			layerLength[3] = 100;
		}
		break;
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 51: // ThruWhere
	{

		// potVal3 = BBB ;      // on choisit de quelle layer on veut copier les infos, vers les layers actives

		// NOTE **** pour l'instant ceci ne fait rien - voir si utile ou pas
		//
		// potVal3 : 1 à 16 -> THRU on et dirigé vers le channel en question
		// 17 -> THRU vers même channel
		// 18 -> THRU OFF
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 52: // REDUCE to 4 MIDI NOTES ( pour Nord Drum ) = ND-HORSE4
	{

		// potVal3 = map(BBB, 0, 1023, Transfo[LineCallingTransfo(51)][1] , Transfo[LineCallingTransfo(51)][2]);   //

		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{

					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					if (tempNote.note != 0)
					{
						tempNote.note = random(60, 64); // transforme les notes non vides en notes ND
						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					};
					//  melodyMatrix[i][j][MMvelocite] = 0;
					// melodyMatrix[i][j][MMdata] = 1;   // çàd les 4 premiers bits = le channel   les 4 suivants sont les données pour chord : 0 = solo
					// melodyMatrix[i][j][MMdata2] = 0;    // le bit played est 0 et le bit silented est 0 ;

					// mais on peut imaginer plus tard des channels différents dans même layer
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 53:
	{ // SLICER HORIZONTAL

		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{
					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					tempNote.startNote += ((j + 1) * potVal3); // on ajoute 30msec au premier son, 60 au 2nd, 90 au troisième, etc.
					tempNote.endNote += ((j + 1) * potVal3);
					// réparation j au lieu de i en février 2018 ! 3 mots pour trouver une connerie pareille, bravo !
					pileNote_fixStartEndNote(tempNote, 0);
					ZEROSetNoteInPile(pInitial[i], j, tempNote);
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 54:
	{					 // SCRAMBLER
		long calcultime; // doit parfois être négatif !!!!

		// on va déplacer entre 1  etv100%

		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{

					if (random(0, 100) < potVal2)
					{

						pileNote tempNote;
						tempNote = ZEROgetNoteFromPile(pInitial[i], j);

						// calcultime =    melodyMatrixTime[i][j][MMTendnote] - melodyMatrixTime[i][j][MMTstartnote] ;
						calcultime = getDuree(tempNote);

						tempNote.startNote = random(0, layerLength[i]); // on ajoute 30msec au premier son, 60 au 2nd, 90 au troisième, etc.
						tempNote.endNote = tempNote.startNote + calcultime;
						// il faut aussi ici fixer la durée CLEANUP
						pileNote_fixStartEndNote(tempNote, 0);

						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 55:
	{ // BASSLINE : réduit les layer actifs à une ligne mélodique de base : C60 à C72

		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{
					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					if (tempNote.note != 0)
					{

						if (random(0, 100) < potVal2)
						{
							// tempNote.note = (tempNote.note % 12) + 48;
							tempNote.note = modifyPitchUniversal(tempNote, (tempNote.note % 12) + 48, 0);
						}
						else
						{
							tempNote.note = 0;
							tempNote.velocity = 0;
							tempNote.killnote = true;
						}

						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
				}
			}

			ZEROtreatPileComplete(pInitial[i]);
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 56:
	{ // CONCAT100 : écrase de 100msec un % de notes

		// on va déplacer entre 1  etv100%

		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{

					if (random(0, 100) < potVal2)
					{
						pileNote tempNote;
						tempNote = ZEROgetNoteFromPile(pInitial[i], j);

						tempNote.startNote -= 100;
						tempNote.endNote -= 100;

						pileNote_fixStartEndNote(tempNote, 0);
						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 57:
	{ // EXCAT100 : écarte de 100msec un % de notes

		// on va déplacer entre 1  etv100%

		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{

					if (random(0, 100) < potVal2)
					{
						pileNote tempNote;
						tempNote = ZEROgetNoteFromPile(pInitial[i], j);

						tempNote.startNote += 100;
						tempNote.endNote += 100;

						pileNote_fixStartEndNote(tempNote, 0);
						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		// modif 2C18 pour que value soit 0 si on éteint
		/////////////////////////////////////////////////////////////////////////////////////

	case 58:
	{ // FUNKY VEL : modifie la velocité de façon rnd

		FunkyVel = true;
		//		FunkyVel = !FunkyVel; // ON ou OFF
		// FunkyVel = true ;

		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				// if (FunkyVel == true)
				{
					FunkyVelValue[i] = potVal2;
				}

				// else
				{
					//			FunkyVelValue[i] = 0;
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 59: // Lg Not : définit la durée des notes  ( pour éviter les 'ping' simples ou trop longs )

	{
		// on va déplacer entre 1  etv100%

		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{
					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					tempNote.endNote = tempNote.startNote + potVal2;
					pileNote_fixStartEndNote(tempNote, 0);
					ZEROSetNoteInPile(pInitial[i], j, tempNote);
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94 - refaite à partir de la nouvelle 43
		/////////////////////////////////////////////////////////////////////////////////////

	case 60: // WOBBLY DELAY
	{

		// potVal3 = BBB ;     // 0 à 127 puis de 127 à 160 = RANDOM

		// il faudra passer toutes les fc en revues : celles appelées en sous-routine par d'autres doivent avoir
		// leurs propres variables ( y compris potVal3, i, j, k )

		for (byte i = 0; i < nbLayer; i++)
		{

			ZEROtreatPileComplete(pInitial[i]);

			if (isLayerActive(i))
			{

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				// clean-up, on met le bit 2 à 0

				// for ( unsigned int j = 0 ;  j < taille  ; j++)
				// { // va parcourir la mélodie pointée ( Layer 1 à 4 : potVal3 )
				// 	melodyMatrix[i][j][MMdata2] = (melodyMatrix[i][j][MMdata2] & B11111011);
				// }

				// vaut-il mieux faire 1 ligne avec les 2 ?
				// à tester dans un sketch où on augmente les valeurs

				taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)

				{ // va parcourir la mélodie pointée ( Layer 1 à 4 : potVal3 )
					// if ((melodyMatrix[i][j][MMdata2] >> 2 & 0001) != 1)
					{
						pileNote tempNote;
						tempNote = ZEROgetNoteFromPile(pInitial[i], j);

						// byte t43 = firstfreenote(i);
						pileNote tempNote2;

						putNoteBinNoteA(tempNote2, tempNote);

						// melodyMatrix[i][t43][MMnote] = melodyMatrix[i][j][MMnote];
						// melodyMatrix[i][t43][MMvelocite] = melodyMatrix[i][j][MMvelocite];
						// on pourrait imaginer un delay avec une velocité réduite, mais faudrait 2 potards

						// melodyMatrix[i][t43][MMdata] = melodyMatrix[i][j][MMdata];

						// melodyMatrix[i][t43][MMdata2] = (melodyMatrix[i][j][MMdata2] ^ B00000111);
						// bit 0 et 1 sont 1 = played and silented pour que la note ne soit pas rejouée avant le tour suivant.
						// bit 2 sont 1 car c'est une note qui vient d'être créé, elle ne doit pas être re-ping-pong-ée

						long wobble = 0;
						potVal3 = random(0, potVal3);

						tempNote2.startNote += wobble;
						tempNote2.endNote += wobble;

						pileNote_fixStartEndNote(tempNote2, 0);
						ZEROaddNewNotetoPile(pInitial[i], tempNote2);
						// melodyMatrixTime[i][t43][MMTstartnote] = melodyMatrixTime[i][j][MMTstartnote] + potVal3;
						// melodyMatrixTime[i][t43][MMTendnote] = melodyMatrixTime[i][j][MMTendnote] + potVal3;
					}
					// insérer plus tard une routine cleanup pour les durées trop longues qui font déborder
				}
				// clean-up, on met le bit 2 à 0

				//   taille = mesureTaillePileComplete(pInitial[i]) ;

				// 				for ( unsigned int j = 0 ;  j < taille  ; j++)
				// 				{ // va parcourir la mélodie pointée ( Layer 1 à 4 : potVal3 )
				// 					melodyMatrix[i][j][MMdata2] = (melodyMatrix[i][j][MMdata2] & B11111011);
				// 				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94 - BUG présent depuis le début, confondant layer et channel !
		/////////////////////////////////////////////////////////////////////////////////////

	case 61: // ECARTELE
	{
		// potVal3 = BBB ;     // 0 à 127 puis de 127 à 160 = RANDOM

		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{
				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{ // va parcourir la mélodie pointée ( Layer 1 à 4 : potVal3 )
					// if ((melodyMatrix[i][j][MMdata2] >> 2 & 0001) != 1)
					{
						byte otherChan = random(4); // on va mettre la note sur un LAYER au hasard
													// byte t43 = firstfreenote(otherChan);

						pileNote tempNote;
						tempNote = ZEROgetNoteFromPile(pInitial[i], j);

						pileNote tempNote2;
						putNoteBinNoteA(tempNote2, tempNote);
						tempNote2.layer = otherChan;
						pileNote_fixStartEndNote(tempNote2);
						ZEROaddNewNotetoPile(pInitial[otherChan], tempNote2);

						putNoteBinNoteA(tempNote, emptyNote);
						tempNote.killnote = true;

						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 62:
	{	// INSERT RND CC  : première modification spéciale pour Behringer FX
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

		// potVal3 = BBB ;

		/*
	melodyMatrix[otherChan][t43][MMnote] = melodyMatrix[i][j][MMnote] ;
	melodyMatrix[otherChan][t43][MMvelocite] = melodyMatrix[i][j][MMvelocite] ;
	melodyMatrix[otherChan][t43][MMdata]  =     melodyMatrix[i][j][MMdata] ;
	melodyMatrix[otherChan][MMdata2] = (    melodyMatrix[i][j][MMdata2] ^ B00000111 );
	// bit 0 et 1 sont 1 = played and silented pour que la note ne soit pas rejouée avant le tour suivant.
	// bit 2 sont 1 car c'est une note qui vient d'être créé, elle ne doit pas être re-déplacée
	melodyMatrix[otherChan][MMTstartnote] =    melodyMatrixTime[i][j][MMTstartnote] ;
	melodyMatrix[otherChan][MMTendnote] =    melodyMatrixTime[i][j][MMTendnote]    ;
	*/
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		// MODIF 2C21 COURONNE
		/////////////////////////////////////////////////////////////////////////////////////

	case 63: // FRACTIONARY REPLICATE
	{
		//	unsigned long calcultime; // doit parfois être négatif !!!! 2C21 NON

		// on va diviser en 16èmes:   ira donc de 1/1, 2/1, 3/1,   1/2, 2/2, 3/2, 4/2, ...  15/16, 16/16
		// on ajoute d'emblée cela à la fraction déjà existante : 1/1 veut dire : duplication ; 2/1 veut dire 3x ; 1/2 veut dire 1 + 1/2 ?

		int potVal3 = (softwareValueEncoderRight / 16) + 1; // était auparavant 9
		int potVal2 = (softwareValueEncoderRight % 16) + 1;

		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				// d'abord on modifie la longueur du layer

				unsigned long initiallayerLength = layerLength[i];

				// on calcule d'abord la durée qu'on va rajouter à la durée du layer initial

				//	calcultime =   layerLength[i] * (unsigned long )potVal2 ;
				//	unsigned long fullcalcultime = calcultime / (unsigned long) potVal3 ; // calcultime indique la durée qui va se rajouter à la longueur initiale
				//	layerLength[i] = layerLength[i] +  fullcalcultime;			  // différent de Meltiply et Multiply : ici on ajoute une fraction

				layerLength[i] = (unsigned long)((float)layerLength[i] * ((float)1 + (float)potVal2 / (float)potVal3));

				// de là, on calcule la durée non entière ajoutée, càd la fraction de durée ajoutée à des multiples exacts de durée.

				int fullRepet = potVal2 / potVal3;
				//         fractcalcultime = fullcalcultime - ( fullRepet * (long) initiallayerLength[i] ) ;
				// pas nécessaire : on va multiplier 1 x de plus que prévu, et on voit si le positionnement tombe en dessous de la durée layerLength nouvelle ou pas.
				// si le startnote > layerlenght on crée pas de note

				// puis on modifie les notes
				// on doit les répliquer potVal2/potVal3 fois en plus de la durée d'origine
				// càd la partie entière de potVal2/potVal3
				// et si startnote < la partie fractionnelle potVal2 par potVal3 * durée Layer de départ (!!!) / potVal3 : ex.   9/4 -> modulo = 1 -> start < 1/4 de layer

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{
					for (int v63 = 1; v63 < (fullRepet + 1); v63++)
					{
						// if ((melodyMatrix[i][j][MMdata2] >> 2 & 0001) != 1)
						{
							pileNote tempNote;
							tempNote = ZEROgetNoteFromPile(pInitial[i], j);
							// ZEROSetNoteInPile(pInitial[i], j, tempNote);

							if (((unsigned long)tempNote.startNote + (initiallayerLength * (unsigned long)v63)) < layerLength[i])
							{ // on ne copie que les notes qui vont tomber avant la fin de la nouvelle longueur du layer !

								pileNote tempNote2;
								putNoteBinNoteA(tempNote2, tempNote);
								tempNote2.startNote = tempNote.startNote + (initiallayerLength * (unsigned long)v63);
								tempNote2.endNote = tempNote2.endNote + getDuree(tempNote);
								pileNote_fixStartEndNote(tempNote2, 0);
								ZEROaddNewNotetoPile(pInitial[i], tempNote2);
							}
							// insérer plus tard une routine cleanup pour les durées trop longues qui font déborder
						}
						// clean-up, on met le bit 2 à 0
					}
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 64:
	{ // CREATE LADDER

		// byte ladStart, ladNbSteps, ladStep, ladIncStep, ladType ;
		// int ladLength, ladPause, ladIncLength, ladIncPause ;
		// potVal3 = BBB ;    // 0 à 127 puis de 127 à 160 = RANDOM

		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				for (byte j = 0; j < ladNbSteps; j++)
				{ // va parcourir la mélodie pointée ( Layer 1 à 4 : potVal3 )

					// byte t64 = firstfreenote(i);
					pileNote tempNote;
					initializeNote(tempNote);
					tempNote.note = ladStart + (j * ladStep) + (j * ladIncStep);
					tempNote.velocity = 100;

					tempNote.channel = layerChannel[i];
					tempNote.layer = i;

					// melodyMatrix[i][t64][MMdata2] = ( melodyMatrix[i][j][MMdata2] ^ B00000100 ); 1C87
					// melodyMatrix[i][t64][MMdata2] = (    melodyMatrix[i][j][MMdata2] ^ B00000011 ); // bit 0 et 1 sont 1 = played and silented pour que la note ne soit pas rejouée avant le tour suivant. 1C87

					// bit 0 et 1 étaient intialement 1 = played and silented pour que la note ne soit pas rejouée avant le tour suivant.
					// melodyMatrixTime[i][t64][MMTstartnote] =  1 + ( j * ladLength) + (j * ladPause ) + (j * ladIncLength) + (j * ladIncPause) ;
					tempNote.startNote = 1 + (j * ladPause) + (j * ladIncLength) + (j * ladIncPause);
					tempNote.endNote = tempNote.startNote + ladPause + (j * ladIncPause);
					pileNote_fixStartEndNote(tempNote, 0);
					ZEROaddNewNotetoPile(pInitial[i], tempNote);
				}
			}
			// clean-up, on met le bit 2 à 0
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 65:
	{ // DEFINE LAD START
		// byte ladStart, ladNbSteps, ladStep, ladIncStep, ladType ;
		// int ladLength, ladPause, ladIncLength, ladIncPause ;
		//potVal3 = BBB ;      // 0 à 127 puis de 127 à 160 = RANDOM
		ladStart = potVal3;
	};
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 66:
	{ // DEFINE LAD Nb Steps
		// byte ladStart, ladNbSteps, ladStep, ladIncStep, ladType ;
		// int ladLength, ladPause, ladIncLength, ladIncPause ;
		//potVal3 = BBB ;     // 0 à 127 puis de 127 à 160 = RANDOM
		ladNbSteps = potVal3;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 67:
	{ // DEFINE LAD Note Length
		// byte ladStart, ladNbSteps, ladStep, ladIncStep, ladType ;
		// int ladLength, ladPause, ladIncLength, ladIncPause ;
		//potVal3 = BBB ;     // 0 à 127 puis de 127 à 160 = RANDOM
		ladLength = potVal3;
	};
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 68:
	{ // DEFINE LAD Pause Length
		// byte ladStart, ladNbSteps, ladStep, ladIncStep, ladType ;
		// int ladLength, ladPause, ladIncLength, ladIncPause ;
		// potVal3 = BBB ;     // 0 à 127 puis de 127 à 160 = RANDOM
		ladLength = potVal3;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 69:
	{ // DEFINE LAD Inc Length
		// byte ladStart, ladNbSteps, ladStep, ladIncStep, ladType ;
		// int ladLength, ladPause, ladIncLength, ladIncPause ;
		//potVal3 = BBB ;     // 0 à 127 puis de 127 à 160 = RANDOM
		ladIncLength = potVal3;
	};
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 70:
	{ // DEFINE LAD Inc Pause
		// byte ladStart, ladNbSteps, ladStep, ladIncStep, ladType ;
		// int ladLength, ladPause, ladIncLength, ladIncPause ;
		//potVal3 = BBB ;     // 0 à 127 puis de 127 à 160 = RANDOM
		ladIncPause = potVal3;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 71:
	{ // DEFINE LAD Inc Pause
		// byte ladStart, ladNbSteps, ladStep, ladIncStep, ladType ;
		// int ladLength, ladPause, ladIncLength, ladIncPause ;
		//potVal3 = BBB ;    // 0 à 127 puis de 127 à 160 = RANDOM
		ladStep = potVal3;
	};
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 72:
	{ // DEFINE LAD Inc Pause
		// byte ladStart, ladNbSteps, ladStep, ladIncStep, ladType ;
		// int ladLength, ladPause, ladIncLength, ladIncPause ;
		//potVal3 = BBB ;     // 0 à 127 puis de 127 à 160 = RANDOM
		ladIncStep = potVal3;
	};
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 73:
	{	// SORT LAYER by START NOTE
		// n'a aucun sens dans le cas d'une pile de notes.
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94 a été modifié en sort by pitch en fait ...
		/////////////////////////////////////////////////////////////////////////////////////

	case 74:
	{ // SORT LAYER by PITCH
		// Ajouter un SORT by Length

		byte currSort;
		byte Tf;

		for (byte i = 0; i < nbLayer; i++)
		{
			unsigned int taille = mesureTaillePileComplete(pInitial[i]);

			if (isLayerActive(i) && (taille > 1))
			{
				currSort = 0;
				// si ptich de note > pitch de note + 1 alors on intervertit
				// et on reprend à note - 1 sauf si = 0
				// jusqu'à ce que note = 32
				do
				{
					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], currSort);

					pileNote tempNote2;
					tempNote2 = ZEROgetNoteFromPile(pInitial[i], currSort + 1);

					if ((tempNote.note > tempNote2.note) && (tempNote.startNote < tempNote2.endNote))
					// problème pour les cases vides, sans note = 0 !
					{
						Tf = tempNote.note;
						tempNote.note = tempNote2.note;
						tempNote2.note = Tf;

						ZEROSetNoteInPile(pInitial[i], currSort, tempNote);
						ZEROSetNoteInPile(pInitial[i], currSort + 1, tempNote2);

						if (currSort > 0)
						{
							currSort = currSort - 1;
						}
					}
					else
					{
						currSort = currSort + 1;
					}
				} while (currSort < (taille - 1));
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 75:
	{ // MODIFY TX81Z parameters

		//  potVal3 = map(BBB, 0, 1023, Transfo[LineCallingTransfo(80)][1] , Transfo[LineCallingTransfo(80)][2]);   // 0 à 127 puis de 127 à 160 = RANDOM

		/* page 67 du manuel Yamaha TX81Z
	There are 8 subgroups of  Parameter Changes :
	VCED, ACED, PCED, Remote Switch, Micro Tuning, Program Change, Effect Data, System Data

	VCED, ACED, PCED and Remote Switch have this hex format :

	F0            Exclusive
	43            Yamaha ID
	1n            Basic Receive Channel
	0ggggghh      ggggg = Group number, hh = subgroup number
	00100 10 pour VCED - au final le double byte est : 12
	00100 11 pour ACED                                 13
	00100 11 pour PCED                                 13
	00100 11 pour Remote Switch avec ddddddd = 0 pour OFF ou 7F pour on
	0ppppppp      ppppppp = Parameter Number
	0ddddddd      ddddddd = Data
	F7            End of Exclusive

	System Parameter Change and Effect Parameter Change have this hex format :

	F0            Exclusive
	43            Yamaha ID
	1n            Basic Receive Channel
	0ggggghh      ggggg = 00100, hh = 00 - au final le double byte est : 10
	0ppppppp      ppppppp = 1111011 = System Parameter
	ppppppp = 1111100 = Effect Parameter
	0kkkkkkk      kkkkkkk = ParameterNumber
	0ddddddd      ddddddd = data
	F7            End of Exclusive

	pour Micro Tune et Program Change table voir page 68 du Manuel

	*/
		byte paramTX81Z = random(0, 63);
		byte dataTX81Z = 2;
		if (paramTX81Z < 52)
		{
			switch (paramTX81Z % 13)
			{
			case 0:
				dataTX81Z = random(0, 32);
			case 1:
				dataTX81Z = random(0, 32);
			case 2:
				dataTX81Z = random(0, 32);
			case 3:
				dataTX81Z = random(1, 16);
			case 4:
				dataTX81Z = random(0, 16);
			case 5:
				dataTX81Z = random(0, 99);
			case 6:
				dataTX81Z = random(0, 3);
			case 7:
				dataTX81Z = random(0, 7);
			case 8:
				dataTX81Z = random(0, 1);
			case 9:
				dataTX81Z = random(0, 7);
			case 10:
				dataTX81Z = random(0, 99);
			case 11:
				dataTX81Z = random(0, 63);
			case 12:
				dataTX81Z = random(0, 6);
			}
		}
		else
		{
			switch (paramTX81Z)
			{
			case 52:
				dataTX81Z = random(0, 7);
			case 53:
				dataTX81Z = random(0, 7);
			case 54:
				dataTX81Z = random(0, 99);
			case 55:
				dataTX81Z = random(0, 99);
			case 56:
				dataTX81Z = random(0, 99);
			case 57:
				dataTX81Z = random(0, 99);
			case 58:
				dataTX81Z = random(0, 1);
			case 59:
				dataTX81Z = random(0, 3);
			case 60:
				dataTX81Z = random(0, 7);
			case 61:
				dataTX81Z = random(0, 3);
			case 62:
				dataTX81Z = random(0, 48);
			default:
				dataTX81Z = 2; // hasard
			}
		}

		Serial3.write(0xF0);
		Serial3.write(0x43);
		Serial3.write(0x10);
		Serial3.write(0x12);
		Serial3.write(paramTX81Z);
		Serial3.write(dataTX81Z);
		Serial3.write(0xF7);
	};
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 76:
	{ // SPLIT : crée dans un %age de note un déboublement à très courte distance, 10msec

		// potVal2 = BBB ;
		// on va déplacer entre 1  etv 100%

		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{
					if (random(0, 100) < potVal2)
					{
						pileNote tempNote;
						tempNote = ZEROgetNoteFromPile(pInitial[i], j);
						tempNote.velocity = random(60, 127); // pour que ne soit pas trop parfait !
						tempNote.startNote = tempNote.startNote + 10;
						tempNote.endNote = tempNote.endNote + 10;
						ZEROaddNewNotetoPile(pInitial[i], tempNote);
						;
					}
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 77:
	{ // RND UP : crée dans un %age de note une aumgmentation ou diminution du pitch

		// potVal2 = BBB ;
		// on va déplacer entre 1  etv 100%

		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{
					if (random(0, 100) < potVal2)
					{

						pileNote tempNote;
						tempNote = ZEROgetNoteFromPile(pInitial[i], j);

						if (random(2) == 0)
						{
							// tempNote.not = limiterForPitch(tempNote.note + random(RndUpScope + 1));
							// modifyPitchADD(tempNote, random(RndUpScope + 1));
							tempNote.note = modifyPitchUniversal(tempNote, tempNote.note, random(RndUpScope + 1));
						}
						else
						{
							// tempNote.not = limiterForPitch(tempNote.note - random(RndUpScope + 1));
							// modifyPitchADD(tempNote, - random(RndUpScope +1 )) ;
							tempNote.note = modifyPitchUniversal(tempNote, tempNote.note, -random(RndUpScope + 1));
						}

						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 78:
	{ // DEFINE RndUpScope
		// potVal3 = BBB ;     // 0 à 127 puis de 127 à 160 = RANDOM
		RndUpScope = potVal3;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 79: // DEPLACE %
	{

		// potVal3 = BBB ;     // on choisit de quelle layer on veut copier les infos, vers les layers actives

		if (random(2) == 0)
		{
			potVal2 = random(DeplaceScope);
		}
		else
		{
			potVal2 = -random(DeplaceScope);
		}

		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{
				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{

					if (random(0, 100) < potVal3)
					{
						pileNote tempNote;
						tempNote = ZEROgetNoteFromPile(pInitial[i], j);

						tempNote.startNote = tempNote.startNote + potVal2;
						tempNote.endNote = tempNote.endNote + potVal2;
						pileNote_fixStartEndNote(tempNote, 0);
						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
				}
			}
		}
	}

	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 80:
	{ // DEFINE DeplaceScope
		// potVal3 = BBB ;     // 0 à 127 puis de 127 à 160 = RANDOM
		DeplaceScope = potVal3;
	}
	break;

	case 81: // SAVE TO SD
		// BLACK 08 : dismiss ???
		// au profit du seul Save et Load02 ?
		// c'est déjà la cas car en figure plus dans l'array
		// je supprime donc.
		// s'il faut retrouver, aller voir dans les archives, avant BLACK08
		{
		}
		break;

	case 82:
	{	// LOAD FROM SD
		// BLACK08 : remplacé par Load2
		// https://www.arduino.cc/en/Serial/ReadStringUntil

		// fin de ligne = "\r\n"
		// TAB =  '\t"
		// Prints data to the serial port as human-readable ASCII text followed by a carriage return character (ASCII 13, or '\r') and a newline character (ASCII 10, or '\n'). This command takes the same forms as Serial.print().
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 83: // DAUBYMODIFICATION
	{
		Daubymodification = !Daubymodification;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94 - DISMISS 2C27
		/////////////////////////////////////////////////////////////////////////////////////

		/* DISMISS 2C27 

	case 84:
	{ // LOAD & PLAY MIDI
	
	}
	break;

*/
		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 85:
	{ // Standardt TX81Z Poly I setup

		for (byte i = 0; i < 8; i++)
		{
			// on va éditer les paramètres de 8 Instruments

			// MAX NOTES
			// *************************************************************************
			// Crééer une fonction qui permette de passer 7 arguments au TX81Z

			Serial3.write(0xF0);
			Serial3.write(0x43);
			Serial3.write(0x10);
			Serial3.write(0x10); // PCED
			// Serial3.write(123);
			Serial3.write((i * 12) + 0);
			Serial3.write(2); // on peut en mettre 8 au total maximum !  si on met 2 ici, seul 4 voix seront enregistrées sur les 8
			Serial3.write(0xF7);

			delay(10);

			// VOICE NUMBER MSB

			Serial3.write(0xF0);
			Serial3.write(0x43);
			Serial3.write(0x10);
			Serial3.write(0x10); // PCED
			// Serial3.write(123);
			Serial3.write((i * 12) + 1);
			Serial3.write(0);
			Serial3.write(0xF7);
			delay(10);
			// VOICE NUMBER

			Serial3.write(0xF0);
			Serial3.write(0x43);
			Serial3.write(0x10);
			Serial3.write(0x10); // PCED
			// Serial3.write(123);
			Serial3.write((i * 12) + 2);
			Serial3.write(i);
			Serial3.write(0xF7);
			delay(10);
			// RECEIVE CHANNEL

			Serial3.write(0xF0);
			Serial3.write(0x43);
			Serial3.write(0x10);
			Serial3.write(0x10); // PCED
			// Serial3.write(123);
			Serial3.write((i * 12) + 3);
			Serial3.write(i);
			Serial3.write(0xF7);
			delay(10);
			// LOW NOTE LIMIT

			Serial3.write(0xF0);
			Serial3.write(0x43);
			Serial3.write(0x10);
			Serial3.write(0x10); // PCED
			// Serial3.write(123);
			Serial3.write((i * 12) + 4);
			Serial3.write(0);
			Serial3.write(0xF7);
			delay(10);
			// HIGH NOTE LIMIT

			Serial3.write(0xF0);
			Serial3.write(0x43);
			Serial3.write(0x10);
			Serial3.write(0x10); // PCED
			// Serial3.write(123);
			Serial3.write((i * 12) + 5);
			Serial3.write(127);
			Serial3.write(0xF7);
			delay(10);
			// INSTRUMENT DETUNE

			Serial3.write(0xF0);
			Serial3.write(0x43);
			Serial3.write(0x10);
			Serial3.write(0x10); // PCED
			// Serial3.write(123);
			Serial3.write((i * 12) + 6);
			Serial3.write(7); // O-14 : center = 7
			Serial3.write(0xF7);
			delay(10);
			// NOTE SHIFT

			Serial3.write(0xF0);
			Serial3.write(0x43);
			Serial3.write(0x10);
			Serial3.write(0x10); // PCED
			// Serial3.write(123);
			Serial3.write((i * 12) + 7);
			Serial3.write(24); // 0-48 ; center = 24
			Serial3.write(0xF7);
			delay(10);
			// VOLUME

			Serial3.write(0xF0);
			Serial3.write(0x43);
			Serial3.write(0x10);
			Serial3.write(0x10); // PCED
			// Serial3.write(123);
			Serial3.write((i * 12) + 8);
			Serial3.write(99);
			Serial3.write(0xF7);
			delay(10);
			// OUTPUT ASSIGN

			Serial3.write(0xF0);
			Serial3.write(0x43);
			Serial3.write(0x10);
			Serial3.write(0x10); // PCED
			// Serial3.write(123);
			Serial3.write((i * 12) + 9);
			Serial3.write(3); // 0 Off ; 1 (I) ; 2 (II) ; 3 (I II)
			Serial3.write(0xF7);
			delay(10);
			// LFO SELECT

			Serial3.write(0xF0);
			Serial3.write(0x43);
			Serial3.write(0x10);
			Serial3.write(0x10); // PCED
			// Serial3.write(123);
			Serial3.write((i * 12) + 10);
			Serial3.write(0); // 0 Off ; 1 Inst1 ; 2 Inst2 ; 3 Vib
			Serial3.write(0xF7);
			delay(10);
			// MicroTune Enable

			Serial3.write(0xF0);
			Serial3.write(0x43);
			Serial3.write(0x10);
			Serial3.write(0x10); // PCED
			// Serial3.write(123);
			Serial3.write((i * 12) + 11);
			Serial3.write(0); // 0 Off ; 1 On
			Serial3.write(0xF7);
			delay(10);
		}

		Serial3.write(0xF0);
		Serial3.write(0x43);
		Serial3.write(0x10);
		Serial3.write(0x10); // PCED
		// Serial3.write(123);
		Serial3.write(97);
		Serial3.write(0); // 0 Normal 1 Alternate : ce denier mode joue une note de chaque voix à tour de rôle
		Serial3.write(0xF7);
		delay(10);
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 86:
	{ // DEFINE midiLongueurFgt
		// potVal3 = BBB ;     // 1 à 32
		midiLongueurFgt = potVal3;
	}
	break;

		// LoadMIDINotesFromFile

	case 87:
	{ // LOAD Sequence MIDI file

		// potVal3 = BBB ;     // 1 à 32
		CurrentMIDIlocation = potVal3;
		LoadMIDINotesFromFile(CurrentMIDIlocation);
		// va charger à l'emplacement CurrentMIDIlocation un nombre de notes défini par midiLongueurFgt ;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 88:
	{ // LOAD NEXT Sequence MIDI file
		if ((unsigned int)CurrentMIDIlocation + (unsigned int)midiLongueurFgt < (unsigned int)midiFile_totalNotes)
		{
			CurrentMIDIlocation = CurrentMIDIlocation + midiLongueurFgt;
		}
		// potVal3 = map(BBB, 0, 1023, Transfo[LineCallingTransfo(86)][1] , Transfo[LineCallingTransfo(86)][2]);   // 1 à 32
		LoadMIDINotesFromFile(CurrentMIDIlocation);
		// va charger à l'emplacement CurrentMIDIlocation un nombre de notes défini par midiLongueurFgt ;
	}
	break;

	case 89:
	{ // LOAD PREVIOUS Sequence MIDI file
		if (CurrentMIDIlocation - midiLongueurFgt > 0)
		{
			CurrentMIDIlocation = CurrentMIDIlocation - midiLongueurFgt;
		}
		else
		{
			CurrentMIDIlocation = 0;
		}
		// qqes test à faire ?

		// potVal3 = map(BBB, 0, 1023, Transfo[LineCallingTransfo(86)][1] , Transfo[LineCallingTransfo(86)][2]);   // 1 à 32
		LoadMIDINotesFromFile(CurrentMIDIlocation);
		// va charger à l'emplacement CurrentMIDIlocation un nombre de notes défini par midiLongueurFgt ;
	}
	break;

	case 90:
	{ // LOAD P+1 Sequence MIDI file
		if ((unsigned int)CurrentMIDIlocation < (unsigned int)midiFile_totalNotes)
		{
			CurrentMIDIlocation = CurrentMIDIlocation + 1;
		}
		// qqes test à faire ?

		// potVal3 = map(BBB, 0, 1023, Transfo[LineCallingTransfo(86)][1] , Transfo[LineCallingTransfo(86)][2]);   // 1 à 32
		LoadMIDINotesFromFile(CurrentMIDIlocation);
		// va charger à l'emplacement CurrentMIDIlocation un nombre de notes défini par midiLongueurFgt ;
	}
	break;

	case 91:
	{ // LOAD P-1 Sequence MIDI file
		if (CurrentMIDIlocation > 1)
		{
			CurrentMIDIlocation = CurrentMIDIlocation - 1;
		}
		// qqes test à faire ?

		// potVal3 = map(BBB, 0, 1023, Transfo[LineCallingTransfo(86)][1] , Transfo[LineCallingTransfo(86)][2]);   // 1 à 32
		LoadMIDINotesFromFile(CurrentMIDIlocation);
		// va charger à l'emplacement CurrentMIDIlocation un nombre de notes défini par midiLongueurFgt ;
	}
	break;

	case 92:
	{ // LOAD Random Sequence MIDI file

		CurrentMIDIlocation = random(1, (unsigned int)midiFile_totalNotes - (unsigned int)midiLongueurFgt);

		// potVal3 = map(BBB, 0, 1023, Transfo[LineCallingTransfo(86)][1] , Transfo[LineCallingTransfo(86)][2]);   // 1 à 32
		LoadMIDINotesFromFile(CurrentMIDIlocation);
		// va charger à l'emplacement CurrentMIDIlocation un nombre de notes défini par midiLongueurFgt ;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 93:
	{ // simply DEFINE MIDI file - ne charge rien.

		// devra définir le nom du fichier midi
		// ainsi que la taille en nb de notes : TailleTotaleMIDI -> 2C28 midiFile_totalNotes

		NameFileNumber(currentFileNumber);
		midiname = name; // est utilisé par les fc Load Midi etc

		buffer_clearScreen_HARD();
		buffer_and_HARD_setCursor(0, 0);
		buffer_and_HARD_print(F("SD init OK"));

		if (calculateMIDIsizeBeforePlay)
		{
			Definemidifile1();

			buffer_and_HARD_setCursor(0, 5);
			buffer_and_HARD_print(F("size : "));
			// buffer_and_HARD_print(tailleTotaleMIDI);
			buffer_and_HARD_print(midiFile_totalNotes); // 2C28
			delay(3000);
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 94: // TAP SQUEEZE ON
	{
		SqueezeOn = !SqueezeOn;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 95: // TAP SQUEEZE KEEP LENGTH : par défaut OFF
	{
		KeepLength = !KeepLength;
	}
	break;

	case 96:
	{ // GRID : recale les notes sur une grille divisant chaque layer en n morceaux équidistants.

		byte nbAB = BBB;
		// on va créer entre 1 et 32 morceaux ( 2 x 16 )

		for (byte i = 0; i < nbLayer; i++)
		{

			unsigned long lgAB = layerLength[i] / nbAB;

			if (isLayerActive(i))
			{

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);
				pileNote tempNote;

				for (unsigned int j = 0; j < taille; j++)
				{
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					// *********************** EXPLORER CES CHGTS DE BITS DANS TRANSFO 96 et 76 !!!!
					// melodyMatrix[i][j][MMdata2] = (melodyMatrix[i][j][MMdata2] ^ B00000100);
					// melodyMatrix[i][j][MMdata2] = (melodyMatrix[i][j][MMdata2] ^ B00000011); // bit 0 et 1 sont 1 = played and silented pour que la note ne soit pas rejouée avant le tour suivant.
					// bit 0 et 1 étaient intialement 1 = played and silented pour que la note ne soit pas rejouée avant le tour suivant.

					KeepDuree = getDuree(tempNote);
					//  melodyMatrixTime[i][j][MMTendnote] - melodyMatrixTime[i][j][MMTstartnote] ;
					unsigned long iAB = tempNote.startNote / lgAB;
					unsigned long tempA = iAB * lgAB;		// donne le temps dans la grille parfaite avant startnote
					unsigned long tempB = (iAB + 1) * lgAB; // donne le temps dans la grille parfaite après startnote

					if ((tempNote.startNote - tempA) < (tempB - tempNote.startNote))
					{
						tempNote.startNote = tempA;
					}
					else
					{
						tempNote.startNote = tempB;

						if ((layerLength[i] - tempNote.startNote) < (signed int)((float)lgAB / (float)3))
						{
							tempNote.startNote = 0; // les notes qui doivent être envoyées à la fin du layer sont en fait envoyées sur la toute première barre.
						}
					}

					tempNote.endNote = tempNote.startNote + KeepDuree;
					pileNote_fixStartEndNote(tempNote, 0);
					ZEROSetNoteInPile(pInitial[i], j, tempNote);
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// PILE VERSION : faite et vérifiée
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 97: // create Euclidian Rhythm
	{
		String Necklace = nulString;
		;
		String usualA = F("1");
		String usualB = F("0");

		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{

				Necklace = Euclidian(kNecklace, mNecklace);
				Necklace = StringShift(Necklace, ShiftNecklace);

				for (byte j = 0; j < mNecklace; j++)
				{
					// on va passer en revue les m positions du Necklace

					if (Necklace[j] == usualA[0])
					{
						// la valeur définie comme un pulse, normalement "1"
						pileNote tempNote;
						initializeNote(tempNote);
						tempNote.note = NoteNecklace;

						tempNote.channel = layerChannel[i];
						// tempNote.data2 = 0;
						tempNote.layer = i;
						tempNote.velocity = VelNecklace;
						tempNote.startNote = (unsigned long)((float)layerLength[i] * (float)j / ((float)mNecklace));
						tempNote.endNote = tempNote.startNote + LGNecklace;
						pileNote_fixStartEndNote(tempNote, 0);
						ZEROaddNewNotetoPile(pInitial[i], tempNote);
						// --------------------------------

						// 							  byte ff = firstfreenote(i);
						// melodyMatrix[i][ff][MMnote] = NoteNecklace;								 // définie par fonction 98
						// melodyMatrix[i][ff][MMdata] = (melodyMatrix[i][ff][MMdata] & B11110000); // on efface les 4 derniers bits
						// melodyMatrix[i][ff][MMdata] = melodyMatrix[i][ff][MMdata] + layerChannel[i];
						// melodyMatrix[i][ff][MMdata2] = (melodyMatrix[i][ff][MMdata2] ^ B00000011); // bit 0 et 1 sont 1 = played and silented pour que la note ne soit pas rejouée avant le tour suivant.
						// melodyMatrix[i][ff][MMvelocite] = VelNecklace;							   // sera définie plus tard par fonction 111
						// melodyMatrixTime[i][ff][MMTstartnote] = (int)((float)layerLength[i] * (float)j / (float)mNecklace) + 1;
						// melodyMatrixTime[i][ff][MMTendnote] = melodyMatrixTime[i][ff][MMTstartnote] + LGNecklace;
						// fixStartEndNote(i, ff, 0);
					}
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 98:
	{ // DEFINE Note for the necklace

		NoteNecklace = potVal3;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 99:
	{ // DEFINE k pulse for the necklace
		kNecklace = potVal3;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 100:
	{ // DEFINE m size for the necklace
		mNecklace = potVal3;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 101:
	{ // DEFINE length of note
		LGNecklace = potVal3;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 102:
	{ // DEFINE Shift dans le necklace
		ShiftNecklace = potVal3;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 103:
	{ // DEFINE vélocité dans le necklace
		VelNecklace = potVal3;
	}
	break;

		// int CHSendToSampler ; // les bits indiquent si le channel doit être envoyé au sampler : ex SendToSampler >> 3 == 1 alors on fait que que jouer note, on envoie des CC au channel 3 pour jouer un sample
		// int CHMuteNote /

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 104:
	{ // SEND CH TO SAMPLER

		// potVal3 = BBB ;    // 1 à 16
		CHSendToSampler = CHSendToSampler ^ (int)(1 << (potVal3 - 1)); // PotVal3 - 1 indique le code du Channel ( Channel 1 = MIDI 0 )

		// ON DOIT AJOUTER LE CODE POUR ARMER LE SAMPLER SI ON A ALLUME UN CHANNEL POUR
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


	Serial3.write(176);  // B0
	Serial3.write(102);  // change FX
	Serial3.write(56);  // FX 56 = SAMPLER

	*/

		buffer_and_HARD_setCursor(0, 1);
		String a = String(CHSendToSampler, BIN);
		//		u8x8.print(CHSendToSampler, BIN);
		buffer_and_HARD_print(a);

		//		delay(100);
		// TODO DEBUG :  à essayer
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 105:
	{ // MUTE CH Notes : mais garde le send to sampler
		// potVal3 = BBB ;     // 1 à 16
		CHMuteNote = CHMuteNote ^ (int)(1 << (potVal3 - 1)); // PotVal3 - 1 indique le code du Channel ( Channel 1 = MIDI 0 )

		buffer_and_HARD_setCursor(0, 1);
		String a = String(CHMuteNote, BIN); /// ???? Black5
											// u8x8.print(CHMuteNote, BIN);
		buffer_and_HARD_print(a);
		//		delay(100);
		// TODO DEBUG :  à essayer
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 106: // GATED MULTIPLE - développemnt à partir de case 43 ( PING PONG )
	{
		byte n;
		// potVal3 = BBB ;     //
		// ce n'est pas potVal3 qui transmet l'info mais MultiplicationsString

		for (byte i = 0; i < nbLayer; i++)
		{

			ZEROtreatPileComplete(pInitial[i]);
			// on fait le ménage avant de commencer

			if (isLayerActive(i))
			{

				n = MultiplicationsString.length();

				layerLength[i] = layerLength[i] * NbMultiplications;

				// clean-up, on met le bit 2 à 0

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);
				// attention, cette valeur changera qd on augmentera le nombre de notes !
				// garder la valeur initiale !

				// for ( unsigned int j = 0 ;  j < taille  ; j++)
				// { // va parcourir la mélodie pointée ( Layer 1 à 4 : potVal3 )
				// 	melodyMatrix[i][j][MMdata2] = (melodyMatrix[i][j][MMdata2] & B11111011);
				// }

				// vaut-il mieux faire 1 ligne avec les 2 ?
				// à tester dans un sketch où on augmente les valeurs

				for (unsigned int j = 0; j < taille; j++)
				{ // va parcourir la mélodie pointée ( Layer 1 à 4 : potVal3 )
					//	 if ((melodyMatrix[i][j][MMdata2] >> 2 & 0001) != 1)

					{
						// alors si le début de note est plus bas que layerlength
						// alors pour k allant de n à 1 ( de façon décroissante)
						// alors si le caractère k du String représentant le rythme OU >> n & 1   VAUT 1
						// alors la première note libre devient même note et son temps = Start + note (start) + k x layerLength
						//
						// par contre si le caractère k du String représentant le rythme OU >> n & 1   VAUT 0
						// et que n = 1 on doit effacer la première note !

						// il faut réfléchir aussi au cas où une fin de note arrivant AVANT un début dans un layer ( overlap)

						pileNote tempNote;
						tempNote = ZEROgetNoteFromPile(pInitial[i], j);

						// if ((melodyMatrix[i][j][MMnote] != 0) )
						// && ((unsigned int)melodyMatrixTime[i][j][MMTstartnote] < layerLength[i]))
						{
							for (byte q = n; q > 0; q--)
							{
								if (MultiplicationsString.substring(q - 1, q) == "1")
								{
									// créer une note
									pileNote tempNote2;

									tempNote2.note = tempNote.note;
									tempNote2.velocity = tempNote.velocity;

									tempNote2.channel = tempNote.channel;
									tempNote2.layer = i;

									tempNote2.startNote = tempNote.startNote + (q * layerLength[i]);
									tempNote2.endNote = tempNote.endNote + (q * layerLength[i]);

									pileNote_fixStartEndNote(tempNote2, 0);

									ZEROaddNewNotetoPile(pInitial[i], tempNote);
								}
								else
								{
									// la valeur est "0" : soit on ne créer rien, ou si on est à la valeur initiale on efface la note
									if (q == 1)
									{
										// on efface la note
										tempNote.note = 0;
										tempNote.velocity = 0;
										tempNote.killnote = true;
										tempNote.startNote = 0;
										tempNote.endNote = 0;
										ZEROSetNoteInPile(pInitial[i], j, tempNote);
									}
								}
							}
						}
					}
					// insérer plus tard une routine cleanup pour les durées trop longues qui font déborder
				}
				// clean-up, on met le bit 2 à 0

				//   taille = mesureTaillePileComplete(pInitial[i]) ;

				// for ( unsigned int j = 0 ;  j < taille  ; j++)
				// { // va parcourir la mélodie pointée ( Layer 1 à 4 : potVal3 )
				// 	melodyMatrix[i][j][MMdata2] = (melodyMatrix[i][j][MMdata2] & B11111011);
				// }
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 107:
	{ // DEFINE Nb de Multiplications

		// potVal3 = BBB ;     // 2 à 16
		NbMultiplications = potVal3;
		Transfo[LineCallingTransfo(106)][2] = MaximumBinForSteps(NbMultiplications) + 1;
		// le +1 est discutable mais il faut éviter le 1110 qui arrive quand le potard n'est pas au bout
		//
		// adapte la valeur du tableau en fc du nombre de pas  pour que le potentiomètre de l'effet 106
		// reflète les différentes valeurs binaires possibles
		// TODO DEBUG very bad code !
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 108:
	{ // VERTICAL MAPPING FROM LOWEST NOTE and HIGHEST NOTE to desired values
		// potVal3 = map(BBB, 0, 1023, Transfo[LineCallingTransfo(108)][1] , Transfo[LineCallingTransfo(108)][2]);   // 2 à 16

		// NOTE DEBUG **** peut être faire un CleanUp avant, car parfois trouve des notes silencieuses !
		//
		byte templow = FindLowestNoteOfActive(); // on peut pas faire une ligne avec un argument qui sera changé durant transfo
		byte temphigh = FindHighestNoteOfActive();

		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{
				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{
					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					// tempNote.not  = noteWrapper(map(tempNote.note, templow, temphigh, Transfo[LineCallingTransfo(108)][1], Transfo[LineCallingTransfo(108)][2]));
					// modifyPitchSET(tempNote, noteWrapper(map(tempNote.note, templow, temphigh, Transfo[LineCallingTransfo(108)][1], Transfo[LineCallingTransfo(108)][2]))) ;

					tempNote.note = modifyPitchUniversal(tempNote, noteWrapper(map(tempNote.note, templow, temphigh, Transfo[LineCallingTransfo(108)][1], Transfo[LineCallingTransfo(108)][2])));

					pileNote_fixStartEndNote(tempNote, 0);
					ZEROSetNoteInPile(pInitial[i], j, tempNote);
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 109:
	{ // SET Target MAX

		// potVal3 = BBB ;    // 2 à 16

		Transfo[LineCallingTransfo(108)][2] = potVal3; // il modifie les valeur du tableau pour 108 !
													   // NOTE DEBUG **** je crois qu'à terme il faut abandonner les transformations du tableau et leur préférer des variables
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 110:
	{ // SET Target MIN

		// potVal3 = BBB ;     // 2 à 16

		Transfo[LineCallingTransfo(108)][1] = potVal3; // il modifie les valeur du tableau pour 108 !
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// PILE VERSION : faite et vérifiée
		/////////////////////////////////////////////////////////////////////////////////////

	case 111:
	{ // MESSAGE ROTATIF

		// ne fait rien
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// PILE VERSION : faite et vérifiée
		/////////////////////////////////////////////////////////////////////////////////////

	case 112:
	{ // AUTO HARMONIZE ON

		autoHarmonize = !autoHarmonize;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// PILE VERSION : faite et vérifiée
		/////////////////////////////////////////////////////////////////////////////////////

	case 113: // TAP SQUEEZE ON
	{
		complexSqueeze = !complexSqueeze;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 114: // CLEAN UP
	{

		// potVal3 = BBB ;     // on choisit de quelle layer on veut copier les infos, vers les layers actives
		// choisir quoi faire avec les notes trop hautes ou trop loin : DELETE / WRAP / limit
		// WRAP
		// LIMIT
		// DELETE
		// BOUNCE

		rememberCleanUpChoice = potVal3;

		for (byte i = 0; i < nbLayer; i++)
		{
			// FAUT IL METTRE LES i ET j en varialbes internes à la fc ?

			if (isLayerActive(i))
			{
				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{ // va parcourir la mélodie pointée ( Layer 1 à 4 : potVal3 )

					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					if (tempNote.note > 127)
					{
						tempNote.note = 126;
						break;
						// switch(potVal3) {
						// 	case 1 : melodyMatrix[i][j ][MMnote] = 127 % melodyMatrix[i][j][MMnote] ; break;
						// 	case 2 : melodyMatrix[i][j ][MMnote] = 126; break;
						// 	case 3 : melodyMatrix[i][j ][MMnote] = 0; break;
						// 	case 4 : melodyMatrix[i][j ][MMnote] = 127-(127 % melodyMatrix[i][j ][MMnote] ); break;
						// }
						// WRAP
						// LIMIT
						// DELETE
						// BOUNCE
					};

					if (tempNote.note == 0 && tempNote.type != 0x50) // n'est pas ACCENT
					{
						// DELETE ALL THE REST
						// ****************** REMPLACER PAR LOOP, car tout vaut 0
						putNoteBinNoteA(tempNote, emptyNote);
						tempNote.killnote = true;
					};

					// ***** PAS SUR QUE SOIT PERTINENT POUR VELOCITE ! à ESSAYER !!!!

					if (tempNote.velocity > 127)
					{
						tempNote.velocity = 126;
						break;
						// switch(potVal3) {
						// 	case 1 : melodyMatrix[i][j ][MMvelocite] = 127 % melodyMatrix[i][j ][MMvelocite] ; break;
						// 	case 2 : melodyMatrix[i][j ][MMvelocite] = 126; break;
						// 	case 3 : melodyMatrix[i][j ][MMvelocite] = 0; break;
						// 	case 4 : melodyMatrix[i][j ][MMvelocite] = 127- (127%melodyMatrix[i][j ][MMvelocite] ); break;
						// }
					};

					if (tempNote.velocity == 0)
					{
						// DELETE ALL THE REST
						// DELETE ALL THE REST
						// ****************** REMPLACER PAR LOOP, car tout vaut 0
						putNoteBinNoteA(tempNote, emptyNote);
						tempNote.killnote = true;
					};

					//melodyMatrix[i][j ][MMdata] =  ;
					// ALLER REVOIR CE QUI S'Y trouve

					//melodyMatrix[i][j ][MMdata2] =   ;
					// ALLER REVOIR CE QUI S'Y trouve

					if ((unsigned long)tempNote.startNote > layerLength[i])
					{
						switch (potVal3)
						{
						case 1:
							tempNote.startNote = layerLength[i] % tempNote.startNote;
							break; // WRAP
						case 2:
							putNoteBinNoteA(tempNote, emptyNote);
							tempNote.killnote = true;
							break; // DELETE
						case 3:
							tempNote.startNote = layerLength[i] - 10;
							break; // LIMIT
						case 4:
							tempNote.startNote = layerLength[i] - (layerLength[i] % tempNote.startNote);
							break; // BOUNCE
						}
					};

					if ((unsigned long)tempNote.endNote > layerLength[i])
					{
						switch (potVal3)
						{
						case 1:
							tempNote.endNote = layerLength[i] % tempNote.endNote;
							break; // WRAP
						case 2:
							putNoteBinNoteA(tempNote, emptyNote);
							tempNote.killnote = true;
							break; // DELETE
						case 3:
							tempNote.endNote = layerLength[i] - 10;
							break; // LIMIT
						case 4:
							tempNote.endNote = layerLength[i] - (layerLength[i] % tempNote.endNote);
							break; // BOUNCE
						}

						// ****  IL FAUT SCINDER LE CLEAN UP PITCH de DUREE !
					};

					ZEROSetNoteInPile(pInitial[i], j, tempNote);
				}
			}
		}
	}

	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 115: //  PLAY PARALLEL MIDI FileExists
		// va jouer le midi en même temps que le reste tourne !

		{
			playParallelMidiON = !playParallelMidiON;

			if (playParallelMidiON == true)
			{
				// initialise
				// midiFile_totalNotes = 0 ;
				midiFile_progress = 0;

				// ouvrir le fichier

				// rotKnobRight.write(500); // valeur approximativement OK pour le départ

				totalDureeDelta = 0;
				unknownRunningStatus = 0;
				calculatedBPM = 120; // par défaut si n'est pas spécifié

				myFile.close();
				myFile = SDcard.open(midiname, FILE_READ);
				if (myFile.isOpen() == false)
				{
					oledWriteStringAtXY("SELECT MIDI FILE", 0, 0);
					playParallelMidiON = false;
					myFile.close();
				}
				else
				{
					// NOTE était midiname
					//
					// OPEN the HEader

					verbosePrintln("OPEN HEADER 4 = 4D546864?");
					readfromFile(4);
					verbosePrintln("OPEN HEADER next 4");
					readfromFile(4);
					// threadlen = mididouble;
					verbosePrintln("OPEN HEADER 2");
					readfromFile(2);
					verbosePrintln("OPEN HEADER 2");
					readfromFile(2);
					verbosePrintln("OPEN HEADER 2");
					readfromFile(2);
					// delay(verboseDelay);

					// timedivision = ( mididouble >> 15 );
					// if ( timedivision == 0 ) {
					//     // NbTicksQuarterNote = mididouble ;
					//     //value1000ticks = long ( (float) 6000000 / ( (float) 4 * (float) 12 * (float) mididouble ) );
					// }

					// readfromFile(threadlen - 6);
					CurrentMIDIChunk = 0;
					verbosePrintln("currentMIDIChunk");
					verbosePrintln(String(CurrentMIDIChunk));

					EndofTrack = false;
					MUSTreadHeader = true;
				}
			}
			else
			{
				// fermer le fichier
				myFile.close();
			}
		}
		break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 116:
	{ // TRANSFO SYMETRIC

		for (byte i = 0; i < nbLayer; i++)
		{

			byte haut = FindHighestNote(i);
			byte bas = FindLowestNote(i);

			if (isLayerActive(i))
			{
				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{
					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					switch (potVal2)
					{

					case 1:
					{ // IDEM - BOUNCE
						if ((j % 2) != 0)
						{
							// modifyPitchSET(tempNote, haut - (tempNote.note - bas)) ;
							tempNote.note = modifyPitchUniversal(tempNote, haut - (tempNote.note - bas), 0);
							//melodyMatrix[i][j][MMvelocite] = 0;
							// melodyMatrix[i][j][MMdata] = 0 ;
							// melodyMatrix[i][j][MMdata2] = 0 ;
						}
					}
					break;

					case 2:
					{ // BOUNCE ALL ou BOUNCE BOUNCE

						tempNote.note = modifyPitchUniversal(tempNote, haut - (tempNote.note - bas), 0);
						//melodyMatrix[i][j][MMvelocite] = 0;
						// melodyMatrix[i][j][MMdata] = 0 ;
						// melodyMatrix[i][j][MMdata2] = 0 ;
					}
					break;
					default:
					{
					}
					break;
					}

					ZEROSetNoteInPile(pInitial[i], j, tempNote);
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94  KeepDuree était incomplet !
		/////////////////////////////////////////////////////////////////////////////////////

	case 117:
	{ // PLUSMIN 100

		// on va augmenter ou réduire un nombre de millisecondes

		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				oldlayerLength = layerLength[i];

				layerLength[i] = layerLength[i] + potVal2;

				if (SqueezeOn == true)
				{
					unsigned int taille = mesureTaillePileComplete(pInitial[i]);

					for (unsigned int j = 0; j < taille; j++)
					{

						pileNote tempNote;
						tempNote = ZEROgetNoteFromPile(pInitial[i], j);

						if (KeepLength == true)
						{
							// KeepDuree = tempNote.endNote - melodyMatrixTime[i][j][MMTstartnote];
							KeepDuree = getDuree(tempNote);
						}

						// melodyMatrix[i][j][MMdata] = 1;   // çàd les 4 premiers bits = le channel   les 4 suivants sont les données pour chord : 0 = solo
						// melodyMatrix[i][j][MMdata2] = 0;    // le bit played est 0 et le bit silented est 0 ;

						tempNote.startNote = (unsigned long)((float)tempNote.startNote * (float)layerLength[i] / (float)oldlayerLength);

						if (KeepLength == true)
						{
							tempNote.endNote = tempNote.startNote + KeepDuree;
						}
						else
						{
							tempNote.endNote = (unsigned long)((float)tempNote.endNote * (float)layerLength[i] / (float)oldlayerLength);
						}

						pileNote_fixStartEndNote(tempNote, 0);
						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		// corrigé en BLACK08 pour les int et long
		/////////////////////////////////////////////////////////////////////////////////////

	case 118:
	{ // GENERATE BPM

		// on va déplacer entre 1  etv100%
		// **** ces lignes très répétitives pourraient faire l'objet d'une fonction !

		// potVal2 a une valeur de BPM
		// on transforme ce BPM en durée en msec
		// 1 BPM = 1 beat toutes les 60 sec = 60 000 msec
		// 2 BPM = 1 beat toutes les 30 sec = 30 000 msec
		// n BPM = 1 beat toutes les 60/n sec = 60 000 / n msec

		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				oldlayerLength = layerLength[i];

				layerLength[i] = (unsigned long)((float)60000.0 / (float)potVal2);

				// à ce moment-ci se produit parfois un grand BANG car il joue soudain toutes les notes en retard !!!
				// ********************************** IL FAUT CHANGER LES BITS JOUES OU PAS JOUES !

				/* si SqueezeOn, il ne suffit pas de modifier la durée, il faut modifier les valeurs de start et end des notes
			selon un ratio correspondant à nouvelletaille / anciennetaille
			si KeepLength est ON, il faut modifier uniquement le Start selon ce ratio
			et calculer un nouveau End correspondant à nouveauStart + ( ancienEnd-ancienStart )
			*/

				if (SqueezeOn == true)
				{
					unsigned int taille = mesureTaillePileComplete(pInitial[i]);

					for (unsigned int j = 0; j < taille; j++)
					{

						pileNote tempNote;
						tempNote = ZEROgetNoteFromPile(pInitial[i], j);

						if (KeepLength == true)
						{
							KeepDuree = getDuree(tempNote);
						}
						else
						{
							KeepDuree = 0;
						}

						tempNote.startNote = (unsigned long)((float)tempNote.startNote * (float)layerLength[i] / (float)oldlayerLength);

						if (KeepLength == true)
						{
							tempNote.endNote = tempNote.startNote + KeepDuree;
						}
						else
						{
							tempNote.endNote = (unsigned long)((float)tempNote.endNote * (float)layerLength[i] / (float)oldlayerLength);
						}

						pileNote_fixStartEndNote(tempNote, 0);
						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 119:
	{ // PHASE 1/n

		// potVal3 = BBB ;     //

		// on va phaser d'une fraction de la durée
		// si n = 2 : on phase d'un demi
		// si n = 3 : on phase d'un tiers,
		// etc...
		// on va faire une CleanUp auparavant pour s'assurer qu'on a rien qui traîne derrière, qui serait phasé vers le début
		// d'abord un simple Phasage qui décale vers la droite d' 1/n
		// **** NOTE plus tard on pourra voir pour des k/n, y compris -2/n, -1/n, etc.
		// suffit de mettre un Set en 120, et de faire répéter le phasage plusieurs fois

		// **** NOTE Transformation (114,10 );

		// **** NOTE ici on doit développer une fonction inverse à partir des valeurs du tableau Transfo :
		// je dois savoir quel BBB envoyer à Transformation 114 pour qu'il donne la valeur de BBB qui crée PotVal3 = DELETE !

		// attention pour le phasage vers la gauche : k négatif, mais les temps sont unsigned int,
		// donc on peut pas jouer avec du négatif !!!

		// 1C88 nouvelle version
		// potVal3 doit être déduit de Denominateur de potVal3, avec signe
		// kPhasage doit être déduit de Numérateur de potVal3, avec signe

		// kPhasage = extractNumRatioPlusMinus(potVal3, 32, 32);
		kPhasage = potVal2;
		//		potVal3 = extractDenRatioPlusMinus(potVal3, 32, 32);
		potVal3 = nPhasage;

		// String affiche = nulString ;
		// affiche = "REEL : " + String(kPhasage) + " / " + String(potVal3 ) ;
		// printRot2(affiche  ,16,400,3);
		// delay(1000) ;

		signed int Phasage;

		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				Phasage = layerLength[i] / potVal3;

				// **** NOTE  intégrer ici travail de kPhasage : si + : faire répéter ; si - plus complexe !!!

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{

					// melodyMatrixTime[i][j][MMTstartnote] += ( kPhasage * Phasage ) ;
					// melodyMatrixTime[i][j][MMTendnote] += ( kPhasage * Phasage )  ;
					// fixStartEndNote(i,j,0);
					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					long temp;
					temp = (long)tempNote.startNote + ((long)kPhasage * (long)Phasage);
					tempNote.startNote = (int)temp;

					temp = (long)tempNote.endNote + ((long)kPhasage * (long)Phasage);
					tempNote.endNote = (int)temp;

					pileNote_fixStartEndNote(tempNote, 0);
					ZEROSetNoteInPile(pInitial[i], j, tempNote);

					// if ( melodyMatrixTime[i][j][MMTstartnote]  > layerLength[i]) {
					// 	melodyMatrixTime[i][j][MMTstartnote]=  melodyMatrixTime[i][j][MMTstartnote] % layerLength[i]   ;    // WRAP
					// } ;
					//
					// if ( melodyMatrixTime[i][j][MMTendnote]  > layerLength[i]) {
					// 	melodyMatrixTime[i][j][MMTendnote]= melodyMatrixTime[i][j ][MMTendnote] % layerLength[i]   ;   // WRAP
					// }
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 120:
	{ // DEFINE k pour le phasage
		// potVal3= BBB ;     // 1 à 32
		nPhasage = potVal3;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 121:
	{ // ROTATION de mélodie
		// potVal3 = BBB ;     // 1 à 32

		point Note2D, NoteEnd2D;

		// d'abord on va calculer un point Centre
		// Centre.x vaudra le % entre 0 et 100% de LayerLength.   initalement : 50%, transformé en millisecondes
		// Centre.y vaudra la note en pitch, entre 0 et 127, mais avec un mapping tel que 127 équivaut à LayerLength
		// initialement Centre.y vaudra 60
		// le rapport PitchLayer vaut LayerLength / 127

		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				Centre.x = (float)layerLength[i] * (float)CentreX / (float)100.0;
				RapportPitchLayer = (float)layerLength[i] / (float)127.0;
				Centre.y = (float)CentreY * (float)RapportPitchLayer;

				// ****NOTE  pas toujours exact pour les notes dont la fin précède le début
				// il faut prévoir une fonction qui calcule cela toujours bien, çàd tenant compte de LayerLength !

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{ // va parcourir la mélodie pointée ( Layer 1 à 4 : potVal3 )
					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					if (tempNote.note != 0)
					{
						unsigned long duree_note = getDuree(tempNote);

						Note2D.x = (float)tempNote.startNote;
						Note2D.y = (float)tempNote.note * (float)RapportPitchLayer;

						NoteEnd2D.x = (float)tempNote.endNote;
						NoteEnd2D.y = (float)tempNote.note * (float)RapportPitchLayer;

						Note2D = Rotate(Centre, Note2D, potVal3);
						NoteEnd2D = Rotate(Centre, Note2D, potVal3);
						// NOTE problème quand la mélodie subit rotation de telle façon que le start passe après le end !!!
						//
						tempNote.startNote = (unsigned long)Note2D.x;
						tempNote.endNote = (unsigned long)NoteEnd2D.x;
						// modifyPitchSET(tempNote, (byte)noteWrapper(((float)Note2D.y / (float)RapportPitchLayer)) ) ;
						tempNote.note = modifyPitchUniversal(tempNote, (byte)noteWrapper(((float)Note2D.y / (float)RapportPitchLayer)));
						// NOTE il faut prévoir quoi faire pour les dépassements vers le haut ou le bas : delete ? wrap ?  bounce ?
						// créer fonction  NoteWrapper ( 0 - 127 ) d'office

						// melodyMatrix[i][j][MMdata] = 0 ;
						// melodyMatrix[i][j][MMdata2] = 0;
						// bit 0 et 1 sont 1 = played and silented pour que la note ne soit pas rejouée avant le tour suivant.
						// bit 2 sont 1 car c'est une note qui vient d'être créé, elle ne doit pas être re-ping-pong-ée

						if (dontRotateLength)
						{
							tempNote.endNote = tempNote.startNote + duree_note;
						}

						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// PILE VERSION : faite et vérifiée
		/////////////////////////////////////////////////////////////////////////////////////

	case 122:
	{
		CentreX = potVal3;
	}
	break;

		// 122 SET x. time (%)   // NOTE**** à compléter

		/////////////////////////////////////////////////////////////////////////////////////
		// PILE VERSION : faite et vérifiée
		/////////////////////////////////////////////////////////////////////////////////////

	case 123:
	{
		CentreY = potVal3;
	}
	break;

		// 123 SET y. pitch

		/////////////////////////////////////////////////////////////////////////////////////
		// PILE VERSION : faite et vérifiée
		/////////////////////////////////////////////////////////////////////////////////////

	case 124:
	{
	}
	break;

		// 124 GIVE current center ( %, pitch)   // Plutôt à mettre dans l'affichage, non ? Rotate(x,y) NOTE****

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C93   e
		/////////////////////////////////////////////////////////////////////////////////////

	case 125:
	{ // Activate Event
		Evenement[potVal2].actif = !Evenement[potVal2].actif;
		CurrentEvenement = potVal2;
		// byte i ;

		// potVal3 = map(BBB, 0, 1023, Transfo[LineCallingTransfo(120)][1] , Transfo[LineCallingTransfo(120)][2]);   // 1 à 32

		// exemple : je veux envoyer vers Strymon Big Sky
		//
		// HALL - Size 40 0-1

		// Serial3.write(0xB0); // CC Channel 1 = Midi 0
		//        Serial3.write(40);
		//        Serial3.write(random(0,1+1));

		//   // Decay 17 0-127
		//
		//   Serial3.write(0xB0); // CC Channel 1 = Midi 0
		//          Serial3.write(17);
		//          Serial3.write(random(0,127+1));

		// SELECT CC ( 40 CH0 ) ( RND (0,127) ) ( 40,45,34 CH0 )
		// on doit d'abord faire simple pour débugger : 1 CC, 1 Channel !
		// ce qui doit varier en priorité c'est la valeur envoyée pour le CC !

		// il faut une fonction trouvant le premier Evenement disponible -> i
		// transporte en [i] ce qui se trouve en [0] et change CurrentEvenement en 0

		// byte i = firstfreeEvenement();

		// Evenement[i].type = 1 ; // 0 = vide  1=CC 2=Transfo
		// // pour les Transfo modifiant valeur, elle devront voir le type défini
		// Evenement[i].intervalle  = 150  ; // 1 = 0.1sec
		// Evenement[i].moment = 50 ; // dans les temps divisible par 300, agit après 100
		// Evenement[i].fidelite  = 100  ; // %
		// Evenement[i].which  =  17 ; // définit quel CC va être modifié
		// Evenement[i].valeur  = 55  ; // définit la valeur initiale qui lui sera attribuée
		// Evenement[i].shape   = 4  ; // 1=fixe 2=inc 3=Dec 4=rnd 5=liste
		// Evenement[i].rangeUP  = 100  ;
		// Evenement[i].rangeLO  = 10  ;
		// Evenement[i].incSize  = 0  ;
		// Evenement[i].behaviorLimit  = 0  ;
		// Evenement[i].channel  = 5 ; // à supposer que la machine recevant le CC soit en midi 5 (CH6)
		// Evenement[i].layer  = 0  ;   // tous layers actives - non pertinent pour un CC, car ne modifie pas de layers !
		// Evenement[i].eventGOON = millis() - ( millis() % Evenement[i].intervalle ) + Evenement[i].intervalle + Evenement[i].moment ;

		buffer_and_HARD_setCursor(0, 1);
		buffer_and_HARD_print(Evenement[CurrentEvenement].eventGOON);

		// CurrentEvenement = 0 ; // ça veut dire qu'on travaille sur du neuf.
	}
	break;

		// case 243:  // activate ou inactivate Sequence
		// {
		// 	//potVal2 = BBB ;
		// 	sequence[potVal2].actif = !sequence[potVal2].actif ;
		// }
		// break;

		// NOTE **** une transformation doit lancer les transformations modifiant les valeurs de Transfo préparant EVENT
		// puis lancer un event Create Event.
		// il se peut qu'une de ces transfo soit de modifier une valeur de Transfor préparant EVENT
		// la machine enclenche ainsi un cycle d'autoprogrammation non prévisible

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C93   e
		/////////////////////////////////////////////////////////////////////////////////////

	case 126:
	{ // DEFINE which
		if (Evenement[CurrentEvenement].type == 1)
		{
			//potVal3 = BBB ;
			Evenement[CurrentEvenement].which = potVal3; // choix d'un CC
		}

		if (Evenement[CurrentEvenement].type == 2)
		{
			//potVal3 = BBB;
			Evenement[CurrentEvenement].which = Transfo[potVal3][0]; // choix d'une transformation
		}

		//  int which ;  // si type 1 : quel CC
		// si type 2 : quelle transformation
		// si type 3 : quelle valeur ( comment définir ? liste dans tableau, ou via transformation ? paraît mieux )

		// NOTE**** si Type 1 : définit le CC
		// Evenement[CurrentEvenement].which  = potVal3 ;
		// si un CurrentEvenement est défini ( != 0) alors on modifie cet évènement
		// si il est indéfini, on modifie une valeur, qui ne sera créée que quand on lance l'évèneement : la valeur .type devient alors != 0
		// et l'évènement est déplacé vers une autre valeur dans Evenemnt[i] ?

		// NOTE**** si Type 2 : définit la transformation
		//  l'affichage doit montrer les noms des transformations sélectionnées
		//  1---- SELECT TRANSFO
		//  ROTATION                  <- name.Transfo(BBB)
		//

		// C'est l'affichage ici qui doit jouer !
		// // au début garder un affichage de N° de Transformation
		// il faut en effet d'abord intégrer le PROGMEM avec les noms de Transformations
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C93   e
		/////////////////////////////////////////////////////////////////////////////////////

	case 127:
	{ // DEFINE Evenement[i]
		// potVal3 = BBB ;     // 1 à NbEvenementMax
		CurrentEvenement = potVal3;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C93   e
		/////////////////////////////////////////////////////////////////////////////////////

	case 128:
	{
		// 128 DELETE Event : initialise un event - à utiliser en début de programme pour tout réinitialiser
		// byte i = CurrentEvenement ; // NOTE**** il faudra peut être modifier ceci si on veut choisir quel evt détruire
		EmptyEvenement(potVal2);
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C93   e
		/////////////////////////////////////////////////////////////////////////////////////

	case 129:
	{
		// 129 DEFINE Type
		// byte type ; // 0 = Vide ( il faut initialiser ! )   1 = CC
		// 2 = Transformation
		// (3) = Modification de valeur ( peut être intégrer à transformation ! )
		//potVal3 = BBB ;     // 1 à NbEvenementMax
		Evenement[CurrentEvenement].type = potVal3;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C93   e
		/////////////////////////////////////////////////////////////////////////////////////

	case 130:
	{
		// 130 DEFINE Intervalle
		// unsigned int intervalle ; // exprimé en 0.1 secondes  10 = 1 seconde 100 = 10 secondes 1000 = 100 secondes 10000 = quasi 1 h
		//potVal3 = BBB ;     // 1 à NbEvenementMax
		Evenement[CurrentEvenement].intervalle = potVal3; // **** NOTE ceci est modifié ici et dans l'afficahge de 131 : à confirmer
		Transfo[LineCallingTransfo(131)][2] = potVal3;	  // on limite la durée possible du moment en fc de l'intervalle
														  // DEBUG TODO : pas beau cette programmation !
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C93   e
		/////////////////////////////////////////////////////////////////////////////////////

	case 131:
	{
		// 131 DEFINE Moment, durant l'intervalle
		// unsigned int moment ; // // dans les temps divisible par 300, agit après 10010000 = quasi 1 h
		//potVal3 = BBB ;     // 1 à NbEvenementMax
		Evenement[CurrentEvenement].moment = potVal3;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C93   e
		/////////////////////////////////////////////////////////////////////////////////////

	case 132:
	{
		// 132 DEFINE fidelite
		// byte fidelite ; // en %
		//potVal3 = BBB ;     // 1 à NbEvenementMax
		Evenement[CurrentEvenement].fidelite = potVal3;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C93   e
		/////////////////////////////////////////////////////////////////////////////////////

	case 133:
	{
		// 133 DEFINE TypeInterval
		// byte typeInterval ; // 1 = constant 2 = random(a,b)  3=gaussian   4 = bi intervalle
		// NOTE **** à implémenter
		// Si 1 : OK pour .intervalle et .moment
		// Si 2 : valeur Min, valeur Max : Valeur Max = Intervalle , Valeur Min = Moment
		// Si 3 : Valeur Min = distrib 0 ; Valeur max = distrib totale
		// Si 4 : bof
		//
		//potVal3 = BBB ;     // 1 à NbEvenementMax
		Evenement[CurrentEvenement].typeInterval = potVal3;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C93   e
		/////////////////////////////////////////////////////////////////////////////////////

	case 134:
		// 134 DEFINE Value
		// int valeur ;
		// pour CC : 0-127
		// pour Transfo : 0-1023, affichée comme pour l'affichage normal
		//  1---- SELECT VALUE FOR TRANSFO(42)=Rotation
		//  30°               <- affichage comme pour Rotation
		{
			if (Evenement[CurrentEvenement].type == 1)
			{
				//potVal3 = BBB ;     // 1 à 127
				Evenement[CurrentEvenement].valeur = potVal3;
			}

			if (Evenement[CurrentEvenement].type == 2)
			{

				//potVal2 = BBB ;

				Evenement[CurrentEvenement].valeur = potVal3;
			}
		}
		break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C93   e
		/////////////////////////////////////////////////////////////////////////////////////

	case 135:
	{
		// 135 DEFINE SHAPE
		// byte shape ;  // 1 = valeur fixe ( pour valeur )
		// 2 = INC
		// 3 = DEC ( ou bien INC avec valeur négative ? )
		// 4 = RND  ( définir min et max, 0-127 par défaut pour CC )
		// 5 = LISTE : ex 4 valeurs 37,69,120,130
		//  5=liste sequential 6 = liste random
		// // NOTE**** il faudra pour la liste réapprendre les pointeurs; j'en aurai de toutes façons besoins pour passer
		// des fragments (bars) à des morceaux.
		//
		//potVal3 = BBB ;     // 1 à NbEvenementMax
		Evenement[CurrentEvenement].shape = potVal3;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C93   e
		/////////////////////////////////////////////////////////////////////////////////////

	case 136:
	{
		// 136 DEFINE RangeUP
		// byte rangeUP ; // pour option 2,3,4
		//potVal3 = BBB ;    // 1 à 127
		Evenement[CurrentEvenement].rangeUP_ = potVal3;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C93   e
		/////////////////////////////////////////////////////////////////////////////////////

	case 137:
	{
		// 137 DEFINE RangeLO
		// byte rangeLO ; // pour option 2,3,4
		//potVal3 = BBB ;    // 1 à NbEvenementMax
		// 1 à NbEvenementMax
		Evenement[CurrentEvenement].rangeLO_ = potVal3;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C93   e
		/////////////////////////////////////////////////////////////////////////////////////

	case 138:
	{
		// 138 DEFINE incSize
		// byte incSize ; // définit de combien la valeur en option 2 et 3 ( qui donnent le sens)
		//potVal3 = BBB ;    // 1 à NbEvenementMax
		Evenement[CurrentEvenement].incSize = potVal3;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C93   e
		/////////////////////////////////////////////////////////////////////////////////////

	case 139:
	{
		// 139 DEFINE behaviorLimit
		// byte behaviorLimit ; // pour options shape 2,3
		// 1 = Change direction
		// 2 = qd dépasse vers haut ou bas, réapparait de l'autre côté
		// 3 = reste à la limite
		//potVal3 = BBB ;     // 1 à NbEvenementMax
		// NOTE **** Franchement on peut pas sortir cette ligne du Switch Case ?  elle se répète 100 fois !
		Evenement[CurrentEvenement].behaviorLimit = potVal3;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C93   e
		/////////////////////////////////////////////////////////////////////////////////////

	case 140:
	{
		// 140 DEFINE channel
		// byte channel ; // channel concerné 0-15
		// Définit sur quel channel on envoie le CC !
		// NOTE **** doit on envoyer un CC sur des channels différents ?
		// il faut alors créer 2 event avec un channel différent plutôt !

		//potVal3 = BBB ;    // 1 à NbEvenementMax
		Evenement[CurrentEvenement].channel = potVal3 - 1;
		// CAR affichage est USER et encodage est MACHINE
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C93   e
		/////////////////////////////////////////////////////////////////////////////////////

	case 141:
	{
		// 141 DEFINE Layer
		// byte layer ; // layer concerné 1-4 ou combinaison ou random ; ou bien layers actives.   par défaut : Actif ; sinon spécifique
		// définit sur quel layer va agir
		// // B11110000 signifie : agir sur les Active Layers
		// // NOTE **** pas OK pour le Active Layer !!!!!!
		//
		//potVal3 = BBB ;    // 1 à NbEvenementMax
		Evenement[CurrentEvenement].layer = potVal3;
		// si la valeur est 0 = 0000 ; 15 = 1111 ; 16 = ACTIVE LAYERS
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C93   e
		/////////////////////////////////////////////////////////////////////////////////////

	case 142:
	{
		iVLVDelta = BBB;
	}
	break;
		// long iVLVDelta = 0;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C93   e
		/////////////////////////////////////////////////////////////////////////////////////

	case 143:
	{
		iDureeDelta = BBB;
	}
	break;
		// long iDureeDelta = 0;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C93   e
		/////////////////////////////////////////////////////////////////////////////////////

	case 144:
	{
		imididouble = BBB;
	}
	break;
		// long imididouble = 0;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C93   e
		/////////////////////////////////////////////////////////////////////////////////////

	case 145:
	{
		iShiftDelta = BBB;
	}
	break;
		// byte iShiftDelta = 8 ;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C93   e
		/////////////////////////////////////////////////////////////////////////////////////

	case 146:
	{
		iShiftDuree = BBB;
	}
	break;
		// byte iShiftDuree = 7 ;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C93   e
		/////////////////////////////////////////////////////////////////////////////////////

	case 147:
	{
		iDureeMask = BBB;
	}
	break;
		// byte iDureeMask = B01111111   ;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C93   e
		/////////////////////////////////////////////////////////////////////////////////////

	case 148:
	{
		iMask2 = BBB;
	}
	break;
		// byte  iMask2 = B10000000 ;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C93   e
		/////////////////////////////////////////////////////////////////////////////////////

	case 149:
	{
		CutOffDureeDelta = BBB;
	}
	break;
		// int CutOffDureeDelta = 1000 ; // NOTE **** devra peut-être être changée pour ne pas avoir de cutoff qd on veut perfection

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C93   e
		/////////////////////////////////////////////////////////////////////////////////////

	case 150:
	{
		BringToDureeDelta = BBB;
	}
	break;
		// int BringToDureeDelta = 0 ;

		// float RatioDeltaToMsec = 4.0 ;

	case 151:
	{
		NumRatioDeltaToMsec = BBB;
		// RatioDeltaToMsec = (float) NumRatioDeltaToMsec / (float) DenomRatioDeltaToMsec ;

		// 2C28 NumRatioDeltaToMsec = extractNumRatio(potVal2, 10, 10);
		// DenomRatioDeltaToMsec = extractDenRatio(potVal2, 10, 10);

		// RatioDeltaToMsec = (float)NumRatioDeltaToMsec / (float)DenomRatioDeltaToMsec;
	}
	break;
		// byte NumRatioDeltaToMsec = 4 ;

	case 152:
	{
		DenomRatioDeltaToMsec = BBB;

	// 	RatioDeltaToMsec = (float)NumRatioDeltaToMsec / (float)DenomRatioDeltaToMsec;
	}
	break;
		// byte DenomRatioDeltaToMsec = 0 ;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C93   e
		/////////////////////////////////////////////////////////////////////////////////////

	case 153:
	{ // va rajouter une lettre ou en retrancher une à un string
		// il faudra isoler ce qui va suivre pour en faire une fc générale
		// le string est affiché dans le programme,
		// une lettre défile avec le bouton ROTATif
		// qd PUSH, si c'est lettre normale : elle est ajouté au string affiché
		// qd PUSH si c'est le caractère "<" et que taille > 1 alors on coupe le dernier morceau du string

		// potVal2 = BBB ;

		if (ChoixString153.substring(potVal2, potVal2 + 1) != "<")
		{

			String153 = String153 + ChoixString153.substring(potVal2, potVal2 + 1);
		}
		else
		{
			if (String153.length() > 0)
			{
				String153 = String153.substring(0, String153.length() - 1);
			}
		}

		// NOTE**** trouver une fc réutilisable, du style : ProcessString (  &String153, &ChoixString153 )

		// NOTE DEBUG si on veut un Easter Egg c'est ici qu'il faut le mettre

		if (String153 == "liv")
		{
			printRot2(F("EASTER EGG"), 16, 300, 3);
			delay(100000);
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// VERSION PILE NOTE
		// 1C93 creation d'une fonction TreatNote alternative : TreatNote_PileNote
		/////////////////////////////////////////////////////////////////////////////////////

	case 154:
	{ // Swing %

		pcAB = BBB; // cette valeur est utilisée dans la trasnformation

		treatNoteTime = nowMillis; // protection contre récursion infinie : qd la différence avec nowMillis est de plus d'1sec,
		// ça s'arrête

		for (byte i = 0; i < nbLayer; i++)
		{

			unsigned long lgAB = layerLength[i] / nSwing;

			if (isLayerActive(i))
			{

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{

					TreatNote_pileNote(i, j, lgAB);
					// ceci est sous-traité car il y a une forme de récursivité qui doit pouvoir apparaître
					// dans les cas tels que Ixx : les notes sont renvoyées à plusieurs reprises au bloc antérieur
					//

					//           KeepDuree =  melodyMatrixTime[i][j][MMTendnote] - melodyMatrixTime[i][j][MMTstartnote] ;
					//           iAB = melodyMatrixTime[i][j][MMTstartnote] / lgAB   ;
					//
					//           // si iAB est pair, on ne fait rien
					//           // si iAB est impair :
					//           // les notes sont amenées à temps(iAB+1)-tempsiAB * pcAB / 100, ajouté à tempsiAB
					//
					//           // NOTE **** ajouter des options pour choisir si
					//           // odd, even
					//           // si l'even doit être aligné ou laissé tel quel
					//
					//           //         if ( isOdd(iAB) ) {
					//           //
					//           //           tempA = iAB * lgAB  ;        // donne le temps dans la grille parfaite avant startnote
					//           //           tempB = (iAB + 1) * lgAB ; // donne le temps dans la grille parfaite après startnote
					//           //           float ftempC = ((float)tempB - (float)tempA) * (float)pcAB / (float)100.0 ;
					//           //           long tempC = (long) tempA + (long) ftempC ;
					//           //
					//           //           melodyMatrixTime[i][j][MMTstartnote] = tempC ;
					//           //           melodyMatrixTime[i][j][MMTendnote] = melodyMatrixTime[i][j][MMTstartnote] + KeepDuree   ; ;
					//           // }
					//
					//           // on a le iAB, càd le lieu dans le layer qui indique la division
					//           // on regarde dans le patternSwing, en fc du nSwing l'action à entreprendre
					//           // si -> : on décale ( déjà fait)
					//           // si <- : on décale, mais le % devient 100-%
					//           // si - : nada
					//           // si I : on met sur tempA
					//
					//           if ( patternSwing.substring(iAB % patternSwing.length(),iAB % patternSwing.length() + 1 )
					//           == flechedroite )
					//           {
					//             tempA = iAB * lgAB  ;        // donne le temps dans la grille parfaite avant startnote
					//             tempB = (iAB + 1) * lgAB ; // donne le temps dans la grille parfaite après startnote
					//             float ftempC = ((float)tempB - (float)tempA) * (float)pcAB / (float)100.0 ;
					//             long tempC = (long) tempA + (long) ftempC ;
					//
					//             melodyMatrixTime[i][j][MMTstartnote] = tempC ;
					//             melodyMatrixTime[i][j][MMTendnote] = melodyMatrixTime[i][j][MMTstartnote] + KeepDuree   ; ;
					//           }
					//
					//           if ( patternSwing.substring(iAB % patternSwing.length(),iAB % patternSwing.length() + 1 )
					//           == flechegauche )
					//           {
					//             pcAB = 100 - pcAB ;
					//             tempA = iAB * lgAB  ;        // donne le temps dans la grille parfaite avant startnote
					//             tempB = (iAB + 1) * lgAB ; // donne le temps dans la grille parfaite après startnote
					//             float ftempC = ((float)tempB - (float)tempA) * (float)pcAB / (float)100.0 ;
					//             long tempC = (long) tempA + (long) ftempC ;
					//
					//             melodyMatrixTime[i][j][MMTstartnote] = tempC ;
					//             melodyMatrixTime[i][j][MMTendnote] = melodyMatrixTime[i][j][MMTstartnote] + KeepDuree   ; ;
					//           }
					//
					//           if ( patternSwing.substring(iAB % patternSwing.length(),iAB % patternSwing.length() + 1 )
					//           == barremilieu )
					//           {
					//             tempA = iAB * lgAB  ;        // donne le temps dans la grille parfaite avant startnote
					//             melodyMatrixTime[i][j][MMTstartnote] = tempA ;
					//             melodyMatrixTime[i][j][MMTendnote] = melodyMatrixTime[i][j][MMTstartnote] + KeepDuree   ; ;
					//           }
					//
					//           if ( patternSwing.substring(iAB % patternSwing.length(),iAB % patternSwing.length() + 1 )
					//           == barreH )
					//           {
					//             tempB = (iAB + 1) * lgAB ; // donne le temps dans la grille parfaite après startnote
					//             melodyMatrixTime[i][j][MMTstartnote] = tempB ;
					//             melodyMatrixTime[i][j][MMTendnote] = melodyMatrixTime[i][j][MMTstartnote] + KeepDuree   ; ;
					//           }
					//
					//           if ( patternSwing.substring(iAB % patternSwing.length(),iAB % patternSwing.length() + 1 )
					//           == "x" )
					//           { // NOTE ***** A définir : va envoyer les notes vers le iAB précédent avec une valeur non "x"
					//           //   n = iAB
					//           //   si n = 0 : stop
					//           //   si n > 0 : m = n-1
					//           //   tester patternSwing.substring(iAB % patternSwing.length(),iAB % patternSwing.length() + 1 )
					//           //    si ="x"
					//           //      recursion
					//           //    sinon : on applique là la transfo exigée
					//
					//           int n = iAB ;
					//
					//           if ( n != 0 ) {
					// // d'abord on va déplacer la note à la même place dans la division précédente
					//
					//             melodyMatrixTime[i][j][MMTstartnote] -= lgAB ;
					//             melodyMatrixTime[i][j][MMTendnote] -= lgAB ;
					//
					//             TreatNote(i,j);
					//
					// // ensuite on doit revoir la situation pour cette note modifiée, par récursivité
					//
					//
					//           }
					//
					//
					//           }
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C93
		/////////////////////////////////////////////////////////////////////////////////////

	case 155:
	{ // define n Swing
		nSwing = potVal2;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C93
		/////////////////////////////////////////////////////////////////////////////////////

	case 156:
	{ // preset pattern Swing
		patternSwing = preset156[potVal2];
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// version Pile NOte
		// 1C93
		/////////////////////////////////////////////////////////////////////////////////////

	case 157:
	{ // Fuse

		unsigned int span157 = potVal2;
		// définit la distance entre 2 notes qui fait qu'une seule survit
		// case 0 :   buffer_and_HARD_print(F("1.Same Note"));  break ;
		// case 1 :   buffer_and_HARD_print(F("2.All Notes"));  break ;

		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{

					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					if (tempNote.note != 0)
					{

						for (byte l = (j + 1); l < taille; l++)
						{

							pileNote tempNote2;
							tempNote2 = ZEROgetNoteFromPile(pInitial[i], l);

							if (abs((int)tempNote.startNote - (int)tempNote2.startNote) < span157)
							{
								// effacer la note NOTE**** créer un fonction qui automatise effacer note (i,j)
								//
								if ((typeFuse == 0) && (tempNote.note == tempNote2.note))
								{
									tempNote2.note = 0;
									tempNote2.velocity = 0;
									tempNote2.startNote = 0;
									tempNote2.endNote = 0;
									ZEROSetNoteInPile(pInitial[i], l, tempNote2);
								}

								if ((typeFuse == 1))
								{
									tempNote2.note = 0;
									tempNote2.velocity = 0;
									tempNote2.startNote = 0;
									tempNote2.endNote = 0;
									ZEROSetNoteInPile(pInitial[i], l, tempNote2);
								}
							}
						}
					}
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C93
		/////////////////////////////////////////////////////////////////////////////////////

	case 158:
	{ // define type Fuse
		typeFuse = potVal2;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C93
		/////////////////////////////////////////////////////////////////////////////////////

	case 159:
	{ // va rajouter une lettre ou en retrancher une à un string
		// il faudra isoler ce qui va suivre pour en faire une fc générale
		// le string est affiché dans le programme,
		// une lettre défile avec le bouton ROTATif
		// qd PUSH, si c'est lettre normale : elle est ajouté au string affiché
		// qd PUSH si c'est le caractère "<" et que taille > 1 alors on coupe le dernier morceau du string

		//potVal2 = BBB ;

		if (ChoixString156.substring(potVal2, potVal2 + 1) != "<")
		{

			String159 = String159 + ChoixString156.substring(potVal2, potVal2 + 1);
		}
		else
		{
			if (String159.length() > 0)
			{
				String159 = String159.substring(0, String159.length() - 1);
			}
		}

		preset156[0] = String159;

		// NOTE**** trouver une fc réutilisable, du style : ProcessString (  &String153, &ChoixString153 )
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C93
		/////////////////////////////////////////////////////////////////////////////////////

	case 160:
	{ // CH MidiMatrix choose Source
		// l'essentiel : choisir une CH et l'envoyer sur un autre

		//potVal2 = BBB ;
		redirectMatrixAtoB(midiRedirect, potVal2 - 1, targetMidiMatrix - 1);
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C93
		/////////////////////////////////////////////////////////////////////////////////////

	case 161:
	{ // CH MidiMatrix choose Target
		// l'essentiel : choisir une CH et l'envoyer sur un autre

		targetMidiMatrix = potVal2;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C93
		/////////////////////////////////////////////////////////////////////////////////////

	case 162:
	{ // Fonctions spéciales pour le MidiMatrix
		switch (potVal2)
		{
		case 1:
			emptyMidiMatrixTotal(midiRedirect);
			break;
		case 2:
			backToSelfMatrixTotal(midiRedirect);
			break;
		case 3:
			randomizeMatrixTotal(midiRedirect);
			break;
		case 4:
			sendAlltoChannelN(midiRedirect, targetMidiMatrix - 1);
			break;
		case 5:
			sendChannelNtoAll(midiRedirect, targetMidiMatrix - 1);
			break;
		case 6:
			distributeAllto4Channel(midiRedirect);
			break;
		case 7:
			randomizePolyMatrixTotal(midiRedirect, targetMidiMatrix);
			break;
		default:
			break;
		}
	}

	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C93
		/////////////////////////////////////////////////////////////////////////////////////

	case 163: // Affichage de la ligne de redirection pour un CH dans MidiMatrix
	{
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C93
		/////////////////////////////////////////////////////////////////////////////////////

	case 164: // change la valeur de SkipChucnk
	{
		SkipChunck = !SkipChunck;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 165: // Lg Not Variable : définit la durée min des notes  ( pour éviter les 'ping' simples ou trop longs )

	{
		if (LGnotMax < potVal2)
		{
			int temp = potVal2;
			potVal2 = LGnotMax;
			LGnotMax = temp;
		}

		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{
					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					// melodyMatrixTime[i][j][MMTendnote] = melodyMatrixTime[i][j][MMTstartnote] + random(potVal2, LGnotMax);
					tempNote.endNote = tempNote.startNote + random(potVal2, LGnotMax);
					pileNote_fixStartEndNote(tempNote, 0);
					ZEROSetNoteInPile(pInitial[i], j, tempNote);
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 166: // Lg Not Variable : définit la durée max des notes  ( pour éviter les 'ping' simples ou trop longs )
	{
		LGnotMax = potVal2;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 167: // Reset MIDI Parallel values - utile qd après avoir bien déconné on veut rentrer dans le droit chemin
		// TODO DEBUG j'ai l'impression parfois que tout n'est pas réinitialisé ! des morceaux ne rejouent pas commme auparavant
	{
		iVLVDelta = 0;
		iDureeDelta = 0;
		imididouble = 0;
		iShiftDelta = 8;
		iShiftDuree = 7;
		iDureeMask = B01111111;
		iMask2 = B10000000;

		CutOffDureeDelta = 20000; // NOTE **** devra peut-être être changée pour ne pas avoir de cutoff qd on veut perfection
		BringToDureeDelta = 0;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 168: // Static LO cut des notes, one-shot
	{
		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{

					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					if (tempNote.note < potVal2)
					{
						tempNote.note = 0;
						tempNote.velocity = 0; // pour que ne soit pas trop parfait !
						tempNote.killnote = true;
						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 169: // Static HI cut des notes, one-shot
	{
		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{

					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					if (tempNote.note > potVal2)
					{
						tempNote.note = 0;
						tempNote.velocity = 0; // pour que ne soit pas trop parfait !
						tempNote.killnote = true;
						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 170: // Dynamic LO cut des notes, modifiable
	{
		dynamicLOcut = BBB;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 171: // Dynamic HI cut des notes, modifiable
	{
		dynamicHIcut = BBB;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 172: // SEND CC To TX81Z
	{
		sendCCtoTX81Z = !sendCCtoTX81Z;
	}
	break;

	case 173: // CUT --] end
	{
		unsigned long temp = nowMillis;

		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{
				layerLength[i] = temp % layerLength[i]; // réduit le layer au moment où cut final

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);
				pileNote tempNote;

				for (unsigned int j = 0; j < taille; j++)
				{
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					// melodyMatrix[i][j][MMnote] = 0 ;
					// melodyMatrix[i][j][MMvelocite] = 0 ;
					// melodyMatrix[i][j][MMdata] = 0;   // çàd les 4 premiers bits = le channel   les 4 suivants sont les données pour chord : 0 = solo
					// melodyMatrix[i][j][MMdata2] = 0;    // le bit played est 0 et le bit silented est 0 ;
					if ((unsigned long)tempNote.startNote > layerLength[i])
					{
						if ((unsigned long)tempNote.endNote < layerLength[i])
						{
							tempNote.startNote = 0;
							// c'est le cas des notes commençant en fin de layer et finissant au début
							pileNote_fixStartEndNote(tempNote);
							ZEROSetNoteInPile(pInitial[i], j, tempNote);
						}
						else
						{
							// on efface la note
							tempNote.note = 0;
							tempNote.velocity = 0;
							tempNote.startNote = 0;
							tempNote.endNote = 0;
							tempNote.killnote = true;
							pileNote_fixStartEndNote(tempNote);
							ZEROSetNoteInPile(pInitial[i], j, tempNote);
						}
					}

					if ((unsigned int)tempNote.endNote > layerLength[i])
					{
						tempNote.endNote = layerLength[i] - 10;
						pileNote_fixStartEndNote(tempNote);
						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
				}
			}
		}
	}
	break;

	case 174: // CUT [-- start
	{
		unsigned long temp = nowMillis;

		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				unsigned long redux = temp % layerLength[i];
				layerLength[i] = layerLength[i] - redux;
				// réduit le layer à la taille du layer initial moins la partie déjà jouée

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);
				pileNote tempNote;

				for (unsigned int j = 0; j < taille; j++)
				{
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					if ((long)((long)tempNote.startNote - (long)redux) < 0)

					{

						if ((long)((long)tempNote.endNote - (long)redux) < 0)
						{
							// on efface la note
							tempNote.note = 0;
							tempNote.velocity = 0;
							tempNote.startNote = 0;
							tempNote.endNote = 0;
							tempNote.killnote = true;
							pileNote_fixStartEndNote(tempNote);
							ZEROSetNoteInPile(pInitial[i], j, tempNote);
						}
						else
						{
							tempNote.startNote = 0;
							pileNote_fixStartEndNote(tempNote);
							ZEROSetNoteInPile(pInitial[i], j, tempNote);
						}
					}
					else
					{
						tempNote.startNote -= redux;
						pileNote_fixStartEndNote(tempNote);
						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}

					if ((long)(tempNote.endNote - redux) < 0)
					{
						tempNote.endNote = 0;
						pileNote_fixStartEndNote(tempNote);
						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
					else
					{
						tempNote.endNote -= redux;
						pileNote_fixStartEndNote(tempNote);
						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 175:
	{						 // SET Horizontal Map Target MAX
		mapHTargetMax = BBB; // 2 à 16
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 176:
	{						 // SET Horizontal Map Target MIN
		mapHTargetMin = BBB; // 2 à 16
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 177:
	{						 // SET Horizontal Map Source MAX
		mapHSourceMax = BBB; // 2 à 16
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 178:
	{						 // SET Horizontal Map Source MIN
		mapHSourceMin = BBB; // 2 à 16
	}
	break;

	case 179:
	{ // HORIZONTAL MAPPING - quite RAW !!!

		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);
				pileNote tempNote;

				for (unsigned int j = 0; j < taille; j++)
				{

					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					long tempTime = map((long)tempNote.startNote, (long)mapHSourceMin, (long)mapHSourceMax, (long)mapHTargetMin, (long)mapHTargetMax);

					if (tempTime < 0)
					{
						tempNote.startNote = 0 + random(1, 10);
					}
					else

						if (tempTime > (long)layerLength[i])
					{
						tempNote.startNote = layerLength[i] - random(1, 20);
					}
					else
					{
						tempNote.startNote = tempTime;
					}

					tempTime = map((long)tempNote.endNote, (long)mapHSourceMin, (long)mapHSourceMax, (long)mapHTargetMin, (long)mapHTargetMax);

					if (tempTime < 0)
					{
						tempNote.endNote = 0 + random(1, 10);
					}
					else

						if (tempTime > (long)layerLength[i])
					{
						tempNote.endNote = layerLength[i] - random(1, 20);
					}
					else
					{
						tempNote.endNote = tempTime;
					}

					pileNote_fixStartEndNote(tempNote, 0);
					ZEROSetNoteInPile(pInitial[i], j, tempNote);
				}
			}
		}
	}
	break;

	case 180:
	{					// BREATHE
		int temp = BBB; // 2 à 16

		// définit la distance entre 2 notes minimum
		// si la note est plus près, cette transfo la repousse de la valeur définie
		// case 0 :   buffer_and_HARD_print(F("1.repousse la note mais pas celles bien distantes"));  break ;
		// case 1 :   buffer_and_HARD_print(F("2.repousse la note et les autres d'emblée du même nombre"));  break ;
		// case 2 :   buffer_and_HARD_print(F("3. = 2 + layerlenght repousse la note mais pas celles bien distantes"));  break ;

		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				unsigned int taille = mesureTaillePileComplete(pInitial[i]);
				pileNote tempNote;

				for (unsigned int j = 0; j < taille; j++)
				{

					tempNote = ZEROgetNoteFromPile(pInitial[i], j);
					// 	ZEROSetNoteInPile(pInitial[i], j, tempNote);

					if (tempNote.note != 0)
					{
						pileNote tempNote2;

						for (byte l = (j + 1); l < taille; l++)
						{
							tempNote2 = ZEROgetNoteFromPile(pInitial[i], l);

							if (abs((long)tempNote.startNote - (long)tempNote2.startNote) < temp)
							{
								// espacer la note l
								//
								if (typeBreathe == 0)
								{
									unsigned long duree = getDuree(tempNote2);
									// melodyMatrixTime[i][l][MMTendnote] - melodyMatrixTime[i][l][MMTstartnote] ;
									// NOTE **** créer une fonction qui calcule durée d'une note car on peut aussi avoir end avant start
									tempNote2.startNote = tempNote.startNote + temp;
									tempNote2.endNote = tempNote2.startNote + duree;
									ZEROSetNoteInPile(pInitial[i], l, tempNote2);
								}

								if ((typeBreathe == 1))
								{
									int duree = getDuree(tempNote2);
									// int duree = melodyMatrixTime[i][l][MMTendnote] - melodyMatrixTime[i][l][MMTstartnote] ;
									int difference = duree - abs((long)tempNote.startNote - (long)tempNote2.startNote);
									int initial = tempNote2.startNote;
									tempNote2.startNote = tempNote.startNote + temp;
									tempNote2.endNote = tempNote2.startNote + duree;
									pileNote tempNote3;

									for (byte n = l; n < taille; n++)
									{
										tempNote3 = ZEROgetNoteFromPile(pInitial[i], n);

										if (abs((long)tempNote3.startNote > (long)initial))
										{
											tempNote3.startNote = tempNote3.startNote + difference;
											tempNote3.endNote = tempNote3.endNote + difference;
											ZEROSetNoteInPile(pInitial[i], n, tempNote3);
										}
									}
									ZEROSetNoteInPile(pInitial[i], l, tempNote2);
								}

								if ((typeBreathe == 2))
								{
									int duree = getDuree(tempNote2);
									// int duree = melodyMatrixTime[i][l][MMTendnote] - melodyMatrixTime[i][l][MMTstartnote] ;
									int difference = duree - abs((long)tempNote.startNote - (long)tempNote2.startNote);
									int initial = tempNote2.startNote;
									tempNote2.startNote = tempNote.startNote + temp;
									tempNote2.endNote = tempNote2.startNote + duree;
									layerLength[i] += difference;

									pileNote tempNote3;

									for (byte n = l; n < taille; n++)
									{
										tempNote3 = ZEROgetNoteFromPile(pInitial[i], n);

										if (abs((long)tempNote3.startNote > (long)initial))
										{
											tempNote3.startNote = tempNote3.startNote + difference;
											tempNote3.endNote = tempNote3.endNote + difference;
											layerLength[i] += difference;
											ZEROSetNoteInPile(pInitial[i], n, tempNote3);
										}
									}
									ZEROSetNoteInPile(pInitial[i], l, tempNote2);
								}
							}
						}
					}
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 181:
	{ // define type Breathe
		typeBreathe = potVal2;
		// typeBreathe = map(potVal2, 0, 1023, Transfo[LineCallingTransfo(TTT)][1] , Transfo[LineCallingTransfo(TTT)][2] ) ;
	}
	break;

	case 182: // Replicate Layer
	{
		// potVal3 = BBB ;    // on choisit de quelle layer on veut copier les infos, vers les layers actives

		unsigned int taille = mesureTaillePileComplete(pInitial[potVal3 - 1]);
		pileNote tempNote;

		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i) && (i != (potVal3 - 1)))
			{
				ZEROdeleteWholePile(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{
					tempNote = ZEROgetNoteFromPile(pInitial[potVal3 - 1], j);
					ZEROaddNewNotetoPile(pInitial[i], tempNote);
				}

				layerLength[i] = layerLength[potVal3 - 1];
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 183:
	{ // DISJOINT

		// on va déplacer entre 1  etv100%
		int temp(0), oldTemp(0);

		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{
				do
				{
					temp = random(0, potVal2);
				} while (temp == oldTemp); // on s'assure qu'on tire 2 nombres différents pour un bon disjoint

				// TODO DEBUG : risque de loop infini si potVal2 = 0 ;
				//
				layerLength[i] = layerLength[i] + temp;
				oldTemp = temp;
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 184:
	{ // Save ALL EVENTS
		// version encore très primitive : sauve tout, de 1 à 10, ne regarde pas si vide ou pas
		// saveAllEvents();
		saveLocal(false, false, true,
				  false, false, false,
				  false, false, false);
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 185: // Note to work on pour Fregoli et Cotard
	{
		noteToWorkOn = potVal3;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 186: // Fregoli
	{
		bool currentStatus = isSilentSByte(noteRedirect[noteToWorkOn]);
		noteRedirect[noteToWorkOn] = makeSByte(potVal3, currentStatus);
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 187: //  Cotard
	{
		bool currentStatus = isSilentSByte(noteRedirect[noteToWorkOn]);
		noteRedirect[noteToWorkOn] = makeSByte(valueSByte(noteRedirect[noteToWorkOn]), !currentStatus);
	}
	break;

	case 188: //  Macro Cotard + Fregoli modifications
	{
		// potVal3 = BBB ;    // on choisit de quelle layer on veut copier les infos, vers les layers actives

		switch (potVal3)
		{
			// Reset noteRedirect
			// Kill all Bemols
			// kill all natural
			// mov up all bemols
			// randomize notes
			// all notes to one
			// all notes to two

		case 0: // REset note redirection
		{
			for (byte i = 0; i < 12; i++)
			{
				noteRedirect[i] = 0; // ça veut dire que le pitch est +0 par rapport à note de départ
									 // et comme bit 6 et 7 sont 0 : valeur (+) et pas silencieux
			}
		}
		break;

		case 1: // Randomize Notes
		{
			randomizeNoteRedirect();
		}
		break;

		case 2: // Kill All Flats
		{
			silenceNoteRedirect(1);	 // C Sharp, D Flat
			silenceNoteRedirect(3);	 // D sharp, E Flat
			silenceNoteRedirect(6);	 // F Sharp , G Flat
			silenceNoteRedirect(8);	 // G Sharp , A Flat
			silenceNoteRedirect(10); // A shapr , B Flat
		}
		break;

		case 3: // Kill All Naturals
		{
			silenceNoteRedirect(0);	 //  C
			silenceNoteRedirect(2);	 //  D
			silenceNoteRedirect(4);	 //  E
			silenceNoteRedirect(5);	 //  F
			silenceNoteRedirect(7);	 //  G
			silenceNoteRedirect(9);	 //  A
			silenceNoteRedirect(11); //  B
		}
		break;

		case 4: // Move Up All Flats
		{
			noteRedirect[1] = makeSByte(random(2) == 0 ? +1 : -1, false);
			noteRedirect[3] = makeSByte(random(2) == 0 ? +1 : -1, false);
			noteRedirect[6] = makeSByte(random(2) == 0 ? +1 : -1, false);
			noteRedirect[8] = makeSByte(random(2) == 0 ? +1 : -1, false);
			noteRedirect[10] = makeSByte(random(2) == 0 ? +1 : -1, false);
		}
		break;

		case 5: // All note to ONE : noteToWorkOn
		{
			for (byte i = 0; i < 12; i++)
			{
				// bool currentStatus = isSilentSByte(noteRedirect[noteToWorkOn] );
				bool currentStatus = false; // on ne garde pas silencieuses les notes qui le sont
				noteRedirect[i] = makeSByte((int)((int)noteToWorkOn - (int)i), currentStatus);
			}
		}
		break;

		case 6: // All note to RAndom ONE :
		{
			byte RndOne = random(0, 12);

			for (byte i = 0; i < 12; i++)
			{
				// bool currentStatus = isSilentSByte(noteRedirect[noteToWorkOn] );
				bool currentStatus = false; // on ne garde pas silencieuses les notes qui le sont
				noteRedirect[i] = makeSByte((int)((int)RndOne - (int)i), currentStatus);
			}
		}
		break;

		case 7: // All note to RAndom TWO
		{
			byte RndOne = random(0, 12);
			byte RndTwo = random(0, 12);

			for (byte i = 0; i < 12; i++)
			{
				// bool currentStatus = isSilentSByte(noteRedirect[noteToWorkOn] );
				bool currentStatus = false; // on ne garde pas silencieuses les notes qui le sont
				byte chosen = (random(2) == 0 ? RndOne : RndTwo);
				noteRedirect[i] = makeSByte((int)((int)chosen - (int)i), currentStatus);
			}
		}
		break;

		case 8: // Random Silencer
		{
			for (byte i = 0; i < 12; i++)
			{
				noteRedirect[i] = makeSByte(valueSByte(noteRedirect[i]), random(2) == 0 ? true : false);
			}
		}
		break;

		case 9: // Inverter
		{
			for (byte i = 0; i < 12; i++)
			{
				noteRedirect[i] = makeSByte(11 - 2 * i, false);
			}
		}
		break;

			// ex : -12 = -2 octaves    +5 = vers sa quinte    0 = note inchangée
			// le bit 7 indique si négatif
			// le bit 6 indique si silencieux
			// il y a des fonctions pour conversion :
			//
			// bool isNegativeSByte( byte const& thi  ) {
			// bool isSilentSByte( byte const& thi  ) {
			// int valueSByte( byte const& thi  ) {
			// byte makeSByte( int  thi , bool const& silent ) {

		default:
			break;
		} // end of switch(PotVal)
	}	  // end of Case 188
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 189:
	{
		// SET Type of CCMod
		//     byte type ; // 0 = vide   1 = VELOCITY  2 = PITCH   3 = NOTE
		// potVal3 = BBB ;    // 1 à NbEvenementMax

		// se mettre sur le CCMod actuel ( en cours de fabrication ) ou un autre qu'on veut modifier

		// pileCCMod* CCModPtr ;

		// CCModPtr = giveNthPtrPileComplete(CCModPointeurInitial, currentCCMod);

		// (*CCModPtr).CCMod.type = potVal3 ;
		CCModTableau[currentCCMod].type = potVal3;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 190:
	{
		// SET currentCCMod
		//     byte type ; // 0 = vide   1 = VELOCITY  2 = PITCH   3 = NOTE
		// potVal3 = BBB ;    // 1 à NbEvenementMax
		// taillePileCCMod doit être la valeur max
		// NOTE **** bien réfléchir si on modifie le tableau Transfo ou si on rentre le map

		currentCCMod = potVal3;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 191:
	{
		// SET sourceMIN
		// potVal3 = BBB ;    // 1 à NbEvenementMax

		// se mettre sur le CCMod actuel ( en cours de fabrication ) ou un autre qu'on veut modifier

		// pileCCMod* CCModPtr ;

		// CCModPtr = giveNthPtrPileComplete(CCModPointeurInitial, currentCCMod);
		// (*CCModPtr).CCMod.sourceMin = potVal3 ;

		CCModTableau[currentCCMod].sourceMin = potVal3;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 192:
	{
		// SET sourceMAX
		// potVal3 = BBB ;    // 1 à NbEvenementMax

		// se mettre sur le CCMod actuel ( en cours de fabrication ) ou un autre qu'on veut modifier

		// pileCCMod* CCModPtr ;
		//
		// CCModPtr = giveNthPtrPileComplete(CCModPointeurInitial, currentCCMod);
		// (*CCModPtr).CCMod.sourceMax = potVal3 ;
		CCModTableau[currentCCMod].sourceMax = potVal3;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 193:
	{
		// SET targetMIN
		// potVal3 = BBB ;    // 1 à NbEvenementMax

		// se mettre sur le CCMod actuel ( en cours de fabrication ) ou un autre qu'on veut modifier

		// pileCCMod* CCModPtr ;
		//
		// CCModPtr = giveNthPtrPileComplete(CCModPointeurInitial, currentCCMod);
		// (*CCModPtr).CCMod.targetMin = potVal3 ;
		CCModTableau[currentCCMod].targetMin = potVal3;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 194:
	{
		// SET targetMax
		// potVal3 = BBB ;    // 1 à NbEvenementMax

		// se mettre sur le CCMod actuel ( en cours de fabrication ) ou un autre qu'on veut modifier

		// pileCCMod* CCModPtr ;
		//
		// CCModPtr = giveNthPtrPileComplete(CCModPointeurInitial, currentCCMod);
		// (*CCModPtr).CCMod.targetMax = potVal3 ;
		CCModTableau[currentCCMod].targetMax = potVal3;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 195:
	{
		// SET BorderBehavior
		// potVal3 = BBB ;    // 1 à NbEvenementMax

		// se mettre sur le CCMod actuel ( en cours de fabrication ) ou un autre qu'on veut modifier

		// pileCCMod* CCModPtr ;
		//
		// CCModPtr = giveNthPtrPileComplete(CCModPointeurInitial, currentCCMod);
		// (*CCModPtr).CCMod.borders = potVal3 ;
		CCModTableau[currentCCMod].borders = potVal3;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 196:
	{
		// SET CC id
		// potVal3 = BBB ;    // 1 à NbEvenementMax

		// se mettre sur le CCMod actuel ( en cours de fabrication ) ou un autre qu'on veut modifier

		// pileCCMod* CCModPtr ;
		//
		// CCModPtr = giveNthPtrPileComplete(CCModPointeurInitial, currentCCMod);
		// (*CCModPtr).CCMod.idCC = potVal3 ;
		CCModTableau[currentCCMod].idCC = potVal3;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 197:
	{
		// SET CC channel
		// potVal3 = BBB ;    // 1 à NbEvenementMax

		// se mettre sur le CCMod actuel ( en cours de fabrication ) ou un autre qu'on veut modifier

		// pileCCMod* CCModPtr ;
		//
		// CCModPtr = giveNthPtrPileComplete(CCModPointeurInitial, currentCCMod);
		// (*CCModPtr).CCMod.channelCC = potVal3 ;
		CCModTableau[currentCCMod].channelCC = potVal3 - 1; // Car on affiche un channel USER et on encode channel MACHINE
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 198:
	{
		// SET Layer for which note will send CC
		// potVal3 = BBB ;    // 1 à NbEvenementMax

		// se mettre sur le CCMod actuel ( en cours de fabrication ) ou un autre qu'on veut modifier

		// pileCCMod* CCModPtr ;
		//
		// CCModPtr = giveNthPtrPileComplete(CCModPointeurInitial, currentCCMod);
		// (*CCModPtr).CCMod.layerConcerne = potVal3 ;
		CCModTableau[currentCCMod].layerConcerne = potVal3;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 199:
	{
		// SET on/off midi //
		// potVal3 = BBB ;    // 1 à NbEvenementMax

		// se mettre sur le CCMod actuel ( en cours de fabrication ) ou un autre qu'on veut modifier

		// pileCCMod* CCModPtr ;
		//
		// CCModPtr = giveNthPtrPileComplete(CCModPointeurInitial, currentCCMod);
		// (*CCModPtr).CCMod.layerConcerne = (*CCModPtr).CCMod.layerConcerne ^ B10000  ;
		CCModTableau[currentCCMod].layerConcerne = CCModTableau[currentCCMod].layerConcerne ^ B10000;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 200:
	{
		// Activate CCMod

		CCModActif = true;
		CCModTableau[currentCCMod].actif = !CCModTableau[currentCCMod].actif;
		// modèle 				sequence[potVal2].actif = !sequence[potVal2].actif ;
	}
	break;

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
		// bool actif ; // une fois "ajouté" à la pile, il devient actif.
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

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 201: // MIDI Thru On/Off
	{
		midiThru = !midiThru;
		if (midiThru == true)
		{
			MIDI3.turnThruOn(midi::Thru::Full);
		}
		else
		{
			MIDI3.turnThruOff();
		};
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 202:
	{ // play 1 note .   le channe est spécifié par 203
		// potVal3 = BBB ;    //
		PlaynoteOn_(0x90 + channelForPlay1Note, (byte)potVal3, 100);
		// NOTE ajouter ici un affichage de la note jouée ;
		// joue une note à 2/3 de la vélocité max
		//
		String affiche;
		affiche = String(0x90 + channelForPlay1Note);
		affiche += F(" ");
		affiche += String((byte)potVal3);
		affiche += F(" ");
		affiche += String(100);

		printRot2(affiche, 16, 400, 3);
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 203:
	{
		// potVal3 = BBB ;    //
		channelForPlay1Note = potVal3 - 1; // car affichage toujours USER, encodage toujours MACHINE
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 204:
	{
		// affiche juste le prochain moment où la note 0 sera jouée
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 205:
	{ // DureePole1
		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{
				DureePole1[i] = potVal2;
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 206:
	{ // minPole1
		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{
				minPole1[i] = potVal2;
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 207:
	{ // maxPole1
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				maxPole1[i] = potVal2;
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 208:
	{ // DureePole2
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				DureePole2[i] = potVal2;
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 209:
	{ // minPole2
		// potVal2 = BBB ;

		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				minPole2[i] = potVal2;
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 210:
	{ // maxPole2

		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				maxPole2[i] = potVal2;
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 211:
	{ // freqPole1
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				freqPole1[i] = potVal2;
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 212:
	{ // bipolar Randomization

		turnOnBipolarRandom = !turnOnBipolarRandom;
		// TODO DEBUG : peut être préciser par layer ?
	};
	break;

	case 213:
	{ // VERTICAL MAPPING FROM LOWEST VELOCITY to desired values
		// potVal3 = map(BBB, 0, 1023, Transfo[LineCallingTransfo(108)][1] , Transfo[LineCallingTransfo(108)][2]);   // 2 à 16

		// NOTE DEBUG **** peut être faire un CleanUp avant, car parfois trouve des notes silencieuses !
		//
		byte templow = FindLowestVelocityOfActive(); // on peut pas faire une ligne avec un argument qui sera changé durant transfo
		byte temphigh = FindHighestVelocityOfActive();

		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);
				pileNote tempNote;

				for (unsigned int j = 0; j < taille; j++)
				{
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);
					tempNote.velocity = map(tempNote.velocity, templow, temphigh, targetVelociteMin, targetVelociteMax);
					ZEROSetNoteInPile(pInitial[i], j, tempNote);
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 214:
	{								 // SET Target MIN
		targetVelociteMin = potVal3; // il modifie les valeur du tableau pour 108 !
									 // NOTE DEBUG **** je crois qu'à terme il faut abandonner les transformations du tableau et leur préférer des variables
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 215:
	{								 // SET Target MAX
		targetVelociteMax = potVal3; // il modifie les valeur du tableau pour 108 !
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 216:
	{							  // POOF = FUNK : modifie la velocité de façon rnd
		turnOnPoof = !turnOnPoof; // ON ou OFF

		if (turnOnPoof)
		{
			for (byte i = 0; i < nbLayer; i++)
			{
				if (isLayerActive(i))
				{
					poof[i] = potVal2;
				}
			}
		}
	}
	break;

	case 217:
	{ // FILTER KILLER NOTE KILLING

		if (killingNote == true)
		{
			for (byte i = 0; i < nbLayer; i++)
			{
				if (isLayerActive(i))
				{

					// supprimer toutes les notes de valeur = noteToKill dans les layers actives
					unsigned int taille = mesureTaillePileComplete(pInitial[i]);

					for (unsigned int j = 0; j < taille; j++)
					{
						pileNote tempNote;
						tempNote = ZEROgetNoteFromPile(pInitial[i], j);
						ZEROSetNoteInPile(pInitial[i], j, tempNote);

						if (tempNote.note == noteToKill)
						{
							tempNote.note = 0;
							tempNote.velocity = 0;

							tempNote.startNote = 0;
							tempNote.endNote = 0;
							tempNote.killnote = true;
							ZEROSetNoteInPile(pInitial[i], j, tempNote);
						}
					}

					ZEROtreatPileComplete(pInitial[i]);
				}
			}
			// noteToKill = 0;
		}
		killingNote = !killingNote;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// PILE VERSION : faite et vérifiée
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 218:
	{ // CELL

		//potVal2 = BBB ;
		// noteForCellArray est définie dans la procédure d'affichage
		// CHANNEL out est défini dans 219 : channelForCell ; -1
		// NOTE OUT est potVal2

		if (isNoteCellConsolidated(channelInForCell, noteForCellArray))
		{
			byte place = givePlaceInTableauForNoteCellConsolidated(channelInForCell, noteForCellArray);
			// String affiche = F("UPDATING AT ") ;
			// affiche += String(place) ;
			// printRot2(affiche,16,400,3 ) ;
			// delay( 2000 ) ;
			tableauCell[place].noteIn = noteForCellArray;
			tableauCell[place].channelOut = channelOutForCell;
			tableauCell[place].noteOut = potVal2;
			tableauCell[place].channelIn = channelInForCell;
		}
		else
		{
			if (nbValeursDansCell < nbValeursMaxPourCell)
			{
				// String affiche = F("SAVING AT ") ;
				// affiche += String(nbValeursDansCell) ;
				// printRot2(affiche,16,400,3 ) ;
				// delay( 2000 ) ;
				tableauCell[nbValeursDansCell].noteIn = noteForCellArray;
				tableauCell[nbValeursDansCell].channelOut = channelOutForCell;
				tableauCell[nbValeursDansCell].noteOut = potVal2;
				tableauCell[nbValeursDansCell].channelOut = channelInForCell;
				nbValeursDansCell++;
			}
			else
			{
				printRot2(F("FULL MEMORY"), 16, 400, 3);
				delay(2000);
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// universel
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 219:
	{ // SET CHANNEL FOR CELL
		channelOutForCell = potVal2 - 1;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// universel
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 220: // SOFT PASS On/Off
	{
		softPass = !softPass;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// universel
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 221: // DELETE CELL Table
	{

		for (byte i = 0; i < nbValeursMaxPourCell; i++)
		{
			tableauCell[i].noteIn = 0;
			tableauCell[i].channelOut = 0;
			tableauCell[i].noteOut = 0;
			tableauCell[i].channelIn = 0;
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version PILE de note
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 222: //  Fit current layer to Euclidian Rhythm
	{
		String Necklace = nulString;
		;
		String usualA = F("1");
		String usualB = F("0");

		// byte ff(0) ; // 1C94 mauvaise initialisation

		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);
				unsigned long euclidianMatrixTime[taille] = {0};

				Necklace = Euclidian(kNecklace, mNecklace);
				Necklace = StringShift(Necklace, ShiftNecklace);

				byte ff(0); // 1C94 initialisation était au mauvais endroit
				// TODO donnait qd mêem un résultat intéressant -> à conserver ? KEEP BUG

				for (byte j = 0; j < mNecklace; j++)
				{
					// on va passer en revue les m positions du Necklace

					if (Necklace[j] == usualA[0])
					{
						// la valeur définie comme un pulse, normalement "1"
						euclidianMatrixTime[ff] = (unsigned long)((float)layerLength[i] * (float)j / (float)mNecklace) + 1;
						ff++;
					}
				} // fin de création du necklace

				// on a donc un euclidianMatrixTime pour la layer active
				// on doit comparer l'euclidianMatrixTime et le melodyMatrixTime de cette layer
				//
				// on prend une première note
				// on parcourt la liste des moments dans euclidian et on repère celui qui est le plus proche
				// on calcule différence entre les deux
				// on applique cela à début et à fin de note
				//
				// on répète ensuite la procédure

				for (unsigned int j = 0; j < taille; j++)
				{

					unsigned int smallest(0);
					bool foundOne(false);
					byte mSmallest(0);

					pileNote tempNote;

					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					for (byte m = 0; m < kNecklace; m++)
					{

						unsigned long differenceEuc;

						differenceEuc = abs(tempNote.startNote - euclidianMatrixTime[m]);

						if (foundOne == false)
						{
							smallest = differenceEuc;
							foundOne = true;
							mSmallest = m;
						}

						if (differenceEuc < smallest)
						{
							smallest = differenceEuc;
							mSmallest = m;
						}
					}

					// on a maintenant un mSmallest qui indique quel moment de l'euclidian matrix est le plus proche de la note j

					unsigned long difference = tempNote.startNote - euclidianMatrixTime[mSmallest];
					// on calclue la différence de façon signée cette fois

					tempNote.startNote -= difference;
					tempNote.endNote -= difference;

					pileNote_fixStartEndNote(tempNote, 0);

					ZEROSetNoteInPile(pInitial[i], j, tempNote);

				} // fin de la boucle j de parcours des notes du layer

			} // fin de boucle layer Active
		}	  // fin de boucle i pour layers

		// il faudrait faire un cleanup
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version PILE de note
		// 1C94
		// 2C22 supprimé
		/////////////////////////////////////////////////////////////////////////////////////

	case 223:
	{	// SENTENCE Rec
		// 2C22 DISMISSED
	};
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version PILE de note
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 224:
	{ // ADJUST END

		// potVal2 = BBB ;

		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				long oldlayerLength = layerLength[i];

				if ((long)-potVal2 > (long)layerLength[i])
				{
					layerLength[i] = 10;
				} // c'est le cas si on retranche une durée plus grande que layerLength
				else
				{
					layerLength[i] += potVal2;
				}

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{

					signed long calculatedStart, calculatedEnd;
					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					calculatedStart = tempNote.startNote;

					if (tempNote.startNote > tempNote.endNote)
					{
						calculatedEnd = oldlayerLength + tempNote.endNote;
					}
					else
					{
						calculatedEnd = tempNote.endNote;
					}

					while ((unsigned long)calculatedStart > layerLength[i])
					{
						calculatedStart -= layerLength[i];
					}
					while ((unsigned long)calculatedEnd > layerLength[i])
					{
						calculatedEnd -= layerLength[i];
					}

					tempNote.startNote = calculatedStart;
					tempNote.endNote = calculatedEnd;

					ZEROSetNoteInPile(pInitial[i], j, tempNote);
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version PILE de note
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 225:
	{ // ADJUST START

		//potVal2 = BBB ;

		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				long oldlayerLength = layerLength[i];

				if ((long)-potVal2 > (long)layerLength[i])
				{
					layerLength[i] = 10;
				} // c'est le cas si on retranche une durée plus grande que layerLength
				else
				{
					layerLength[i] += potVal2;
				}

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{

					signed int calculatedStart, calculatedEnd;
					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					calculatedStart = tempNote.startNote;

					if (tempNote.startNote > tempNote.endNote)
					{
						calculatedEnd = oldlayerLength + tempNote.endNote;
					}
					else
					{
						calculatedEnd = tempNote.endNote;
					}

					calculatedStart += potVal2;
					calculatedEnd += potVal2;

					while (calculatedStart < 0)
					{
						calculatedStart = layerLength[i] - calculatedStart;
					}
					while (calculatedEnd < 0)
					{
						calculatedEnd = layerLength[i] - calculatedEnd;
					}

					while ((unsigned long)calculatedStart > layerLength[i])
					{
						calculatedStart -= layerLength[i];
					}
					while ((unsigned long)calculatedEnd > layerLength[i])
					{
						calculatedEnd -= layerLength[i];
					}

					tempNote.startNote = calculatedStart;
					tempNote.endNote = calculatedEnd;

					ZEROSetNoteInPile(pInitial[i], j, tempNote);
				}
			}
		}
	}

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 226:
	{ // Create ou plutôt modify Sequence

		//potVal2 = BBB ;
		if (sequence[currentSequence].tailleSeq < tailleMaxTableauSequence)
		{
			sequence[currentSequence].tableauSeq[sequence[currentSequence].tailleSeq] = potVal2;
			sequence[currentSequence].tailleSeq++;
		}
		else
		{
			String affiche = nulString;
			affiche = F("Max Atteint");
			printRot2(affiche, 16, 400, 3);
			delay(1000);
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 227:
	{ // Delete Sequence

		//potVal2 = BBB ;

		// sequenceSize[potVal2] = 0 ;
		sequence[potVal2].tailleSeq = 0;
		// ce qui suit n'est pas indispensable, mais précaution

		for (byte i = 0; i < tailleMaxTableauSequence; i++)
		{
			sequence[potVal2].tableauSeq[i] = 0;
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 228:
	{ // Select Sequence
		//potVal2 = BBB ;
		currentSequence = potVal2;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 229:
	{ // Erase 1 digit from Sequence
		// potVal2 = BBB ;

		//	potVal2 = BBB ;

		if (sequence[potVal2].tailleSeq > 0)
		{
			sequence[potVal2].tailleSeq--;
		};
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 230:
	{ // SET Type for Sequence

		//	potVal2 = BBB ;
		//potVal2 = BBB ;
		sequence[currentSequence].modeSeq = potVal2;
		// sequence[currentSequence].compteur = 0 ; NOTE DEBUG je ne sais pas quand l'initialiser

		if (potVal2 == 4)
		{
			// intialisation pour jitter
			sequence[currentSequence].jitterMinUP = 0;
			sequence[currentSequence].jitterMaxUP = 1;
			sequence[currentSequence].jitterMinDOWN = 0;
			sequence[currentSequence].jitterMaxDOWN = 1;
			sequence[currentSequence].jitterStaticMin = 0;
			sequence[currentSequence].jitterStaticMax = 0;
			sequence[currentSequence].jitterRichting = 1;
			sequence[currentSequence].jitterWhereNow = 0;
			sequence[currentSequence].jitterWhereTo = 2;
			sequence[currentSequence].jitterStaticCompteur = 0;
			sequence[currentSequence].jitterStaticToReach = 0;
			sequence[currentSequence].beingStatic = false;
		}

		if (potVal2 == 5)
		{
			// intialisation pour Random Fragment
			sequence[currentSequence].sizeRANDOMFRAG = 5;
			sequence[currentSequence].repeatRANDOMFRAG = 0;
			sequence[currentSequence].toReachRANDOMFRAG = 1;
		}

		// const byte modeFWD = 0 ; //
		// const byte modeBACK = 1 ;//
		// const byte modeRANDOM = 2 ;
		// const byte modeALLERRETOUR = 3 ;
		// const byte modeJITTER = 4 ;
		// const byte modeRANDOMFRAG = 5 ;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 231:
	{ // SET data for Jitter : UPmin
		//potVal2 = BBB ;
		sequence[currentSequence].jitterMinUP = potVal2;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 232:
	{ // SET data for Jitter : jitterMaxUP
		//potVal2 = BBB ;
		sequence[currentSequence].jitterMaxUP = potVal2;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 233:
	{ // SET data for Jitter : jitterMinDOWN
		//potVal2 = BBB ;
		sequence[currentSequence].jitterMinDOWN = potVal2;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 234:
	{ // SET data for Jitter : jitterMaxDOWN
		//potVal2 = BBB ;
		sequence[currentSequence].jitterMaxDOWN = potVal2;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 235:
	{ // SET data for Jitter : jitterStaticMin
		//potVal2 = BBB ;
		sequence[currentSequence].jitterStaticMin = potVal2;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 236:
	{ // SET data for Jitter : jitterStaticMax
		//potVal2 = BBB ;
		sequence[currentSequence].jitterStaticMax = potVal2;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 237:
	{ // SET data for Rnd Frag : fragment Size
		//potVal2 = BBB ;
		sequence[currentSequence].sizeRANDOMFRAG = potVal2;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 238:
	{ // SET data for Rnd Frag : fnb of Repeatz
		//potVal2 = BBB ;
		sequence[currentSequence].repeatRANDOMFRAG = potVal2;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 239:
	{ // SET Action
		/*
		0 send CC 
	    case 1:            affiche += (F("2.Pitch ADD"));
        case 2:            affiche += (F("3.chg Velocity"));
        case 3:            affiche += (F("4.Pitch SET")); // 2C02
        case 4:            affiche += (F("5.Event List"));
        case 5:            affiche += (F("6.LG note"));
        case 6:            affiche += (F("7.LayerL SET"));
        case 7:            affiche += F("8.LayerL ADD");
        case 8:            affiche += F("9.LayerL %");
         case 9:            affiche += (F("10.Vel SET"));
        case 10:            affiche += F("11.Vel ADD");
        case 11:            affiche += F("12.Vel %");
        case 12:            affiche += F("13.NoteL SET");
        case 13:            affiche += F("14.NoteL ADD");
        case 14:            affiche += F("15.NoteL %");
		case 15 :			affiche += F("16.Degree ADD");   
		case 16 : 			affiche += F("17.Degree RND"); 
		case 17 : 			affiche += F("18.Vel RND"); 
		case 18 : 		 affiche += F("19.Note ON/OFF");  
		case 19 : 		affiche += F("20.Pitch RND");  
		20		21 Set CH
		21		22 Add CH 
     */

		sequence[currentSequence].action = potVal2;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 240:
	{ // SET CC
		//potVal2 = BBB ;

		sequence[currentSequence].CC = potVal2;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 241:
	{ // SET Channel for CC sent by Sequence
		//potVal2 = BBB ;
		sequence[currentSequence].channel = potVal2 - 1;
		// on affiche pour l'End USER et on encode pour les machines
		// donc Channel 1 affiché devient Channel 0 enregistré .
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 242: // ACTIVE LAYER CHOOSER  pour Sequence  via le menu, de sorte qu'il puisse se produire quand on passe en mode automatique
	{
		//potVal3 = BBB ;     // on choisit de quelle layer on veut copier les infos, vers les layers actives
		sequence[currentSequence].layer = potVal3;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 243: // activate ou inactivate Sequence
	{
		//potVal2 = BBB ;
		sequence[potVal2].actif = !sequence[potVal2].actif;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 244: // activate ON OFF DynHarmonize
	{

		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				harmonizeLayer[i] = !harmonizeLayer[i];
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 245: // mode  DynHarmonize
	{
		//potVal2 = BBB ;
		dynamicHarmonizeMode = potVal2;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 246:
	{
		// layerCounter : remet à zéro

		for (byte i = 0; i < nbLayer; i++)
		{
			layerCounter[i] = 0;
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 247: // Sequence compte Layer ou notes
	{
		// potVal2 = BBB ;
		sequence[potVal2].compteLayer = !sequence[potVal2].compteLayer;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 248: // modulo layer Eclipse
	{

		String affiche;
		// potVal2 = BBB ;

		switch (status248)
		{
		case (0):
		{
			if (potVal2 > 0)
			{

				eclipseModulo = potVal2;
				status248 = 1;
				//affiche = F("status 1") ;
				//printRot2(affiche  ,16,400,3);
				// delay(1000) ;
				//
				for (byte i = 0; i < nbLayer; i++)
				{
					if (isLayerActive(i))
					{
						eclipseModuloTab[i] = eclipseModulo;
					}
				}
			}
			else
			{
				String affiche = nulString;
				affiche = F("Modulo > 0 SVP");
				printRot2(affiche, 16, 400, 3);
				delay(1000);
			}
		};
		break;

		case (1):
		{

			eclipseK = potVal2;

			for (byte i = 0; i < nbLayer; i++)
			{

				if (isLayerActive(i))
				{
					eclipseKTab[i] = eclipseK;
				}
			}
			status248 = 2;
			//affiche = F("status 2") ;
			//printRot2(affiche  ,16,400,3);
			// delay(1000) ;
		};
		break;

		case (2):
		{

			byte eclipseModeTemp(0);
			if (potVal2 < Transfo[softwareValueEncoderLeft][2] / 2)
			{
				eclipseModeTemp = 1; // mute SelectedNumbers
			}
			else
			{
				eclipseModeTemp = 2; // mute all the non Selected Numbers }
			}

			// NOTE DEBUG prévoir un choix pour en ressortir !
			// EFFACE ECLIPSE ?  ou on laisse faire Eclipse 1+0

			for (byte i = 0; i < nbLayer; i++)
			{

				if (isLayerActive(i))
				{
					eclipseMode[i] = eclipseModeTemp;
				}
			}

			status248 = 0;
			affiche = F("retour status 0");
			printRot2(affiche, 16, 400, 3);
			// delay(1000) ;
		};
		break;

		default:
		{
		};
		break;
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 249: // Markov Chain ON
	{
		// 	markovChainGlobal = !markovChainGlobal;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 250: // TAP ACCOU  : par défaut OFF
		// le Tap Accou va ajuster les layers actives à la durée du TAP introduit et
		// modifier les autres de façon à garder les rapports entre durée de layers
		{
			tapAccou = !tapAccou;
		}
		break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 251: // Mode Chooser
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			modeActuelle[i] = potVal3; // on doit le retenir pour AutoHarmonize
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 252: // AUTO CLEAN UP : par défaut ON

	{
		autoCleanUp = !autoCleanUp;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 253: // HASH
	{

		hashSegments = potVal3;

		// va segmenter le layer en hashSegments
		// pour chaque note qui dépasse, doit créer une nouvelle note en cherchant firstFreeNote
		// Crée un tableau avec des 1 et 0 en fonction d'occupé par une note ou pas 1 0 0 0 1 0
		// va passer en revue les segments : en fc du %Permute, va permuter ( non destructif )
		// va passer en revue les segments : si 1 : repeater ( plutôt le faire dans 2nd temps )
		// va passer en revue les semgents : en fc du %invert, va inverser
		// qd tout est fini, va voir si doit unifier des notes : crééer une fonction à part qui ne fait que ça ?

		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				unsigned long longueurSegment = layerLength[i] / (unsigned long)hashSegments;

				// clean-up, on met le bit 2 à 0

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				// va segmenter le layer en hashSegments
				//
				for (byte k = 1; k < hashSegments; k++)
				{
					for (unsigned int j = 0; j < taille; j++)
					{ // va parcourir la mélodie pointée ( Layer 1 à 4 : potVal3 )

						pileNote tempNote;
						tempNote = ZEROgetNoteFromPile(pInitial[i], j);

						// pour ce k : on a une valeur de cut off
						unsigned long kCutOff = (unsigned long)k * longueurSegment;

						if ((tempNote.startNote < kCutOff) && (tempNote.endNote > kCutOff))
						{
							// cela signifie qu'on a une note qui coupe le cutOff : il faut la scinder

							pileNote tempNote2;
							putNoteBinNoteA(tempNote2, tempNote);

							tempNote2.startNote = kCutOff + 2;
							ZEROaddNewNotetoPile(pInitial[i], tempNote2);
							// modification de l'ancienne note :
							tempNote.endNote = kCutOff - 2;
							ZEROSetNoteInPile(pInitial[i], j, tempNote);

							taille = mesureTaillePileComplete(pInitial[i]); // on ajuste la taille de la pile à parcourir
						}
					}
				}
				// on a maintenant un layer découpé selon les segments

				// va passer en revue les segments : en fc du %Permute, va permuter ( non destructif )

				for (byte k = 0; k < hashSegments; k++)
				{

					if (random(100) < hashPercentPermute)
					{

						// pour ce k : on a une valeur de cut off
						unsigned long kSegmentBegin = (unsigned long)k * longueurSegment;
						unsigned long kSegmentEnd = (unsigned long)(k + 1) * longueurSegment;

						byte otherK = random(0, hashSegments);
						unsigned long otherkSegmentBegin = (unsigned long)otherK * longueurSegment;
						unsigned long otherkSegmentEnd = (unsigned long)(otherK + 1) * longueurSegment;

						unsigned int taille = mesureTaillePileComplete(pInitial[i]);

						// on doit maintenant déplacer les notes qui sont comprises entre kSegmentBegin et kSegmentEnd
						// vers la zone commençant à otherkSegmentBegin
						//
						// ensuite, on doit rapatrier les notes entre otherkSemgentBegin et otherkSegmentEnd qui ne viennet pas d'y être placé ( tag ! )
						// ves la zone kSegmentBegin

						for (unsigned int j = 0; j < taille; j++)
						{ // va parcourir la mélodie pointée ( Layer 1 à 4 : potVal3 )
							pileNote tempNote;
							tempNote = ZEROgetNoteFromPile(pInitial[i], j);

							if ((tempNote.startNote > kSegmentBegin) && (tempNote.endNote < kSegmentEnd))
							{
								tempNote.startNote = tempNote.startNote + otherkSegmentBegin - kSegmentBegin;
								tempNote.endNote = tempNote.endNote + otherkSegmentBegin - kSegmentBegin;
								ZEROSetNoteInPile(pInitial[i], j, tempNote);
							}
							else if ((tempNote.startNote > otherkSegmentBegin) && (tempNote.endNote < otherkSegmentEnd))
							{
								tempNote.startNote = tempNote.startNote - otherkSegmentBegin + kSegmentBegin;
								tempNote.endNote = tempNote.endNote - otherkSegmentBegin + kSegmentBegin;
								ZEROSetNoteInPile(pInitial[i], j, tempNote);
							}
						}
					}
				}

				// va passer en revue les segments : si 1 : repeater ( plutôt le faire dans 2nd temps )

				for (byte k = 0; k < hashSegments; k++)
				{

					if (random(0, 100) < hashPercentRepeats)
					{

						// pour ce k : on a une valeur de cut off
						unsigned long kSegmentBegin = (unsigned long)k * longueurSegment;
						unsigned long kSegmentEnd = (unsigned long)(k + 1) * longueurSegment;

						byte nbRepeatsThisTime = random(0, hashNbRepeatsMax + 1);

						unsigned int taille = mesureTaillePileComplete(pInitial[i]);

						for (byte l = 1; l <= nbRepeatsThisTime; l++)
						{
							unsigned long otherkSegmentBegin = kSegmentBegin + (unsigned long)l * longueurSegment;
							unsigned long otherkSegmentEnd = kSegmentEnd + (unsigned long)l * longueurSegment;

							// d'abord effacer la zone où on va dupliquer

							for (unsigned int j = 0; j < taille; j++)
							{ // va parcourir la mélodie pointée ( Layer 1 à 4 : potVal3 )
								pileNote tempNote;
								tempNote = ZEROgetNoteFromPile(pInitial[i], j);

								if ((tempNote.startNote > otherkSegmentBegin) && (tempNote.endNote < otherkSegmentEnd))
								{
									// delete Note
									putNoteBinNoteA(tempNote, emptyNote);
									tempNote.killnote = true;
									ZEROSetNoteInPile(pInitial[i], j, tempNote);
								}
							}

							taille = mesureTaillePileComplete(pInitial[i]);

							for (unsigned int j = 0; j < taille; j++)
							{ // va parcourir la mélodie pointée ( Layer 1 à 4 : potVal3 )
								pileNote tempNote;
								tempNote = ZEROgetNoteFromPile(pInitial[i], j);

								if ((tempNote.startNote > kSegmentBegin) && (tempNote.endNote < kSegmentEnd))
								{
									tempNote.startNote = tempNote.startNote + l * (longueurSegment);
									tempNote.endNote = tempNote.endNote + l * (longueurSegment);
									ZEROaddNewNotetoPile(pInitial[i], tempNote);
								}
							}
						}
					}
				}

				// va passer en revue les segments : en fc du %invert, va inverser

				for (byte k = 0; k < hashSegments; k++)
				{

					if (random(0, 100) < hashPercentInvert)
					{

						// pour ce k : on a une valeur de cut off
						unsigned long kSegmentBegin = (unsigned long)k * longueurSegment;
						unsigned long kSegmentEnd = (unsigned long)(k + 1) * longueurSegment;

						unsigned int taille = mesureTaillePileComplete(pInitial[i]);

						for (unsigned int j = 0; j < taille; j++)
						{ // va parcourir la mélodie pointée ( Layer 1 à 4 : potVal3 )

							pileNote tempNote;
							tempNote = ZEROgetNoteFromPile(pInitial[i], j);

							if ((tempNote.startNote > kSegmentBegin) && (tempNote.endNote < kSegmentEnd))

							{

								// kSegmentBegin   note                kSegmentEnd
								// kSegmentBegin               note    kSegmentEnd
								//
								// la note commence à kSegmentBegin + ( note - kSEgmentBegin )
								// elle s'arrête à kSegmentBegin + ( noteEnd - kSegmentBegin )
								//
								// elle doit aller à kSegmentEnd - ( noteEnd  - kSegmentBegin )
								// elle s'arrêtera à kSEgmentEnd - ( note - kSegmentBegin )

								// valeur de nouvelle note :
								// melodyMatrix[i][j][MMdata2] = (    melodyMatrix[i][j][MMdata2] ^ B00000111 );
								// melodyMatrix[i][j][MMdata2] = (    melodyMatrix[i][j][MMdata2] ^ B00000100 );

								// bit 0 et 1 sont 1 = played and silented pour que la note ne soit pas rejouée avant le tour suivant.
								// bit 2 sont 1 car c'est une note qui vient d'être créé, elle ne doit pas être re-ping-pong-ée
								unsigned long noteEnd = tempNote.endNote;
								unsigned long noteStart = tempNote.startNote;
								tempNote.startNote = kSegmentEnd + kSegmentBegin - noteEnd;
								tempNote.endNote = kSegmentEnd + kSegmentBegin - noteStart;
								ZEROSetNoteInPile(pInitial[i], j, tempNote);
							}
						}
					}
				}

				if (hashUnify)
				{
					Transformation(259, 5); // on fait un REUnited à 10 msec
				}

				ZEROupdatePileTimeToPlayMODEDIRECT(i);
			}
		}
		// qd tout est fini, va voir si doit unifier des notes : crééer une fonction à part qui ne fait que ça ?
	}
	break;

		// {253,1,32,B0000}, // HASH : la valeur définit le nombre de segments
		// {254,0,101,B0011}, // % Repeats
		// {255,0,32,B0000}, // Nb de repeats maximum
		// {256,0,101,B0011}, // chance de permutation pour un segment
		// {257,0,101,B0011}, // chance d'inversion pour un segment
		// {258,0,0,B0000}   // ON OFF pour unify après HASH

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 254: // HASH : % Repeats
	{
		hashPercentRepeats = potVal3;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 255: // HASH : Nb Repeats MAx
	{
		hashNbRepeatsMax = potVal3;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 256: // HASH : % Permute
	{
		hashPercentPermute = potVal3;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 257: // HASH : % Inversion
	{
		hashPercentInvert = potVal3;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 258: // Hash Unify ON OFF
	{
		hashUnify = !hashUnify;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 259: // REUNITED
	{

		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{ // va parcourir la mélodie pointée ( Layer 1 à 4 : potVal3 )
					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					if (tempNote.note != 0)
					{
						for (byte k = j + 1; k < taille; k++)
						{
							pileNote tempNote2;
							tempNote2 = ZEROgetNoteFromPile(pInitial[i], k);

							if ((tempNote.note == tempNote2.note) && (abs(tempNote2.startNote - tempNote.endNote) < (unsigned long)potVal2) && (tempNote.channel == tempNote2.channel))

							{
								// alors on fusionne
								// melodyMatrix[i][j][MMvelocite] = ( melodyMatrix[i][j][MMvelocite]/2 + melodyMatrix[i][k][MMvelocite]/2 )   ;
								tempNote.endNote = tempNote2.endNote;

								// et on supprime la deuxième note
								putNoteBinNoteA(tempNote2, emptyNote);
								tempNote2.killnote = true;
								ZEROSetNoteInPile(pInitial[i], k, tempNote2);
								ZEROSetNoteInPile(pInitial[i], j, tempNote);
							}
						}
					}
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 260:
	{ // DELETE CCMod

		// CCModTableau[currentCCMod].type = 0;
		// CCModTableau[currentCCMod].sourceMin = 0;
		// CCModTableau[currentCCMod].sourceMax = 127;
		// CCModTableau[currentCCMod].targetMin = 0;
		// CCModTableau[currentCCMod].targetMax = 127;
		// CCModTableau[currentCCMod].borders = 0;
		// CCModTableau[currentCCMod].idCC = 69; // au hasard
		// CCModTableau[currentCCMod].channelCC = 0;
		// CCModTableau[currentCCMod].layerConcerne = 1;
		// CCModTableau[currentCCMod].actif = false;
		deleteCCMod(currentCCMod);
	}
	break;

		// 1C81Regression2
		/*
		case 261 : NameTransfo = F("Activate Condition") ; break ;
		case 262 : NameTransfo = F(">WhichTest") ; break ;
		case 263 : NameTransfo = F(">WhichOperation") ; break ;
		case 264 : NameTransfo = F(">WhichValue") ; break ;
		// case 265 : NameTransfo = F(">WhichOperation") ; break ;
		case 265 : NameTransfo = F(">WhichComparator") ; break ;
		case 266 : NameTransfo = F(">WhichReference") ; break ;
		case 267 : NameTransfo = F(">WhichAction") ; break ;
		*/

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 261: // Activate Condition in Evenement current
	{
		Evenement[CurrentEvenement].conditionActive = !Evenement[CurrentEvenement].conditionActive;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 262: // Condition Event : which test
	{
		Evenement[CurrentEvenement].whichTest = potVal3;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 263: // Condition Event : which operation
	{
		Evenement[CurrentEvenement].whichOperation = potVal3;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 264: // Condition Event : which value
	{
		Evenement[CurrentEvenement].whichValue = potVal3;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 265: // condition : which comparison
	{
		Evenement[CurrentEvenement].whichComparison = potVal3;
	}

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 266: // Condition Event : which REference ( pour la comparaison : T / 5 < 10 : ici 10 )
	{
		Evenement[CurrentEvenement].whichReference = potVal3;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 267: // Condition Event : which Action
	{
		Evenement[CurrentEvenement].whichAction = potVal3;
	}
	break;

		// TODO ne pas oublier l'initialisation des Events

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 269: // Condition Event : which Layers to Test
	{
		Evenement[CurrentEvenement].whichLayer2test = potVal3;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 270: // modulo layer Eclipse
	{

		String affiche;

		switch (status270)
		{
		case (0):
		{
			if (potVal2 > 0)
			{

				status270 = 1;

				Evenement[CurrentEvenement].moduloLayers = activeLayer;
				Evenement[CurrentEvenement].modulo = potVal2;
			}
			else
			{
				String affiche = nulString;
				affiche = F("Modulo > 0 SVP");
				printRot2(affiche, 16, 400, 3);
				delay(1000);
			}
		};
		break;

		case (1):
		{

			Evenement[CurrentEvenement].moduloLayers = activeLayer;
			Evenement[CurrentEvenement].moduloK = potVal2;

			status270 = 2;
		};
		break;

		case (2):
		{

			byte eclipseModeTemp(0);
			if (potVal2 < Transfo[softwareValueEncoderLeft][2] / 2)
			{
				eclipseModeTemp = 1; // event just those
			}
			else
			{
				eclipseModeTemp = 2; // event all the others
			}

			// NOTE DEBUG prévoir un choix pour en ressortir !
			// EFFACE ECLIPSE ?  ou on laisse faire Eclipse 1+0

			Evenement[CurrentEvenement].moduloMode = eclipseModeTemp;

			status270 = 0;
			affiche = F("retour status 0");
			printRot2(affiche, 16, 400, 3);
			// delay(1000) ;
		};
		break;

		default:
		{
		};
		break;
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 271: // SPANDEX, adapté de TRANSPOSE
	{

		int middle = meanPitch_MultipleLayer(activeLayer);

		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{

					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					// pour les notes au-dessus de moyenne, on ajoute potVal2 demi-tons
					// pour les notes en-dessous de moyenne, on retir potVal2 demi-tons

					if (tempNote.note != 0)
					{

						int temp;
						temp = tempNote.note;

						if (temp > middle)
						{
							temp += potVal2;
						}
						else
						{
							temp -= potVal2;
						};

						if (temp < 0)
						{
							temp = tempNote.note % 12;
						} //  pour conserver une mélodie
						else if (temp > 127)
						{
							temp = tempNote.note % 12 + 108;
						}
						else
						{
							tempNote.note = temp;
						}
						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 272: // SPANFLEX, adapté de SPANDEX, adapté de TRANSPOSE
	{

		int middle = meanPitch_MultipleLayer(activeLayer);

		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{

					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					// pour les notes au-dessus de moyenne, on ajoute potVal2 demi-tons
					// pour les notes en-dessous de moyenne, on retir potVal2 demi-tons

					if (tempNote.note != 0)
					{

						int temp;
						temp = tempNote.note;

						float diff;
						diff = (float)temp - (float)middle;
						diff = (float)((diff * potVal2) / (float)10.0);

						temp = (int)((float)middle + (float)diff);

						if (temp < 0)
						{
							temp = tempNote.note % 12;
						} // on ne fai rien, pour conserver une mélodie
						else if (temp > 127)
						{
							temp = tempNote.note % 12 + 108;
						}
						else
						{
							tempNote.note = temp;
						}

						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 273: // KeepSyncopBUG
	{
		// le potard indique combien de secondes avant auto-modifcation

		KeepSyncopBUG = !KeepSyncopBUG;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 274:
		// Replicate Event
		{

			Evenement[CurrentEvenement].actif = false;
			// c'est une des seules valeurs qui n'est pas identique !

			Evenement[CurrentEvenement].type = Evenement[potVal2].type; // 0 = vide  1=CC 2=Transfo
			// pour les Transfo modifiant valeur, elle devront voir le type défini
			Evenement[CurrentEvenement].intervalle = Evenement[potVal2].intervalle; // 1 = 0.1sec - sinon risque de déclencher un event super réptitif en ativant un event nul
			Evenement[CurrentEvenement].moment = Evenement[potVal2].moment;			// dans les temps divisible par 300, agit après 100
			// Transfo[LineCallingTransfo(131)][2]=(int) 18000;
			Evenement[CurrentEvenement].fidelite = Evenement[potVal2].fidelite; // %
			Evenement[CurrentEvenement].which = Evenement[potVal2].which;		// définit quel CC va être modifié
			Evenement[CurrentEvenement].valeur = Evenement[potVal2].valeur;		// définit la valeur initiale qui lui sera attribuée
			Evenement[CurrentEvenement].shape = Evenement[potVal2].shape;		// 1=fixe 2=inc 3=Dec 4=rnd 5=liste
			Evenement[CurrentEvenement].rangeUP_ = Evenement[potVal2].rangeUP_;
			Evenement[CurrentEvenement].rangeLO_ = Evenement[potVal2].rangeLO_;
			Evenement[CurrentEvenement].incSize = Evenement[potVal2].incSize;
			Evenement[CurrentEvenement].behaviorLimit = Evenement[potVal2].behaviorLimit;
			Evenement[CurrentEvenement].channel = Evenement[potVal2].channel; //
			Evenement[CurrentEvenement].layer = Evenement[potVal2].layer;	  // tous layers actives - non pertinent pour un CC, car ne modifie pas de layers !
			// Evenement[CurrentEvenement].eventGOON = Evenement[potVal2].eventGOON;
			Evenement[potVal2].eventGOON = 0;

			Evenement[CurrentEvenement].conditionActive = Evenement[potVal2].conditionActive;
			Evenement[CurrentEvenement].whichTest = Evenement[potVal2].whichTest;
			Evenement[CurrentEvenement].whichOperation = Evenement[potVal2].whichOperation;
			Evenement[CurrentEvenement].whichValue = Evenement[potVal2].whichValue;
			Evenement[CurrentEvenement].whichComparison = Evenement[potVal2].whichComparison;
			Evenement[CurrentEvenement].whichReference = Evenement[potVal2].whichReference;
			Evenement[CurrentEvenement].whichAction = Evenement[potVal2].whichAction; // 1 DO 2.BLOCK

			Evenement[potVal2].launch = false;												  // utilisé pour ce qui est lié  à des notes ou layer modulo play
			Evenement[CurrentEvenement].whichLayer2test = Evenement[potVal2].whichLayer2test; //

			Evenement[CurrentEvenement].modulo = Evenement[potVal2].modulo;
			Evenement[CurrentEvenement].moduloK = Evenement[potVal2].moduloK;
			Evenement[CurrentEvenement].moduloLayers = Evenement[potVal2].moduloLayers;
			Evenement[CurrentEvenement].moduloMode = Evenement[potVal2].moduloMode;

			Evenement[CurrentEvenement].echeance = Evenement[potVal2].echeance;			  // decrit le nb de fois maximum qu'un evt peut avoir lieu
			Evenement[CurrentEvenement].latence = Evenement[potVal2].latence;			  // décrit à partir de quand l'event aura réellement lieu
			Evenement[CurrentEvenement].fugace = Evenement[potVal2].fugace;				  // si YES, qd compteur arrive à échéance, l'evt est supprimé
			Evenement[CurrentEvenement].seqUsedAsList = Evenement[potVal2].seqUsedAsList; // TODO DEBUG : mettre àjour initialisation, delete et save
		}
		break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 275:
		// SUSPEND ALL Events
		{

			for (byte i = 0; i < NbEvenementMax; i++)
			{

				Evenement[i].actif = false;
			}
		}
		break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 276:
		// PANIC
		{
			resetMidiPanic();
		}

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 277: // MIRROR Extended
	{
		unsigned long foldtime;
		long calcultime; // doit parfois être négatif !!!!

		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				calcultime = (((long)layerLength[i] * (long)potVal2) / (long)100);
				foldtime = (signed int)(calcultime); // on doit pouvoir se passer de la variable foldtime en fait !
				if (potVal2 > 50)
				{
					layerLength[i] = 2 * foldtime;
				}

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{ // va parcourir la mélodie pointée ( Layer 1 à 4 : potVal3 )
					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					// d'abord on fait le ménage : on supprime les notes commençant après foldtime
					if (tempNote.startNote > foldtime)
					{
						tempNote.note = 0;
						tempNote.killnote = true;
						// si on veut on devrait compléter le reste des datas, mais faisons vite
					}
					ZEROSetNoteInPile(pInitial[i], j, tempNote);
				}

				// ensuite les notes qui commencent avant et finissent après ont juste modif de endtime

				for (unsigned int j = 0; j < taille; j++)
				{ //
					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					if ((tempNote.note != 0) && (tempNote.startNote < foldtime) && (tempNote.endNote > foldtime))
					{

						tempNote.endNote = foldtime + (foldtime - tempNote.startNote);
						// melodyMatrixTime[i][j][MMTendnote] = constrain ( melodyMatrixTime[i][j][MMTendnote], 0, layerLength[i] )  ;
						// fixStartEndNote(i, j);
						pileNote_fixStartEndNote(tempNote);
						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
					else
						// enfin les notes qui commencent avant et finissent avant -> nouvelle note.

						if ((tempNote.note != 0) && (tempNote.startNote < foldtime) && (tempNote.endNote < foldtime))
					{
						calcultime = (long)tempNote.endNote;
						tempNote.endNote = (unsigned long)((long)foldtime + ((long)foldtime - (long)tempNote.startNote));
						// melodyMatrixTime[i][j_35][MMTendnote] = constrain ( melodyMatrixTime[i][j_35][MMTendnote], 0, layerLength[i] )  ;

						tempNote.startNote = (unsigned long)((long)foldtime + (long)foldtime - (long)tempNote.startNote);
						// melodyMatrixTime[i][j_35][MMTstartnote] = constrain ( melodyMatrixTime[i][j_35][MMTstartnote], 0, layerLength[i] )  ;
						ZEROaddNewNotetoPile(pInitial[i], tempNote);
					}
				}
			}
		}
	}

	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 278:
		// COMPTEUR DISPLAY+SET Event
		{
			Evenement[CurrentEvenement].compteur = potVal3;
		}
		break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 279:
		// SET Latence Event
		{
			Evenement[CurrentEvenement].latence = potVal3;
		}
		break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 280:
		// SET Echeance Event
		{
			Evenement[CurrentEvenement].echeance = potVal3;
		}
		break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 281:
	{ // EVENT ON/OFF ECHEANCE
		Evenement[CurrentEvenement].fugace = !Evenement[CurrentEvenement].fugace;
	}
	break;

	case 282:
	{
		// LOAD ALL EVENTS from one FILE

		// LoadAllEvents(name);
		loadLocal2(false, true, false, false);
	}

	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 283: // FIDELITY whole layer
	{

		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				// vaut-il mieux faire 1 ligne avec les 2 ?
				// à tester dans un sketch où on augmente les valeurs

				fidelityWholeLayer[i] = potVal3;
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 284: // Numerote Transfo
	{
		// le potard indique combien de secondes avant auto-modifcation

		numeroteTransfo = !numeroteTransfo;
	}
	break;

	case 285:
	{ // Save ALL SEQUENCES
		// saveAllSequences();
		saveLocal(false, true, false,
				  false, false, false,
				  false, false, false);
	}
	break;

	case 286:
	{
		// LOAD ALL SEQUENCES from one FILE
		/// LoadAllSequences(name);
		loadLocal2(false, true, false, false);
	}
	break;

	case 287:
	{ // Save ALL CCMOD
		saveLocal(false, false, false,
				  true, false, false,
				  false, false, false);
	}
	break;

	case 288:
	{
		// LOAD ALL CCMOD
		loadLocal2(false, false, false, true);
	}
	break;

	case 289:
	{ // Save ALL Cell
		// saveAllCell();
		saveLocal(false, false, false,
				  false, true, false,
				  false, false, false); // = Save CELL seulement
	}
	break;

	case 290:
	{
		// LOAD ALL Cell
		loadLocal2(false, false, false, false, true); // = Load CELL seulement
	}
	break;

	case 291:
	{
		// if the file opened okay, write to it:
		if (myFile.isOpen())
		{

			NameTransfo = String153 + F(".txt");

			char charmant[21];
			NameTransfo.toCharArray(charmant, 20);

			bool ren = myFile.rename(charmant);

			myFile.close();

			measureSDFileCount();
			buffer_and_HARD_setCursor(0, 0);
			if (ren == true)
			{
				buffer_and_HARD_print(F("**done**"));
			}
			else
			{
				buffer_and_HARD_print(F("**argh**"));
			}
			delay(2000);
		}
		else
		{
			buffer_and_HARD_setCursor(0, 0);
			buffer_and_HARD_print(F("Errorrr changing "));
			delay(1000);
			return;
		}
	}
	break;

	case 292:
	{
		myFile.close();
		myFile = SDcard.open(name, FILE_WRITE);

		// if the file opened okay, write to it:
		if (myFile.isOpen())
		{

			// bool ren = myFile.remove( );
			myFile.remove(); // 1C86

			myFile.close();

			measureSDFileCount();
			buffer_and_HARD_setCursor(0, 0);
			buffer_and_HARD_print(F("**done**"));
		}
		else
		{
			buffer_and_HARD_setCursor(0, 0);
			buffer_and_HARD_print(F("Errorrr"));
			delay(1000);
			return;
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 293: // Octaver
	{
		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				// clean-up, on met le bit 2 à 0
				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				// vaut-il mieux faire 1 ligne avec les 2 ?
				// à tester dans un sketch où on augmente les valeurs

				for (unsigned int j = 0; j < taille; j++)
				{ // va parcourir la mélodie pointée ( Layer 1 à 4 : potVal3 )
					// if ((melodyMatrix[i][j][MMdata2] >> 2 & 0001) != 1)
					{
						pileNote tempNote;
						tempNote = ZEROgetNoteFromPile(pInitial[i], j);

						if (random(0, 100) < octaverPercent)
						{
							// int newPitch = tempNote.note + (potVal2 * 12);
							// tempNote.not= limiterForPitch(newPitch);
							// modifyPitchADD(tempNote, (potVal2 * 12)) ;
							tempNote.note = modifyPitchUniversal(tempNote, tempNote.note, (potVal2 * 12));
							int newVelocite = (int)(tempNote.velocity * octaverRatioNum / octaverRatioDen);
							tempNote.velocity = limiter(newVelocite, 0, 127);

							// melodyMatrix[i][t43][MMdata] = melodyMatrix[i][j][MMdata];
							// melodyMatrix[i][t43][MMdata2] = (melodyMatrix[i][j][MMdata2] ^ B00000111);
							// bit 0 et 1 sont 1 = played and silented pour que la note ne soit pas rejouée avant le tour suivant.
							// bit 2 sont 1 car c'est une note qui vient d'être créé, elle ne doit pas être re-ping-pong-ée

							pileNote_fixStartEndNote(tempNote);
							ZEROaddNewNotetoPile(pInitial[i], tempNote);
						}
						// insérer plus tard une routine cleanup pour les durées trop longues qui font déborder
					}
					// clean-up, on met le bit 2 à 0
				}
			}

			fuseIdenticalNotes(i);
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 294: // %Notes pour Octaver
	{
		octaverPercent = potVal2;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 295: // Ratio vel octaver
	{
		octaverRatioNum = extractNumRatio(potVal2, 10, 10);
		octaverRatioDen = extractDenRatio(potVal2, 10, 10);
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 296:
	{ // FUNKY VEL pour midi player

		// velocité +- 100
		FunkyVel = !FunkyVel; // ON ou OFF
		// FunkyVel = true ;

		FunkyVelValueMIDIPlayer = potVal2;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 297:
	{ // ShiftR  Sequence
		shiftRightSequence(sequence[potVal2]);
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 298:
	{ // ShiftL  Sequence
		shiftLeftSequence(sequence[potVal2]);
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 299:
	{ // Randomize  Sequence
		scrambleSequence(sequence[potVal2]);
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 301:
	{ // InvertV   Sequence ou plutôt Mirror
		mirrorSequence(sequence[potVal2]);
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 302:
	{ // RevertH   Sequence
		revertSequence(sequence[potVal2]);
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 304:
	{ // Double   Sequence
		doubleSequence(sequence[potVal2]);
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 305: // Liste Event : which sequence is used as a list
	{
		Evenement[CurrentEvenement].seqUsedAsList = potVal3;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 306:
	{ // Soft Mute
		softMute = !softMute;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 303:
		// Replicate Sequence
		{

			sequence[currentSequence].tailleSeq = sequence[potVal2].tailleSeq;

			for (byte i = 0; i < tailleMaxTableauSequence; i++)
			{
				sequence[currentSequence].tableauSeq[i] = sequence[potVal2].tableauSeq[i];
			}
			// sequence[currentSequence].modeSeq = sequence[potVal2].modeSeq;
			sequence[currentSequence].actif = false;
			sequence[currentSequence].modeSeq = sequence[potVal2].modeSeq;
			sequence[currentSequence].jitterMinUP = sequence[potVal2].jitterMinUP;
			sequence[currentSequence].jitterMinDOWN = sequence[potVal2].jitterMinDOWN;
			sequence[currentSequence].jitterMaxDOWN = sequence[potVal2].jitterMaxDOWN;
			sequence[currentSequence].jitterStaticMin = sequence[potVal2].jitterStaticMin;
			sequence[currentSequence].jitterStaticMax = sequence[potVal2].jitterStaticMax;
			sequence[currentSequence].sizeRANDOMFRAG = sequence[potVal2].sizeRANDOMFRAG;
			sequence[currentSequence].repeatRANDOMFRAG = sequence[potVal2].repeatRANDOMFRAG;
			sequence[currentSequence].compteLayer = sequence[potVal2].compteLayer;
			sequence[currentSequence].action = sequence[potVal2].action;
			sequence[currentSequence].CC = sequence[potVal2].CC;
			sequence[currentSequence].channel = sequence[potVal2].channel;
			sequence[currentSequence].layer = sequence[potVal2].layer;
		}
		break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 300:
		// Permute Sequence
		{
			if (potVal2 < 0)
			{
				potVal2 = -potVal2;
				for (int i = 0; i < potVal2; i++)
				{
					antiPermuteInt(&sequence[currentSequence].tableauSeq[0], sequence[currentSequence].tailleSeq);
				}
			}
			else
			{
				for (int i = 0; i < potVal2; i++)
				{
					permuteInt(&sequence[currentSequence].tableauSeq[0], sequence[currentSequence].tailleSeq);
				}
			}
		}
		break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 307:
	{
		eduLight = !eduLight;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 308:
	{
		limitEduLight = potVal2;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 309: // Scalpel Channel out ( efface juste les notes d'un certain channel )
	{

		int channelToKill = potVal2 - 1; // encodage machine = 1 de moins que l'affichage

		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{

					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					if (tempNote.channel == channelToKill)
					{
						putNoteBinNoteA(tempNote, emptyNote);
						tempNote.killnote = true;
						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
				}
			}
		}
	}
	break;

		// pas de 310 : BPM Sniffer = affichage pur

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94 : n'a plus de raison d'être.   vidé.   remplacé par un update
		/////////////////////////////////////////////////////////////////////////////////////

	case 311: // Editha
	{
		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{
				// simplePartialSortZeroNotesv2(i);
				ZEROtreatPileComplete(pInitial[i]); // supprime les notes inutiles
				ZEROupdatePileTimeToPlayMODEDIRECT(i);
				// va à la fois fixer les début et fins, mais aussi mettre à jour la prochaine fois ou doit jouer
				ZEROtreatPileComplete(pInitial[i]); // supprime les notes inutiles
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94 : n'a plus de raison d'être.   vidé
		/////////////////////////////////////////////////////////////////////////////////////

	case 312: // Axe 12 Tones
	{

		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{

				if (hasSymmetricAxe(gammeActuelle[i], modeActuelle[i]) == true)
				{
					int axe = 0;
					axe = getSymmetricAxe(gammeActuelle[i], modeActuelle[i]);

					unsigned int taille = mesureTaillePileComplete(pInitial[i]);

					// on a dès lors l'axe ( en quart de ton ) qui est symmétrique
					for (unsigned int j = 0; j < taille; j++)
					{
						pileNote tempNote;
						tempNote = ZEROgetNoteFromPile(pInitial[i], j);

						byte noteDepart = 0;
						noteDepart = tempNote.note;
						byte noteFin = 0;
						noteFin = getSymmetricNoteOfAThroughAxeBQuartTon(noteDepart, axe);
						// si la note vaut 0, il renvoie 0 ;
						tempNote.note = noteFin;

						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94 : n'a plus de raison d'être.   vidé
		/////////////////////////////////////////////////////////////////////////////////////

	case 313: // RealTransposerGamme
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{

				// connaître la gamme actuelle
				// connaître la gamme vers laquelle on dirige = potVal2
				//
				int diff = 0;
				diff = potVal2;
				diff = diff - (int)gammeActuelle[i];
				// exemple : C -> A : diff = 9 - 0 = + 9 demi-tons
				// exemple : A -> C : diff = 0 - 9 = - 9 demi-tons
				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{
					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					int noteDepart = 0;
					noteDepart = tempNote.note;
					int noteFin = 0;
					noteFin = limiterForPitch(noteDepart + diff);
					// si la note vaut 0, il renvoie 0 ;
					// modifyPitchSET(tempNote,noteFin);
					tempNote.note = modifyPitchUniversal(tempNote, noteFin, 0);

					ZEROSetNoteInPile(pInitial[i], j, tempNote);
				}

				gammeActuelle[i] = potVal2;
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94 : n'a plus de raison d'être.   vidé
		/////////////////////////////////////////////////////////////////////////////////////

	case 314: // Fix To Closest Random Scale and Mode
	{
		float plusHautScore = 0;
		int nbDeHiScores = 0;
		int choixGamme = 0;
		int choixMode = 0;

		for (int k = 0; k < 12; k++)
		{
			// for (int j = 0; j < nbDeModesConnus; j++)
			for (int j = 0; j <= 12; j++) // MODIF 2C28 car avait tendance à accepter dodeca et messiaen, à éviter

			{

				// cout << " inclus dans gamme \t" << GiveNameOfNote(k) <<  GiveNameOfMode(j) << "\t : " << giveInclusion(k,j) << "%" << endl ;

				if (giveInclusion(k, j) == plusHautScore)
				{
					nbDeHiScores++;
				}

				if (giveInclusion(k, j) > plusHautScore)
				{
					plusHautScore = giveInclusion(k, j);
					nbDeHiScores = 1;
				}
			}
		}

		int choixHiScore;
		// choixHiScore= rand() % nbDeHiScores + 1 ;
		choixHiScore = random(1, nbDeHiScores + 1);

		int compteur = 1;

		for (int k = 0; k < 12; k++)
		{
			// for (int j = 0; j < nbDeModesConnus; j++)
			for (int j = 0; j <= 12; j++)
			{
				if (giveInclusion(k, j) == plusHautScore)
				{

					if (compteur == choixHiScore)
					{
						choixGamme = k;
						choixMode = j;
					}

					compteur++;

					// cout << "[" << compteur++ << "/" << nbDeHiScores << "] " << " inclus dans gamme \t" << GiveNameOfNote(k) << " "  << GiveNameOfMode(j) << "\t : " << giveInclusion(k,j) << "%" << endl ;
				}
			}
		}

		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				gammeActuelle[i] = choixGamme;
				modeActuelle[i] = choixMode;
				newHarmonize(i, gammeActuelle[i], modeActuelle[i]);
				// il parcourt automatiquemet les layers actives ! FAUX depuis 2C12 : il s'occupe de la layer indiquée
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C95
		/////////////////////////////////////////////////////////////////////////////////////

	case 315: // RAndom All Triad Change
	{
		// int degreA = rand()%7 + 1 ;
		// int degreB = rand()%7 + 1 ;
		//
		int degreA = random(1, 8);
		int degreB = random(1, 8);

		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				transposeAllFromDegreeAtoDegreeB(i, degreA, degreB, gammeActuelle[i], modeActuelle[i]);
				// la fonction parcourt elle-même les layers actifs
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C95
		/////////////////////////////////////////////////////////////////////////////////////

	case 316: // ONE CONTIGUOUS RAndom   Triad Change
	{
		// int degreA = rand()%7 + 1 ;
		// int degreB = rand()%7 + 1 ;
		//
		int degreA = random(1, 8);
		int degreB = random(1, 8);
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				transposeOneRandomTriad(i, degreA, degreB, gammeActuelle[i], modeActuelle[i], potVal2);
				// la fonction parcourt elle-même les layers actifs
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C95
		/////////////////////////////////////////////////////////////////////////////////////

	case 317: // ONE SPACED  RAndom   Triad Change
	{
		// int degreA = rand()%7 + 1 ;
		// int degreB = rand()%7 + 1 ;
		//
		int degreA = random(1, 8);
		int degreB = random(1, 8);
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				transposeOneRandomSpacedTriad(i, degreA, degreB, gammeActuelle[i], modeActuelle[i], potVal2);
				// la fonction parcourt elle-même les layers actifs
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C95
		/////////////////////////////////////////////////////////////////////////////////////

	case 318: // unFlatten Triads
	{
		// int degreA = rand()%7 + 1 ;
		// int degreB = rand()%7 + 1 ;
		//
		int degreA = random(1, 8);
		int degreB = random(1, 8);
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				unflattenTriad(i, degreA, degreB, gammeActuelle[i], modeActuelle[i]);
				// la fonction parcourt elle-même les layers actifs
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C95
		// modif 2C26
		/////////////////////////////////////////////////////////////////////////////////////

	case 319: // center around C60
	{
		for (int i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{
					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					if (tempNote.note != 0 && tempNote.type == 0x90) // modif 2C26
					{
						// modifyPitchSET(tempNote, 60 + tempNote.note % 12 ) ;
						tempNote.note = modifyPitchUniversal(tempNote, 60 + tempNote.note % 12, 0);
						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C95
		/////////////////////////////////////////////////////////////////////////////////////

	case 320: // Real Mode Transposer
	{

		// 1. connaître le mode de départ
		// 2. connaître la gamme de départ
		// 3. pour une note donnée, savoir quelle est sa place dans la gamme G1, mode M1
		// 4. il faut également savoir de quelle octave il s'agit pour cette note : O1
		// 5. de là calculer quel est le pitch de la note dans l'octave 01, G2, M2
		// à développer donc :
		// giveOctave(p,G1)
		// givePlaceNoteAdansGammeBModeC
		// givePitchOctaveAGammeBModeCPlaceD

		int O1 = 0;
		int G1 = 0;
		int M1 = 0;

		// int G2 = 0;
		int M2 = 0;

		M2 = potVal2;

		for (int i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				G1 = gammeActuelle[i];
				M1 = modeActuelle[i];

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{

					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					if (tempNote.note != 0)
					{
						int N1 = 0;
						int N2 = 0;
						int P1 = 0;

						N1 = tempNote.note;

						// 3. pour une note donnée, savoir quelle est sa place dans la gamme G1, mode M1

						P1 = getPlaceNoteAdansGammeCmodeD(N1, G1, M1);

						// 4. il faut également savoir de quelle octave il s'agit pour cette note : O1

						O1 = (tempNote.note - (gammeActuelle[i] % 12)) / 12;
						// on calcule ainsi le C sous la note de la gammeActuelle et sa position d'octave

						// 5. de là calculer quel est le pitch de la note dans l'octave 01, G2, M2

						N2 = getNotePlaceAdansGammeCmodeDoctaveE(P1 - 1, G1, M2, O1);

						// à développer donc :
						// giveOctave(p,G1)
						// givePlaceNoteAdansGammeBModeC
						// givePitchOctaveAGammeBModeCPlaceD

						// modifyPitchSET(tempNote, N2) ;
						tempNote.note = modifyPitchUniversal(tempNote, N2, 0);
						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
				}

				modeActuelle[i] = M2;
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C95
		/////////////////////////////////////////////////////////////////////////////////////

	case 321: // Axe 12 Tones -> SuperSymmetry ( symétrise toute la mélodie, pas par octave )
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				if (hasSymmetricAxe(gammeActuelle[i], modeActuelle[i]) == true)
				{
					int axe = 0;
					axe = getSymmetricAxe(gammeActuelle[i], modeActuelle[i]);

					int meanPitch = 0;
					meanPitch = meanPitch_MultipleLayer(activeLayer);
					int basicC = 0;
					basicC = meanPitch - (meanPitch % 12);
					int totalAxe = 0; // attention, quart de tons
					totalAxe = (basicC + gammeActuelle[i]) * 2 + (axe);

					unsigned int taille = mesureTaillePileComplete(pInitial[i]);

					// on a dès lors l'axe ( en quart de ton ) qui est symmétrique
					for (unsigned int j = 0; j < taille; j++)
					{

						pileNote tempNote;
						tempNote = ZEROgetNoteFromPile(pInitial[i], j);

						int noteDepart = 0;
						noteDepart = tempNote.note;
						int noteFin = 0;
						noteFin = totalAxe - (noteDepart * 2 - totalAxe);
						noteFin = noteFin / 2;
						// si la note vaut 0, il renvoie 0 ;
						// modifyPitchSET(tempNote, noteFin ) ;
						tempNote.note = modifyPitchUniversal(tempNote, noteFin);
						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C95
		/////////////////////////////////////////////////////////////////////////////////////

	case 322: // Comb
	{
		// attention, ici on passe j d'abord, pour ne pas comber une layer en entier puis la suivante
		// on va donc traiter une note de chaque layer l'une après l'autre. L1 L2 L3 .. L1 L2 L3 ..

		// 1C94 ; je crois qu'on devra d'abord chercher le nbNotes max, pour les 4 layers,
		// puis qd j est trop gd pour son propre layer, faire skip

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

		for (unsigned int j = 0; j < tailleMax; j++)
		{
			for (int i = 0; i < nbLayer; i++)
			{
				if (isLayerActive(i) && (j < taille[i]))
				// on a auparavant déterminé jusqu'où devait aller j au max, mais on le limite pour cette layer
				{
					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					int noote = tempNote.note;

					if (noote != 0)
					{
						int direction = 0;
						direction = random(2) == 0 ? 1 : -1; // choisit soit +1, soit -1

						while ((howManyNotesAinLayersOtherThanB(noote, i) > 0) || (isNoteInScaleMode(noote, gammeActuelle[i], modeActuelle[i]) == false))
						{
							noote = tempNote.note + direction;
							// modifyPitchSET(tempNote,noote);
							tempNote.note = modifyPitchUniversal(tempNote, noote);
							ZEROSetNoteInPile(pInitial[i], j, tempNote);
						}
					}
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C95
		/////////////////////////////////////////////////////////////////////////////////////

	case 323:
	{
		Comb2();
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 324:
		// Inspector Note
		{

			// d'abord déterminer le premier LAYER activé

			byte firstOne = 10;
			for (int i = 3; i >= 0; i--)
			{
				if (isLayerActive(i))
				{
					firstOne = i;
				} // va choisir la première layer activée dans l'ordre 1234     ex : -23- donnera 2    1--4 donnera 1
				  // si on a la valeur 10 ça veut dire qu'aucune layer n'est activée, on affichera rien
			}

			if (firstOne <= 3)
			{
				// on a une valeur de Layer
				// on doit maintenant examiner la pile liée à ce layer
				// on commence par déterminer la taille de cette pile
				// ensuite on affiche le n° de note défini par PotVal2, si ce nb est <= taille

				// pileNote *pileExaminee ;

				int taillePile;
				taillePile = mesureTaillePileComplete(pInitial[firstOne]);

				if (potVal2 > taillePile)
				{
					buffer_and_HARD_setCursor(0, 3);
					buffer_and_HARD_print("value TOO HIGH ");

					buffer_and_HARD_setCursor(0, 4);
					buffer_and_HARD_print("MAX taille=");
					buffer_and_HARD_print(taillePile);
					buffer_and_HARD_print("   ");

					buffer_and_HARD_setCursor(0, 5);
					buffer_and_HARD_print("           ");
				}
				else
				{
					buffer_and_HARD_setCursor(0, 3);
					buffer_and_HARD_print("layer");
					buffer_and_HARD_print(firstOne + 1);
					buffer_and_HARD_setCursor(0, 4);
					buffer_and_HARD_print("taille=");
					buffer_and_HARD_print(taillePile);
					buffer_and_HARD_print("   ");

					// pileExaminee = pInitial[firstOne] ;

					pileNote showIt;
					showIt = giveNthNoteofPile(pInitial[firstOne], potVal2);

					buffer_and_HARD_setCursor(0, 5);
					// u8x8.clearLine(5);
					buffer_clearLine(5);
					buffer_and_HARD_print("CH");
					buffer_and_HARD_print(showIt.channel + 1);
					buffer_and_HARD_print(" N");
					buffer_and_HARD_print(showIt.note);
					buffer_and_HARD_print(" V");
					buffer_and_HARD_print(showIt.velocity);

					buffer_and_HARD_setCursor(0, 6);
					// u8x8.clearLine(6);
					buffer_clearLine_HARD(6);
					buffer_and_HARD_print("S");
					buffer_and_HARD_print(showIt.startNote);
					buffer_and_HARD_print(" E");
					buffer_and_HARD_print(showIt.endNote);

					buffer_and_HARD_setCursor(0, 7);
					// u8x8.clearLine(7);
					buffer_clearLine_HARD(7);
					buffer_and_HARD_print("L");
					buffer_and_HARD_print(layerLength[firstOne]);
				}
			}
		}
		break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 325:
	{

		byte firstOne = 10;
		for (int i = 3; i >= 0; i--)
		{
			if (isLayerActive(i))
			{
				firstOne = i;
			} // va choisir la première layer activée dans l'ordre 1234     ex : -23- donnera 2    1--4 donnera 1
			  // si on a la valeur 10 ça veut dire qu'aucune layer n'est activée, on affichera rien
		}

		if (firstOne <= 3)
		{
			// on a une valeur de Layer
			// on doit maintenant examiner la pile liée à ce layer
			// on commence par déterminer la taille de cette pile
			// ensuite on affiche le n° de note défini par PotVal2, si ce nb est <= taille

			// pileNote *pileExaminee;

			int taillePile;
			taillePile = mesureTaillePileComplete(pInitial[firstOne]);

			if (potVal2 > taillePile)
			{
				buffer_and_HARD_setCursor(0, 3);
				buffer_and_HARD_print("value TOO HIGH ");

				buffer_and_HARD_setCursor(0, 4);
				buffer_and_HARD_print("MAX taille=");
				buffer_and_HARD_print(taillePile);
				buffer_and_HARD_print("   ");

				buffer_and_HARD_setCursor(0, 5);
				buffer_and_HARD_print("           ");
			}
			else
			{

				deleteNoteFromPile(pInitial[firstOne], potVal2);

				buffer_and_HARD_setCursor(0, 5);
				buffer_and_HARD_print("DELETED      ");
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 326:
	{
		vel2Vol[potVal2 - 1] = !vel2Vol[potVal2 - 1];
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 327:
	{
		// delayLayer[potVal2 - 1] = !delayLayer[potVal2 - 1];
		// remplacer par Active Layer ??? TODO DEBUG
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				delayLayer[i] = !delayLayer[i];
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 328: // définition du Nb de Delais, pour chaque layer
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				nbDeDelais[i] = potVal2;
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 329: // définition du space en Msec pour chaque Layer
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				nbDeMsecPourDelais[i] = potVal2; // jusque BLACK 10
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 330: // définition du Drop pour chaque Layer
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				dropPourDelais[i] = potVal2;
			}
		}
	}
	break;

		/*
	/////////////////////////////////////////////////////////////////////////////////////
	// Version Pile NOTE
	// 1C94
	/////////////////////////////////////////////////////////////////////////////////////

	case 331: // définition du Down pour chaque Layer
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				downPourDelais[i] = potVal2;
			}
		}
	}
	break;
*/

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Kannibal
		// 2C19 basé sur 468, mélangé avec le vieux 331
		/////////////////////////////////////////////////////////////////////////////////////

	case 331: // bitMapDende
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				affiche = nulString;
				if (potVal2 >= 0)
				{
					downPourDelais[i] = potVal2;
				}
				else
				{
					downPourDelais[i] = potVal2; // aussi, mais valeur négative

					// int t = giveNextValue(sequence[-potVal2 - 1], sequence[-potVal2 - 1].compteur)
					// sequence[-potVal2 - 1].compteur++;
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 332: // définition du random pour chaque Layer
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				randomPourDelais[i] = potVal2;
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 2C12
		/////////////////////////////////////////////////////////////////////////////////////

	case 333: // pousse en mode REPEATER
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				if (potVal2 >= 0)
				{
					repeaterNb[i] = potVal2;
					// le délais est défini en 16e de layerLength
					unsigned long newDelay = 0;
					newDelay = (long)layerLength[i] * (long)potVal2 / (long)32;
					nbDeMsecPourDelais[i] = (int)newDelay;

					// dropPourDelais[i] = 100;
					// downPourDelais[i] = 100;
				}
				else
				{
					repeaterNb[i] = potVal2;
					int valueRepeater = giveNextValue(sequence[-potVal2 - 1], sequence[-potVal2 - 1].compteur);

					// le délais est défini en 16e de layerLength
					unsigned long newDelay = 0;
					newDelay = (long)layerLength[i] * (long)valueRepeater / (long)32;
					nbDeMsecPourDelais[i] = (int)newDelay;
					sequence[-potVal2 - 1].compteur++;
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 2C12
		/////////////////////////////////////////////////////////////////////////////////////

	case 334: // bitMap
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{

				affiche = nulString;
				if (potVal2 >= 0)
				{
					seqBitMapDelay[i] = potVal2;
					int nbCases = 8;
					// int nombre = plusGrandNombreAvecAbits(nbCases);

					int t = potVal2;

					for (int j = 0; j < nbCases; j++)
					{
						int k = 0;
						k = (t >> j) & 1;
						bitMapDelay[i][7 - j] = k;
					}
				}
				else
				{
					seqBitMapDelay[i] = potVal2;

					int nbCases = 8;
					// int nombre = plusGrandNombreAvecAbits(nbCases);

					int t = giveNextValue(sequence[-potVal2 - 1], sequence[-potVal2 - 1].compteur);

					for (int j = 0; j < nbCases; j++)
					{
						int k = 0;
						k = (t >> j) & 1;
						bitMapDelay[i][7 - j] = k;
					}

					sequence[-potVal2 - 1].compteur++;
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 336:
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				modeDeletionWhenExtraNotes[i] = (byte)potVal2;
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 337:
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				nbMaxDeNotesParLayer[i] = (byte)potVal2;
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 338:
	{
		valueC60 = potVal2;

		PlaynoteOn_(0x90, valueC60, 100);

		addNewNotetoPileONCE(pointeurInitial, 0, valueC60,
							 0, nowMillis + 100, 0x80, 1);
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version PILE de note
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 339: //  Fit current layer to Euclidian Rhythm
	{
		String Necklace = nulString;
		;
		String usualA = F("1");
		String usualB = F("0");

		byte ff(0); // 1C94 mauvaise initialisation -> gardée pour la version BUGGY

		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);
				unsigned long euclidianMatrixTime[taille] = {0};

				Necklace = Euclidian(kNecklace, mNecklace);
				Necklace = StringShift(Necklace, ShiftNecklace);

				// byte ff(0); // 1C94 initialisation était au mauvais endroit
				// TODO donnait qd mêem un résultat intéressant -> à conserver ? KEEP BUG

				for (byte j = 0; j < mNecklace; j++)
				{
					// on va passer en revue les m positions du Necklace

					if (Necklace[j] == usualA[0])
					{
						// la valeur définie comme un pulse, normalement "1"
						euclidianMatrixTime[ff] = (unsigned long)((float)layerLength[i] * (float)j / (float)mNecklace) + 1;
						ff++;
					}
				} // fin de création du necklace

				// on a donc un euclidianMatrixTime pour la layer active
				// on doit comparer l'euclidianMatrixTime et le melodyMatrixTime de cette layer
				//
				// on prend une première note
				// on parcourt la liste des moments dans euclidian et on repère celui qui est le plus proche
				// on calcule différence entre les deux
				// on applique cela à début et à fin de note
				//
				// on répète ensuite la procédure

				for (unsigned int j = 0; j < taille; j++)
				{

					unsigned int smallest(0);
					bool foundOne(false);
					byte mSmallest(0);

					pileNote tempNote;

					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					for (byte m = 0; m < kNecklace; m++)
					{

						unsigned long differenceEuc;

						differenceEuc = abs(tempNote.startNote - euclidianMatrixTime[m]);

						if (foundOne == false)
						{
							smallest = differenceEuc;
							foundOne = true;
							mSmallest = m;
						}
						if (differenceEuc < smallest)
						{
							smallest = differenceEuc;
							mSmallest = m;
						}
					}

					// on a maintenant un mSmallest qui indique quel moment de l'euclidian matrix est le plus proche de la note j

					unsigned long difference = tempNote.startNote - euclidianMatrixTime[mSmallest];
					// on calclue la différence de façon signée cette fois

					tempNote.startNote += difference; // Etait un bug, conservé pour cette version
					tempNote.endNote += difference;	  // était un bug, conservé pour cette version !

					pileNote_fixStartEndNote(tempNote, 0);

					ZEROSetNoteInPile(pInitial[i], j, tempNote);

				} // fin de la boucle j de parcours des notes du layer

			} // fin de boucle layer Active
		}	  // fin de boucle i pour layers

		// il faudrait faire un cleanup
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version PILE de note
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 340: //  Sauve Layer vers Mémoire
		// option 1 : sauver les layers sélectionnés vers mémoire sélectionnée
		// option 2 : sauvers les layers sélectionnés vers mémoires consécutives, 1 pour chaque layer sélect

		{
			byte compteur = 0;

			for (byte i = 0; i < nbLayer; i++)
			{
				if (isLayerActive(i))
				{
					layerLength[potVal2 - 1 + compteur + 4] = layerLength[i];
					// doit précéder la copie
					ZEROcopyLayerAtoMemoryB(i, potVal2 - 1 + compteur);
					compteur++;
					currentMem = potVal2 - 1 + compteur;
				}
			}
		}
		break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version PILE de note
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 341: //  rappatrie de mémoire vers layer
		// option 1 : sauver les layers sélectionnés vers mémoire sélectionnée
		// option 2 : sauvers les layers sélectionnés vers mémoires consécutives, 1 pour chaque layer sélect

		{
			byte compteur = 0;

			for (byte i = 0; i < nbLayer; i++)
			{
				if (isLayerActive(i))
				{
					int memoireVisee = pFluxStart + potVal2 - 1;
					unsigned int taille = mesureTaillePileComplete(pInitial[memoireVisee]);

					if (taille > 0)
					{
						layerLength[i] = layerLength[potVal2 - 1 + compteur + 4];
						// doit précéder la copie, pour que les calculs soient OK
						ZEROcopyMemoryAtoLayerB(potVal2 - 1 + compteur, i);
						ZEROupdatePileTimeToPlayMODEDIRECT(i);
					}
					compteur++;
				}
			}
		}
		break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version PILE de note
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 342: //  copie de méoire source vers méoire
		// option 1 : sauver les layers sélectionnés vers mémoire sélectionnée
		// option 2 : sauvers les layers sélectionnés vers mémoires consécutives, 1 pour chaque layer sélect

		{
			ZEROcopyMemoryAtoMemoryB(sourceMem, potVal2 - 1);
		}
		break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version PILE de note
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 343: //  set mémoire source
		// option 1 : sauver les layers sélectionnés vers mémoire sélectionnée
		// option 2 : sauvers les layers sélectionnés vers mémoires consécutives, 1 pour chaque layer sélect

		{
			sourceMem = potVal2 - 1;
		}
		break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version PILE de note
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 344: //  sauve les UNDO successifs
		// option 1 : sauver les layers sélectionnés vers mémoire sélectionnée
		// option 2 : sauvers les layers sélectionnés vers mémoires consécutives, 1 pour chaque layer sélect

	{
		// byte compteur = 0;

		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				ZEROcopyLayerAtoUndo(i);
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version PILE de note
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 345: //  UNDO
		// option 1 : sauver les layers sélectionnés vers mémoire sélectionnée
		// option 2 : sauvers les layers sélectionnés vers mémoires consécutives, 1 pour chaque layer sélect

	{
		// byte compteur = 0;

		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				ZEROcopyUndoToLayer(i);
				ZEROupdatePileTimeToPlayMODEDIRECT(i);
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 346:
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				pitchableLayer[i] = !pitchableLayer[i];
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C94
		/////////////////////////////////////////////////////////////////////////////////////

	case 347:
	{
		pitchableChannel[potVal2 - 1] = !pitchableChannel[potVal2 - 1];
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C96
		// SI ON, les notes entrantes sont effacées de la pile.
		/////////////////////////////////////////////////////////////////////////////////////

	case 348:
	{
		eraseRecording = !eraseRecording;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C96
		// SI ON, les notes entrantes sont effacées de la pile.
		/////////////////////////////////////////////////////////////////////////////////////

	case 349:
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				dynamicSwing[i] = !dynamicSwing[i];
				valueDynamicSwing[i] = potVal2;
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C98
		// SI ON, les CC sont enregistrés dans le layer actif
		/////////////////////////////////////////////////////////////////////////////////////

	case 350:
	{
		recAutomation = !recAutomation;
		automationStep = potVal2;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C98 Automaton
		/////////////////////////////////////////////////////////////////////////////////////

	case 351:
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				baroccoLayer[i] = !baroccoLayer[i];
			}
		}
	}
	break;

	case 352:
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				baroccoChosenSeq[i] = potVal2;
			}
		}
	}
	break;

	case 353: // définition du space en Msec pour chaque Layer
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				nbDeMsecPourBarocco[i] = potVal2;
			}
		}
	}
	break;

	case 354: // définition du Drop pour chaque Layer
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				dropPourBarocco[i] = potVal2;
			}
		}
	}
	break;

	case 355: // définition du random pour chaque Layer
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				randomPourBarocco[i] = potVal2;
			}
		}
	}
	break;

	case 356: // définition du SymH pour Barocco
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				symHBarocco[i] = potVal2;
			}
		}
	}
	break;

	case 357: // définition du Sym Vertical pour Barocco
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				symVBarocco[i] = potVal2;
			}
		}
	}
	break;

	case 358: // définition du Sym Vertical pour Barocco
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				refletBarocco[i] = potVal2;
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C98
		/////////////////////////////////////////////////////////////////////////////////////

	case 359:
	{
		panMicroSamplerOnChannel[potVal2 - 1] = !panMicroSamplerOnChannel[potVal2 - 1];
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C98
		/////////////////////////////////////////////////////////////////////////////////////

	case 360:
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				panMicroSampler[i] = potVal2;
				panMicroSamplerOnLayer[i] = !panMicroSamplerOnLayer[i];
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C98
		/////////////////////////////////////////////////////////////////////////////////////

	case 361:
	{
		pitchMicroSamplerOnChannel[potVal2 - 1] = !pitchMicroSamplerOnChannel[potVal2 - 1];
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C98
		/////////////////////////////////////////////////////////////////////////////////////

	case 362:
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				pitchMicroSampler[i] = potVal2;
				pitchMicroSamplerOnLayer[i] = !pitchMicroSamplerOnLayer[i];
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C99
		/////////////////////////////////////////////////////////////////////////////////////

	case 363: // activate ON OFF NEW Markov
	{

		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				participateNEWMarkov[i] = !participateNEWMarkov[i];
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C99
		/////////////////////////////////////////////////////////////////////////////////////

	case 364: // activate ON OFF NEW Markov LAYER
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				participateLAYERMarkov[i] = !participateLAYERMarkov[i];
			}
		}
	}
	break;

	/////////////////////////////////////////////////////////////////////////////////////
	// Version Pile NOTE
	// 1C99
	/////////////////////////////////////////////////////////////////////////////////////
	case 365: // Send F8
	{
		sendF8 = !sendF8;
		nbBeatsPourLayer1 = potVal2;
		// nombre de BPM en fc de la durée de layer 1
		float nbBpm = 60000 / layerLength[0]; // nbBPM indique combien de fois layer1 dans une minute
		float dureeF8_ = 625000 / nbBpm;
		dureeF8 = (long)((float)dureeF8_ / (float)nbBeatsPourLayer1);

		dureeF8 = ((float)layerLength[0] * (float)1000) / ((float)24 * (float)nbBeatsPourLayer1);
		long longDureeF8 = (long)dureeF8;

		myTimerF8.update(longDureeF8);

		affiche = String(dureeF8);
		affiche = affiche + "--";

		buffer_and_HARD_setCursor(0, 4);
		buffer_and_HARD_print(affiche);
		// delay(1000);
	}

	case 366: // définition du pre Reflet pour Barocco
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				preReflet[i] = potVal2;
			}
		}
	}
	break;

	case 367: // définition du pre  Horizontal  pour Barocco
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				preH[i] = potVal2;
			}
		}
	}
	break;

	case 368: // définition du pre Vertical pour Barocco
		// DEBUG BLACK08 : la valeur n'est pas utilisée !
		{
			for (byte i = 0; i < nbLayer; i++)
			{
				if (isLayerActive(i))
				{
					preV[i] = potVal2;
				}
			}
		}
		break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C99
		/////////////////////////////////////////////////////////////////////////////////////

	case 369:
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				derazerSeqChoice[i] = potVal2;
				derazerActif[i] = !derazerActif[i];
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C99
		/////////////////////////////////////////////////////////////////////////////////////

	case 370:
	{
		derazerOnChannel[potVal2 - 1] = !derazerOnChannel[potVal2 - 1];
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C99
		/////////////////////////////////////////////////////////////////////////////////////

	case 371: // MULTIPLIER
	{
		// potVal2 est le nombre de fois qu'il faut multiplier le layer

		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				long initLength = layerLength[i];
				layerLength[i] = layerLength[i] * (long)potVal2;

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{
					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);
					long initStart = tempNote.startNote;
					long initEnd = tempNote.endNote;

					for (signed int k = 1; k < potVal2; k++)
					{
						if (tempNote.note != 0)
						{
							tempNote.startNote = initStart + k * initLength;
							tempNote.endNote = initEnd + k * initLength;
							// initialement bug intéressant ici !
							ZEROaddNewNotetoPile(pInitial[i], tempNote);
						};
					}
				}
			}
			ZEROupdatePileTimeToPlayMODEDIRECT(i);
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C99
		/////////////////////////////////////////////////////////////////////////////////////

	case 372: // SAVE LOACAL TO SD - 1C99
	{
		saveLocal(true, true, true,
				  true, true, true,
				  true, true, true); // va sauver un fichier .txt avec toutes les infos
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 1C99
		/////////////////////////////////////////////////////////////////////////////////////

	case 373:
	{
		Serial.println("current fileName >" + fileName); // BLACK08

		loadLocal2(); // tout est vrai d'emblée
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C99
		/////////////////////////////////////////////////////////////////////////////////////

	case 374:
	{ // va rajouter une lettre ou en retrancher une à un string
		// il faudra isoler ce qui va suivre pour en faire une fc générale
		// le string est affiché dans le programme,
		// une lettre défile avec le bouton ROTATif
		// qd PUSH, si c'est lettre normale : elle est ajouté au string affiché
		// qd PUSH si c'est le caractère "<" et que taille > 1 alors on coupe le dernier morceau du string

		// potVal2 = BBB ;
		String tempFileName = fileNameSave;
		tempFileName.replace(".txt", "");

		if (ChoixString153.substring(potVal2, potVal2 + 1) != "<")
		{
			tempFileName = tempFileName + ChoixString153.substring(potVal2, potVal2 + 1);
		}
		else
		{
			if (tempFileName.length() > 0)
			{
				tempFileName = tempFileName.substring(0, tempFileName.length() - 1);
			}
		}

		fileNameSave = tempFileName + ".txt";

		// NOTE**** trouver une fc réutilisable, du style : ProcessString (  &String153, &ChoixString153 )

		// NOTE DEBUG si on veut un Easter Egg c'est ici qu'il faut le mettre
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 1C99 - 2ème version multiLayer
		/////////////////////////////////////////////////////////////////////////////////////

	case 375:
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				omitEndValue[i] = potVal2;
				omitEnd[i] = !omitEnd[i];
			}
		}
	}

	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 2C01
		/////////////////////////////////////////////////////////////////////////////////////

	case 376:
	{
		// Affichage mémoire libre, pour Teensy.
		// code trouvé sur pjrc.com
		// affiche += String(freeRam());

		affiche = String(FreeMem());
		printRot2(affiche, 16, 400, 3);
	}

	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 2C01
		/////////////////////////////////////////////////////////////////////////////////////

	case 377:
	{
		// va générer un nom pour fileNameSave aléatoire

		fileNameSave = nulString;
		fileNameSave +=
			letters[random(0, 26)] + letters[random(0, 26)] + letters[random(0, 26)] + "_" + letters[random(26, 36)] + letters[random(26, 36)] + letters[random(26, 36)] + letters[random(26, 36)] + F(".txt");
	}

	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 2C02
		/////////////////////////////////////////////////////////////////////////////////////

	case 378:
	{
		// Transformation de Layer vers Sequence

		int layerChoice = potVal2 - 1;

		// initializeSequence(currentSequence);

		unsigned int taille = mesureTaillePileComplete(pInitial[layerChoice]);

		if (taille > tailleMaxTableauSequence)
		{
			taille = tailleMaxTableauSequence;
		}
		sequence[currentSequence].tailleSeq = taille;
		sequence[currentSequence].compteur = 0;

		for (unsigned int j = 0; j < taille; j++)
		{
			pileNote tempNote;
			tempNote = ZEROgetNoteFromPile(pInitial[layerChoice], j);
			int valeur = tempNote.note;

			sequence[currentSequence].tableauSeq[j] = valeur;
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version PILE de note
		// 2C02
		/////////////////////////////////////////////////////////////////////////////////////

	case 379:
	{ // SENTENCE Rec

		if (statusTapRecIn == 0)
		{
			// vient d'appuyer première fois
			// on va mettre les layers actives à 100 x 60 x 60 secondes
			// et les effacer
			while (Serial3.available()) // BUG trouvé à la version 1C99 ! était resté Serial.available
			{
				Serial3.read(); // on vide le buffer
			}

			for (byte i = 0; i < nbLayer; i++)
			{
				if (isLayerActive(i))
				{
					layerLength[i] = 10 * 60 * 60 * 1000; // BAM, 10 heures d'emblée.
					ZEROdeleteWholePile(pInitial[i]);
				}
			}

			record = true;
			// softPass = true;
			statusTapRecIn = 1;
		}
		else if (statusTapRecIn == 1)
		{
			// vient d'appuyer une deuxième fois
			// on va chercher début de première note
			// on compte différence entre début et 0
			// on décale toutes valeurs de temps (début, fin de note) de cette valeur (+5 disons)
			// on calcule une layerlength entre MomentTap2 et Début

			// recherche début première note
			unsigned long momentTap2 = nowMillis;

			for (byte i = 0; i < nbLayer; i++)
			{
				//	ZEROupdatePileTimeToPlayMODEDIRECT(i);
			}

			unsigned long lowest = 0;
			pileNote tempNote;

			for (byte i = 0; i < nbLayer; i++)
			{
				if (isLayerActive(i))
				{
					unsigned int taille = mesureTaillePileComplete(pInitial[i]);

					for (unsigned int j = 0; j < taille; j++)
					{
						tempNote = ZEROgetNoteFromPile(pInitial[i], j);
						if (tempNote.startNote < lowest || lowest == 0)
						{
							lowest = tempNote.startNote - 1;
						}
					}
				}
			}

			for (byte i = 0; i < nbLayer; i++)
			{
				if (isLayerActive(i))
				{
					layerLength[i] = momentTap2 - lowest;
				}
			}

			// lowest vaut maintenant le début le plus bas
			// on retire cette valeur de toutes les notes

			// for (byte i = 0; i < nbLayer; i++)
			// {
			// 	if (isLayerActive(i))
			// 	{
			// 		unsigned int taille = mesureTaillePileComplete(pInitial[i]);

			// 		for (unsigned int j = 0; j < taille; j++)
			// 		{
			// 			tempNote = ZEROgetNoteFromPile(pInitial[i], j);
			// 			tempNote.startNote  = tempNote.startNote - lowest;
			// 			tempNote.endNote  = tempNote.endNote - lowest;

			// 			ZEROSetNoteInPile(pInitial[i],j,tempNote);
			// 		}
			// 	}
			// }

			// on fait maintenant un cut de layerLength : momentTap2 - lowest

			for (byte i = 0; i < nbLayer; i++)
			{
				ZEROupdatePileTimeToPlayMODEDIRECT(i);
			}

			// record = false;
			// softPass = true;
			statusTapRecIn = 0;
		}
	};
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 2C02
		/////////////////////////////////////////////////////////////////////////////////////

	case 380: // KOMPAKTER (msec)
	{
		// potVal2 est le nombre de fois qu'il faut multiplier le layer

		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				// long initLength = layerLength[i];

				// plusieurs passage random, pour choisir des points de concentration non systématiquement dictés par les premières notes

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				// on va parcourir aléatoirement 3 * toutes les notes
				for (unsigned int k = 0; k < taille * 3; k++)
				{
					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], random(0, taille));
					unsigned long centerTime = tempNote.startNote;

					for (unsigned int l = 0; l < taille; l++)
					{
						pileNote tempNote2;
						tempNote2 = ZEROgetNoteFromPile(pInitial[i], l);
						long dist = distanceEntre2StartSimple(tempNote2.startNote, centerTime, layerLength[i]);
						if (dist < potVal2)
						{
							unsigned long Duree = getDuree(tempNote2);
							tempNote2.startNote = centerTime;
							tempNote2.endNote = tempNote2.startNote + Duree;
							ZEROSetNoteInPile(pInitial[i], l, tempNote2);
						}
					}
				}

				// on va parcourir séquentiellement toutes les notes
				for (unsigned int k = 0; k < taille; k++)
				{
					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], k);
					unsigned long centerTime = tempNote.startNote;

					for (unsigned int l = k + 1; l < taille; l++)
					{
						pileNote tempNote2;
						tempNote2 = ZEROgetNoteFromPile(pInitial[i], l);
						long dist = distanceEntre2StartSimple(tempNote2.startNote, centerTime, layerLength[i]);
						if (dist < potVal2)
						{
							unsigned long Duree = getDuree(tempNote2);
							tempNote2.startNote = centerTime;
							tempNote2.endNote = tempNote2.startNote + Duree;
							ZEROSetNoteInPile(pInitial[i], l, tempNote2);
						}
					}
				}
			}

			ZEROupdatePileTimeToPlayMODEDIRECT(i);
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 2C03 Feel
		/////////////////////////////////////////////////////////////////////////////////////

	case 381: // Kannon ON / OFF
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				kannonActif[i] = !kannonActif[i];
			}
		}
	}
	break;

	case 382: // Delay Kannon : >0 = msec , <0 = séquences
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				kannonDelay[i] = potVal2;
				// + = msec, - = séquence
			}
		}
	}
	break;

	case 383: // définition du pitch shif pour chaque Layer
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				kannonPitch[i] = potVal2;
			}
		}
	}
	break;

	case 384: // définition du Stretch
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				// kannonStretchRatio[i] = (float)potVal2 / (float)16;
				kannonStretchRatio[i] = potVal2; // 2C22
			}
		}
	}
	break;

	case 385: // définition du % d'écrevisse
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				kannonRevert[i] = potVal2;
			}
		}
	}
	break;

	case 386: // définition du % de reflet
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				kannonReflet[i] = potVal2;
			}
		}
	}
	break;

	case 387: // définition du random pour kannon
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				kannonRandom[i] = potVal2;
			}
		}
	}
	break;

	case 388: // Kompakteur (N)
	{
		// dans premier temps, calcul du nombre de startNote différents
		// si > potVal2, il faudra réduire
		// calcul de la valeur la plus petite de distance entre startNote, différent de zéro
		// faire liste des notes concernées par cette petite valeur ( au moins 2 ! )
		// parmi elles, choisir une note au hasard = startNoteCible
		// parcourir toutes les notes et les ramener à ce startNoteCible, tant que NbTiming > N
		// ( on ne peut pas toutes les y amener car on risque de passer de 2N à N, en skippant donc N+1 )

		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))

			{

				while (nbStartNotesDifferents(i) > (unsigned int)potVal2)
				{
					// u8x8.clear();
					buffer_clearScreen_HARD();
					buffer_and_HARD_setCursor(0, 0);
					buffer_and_HARD_print("NB diff : ");
					buffer_and_HARD_print(nbStartNotesDifferents(i));

					unsigned long shortest = shortestNonNulStartDistance(i);
					buffer_and_HARD_setCursor(0, 1);
					buffer_and_HARD_print("shortest=");
					buffer_and_HARD_print(shortest);

					// nb de notes répondant à cela
					buffer_and_HARD_setCursor(0, 2);
					buffer_and_HARD_print("match=");
					buffer_and_HARD_print(nbNotesAtDistance(i, shortest));

					// choisir un chiffre
					int choice;
					choice = random(0, nbNotesAtDistance(i, shortest) + 1);
					buffer_and_HARD_setCursor(0, 3);
					buffer_and_HARD_print("src=");
					buffer_and_HARD_print(choice);

					// trouver la n-ième note répondant à cela
					int noteSource = giveNbOfNthNoteAtDistance(i, choice, shortest);

					buffer_and_HARD_print(" pos=");
					buffer_and_HARD_print(noteSource);

					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], noteSource);
					unsigned long startSource = tempNote.startNote;

					// son start devient la cible
					// parcourir toutes les notes et les ramener à ce startNoteCible, tant que NbTiming > N
					// ( on ne peut pas toutes les y amener car on risque de passer de 2N à N, en skippant donc N+1 )

					int iteration = 0; // pour eviter boucle infinie

					// && shortestNonNulStartDistance(i) <= shortest
					while (nbStartNotesDifferents(i) > (unsigned int)potVal2 && iteration < 100 && nbNotesAtDistance(i, shortest) > 0)
					{
						// choisir un chiffre
						int choice2;
						choice2 = random(0, nbNotesAtDistance(i, shortest) + 1);
						buffer_and_HARD_setCursor(0, 5);
						buffer_and_HARD_print("Tgt=");
						buffer_and_HARD_print(choice2);

						// trouver la n-ième note répondant à cela
						int noteCible = giveNbOfNthNoteAtDistance(i, choice2, shortest);
						buffer_and_HARD_print(" pos=");
						buffer_and_HARD_print(noteCible);

						tempNote = ZEROgetNoteFromPile(pInitial[i], noteCible);
						unsigned long duree = getDuree(tempNote);
						tempNote.startNote = startSource;
						tempNote.endNote = tempNote.startNote + duree;

						ZEROSetNoteInPile(pInitial[i], noteCible, tempNote);
						iteration++;

						buffer_and_HARD_setCursor(0, 6);
						buffer_and_HARD_print(iteration);

						// delay(100);
					}

					//	delay(100);
				}

				// delay(100);
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 2C14
		/////////////////////////////////////////////////////////////////////////////////////

	case 389:
	{
		// currentLFO = potVal2;
		LFO[potVal2].actif = !LFO[potVal2].actif;
		LFO[potVal2].eventGOON = 0; // pousse à agir avec nouvelle donne
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 2C14
		/////////////////////////////////////////////////////////////////////////////////////

	case 390:
	{
		LFO[currentLFO].shape = potVal2;
		LFO[currentLFO].eventGOON = 0; // pousse à agir avec nouvelle donne
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 2C14
		/////////////////////////////////////////////////////////////////////////////////////

	case 391:
	{
		LFO[currentLFO].duration = (long)potVal2 * (long)100; // transfo de dixièmes en millisecondes
		LFO[currentLFO].eventGOON = 0;						  // pousse à agir avec nouvelle donne
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 2C14
		/////////////////////////////////////////////////////////////////////////////////////

	case 392: // Choix du CC
	{
		LFO[currentLFO].which = potVal2; // choix du CC
										 //	LFO[currentLFO].eventGOON = 0;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 2C14
		/////////////////////////////////////////////////////////////////////////////////////

	case 393: // Choix du Channel
	{
		LFO[currentLFO].channel = potVal2 - 1; // choix du Channel
		LFO[currentLFO].eventGOON = 0;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 2C14
		/////////////////////////////////////////////////////////////////////////////////////

	case 394: // Min Value
	{
		LFO[currentLFO].lowest = potVal2; // choix de valeur minimale du CC
		LFO[currentLFO].eventGOON = 0;	  // pousse à agir avec nouvelle donne
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 2C14
		/////////////////////////////////////////////////////////////////////////////////////

	case 395: // Max Value
	{
		LFO[currentLFO].highest = potVal2; // choix de valeur maximale du CC
		LFO[currentLFO].eventGOON = 0;	   // pousse à agir avec nouvelle donne
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 2C11
		/////////////////////////////////////////////////////////////////////////////////////

	case 398:
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				drunk[i] = !drunk[i];
			}
		}
	}
	break;

	case 399: // définition du min pour Drunk
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				drunkMin[i] = potVal2;
			}
		}
	}
	break;

	case 400: // définition du max pour Drunk
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				drunkMax[i] = potVal2;
			}
		}
	}
	break;

	case 401: // définition du step pour Drunk
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				drunkStep[i] = potVal2;
			}
		}
	}
	break;

	case 402: // Gamme par layer 2C12
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				gammeActuelle[i] = potVal2;
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 2C13
		/////////////////////////////////////////////////////////////////////////////////////

	case 403: // DEL Notes only
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))

			{

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{

					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					if (tempNote.type == 0x90 || tempNote.type == 0x80)
					{
						tempNote.note = 0;
						tempNote.velocity = 0;
						tempNote.killnote = true;
						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
				}

				ZEROupdatePileTimeToPlayMODEDIRECT(i);
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 2C13
		/////////////////////////////////////////////////////////////////////////////////////

	case 404: // DEL CC only
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))

			{

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{

					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					if (tempNote.type == 0xB0)
					{
						tempNote.note = 0;
						tempNote.velocity = 0;
						tempNote.killnote = true;
						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
				}
				ZEROupdatePileTimeToPlayMODEDIRECT(i);
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 2C13
		/////////////////////////////////////////////////////////////////////////////////////

	case 405: // CC Reflet
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))

			{

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);
				int highest = 0;
				int lowest = 0;
				bool first = true;

				for (unsigned int j = 0; j < taille; j++)
				{

					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					if (tempNote.type == 0xB0)
					{
						if (first == true)
						{
							highest = tempNote.note;
							lowest = tempNote.note;
							first = false;
						}

						if (tempNote.note < lowest)
						{
							lowest = tempNote.note;
						}

						if (tempNote.note > highest)
						{
							highest = tempNote.note;
						}
					}
				}

				// on connait maintenant les CC les plus élevés et les plus bas
				// on repasse toute la liste en revue en modifiant

				for (unsigned int j = 0; j < taille; j++)
				{

					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					if (tempNote.type == 0xB0)
					{
						tempNote.velocity = highest - (tempNote.velocity - lowest);
						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
				}

				ZEROupdatePileTimeToPlayMODEDIRECT(i);
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 2C13
		/////////////////////////////////////////////////////////////////////////////////////

	case 406: // CC Mirroir
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))

			{

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{

					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					if (tempNote.type == 0xB0)
					{
						long start = tempNote.startNote;
						long end = tempNote.endNote;
						tempNote.startNote = layerLength[i] - end;
						tempNote.endNote = layerLength[i] - start;
						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
				}

				ZEROupdatePileTimeToPlayMODEDIRECT(i);
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 2C13
		/////////////////////////////////////////////////////////////////////////////////////

	case 407: // CC Reduce
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{
					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					if (tempNote.type == 0xB0)
					{
						if (random(100) < (unsigned int)potVal2) // % de chance qu'on supprime le CC
						{
							tempNote.note = 0;
							tempNote.velocity = 0;
							tempNote.killnote = true;
							ZEROSetNoteInPile(pInitial[i], j, tempNote);
						}
					}
				}

				ZEROupdatePileTimeToPlayMODEDIRECT(i);
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 2C13
		/////////////////////////////////////////////////////////////////////////////////////

	case 408: // CC Multiplier : va multiplier entre 1 et 64 seizièmes
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{
					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					if (tempNote.type == 0xB0)
					{
						tempNote.startNote = tempNote.startNote * (long)potVal2 / (long)16;
						tempNote.endNote = tempNote.endNote * (long)potVal2 / (long)16;

						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
				}

				ZEROupdatePileTimeToPlayMODEDIRECT(i);
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 2C13
		/////////////////////////////////////////////////////////////////////////////////////

	case 409: // CC Addition
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{
					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					if (tempNote.type == 0xB0)
					{
						int value = tempNote.velocity;
						value += potVal2;
						if (value > 127)
						{
							value = 127;
						}
						if (value < 0)
						{
							value = 0;
						}

						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
				}

				ZEROupdatePileTimeToPlayMODEDIRECT(i);
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 2C13
		/////////////////////////////////////////////////////////////////////////////////////

	case 410: // CC Grosser ( c'est Liv qui a trouvé ce nom là )
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))

			{

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);
				int highest = 0;
				int lowest = 0;
				bool first = true;

				for (unsigned int j = 0; j < taille; j++)
				{

					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					if (tempNote.type == 0xB0)
					{
						if (first == true)
						{
							highest = tempNote.note;
							lowest = tempNote.note;
							first = false;
						}

						if (tempNote.note < lowest)
						{
							lowest = tempNote.note;
						}

						if (tempNote.note > highest)
						{
							highest = tempNote.note;
						}
					}
				}

				int middle = (lowest + highest) / 2;

				// on connait maintenant les CC les plus élevés et les plus bas
				// on repasse toute la liste en revue en modifiant

				for (unsigned int j = 0; j < taille; j++)
				{

					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					if (tempNote.type == 0xB0)
					{
						tempNote.velocity = (int)((float)middle + ((float)tempNote.velocity - (float)middle) * (float)potVal2 / (float)16);
						tempNote.velocity = limiter(tempNote.velocity, 0, 127);
						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
				}
				ZEROupdatePileTimeToPlayMODEDIRECT(i);
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 2C13
		/////////////////////////////////////////////////////////////////////////////////////

	case 414: // Implicit 1
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				implicitLayerUntilOne(i);
				ZEROupdatePileTimeToPlayMODEDIRECT(i);
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 2C13
		/////////////////////////////////////////////////////////////////////////////////////

	case 415: // Implicit max n beats
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				implicitLayerBeats(i, 2, potVal2);
				ZEROupdatePileTimeToPlayMODEDIRECT(i);
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 2C13
		/////////////////////////////////////////////////////////////////////////////////////

	case 416: // Implicit précisément n beats
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				implicitLayerBeats(i, potVal2, potVal2);
				ZEROupdatePileTimeToPlayMODEDIRECT(i);
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 2C13
		/////////////////////////////////////////////////////////////////////////////////////

	case 417: // swingplicit maximum n beats
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				swingplicitLayerBeats(i, potVal2, potVal2);
				ZEROupdatePileTimeToPlayMODEDIRECT(i);
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 2C13
		/////////////////////////////////////////////////////////////////////////////////////

	case 418: // swingplicit pourcentage
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				swingplicitLayerBeats(i, 2, 32, potVal2);
				ZEROupdatePileTimeToPlayMODEDIRECT(i);
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 2C13
		/////////////////////////////////////////////////////////////////////////////////////

	case 419: // arpeggio v1
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				unsigned int taille = mesureTaillePileComplete(pInitial[i]);
				bool tagged[taille] = {false}; // va servir de tagged

				for (unsigned int j = 0; j < taille - 1; j++)
				{
					if (tagged[j] == false) // çàd il n'a pas été taggué
					{
						tagged[j] = true;
						int compteur = 0;

						pileNote tempNote;
						tempNote = ZEROgetNoteFromPile(pInitial[i], j);

						for (unsigned k = j + 1; k < taille; k++)
						{
							if (tagged[k] == false)
							{
								pileNote tempNote2;
								tempNote2 = ZEROgetNoteFromPile(pInitial[i], k);

								if (distanceEntre2Start(tempNote, tempNote2) < (unsigned long)spanArp[i]) // 30msec est discutable
								{
									tagged[k] = true;
									compteur++;
									long duree = getDuree(tempNote2);
									tempNote2.startNote = tempNote.startNote + compteur * potVal2;
									tempNote2.endNote = tempNote2.startNote + duree;
									ZEROSetNoteInPile(pInitial[i], k, tempNote2);
								}
							}
						}
					}
				}
				ZEROupdatePileTimeToPlayMODEDIRECT(i);
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 2C13
		/////////////////////////////////////////////////////////////////////////////////////

	case 420: // sort notes in the pile order only / should be unheard
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				unsigned int taille = mesureTaillePileComplete(pInitial[i]);
				// u8x8.clear();
				buffer_clearScreen_HARD();
				buffer_and_HARD_setCursor(0, 1);
				buffer_and_HARD_print(taille);
				buffer_and_HARD_print("notes");

				// for ( int z = 0 ; z < 1000 ; z++ )
				{
					// int a = random(taille);
					// int b = random(taille);
					// switch2NotesInPile(pInitial[i],a,b); // OK Debuggé, fonctionne
					// insertNoteABeforeBInPile(pInitial[i],a,b); // OK Debuggé, fonctionne
					sortPile(pInitial[i], sortMode[i], ascendantMode[i]);
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 2C13
		/////////////////////////////////////////////////////////////////////////////////////

	case 421: // define Span
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				spanArp[i] = potVal2;
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Pile NOTE
		// 2C13
		/////////////////////////////////////////////////////////////////////////////////////

	case 422: // define Sort DATA by
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				sortMode[i] = potVal2 / 2;		//  0 -> 0 1->0 2-> 1 3->1 etc
				ascendantMode[i] = potVal2 % 2; // 0 -> 0 ; 1 -> 1 ; 2-> 0 ; 3 -> 1 etc
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 2C15
		/////////////////////////////////////////////////////////////////////////////////////

	case 424:
	{ // step Sequencer : joue la note sélectionnée
		if (potVal2 > 0 && potVal2 < 128)
		{
			PlaynoteOn_(0x90 + stepChannel, (byte)potVal3, stepVelocity);
			// BLACK08 j'ai modifié 100 en stepVelocity pour plus de réalisme

			addNewNotetoPileONCE(pointeurInitial, stepChannel, (byte)potVal3,
								 0, nowMillis + 1000, 0x80, 1);
		}
		// l'inscription d'un STePZ se fait actuellement par les touches MUTE et se trouve
		// en SubParts.h void checkStepzButtons(void)
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 2C15
		/////////////////////////////////////////////////////////////////////////////////////

	case 425:
	{ // step Sequencer : define nbStepz
		nbStepz = potVal2;
		numberMaxPageStepz = (nbStepz - 1) / nbStepzAffiches;
		// 5 steps, max 6 -> 0 (0)
		// 8 steps, max 3 -> 2 (0,1,2)
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				layerLength[i] = (60000 * nbStepz) / (nbBPM_Stepz * nbStepzFor1BPM);
				// 1 min = nbBPM_Stepz * durée 1 Beat
				// 1 min = nbBPM_Stepz * duréé 1 Step * nbStezp pour 1 BPM
				// durée 1 STep = 1 min / ( nbBPM * nbStepz1BPM )
				// or
				// 1 layer = nbStepz * durée 1 Step
				// 1 layer = nbStepz * 60000 / (nbBPM * nbSTepz1BPM )
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 2C15
		/////////////////////////////////////////////////////////////////////////////////////

	case 426:
	{ // step Sequencer : define nb Sub Stepz
		nbSubStepz = potVal2;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// 2C15
		/////////////////////////////////////////////////////////////////////////////////////

	case 427:
	{							   // step Sequencer : set Channel
		stepChannel = potVal2 - 1; // encodage machine donc
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// 2C15
		/////////////////////////////////////////////////////////////////////////////////////

	case 428:
	{ // step Sequencer : set Velocity
		stepVelocity = potVal2;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// 2C15
		/////////////////////////////////////////////////////////////////////////////////////

	case 429:
	{ // step Sequencer : stop Mode
		stopModeStepz = potVal2;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// 2C16 LAUSANNE
		// Va être le gros morceau, va remplir la pile ONCE et mettre MUTE
		/////////////////////////////////////////////////////////////////////////////////////

	case 430: // lance un gabberism sur le layer
	{
		// GABBERISM
		// byte gabberismLevels = 3;
		// byte gabberismDivisions = 2;
		// float gabberismDivRatio = 2;
		// byte gabberismRepeats = 2;
		// float gabberismRepeatRatio = 2;

		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				// muteLayer = muteLayer | (1 << i); // va muter le layer
				showMutedStatus();

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				unsigned long init = nowMillis - (nowMillis / layerLength[i]);
				unsigned long whereItsAt = init;

				int nbRepeatsForThatLevel = 1;
				//	unsigned int nbDivideForThatLevel = 1;
				float nbRepeatsForThatLevelFLOAT = 1;
				float nbDivideForThatLevelFLOAT = 1;

				for (int k = 1; k <= gabberismLevels; k++)
				{
					unsigned long tailleDivide = layerLength[i] / nbDivideForThatLevelFLOAT;

					for (int m = 1; m <= nbRepeatsForThatLevel; m++)
					{

						for (unsigned int j = 0; j < taille; j++)
						{
							pileNote tempNote;
							tempNote = ZEROgetNoteFromPile(pInitial[i], j);

							unsigned long gabberStart = whereItsAt + (float)tempNote.startNote / nbDivideForThatLevelFLOAT;
							unsigned long gabberEnd = whereItsAt + (float)tempNote.endNote / nbDivideForThatLevelFLOAT;

							if (gabberStart > nowMillis)
							{
								addNewNotetoPileONCE(pointeurInitial, tempNote.channel, tempNote.note,
													 tempNote.velocity, gabberStart, 0x90, i);
							}

							if (gabberEnd > nowMillis)
							{
								addNewNotetoPileONCE(pointeurInitial, tempNote.channel, tempNote.note,
													 0, gabberEnd, 0x80, i);
							}

						} // fin du J

						// aura un effet de rabotage du à l'int qui se répète... peut être prévoir une valeur float intermédiaire ?
						whereItsAt += tailleDivide;

					} // fin du M

					nbDivideForThatLevelFLOAT = (int)((float)nbDivideForThatLevelFLOAT * (float)gabberismDivRatio / (float)10);
					// nbDivideForThatLevel = (int) nbDivideForThatLevelFLOAT ;
					nbRepeatsForThatLevelFLOAT = (int)((float)nbRepeatsForThatLevelFLOAT * (float)gabberismRepeatRatio / (float)10);
					nbRepeatsForThatLevel = (int)nbRepeatsForThatLevelFLOAT;

				} // fin du K

			} //
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// 2C16 LAUSANNE
		/////////////////////////////////////////////////////////////////////////////////////

	case 431:
	{ // Gabberism levels
		gabberismLevels = potVal2;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// 2C16 LAUSANNE
		/////////////////////////////////////////////////////////////////////////////////////

	case 432:
	{ // Gabberism divisions
		gabberismDivisions = potVal2;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// 2C16 LAUSANNE
		/////////////////////////////////////////////////////////////////////////////////////

	case 433:
	{ // Gabberism Division Ratio
		// gabberismDivRatio = potVal2 / 10;
		gabberismDivRatio = potVal2;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// 2C16 LAUSANNE
		/////////////////////////////////////////////////////////////////////////////////////

	case 434:
	{ // Gabberism Repeats
		gabberismRepeats = potVal2;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// 2C16 LAUSANNE
		/////////////////////////////////////////////////////////////////////////////////////

	case 435:
	{ // Gabberism Repeat Ratio
		//	gabberismRepeatRatio = potVal2 / 10;
		gabberismRepeatRatio = potVal2;
	}
	break;

	/////////////////////////////////////////////////////////////////////////////////////
	// 2C16 POST LAUSANNE
	/////////////////////////////////////////////////////////////////////////////////////
	case 436:
	{ // Nombre de Stepz affiché : sera max 8 pour Piccolo T2, mais devrait aller à 16 pour PBlack
		nbStepzAffiches = potVal2;
		numberMaxPageStepz = (nbStepz - 1) / nbStepzAffiches;
	}
	break;

	/////////////////////////////////////////////////////////////////////////////////////
	// 2C16 POST LAUSANNE
	/////////////////////////////////////////////////////////////////////////////////////
	case 437:
	{ // Nombre de Stepz pour 1 BPM : sur Electribe vaut 4
		nbStepzFor1BPM = potVal2;
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				layerLength[i] = (60000 * nbStepz) / (nbBPM_Stepz * nbStepzFor1BPM);
				// 1 min = nbBPM_Stepz * durée 1 Beat
				// 1 min = nbBPM_Stepz * duréé 1 Step * nbStezp pour 1 BPM
				// durée 1 STep = 1 min / ( nbBPM * nbStepz1BPM )
				// or
				// 1 layer = nbStepz * durée 1 Step
				// 1 layer = nbStepz * 60000 / (nbBPM * nbSTepz1BPM )
			}
		}
	}
	break;

	/////////////////////////////////////////////////////////////////////////////////////
	// 2C16 POST LAUSANNE
	/////////////////////////////////////////////////////////////////////////////////////
	case 438:
	{ // Nombre de BPM = BPM FIX
		nbBPM_Stepz = potVal2;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// 2C16 POST LAUSANNE
		/////////////////////////////////////////////////////////////////////////////////////

	case 439:
	{ // step Sequencer : define nbStepz AVEC MODIF LAYER LENGTH
		nbStepz = potVal2;
		numberMaxPageStepz = (nbStepz - 1) / nbStepzAffiches;

		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				layerLength[i] = (60000 * nbStepz) / (nbBPM_Stepz * nbStepzFor1BPM);
				// 1 min = nbBPM_Stepz * durée 1 Beat
				// 1 min = nbBPM_Stepz * duréé 1 Step * nbStezp pour 1 BPM
				// durée 1 STep = 1 min / ( nbBPM * nbStepz1BPM )
				// or
				// 1 layer = nbStepz * durée 1 Step
				// 1 layer = nbStepz * 60000 / (nbBPM * nbSTepz1BPM )
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// 2C16 POST LAUSANNE
		/////////////////////////////////////////////////////////////////////////////////////

	case 440:
	{ // step Sequencer : ">BPM Stepz (chgL)") ; break ;  + BPM CHG
		nbBPM_Stepz = potVal2;

		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				layerLength[i] = (60000 * nbStepz) / (nbBPM_Stepz * nbStepzFor1BPM);
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// UNIVERSEL
		// 2C16
		/////////////////////////////////////////////////////////////////////////////////////

	case 441:
	{ // step Sequencer : joue la note sélectionnée : PUNTZ
		if (potVal2 > 0 && potVal2 < 128)
		{
			PlaynoteOn_(0x90 + stepChannel, (byte)potVal3, 100);
			addNewNotetoPileONCE(pointeurInitial, stepChannel, (byte)potVal3,
								 0, nowMillis + 1000, 0x80, 1);
		}
		// L'inscription en tant que tel de la note ne se fait pas par OK, qui joue juste 1 fois,
		// mais par les boutons MUTE dans SubParts.h void checkStepzButtons(void)
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// 2C16 POST LAUSANNE
		/////////////////////////////////////////////////////////////////////////////////////

	case 442:
	{ // taille du Cut Slice
		cutSlice = (byte)potVal2;
		KcutSlice = 1;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version post Lausanne
		// 2C16
		/////////////////////////////////////////////////////////////////////////////////////

	case 443: //  cut Slice de la mémoire inidquée vers les layers actifs
	{
		// byte compteur = 0;

		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				layerLength[i] = layerLength[potVal2 - 1 + 4] * (long)KcutSlice / (long)cutSlice;
				// doit précéder la copie, pour que les calculs soient OK
				ZEROcutSliceMemoryAtoLayerB(potVal2 - 1, i, layerLength[i], layerLength[potVal2 - 1 + 4]);
				ZEROupdatePileTimeToPlayMODEDIRECT(i);
				// compteur++;
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Kannibal
		// 2C17
		/////////////////////////////////////////////////////////////////////////////////////

	case 466: // activate ON OFF Dende
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				dende[i] = !dende[i];
			}
		}
	}
	break;

	case 444: // Dende Sec
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				dendeSec[i] = potVal2;
			}
		}
	}
	break;

	case 445: // Dende MSec
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				dendeMSec[i] = potVal2;
			}
		}
	}
	break;

	case 446: // Dende Divide
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				dendeDivide[i] = potVal2;
			}
		}
	}
	break;

	case 447: // Dende Drop
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				dendeDrop[i] = potVal2;
			}
		}
	}
	break;

	case 448: // Dende Down
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				dendeDown[i] = potVal2;
			}
		}
	}
	break;

	case 449: // Dende Levels
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				dendeLevels[i] = potVal2;
			}
		}
	}
	break;

	case 450: // Dende Decay
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				dendeDecay[i] = potVal2;
			}
		}
	}
	break;

	case 451: // Dende Start
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				dendeStart[i] = potVal2;
			}
		}
	}
	break;

	case 452: // Dende End
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				dendeEnd[i] = potVal2;
			}
		}
	}
	break;

	case 453: // Dende Ratio
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				dendeRatio[i] = potVal2;
			}
		}
	}
	break;

	case 454: // Dende Stretch
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				dendeStretch[i] = potVal2;
			}
		}
	}
	break;

	case 455: // Dende Pitch
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				dendePitch[i] = potVal2;
			}
		}
	}
	break;

	case 456: // Dende Fidelity
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				dendeFidelity[i] = potVal2;
			}
		}
	}
	break;

	case 457: // Dende Overload
	{

		overLoadLimit = potVal2;
	}
	break;

	case 458: // Dende  Retard
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				dendeRetard[i] = potVal2;
			}
		}
	}
	break;

	case 459: // Dende Gamme
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				dendeGamme[i] = potVal2;
			}
		}
	}
	break;

	case 460: // Dende Mode
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				dendeMode[i] = potVal2;
			}
		}
	}
	break;

	case 461: // Dende Degres
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				dendeDegres[i] = potVal2;
			}
		}
	}
	break;

	case 462: // Dende Expand
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				dendeExpand[i] = potVal2;
			}
		}
	}
	break;

	case 463: // Dende Arp1
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				dendeArp1[i] = potVal2;
			}
		}
	}
	break;

	case 464: // Dende Arp2
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				dendeArp2[i] = potVal2;
			}
		}
	}
	break;

	case 465: // Dende NoteOff
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				dendeNoteOff[i] = potVal2;
			}
		}
	}
	break;

		// 466 est plus haut !

	case 467: // Dende SubFidelity
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				dendeSubFidelity[i] = potVal2;
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Kannibal
		// 2C17
		/////////////////////////////////////////////////////////////////////////////////////

	case 468: // bitMapDende
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{

				affiche = nulString;
				if (potVal2 >= 0)
				{
					dendeBitMapDelay[i] = potVal2;
					int nbCases = 8;
					// int nombre = plusGrandNombreAvecAbits(nbCases);

					int t = potVal2;

					for (int j = 0; j < nbCases; j++)
					{
						int k = 0;
						k = (t >> j) & 1;
						_bitMapDelay[i][7 - j] = k;
					}
				}
				else
				{
					dendeBitMapDelay[i] = potVal2;

					int nbCases = 8;
					// int nombre = plusGrandNombreAvecAbits(nbCases);

					int t = giveNextValue(sequence[-potVal2 - 1], sequence[-potVal2 - 1].compteur);

					for (int j = 0; j < nbCases; j++)
					{
						int k = 0;
						k = (t >> j) & 1;
						_bitMapDelay[i][7 - j] = k;
					}

					sequence[-potVal2 - 1].compteur++;
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version post Lausanne
		// 2C17
		/////////////////////////////////////////////////////////////////////////////////////

	case 469: //  add Slice de la mémoire inidquée vers les layers actifs
	{
		// byte compteur = 0;

		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				unsigned long oldLayerLength = layerLength[i];
				unsigned long cut = layerLength[potVal2 - 1 + 4] * (unsigned long)KcutSlice / (unsigned long)cutSlice;
				layerLength[i] += layerLength[potVal2 - 1 + 4] * (unsigned long)KcutSlice / (unsigned long)cutSlice;
				// doit précéder la copie, pour que les calculs soient OK
				// ZEROaddSliceMemoryAtoLayerB(potVal2 - 1, i, layerLength[i], layerLength[potVal2 - 1 + 4], oldLayerLength);
				ZEROaddSliceMemoryAtoLayerB(potVal2 - 1, i, cut, layerLength[potVal2 - 1 + 4], oldLayerLength); // 2C24

				ZEROupdatePileTimeToPlayMODEDIRECT(i);
				// compteur++;
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Kannibal
		// 2C17
		/////////////////////////////////////////////////////////////////////////////////////

	case 470:
	{ // CH KILLER
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{

				// supprimer toutes les notes de valeur pour lesquelles channel = potVal3 dans les layers actives
				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{
					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					if (tempNote.channel == (potVal3 - 1))
					{
						tempNote.note = 0;
						tempNote.velocity = 0;
						tempNote.startNote = 0;
						tempNote.endNote = 0;
						tempNote.killnote = true;
						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
				}

				ZEROtreatPileComplete(pInitial[i]);
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Kannibal
		// 2C17
		/////////////////////////////////////////////////////////////////////////////////////

	case 471:
	{ // ALL\ CH KILLER
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{

				// supprimer toutes les notes de valeur pour lesquelles channel = potVal3 dans les layers actives
				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{
					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					if (tempNote.channel != (potVal3 - 1))
					{
						tempNote.note = 0;
						tempNote.velocity = 0;
						tempNote.startNote = 0;
						tempNote.endNote = 0;
						tempNote.killnote = true;
						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
				}

				ZEROtreatPileComplete(pInitial[i]);
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Kannibal 2C17
		/////////////////////////////////////////////////////////////////////////////////////

	case 472:
	{
		currentLFO = potVal2 - 1; // utilisateur LFO1 : machine LFO[0]
								  //		LFO[potVal2].actif = !LFO[potVal2].actif;
		LFO[currentLFO].eventGOON = 0;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version Kannibal 2C17
		/////////////////////////////////////////////////////////////////////////////////////

	case 473: // LFO Data REsume
	{
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// KANNIBAL 2C17
		/////////////////////////////////////////////////////////////////////////////////////

	case 474:
	{
		LFO[currentLFO].fidelite = potVal2; // transfo de dixièmes en millisecondes
		LFO[currentLFO].eventGOON = 0;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// KANNIBAL 2C17
		/////////////////////////////////////////////////////////////////////////////////////

	case 475:
	{
		LFO[currentLFO].modC = potVal2; // transfo de dixièmes en millisecondes
		LFO[currentLFO].eventGOON = 0;	// pousse à agir avec nouvelle donne
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// KANNIBAL 2C17
		/////////////////////////////////////////////////////////////////////////////////////

	case 476:
	{
		LFO[currentLFO].modV = potVal2; // transfo de dixièmes en millisecondes
		LFO[currentLFO].eventGOON = 0;	// pousse à agir avec nouvelle donne
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// KANNIBAL 2C17
		/////////////////////////////////////////////////////////////////////////////////////

	case 477:
	{
		copyLFOAintoLFOB(potVal2 - 1, currentLFO);
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// KANNIBAL 2C17
		/////////////////////////////////////////////////////////////////////////////////////

	case 478: // pousse en mode REPEATER
	{

		if (potVal2 > 0)
		{
			repeaterNbLFO[currentLFO] = potVal2;
			// le délais est défini en 16e de layerLength
			unsigned long newDelay = 0;
			newDelay = (long)layerLength[0] * (long)potVal2 / (long)32;
			// Se définit par rapport à layer 0 !
			LFO[currentLFO].duration = (long)newDelay;
			LFO[currentLFO].eventGOON = 0;
		}
		else
		{
			repeaterNbLFO[currentLFO] = potVal2;
			int valueRepeater = giveNextValue(sequence[-potVal2 - 1], sequence[-potVal2 - 1].compteur);

			// le délais est défini en 32e de layerLength
			unsigned long newDelay = 0;
			newDelay = (long)layerLength[0] * (long)valueRepeater / (long)32;
			// Se définit par rapport à layer 0 !
			LFO[currentLFO].duration = (long)newDelay;
			sequence[-potVal2 - 1].compteur++;
			LFO[currentLFO].eventGOON = 0;
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// KANNIBAL2 2C18
		/////////////////////////////////////////////////////////////////////////////////////

	case 479: // choix de machine pour CC LFO
	{

		LFO[currentLFO].typeMachine = potVal2;
		LFO[currentLFO].eventGOON = 0;
		// 0 étant CC normal
		// 1 TX81Z
		// 2 Sherman
		// 3 Korg MicroSampler
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// KANNIBAL2 2C18
		/////////////////////////////////////////////////////////////////////////////////////

	case 480: // Set CH0 as Machine Specific
	{
		if (level480 == 0)
		{
			machineSpecificChannel = potVal2;
			level480 = 1;
		}
		else
		{
			machineSpecific[machineSpecificChannel] = potVal2;
			level480 = 0;
		}
		// DEBUG 2C29 : ne fonctionnait que pour Channel 0, or le Sherman est d'office 15 machine (16 humain) et TX81Z n'importe
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// KANNIBAL2 2C18
		/////////////////////////////////////////////////////////////////////////////////////

	case 481: // twoplets
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				long newEcart = 0;

				if (msecTwoplets == false) // signifie qu'il est exprimé en 32èmes
				{

					if (ecartTwoplets32 < 0)
					{
						newEcart = giveNextValue(sequence[-ecartTwoplets32 - 1], sequence[-ecartTwoplets32 - 1].compteur);
						sequence[-ecartTwoplets32 - 1].compteur++;
					}
					else
					{
						newEcart = ecartTwoplets32;
					}

					newEcart = layerLength[i] * newEcart / 32;
				}
				else
				{ // signifie qu'il est exprimé en msec
					if (ecartTwoplets < 0)
					{
						newEcart = giveNextValue(sequence[-ecartTwoplets - 1], sequence[-ecartTwoplets - 1].compteur);
						sequence[-ecartTwoplets - 1].compteur++;
					}
					else
					{
						newEcart = ecartTwoplets;
					}
				}

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{
					if ((int)random(100) < potVal2)
					{
						pileNote tempNote;
						tempNote = ZEROgetNoteFromPile(pInitial[i], j);
						long initStart = tempNote.startNote;
						long initEnd = tempNote.endNote;

						tempNote.startNote = initStart - (newEcart / 2);
						tempNote.endNote = initEnd - (newEcart / 2);

						ZEROSetNoteInPile(pInitial[i], j, tempNote);

						tempNote.startNote = initStart + (newEcart / 2);
						tempNote.endNote = initEnd + (newEcart / 2);

						ZEROaddNewNotetoPile(pInitial[i], tempNote);
					}
				}
			}
			ZEROupdatePileTimeToPlayMODEDIRECT(i);
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// KANNIBAL2 2C18
		/////////////////////////////////////////////////////////////////////////////////////

	case 482: // indique l'écart pour twoplets en msec
	{
		ecartTwoplets = potVal2;
		msecTwoplets = true;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// KANNIBAL2 2C18
		/////////////////////////////////////////////////////////////////////////////////////

	case 483: // indique l'écart pour twoplets en 32èmes
	{
		ecartTwoplets32 = potVal2;
		msecTwoplets = false;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// KANNIBAL2 2C18
		// on prend ici juste 1 note, mais on la splitte en potVal2 -plets
		/////////////////////////////////////////////////////////////////////////////////////

	case 484: // n-plets
	{

		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				long newEcart = 0;

				if (msecTwoplets == false) // signifie qu'il est exprimé en 32èmes
				{

					if (ecartTwoplets32 < 0)
					{
						newEcart = giveNextValue(sequence[-ecartTwoplets32 - 1], sequence[-ecartTwoplets32 - 1].compteur);
						sequence[-ecartTwoplets32 - 1].compteur++;
					}
					else
					{
						newEcart = ecartTwoplets32;
					}

					newEcart = layerLength[i] * newEcart / 32;
				}
				else
				{ // signifie qu'il est exprimé en msec
					if (ecartTwoplets < 0)
					{
						newEcart = giveNextValue(sequence[-ecartTwoplets - 1], sequence[-ecartTwoplets - 1].compteur);
						sequence[-ecartTwoplets - 1].compteur++;
					}
					else
					{
						newEcart = ecartTwoplets;
					}
				}

				// long firstPlet;
				long distancePlet = newEcart / potVal2;
				// long nextPlet;

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);
				unsigned int j = random(0, taille);

				pileNote tempNote;
				tempNote = ZEROgetNoteFromPile(pInitial[i], j);
				long initStart = tempNote.startNote;
				long initEnd = tempNote.endNote;

				tempNote.startNote = initStart - (newEcart / 2);
				tempNote.endNote = initEnd - (newEcart / 2);

				ZEROSetNoteInPile(pInitial[i], j, tempNote);

				for (int k = 1; k < potVal2; k++)
				{
					tempNote.startNote = initStart + (long)k * distancePlet;
					tempNote.endNote = initEnd + (long)k * distancePlet;

					ZEROaddNewNotetoPile(pInitial[i], tempNote);
				}
			}
			ZEROupdatePileTimeToPlayMODEDIRECT(i);
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// KANNIBAL2 2C18
		// basé sur Tap Accou 250
		/////////////////////////////////////////////////////////////////////////////////////

	case 485: // TAP BPM : par défaut OFF
		// le TAP BPM va modifier les layers sélectionnés pour correspondre au BPM indiqué dans Stepz et Puntz
		{
			tapBPM = !tapBPM;
		}
		break;

		/////////////////////////////////////////////////////////////////////////////////////
		// KANNIBAL 2C19
		// basé sur 333
		// Dende tme defined by /32
		/////////////////////////////////////////////////////////////////////////////////////

	case 486: // pousse en mode REPEATER
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				if (potVal2 >= 0)
				{
					repeaterNbDende[i] = potVal2; // SERA POSITIF : on calcule 1x
					dendeSec[i] = (long)layerLength[i] * (long)potVal2 / (long)32000;
					dendeMSec[i] = ((long)layerLength[i] * (long)potVal2 / (long)32) % 1000;
				}
				else
				{
					repeaterNbDende[i] = potVal2; // SERA NEGATIF : on calculera à chaque fois
					int valueRepeater = giveNextValue(sequence[-potVal2 - 1], sequence[-potVal2 - 1].compteur);

					// unsigned long newDelay = 0;
					dendeSec[i] = (long)layerLength[i] * (long)valueRepeater / (long)32000;
					dendeMSec[i] = ((long)layerLength[i] * (long)valueRepeater / (long)32) % 1000;

					sequence[-potVal2 - 1].compteur++;
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// KANNIBAL 2C19
		// basé sur 319
		// modif 2C26 suite à fatal bug ( While )
		/////////////////////////////////////////////////////////////////////////////////////

	case 487: // center around pitch n
	{
		for (int i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{
					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					if (tempNote.type == 0x90)
					{
						while (((int)tempNote.note - (int)potVal2) > 6)
						{
							// tempNote.note = tempNote.note - 12;   2C24
							// tempNote.note = modifyPitchUniversal(tempNote, tempNote.note, -12); 2C26 pas ok, car doit pouvoir descendre
							tempNote.note = tempNote.note - 12;
						}

						while (((int)tempNote.note - (int)potVal2) <= -6)
						{
							// tempNote.note = tempNote.note + 12;
							// tempNote.note = modifyPitchUniversal(tempNote, tempNote.note, +12);
							tempNote.note = tempNote.note + 12;
						}

						tempNote.note = modifyPitchUniversal(tempNote, tempNote.note, 0);

						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// KANNIBAL 2C19
		// basé sur 4 et tout le travail sur la théorie musicale entretemps
		/////////////////////////////////////////////////////////////////////////////////////

	case 488: // TRANSPOSE DEGREES
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{

					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					if (tempNote.note != 0)
					{
						signed int Value;

						if (pitchableChannel[tempNote.channel] == true && pitchableLayer[tempNote.layer] == true)
						{
							Value = noteADegreesAboveNoteBinGammeCmodeD(potVal2, tempNote.note, gammeActuelle[i], modeActuelle[i]);
							Value = limiterForPitch(Value);
						}
						else
						{
							Value = tempNote.note; // si non pitchable on doit toujours revenir à la valeur initiale
						}

						tempNote.note = (byte)(Value);

						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// KANNIBAL 2C19
		/////////////////////////////////////////////////////////////////////////////////////

	case 489: // LFO Select Type : Void CC Transfo
	{
		LFO[currentLFO].type = potVal2;
		LFO[currentLFO].eventGOON = 0;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// KANNIBAL 2C19
		/////////////////////////////////////////////////////////////////////////////////////

	case 490: // Primer : modifie la dur��e des layers actifs pour que soient premiers entre eux
	{
		for (byte i = 1; i < nbLayer; i++)
		{
			buffer_and_HARD_setCursor(3, 3);
			buffer_and_HARD_print(">");
			buffer_and_HARD_print(i);
			delay(1000);

			if (isLayerActive(i))
			{
				buffer_and_HARD_print("A");
				buffer_and_HARD_print(i);
				delay(100);

				/*
				bool stopIt = false;

				do
				{
					if ((layerLengthAisIdemAtLeastAnotherActiveOne(i) == false) && (isPrime(layerLength[i]) == true))
					{
						stopIt = true;
					}
					else
					{
						layerLength[i]++;
					}
				} 
				while (stopIt == false);
*/

				while (((layerLengthAisIdemAtLeastAnotherActiveOne(i) == false) && (isPrime(layerLength[i]) == true)) == false)
				{
					layerLength[i]++;
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// THINNER 2C20
		/////////////////////////////////////////////////////////////////////////////////////

	case 491:
	{
		switch (potVal2)
		{
		case 0:
			affiche = "Stop ALL Evts";

			for (byte i = 0; i < NbEvenementMax; i++)
			{
				if (Evenement[i].type != 0)
				{
					Evenement[i].actif = false;
					// Evenement[i].type = 0; // 0 = vide  1=CC 2=Transfo
					Evenement[i].eventGOON = 0;
					Evenement[i].compteur = 0;
				}
			}
			break;
		case 1:
			affiche = "Start ALL Evts";

			for (byte i = 0; i < NbEvenementMax; i++)
			{
				if (Evenement[i].type != 0)
				{
					Evenement[i].actif = true;
					// Evenement[i].type = 0; // 0 = vide  1=CC 2=Transfo
					Evenement[i].eventGOON = 0;
					Evenement[i].compteur = 0;
				}
			}
			break;
		case 2:
			affiche = "Rnd Start/Stop Evts";
			for (byte i = 0; i < NbEvenementMax; i++)
			{
				if (Evenement[i].type != 0)
				{
					if (random(2) == 0)
					{
						Evenement[i].actif = false;
					}
					else
					{
						Evenement[i].actif = true;
					}
					// Evenement[i].type = 0; // 0 = vide  1=CC 2=Transfo
					Evenement[i].eventGOON = 0;
					Evenement[i].compteur = 0;
				}
			}
			break;
		case 3:
			affiche = "invert Start/Stop Evts";
			for (byte i = 0; i < NbEvenementMax; i++)
			{
				if (Evenement[i].type != 0)
				{

					Evenement[i].actif = !Evenement[i].actif;
					// Evenement[i].type = 0; // 0 = vide  1=CC 2=Transfo
					Evenement[i].eventGOON = 0;
					Evenement[i].compteur = 0;
				}
			}
			break;
		default:
			break;
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// THINNER 2C20
		/////////////////////////////////////////////////////////////////////////////////////

	case 492: // activate ON OFF Variations // basé sur 244 DynHarmonize
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				variationLayer[i] = !variationLayer[i];
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// THINNER 2C20
		/////////////////////////////////////////////////////////////////////////////////////

	case 493: // activate ON du Minimum Notes et le spécifie
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				modeMinimumVariation[i] = true;
				minimumNotesVariation[i] = potVal2;
				// byte percentDisparitionChanceVariation[nbLayer] = {50, 50, 50, 50};
				//byte minimumNotesVariation[nbLayer] = {1, 1, 1, 1};
				//byte maximumReduxVariation[nbLayer] = {3, 3, 3, 3};
				// bool modeMinimumVariation[nbLayer] = {true, true, true, true};
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// THINNER 2C20
		/////////////////////////////////////////////////////////////////////////////////////

	case 494: // activate ON du Maximum Redux Notes et le spécifie
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				modeMinimumVariation[i] = false;
				maximumReduxVariation[i] = potVal2;
				// byte percentDisparitionChanceVariation[nbLayer] = {50, 50, 50, 50};
				//byte minimumNotesVariation[nbLayer] = {1, 1, 1, 1};
				//byte maximumReduxVariation[nbLayer] = {3, 3, 3, 3};
				// bool modeMinimumVariation[nbLayer] = {true, true, true, true};
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// THINNER 2C20
		/////////////////////////////////////////////////////////////////////////////////////

	case 495: // pourcentage disappear
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				percentDisparitionChanceVariation[i] = potVal2;
				// byte percentDisparitionChanceVariation[nbLayer] = {50, 50, 50, 50};
				//byte minimumNotesVariation[nbLayer] = {1, 1, 1, 1};
				//byte maximumReduxVariation[nbLayer] = {3, 3, 3, 3};
				// bool modeMinimumVariation[nbLayer] = {true, true, true, true};
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// THINNER 2C20
		/////////////////////////////////////////////////////////////////////////////////////

	case 496: // mode Variation : 0 = Degressif 1=Constant
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				modeVariation[i] = potVal2;
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// THINNER 2C20
		/////////////////////////////////////////////////////////////////////////////////////

	case 497: // Primer2 : modifie la durée des layers actifs pour que soient premiers entre eux
	{

		// on va faire simple : division de la longueur par PotVal, puis multiplication
		// introduit évidemment une modification de longueur, mais c'est le jeu :)

		for (byte i = 1; i < nbLayer; i++)
		{
			layerLength[i] = layerLength[i] / potVal2;
			if (layerLength[i] < 1)
			{
				layerLength[i] = 1;
			}
		}

		for (byte i = 1; i < nbLayer; i++)
		{
			buffer_and_HARD_setCursor(3, 3);
			buffer_and_HARD_print(">");
			buffer_and_HARD_print(i);
			delay(1000);

			if (isLayerActive(i))
			{
				buffer_and_HARD_print("A");
				buffer_and_HARD_print(i);
				delay(100);

				while (((layerLengthAisIdemAtLeastAnotherActiveOne(i) == false) && (isPrime(layerLength[i]) == true)) == false)
				{
					layerLength[i]++;
				}
			}
		}

		for (byte i = 1; i < nbLayer; i++)
		{
			layerLength[i] = layerLength[i] * potVal2;
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// THINNER 2C20
		/////////////////////////////////////////////////////////////////////////////////////

	case 502: // FOLD
	{
		if (potVal2 == 0)
		{
			return;
		}

		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				int valueFold = 0;
				// si Sequence ( potVal < 0 )
				// alors potVal2 est déterminé par Sequence

				if (potVal2 < 0)
				{
					valueFold = giveNextValue(sequence[-potVal2 - 1], sequence[-potVal2 - 1].compteur);
					sequence[-potVal2 - 1].compteur++;
				}
				else
				{
					valueFold = potVal2;
				}

				buffer_setCursor(0, 2);
				buffer_print(String(valueFold));
				buffer_print(" ");

				// choix du point de suture : entre 0 et 64-n
				byte suture = random(0, 64 - valueFold + 1);
				// signed long sutureMoment = layerLength[i] * (signed long)suture / (signed long)64;

				// choix du point de coupure : entre 0+n et 64-n+n, soit entre 0+n et 64
				byte coupure = suture + valueFold;
				unsigned long coupureMoment = layerLength[i] * (signed long)coupure / (signed long)64;

				signed long nDuree = layerLength[i] * (signed long)valueFold / (signed long)64;

				// déplacement des notes comprises après coupure : -n
				// taille layer : réduire de n/64 èmes

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{

					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					if (tempNote.note != 0 && tempNote.startNote >= coupureMoment)
					{
						tempNote.startNote -= nDuree;

						// va modifier le pitch sauf si note appartenant à layer ou channel unpitchable
						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
				}

				//	layerLength[i] -= nDuree ;
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// THINNER 2C20
		/////////////////////////////////////////////////////////////////////////////////////

	case 503: // FOLD Shrten
	{
		if (potVal2 == 0)
		{
			return;
		}

		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				int valueFold = 0;
				// si Sequence ( potVal < 0 )
				// alors potVal2 est déterminé par Sequence

				if (potVal2 < 0)
				{
					valueFold = giveNextValue(sequence[-potVal2 - 1], sequence[-potVal2 - 1].compteur);
					sequence[-potVal2 - 1].compteur++;
				}
				else
				{
					valueFold = potVal2;
				}

				buffer_setCursor(0, 2);
				buffer_print(String(valueFold));
				buffer_print(" ");

				// choix du point de suture : entre 0 et 64-n
				byte suture = random(0, 64 - valueFold + 1);
				// unsigned long sutureMoment = layerLength[i] * (unsigned long)suture / (unsigned long)64;

				// choix du point de coupure : entre 0+n et 64-n+n, soit entre 0+n et 64
				byte coupure = suture + valueFold;
				unsigned long coupureMoment = layerLength[i] * (unsigned long)coupure / (unsigned long)64;

				unsigned long nDuree = layerLength[i] * (unsigned long)valueFold / (unsigned long)64;

				// déplacement des notes comprises après coupure : -n
				// taille layer : réduire de n/64 èmes

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{

					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					if (tempNote.note != 0 && tempNote.startNote >= coupureMoment)
					{
						tempNote.startNote -= nDuree;

						// va modifier le pitch sauf si note appartenant à layer ou channel unpitchable
						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
				}

				layerLength[i] -= nDuree;
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// THINNER 2C20
		/////////////////////////////////////////////////////////////////////////////////////

	case 504: // UNFOLD
	{
		if (potVal2 == 0)
		{
			return;
		}

		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				int valueFold = 0;
				// si Sequence ( potVal < 0 )
				// alors potVal2 est déterminé par Sequence

				if (potVal2 < 0)
				{
					valueFold = giveNextValue(sequence[-potVal2 - 1], sequence[-potVal2 - 1].compteur);
					sequence[-potVal2 - 1].compteur++;
				}
				else
				{
					valueFold = potVal2;
				}

				buffer_setCursor(0, 2);
				buffer_print(String(valueFold));
				buffer_print(" ");

				// dans UnFold, la partie entre suture et coupure va être écartelée :
				// 1 sur 2 est renvoyé au delà de coupure
				// tout ce qui est initialement au-delà de coupure est décalé

				// choix du point de suture : entre 0 et 64-n
				byte suture = random(0, 64 - valueFold + 1);
				unsigned long sutureMoment = layerLength[i] * (unsigned long)suture / (unsigned long)64;

				// choix du point de coupure : entre 0+n et 64-n+n, soit entre 0+n et 64
				byte coupure = suture + valueFold;
				unsigned long coupureMoment = layerLength[i] * (unsigned long)coupure / (unsigned long)64;

				signed long nDuree = layerLength[i] * (signed long)valueFold / (signed long)64;

				// déplacement des notes comprises après coupure : +n
				// déplacement de 1 notes sur 2 comprise entre suture et coupure : +n (1/2)
				// taille layer : augmenter de n/64 èmes si UnFold Longer

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{

					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					if (tempNote.note != 0 && tempNote.startNote >= coupureMoment)
					{
						tempNote.startNote += nDuree;

						// va modifier le pitch sauf si note appartenant à layer ou channel unpitchable
						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}

					if (tempNote.note != 0 && tempNote.startNote >= sutureMoment && tempNote.startNote < coupureMoment && random(2) == 1)
					{
						tempNote.startNote += nDuree;

						// va modifier le pitch sauf si note appartenant à layer ou channel unpitchable
						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
				}

				//	layerLength[i] -= nDuree ;
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// THINNER 2C20
		/////////////////////////////////////////////////////////////////////////////////////

	case 505: // UNFOLD LONGER
	{
		if (potVal2 == 0)
		{
			return;
		}

		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				int valueFold = 0;
				// si Sequence ( potVal < 0 )
				// alors potVal2 est déterminé par Sequence

				if (potVal2 < 0)
				{
					valueFold = giveNextValue(sequence[-potVal2 - 1], sequence[-potVal2 - 1].compteur);
					sequence[-potVal2 - 1].compteur++;
				}
				else
				{
					valueFold = potVal2;
				}

				buffer_setCursor(0, 2);
				buffer_print(String(valueFold));
				buffer_print(" ");

				// dans UnFold, la partie entre suture et coupure va être écartelée :
				// 1 sur 2 est renvoyé au delà de coupure
				// tout ce qui est initialement au-delà de coupure est décalé

				// choix du point de suture : entre 0 et 64-n
				byte suture = random(0, 64 - valueFold + 1);
				unsigned long sutureMoment = layerLength[i] * (unsigned long)suture / (unsigned long)64;

				// choix du point de coupure : entre 0+n et 64-n+n, soit entre 0+n et 64
				byte coupure = suture + valueFold;
				unsigned long coupureMoment = layerLength[i] * (unsigned long)coupure / (unsigned long)64;

				unsigned long nDuree = layerLength[i] * (unsigned long)valueFold / (unsigned long)64;

				// déplacement des notes comprises après coupure : +n
				// déplacement de 1 notes sur 2 comprise entre suture et coupure : +n (1/2)
				// taille layer : augmenter de n/64 èmes si UnFold Longer

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{

					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					if (tempNote.note != 0 && tempNote.startNote >= coupureMoment)
					{
						tempNote.startNote += nDuree;

						// va modifier le pitch sauf si note appartenant à layer ou channel unpitchable
						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}

					if (tempNote.note != 0 && tempNote.startNote >= sutureMoment && tempNote.startNote < coupureMoment && random(2) == 1)
					{
						tempNote.startNote += nDuree;

						// va modifier le pitch sauf si note appartenant à layer ou channel unpitchable
						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
				}

				layerLength[i] += nDuree;
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// COURONNE 2C21
		/////////////////////////////////////////////////////////////////////////////////////

	case 506: // BATCH MUTE UNMUTE
	{
		// on va devoir jeter dans la pile ONCE un .type = 10
		// qui quand il sera lu va déclencher un MUTE des layers actives
		// et 5 secondes plus tard, un UNMUTE

		// 2C21 d'abord essai avec le pile ONCE, puis développement de pile BATCH
		// Un problème se pose pour utiliser la pile ONCE pour des Batch : en effet, la valeur des nb de Transfo et des contenus
		// de transfos sont supérieures à 127 ( byte )

		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				// potVal2 = 20 ; // MUTE UNMUTE
				potVal2 = 4; // Transpose

				//  addNewtoPileBATCH (pBatch, potVal2, +1, i, nowMillis + 500 );
				//  addNewtoPileBATCH(pBatch, potVal2,  -5, i, nowMillis + 5000 );
				//  addNewtoPileBATCH(pBatch, potVal2, +1, i, nowMillis + 100500);
				//  addNewtoPileBATCH(pBatch, potVal2, -5, i, nowMillis + 125000);

				addNewtoPileBATCH(pBatch, 20, 0, i, nowMillis + 1000);
				addNewtoPileBATCH(pBatch, 20, 0, i, nowMillis + 3000);

				//   addNewNotetoPileONCE(pInitial[i], potVal2 / 127, potVal2 % 127, +5, nowMillis + 5000, 0x10);
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// 2C22 basé sur 372
		/////////////////////////////////////////////////////////////////////////////////////

	case 507: // SAVE KARAKT - 2C22
	{
		// saveLocal(); // va sauver un fichier .txt avec toutes les infos
		// attention, ceci ne sauve que le Karakt actuel
		// saveCurrentKarakt(true);
		// 2C26 choix de sauver Karakt actuel + les Karakt mémoire, sinon trompeur
		saveLocal(false, false, false, false, false, false, false, false, true); // = save seulement Karakt current + mem
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// 2C22 basé sur 373
		/////////////////////////////////////////////////////////////////////////////////////

	case 508: // LOAD KARAKT - 2C23
	{
		// loadLocal();
		// loadLocal_Karakt();
		loadLocal2(false, false, false, false, false, false, false, false, true); // = load seulement Karakt
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// CORONA DAYZ 2C22 basé sur 340
		/////////////////////////////////////////////////////////////////////////////////////

	case 509: //  Sauve Karakt vesr K_Mem
	{
		free_KMem(potVal2 - 1); // BLACK08
		saveKarakt_to_KMem(potVal2 - 1);
		verboseKMem(potVal2 - 1);
		currentKarakt = potVal2 - 1;
		// pour DEBUG : explicit content KMem !!! car même qd rien ne change, il en sauve 4 !!!
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// CORONA DAYZ 2C22 basé sur 340
		/////////////////////////////////////////////////////////////////////////////////////

	case 510: //  charge KMem vers Karakt

	{
		loadKarakt_from_KMem(potVal2 - 1);
		currentKarakt = potVal2 - 1;
		// verboseKMem(potVal2 - 1);
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// CORONA DAYZ 2C22 basé sur 340
		/////////////////////////////////////////////////////////////////////////////////////

	case 511: //  charge KMem vers Karakt

	{
		free_KMem(potVal2 - 1);
		// verboseKMem(potVal2 - 1);
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// CORONA DAYZ 2C22 basé sur 424
		/////////////////////////////////////////////////////////////////////////////////////

	case 512:
	{ // Lapidz : joue les notes déjà dans layer
		if (potVal2 > 0 && potVal2 < 128)
		{
			PlaynoteOn_(0x90 + stepChannel, (byte)currentStepzNote, 100);

			addNewNotetoPileONCE(pointeurInitial, stepChannel, (byte)currentStepzNote,
								 0, nowMillis + 1000, 0x80, 1);
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// CORONA DAYZ 2C23
		/////////////////////////////////////////////////////////////////////////////////////

	case 513: // keep STepzBUG
	{
		keepStepzBUG = !keepStepzBUG;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// CORONA DAYZ 2C23
		/////////////////////////////////////////////////////////////////////////////////////

	case 514: // verbose FULL PILE
	{

		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				verboseFullPile(i);
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// CORONA DAYZ 2C23
		/////////////////////////////////////////////////////////////////////////////////////

	case 515: // CH Mute
	{
		CHMute = CHMute ^ ((int)1 << ((int)potVal2 - 1));
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// CORONA DAYZ 2C23
		/////////////////////////////////////////////////////////////////////////////////////

	case 516: // CH Solo
	{
		CHMute = 65535 ^ ((int)1 << ((int)potVal2 - 1));
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// CORONA DAYZ 2C23
		/////////////////////////////////////////////////////////////////////////////////////

	case 517: // CH unmute All
	{
		CHMute = 0;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// CORONA DAYZ 2C23
		/////////////////////////////////////////////////////////////////////////////////////

	case 518:
	{ // taille du Cut Slice
		KcutSlice = (byte)potVal2;
		// cutSlice = 1;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// CORONA DAYZ 2C23
		/////////////////////////////////////////////////////////////////////////////////////

	case 519:
	{ // Gabberito

		// on va devoir jeter dans la pile ONCE un .type = 10
		// qui quand il sera lu va déclencher un MUTE des layers actives
		// et n secondes plus tard, un UNMUTE

		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{

				long placeToAdd = 0;

				addNewtoPileBATCH(pBatch, 20, 0, i, nowMillis); // MUTE

				// on va parcourir la pile
				// chaque note va être répéte potVal2 fois, à un intervalle donné : gabberitoInterval
				// on comptabilise la durée de tous ces intervalles pour savoir qd on doit faire l'unmute

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{
					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					for (int k = 0; k < potVal2; k++)
					{

						addNewNotetoPileONCE(pointeurInitial, tempNote.channel, tempNote.note,
											 tempNote.velocity, nowMillis + placeToAdd, 0x90, i);
						placeToAdd += gabberitoInterval;
					}
					addNewNotetoPileONCE(pointeurInitial, tempNote.channel, tempNote.note,
										 tempNote.velocity, nowMillis + placeToAdd - 1, 0x80, i);
				}

				addNewtoPileBATCH(pBatch, 20, 0, i, nowMillis + placeToAdd + gabberitoInterval + 10); // UNMUTE
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// CORONA DAYZ 2C23
		/////////////////////////////////////////////////////////////////////////////////////

	case 520:
	{ // gabberito interval
		gabberitoInterval = potVal2;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// TOTAL ACCENT 2C24
		// modif 2C28
		/////////////////////////////////////////////////////////////////////////////////////

	case 521:
	{ // set Accent Min
		accentMinON = !accentMinON;
		//		accentMin = potVal2;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// TOTAL ACCENT 2C24
		// modif 2C28
		/////////////////////////////////////////////////////////////////////////////////////

	case 522:
	{ // set Accent Max
		// accentMax = potVal2;
		accentMaxON = !accentMaxON;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// 2C25 basé sur 372 et 507
		/////////////////////////////////////////////////////////////////////////////////////

	case 523: // SAVE ALL KARAKT - 2C25

	{
		// saveLocal(); // va sauver un fichier .txt avec toutes les infos
		saveCurrentandMemKarakt(true, true);
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Version KaraktFinisher
		// 2C25, basé sur 379
		/////////////////////////////////////////////////////////////////////////////////////

	case 524:
	{ // FLUX REC

		if (statusFluxRecIn == 0)
		{
			// vient d'appuyer première fois
			// on va mettre la layers de flux à 100 x 60 x 60 secondes
			// et les effacer
			layerLength[pFluxStart + potVal2] = 10 * 60 * 60 * 1000; // BAM, 10 heures d'emblée.
			ZEROdeleteWholePile(pInitial[pFluxStart + potVal2]);
			currentFluxMem = potVal2;
			statusFluxRecIn = 1;
			// toutes les notes played vont se retrouver entretemps dans la layer de Flux currentFluxMem
			// prévoir une limite de notes !!!! sinon pourrait tout enregistrer
			buffer_and_HARD_setCursor(0, 4);
			buffer_clearLine(4);
			buffer_and_HARD_print("REC      ");
		}
		else if (statusFluxRecIn == 1)
		{
			// vient d'appuyer une deuxième fois
			// toutes les notes played vont se retrouver entretemps dans la layer de Flux currentFluxMem

			// on va chercher début de première note
			// on compte différence entre début et 0
			// on décale toutes valeurs de temps (début, fin de note) de cette valeur (+5 disons)
			// on calcule une layerlength entre MomentTap2 et Début

			// recherche début première note
			unsigned long momentTap2 = nowMillis;
			//Serial.println("----------------------");
			//Serial.println(String(momentTap2));
			//Serial.println("----------------------");

			for (byte i = 0; i < nbLayer; i++)
			{
				//	ZEROupdatePileTimeToPlayMODEDIRECT(i);
			}

			unsigned long lowest = 0;
			pileNote tempNote;

			unsigned int taille = mesureTaillePileComplete(pInitial[pFluxStart + currentFluxMem]);

			for (unsigned int j = 0; j < taille; j++)
			{
				tempNote = ZEROgetNoteFromPile(pInitial[pFluxStart + currentFluxMem], j);
				//Serial.println(String(tempNote.startNote));
				if (tempNote.startNote < lowest || lowest == 0)
				{
					lowest = tempNote.startNote - 1;
				}
			}

			layerLength[pFluxStart + currentFluxMem] = momentTap2 - lowest;
			//Serial.println("----------------------");
			//Serial.println(String(layerLength[pFluxStart + currentFluxMem]));
			//Serial.println("----------------------");

			// lowest vaut maintenant le début le plus bas
			// on retire cette valeur de toutes les notes

			// for (byte i = 0; i < nbLayer; i++)
			// {
			// 	if (isLayerActive(i))
			// 	{
			taille = mesureTaillePileComplete(pInitial[pFluxStart + currentFluxMem]);

			for (unsigned int j = 0; j < taille; j++)
			{
				tempNote = ZEROgetNoteFromPile(pInitial[pFluxStart + currentFluxMem], j);
				tempNote.startNote = tempNote.startNote - lowest;
				tempNote.endNote = tempNote.endNote - lowest;

				ZEROSetNoteInPile(pInitial[pFluxStart + currentFluxMem], j, tempNote);
			}
			// 	}
			// }

			// on fait maintenant un cut de layerLength : momentTap2 - lowest

			// ZEROupdatePileTimeToPlayMODEDIRECT(pFluxStart + currentFluxMem);

			// record = false;
			// softPass = true;
			statusFluxRecIn = 0;
			buffer_and_HARD_setCursor(0, 4);
			buffer_and_HARD_print(taille);
		}
	};
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// KARAKT FINISHER 2C25 - 8 may 2020
		/////////////////////////////////////////////////////////////////////////////////////

	case 525:
	{ // Gabberito 2

		// on définit ici le distance
		gabberito2Interval = potVal2;

		// on définit les valeurs pour cette fois-ci
		long currentGabberito2Interval;
		int currentGabberito2K;
		int currentGabberito2N;

		// si ( potVal < 0 )
		// alors potVal2 est déterminé par Sequence
		/*
		long gabberito2Interval = 20;
		int gabberito2K = 1;
		int gabberito2N = 20;
*/

		if (gabberito2K < 0)
		{
			currentGabberito2K = giveNextValue(sequence[-gabberito2K - 1], sequence[-gabberito2K - 1].compteur);
			sequence[-gabberito2K - 1].compteur++;
		}
		else
		{
			currentGabberito2K = gabberito2K;
		}

		// on va devoir jeter dans la pile ONCE un .type = 10
		// qui quand il sera lu va déclencher un MUTE des layers actives
		// et n secondes plus tard, un UNMUTE

		for (byte i = 0; i < nbLayer; i++)
		{
			currentGabberito2Interval = 0; // précaution de debug

			if (isLayerActive(i))
			{
				long placeToAdd = 0;

				addNewtoPileBATCH(pBatch, 20, 0, i, nowMillis); // MUTE

				// on va parcourir la pile
				// chaque note va être répéte potVal2 fois, à un intervalle donné : gabberitoInterval
				// on comptabilise la durée de tous ces intervalles pour savoir qd on doit faire l'unmute

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (int m = 0; m < currentGabberito2K; m++)
				{
					for (unsigned int j = 0; j < taille; j++)
					{
						pileNote tempNote;
						tempNote = ZEROgetNoteFromPile(pInitial[i], j);

						if (gabberito2N < 0)
						{
							currentGabberito2N = giveNextValue(sequence[-gabberito2N - 1], sequence[-gabberito2N - 1].compteur);
							sequence[-gabberito2N - 1].compteur++;
						}
						else
						{
							currentGabberito2N = gabberito2N;
						}

						for (int k = 0; k < currentGabberito2N; k++)
						{

							if (gabberito2Interval < 0)
							{
								currentGabberito2Interval = giveNextValue(sequence[-gabberito2Interval - 1], sequence[-gabberito2Interval - 1].compteur);
								sequence[-gabberito2Interval - 1].compteur++;
							}
							else
							{
								currentGabberito2Interval = gabberito2Interval;
							}

							addNewNotetoPileONCE(pointeurInitial, tempNote.channel, tempNote.note,
												 tempNote.velocity, nowMillis + placeToAdd, 0x90, i);
							placeToAdd += currentGabberito2Interval;
						}
						addNewNotetoPileONCE(pointeurInitial, tempNote.channel, tempNote.note,
											 tempNote.velocity, nowMillis + placeToAdd - 1, 0x80, i);
					}
				}

				addNewtoPileBATCH(pBatch, 20, 0, i, nowMillis + placeToAdd + currentGabberito2Interval + 10); // UNMUTE
			}
		}
	}
	break;

		/*
		long gabberito2Interval = 20;
		int gabberito2K = 1;
		int gabberito2N = 20;
*/

		/////////////////////////////////////////////////////////////////////////////////////
		// KARAKT FINISHER 2C25 - 8 may 2020
		/////////////////////////////////////////////////////////////////////////////////////

	case 526:
	{ // Gabberito 2
		gabberito2K = potVal2;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// KARAKT FINISHER 2C25 - 8 may 2020
		/////////////////////////////////////////////////////////////////////////////////////

	case 527:
	{ // Gabberito 2
		gabberito2N = potVal2;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// KARAKT FINISHER 2C25 - 8 may 2020
		/////////////////////////////////////////////////////////////////////////////////////

	case 528:
	{ // FLUSH
		ZEROdeleteWholePile(pointeurInitial);
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// 2C26 BUFFLON2 - 21 may 2020
		/////////////////////////////////////////////////////////////////////////////////////

	case 529: // d'après 377
	{
		// va générer un nom pour fileNameSave aléatoire
		myFile.close();
		//  String fileName;
		String ligne;
		int check;
		fileName = "titles.txt";

		if (SDcard.exists("titles.txt"))
		{
			oledWriteStringAtXY("titles.txt exists.", 0, 2);
			myFile.close();
		}
		else
		{
			oledWriteStringAtXY("titles.txt absent, create", 0, 2);

			myFile = SDcard.open("titles.txt", FILE_WRITE);
			myFile.println("data");
			myFile.println("bronchus");
			myFile.println("remix");
			myFile.println("escalier");
			myFile.println("manger");
			myFile.println("boire");
			myFile.println("nuage");
			myFile.println("feu");
			myFile.println("paprika");
			myFile.close();
		}

		myFile = SDcard.open(fileName, FILE_READ);

		// NOTE était midiname

		int compteur = 1;

		do
		{
			ligne = myFile.readStringUntil('\n');

			check = myFile.peek();

			Serial.print(String(compteur));
			Serial.print(":");
			Serial.println(ligne);
			compteur++;

		} while (check != -1);

		compteur--;

		// Serial.println();
		// Serial.print("il y a ");
		// Serial.print(String(compteur));
		// Serial.print(" titres dispo");
		// Serial.println();
		// Serial.println();

		// on a maintenant dans compteur le nombre de lignes
		// on en choisit potVal2 au hasard et on accrétionne

		fileName = nulString;

		for (int i = 0; i < potVal2; i++)
		{
			myFile.rewind();

			int compteur2 = 0;
			int toReach = random(1, compteur + 1);

			do
			{
				ligne = myFile.readStringUntil('\n');

				check = myFile.peek();

				Serial.print(String(compteur2));
				Serial.print(":");
				Serial.println(ligne);
				compteur2++;
				if (compteur2 == toReach)
				{
					Serial.println("<<<");
					miniCleanLigne(ligne);
					fileName = fileName + ligne;
					Serial.println();
					Serial.print("current filename : ");
					Serial.println(fileName);
					Serial.println();
					Serial.println();
				}

			} while (compteur2 < toReach);
		}

		Serial.println();
		Serial.println(fileName);

		myFile.close();

		// fileNameSave = nulString;
		// fileNameSave +=			letters[random(0, 26)] + letters[random(0, 26)] + letters[random(0, 26)] + "_" + letters[random(26, 36)] + letters[random(26, 36)] + letters[random(26, 36)] + letters[random(26, 36)] + F(".txt");

		fileNameSave = fileName;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// 2C26 BUFFLON2 - 21 may 2020
		/////////////////////////////////////////////////////////////////////////////////////

	case 530:
	{ // Process
		reduxProcessLigne(fileName);
		fileNameSave = fileName;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// 2C26 BUFFLON2 - 30 may 2020
		/////////////////////////////////////////////////////////////////////////////////////

	case 531:
	{ // Permute

		// création de la table de permutation !
		int table[127] = {0};

		int k = potVal2;
		int n = 127 - potVal2; // inclus

		for (int i = k; i <= n; i++)
		{
			int newPlace = random(k, n + 1);
			while (table[newPlace] != 0)
			{
				newPlace++;
				if (newPlace > n)
				{
					newPlace = k;
				}
			}
			table[newPlace] = i;
		}

		// ensuite parcours des piles actives pour modifier celles-ci.

		pileNote tempNote;
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					if (tempNote.note != 0 && tempNote.type == 0x90 && table[tempNote.note] != 0)
					{
						tempNote.note = modifyPitchUniversal(tempNote, table[tempNote.note]);
						// va modifier le pitch sauf si note appartenant à layer ou channel unpitchable
						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// 2C26 BUFFLON2 - 30 may 2020
		/////////////////////////////////////////////////////////////////////////////////////

	case 532:
	{ // CH Kannon % appear
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				kannonRandomAppear[i] = potVal2;
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// 2C26 BUFFLON2 - 30 may 2020
		/////////////////////////////////////////////////////////////////////////////////////

	case 533:
	{ // CH Kannon
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				if (potVal2 < 17)
				{
					// affiche = "redirect -> CH" + String(potVal2) + ":";
					kannonDestinationChanel[i] = potVal2 - 1;
				}
				else
				{
					// affiche = "to CH idem";
					kannonDestinationChanel[i] = potVal2 - 1; // également, mais permet de se souvenir de raison
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// 2C26 BUFFLON2 - 30 may 2020
		// basé sur 334
		/////////////////////////////////////////////////////////////////////////////////////

	case 534: // bitMap Kannon
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{

				affiche = nulString;
				if (potVal2 >= 0)
				{
					kannonSeqBitmap[i] = potVal2; // sera positif et donc  on travaille sur bitMapDelay (pas SEQ)
					int nbCases = 8;
					// int nombre = plusGrandNombreAvecAbits(nbCases);

					int t = potVal2;

					for (int j = 0; j < nbCases; j++)
					{
						int k = 0;
						k = (t >> j) & 1;
						// bitMapDelay[i][7 - j] = k;
						// setBit(bitMapDelay[i],7-j,k);
						setBit(kannonBitmap[i], 7 - j, k);
					}
				}
				else
				{
					kannonSeqBitmap[i] = potVal2; // sera négatif et donc on travaille sur SEQ

					int nbCases = 8;
					// int nombre = plusGrandNombreAvecAbits(nbCases);

					int t = giveNextValue(sequence[-potVal2 - 1], sequence[-potVal2 - 1].compteur);

					for (int j = 0; j < nbCases; j++)
					{
						int k = 0;
						k = (t >> j) & 1;
						// bitMapDelay[i][7 - j] = k;
						setBit(kannonBitmap[i], 7 - j, k);
					}

					sequence[-potVal2 - 1].compteur++;
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// 2C26 BUFFLON2 - 1st June 2020
		/////////////////////////////////////////////////////////////////////////////////////

	case 535: // tracker
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				trackerActif[i] = !trackerActif[i];
				trackerSeq[i] = potVal2;
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// 2C26 BUFFLON2 - 7 June 2020
		// basé sur 340
		/////////////////////////////////////////////////////////////////////////////////////

	case 538: //  Sauve Layer vers Mémoire, first free
		// option 1 : sauver les layers sélectionnés vers mémoire sélectionnée
		// option 2 : sauvers les layers sélectionnés vers mémoires consécutives, 1 pour chaque layer sélect

		{
			String display = "";
			for (byte i = 0; i < nbLayer; i++)
			{
				if (isLayerActive(i))
				{
					if (isThereFreeMem())
					{
						int target = giveFirstFreeMem();

						layerLength[target + 4] = layerLength[i];
						// doit précéder la copie
						ZEROcopyLayerAtoMemoryB(i, target);
						display += String(i + 1) + "at" + String(target + 1) + " ";
						currentMem = target;
					}
				}
			}
			printRot2(display, 16, 400, 7);
		}
		break;

		/////////////////////////////////////////////////////////////////////////////////////
		// 2C26 BUFFLON2 - 7 June 2020
		// basé sur 509
		/////////////////////////////////////////////////////////////////////////////////////

	case 539: //  Sauve Karakt vesr K_Mem, first free

	{
		if (isThereFreeKMem())
		{
			int target = giveFirstFreeKMem();
			saveKarakt_to_KMem(target);
			currentKarakt = target;
			verboseKMem(target);
			String display = "savedK " + String(target + 1);
			printRot2(display, 16, 400, 7);
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// 2C26 BUFFLON2 - 7 June 2020
		/////////////////////////////////////////////////////////////////////////////////////

	case 540: // Previous Load
	{
		String display = "";
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{

				int index = currentMem;

				unsigned int tailleCheck;

				do
				{
					index--;
					if (index < 0)
					{
						index = 63;
					}
					tailleCheck = mesureTaillePileComplete(pInitial[index + 4]);

				} while (tailleCheck == 0 && index != currentMem);

				currentMem = index;

				layerLength[i] = layerLength[index + 4];
				// doit précéder la copie, pour que les calculs soient OK
				ZEROcopyMemoryAtoLayerB(index, i);
				display += String(index + 1) + "atL" + String(i + 1) + " ";
				ZEROupdatePileTimeToPlayMODEDIRECT(i);
			}
		}
		printRot2(display, 16, 400, 7);
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// 2C26 BUFFLON2 - 13 June 2020
		/////////////////////////////////////////////////////////////////////////////////////

	case 541: // Variations repeat
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				repeatVariations[i] = potVal2;
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// 2C26 BUFFLON2 - 25 June 2020
		// basé sur 538
		/////////////////////////////////////////////////////////////////////////////////////

	case 543: // basé sur 538 DIALYSE : place chaque ensemble de notes idem dans une mem séparée

		// pour chaque layer Actif
		// si il y a FreeMem
		// on pratique une analyse du layer pour en séparer les différentes notes, comme Variations, ou Stepz
		// ensuite, on parcourt cette nouvelle liste et on sauve une layer avec UNIQUEMENT cette note là
		// on efface la liste temporaire

		{
			for (byte i = 0; i < nbLayer; i++)
			{
				if (isLayerActive(i))
				{
					if (isThereFreeMem())
					{
						ZEROdeleteWholePile(pVariationsDialyse); // 2C26

						unsigned int taille = mesureTaillePileComplete(pInitial[i]);

						for (unsigned int j = 0; j < taille; j++)
						{
							pileNote tempNote;
							tempNote = ZEROgetNoteFromPile(pInitial[i], j);

							if (tempNote.type != 0x90 || checkNoteAChannelBinDialysePiles(tempNote.note, tempNote.channel) == true)
							{
							}
							else
							{
								// Ajout NOte à Variations
								ZEROaddNewNotetoPile(pVariationsDialyse, tempNote);
								Serial.print("+");
								Serial.print(tempNote.note);
							}
						}

						// la pile Variations devrait contenir toutes les notes différentes

						int tailleVariations = mesureTaillePileComplete(pVariationsDialyse);
						// int tailleVariationsInitiale = tailleVariations;

						tailleVariations = mesureTaillePileComplete(pVariationsDialyse);
						taille = mesureTaillePileComplete(pInitial[i]);

						for (int j = 0; j < tailleVariations; j++)
						{
							if (isThereFreeMem())
							{
								int target = giveFirstFreeMem();

								layerLength[target + 4] = layerLength[i];
								// doit précéder la copie

								pileNote tempNote;
								tempNote = ZEROgetNoteFromPile(pVariationsDialyse, j);

								if (tempNote.type == 0x90)
								{
									// faire adaptation de SaveLayer2Mem, spécifique à une seule CH et Note
									ZEROcopyLayerAtoMemoryB_ONLY_ChannelC_NoteD(i, target, tempNote.channel, tempNote.note);
								}
							}
						}
					}
				}
			}
		}
		break;

		/////////////////////////////////////////////////////////////////////////////////////
		// 2C26 BUFFLON2 - 28 June 2020
		/////////////////////////////////////////////////////////////////////////////////////

	case 545: // affichage verbose
	{
		affiche = nulString;
		affiche = "";

		for (byte i = 0; i < nbLayer; i++)
		{
			Serial.print("layer ");
			Serial.print(String(i));
			Serial.println(" : ");
			Serial.println("------------------------");

			unsigned int taille = mesureTaillePileComplete(pInitial[i]);

			for (unsigned int j = 0; j < taille; j++)
			{
				pileNote tempNote;
				tempNote = ZEROgetNoteFromPile(pInitial[i], j);

				Serial.print(String(j));
				Serial.print(":");
				Serial.print("CH");
				Serial.print(String(tempNote.channel));
				Serial.print(" N");
				Serial.print(String(tempNote.note));
				Serial.print(" (");
				Serial.print(String(tempNote.type, HEX));
				Serial.print(") ");
				Serial.print(String(tempNote.startNote));
				Serial.print(" - ");
				Serial.print(String(tempNote.endNote));
				Serial.println();
			}
		}
		// printRot2(affiche, 15, 400, 3);
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// 2C27 PolyRittim - 1 juillet 2020
		/////////////////////////////////////////////////////////////////////////////////////

	case 546: // numérateur time signature
	{
		numerateurTimeSignature = potVal2;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// 2C27 PolyRittim - 1 juillet 2020
		/////////////////////////////////////////////////////////////////////////////////////

	case 547: // dénominateur time signature
	{
		denominateurTimeSignature = potVal2;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// 2C27 PolyRittim - 2 juillet 2020
		/////////////////////////////////////////////////////////////////////////////////////

	case 548: // basé sur 424:
	{		  // step Sequencer : joue la note sélectionnée : POLYZ
		if (potVal2 > 0 && potVal2 < 128)
		{
			PlaynoteOn_(0x90 + stepChannel, (byte)potVal3, 100);

			addNewNotetoPileONCE(pointeurInitial, stepChannel, (byte)potVal3,
								 0, nowMillis + 1000, 0x80, 1);
		}
		// l'inscription d'un STePZ se fait actuellement par les touches MUTE et se trouve
		// en SubParts.h void checkStepzButtons(void)
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// 2C27 PolyRittim - 14 juillet 2020
		// Random pour le LFO
		/////////////////////////////////////////////////////////////////////////////////////

	case 550:
	{
		LFO[currentLFO].LFORandom = potVal2; // random pour le LFO
		LFO[currentLFO].eventGOON = 0;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// 2C27 PolyRittim - 14 juillet 2020
		// Random pour le LFO
		/////////////////////////////////////////////////////////////////////////////////////

	case 551: // TRANSPOSE CC
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{
					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					if (tempNote.type == 0xB0) // 2C27 : on modifie du pitch
					{
						int newValue = tempNote.note + potVal2;
						if (newValue > 127)
						{
							newValue = 127;
						}
						if (newValue < 0)
						{
							newValue = 0;
						}
						// j'ai préféré réécrire ceci car des fc telles que modifyPitchUniversal et limiterForPitch
						// joue sur des %12 et la nature NOTE de ce qu'on leur envoie.
						tempNote.note = newValue;

						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// 2C27 PolyRittim - 14 juillet 2020
		/////////////////////////////////////////////////////////////////////////////////////

	case 552: // verbose Serial
	{
		switch (potVal2)
		{
		case 0:
		{
			verboseSerial_sendToSerial = !verboseSerial_sendToSerial;
		}
		break;

		case 1:
		{
			verboseSerial_sendToLCD = !verboseSerial_sendToLCD;
		}
		break;

		case 2:
		{
			verboseSerial_sendChannel = !verboseSerial_sendChannel;
		}
		break;

		case 3:
		{
			verboseSerial_sendNotes = !verboseSerial_sendNotes;
		}
		break;

		case 4:
		{
			verboseSerial_sendCC = !verboseSerial_sendCC;
		}
		break;

		case 5:
		{
			verboseSerial_valueCC = !verboseSerial_valueCC;
		}
		break;

		case 6:
		{
			verboseSerial_velocity = !verboseSerial_velocity;
		}
		break;

		case 7:
		{
			verboseSerial_string = !verboseSerial_string;
		}
		break;

		default:
		{
			affiche = "";
		}
		break;
		}

		printRot2(affiche, 16, 400, 3);
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// 2C27 PolyRittim - 14 juillet 2020
		/////////////////////////////////////////////////////////////////////////////////////

	case 553: // Send CC - set CC number
	{
		sendCC_CC = potVal2;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// 2C27 PolyRittim - 14 juillet 2020
		/////////////////////////////////////////////////////////////////////////////////////

	case 554: // Send CC - set CH
	{
		sendCC_CH = potVal2 - 1; // car devient du machine
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// 2C27 PolyRittim - 14 juillet 2020
		/////////////////////////////////////////////////////////////////////////////////////

	case 555: // Send CC - set Value CC and send
	{
		sendCC_Value = potVal2;

		if (machineSpecific[sendCC_CH] == 1) //TX81Z
		{
			// PlaynoteOn_(0xB0 + k, (*playPtr).note, (*playPtr).velocity);
			convertData1and2toTX81ZModif(sendCC_CC, sendCC_Value, sendCC_CH);
		}
		else if (machineSpecific[sendCC_CH] == 2) // SHERMAN
		{
			// PlaynoteOn_(0xB0 + k, (*playPtr).note, (*playPtr).velocity);
			convertData1and2toSHERMAN(sendCC_CC, sendCC_Value, 15); // toujours CHANNE L 16
		}
		else
		{
			Serial3.write((byte)0xB0 + sendCC_CH);
			Serial3.write((byte)sendCC_CC);
			Serial3.write((byte)sendCC_Value);

			// TODO remplacer ceci et dans convertData par PlayNoteOn !!!
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// 2C28 Conway - 17 juillet 2020
		// basé sur 341
		// intégration et modification des procédures ZeroCopy, pour pouvoir modifier le fonctionnement
		/////////////////////////////////////////////////////////////////////////////////////

	case 556: //   Merge ADD, basé sur 341 Mem(s) -> Layer(s)

	{
		// byte compteur = 0;

		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				// layerLength[i] = layerLength[potVal2 - 1 + compteur + 4];
				// doit précéder la copie, pour que les calculs soient OK
				// contrairemnet au 341, l'ADD ne modifie pas la longueur de layer, il le conserve
				// avant 2C28 : ZEROcopyMemoryAtoLayerB(potVal2 - 1 + compteur, i);

				//	  ZEROcopyMemoryAtoLayerB(potVal2 - 1 + compteur, i);
				// = basicCopyPileAtoPileB( potVal2 - 1 + compteur +4, i )
				// = basicCopyPIleAtoPileB( potVal2 - 1 + 4 , i )

				int a = potVal2 - 1 + 4;
				int b = i;

				//					void basicCopyPileAtoPileB(int a, int b)
				{
					unsigned int taille = mesureTaillePileComplete(pInitial[a]);
					if (taille == 0)
					{
					}
					else
					{
						for (unsigned int j = 0; j < taille; j++)
						{ // va parcourir la mélodie pointée ( Layer 1 à 4 : potVal3 )
							pileNote tempNote;
							tempNote = ZEROgetNoteFromPile(pInitial[a], j);
							if (b < 4)
							{
								tempNote.layer = b;
							}
							pileNote_fixStartEndNote(tempNote, 0);
							ZEROaddNewNotetoPile(pInitial[b], tempNote);
						}
					}
				}

				ZEROupdatePileTimeToPlayMODEDIRECT(i);
				// compteur++;
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// 2C29 Conway - 20 juillet 2020
		// adaptation des Johnson réalisés avec Mathieu Lilin l'année passée
		/////////////////////////////////////////////////////////////////////////////////////

	case 557: // Johnson
	{
		switch (potVal2)
		{
		case 0:
		{
			transformJohnson1UP(currentSequence);
		}
		break;

		case 1:
		{
			transformJohnson1DOWN(currentSequence);
		}
		break;

		case 2:
		{
			transformJohnson2UP(currentSequence);
		}
		break;

		case 3:
		{
			transformJohnson2DOWN(currentSequence);
		}
		break;

		case 4:
		{
			transformJohnson3UP(currentSequence);
		}
		break;

		case 5:
		{
			transformJohnson3DOWN(currentSequence);
		}
		break;

		case 6:
		{
			transformJohnson4UP(currentSequence);
		}
		break;

		case 7:
		{
			transformJohnson4DOWN(currentSequence);
		}
		break;
		case 8:
		{
			transformJohnson5UP(currentSequence);
		}
		break;

		case 9:
		{
			transformJohnson5DOWN(currentSequence);
		}
		break;

		case 10:
		{
			transformJohnson6UP(currentSequence);
		}
		break;

		case 11:
		{
			transformJohnson6DOWN(currentSequence);
		}
		break;

		case 12:
		{
			transformJohnson7UP(currentSequence);
		}
		break;

		default:
		{
		}
		break;
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// 2C29 Conway - 20 juillet 2020
		// cut first note : commence par les trier
		/////////////////////////////////////////////////////////////////////////////////////

	case 558:
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				unsigned int taille = mesureTaillePileComplete(pInitial[i]);
				if (taille > 0)
				{
					ZEROtreatPileComplete(pInitial[i]); // on en profite pour un peu faire le ménage, suppression des notes 0 et des killnote
														// c'était le manque de ceci qui faisait foirer toute une procédure - mystère quant à ce que ceci résout !

					sortPile_CLEAN(pInitial[i], 2, 0);
					// 2 = startNote
					// 0 = ascendant
				}

				taille = mesureTaillePileComplete(pInitial[i]);
				if (taille > 0)
				{
					// on prend le start de la première note
					pileNote tempNote;

					tempNote = ZEROgetNoteFromPile(pInitial[i], 0);
					long start = tempNote.startNote;

					tempNote.note = 0;
					tempNote.velocity = 0;
					tempNote.killnote = true;

					ZEROSetNoteInPile(pInitial[i], 0, tempNote);

					for (unsigned int j = 1; j < taille; j++)
					{
						tempNote = ZEROgetNoteFromPile(pInitial[i], j);
						tempNote.startNote = tempNote.startNote - start;
						tempNote.endNote = tempNote.endNote - start;
						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}

					layerLength[i] = layerLength[i] - start;

					ZEROtreatPileComplete(pInitial[i]); // on en profite pour un peu faire le ménage, suppression des notes 0 et des killnote
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// 2C29 Conway - 20 juillet 2020
		// cut last note : commence par les trier
		/////////////////////////////////////////////////////////////////////////////////////

	case 559:
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				unsigned int taille = mesureTaillePileComplete(pInitial[i]);
				if (taille > 0)
				{
					ZEROtreatPileComplete(pInitial[i]); // on en profite pour un peu faire le ménage, suppression des notes 0 et des killnote
														// c'était le manque de ceci qui faisait foirer toute une procédure - mystère quant à ce que ceci résout !

					sortPile_CLEAN(pInitial[i], 2, 0);
					// 2 = startNote
					// 0 = ascendant
				}

				taille = mesureTaillePileComplete(pInitial[i]);
				if (taille > 0)
				{
					// on prend le start de la dernière note
					pileNote tempNote;

					tempNote = ZEROgetNoteFromPile(pInitial[i], taille - 1);
					long start = tempNote.startNote;

					tempNote.note = 0;
					tempNote.velocity = 0;
					tempNote.killnote = true;

					ZEROSetNoteInPile(pInitial[i], taille - 1, tempNote);

					layerLength[i] = start;

					ZEROtreatPileComplete(pInitial[i]); // on en profite pour un peu faire le ménage, suppression des notes 0 et des killnote
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// TOTAL ACCENT 2C24
		// modif 2C28 basé sur 521
		/////////////////////////////////////////////////////////////////////////////////////

	case 560:
	{ // set midi speed ON
		modifMidiSpeedON = !modifMidiSpeedON;
		//		accentMin = potVal2;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// 2C29 Conway - 20 juillet 2020
		// Spread Conway : basé sur 340
		/////////////////////////////////////////////////////////////////////////////////////

	case 561: //  Sauve de Layer vers un pourcentage des Mémoire
		// le fait de façon aléatoire et écrasante
		// il n'y a pas de métissage avec les layers préexistants !
		// version très brutale en fait
		// une version plus raffinée diffuserait autour d'un certain layer
		// mais ceci sert juste à planter un tableau initial

		{
			for (byte i = 0; i < nbLayer; i++)
			{
				if (isLayerActive(i))
				{
					for (byte k = 0; k < nbLayersMemory; k++)
					{
						if (random(100) < (unsigned int)potVal2)
						{
							layerLength[k + 4] = layerLength[i];
							// doit précéder la copie
							ZEROcopyLayerAtoMemoryB(i, k);
							// compteur++;
							currentMem = k;
						}
					}
				}
			}
		}
		break;

		/////////////////////////////////////////////////////////////////////////////////////
		// 2C29 Conway - 27 juillet 2020
		/////////////////////////////////////////////////////////////////////////////////////

	case 562: //  skip note midi player
	{
		GoONmidi = 0;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// CONWAY 2C28
		// 27 juillet 2020
		/////////////////////////////////////////////////////////////////////////////////////

	case 563:
	{ // midi ratio multiplier

		if (potVal2 > 1)
		{
			 NumRatioDeltaToMsec = NumRatioDeltaToMsec * potVal2 ; 
		}

		if (potVal2 < -1)
		{
		DenomRatioDeltaToMsec = DenomRatioDeltaToMsec  * (-potVal2);
		}

		simplifie(NumRatioDeltaToMsec,DenomRatioDeltaToMsec); 
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// 2C29 Conway - 27 juillet 2020
		/////////////////////////////////////////////////////////////////////////////////////

	case 564: //  auto harmonize live playing
	{
		autoHarmonizeLivePlaying = !autoHarmonizeLivePlaying;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// 2C29 Conway - 27 juillet 2020
		/////////////////////////////////////////////////////////////////////////////////////

	case 565: //  auto harmonize live playing GAmme Root Note
	{
		gammeActuelleLive = potVal2;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// 2C29 Conway - 27 juillet 2020
		/////////////////////////////////////////////////////////////////////////////////////

	case 566: //  auto harmonize live playing MOde
	{
		modeActuelleLive = potVal2;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// 2C29 Conway - 27 juillet 2020
		/////////////////////////////////////////////////////////////////////////////////////

	case 567: //  auto harmonize live playing MOde
	{
		typeAutoHarmonizeLivePlayingMode = potVal2;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// 2C28 Conway - 2 août 2020
		/////////////////////////////////////////////////////////////////////////////////////

	case 568: // Delay SubFidelity = Delay Appear
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				// dendeSubFidelity[i] = potVal2;
				randomSubFidelityPourDelais[i] = potVal2;
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// 2C28 Conway - 2 août 2020
		/////////////////////////////////////////////////////////////////////////////////////

	case 569: // définition du Drop pour chaque Layer pour Kannon
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				//	dropPourBarocco[i] = potVal2;
				dropPourKannon[i] = potVal2;
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// CONWAY 2C28
		// 2 août 2020
		/////////////////////////////////////////////////////////////////////////////////////

	case 570:
	{ // sustain blocker
		sustainBlockerON = !sustainBlockerON;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// CONWAY 2C28
		// 5 août 2020
		/////////////////////////////////////////////////////////////////////////////////////

	case 571:
	{ // proxy blocker
		proxyBlockerON = !proxyBlockerON;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// CONWAY 2C28
		// 8 août 2020
		////////////////////////////////////////////////////////////////////////////////////

	case 572:
	{ // proxy blocker pitch value
		proximityPitch = potVal2;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// CONWAY 2C28
		// 23 août 2020
		// basé sur Grid 96
		////////////////////////////////////////////////////////////////////////////////////

	case 573:
	{ // Decalagem : cale les valeurs sur une grille donnée, avec un % de chance de décalage, récursif,
		// jusqu'à un max de potVal2 - 1

		byte nbAB = BBB;

		for (byte i = 0; i < nbLayer; i++)
		{

			if (isLayerActive(i))
			{

				unsigned long lgAB = layerLength[i] / nbAB;

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);
				pileNote tempNote;

				for (unsigned int j = 0; j < taille; j++)
				{
					bool stopIt = false;
					int compteur = 0;

					while (stopIt == false && compteur < maximumDecalagem)
					{
						compteur++;
						if ((int)random(100) < (int)pourcentageDecalagem)
						{

							tempNote = ZEROgetNoteFromPile(pInitial[i], j);
							KeepDuree = getDuree(tempNote);

							unsigned long iAB = tempNote.startNote / lgAB;

							if (random(2) == 0)
							{
								iAB--;
							}
							else
							{
								iAB++;
							}

							if (iAB < 0)
							{
								iAB = nbAB - 1;
							}
							if (iAB >= nbAB)
							{
								iAB = 0;
							}

							tempNote.startNote = iAB * lgAB;

							if ((decalagemSwing != 0) && ((iAB % 2) == 1))
							{
								signed long addendum = (signed long)lgAB * (signed long)decalagemSwing / (signed long)100;
								tempNote.startNote = (signed long)tempNote.startNote + (signed long)addendum;
								if (tempNote.startNote < 0)
								{
									tempNote.startNote = 1;
								}
							}

							if (tempNote.startNote == 0)
							{
								tempNote.startNote++;
							}
							tempNote.endNote = tempNote.startNote + KeepDuree;
							pileNote_fixStartEndNote(tempNote, 0);
							ZEROSetNoteInPile(pInitial[i], j, tempNote);
						}
						else
						{
							stopIt = true;
						}
					}
				}
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// CONWAY 2C28
		// 23 août 2020
		// basé sur Grid 96
		////////////////////////////////////////////////////////////////////////////////////

	case 574:
	{ // Decalagem %
		pourcentageDecalagem = potVal2;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// CONWAY 2C28
		// 23 août 2020
		// basé sur Grid 96
		////////////////////////////////////////////////////////////////////////////////////

	case 575:
	{ // Decalagem %
		maximumDecalagem = potVal2;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// CONWAY 2C28
		// 23 août 2020
		////////////////////////////////////////////////////////////////////////////////////

	case 576:
	{ // shutUP
		shutUP = !shutUP;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// RESIDENZ1 2C31
		// 5 decembre 2020
		////////////////////////////////////////////////////////////////////////////////////

	case 577: // Sequential PostPone
	{

		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				turnOnSequentialPostpone[i] = !turnOnSequentialPostpone[i];
				seqPostpone[i] = potVal2;
			}
		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// BLACK07 - 24 juillet 2021 - rue Hespel
		/////////////////////////////////////////////////////////////////////////////////////

	case 579: // Previous Load Karakt
	{
		String display = "";
		//	for (byte i = 0; i < nbLayer; i++)
		{
			//		if (isLayerActive(i))
			{

				int index = currentKarakt;

				unsigned int tailleCheck;

				do
				{
					index--;
					if (index < 0)
					{
						index = 63;
					}
					tailleCheck = karakt_mesureTaillePileComplete(pKarakt_Header[index]);

				} while (tailleCheck == 0 && index != currentKarakt);

				currentKarakt = index;

				loadKarakt_from_KMem(index);
				display = "K" + String(index + 1) + " loaded";
			}
		}
		printRot2(display, 16, 400, 7);
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// BLACK 08 - rue Hespel
		// 26 juillet 2021
		// basé sur 118, amélioré
		////////////////////////////////////////////////////////////////////////////////////

	case 580:
	{
		affiche = nulString;

		affiche += String(potVal2 % 175);
		affiche += "BPM x";
		affiche += String(potVal2 / 175);
		affiche += "bars";

		float duree = (60000.0 * (float)((potVal2 / 175) + 1)) / (float)(potVal2 % 175);

		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				oldlayerLength = layerLength[i];

				layerLength[i] = (unsigned long)duree;

				if (SqueezeOn == true)
				{
					unsigned int taille = mesureTaillePileComplete(pInitial[i]);

					for (unsigned int j = 0; j < taille; j++)
					{
						pileNote tempNote;
						tempNote = ZEROgetNoteFromPile(pInitial[i], j);

						if (KeepLength == true)
						{
							KeepDuree = getDuree(tempNote);
						}
						else
						{
							KeepDuree = 0;
						}

						tempNote.startNote = (unsigned long)((float)tempNote.startNote * (float)layerLength[i] / (float)oldlayerLength);

						if (KeepLength == true)
						{
							tempNote.endNote = tempNote.startNote + KeepDuree;
						}
						else
						{
							tempNote.endNote = (unsigned long)((float)tempNote.endNote * (float)layerLength[i] / (float)oldlayerLength);
						}

						pileNote_fixStartEndNote(tempNote, 0);
						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
				}
			}
		}
	}
	break;

		/////////////////////////
		// BLACK09
		/////////////////////////

	case 581: // tracker pour Karakt
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				trackerKaraktActif[i] = !trackerKaraktActif[i];
				trackerKaraktSeq[i] = potVal2;
			}
		}
	}
	break;

		/////////////////////////
		// BLACK09
		/////////////////////////

	case 582: // keyboard mode select
	{
		keyboardMode = potVal2;

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

	break;

		/////////////////////////
		// BLACK09
		/////////////////////////

	case 583: // keyboard mode select
	{
		int intCH = potVal2 / 256;
		int intNT = potVal2 % 256;
		dmxTx.set(intCH, intNT);
	}
	break;

		/////////////////////////
		// BLACK09
		/////////////////////////

	case 584: // keyboard mode select
	{
		DMXautoGenerate = !DMXautoGenerate;
	}
	break;

		/////////////////////////
		// BLACK09
		/////////////////////////

	case 585: // Ezy multiplier
	{
		nbStepz = nbStepz * potVal2;
		numberMaxPageStepz = (nbStepz - 1) / nbStepzAffiches;

		// potVal2 est le nombre de fois qu'il faut multiplier le layer
		// repiqué de 371
		// on pourrait faire un raccourci Modification(371,potVal2) ;

		Transformation(371, potVal2);
	}
	break;

		/////////////////////////
		// BLACK09
		/////////////////////////

	case 586: // quick Trackerz Stop
	{

		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				trackerActif[i] = false;
				trackerKaraktActif[i] = false;
			}
		}
		// ne peut pas être sauvé dans Karakt ,
		// car en  tant que telle cette fonction va charger d'autres karakt
		// et donc soit s'annuler, soit allumer d'autres fonctions
		// elle doit rester HORS automatisme

		// ce qu'il faut c'est un shortcut pour l'appeler, surtout !
	}
	break;

		/////////////////////////
		// BLACK09
		// basé sur 573 Decalagem
		/////////////////////////

	case 587:
	{ // Decalagem : cale les valeurs sur une grille donnée, avec un % de chance de décalage, récursif,
		// jusqu'à un max de potVal2 - 1

		byte size = 127 / potVal2;
		byte maximumVelcalagem = 1;
		int pourcentageVelcalagem = 100;

		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				unsigned int taille = mesureTaillePileComplete(pInitial[i]);
				pileNote tempNote;

				for (unsigned int j = 0; j < taille; j++)
				{
					bool stopIt = false;
					int compteur = 0;
					int velocity;

					while (stopIt == false && compteur < maximumVelcalagem)
					{
						compteur++;
						if ((int)random(100) < (int)pourcentageVelcalagem)
						{

							tempNote = ZEROgetNoteFromPile(pInitial[i], j);
							velocity = tempNote.velocity;
							// velocity = ( velocity / size ) * size ; // on approxime la valeur de la note pour commencer

							unsigned int nbRepeats = velocity / size;

							if (random(2) == 0)
							{
								nbRepeats--;
							}
							else
							{
								nbRepeats++;
							}

							if (nbRepeats <= 0)
							{
								nbRepeats = potVal2;
							}

							if ((int)nbRepeats > potVal2)
							{
								nbRepeats = 1;
							}

							tempNote.velocity = nbRepeats * size;
							ZEROSetNoteInPile(pInitial[i], j, tempNote);
						}
						else
						{
							stopIt = true;
						}
					}
				}
			}
		}
	}
	break;

		/////////////////////////
		// BLACK09
		/////////////////////////

	case 588: // set Decalagem Swing
	{
		decalagemSwing = potVal2;
		// -99 va rapprocher le beat impair du beat pair précédent
		// 99 va rapprocher le beat impair du beat pair qui suit
		// 0 ne modifie rien, le beat est calé sur une division parfaite
		// -200 modifie en deça de la valeur
		// +200 modifie au delà
	}
	break;

		/////////////////////////
		// BLACK 10
		/////////////////////////

	case 589: // pinch Value DMX
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				{ // va parcourir la mélodie pointée ( Layer 1 à 4 : potVal3 )
					pileNote tempNote;
					initializeNote(tempNote);

					// on va devoir sauver 2 INT
					// répartis sur 3 byte

					int intCH = potVal2 / 256;
					int intNT = potVal2 % 256;

					tempNote.type = 0x10; // disons que ça signifiera DMX !

					tempNote.channel = intCH & 0x11111111; // les 8 derniers bits
					tempNote.velocity = intCH >> 8;		   // les 8 bits hauts

					tempNote.note = intNT; // byte garde de 0 à 255 donc OK

					tempNote.layer = i;
					tempNote.startNote = nowMillis;
					tempNote.endNote = nowMillis + 1;

					pileNote_fixStartEndNote(tempNote, 0);
					ZEROaddNewNotetoPile(pInitial[i], tempNote);
				}
			}
			// clean-up, on met le bit 2 à 0
		}
	}

	//		dmxTx.set(intCH, intNT);

	break;

		/////////////////////////
		// BLACK 10
		/////////////////////////

	case 590: // vertical CC slide
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{
					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					if (tempNote.type == 0xB0)
					{
						int Pitch = getValueStraightOrFromSequence(potVal2, 0);

						int newValue = (int)tempNote.note + Pitch;

						if (newValue > 127)
						{
							newValue = newValue % 127;
						}
						if (newValue < 0)
						{
							newValue = newValue % 127;
						}

						tempNote.note = newValue;

						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
				}
			}
		}
	}
	break;

		/////////////////////////
		// BLACK 10
		/////////////////////////

	case 591: // horizontal CC slide
	{		  // tout commme pour 590, si on utilise une séquence, elle s'appliquera
			  // CC par CC, par sur l'ensemble de tout le CC
			  // ceci est un peu holé holé mais ça me semble plus intéressant qu'un décalage propre et variable

		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{
					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					if (tempNote.type == 0xB0)
					{
						unsigned long change = getValueStraightOrFromSequence(potVal2, 0);

						unsigned long newStart = (tempNote.startNote + change) % layerLength[i];
						unsigned long newEnd = (tempNote.endNote + change) % layerLength[i];

						tempNote.startNote = newStart;
						tempNote.endNote = newEnd;

						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
				}
			}
		}
	}
	break;

		/////////////////////////
		// BLACK 10
		/////////////////////////

	case 592: // vertical slide note
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{
					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					if (tempNote.type == 0x90)
					{
						int Pitch = getValueStraightOrFromSequence(potVal2, 0);

						int newValue = (int)tempNote.note + Pitch;

						newValue = newValue % 127;

						tempNote.note = newValue;

						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
				}
			}
		}
	}
	break;

		/////////////////////////
		// BLACK 10
		/////////////////////////

	case 593: // horizontal slide note
	{		  // tout commme pour 590, si on utilise une séquence, elle s'appliquera
			  // CC par CC, par sur l'ensemble de tout le CC
			  // ceci est un peu holé holé mais ça me semble plus intéressant qu'un décalage propre et variable

		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{
					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					if (tempNote.type == 0x90)
					{
						unsigned long change = getValueStraightOrFromSequence(potVal2, 0);

						unsigned long newStart = (tempNote.startNote + change) % layerLength[i];
						unsigned long newEnd = (tempNote.endNote + change) % layerLength[i];

						tempNote.startNote = newStart;
						tempNote.endNote = newEnd;

						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
				}
			}
		}
	}
	break;

		/////////////////////////
		// BLACK 10
		/////////////////////////

	case 594: // EMPTY  MULTIPLIER
	{
		// potVal2 est le nombre de fois qu'il faut multiplier le layer

		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				// long initLength = layerLength[i];
				layerLength[i] = layerLength[i] * (long)potVal2;

				// unsigned int taille = mesureTaillePileComplete(pInitial[i]);
			}
			ZEROupdatePileTimeToPlayMODEDIRECT(i);
		}
	}
	break;

		/////////////////////////
		// BLACK 10
		/////////////////////////

	case 595:
	{ // gabbUltrino

		// on définit ici le distance
		gabberito2Interval = potVal2;

		// on définit les valeurs pour cette fois-ci
		long currentGabberito2Interval;
		int currentGabberito2K;
		int currentGabberito2N;

		if (gabberito2K < 0)
		{
			currentGabberito2K = giveNextValue(sequence[-gabberito2K - 1], sequence[-gabberito2K - 1].compteur);
			sequence[-gabberito2K - 1].compteur++;
		}
		else
		{
			currentGabberito2K = gabberito2K;
		}

		// on va devoir jeter dans la pile ONCE un .type = 10
		// qui quand il sera lu va déclencher un MUTE des layers actives
		// et n secondes plus tard, un UNMUTE

		// TODO vérifier que n'interfère pas avec DMX !!! BLACK 10

		for (byte i = 0; i < nbLayer; i++)
		{
			currentGabberito2Interval = 0; // précaution de debug

			if (isLayerActive(i))
			{
				long placeToAdd = 0;

				addNewtoPileBATCH(pBatch, 20, 0, i, nowMillis); // MUTE

				// on va parcourir la pile
				// chaque note va être répéte potVal2 fois, à un intervalle donné : gabberitoInterval
				// on comptabilise la durée de tous ces intervalles pour savoir qd on doit faire l'unmute

				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (int m = 0; m < currentGabberito2K; m++)
				{
					// for (unsigned int j = 0; j < taille; j++)

					// ici se trouve la différence de gabbUltrino :
					// il choisit une note en fc de la séquence indiquée :
					// Seq % taille !

					int j = giveNextValue(sequence[gabbUltrinoSequence], sequence[gabbUltrinoSequence].compteur);
					sequence[gabbUltrinoSequence].compteur++;

					j = j % taille; // on prend une note parmi les notes du layer,
					// en fc de séquence choisie, mais aussi se trouvant dans le layer !

					{
						pileNote tempNote;
						tempNote = ZEROgetNoteFromPile(pInitial[i], j);

						if (gabberito2N < 0)
						{
							currentGabberito2N = giveNextValue(sequence[-gabberito2N - 1], sequence[-gabberito2N - 1].compteur);
							sequence[-gabberito2N - 1].compteur++;
						}
						else
						{
							currentGabberito2N = gabberito2N;
						}

						for (int k = 0; k < currentGabberito2N; k++)
						{

							if (gabberito2Interval < 0)
							{
								currentGabberito2Interval = giveNextValue(sequence[-gabberito2Interval - 1], sequence[-gabberito2Interval - 1].compteur);
								sequence[-gabberito2Interval - 1].compteur++;
							}
							else
							{
								currentGabberito2Interval = gabberito2Interval;
							}

							addNewNotetoPileONCE(pointeurInitial, tempNote.channel, tempNote.note,
												 tempNote.velocity, nowMillis + placeToAdd, 0x90, i);
							placeToAdd += currentGabberito2Interval;
						}
						addNewNotetoPileONCE(pointeurInitial, tempNote.channel, tempNote.note,
											 tempNote.velocity, nowMillis + placeToAdd - 1, 0x80, i);
					}
				}

				addNewtoPileBATCH(pBatch, 20, 0, i, nowMillis + placeToAdd + currentGabberito2Interval + 10); // UNMUTE
			}
		}
	}
	break;

		/*
		long gabberito2Interval = 20;
		int gabberito2K = 1;
		int gabberito2N = 20;
*/

		/////////////////////////
		// BLACK 10
		/////////////////////////

	case 596:
	{
		gabbUltrinoSequence = potVal2;
	}
	break;

		/////////////////////////
		// BLACK 10
		/////////////////////////

	case 597: // set CH and CC
	{

		byte channelCC = potVal2 / 16;
		byte valueCC = potVal2 % 127;

		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				unsigned int taille = mesureTaillePileComplete(pInitial[i]);

				for (unsigned int j = 0; j < taille; j++)
				{
					pileNote tempNote;
					tempNote = ZEROgetNoteFromPile(pInitial[i], j);

					if (tempNote.type == 0xB0)
					{
						tempNote.channel = channelCC;
						tempNote.note = valueCC;

						ZEROSetNoteInPile(pInitial[i], j, tempNote);
					}
				}
			}
		}
	}
	break;

		/////////////////////////
		// BLACK 10
		/////////////////////////

	case 598:
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				premonitionON[i] = !premonitionON[i];
				premonitionValue[i] = potVal2;
			}
		}
	}
	break;

		/////////////////////////
		// BLACK 10
		/////////////////////////

	case 599: // drop pour premonition
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				dropPourPremonition[i] = potVal2;
			}
		}
	}
	break;

		/////////////////////////
		// BLACK 10
		/////////////////////////

	case 600: // premonition random appear
	{
		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				premonitionRandomAppear[i] = potVal2;
			}
		}
	}
	break;

		/////////////////////////
		// BLACK 10
		/////////////////////////

	case 601: // stepz CC randomizer
	{
		int nowNbStepz =  getValueStraightOrFromSequence(potVal2, 2);

		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				// effacer les valeurs de  CH CC concernés

						unsigned int taille = mesureTaillePileComplete(pInitial[i]);

						for (unsigned int j = 0; j < taille; j++)
						{
							pileNote tempNote;
							tempNote = ZEROgetNoteFromPile(pInitial[i], j);

							if (tempNote.channel == rndStepzCH && tempNote.note == rndStepzCC) // n'est pas ACCENT
							{
								putNoteBinNoteA(tempNote, emptyNote);
								tempNote.killnote = true;
							};

							ZEROSetNoteInPile(pInitial[i], j, tempNote);
						}


	// générer le nb de stepz avec CH CC et des valeurs random

for ( int k = 0 ; k < nowNbStepz ; k++ )
{
	long place = ( layerLength[i] * (long) k ) / (long) nowNbStepz ;
	pileNote tempNote;
	putNoteBinNoteA(tempNote, emptyNote);
	tempNote.channel = rndStepzCH ;
	tempNote.note = rndStepzCC ;
	tempNote.velocity = random(0,127);
	tempNote.killnote = false ; 
	tempNote.startNote = place ; 
	tempNote.endNote = place + 1 ;
	tempNote.type = 0xB0 ; // CC
	ZEROaddNewNotetoPile(pInitial[i],tempNote); 
}

		// tout en tenant compte du random absent


			}
		}

	
	}

	break;

	case 602:
	{
		rndStepzCH = (potVal2 ) / 127;
		rndStepzCC = (potVal2 ) % 127;
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Black10
		// 29 janvier 2022 
		/////////////////////////////////////////////////////////////////////////////////////

	case 603:
	{ // reprise de 55 

int acidNbSteps = potVal2 ; 
if (acidNbSteps < 1 ) { acidNbSteps = 2 ; }

	pileNote tempNote;

		for (byte i = 0; i < nbLayer; i++)
		{
			if (isLayerActive(i))
			{
				// effacer la layer
				// diviser en nb de steps
				// parcourir les steps
				// choisir ce qu'on va y installer avec chooseWithRatio
				// créer la note
				// 0xEn en plus ?  pas de suite

		ZEROdeleteWholePile(pInitial[i]); 

for ( int j = 0 ; j < acidNbSteps ; j++ )
{
	// on définit le start et end de ce step
	// en faisant attention à une répartition la plus égale possible des steps

	unsigned long acidStart = layerLength[i] * (unsigned long) j / (unsigned long) acidNbSteps ;
	unsigned long acidEnd = layerLength[i] * (unsigned long)( j + 1 ) / (unsigned long)acidNbSteps;
	unsigned long acidLength = acidEnd - acidStart ; 

int choice = chooseWithRatio(acidNbFullNote,acidNbEmptyNote,acidNbShortNote,acidNbSlideNote,acidNbTripletNote); 
Serial.print("choix="); 
Serial.println(choice); 



if ( choice == 0 )  // Long note 
{
	initializeNote(tempNote);
				tempNote.note = random(acidPitchMin,acidPitchMax); 
				tempNote.channel = acid_CH; 
				tempNote.layer = i;
				tempNote.velocity = random(acidVelocityMin,acidVelocityMax); 
				tempNote.startNote = acidStart + 2 ; 
				tempNote.endNote = acidEnd - 2 ;
				 tempNote.channelReallyPlayed = acid_CH;
            tempNote.killnote = false;
            tempNote.type = 0x90;
			tempNote.noteReallyPlayed = tempNote.note;
			tempNote.thereIsAnEnd = true;

			pileNote_fixStartEndNote(tempNote, 0);
			ZEROaddNewNotetoPile(pInitial[i], tempNote);
}

// choice == 1 : on ne fait rien, empty note 

if ( choice == 2 ) // short note 
{
	initializeNote(tempNote);
				tempNote.note = random(acidPitchMin,acidPitchMax); 
				tempNote.channel = acid_CH; 
				tempNote.layer = i;
				tempNote.velocity = random(acidVelocityMin,acidVelocityMax); 
				tempNote.startNote = acidStart + 2 ; 
				tempNote.endNote = acidStart + 10  ;
				 tempNote.channelReallyPlayed = acid_CH;
            tempNote.killnote = false;
            tempNote.type = 0x90;
			tempNote.noteReallyPlayed = tempNote.note;
			tempNote.thereIsAnEnd = true;

			pileNote_fixStartEndNote(tempNote, 0);
			ZEROaddNewNotetoPile(pInitial[i], tempNote);
}


if ( choice == 3 ) // slide note 
{
	initializeNote(tempNote);
				tempNote.note = random(acidPitchMin,acidPitchMax); 
				tempNote.channel = acid_CH; 
				tempNote.layer = i;
				tempNote.velocity = random(acidVelocityMin,acidVelocityMax); 
				tempNote.startNote = acidStart + 2 ; 
				tempNote.endNote = acidEnd + acidLength / 2   ; // slide jusqu'au milieu de la note suivante 
				 tempNote.channelReallyPlayed = acid_CH;
            tempNote.killnote = false;
            tempNote.type = 0x90;
			tempNote.noteReallyPlayed = tempNote.note;
			tempNote.thereIsAnEnd = true;

			pileNote_fixStartEndNote(tempNote, 0);
			ZEROaddNewNotetoPile(pInitial[i], tempNote);
}

if (choice == 4) // triplet note
{
	initializeNote(tempNote);
	tempNote.note = random(acidPitchMin, acidPitchMax);
	tempNote.channel = acid_CH;
	tempNote.layer = i;
	tempNote.velocity = random(acidVelocityMin, acidVelocityMax);
	tempNote.channelReallyPlayed = acid_CH;
	tempNote.killnote = false;
	tempNote.type = 0x90;
	tempNote.noteReallyPlayed = tempNote.note;
	tempNote.thereIsAnEnd = true;

	int nbSub = 3 ; // nombre de suvdivisions // on peut d'emblée prévoir une valeur mod 
	unsigned long sp = 3 ; // nb de msec entre chaque burst 
unsigned long burstLength =  acidLength - ( (unsigned long) nbSub +1 ) * sp ;
burstLength /= (unsigned long ) nbSub ; 

	for ( int m = 0 ; m < nbSub ; m++)
	{
		tempNote.startNote = acidStart + ((unsigned long)m * (unsigned long)( burstLength + sp ) ) + (unsigned long ) sp ;
		tempNote.endNote = acidStart + ((unsigned long) ( m + 1 ) * (unsigned long)(burstLength + sp));
		pileNote_fixStartEndNote(tempNote, 0);
	}
	ZEROaddNewNotetoPile(pInitial[i], tempNote);
	ZEROupdatePileTimeToPlayMODEDIRECT(i);
}
						}
						
									}

			ZEROtreatPileComplete(pInitial[i]);

		}
	}
	break;

		/////////////////////////////////////////////////////////////////////////////////////
		// Black10
		// 29 janvier 2022
		/////////////////////////////////////////////////////////////////////////////////////

	case 604:
		acid_CH = potVal2 - 1 ; 
		break; //	int acid_CH = 0; // CH pour la bassline Acid
	case 605:
	acidNbFullNote = potVal2 ; 
 		break; //	int acidNbFullNote = 10;
	case 606:
	acidNbEmptyNote = potVal2; 
 		break; //	int acidNbEmptyNote = 7;
	case 607:
	acidNbShortNote = potVal2; 
 		break; //	int acidNbShortNote = 3;
	case 608:
	acidNbTripletNote = potVal2 ; 
 		break; //	int acidNbTripletNote = 2;
	case 609:
	acidNbSlideNote = potVal2 ; 
 		break; //	int acidNbSlideNote = 5;
			   // pour ces nombres, je n'emploie plsu des pourcentages, mais des ratio
			   // par exemple si l'un vaut 2 et l'autre 3, la première apparaitra 2/(2+3) et l'autre 3/(2+3)
	case 610:
	acidPitchMin = potVal2 ; 
 		break; //	int acidPitchMin = 24; // pour Bhringer TD3
	case 611:
	acidPitchMax = potVal2 ; 
 		break; //	int acidPitchMax = 72; // pour Behringer TD3
	case 612:
	acidVelocityMin = potVal2 ; 
 		break; //	int acidVelocityMin = 80;
	case 613:
	acidVelocityMax = potVal2 ; 
 		break; //	int acidVelocityMax = 120;

	case 614:
		acidNbDivide = potVal2;
		break; //	int acidVelocityMax = 120;


		///////////////////////////////////////////////////////////////////////////////	///
		// DEFAULT - do not remove !
		/////////////////////////////////////////////////////////////////////////////////////

	default:
	{
	}
	break;

		// on peut imaginer faire un edit de valeur d'Event
		// EDIT for Event(i) : CC     ****NOTE SUPER IMPORTANT
		//
		// on peut imaginer que la machine fasse un LOG dans sa mémoire pour voir jusqu'où elle a pu aller avant de planter
		// EVOLUTIONARY MACHINES

		// le Default break est au début

	} // end of switch
	  //
	  // if ( TTT == 323 )
	  // {
	  // 	// Comb2();
	  // }

} // end of Transformation

// END OF TRANSFORMATIONS
