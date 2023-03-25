// 1C99
// si on modifie un STRUCT : note, sequence, event, CCMod, etc.
// il faut en conséquence modifier saveLoadLocal.h pour enregistrer cette modification
// il faut modifier la procédure SAVE et LOAD en parallèle
// il faut également modifier la fonction qui initialise la valeur à zéro

// reste à faire :
// les valeurs doivent être inclues dans la structure, car il y a aura plusieurs structures simultanément !
// la subséquence random doit être recéée à chaque fois, on conserve juste le point de départ et la durée.

// essayer la creation via new de nouveau tableaux de ce type
// et en récupérer les données

typedef struct pileNote pileNote; // poids initial 19 byte !!!  doit être optimisé !

struct pileNote
{
    byte channel;  // ex 0
    byte note;     // ex 1
    byte velocity; // ex 2

    byte MMnoteReallyPlayed;

    // ceci définit le début et fin à l'intérieur d'une boucle
    unsigned long startNote;
    unsigned long endNote;
    bool thereIsAnEnd;
    // TODO ajouté à 1C92 pour éviter des chipotages dans l'ignorance

    // ceci par contre définit le moment précis en valeur absolue où la note devra être jouée la prochaine fois
    unsigned long timeToPlayON;
    // unsigned long timeToPlayOFF; // 2C21 on gagne 4 bytes par note et delais ce qui n'est pas rien ! 
    boolean killnote;
    // byte data1;  SUPPRIME DEPUIS 1C98 - cfr 1C97 pour détails

    byte type;  // 0x90 = noteON   0x80 = OFF    0xB0 = CC
    byte layer; // je me demande si c'est utile - oui ça l'est, car certaines modifs sont liées au layer de la note !

    pileNote *nextPile;
}; // j'ai pas oublié cette fois !

// poids initial 19 byte !!!  doit être optimisé !
// optimisation : on n'a jamais besoin à la fois de startnote et endnote : on fait une pile pour start et une pile pour end.
// mène à 15 bytes
// on doit pouvoir combiner ilayer et jnote en un byte ?
// reste 14 bytes
// on peut supprimer un des 2 pointeurs et juste garder un pointeur vers le début de la pile et vers le suivant ( on doit toujours partir de 1 pour arriver au bout )
// on garde le numeroPile pour le debug mais après sera supprimé : -1
// 12 bytes ( -1 sans numeroPile )

// byte const nbSequencesDataMax = 12 ; // pour commencer max 12 variations dans une séquence
// devient
// byte const tailleMaxTableauSequence(32);

// byte const nbDonnesComportement = 4 ;

// int tableauSequence[tailleMaxTableau] {0};
// int tableauSequence2[tailleMaxTableau] {0};
// int tableauSequenceRand[tailleMaxTableau] {0};

// les tableaux de Séquence sont des tableaux dont le premier élement [0] indique la longueur !
// alternativement on pourrait créer une structure qui comporterait un byte pour longueur et un array

typedef struct pileBatch pileBatch ;  

struct pileBatch 
{
    int transfoNb ;
    int transfoValue ;
    unsigned long timeToPlayON ;
    bool killnote ; 
   
    byte layer; // je me demande si c'est utile - oui ça l'est, car certaines modifs sont liées au layer de la note !

    pileBatch *nextPile;
}; // j'ai pas oublié cette fois !


// *********************************************************
// * 2C22 KARAKT
/// *********************************************************

typedef struct pileKarakt pileKarakt; // poids initial 19 byte !!!  doit être optimisé !

struct pileKarakt
{
    int valeur ; 
    int number ; 
    byte layer ; 
   
