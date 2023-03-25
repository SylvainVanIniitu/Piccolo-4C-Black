/*
Su Doku

Problem 96
Su Doku (Japanese meaning number place) is the name given to a popular puzzle concept. Its origin is unclear, but credit must be attributed to Leonhard Euler who invented a similar, and much more difficult, puzzle idea called Latin Squares. The objective of Su Doku puzzles, however, is to replace the blanks (or zeros) in a 9 by 9 grid in such that each row, column, and 3 by 3 box contains each of the digits 1 to 9. Below is an example of a typical starting puzzle grid and its solution grid.

p096_1.png     p096_2.png
A well constructed Su Doku puzzle has a unique solution and can be solved by logic, although it may be necessary to employ "guess and test" methods in order to eliminate options (there is much contested opinion over this). The complexity of the search determines the difficulty of the puzzle; the example above is considered easy because it can be solved by straight forward direct deduction.

The 6K text file, sudoku.txt (right click and 'Save Link/Target As...'), contains fifty different Su Doku puzzles ranging in difficulty, but all with unique solutions (the first puzzle in the file is the example above).

By solving all fifty puzzles find the sum of the 3-digit numbers found in the top left corner of each solution grid; for example, 483 is the 3-digit number found in the top left corner of the solution grid above.
*/

#include <iostream>
#include <math.h>
#include <string.h>
#include <vector>
#include <string>
#include <windows.h>   // WinApi header
#include <stdlib.h>     /* srand, rand */

using namespace std;


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

byte const tailleMaxTableauSequence(64); // = nb de data max dans une s�quence

string nulstring = "";

typedef struct sequenceStruct sequenceStruct;
struct sequenceStruct
{
    int tableauSeq[tailleMaxTableauSequence];
    int tailleSeq;
    byte modeSeq; // 0 FWD    1 BACK    2 RND    3 A-R    4 JITTER    5 RND-FRG
    boolean actif;

    byte jitterMinUP;
    byte jitterMaxUP;
    // on pourrait coder 16 sur 2 demi-bytes NOTE DEBUG
    byte jitterMinDOWN;
    byte jitterMaxDOWN;
    byte jitterStaticMin;
    byte jitterStaticMax;
    int jitterRichting; // variables internes, qu'on ne modifie pas soi m�me
    // NOTE DEBUG peut �tre transform� en version courte, si code suit

    int jitterWhereNow; // variables internes, qu'on ne modifie pas soi m�me
    int jitterWhereTo;  // variables internes, qu'on ne modifie pas soi m�me

    byte jitterStaticCompteur; // variables internes, qu'on ne modifie pas soi m�me
    byte jitterStaticToReach;  // variables internes, qu'on ne modifie pas soi m�me

    // int jitterPolarity(0); // -1 : wherenow < whereTo : +1 : > : 0 =
    // int oldJitterPolarity(0) ; // derni�re valeur
    boolean beingStatic; // variables internes, qu'on ne modifie pas soi m�me
    // on pourrait coder les bool sur des bits NOTE DEBUG
    //
    byte sizeRANDOMFRAG;
    byte repeatRANDOMFRAG;
    byte toReachRANDOMFRAG; // variables internes, qu'on ne modifie pas soi m�me
    byte keepN;

    boolean compteLayer; // si cette valeur est fausse il compte les notes
    // si elle est vraie, il compte les layers

    long compteur; // +1 � chaque it�ration, vaudra un grand nombre

    byte action; // 00 = CC    1 = NOte     2 = Velocite    plus tard : 3 = SWING
    // byte type ; // 0 FWD    1 BACK    2 RND    3 A-R    4 JITTER    5 RND-FRG est MODESEQ
    byte CC;
    byte channel;
    byte layer;

    // const byte modeFWD = 0 ; //
    // const byte modeBACK = 1 ;//
    // const byte modeRANDOM = 2 ;
    // const byte modeALLERRETOUR = 3 ;
    // const byte modeJITTER = 4 ;
    // const byte modeRANDOMFRAG = 5 ;

