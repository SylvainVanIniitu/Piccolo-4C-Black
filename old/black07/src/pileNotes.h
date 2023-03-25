#include <Arduino.h>

// const long valeurPremiereNote = 30000 ;
// const int dureeLayer = 20000 ;
// const int nbNotes = 20;

byte val(0);
// objectif :
// à partir des tableaux melodyMatrix et melodyMatrixTime
// créer 2 piles de notes, une pour celles qui doivent être jouées et une pour celles qui doivent être éteintes
// comme la nouvelle fc PlayNote peut envoyer des notes à partir d'un exemplaire dans Matrix vers plusieurs CH et Pitch, il faut 2 listes distinctes
// il faudra un contrôle pour les notes qui se superposent (polyphonie ou 2 layers qui envoient vers même note ) : il ne faut pas qu'une note A se finissant avec une note B identique éteigne celle-ci.

// en quoi consistent les infos ?
// pileNoteON : channel, note, vélocité, moment où elle doit être jouée ( éventuellement éteinte ), i,j initiaux ( ! sort )
// pileNoteOFF : channel, note, moment d'éteindre
// la pileNoteOFF est alimentée au moment où une note est jouée !
// la pile NoteON est alimentée quand une note est jouée : il faut regarder quand aura lieu la suivante.

// pour Piccoloe on pourrait déjà commencer avec une PileOFF simplement !
// en effet le démarrage des notes fonctionne bien, le seul soucis c'est d'éteindre les multiples notes créées par la modulation

// NOTE****
// étape suivante :
// que faire quand on arrive à la dernière note ?
//

bool DEBUG = false;

// pileNote laPile;
// pileNote *pointeurInitial;

const byte nbLayersMemory = 64;
const byte nbUndoLevels = 4;
const byte nbTotalPiles = 4 + 64 + 4 * 4 + 10 ;
const byte nbTotalKMem = 64 ;

const int pLayerStart = 0;
const int pMemStart = 4;
const int pUndoStart = 4 + 64;
// const int pFluxStart = 4 + 64 + 16;
const int pFluxStart = 4 ; // au début, idem à mem ! 

// ( nbLayer + nbLayersMemory + nbUndoLevels * nbLayer ) ;

pileNote pileLayer[nbTotalPiles];
pileNote *pInitial[nbTotalPiles];

pileNote markov[5];
pileNote *pMarkov[5];

// 2C21 Batch

pileBatch batch;
pileBatch *pBatch;

// 2C20
pileNote variations;
pileNote *pVariations;

// 2C26
pileNote variationsLapidz;
pileNote *pVariationsLapidz;

pileNote variationsDialyse;
pileNote *pVariationsDialyse;

// 2C28 
pileNote softPassRedirect ;
pileNote *pSoftPassRedirect ;

pileNote sustainBlock ;
pileNote *pSustainBlock ;

pileNote proxyBlock;
pileNote *pProxyBlock;

// pileNote pileMemory[nbLayersMemory];
// pileNote *pInitialMemory[nbLayersMemory];

// donc 0 1 2 3 sont les layers actifs
// 4..67 seront les mémoires
// 68 69 70 71 les UNDO level 1
// 72 73 74 75 les UNDO level 2
// 76 77 78 79 les UNDO level 3
// 80 81 82 83 les UNDO level 4

// faudra mesure l'évolution de la mémoire !

// PROTOTYPE de fc se trouvant dans fonctions.h

void PlaynoteOn_(byte cmd, byte pitch, byte velocity);
void pileNote_fixStartEndNote(pileNote &celleCi, long potVal2);
int giveBipolarRandomForLayer(byte i);
int limiter(int num, int bas, int haut);
void Transformation(int TTT, int BBB);

int freeRam()
{
    extern int __heap_start, *__brkval;
    int v;
    return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}

uint32_t FreeMem()
{ // for Teensy 3.0 by BlackKetter on pjrc.com
    uint32_t stackTop;
    uint32_t heapTop;

    // current position of the stack.
    stackTop = (uint32_t)&stackTop;

    // current position of heap.
    void *hTop = malloc(1);
    heapTop = (uint32_t)hTop;
    free(hTop);

    // The difference is (approximately) the free, available ram.
    return stackTop - heapTop;
}

//
// void affichePileComplete ( pileNote*  pilePtr ) {
//
//     bool goON (false) ;
//
//     if ( pilePtr == 0 ) {
//         // Serial.print("PLUS DE NOTE") ;
//     } else {
//
//         do {
//             Serial.print("nb : ");
//             // Serial.print ((*pilePtr).numeroPile) ;
//             Serial.print("\t\t Note : ");
//             Serial.print((*pilePtr).note, HEX) ;
//             Serial.print("\t vel : ");
//             Serial.print((*pilePtr).velocity, HEX) ;
//             Serial.print("\t moment : ");
//             // Serial.print(  (*pilePtr).startNote );
//
//             // if ( millis() > (*pilePtr).startNote  ) {
//             //     Serial.print("\t*") ;
//             // }
//
//             Serial.println();
//
//             if ((*pilePtr).nextPile != 0 ) {
//                 pilePtr = (*pilePtr).nextPile ;
//                 goON = true ;
//             } else {
//                 goON = false ;
//             }
//         }
//
//         while ( goON );
//     }
// }

pileNote *giveDernierPtrPileComplete(pileNote *pilePtr)
{
    pileNote *thisPtr = pilePtr;
    while ((*thisPtr).nextPile != 0)
    {
        thisPtr = (*thisPtr).nextPile;
    }
    return thisPtr;
}

/////////////////////////////////////////////////////////////////////////////////////
// COURONNE 2C21  fait sur le modèle de PileNote
/////////////////////////////////////////////////////////////////////////////////////

pileBatch *giveDernierPtrPileBatchComplete(pileBatch *pilePtr)
{
    pileBatch *thisPtr = pilePtr;
    while ((*thisPtr).nextPile != 0)
    {
        thisPtr = (*thisPtr).nextPile;
    }
    return thisPtr;
}

