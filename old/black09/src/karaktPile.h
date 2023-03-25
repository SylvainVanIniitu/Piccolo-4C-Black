#include <Arduino.h>

// PROTOTYPES, fc de SaveLoadLocal

void byte_loadFromKaraktDISMISS(int number, int layer, byte *here);
void int_loadFromKarakt(int number, int *here);
void multi_int_loadFromKarakt(int number, int *here);
void bool_loadFromKarakt(int number, bool *here);
void multi_bool_loadFromKarakt(int number, bool *here);
void single_bool_loadFromKarakt(int number, int layer, bool *here) ; 
void free_KMem(byte place);

// byte val(0);

// à partir de pileNote, travail sur les fonctions de base pour arriver à créer ceci :
// une série de Karakt_Headers, de [0] à [64]
// de là, pointeurs vers une chaîne de pointeurs indiquant des valeurs
// chacune des valeurs comporte
// - un int = karakt valeur
// - une karakt number
// - un karakt layer ( ou numéro pour les CHANNEL )
// - un pointeur NEXT
//
// il faudra créer :
// SAVE KARAKT TO K_MEM  : cette fc ne doit sauver que l'essentiel, en comparant la valeur actuelle et celle enregistrée dnas le K_MEM
// précédent non vide
// nécessite CREE MAILLON
// nécessite EXISTETIL KARAKT NUMBER (n) dans HEADER (h) ?
// nécessite GIVEVALUE KARAKT NUMBER (n) dans HEADER (h)
// LOAD KARAKT FROM MEM : nécessite de parcourir les K_MEM en sens inverse, pour reconstituer les valeurs
//

const byte karakt_nbTotalPiles = 64;

pileKarakt karakt_Header[karakt_nbTotalPiles + 1]; // +1 de précaution
pileKarakt *pKarakt_Header[karakt_nbTotalPiles + 1];

/////////////////////////////////////////////////////////////////////////////////////
// 2C22 CORONADAYZ
// code solide, simple, super rapide je suppose
/////////////////////////////////////////////////////////////////////////////////////

pileKarakt *karakt_giveDernierPtrPileComplete(pileKarakt *pilePtr)
{
    pileKarakt *thisPtr = pilePtr;
    while ((*thisPtr).nextPile != NULL)
    {
        thisPtr = (*thisPtr).nextPile;
    }
    return thisPtr;
}

/////////////////////////////////////////////////////////////////////////////////////
// 2C22 CORONADAYZ
// code :
/////////////////////////////////////////////////////////////////////////////////////

void karakt_putKaraktBinA(pileKarakt &A, pileKarakt &B)
{
    A.valeur = B.valeur;
    A.number = B.number;
    A.layer = B.layer;
    A.nextPile = B.nextPile;
}

/////////////////////////////////////////////////////////////////////////////////////
// 2C22 CORONADAYZ
// code :
/////////////////////////////////////////////////////////////////////////////////////

void karakt_addNewKarakttoPile(pileKarakt *cettePileADD,
                               pileKarakt addedKarakt)
{
    pileKarakt *pilePtr;

    pilePtr = karakt_giveDernierPtrPileComplete(cettePileADD);

    (*pilePtr).nextPile = new pileKarakt;
    pilePtr = (*pilePtr).nextPile;

    karakt_putKaraktBinA((*pilePtr), addedKarakt);

    // SAUF
    (*pilePtr).nextPile = 0; // ne pointe vers rien car est la dernière
}

/////////////////////////////////////////////////////////////////////////////////////
// 2C22 CORONADAYZ
// code : amélioré, simplifié, ne parcourt qu'une fois la pile, jusqu'où nécessaire, pas plus
/////////////////////////////////////////////////////////////////////////////////////

