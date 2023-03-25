// on inclut ici la méga fonction Save Local
// puis load Local
// les load partiel doivent être inclus dans Load Local
// 1 seule page pour ces deux fonctions centrales et complexes

// QUAND ON RAJOUTE DES VARIABLES Qu'on VEUT SAUVER DANS KARAKT
// PROCEDURE A SUIVRE
// aller dans saveCurrentandMemKarakt()
// on rajoute en bas de cette fonction des _printToKarakt avec des chiffres consécutifs, ainsi que le nom de variable et description
// faire un compile pour vérifier que OK
// un peu plus bas, dans la même fonction, recopier les nouvelles lignes et modifier en fromKMem_int_printToKarakt
// re-compiler
// aller dans loadLocal
// ajouters les nouvelles lignes et modifier en    int_loadFromKarakt(128, &keepStepzBUG );
// re-compiler
// aller ensuite dans le fichier karaktPile.h
// trouver la fonction saveKarakt_to_KMem
// y recopier les lignes juste modifée et en changeant le début par econome_int_saveToKMem(place,
// re-compiler
// recopier ces mêmes lignes EXACTEMENT sans rien modifer dans la fonction loadKarakt_from_KMem 
// re-compiler 

void initializeAllSequences(void);
// prototype de fc qui se trouve dans SubParts.h
void saveCurrentandMemKarakt(bool saveKarakt   , bool includeHeader   ) ; 



void byte_loadFromKaraktDISMISS(int number, int layer, byte *here)
{
    if (karaktNumber == number && karaktLayer == layer)
    {
        *here = (byte)karaktValue;
    }
}

void int_loadFromKarakt(int number, int *here)
{
    if (karaktNumber == number)
    {
        *here = (int)karaktValue;
    }
}

void multi_int_loadFromKarakt(int number, int *here)
{
    if (karaktNumber == number)
    {
        // *here[karaktLayer] = (int)karaktValue;
        // TODO DEBUG
        *(here + karaktLayer) = (int)karaktValue;
    }
}

void bool_loadFromKarakt(int number, bool *here)
{
    if (karaktNumber == number)
    {
        *here = (bool)karaktValue;
    }
}

void multi_bool_loadFromKarakt(int number, bool *here)
{
    if (karaktNumber == number)
    {
        // *here[karaktLayer] = (bool)karaktValue;
        // TODO DEBUG
        *(here + karaktLayer) = (bool)karaktValue;
    }
}


void single_bool_loadFromKarakt(int number, int layer, bool *here)
{
    if (karaktNumber == number && karaktLayer == layer )
    {
        // *here[karaktLayer] = (bool)karaktValue;
        // TODO DEBUG
        *(here) = (bool)karaktValue;
    }
}

