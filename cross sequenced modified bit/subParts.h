void measureSDFileCount(void)
{
    sd.vwd()->rewind();
    rootFileCount = 0;
    while (myFile.openNext(sd.vwd(), O_READ))
    {
        rootFileCount++;
        myFile.close();
    }
}

void initialisationsDiverses(void)
{

    //    if ( specialDebug ) { Serial.begin(115200); }

    // MIDI3.turnThruOff(); // on pourra un jour quand même switcher vers ON si on veut jouer un instrument midi avec un autre clavier !
    // depuis 2C28 il est d'emblée ON, car c'était la situation la plus fréquente
    // si on a un clavier maître qui est le synthé principal, il vaut mieux OFF pour éviter feedback midi 
    
    if (midiThru == true)
    {
        MIDI3.turnThruOn(midi::Thru::Full);
    }
    else
    {
        MIDI3.turnThruOff();
    };

    Serial3.begin(31250);

    // Channel:
    //     0: OMNI mode, the message is sent on all channels.
    //   1 to 16: classic channels. (transposed to 0-15 for binary transmission)
    // 17+: OFF (nothing sent)

    PiccoloVersion = F("Piccolo T2C30 Bore Stepz"); // petit subterfuge pour ne rien laisser en mémoire vive
    PiccoloDate = F("17/9/2020");
    midiname = F("turca.mid");

    ChoixString153 = F("<_abcdefghij<klmnopqrst<uvwxyz0123<456789ABCD<EFGHIJKLMN<OPQRSTUVWXYZ<");

    String159 = F("---");
    ChoixString156 = F("-");
    ChoixString156 = ChoixString156 + flechegauche + flechedroite + barremilieu + barreH + F("x!") + F("<");

    u8x8.begin();

    defineSlot(1, 0, 0);  // la première zone d'affichage sera 0,0
    defineSlot(2, 5, 0);  // la deuxième correspond à l'affichage des effets
    defineSlot(3, 0, 1);  // la troisième pour les valeurs des transfos
    defineSlot(4, 10, 6); // pour les transformations
    defineSlot(5, 0, 5);  // pour les MUTE et STEPZ
    defineSlot(6, 0, 4);  // pour le déroulement

    // printRot2(PiccoloVersion, 16, 250, 1);
    buffer_and_HARD_setCursor(0, 0);
    buffer_and_HARD_print(PiccoloVersion);
    // } while (millis() < 3000); // NE PAS REMPLACER PAR NOWMILLIS , car le précède !

    // if (!sd.begin(53))
    if (!sd.begin())
    {
        // printRot2(PiccoloDate + F(" NO SD"), 16, 200, 3);
        buffer_and_HARD_setCursor(0, 2);
        buffer_and_HARD_print(PiccoloDate + F(" NO SD"));
    }
    else
    {
        // printRot2(PiccoloDate + F(" SD OK"), 16, 200, 3);
        buffer_and_HARD_setCursor(0, 2);
        buffer_and_HARD_print(PiccoloDate + F(" SD OK"));
    }

    buffer_and_HARD_setCursor(1, 7);
    u8x8.draw2x2String(1, 5, "Piccolo");

    delay(2000);
    // u8x8.clear();

    // delay(5000);

    // Initialize at the highest speed supported by the board that is
    // not over 50 MHz. Try a lower speed if SPI errors occur.

    // if (!sd.begin(chipSelect, SD_SCK_MHZ(50)))
    if (!sd.begin())
    {
        //    sd.initErrorHalt();
        SDCardPresente = false;
    }
    else
    {

        // calcul du nombre de fichiers sur la carte SD
        SDCardPresente = true;
        measureSDFileCount();
    }

    buffer_and_HARD_setCursor(0, 4);

    buffer_and_HARD_print("files:" + String(rootFileCount));
    delay(1000);
    u8x8.clear();

    /*
    pinMode(5, OUTPUT);  // petite lumière qui s'allume pour divers usages
    pinMode(32, OUTPUT); // petite lampe verte qui s'allume pour le tempo.
    pinMode(33, OUTPUT); // petite lampe verte qui s'allume pour le tempo.
    pinMode(34, OUTPUT); // petite lampe verte qui s'allume pour le tempo.
    pinMode(35, OUTPUT); // petite lampe verte qui s'allume pour le tempo.



    pinMode(4, INPUT); // pour le bouton poussoir noir 1 = TAP
    pinMode(6, INPUT); // pour le bouton poussoir noir 2 = LAYER SELECT : sera remplacé à terme par des switch !
    pinMode(9, INPUT); // pour le bouton poussoir noir 3 = OK GO
*/
    // TODO DEBUG n'est plus actif pour Dende, mais à réactiver plus tard

    // TODO DEBUG

    // pinMode(62, INPUT) ;
    // pinMode(63, INPUT) ;

    /*
    pinMode(38, INPUT); // switch n°1 UP
    pinMode(39, INPUT); //            DOWN
    pinMode(40, INPUT); // switch 2    UP
    pinMode(41, INPUT); //             DOWN
    pinMode(42, INPUT); // etc
    pinMode(43, INPUT); //
    pinMode(44, INPUT); //
    pinMode(45, INPUT); //


    for (byte i = 0 ; i < 8 ; i++)
    {
        digitalWrite(38+i,HIGH);
    }
*/

    randomSeed(analogRead(A5));

    /*  VIEILLE INITIALISATION DES PREMIERS PICCOLO.   moment de nostalgie 
    for (byte i = 0; i < nbLayer; i++)
    { // layers
        for (byte j = 0; j < nbNotesByLayer; j++)
        { // mélodie
            melodyMatrix[i][j][MMnote] = 0;
            melodyMatrix[i][j][MMvelocite] = 0;
            melodyMatrix[i][j][MMdata] = 1;  // çàd les 4 premiers bits = le channel   les 4 suivants sont les données pour chord : 0 = solo
            melodyMatrix[i][j][MMdata2] = 0; // le bit played est 0 et le bit silented est 0 ;

            // mais on peut imaginer plus tard des channels différents dans même layer

            melodyMatrixTime[i][j][MMTstartnote] = 0;
            melodyMatrixTime[i][j][MMTendnote] = 0;

            //      melodyMatrixTime[i][j][MMTstartnote] = ((j + 1) * 1000L) / (i + 1) ;
            //    melodyMatrixTime[i][j][MMTendnote] =  melodyMatrixTime[i][j][MMTstartnote] + ( 900L / (i + 1))  ;

            // abandonner l'idée des durées de notes et de pause et juste garder le début et la fin de la note
            // dans son layer.  évitera des incohérences
        }
    } */

    for (byte i = 0; i < NbEvenementMax; i++)
    { // layers
        EmptyEvenement(i);

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

    for (byte i = 0; i < nbLFOMax; i++)
    {
        EmptyLFO(i);
    }

    backToSelfMatrixTotal(midiRedirect);

    rotKnob9.write(0);
    rotKnob7.write(0);

    // u8x8.clear();

    // initialisation du pointeur initial
    // Initialisations CCMOD

    // CCModule1.type = 0 ;
    // CCModule1.sourceMin = 0;
    // CCModule1.sourceMax = 127 ;
    // CCModule1.targetMin = 0;
    // CCModule1.targetMax = 127;
    // CCModule1.borders = 0;
    // CCModule1.idCC = 69 ; // au hasard
    // CCModule1.channelCC = 3 ;
    // CCModule1.layerConcerne = 1 ;
    // CCModule1.actif = false ;
    //
    // pileCCMod1.CCMod.type = 0 ;
    // pileCCMod1.CCMod.sourceMin = 0;
    // pileCCMod1.CCMod.sourceMax = 127 ;
    // pileCCMod1.CCMod.targetMin = 0;
    // pileCCMod1.CCMod.targetMax = 127;
    // pileCCMod1.CCMod.borders = 0;
    // pileCCMod1.CCMod.idCC = 69 ; // au hasard
    // pileCCMod1.CCMod.channelCC = 3 ;
    // pileCCMod1.CCMod.layerConcerne = 1 ;
    // pileCCMod1.CCMod.actif = false ;
    // pileCCMod1.numeroPile = 1 ;
    // pileCCMod1.nextCCMod = 0 ;

    // CCModPointeurInitial = &pileCCMod1 ;

    deleteAllCCMod(); // 2C01 - remplace une boucle

    // Initialisation des array
    // 1C96 je découvre aujourd'hui que int[16] = {1} initialise 1 le premier et 0 le reste :( !!!

    for (int i = 0; i < nbLayer; i++)
    {
        pitchableLayer[i] = true;
        nbMaxDeNotesParLayer[i] = 10000;
        modeDeletionWhenExtraNotes[i] = 1;
        delayLayer[i] = false;
        nbDeDelais[i] = 4;
        nbDeMsecPourDelais[i] = 100;
        dropPourDelais[i] = 90;
        downPourDelais[i] = 60;
        randomPourDelais[i] = 100;
        // modStartPourDelais[i] = 0;
        // modEndPourDelais[i] = 100;
        for (int j = 0; j < 8; j++)
        {
            bitMapDelay[i][j] = 1;  // le delay aura lieu
            _bitMapDelay[i][j] = 1; // le delay dans Dende aura lieu
        }
    }

    // TODO regrouper dans une struct toutes les caractéristiques de layers ! durée, pitchable, fidelity, etc. !
    for (int i = 0; i < 16; i++)
    {
        vel2Vol[i] = false;
        pitchableChannel[i] = true;
    }

    // initialisation de Cell
    for (int i = 0; i < nbValeursMaxPourCell; i++)
    {
        tableauCell[i].channelIn = 0;
        tableauCell[i].channelOut = 0;

        tableauCell[i].noteIn = 0;
        tableauCell[i].noteOut = 0;
    }

    // saveKarakt_to_KMem(1); // on sauve les valeurs initiales dans la première mémoire
}

void checkButton4TAPtempo(void)
{
    if (myButtonChannel5.risingEdge())
    {
        if (tempoStarted == false)
        {
            tempoStarted = true;
            tapBegin = nowMillis;
            // 2C19 buffer_and_HARD_setCursor(0, 3);
            // buffer_and_HARD_print("TAP");
            buffer_setCursor(0, 3);
            buffer_print("TAP");
            time1 = nowMillis;
            RECtiming = true;
        }
        else
        {
            tempoStarted = false;
            tapEnd = nowMillis;
            newTempo = tapEnd - tapBegin;
            // 2C19 buffer_and_HARD_setCursor(0, 3);
            // buffer_and_HARD_print(newTempo);
            // buffer_and_HARD_print(" ");
            buffer_setCursor(0, 3);
            buffer_print(String(newTempo));
            buffer_print(" ");

            time2 = nowMillis;
            RECtiming = false;

            if (tapBPM == true)
            {
                // on détermine une durée de l'espace entré récemment
                // on moyenne avec les 4 précédents
                // on définit ensuite une durée de layer, par durée 1BPM x nb Stepz dans un layer
                // la durée de layer devient cela

                unsigned long lastBeatLength = time2 - time1;
                unsigned long meanBeat = 100;

                if (time2 - previousTime2 < 60000) // on est dans le cas de tap successifs
                {
                    for (byte i = 1; i < 4; i++)
                    {
                        beatLengthHistory[i - 1] = beatLengthHistory[i];
                    }
                    beatLengthHistory[3] = lastBeatLength;
                    previousTime2 = time2;
                    meanBeat = (beatLengthHistory[0] + beatLengthHistory[1] + beatLengthHistory[2] + beatLengthHistory[3]) / 4;
                }
                else
                { // on remplit l'historique avec la dernière valeur
                    for (byte i = 1; i < 4; i++)
                    {
                        beatLengthHistory[i] = lastBeatLength;
                    }
                    previousTime2 = time2;
                    meanBeat = (beatLengthHistory[0] + beatLengthHistory[1] + beatLengthHistory[2] + beatLengthHistory[3]) / 4;
                }

                unsigned long fullLength = meanBeat * nbStepz / nbStepzFor1BPM;

                // 	layerLength[i] = (60000 * nbStepz) / (nbBPM_Stepz * nbStepzFor1BPM);

                for (byte i = 0; i < nbLayer; i++)
                {
                    // déterminer si la layer i est active

                    if (isLayerActive(i))
                    {
                        oldlayerLength = layerLength[i];

                        layerLength[i] = fullLength; // on voit la différence entre le push1x et push2x pour calculer le temps
                        // layerLength[i] = (60000 * nbStepz) / (nbBPM_Stepz * nbStepzFor1BPM);
                        // layerlength * (nbBPM * nbSTepz1BPM ) = 60000 * nbStepz
                        // nbBPM = ( 60000 * nbSTepez ) / ( layerlength * nbSTezp1BPM )
                        nbBPM_Stepz = ((long)60000 * (long)nbStepz) / (layerLength[i] * nbStepzFor1BPM);

                        buffer_setCursor(6, 3);
                        buffer_print(String(nbBPM_Stepz));
                        buffer_print(" ");

                        // à ce moment-ci se produit parfois un grand BANG car il joue soudain toutes les notes en retard !!!
                        // ********************************** IL FAUT CHANGER LES BITS JOUES OU PAS JOUES !

                        /* si SqueezeOn, il ne suffit pas de modifier la durée, il faut modifier les valeurs de start et end des notes
                                            selon un ratio correspondant à nouvelletaille / anciennetaille
                                            si KeepLength est ON, il faut modifier uniquement le Start selon ce ratio
                                            et calculer un nouveau End correspondant à nouveauStart + ( ancienEnd-ancienStart )
                                            */

                        unsigned int taille = mesureTaillePileComplete(pInitial[i]);
                        pileNote tempNote;

                        for (byte j = 0; j < taille; j++)
                        {
                            tempNote = ZEROgetNoteFromPile(pInitial[i], j);

                            if (KeepLength == true)
                            {
                                KeepDuree = getDuree(tempNote);
                            }

                            tempNote.startNote = (unsigned long)((float)tempNote.startNote * (float)layerLength[i] / (float)oldlayerLength);

                            if (KeepLength == true)
                            {
                                tempNote.endNote = tempNote.startNote + KeepDuree;
                            }
                            else
                            {
                                tempNote.endNote = (unsigned int)((float)tempNote.endNote * (float)layerLength[i] / (float)oldlayerLength);
                            }
                            pileNote_actualizeTimeToPlay(tempNote);
                            ZEROSetNoteInPile(pInitial[i], j, tempNote);
                        }
                    }
                }
            }
            else if (tapAccou == true)
            {
                // d'abord déterminer la plus petite des layers actives = minLength
                // ensuite, passer en revue les layers
                // calculer un ratio entre les autres layers et cette minLength
                // diviser les longueurs et les mélodies de ces layers

                unsigned int shortest = layerLength[0];

                for (byte i = 0; i < nbLayer; i++)
                {
                    if (isLayerActive(i))
                    {
                        if (layerLength[i] < shortest)
                        {
                            shortest = layerLength[i];
                        }
                    }
                }

                for (byte i = 0; i < nbLayer; i++)
                {

                    float ratioDivide = layerLength[i] / shortest;

                    oldlayerLength = layerLength[i];

                    layerLength[i] = (time2 - time1) * ratioDivide;

                    unsigned int taille = mesureTaillePileComplete(pInitial[i]);
                    pileNote tempNote;

                    for (byte j = 0; j < taille; j++)
                    {
                        tempNote = ZEROgetNoteFromPile(pInitial[i], j);

                        if (KeepLength == true)
                        {
                            KeepDuree = getDuree(tempNote);
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

                        pileNote_actualizeTimeToPlay(tempNote);
                        // cette procédure actualise le temps pour une note JOUEEE, dans le prochain segment
                        // TODO il fautdrait une procédure qui l'actualise dans le segment actuel
                        // ou différencier les deux

                        ZEROSetNoteInPile(pInitial[i], j, tempNote);
                    }

                } // fin de i
            }     // fin de Accou

            else

            { // non TapAccou

                for (byte i = 0; i < nbLayer; i++)
                {
                    // déterminer si la layer i est active

                    if (isLayerActive(i))
                    {

                        oldlayerLength = layerLength[i];
                        if (complexSqueeze == false)
                        {
                            layerLength[i] = (time2 - time1); // on voit la différence entre le push1x et push2x pour calculer le temps
                        }
                        else
                        {
                            byte j = layerLength[i] / (time2 - time1); // donne un chiffre indiquant le nb de fois (arrondi à l'unité inférieure) où on retrouve le tap dans la durée initiallayerLength
                            if (j == 0)
                            {
                                j++;
                            }
                            layerLength[i] = j * (time2 - time1);
                            //
                        }

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
                            pileNote tempNote;

                            for (byte j = 0; j < taille; j++)
                            {
                                tempNote = ZEROgetNoteFromPile(pInitial[i], j);

                                if (KeepLength == true)
                                {
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
                                    tempNote.endNote = (unsigned int)((float)tempNote.endNote * (float)layerLength[i] / (float)oldlayerLength);
                                }
                                pileNote_actualizeTimeToPlay(tempNote);
                                ZEROSetNoteInPile(pInitial[i], j, tempNote);
                            }
                        }

                        // buffer_and_HARD_print(F("     "));
                        // ici on modifie la valeur des layers allumées...
                        // en fonction des switch on doit voir les autres valeurs sont ajustées automatiquement ou pas, strictement ou pas ( 2 switchs ou bien 2 choix dans le MENU ? )

                        // if switch ON : valeur strictes
                        // if switch OFF : valeurs irrégulières
                    }
                }
            } // fin du NON ACCOU
        }
    }
}

void checkJumpButton(void)
{

    if (myButtonChannel6.risingEdge())
    {
        // si le bouton poussoir CH6 a été poussé, on va faire jump pour sélection des layers

        // if ( ( millis() - pressed2time ) > blocktime )
        {

            counterEncoder = rotKnob9.read(); // il lit la position dans le menu
            counterEndUser = counterEncoder / 8;

            // qd on est au-dessus de la première transfo d'une catégorie, on envoie à la suivante

            int jump[] = {24, 424, 466, 472, 420, 351, 372, 111, 58, 119, 108, 64, 127, 81, 167, 190, 218, 228, 244, 311, 327, 340, 24};
            // ceci indique la succession des numéros de transfo dans le TransfoArray
            // qd on fait un jump, on saute d'une transfo à la précédente dans cette liste
            // qd on modifie le tableau array, et qu'on change les catégories on doit changer ceci aussi !

            // TODO automatiser la valeur 22 !

            byte r = 0;
            while ((counterEndUser >= LineCallingTransfo(jump[r])) && (r < 22))
            {
                r++;
            }

            counterEndUser = LineCallingTransfo(jump[r]);

            counterEncoder = counterEndUser * 8;
            rotKnob9.write(counterEncoder);

            // pressed2time = millis();
        }

        buffer_clearScreen_HARD(); // pour nettoyage !
    }
}

void checkDeleteButton(void)
{

    if (myButtonSHIFT.risingEdge())
    { // si le bouton poussoir noir 2 a été poussé, on va faire +1 pour sélection des layers

        // if ( ( millis() - pressedE1time ) > blocktime )
        if (Transfo[counterEndUser][0] != 424 
        && Transfo[counterEndUser][0] != 512 
        && Transfo[counterEndUser][0] != 441
        && Transfo[counterEndUser][0] != 548
        
         )
        // 2C15 + 2C23 pour adapter à PUNTZ et LAPIDZ
        {
            Transformation(5, 0); // DELETE les layers actives

            // pressedE1time = millis();

            buffer_setCursor(0, 6);
            buffer_print("DEL");
        }
        else
        {
            // effacer dans les layers actifs toutes les notes stepChannel, stepNote

            for (byte i = 0; i < nbLayer; i++)
            {
                if (isLayerActive(i))
                {
                    unsigned int taille = mesureTaillePileComplete(pInitial[i]);

                    for (unsigned int j = 0; j < taille; j++)
                    {
                        pileNote tempNote;
                        tempNote = ZEROgetNoteFromPile(pInitial[i], j);

                        if (tempNote.note == currentStepzNote && tempNote.channel == stepChannel)
                        {
                            tempNote.note = 0;
                            tempNote.killnote = true;
                            tempNote.velocity = 0;
                            ZEROSetNoteInPile(pInitial[i], j, tempNote);
                        }

                        if (tempNote.note == currentStepzNote && currentStepzNote == 128)
                        // TOTAL ACCENT, CH indépendant
                        {
                            tempNote.note = 0;
                            tempNote.killnote = true;
                            tempNote.velocity = 0;
                            ZEROSetNoteInPile(pInitial[i], j, tempNote);
                        }
                    }
                }
            }
        }
    }
}

void checkButton10to13LayerSelect(void)
{

    if (myButtonChannel1.risingEdge())
    { // si le bouton poussoir noir bas 1 a été poussé, on va changer valeur de layer 1
        //if ((nowMillis - pressedL1time) > blocktime)
        {

            activeLayer = activeLayer ^ (B0001 << (1 - 1));
            // 1 -> 0    0 -> 1
            activeLayer = (activeLayer & B00001111); // on coupe tout ce qui dépasse au dessus des 4 layers ! attention si on augmente le nb de layers faudra changer cette valeur
                                                     // pressedL1time = nowMillis;
        }

        if (
            Transfo[counterEndUser][0] != 30 && Transfo[counterEndUser][0] != 14 && Transfo[counterEndUser][0] != 246 && Transfo[counterEndUser][0] != 46 && Transfo[counterEndUser][0] != 48 && Transfo[counterEndUser][0] != 14)
        {
            showActiveStatus();
        }
    }

    if (myButtonChannel2.risingEdge())
    { // si le bouton poussoir noir bas 1 a été poussé, on va changer valeur de layer 1
        // if ((nowMillis - pressedL2time) > blocktime)
        {

            activeLayer = activeLayer ^ (B0001 << (2 - 1));
            // 1 -> 0    0 -> 1
            activeLayer = (activeLayer & B00001111); // on coupe tout ce qui dépasse au dessus des 4 layers ! attention si on augmente le nb de layers faudra changer cette valeur
                                                     //   pressedL2time = nowMillis;
        }

        if (
            Transfo[counterEndUser][0] != 30 && Transfo[counterEndUser][0] != 14 && Transfo[counterEndUser][0] != 246 && Transfo[counterEndUser][0] != 46 && Transfo[counterEndUser][0] != 48 && Transfo[counterEndUser][0] != 14)
        {
            showActiveStatus();
        }
    }

    if (myButtonChannel3.risingEdge())
    { // si le bouton poussoir noir bas 1 a été poussé, on va changer valeur de layer 1
        //if ((nowMillis - pressedL3time) > blocktime)
        {

            activeLayer = activeLayer ^ (B0001 << (3 - 1));
            // 1 -> 0    0 -> 1
            activeLayer = (activeLayer & B00001111); // on coupe tout ce qui dépasse au dessus des 4 layers ! attention si on augmente le nb de layers faudra changer cette valeur
            //pressedL3time = nowMillis;
        }

        if (
            Transfo[counterEndUser][0] != 30 && Transfo[counterEndUser][0] != 14 && Transfo[counterEndUser][0] != 246 && Transfo[counterEndUser][0] != 46 && Transfo[counterEndUser][0] != 48 && Transfo[counterEndUser][0] != 14)
        {
            showActiveStatus();
        }
    }

    if (myButtonChannel4.risingEdge())
    { // si le bouton poussoir noir bas 1 a été poussé, on va changer valeur de layer 1
        //if ((nowMillis - pressedL4time) > blocktime)

        activeLayer = activeLayer ^ (B0001 << (4 - 1));
        // 1 -> 0    0 -> 1
        activeLayer = (activeLayer & B00001111); // on coupe tout ce qui dépasse au dessus des 4 layers ! attention si on augmente le nb de layers faudra changer cette valeur
                                                 // pressedL4time = nowMillis;

        if (
            Transfo[counterEndUser][0] != 30 && Transfo[counterEndUser][0] != 14 && Transfo[counterEndUser][0] != 246 && Transfo[counterEndUser][0] != 46 && Transfo[counterEndUser][0] != 48 && Transfo[counterEndUser][0] != 14)
        {
            showActiveStatus();
        }
    }
}

void checkMuteButtons(void)
{

    if (myButtonDelay1.risingEdge())
    { // si le bouton poussoir noir bas 1 a été poussé, on va changer valeur de layer 1
        {
            switches[0] = !switches[0];
            //activeLayer = activeLayer ^ (B0001 << (1-1) );
            // 1 -> 0    0 -> 1
            //activeLayer = ( activeLayer & B00001111 ) ; // on coupe tout ce qui dépasse au dessus des 4 layers ! attention si on augmente le nb de layers faudra changer cette valeur
            //pressedL1time = millis();
        }

        if (softMute == false)
        {
            muteLayer = 0;
            for (byte i = 0; i < nbLayer; i++)
            {
                muteLayer = muteLayer + (switches[i] << (i));
            }
        }

        if (
            Transfo[counterEndUser][0] != 30 && Transfo[counterEndUser][0] != 14 && Transfo[counterEndUser][0] != 246 && Transfo[counterEndUser][0] != 46 && Transfo[counterEndUser][0] != 48 && Transfo[counterEndUser][0] != 14)
        {
            showActiveStatus();
        }

        showMutedStatus();
    }

    if (myButtonDelay2.risingEdge())
    { // si le bouton poussoir noir bas 1 a été poussé, on va changer valeur de layer 1
        {
            switches[1] = !switches[1];
            //activeLayer = activeLayer ^ (B0001 << (2-1) );
            // 1 -> 0    0 -> 1
            //activeLayer = ( activeLayer & B00001111 ) ; // on coupe tout ce qui dépasse au dessus des 4 layers ! attention si on augmente le nb de layers faudra changer cette valeur
            //pressedL2time = millis();
        }

        if (softMute == false)
        {
            muteLayer = 0;
            for (byte i = 0; i < nbLayer; i++)
            {
                muteLayer = muteLayer + (switches[i] << (i));
            }
        }

        if (
            Transfo[counterEndUser][0] != 30 && Transfo[counterEndUser][0] != 14 && Transfo[counterEndUser][0] != 246 && Transfo[counterEndUser][0] != 46 && Transfo[counterEndUser][0] != 48 && Transfo[counterEndUser][0] != 14)
        {
            showActiveStatus();
        }

        showMutedStatus();
    }

    if (myButtonDelay3.risingEdge())
    { // si le bouton poussoir noir bas 1 a été poussé, on va changer valeur de layer 1
        {
            switches[2] = !switches[2];
            //activeLayer = activeLayer ^ (B0001 << (3-1) );
            // 1 -> 0    0 -> 1
            //activeLayer = ( activeLayer & B00001111 ) ; // on coupe tout ce qui dépasse au dessus des 4 layers ! attention si on augmente le nb de layers faudra changer cette valeur
            //pressedL3time = millis();
        }

        if (softMute == false)
        {
            muteLayer = 0;
            for (byte i = 0; i < nbLayer; i++)
            {
                muteLayer = muteLayer + (switches[i] << (i));
            }
        }

        if (
            Transfo[counterEndUser][0] != 30 && Transfo[counterEndUser][0] != 14 && Transfo[counterEndUser][0] != 246 && Transfo[counterEndUser][0] != 46 && Transfo[counterEndUser][0] != 48 && Transfo[counterEndUser][0] != 14)
        {
            showActiveStatus();
        }

        showMutedStatus();
    }

    if (myButtonDelay4.risingEdge())
    { // si le bouton poussoir noir bas 1 a été poussé, on va changer valeur de layer 1
        {
            switches[3] = !switches[3];
            //activeLayer = activeLayer ^ (B0001 << (4-1) );
            // 1 -> 0    0 -> 1
            //activeLayer = ( activeLayer & B00001111 ) ; // on coupe tout ce qui dépasse au dessus des 4 layers ! attention si on augmente le nb de layers faudra changer cette valeur
            //pressedL4time = millis();
        }

        if (softMute == false)
        {
            muteLayer = 0;
            for (byte i = 0; i < nbLayer; i++)
            {
                muteLayer = muteLayer + (switches[i] << (i));
            }
        }

        if (
            Transfo[counterEndUser][0] != 30 && Transfo[counterEndUser][0] != 14 && Transfo[counterEndUser][0] != 246 && Transfo[counterEndUser][0] != 46 && Transfo[counterEndUser][0] != 48 && Transfo[counterEndUser][0] != 14)
        {
            showActiveStatus();
        }

        showMutedStatus();
    }
}

// void checkButton8and9PushButtonsRotaryEncoders ( void ) {

//     if ( digitalRead(62) == HIGH ) {      // digital62 = Analog A8
//         if ( ( millis() - pressedRE8time ) > blocktime ) {

//             counterEncoder = rotKnob9.read() ;
//             counterEndUser = counterEncoder / 4 ;

//             u8x8.clear();
//             buffer_and_HARD_setCursor(11, 4);
//             buffer_and_HARD_print("PUSH a8");
//             delay(100);

//             // qd on est au-dessus de la première transfo d'une catégorie, on envoie à la suivante

//             if (  counterEndUser >= LineCallingTransfo(228) ) { counterEndUser = LineCallingTransfo(111); } else
//             if (  counterEndUser >= LineCallingTransfo(218) ) { counterEndUser = LineCallingTransfo(228) ; }else
//             if (  counterEndUser >= LineCallingTransfo(190) ) { counterEndUser = LineCallingTransfo(218) ; }else
//             if (  counterEndUser >= LineCallingTransfo(87) ) { counterEndUser = LineCallingTransfo(190) ; }else
//             if (  counterEndUser >= LineCallingTransfo(81) ) { counterEndUser = LineCallingTransfo(87) ; }else
//             if (  counterEndUser >= LineCallingTransfo(127) ) { counterEndUser = LineCallingTransfo(81) ; }else
//             if (  counterEndUser >= LineCallingTransfo(64) ) { counterEndUser = LineCallingTransfo(127) ; }else
//             if (  counterEndUser >=  LineCallingTransfo(111) ) { counterEndUser = LineCallingTransfo(64) ; }

//             counterEncoder = counterEndUser * 4  ;
//             rotKnob9.write(counterEncoder ) ;

//             pressedRE8time = millis();
//         }
//     }

//     if ( digitalRead(63) == HIGH ) {      //  digital63 = A9
//         if ( ( millis() - pressedRE9time ) > blocktime ) {

//             counterEncoder = rotKnob9.read() ;
//             counterEndUser = counterEncoder / 4 ;

//             u8x8.clear();
//             buffer_and_HARD_setCursor(11, 4);
//             buffer_and_HARD_print("PUSH a9");
//             delay(100);

//             counterEndUser += 20 ;

//             counterEncoder = counterEndUser * 4  ;
//             rotKnob9.write(counterEncoder ) ;

//             pressedRE9time = millis();
//         }
//     }

// }

void checkButton9OKTransformation(void)
{

    if (myButtonOK.risingEdge())
    {

        // if ( ( millis() - pressed3time ) > blocktime )
        {
            buffer_setCursor(0, 6);
            buffer_print("OK");
            // digitalWrite(5, HIGH);

            Transformation(Transfo[counterEndUser][0], counter2EndUser); //

            // autoHarmonize
            if (autoHarmonize == true)
            {
                //  Transformation(40, modeActuelle);
                for (byte i = 0; i < nbLayer; i++)
                {
                    newHarmonize(i, gammeActuelle[i], modeActuelle[i]);
                }
            }
            // la valeur de gamme est enregistré dans une variable générale
            // la valeur de mode est repassée par modeActuelle

            if (autoCleanUp == true)
            {
                Transformation(114, rememberCleanUpChoice);
            } // est vrai par défaut

            // digitalWrite(5, LOW);

            // pressed3time = millis();
        }
    }
}

void verbosePrintln(String phrase)
{
    if (verboseMIDI)
    {
        Serial.println(phrase);
    }
}

void verbosePrint(String phrase)
{
    if (verboseMIDI)
    {
        Serial.print(phrase);
    }
}

void parallelMidiPlayer(void)
{
    if (playParallelMidiON == true)
    {
        // qd on lance le ParralelMidiPlay, il lit 4+4+2+2+2 bytes de départ, avant la procédure répétitive

        if (MUSTreadHeader == true)
        {
            CurrentMIDIChunk++;
            verbosePrintln("----------------------------");
            verbosePrintln("currentMIDIChunk");
            verbosePrintln(CurrentMIDIChunk);

            verbosePrintln("MTRK? 4D54726B");
            readfromFile(4); // MTrk
            verbosePrintln("longueur chunk");
            readfromFile(4); // code la longueur du Chunk
            MUSTreadHeader = false;
            EndofTrack = false;
            delay(verboseDelay);

            verbosePrint("VLV = ");
            ReadVLVDelta(); // renvoie info dans DureeDelta et VLVDelta !
            verbosePrintln("=" + String(DureeDelta) + "=" + String(VLVDelta));

            delay(verboseDelay);

            checkSkipChunckAndCalcGoOn(); // modifie DureeDelta et GoONmidi

            verbosePrint("goONmidi = ");
            verbosePrintln(String(GoONmidi));
            delay(verboseDelay);
        }

        parallelMIDItimeINIT = nowMillis;

        if (parallelMIDItimeINIT >= GoONmidi)
        {

            // AJOUT 1C86
            if (myFile.available() == false)
            {
                EndofTrack = true;
            }

            // alors il est temps de lire une action MIDI et de l'exécuter

            do
            {
                readfromFile(1);

                exact1stbyte = mididouble & B11111111;
                byte midicategory = (mididouble & B11110000);
                // byte midichannel = ( mididouble & B00001111 ) ;

                switch (midicategory)
                {
                case (0x80):
                {
                    verbosePrint("note OFF ");
                    readfromFile(2);
                    if (!SkipChunck)
                    {
                        // PlaynoteOn2(exact1stbyte, mididouble >> 8, mididouble &  B01111111, B10000000);
                        PlaynoteOn2(exact1stbyte, mididouble >> 8, 0, B10000000); // 1C87
                    }
                    lastRunningStatus = 0x80; // on en aura besoin pour compter les notes On et Off
                    lastexact1stbyte = exact1stbyte;
                }
                break;

                case (0x90):
                {
                    verbosePrint("note ON ");
                    readfromFile(2);
                    if (!SkipChunck)
                    {
                        PlaynoteOn2(exact1stbyte, byte(mididouble >> 8), byte(mididouble & B01111111), B10000000);
                    }
                    lastexact1stbyte = exact1stbyte;
                    lastRunningStatus = 0x90; // on en aura besoin pour compter les notes On et Off
                    midiFile_progress++ ; // 2C28 
                }
                break;

                case (0xA0):
                { // Polyphonic Key Pressure
                    verbosePrint("polyphonic ");
                    readfromFile(2);
                    if (!SkipChunck)
                    {
                        PlaynoteOn2(exact1stbyte, byte(mididouble >> 8), byte(mididouble & B01111111), B10000000);
                    }
                }
                break;

                case (0xB0):
                { // Control Change
                    verbosePrint("CC ");
                    readfromFile(2);
                    if (!SkipChunck)
                    {
                        PlaynoteOn2(exact1stbyte, byte(mididouble >> 8), byte(mididouble & B01111111), B10000000);
                    }
                }
                break;

                case (0xC0):
                { // Program Change
                    verbosePrint("PC ");
                    readfromFile(1);
                }
                break;

                case (0xD0):
                { // Channel Pressure
                    verbosePrint("channel pressure ");
                    readfromFile(1);
                }
                break;

                case (0xE0):
                { // pitch Wheel Change
                    verbosePrint("pitch wheel change ");
                    readfromFile(2);
                    if (!SkipChunck)
                    {
                        PlaynoteOn2(exact1stbyte, byte(mididouble >> 8), byte(mididouble & B01111111), B10000000);
                    }
                }
                break;

                case (0xF0):
                { // System Common Messages
                    verbosePrint("system common message ");
                    exact1stbyte = mididouble;
                    readfromFile(1); // lit ici le 2nd byte du FF
                    exact2ndbyte = mididouble;

                    if ((exact1stbyte == 0xFF) && (exact2ndbyte == 0x2F))
                    {
                        verbosePrint("END OF TRACK ");
                        EndofTrack = true;
                        MUSTreadHeader = true;
                        SkipChunck = false;
                    }

                    readfromFile(1);                      // lit le 3ème byte = la durée à lire -> mididouble
                    readfromFile(mididouble & B11111111); // va lire le nb de bytes précisés juste au dessus

                    if (exact2ndbyte == 0x51)
                    {
                        verbosePrintln("0x51");
                        calculatedBPM = int(long((float)60000000 / (float)mididouble));
                    }
                }
                break;

                default:
                { // Running Status ???

                    verbosePrintln("running status ?");

                    switch (lastRunningStatus)
                    {

                    case (0x80):
                    {
                        verbosePrint("RS note OFF ");
                        specialmididouble = mididouble;
                        readfromFile(1);
                        mididouble = (specialmididouble << 8) + mididouble;
                        if (!SkipChunck)
                        {
                            PlaynoteOn2(lastexact1stbyte, byte(mididouble >> 8), byte(mididouble & B01111111), B10000000);
                        }
                    }
                    break;

                    case (0x90):
                    {
                        verbosePrint("RS note ON ");
                        specialmididouble = mididouble;
                        readfromFile(1);
                        midiFile_progress++; // 2C28
                        mididouble = (specialmididouble << 8) + mididouble;
                        if (!SkipChunck)
                        {
                            PlaynoteOn2(lastexact1stbyte, byte(mididouble >> 8), byte(mididouble & B01111111), B10000000);
                        }
                    }
                    break;

                    case (0xA0):
                    { // Polyphonic Key Pressure
                        verbosePrint("RS polyphonic ");
                        specialmididouble = mididouble;
                        readfromFile(1);
                        mididouble = (specialmididouble << 8) + mididouble;
                        if (!SkipChunck)
                        {
                            PlaynoteOn2(lastexact1stbyte, byte(mididouble >> 8), byte(mididouble & B01111111), B10000000);
                        }
                    }
                    break;

                    case (0xB0):
                    { // Control Change
                        verbosePrint("RS CC ");
                        specialmididouble = mididouble;
                        readfromFile(1);
                        mididouble = (specialmididouble << 8) + mididouble;
                        if (!SkipChunck)
                        {
                            PlaynoteOn2(lastexact1stbyte, byte(mididouble >> 8), byte(mididouble & B01111111), B10000000);
                        }
                    }
                    break;

                    case (0xC0):
                    { // Program Change
                        verbosePrintln("RS PC");
                    }
                    break;

                    case (0xD0):
                    { // Channel Pressure
                        verbosePrintln("RS channel pressure");
                    }
                    break;

                    case (0xE0):
                    { // pitch Wheel Change
                        verbosePrint("RS pitch wheeel ");
                        //midiplayerdel y() ;
                        specialmididouble = mididouble;
                        readfromFile(1);
                        mididouble = (specialmididouble << 8) + mididouble;
                        if (!SkipChunck)
                        {
                            PlaynoteOn2(lastexact1stbyte, byte(mididouble >> 8), byte(mididouble & B01111111), B10000000);
                        }
                    }
                    break;

                    case (0xF0):
                    { // System Common Messages
                        verbosePrint("RS system common message ");
                        exact1stbyte = mididouble;
                        readfromFile(1); // lit ici le 2nd byte du FF
                        exact2ndbyte = mididouble;

                        if ((exact1stbyte == 0xFF) && (exact2ndbyte == 0x2F))
                        {
                            verbosePrintln("RS end of track");
                            EndofTrack = true;
                            MUSTreadHeader = true;
                            SkipChunck = false;
                        }

                        readfromFile(1);                      // lit le 3ème byte = la durée à lire -> mididouble
                        readfromFile(mididouble & B11111111); // va lire le nb de bytes précisés juste au dessus

                        if (exact2ndbyte == 0x51)
                        {
                            calculatedBPM = int(long((float)60000000 / (float)mididouble));
                        }
                    }
                    break;

                        // Par rapport au MIDI parser cette  partie a été modifiée pour tenir compte d'éventuels Running Status avec autre chose que 0x80 et 0x90

                    default:
                        unknownRunningStatus++;
                        verbosePrintln("unknow running status !!!");
                        break;
                    }

                } // end de Running Status ! qd le 1er byte n'est pas 80 90 A0 B0 C0 D0 E0 F0
                break;

                } // end du Switch de Midicategory après lecture d'un byte

                if (EndofTrack == false)
                {

                    if (specialDebug)
                    {
                        // Serial.print(F("B"));
                    }
                    verbosePrint("VLV = ");
                    ReadVLVDelta(); // renvoie info dans DureeDelta et VLVDelta !

                    checkSkipChunckAndCalcGoOn();

                    // if (myFile.available() != true) {EndofTrack = true ; }
                }
                // *****************
                // DEBUG checke peut être trop le myFile.available();
                //    }  while ( ( EndofTrack==false) && (DureeDelta==0) && (myFile.available() == true)  ) ;
                // }  while ( ( EndofTrack==false) && (DureeDelta==0) && (myFile.available() == true)    ) ;
            } while ((EndofTrack == false) && (DureeDelta == 0));

            // EndofTrack = false ;
            // MUSTreadHeader  = true ;

            if (EndofTrack == true)
            {
                verbosePrintln("END OF TRACK");
                if (myFile.available() == false)
                {
                    verbosePrintln("ENF OF FILE");
                    playParallelMidiON = false;
                    myFile.close();
                    measureSDFileCount();
                }
                // EndofTrack = true ;
                verbosePrintln("MUST READ HEADER");
                MUSTreadHeader = true;
                verbosePrintln("NO SKIP CHUNCK");
                SkipChunck = false;
            }

        } // fin du millis > GoON

    } // fin du parallèle midi player
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
    sequence[t].jitterRichting = 1; // variables internes, qu'on ne modifie pas soi même
    // NOTE DEBUG peut être transformé en version courte, si code suit
    sequence[t].jitterWhereNow = 0; // variables internes, qu'on ne modifie pas soi même
    sequence[t].jitterWhereTo = 1;  // variables internes, qu'on ne modifie pas soi même

    sequence[t].jitterStaticCompteur = 0; // variables internes, qu'on ne modifie pas soi même
    sequence[t].jitterStaticToReach = 0;  // variables internes, qu'on ne modifie pas soi même

    // int jitterPolarity(0); // -1 : wherenow < whereTo : +1 : > : 0 =
    // int oldJitterPolarity(0) ; // dernière valeur
    sequence[t].beingStatic = false; // variables internes, qu'on ne modifie pas soi même

    sequence[t].sizeRANDOMFRAG = 1;
    sequence[t].repeatRANDOMFRAG = 0;
    sequence[t].toReachRANDOMFRAG = 1; // variables internes, qu'on ne modifie pas soi même
    sequence[t].keepN = 0;

    sequence[t].compteur = 0; // +1 à chaque itération, vaudra un grand nombre
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

void verboseNoteAPileB(byte noteToCreate, int i)
{

    unsigned int taille = mesureTaillePileComplete(pInitial[i]);
    pileNote tempNote;

    Serial.println("-------------------------------");

    for (unsigned int j = 0; j < taille; j++)
    { //
        tempNote = ZEROgetNoteFromPile(pInitial[i], j);

        if (tempNote.note == noteToCreate && tempNote.channel == stepChannel)
        {
            Serial.print("CH");
            Serial.print(String(tempNote.channel));
            Serial.print("-N");
            Serial.print(String(tempNote.note));
            Serial.print("\t");
            Serial.print(String(tempNote.startNote));
            Serial.print("\t");
            Serial.print(String(tempNote.endNote));
            Serial.println();
        }
    }

    Serial.println("-------------------------------");
}

void verboseFullPile(int i)
{

    unsigned int taille = mesureTaillePileComplete(pInitial[i]);
    pileNote tempNote;

    Serial.println("\n-------------------------------");

    for (unsigned int j = 0; j < taille; j++)
    { //
        tempNote = ZEROgetNoteFromPile(pInitial[i], j);

        //  if (tempNote.note == noteToCreate && tempNote.channel == stepChannel)
        {
            Serial.print("CH");
            Serial.print(String(tempNote.channel));
            Serial.print("-N");
            Serial.print(String(tempNote.note));
            Serial.print("\t");
            Serial.print("v");
            Serial.print(String(tempNote.velocity));
            Serial.print("\t");
            Serial.print(String(tempNote.startNote));
            Serial.print("-");
            Serial.print(String(tempNote.endNote));
            // COMPLETER SI NECESSAIRE
            Serial.println();
        }
    }

    Serial.println("-------------------------------");
}

bool thereIsCHAccent(int channel, int i)
{
    unsigned int taille = mesureTaillePileComplete(pInitial[i]);
    pileNote tempNote;

    for (unsigned int j = 0; j < taille; j++)
    { //
        tempNote = ZEROgetNoteFromPile(pInitial[i], j);

        if (tempNote.note == 0 && tempNote.channel == channel && tempNote.type == 0x50)
        {
            return true; // on a trouvé au moins 1 accent CHANNEL sur le layer i
        }
    }
    return false;
}

bool noteStartingAtAisinAccentZoneCHBLayerC(unsigned long start, byte channel, byte i)
{
    unsigned int taille = mesureTaillePileComplete(pInitial[i]);
    pileNote tempNote;

    for (unsigned int j = 0; j < taille; j++)
    { //
        tempNote = ZEROgetNoteFromPile(pInitial[i], j);

        if (tempNote.note == 0 && tempNote.channel == channel && tempNote.type == 0x50 && tempNote.startNote <= start && tempNote.endNote >= start)
        {
            return true; // on a trouvé au moins 1 accent CHANNEL sur le layer i
        }
    }
    return false;
}

bool thereIsTotalAccent(int i)
{
    unsigned int taille = mesureTaillePileComplete(pInitial[i]);
    pileNote tempNote;

    for (unsigned int j = 0; j < taille; j++)
    { //
        tempNote = ZEROgetNoteFromPile(pInitial[i], j);

        if (tempNote.note == 128 && tempNote.type == 0x50)
        {
            return true; // on a trouvé au moins 1 accent TOTAL sur le layer i
        }
    }
    return false;
}

bool noteStartingAtAisinTotalAccentZoneLayerC(unsigned long start, byte i)
{
    unsigned int taille = mesureTaillePileComplete(pInitial[i]);
    pileNote tempNote;

    for (unsigned int j = 0; j < taille; j++)
    { //
        tempNote = ZEROgetNoteFromPile(pInitial[i], j);

        if (tempNote.note == 128 && tempNote.type == 0x50 && tempNote.startNote <= start && tempNote.endNote >= start)
        {
            return true; // on a trouvé au moins 1 accent CHANNEL sur le layer i
        }
    }
    return false;
}

void StepzButtonPushed_LayerA_NoteB_ButtonC(int i, byte noteToCreate, byte ButtonPushed)
{
    // petit tour de passe-passe transformant un affichage de n boutons en affichage de p pages de n boutons !

    ButtonPushed = currentPageStepz * nbStepzAffiches + ButtonPushed;

    unsigned int nbCases = nbStepz * nbSubStepz;
    unsigned long duree1subStep = layerLength[i] / (unsigned long)(nbCases);

    // on peut dès lors déterminer combien de notes dans chaque subStep
    // puis les regrouper par Step _ - = # 11 52

    byte tableauSubStepz[nbCases] = {0};
    byte tableauStepz[nbStepz] = {0};

    // insérer ici un VerbosePile(noteN) ;
    // verboseNoteAPileB(noteToCreate,i);

    unsigned int taille = mesureTaillePileComplete(pInitial[i]);
    pileNote tempNote;

    for (unsigned int j = 0; j < taille; j++)
    { //
        tempNote = ZEROgetNoteFromPile(pInitial[i], j);

        if (tempNote.note == noteToCreate && tempNote.channel == stepChannel)
        {
            tableauSubStepz[tempNote.startNote / duree1subStep] = 1;
        }

        if (tempNote.note == noteToCreate && noteToCreate == 128)
        {
            tableauSubStepz[tempNote.startNote / duree1subStep] = 1;
        }
    }

    for (unsigned int j = ButtonPushed; j <= ButtonPushed; j++)
    {
        for (unsigned k = 0; k < nbSubStepz; k++)
        {
            tableauStepz[j] = tableauStepz[j] + (tableauSubStepz[j * nbSubStepz + k] << k);
        }
    }

    // le tableau Stepz est OK
    // on rajoute 1 et si trop haut on remet à zéro

    tableauStepz[ButtonPushed] = tableauStepz[ButtonPushed] + 1;

    // tableauStepz[ButtonPushed] = 4 ;

    if (tableauStepz[ButtonPushed] > plusGrandNombreAvecAbits(nbSubStepz))
    {
        tableauStepz[ButtonPushed] = 0;
    }

    // on déduit maintenant les subStepz à partir des Stepz ;

    for (unsigned k = 0; k < nbSubStepz; k++)
    {
        // tableauStepz[j] = tableauStepz[j] + (tableauSubStepz[j * nbSubStepz + k] << k);
        // int place = ButtonPushed * ( nbSubStepz ) + k;  // avant 2C23
        int place = ButtonPushed * (nbSubStepz) + k;
        // DEBUG 2C23

        tableauSubStepz[place] = ((byte)tableauStepz[ButtonPushed] >> k) & B00000001;

        // si vaut 1 : créer note
        // si vaut 0 : effacer les notes

        // on va maintenant passer en revue la pile de notes, effacer et créer.
        // d'abord on efface

        if (tableauSubStepz[place] == 0)
        {
            // on efface
            for (unsigned int j = 0; j < taille; j++)
            { //
                tempNote = ZEROgetNoteFromPile(pInitial[i], j);

                if (tempNote.note == noteToCreate && tempNote.channel == stepChannel)
                {
                    if (((unsigned long)tempNote.startNote / (unsigned long)duree1subStep) == (unsigned long)place)
                    {
                        tempNote.note = 0;
                        tempNote.killnote = true;
                        tempNote.startNote = 0;
                        tempNote.endNote = 0;
                        tempNote.endNote = 0;
                        tempNote.velocity = 0;
                        ZEROSetNoteInPile(pInitial[i], j, tempNote);
                    }
                };

                if (tempNote.note == noteToCreate && noteToCreate == 128)
                {
                    if (((unsigned long)tempNote.startNote / (unsigned long)duree1subStep) == (unsigned long)place)
                    {
                        tempNote.note = 0;
                        tempNote.killnote = true;
                        tempNote.startNote = 0;
                        tempNote.endNote = 0;
                        tempNote.endNote = 0;
                        tempNote.velocity = 0;
                        ZEROSetNoteInPile(pInitial[i], j, tempNote);
                    }
                };
            }
            // ZEROupdatePileTimeToPlayMODEDIRECT(i);
        }
        else
        {
            // on crée la note
            pileNote tempNote2;
            tempNote2.startNote = ((unsigned long)place * duree1subStep + (unsigned long)1) % layerLength[i];
            tempNote2.thereIsAnEnd = true;

            switch (stopModeStepz)
            {
            case 0:
            {
                tempNote2.endNote = (tempNote2.startNote + duree1subStep - (unsigned long)1) % layerLength[i];
            }
            break;

            case 1:
            {
                tempNote2.endNote = (tempNote2.startNote + ((unsigned long)nbSubStepz * duree1subStep) - 1) % layerLength[i];
            }
            break;

            case 2:
            {
                tempNote2.endNote = (tempNote2.startNote + (unsigned long)10) % layerLength[i];
            }
            break;

            case 3:
            {
                tempNote2.endNote = 0;
                tempNote2.thereIsAnEnd = false;
            }
            break;

            default:
            {
                tempNote2.endNote = (tempNote2.startNote + (layerLength[i] * (unsigned long)(stopModeStepz - 3)) / (unsigned long)32) % layerLength[i];
            }
            break;
            }

            tempNote2.note = noteToCreate;
            tempNote2.layer = i;
            tempNote2.channel = stepChannel;
            tempNote2.velocity = stepVelocity;
            tempNote2.killnote = false;
            tempNote2.type = 0x90;

            if (tempNote2.note == 0)
            {
                tempNote2.type = 0x50;
            } // ACCENT  2C24

            if (tempNote2.note == 128) // TOTAL ACCENT 2C24
            {
                tempNote2.type = 0x50;
            } // ACCENT  2C24

            tempNote2.MMnoteReallyPlayed = noteToCreate;
            tempNote2.thereIsAnEnd = true;
            // unsigned long timeToPlayON;

            // suppression des doublons
            for (unsigned int j = 0; j < taille; j++)
            { //
                tempNote = ZEROgetNoteFromPile(pInitial[i], j);

                if (keepStepzBUG == false)
                {
                    if (tempNote.note == tempNote2.note && tempNote.channel == tempNote2.channel && tempNote.startNote == tempNote2.startNote)
                    {
                        // if (((unsigned long)tempNote.startNote / (unsigned long)duree1subStep) == (unsigned long)place)
                        {
                            tempNote.note = 0;
                            tempNote.killnote = true;
                            tempNote.startNote = 0;
                            tempNote.endNote = 0;
                            tempNote.endNote = 0;
                            tempNote.velocity = 0;
                            ZEROSetNoteInPile(pInitial[i], j, tempNote);
                        }
                    };
                }
            }

            ZEROaddNewNotetoPile(pInitial[i], tempNote2);
        }
    }

    // insérer ici un VerbosePile(noteN) ;
    // verboseNoteAPileB(noteToCreate, i);

    ZEROupdatePileTimeToPlayMODEDIRECT(i);

    // insérer ici un VerbosePile(noteN) ;
    //   verboseNoteAPileB(noteToCreate, i);
}

void checkStepzButtons(void)
{
    // qd un bouton est poussé :
    // on fait l'état des lieux des subStepez
    // cela détermine par synthèse un état du Stepz
    // on prend le Stepz au-dessus
    // on efface les notes concernées qui se trouvent dans l'état initial
    // on place les notes du nouvel état

    // la procédure est la même pour chaque bouton Stepz
    // sauf décalage
    // il faut donc une sous-procédure pour la réutiliser

for ( byte j = 0 ; j< 16 ; j++ )
{
if ( keyboardStepz[j] == 1 )
{
    for (byte i = 0; i < nbLayer; i++)
    {
        if (isLayerActive(i))
        {
            StepzButtonPushed_LayerA_NoteB_ButtonC(i, currentStepzNote, j);
        }
    }
    keyboardStepz[j] = 0 ; 
}
}

    if (myButtonDelay1.risingEdge())
    { // si le bouton poussoir noir bas 1 a été poussé, on va changer valeur de layer 1
        for (byte i = 0; i < nbLayer; i++)
        {
            if (isLayerActive(i))
            {
                StepzButtonPushed_LayerA_NoteB_ButtonC(i, currentStepzNote, 0);
            }
        }
    }

    if (myButtonDelay2.risingEdge())
    { // si le bouton poussoir noir bas 1 a été poussé, on va changer valeur de layer 1
        for (byte i = 0; i < nbLayer; i++)
        {
            if (isLayerActive(i))
            {
                StepzButtonPushed_LayerA_NoteB_ButtonC(i, currentStepzNote, 1);
            }
        }
    }

    if (myButtonDelay3.risingEdge())
    { // si le bouton poussoir noir bas 1 a été poussé, on va changer valeur de layer 1
        for (byte i = 0; i < nbLayer; i++)
        {
            if (isLayerActive(i))
            {
                StepzButtonPushed_LayerA_NoteB_ButtonC(i, currentStepzNote, 2);
            }
        }
    }

    if (myButtonDelay4.risingEdge())
    { // si le bouton poussoir noir bas 1 a été poussé, on va changer valeur de layer 1
        for (byte i = 0; i < nbLayer; i++)
        {
            if (isLayerActive(i))
            {
                StepzButtonPushed_LayerA_NoteB_ButtonC(i, currentStepzNote, 3);
            }
        }
    }
}

void PuntzButtonPushed_LayerA_NoteB_ButtonC(int i, byte noteToCreate, byte puntzVelocity)
{
    unsigned int nbCases = nbStepz * nbSubStepz;
    unsigned long duree1subStep = layerLength[i] / (unsigned long)(nbCases);

    // on crée la note
    pileNote tempNote2;
    tempNote2.startNote = nowMillis % layerLength[i];
    tempNote2.thereIsAnEnd = true;

    switch (stopModeStepz)
    {
    case 0:
    {
        tempNote2.endNote = (tempNote2.startNote + duree1subStep - 3) % layerLength[i];
    }
    break;
    case 1:
    {
        tempNote2.endNote = (tempNote2.startNote + (nbSubStepz * duree1subStep) - 3) % layerLength[i];
    }
    break;
    case 2:
    {
        tempNote2.endNote = (tempNote2.startNote + 10) % layerLength[i];
    }
    break;
    case 3:
    {
        tempNote2.endNote = 0;
        tempNote2.thereIsAnEnd = false;
    }
    break;
    default:
    {
        tempNote2.endNote = (tempNote2.startNote + (layerLength[i] * (unsigned long)(stopModeStepz - 3) / (unsigned long)32)) % layerLength[i];
    }
    break;
    }

    tempNote2.note = noteToCreate;
    tempNote2.layer = i;
    tempNote2.channel = stepChannel;
    tempNote2.velocity = puntzVelocity;
    tempNote2.killnote = false;
    tempNote2.type = 0x90;
    ZEROaddNewNotetoPile(pInitial[i], tempNote2);

    ZEROupdatePileTimeToPlayMODEDIRECT(i);
}

void checkPuntzButtons(void)
{

    // qd un bouton est poussé :
    // on vérifie lequel
    // on attribue une vélocité en fc du bouton
    // une note est ajoutée au layer actif

    int velocityPuntz = 100;

    if (myButtonDelay1.risingEdge())
    {
        velocityPuntz = 30;
        for (byte i = 0; i < nbLayer; i++)
        {
            if (isLayerActive(i))
            {
                PuntzButtonPushed_LayerA_NoteB_ButtonC(i, currentStepzNote, velocityPuntz);
            }
        }
    }

    if (myButtonDelay2.risingEdge())
    {
        velocityPuntz = 60;
        for (byte i = 0; i < nbLayer; i++)
        {
            if (isLayerActive(i))
            {
                PuntzButtonPushed_LayerA_NoteB_ButtonC(i, currentStepzNote, velocityPuntz);
            }
        }
    }

    if (myButtonDelay3.risingEdge())
    {
        velocityPuntz = 90;
        for (byte i = 0; i < nbLayer; i++)
        {
            if (isLayerActive(i))
            {
                PuntzButtonPushed_LayerA_NoteB_ButtonC(i, currentStepzNote, velocityPuntz);
            }
        }
    }

    if (myButtonDelay4.risingEdge())
    {
        velocityPuntz = 120;
        for (byte i = 0; i < nbLayer; i++)
        {
            if (isLayerActive(i))
            {
                PuntzButtonPushed_LayerA_NoteB_ButtonC(i, currentStepzNote, velocityPuntz);
            }
        }
    }
}

void PolyZButtonPushed_LayerA_NoteB_ButtonC(int i, byte noteToCreate, byte ButtonPushed)
{
    // petit tour de passe-passe transformant un affichage de n boutons en affichage de p pages de n boutons !

    ButtonPushed = currentPageStepz * nbStepzAffiches + ButtonPushed;

    unsigned int nbCases = numerateurTimeSignature * denominateurTimeSignature; 
    unsigned int nbCasesTotal = numerateurTimeSignature * denominateurTimeSignature;

    byte tableauCasesTotal[nbCasesTotal]; // contient un tableau avec 1 si fait partie du temps, 0 si pas 

    unsigned long duree1subStep = layerLength[i] / (unsigned long)(nbCases);

    // on peut dès lors déterminer combien de notes dans chaque subStep
    // puis les regrouper par Step _ - = # 11 52

    int CtStepz = 0;
    for (unsigned int j = 0; j < nbCasesTotal; j++)
    {
        if ((j % numerateurTimeSignature == 0) || (j % denominateurTimeSignature == 0))
        {
            tableauCasesTotal[j] = 1;
            CtStepz++;
        }
        else
        {
            tableauCasesTotal[j] = 0;
        }
        // on remplit les cases du tableau qui divisent num ou dénom, çàd les temps qui sont ON
    }

    byte tableauSubStepz[nbCases] = {0};
    byte tableauStepz[CtStepz] = {0};

// on va remplir ces 2 tableaux, sachant la répartition qui mène de tableauSubStepz à tableauStepz 

    unsigned int taille = mesureTaillePileComplete(pInitial[i]);
    pileNote tempNote;

    for (unsigned int j = 0; j < taille; j++)
    { //
        tempNote = ZEROgetNoteFromPile(pInitial[i], j);

        if (tempNote.note == noteToCreate && tempNote.channel == stepChannel)
        {
            tableauSubStepz[tempNote.startNote / duree1subStep] = 1;
        }

        if (tempNote.note == noteToCreate && noteToCreate == 128)
        {
            tableauSubStepz[tempNote.startNote / duree1subStep] = 1;
        }
    }

    // for (unsigned int j = ButtonPushed; j <= ButtonPushed; j++)
    // {
    //     for (unsigned k = 0; k < nbSubStepz; k++)
    //     {
    //         tableauStepz[j] = tableauStepz[j] + (tableauSubStepz[j * nbSubStepz + k] << k);
    //     }
    // }

    CtStepz = 0;
    for (unsigned int j = 0; j < nbCasesTotal; j++)
    {
        if (tableauCasesTotal[j] == 1)
        {
//            tableauStepz[CtStepz] = tableauSubStepz[j];
          if ( tableauSubStepz[j] > 0 ) 
            {
                 tableauStepz[CtStepz] = 1 ;   
            };
            CtStepz++;
        }
    }
    numberMaxPageStepz = ( CtStepz - 1 ) / nbStepzAffiches;

    // le tableau Stepz est OK
    // on rajoute 1 et si trop haut on remet à zéro

    tableauStepz[ButtonPushed] = tableauStepz[ButtonPushed] + 1;

    // tableauStepz[ButtonPushed] = 4 ;

   // if (tableauStepz[ButtonPushed] > plusGrandNombreAvecAbits(nbSubStepz))
    if (tableauStepz[ButtonPushed] > 1 ) 
    {
        tableauStepz[ButtonPushed] = 0;
    }

    // on déduit maintenant les subStepz à partir des Stepz ;

    CtStepz = 0;
    bool foundPlace = false ; 
  //  int place = 0 ; 

    for (unsigned int j = 0; j < nbCasesTotal; j++)
    {
        if ( CtStepz == ButtonPushed && foundPlace == false )
        {
          //  place = j ; 
            foundPlace = true ;
        }

        if (tableauCasesTotal[j] == 1)
        {
            tableauSubStepz[j] = tableauStepz[CtStepz] ; 
            CtStepz++;
        }

       // Serial.print(String(tableauSubStepz[j])); 
    }
    // Serial.println(); 


    for (unsigned k = 0; k < nbCasesTotal; k++)
    // int k = 0; 
    {
        // tableauStepz[j] = tableauStepz[j] + (tableauSubStepz[j * nbSubStepz + k] << k);
        // int place = ButtonPushed * ( nbSubStepz ) + k;  // avant 2C23
        // int place = ButtonPushed * (nbSubStepz) + k;
        // 2C27 place est déterminé ici au-dessus 
        // DEBUG 2C23

      //  tableauSubStepz[place] = ((byte)tableauStepz[ButtonPushed] >> k) & B00000001;

        // si vaut 1 : créer note
        // si vaut 0 : effacer les notes

        // on va maintenant passer en revue la pile de notes, effacer et créer.
        // d'abord on efface

        if (tableauSubStepz[k] == 0)
        {
            // on efface
            taille = mesureTaillePileComplete(pInitial[i]);

            for (unsigned int j = 0; j < taille; j++)
            { //
                tempNote = ZEROgetNoteFromPile(pInitial[i], j);

                if (tempNote.note == noteToCreate && tempNote.channel == stepChannel)
                {
                    if (((unsigned long)tempNote.startNote / (unsigned long)duree1subStep) == (unsigned long) k)
                    {
                        tempNote.note = 0;
                        tempNote.killnote = true;
                        tempNote.startNote = 0;
                        tempNote.endNote = 0;
                        tempNote.endNote = 0;
                        tempNote.velocity = 0;
                        ZEROSetNoteInPile(pInitial[i], j, tempNote);
                    }
                };

                if (tempNote.note == noteToCreate && noteToCreate == 128)
                {
                    if (((unsigned long)tempNote.startNote / (unsigned long)duree1subStep) == (unsigned long) k )
                    {
                        tempNote.note = 0;
                        tempNote.killnote = true;
                        tempNote.startNote = 0;
                        tempNote.endNote = 0;
                        tempNote.endNote = 0;
                        tempNote.velocity = 0;
                        ZEROSetNoteInPile(pInitial[i], j, tempNote);
                    }
                };
            }
            // ZEROupdatePileTimeToPlayMODEDIRECT(i);
        }
        else
        {
            // on crée la note
            pileNote tempNote2;
            tempNote2.startNote = ((unsigned long) k * duree1subStep + (unsigned long)1) % layerLength[i];
            tempNote2.thereIsAnEnd = true;

            switch (stopModeStepz)
            {
            case 0:
            {
                tempNote2.endNote = (tempNote2.startNote + duree1subStep - (unsigned long)1) % layerLength[i];
            }
            break;

            case 1:
            {
                tempNote2.endNote = (tempNote2.startNote + ((unsigned long)nbSubStepz * duree1subStep) - 1) % layerLength[i];
            }
            break;

            case 2:
            {
                tempNote2.endNote = (tempNote2.startNote + (unsigned long)10) % layerLength[i];
            }
            break;

            case 3:
            {
                tempNote2.endNote = 0;
                tempNote2.thereIsAnEnd = false;
            }
            break;

            default:
            {
                tempNote2.endNote = (tempNote2.startNote + (layerLength[i] * (unsigned long)(stopModeStepz - 3)) / (unsigned long)32) % layerLength[i];
            }
            break;
            }

            tempNote2.note = noteToCreate;
            tempNote2.layer = i;
            tempNote2.channel = stepChannel;
            tempNote2.velocity = stepVelocity;
            tempNote2.killnote = false;
            tempNote2.type = 0x90;

            if (tempNote2.note == 0)
            {
                tempNote2.type = 0x50;
            } // ACCENT  2C24

            if (tempNote2.note == 128) // TOTAL ACCENT 2C24
            {
                tempNote2.type = 0x50;
            } // ACCENT  2C24

            tempNote2.MMnoteReallyPlayed = noteToCreate;
            tempNote2.thereIsAnEnd = true;
            // unsigned long timeToPlayON;

            // suppression des doublons
            for (unsigned int j = 0; j < taille; j++)
            { //
                tempNote = ZEROgetNoteFromPile(pInitial[i], j);

                if (keepStepzBUG == false)
                {
                    if (tempNote.note == tempNote2.note && tempNote.channel == tempNote2.channel && tempNote.startNote == tempNote2.startNote)
                    {
                        // if (((unsigned long)tempNote.startNote / (unsigned long)duree1subStep) == (unsigned long)place)
                        {
                            tempNote.note = 0;
                            tempNote.killnote = true;
                            tempNote.startNote = 0;
                            tempNote.endNote = 0;
                            tempNote.endNote = 0;
                            tempNote.velocity = 0;
                            ZEROSetNoteInPile(pInitial[i], j, tempNote);
                        }
                    };
                }
            }

            ZEROaddNewNotetoPile(pInitial[i], tempNote2);
        }
    }

    // insérer ici un VerbosePile(noteN) ;
    // verboseNoteAPileB(noteToCreate, i);

    ZEROupdatePileTimeToPlayMODEDIRECT(i);

    // insérer ici un VerbosePile(noteN) ;
    //   verboseNoteAPileB(noteToCreate, i);
}


void checkPolyZButtons(void)
{
    // qd un bouton est poussé :
    // on fait l'état des lieux des subStepez
    // cela détermine par synthèse un état du Stepz
    // on prend le Stepz au-dessus
    // on efface les notes concernées qui se trouvent dans l'état initial
    // on place les notes du nouvel état

    // la procédure est la même pour chaque bouton Stepz
    // sauf décalage
    // il faut donc une sous-procédure pour la réutiliser

    if (myButtonDelay1.risingEdge())
    { // si le bouton poussoir noir bas 1 a été poussé, on va changer valeur de layer 1
        for (byte i = 0; i < nbLayer; i++)
        {
            if (isLayerActive(i))
            {
                PolyZButtonPushed_LayerA_NoteB_ButtonC(i, currentStepzNote, 0);
            }
        }
    }

    if (myButtonDelay2.risingEdge())
    { // si le bouton poussoir noir bas 1 a été poussé, on va changer valeur de layer 1
        for (byte i = 0; i < nbLayer; i++)
        {
            if (isLayerActive(i))
            {
                PolyZButtonPushed_LayerA_NoteB_ButtonC(i, currentStepzNote, 1);
            }
        }
    }

    if (myButtonDelay3.risingEdge())
    { // si le bouton poussoir noir bas 1 a été poussé, on va changer valeur de layer 1
        for (byte i = 0; i < nbLayer; i++)
        {
            if (isLayerActive(i))
            {
                PolyZButtonPushed_LayerA_NoteB_ButtonC(i, currentStepzNote, 2);
            }
        }
    }

    if (myButtonDelay4.risingEdge())
    { // si le bouton poussoir noir bas 1 a été poussé, on va changer valeur de layer 1
        for (byte i = 0; i < nbLayer; i++)
        {
            if (isLayerActive(i))
            {
                PolyZButtonPushed_LayerA_NoteB_ButtonC(i, currentStepzNote, 3);
            }
        }
    }
}