    pileKarakt *nextPile;
}; // j'ai pas oublié  


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
    int jitterRichting; // variables internes, qu'on ne modifie pas soi même
    // NOTE DEBUG peut être transformé en version courte, si code suit

    int jitterWhereNow; // variables internes, qu'on ne modifie pas soi même
    int jitterWhereTo;  // variables internes, qu'on ne modifie pas soi même

    byte jitterStaticCompteur; // variables internes, qu'on ne modifie pas soi même
    byte jitterStaticToReach;  // variables internes, qu'on ne modifie pas soi même

    // int jitterPolarity(0); // -1 : wherenow < whereTo : +1 : > : 0 =
    // int oldJitterPolarity(0) ; // dernière valeur
    boolean beingStatic; // variables internes, qu'on ne modifie pas soi même
    // on pourrait coder les bool sur des bits NOTE DEBUG
    //
    byte sizeRANDOMFRAG;
    byte repeatRANDOMFRAG;
    byte toReachRANDOMFRAG; // variables internes, qu'on ne modifie pas soi même
    byte keepN;

    boolean compteLayer; // si cette valeur est fausse il compte les notes
    // si elle est vraie, il compte les layers

    long compteur; // +1 à chaque itération, vaudra un grand nombre

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

typedef struct point point;
struct point
{
    float x;
    float y;
};

typedef struct cellBijection cellBijection;
struct cellBijection
{
    byte channelIn;
    byte noteIn;
    byte channelOut;
    byte noteOut;
};

typedef struct CCModule CCModule;
struct CCModule
{
    byte type;          // 0 = vide   1 = VELOCITY  2 = PITCH   3 = NOTE
    int sourceMin;      // 0 à 127 : vélocité, pitch, note (0-12) - va au delà de 127 pour la durée de notes
    int sourceMax;      // 0 à 127
    byte targetMin;     // 0 à 127 : valeur du CC 0x Bn 103 Valeur
    byte targetMax;     // 0 à 127 : valeur du CC 0x Bn 103 Valeur
    byte borders;       // 0 = limite    1 = expand     2 = No Send audelà et endeça des valeurs min et max   3 = invert : envoie au-delà mais pas entre !
    byte idCC;          // définit le CC auquel on envoie la valeur : ex : 103 pour EditA sur Behringer2024
    byte channelCC;     // définit sur quel Channel n on doit envoyer l'info CC : 0x Bn 103 Valeur
    byte layerConcerne; // définit quel layer sont concernés : 00004321
    // attention : 1000 0000 = concerne le Midi parallel player
    boolean actif; // permet d'avoir un CCMod non activé sur lequel on travaille
};

// typedef struct pileCCMod pileCCMod ;
// struct pileCCMod {
//     CCModule CCMod ;
//     int numeroPile ;   // pourra disparaître
//     pileCCMod* nextCCMod ;
// } ; // j'ai pas oublié cette fois !

typedef struct event event;

// NOTE attention ! si on change la structure d'event il faut changer LOAD et SAVE event

struct event
{
    boolean actif; // rajouté en nov2018, sur le modèle de Sequence
    byte type;     // 0 = Vide ( il faut initialiser ! )   1 = CC
    // 2 = Transformation
    // (3) = Modification de valeur ( peut être intégrer à transformation ! )

    unsigned int intervalle; // exprimé en 0.1 secondes  10 = 1 seconde 100 = 10 secondes 1000 = 100 secondes 10000 = quasi 1 h
    unsigned int moment;     // // dans les temps divisible par 300, agit après 100
    byte fidelite;           // en %
    byte typeInterval;       // 1 = constant 2 = random(a,b)  3=gaussian   4 = bi intervalle
    // NOTE **** à implémenter