void saveLocal(bool saveLayers = true, bool saveSequences = true, bool saveEvents = true,
               bool saveCCMods = true, bool saveCell = true, bool saveMidiMatrix = true, 
               bool saveNoteRedirect = true, bool saveLFOs = true, bool saveKarakt = true )
{
    // fileString = createTHXname();
    fileString = fileNameSave; // nouveau nom du fichier : reste idem, sauf si on demande à changer de nom
                               // on va donc clairement écraser les anciens fichiers, plutôt que d'en créer de nouveaux à chaque fois

/*
    myFile = SD.open(fileString, FILE_WRITE);
  */
    // myFile.remove(fileString);
    // myFile = sd.open(fileString, FILE_WRITE);

    // if the file opened okay, write to it:
    if (myFile.isOpen())
    {
        myFile.remove(); // 1C86
        myFile.close();
    }

/*
    myFile = sd.open(fileString, FILE_WRITE);
*/

    // if the file opened okay, write to it:
    if (myFile)
    {
      //   buffer_clearScreen_HARD();
      buffer_clearScreen_HARD(); 
        buffer_and_HARD_setCursor(0, 0);
        buffer_and_HARD_print(F("Opened OK "));
        buffer_and_HARD_setCursor(0, 1);
        buffer_and_HARD_print(fileString);
        delay(watchDelay);
    }
    else
    {
      //  buffer_clearScreen_HARD();
      buffer_clearScreen_HARD();
      buffer_and_HARD_setCursor(0, 0);
      buffer_and_HARD_print(F("PROBLEM at Open "));
      buffer_and_HARD_setCursor(0, 1);
      buffer_and_HARD_print(fileString);
      delay(watchDelay);
    }

    myFile.println("C\t//VERSION");
    // lire en première lettre un B indique que c'est ce modèle-ci de fichier
    // modifié à partir de 1C99

    // écriture des layers DONE
    // écriture des séquences DONE
    // écriture des events DONE
    // écriture de midimatrix
    // écriture de Cell
    // écriture CCMod DONE

    // que faut-il écrire des infos générales, et par layer ?
    // ecriture CHARACTERISTICS

    // LAYERS
    if (saveLayers == true)
    {
        myFile.println("[\t//LAYERS START");
        myFile.print("\t");
        myFile.println(idBlockLayers);

        for (unsigned int i = 0; i < nbTotalPiles; i++)
        {
            unsigned int taille = mesureTaillePileComplete(pInitial[i]);

            if (taille > 0)
            {
                myFile.print("\t{\t//LAYER ");
                myFile.print(String(i + 1));
                myFile.println(idRecordLayer_Start);

                myFile.print("\t\t");
                myFile.print(String(i + 1));
                myFile.println(idRecordLayer_Number);

                myFile.print("\t\t");
                myFile.print(String(layerLength[i]));
                myFile.print("\t");
                myFile.println(idRecordLayer_Length);

                for (unsigned int j = 0; j < taille; j++)
                {

                    buffer_and_HARD_setCursor(0, 3);
                    buffer_and_HARD_print(F("writing note "));

                    buffer_and_HARD_print(j);
                    buffer_and_HARD_print(" ");
                 delay(watchDelay);

                    pileNote tempNote;
                    tempNote = ZEROgetNoteFromPile(pInitial[i], j);

                    myFile.print("\t\t(\t//NOTE ");
                    myFile.print(String(j + 1));
                    myFile.println(" START");

                    // on va ensuite écrire les valeurs, suivies d'un TAG id
                    myFile.print("\t\t\t");
                    myFile.print(String(tempNote.note));
                    myFile.print("\t");
                    myFile.println(idRecordNote_Note);

                    myFile.print("\t\t\t");
                    myFile.print(String(tempNote.channel));
                    myFile.print("\t");
                    myFile.println(idRecordNote_Channel);

                    myFile.print("\t\t\t");
                    myFile.print(String(tempNote.layer));
                    myFile.print("\t");
                    myFile.println(idRecordNote_Layer);

                    myFile.print("\t\t\t");
                    myFile.print(String(tempNote.velocity));
                    myFile.print("\t");
                    myFile.println(idRecordNote_Velocity);

                    myFile.print("\t\t\t");
                    myFile.print(String(tempNote.startNote));
                    myFile.print("\t");
                    myFile.println(idRecordNote_StartNote);

                    myFile.print("\t\t\t");
                    myFile.print(String(tempNote.endNote));
                    myFile.print("\t");
                    myFile.println(idRecordNote_EndNote);

                    myFile.print("\t\t\t");
                    myFile.print(String(tempNote.thereIsAnEnd));
                    myFile.print("\t");
                    myFile.println(idRecordNote_ThereIsAnEnd);

                    myFile.print("\t\t\t");
                    myFile.print(String(tempNote.type));
                    myFile.print("\t");
                    myFile.println(idRecordNote_Type);

                    myFile.print("\t\t)\t//NOTE ");
                    myFile.print(String(j + 1));
                    myFile.println(" END");
                }

                myFile.print("\t}\t//LAYER ");
                myFile.print(String(i + 1));
                myFile.println(idRecordLayer_End);
            }
        }

        myFile.println("]\t//LAYERS END");
    }

    // SEQUENCES
    if (saveSequences == true)
    {
        myFile.println("[\t//SEQUENCES START");
        myFile.print("\t");
        myFile.println(idBlockSequences);

        for (unsigned int i = 0; i < nbSequencesMax; i++)
        {
            unsigned int taille = sequence[i].tailleSeq;

            if (taille > 0)
            {
                myFile.print("\t{\t//SEQUENCE ");
                myFile.print(String(i + 1));
                myFile.println(idRecordSequence_Start);

                myFile.print("\t\t");
                myFile.print(String(i + 1));
                myFile.println(idRecordSequence_Number);

                // insérer ici les caractéristiques de chaque séquence, l'une après l'autre, avec idTag
                // pour l'instant l'idTag est un peu inutile, mais une version ultérieure reconnaîtra peut être le contenu à partir du Tag,
                // et non pas de l'emplacement.

                //   int tableauSeq[tailleMaxTableauSequence] ;

                //     byte tailleSeq ;
                // on ne va en fait PAS sauver cette info, de sorte que le fichier .Txt puisse être modifié facielemtn :
                // il suffira de rajouter des notes.   par contre, au load, il faut faire attention de calculer la valeur de tailleSeq !

                //  myFile.print("\t\t");
                //             myFile.print(String(sequence[i].tailleSeq );
                //             myFile.print("\t");
                //             myFile.println(idRecordSequence_tailleSeq );

                //     byte modeSeq ; // 0 FWD    1 BACK    2 RND    3 A-R    4 JITTER    5 RND-FRG

                myFile.print("\t\t");
                myFile.print(String(sequence[i].modeSeq));
                myFile.print("\t");
                myFile.println(idRecordSequence_modeSeq);

                //     bool actif ;
                myFile.print("\t\t");
                myFile.print(String(sequence[i].actif));
                myFile.print("\t");
                myFile.println(idRecordSequence_actif);

                //     byte jitterMinUP;
                myFile.print("\t\t");
                myFile.print(String(sequence[i].jitterMinUP));
                myFile.print("\t");
                myFile.println(idRecordSequence_jitterMinUP);

                //     byte jitterMaxUP;
                myFile.print("\t\t");
                myFile.print(String(sequence[i].jitterMaxUP));
                myFile.print("\t");
                myFile.println(idRecordSequence_jitterMaxUP);

                //     // on pourrait coder 16 sur 2 demi-bytes NOTE DEBUG
                //     byte jitterMinDOWN;
                myFile.print("\t\t");
                myFile.print(String(sequence[i].jitterMinDOWN));
                myFile.print("\t");
                myFile.println(idRecordSequence_jitterMinDOWN);

                //     byte jitterMaxDOWN;
                myFile.print("\t\t");
                myFile.print(String(sequence[i].jitterMaxDOWN));
                myFile.print("\t");
                myFile.println(idRecordSequence_jitterMaxDOWN);

                //     byte jitterStaticMin;
                myFile.print("\t\t");
                myFile.print(String(sequence[i].jitterStaticMin));
                myFile.print("\t");
                myFile.println(idRecordSequence_jitterStaticMin);

                //     byte jitterStaticMax;
                myFile.print("\t\t");
                myFile.print(String(sequence[i].jitterStaticMax));
                myFile.print("\t");
                myFile.println(idRecordSequence_jitterStaticMax);

                //     int jitterRichting;   // variables internes, qu'on ne modifie pas soi même
                //     // NOTE DEBUG peut être transformé en version courte, si code suit

                //     int jitterWhereNow; // variables internes, qu'on ne modifie pas soi même
                //     int jitterWhereTo ; // variables internes, qu'on ne modifie pas soi même

                //     byte jitterStaticCompteur;  // variables internes, qu'on ne modifie pas soi même
                //     byte jitterStaticToReach; // variables internes, qu'on ne modifie pas soi même

                //     // int jitterPolarity(0); // -1 : wherenow < whereTo : +1 : > : 0 =
                //     // int oldJitterPolarity(0) ; // dernière valeur
                //     bool beingStatic ;  // variables internes, qu'on ne modifie pas soi même
                //     // on pourrait coder les bool sur des bits NOTE DEBUG
                //     //
                //     byte sizeRANDOMFRAG  ;
                myFile.print("\t\t");
                myFile.print(String(sequence[i].sizeRANDOMFRAG));
                myFile.print("\t");
                myFile.println(idRecordSequence_sizeRANDOMFRAG);

                //     byte repeatRANDOMFRAG ;
                myFile.print("\t\t");
                myFile.print(String(sequence[i].repeatRANDOMFRAG));
                myFile.print("\t");
                myFile.println(idRecordSequence_repeatRANDOMFRAG);

                //     byte toReachRANDOMFRAG;  // variables internes, qu'on ne modifie pas soi même
                //     byte keepN ;

                //     bool compteLayer  ; // si cette valeur est fausse il compte les notes
                //     // si elle est vraie, il compte les layers
                myFile.print("\t\t");
                myFile.print(String(sequence[i].compteLayer));
                myFile.print("\t");
                myFile.println(idRecordSequence_compteLayer);

                //     long compteur  ; // +1 à chaque itération, vaudra un grand nombre

                //     byte action ; // 00 = CC    1 = NOte     2 = Velocite    plus tard : 3 = SWING
                myFile.print("\t\t");
                myFile.print(String(sequence[i].action));
                myFile.print("\t");
                myFile.println(idRecordSequence_action);

                //     byte CC ;
                myFile.print("\t\t");
                myFile.print(String(sequence[i].CC));
                myFile.print("\t");
                myFile.println(idRecordSequence_CC);

                //     byte channel ;
                myFile.print("\t\t");
                myFile.print(String(sequence[i].channel));
                myFile.print("\t");
                myFile.println(idRecordSequence_channel);

                //     byte layer ;
                myFile.print("\t\t");
                myFile.print(String(sequence[i].layer));
                myFile.print("\t");
                myFile.println(idRecordSequence_layer);

                for (unsigned int j = 0; j < taille; j++)
                {

                    buffer_and_HARD_setCursor(0, 3);
                    buffer_and_HARD_print(F("wrt Seq values "));

                    buffer_and_HARD_print(j);
                    buffer_and_HARD_print(" ");
                    delay(watchDelay);

                    myFile.print("\t\t(\t//SEQ VALUE ");
                    myFile.print(String(j + 1));
                    myFile.println(" START");

                    // on va ensuite écrire les valeurs, suivies d'un TAG id
                    myFile.print("\t\t\t");
                    myFile.print(String(sequence[i].tableauSeq[j]));
                    myFile.print("\t");
                    myFile.println(idRecordSequence_values);

                    myFile.print("\t\t)\t//SEQ VALUE ");
                    myFile.print(String(j + 1));
                    myFile.println(" END");
                }

                myFile.print("\t}\t//LAYER ");
                myFile.print(String(i + 1));
                myFile.println(idRecordSequence_End);
            }
        }

        myFile.println("]\t//SEQUENCES END");
    }

    // EVENTS
    if (saveEvents == true)
    {
        myFile.println("[\t//EVENTS START");
        myFile.print("\t");
        myFile.println(idBlockEvenements);

        for (unsigned int i = 0; i < NbEvenementMax; i++)
        {
            unsigned int taille = Evenement[i].type;
            // si l'event est vide, type vaut 0

            if (taille > 0)
            {
                myFile.print("\t{\t//EVENT ");
                myFile.print(String(i + 1));
                myFile.println(idRecordEvenement_Start);

                myFile.print("\t\t");
                myFile.print(String(i + 1));
                myFile.println(idRecordEvenement_Number);

                // insérer ici les caractéristiques de chaque event, l'une après l'autre, avec idTag
                // pour l'instant l'idTag est un peu inutile, mais une version ultérieure reconnaîtra peut être le contenu à partir du Tag,
                // et non pas de l'emplacement.

                //   bool actif ; // rajouté en nov2018, sur le modèle de Sequence

                myFile.print("\t\t");
                myFile.print(String(Evenement[i].actif));
                myFile.print("\t");
                myFile.println(idRecordEvenement_actif);

                //     byte type ; // 0 = Vide ( il faut initialiser ! )   1 = CC
                //     // 2 = Transformation
                //     // (3) = Modification de valeur ( peut être intégrer à transformation ! )
                myFile.print("\t\t");
                myFile.print(String(Evenement[i].type));
                myFile.print("\t");
                myFile.println(idRecordEvenement_type);

                //     unsigned int intervalle ; // exprimé en 0.1 secondes  10 = 1 seconde 100 = 10 secondes 1000 = 100 secondes 10000 = quasi 1 h
                myFile.print("\t\t");
                myFile.print(String(Evenement[i].intervalle));
                myFile.print("\t");
                myFile.println(idRecordEvenement_intervalle);

                //     unsigned int moment ; // // dans les temps divisible par 300, agit après 100
                myFile.print("\t\t");
                myFile.print(String(Evenement[i].moment));
                myFile.print("\t");
                myFile.println(idRecordEvenement_moment);

                //     byte fidelite ; // en %
                myFile.print("\t\t");
                myFile.print(String(Evenement[i].fidelite));
                myFile.print("\t");
                myFile.println(idRecordEvenement_fidelite);

                //     byte typeInterval ; // 1 = constant 2 = random(a,b)  3=gaussian   4 = bi intervalle
                myFile.print("\t\t");
                myFile.print(String(Evenement[i].typeInterval));
                myFile.print("\t");
                myFile.println(idRecordEvenement_typeInterval);

                //     int which ;  // si type 1 : quel CC
                //     // si type 2 : quelle transformation
                //     // si type 3 : quelle valeur ( comment définir ? liste dans tableau, ou via transformation ? paraît mieux )
                myFile.print("\t\t");
                myFile.print(String(Evenement[i].which));
                myFile.print("\t");
                myFile.println(idRecordEvenement_which);

                //     int valeur ;
                myFile.print("\t\t");
                myFile.print(String(Evenement[i].valeur));
                myFile.print("\t");
                myFile.println(idRecordEvenement_valeur);

                //     byte shape ;  // 1 = valeur fixe ( pour valeur )
                //     // 2 = INC
                //     // 3 = DEC ( ou bien INC avec valeur négative ? )
                //     // 4 = RND  ( définir min et max, 0-127 par défaut pour CC )
                //     // 5 = LISTE : ex 4 valeurs 37,69,120,130
                //     //  5=liste sequential 6 = liste random
                myFile.print("\t\t");
                myFile.print(String(Evenement[i].shape));
                myFile.print("\t");
                myFile.println(idRecordEvenement_shape);

                //     byte rangeUP_ ; // pour option 2,3,4
                myFile.print("\t\t");
                myFile.print(String(Evenement[i].rangeUP_));
                myFile.print("\t");
                myFile.println(idRecordEvenement_rangeUP_);

                //     // NOTE DEBUG : le préciser dans affichae, comme j'ai fait pour Seq, c'est bcp plus compréhensible
                //     byte rangeLO_ ; // pour option 2,3,4
                myFile.print("\t\t");
                myFile.print(String(Evenement[i].rangeLO_));
                myFile.print("\t");
                myFile.println(idRecordEvenement_rangeLO_);

                //     byte incSize ; // définit de combien la valeur en option 2 et 3 ( qui donnent le sens)
                myFile.print("\t\t");
                myFile.print(String(Evenement[i].incSize));
                myFile.print("\t");
                myFile.println(idRecordEvenement_incSize);

                //     byte behaviorLimit ; // pour options shape 2,3
                //     // 1 = Change direction
                //     // 2 = qd dépasse vers haut ou bas, réapparait de l'autre côté
                //     // 3 = reste à la limite
                myFile.print("\t\t");
                myFile.print(String(Evenement[i].behaviorLimit));
                myFile.print("\t");
                myFile.println(idRecordEvenement_behaviorLimit);

                //     byte channel ; // channel concerné 0-15
                myFile.print("\t\t");
                myFile.print(String(Evenement[i].channel));
                myFile.print("\t");
                myFile.println(idRecordEvenement_channel);

                //     byte layer ; // layer concerné 1-4 ou combinaison ou random ; ou bien layers actives.   par défaut : Actif ; sinon spécifique
                myFile.print("\t\t");
                myFile.print(String(Evenement[i].layer));
                myFile.print("\t");
                myFile.println(idRecordEvenement_layer);

                //     unsigned long eventGOON ; // indique à quel moment le prochain Evènement doit avoir lieu
                // myFile.print("\t\t");
                // myFile.print(String(Evenement[i].eventGOON));
                // myFile.print("\t");
                // myFile.println(idRecordEvenement_eventGOON);

                //     bool conditionActive ;
                myFile.print("\t\t");
                myFile.print(String(Evenement[i].conditionActive));
                myFile.print("\t");
                myFile.println(idRecordEvenement_conditionActive);
                //TODO on pourrait faire une version abrégée si il n'y a pas de test

                //     byte whichTest ;
                myFile.print("\t\t");
                myFile.print(String(Evenement[i].whichTest));
                myFile.print("\t");
                myFile.println(idRecordEvenement_whichTest);

                //     byte whichOperation ;
                // Which Operation 1% 2/ 3x 4%+ 5+
                myFile.print("\t\t");
                myFile.print(String(Evenement[i].whichOperation));
                myFile.print("\t");
                myFile.println(idRecordEvenement_whichOperation);

                //     byte whichValue ;
                myFile.print("\t\t");
                myFile.print(String(Evenement[i].whichValue));
                myFile.print("\t");
                myFile.println(idRecordEvenement_whichValue);

                //     byte whichComparison ;
                myFile.print("\t\t");
                myFile.print(String(Evenement[i].whichComparison));
                myFile.print("\t");
                myFile.println(idRecordEvenement_whichComparison);

                //     byte whichReference ;
                myFile.print("\t\t");
                myFile.print(String(Evenement[i].whichReference));
                myFile.print("\t");
                myFile.println(idRecordEvenement_whichReference);

                //     byte whichAction ; // 1 DO 2.BLOCK
                myFile.print("\t\t");
                myFile.print(String(Evenement[i].whichAction));
                myFile.print("\t");
                myFile.println(idRecordEvenement_whichAction);

                //     bool launch ; // utilisé pour ce qui est lié  à des notes ou layer modulo play
                //     byte whichLayer2test ; //
                myFile.print("\t\t");
                myFile.print(String(Evenement[i].whichLayer2test));
                myFile.print("\t");
                myFile.println(idRecordEvenement_whichLayer2test);

                //     byte modulo ;
                myFile.print("\t\t");
                myFile.print(String(Evenement[i].modulo));
                myFile.print("\t");
                myFile.println(idRecordEvenement_modulo);

                //     byte moduloK ;
                myFile.print("\t\t");
                myFile.print(String(Evenement[i].moduloK));
                myFile.print("\t");
                myFile.println(idRecordEvenement_moduloK);

                //     byte moduloLayers ;
                myFile.print("\t\t");
                myFile.print(String(Evenement[i].moduloLayers));
                myFile.print("\t");
                myFile.println(idRecordEvenement_moduloLayers);

                //     byte moduloMode ;
                myFile.print("\t\t");
                myFile.print(String(Evenement[i].moduloMode));
                myFile.print("\t");
                myFile.println(idRecordEvenement_moduloMode);

                //     int compteur ; // sera un compteur qui décompte le nb de fois qu'un evt a lieu

                //     int echeance ; // decrit le nb de fois maximum qu'un evt peut avoir lieu
                myFile.print("\t\t");
                myFile.print(String(Evenement[i].echeance));
                myFile.print("\t");
                myFile.println(idRecordEvenement_echeance);

                //     int latence ; // décrit à partir de quand l'event aura réellement lieu
                myFile.print("\t\t");
                myFile.print(String(Evenement[i].latence));
                myFile.print("\t");
                myFile.println(idRecordEvenement_latence);

                //     bool fugace ; // si YES, qd compteur arrive à échéance, l'evt est supprimé
                myFile.print("\t\t");
                myFile.print(String(Evenement[i].fugace));
                myFile.print("\t");
                myFile.println(idRecordEvenement_fugace);

                //     byte seqUsedAsList  ; // TODO DEBUG : mettre àjour initialisation, delete et save
                myFile.print("\t\t");
                myFile.print(String(Evenement[i].seqUsedAsList));
                myFile.print("\t");
                myFile.println(idRecordEvenement_seqUsedAsList);

                myFile.print("\t}\t//EVENT ");
                myFile.print(String(i + 1));
                myFile.println(idRecordEvenement_End);
            }
        }

        myFile.println("]\t//EVENTS END");
    }

    // CCMOD
    if (saveCCMods == true)
    {
        myFile.println("[\t//CCMODS START");
        myFile.print("\t");
        myFile.println(idBlockCCMod);

        for (unsigned int i = 0; i < NbCCModMax; i++)
        {
            // unsigned int taille = mesureTaillePileComplete(pInitial[i]);
            unsigned int taille = CCModTableau[i].type; // vaut 0 si vide

            if (taille > 0)
            {
                myFile.print("\t{\t//CC MOD ");
                myFile.print(String(i + 1));
                myFile.println(idRecordCCMod_Start);

                myFile.print("\t\t");
                myFile.print(String(i + 1));
                myFile.println(idRecordCCMod_Number);

                // byte type;          // 0 = vide   1 = VELOCITY  2 = PITCH   3 = NOTE

                myFile.print("\t\t");
                myFile.print(String(CCModTableau[i].type));
                myFile.print("\t");
                myFile.println(idRecordCCMod_type);

                // int sourceMin;      // 0 à 127 : vélocité, pitch, note (0-12) - va au delà de 127 pour la durée de notes

                myFile.print("\t\t");
                myFile.print(String(CCModTableau[i].sourceMin));
                myFile.print("\t");
                myFile.println(idRecordCCMod_sourceMin);

                // int sourceMax;      // 0 à 127

                myFile.print("\t\t");
                myFile.print(String(CCModTableau[i].sourceMax));
                myFile.print("\t");
                myFile.println(idRecordCCMod_sourceMax);

                // byte targetMin;     // 0 à 127 : valeur du CC 0x Bn 103 Valeur

                myFile.print("\t\t");
                myFile.print(String(CCModTableau[i].targetMin));
                myFile.print("\t");
                myFile.println(idRecordCCMod_targetMin);

                // byte targetMax;     // 0 à 127 : valeur du CC 0x Bn 103 Valeur

                myFile.print("\t\t");
                myFile.print(String(CCModTableau[i].targetMax));
                myFile.print("\t");
                myFile.println(idRecordCCMod_targetMax);

                // byte borders;       // 0 = limite    1 = expand     2 = No Send audelà et endeça des valeurs min et max   3 = invert : envoie au-delà mais pas entre !

                myFile.print("\t\t");
                myFile.print(String(CCModTableau[i].borders));
                myFile.print("\t");
                myFile.println(idRecordCCMod_borders);

                // byte idCC;          // définit le CC auquel on envoie la valeur : ex : 103 pour EditA sur Behringer2024

                myFile.print("\t\t");
                myFile.print(String(CCModTableau[i].idCC));
                myFile.print("\t");
                myFile.println(idRecordCCMod_idCC);

                // byte channelCC;     // définit sur quel Channel n on doit envoyer l'info CC : 0x Bn 103 Valeur

                myFile.print("\t\t");
                myFile.print(String(CCModTableau[i].channelCC));
                myFile.print("\t");
                myFile.println(idRecordCCMod_channelCC);

                // byte layerConcerne; // définit quel layer sont concernés : 00004321

                myFile.print("\t\t");
                myFile.print(String(CCModTableau[i].layerConcerne));
                myFile.print("\t");
                myFile.println(idRecordCCMod_layerConcerne);

                // // attention : 1000 0000 = concerne le Midi parallel player
                // bool actif; // permet d'avoir un CCMod non activé sur lequel on travaille

                myFile.print("\t\t");
                myFile.print(String(CCModTableau[i].actif));
                myFile.print("\t");
                myFile.println(idRecordCCMod_actif);

                myFile.print("\t}\t//CC MOD ");
                myFile.print(String(i + 1));
                myFile.println(idRecordCCMod_End);
            }
        }

        myFile.println("]\t//CCMODS END");
    }

    // CELL
    if (saveCell == true)
    {
        myFile.println("[\t//CELLS START");
        myFile.print("\t");
        myFile.println(idBlockCell);

        for (unsigned int i = 0; i < nbValeursDansCell; i++)
        {
            // if (taille > 0)
            {
                myFile.print("\t{\t//CELL ");
                myFile.print(String(i + 1));
                myFile.println(idRecordCell_Start);

                myFile.print("\t\t");
                myFile.print(String(i + 1));
                myFile.println(idRecordCell_Number);

                // {
                //     byte channelIn ;
                //     byte noteIn ;
                //     byte channelOut ;
                //     byte noteOut ;
                // };

                myFile.print("\t\t");
                myFile.print(String(tableauCell[i].channelIn));
                myFile.print("\t");
                myFile.println(idRecordCell_channelIn);

                myFile.print("\t\t");
                myFile.print(String(tableauCell[i].noteIn));
                myFile.print("\t");
                myFile.println(idRecordCell_noteIn);

                myFile.print("\t\t");
                myFile.print(String(tableauCell[i].channelOut));
                myFile.print("\t");
                myFile.println(idRecordCell_channelOut);

                myFile.print("\t\t");
                myFile.print(String(tableauCell[i].noteOut));
                myFile.print("\t");
                myFile.println(idRecordCell_noteOut);

                myFile.print("\t}\t//CC MOD ");
                myFile.print(String(i + 1));
                myFile.println(idRecordCell_End);
            }
        }

        myFile.println("]\t//CELLS END");
    }

    // MIDIMATRIX
    if (saveMidiMatrix == true)
    {
        myFile.println("[\t//MIDIMATRIX START");
        myFile.print("\t");
        myFile.println(idBlockMidiMatrix);

        for (unsigned int i = 0; i < 16; i++)
        {
            // if (taille > 0)
            {
                myFile.print("\t{\t//MIDI MATRIX ");
                myFile.print(String(i + 1));
                myFile.println(idRecordMidiMatrix_Start);

                myFile.print("\t\t");
                myFile.print(String(i + 1));
                myFile.println(idRecordMidiMatrix_Number);

                myFile.print("\t\t");
                myFile.print(String(midiRedirect[i]));
                myFile.print("\t");
                myFile.println(idRecordMidiMatrix_midiRedirect);

                myFile.print("\t}\t//MIDI MATRIX");
                myFile.print(String(i + 1));
                myFile.println(idRecordMidiMatrix_End);
            }
        }

        myFile.println("]\t//MIDIMATRIX END");
    }

    // NOTE REDIRECT
    if (saveNoteRedirect == true)
    {
        myFile.println("[\t//NOTEREDIRECT START");
        myFile.print("\t");
        myFile.println(idBlockNoteRedirect);

        for (unsigned int i = 0; i < 12; i++)
        {
            // if (taille > 0)
            {
                myFile.print("\t{\t//NOTE REDIRECT ");
                myFile.print(String(i + 1));
                myFile.println(idRecordNoteRedirect_Start);

                myFile.print("\t\t");
                myFile.print(String(i + 1));
                myFile.println(idRecordNoteRedirect_Number);

                myFile.print("\t\t");
                myFile.print(String(noteRedirect[i]));
                myFile.print("\t");
                myFile.println(idRecordNoteRedirect_noteRedirect);

                myFile.print("\t}\t//MIDI MATRIX");
                myFile.print(String(i + 1));
                myFile.println(idRecordNoteRedirect_End);
            }
        }

        myFile.println("]\t//NOTEREDIRECT END");
    }

    // LFO
    if (saveLFOs == true)
    {
        myFile.println("[\t//LFOs START");
        myFile.print("\t");
        myFile.println(idBlockLFOs);

        for (unsigned int i = 0; i < nbLFOMax; i++)
        {
            unsigned int taille = LFO[i].type;
            // si l'event est vide, type vaut 0

            if (taille > 0)
            {
                myFile.print("\t{\t//LFO ");
                myFile.print(String(i + 1));
                myFile.println(idRecordLFO_Start);

                myFile.print("\t\t");
                myFile.print(String(i + 1));
                myFile.println(idRecordLFO_Number);

                // insérer ici les caractéristiques de chaque event, l'une après l'autre, avec idTag
                // pour l'instant l'idTag est un peu inutile, mais une version ultérieure reconnaîtra peut être le contenu à partir du Tag,
                // et non pas de l'emplacement.

                myFile.print("\t\t");
                myFile.print(String(LFO[i].actif));
                myFile.print("\t");
                myFile.println(idRecordLFO_actif);

                //     byte type ; // 0 = Vide ( il faut initialiser ! )   1 = CC
                //     // 2 = Transformation
                //     // (3) = Modification de valeur ( peut être intégrer à transformation ! )
                myFile.print("\t\t");
                myFile.print(String(LFO[i].type));
                myFile.print("\t");
                myFile.println(idRecordLFO_type);

                //     unsigned int intervalle ; // exprimé en 0.1 secondes  10 = 1 seconde 100 = 10 secondes 1000 = 100 secondes 10000 = quasi 1 h
                myFile.print("\t\t");
                myFile.print(String(LFO[i].shape));
                myFile.print("\t");
                myFile.println(idRecordLFO_shape);

                //     unsigned int moment ; // // dans les temps divisible par 300, agit après 100
                myFile.print("\t\t");
                myFile.print(String(LFO[i].fidelite));
                myFile.print("\t");
                myFile.println(idRecordLFO_fidelite);

                myFile.print("\t\t");
                myFile.print(String(LFO[i].duration));
                myFile.print("\t");
                myFile.println(idRecordLFO_duration);

                myFile.print("\t\t");
                myFile.print(String(LFO[i].decalage));
                myFile.print("\t");
                myFile.println(idRecordLFO_decalage);

                myFile.print("\t\t");
                myFile.print(String(LFO[i].lowest));
                myFile.print("\t");
                myFile.println(idRecordLFO_lowest);

                myFile.print("\t\t");
                myFile.print(String(LFO[i].highest));
                myFile.print("\t");
                myFile.println(idRecordLFO_highest);

                myFile.print("\t\t");
                myFile.print(String(LFO[i].channel));
                myFile.print("\t");
                myFile.println(idRecordLFO_channel);

                myFile.print("\t\t");
                myFile.print(String(LFO[i].which));
                myFile.print("\t");
                myFile.println(idRecordLFO_which);

                myFile.print("\t\t");
                myFile.print(String(LFO[i].typeMachine));
                myFile.print("\t");
                myFile.println(idRecordLFO_typeMachine);

                myFile.print("\t\t");
                myFile.print(String(LFO[i].layer));
                myFile.print("\t");
                myFile.println(idRecordLFO_layer);

                myFile.print("\t\t");
                myFile.print(String(LFO[i].modC));
                myFile.print("\t");
                myFile.println(idRecordLFO_modC);

                myFile.print("\t\t");
                myFile.print(String(LFO[i].modV));
                myFile.print("\t");
                myFile.println(idRecordLFO_modV);

  myFile.print("\t\t");
  myFile.print(String(LFO[i].LFORandom));
  myFile.print("\t");
  myFile.println(idRecordLFO_LFORandom);

  myFile.print("\t}\t//LFO ");
  myFile.print(String(i + 1));
  myFile.println(idRecordLFO_End);
            }
        }

        myFile.println("]\t//LFOs END");
    }

if ( saveKarakt == true )
{
    saveCurrentandMemKarakt(true,false ) ; // version sans HEADER 
}

        // TODO poursuivre ceci avec Characteristics, sur les mêmes modes

        myFile.println(F(""));
    myFile.println(F(""));

    myFile.println(F(""));
    myFile.println(F("Created by Piccolo "));
    myFile.println(PiccoloVersion);
    myFile.println(PiccoloDate);
    myFile.println(F(""));
    myFile.println(F("you can change numbers but not the structure"));
    myFile.println(F("do not insert or delete lines or TAB"));
    myFile.println(F("you can experiment with numbers though !"));
    myFile.println(F("now you can be an errorist"));

    // close the file:
    myFile.close();
    buffer_and_HARD_setCursor(0, 0);
    buffer_and_HARD_print(F("Saved  "));
    delay(watchDelay);

    for (byte i = 0; i < nbLayer; i++)
    {
        ZEROupdatePileTimeToPlayMODEDIRECT(i);
    }

    rootFileCount++;
    measureSDFileCount();
}

