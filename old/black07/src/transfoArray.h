

// toujours +1 par rapport au nb de transfo existantes
// NOTE on devrait pouvoir le déduire de la taille pragmatique de TRANSFO

/*
note sur l'affichage des channels
on affiche le numéro ENDUSER, en tous cas dans ce tableau-ci
ensuite, quand la transformation a lieu, il faut que ce channel soit converti en valeur machine ( moins 1 )

il ne faut pas confondre les valeurs de channel : 1-16, correspondant à 0-15 machine
et le choix des layers :
les valeurs y vont de 0 ( rien de sélectionné 0000) à 15 ( tout sélectionné 1111 )
nénamoins nous rajoutons 16 ( 10000 ) qui signifiera : les layers actifs !!!!

*/
const byte TRANSFO_NO_VALUE = B0000; // signifie qu'on va gérer l'affichage nous-même
const byte TRANSFO_SIMPLE_VALUE = B0001;
const byte TRANSFO_PERCENT = B00011;
const byte TRANSFO_MSEC = B0101;
const byte TRANSFO_LAYER = B0111;
const byte TRANSFO_CHANNEL = B1001;
const byte TRANSFO_NbNOTES = B1011;
const byte TRANSFO_BEATS = B1101;
const byte TRANSFO_DEMITONS = B1111;
const byte TRANSFO_NomDeNOTE = B10001;
const byte TRANSFO_ON_OFF = B10011;
const byte TRANSFO_MSEC_SEQ = B10101;