    int which; // si type 1 : quel CC
    // si type 2 : quelle transformation
    // si type 3 : quelle valeur ( comment définir ? liste dans tableau, ou via transformation ? paraît mieux )
    int valeur;
    byte shape; // 1 = valeur fixe ( pour valeur )
    // 2 = INC
    // 3 = DEC ( ou bien INC avec valeur négative ? )
    // 4 = RND  ( définir min et max, 0-127 par défaut pour CC )
    // 5 = LISTE : ex 4 valeurs 37,69,120,130
    //  5=liste sequential 6 = liste random
    // // NOTE**** il faudra pour la liste réapprendre les pointeurs; j'en aurai de toutes façons besoins pour passer
    // des fragments (bars) à des morceaux.
   int rangeUP_; // pour option 2,3,4
    // NOTE DEBUG : le préciser dans affichae, comme j'ai fait pour Seq, c'est bcp plus compréhensible
   int rangeLO_;      // pour option 2,3,4
    byte incSize;       // définit de combien la valeur en option 2 et 3 ( qui donnent le sens)
    byte behaviorLimit; // pour options shape 2,3
    // 1 = Change direction
    // 2 = qd dépasse vers haut ou bas, réapparait de l'autre côté
    // 3 = reste à la limite
    byte channel; // channel concerné 0-15
    // voir le codage ligne 257 environ
    byte layer;              // layer concerné 1-4 ou combinaison ou random ; ou bien layers actives.   par défaut : Actif ; sinon spécifique
    unsigned long eventGOON; // indique à quel moment le prochain Evènement doit avoir lieu

    boolean conditionActive;
    byte whichTest;
    byte whichOperation;
    // Which Operation 1% 2/ 3x 4%+ 5+
    byte whichValue;
    byte whichComparison;
    byte whichReference;
    byte whichAction;     // 1 DO 2.BLOCK
    boolean launch;       // utilisé pour ce qui est lié  à des notes ou layer modulo play
    byte whichLayer2test; //

    byte modulo;
    byte moduloK;
    byte moduloLayers;
    byte moduloMode;

    int compteur; // sera un compteur qui décompte le nb de fois qu'un evt a lieu
    // doit pouvoir être testé
    int echeance;       // decrit le nb de fois maximum qu'un evt peut avoir lieu
    int latence;        // décrit à partir de quand l'event aura réellement lieu
    boolean fugace;     // si YES, qd compteur arrive à échéance, l'evt est supprimé
    byte seqUsedAsList; // TODO DEBUG : mettre àjour initialisation, delete et save

}; // ne pas oublier le ; après une définition de struct !

typedef struct LFOstruct LFOstruct;

// NOTE attention ! si on change la structure d'event il faut changer LOAD et SAVE event

struct LFOstruct
{
    boolean actif;          // rajouté en nov2018, sur le modèle de Sequence
    byte type;              // 0 = vide 1 = CC 2 = transfo : on va développer 1 pour l'instant, 2 sera pour plus tard, peut être
    byte shape;             // sin square Sample&Hold saw
    byte fidelite;          // normalement 100, mais on pourra avoir dégradation du signal
    unsigned long duration; // en millisecondes
    unsigned long decalage;
    // int amplitude;
    int lowest; // je mets INT car je pense que ce ne sera pas que pour CC, mais aussi Transfo !!!
    int highest;
    // int stepValue;  // augment de n à chaque pas.
    int compteur;   // indique la valeur actuelle
    // byte direction; // 0 = UP  1 = DOWN

    byte channel;     // channel concerné 0-15
    int which;        // CC Value ou valeur de Transfo
    byte typeMachine; // 0 = normal : CC en 3 byte ; faudra spécifique pour microSampler, TX81Z, WavePad
    int valeur;       // valeur à jouer au moment où eventGOON arrive

    byte layer;              // layer concerné 1-4 ou combinaison ou random ; ou bien layers actives.   par défaut : Actif ; sinon spécifique
    unsigned long eventGOON; // indique à quel moment le prochain Evènement doit avoir lieu
byte modC ;
byte modV ; 
// long residue ;
long oldResidue ; 

byte LFORandom ; // 2C27 ajout -> TODO ajouter à SAVE LOAD 

}; // ne pas oublier le ; après une définition de struct !

typedef struct slotType slotType;
struct slotType
{
    byte x;
    byte y;
};

typedef struct fract fract;
struct fract
{
    byte num;
    byte den;
    float imprecision; // exprime l'imprécision par rapport à la valeur de départ
};