// on devra faire figurer ici des paramètres pour dire ce qu'on veut charger et de quelle façon :
// remplacer les anciennes valeurs, ou les rajouter aux existantes
// 1 layer ou tous, ou sélection
// 1 channel ou tous, etc.
// mode verbose ou pas

void loadLocal2(bool loadLayers = true, bool loadSequences = true, bool loadEvents = true, 
                bool loadCCMods = true, bool loadCells = true, bool loadMidiMatrix = true, 
                bool loadNoteRedirect = true, bool loadLFOs = true, bool loadKarakt = true)
{
    // LOAD LOCAL 2C01

    long startTime = millis(); // protection contre boucle infinies dans les while
    fileNameSave = fileName;

    // INITIALISATIONS

    if (loadLayers)
    {
        // on efface toutes les mémoires
        for (int i = 0; i < nbTotalPiles; i++)
        {
            ZEROdeleteWholePile(pInitial[i]);
        }
    }

    if (loadSequences)
    {
        initializeAllSequences();
    }

    if (loadEvents)
    {
        for (byte i = 0; i < NbEvenementMax; i++)
        { // layers
            EmptyEvenement(i);
        }
    }

    if (loadCCMods)
    {
        deleteAllCCMod();
    }

    if (loadLFOs)
    {
        for (byte i = 0; i < nbLFOMax; i++)
        {
            EmptyLFO(i);
        }
    }

    if ( loadKarakt )
    {
        // eraseAllKarakt TODO 
        for ( int i = 0 ; i < 64 ; i++ )
        {
            erase_KMem(i); 
        }
    }

    // pas d'initialisation pour LoadKarakt

    // TODO mettre un peu d'ordre et des noms similaires ici ! INITIALIZE me paraît le mieux
    // qd transfo vers pile, devra être remis au point

    // TODO on doit effacer les CCMODS, EVENTS, SEQUENCES, en fc de ce qu'on veut charger !

    // TODO mettre un timer pour évitre des boucles WHILE infinies
    // si le load prend plus de 5 sec, ABORT

    // PREPARATION VISUELLE ET MATERIELLE

    // on efface l'écran
   // u8x8.clear();
    buffer_clearScreen_HARD();

/*
    if (!SD.begin())
    {
        buffer_and_HARD_setCursor(0, 0);
        buffer_and_HARD_print(F("SD init error !"));
        return;
    }
*/

    buffer_and_HARD_setCursor(0, 0);
    buffer_and_HARD_print(F("SD init OK"));
    delay(watchDelay);

/*
    myFile = SD.open(name, FILE_READ);
*/

    // LECTURE
    // OPEN the HEader

    String ligne;

    ligne = myFile.readStringUntil('\r');
    cleanLigne(ligne);
    buffer_and_HARD_setCursor(0, 2);
    buffer_and_HARD_print(ligne);
    if (ligne == "C")
    {
        buffer_and_HARD_print("OK");
    }
    delay(watchDelay);

    // Nouvelle structure du LoadLocal2
    int check = 0;
    do
    { // Debut lecture
        // lecture d'une ligne
        ligne = myFile.readStringUntil('\n');
        cleanLigne(ligne);
        check = myFile.peek();

      //  buffer_clearLine(7);
      buffer_clearLine(7); 
        buffer_and_HARD_setCursor(0, 7);
        buffer_and_HARD_print(ligne.substring(0, 12));
        delay(watchDelay);

        if (check != -1)
        {
            if (ligne == "[")
            {
                // entrée dans un block - on devra lire jusqu'à ]
                buffer_and_HARD_setCursor(0, 2);
               //  buffer_clearLine(2);
                buffer_clearLine(2); 
                buffer_and_HARD_print("ENTER BLOCK");
                delay(watchDelay);

                // va lire le type de block
                ligne = myFile.readStringUntil('\n');
                cleanLigne(ligne);
                check = myFile.peek();
                buffer_and_HARD_setCursor(0, 2);
               // buffer_clearLine(2);
               buffer_clearLine(2); 
                buffer_and_HARD_print(ligne);
                delay(watchDelay);

                if (ligne == idBlockLayers)
                // *************************************************************
                // LECTURE LAYERS
                // *************************************************************
                {
                    buffer_and_HARD_setCursor(0, 2);
                   //  buffer_clearLine(2);
                    buffer_clearLine(2); 
                    buffer_and_HARD_print("Found LAYERS");
                    delay(watchDelay);

                    if (loadLayers == true)
                    {
                        do
                        { // Debut lecture
                            // lecture d'une ligne
                            ligne = myFile.readStringUntil('\n');
                            cleanLigne(ligne);
                            check = myFile.peek();

                           // buffer_clearLine(7);
                           buffer_clearLine(7); 
                            buffer_and_HARD_setCursor(0, 7);
                            buffer_and_HARD_print(ligne.substring(0, 12));
                            delay(watchDelay);

                            if (check != -1)
                            {
                                if (ligne == "{")
                                {
                                    // entrée dans un layer - on devra lire jusqu'à }
                                    buffer_and_HARD_setCursor(0, 2);
                                   // buffer_clearLine(2);
                                   buffer_clearLine(2); 
                                    buffer_and_HARD_print("ENTER ONE LAYER");
                                    delay(watchDelay);

                                    do
                                    {
                                        // la suivante doit décrire le numéro de LAYER
                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);

                                        int currentUploadLayer = 0;
                                        currentUploadLayer = (int)ligne.toInt() - 1;

                                        buffer_and_HARD_setCursor(0, 2);
                                       // buffer_clearLine(2);
                                       buffer_clearLine(2); 
                                        buffer_and_HARD_print("upload L");
                                        buffer_and_HARD_print(currentUploadLayer);
                                        delay(watchDelay);

                                        // la suivante doit décrire la durée de LAYER
                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);

                                        layerLength[currentUploadLayer] = (long)ligne.toFloat(); // pas toInt car >>

                                        buffer_and_HARD_setCursor(0, 2);
                                       // buffer_clearLine(2);
                                       buffer_clearLine(2); 
                                        buffer_and_HARD_print("duree");
                                        buffer_and_HARD_print(layerLength[currentUploadLayer]);
                                        delay(watchDelay);

                                        int compteurNote = 0;

                                        // on va chercher l'ouverture de notes
                                        ligne = myFile.readStringUntil('(');
                                        myFile.peek();
                                        // on lit cette ligne d'ouverture
                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        // pour les lignes suivantes on va vérifier le contenu à partir des commentaires

                                        do
                                        { // TANT QUE CARACTERE Après lecture est une parenthèse ouverte (

                                            buffer_and_HARD_setCursor(0, 4);
                                          //  buffer_clearLine(4);
                                          buffer_clearLine(4); 
                                            buffer_and_HARD_print("note");
                                            buffer_and_HARD_print(compteurNote);

                                            delay(watchDelay);

                                            pileNote addNote;
                                            putNoteBinNoteA(addNote, emptyNote);

                                            //NOTE VALUE
                                            ligne = myFile.readStringUntil('\n');
                                            myFile.read();
                                            cleanLigne(ligne);
                                            addNote.note = ligne.toInt();

                                            //CHANNEL
                                            ligne = myFile.readStringUntil('\n');
                                            myFile.peek();
                                            cleanLigne(ligne);
                                            addNote.channel = ligne.toInt();

                                            //LAYER
                                            ligne = myFile.readStringUntil('\n');
                                            myFile.peek();
                                            cleanLigne(ligne);
                                            addNote.layer = ligne.toInt();

                                            //VELOCITY
                                            ligne = myFile.readStringUntil('\n');
                                            myFile.peek();
                                            cleanLigne(ligne);
                                            addNote.velocity = ligne.toInt();

                                            //START (msec)
                                            ligne = myFile.readStringUntil('\n');
                                            myFile.peek();
                                            cleanLigne(ligne);
                                            addNote.startNote = (unsigned long)ligne.toFloat(); // car trop grand pour toInt !

                                            //END (msec)
                                            ligne = myFile.readStringUntil('\n');
                                            myFile.peek();
                                            cleanLigne(ligne);
                                            addNote.endNote = (unsigned long)ligne.toFloat();

                                            //FINITUDE
                                            ligne = myFile.readStringUntil('\n');
                                            myFile.peek();
                                            cleanLigne(ligne);
                                            addNote.killnote = ligne.toFloat();

                                            //TYPE (144=0x90 ; 128=0x80 ; 176=0xB0=CC)
                                            ligne = myFile.readStringUntil('\n');
                                            myFile.peek();
                                            cleanLigne(ligne);
                                            addNote.type = ligne.toInt();

                                            ZEROaddNewNotetoPile(pInitial[currentUploadLayer], addNote);

                                            // fin de note ), voir si encore une après (, ou si autre layer {
                                            // lit la parenthèse
                                            ligne = myFile.readStringUntil('\n');
                                            myFile.peek();

                                            // lit le signe qui suit : si parenthèse ( nouvelle note, si { nouveau layer, si } fin des layers, si ] fin de block
                                            ligne = myFile.readStringUntil('\n');
                                            myFile.peek();
                                            cleanLigne(ligne);

                                            buffer_and_HARD_setCursor(0, 4);
                                           //  buffer_clearLine(4);
                                           buffer_clearLine(4); 
                                            buffer_and_HARD_print("FIN DE NOTE");
                                            buffer_and_HARD_print(compteurNote);
                                            buffer_and_HARD_print(ligne);
                                            delay(watchDelay);

                                            compteurNote++;
                                        } while ((ligne.compareTo("(") == 0) && (millis() - startTime < 100000));

                                        buffer_and_HARD_setCursor(0, 4);
                                        // buffer_clearLine(4);
                                        buffer_clearLine(4); 
                                        buffer_and_HARD_print("GO TO LAYERUP");
                                        delay(watchDelay);

                                        // on a quelles possibiltés  ( ) (      ( ) } {    ( ) } ]

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);

                                    } while ((ligne.compareTo("{") == 0) && (millis() - startTime < 100000));
                                }
                            }
                        } while (ligne != "]");
                    }
                    else
                    {
                        do
                        {
                            ligne = myFile.readStringUntil('\n');
                            cleanLigne(ligne);
                        } while (ligne != "]");
                    }
                }
                else if (ligne == idBlockSequences)
                // *************************************************************
                // LECTURE SQUENCES
                // *************************************************************
                {
                    buffer_and_HARD_setCursor(0, 2);
                  //  buffer_clearLine(2);
                  buffer_clearLine(2); 
                    buffer_and_HARD_print("Found SEQUENCES");
                    delay(watchDelay);

                    if (loadSequences == true)
                    {
                        do
                        { // Debut lecture
                            // lecture d'une ligne
                            ligne = myFile.readStringUntil('\n');
                            cleanLigne(ligne);
                            check = myFile.peek();

                          //  buffer_clearLine(7);
                          buffer_clearLine(7); 
                            buffer_and_HARD_setCursor(0, 7);
                            buffer_and_HARD_print(ligne.substring(0, 12));
                            delay(watchDelay);

                            if (check != -1)
                            {
                                if (ligne == "{")
                                {
                                    // entrée dans un sequence - on devra lire jusqu'à }
                                    buffer_and_HARD_setCursor(0, 2);
                                   // buffer_clearLine(2);
                                   buffer_clearLine(2); 
                                    buffer_and_HARD_print("ENTER ONE SEQUENCE");
                                    delay(watchDelay);

                                    do
                                    {
                                        // la suivante doit décrire le numéro de Sequence
                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);

                                        int currentUploadSequence = 0;
                                        currentUploadSequence = (int)ligne.toFloat() - 1;

                                        buffer_and_HARD_setCursor(0, 2);
                                      //   buffer_clearLine(2);
                                      buffer_clearLine(2); 
                                        buffer_and_HARD_print("upload S");
                                        buffer_and_HARD_print(currentUploadSequence);
                                        delay(watchDelay);

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        sequence[currentUploadSequence].modeSeq = (byte)ligne.toFloat();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        sequence[currentUploadSequence].actif = (byte)ligne.toInt();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        sequence[currentUploadSequence].jitterMinUP = (byte)ligne.toInt();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        sequence[currentUploadSequence].jitterMaxUP = (byte)ligne.toInt();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        sequence[currentUploadSequence].jitterMinDOWN = (byte)ligne.toInt();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        sequence[currentUploadSequence].jitterMaxDOWN = (byte)ligne.toInt();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        sequence[currentUploadSequence].jitterStaticMin = (byte)ligne.toInt();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        sequence[currentUploadSequence].jitterStaticMax = (byte)ligne.toInt();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        sequence[currentUploadSequence].sizeRANDOMFRAG = (byte)ligne.toInt();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        sequence[currentUploadSequence].repeatRANDOMFRAG = (byte)ligne.toInt();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        sequence[currentUploadSequence].compteLayer = (byte)ligne.toInt();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        sequence[currentUploadSequence].action = (byte)ligne.toInt();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        sequence[currentUploadSequence].CC = (byte)ligne.toInt();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        sequence[currentUploadSequence].channel = (byte)ligne.toInt();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        sequence[currentUploadSequence].layer = (byte)ligne.toInt();

                                        int compteurSeq = 0;

                                        // on va chercher l'ouverture de notes
                                        ligne = myFile.readStringUntil('(');
                                        myFile.peek();
                                        // on lit cette ligne d'ouverture
                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        // pour les lignes suivantes on va vérifier le contenu à partir des commentaires

                                        do
                                        { // TANT QUE CARACTERE Après lecture est une parenthèse ouverte (

                                            buffer_and_HARD_setCursor(0, 4);
                                          //  buffer_clearLine(4);
                                          buffer_clearLine(4); 
                                            buffer_and_HARD_print("seq[");
                                            buffer_and_HARD_print(currentUploadSequence);
                                            buffer_and_HARD_print(",");
                                            buffer_and_HARD_print(compteurSeq);
                                            buffer_and_HARD_print("]=");
                                            //NOTE VALUE
                                            ligne = myFile.readStringUntil('\n');
                                            myFile.read();
                                            cleanLigne(ligne);
                                            int SeqValue = ligne.toInt();
                                            buffer_and_HARD_print(SeqValue);
                                            delay(watchDelay);

                                            sequence[currentUploadSequence].tableauSeq[compteurSeq] = SeqValue;

                                            // fin de note ), voir si encore une après (, ou si autre layer {
                                            // lit la parenthèse
                                            ligne = myFile.readStringUntil('\n');
                                            myFile.peek();

                                            // lit le signe qui suit : si parenthèse ( nouvelle note, si { nouveau layer, si } fin des layers, si ] fin de block
                                            ligne = myFile.readStringUntil('\n');
                                            myFile.peek();
                                            cleanLigne(ligne);

                                            buffer_and_HARD_setCursor(0, 4);
                                          //  buffer_clearLine(4);
                                          buffer_clearLine(4); 
                                            buffer_and_HARD_print("FIN DE SEQ");
                                            buffer_and_HARD_print(compteurSeq);
                                            buffer_and_HARD_print(ligne);
                                            delay(watchDelay);

                                            compteurSeq++;
                                        } while ((ligne.compareTo("(") == 0) && (millis() - startTime < 100000));

                                        sequence[currentUploadSequence].tailleSeq = compteurSeq;

                                        buffer_and_HARD_setCursor(0, 4);
                                     //   buffer_clearLine(4);
                                     buffer_clearLine(4); 
                                        buffer_and_HARD_print("GO TO LAYERUP");
                                        delay(watchDelay);

                                        // on a quelles possibiltés  ( ) (      ( ) } {    ( ) } ]

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);

                                    } while ((ligne.compareTo("{") == 0) && (millis() - startTime < 100000));
                                }
                            }
                        } while (ligne != "]");
                    }
                    else
                    {
                        do
                        {
                            ligne = myFile.readStringUntil('\n');
                            cleanLigne(ligne);
                        } while (ligne != "]");
                    }
                }
                else if (ligne == idBlockEvenements)
                // *************************************************************
                // LECTURE EVENTS
                // *************************************************************
                {
                    buffer_and_HARD_setCursor(0, 2);
                   // buffer_clearLine(2);
                   buffer_clearLine(2); 
                    buffer_and_HARD_print("Found EVENTS");
                    delay(watchDelay);

                    if (loadEvents == true)
                    {
                        do
                        { // Debut lecture
                            // lecture d'une ligne
                            ligne = myFile.readStringUntil('\n');
                            cleanLigne(ligne);
                            check = myFile.peek();

                         //   buffer_clearLine(7);
                         buffer_clearLine(7); 
                            buffer_and_HARD_setCursor(0, 7);
                            buffer_and_HARD_print(ligne.substring(0, 12));
                            delay(watchDelay);

                            if (check != -1)
                            {
                                if (ligne == "{")
                                {
                                    // entrée dans un event - on devra lire jusqu'à }
                                    buffer_and_HARD_setCursor(0, 2);
                                  //  buffer_clearLine(2);
                                  buffer_clearLine(2); 
                                    buffer_and_HARD_print("ENTER ONE EVENT");
                                    delay(watchDelay);

                                    do
                                    {
                                        // la suivante doit décrire le numéro de l'event
                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        int currentUploadEvent = 0;
                                        currentUploadEvent = (int)ligne.toFloat() - 1;

                                        buffer_and_HARD_setCursor(0, 2);
                                      //  buffer_clearLine(2);
                                      buffer_clearLine(2); 
                                        buffer_and_HARD_print("upload Event");
                                        buffer_and_HARD_print(currentUploadEvent);
                                        delay(watchDelay);

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        Evenement[currentUploadEvent].actif = (byte)ligne.toFloat();
                                        buffer_and_HARD_setCursor(0, 7);
                                       // buffer_clearLine(7);
                                       buffer_clearLine(7); 
                                        buffer_and_HARD_print("AC" + ligne);
                                        delay(watchDelay);

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        Evenement[currentUploadEvent].type = (byte)ligne.toFloat();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        Evenement[currentUploadEvent].intervalle = (long)ligne.toFloat();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        Evenement[currentUploadEvent].moment = (long)ligne.toFloat();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        Evenement[currentUploadEvent].fidelite = (byte)ligne.toFloat();
                                        buffer_and_HARD_setCursor(0, 7);
                                     //   buffer_clearLine(7);
                                     buffer_clearLine(7); 
                                        buffer_and_HARD_print("FID" + ligne);
                                        delay(watchDelay);

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        Evenement[currentUploadEvent].typeInterval = (byte)ligne.toFloat();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        Evenement[currentUploadEvent].which = (byte)ligne.toFloat();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        Evenement[currentUploadEvent].valeur = (int)ligne.toFloat();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        Evenement[currentUploadEvent].shape = (byte)ligne.toFloat();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        Evenement[currentUploadEvent].rangeUP_ = (int)ligne.toFloat();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        Evenement[currentUploadEvent].rangeLO_ = (int)ligne.toFloat();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        Evenement[currentUploadEvent].incSize = (byte)ligne.toFloat();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        Evenement[currentUploadEvent].behaviorLimit = (byte)ligne.toFloat();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        Evenement[currentUploadEvent].channel = (byte)ligne.toFloat();
                                        buffer_and_HARD_setCursor(0, 7);
                                     //   buffer_clearLine(7);
                                        buffer_clearLine(7); 
                                        buffer_and_HARD_print("CH" + ligne);
                                        delay(watchDelay);

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        Evenement[currentUploadEvent].layer = (byte)ligne.toFloat();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        Evenement[currentUploadEvent].conditionActive = (byte)ligne.toFloat();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        Evenement[currentUploadEvent].whichTest = (byte)ligne.toFloat();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        Evenement[currentUploadEvent].whichOperation = (byte)ligne.toFloat();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        Evenement[currentUploadEvent].whichValue = (byte)ligne.toFloat();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        Evenement[currentUploadEvent].whichComparison = (byte)ligne.toFloat();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        Evenement[currentUploadEvent].whichReference = (byte)ligne.toFloat();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        Evenement[currentUploadEvent].whichAction = (byte)ligne.toFloat();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        Evenement[currentUploadEvent].whichLayer2test = (byte)ligne.toFloat();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        Evenement[currentUploadEvent].modulo = (byte)ligne.toFloat();
                                        buffer_and_HARD_setCursor(0, 7);
                                        // buffer_clearLine(7);
buffer_clearLine(7); 
                                        buffer_and_HARD_print("mod" + ligne);
                                        delay(watchDelay);

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        Evenement[currentUploadEvent].moduloK = (byte)ligne.toFloat();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        Evenement[currentUploadEvent].moduloLayers = (byte)ligne.toFloat();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        Evenement[currentUploadEvent].moduloMode = (byte)ligne.toFloat();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        Evenement[currentUploadEvent].echeance = (int)ligne.toFloat();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        Evenement[currentUploadEvent].latence = (int)ligne.toFloat();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        Evenement[currentUploadEvent].fugace = (byte)ligne.toFloat();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        Evenement[currentUploadEvent].seqUsedAsList = (byte)ligne.toFloat();
                                        buffer_and_HARD_setCursor(0, 7);
                                        // buffer_clearLine(7);
                                        buffer_clearLine(7); 
                                        buffer_and_HARD_print("seq" + ligne);
                                        delay(watchDelay);

                                        // do
                                        // { // TANT QUE CARACTERE Après lecture est une parenthèse ouverte (
                                        //     ligne = myFile.readStringUntil('\n');
                                        //     myFile.peek();
                                        //     cleanLigne(ligne);
                                        // } while ((ligne != "}" && (millis() - startTime < 100000));

                                        buffer_and_HARD_setCursor(0, 4);
                                       //  buffer_clearLine(4);
                                       buffer_clearLine(4); 
                                        buffer_and_HARD_print("GO TO LAYERUP");
                                        delay(watchDelay);

                                        // on a quelles possibiltés  ( ) (      ( ) } {    ( ) } ]

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        buffer_and_HARD_setCursor(0, 2);
                                      //   buffer_clearLine(2);
                                      buffer_clearLine(2); 
                                        buffer_and_HARD_print("last read ");
                                        buffer_and_HARD_print(ligne);
                                        delay(watchDelay);

                                    } while ((ligne.compareTo("{") == 0) && (millis() - startTime < 100000));
                                }
                            }
                        } while (ligne != "]");
                    }
                    else
                    {
                        do
                        {
                            ligne = myFile.readStringUntil('\n');
                            cleanLigne(ligne);
                        } while (ligne != "]");
                    }
                }
                else if (ligne == idBlockCCMod)
                // *************************************************************
                // LECTURE CCMODS
                // *************************************************************
                {
                    buffer_and_HARD_setCursor(0, 2);
                   //  buffer_clearLine(2);
                   buffer_clearLine(2); 
                    buffer_and_HARD_print("Found CCMOD");
                    delay(watchDelay);

                    if (loadCCMods == true)
                    {
                        do
                        { // Debut lecture
                            // lecture d'une ligne
                            ligne = myFile.readStringUntil('\n');
                            cleanLigne(ligne);
                            check = myFile.peek();

                            buffer_clearLine(7);
                            buffer_and_HARD_setCursor(0, 7);
                            buffer_and_HARD_print(ligne.substring(0, 12));
                            delay(watchDelay);

                            if (check != -1)
                            {
                                if (ligne == "{")
                                {
                                    // entrée dans un CCMOD - on devra lire jusqu'à }
                                    buffer_and_HARD_setCursor(0, 2);
                                    buffer_clearLine(2);
                                    buffer_and_HARD_print("ENTER ONE CCMOD");
                                    delay(watchDelay);

                                    do
                                    {
                                        // la suivante doit décrire le numéro du CCMod
                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        int currentUploadCCMod = 0;
                                        currentUploadCCMod = (int)ligne.toFloat() - 1;

                                        buffer_and_HARD_setCursor(0, 2);
                                        buffer_clearLine(2);
                                        buffer_and_HARD_print("upload CCMod");
                                        buffer_and_HARD_print(currentUploadCCMod);
                                        delay(watchDelay);

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        CCModTableau[currentUploadCCMod].type = (byte)ligne.toFloat();
                                        buffer_and_HARD_setCursor(0, 7);
                                        buffer_clearLine(7);
                                        buffer_and_HARD_print("Type" + ligne);
                                        delay(watchDelay);

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        CCModTableau[currentUploadCCMod].sourceMin = (int)ligne.toFloat();
                                        buffer_and_HARD_setCursor(0, 7);
                                        buffer_clearLine(7);
                                        buffer_and_HARD_print("SrcMin" + ligne);
                                        delay(watchDelay);

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        CCModTableau[currentUploadCCMod].sourceMax = (int)ligne.toFloat();
                                        buffer_and_HARD_setCursor(0, 7);
                                        buffer_clearLine(7);
                                        buffer_and_HARD_print("SrcMax" + ligne);
                                        delay(watchDelay);

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        CCModTableau[currentUploadCCMod].targetMin = (int)ligne.toFloat();
                                        buffer_and_HARD_setCursor(0, 7);
                                        buffer_clearLine(7);
                                        buffer_and_HARD_print("TrgMin" + ligne);
                                        delay(watchDelay);

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        CCModTableau[currentUploadCCMod].targetMax = (int)ligne.toFloat();
                                        buffer_and_HARD_setCursor(0, 7);
                                        buffer_clearLine(7);
                                        buffer_and_HARD_print("TrgMax" + ligne);
                                        delay(watchDelay);

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        CCModTableau[currentUploadCCMod].borders = (byte)ligne.toFloat();
                                        buffer_and_HARD_setCursor(0, 7);
                                        buffer_clearLine(7);
                                        buffer_and_HARD_print("Borders" + ligne);
                                        delay(watchDelay);

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        CCModTableau[currentUploadCCMod].idCC = (byte)ligne.toFloat();
                                        buffer_and_HARD_setCursor(0, 7);
                                        buffer_clearLine(7);
                                        buffer_and_HARD_print("idCC" + ligne);
                                        delay(watchDelay);

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        CCModTableau[currentUploadCCMod].channelCC = (byte)ligne.toFloat();
                                        buffer_and_HARD_setCursor(0, 7);
                                        buffer_clearLine(7);
                                        buffer_and_HARD_print("channel" + ligne);
                                        delay(watchDelay);

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        CCModTableau[currentUploadCCMod].layerConcerne = (byte)ligne.toFloat();
                                        buffer_and_HARD_setCursor(0, 7);
                                        buffer_clearLine(7);
                                        buffer_and_HARD_print("layer" + ligne);
                                        delay(watchDelay);

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        CCModTableau[currentUploadCCMod].actif = (byte)ligne.toFloat();
                                        buffer_and_HARD_setCursor(0, 7);
                                        buffer_clearLine(7);
                                        buffer_and_HARD_print("actif " + ligne);
                                        delay(watchDelay);

                                        // do
                                        // { // TANT QUE CARACTERE Après lecture est une parenthèse ouverte (
                                        //     ligne = myFile.readStringUntil('\n');
                                        //     myFile.peek();
                                        //     cleanLigne(ligne);
                                        // } while ((ligne != "}" && (millis() - startTime < 100000));

                                        buffer_and_HARD_setCursor(0, 4);
                                        buffer_clearLine(4);
                                        buffer_and_HARD_print("GO TO LAYERUP");
                                        delay(watchDelay);

                                        // on a quelles possibiltés  ( ) (      ( ) } {    ( ) } ]

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        buffer_and_HARD_setCursor(0, 2);
                                        buffer_clearLine(2);
                                        buffer_and_HARD_print("last read ");
                                        buffer_and_HARD_print(ligne);
                                        delay(watchDelay);

                                    } while ((ligne.compareTo("{") == 0) && (millis() - startTime < 100000));
                                }
                            }
                        } while (ligne != "]");
                    }
                    else
                    {
                        do
                        {
                            ligne = myFile.readStringUntil('\n');
                            cleanLigne(ligne);
                        } while (ligne != "]");
                    }
                }
                else if (ligne == idBlockCell)
                // *************************************************************
                // LECTURE CELLS
                // *************************************************************
                {
                    buffer_and_HARD_setCursor(0, 2);
                    buffer_clearLine(2);
                    buffer_and_HARD_print("Found CELLS");
                    delay(watchDelay);

                    if (loadCells == true)
                    {
                        do
                        { // Debut lecture
                            // lecture d'une ligne
                            ligne = myFile.readStringUntil('\n');
                            cleanLigne(ligne);
                            check = myFile.peek();

                            buffer_clearLine(7);
                            buffer_and_HARD_setCursor(0, 7);
                            buffer_and_HARD_print(ligne.substring(0, 12));
                            delay(watchDelay);

                            if (check != -1)
                            {
                                if (ligne == "{")
                                {
                                    // entrée dans un CELL - on devra lire jusqu'à }
                                    buffer_and_HARD_setCursor(0, 2);
                                    buffer_clearLine(2);
                                    buffer_and_HARD_print("ENTER ONE CCMOD");
                                    delay(watchDelay);

                                    do
                                    {
                                        // la suivante doit décrire le numéro du CCMod
                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        int currentUploadCell = 0;
                                        currentUploadCell = (int)ligne.toFloat();

                                        buffer_and_HARD_setCursor(0, 2);
                                        buffer_clearLine(2);
                                        buffer_and_HARD_print("upload Cell");
                                        buffer_and_HARD_print(currentUploadCell);
                                        delay(watchDelay);

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        tableauCell[currentUploadCell].channelIn = (byte)ligne.toFloat();
                                        buffer_and_HARD_setCursor(0, 7);
                                        buffer_clearLine(7);
                                        buffer_and_HARD_print("CH IN" + ligne);
                                        delay(watchDelay);

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        tableauCell[currentUploadCell].noteIn = (int)ligne.toFloat();
                                        buffer_and_HARD_setCursor(0, 7);
                                        buffer_clearLine(7);
                                        buffer_and_HARD_print("Note IN" + ligne);
                                        delay(watchDelay);

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        tableauCell[currentUploadCell].channelOut = (int)ligne.toFloat();
                                        buffer_and_HARD_setCursor(0, 7);
                                        buffer_clearLine(7);
                                        buffer_and_HARD_print("CH OUT" + ligne);
                                        delay(watchDelay);

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        tableauCell[currentUploadCell].noteOut = (int)ligne.toFloat();
                                        buffer_and_HARD_setCursor(0, 7);
                                        buffer_clearLine(7);
                                        buffer_and_HARD_print("Note OUT" + ligne);
                                        delay(watchDelay);

                                        // do
                                        // { // TANT QUE CARACTERE Après lecture est une parenthèse ouverte (
                                        //     ligne = myFile.readStringUntil('\n');
                                        //     myFile.peek();
                                        //     cleanLigne(ligne);
                                        // } while ((ligne != "}" && (millis() - startTime < 100000));

                                        buffer_and_HARD_setCursor(0, 4);
                                        buffer_clearLine(4);
                                        buffer_and_HARD_print("GO TO LAYERUP");
                                        delay(watchDelay);

                                        // on a quelles possibiltés  ( ) (      ( ) } {    ( ) } ]

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        buffer_and_HARD_setCursor(0, 2);
                                        buffer_clearLine(2);
                                        buffer_and_HARD_print("last read ");
                                        buffer_and_HARD_print(ligne);
                                        delay(watchDelay);

                                    } while ((ligne.compareTo("{") == 0) && (millis() - startTime < 100000));
                                }
                            }
                        } while (ligne != "]");
                    }
                    else
                    {
                        do
                        {
                            ligne = myFile.readStringUntil('\n');
                            cleanLigne(ligne);
                        } while (ligne != "]");
                    }
                }
                else if (ligne == idBlockMidiMatrix)
                // *************************************************************
                // LECTURE MIDI MATRIX
                // *************************************************************
                {
                    buffer_and_HARD_setCursor(0, 2);
                    buffer_clearLine(2);
                    buffer_and_HARD_print("Found MIDI MATRIX");
                    delay(watchDelay);

                    if (loadMidiMatrix == true)
                    {
                        do
                        { // Debut lecture
                            // lecture d'une ligne
                            ligne = myFile.readStringUntil('\n');
                            cleanLigne(ligne);
                            check = myFile.peek();

                            buffer_clearLine(7);
                            buffer_and_HARD_setCursor(0, 7);
                            buffer_and_HARD_print(ligne.substring(0, 12));
                            delay(watchDelay);

                            if (check != -1)
                            {
                                if (ligne == "{")
                                {
                                    // entrée dans un Midi Matrix - on devra lire jusqu'à }
                                    buffer_and_HARD_setCursor(0, 2);
                                    buffer_clearLine(2);
                                    buffer_and_HARD_print("ENTER ONE MIDIMATRIX");
                                    delay(watchDelay);

                                    do
                                    {
                                        // la suivante doit décrire le numéro du CCMod
                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        int currentUploadMidiMatrix = 0;
                                        currentUploadMidiMatrix = (int)ligne.toFloat() - 1;

                                        buffer_and_HARD_setCursor(0, 2);
                                        buffer_clearLine(2);
                                        buffer_and_HARD_print("upload Midi Matrix ");
                                        buffer_and_HARD_print(currentUploadMidiMatrix);
                                        delay(watchDelay);

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        midiRedirect[currentUploadMidiMatrix] = (byte)ligne.toFloat();
                                        buffer_and_HARD_setCursor(0, 7);
                                        buffer_clearLine(7);
                                        buffer_and_HARD_print("CH " + ligne);
                                        delay(watchDelay);

                                        buffer_and_HARD_setCursor(0, 4);
                                        buffer_clearLine(4);
                                        buffer_and_HARD_print("GO TO LAYERUP");
                                        delay(watchDelay);

                                        // on a quelles possibiltés  ( ) (      ( ) } {    ( ) } ]

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        buffer_and_HARD_setCursor(0, 2);
                                        buffer_clearLine(2);
                                        buffer_and_HARD_print("last read ");
                                        buffer_and_HARD_print(ligne);
                                        delay(watchDelay);

                                    } while ((ligne.compareTo("{") == 0) && (millis() - startTime < 100000));
                                }
                            }
                        } while (ligne != "]");
                    }
                    else
                    {
                        do
                        {
                            ligne = myFile.readStringUntil('\n');
                            cleanLigne(ligne);
                        } while (ligne != "]");
                    }
                }
                else if (ligne == idBlockNoteRedirect)
                // *************************************************************
                // LECTURE NOTE REDIRECT
                // *************************************************************
                {
                    buffer_and_HARD_setCursor(0, 2);
                    buffer_clearLine(2);
                    buffer_and_HARD_print("Found Note Redirect");
                    delay(watchDelay);

                    if (loadNoteRedirect == true)
                    {
                        do
                        { // Debut lecture
                            // lecture d'une ligne
                            ligne = myFile.readStringUntil('\n');
                            cleanLigne(ligne);
                            check = myFile.peek();

                            buffer_clearLine(7);
                            buffer_and_HARD_setCursor(0, 7);
                            buffer_and_HARD_print(ligne.substring(0, 12));
                            delay(watchDelay);

                            if (check != -1)
                            {
                                if (ligne == "{")
                                {
                                    // entrée dans un Note Redirect - on devra lire jusqu'à }
                                    buffer_and_HARD_setCursor(0, 2);
                                    buffer_clearLine(2);
                                    buffer_and_HARD_print("ENTER ONE NOTEREDIRECT");
                                    delay(watchDelay);

                                    do
                                    {
                                        // la suivante doit décrire le numéro du CCMod
                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        int currentUploadNoteRedirect = 0;
                                        currentUploadNoteRedirect = (int)ligne.toFloat() - 1;

                                        buffer_and_HARD_setCursor(0, 2);
                                        buffer_clearLine(2);
                                        buffer_and_HARD_print("upload Note Redirect");
                                        buffer_and_HARD_print(currentUploadNoteRedirect);
                                        delay(watchDelay);

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        noteRedirect[currentUploadNoteRedirect] = (byte)ligne.toFloat();
                                        buffer_and_HARD_setCursor(0, 7);
                                        buffer_clearLine(7);
                                        buffer_and_HARD_print("Note" + ligne);
                                        delay(watchDelay);

                                        // do
                                        // { // TANT QUE CARACTERE Après lecture est une parenthèse ouverte (
                                        //     ligne = myFile.readStringUntil('\n');
                                        //     myFile.peek();
                                        //     cleanLigne(ligne);
                                        // } while ((ligne != "}" && (millis() - startTime < 100000));

                                        buffer_and_HARD_setCursor(0, 4);
                                        buffer_clearLine(4);
                                        buffer_and_HARD_print("GO TO LAYERUP");
                                        delay(watchDelay);

                                        // on a quelles possibiltés  ( ) (      ( ) } {    ( ) } ]

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        buffer_and_HARD_setCursor(0, 2);
                                        buffer_clearLine(2);
                                        buffer_and_HARD_print("last read ");
                                        buffer_and_HARD_print(ligne);
                                        delay(watchDelay);

                                    } while ((ligne.compareTo("{") == 0) && (millis() - startTime < 100000));
                                }
                            }
                        } while (ligne != "]");
                    }
                    else
                    {
                        do
                        {
                            ligne = myFile.readStringUntil('\n');
                            cleanLigne(ligne);
                        } while (ligne != "]");
                    }
                }
                else if (ligne == idBlockLFOs)
                // *************************************************************
                // LECTURE LFOs
                // *************************************************************
                {
                    buffer_and_HARD_setCursor(0, 2);
                    buffer_clearLine(2);
                    buffer_and_HARD_print("Found LFOs");
                    delay(watchDelay);

                    if (loadLFOs == true)
                    {
                        do
                        { // Debut lecture
                            // lecture d'une ligne
                            ligne = myFile.readStringUntil('\n');
                            cleanLigne(ligne);
                            check = myFile.peek();

                            buffer_clearLine(7);
                            buffer_and_HARD_setCursor(0, 7);
                            buffer_and_HARD_print(ligne.substring(0, 12));
                            delay(watchDelay);

                            if (check != -1)
                            {
                                if (ligne == "{")
                                {
                                    // entrée dans un LFO - on devra lire jusqu'à }
                                    buffer_and_HARD_setCursor(0, 2);
                                    buffer_clearLine(2);
                                    buffer_and_HARD_print("ENTER ONE LFO");
                                    delay(watchDelay);

                                    do
                                    {
                                        // la suivante doit décrire le numéro de l'event
                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        int currentUploadLFO = 0;
                                        currentUploadLFO = (int)ligne.toFloat() - 1;

                                        buffer_and_HARD_setCursor(0, 2);
                                        buffer_clearLine(2);
                                        buffer_and_HARD_print("upload LFO");
                                        buffer_and_HARD_print(currentUploadLFO);
                                        delay(watchDelay);

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        LFO[currentUploadLFO].actif = (byte)ligne.toFloat();
                                        buffer_and_HARD_setCursor(0, 7);
                                        buffer_clearLine(7);
                                        buffer_and_HARD_print("AC" + ligne);
                                        delay(watchDelay);

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        LFO[currentUploadLFO].type = (byte)ligne.toFloat();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        LFO[currentUploadLFO].shape = (byte)ligne.toFloat();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        LFO[currentUploadLFO].fidelite = (long)ligne.toFloat();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        LFO[currentUploadLFO].duration = (long)ligne.toFloat();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        LFO[currentUploadLFO].decalage = (byte)ligne.toFloat();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        LFO[currentUploadLFO].lowest = (int)ligne.toFloat();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        LFO[currentUploadLFO].highest = (int)ligne.toFloat();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        LFO[currentUploadLFO].channel = (byte)ligne.toFloat();
                                        buffer_and_HARD_setCursor(0, 7);
                                        buffer_clearLine(7);
                                        buffer_and_HARD_print("CH" + ligne);
                                        delay(watchDelay);

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        LFO[currentUploadLFO].which = (byte)ligne.toFloat();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        LFO[currentUploadLFO].typeMachine = (int)ligne.toFloat();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        LFO[currentUploadLFO].layer = (int)ligne.toFloat();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        LFO[currentUploadLFO].modC = (byte)ligne.toFloat();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        LFO[currentUploadLFO].modV = (byte)ligne.toFloat();

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        LFO[currentUploadLFO].LFORandom = (byte)ligne.toFloat();

                                        // do
                                        // { // TANT QUE CARACTERE Après lecture est une parenthèse ouverte (
                                        //     ligne = myFile.readStringUntil('\n');
                                        //     myFile.peek();
                                        //     cleanLigne(ligne);
                                        // } while ((ligne != "}" && (millis() - startTime < 100000));

                                        buffer_and_HARD_setCursor(0, 4);
                                        buffer_clearLine(4);
                                        buffer_and_HARD_print("GO TO LFO UP");
                                        delay(watchDelay);

                                        // on a quelles possibiltés  ( ) (      ( ) } {    ( ) } ]

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        buffer_and_HARD_setCursor(0, 2);
                                        buffer_clearLine(2);
                                        buffer_and_HARD_print("last read ");
                                        buffer_and_HARD_print(ligne);
                                        delay(watchDelay);

                                    } while ((ligne.compareTo("{") == 0) && (millis() - startTime < 100000));
                                }
                            }
                        } while (ligne != "]");
                    }
                    else
                    {
                        do
                        {
                            ligne = myFile.readStringUntil('\n');
                            cleanLigne(ligne);
                        } while (ligne != "]");
                    }
                }
                else if (ligne == idBlockKarakt) // 2C22
                // *************************************************************
                // LECTURE Karakt
                // *************************************************************
                {
                    buffer_and_HARD_setCursor(0, 2);
                    buffer_clearLine(2);
                    buffer_and_HARD_print("Found KARAKT!!!");
                    delay(watchDelay);

                    if (loadKarakt == true)
                    {
                        do
                        { // Debut lecture
                            // lecture d'une ligne
                            ligne = myFile.readStringUntil('\n');
                            cleanLigne(ligne);
                            check = myFile.peek();

                            buffer_clearLine(7);
                            buffer_and_HARD_setCursor(0, 7);
                            buffer_and_HARD_print(ligne.substring(0, 12));
                            delay(watchDelay);

                            if (check != -1)
                            {
                                if (ligne == "{")
                                {
                                    // entrée dans un Karakt - on devra lire jusqu'à }
                                    buffer_and_HARD_setCursor(0, 2);
                                    buffer_clearLine(2);
                                    Serial.println("ENTER ONE KARAKT");
                                    delay(watchDelay);

                                    do
                                    {
                                        // la suivante doit décrire le numéro de Karakt
                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);
                                        int currentUploadKarakt = 0;
                                        currentUploadKarakt = (int)ligne.toFloat();
                                        Serial.println(currentUploadKarakt); 

                                        // on va charger dans currentUploadKarakt 

                                        buffer_and_HARD_setCursor(0, 2);
                                        buffer_clearLine(2);
                                        Serial.println("upload Karakt ");
                                        Serial.println(currentUploadKarakt);
                                        delay(watchDelay);

                                        // APRES ICI ON A UN NOUVELLE FACON DE CONCEVOIR LE TRAVAIL DE LOAD 2C22

                                        // VIEUX STYLE
                                        // ligne = myFile.readStringUntil('\n');
                                        // myFile.peek();
                                        // cleanLigne(ligne);
                                        // LFO[currentUploadLFO].actif = (byte)ligne.toFloat();
                                        // buffer_and_HARD_setCursor(0, 7);
                                        // buffer_clearLine(7);
                                        // buffer_and_HARD_print("AC" + ligne);
                                        // delay(watchDelay);

                                        // REPETITION DE N fois ceci, mais devant être tout à fait réglo :
                                        // ligne = myFile.readStringUntil('\n');
                                        // myFile.peek();
                                        // cleanLigne(ligne);
                                        // LFO[currentUploadLFO].which = (byte)ligne.toFloat();

                                        // ici, on va lire une ligne
                                        // si la ligne simplifiée est } alors on a fini
                                        // sinon, il faut traiter la ligne et lire la suivante

                                        bool keepon = true;
                                        int compteurWhile = 0;

                                        while (keepon == true)
                                        {
                                            ligne = myFile.readStringUntil('\n');
                                            myFile.peek();
                                            delay(watchDelay);

                                            String copie;
                                            copie = ligne;
                                            cleanLigne(ligne);

                                            //  buffer_and_HARD_setCursor(0, 2);
                                            //  buffer_clearLine(2);
                                            //  buffer_and_HARD_print(ligne);
                                            delay(watchDelay);

                                            if (ligne == "}" || compteurWhile > 10000)
                                            {
                                                keepon = false;
                                            }
                                            else
                                            {
                                                // traitement de la ligne et de copie
                                                /*
                                                Serial.println(copie);
                                                Serial.print("makes ");
                                                Serial.print(ligne);
                                                Serial.println(" as data ");
                                                */
                                                karaktValue = (int)ligne.toFloat();

                                                // Serial.print("[] is ");
                                                // Serial.println(getBracketsFromLigne(copie));
                                                karaktNumber = (int)getBracketsFromLigne(copie).toInt();

                                                // Serial.print("() is ");
                                                // Serial.println(getParenthesesFromLigne(copie));
                                                karaktLayer = (int)getParenthesesFromLigne(copie).toInt();

                                                // Serial.println("------------");
                                                compteurWhile++;
                                                delay(watchDelay);

                                                Serial.print(karaktValue); 
                                                Serial.print("-");
                                                Serial.print(karaktNumber);
                                                Serial.print("-");
                                                Serial.println(karaktLayer); 

                                                // RESTE plus qu'à
                                                // attribuer la valeur [] et () à une variable
                                                // effectuer un tri sur ces valeurs pour les mettre dans les bonnes variables

                                                // la fc ci après vérifie que karaktNumber est égal au premier
                                                // si, oui dans valeur [karaktLayer] va mettre la valeur de karaktValue dans la variable indiquée

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
                                                int_loadFromKarakt(121, &gabberito2Interval );
                                                int_loadFromKarakt(122, &gabberito2K );
                                                int_loadFromKarakt(123, &gabberito2N );

                                                int_loadFromKarakt(124, &accentMax );
                                                int_loadFromKarakt(125, &accentMin );

                                                // Gabberito
                                                int_loadFromKarakt(126, &gabberitoInterval );

                                                int_loadFromKarakt(127, &CHMute );

                                                // STEPZBUG
                                                bool_loadFromKarakt(128, &keepStepzBUG );

                                                for (int i = 0; i < nbSequencesMax; i++)
                                                {
                                                    single_bool_loadFromKarakt(129, i, &sequence[i].actif );
                                                }

                                                for (int i = 0; i < NbCCModMax; i++)
                                                {
                                                    single_bool_loadFromKarakt(130, i, &CCModTableau[i].actif );
                                                }
                                                for (int i = 0; i < NbEvenementMax; i++)
                                                {
                                                    single_bool_loadFromKarakt(131, i, &Evenement[i].actif );
                                                }
                                                for (int i = 0; i < nbLFOMax; i++)
                                                {
                                                    single_bool_loadFromKarakt(132, i, &LFO[i].actif );
                                                }


                                            }
                                        }

                                        // on doit avoir lu la ligne avec } // --------------

                                        buffer_and_HARD_setCursor(0, 4);
                                        buffer_clearLine(4);
                                        buffer_and_HARD_print("GO TO KARAKT UP");
                                        buffer_and_HARD_print(String(currentUploadKarakt)); 
                                        delay(watchDelay);

                                        // ligne = myFile.readStringUntil('\n');
                                        // myFile.peek();
                                        // cleanLigne(ligne);

                                        ligne = myFile.readStringUntil('\n');
                                        myFile.peek();
                                        cleanLigne(ligne);

                                        buffer_and_HARD_setCursor(0, 2);
                                        buffer_clearLine(2);
                                        buffer_and_HARD_print("last read ");
                                        buffer_and_HARD_print(ligne);
                                        delay(watchDelay);

Serial.println("saving to KMem"); 

if ( currentUploadKarakt == 0 )
{
    saveKarakt_to_KMem(63);
} 
else 
{
                                        saveKarakt_to_KMem(currentUploadKarakt - 1); 
}

                                    } while ((ligne.compareTo("{") == 0) && (millis() - startTime < 100000));
                                }
                            }
                        } while (ligne != "]");
                    }
                    else
                    {
                        do
                        {
                            ligne = myFile.readStringUntil('\n');
                            cleanLigne(ligne);
                        } while (ligne != "]");
                    }
                }
                else

                {
                    buffer_and_HARD_setCursor(0, 2);
                    buffer_clearLine(2);
                    buffer_and_HARD_print("UNKNOWN");
                    delay(watchDelay);
                    do
                    {
                        ligne = myFile.readStringUntil('\n');
                        cleanLigne(ligne);
                    } while (ligne != "]");
                }
            }
        }
    }                    // fin de lecture
    while (check != -1); // ce qui signifierait problème ou fin de fichier

    buffer_and_HARD_setCursor(0, 2);
    buffer_clearLine(2);
    buffer_and_HARD_print("FINISHED READING");
    delay(watchDelay);

    // close the file:
    myFile.close();
    // u8x8.clear();
    buffer_clearScreen_HARD(); 
    buffer_and_HARD_setCursor(0, 0);
    buffer_and_HARD_print(F("***LOADED***"));
    delay(watchDelay);

    // https://www.arduino.cc/en/Serial/ReadStringUntil

    // fin de ligne = "\r\n"
    // TAB =  '\t"
    // Prints data to the serial port as human-readable ASCII text followed by a carriage return character (ASCII 13, or '\r') and a newline character (ASCII 10, or '\n'). This command takes the same forms as Serial.print().
}