unsigned int mesureTaillePileComplete(pileNote *pilePtr)
{
    unsigned int taillePile = 0;
    pileNote *thisPtr = pilePtr;
    while ((*thisPtr).nextPile != 0)
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

pileNote *treatPileCompleteMSEC(pileNote *firstPtr)
{
    // supprime les anciennes valeurs de la pile et renvoie le nouveau premier pointeur
    // in fine, va faire le noteStop pour ces notes !

    if (firstPtr == 0)
    {
        return 0;
    }

    bool goON(false);

    pileNote *pilePtr = firstPtr;

    pileNote *precedentPtr = firstPtr;

    pileNote *newFirstPtr = firstPtr;

    do
    {

        if ((*pilePtr).note == 0 && (*pilePtr).type != 0x50 && pilePtr != firstPtr) 
        // on efface une note vide qui n'est pas première
        {
            (*precedentPtr).nextPile = (*pilePtr).nextPile;

            delete pilePtr;

            pilePtr = precedentPtr;

            if ((*pilePtr).nextPile != 0)
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

// BLACK06 pour tenter d'effacer Accent 0x50, dont la valeur vaut Zéro 
        if ((*pilePtr).note == 0 && (*pilePtr).type == 0x50 && (*pilePtr).killnote == true && pilePtr != firstPtr)
        // on efface une note vide, ACCENT en fait, qui n'est pas première et qui doit être éliminée
        {
            (*precedentPtr).nextPile = (*pilePtr).nextPile;

            delete pilePtr;

            pilePtr = precedentPtr;

            if ((*pilePtr).nextPile != 0)
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

        if ((*pilePtr).killnote)
        {
            if (pilePtr != firstPtr) // on efface une notePile qui n'est pas la première
            {
                (*precedentPtr).nextPile = (*pilePtr).nextPile;

                delete pilePtr;

                pilePtr = precedentPtr;

                if ((*pilePtr).nextPile != 0)
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
            else // on efface une notePile qui est le première
            {
                if ((*pilePtr).nextPile != 0)
                { // et qui a des successeurs
                    precedentPtr = (*pilePtr).nextPile;
                    newFirstPtr = (*pilePtr).nextPile;
                    delete pilePtr;

                    pilePtr = precedentPtr;
                    firstPtr = pilePtr;
                    goON = true;
                }
                else
                { // et qui n'a pas de note après = la dernière
                    goON = false;
                    (*pilePtr).channel = 0;
                    (*pilePtr).note = 0; // une note vide
                    (*pilePtr).velocity = 0;
                    (*pilePtr).startNote = 0; //
                    // (*pilePtr).repeatsEffectues = 0 ; // on dit que c'est une note créée par event, si venait de matrix on devrait ici cocer la localisation dans la matrice
                    (*pilePtr).nextPile = 0; // pointe nulle part - y a pas de case 0 dans la mémoire
                    (*pilePtr).killnote = false;
                    return pilePtr; // cas de la dernière note.
                }
            }
        }

        else
        {
            if ((*pilePtr).nextPile != 0)
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

    } while (goON);

    return newFirstPtr;
}

void addNewNotetoPileMSEC(pileNote *cettePileADD,
                          const byte &channelADD, const byte &noteADD, const byte &velocityADD, unsigned long msec,
                          byte type_note = 0x90)
{

    pileNote *pilePtr;

    pilePtr = giveDernierPtrPileComplete(cettePileADD);

    (*pilePtr).nextPile = new pileNote;
    // int a = (*pilePtr).numeroPile ;

    pilePtr = (*pilePtr).nextPile;

    (*pilePtr).channel = channelADD;
    (*pilePtr).note = noteADD; // C60 sur channel 2
    (*pilePtr).velocity = velocityADD;
    (*pilePtr).startNote = msec;
    // (*pilePtr).repeatsEffectues = repeats ;
    // (*pilePtr).nbF8avantJeu = nbF8 ;

    (*pilePtr).killnote = false;
    (*pilePtr).type = type_note;

    (*pilePtr).nextPile = 0; // ne pointe vers rien car est la dernière
}

pileNote giveNthNoteofPile(pileNote *pilePtr, int N)
{
    pileNote *thisPtr = pilePtr;
    pileNote laNote;
    // int compteur = 0;

    for (int i = 0; i < N; i++)
    {
        if ((*thisPtr).nextPile != 0)
        {
            thisPtr = (*thisPtr).nextPile;
        }
    }

    laNote.channel = (*thisPtr).channel;
    laNote.killnote = (*thisPtr).killnote;
    laNote.layer = (*thisPtr).layer;
    laNote.startNote = (*thisPtr).startNote;
    laNote.endNote = (*thisPtr).endNote;
    laNote.thereIsAnEnd = (*thisPtr).thereIsAnEnd;
    laNote.nextPile = (*thisPtr).nextPile;
    laNote.note = (*thisPtr).note;
    laNote.type = (*thisPtr).type;
    laNote.velocity = (*thisPtr).velocity;

    return laNote;
}

void deleteNoteFromPile(pileNote *firstPtr, int N)
// efface la Nème note d'une pile
// attention, ne peut effacer la note 0, qui est le pointeur initial
// la première note est donc 1, ce qui pose problème pour toutes les fc développées jusqu'à présent, qui commençaient à ZERO
// on va donc bypasser cela ... avec une fonction qui sera idem mais appelée START_AT_ZERO
{

    if (firstPtr == 0)
    {
        return;
    }

    if (N <= 0)
    {
        return;
    }

    int taillePile;
    taillePile = mesureTaillePileComplete(firstPtr);

    if (N > taillePile)
    {
        return;
    }

    pileNote *pilePtr = firstPtr;
    pileNote *precedentPtr = firstPtr;

    for (int i = 0; i < N; i++)
    {
        if ((*pilePtr).nextPile != 0)
        {
            precedentPtr = pilePtr;
            pilePtr = (*pilePtr).nextPile;
        }
    }

    // on va travailler sur *thisPtr, qui est la note à effacer

    if (pilePtr != firstPtr) // on efface une notePile qui n'est pas la première
    {
        (*precedentPtr).nextPile = (*pilePtr).nextPile;

        delete pilePtr;
    }

    // on ne touche pas à la première note de la pile..
}

void SetNoteInPile(pileNote *firstPtr, int N, pileNote laNote)
{
    if (firstPtr == 0)
    {
        return;
    }

    if (N <= 0)
    {
        return;
    }

    int taillePile;
    taillePile = mesureTaillePileComplete(firstPtr);

    if (N > taillePile)
    {
        return;
    }

    pileNote *pilePtr = firstPtr;

    for (int i = 0; i < N; i++)
    {
        if ((*pilePtr).nextPile != 0)
        {
            pilePtr = (*pilePtr).nextPile;
        }
    }

    (*pilePtr).channel = laNote.channel;
    (*pilePtr).killnote = laNote.killnote;
    (*pilePtr).layer = laNote.layer;
    (*pilePtr).startNote = laNote.startNote;
    (*pilePtr).nextPile = laNote.nextPile;
    (*pilePtr).note = laNote.note;
    (*pilePtr).type = laNote.type;
    (*pilePtr).velocity = laNote.velocity;
}

pileNote emptyNote; // TODO il faut la définir ( note = 0, vel = 0 )

pileNote getNoteFromPile(pileNote *pilePtr, int N)
{
    if (pilePtr == 0)
    {
        return emptyNote;
    }

    if (N <= 0)
    {
        return emptyNote;
    }

    int taillePile;
    taillePile = mesureTaillePileComplete(pilePtr);

    if (N > taillePile)
    {
        return emptyNote;
    }

    pileNote *thisPtr = pilePtr;
    pileNote laNote;
    // int compteur = 0;

    for (int i = 0; i < N; i++)
    {
        if ((*thisPtr).nextPile != 0)
        {
            thisPtr = (*thisPtr).nextPile;
        }
    }

    laNote.channel = (*thisPtr).channel;
    laNote.killnote = (*thisPtr).killnote;
    laNote.layer = (*thisPtr).layer;
    laNote.startNote = (*thisPtr).startNote;
    laNote.nextPile = (*thisPtr).nextPile;
    laNote.note = (*thisPtr).note;
    laNote.type = (*thisPtr).type;
    laNote.velocity = (*thisPtr).velocity;

    return laNote;
}

// ZERO DEFINITIONS
// celles-ci respectent la nomenclature d'un Array
// Zero sera ainsi la prmemière note
// UN sera la dernière
// les boucles doivent aller de 0 à < TaillePile

void putNoteBinNoteA(pileNote &A, pileNote &B)
{
    A.channel = B.channel;
    A.note = B.note;
    A.endNote = B.endNote; // bien réfléchir à cec i !
    A.layer = B.layer;
    A.startNote = B.startNote;
    A.type = B.type;
    A.velocity = B.velocity;
    A.timeToPlayON = B.timeToPlayON;
    //  A.timeToPlayOFF = B.timeToPlayOFF;
    A.MMnoteReallyPlayed = B.MMnoteReallyPlayed;
    A.thereIsAnEnd = B.thereIsAnEnd;
    A.killnote = B.killnote;
    A.nextPile = B.nextPile; // ne pointe vers rien car est la dernière
}

void ZEROaddNewNotetoPile(pileNote *cettePileADD,
                          pileNote addedNote)
{

    pileNote *pilePtr;

    pilePtr = giveDernierPtrPileComplete(cettePileADD);

    (*pilePtr).nextPile = new pileNote;
    pilePtr = (*pilePtr).nextPile;

    putNoteBinNoteA((*pilePtr), addedNote);

    // SAUF
    (*pilePtr).killnote = false;
    (*pilePtr).nextPile = 0; // ne pointe vers rien car est la dernière
}

pileNote ZEROgiveNthNoteofPile(pileNote *pilePtr, int N)
{
    pileNote *thisPtr = pilePtr;
    pileNote laNote;
    // int compteur = 0;

    N++; // ainsi ZERO sera la première réelle note, pas le pointeur initial

    for (int i = 0; i < N; i++)
    {
        if ((*thisPtr).nextPile != 0)
        {
            thisPtr = (*thisPtr).nextPile;
        }
    }

    putNoteBinNoteA(laNote, (*thisPtr));

    // TODO DEBUG ! attention à bien remplir tous les champs !
    // il faut créer une surchage d'opérateur pour mener cela à bien !

    return laNote;
}

void ZEROdeleteNoteFromPile(pileNote *firstPtr, int N)
// efface la Nème note d'une pile
// attention, ne peut effacer la note 0, qui est le pointeur initial
// la première note est donc 1, ce qui pose problème pour toutes les fc développées jusqu'à présent, qui commençaient à ZERO
// on va donc bypasser cela ... avec une fonction qui sera idem mais appelée START_AT_ZERO
{

    N++;

    if (firstPtr == 0)
    {
        return;
    }

    if (N <= 0)
    {
        return;
    }

    int taillePile;
    taillePile = mesureTaillePileComplete(firstPtr);

    if (N > taillePile)
    {
        return;
    }

    pileNote *pilePtr = firstPtr;
    pileNote *precedentPtr = firstPtr;

    for (int i = 0; i < N; i++)
    {
        if ((*pilePtr).nextPile != 0)
        {
            precedentPtr = pilePtr;
            pilePtr = (*pilePtr).nextPile;
        }
    }

    // on va travailler sur *thisPtr, qui est la note à effacer

    if (pilePtr != firstPtr) // on efface une notePile qui n'est pas la première
    {
        (*precedentPtr).nextPile = (*pilePtr).nextPile;

        delete pilePtr;
    }

    // on ne touche pas à la première note de la pile..
}

void ZEROSetNoteInPile(pileNote *firstPtr, int N, pileNote &laNote)
{
    N++;

    if (firstPtr == 0)
    {
        return;
    }

    if (N <= 0)
    {
        return;
    }

    int taillePile;
    taillePile = mesureTaillePileComplete(firstPtr);

    if (N > taillePile)
    {
        return;
    }

    pileNote *pilePtr = firstPtr;

    for (int i = 0; i < N; i++)
    {
        if ((*pilePtr).nextPile != 0)
        {
            pilePtr = (*pilePtr).nextPile;
        }
    }

    putNoteBinNoteA((*pilePtr), laNote);
}

// pileNote emptyNote; // TODO il faut la définir ( note = 0, vel = 0 )

pileNote ZEROgetNoteFromPile(pileNote *pilePtr, int N)
{
    N++;

    if (pilePtr == 0)
    {
        return emptyNote;
    }

    if (N <= 0)
    {
        return emptyNote;
    }

    int taillePile;
    taillePile = mesureTaillePileComplete(pilePtr);

    if (N > taillePile)
    {
        return emptyNote;
    }

    pileNote *thisPtr = pilePtr;
    pileNote laNote;
    // int compteur = 0;

    for (int i = 0; i < N; i++)
    {
        if ((*thisPtr).nextPile != 0)
        {
            thisPtr = (*thisPtr).nextPile;
        }
    }

    putNoteBinNoteA(laNote, (*thisPtr));
    return laNote;

    // TODO essayer : return (*thisPtr) ;
}

void ZEROdeleteWholePile(pileNote *firstPtr)
{
    // supprime les anciennes valeurs de la pile et renvoie le nouveau premier pointeur
    // in fine, va faire le noteStop pour ces notes !

    if (firstPtr == 0)
    {
        return;
    }

    bool goON(false);

    pileNote *pilePtr = firstPtr;

    pileNote *precedentPtr = firstPtr;

    // pileNote *newFirstPtr = firstPtr;

    do
    {

        if (pilePtr != firstPtr) // on efface une notePile qui n'est pas la première
        {
            (*precedentPtr).nextPile = (*pilePtr).nextPile;

            delete pilePtr;

            pilePtr = precedentPtr;

            if ((*pilePtr).nextPile != 0)
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

        else // on efface une notePile qui est le première
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

// A DEVELOPPER :
// MOVE WHOLE LAYER (n) to MEMORY (m)
// MOVE WHOLE MEMORY (m) to LAYER (n)
// SWITCH MEMORY / LAYER
// SWITCH MEMORY / MEMORY
// SWITCH LAYER / LAYER
// LOAD  BLOCK FILE (k) TO MEMORY (m)
// SAVE BLOCK FILE (k) to LAYER (L)

void initializeNote(pileNote &A)
{
    putNoteBinNoteA(A, emptyNote);
}

// Partie reprise de Dende pour PileToPlayOnce
// Dans la pile DENDE, on met la note à jouer ( ou à arrêter ),
// la vélocité
// le type : 0x90 pour play, 0x80 pour stop !
// enfin, le channel
// rien d'autre !
// c'est une pile dont le pointeur initial change !

pileNote laPile;
pileNote *pointeurInitial;

void playAndStopFromONCEPile()
{
    bool goON(false);

    unsigned long nowMoment = 0;
    nowMoment = nowMillis; // TODO DEBUG remplacer par Micros un peu plus tard

    pileNote *pilePtr;
    pilePtr = pointeurInitial;

    if (pilePtr == 0)
    {
    }
    else
    {
        do
        {
            // jouer les notes ici

            // le playON est en fait un playOFF dans le cas des fins de note !

            if ((*pilePtr).timeToPlayON <= nowMoment)
            {

                if ((*pilePtr).note != 0)
                // if ( true == true )
                {

                    // 28/8 DEBUG j'ai inversé les 2 lignes qui précèdent, la première condition me paraissant moins fréquente que la 2nde

                    // NOTE DEBUG est-il pertinent de mesure millis() une fois plutôt que de répéter l'action ?

                    // byte z = (*pilePtr).repeatsEffectues ;
                    byte calcul;

                    calcul = limiter((int)(*pilePtr).velocity, 0, 126);
                    // if (calcul > 127)
                    // {
                    //     calcul = 127;
                    // }
                    // if (calcul < 0)
                    // {
                    //     calcul = 0;
                    // }

                    byte calcul2 = ((*pilePtr).type + (*pilePtr).channel);
                    // le type vaut 0x90 pour note ON et 0x80 pour note OFF :)

                    int tempcalcul3 = (*pilePtr).note;

                    if (tempcalcul3 > 127)
                    {
                        tempcalcul3 = tempcalcul3 % 12 + 108;
                    }; // plus intelligent que limiter à 127

                    if (tempcalcul3 < 0)
                    {
                        tempcalcul3 = 0;
                    };

                    byte tempcalcul4 = (byte)tempcalcul3;

                    if (omitEnd[(*pilePtr).layer] == true && (*pilePtr).type == 0x80 && (int) random(100) < omitEndValue[(*pilePtr).layer])
                    {
                        // on "omet" de jouer la fin de note
                    }
                    else
                    {
                        PlaynoteOn_(calcul2, tempcalcul4, calcul);
                    }

                    (*pilePtr).killnote = true;
                    (*pilePtr).note = 0; // ne la rejouera d'office pas
                }
                else
                {
                    // (*pilePtr).killnote = true  ;
                }
            }

            // passe à note suivante

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

/////////////////////////////////////////////////////////////////////////////////////
// COURONNE 2C21
/////////////////////////////////////////////////////////////////////////////////////

/*
if (((*pilePtr).type) == 0x10)
{
    Transformation((*pilePtr).channel * 127 + (*pilePtr).note, (*pilePtr).velocity);
    (*pilePtr).killnote = true;
}
else
*/

void playFromBATCHPile()
{
    bool goON(false);

    unsigned long nowMoment = 0;
    nowMoment = nowMillis; // TODO DEBUG remplacer par Micros un peu plus tard

    pileBatch *pilePtr;
    pilePtr = pBatch;

    if (pilePtr == 0)
    {
    }
    else
    {
        do
        {
            if ((*pilePtr).timeToPlayON <= nowMoment && (*pilePtr).killnote == false)
            {

// TODO on doit sauver le layer ACTIF, mettre en place le .layer et restaurer après

byte rememberLayer = activeLayer;
activeLayer = (B0001 << (*pilePtr).layer );

Transformation((*pilePtr).transfoNb, (*pilePtr).transfoValue);

activeLayer = rememberLayer;

(*pilePtr).killnote = true;
            }

            // passe à note suivante

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

// DEBUG  if (omitEnd[(*pilePtr).layer] == true && (*pilePtr).type == 0x80 && random(100) < omitEndValue[(*pilePtr).layer])

pileNote *treatPileCompleteONCE(pileNote *firstPtr)
{
    // supprime les anciennes valeurs de la pile et renvoie le nouveau premier pointeur
    // in fine, va faire le noteStop pour ces notes !

    if (firstPtr == 0)
    {
        return 0;
    }

    bool goON(false);

    pileNote *pilePtr = firstPtr;

    pileNote *precedentPtr = firstPtr;

    pileNote *newFirstPtr = firstPtr;

    do
    {

        if ((*pilePtr).note == 0 && (*pilePtr).type != 0x50 && pilePtr != firstPtr) // on efface une note vide qui n'est pas première
        {
            (*precedentPtr).nextPile = (*pilePtr).nextPile;

            delete pilePtr;

            pilePtr = precedentPtr;

            if ((*pilePtr).nextPile != 0)
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

        if ((*pilePtr).killnote)
        {
            if (pilePtr != firstPtr) // on efface une notePile qui n'est pas la première
            {
                (*precedentPtr).nextPile = (*pilePtr).nextPile;

                delete pilePtr;

                pilePtr = precedentPtr;

                if ((*pilePtr).nextPile != 0)
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
            else // on efface une notePile qui est le première
            {
                if ((*pilePtr).nextPile != 0)
                { // et qui a des successeurs
                    precedentPtr = (*pilePtr).nextPile;
                    newFirstPtr = (*pilePtr).nextPile;
                    delete pilePtr;

                    pilePtr = precedentPtr;
                    firstPtr = pilePtr;
                    goON = true;
                }
                else
                { // et qui n'a pas de note après = la dernière
                    goON = false;
                    (*pilePtr).channel = 0;
                    (*pilePtr).note = 0; // une note vide
                    (*pilePtr).velocity = 0;
                    (*pilePtr).timeToPlayON = 0; //
                    // (*pilePtr).repeatsEffectues = 0 ; // on dit que c'est une note créée par event, si venait de matrix on devrait ici cocer la localisation dans la matrice
                    (*pilePtr).nextPile = 0; // pointe nulle part - y a pas de case 0 dans la mémoire
                    (*pilePtr).killnote = false;
                    return pilePtr; // cas de la dernière note.
                }
            }
        }

        else
        {
            if ((*pilePtr).nextPile != 0)
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

    } while (goON);

    return newFirstPtr;
}

// pileNote *giveDernierPtrPileComplete(pileNote *pilePtr)
// {
//     pileNote *thisPtr = pilePtr;
//     while ((*thisPtr).nextPile != 0)
//     {
//         thisPtr = (*thisPtr).nextPile;
//     }
//     return thisPtr;
// }

/////////////////////////////////////////////////////////////////////////////////////
// COURONNE 2C21
/////////////////////////////////////////////////////////////////////////////////////

void treatPileCompleteBATCH ( void )
{
    if (pBatch == 0)
    {
        return ;
    }

pileBatch *firstPtr = pBatch ; 

    bool goON(false);

    pileBatch *pilePtr = firstPtr;

    pileBatch *precedentPtr = firstPtr;

    // pileBatch *newFirstPtr = firstPtr;

    do
    {
        if ((*pilePtr).killnote)
        {
            if (pilePtr != firstPtr) // on efface une notePile qui n'est pas la première
            {
                (*precedentPtr).nextPile = (*pilePtr).nextPile;

                delete pilePtr;

                pilePtr = precedentPtr;

                if ((*pilePtr).nextPile != 0)
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
        }

            if ((*pilePtr).nextPile != 0)
            {
                precedentPtr = pilePtr;
                pilePtr = (*pilePtr).nextPile;
                goON = true;
            }
            else
            {
                goON = false;
            }

    } while (goON);

    return ;
}

void addNewNotetoPileONCE(pileNote *cettePileADD,
                          const byte &channelADD, const byte &noteADD, const byte &velocityADD, unsigned long msec,
                          byte type_note = 0x90, const byte &layerADD = 1)
{

    if ((CHMute >> (byte)(channelADD) & 1 ) != 0)
{
    return ; 
    // on n'ajoute pas de notes d'un channel qui est muté ! 
}

        pileNote *pilePtr;

    pilePtr = giveDernierPtrPileComplete(cettePileADD);

    (*pilePtr).nextPile = new pileNote;
    // int a = (*pilePtr).numeroPile ;

    pilePtr = (*pilePtr).nextPile;

    (*pilePtr).channel = channelADD;
    (*pilePtr).note = noteADD; // C60 sur channel 2
    (*pilePtr).velocity = velocityADD;
    (*pilePtr).timeToPlayON = msec;
    (*pilePtr).layer = layerADD;

    // (*pilePtr).repeatsEffectues = repeats ;
    // (*pilePtr).nbF8avantJeu = nbF8 ;

    (*pilePtr).killnote = false;
    (*pilePtr).type = type_note;

    (*pilePtr).nextPile = 0; // ne pointe vers rien car est la dernière
}

/////////////////////////////////////////////////////////////////////////////////////
// COURONNE 2C21
/////////////////////////////////////////////////////////////////////////////////////

// 2C21 développé sur modèle de AddNewNoteToPileONCE

void addNewtoPileBATCH(pileBatch *cettePileADD, const int &transfoNb, const int &transfoValue, byte layer, unsigned long msec)
{

    pileBatch *pilePtr;

    pilePtr = giveDernierPtrPileBatchComplete(cettePileADD);

    (*pilePtr).nextPile = new pileBatch;
    // int a = (*pilePtr).numeroPile ;

    pilePtr = (*pilePtr).nextPile;

    (*pilePtr).transfoNb = transfoNb;
    (*pilePtr).transfoValue = transfoValue;
    (*pilePtr).timeToPlayON = msec;
    (*pilePtr).layer = layer;
    (*pilePtr).killnote = false;

    (*pilePtr).nextPile = 0; // ne pointe vers rien car est la dernière
}

void killNoteOffFromPileONCE(pileNote *cettePileADD, const byte &channelKILL, const byte &noteKILL)
{
    bool goON(false);

    pileNote *pilePtr;
    pilePtr = pointeurInitial;

    if (pilePtr == NULL )
    {
        return;
    }

    pilePtr = (*pilePtr).nextPile;

    if (pilePtr == NULL )
    {
        return;
    }
    else
    {

        do
        {
            // jouer les notes ici

            if (  (*pilePtr).note == noteKILL
            &&    (*pilePtr).channel == channelKILL
            &&    (*pilePtr).type == 0x80 )
                    {
                        (*pilePtr).killnote = true;
                        (*pilePtr).note = 0; // ne la rejouera d'office pas
                    }


            // passe à note suivante

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

unsigned int ZEROFindClosestPitch(pileNote *cettePileADD, const byte &channelFIND, const byte &noteFIND)
{

    // unsigned int smallestPitchFound = 0 ;
    signed int smallestDifference = 500; // bien surestimé, la plus grande diff est évidemment 127 !
    unsigned int noteFound = 0;

    bool goON(false);
    unsigned int compteur = 0;

    pileNote *pilePtr;
    pilePtr = pointeurInitial;

    pilePtr = (*pilePtr).nextPile;

    if (pilePtr == 0)
    {
        return 0;
    }
    else
    {

        do
        {
            // jouer les notes ici

            if ((*pilePtr).channel == channelFIND)
            {

                int difference = (int)(*pilePtr).note - (int)noteFIND;
                if (difference < 0)
                {
                    difference = -difference;
                }

                if (difference < smallestDifference)
                {
                    noteFound = compteur;
                    smallestDifference = difference;
                }
            }

            // passe à note suivante

            if ((*pilePtr).nextPile != 0)
            {
                pilePtr = (*pilePtr).nextPile;
                goON = true;
            }
            else
            {
                goON = false;
            }

            compteur++;

        }

        while (goON);
    }

    return noteFound;
}

void ZEROtreatPileComplete(pileNote *firstPtr)
{
    // supprime les anciennes valeurs de la pile et renvoie le nouveau premier pointeur
    // in fine, va faire le noteStop pour ces notes !

    if (firstPtr == 0)
    {
        return;
    }

    bool goON(false);

    pileNote *pilePtr = firstPtr;

    pileNote *precedentPtr = firstPtr;

    // pileNote *newFirstPtr = firstPtr;

    do
    {

        if ((*pilePtr).note == 0 && (*pilePtr).type != 0xB0 && (*pilePtr).type != 0x50 && pilePtr != firstPtr) // on efface une note vide qui n'est pas première
        {
            (*precedentPtr).nextPile = (*pilePtr).nextPile;

            delete pilePtr;

            pilePtr = precedentPtr;

            if ((*pilePtr).nextPile != 0)
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

        if ((*pilePtr).killnote)
        {
            if (pilePtr != firstPtr) // on efface une notePile qui n'est pas la première
            {
                (*precedentPtr).nextPile = (*pilePtr).nextPile;

                delete pilePtr;

                pilePtr = precedentPtr;

                if ((*pilePtr).nextPile != 0)
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
            else // on efface une notePile qui est le première
            {
                // contrairement à la pile ONCE, on n'efface ici jamais la première note de la pile
                if ((*pilePtr).nextPile != 0)
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
        }

        else
        {
            if ((*pilePtr).nextPile != 0)
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

    } while (goON);

    return;
}

unsigned long getDuree(pileNote &celleCi)
{
    unsigned long dureeNote;

    byte i = celleCi.layer;

    if (celleCi.startNote < celleCi.endNote)
    {
        dureeNote = celleCi.endNote - celleCi.startNote;
    }
    else
    {
        dureeNote = layerLength[i] - celleCi.startNote + celleCi.endNote;
    }

    return dureeNote;
}

void basicCopyPileAtoPileB(int a, int b)
{
    // d'abord effacer pile B
    // ensuite on parcours pile A et on ajoute chaque note à pile B

    ZEROdeleteWholePile(pInitial[b]);

    // première version très safe, on pourra faire une version plus rapide après !

    unsigned int taille = mesureTaillePileComplete(pInitial[a]);
    if (taille == 0)
    {
        return;
    }

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

void ZEROcopyLayerAtoMemoryB(int a, int b)
// LAYER = 0 1 2 3
// MEMORY = 0 1 2 3 .. 63 : en réalité : pInitial 4 5 6 .. 67
{
    basicCopyPileAtoPileB(a, b + 4);
}

void ZEROcopyLayerAtoMemoryB_ONLY_ChannelC_NoteD(int a, int b, byte c, byte d )
// LAYER = 0 1 2 3
// MEMORY = 0 1 2 3 .. 63 : en réalité : pInitial 4 5 6 .. 67
{
b = b+4 ; 

    {
        // d'abord effacer pile B
        // ensuite on parcours pile A et on ajoute chaque note à pile B

        ZEROdeleteWholePile(pInitial[b]);

        // première version très safe, on pourra faire une version plus rapide après !

        unsigned int taille = mesureTaillePileComplete(pInitial[a]);
        if (taille == 0)
        {
            return;
        }

        for (unsigned int j = 0; j < taille; j++)
        { // va parcourir la mélodie pointée ( Layer 1 à 4 : potVal3 )
            pileNote tempNote;
            tempNote = ZEROgetNoteFromPile(pInitial[a], j);
            if (b < 4)
            {
                tempNote.layer = b;
            }
            if ( tempNote.channel == c && tempNote.note == d )  // 2C26 seule différence : ne copie que si CH et Note idem 
            {
            pileNote_fixStartEndNote(tempNote, 0);
            ZEROaddNewNotetoPile(pInitial[b], tempNote);
            }
        }
    }
}


void ZEROcopyMemoryAtoLayerB(int a, int b)
{
    basicCopyPileAtoPileB(a + 4, b);
}

void ZEROcopyMemoryAtoMemoryB(int a, int b)
{
    basicCopyPileAtoPileB(a + 4, b + 4);
}

// donc 0 1 2 3 sont les layers actifs
// 4..67 seront les mémoires
// 68 69 70 71 les UNDO level 1
// 72 73 74 75 les UNDO level 2
// 76 77 78 79 les UNDO level 3
// 80 81 82 83 les UNDO level 4

void ZEROcopyLayerAtoUndo(int a)
{
    // copier UNDO 3 vers UNDO 4

    basicCopyPileAtoPileB(3 * nbLayer + nbLayersMemory + a, nbLayer + nbLayersMemory + a + 3 * nbLayer);

    // copier UNDO 2 vers UNDO 3

    basicCopyPileAtoPileB(2 * nbLayer + nbLayersMemory + a, nbLayer + nbLayersMemory + a + 2 * nbLayer);

    // copier UNDO 1 vers UNDO 2

    basicCopyPileAtoPileB(nbLayer + nbLayersMemory + a, nbLayer + nbLayersMemory + a + nbLayer);

    // copier LAYER vers undo 1

    basicCopyPileAtoPileB(a, nbLayer + nbLayersMemory + a);
}

void ZEROcopyUndoToLayer(int a)
{

    // copier UNDO 1 vers LAYER

    basicCopyPileAtoPileB(nbLayer + nbLayersMemory + a, a);

    // copier UNDO 2 vers UNDO 1

    basicCopyPileAtoPileB(nbLayer + nbLayersMemory + a + nbLayer, nbLayer + nbLayersMemory + a);

    // copier UNDO 3 vers UNDO 2

    basicCopyPileAtoPileB(nbLayer + nbLayersMemory + a + 2 * nbLayer, 2 * nbLayer + nbLayersMemory + a);

    // copier UNDO 4 vers UNDO 3

    basicCopyPileAtoPileB(nbLayer + nbLayersMemory + a + 3 * nbLayer, 3 * nbLayer + nbLayersMemory + a);
}

unsigned long distanceEntre2Start(pileNote &celleCi, pileNote &celleLa)
{
    unsigned long A = celleCi.startNote;
    unsigned long B = celleLa.startNote;
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
        D = layerLength[celleLa.layer] - B + A;
    }
    else
    {
        D = layerLength[celleCi.layer] - A + B;
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

void ZEROcutSliceMemoryAtoLayerB(int a, int b, long cutSize, long oldLayerSize)
/**
 * @brief coupe un fragemnt de taille cutSize de la mémoire a vers b 
 * 
 */
{
    // d'abord on copie rapidement la mémoire vers le layer

    ZEROdeleteWholePile(pInitial[b]);

    a = a + 4; // pour mémoire

    unsigned int taille = mesureTaillePileComplete(pInitial[a]);
    if (taille == 0)
    {
        return;
    }

    // ensuite :
    // 1. on mesure la taille
    // 2. on choisit un des points de départ
    // 3. on sélectionne les notes qui à partir de là sont dans le scope : cutSize (msec)

    pileNote tempNote;

    unsigned int chosenStartNumber = random(0, taille);
    tempNote = ZEROgetNoteFromPile(pInitial[a], chosenStartNumber);

    unsigned long chosenStartTime = tempNote.startNote;
    unsigned long calculatedEndTime = tempNote.startNote + cutSize;
    calculatedEndTime = calculatedEndTime % oldLayerSize;

    for (unsigned int j = 0; j < taille; j++)
    { // va parcourir la mélodie pointée ( Layer 1 à 4 : potVal3 )
        pileNote tempNote;
        tempNote = ZEROgetNoteFromPile(pInitial[a], j);

        if (b < 4)
        {
            tempNote.layer = b;
        }

        unsigned long duree = getDuree(tempNote);

        if (chosenStartTime < calculatedEndTime)
        {
            if (tempNote.startNote > chosenStartTime && tempNote.startNote < calculatedEndTime)
            {
                tempNote.startNote = tempNote.startNote - chosenStartTime;
               // tempNote.endNote = tempNote.endNote + oldLayerSize - chosenStartTime; 2C24
               tempNote.endNote = tempNote.startNote + duree ; 
                pileNote_fixStartEndNote(tempNote, 0);
                ZEROaddNewNotetoPile(pInitial[b], tempNote);
            }
        }
        else
        {
            if (tempNote.startNote < calculatedEndTime || tempNote.startNote > chosenStartTime)
            {
                tempNote.startNote = tempNote.startNote - chosenStartTime;
                // tempNote.endNote = tempNote.endNote + oldLayerSize - chosenStartTime; 2C24 
                tempNote.endNote = tempNote.startNote + duree;
                pileNote_fixStartEndNote(tempNote, 0);
                ZEROaddNewNotetoPile(pInitial[b], tempNote);
            }
        }
    }
}

void ZEROaddSliceMemoryAtoLayerB(int a, int b, long cutSize, long oldLayerSize, unsigned long receiver)
/**
 * @brief coupe un fragemnt de taille cutSize de la mémoire a vers b 
 * 
 */
{
    // d'abord on copie rapidement la mémoire vers le layer

    // ZEROdeleteWholePile(pInitial[b]);

    a = a + 4; // pour mémoire

    unsigned int taille = mesureTaillePileComplete(pInitial[a]);
    if (taille == 0)
    {
        return;
    }

    // ensuite :
    // 1. on mesure la taille
    // 2. on choisit un des points de départ
    // 3. on sélectionne les notes qui à partir de là sont dans le scope : cutSize (msec)

    pileNote tempNote;

    unsigned int chosenStartNumber = random(0, taille);
    tempNote = ZEROgetNoteFromPile(pInitial[a], chosenStartNumber);

    unsigned long chosenStartTime = tempNote.startNote;
    unsigned long calculatedEndTime = tempNote.startNote + cutSize;
    calculatedEndTime = calculatedEndTime % oldLayerSize;

    for (unsigned int j = 0; j < taille; j++)
    { // va parcourir la mélodie pointée ( Layer 1 à 4 : potVal3 )
        pileNote tempNote;
        tempNote = ZEROgetNoteFromPile(pInitial[a], j);

unsigned long duree = getDuree(tempNote); 

        if (b < 4)
        {
            tempNote.layer = b;
        }

        if (chosenStartTime < calculatedEndTime)
        {
            if (tempNote.startNote > chosenStartTime && tempNote.startNote < calculatedEndTime)
            {
                tempNote.startNote = tempNote.startNote - chosenStartTime + receiver;
               // tempNote.endNote = tempNote.endNote + oldLayerSize - chosenStartTime + receiver;
                tempNote.endNote = tempNote.startNote + duree  ;
                pileNote_fixStartEndNote(tempNote, 0);
                ZEROaddNewNotetoPile(pInitial[b], tempNote);
            }
        }
        else
        {
            if (tempNote.startNote < calculatedEndTime || tempNote.startNote > chosenStartTime)
            {
                tempNote.startNote = tempNote.startNote - chosenStartTime + receiver;
               // tempNote.endNote = tempNote.endNote + oldLayerSize - chosenStartTime + receiver;
                tempNote.endNote = tempNote.startNote + duree;
                pileNote_fixStartEndNote(tempNote, 0);
                ZEROaddNewNotetoPile(pInitial[b], tempNote);
            }
        }
    }
}

bool checkNoteAChannelBinVariationsPiles(byte n, byte CH)
{
    unsigned int taille = mesureTaillePileComplete(pVariations);
    if (taille == 0)
    {
        return false;
    }

    for (unsigned int j = 0; j < taille; j++)
    { // va parcourir la mélodie pointée ( Layer 1 à 4 : potVal3 )
        pileNote tempNote;
        tempNote = ZEROgetNoteFromPile(pVariations, j);
        if (tempNote.note == n && tempNote.channel == CH)
        {
            return true;
        }
    }

    return false;
}


bool checkNoteAChannelBinStepzPiles(byte n, byte CH)
{
    unsigned int taille = mesureTaillePileComplete(pVariationsLapidz);
    if (taille == 0)
    {
        return false;
    }

    for (unsigned int j = 0; j < taille; j++)
    { // va parcourir la mélodie pointée ( Layer 1 à 4 : potVal3 )
        pileNote tempNote;
        tempNote = ZEROgetNoteFromPile(pVariationsLapidz, j);
        if (tempNote.note == n && tempNote.channel == CH)
        {
            return true;
        }
    }

    return false;
}

bool checkNoteAChannelBinDialysePiles(byte n, byte CH)
{
    unsigned int taille = mesureTaillePileComplete(pVariationsDialyse);
    if (taille == 0)
    {
        return false;
    }

    for (unsigned int j = 0; j < taille; j++)
    { // va parcourir la mélodie pointée ( Layer 1 à 4 : potVal3 )
        pileNote tempNote;
        tempNote = ZEROgetNoteFromPile(pVariationsDialyse, j);
        if (tempNote.note == n && tempNote.channel == CH)
        {
            return true;
        }
    }

    return false;
}

bool checkNoteAChannelBinSoftPassPiles(byte n, byte CH)
{
    unsigned int taille = mesureTaillePileComplete(pSoftPassRedirect);
    if (taille == 0)
    {
        return false;
    }

        pileNote tempNote;

    for (unsigned int j = 0; j < taille; j++)
    { // va parcourir la mélodie pointée ( Layer 1 à 4 : potVal3 )

        tempNote = ZEROgetNoteFromPile(pSoftPassRedirect, j);
        if (tempNote.note == n && tempNote.channel == CH)
        {
            return true;
        }
    }

    return false;
}

int getRedirectValue_from_NoteAChannelBinSoftPassPiles(byte n, byte CH)
{
    unsigned int taille = mesureTaillePileComplete(pSoftPassRedirect);
    if (taille == 0)
    {
        return false;
    }

    pileNote tempNote;

    for (unsigned int j = 0; j < taille; j++)
    { // va parcourir la mélodie pointée ( Layer 1 à 4 : potVal3 )

        tempNote = ZEROgetNoteFromPile(pSoftPassRedirect, j);
        if (tempNote.note == n && tempNote.channel == CH)
        {
            return tempNote.MMnoteReallyPlayed ;
        }
    }

    return 0 ;
}

void deleteNoteAChannelBinSoftPassPiles(byte n, byte CH)
{
    unsigned int taille = mesureTaillePileComplete(pSoftPassRedirect);
    if (taille == 0)
    {
        return ;
    }

    pileNote tempNote;

    for (unsigned int j = 0; j < taille; j++)
    { // va parcourir la mélodie pointée ( Layer 1 à 4 : potVal3 )

        tempNote = ZEROgetNoteFromPile(pSoftPassRedirect, j);
        if (tempNote.note == n && tempNote.channel == CH)
        {
            tempNote.note = 0 ; 
            tempNote.killnote = true;
            tempNote.velocity = 0;
            ZEROSetNoteInPile(pSoftPassRedirect,j,tempNote); 

            ZEROdeleteNoteFromPile(pSoftPassRedirect,j);
            return ; 
        }
    }

    return  ;
}


bool checkNoteAChannelBinSustainBlockPiles(byte n, byte CH)
{
    unsigned int taille = mesureTaillePileComplete(pSustainBlock);
    if (taille == 0)
    {
        return false;
    }

    pileNote tempNote;

    for (unsigned int j = 0; j < taille; j++)
    { // va parcourir la mélodie pointée ( Layer 1 à 4 : potVal3 )

        tempNote = ZEROgetNoteFromPile(pSustainBlock, j);
        if (tempNote.note == n && tempNote.channel == CH)
        {
            return true;
        }
    }

    return false;
}

unsigned long getTimeValue_from_NoteAChannelBinSustainBlockPiles(byte n, byte CH)
{
    unsigned int taille = mesureTaillePileComplete(pSustainBlock);
    if (taille == 0)
    {
        return false;
    }

    pileNote tempNote;

    for (unsigned int j = 0; j < taille; j++)
    { // va parcourir la mélodie pointée ( Layer 1 à 4 : potVal3 )

        tempNote = ZEROgetNoteFromPile(pSustainBlock, j);
        if (tempNote.note == n && tempNote.channel == CH)
        {
            return tempNote.startNote ;
        }
    }

    return 0;
}

void deleteNoteAChannelBinSustainBlockPiles(byte n, byte CH)
{
    unsigned int taille = mesureTaillePileComplete(pSustainBlock);
    if (taille == 0)
    {
        return;
    }

    pileNote tempNote;

    for (unsigned int j = 0; j < taille; j++)
    { // va parcourir la mélodie pointée ( Layer 1 à 4 : potVal3 )

        tempNote = ZEROgetNoteFromPile(pSustainBlock, j);
        if (tempNote.note == n && tempNote.channel == CH)
        {
            tempNote.note = 0;
            tempNote.killnote = true;
            tempNote.velocity = 0;
            ZEROSetNoteInPile(pSustainBlock, j, tempNote);

            ZEROdeleteNoteFromPile(pSustainBlock, j);
            taille = mesureTaillePileComplete(pSustainBlock);
            j = 0 ; 
//            return;
        }

        // TODO peut être faire en deux fois pour bien éliminer tous les éléments ?
        // d'abord un loop SET et ensuite un loop DELETE ? 
    }

    return;
}

// Proxy BLOCK

bool checkNoteAChannelBinProxyBlockPiles(byte n, byte CH)
{
    unsigned int taille = mesureTaillePileComplete(pProxyBlock);
    if (taille == 0)
    {
        return false;
    }

    pileNote tempNote;

    for (unsigned int j = 0; j < taille; j++)
    { // va parcourir la mélodie pointée ( Layer 1 à 4 : potVal3 )

        tempNote = ZEROgetNoteFromPile(pProxyBlock, j);
        if (tempNote.note == n && tempNote.channel == CH)
        {
            return true;
        }
    }

    return false;
}

unsigned long getTimeValue_from_NoteAChannelBinProxyBlockPiles(byte n, byte CH)
{
    unsigned int taille = mesureTaillePileComplete(pProxyBlock);
    if (taille == 0)
    {
        return false;
    }

    pileNote tempNote;

    for (unsigned int j = 0; j < taille; j++)
    { // va parcourir la mélodie pointée ( Layer 1 à 4 : potVal3 )

        tempNote = ZEROgetNoteFromPile(pProxyBlock, j);
        if (tempNote.note == n && tempNote.channel == CH)
        {
            return tempNote.startNote;
        }
    }

    return 0;
}

void deleteNoteAChannelBinProxyBlockPiles(byte n, byte CH)
{
    unsigned int taille = mesureTaillePileComplete(pProxyBlock);
    if (taille == 0)
    {
        return;
    }

    pileNote tempNote;

    for (unsigned int j = 0; j < taille; j++)
    { // va parcourir la mélodie pointée ( Layer 1 à 4 : potVal3 )

        tempNote = ZEROgetNoteFromPile(pProxyBlock, j);
        if (tempNote.note == n && tempNote.channel == CH)
        {
            tempNote.note = 0;
            tempNote.killnote = true;
            tempNote.velocity = 0;
            ZEROSetNoteInPile(pProxyBlock, j, tempNote);

            ZEROdeleteNoteFromPile(pProxyBlock, j);
            taille = mesureTaillePileComplete(pProxyBlock);
            j = 0;
            //            return;
        }

        // TODO peut être faire en deux fois pour bien éliminer tous les éléments ?
        // d'abord un loop SET et ensuite un loop DELETE ?
    }

    return;
}

void updateNoteAChannelBinProxyBlockPiles(byte n, byte CH)
{
    unsigned int taille = mesureTaillePileComplete(pProxyBlock);
    if (taille == 0)
    {
        return;
    }

    pileNote tempNote;

    for (unsigned int j = 0; j < taille; j++)
    { // va parcourir la mélodie pointée ( Layer 1 à 4 : potVal3 )

        tempNote = ZEROgetNoteFromPile(pProxyBlock, j);
        if (tempNote.note == n && tempNote.channel == CH)
        {
            tempNote.endNote = nowMillis ; 
            tempNote.thereIsAnEnd = true ; 
            ZEROSetNoteInPile(pProxyBlock, j, tempNote);
        }

        // TODO peut être faire en deux fois pour bien éliminer tous les éléments ?
        // d'abord un loop SET et ensuite un loop DELETE ?
    }

    return;
}

bool isThereFreeMem (void )
{
    // 2C26 basé sur 536: // first free mem
    int firstfree = 100;

    for (int i = 0; i < 64; i++)
    {
        int memoireVisee = pFluxStart + i;
        unsigned int taille = mesureTaillePileComplete(pInitial[memoireVisee]);
       

        if (taille == 0)
        {
            if (firstfree == 100)
            {
                firstfree = i;
                return true ; 
            }
        }
    }

    return false ; 
}

int giveFirstFreeMem( void )
{

// 2C26 basé sur 536: // first free mem
int firstfree = 100;

for (int i = 0; i < 64; i++)
{
    int memoireVisee = pFluxStart + i;
    unsigned int taille = mesureTaillePileComplete(pInitial[memoireVisee]);

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