int Transfo[][5] = {
    // [ ] = ORDRE d'AFFICHAGE (PotVal-1) , N° du PROGRAMME ( TTT dans Transformation ), valeur MIN, valeur MAX, description
    // il faut ici compter les lignes : les modules s'afficheront dans l'odre où ils se trouvent ici !!!

    // le bit 1 (droite) décrit si est affichage NL ( min, max, symbole )
    // les bits 2, 3 et 4 (gauche) décrivent le type de symbole
    // 0000 = complètement particulier, géré
    // 001 = %   010 = msec   011 = Layer   100 = Channel    101 = notes    110 = beats   111 = /2 tons
    // 1000 = la note xxx ( NOTE addition avril 2018 )
    // 1001 = ON/OFF NOTE : ça marche déjà ? NON, car l'affichage de cela ne dépend pas du potVal, mais d'une valeur particulière à la transformation.  il faudrait pouvoir associer le numéro de transformation à la valeur en question

    // NUMERO - VAL MIN - VAL MAX - TYPE AFFICHAGE
    //
    // il faudra peut-être quitter CONST car pour l'affichage des noms de fichiers ce serait plus utile

    {24, 1, 100, TRANSFO_NO_VALUE}, // RECORD ON/OFF
    {576,0,0,TRANSFO_NO_VALUE},  // SHUT UP 
    {577,0,nbSequencesMax, TRANSFO_NO_VALUE}, // Sequential PostPone 2C31 
    
    {573,2, 64, TRANSFO_SIMPLE_VALUE}, // Decalagem 
    {574,1,100,TRANSFO_PERCENT},   // % Decalagem
    {575,1,64,TRANSFO_SIMPLE_VALUE}, // max Decalagem 

    {570, 1, 30000, TRANSFO_NO_VALUE}, // Sustain Blocker
    {571, 1, 30000, TRANSFO_NO_VALUE}, // Proxy Pitcher 
    { 572, -24,24, TRANSFO_DEMITONS},   // proxy pitch value 

    {564, 0, 0, TRANSFO_NO_VALUE},               // Auto Harmonize Live Playing
    {565, 0, 12, TRANSFO_NO_VALUE},              // Select SCALE TONE Auto Gamme LIVE      !!!402
    {566, 0, nbDeModesConnus, TRANSFO_NO_VALUE}, // Select MODE without Harmonize // sur 251
    {567, 0, 6, TRANSFO_NO_VALUE},               // type Filtre pour AutoHarmonize Live Playing

    {558, 0, 0, TRANSFO_NO_VALUE}, // Cut first note
    {559, 0, 0, TRANSFO_NO_VALUE}, // Cut last note

    {552, 0, 10, TRANSFO_NO_VALUE}, // Serial Verbose
    {545, 0, 0, TRANSFO_NO_VALUE},  // Verbose Layers content at once

    {553, 0, 127, TRANSFO_SIMPLE_VALUE}, // CC send : set CC Number
    {554, 1, 16, TRANSFO_SIMPLE_VALUE},  // CC send : set CH number
    {555, 0, 127, TRANSFO_SIMPLE_VALUE}, // CC send : send value ! attention, MachineSpecific fonctionne !
    // Définit la valeur par rapport au channel, pas par rapport au LFO !!!! 

    {535, 0, nbSequencesMax, TRANSFO_NO_VALUE}, // Tracker ON/OFF + Sequence

    {531, 0, 128, TRANSFO_SIMPLE_VALUE}, // Permute les notes entre n et 127-n

    {524, 0, 63, TRANSFO_NO_VALUE}, // enregistre REC flux dans MEM

    {523, 0, 0, TRANSFO_NO_VALUE}, // SAVE ALL KARAKT

    {519, 1, 100, TRANSFO_NO_VALUE}, // Gaberrito 2C23
    {520, 1, 1000, TRANSFO_MSEC},    // Gabberito interval 2C23

    {525, -nbSequencesMax, 100, TRANSFO_NO_VALUE}, // Gaberrito v2 2C25
    {526, -nbSequencesMax, 100, TRANSFO_NO_VALUE}, // K * (n rep)
    {527, -nbSequencesMax, 100, TRANSFO_NO_VALUE}, // k * (N rep)
    {528, 0, 0, TRANSFO_NO_VALUE},                 // FLUSH

    {515, 1, 16, TRANSFO_NO_VALUE}, // Mute CHANNEL
    {516, 1, 16, TRANSFO_NO_VALUE}, // Mute CHANNEL
    {517, 1, 16, TRANSFO_NO_VALUE}, // unMute ALL

    {514, 0, 0, TRANSFO_NO_VALUE},       // VERBOSE FULL PILE todo option plus ou moins full pile
    {118, 5, 190, TRANSFO_SIMPLE_VALUE}, // Set BPM

    {506, 0, 0, TRANSFO_NO_VALUE}, // BATCH MUTE UNMUTE

    {502, -nbSequencesMax, 64, TRANSFO_NO_VALUE}, // Fold /64èmes
    {503, -nbSequencesMax, 64, TRANSFO_NO_VALUE}, // Fold Shorten /64ème
    {504, -nbSequencesMax, 64, TRANSFO_NO_VALUE}, // unFold /64èmes
    {505, -nbSequencesMax, 64, TRANSFO_NO_VALUE}, // unFold Longer /64ème

    {492, 0, 0, TRANSFO_NO_VALUE},      // Variations ON/OFF
    {493, 0, 30, TRANSFO_NbNOTES},      // variations Minimum notes
    {494, 0, 30, TRANSFO_NbNOTES},      // variations Redux Max notes
    {495, 0, 100, TRANSFO_PERCENT},     // %Disparition des notes
    {496, 0, 1, TRANSFO_NO_VALUE},      //  // choix du mode variation : 0 = dégressif 1 = constant
    {541, 1, 64, TRANSFO_SIMPLE_VALUE}, // nb de variations répétées à l'identique

    {379, 0, 0, TRANSFO_NO_VALUE},  // TAP RECORD IN
    {350, 1, 12, TRANSFO_NO_VALUE}, // Rec Automation
    {480, 0, 10, TRANSFO_NO_VALUE}, // Set CH0 as machine specific

    {276, 0, 0, TRANSFO_NO_VALUE},      // PANIC : arrête toutes les notes d'un synthé
    {371, 2, 32, TRANSFO_SIMPLE_VALUE}, // Multiplier Simple

    {348, 0, 0, TRANSFO_NO_VALUE}, // Erase Recording

    {25, 1, 100, TRANSFO_NO_VALUE}, // MIDI OX

    {546, 1, 23, TRANSFO_NO_VALUE},  // numérateur time signature
    {547, 1, 23, TRANSFO_NO_VALUE},  // dénominateur time signature
    {548, 0, 128, TRANSFO_NO_VALUE}, // POLYZ choix des notes polyrythmiques
    {512, 1, 128, TRANSFO_NO_VALUE}, // LAPIDZ choix des notes parmi celles qui existent au début
                                     // !!! va modifier le currentChannel en fc de ce qu'on y trouve !
    {546, 1, 23, TRANSFO_NO_VALUE},  // numérateur time signature
    {547, 1, 23, TRANSFO_NO_VALUE},  // dénominateur time signature
    {548, 0, 128, TRANSFO_NO_VALUE}, // POLYZ choix des notes polyrythmiques
    {441, 0, 128, TRANSFO_NO_VALUE}, /// PUNTZ choix de la note qui va être balancée
    {424, 0, 128, TRANSFO_NO_VALUE}, /// STEPZ choix de la note qui va être balancée, mais aussi état des différents layers
    // Attention, tout ce qui est la note ZERO sera ACCENT lié au CHANNEL, pas une note !!!!
    // tout ce qui est 128 est TOTAL ACCENT, peu importe le channel

    {427, 1, 16, TRANSFO_CHANNEL}, // Set Channel

    {521, 1, 127, TRANSFO_SIMPLE_VALUE}, // Set accent MIN
    {522, 1, 127, TRANSFO_SIMPLE_VALUE}, // Set accent MAX

    {440, 1, 300, TRANSFO_SIMPLE_VALUE}, // nombre de BPM pour Stepz, avec modif de LayerLength

    {439, 1, 256, TRANSFO_BEATS},       // nb Stepz avec Change Length
    {437, 1, 16, TRANSFO_SIMPLE_VALUE}, // nombre de Stepz pour 1BPM, 1 beat - sur Electribe = 4

    {426, 1, 5, TRANSFO_BEATS},   // choix du nb de subStepz ( illimité, sauf qu'au-delà de 3 faut du push ! )
    {425, 1, 256, TRANSFO_BEATS}, // Choix nb Stepz ( limité par hardware à 4, mais on peut rêver ) - LayerLength reste idem

    {428, 0, 127, TRANSFO_SIMPLE_VALUE}, // Set velocity for step

    {429, 0, 36, TRANSFO_NO_VALUE},      // set mode pour stop de Stepz : 0 à 4 simples, au-delà 32èmes
    {436, 1, 8, TRANSFO_SIMPLE_VALUE},   /// nombre de Stepz affichés
    {438, 1, 300, TRANSFO_SIMPLE_VALUE}, // nombre de BPM pour Stepz
    {513, 0, 0, TRANSFO_NO_VALUE},       // keep STepz BUG

    {485, 0, 0, TRANSFO_NO_VALUE}, // TAP BPM

    {466, 0, 0, TRANSFO_NO_VALUE},                 // Dende On/OFF
    {444, 0, 300, TRANSFO_SIMPLE_VALUE},           // SEQ! Dende Sec
    {445, 0, 4999, TRANSFO_MSEC},                  // SEQ! msec
    {486, -nbSequencesMax, 64, TRANSFO_NO_VALUE},  // time Dende en 32èmes
    {446, -nbSequencesMax, 36, TRANSFO_NO_VALUE},  // SEQ! divide
    {447, 0, 200, TRANSFO_SIMPLE_VALUE},           // SEQ! drop   !!! différent de Dende Original
    {448, 0, 200, TRANSFO_SIMPLE_VALUE},           // SEQ! down    !!! différent du Dende Original : -100 à 100 -> 0 à 200
    {449, 1, 4, TRANSFO_SIMPLE_VALUE},             // SEQ! levels
    {450, 0, 100, TRANSFO_PERCENT},                // SEQ! decay -> branch
    {451, 0, 36, TRANSFO_SIMPLE_VALUE},            // SEQ! start
    {452, 0, 36, TRANSFO_SIMPLE_VALUE},            // SEQ! end
    {453, 0, 400, TRANSFO_SIMPLE_VALUE},           // SEQ! ratio
    {454, 0, 400, TRANSFO_PERCENT},                // SEQ! stretch
    {455, -24, 24, TRANSFO_SIMPLE_VALUE},          // pitch
    {456, 0, 100, TRANSFO_PERCENT},                // SEQ! fidelity %appear
    {467, 0, 100, TRANSFO_PERCENT},                // SEQ! subFidelity  %happen
    {457, 1, 1000, TRANSFO_SIMPLE_VALUE},          // overlaod
    {458, -99, 200, TRANSFO_SIMPLE_VALUE},         // retard
    {459, 0, 12, TRANSFO_NO_VALUE},                // gamme
    {460, 0, 14, TRANSFO_NO_VALUE},                // mode
    {461, -12, +12, TRANSFO_SIMPLE_VALUE},         // degres
    {462, 1, 12, TRANSFO_SIMPLE_VALUE},            // expand
    {463, -12, 12, TRANSFO_SIMPLE_VALUE},          // arp1
    {464, -12, 12, TRANSFO_SIMPLE_VALUE},          // arp2
    {465, 0, 3, TRANSFO_SIMPLE_VALUE},             // noteOff
    {468, -nbSequencesMax, 255, TRANSFO_NO_VALUE}, // BitMap - en nég sera pour Seq  ! :)  YEAH

    {472, 1, nbLFOMax + 1, TRANSFO_SIMPLE_VALUE},  // choix du LFO actuel
    {389, 0, nbLFOMax - 1, TRANSFO_NO_VALUE},      // LFO ON/OFF
    {489, 0, 2, TRANSFO_NO_VALUE},                 // choix du LFO : 0=Void 1=CC 2=Transfo
    {477, 1, nbLFOMax + 1, TRANSFO_NO_VALUE},      // Replicate LFO
    {479, 0, 10, TRANSFO_NO_VALUE},                // LFO select machine
    {390, 2, 10, TRANSFO_SIMPLE_VALUE},            // LFO Type d'onde 2 = SIN
    {391, 1, 100000, TRANSFO_NO_VALUE},            // LFO Duration msec -> dixièmes de seconde ?
    {478, -nbSequencesMax, 320, TRANSFO_NO_VALUE}, // LFO duration /32
    {393, 1, 16, TRANSFO_CHANNEL},                 // Which Channel
    {392, -nbSequencesMax, 127, TRANSFO_NO_VALUE}, // Choix du CC // modif 2C28 pour seq
    {474, 0, 100, TRANSFO_PERCENT},                // LFO Fidelity
    {550, 0, 127, TRANSFO_SIMPLE_VALUE},           // LFO Random add

    {394, 0, 4096, TRANSFO_SIMPLE_VALUE}, // min Value - 0 à 127 pour l'instant, mais à voir si LFO évolue
    {395, 0, 4096, TRANSFO_SIMPLE_VALUE}, // max Value : idem, 0 à 127
    {475, 1, 120, TRANSFO_SIMPLE_VALUE},  // LFO mod compteur
    {476, 1, 120, TRANSFO_SIMPLE_VALUE},  // LFO mod result

    {473, 0, nbLFOMax, TRANSFO_NO_VALUE}, // résumé LFO

    {430, 0, 0, TRANSFO_NO_VALUE},      // lance le Gabberism
    {431, 1, 10, TRANSFO_SIMPLE_VALUE}, // nombre de levels
    {432, 1, 10, TRANSFO_SIMPLE_VALUE}, // nombre de divisions
    {433, 1, 50, TRANSFO_NO_VALUE},     // ratio qui multiplie les divisions en dixièmes
    {434, 1, 10, TRANSFO_SIMPLE_VALUE}, // nombre de repeats
    {435, 1, 50, TRANSFO_NO_VALUE},     // ratio qui multiplie le nombre de repeats

    /*
NEW STUFF TO DEBUG 
*/

    {420, 0, 0, TRANSFO_NO_VALUE},  // Sort it
    {422, 0, 10, TRANSFO_NO_VALUE}, // Sort : choose mode

    {488, -12, +12, TRANSFO_SIMPLE_VALUE}, // Degree Transposer

    {481, 0, 100, TRANSFO_PERCENT},                 // Twoplets : pourcentage de chance
    {482, -nbSequencesMax, 1000, TRANSFO_NO_VALUE}, // twoplets écart
    {483, -nbSequencesMax, 32, TRANSFO_NO_VALUE},   // twoplets 32èmes
    {484, -nbSequencesMax, 32, TRANSFO_NO_VALUE},   // n-Plets : prend 1 valeur, mais la splitte en n-plets

    {419, 1, 1000, TRANSFO_MSEC},  // Arpegiate : définit quel intervalle il y a aura entre notes arpégiées
    {421, 0, 10000, TRANSFO_MSEC}, // SpanArp : définit quel span initial va être considéré comme chord

    {403, 0, 0, TRANSFO_NO_VALUE},    // Del Notes only
    {404, 0, 0, TRANSFO_NO_VALUE},    // Del CC Only
    {405, 0, 0, TRANSFO_NO_VALUE},    // CC Reflet
    {406, 0, 0, TRANSFO_NO_VALUE},    // CC Mirror
    {407, 0, 100, TRANSFO_PERCENT},   // CC Reduce
    {408, 1, 64, TRANSFO_NO_VALUE},   // CC Multiply
    {409, -36, 36, TRANSFO_DEMITONS}, // CC Addition
    {410, -64, 64, TRANSFO_NO_VALUE}, // CC Grosser

    {411, 0, 0, TRANSFO_NO_VALUE},

    {412, 0, 0, TRANSFO_NO_VALUE},
    {413, 0, 0, TRANSFO_NO_VALUE},
    {414, 0, 0, TRANSFO_NO_VALUE}, // IMPLICIT until ONE max
    {415, 2, 1024, TRANSFO_BEATS}, // implicit max n beats
    {416, 2, 1024, TRANSFO_BEATS}, // implicit précisément n beats

    {417, 2, 1024, TRANSFO_BEATS},  // swingplicit pour max n beats
    {418, 2, 100, TRANSFO_PERCENT}, // swingplicit pour pourcentage

    {402, 0, 12, TRANSFO_NO_VALUE}, // nouveau GAMME select, par layer

    {380, 1, 10000, TRANSFO_MSEC}, // Kompakter (msec)

    {8, 1, 4, TRANSFO_LAYER}, // Equallength

    {388, 1, 32, TRANSFO_BEATS}, // Kompakteur N

    /*
BAROQUE STUFF
*/

    // valeurs liées au Barocco
    {351, 0, 0, TRANSFO_NO_VALUE}, // sélectionne le layer sur lequel le delay s'appliquera
    // TODO réécriture de ce qui chipote dans select Layer dans certaines fonctions
    // TODO DEBUG : peut être dont on aussi pouvoir préciser le channel ?
    {352, 0, nbSequencesMax, TRANSFO_NO_VALUE},     // choix de la séquence
    {353, -nbSequencesMax, 1000, TRANSFO_MSEC_SEQ}, // espacement des notes baroques
    {354, 0, 200, TRANSFO_PERCENT},                 // DROP velocity
    {355, 0, 100, TRANSFO_PERCENT},                 // Random Percent
    {356, 0, 100, TRANSFO_PERCENT},                 // Sym Horizontale 100 = parfait ; 0 = inversé
    {357, 0, 100, TRANSFO_PERCENT},                 // Sym Verticale 100 = parfait ; 0 = inversé
    {358, 0, 100, TRANSFO_PERCENT},                 // Reflet  100 = hors de l'eau ; 0 = inversé par l'horizontale
    {366, 0, 100, TRANSFO_PERCENT},                 // Pre Reflet : avant descente
    {367, 0, 100, TRANSFO_PERCENT},                 // Pre H
    {368, 0, 100, TRANSFO_PERCENT},                 // Pre V

    {381, 0, 0, TRANSFO_NO_VALUE},                      // Kannon ON/OFF
    {382, -36, 10000, TRANSFO_MSEC_SEQ},                // Kannon delay : NEW 2C03 les valeurs négatives sont des séquences
    {383, -36 - nbSequencesMax, +36, TRANSFO_NO_VALUE}, // pitch shift du Kannon
    {569, 0, 200, TRANSFO_SIMPLE_VALUE},                // drop velocité du Kannon
    // valeur sous 36 sont des séquences
    {384, -nbSequencesMax, +50, TRANSFO_NO_VALUE}, // stretch du layer : en dixième de durée, ou 16èmes ?
    // valeur négatives sont des séquences
    {385, 0, 100, TRANSFO_PERCENT},                // Revert : à l'écrevisse
    {386, 0, 100, TRANSFO_PERCENT},                // Reflet : modifie le pitch en fc du plus élevé et plus bas
    {387, 0, 100, TRANSFO_PERCENT},                // Kannon Random happen
    {532, 0, 100, TRANSFO_PERCENT},                // Kannon random appear
    {533, 1, 17, TRANSFO_NO_VALUE},                // CH destination : 1-16 = spécifique 17 idem
    {534, -nbSequencesMax, 255, TRANSFO_NO_VALUE}, // s'inspirer du {334, -nbSequencesMax, 255, TRANSFO_NO_VALUE}, // BitMap - valeur négatives = SEQ

    /*
New 2019 TO SORT
*/

    {369, 0, nbSequencesMax, TRANSFO_NO_VALUE}, // DERAZER : sélectionne Sequence
    {370, 1, 16, TRANSFO_NO_VALUE},             // Choix du Channel pour DERAZER

    {372, 0, 0, TRANSFO_NO_VALUE},  // Save LOCAL 1C99
    {373, 0, 0, TRANSFO_NO_VALUE},  // Load LOCAL 1C99
    {374, 0, 67, TRANSFO_NO_VALUE}, // Création et affichage String374
    {377, 0, 67, TRANSFO_NO_VALUE}, // Génère un nom de fileName THX
    {529, 1, 8, TRANSFO_NO_VALUE},  // génère un nom de fileName TITLES
    {530, 1, 8, TRANSFO_NO_VALUE},  // génère un nom de fileName TITLES

    {507, 0, 0, TRANSFO_NO_VALUE}, // Save Karakt 2C22 - basé sur 372
    {508, 0, 0, TRANSFO_NO_VALUE}, // Load Karakt 2C22 - basé sur 373

    {375, 0, 100, TRANSFO_NO_VALUE}, // OMIT end
    {376, 0, 0, TRANSFO_NO_VALUE},   // affiche la mémoire libre

    {363, 0, 4, TRANSFO_NO_VALUE},  // NEW MARKOV GLOBAL
    {364, 0, 4, TRANSFO_NO_VALUE},  // NEW MARKOV LAYER
    {365, 1, 32, TRANSFO_NO_VALUE}, // envoie des F8 en spécifiant combine de beats contenu dans layer 1

    {359, 1, 16, TRANSFO_NO_VALUE},               // pan automatique sur Channel sélectionné, sur layer actif ON/OFF
    {360, -nbSequencesMax, 60, TRANSFO_NO_VALUE}, // valeur du PAN      négatif = SEQ
    {361, 1, 16, TRANSFO_NO_VALUE},               // pitch automatique sur Channel sélectionné, sur layer actif ON/OFF
    {362, -nbSequencesMax, 60, TRANSFO_NO_VALUE}, // valeur du PPITCH   négatif = SEQ !

    /*
OTHER 
*/

    {111, 0, 0, TRANSFO_NO_VALUE}, // simplement message rotatif marrant

    {220, 0, 0, TRANSFO_NO_VALUE}, // SOFT PASS // plus logique avant midi ox car c'est l'oreille qui compte, pas l'oeil
    {5, 0, 0, TRANSFO_NO_VALUE},   // Delete
    {201, 0, 0, TRANSFO_NO_VALUE}, // MIDI THRU
    {32, 1, 2, TRANSFO_NO_VALUE},  // FIRST FREE NOTE = DEBUG
    {306, 0, 0, TRANSFO_NO_VALUE}, // SOFT MUTE

    {112, 0, 0, TRANSFO_NO_VALUE},               // AutoHarmonize ON/OFF
    {41, 0, 12, TRANSFO_NO_VALUE},               // Select SCALE TONE       // not much used   !!!402
    {251, 0, nbDeModesConnus, TRANSFO_NO_VALUE}, // Select MODE without Harmonize
    {542, 0, 0, TRANSFO_NO_VALUE},               // affichage du Piano Roll 2C26
    {40, 0, 12, TRANSFO_NO_VALUE},               // HARMONIZE

    {349, 0, 100, TRANSFO_NO_VALUE},    // Dynamic Swing
    {154, 0, 100, TRANSFO_PERCENT},     // Swing %
    {155, 1, 33, TRANSFO_SIMPLE_VALUE}, // n Swing
    {156, 0, 13, TRANSFO_NO_VALUE},     // pattern Swing
    {159, 0, 7, TRANSFO_NO_VALUE},      // customize pattern Swing

    {113, 0, 0, TRANSFO_NO_VALUE},         // complexSqueeze ON/OFF
    {94, 0, 0, TRANSFO_NO_VALUE},          // Set SQUEEZE ON
    {95, 0, 0, TRANSFO_NO_VALUE},          // Set SQUEEZE KEEP LENGTH ON
    {250, 0, 0, TRANSFO_NO_VALUE},         // Set TAP ACCOU on
    {42, 0, 2, TRANSFO_NO_VALUE},          // RECORD MODE
                                           // 2C17 {223, 10, 2000, TRANSFO_NO_VALUE},     // Sentence REC : le nombre indique le nb de msec à laisser avant début première note et après début de dernière note
    {224, -1000, +1000, TRANSFO_NO_VALUE}, // adjust END
    {225, -1000, +1000, TRANSFO_NO_VALUE}, // adjust START

    {19, 01, 126, TRANSFO_NomDeNOTE},     // Sketch
    {59, 10, 1000, TRANSFO_MSEC},         // LG Not
    {4, -12, 12, TRANSFO_DEMITONS},       // Transpose
    {551, -60, 60, TRANSFO_SIMPLE_VALUE}, // Transpose CC

    {157, 0, 1000, TRANSFO_MSEC},  // Fuse ( en msec )
    {158, 0, 1, TRANSFO_NO_VALUE}, // >Type Fuse

    {96, 1, 33, TRANSFO_BEATS},          // GRID
    {97, 0, 0, TRANSFO_NO_VALUE},        // CREATE EUCLIDIAN RHYTHM
    {98, 1, 127, TRANSFO_NomDeNOTE},     // Set NOTE euclidian
    {99, 1, 32, TRANSFO_BEATS},          // Set k pulse
    {100, 2, 32, TRANSFO_BEATS},         // Set m size
    {101, 10, 1000, TRANSFO_MSEC},       // Set Euclid Length
    {102, -32, 32, TRANSFO_BEATS},       // Set Euclid Shift in the necklace
    {103, 1, 127, TRANSFO_SIMPLE_VALUE}, // Set Vélocité
    {222, 0, 0, TRANSFO_NO_VALUE},       // FIT EUCLIDIAN : va adapter le layer actuel à une structure euclidienne
    {339, 0, 0, TRANSFO_NO_VALUE},       // BUGGY ou Broken Euclidian : version antérieure, foireuse mais intéressante !
    {54, 1, 100, TRANSFO_PERCENT},       // SCRAMBLE

    /*
FUNKY
*/

    {58, 0, 100, TRANSFO_NO_VALUE}, // FUNKY VELocity
    {45, 5, 200, B0010},            // Varispeed
    {11, 1, 49, TRANSFO_NO_VALUE},  // mulTIply
    {12, 1, 49, TRANSFO_NO_VALUE},  // MELtiply
    {63, 1, 100, TRANSFO_NO_VALUE}, // REPLICATE FRACTIONARY
    {47, 0, 100, TRANSFO_PERCENT},  // Fidelity

    {121, -180, 180, TRANSFO_NO_VALUE}, // Rotate Melody
    {122, 0, 100, TRANSFO_PERCENT},     // Rotate Melody - % X
    {123, 0, 127, TRANSFO_NomDeNOTE},   // Rotate Melody - pitch Y -> NOTE**** transformer x Notes en Note Y
    // {124,0,0,TRANSFO_NO_VALUE},       // Rotate : affichage (X,Y) -> NOTE**** à incorporer dans affichage 121

    {13, 1, 16, TRANSFO_CHANNEL}, // Channel Changer
    // NOTE soucis pour ce qui concerne Channel : il faut définir une fois pour toutes
    // si on parle en terme de CH codé ou de CHANNEL réel.
    // mieux vaut parler en terme de CHANNEL 0 à 15 à priori !
    // en effet : l'affichage est USER et l'encodage est MACHINE !
    {31, 0, 15, TRANSFO_NO_VALUE}, // ACTIVE LAYER CHOOSER

    {20, 0, 0, B0110}, // (un)MUTE

    {7, 1, 4, TRANSFO_LAYER},   // Copy From
    {27, 1, 4, TRANSFO_LAYER},  // COPY\CH   : copie mais force la note dans le nouveau channel
    {182, 1, 4, TRANSFO_LAYER}, // Replicate = copy + layer Length

    {28, 1, 4, TRANSFO_LAYER},    // ADD
    {29, 1, 4, TRANSFO_LAYER},    // ADD\CH
    {61, 0, 0, TRANSFO_NO_VALUE}, // ECARTELE
    {1, 1, 32, TRANSFO_NbNOTES},  //Continuous random melody

    {9, -1000, 1000, TRANSFO_MSEC}, // deplace

    {10, 1, 100, TRANSFO_PERCENT}, // decimate
    {53, 10, 300, TRANSFO_MSEC},   // SLICER HORIZONTAL

    {56, 1, 100, TRANSFO_PERCENT}, // CONCAT100
    {57, 1, 100, TRANSFO_PERCENT}, // EXCAT100

    {16, 1, 100, TRANSFO_PERCENT},  // Populate
    {17, 1, 100, TRANSFO_PERCENT},  // flip loc
    {49, 1, 100, TRANSFO_PERCENT},  // flip not
    {21, 0, 100, TRANSFO_PERCENT},  // REVERSE RND note
    {18, 1, 100, TRANSFO_PERCENT},  // Golden Ratio
    {22, 1, 40, TRANSFO_PERCENT},   //PFunk
    {43, 5, 1000, TRANSFO_MSEC},    // PING PONG
    {60, 5, 1000, TRANSFO_MSEC},    // WOBBLY DELAY
    {23, 1, 600, TRANSFO_NO_VALUE}, // AutoFX

    {33, 0, 100, TRANSFO_PERCENT}, // SCHIZOPHRENIA
    {34, 0, 100, TRANSFO_PERCENT}, // SCZ 2

    {35, 0, 100, TRANSFO_NO_VALUE},  // MIRROR
    {277, 0, 100, TRANSFO_NO_VALUE}, // MIRROR Extended
    {36, 36, 84, TRANSFO_NomDeNOTE}, // REFLET
    {37, -11, 12, TRANSFO_PERCENT},  // SHRINK EXPAND de PRECISION
    {117, -100, 100, TRANSFO_MSEC},  // PLUSMIN 100 = SHRINK EXPAND absolu (msec)

    // {119, 2, 32, TRANSFO_SIMPLE_VALUE}    // Phasage 1/n
    // {120, -32, 32, TRANSFO_SIMPLE_VALUE}   // SET k Phasage

    /*
PHAS
*/

    {119, -32, 32, TRANSFO_NO_VALUE}, // Phasage k/-
    {120, -32, 32, TRANSFO_NO_VALUE}, // SET -/n Phasage

    {38, 1, 2, TRANSFO_NO_VALUE},   // REVERT FULL
    {39, 1, 160, TRANSFO_NO_VALUE}, // VELOCITY FIX

    {6, 1, 5, B0100},                 // Create C60
    {338, 0, 127, TRANSFO_NomDeNOTE}, // valeur de la note qui est créée.

    {15, 1, 5, TRANSFO_LAYER},     // MACRO Poly Rythm
    {50, 1, 9, TRANSFO_NO_VALUE},  // TABULA RASA
    {51, 1, 18, TRANSFO_NO_VALUE}, // THRU WHERE
    {52, 0, 1, TRANSFO_NO_VALUE},  // ND-4 HORSE REDUCE

    {165, 10, 1000, TRANSFO_MSEC}, // Affichage LG note MIN
    {166, 20, 3000, TRANSFO_MSEC}, // Affichage LG note MIN

    {168, 0, 127, TRANSFO_NomDeNOTE}, // Static LO Cut
    {169, 0, 127, TRANSFO_NomDeNOTE}, // Static HI Cut
    {170, 0, 127, TRANSFO_NomDeNOTE}, // Dynamic LO Cut
    {171, 0, 127, TRANSFO_NomDeNOTE}, // Dynamic HI Cut

    {172, 0, 0, TRANSFO_NO_VALUE}, // Send CC to TX81Z : affichage ON/OFF pas encore éclairci NOTE ****

    {173, 0, 0, TRANSFO_NO_VALUE}, // CUT --] end
    {174, 0, 0, TRANSFO_NO_VALUE}, // CUT [-- start

    {180, 0, 1000, TRANSFO_MSEC},  // Breathe
    {181, 0, 2, TRANSFO_NO_VALUE}, // Type Breathe
    {182, 1, 4, TRANSFO_LAYER},    // Replicate = copy + layer Length
    {183, 2, 1000, TRANSFO_MSEC},  // Disjoint, décale les layers d'un random max de msec

    {73, 0, 0, TRANSFO_NO_VALUE},  // SORT NOTES by order ( devrait être imperceptible )
    {74, 0, 0, TRANSFO_NO_VALUE},  // SORT NOTES by pitch
    {55, 1, 100, TRANSFO_PERCENT}, // BASSLINE creator
    {75, 1, 2, TRANSFO_NO_VALUE},  // TX81Z Randomizer
    {76, 1, 100, TRANSFO_PERCENT}, // SPLIT : dédouble un % des notes à courte distance
    {77, 1, 100, TRANSFO_PERCENT}, // RndUp : change le pitch d'un % de notes, d'une valeur allant de 1 à RndUpScope
    {78, 0, 12, TRANSFO_NbNOTES},  // RndUpScope : définit la taille du changment max possible de pitch par RndUp
    {79, 1, 100, TRANSFO_PERCENT}, // Deplace% : défini un % de notes à déplacer selon DeplaceScope (rnd)
    {80, 0, 2000, TRANSFO_MSEC},   // DeplaceScope : définit la taille de la modification de déplacement
    {83, 0, 0, TRANSFO_NO_VALUE},  // DaubyModification : si ON effectue des modification de TX81Z à chaque itération - sera plus tard remplacé par des EventLayers ?
    {85, 0, 0, TRANSFO_NO_VALUE},  // Standart Setup for TX81Z
    {86, 1, 32, TRANSFO_NbNOTES},  // Define midiLongueurNotes

    {104, 1, 16, TRANSFO_CHANNEL}, // SEND CH 2 SAMPL
    {105, 1, 16, TRANSFO_CHANNEL}, // MUTE NOTE CH
    {106, 1, 9, TRANSFO_NO_VALUE}, // GATED MULTIPLE - la seconde valeur est changée par transfo 107
    {107, 2, 8, TRANSFO_NO_VALUE},

    {114, 1, 4, TRANSFO_NO_VALUE}, // Clean Up    1 = DELETE 2 = WRAP  3 = LIMIT   4 = BOUNCE

    {116, 1, 2, TRANSFO_NO_VALUE}, // TRANSFO SYMETRIC

    {202, 1, 126, TRANSFO_NomDeNOTE}, // play Note ( sans Rec )
    {203, 1, 16, TRANSFO_CHANNEL},    // select Channel for play note : ici on considère CHANNEL en tant que chiffre codé
    {30, 1, 2, TRANSFO_NO_VALUE},     // EYE CANDY
    {26, 1, 100, TRANSFO_NO_VALUE},   // SPEED

    {217, 0, 127, TRANSFO_NO_VALUE}, // Filter / Kill Note

    {14, 0, 0, TRANSFO_NO_VALUE},  // CH+time indicateur
    {246, 0, 0, TRANSFO_NO_VALUE}, // layer Counters
    {46, 0, 0, TRANSFO_NO_VALUE},  // CH+ratio indicateur
    {48, 0, 0, TRANSFO_NO_VALUE},  // CH+fidelity indicateur

    {490, 0, 0, TRANSFO_NO_VALUE}, // Primer : adapte la durée pour avoir premier entre eux
    {497, 1, 10000, TRANSFO_MSEC}, // Primer2 : idem, mais on peut ajuster la base, pas juste 1 msec

    /*
MAPPINGS
 */
    {108, 60, 80, TRANSFO_NO_VALUE},  // VERTICAL MAP HI-LO   les valeurs dans le tableau sont les valeurs target !!!
    {110, 1, 127, TRANSFO_NomDeNOTE}, // Set Target MIN
    {109, 1, 127, TRANSFO_NomDeNOTE}, // Set Target MAX

    {179, 0, 0, TRANSFO_NO_VALUE}, // Horizontal Mapping
    {178, 0, 5000, TRANSFO_MSEC},  // // SET Horizontal Map Source MIN
    {177, 0, 5000, TRANSFO_MSEC},  // // SET Horizontal Map Source MAX
    {176, 0, 5000, TRANSFO_MSEC},  // // SET Horizontal Map Target MIN
    {175, 0, 5000, TRANSFO_MSEC},  // // SET Horizontal Map Target MAX

    {213, 0, 0, TRANSFO_NO_VALUE},       // Vertical Map for velocité
    {214, 0, 127, TRANSFO_SIMPLE_VALUE}, // velocité min
    {215, 0, 127, TRANSFO_SIMPLE_VALUE}, // velocité max
    // TODO réduire à encodage successif

    /*
DELAYS
 */

    {216, 0, 1000, TRANSFO_NO_VALUE}, // valeur du Poof ( deviation random )

    {398, 0, 0, TRANSFO_NO_VALUE},    // Drunk ON/OFF
    {399, -1000, 1000, TRANSFO_MSEC}, // Min
    {400, -1000, 1000, TRANSFO_MSEC}, // Max
    {401, 0, 1000, TRANSFO_MSEC},     // step

    {205, 0, 1000, TRANSFO_MSEC},          // dureePole1
    {206, -10, +10, TRANSFO_SIMPLE_VALUE}, // min POle1
    {207, -10, +10, TRANSFO_SIMPLE_VALUE}, // max POle1
    {208, 0, 1000, TRANSFO_MSEC},          // dureePole2
    {209, -10, +10, TRANSFO_SIMPLE_VALUE}, // min POle2
    {210, -10, +10, TRANSFO_SIMPLE_VALUE}, // max POle2
    {211, 0, 100, TRANSFO_PERCENT},        // freq Pole 1
    {212, 0, 0, TRANSFO_NO_VALUE},         // Turn On Bipolar RAndomization

    /*
GENERATORS
*/

    {64, 0, 0, TRANSFO_NO_VALUE},         // CREATE LADDER with the following values (if not defined, preset )
    {65, 1, 126, TRANSFO_NomDeNOTE},      // CREATE LADDER DEFINE Start Note
    {66, 1, 32, TRANSFO_SIMPLE_VALUE},    // CREATE LADDER DEFINE Nb Steps
    {67, 1, 1000, TRANSFO_MSEC},          // CREATE LADDER DEFINE Length
    {68, 1, 1000, TRANSFO_MSEC},          // CREATE LADDER DEFINE Pause
    {69, 1, 1000, TRANSFO_MSEC},          // CREATE LADDER DEFINE Inc Length
    {70, 1, 1000, TRANSFO_MSEC},          // CREATE LADDER DEFINE Inc Pause
    {71, -13, +13, TRANSFO_SIMPLE_VALUE}, // CREATE LADDER DEFINE Step Size ( en demi-tons )
    {72, -13, +13, TRANSFO_SIMPLE_VALUE}, // CREATE LADDER DEFINE Step inc Size

    {2, 1, 13, TRANSFO_BEATS},    // HAMMER
    {3, 1, 13, TRANSFO_BEATS},    // Hammer Funk
    {44, 2, 32, TRANSFO_NbNOTES}, // MORSE

    /*
EVENTS
*/

    // NOTE **** réfléchir à l'ordre des transfos qui suivent :
    // l'idéal serait de n'avoir à parcourir la liste que dans un seul sens
    // jusqu'à présent on commmence en fait par un sens vers la droite avant de remonter au launch
    // est-ce pratique ?
    //
    {127, 0, NbEvenementMax - 1, TRANSFO_NO_VALUE}, // SELECT EVENT - DEFINE .i -    // NOTE **** si pas défini, travail sur NEW .  CurrentEvenement = 0 qd créé
    // {128,0,NbEvenementMax,TRANSFO_NO_VALUE},    // 128 DELETE Event : initialise un event - à utiliser en début de programme pour tout réinitialiser
    // Devra afficher CurrentEvenement ou les autres
    {125, 0, NbEvenementMax - 1, TRANSFO_NO_VALUE}, // ACTIVATE Evenement
    {500, 0, 0, TRANSFO_NO_VALUE},                  // View Active Events
    {498, 0, NbEvenementMax - 1, TRANSFO_NO_VALUE}, // Résumé de l'Event - basé sur 473
    {491, 0, 3, TRANSFO_NO_VALUE},                  // Gestion des evenements : stopp all, start all, rnd, invert

    {128, 0, 0, TRANSFO_NO_VALUE}, // DELETE Evenement : le CurrentEvenement dans un premier temps !

    {129, 1, 2, TRANSFO_NO_VALUE}, // DEFINE Type
    // 129 DEFINE Type
    // byte type ; // 0 = Vide ( il faut initialiser ! )   1 = CC
    // 2 = Transformation
    // (3) = Modification de valeur ( peut être intégrer à transformation ! )
    {126, 0, 127, TRANSFO_NO_VALUE}, // DEFINE .which CC or Transfo
    {133, 1, 4, TRANSFO_NO_VALUE},   // 133 DEFINE TypeInterval
    // byte typeInterval ; // 1 = constant 2 = random(a,b)  3=gaussian   4 = layer modulo    5 bi polar
    // NOTE **** à implémenter
    // Si 1 : OK pour .intervalle et .moment
    // Si 2 : valeur Min, valeur Max : Valeur Max = Intervalle , Valeur Min = Moment
    // Si 3 : Valeur Min = distrib 0 ; Valeur max = distrib totale
    // Si 4 : Layer Modulo
    //
    {270, 0, 25, TRANSFO_NO_VALUE},   // 270 MODULO Layer pour Event
    {130, 1, 1800, TRANSFO_NO_VALUE}, // 130 DEFINE Intervalle
    // unsigned int intervalle ; // exprimé en 0.1 secondes  10 = 1 seconde 100 = 10 secondes 1000 = 100 secondes 10000 = quasi 1 h
    {131, 0, 1800, TRANSFO_NO_VALUE}, // 131 DEFINE Moment, durant l'intervalle
    // unsigned int moment ; // // dans les temps divisible par 300, agit après 100
    {132, 0, 100, TRANSFO_NO_VALUE}, // 132 DEFINE fidelite
    // byte fidelite ; // en %

    {134, 0, 127, TRANSFO_NO_VALUE}, // 134 DEFINE Value
    // int valeur ;
    // pour CC : 0-127
    // pour Transfo : 0-1023, affichée comme pour l'affichage normal
    //  1---- SELECT VALUE FOR TRANSFO(42)=Rotation
    //  30°               <- affichage comme pour Rotation
    //
    {137, 0, 127, TRANSFO_NO_VALUE}, // 137 DEFINE RangeLO
    // byte rangeLO ; // pour option 2,3,4
    {136, 0, 127, TRANSFO_NO_VALUE}, // 136 DEFINE RangeUP
    // byte rangeUP ; // pour option 2,3,4
    {138, 0, 24, TRANSFO_SIMPLE_VALUE}, // 138 DEFINE incSize
    // byte incSize ; // définit de combien la valeur en option 2 et 3 ( qui donnent le sens)
    {135, 1, 5, TRANSFO_NO_VALUE}, // 135 DEFINE SHAPE
    // byte shape ;  // 1 = valeur fixe ( pour valeur )
    // 2 = INC
    // 3 = DEC ( ou bien INC avec valeur négative ? )
    // 4 = RND  ( définir min et max, 0-127 par défaut pour CC )
    // 5 = SEQ = LI5STE : ex 4 valeurs 37,69,120,130
    //  5=liste sequential 6 = liste random   TODO DEBUG
    // // NOTE**** il faudra pour la liste réapprendre les pointeurs; j'en aurai de toutes façons besoins pour passer
    // des fragments (bars) à des morceaux.

    {305, 0, nbSequencesMax - 1, TRANSFO_NO_VALUE}, // Définit Séquence servant de liste

    {139, 1, 3, TRANSFO_NO_VALUE}, // 139 DEFINE behaviorLimit
    // byte behaviorLimit ; // pour options shape 2,3
    // 1 = Change direction
    // 2 = qd dépasse vers haut ou bas, réapparait de l'autre côté
    // 3 = reste à la limite
    {140, 1, 16, TRANSFO_NO_VALUE}, // 140 DEFINE channel
    // byte channel ; // channel concerné 0-15
    // Définit sur quel channel on envoie le CC !
    {141, 0, 16, TRANSFO_NO_VALUE}, // 141 DEFINE Layer
    // byte layer ; // layer concerné 1-4 ou combinaison ou random ; ou bien layers actives.   par défaut : Actif ; sinon spécifique
    // définit sur quel layer va agir
    // // B11110000 signifie : agir sur les Active Layers

    // on peut imaginer faire un edit de valeur d'Event
    // EDIT for Event(i) : CC     ****NOTE SUPER IMPORTANT
    //
    // on peut imaginer que la machine fasse un LOG dans sa mémoire pour voir jusqu'où elle a pu aller avant de planter
    // EVOLUTIONARY MACHINES
    //
    {261, 0, 0, TRANSFO_NO_VALUE},   // Condition transfo : ON/OFF
    {262, 1, 32, TRANSFO_NO_VALUE},  // Conditino : which Test
    {269, 0, 16, TRANSFO_NO_VALUE},  // Set combinaison de layers auxquelles la sequence s'applique
    {263, 1, 5, TRANSFO_NO_VALUE},   // Condition : which opération sur la valeur test
    {264, 0, 100, TRANSFO_NO_VALUE}, // Condition : valeur qui complète l'opération en 263
    {265, 1, 4, TRANSFO_NO_VALUE},   // Condition : which comparison is applied : >< = !=
    {266, 0, 100, TRANSFO_NO_VALUE}, // Condition : valeur de référence
    {267, 1, 2, TRANSFO_NO_VALUE},   // Condition : DO ou BLOCK
    {268, 1, 32, TRANSFO_NO_VALUE},  // condition : affiche Test Values
    {204, 0, 4, 0100},               // affichage moment prochaine note   // DEBUG doit être supprimé à terme

    {278, 0, 1000, TRANSFO_NO_VALUE}, // Set Compteur d'Event
    {279, 0, 1000, TRANSFO_NO_VALUE}, // Set Latence
    {280, 0, 1000, TRANSFO_NO_VALUE}, // Set Echeance
    {281, 0, 0, TRANSFO_NO_VALUE},    // Switch Echeance ON/OFF

    {274, 0, NbEvenementMax - 1, TRANSFO_NO_VALUE}, // Replicate EVENT
    {275, 0, 0, TRANSFO_NO_VALUE},                  // Suspend All EVENTs

    /*
LOAD SAVE
*/

    {81, 0, 0, TRANSFO_NO_VALUE}, // SAVE TXT
    {82, 0, 0, TRANSFO_NO_VALUE}, // LOAD TXT
                                  //  {84, 0, 0, TRANSFO_NO_VALUE}, // LOAD & PLAY MIDI  // STOP 2C27

    {184, 0, 0, TRANSFO_NO_VALUE}, // SAVE all Events (version primitivo : sauve de 1 à 10 )
    {282, 0, 0, TRANSFO_NO_VALUE}, /// LOAD all Events ( version primitivo : charge tous les data d'un fichier )

    /*
MIDI FILES
*/

    {167, 0, 0, TRANSFO_NO_VALUE}, // Reset MIDI parallel original values

    {93, 0, 0, TRANSFO_NO_VALUE},  // Define MIDI File + calcul taille
    {549, 0, 0, TRANSFO_NO_VALUE}, // info complètes sur fichier midi
    {115, 0, 0, TRANSFO_NO_VALUE}, // Parallel MIDI ON/OFF
    {562, 0, 0, TRANSFO_NO_VALUE}, // Skip Midi Note
    {164, 0, 0, TRANSFO_NO_VALUE}, // Skip Chunck pour Parallel Midi Player

    // NOTE **** problème avec le fait que le display donne un midiname, qui est ensuite exploité
    // par le parallel midi ON : il faut entrer un nom quand on fait OK, pas pour display simplement

    {151, 1, 1000, TRANSFO_NO_VALUE},     // GLITCH NumRatioDeltaToMsec
    {152, 1, 1000, TRANSFO_NO_VALUE},     // GLITCH DenomRatioDeltaToMsec
    {560, 1, 20000, TRANSFO_NO_VALUE},    // GLITCH NumRatioDeltaToMsec DenomRatioDeltaToMsec RatioDeltaToMsec
    {563, -7, 7, TRANSFO_NO_VALUE},       // multiplication de ratio
    {142, 0, 100, TRANSFO_SIMPLE_VALUE},  // GLITCH iVLVDelta
    {143, 0, 100, TRANSFO_SIMPLE_VALUE},  // GLITCH iDureeDelta
    {144, 0, 100, TRANSFO_SIMPLE_VALUE},  // GLITCH imididouble = 0;
    {145, 0, 16, TRANSFO_SIMPLE_VALUE},   // GLITCH iShiftDelta = 8 ;
    {146, 0, 16, TRANSFO_SIMPLE_VALUE},   // GLITCH iShiftDuree = 7 ;
    {147, 0, 255, TRANSFO_SIMPLE_VALUE},  // GLITCH iDureeMask = B01111111   ;
    {148, 0, 255, TRANSFO_SIMPLE_VALUE},  // GLITCH iMask2 = B10000000 ;
    {149, 0, 2000, TRANSFO_SIMPLE_VALUE}, // GLITCH CutOffDureeDelta
    {150, 0, 2000, TRANSFO_SIMPLE_VALUE}, // GLITCH BringToDureeDelta
    {296, 0, 100, TRANSFO_NO_VALUE},      // FUNKY VELocity pour Midi Player

    {160, 1, 16, TRANSFO_CHANNEL},  // Midi matrix Redirect : Source
    {161, 1, 16, TRANSFO_CHANNEL},  // Midi matrix Redirect : Target
    {162, 1, 7, TRANSFO_NO_VALUE},  // Midi Matrix fc spéciales
    {163, 1, 16, TRANSFO_NO_VALUE}, // Affiche la redirection MIDI pour ce channel

    {185, 0, 13, TRANSFO_NomDeNOTE},  // Choix de note pour Fregoli et Cotard
    {186, -24, 25, TRANSFO_NO_VALUE}, // Fregolize : pitch constant d'une note, dynamique
    {187, 0, 0, TRANSFO_NO_VALUE},    // Cotardize : silent une note, dynamique
    {188, 0, 10, TRANSFO_NO_VALUE},   // macro Cotard / FRegoli

    {87, 1, 100, TRANSFO_NbNOTES}, // LOAD MIDI Sequence from Midi file
    // fixe aussi le CurrentMIDIPosition de façon grossière
    // temporairement ici pour DEBUG
    //
    {88, 0, 0, TRANSFO_NO_VALUE}, // LOAD NEXT Sequence from Midi file
    {89, 0, 0, TRANSFO_NO_VALUE}, // LOAD PREVIOUS Sequence from MIDI file
    {90, 0, 0, TRANSFO_NO_VALUE}, // LOAD +1 Sequence
    {91, 0, 0, TRANSFO_NO_VALUE}, // LOAD -1 Sequence
    {92, 0, 0, TRANSFO_NO_VALUE}, // LOAD Random Sequence

    /*
CC MOD
*/

    // SELECT ACTIVATE DELETE MODIFY
    {190, 0, NbCCModMax - 1, TRANSFO_NO_VALUE}, // SET Current CCMod = SELECT
    {200, 0, 0, TRANSFO_NO_VALUE},              // Create CC Mod = ACTIVATE
    {260, 0, 0, TRANSFO_NO_VALUE},              // DELETE CC MOD
    {189, 1, 4, TRANSFO_NO_VALUE},              // SET type CCMod
    {191, 0, 5000, TRANSFO_SIMPLE_VALUE},       // SET SourceMin
    {192, 0, 5000, TRANSFO_SIMPLE_VALUE},       // SET SourceMax
    {193, 0, 127, TRANSFO_SIMPLE_VALUE},        // SET TargetMin
    {194, 0, 127, TRANSFO_SIMPLE_VALUE},        // SET TargetMax
    {195, 0, 5, TRANSFO_NO_VALUE},              // SET Border Behavior
    {196, 0, 127, TRANSFO_SIMPLE_VALUE},        // SET id CC
    {197, 1, 16, TRANSFO_CHANNEL},              // SET channel CC
    {198, 0, 16, TRANSFO_NO_VALUE},             // SET layers    15 = 1111   0 = 0000    16 = ACTIFS
    {199, 0, 1, TRANSFO_NO_VALUE},              // SET send CCMod from MIdi Parallel

    /*
CELL
*/

    {218, 0, 127, B10000},         // CELL : va rediriger un CH N vers un autre CH N
    {219, 1, 16, TRANSFO_CHANNEL}, // Select CHANNEL for CELL.  NOTE remember : Channel affiché pour enduser ! CH1 à CH16
    {221, 0, 0, TRANSFO_NO_VALUE}, // Delete Cell table

    /*
SEQUENCES
*/

    {228, 0, nbSequencesMax - 1, TRANSFO_NO_VALUE}, // Select Current Sequence
    {243, 0, nbSequencesMax - 1, TRANSFO_NO_VALUE}, // Set Actif ou inactif pour current Sequence
    {499, 0, nbSequencesMax - 1, TRANSFO_NO_VALUE}, // Résumé - data d'une séquence
    {501, 0, 0, TRANSFO_NO_VALUE},                  // Affiche les Séquences ACTIVES - !!! est différent de non-vide, p-êt combiner les deux ?
    {227, 0, nbSequencesMax - 1, TRANSFO_NO_VALUE}, // Delete Sequence
    {303, 0, nbSequencesMax - 1, TRANSFO_NO_VALUE}, // Replicate Sequence
    {378, 1, 4, TRANSFO_LAYER},                     // Transfo Layer to Sequence

    {226, -127, +2000, TRANSFO_NO_VALUE}, // Define Sequence.
    // pour le pitch cela doit aller de -24 à +24
    // mais pour velocité il faut pouvoir aller de -127 à +127
    // et pour les CC de 0 à 127
    // pour les durées de notes enfin, il faut aller de 0 à 2000
    // NOTE DEBUG à peaufiner
    {229, 0, nbSequencesMax - 1, TRANSFO_NO_VALUE},       // Erase 1 Digit from Sequence
    {230, 0, 11, TRANSFO_NO_VALUE},                       // Set Sequence Type
    {231, 0, 100, TRANSFO_NO_VALUE},                      // Set jitterMinUP
    {232, 0, 100, TRANSFO_NO_VALUE},                      // Set jitterMaxUP
    {233, 0, 100, TRANSFO_NO_VALUE},                      // Set jitterMinDOWN
    {234, 0, 100, TRANSFO_NO_VALUE},                      // Set jitterMaxDOWN
    {235, 0, 100, TRANSFO_NO_VALUE},                      // Set jitterStaticMin
    {236, 0, 100, TRANSFO_NO_VALUE},                      // Set jitterStaticMax
    {237, 0, tailleMaxTableauSequence, TRANSFO_NO_VALUE}, // Set rnd Frag Size
    {238, 0, 100, TRANSFO_NO_VALUE},                      // Set rnd Frag repeats
    {239, 0, 19, TRANSFO_NO_VALUE},                       // Set ACTION
    {240, 0, 127, TRANSFO_NO_VALUE},                      // Set CC linked to SEquence
    {241, 1, 16, TRANSFO_NO_VALUE},                       // Set Channel for CC linked to SEquence
    {242, 0, 16, TRANSFO_NO_VALUE},                       // Set combinaison de layers auxquelles la sequence s'applique
    {247, 0, nbSequencesMax - 1, TRANSFO_NO_VALUE},       // indique si la Seq s'applique à notes ou à layers au niveau comptage

    /*
DIVERS
*/

    {244, 0, 3, TRANSFO_NO_VALUE},  // choix de layer pour ON OFF DynHarmonize
    {245, 0, 2, TRANSFO_NO_VALUE},  // choix du mode de DynHarmonize
    {248, 0, 25, TRANSFO_NO_VALUE}, // Eclipse Layer ( = modulo sur layer )
                                    //  {249, 0, 0, TRANSFO_NO_VALUE},  // Turn on ou off Chaine de Markov globale
    {252, 0, 0, TRANSFO_NO_VALUE},  // Turn ON ou OFF AutoCleanUp

    {253, 2, 32, TRANSFO_BEATS},        // HASH : la valeur définit le nombre de segments
    {254, 0, 100, TRANSFO_PERCENT},     // % Repeats
    {255, 0, 16, TRANSFO_SIMPLE_VALUE}, // Nb de repeats maximum
    {256, 0, 100, TRANSFO_PERCENT},     // chance de permutation pour un segment
    {257, 0, 100, TRANSFO_PERCENT},     // chance d'inversion pour un segment
    {258, 0, 0, TRANSFO_NO_VALUE},      // ON OFF pour unify après HASH
    {259, 0, 1000, B000101},            // ReUnited
    {271, -24, 24, B0001111},           // SpanDex
    {272, 0, 30, TRANSFO_NO_VALUE},     // SpanFlex
    {273, 0, 0, TRANSFO_NO_VALUE},      // Keep Syncop BUG : réinstalle artificiellement le bug découvert le 27 nov 2018, créant syncopation entre 2 notes identiques disjointes sur des layers de durées différentes .

    // case 278 :NameTransfo = F(">SetCompteur") ; break ;
    // case 279 :NameTransfo = F(">SetLatence") ; break ;
    // case 280 :NameTransfo = F(">SetEcheance") ; break ;
    // case 281 :NameTransfo = F(">SwitchEcheance") ; break ;

    {283, 0, 100, TRANSFO_PERCENT}, // FIdelity Whole Layer
    {284, 0, 0, TRANSFO_NO_VALUE},  // Numerote Transfo
    {285, 0, 0, TRANSFO_NO_VALUE},  // SAVE all sequences
    {286, 0, 0, TRANSFO_NO_VALUE},  // LOAD All Sequences
    {287, 0, 0, TRANSFO_NO_VALUE},  // SAVE all CCMod
    {288, 0, 0, TRANSFO_NO_VALUE},  // LOAD All CCMod
    {289, 0, 0, TRANSFO_NO_VALUE},  // SAVE all Cell
    {290, 0, 0, TRANSFO_NO_VALUE},  // LOAD All Cell

    {153, 0, 67, TRANSFO_NO_VALUE}, // Création et affichage String153
    {291, 0, 0, TRANSFO_NO_VALUE},  // Renomme le fichier choisi selon le nom défini en 153

    {292, 0, 0, TRANSFO_NO_VALUE}, // Remove, supprime, delete le fichier choisi

    {293, -4, +4, TRANSFO_SIMPLE_VALUE}, // Octaver
    {294, 0, 100, TRANSFO_PERCENT},      // % Octaver
    {295, 0, 99, TRANSFO_NO_VALUE},      // Vel Ratio Octaver
                                         // TODO DEBUG : développer un affichage standard pour fractions ?

    {228, 0, nbSequencesMax - 1, TRANSFO_NO_VALUE}, // Select Current Sequence
    {557, 0, 13, TRANSFO_NO_VALUE},                 // Tom Johnson's modifications
    {297, 0, 0, TRANSFO_NO_VALUE},                  // >ShiftR Seq
    {298, 0, 0, TRANSFO_NO_VALUE},                  // >ShiftL Seq
    {299, 0, 0, TRANSFO_NO_VALUE},                  // >Rndmize Seq
    // {300,0,0,0}, // >LogicShift Seq
    {301, 0, 0, TRANSFO_NO_VALUE}, // >InvertV Seq
    {302, 0, 0, TRANSFO_NO_VALUE}, // >RevertH Seq
    // {303,0,0,0}, // >duplicate Seq
    {304, 0, 0, TRANSFO_NO_VALUE},       // >Double Seq
    {300, -100, +100, TRANSFO_NO_VALUE}, // Permute Sequence
    {307, 0, 0, TRANSFO_NO_VALUE},       // LED Mode
    {308, 1, 500, B101},                 // blink time
    {309, 1, 16, TRANSFO_CHANNEL},       // Scalpel CH
    {310, 0, 0, TRANSFO_NO_VALUE},       // BPM Sniffer  TODO DEBUG : faut-il préciser le channel à sniffer ?

    /*
CHORDAL 
*/

    {311, 0, 0, TRANSFO_NO_VALUE},               // Editha : remet de l'ordre dans la suite de notes
    {487, 7, 120, TRANSFO_SIMPLE_VALUE},         // va centrer la mélodie sur 12 demi-tons autour du pitch spécifié
    {312, 0, 0, TRANSFO_NO_VALUE},               // Axe 12 Tones : symétrie liée à gamme + mode
    {321, 0, 0, TRANSFO_NO_VALUE},               // SuperSymmetry
    {313, 0, 12, TRANSFO_NO_VALUE},              // RealTransposerGamme = RealScaleTranspose
    {320, 0, nbDeModesConnus, TRANSFO_NO_VALUE}, // RealModeTranspose
    {314, 0, 0, TRANSFO_NO_VALUE},               // Fix To Closest Random Scale + Mode : limité au 12 premières valeurs, car Messiaen et Dodeca = no sense
    {315, 0, 0, TRANSFO_NO_VALUE},               // Random All Triad Change
    {316, 2, 5, TRANSFO_SIMPLE_VALUE},           // One Random Triad Change ( contiguous )
    {317, 2, 5, TRANSFO_SIMPLE_VALUE},           // One Spaced Triad Change ( non contiguous )
    {318, 0, 0, TRANSFO_NO_VALUE},               // UnFlatten Triads
    {319, 0, 0, TRANSFO_NO_VALUE},               // put it back around C60
    {322, 0, 0, TRANSFO_NO_VALUE},               // COMB
    {323, 0, 12, TRANSFO_SIMPLE_VALUE},          // COMB2

    {324, 0, 100, TRANSFO_SIMPLE_VALUE}, // inspector Note
    {325, 0, 100, TRANSFO_SIMPLE_VALUE}, // delete note from pile

    {326, 1, 16, TRANSFO_NO_VALUE}, // Select channel for which Velocity will -> Volume change ( MicroSampler )
    {39, 1, 160, TRANSFO_NO_VALUE}, // VELOCITY FIX

    /*
DYNAMIC DELAY 
*/

    // valeurs liées au Delay dynamique
    {327, 1, 4, TRANSFO_NO_VALUE}, // sélectionne le layer sur lequel le delay s'appliquera
    // TODO DEBUG : peut être dont on aussi pouvoir préciser le channel ?
    {328, -nbSequencesMax, 20, TRANSFO_NO_VALUE},   // nombre de délais - négatif = SEQ
    {329, -nbSequencesMax, 1000, TRANSFO_MSEC_SEQ}, // espacement des délais
    {330, 0, 200, TRANSFO_PERCENT},                 // DROP velocity
    {331, -nbSequencesMax, 200, TRANSFO_NO_VALUE},  // DOWN Velocity
    {332, 0, 100, TRANSFO_PERCENT},                 // Random Percent
    {568, 0, 100, TRANSFO_PERCENT},                 // Random Appear pour Delay

    {333, -nbSequencesMax, 128, TRANSFO_NO_VALUE}, // Repeater - valeur négatives = SEQ
    {334, -nbSequencesMax, 255, TRANSFO_NO_VALUE}, // BitMap - valeur négatives = SEQ
    {335, 1, 1000, TRANSFO_SIMPLE_VALUE},          // Zat

    {336, 1, 4, TRANSFO_NO_VALUE},    // choix de suppression de notes qd overflow
    {337, 0, 10000, TRANSFO_NbNOTES}, // choix du nombre de notes avant overflow

    /*
NEW SAVE LOAD 2019 
*/

    {340, 1, 64, TRANSFO_NO_VALUE}, // Sauve de Layer vers Mémoire
    {341, 1, 74, TRANSFO_NO_VALUE}, // Load de Mémoire vers Layer 1-64 = MEM ; 65-74 = FLUX
    {536, 0, 0, TRANSFO_NO_VALUE},  // affichage first free Mem
    {538, 0, 0, TRANSFO_NO_VALUE},  // Quick Save to first free Mem
    {540, 0, 0, TRANSFO_NO_VALUE},  // Previous Load Mem

    {509, 1, 64, TRANSFO_NO_VALUE}, // Sauve Karakt vers K_Mémoire
    {510, 1, 64, TRANSFO_NO_VALUE}, // Charge Karkt à partir de la mémoire
    {511, 1, 64, TRANSFO_NO_VALUE}, // efface une mémoire
    {537, 0, 0, TRANSFO_NO_VALUE},  // affichage first free Karakt
    {539, 0, 0, TRANSFO_NO_VALUE},  // Quick Save to first free Mem

    {543, 0, 0, TRANSFO_NO_VALUE},  // Dialyse : sauve dans first free mem chaque note séparément
    {556, 1, 64, TRANSFO_NO_VALUE}, // Merge ADD

    {561, 1, 100, TRANSFO_PERCENT}, // Conway Spread %

    {443, 1, 64, TRANSFO_NO_VALUE}, // cut Slice de la mémoire vers Layer  2C16
    {469, 1, 64, TRANSFO_NO_VALUE}, // add Slice de la mémoire vers Layer 2C17
    {442, 1, 12, TRANSFO_NO_VALUE}, // définit la taille du cutSlice  2C16
    {518, 1, 36, TRANSFO_NO_VALUE}, // définit la taille du K cutSlice  2C23

    {371, 2, 32, TRANSFO_SIMPLE_VALUE}, // Multiplier Simple - doublon, se trouve ailleurs aussi, mais utile ici !
    {63, 1, 100, TRANSFO_NO_VALUE},     // REPLICATE FRACTIONARY

    {470, 1, 16, TRANSFO_CHANNEL}, // Kill CH
    {471, 1, 16, TRANSFO_CHANNEL}, // Kill All\CH

    {342, 1, 64, TRANSFO_NO_VALUE},      // Transfert entre 2 mémoires
    {343, 1, 64, TRANSFO_SIMPLE_VALUE},  // Set mémoire Source ( qui sera transférée )
    {344, 0, 0, TRANSFO_NO_VALUE},       // Sauver vers UNDO
    {345, 0, 0, TRANSFO_NO_VALUE},       // Recharge de l'UNDO
    {346, 1, nbLayer, TRANSFO_NO_VALUE}, // select Pitchable Layer
    {347, 1, 16, TRANSFO_NO_VALUE},       // select Pitchable Channel

    // BLACK06
    {578, 0,0, TRANSFO_NO_VALUE},        // affichage des boutons appuyés DEBUG simple
{579,0,0,TRANSFO_NO_VALUE}              // charge Karakt previous précédent 

};

// qd on est au-dessus de la première transfo d'une catégorie, on envoie à la suivante
const int jump[] = {24, 424, 466, 472, 420, 351, 372, 111, 58, 119, 108, 64, 127, 81, 167, 190, 218, 228, 244, 311, 327, 340, 24};
// ceci indique la succession des numéros de transfo dans le TransfoArray
// qd on fait un jump, on saute d'une transfo à la précédente dans cette liste
// qd on modifie le tableau array, et qu'on change les catégories on doit changer ceci aussi !

const int tailleJump = (sizeof(jump) / sizeof(int)) - 1; // BLACK06 :) 23 juillet 2021, rue Hespel

// TODO DEBUG remplacer les chiffres ésotériques dans ce tableau par des constantes
// _MSEC _PCENT _LAYER _NOTES etc
// le bit 1 (droite) décrit si est affichage NL ( min, max, symbole )
// les bits 2, 3 et 4 (gauche) décrivent le type de symbole
// 001 = %   010 = msec   011 = Layer   100 = Channel    101 = notes    110 = beats   111 = /2 tons
// 1000 = la note xxx ( NOTE addition avril 2018 )
// 1001 = ON/OFF
//
// ****NOTE  il faut augmenter les possibilités : Note xx    ON/Off