void int_printToKarakt(int number, int value, String comment)
{
    myFile.print("\t\t");
    myFile.print(String(value));
    myFile.print("\t");
    myFile.print("// [");
    myFile.print(String(number));
    myFile.print("] ");
    myFile.print("(0)");
    myFile.println(comment);
}

void bool_printToKarakt(int number, bool value, String comment)
{
    myFile.print("\t\t");
    myFile.print(String(value));
    myFile.print("\t");
    myFile.print("// [");
    myFile.print(String(number));
    myFile.print("] ");
    myFile.print("(0)");
    myFile.println(comment);
}

void multi_int_printToKarakt(int number, int nbLayer, int value[], const String &comment)
{
    for (int i = 0; i < nbLayer; i++)
    {
        myFile.print("\t\t");
        myFile.print(String(value[i]));
        myFile.print("\t");
        myFile.print("// [");
        myFile.print(String(number));
        myFile.print("] ");
        myFile.print("(");
        myFile.print(String(i));
        myFile.print(") ");
        myFile.println(comment);
    }
}

void multi_bool_printToKarakt(int number, int nbLayer, bool value[], const String &comment)
{
    for (int i = 0; i < nbLayer; i++)
    {
        myFile.print("\t\t");
        myFile.print(String(value[i]));
        myFile.print("\t");
        myFile.print("// [");
        myFile.print(String(number));
        myFile.print("] ");
        myFile.print("(");
        myFile.print(String(i));
        myFile.print(") ");
        myFile.println(comment);
    }
}