bool exist_numberA_andLayerB_KaraktinPileC(int number, int layer, pileKarakt *pilePtr)
{
    pileKarakt *thisPtr = pilePtr;
    while ((*thisPtr).nextPile != NULL)
    {
        thisPtr = (*thisPtr).nextPile;
        if ((*thisPtr).number == number && (*thisPtr).layer == layer)
        {
            return true;
        }
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////////////////
// 2C22 CORONADAYZ
// code : amélioré, simplifié, ne parcourt qu'une fois la pile, jusqu'où nécessaire, pas plus
/////////////////////////////////////////////////////////////////////////////////////

int giveValue_numberA_andLayerB_KaraktinPileC(int number, int layer, pileKarakt *pilePtr)
{
    pileKarakt *thisPtr = pilePtr;
    while ((*thisPtr).nextPile != 0)
    {
        thisPtr = (*thisPtr).nextPile;
        if ((*thisPtr).number == number && (*thisPtr).layer == layer)
        {
            return (*thisPtr).valeur;
            // return true;
        }
    }
    return 0;
}

// AJOUTER
// donner la valeur de Karakt de nième champ de Header : utile pour recopier un Header vers un autre
// NON, car on charge un header vers valeurs et on sauve valeurs vesr header, pas copier direct
// garder la macro : copie de HEADER vers un autre ? non
// effacer Header
// OUI
// SAVE Karakt
// LOAD Karakt

bool exist_NumberA_andLayerB_KaraktinPilesBetweenCandDincluded(int number, int layer, int numberC, int numberD)
{
    for (  int i = numberC; i <= numberD; i++)
    {
        if (exist_numberA_andLayerB_KaraktinPileC(number, layer, pKarakt_Header[i]) == true)
        {
            return true;
        }
    }
    return false;
}

int giveLastValueBeforePileC(int number, int layer, int place)
{
    for (int j = place; j > 0; j--)
    {
        if (exist_numberA_andLayerB_KaraktinPileC(number, layer, pKarakt_Header[j - 1]))
        {
            // on trouve dans un layer inférieur une variable identique

            return (giveValue_numberA_andLayerB_KaraktinPileC(number, layer, pKarakt_Header[j - 1]));
        }
    }
    return 0;
}

pileKarakt *currentWorkingPlace;
 
/////////////////////////////////////////////////////////////////////////////////////
// 2C22 CORONADAYZ
// Econome_ versions
/////////////////////////////////////////////////////////////////////////////////////

void econome_multi_int_saveToKMem(byte place, int number, int layer, int value[])
{
    for (int i = 0; i < layer; i++)
    {
        currentWorkingPlace = pKarakt_Header[place];

        bool forceSave = true;

        for (int j = place; j > 0; j--)
        {
            if (exist_numberA_andLayerB_KaraktinPileC(number, i, pKarakt_Header[j - 1]))
            {
                // on trouve dans un layer inférieur une variable identique

                if (giveValue_numberA_andLayerB_KaraktinPileC(number, i, pKarakt_Header[j - 1]) == value[i])
                {
                    // identique : on ne doit pas sauver
                    j = 0;
                    forceSave = false;
                }
                else
                {
                    // déterminer si le header supérieur prochain qui contient un même number+layer, a une valeur différente
                    // si OUI c'est un insert
                    // si la valeur juste au dessus (header+1) est différente, on fait rien
                    // si le slot juste au dessus est vide, on remplit

                    // on a donc trouvé dans un header inférieur un même number, mais valeur différente
                    // 1er cas de figure : on n'a pas de header supérieur avec même number = AJOUT

                    // if (     exist_numberA_andLayerB_KaraktinPileC( number, i, pKarakt_Header[place+1] == false )

                    pileKarakt A;

                    A.valeur = value[i];
                    A.number = number;
                    A.layer = i;
                    A.nextPile = NULL;

                    karakt_addNewKarakttoPile(currentWorkingPlace, A);
                    j = 0;
                    forceSave = false;

                    //2ème cas de figure, on un INSERT
                    // il y A au moins un header supérieur avec même number, valeur différente
                    // il faut alors ajouter au header+1 la valeur contenue AVANT

                    // 2C25
                    /*
if ( exist_NumberA_andLayerB_KaraktinPilesBetweenCandDincluded ( number, i, place+1, karakt_nbTotalPiles-1 ) )
{
                        // il faut alors remettre dans la pile place+1 la dernière valeur avant place
                        int valeur = giveLastValueBeforePileC(number,i,place);
                        pileKarakt B;

                        B.valeur = valeur;
                        B.number = number;
                        B.layer = i;
                        B.nextPile = NULL;

                        karakt_addNewKarakttoPile(pKarakt_Header[place + 1], B);
                            
}
*/
                }
                j = 0;
            }
            else
            {
            }
        }

        // si on arrive ici c'est qu'on a parcouru tous les headers sans trouver de variable sauvée
        // il faut dès lors l'enregistrer : ça doit être le cas pour le tout premier SAVE MEM

        if (forceSave == true)
        {
            pileKarakt A;

            A.valeur = value[i];
            A.number = number;
            A.layer = i;
            A.nextPile = NULL;

            karakt_addNewKarakttoPile(currentWorkingPlace, A);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////
// 2C22 CORONADAYZ
// Econome_ versions
/////////////////////////////////////////////////////////////////////////////////////

void econome_multi_bool_saveToKMem(byte place, int number, int layer, bool value[])
{
    for (int i = 0; i < layer; i++)
    {
        currentWorkingPlace = pKarakt_Header[place];

        bool forceSave = true;

        for (int j = place; j > 0; j--)
        {
            if (exist_numberA_andLayerB_KaraktinPileC(number, i, pKarakt_Header[j - 1]))
            {
                // on trouve dans un layer inférieur une variable identique

                if (giveValue_numberA_andLayerB_KaraktinPileC(number, i, pKarakt_Header[j - 1]) == (int)value[i])
                {
                    // identique : on ne doit pas sauver
                    j = 0;
                    forceSave = false;
                }
                else
                {
                    pileKarakt A;

                    A.valeur = (int)value[i];
                    A.number = number;
                    A.layer = i;
                    A.nextPile = NULL;

                    karakt_addNewKarakttoPile(currentWorkingPlace, A);
                    j = 0;
                    forceSave = false;
                }
                j = 0;
            }
            else
            {
            }
        }

        // si on arrive ici c'est qu'on a parcouru tous les headers sans trouver de variable sauvée
        // il faut dès lors l'enregistrer : ça doit être le cas pour le tout premier SAVE MEM

        if (forceSave == true)
        {
            pileKarakt A;

            A.valeur = (int)value[i];
            A.number = number;
            A.layer = i;
            A.nextPile = NULL;

            karakt_addNewKarakttoPile(currentWorkingPlace, A);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////
// 2C22 CORONADAYZ
// Econome_ versions
/////////////////////////////////////////////////////////////////////////////////////

void econome_int_saveToKMem(byte place, int number, int value)
{
    //  for (int i = 0; i < layer; i++)
    {
        currentWorkingPlace = pKarakt_Header[place];

        bool forceSave = true;

        for (int j = place; j > 0; j--)
        {
            if (exist_numberA_andLayerB_KaraktinPileC(number, 0, pKarakt_Header[j - 1]))
            {
                // on trouve dans un layer inférieur une variable identique

                if (giveValue_numberA_andLayerB_KaraktinPileC(number, 0, pKarakt_Header[j - 1]) == value)
                {
                    // identique : on ne doit pas sauver
                    j = 0;
                    forceSave = false;
                }
                else
                {
                    pileKarakt A;

                    A.valeur = value;
                    A.number = number;
                    A.layer = 0;
                    A.nextPile = NULL;

                    karakt_addNewKarakttoPile(currentWorkingPlace, A);
                    j = 0;
                    forceSave = false;
                }
                j = 0;
            }
            else
            {
            }
        }

        // si on arrive ici c'est qu'on a parcouru tous les headers sans trouver de variable sauvée
        // il faut dès lors l'enregistrer : ça doit être le cas pour le tout premier SAVE MEM

        if (forceSave == true)
        {
            pileKarakt A;

            A.valeur = value;
            A.number = number;
            A.layer = 0;
            A.nextPile = NULL;

            karakt_addNewKarakttoPile(currentWorkingPlace, A);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////
// 2C22 CORONADAYZ
// Econome_ versions
/////////////////////////////////////////////////////////////////////////////////////

void econome_bool_saveToKMem(byte place, int number, bool value, int layer = 0 )
{
    // for (int i = 0; i < layer; i++)
    {
        currentWorkingPlace = pKarakt_Header[place];

        bool forceSave = true;

        for (int j = place; j > 0; j--)
        {
            if (exist_numberA_andLayerB_KaraktinPileC(number, layer , pKarakt_Header[j - 1]))
            {
                // on trouve dans un layer inférieur une variable identique

                if (giveValue_numberA_andLayerB_KaraktinPileC(number, layer , pKarakt_Header[j - 1]) == (int)value)
                {
                    // identique : on ne doit pas sauver
                    j = 0;
                    forceSave = false;
                }
                else
                {
                    pileKarakt A;

                    A.valeur = (int)value;
                    A.number = number;
                    A.layer = layer ;
                    A.nextPile = NULL;

                    karakt_addNewKarakttoPile(currentWorkingPlace, A);
                    j = 0;
                    forceSave = false;
                }
                j = 0;
            }
            else
            {
            }
        }

        // si on arrive ici c'est qu'on a parcouru tous les headers sans trouver de variable sauvée
        // il faut dès lors l'enregistrer : ça doit être le cas pour le tout premier SAVE MEM

        if (forceSave == true)
        {
            pileKarakt A;

            A.valeur = (int)value;
            A.number = number;
            A.layer = layer ;
            A.nextPile = NULL;

            karakt_addNewKarakttoPile(currentWorkingPlace, A);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////
// 2C22 CORONADAYZ
/////////////////////////////////////////////////////////////////////////////////////

void saveKarakt_to_KMem(byte place)
{
    // free_KMem(place); // 2C25 : on fait insertion réelle plutôt qu'écrabouillage

    // pas très safe d'utiliser une variable générale de cette façon, mais plus rapide que de mettre cette valeur en paramètre
    // currentWorkingPlace = pKarakt_Header[0]; // debug

    econome_int_saveToKMem(place, 1, tailleChaineMarkov);
    econome_multi_int_saveToKMem(place, 2, nbLayer, nbDeDelais);
    econome_multi_bool_saveToKMem(place, 3, nbLayer, delayLayer);
    econome_multi_int_saveToKMem(place, 4, nbLayer, nbDeMsecPourDelais);
    econome_multi_int_saveToKMem(place, 5, nbLayer, dropPourDelais);
    econome_multi_int_saveToKMem(place, 6, nbLayer, downPourDelais);
    econome_multi_int_saveToKMem(place, 7, nbLayer, randomPourDelais);
    // econome_multi_int_saveToKMem( place,8,nbLayer,bitMapDelay,"bitmap");  // d'abord transformer bitmapDelay en [] << i
    econome_multi_int_saveToKMem(place, 9, nbLayer, repeaterNb);
    econome_multi_int_saveToKMem(place, 10, nbLayer, seqBitMapDelay);

    econome_multi_bool_saveToKMem(place, 11, nbLayer, dende);
    econome_multi_int_saveToKMem(place, 12, nbLayer, dendeSec);
    econome_multi_int_saveToKMem(place, 13, nbLayer, dendeMSec);
    econome_multi_int_saveToKMem(place, 14, nbLayer, dendeDivide);
    econome_multi_int_saveToKMem(place, 15, nbLayer, dendeDrop);
    econome_multi_int_saveToKMem(place, 16, nbLayer, dendeDown);
    econome_multi_int_saveToKMem(place, 17, nbLayer, dendeLevels);
    econome_multi_int_saveToKMem(place, 18, nbLayer, dendeDecay);
    econome_multi_int_saveToKMem(place, 19, nbLayer, dendeStart);
    econome_multi_int_saveToKMem(place, 20, nbLayer, dendeEnd);
    econome_multi_int_saveToKMem(place, 21, nbLayer, dendeRatio);
    econome_multi_int_saveToKMem(place, 22, nbLayer, dendeStretch);
    econome_multi_int_saveToKMem(place, 23, nbLayer, dendePitch);
    econome_multi_int_saveToKMem(place, 24, nbLayer, dendeFidelity);
    econome_int_saveToKMem(place, 25, overLoadLimit);
    econome_multi_int_saveToKMem(place, 26, nbLayer, dendeRetard);
    econome_multi_int_saveToKMem(place, 27, nbLayer, dendeGamme);
    econome_multi_int_saveToKMem(place, 28, nbLayer, dendeMode);
    econome_multi_int_saveToKMem(place, 29, nbLayer, dendeDegres);
    econome_multi_int_saveToKMem(place, 30, nbLayer, dendeExpand);
    econome_multi_int_saveToKMem(place, 31, nbLayer, dendeArp1);
    econome_multi_int_saveToKMem(place, 32, nbLayer, dendeArp2);
    econome_multi_int_saveToKMem(place, 33, nbLayer, dendeNoteOff);
    econome_multi_int_saveToKMem(place, 34, nbLayer, dendeSubFidelity);
    econome_multi_int_saveToKMem(place, 35, nbLayer, repeaterNbDende);
    // econome_multi_int_saveToKMem( place,36, _bitMapDelay[nbLayer][8] = {1}; // à initialiser .  si vaut 1 delay a lieu, si vaut 0 pas
    // pour au-delà de 8 delay : %8 !
    econome_multi_int_saveToKMem(place, 37, nbLayer, dendeBitMapDelay);

    // 26 mars 2020 - en plein dans les CoronaDayz

    econome_multi_bool_saveToKMem(place, 38, nbLayer, omitEnd);
    econome_multi_int_saveToKMem(place, 39, nbLayer, omitEndValue);

    // DRUNK
    econome_multi_bool_saveToKMem(place, 40, nbLayer, drunk);
    econome_multi_int_saveToKMem(place, 41, nbLayer, drunkMin);
    econome_multi_int_saveToKMem(place, 42, nbLayer, drunkMax);
    econome_multi_int_saveToKMem(place, 43, nbLayer, drunkStep);
    econome_multi_int_saveToKMem(place, 44, nbLayer, currentDrunkValue);

    // NEW Global Markov
    econome_int_saveToKMem(place, 45, tailleChaineMarkov);
    econome_bool_saveToKMem(place, 46, markovNEWChainGlobal);
    econome_int_saveToKMem(place, 47, currentTailleNEWMarkovTable);
    econome_int_saveToKMem(place, 48, markovNEWTailleMax);
    econome_multi_bool_saveToKMem(place, 49, nbLayer, participateNEWMarkov);
    econome_multi_bool_saveToKMem(place, 50, nbLayer, participateLAYERMarkov);
    econome_multi_int_saveToKMem(place, 51, nbLayer, currentSizeLayerMarkov);
    econome_multi_int_saveToKMem(place, 52, nbLayer, markovLayerMax);

    //Barocco
    econome_multi_bool_saveToKMem(place, 53, nbLayer, baroccoLayer);
    econome_multi_int_saveToKMem(place, 54, nbLayer, baroccoChosenSeq);
    econome_multi_int_saveToKMem(place, 55, nbLayer, nbDeMsecPourBarocco);
    econome_multi_int_saveToKMem(place, 56, nbLayer, dropPourBarocco);
    econome_multi_int_saveToKMem(place, 57, nbLayer, randomPourBarocco);
    econome_multi_int_saveToKMem(place, 58, nbLayer, symHBarocco);
    econome_multi_int_saveToKMem(place, 59, nbLayer, symVBarocco);
    econome_multi_int_saveToKMem(place, 60, nbLayer, refletBarocco);
    econome_multi_int_saveToKMem(place, 61, nbLayer, preReflet);
    econome_multi_int_saveToKMem(place, 62, nbLayer, preH);
    econome_multi_int_saveToKMem(place, 63, nbLayer, preV);

    // VARIATIONS
    econome_multi_bool_saveToKMem(place, 64, nbLayer, variationLayer);
    econome_multi_int_saveToKMem(place, 65, nbLayer, percentDisparitionChanceVariation);
    econome_multi_int_saveToKMem(place, 66, nbLayer, minimumNotesVariation);
    econome_multi_int_saveToKMem(place, 67, nbLayer, maximumReduxVariation);
    econome_multi_bool_saveToKMem(place, 68, nbLayer, modeMinimumVariation);
    econome_multi_int_saveToKMem(place, 69, nbLayer, modeVariation);

    econome_multi_bool_saveToKMem(place, 70, nbLayer, pitchMicroSamplerOnChannel);
    econome_multi_bool_saveToKMem(place, 71, nbLayer, pitchMicroSamplerOnLayer);
    econome_multi_int_saveToKMem(place, 72, nbLayer, pitchMicroSampler);

    econome_multi_bool_saveToKMem(place, 73, nbLayer, dynamicSwing);
    econome_multi_int_saveToKMem(place, 74, nbLayer, valueDynamicSwing);

    // DIVERS

    econome_multi_int_saveToKMem(place, 75, nbLayer, fidelityWholeLayer);
    econome_multi_bool_saveToKMem(place, 76, nbLayer, isWholeLayerFidelityMuted);

    // modulo layer
    econome_multi_int_saveToKMem(place, 77, nbLayer, eclipseMode);      //  1 = mute selected    2 = mute all except selected
    econome_int_saveToKMem(place, 78, eclipseModulo);                   //
    econome_int_saveToKMem(place, 79, eclipseK);                        //
    econome_multi_int_saveToKMem(place, 80, nbLayer, eclipseModuloTab); //
    econome_multi_int_saveToKMem(place, 81, nbLayer, eclipseKTab);      //
    econome_bool_saveToKMem(place, 82, modeConstipe);                   //

    // dynramic harmonize
    econome_int_saveToKMem(place, 83, dynamicHarmonizeMode);           //
    econome_multi_bool_saveToKMem(place, 84, nbLayer, harmonizeLayer); //

    econome_multi_int_saveToKMem(place, 85, nbLayer, DureePole1); //
    econome_multi_int_saveToKMem(place, 86, nbLayer, minPole1);
    econome_multi_int_saveToKMem(place, 87, nbLayer, maxPole1);
    econome_multi_int_saveToKMem(place, 88, nbLayer, DureePole2);
    econome_multi_int_saveToKMem(place, 89, nbLayer, minPole2);
    econome_multi_int_saveToKMem(place, 90, nbLayer, maxPole2);
    econome_multi_int_saveToKMem(place, 91, nbLayer, freqPole1);

    econome_bool_saveToKMem(place, 92, turnOnBipolarRandom);

    econome_bool_saveToKMem(place, 93, FunkyVel); //  // DEBUG : pourquoi pas lié à layer ???
    econome_multi_int_saveToKMem(place, 94, nbLayer, FunkyVelValue);

    // autoharmonize
    econome_bool_saveToKMem(place, 95, autoHarmonize);
    econome_multi_int_saveToKMem(place, 96, nbLayer, modeActuelle); //// mémorise le mode actuelle - modif 2C12

    econome_multi_bool_saveToKMem(place, 97, nbLayer, pitchableLayer);
    econome_multi_bool_saveToKMem(place, 98, 16, pitchableChannel);

    // syncop BUG
    econome_bool_saveToKMem(place, 99, KeepSyncopBUG);

    econome_multi_int_saveToKMem(place, 100, nbLayer, poof);
    econome_bool_saveToKMem(place, 101, turnOnPoof);

    econome_int_saveToKMem(place, 102, nSwing);

    econome_multi_int_saveToKMem(place, 103, nbLayer, gammeActuelle);
    econome_multi_int_saveToKMem(place, 104, 12, noteRedirect); //   // contient la matrice de pitch des notes

    // 27 mars 2020 - en plein dans les CoronaDayz

    // KANNON multi layer
    econome_multi_bool_saveToKMem(place, 105, nbLayer, kannonActif);
    econome_multi_int_saveToKMem(place, 106, nbLayer, kannonRevert);
    econome_multi_int_saveToKMem(place, 107, nbLayer, kannonReflet);
    econome_multi_int_saveToKMem(place, 108, nbLayer, kannonPitch);
    econome_multi_int_saveToKMem(place, 109, nbLayer, kannonDelay);
    econome_multi_int_saveToKMem(place, 110, nbLayer, kannonRandom);
    econome_multi_int_saveToKMem(place, 111, nbLayer, kannonStretchRatio);

    // PAN + DERAZER
    econome_multi_bool_saveToKMem(place, 112, 16, panMicroSamplerOnChannel);
    econome_multi_bool_saveToKMem(place, 113, nbLayer, panMicroSamplerOnLayer);
    econome_multi_int_saveToKMem(place, 114, nbLayer, panMicroSampler);

    // GABBERISM
    econome_int_saveToKMem(place, 115, gabberismLevels);
    econome_int_saveToKMem(place, 117, gabberismDivRatio);
    econome_int_saveToKMem(place, 116, gabberismDivisions);
    econome_int_saveToKMem(place, 118, gabberismRepeats);
    econome_int_saveToKMem(place, 119, gabberismRepeatRatio);

    econome_multi_int_saveToKMem(place, 120, nbLayer, fidelityLayer_);

    // GABBERITO 2
    econome_int_saveToKMem(place, 121, gabberito2Interval);
    econome_int_saveToKMem(place, 122, gabberito2K);
    econome_int_saveToKMem(place, 123, gabberito2N);

    econome_int_saveToKMem(place, 124, accentMax);
    econome_int_saveToKMem(place,125, accentMin);

    // Gabberito
    econome_int_saveToKMem(place,126, gabberitoInterval);

    econome_int_saveToKMem(place,127, CHMute);

    // STEPZBUG
    econome_bool_saveToKMem(place,128, keepStepzBUG);

    for (int i = 0; i < nbSequencesMax; i++)
    {
        econome_bool_saveToKMem(place, 129,  sequence[i].actif, i);
    }

    for (int i = 0; i < NbCCModMax; i++)
    {
        econome_bool_saveToKMem(place, 130,  CCModTableau[i].actif, i);
    }
    for (int i = 0; i < NbEvenementMax; i++)
    {
        econome_bool_saveToKMem(place, 131,  Evenement[i].actif, i);
    }
    for (int i = 0; i < nbLFOMax; i++)
    {
        econome_bool_saveToKMem(place, 132,  LFO[i].actif, i );
    }
}

/////////////////////////////////////////////////////////////////////////////////////
// 2C22 CORONADAYZ
/////////////////////////////////////////////////////////////////////////////////////

unsigned int karakt_mesureTaillePileComplete(pileKarakt *pilePtr)
{
    unsigned int taillePile = 0;
    pileKarakt *thisPtr = pilePtr;
    // buffer_and_HARD_print("ZIP"); ok, il arrive jusqu'ici

    while ((*thisPtr).nextPile != NULL)
    {
        thisPtr = (*thisPtr).nextPile;

        taillePile++;
    }
    return taillePile;
    // il ne compte en fait pas la dernière note de la pile,
    // ce qui fait un déficit de 1
    // cela tombe bien, car la première note, le pointeur initial ne doit pas être compté
    // il ne faut néanmoins pas oublier cela
}

/////////////////////////////////////////////////////////////////////////////////////
// 2C22 CORONADAYZ
/////////////////////////////////////////////////////////////////////////////////////

void verboseKMem(byte place)
{
    // pas très safe d'utiliser une variable générale de cette façon, mais plus rapide que de mettre cette valeur en paramètre
    currentWorkingPlace = pKarakt_Header[place]; // debug

    Serial.println("*");

    while ((*currentWorkingPlace).nextPile != NULL)
    {
        int a = (*currentWorkingPlace).number;
        int b = (*currentWorkingPlace).layer;
        int c = (*currentWorkingPlace).valeur;

        Serial.print("[");
        Serial.print(String(a));
        Serial.print("](");
        Serial.print(String(b));
        Serial.print(")=");
        Serial.println(String(c));
        Serial.print(" ");

        currentWorkingPlace = (*currentWorkingPlace).nextPile;
    }

    Serial.println("\n------------------------");
}

// plein de trucs mis en commentaires effacés dans 2C25 ; voir 2C22 à 2C24 pour les retrouver

void loadKarakt_from_KMem(byte place)
{
    // pas très safe d'utiliser une variable générale de cette façon, mais plus rapide que de mettre cette valeur en paramètre
    currentWorkingPlace = pKarakt_Header[0]; // debug

    // traitement de la ligne et de copie

    // parcourir la mémoire de header 0 à j
    // parcourir la chaîne
    // charger value, number, layber
    // dispatch dans les variables.

    for (int j = 0; j <= place; j++)
    {
        currentWorkingPlace = pKarakt_Header[j]; // debug

        Serial.print("load");
        Serial.println(String(j));

        bool keepOn = true;

        do
        {
            karaktNumber = (*currentWorkingPlace).number;
            karaktLayer = (*currentWorkingPlace).layer;
            karaktValue = (*currentWorkingPlace).valeur;

           // Serial.print("H");
           // Serial.print(String(j));
           // Serial.print("[");
           // Serial.print(String(karaktNumber));
           // Serial.print("](");
           // Serial.print(String(karaktLayer));
           // Serial.print(")=");
           // Serial.println(String(karaktValue));
           // Serial.print(" ");

            // Ce qui suit est recopié tel quel de la fonction qui charge KARAKT FROM FILE
            // on pourrait communautariser

            int_loadFromKarakt(1, &tailleChaineMarkov);
            multi_int_loadFromKarakt(2, &nbDeDelais[0]);
            multi_bool_loadFromKarakt(3, &delayLayer[0]);
            multi_int_loadFromKarakt(4, &nbDeMsecPourDelais[0]);
            multi_int_loadFromKarakt(5, &dropPourDelais[0]);
            multi_int_loadFromKarakt(6, &downPourDelais[0]);
            multi_int_loadFromKarakt(7, &randomPourDelais[0]);
            // multi_int_printToKarakt(8,nbLayer,bitMapDelay,"bitmap");  // d'abord transformer bitmapDelay en [] << i
            multi_int_loadFromKarakt(9, &repeaterNb[0]);
            multi_int_loadFromKarakt(10, &seqBitMapDelay[0]);

            multi_bool_loadFromKarakt(11, &dende[0]);
            multi_int_loadFromKarakt(12, &dendeSec[0]);
            multi_int_loadFromKarakt(13, &dendeMSec[0]);
            multi_int_loadFromKarakt(14, &dendeDivide[0]);
            multi_int_loadFromKarakt(15, &dendeDrop[0]); // DEBUG il faudra modifier, pour avoir 0 - 100 - 200
            multi_int_loadFromKarakt(16, &dendeDown[0]); // DEBUG il faudra modifier, pour avoir 0 - 100 - 200
            multi_int_loadFromKarakt(17, &dendeLevels[0]);
            multi_int_loadFromKarakt(18, &dendeDecay[0]);
            multi_int_loadFromKarakt(19, &dendeStart[0]);
            multi_int_loadFromKarakt(20, &dendeEnd[0]);
            multi_int_loadFromKarakt(21, &dendeRatio[0]);
            multi_int_loadFromKarakt(22, &dendeStretch[0]);
            multi_int_loadFromKarakt(23, &dendePitch[0]);
            multi_int_loadFromKarakt(24, &dendeFidelity[0]);
            int_loadFromKarakt(25, &overLoadLimit);
            multi_int_loadFromKarakt(26, &dendeRetard[0]);
            multi_int_loadFromKarakt(27, &dendeGamme[0]);
            multi_int_loadFromKarakt(28, &dendeMode[0]);
            multi_int_loadFromKarakt(29, &dendeDegres[0]);
            multi_int_loadFromKarakt(30, &dendeExpand[0]);
            multi_int_loadFromKarakt(31, &dendeArp1[0]);
            multi_int_loadFromKarakt(32, &dendeArp2[0]);
            multi_int_loadFromKarakt(33, &dendeNoteOff[0]);     // sera à préciser
            multi_int_loadFromKarakt(34, &dendeSubFidelity[0]); // TODO prévoir fc pour modifier
            multi_int_loadFromKarakt(35, &repeaterNbDende[0]);
            // multi_int_loadFromKarakt(36, &_bitMapDelay[nbLayer][8] = {1}; // à initialiser .  si vaut 1 delay a lieu, si vaut 0 pas
            // pour au-delà de 8 delay : %8 !
            multi_int_loadFromKarakt(37, &dendeBitMapDelay[0]);

            multi_bool_loadFromKarakt(38, &omitEnd[0]);
            multi_int_loadFromKarakt(39, &omitEndValue[0]);

            // DRUNK
            multi_bool_loadFromKarakt(40, &drunk[0]);
            multi_int_loadFromKarakt(41, &drunkMin[0]);
            multi_int_loadFromKarakt(42, &drunkMax[0]);
            multi_int_loadFromKarakt(43, &drunkStep[0]);
            multi_int_loadFromKarakt(44, &currentDrunkValue[0]); // PERTINENCE ?

            // NEW Global Markov
            int_loadFromKarakt(45, &tailleChaineMarkov);
            bool_loadFromKarakt(46, &markovNEWChainGlobal);
            int_loadFromKarakt(47, &currentTailleNEWMarkovTable);
            int_loadFromKarakt(48, &markovNEWTailleMax);
            multi_bool_loadFromKarakt(49, &participateNEWMarkov[0]);
            multi_bool_loadFromKarakt(50, &participateLAYERMarkov[0]);
            multi_int_loadFromKarakt(51, &currentSizeLayerMarkov[0]);
            multi_int_loadFromKarakt(52, &markovLayerMax[0]);

            //Barocco
            multi_bool_loadFromKarakt(53, &baroccoLayer[0]);
            multi_int_loadFromKarakt(54, &baroccoChosenSeq[0]);
            multi_int_loadFromKarakt(55, &nbDeMsecPourBarocco[0]);
            multi_int_loadFromKarakt(56, &dropPourBarocco[0]);
            multi_int_loadFromKarakt(57, &randomPourBarocco[0]);
            multi_int_loadFromKarakt(58, &symHBarocco[0]);
            multi_int_loadFromKarakt(59, &symVBarocco[0]);
            multi_int_loadFromKarakt(60, &refletBarocco[0]);
            multi_int_loadFromKarakt(61, &preReflet[0]);
            multi_int_loadFromKarakt(62, &preH[0]);
            multi_int_loadFromKarakt(63, &preV[0]);

            // VARIATIONS
            multi_bool_loadFromKarakt(64, &variationLayer[0]);
            multi_int_loadFromKarakt(65, &percentDisparitionChanceVariation[0]);
            multi_int_loadFromKarakt(66, &minimumNotesVariation[0]);
            multi_int_loadFromKarakt(67, &maximumReduxVariation[0]);
            multi_bool_loadFromKarakt(68, &modeMinimumVariation[0]);
            multi_int_loadFromKarakt(69, &modeVariation[0]);

            multi_bool_loadFromKarakt(70, &pitchMicroSamplerOnChannel[0]);
            multi_bool_loadFromKarakt(71, &pitchMicroSamplerOnLayer[0]);
            multi_int_loadFromKarakt(72, &pitchMicroSampler[0]);

            multi_bool_loadFromKarakt(73, &dynamicSwing[0]);
            multi_int_loadFromKarakt(74, &valueDynamicSwing[0]);

            multi_int_loadFromKarakt(75, &fidelityWholeLayer[0]);
            multi_bool_loadFromKarakt(76, &isWholeLayerFidelityMuted[0]);

            // modulo layer
            multi_int_loadFromKarakt(77, &eclipseMode[0]);      //  1 = mute selected    2 = mute all except selected
            int_loadFromKarakt(78, &eclipseModulo);             //
            int_loadFromKarakt(79, &eclipseK);                  //
            multi_int_loadFromKarakt(80, &eclipseModuloTab[0]); //
            multi_int_loadFromKarakt(81, &eclipseKTab[0]);      //
            bool_loadFromKarakt(82, &modeConstipe);             //

            // dynramic harmonize
            int_loadFromKarakt(83, &dynamicHarmonizeMode); //
            multi_bool_loadFromKarakt(84, &harmonizeLayer[0]);

            multi_int_loadFromKarakt(85, &DureePole1[0]);
            multi_int_loadFromKarakt(86, &minPole1[0]);
            multi_int_loadFromKarakt(87, &maxPole1[0]);
            multi_int_loadFromKarakt(88, &DureePole2[0]);
            multi_int_loadFromKarakt(89, &minPole2[0]);
            multi_int_loadFromKarakt(90, &maxPole2[0]);
            multi_int_loadFromKarakt(91, &freqPole1[0]);

            bool_loadFromKarakt(92, &turnOnBipolarRandom);

            bool_loadFromKarakt(93, &FunkyVel); // DEBUG : pourquoi pas lié à layer ???
            multi_int_loadFromKarakt(94, &FunkyVelValue[0]);

            // autoharmonize
            bool_loadFromKarakt(95, &autoHarmonize);
            multi_int_loadFromKarakt(96, &modeActuelle[0]); //// mémorise le mode actuelle - modif 2C12

            multi_bool_loadFromKarakt(97, &pitchableLayer[0]);
            multi_bool_loadFromKarakt(98, &pitchableChannel[0]);

            // syncop BUG
            bool_loadFromKarakt(99, &KeepSyncopBUG);

            multi_int_loadFromKarakt(100, &poof[0]);
            bool_loadFromKarakt(101, &turnOnPoof); // DEBUG : pourquoi pas Layered ????

            int_loadFromKarakt(102, &nSwing); // par défaut

            multi_int_loadFromKarakt(103, &gammeActuelle[0]);
            multi_int_loadFromKarakt(104, &noteRedirect[0]); //   // contient la matrice de pitch des notes

            // KANNON multi layer
            multi_bool_loadFromKarakt(105, &kannonActif[0]);
            multi_int_loadFromKarakt(106, &kannonRevert[0]);
            multi_int_loadFromKarakt(107, &kannonReflet[0]);
            multi_int_loadFromKarakt(108, &kannonPitch[0]);
            multi_int_loadFromKarakt(109, &kannonDelay[0]);
            multi_int_loadFromKarakt(110, &kannonRandom[0]);
            multi_int_loadFromKarakt(111, &kannonStretchRatio[0]);

            // PAN + DERAZER
            multi_bool_loadFromKarakt(112, &panMicroSamplerOnChannel[0]);
            multi_bool_loadFromKarakt(113, &panMicroSamplerOnLayer[0]);
            multi_int_loadFromKarakt(114, &panMicroSampler[0]);

            // GABBERISM
            int_loadFromKarakt(115, &gabberismLevels);
            int_loadFromKarakt(116, &gabberismDivisions);
            int_loadFromKarakt(117, &gabberismDivRatio);
            int_loadFromKarakt(118, &gabberismRepeats);
            int_loadFromKarakt(119, &gabberismRepeatRatio);

            multi_int_loadFromKarakt(120, &fidelityLayer_[0]);

            // GABBERITO 2
            int_loadFromKarakt(121, &gabberito2Interval);
            int_loadFromKarakt(122, &gabberito2K);
            int_loadFromKarakt(123, &gabberito2N);

            int_loadFromKarakt(124, &accentMax);
            int_loadFromKarakt(125, &accentMin);

            // Gabberito
            int_loadFromKarakt(126, &gabberitoInterval);

            int_loadFromKarakt(127, &CHMute);

            // STEPZBUG
            bool_loadFromKarakt(128, &keepStepzBUG);

            for (int i = 0; i < nbSequencesMax; i++)
            {
                single_bool_loadFromKarakt(129, i, &sequence[i].actif);
            }

            for (int i = 0; i < NbCCModMax; i++)
            {
                single_bool_loadFromKarakt(130, i, &CCModTableau[i].actif);
            }
            for (int i = 0; i < NbEvenementMax; i++)
            {
                single_bool_loadFromKarakt(131, i, &Evenement[i].actif);
            }
            for (int i = 0; i < nbLFOMax; i++)
            {
                single_bool_loadFromKarakt(132, i, &LFO[i].actif);
            }

            if ((*currentWorkingPlace).nextPile == NULL)
            {
                keepOn = false;
            }
            else
            {
                currentWorkingPlace = (*currentWorkingPlace).nextPile;
            }

        } while (keepOn == true);

        Serial.println();
    }
}

void free_KMem(byte place)
// efface un KMem en préservant ses données pour les autres KMem !
// va en fait plutôt libérer un KMem
{

    if (place > 63)
    {
        return;
    }

    if (pKarakt_Header[place] == NULL) // Ne devrait normalement jamais arriver, mais on sait jamais
    {
        return;
    }

    currentWorkingPlace = pKarakt_Header[place]; // debug

    // parcours de la pile à partir du header
    // lecture du number, layer, valeur
    // recherche si il y a ce number,layer dans le header+1 ( sauf si > nombreHeader - 1 )
    // si OUI : on ne fait rien
    // si NON : on ajoute ce number, layer au header+1

    //  Serial.print("load");
    //  Serial.println(String(j));

    // PARTIE sauvegarde des infos contenues dans cette pile vers la pile suivante
    // indispensable, car vu la modalité économe, on ne sauve que ce qui est différent de la pile précédente

    bool keepOn = true;
    int j = place;
    currentWorkingPlace = pKarakt_Header[j]; // debug

    do
    {
        karaktNumber = (*currentWorkingPlace).number;
        karaktLayer = (*currentWorkingPlace).layer;
        karaktValue = (*currentWorkingPlace).valeur;

        if (exist_numberA_andLayerB_KaraktinPileC(karaktNumber, karaktLayer, pKarakt_Header[j + 1]) == false)
        {
            karakt_addNewKarakttoPile(pKarakt_Header[j + 1], (*currentWorkingPlace));
        }

        if ((*currentWorkingPlace).nextPile == NULL)
        {
            keepOn = false;
        }
        else
        {
            currentWorkingPlace = (*currentWorkingPlace).nextPile;
        }
    } while (keepOn == true);

    // PARTIE effacement

    if (pKarakt_Header[place] == NULL) // Ne devrait normalement jamais arriver, mais on sait jamais
    {
        return;
    }

    bool goON(false);
    pileKarakt *pilePtr = pKarakt_Header[j];
    pileKarakt *precedentPtr = pKarakt_Header[j];
    pileKarakt *firstPtr = pKarakt_Header[j];

    do
    {

        if (pilePtr != firstPtr) // on efface une notePile qui n'est pas la première
        {
            (*precedentPtr).nextPile = (*pilePtr).nextPile;

            delete pilePtr;

            pilePtr = precedentPtr;

            if ((*pilePtr).nextPile != NULL)
            {
                precedentPtr = pilePtr;
                pilePtr = (*pilePtr).nextPile;
                goON = true;
            }
            else
            {
                goON = false;
            }
        }

        else // on traite une notePile qui est le première
        {
            if ((*pilePtr).nextPile != 0)
            { // et qui a des successeurs
                pilePtr = (*pilePtr).nextPile;

                goON = true;
            }
            else
            { // et qui n'a pas de note après = la dernière
                goON = false;
            }
        }

    } while (goON);
}

void erase_KMem(byte place)
// efface brutalement un KMem, sans se soucier de l'impact sur autres KMem
{
    if (place > 63)
    {
        return;
    }

    if (pKarakt_Header[place] == NULL) // Ne devrait normalement jamais arriver, mais on sait jamais
    {
        return;
    }

    // currentWorkingPlace = pKarakt_Header[place]; // debug

    // bool keepOn = true;
    int j = place;
    // currentWorkingPlace = pKarakt_Header[j]; // debug

    // PARTIE effacement

    if (pKarakt_Header[place] == NULL) // Ne devrait normalement jamais arriver, mais on sait jamais
    {
        return;
    }

    bool goON(false);
    pileKarakt *pilePtr = pKarakt_Header[j];
    pileKarakt *precedentPtr = pKarakt_Header[j];
    pileKarakt *firstPtr = pKarakt_Header[j];

    do
    {
        if (pilePtr != firstPtr) // on efface une notePile qui n'est pas la première
        {
            (*precedentPtr).nextPile = (*pilePtr).nextPile;

            delete pilePtr;

            pilePtr = precedentPtr;

            if ((*pilePtr).nextPile != NULL)
            {
                precedentPtr = pilePtr;
                pilePtr = (*pilePtr).nextPile;
                goON = true;
            }
            else
            {
                goON = false;
            }
        }

        else // on traite une notePile qui est le première
        {
            if ((*pilePtr).nextPile != 0)
            { // et qui a des successeurs
                pilePtr = (*pilePtr).nextPile;

                goON = true;
            }
            else
            { // et qui n'a pas de note après = la dernière
                goON = false;
            }
        }

    } while (goON);
}

bool isThereFreeKMem(void)
{
    // 2C26 basé sur 536: // first free mem
    int firstfree = 100;

    for (int i = 0; i < karakt_nbTotalPiles; i++)
    {
        int memoireVisee = i;
        unsigned int taille = karakt_mesureTaillePileComplete(pKarakt_Header[memoireVisee]);

        if (taille == 0)
        {
            if (firstfree == 100)
            {
                firstfree = i;
                return true;
            }
        }
    }

    return false;
}

int giveFirstFreeKMem(void)
{

    // 2C26 basé sur 536: // first free mem
    int firstfree = 100;

    for (int i = 0; i < karakt_nbTotalPiles; i++)
    {
        int memoireVisee = i;
        unsigned int taille = karakt_mesureTaillePileComplete(pKarakt_Header[memoireVisee]);

        if (taille == 0)
        {
            if (firstfree == 100)
            {
                return i;
            }
        }
    }

    return 100; /// clair que hors de limite mais dangereux car va écraser qqch si on le croit ! 
}
 