    // NOTE DEBUG ceci sera pour plus tard !
    // sequenceStruct* previousSeq  ;
    // sequenceStruct* nextSeq  ;
    //byte numeroSeq ; // pour un peu s'y retrouver !

}; // ON OUBLIE TOUJOURS LE POINT VIRGULE ICI !

byte const nbSequencesMax = 32;

sequenceStruct sequence[nbSequencesMax];

string F(string A)
{
    return  A ;
}
string transformSequenceEnstring(sequenceStruct &tab)
{
	string versionTemporaire = "(" ;

	for (byte i = 0; i < tab.tailleSeq; i++)
	{
		if (tab.tableauSeq[i] > 0)
		{
			versionTemporaire += "+" ;
			versionTemporaire +=  to_string(tab.tableauSeq[i]);
		}
		else
		{
			versionTemporaire += to_string(tab.tableauSeq[i]);
		}

		if (i != (tab.tailleSeq - 1))
		{
			versionTemporaire += ",";
		}
	}
	versionTemporaire += F(")");
	return versionTemporaire;
}

int random ( int a, int b )
{
    int c ;
    c =   rand() % (b-a) + a ;
    return c ;
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
		{ // on veille � ne pas remplir un array au del� de sa taille
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

int tailleBetween2Numbers(int a, int b) // fonction utilis�e par GiveNextValue : donne distance entre 2 chiffres, sauf si IDEM
// si ils sont idntiques, donnera 1, pour pr�server l'apparition de 2x le m�me chiffre dans une s�quence

{
	int c = b - a;

	c = abs(c);

	if (c == 0)
	{
		c++;
	}

	return c;
}

int giveNextValue(sequenceStruct(&tab)) ;

int processReturn ( int b )
{
    if ( b <= -100 )
        {
            int a ;
            a = giveNextValue(sequence[-(b+100)]);
            return a ;
        }
        else
			{
			    return b ;
			}
}

int giveNextValue(sequenceStruct(&tab))
{

long compteurThat = tab.compteur ;

    tab.compteur++ ;

	// on rep�re sur le compteur � quelle valeur du tableau on fera r�f�rence
	// le nb de note est donn� par tab[0]
	// on fait donc compteur % tab[0] pour avoir d�coupage : 0, 1        0, 1, 2, .. nb notes-1
	// si FWD -> on donne valeur du modulo + 1
	// si BACK -> on donnne valeur de taille - modulo
	// si FWDBACK -> on doit calculer un modulo par ( tab[0]*2 ) si le r�sultat <= tab[0] alors calcul FWD, sinon calcul BACK
	// si RANDOM -> on donne une valeur random parmi le tableau
	// si JITTER -> on devrait savoir � quelle fr�quence le jitter change de sens
	// case 6:	affiche += F("7.progression a->b->c");
	// case 7:	affiche += F("8.rnd spectum a-b");
	// case 8:	affiche += F("9.bipolar A *c-d E*f-g H%deA");
	// case 9:	affiche += F("10.bipolar A B C%deA");
	// case 10:	affiche += F("11.n repeater");

	int decoupe;
	int taille = tab.tailleSeq;
	byte mode = tab.modeSeq;

	if (taille == 0)
	{
		return 0;
	}

	if (taille == 1)
	{
		return ( processReturn ( tab.tableauSeq[0] ) );
	}

	if (taille > 1)
	{

		decoupe = (int)((long)compteurThat % (long)taille);

		if (mode == modeFWD)
		{
			return ( processReturn (tab.tableauSeq[decoupe] )) ;
		}
		else if (mode == modeBACK)
		{
			return ( processReturn (tab.tableauSeq[taille - decoupe - 1])) ;
		}
		else if (mode == modeRANDOM)
		{
			return ( processReturn ( tab.tableauSeq[random(0, taille)] )) ;
		}
		else if (mode == modeALLERRETOUR)
		{
			byte tailleAR = (taille * 2) - 2;
			byte decoupeAR = (byte)(compteurThat % tailleAR);

			if (decoupeAR < taille)
			{
				return ( processReturn (tab.tableauSeq[decoupeAR] ));
			}
			else
			{
				return ( processReturn (tab.tableauSeq[tailleAR - decoupeAR] )) ;
			}
		}
		else if (mode == modeJITTER)
		{
			if (tab.jitterWhereTo < 0)
			{
				// Serial.println("WOW problem !!! WhereTo n�gatif ou nul ");
				while (tab.jitterWhereTo < 0)
				{
					tab.jitterWhereTo += taille;
				};
				tab.jitterWhereTo = abs(tab.jitterWhereTo % taille);
			}

			if (tab.jitterWhereNow < 0)
			{
				// Serial.println("WOW problem !!! WhereNow n�gatif !");
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

			else // ��d NON STATIQUE, est en train de bouger
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
					// �a signifie qu'on est arriv� sur la cible
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

			return ( processReturn (tab.tableauSeq[tab.jitterWhereNow])) ;
		}
		else if (mode == modeRANDOMFRAG)
		{
			// pour avoir un tableau non partag� par diff�rentes s�quences on doit le recalculer
			// pr�f�rable au fait de l'enregistrer pour chaque s�quence ( 30 digits par s�quence ! )

			int subSequence[tab.sizeRANDOMFRAG];

			if (tab.repeatRANDOMFRAG == 0)
				{
	tab.repeatRANDOMFRAG = 1 ;
				}


			// if ((compteurThat % tab.sizeRANDOMFRAG) == 0)
			// if ((compteurThat % tab.repeatRANDOMFRAG ) == 0)
			{
				if (tab.repeatRANDOMFRAG >= tab.toReachRANDOMFRAG)
				{
					tab.repeatRANDOMFRAG = 0;
					byte n = random(0, taille); // point de d�part entre 1 et la derni�re valeur du TAb
					tab.keepN = n;
				}

				tab.repeatRANDOMFRAG++;
			}

			for (byte i = 0; i < tab.sizeRANDOMFRAG; i++)
			{
				byte m;
				m = tab.keepN + i;
				m = m % taille ;
				subSequence[i] = tab.tableauSeq[m];
			}

			return ( processReturn ( subSequence[(compteurThat % tab.sizeRANDOMFRAG)] ) ); // redonner ce r�sultat, sans passer par la cr�ation d'un tableauSequenceRand !

		} // fin RandomFRAG
		else if (mode == modePROGRESSION)
		{ // ex [1,10,3] devient [1 2 3 4 5 6 7 8 9 10 9 8 7 6 5 4 3]
			// d'abord calculer la taille du tableau n�cessaire
			// puis l'�tendre
			// on part d'un array qui contient des valeurs n�gatives et positives, dont on connait la taille
			// tab1[10]
			// tab1Size
			// on veut l'�tendre vers un autre array

			// tab1[] remplac� par tab.tableauSeq[]
			int tab1Size = tab.tailleSeq;

			int step = 1; // TODO on doit r�fl�chir � comment le modifier pour une s�quence - � priori menu suppl�mentaire ?

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
			return ( processReturn ( tab2[decoupe] )) ; // on renvoie la valeur du tableau2, version expandue du tabSeq !
		}
		else if (mode == modeSPECTRUM)
		{ // on part du m�me concept que progression sauf qu'ici on choisira au hasard

			// tab1[] remplac� par tab.tableauSeq[]
			int tab1Size = tab.tailleSeq;

			int step = 1; // TODO on doit r�fl�chir � comment le modifier pour une s�quence - � priori menu suppl�mentaire ?

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
			return ( processReturn ( tab2[random(0, tab2Size)] ) ) ; // on renvoie la valeur du tableau2, version expandue du tabSeq !
		}
		else if (mode == modeBIPOLAR)
		{
			if (tab.tailleSeq < 6)
			{
				// printRot2("NOT ENUF NUMB", 16, 300, 3);
				// delay(1000);
				return 0;
			}

			// A *c-d E*f-g H%deA
			int duree;
			if (random(0, 100) < tab.tableauSeq[6])
			{
				// on attribue une dur�e en fc des crit�res pole1
				// DEBUG v�rifier que random fonctionne avec des chiffres n�gatifs
				int a = random(tab.tableauSeq[1], tab.tableauSeq[2] + 1);
				duree = a * tab.tableauSeq[0];
			}
			else
			{
				// on attribue une dur�e en fc des crit�res pole2
				int a = random(tab.tableauSeq[4], tab.tableauSeq[5] + 1);
				duree = a * tab.tableauSeq[3];
			}

			return ( processReturn ( duree ) ) ;
		}

		// const byte modeBIPOLAR = 8 ;
		// const byte modeSIMPLEBIPOLAR = 9 ;
		// const byte modeREPEATER = 10 ;

	} // fin Taille > 1

	return 0 ;

} // fin GiveNextValue

string givestringOfSequence(byte s)
{
	string affiche = nulstring;

	affiche = F("Seq");
	affiche += to_string(s + 1);
	affiche += "=[";

	byte i(0);

	while (i < sequence[s].tailleSeq)
	{
		string ajout = to_string((int)sequence[s].tableauSeq[i]);
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

string giveShortstringOfSequence(byte s)
{
	string affiche = nulstring;

	affiche =  F("Seq");
	affiche += to_string(s + 1);
	affiche += F(" ");

	return affiche;
}

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
// adaptation des Johnson r�alis�s avec Mathieu Lilin l'ann�e pass�e
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

	// J'ai eu du fil � retordre ici, en effet je commmen�ais par transformer le d�but de la s�quence
	// et puis la fin, oubliant que la transfo du d�but modifiait les valeurs de fin !!!!

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


void initializeSequence(byte t)
{
    for (int i = 0; i < tailleMaxTableauSequence; i++)
    {
        sequence[t].tableauSeq[i] = 0;
    }

    sequence[t].tailleSeq = 0;
    sequence[t].modeSeq = 0; // 0 FWD    1 BACK    2 RND    3 A-R    4 JITTER    5 RND-FRG
    sequence[t].actif = false;

    sequence[t].jitterMinUP = 1;
    sequence[t].jitterMaxUP = 1;
    sequence[t].jitterMinDOWN = 1;
    sequence[t].jitterMaxDOWN = 1;
    sequence[t].jitterStaticMin = 0;
    sequence[t].jitterStaticMax = 0;
    sequence[t].jitterRichting = 1; // variables internes, qu'on ne modifie pas soi m�me
    // NOTE DEBUG peut �tre transform� en version courte, si code suit
    sequence[t].jitterWhereNow = 0; // variables internes, qu'on ne modifie pas soi m�me
    sequence[t].jitterWhereTo = 1;  // variables internes, qu'on ne modifie pas soi m�me

    sequence[t].jitterStaticCompteur = 0; // variables internes, qu'on ne modifie pas soi m�me
    sequence[t].jitterStaticToReach = 0;  // variables internes, qu'on ne modifie pas soi m�me

    // int jitterPolarity(0); // -1 : wherenow < whereTo : +1 : > : 0 =
    // int oldJitterPolarity(0) ; // derni�re valeur
    sequence[t].beingStatic = false; // variables internes, qu'on ne modifie pas soi m�me

    sequence[t].sizeRANDOMFRAG = 1;
    sequence[t].repeatRANDOMFRAG = 1;
    sequence[t].toReachRANDOMFRAG = 1; // variables internes, qu'on ne modifie pas soi m�me
    sequence[t].keepN = 0;

    sequence[t].compteur = 0; // +1 � chaque it�ration, vaudra un grand nombre
    sequence[t].compteLayer = false;

    sequence[t].action = 1; // 00 = CC    1 = NOte     2 = Velocite    plus tard : 3 = SWING
    // byte type ; // 0 FWD    1 BACK    2 RND    3 A-R    4 JITTER    5 RND-FRG est MODESEQ
    sequence[t].CC = 33;
    sequence[t].channel = 0;
    sequence[t].layer = 0;
}

void initializeAllSequences(void)
{
    for (byte i = 0; i < nbSequencesMax; i++)
    {
        initializeSequence(i);
    }
}


inline void wait_on_enter()
{
      std::string dummy;
      std::cout << std::endl << "Enter to continue..." << std::endl;
      std::getline(std::cin, dummy);
      cout << endl << endl ;
}

int main( )
{

initializeAllSequences() ;


    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

     addValue2Sequence(sequence[1],14);
     addValue2Sequence(sequence[1],224);
     addValue2Sequence(sequence[1],14);
     addValue2Sequence(sequence[1],224);
     addValue2Sequence(sequence[1],14);
     addValue2Sequence(sequence[1],14);
   //addValue2Sequence(sequence[1], -100-3 );
   //addValue2Sequence(sequence[1], -100-4 );
    sequence[1].modeSeq = 0;
    sequence[1].jitterMinUP = 0 ;
    sequence[1].jitterMaxUP = 1 ;
    sequence[1].jitterMinDOWN = 0 ;
    sequence[1].jitterMaxDOWN = 1 ;
    sequence[1].jitterStaticMin = 50 ;
    sequence[1].jitterStaticMax= 100 ;
    sequence[1].sizeRANDOMFRAG = 1 ;
    sequence[1].toReachRANDOMFRAG = 50 ;

     addValue2Sequence(sequence[2],14);

     addValue2Sequence(sequence[2],224);     addValue2Sequence(sequence[2],224);     addValue2Sequence(sequence[2],14);

     addValue2Sequence(sequence[2],224);     addValue2Sequence(sequence[2],224);     addValue2Sequence(sequence[2],14);

     addValue2Sequence(sequence[2],224);     addValue2Sequence(sequence[2],224);
   //addValue2Sequence(sequence[1], -100-3 );
   //addValue2Sequence(sequence[1], -100-4 );
    sequence[2].modeSeq = 3;
    sequence[2].jitterMinUP = 0 ;
    sequence[2].jitterMaxUP = 1 ;
    sequence[2].jitterMinDOWN = 0 ;
    sequence[2].jitterMaxDOWN = 1 ;
    sequence[2].jitterStaticMin = 20 ;
    sequence[2].jitterStaticMax= 40 ;
    sequence[2].sizeRANDOMFRAG = 1 ;
    sequence[2].toReachRANDOMFRAG = 30 ;

    addValue2Sequence(sequence[3],-100-1);
    addValue2Sequence(sequence[3], -100-2 );
   sequence[3].modeSeq = 0 ;
    sequence[3].jitterMinUP = 0 ;
    sequence[3].jitterMaxUP = 1 ;
    sequence[3].jitterMinDOWN = 0 ;
    sequence[3].jitterMaxDOWN = 1 ;
    sequence[3].jitterStaticMin = 50 ;
    sequence[3].jitterStaticMax= 70 ;
    sequence[3].sizeRANDOMFRAG = 1 ;
    sequence[3].toReachRANDOMFRAG =  600 ;


        addValue2Sequence(sequence[4],33);
        addValue2Sequence(sequence[4],31);
        addValue2Sequence(sequence[4],32);
    addValue2Sequence(sequence[4], -100-1) ;
    sequence[4].modeSeq = 2 ;
    sequence[4].jitterMinUP = 0 ;
    sequence[4].jitterMaxUP = 1 ;
    sequence[4].jitterMinDOWN = 0 ;
    sequence[4].jitterMaxDOWN = 1 ;
    sequence[4].jitterStaticMin = 50 ;
    sequence[4].jitterStaticMax= 10 ;
    sequence[4].sizeRANDOMFRAG = 1 ;
    sequence[4].toReachRANDOMFRAG = 50 ;



 /*
    byte modeSeq; // 0 FWD    1 BACK    2 RND    3 A-R    4 JITTER    5 RND-FRG

*/

for ( int i = 0 ; i < 255 ; i++ )
{
      SetConsoleTextAttribute(hConsole, i);
      cout << " " << i << " " ;
}

wait_on_enter() ;


    for ( int i = 0 ; i < 12550 ; i++ )
    {

int color = giveNextValue(sequence[3])  ;

            SetConsoleTextAttribute(hConsole, color );

            int bit =   giveNextValue(sequence[4]) ;

 // cout << char (bit) ;
 cout << "/" ;

// cout << "*" ;


      // you can loop k higher to see more color choices

    }

}