void single_bool_printToKarakt(int number, int nbLayer, bool value , const String &comment)
{
     int i =  nbLayer;  
    {
        myFile.print("\t\t");
        myFile.print(String(value ));
        myFile.print("\t");
        myFile.print("// [");
        myFile.print(String(number));
        myFile.print("] ");
        myFile.print("(");
        myFile.print(String(i));
        myFile.print(") ");
        myFile.println(comment);
    }
}

void saveCurrentKaraktDISMISS(bool saveKarakt = true)   //2C26 au profit de SaveLocal, qui sauve current + mem 
{
    // fileString = createTHXname();
    fileString = "K_" + fileNameSave;

    // nouveau nom du fichier : reste idem, sauf si on demande à changer de nom
    // on va donc clairement écraser les anciens fichiers, plutôt que d'en créer de nouveaux à chaque fois

/*
    myFile = SD.open(fileString, FILE_WRITE);
*/

    // myFile.remove(fileString);
    // myFile = sd.open(fileString, FILE_WRITE);

    // if the file opened okay, write to it:
    if (myFile.isOpen())
    {
        myFile.remove(); // 1C86
        myFile.close();
    }

/*
    myFile = sd.open(fileString, FILE_WRITE);
*/

    // if the file opened okay, write to it:
    if (myFile)
    {
        // buffer_clearScreen_HARD();
buffer_clearScreen_HARD (); 
        
        buffer_and_HARD_setCursor(0, 0);
        buffer_and_HARD_print(F("Opened OK "));
        buffer_and_HARD_setCursor(0, 1);
        buffer_and_HARD_print(fileString);
        delay(watchDelay);
    }
    else
    {
        buffer_clearScreen_HARD();
        buffer_and_HARD_setCursor(0, 0);
        buffer_and_HARD_print(F("PROBLEM at Open "));
        buffer_and_HARD_setCursor(0, 1);
        buffer_and_HARD_print(fileString);
        delay(watchDelay);
    }

    myFile.println("C\t//VERSION");
    // lire en première lettre un B indique que c'est ce modèle-ci de fichier
    // modifié à partir de 1C99

    // écriture des layers DONE
    // écriture des séquences DONE
    // écriture des events DONE
    // écriture de midimatrix
    // écriture de Cell
    // écriture CCMod DONE

    // que faut-il écrire des infos générales, et par layer ?
    // ecriture CHARACTERISTICS

    // KARAKT
    // je le développe ici en pensant sans doute l'incorporer un jour à Save Local

    if (saveKarakt == true)
    {
        myFile.println("[\t//KARAKT START");
        myFile.print("\t");
        myFile.println(idBlockKarakt);

        for (unsigned int i = 0; i < 1; i++) // plus tard, parcours de pile de Karakt
        {
            // unsigned int taille = Evenement[i].type;
            // si l'event est vide, type vaut 0

            // if (taille > 0)
            {
                myFile.print("\t{\t//KARAKT ");
                myFile.print(String(i + 1));

                myFile.println(idRecordKarakt_Start);

                myFile.print("\t\t");
                myFile.print(String(i + 1));
                myFile.println(idRecordKarakt_Number);

                // insérer ici les caractéristiques de chaque karakt, l'une après l'autre, avec idTag
                // dans le cas de Karakt on ne va pas tout nommer, à priori, mais mettre un chiffre entre []
                // ainsi on aura :   valeur      // [chiffre]
                // et éventuellement : valeur    // [chiffre] delay 1
                // en sachant qu'un contrôle backwards va ici s'effectuer : c'est [chiffre] et pas l'ordre qui servira
                // à mettre dans une variable la valeur

                // pour l'instant l'idTag est un peu inutile, mais une version ultérieure reconnaîtra peut être le contenu à partir du Tag,
                // et non pas de l'emplacement.

                // pour Karakt il y a tant de données à sauver qu'il me paraît plus raisonnable de
                // faire un idTag qui en effet permettre la reconnaissance de la valeur
                // mais sans spécifier in extenso quelle en est la signification
                // ce sera plutôt pour le manuel d'emploi
                // on aurait dès lors    -   valeur    // [103]
                // serait plus simple à coder mais moins facile à hacker

                //   bool actif ; // rajouté en nov2018, sur le modèle de Sequence

                /*
                myFile.print("\t\t");
                myFile.print(String(Evenement[i].actif));
                myFile.print("\t");
                myFile.println(idRecordEvenement_actif);
 */

                int_printToKarakt(1, tailleChaineMarkov, "taille Markov");
                multi_int_printToKarakt(2, nbLayer, nbDeDelais, "nb delais");
                multi_bool_printToKarakt(3, nbLayer, delayLayer, "delay?");
                multi_int_printToKarakt(4, nbLayer, nbDeMsecPourDelais, "nb msec");
                multi_int_printToKarakt(5, nbLayer, dropPourDelais, "drop");
                multi_int_printToKarakt(6, nbLayer, downPourDelais, "down");
                multi_int_printToKarakt(7, nbLayer, randomPourDelais, "rnd");
                // multi_int_printToKarakt(8,nbLayer,bitMapDelay,"bitmap");  // d'abord transformer bitmapDelay en [] << i
                multi_int_printToKarakt(9, nbLayer, repeaterNb, "repeater");
                multi_int_printToKarakt(10, nbLayer, seqBitMapDelay, "seqBitMap");

                multi_bool_printToKarakt(11, nbLayer, dende, "on off");
                multi_int_printToKarakt(12, nbLayer, dendeSec, "sec");
                multi_int_printToKarakt(13, nbLayer, dendeMSec, "msec");
                multi_int_printToKarakt(14, nbLayer, dendeDivide, "divid");
                multi_int_printToKarakt(15, nbLayer, dendeDrop, "drop"); // DEBUG il faudra modifier, pour avoir 0 - 100 - 200
                multi_int_printToKarakt(16, nbLayer, dendeDown, "down"); // DEBUG il faudra modifier, pour avoir 0 - 100 - 200
                multi_int_printToKarakt(17, nbLayer, dendeLevels, "levels");
                multi_int_printToKarakt(18, nbLayer, dendeDecay, "decay");
                multi_int_printToKarakt(19, nbLayer, dendeStart, "start");
                multi_int_printToKarakt(20, nbLayer, dendeEnd, "end");
                multi_int_printToKarakt(21, nbLayer, dendeRatio, "ratio");
                multi_int_printToKarakt(22, nbLayer, dendeStretch, "stretch");
                multi_int_printToKarakt(23, nbLayer, dendePitch, "pitch");
                multi_int_printToKarakt(24, nbLayer, dendeFidelity, "fidelity");
                int_printToKarakt(25, overLoadLimit, "limit");
                multi_int_printToKarakt(26, nbLayer, dendeRetard, "retard");
                multi_int_printToKarakt(27, nbLayer, dendeGamme, "gamme");
                multi_int_printToKarakt(28, nbLayer, dendeMode, "mode");
                multi_int_printToKarakt(29, nbLayer, dendeDegres, "degres");
                multi_int_printToKarakt(30, nbLayer, dendeExpand, "expand");
                multi_int_printToKarakt(31, nbLayer, dendeArp1, "arp1");
                multi_int_printToKarakt(32, nbLayer, dendeArp2, "arp2");
                multi_int_printToKarakt(33, nbLayer, dendeNoteOff, "noteOff");    // sera à préciser
                multi_int_printToKarakt(34, nbLayer, dendeSubFidelity, "subFid"); // TODO prévoir fc pour modifier
                multi_int_printToKarakt(35, nbLayer, repeaterNbDende, "repeat");
                // multi_int_printToKarakt(36, _bitMapDelay[nbLayer][8] = {1}; // à initialiser .  si vaut 1 delay a lieu, si vaut 0 pas
                // pour au-delà de 8 delay : %8 !
                multi_int_printToKarakt(37, nbLayer, dendeBitMapDelay, "bitmap");

                // 26 mars 2020 - en plein dans les CoronaDayz

                multi_bool_printToKarakt(38, nbLayer, omitEnd, "omit");
                multi_int_printToKarakt(39, nbLayer, omitEndValue, "omit%");

                // DRUNK
                multi_bool_printToKarakt(40, nbLayer, drunk, "drunk");
                multi_int_printToKarakt(41, nbLayer, drunkMin, "min");
                multi_int_printToKarakt(42, nbLayer, drunkMax, "max");
                multi_int_printToKarakt(43, nbLayer, drunkStep, "step");
                multi_int_printToKarakt(44, nbLayer, currentDrunkValue, "current"); // PERTINENCE ?

                // NEW Global Markov
                int_printToKarakt(45, tailleChaineMarkov, "Markov taille");
                bool_printToKarakt(46, markovNEWChainGlobal, "Markov ON");
                int_printToKarakt(47, currentTailleNEWMarkovTable, "current taille");
                int_printToKarakt(48, markovNEWTailleMax, "new taille");
                multi_bool_printToKarakt(49, nbLayer, participateNEWMarkov, "participate GLO");
                multi_bool_printToKarakt(50, nbLayer, participateLAYERMarkov, "participate LAY");
                multi_int_printToKarakt(51, nbLayer, currentSizeLayerMarkov, "size");
                multi_int_printToKarakt(52, nbLayer, markovLayerMax, "layerMax");

                //Barocco
                multi_bool_printToKarakt(53, nbLayer, baroccoLayer, "barocco");
                multi_int_printToKarakt(54, nbLayer, baroccoChosenSeq, "seq");
                multi_int_printToKarakt(55, nbLayer, nbDeMsecPourBarocco, "msec");
                multi_int_printToKarakt(56, nbLayer, dropPourBarocco, "drop");
                multi_int_printToKarakt(57, nbLayer, randomPourBarocco, "rnd");
                multi_int_printToKarakt(58, nbLayer, symHBarocco, "symH");
                multi_int_printToKarakt(59, nbLayer, symVBarocco, "symV");
                multi_int_printToKarakt(60, nbLayer, refletBarocco, "refl");
                multi_int_printToKarakt(61, nbLayer, preReflet, "preRefl");
                multi_int_printToKarakt(62, nbLayer, preH, "preH");
                multi_int_printToKarakt(63, nbLayer, preV, "preV");

                // VARIATIONS
                multi_bool_printToKarakt(64, nbLayer, variationLayer, "variations");
                multi_int_printToKarakt(65, nbLayer, percentDisparitionChanceVariation, "%disappear");
                multi_int_printToKarakt(66, nbLayer, minimumNotesVariation, "min");
                multi_int_printToKarakt(67, nbLayer, maximumReduxVariation, "maxRedux");
                multi_bool_printToKarakt(68, nbLayer, modeMinimumVariation, "modeMinimum");
                multi_int_printToKarakt(69, nbLayer, modeVariation, "modeVar");

                multi_bool_printToKarakt(70, nbLayer, pitchMicroSamplerOnChannel, "pitchMS");
                multi_bool_printToKarakt(71, nbLayer, pitchMicroSamplerOnLayer, "pitchLayer");
                multi_int_printToKarakt(72, nbLayer, pitchMicroSampler, "pitchMS");

                multi_bool_printToKarakt(73, nbLayer, dynamicSwing, "dynSwing");
                multi_int_printToKarakt(74, nbLayer, valueDynamicSwing, "valueSwing");

                // DIVERS

                multi_int_printToKarakt(75, nbLayer, fidelityWholeLayer, "fidelity whole layer");
                multi_bool_printToKarakt(76, nbLayer, isWholeLayerFidelityMuted, "fidelity muted whole"); //

                // modulo layer
                multi_int_printToKarakt(77, nbLayer, eclipseMode, "eclipse");         //  1 = mute selected    2 = mute all except selected
                int_printToKarakt(78, eclipseModulo, "modulo");                       //
                int_printToKarakt(79, eclipseK, "K");                                 //
                multi_int_printToKarakt(80, nbLayer, eclipseModuloTab, "modulo tab"); //
                multi_int_printToKarakt(81, nbLayer, eclipseKTab, "K tab");           //
                bool_printToKarakt(82, modeConstipe, "constip");                      //

                // dynramic harmonize
                int_printToKarakt(83, dynamicHarmonizeMode, "dynamic harmonize mode");    //
                multi_bool_printToKarakt(84, nbLayer, harmonizeLayer, "harmonize layer"); //

                multi_int_printToKarakt(85, nbLayer, DureePole1, "duree pol1"); //
                multi_int_printToKarakt(86, nbLayer, minPole1, "min p1");       //
                multi_int_printToKarakt(87, nbLayer, maxPole1, "max p1");       //
                multi_int_printToKarakt(88, nbLayer, DureePole2, "duree pol2"); //
                multi_int_printToKarakt(89, nbLayer, minPole2, "min p2");       //
                multi_int_printToKarakt(90, nbLayer, maxPole2, "max p2");       //
                multi_int_printToKarakt(91, nbLayer, freqPole1, "freq p1");     //

                bool_printToKarakt(92, turnOnBipolarRandom, "bip rnd"); //

                bool_printToKarakt(93, FunkyVel, "funky Vel");                          //  // DEBUG : pourquoi pas lié à layer ???
                multi_int_printToKarakt(94, nbLayer, FunkyVelValue, "funky vel Value"); //

                // autoharmonize
                bool_printToKarakt(95, autoHarmonize, "auto Harmo");        //
                multi_int_printToKarakt(96, nbLayer, modeActuelle, "mode"); //// mémorise le mode actuelle - modif 2C12

                multi_bool_printToKarakt(97, nbLayer, pitchableLayer, "pitchable layer"); //
                multi_bool_printToKarakt(98, 16, pitchableChannel, "pitchable channel");  //

                // syncop BUG
                bool_printToKarakt(99, KeepSyncopBUG, "keep syncop bug"); //

                multi_int_printToKarakt(100, nbLayer, poof, "poof"); //
                bool_printToKarakt(101, turnOnPoof, "turn on poof"); //  // DEBUG : pourquoi pas Layered ????

                int_printToKarakt(102, nSwing, "n Swing"); // // par défaut

                multi_int_printToKarakt(103, nbLayer, gammeActuelle, "gamme Actuelle"); //
                multi_int_printToKarakt(104, 12, noteRedirect, "note Redirect");        //   // contient la matrice de pitch des notes

                // 27 mars 2020 - en plein dans les CoronaDayz

                // KANNON multi layer
                multi_bool_printToKarakt(105, nbLayer, kannonActif, "kannon");
                multi_int_printToKarakt(106, nbLayer, kannonRevert, "rev");
                multi_int_printToKarakt(107, nbLayer, kannonReflet, "ref");
                multi_int_printToKarakt(108, nbLayer, kannonPitch, "pitch");
                multi_int_printToKarakt(109, nbLayer, kannonDelay, "delay");
                multi_int_printToKarakt(110, nbLayer, kannonRandom, "rnd");
                multi_int_printToKarakt(111, nbLayer, kannonStretchRatio, "stretch");

                // PAN + DERAZER
                multi_bool_printToKarakt(112, 16, panMicroSamplerOnChannel, "panMS CH");
                multi_bool_printToKarakt(113, nbLayer, panMicroSamplerOnLayer, "panMS Lay");
                multi_int_printToKarakt(114, nbLayer, panMicroSampler, "pan");

                // GABBERISM
                int_printToKarakt(115, gabberismLevels, "gabb Lvl");
                int_printToKarakt(116, gabberismDivisions, "gabb Div");
                int_printToKarakt(117, gabberismDivRatio, "gabb Div ratio");
                int_printToKarakt(118, gabberismRepeats, "gabb repeatz");
                int_printToKarakt(119, gabberismRepeatRatio, "gabb repeazt ratio");

                multi_int_printToKarakt(120, nbLayer, fidelityLayer_, "FIDELITY");

                myFile.print("\t}\t//KARAKT ");
                myFile.print(String(i + 1));
                myFile.println(idRecordKarakt_End);
            }
        }

       

        myFile.println("]\t//KARAKT END");
    }

    myFile.println(F(""));
    myFile.println(F(""));

    myFile.println(F(""));
    myFile.println(F("Created by Piccolo "));
    myFile.println(PiccoloVersion);
    myFile.println(PiccoloDate);
    myFile.println(F(""));
    myFile.println(F("you can change numbers but not the structure"));
    myFile.println(F("do not insert or delete lines or TAB"));
    myFile.println(F("you can experiment with numbers though !"));
    myFile.println(F("dont modify anything between [ ]"));
    myFile.println(F("now you can be an errorist"));

    // close the file:
    myFile.close();
    buffer_and_HARD_setCursor(0, 0);
    buffer_and_HARD_print(F("Saved  "));
    delay(watchDelay);

    for (byte i = 0; i < nbLayer; i++)
    {
        ZEROupdatePileTimeToPlayMODEDIRECT(i);
    }

    rootFileCount++;
    measureSDFileCount();
}

void fromKMem_int_printToKarakt(int number, int value, String comment)
{
    if (karaktNumber == number)
    {
        myFile.print("\t\t");
      //  myFile.print(String(value));
        myFile.print(String(karaktValue));
        myFile.print("\t");
        myFile.print("// [");
        myFile.print(String(number));
        myFile.print("] ");
        myFile.print("(0)");
        myFile.println(comment);
    }
}

void fromKMem_bool_printToKarakt(int number, bool value, String comment)
{
    if (karaktNumber == number)
    {
        myFile.print("\t\t");
       // myFile.print(String(value));
        myFile.print(String(karaktValue));
        myFile.print("\t");
        myFile.print("// [");
        myFile.print(String(number));
        myFile.print("] ");
        myFile.print("(0)");
        myFile.println(comment);
    }
}

void fromKMem_multi_int_printToKarakt(int number, int nbLayer, int value[], const String &comment)
{
    if (karaktNumber == number)
    {
       // for (int i = 0; i < nbLayer; i++)
        {
            myFile.print("\t\t");
            // myFile.print(String(value[i]));
            myFile.print(String(karaktValue));
            myFile.print("\t");
            myFile.print("// [");
            myFile.print(String(number));
            myFile.print("] ");
            myFile.print("(");
            myFile.print(String(karaktLayer));
            myFile.print(") ");
            myFile.println(comment);
        }
    }
}

void fromKMem_multi_bool_printToKarakt(int number, int nbLayer, bool value[], const String &comment)
{
    if (karaktNumber == number)
    {
        // for (int i = 0; i < nbLayer; i++)
        {
            myFile.print("\t\t");
           // myFile.print(String(value[i]));
           myFile.print(String(karaktValue)); 
            myFile.print("\t");
            myFile.print("// [");
            myFile.print(String(number));
            myFile.print("] ");
            myFile.print("(");
            myFile.print(String(karaktLayer));
            myFile.print(") ");
            myFile.println(comment);
        }
    }
}

void fromKMem_single_bool_printToKarakt(int number, int nbLayer, bool value , const String &comment)
{
     if (karaktNumber == number && karaktLayer == nbLayer )
    /// if (karaktNumber == number )
    {
        // for (int i = 0; i < nbLayer; i++)
        {
            myFile.print("\t\t");
            // myFile.print(String(value[i]));
            myFile.print(String(karaktValue));
            myFile.print("\t");
            myFile.print("// [");
            myFile.print(String(number));
            myFile.print("] ");
            myFile.print("(");
            myFile.print(String(karaktLayer));
            myFile.print(") ");
            myFile.println(comment);
        }
    }
}


void saveCurrentandMemKarakt(bool saveKarakt   , bool includeHeader   )
{
    if ( includeHeader )
    {
    // fileString = createTHXname();
    fileString = "K_" + fileNameSave;

    // nouveau nom du fichier : reste idem, sauf si on demande à changer de nom
    // on va donc clairement écraser les anciens fichiers, plutôt que d'en créer de nouveaux à chaque fois

/*
    myFile = SD.open(fileString, FILE_WRITE);
 */
    // myFile.remove(fileString);
    // myFile = sd.open(fileString, FILE_WRITE);

    // if the file opened okay, write to it:
    if (myFile.isOpen())
    {
        myFile.remove(); // 1C86
        myFile.close();
    }

/*
    myFile = sd.open(fileString, FILE_WRITE);
*/

    // if the file opened okay, write to it:
    if (myFile)
    {
        buffer_clearScreen_HARD();
        buffer_and_HARD_setCursor(0, 0);
        buffer_and_HARD_print(F("Opened OK "));
        buffer_and_HARD_setCursor(0, 1);
        buffer_and_HARD_print(fileString);
        delay(watchDelay);
    }
    else
    {
        buffer_clearScreen_HARD();
        buffer_and_HARD_setCursor(0, 0);
        buffer_and_HARD_print(F("PROBLEM at Open "));
        buffer_and_HARD_setCursor(0, 1);
        buffer_and_HARD_print(fileString);
        delay(watchDelay);
    }

    myFile.println("C\t//VERSION");
    }

    // lire en première lettre un B indique que c'est ce modèle-ci de fichier
    // modifié à partir de 1C99

    // écriture des layers DONE
    // écriture des séquences DONE
    // écriture des events DONE
    // écriture de midimatrix
    // écriture de Cell
    // écriture CCMod DONE

    // que faut-il écrire des infos générales, et par layer ?
    // ecriture CHARACTERISTICS

    // KARAKT
    // je le développe ici en pensant sans doute l'incorporer un jour à Save Local

    if (saveKarakt == true)
    {
        myFile.println("[\t//KARAKT START");
        myFile.print("\t");
        myFile.println(idBlockKarakt);

        for (unsigned int i = 0; i < 1; i++) // plus tard, parcours de pile de Karakt
        {
            // unsigned int taille = Evenement[i].type;
            // si l'event est vide, type vaut 0

            // if (taille > 0)
            {
                myFile.print("\t{\t//KARAKT ");
                myFile.print(String(0)); // = current 

                myFile.println(idRecordKarakt_Start);

                myFile.print("\t\t");
                myFile.print(String(0)); // = current 
                myFile.println(idRecordKarakt_Number);

                // insérer ici les caractéristiques de chaque karakt, l'une après l'autre, avec idTag
                // dans le cas de Karakt on ne va pas tout nommer, à priori, mais mettre un chiffre entre []
                // ainsi on aura :   valeur      // [chiffre]
                // et éventuellement : valeur    // [chiffre] delay 1
                // en sachant qu'un contrôle backwards va ici s'effectuer : c'est [chiffre] et pas l'ordre qui servira
                // à mettre dans une variable la valeur

                // pour l'instant l'idTag est un peu inutile, mais une version ultérieure reconnaîtra peut être le contenu à partir du Tag,
                // et non pas de l'emplacement.

                // pour Karakt il y a tant de données à sauver qu'il me paraît plus raisonnable de
                // faire un idTag qui en effet permettre la reconnaissance de la valeur
                // mais sans spécifier in extenso quelle en est la signification
                // ce sera plutôt pour le manuel d'emploi
                // on aurait dès lors    -   valeur    // [103]
                // serait plus simple à coder mais moins facile à hacker

                //   bool actif ; // rajouté en nov2018, sur le modèle de Sequence

                /*
                myFile.print("\t\t");
                myFile.print(String(Evenement[i].actif));
                myFile.print("\t");
                myFile.println(idRecordEvenement_actif);
 */

                int_printToKarakt(1, tailleChaineMarkov, "taille Markov");
                multi_int_printToKarakt(2, nbLayer, nbDeDelais, "nb delais");
                multi_bool_printToKarakt(3, nbLayer, delayLayer, "delay?");
                multi_int_printToKarakt(4, nbLayer, nbDeMsecPourDelais, "nb msec");
                multi_int_printToKarakt(5, nbLayer, dropPourDelais, "drop");
                multi_int_printToKarakt(6, nbLayer, downPourDelais, "down");
                multi_int_printToKarakt(7, nbLayer, randomPourDelais, "rnd");
                // multi_int_printToKarakt(8,nbLayer,bitMapDelay,"bitmap");  // d'abord transformer bitmapDelay en [] << i
                multi_int_printToKarakt(9, nbLayer, repeaterNb, "repeater");
                multi_int_printToKarakt(10, nbLayer, seqBitMapDelay, "seqBitMap");

                multi_bool_printToKarakt(11, nbLayer, dende, "on off");
                multi_int_printToKarakt(12, nbLayer, dendeSec, "sec");
                multi_int_printToKarakt(13, nbLayer, dendeMSec, "msec");
                multi_int_printToKarakt(14, nbLayer, dendeDivide, "divid");
                multi_int_printToKarakt(15, nbLayer, dendeDrop, "drop"); // DEBUG il faudra modifier, pour avoir 0 - 100 - 200
                multi_int_printToKarakt(16, nbLayer, dendeDown, "down"); // DEBUG il faudra modifier, pour avoir 0 - 100 - 200
                multi_int_printToKarakt(17, nbLayer, dendeLevels, "levels");
                multi_int_printToKarakt(18, nbLayer, dendeDecay, "decay");
                multi_int_printToKarakt(19, nbLayer, dendeStart, "start");
                multi_int_printToKarakt(20, nbLayer, dendeEnd, "end");
                multi_int_printToKarakt(21, nbLayer, dendeRatio, "ratio");
                multi_int_printToKarakt(22, nbLayer, dendeStretch, "stretch");
                multi_int_printToKarakt(23, nbLayer, dendePitch, "pitch");
                multi_int_printToKarakt(24, nbLayer, dendeFidelity, "fidelity");
                int_printToKarakt(25, overLoadLimit, "limit");
                multi_int_printToKarakt(26, nbLayer, dendeRetard, "retard");
                multi_int_printToKarakt(27, nbLayer, dendeGamme, "gamme");
                multi_int_printToKarakt(28, nbLayer, dendeMode, "mode");
                multi_int_printToKarakt(29, nbLayer, dendeDegres, "degres");
                multi_int_printToKarakt(30, nbLayer, dendeExpand, "expand");
                multi_int_printToKarakt(31, nbLayer, dendeArp1, "arp1");
                multi_int_printToKarakt(32, nbLayer, dendeArp2, "arp2");
                multi_int_printToKarakt(33, nbLayer, dendeNoteOff, "noteOff");    // sera à préciser
                multi_int_printToKarakt(34, nbLayer, dendeSubFidelity, "subFid"); // TODO prévoir fc pour modifier
                multi_int_printToKarakt(35, nbLayer, repeaterNbDende, "repeat");
                // multi_int_printToKarakt(36, _bitMapDelay[nbLayer][8] = {1}; // à initialiser .  si vaut 1 delay a lieu, si vaut 0 pas
                // pour au-delà de 8 delay : %8 !
                multi_int_printToKarakt(37, nbLayer, dendeBitMapDelay, "bitmap");

                // 26 mars 2020 - en plein dans les CoronaDayz

                multi_bool_printToKarakt(38, nbLayer, omitEnd, "omit");
                multi_int_printToKarakt(39, nbLayer, omitEndValue, "omit%");

                // DRUNK
                multi_bool_printToKarakt(40, nbLayer, drunk, "drunk");
                multi_int_printToKarakt(41, nbLayer, drunkMin, "min");
                multi_int_printToKarakt(42, nbLayer, drunkMax, "max");
                multi_int_printToKarakt(43, nbLayer, drunkStep, "step");
                multi_int_printToKarakt(44, nbLayer, currentDrunkValue, "current"); // PERTINENCE ?

                // NEW Global Markov
                int_printToKarakt(45, tailleChaineMarkov, "Markov taille");
                bool_printToKarakt(46, markovNEWChainGlobal, "Markov ON");
                int_printToKarakt(47, currentTailleNEWMarkovTable, "current taille");
                int_printToKarakt(48, markovNEWTailleMax, "new taille");
                multi_bool_printToKarakt(49, nbLayer, participateNEWMarkov, "participate GLO");
                multi_bool_printToKarakt(50, nbLayer, participateLAYERMarkov, "participate LAY");
                multi_int_printToKarakt(51, nbLayer, currentSizeLayerMarkov, "size");
                multi_int_printToKarakt(52, nbLayer, markovLayerMax, "layerMax");

                //Barocco
                multi_bool_printToKarakt(53, nbLayer, baroccoLayer, "barocco");
                multi_int_printToKarakt(54, nbLayer, baroccoChosenSeq, "seq");
                multi_int_printToKarakt(55, nbLayer, nbDeMsecPourBarocco, "msec");
                multi_int_printToKarakt(56, nbLayer, dropPourBarocco, "drop");
                multi_int_printToKarakt(57, nbLayer, randomPourBarocco, "rnd");
                multi_int_printToKarakt(58, nbLayer, symHBarocco, "symH");
                multi_int_printToKarakt(59, nbLayer, symVBarocco, "symV");
                multi_int_printToKarakt(60, nbLayer, refletBarocco, "refl");
                multi_int_printToKarakt(61, nbLayer, preReflet, "preRefl");
                multi_int_printToKarakt(62, nbLayer, preH, "preH");
                multi_int_printToKarakt(63, nbLayer, preV, "preV");

                // VARIATIONS
                multi_bool_printToKarakt(64, nbLayer, variationLayer, "variations");
                multi_int_printToKarakt(65, nbLayer, percentDisparitionChanceVariation, "%disappear");
                multi_int_printToKarakt(66, nbLayer, minimumNotesVariation, "min");
                multi_int_printToKarakt(67, nbLayer, maximumReduxVariation, "maxRedux");
                multi_bool_printToKarakt(68, nbLayer, modeMinimumVariation, "modeMinimum");
                multi_int_printToKarakt(69, nbLayer, modeVariation, "modeVar");

                multi_bool_printToKarakt(70, nbLayer, pitchMicroSamplerOnChannel, "pitchMS");
                multi_bool_printToKarakt(71, nbLayer, pitchMicroSamplerOnLayer, "pitchLayer");
                multi_int_printToKarakt(72, nbLayer, pitchMicroSampler, "pitchMS");

                multi_bool_printToKarakt(73, nbLayer, dynamicSwing, "dynSwing");
                multi_int_printToKarakt(74, nbLayer, valueDynamicSwing, "valueSwing");

                // DIVERS

                multi_int_printToKarakt(75, nbLayer, fidelityWholeLayer, "fidelity whole layer");
                multi_bool_printToKarakt(76, nbLayer, isWholeLayerFidelityMuted, "fidelity muted whole"); //

                // modulo layer
                multi_int_printToKarakt(77, nbLayer, eclipseMode, "eclipse");         //  1 = mute selected    2 = mute all except selected
                int_printToKarakt(78, eclipseModulo, "modulo");                       //
                int_printToKarakt(79, eclipseK, "K");                                 //
                multi_int_printToKarakt(80, nbLayer, eclipseModuloTab, "modulo tab"); //
                multi_int_printToKarakt(81, nbLayer, eclipseKTab, "K tab");           //
                bool_printToKarakt(82, modeConstipe, "constip");                      //

                // dynramic harmonize
                int_printToKarakt(83, dynamicHarmonizeMode, "dynamic harmonize mode");    //
                multi_bool_printToKarakt(84, nbLayer, harmonizeLayer, "harmonize layer"); //

                multi_int_printToKarakt(85, nbLayer, DureePole1, "duree pol1"); //
                multi_int_printToKarakt(86, nbLayer, minPole1, "min p1");       //
                multi_int_printToKarakt(87, nbLayer, maxPole1, "max p1");       //
                multi_int_printToKarakt(88, nbLayer, DureePole2, "duree pol2"); //
                multi_int_printToKarakt(89, nbLayer, minPole2, "min p2");       //
                multi_int_printToKarakt(90, nbLayer, maxPole2, "max p2");       //
                multi_int_printToKarakt(91, nbLayer, freqPole1, "freq p1");     //

                bool_printToKarakt(92, turnOnBipolarRandom, "bip rnd"); //

                bool_printToKarakt(93, FunkyVel, "funky Vel");                          //  // DEBUG : pourquoi pas lié à layer ???
                multi_int_printToKarakt(94, nbLayer, FunkyVelValue, "funky vel Value"); //

                // autoharmonize
                bool_printToKarakt(95, autoHarmonize, "auto Harmo");        //
                multi_int_printToKarakt(96, nbLayer, modeActuelle, "mode"); //// mémorise le mode actuelle - modif 2C12

                multi_bool_printToKarakt(97, nbLayer, pitchableLayer, "pitchable layer"); //
                multi_bool_printToKarakt(98, 16, pitchableChannel, "pitchable channel");  //

                // syncop BUG
                bool_printToKarakt(99, KeepSyncopBUG, "keep syncop bug"); //

                multi_int_printToKarakt(100, nbLayer, poof, "poof"); //
                bool_printToKarakt(101, turnOnPoof, "turn on poof"); //  // DEBUG : pourquoi pas Layered ????

                int_printToKarakt(102, nSwing, "n Swing"); // // par défaut

                multi_int_printToKarakt(103, nbLayer, gammeActuelle, "gamme Actuelle"); //
                multi_int_printToKarakt(104, 12, noteRedirect, "note Redirect");        //   // contient la matrice de pitch des notes

                // 27 mars 2020 - en plein dans les CoronaDayz

                // KANNON multi layer
                multi_bool_printToKarakt(105, nbLayer, kannonActif, "kannon");
                multi_int_printToKarakt(106, nbLayer, kannonRevert, "rev");
                multi_int_printToKarakt(107, nbLayer, kannonReflet, "ref");
                multi_int_printToKarakt(108, nbLayer, kannonPitch, "pitch");
                multi_int_printToKarakt(109, nbLayer, kannonDelay, "delay");
                multi_int_printToKarakt(110, nbLayer, kannonRandom, "rnd");
                multi_int_printToKarakt(111, nbLayer, kannonStretchRatio, "stretch");

                // PAN + DERAZER
                multi_bool_printToKarakt(112, 16, panMicroSamplerOnChannel, "panMS CH");
                multi_bool_printToKarakt(113, nbLayer, panMicroSamplerOnLayer, "panMS Lay");
                multi_int_printToKarakt(114, nbLayer, panMicroSampler, "pan");

                // GABBERISM
                int_printToKarakt(115, gabberismLevels, "gabb Lvl");
                int_printToKarakt(116, gabberismDivisions, "gabb Div");
                int_printToKarakt(117, gabberismDivRatio, "gabb Div ratio");
                int_printToKarakt(118, gabberismRepeats, "gabb repeatz");
                int_printToKarakt(119, gabberismRepeatRatio, "gabb repeazt ratio");

                multi_int_printToKarakt(120, nbLayer, fidelityLayer_, "FIDELITY");

                // GABBERITO 2
                int_printToKarakt(121, gabberito2Interval, "gabb2 interval"); 
                int_printToKarakt(122,gabberito2K,"gabb2K"); 
                int_printToKarakt(123,gabberito2N,"gabb2N"); 

int_printToKarakt(124, accentMax, "accentMax");
int_printToKarakt(125,accentMin,"accentMin"); 

                // Gabberito
int_printToKarakt(126, gabberitoInterval,"gabb interval");

int_printToKarakt(127, CHMute, "CH mute"); 

                // STEPZBUG
bool_printToKarakt(128, keepStepzBUG,"stepzBUG"); 

// 2C26 pour commencer valeurs de bases de SEQ

for (int i = 0; i < nbSequencesMax; i++ ) {
     single_bool_printToKarakt(129, i , sequence[i].actif, "Seq actif");
}

for (int i = 0; i < NbCCModMax; i++)
{
    single_bool_printToKarakt(130,i , CCModTableau[i].actif, "CCMod actif");
}
for (int i = 0; i < NbEvenementMax; i++)
{
    single_bool_printToKarakt(131, i, Evenement[i].actif, "Event actif");
}
for (int i = 0; i < nbLFOMax; i++)
{
    single_bool_printToKarakt(132, i, LFO[i].actif, "LFO actif");
}

                myFile.print("\t}\t//KARAKT ");
                myFile.print(String(0));
                myFile.println(idRecordKarakt_End);
}
        }

        // sauver le Karakt Actuel en mémoire Karakt 64
        // si ils sont > 0
        // à tour de rôle charger les karakt en mémoire 1 à 64
        // sauver le current à nouveau

        // NON, car ferait en sorte qu'on crée une redoncance terrible, on sauverait à chaque fois tous les Karakt !

        // plutôt lire Kmem, et utiliser la valeur pour l'écrire telle quelle dans File

        saveKarakt_to_KMem(karakt_nbTotalPiles-1);
        // DEBUG à priori pas nécessaire, car on vient de sauver !!!

        for (unsigned int j = 0; j < karakt_nbTotalPiles; j++)
        {
            // pas très safe d'utiliser une variable générale de cette façon, mais plus rapide que de mettre cette valeur en paramètre
           // currentWorkingPlace = pKarakt_Header[0]; // debug
            currentWorkingPlace = pKarakt_Header[j]; // debug

          
            // traitement de la ligne et de copie

            // parcourir la mémoire de header 0 à j
            // parcourir la chaîne
            // charger value, number, layber
            // NON PAS dispatch dans les variables.
            // MAIS écriture dans le fichier

// pour après :
          unsigned int taille = karakt_mesureTaillePileComplete(pKarakt_Header[j]);

if ( taille == 0 )
{
    Serial.println("empty");
}
else 
{

    myFile.print("\t{\t//KARAKT MEM");
    myFile.print(String(j + 1));

    myFile.println(idRecordKarakt_Start);

    myFile.print("\t\t");
    myFile.print(String(j + 1));
    myFile.println(idRecordKarakt_Number);

    Serial.print("load");
    Serial.println(String(j));

    bool keepOn = true;

    do
    {
        karaktNumber = (*currentWorkingPlace).number;
        karaktLayer = (*currentWorkingPlace).layer;
        karaktValue = (*currentWorkingPlace).valeur;

        Serial.print("H");
        Serial.print(String(j));
        Serial.print("[");
        Serial.print(String(karaktNumber));
        Serial.print("](");
        Serial.print(String(karaktLayer));
        Serial.print(")=");
        Serial.println(String(karaktValue));
        Serial.print(" ");

        // Ce qui suit est recopié tel quel de la fonction qui charge KARAKT FROM FILE
        // on pourrait communautariser

        // dans les fonctions suivantes on vérifiera si karaktNumber est égal au premier chiffre, si oui, imprime la valeur et texte
// par facilité on passe dans nbLayer la valeur de karaktLayer 

        fromKMem_int_printToKarakt(1, tailleChaineMarkov, "taille Markov");
        fromKMem_multi_int_printToKarakt(2, nbLayer, nbDeDelais, "nb delais");
        fromKMem_multi_bool_printToKarakt(3, nbLayer, delayLayer, "delay?");
        fromKMem_multi_int_printToKarakt(4, nbLayer, nbDeMsecPourDelais, "nb msec");
        fromKMem_multi_int_printToKarakt(5, nbLayer, dropPourDelais, "drop");
        fromKMem_multi_int_printToKarakt(6, nbLayer, downPourDelais, "down");
        fromKMem_multi_int_printToKarakt(7, nbLayer, randomPourDelais, "rnd");
        // multi_int_printToKarakt(8,nbLayer,bitMapDelay,"bitmap");  // d'abord transformer bitmapDelay en [] << i
        fromKMem_multi_int_printToKarakt(9, nbLayer, repeaterNb, "repeater");
        fromKMem_multi_int_printToKarakt(10, nbLayer, seqBitMapDelay, "seqBitMap");

        fromKMem_multi_bool_printToKarakt(11, nbLayer, dende, "on off");
        fromKMem_multi_int_printToKarakt(12, nbLayer, dendeSec, "sec");
        fromKMem_multi_int_printToKarakt(13, nbLayer, dendeMSec, "msec");
        fromKMem_multi_int_printToKarakt(14, nbLayer, dendeDivide, "divid");
        fromKMem_multi_int_printToKarakt(15, nbLayer, dendeDrop, "drop"); // DEBUG il faudra modifier, pour avoir 0 - 100 - 200
        fromKMem_multi_int_printToKarakt(16, nbLayer, dendeDown, "down"); // DEBUG il faudra modifier, pour avoir 0 - 100 - 200
        fromKMem_multi_int_printToKarakt(17, nbLayer, dendeLevels, "levels");
        fromKMem_multi_int_printToKarakt(18, nbLayer, dendeDecay, "decay");
        fromKMem_multi_int_printToKarakt(19, nbLayer, dendeStart, "start");
        fromKMem_multi_int_printToKarakt(20, nbLayer, dendeEnd, "end");
        fromKMem_multi_int_printToKarakt(21, nbLayer, dendeRatio, "ratio");
        fromKMem_multi_int_printToKarakt(22, nbLayer, dendeStretch, "stretch");
        fromKMem_multi_int_printToKarakt(23, nbLayer, dendePitch, "pitch");
        fromKMem_multi_int_printToKarakt(24, nbLayer, dendeFidelity, "fidelity");
        fromKMem_int_printToKarakt(25, overLoadLimit, "limit");
        fromKMem_multi_int_printToKarakt(26, nbLayer, dendeRetard, "retard");
        fromKMem_multi_int_printToKarakt(27, nbLayer, dendeGamme, "gamme");
        fromKMem_multi_int_printToKarakt(28, nbLayer, dendeMode, "mode");
        fromKMem_multi_int_printToKarakt(29, nbLayer, dendeDegres, "degres");
        fromKMem_multi_int_printToKarakt(30, nbLayer, dendeExpand, "expand");
        fromKMem_multi_int_printToKarakt(31, nbLayer, dendeArp1, "arp1");
        fromKMem_multi_int_printToKarakt(32, nbLayer, dendeArp2, "arp2");
        fromKMem_multi_int_printToKarakt(33, nbLayer, dendeNoteOff, "noteOff");    // sera à préciser
        fromKMem_multi_int_printToKarakt(34, nbLayer, dendeSubFidelity, "subFid"); // TODO prévoir fc pour modifier
        fromKMem_multi_int_printToKarakt(35, nbLayer, repeaterNbDende, "repeat");
        //fromKMem_multi_int_printToKarakt(36, _bitMapDelay[nbLayer][8] = {1}; // à initialiser .  si vaut 1 delay a lieu, si vaut 0 pas
        // pour au-delà de 8 delay : %8 !
        fromKMem_multi_int_printToKarakt(37, nbLayer, dendeBitMapDelay, "bitmap");

        // 26 mars 2020 - en plein dans les CoronaDayz

        fromKMem_multi_bool_printToKarakt(38, nbLayer, omitEnd, "omit");
        fromKMem_multi_int_printToKarakt(39, nbLayer, omitEndValue, "omit%");

        // DRUNK
        fromKMem_multi_bool_printToKarakt(40, nbLayer, drunk, "drunk");
        fromKMem_multi_int_printToKarakt(41, nbLayer, drunkMin, "min");
        fromKMem_multi_int_printToKarakt(42, nbLayer, drunkMax, "max");
        fromKMem_multi_int_printToKarakt(43, nbLayer, drunkStep, "step");
        fromKMem_multi_int_printToKarakt(44, nbLayer, currentDrunkValue, "current"); // PERTINENCE ?

        // NEW Global Markov
        fromKMem_int_printToKarakt(45, tailleChaineMarkov, "Markov taille");
        fromKMem_bool_printToKarakt(46, markovNEWChainGlobal, "Markov ON");
        fromKMem_int_printToKarakt(47, currentTailleNEWMarkovTable, "current taille");
        fromKMem_int_printToKarakt(48, markovNEWTailleMax, "new taille");
        fromKMem_multi_bool_printToKarakt(49, nbLayer, participateNEWMarkov, "participate GLO");
        fromKMem_multi_bool_printToKarakt(50, nbLayer, participateLAYERMarkov, "participate LAY");
        fromKMem_multi_int_printToKarakt(51, nbLayer, currentSizeLayerMarkov, "size");
        fromKMem_multi_int_printToKarakt(52, nbLayer, markovLayerMax, "layerMax");

        //Barocco
        fromKMem_multi_bool_printToKarakt(53, nbLayer, baroccoLayer, "barocco");
        fromKMem_multi_int_printToKarakt(54, nbLayer, baroccoChosenSeq, "seq");
        fromKMem_multi_int_printToKarakt(55, nbLayer, nbDeMsecPourBarocco, "msec");
        fromKMem_multi_int_printToKarakt(56, nbLayer, dropPourBarocco, "drop");
        fromKMem_multi_int_printToKarakt(57, nbLayer, randomPourBarocco, "rnd");
        fromKMem_multi_int_printToKarakt(58, nbLayer, symHBarocco, "symH");
        fromKMem_multi_int_printToKarakt(59, nbLayer, symVBarocco, "symV");
        fromKMem_multi_int_printToKarakt(60, nbLayer, refletBarocco, "refl");
        fromKMem_multi_int_printToKarakt(61, nbLayer, preReflet, "preRefl");
        fromKMem_multi_int_printToKarakt(62, nbLayer, preH, "preH");
        fromKMem_multi_int_printToKarakt(63, nbLayer, preV, "preV");

        // VARIATIONS
        fromKMem_multi_bool_printToKarakt(64, nbLayer, variationLayer, "variations");
        fromKMem_multi_int_printToKarakt(65, nbLayer, percentDisparitionChanceVariation, "%disappear");
        fromKMem_multi_int_printToKarakt(66, nbLayer, minimumNotesVariation, "min");
        fromKMem_multi_int_printToKarakt(67, nbLayer, maximumReduxVariation, "maxRedux");
        fromKMem_multi_bool_printToKarakt(68, nbLayer, modeMinimumVariation, "modeMinimum");
        fromKMem_multi_int_printToKarakt(69, nbLayer, modeVariation, "modeVar");

        fromKMem_multi_bool_printToKarakt(70, nbLayer, pitchMicroSamplerOnChannel, "pitchMS");
        fromKMem_multi_bool_printToKarakt(71, nbLayer, pitchMicroSamplerOnLayer, "pitchLayer");
        fromKMem_multi_int_printToKarakt(72, nbLayer, pitchMicroSampler, "pitchMS");

        fromKMem_multi_bool_printToKarakt(73, nbLayer, dynamicSwing, "dynSwing");
        fromKMem_multi_int_printToKarakt(74, nbLayer, valueDynamicSwing, "valueSwing");

        // DIVERS

        fromKMem_multi_int_printToKarakt(75, nbLayer, fidelityWholeLayer, "fidelity whole layer");
        fromKMem_multi_bool_printToKarakt(76, nbLayer, isWholeLayerFidelityMuted, "fidelity muted whole"); //

        // modulo layer
        fromKMem_multi_int_printToKarakt(77, nbLayer, eclipseMode, "eclipse");         //  1 = mute selected    2 = mute all except selected
        fromKMem_int_printToKarakt(78, eclipseModulo, "modulo");                       //
        fromKMem_int_printToKarakt(79, eclipseK, "K");                                 //
        fromKMem_multi_int_printToKarakt(80, nbLayer, eclipseModuloTab, "modulo tab"); //
        fromKMem_multi_int_printToKarakt(81, nbLayer, eclipseKTab, "K tab");           //
        fromKMem_bool_printToKarakt(82, modeConstipe, "constip");                      //

        // dynramic harmonize
        fromKMem_int_printToKarakt(83, dynamicHarmonizeMode, "dynamic harmonize mode");    //
        fromKMem_multi_bool_printToKarakt(84, nbLayer, harmonizeLayer, "harmonize layer"); //

        fromKMem_multi_int_printToKarakt(85, nbLayer, DureePole1, "duree pol1"); //
        fromKMem_multi_int_printToKarakt(86, nbLayer, minPole1, "min p1");       //
        fromKMem_multi_int_printToKarakt(87, nbLayer, maxPole1, "max p1");       //
        fromKMem_multi_int_printToKarakt(88, nbLayer, DureePole2, "duree pol2"); //
        fromKMem_multi_int_printToKarakt(89, nbLayer, minPole2, "min p2");       //
        fromKMem_multi_int_printToKarakt(90, nbLayer, maxPole2, "max p2");       //
        fromKMem_multi_int_printToKarakt(91, nbLayer, freqPole1, "freq p1");     //

        fromKMem_bool_printToKarakt(92, turnOnBipolarRandom, "bip rnd"); //

        fromKMem_bool_printToKarakt(93, FunkyVel, "funky Vel");                          //  // DEBUG : pourquoi pas lié à layer ???
        fromKMem_multi_int_printToKarakt(94, nbLayer, FunkyVelValue, "funky vel Value"); //

        // autoharmonize
        fromKMem_bool_printToKarakt(95, autoHarmonize, "auto Harmo");        //
        fromKMem_multi_int_printToKarakt(96, nbLayer, modeActuelle, "mode"); //// mémorise le mode actuelle - modif 2C12

        fromKMem_multi_bool_printToKarakt(97, nbLayer, pitchableLayer, "pitchable layer"); //
        fromKMem_multi_bool_printToKarakt(98, 16, pitchableChannel, "pitchable channel");  //

        // syncop BUG
        fromKMem_bool_printToKarakt(99, KeepSyncopBUG, "keep syncop bug"); //

        fromKMem_multi_int_printToKarakt(100, nbLayer, poof, "poof"); //
        fromKMem_bool_printToKarakt(101, turnOnPoof, "turn on poof"); //  // DEBUG : pourquoi pas Layered ????

        fromKMem_int_printToKarakt(102, nSwing, "n Swing"); // // par défaut

        fromKMem_multi_int_printToKarakt(103, nbLayer, gammeActuelle, "gamme Actuelle"); //
        fromKMem_multi_int_printToKarakt(104, 12, noteRedirect, "note Redirect");        //   // contient la matrice de pitch des notes

        // 27 mars 2020 - en plein dans les CoronaDayz

        // KANNON multi layer
        fromKMem_multi_bool_printToKarakt(105, nbLayer, kannonActif, "kannon");
        fromKMem_multi_int_printToKarakt(106, nbLayer, kannonRevert, "rev");
        fromKMem_multi_int_printToKarakt(107, nbLayer, kannonReflet, "ref");
        fromKMem_multi_int_printToKarakt(108, nbLayer, kannonPitch, "pitch");
        fromKMem_multi_int_printToKarakt(109, nbLayer, kannonDelay, "delay");
        fromKMem_multi_int_printToKarakt(110, nbLayer, kannonRandom, "rnd");
        fromKMem_multi_int_printToKarakt(111, nbLayer, kannonStretchRatio, "stretch");

        // PAN + DERAZER
        fromKMem_multi_bool_printToKarakt(112, 16, panMicroSamplerOnChannel, "panMS CH");
        fromKMem_multi_bool_printToKarakt(113, nbLayer, panMicroSamplerOnLayer, "panMS Lay");
        fromKMem_multi_int_printToKarakt(114, nbLayer, panMicroSampler, "pan");

        // GABBERISM
        fromKMem_int_printToKarakt(115, gabberismLevels, "gabb Lvl");
        fromKMem_int_printToKarakt(116, gabberismDivisions, "gabb Div");
        fromKMem_int_printToKarakt(117, gabberismDivRatio, "gabb Div ratio");
        fromKMem_int_printToKarakt(118, gabberismRepeats, "gabb repeatz");
        fromKMem_int_printToKarakt(119, gabberismRepeatRatio, "gabb repeazt ratio");

        fromKMem_multi_int_printToKarakt(120, nbLayer, fidelityLayer_, "FIDELITY");

        // GABBERITO 2
        fromKMem_int_printToKarakt(121, gabberito2Interval, "gabb2 interval");
        fromKMem_int_printToKarakt(122, gabberito2K, "gabb2K");
        fromKMem_int_printToKarakt(123, gabberito2N, "gabb2N");

        fromKMem_int_printToKarakt(124, accentMax, "accentMax");
        fromKMem_int_printToKarakt(125, accentMin, "accentMin");

        // Gabberito
        fromKMem_int_printToKarakt(126, gabberitoInterval, "gabb interval");

        fromKMem_int_printToKarakt(127, CHMute, "CH mute");

        // STEPZBUG
        fromKMem_int_printToKarakt(128, keepStepzBUG, "stepzBUG");

        for (int i = 0; i < nbSequencesMax; i++)
        {
            fromKMem_single_bool_printToKarakt(129, i, sequence[i].actif, "Seq actif");
        }

        for (int i = 0; i < NbCCModMax; i++)
        {
            fromKMem_single_bool_printToKarakt(130, i, CCModTableau[i].actif, "CCMod actif");
        }
        for (int i = 0; i < NbEvenementMax; i++)
        {
            fromKMem_single_bool_printToKarakt(131, i, Evenement[i].actif, "Event actif");
        }
        for (int i = 0; i < nbLFOMax; i++)
        {
            fromKMem_single_bool_printToKarakt(132, i, LFO[i].actif, "LFO actif");
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

            myFile.print("\t}\t//KARAKT ");
            myFile.print(String(j + 1));
            myFile.println(idRecordKarakt_End);

            Serial.println();
}
    }



        myFile.println("]\t//KARAKT END");
    }

if ( includeHeader )
{
    myFile.println(F(""));
    myFile.println(F(""));

    myFile.println(F(""));
    myFile.println(F("Created by Piccolo "));
    myFile.println(PiccoloVersion);
    myFile.println(PiccoloDate);
    myFile.println(F(""));
    myFile.println(F("you can change numbers but not the structure"));
    myFile.println(F("do not insert or delete lines or TAB"));
    myFile.println(F("you can experiment with numbers though !"));
    myFile.println(F("dont modify anything between [ ]"));
    myFile.println(F("now you can be an errorist"));

    // close the file:
    myFile.close();
    buffer_and_HARD_setCursor(0, 0);
    buffer_and_HARD_print(F("Saved  "));
    delay(watchDelay);
}

    for (byte i = 0; i < nbLayer; i++)
    {
        ZEROupdatePileTimeToPlayMODEDIRECT(i);
    }

    rootFileCount++;
    measureSDFileCount();
}
 