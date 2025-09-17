void measureSDFileCount(void)
{
    rootFileCount = 0;

    // Open root directory
    if (!dir.open("/"))
    {
        error("dir.open failed");
    }

    // Open next file in root.
    // Warning, openNext starts at the current position of dir so a
    // rewind may be necessary in your application.
    while (myFile.openNext(&dir, O_READ))
    {
        rootFileCount++;

        // myFile.printFileSize(&Serial);
        // Serial.write(' ');
        // myFile.printModifyDateTime(&Serial);
        // Serial.write(' ');
        // myFile.printName(&Serial);
        if (myFile.isDir())
        {
            // Indicate a directory.
            //    Serial.write('/');
        }
        // Serial.println();
        myFile.close();
    }
    // Serial.println("Done!");

    Transfo[LineCallingTransfo(373)][2] = rootFileCount;
    // max pour encoderRight est le rootfilecount
}

void initialisationsDiverses(void)
{

    SizeTransfo = sizeof(Transfo) / sizeof(Transfo[0]);
    // Serial.print("SizeTransfo=");
    // Serial.print(SizeTransfo);
    // BLACK08 : il m'aura fallu tout ce temps pour trouver comment automatiser le calcul de taille d'un array
    // à deux dimensions...   jusqu'ici je modifiais cela à la main...

    for (int i = 0; i < SizeTransfo; i++)
    {
        if (Transfo[i][2] > 10 && Transfo[i][1] < -10)
        {
            Transfo[i][4] = 0; // si on a des valeurs positives et négatives, on met zéro comme init
        }
        else
        {
            Transfo[i][4] = Transfo[i][1]; // sinon on initialise toutes les valeurs enregistrées au minimum
        }
    }
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

    // BLACK06    Serial3.begin(31250);

    // Channel:
    //     0: OMNI mode, the message is sent on all channels.
    //   1 to 16: classic channels. (transposed to 0-15 for binary transmission)
    // 17+: OFF (nothing sent)

    PiccoloVersion = F("Piccolo THOU25"); // petit subterfuge pour ne rien laisser en mémoire vive
    // BLACK08 euh... du coup ça l'y charge, non ?
    PiccoloDate = F("13.1.2025");
    midiname = F("turca.mid");

    ChoixString153 = F("<_abcdefghij<klmnopqrst<uvwxyz0123<456789ABCD<EFGHIJKLMN<OPQRSTUVWXYZ<");

    String159 = F("---");
    ChoixString156 = F("-");
    ChoixString156 = ChoixString156 + flechegauche + flechedroite + barremilieu + barreH + F("x!") + F("<");

    // u8x8.begin();

    defineSlot(1, 0, 0);  // la première zone d'affichage sera 0,0
    defineSlot(2, 5, 0);  // la deuxième correspond à l'affichage des effets
    defineSlot(3, 0, 1);  // la troisième pour les valeurs des transfos
    defineSlot(4, 10, 3); // pour les transformations
    defineSlot(5, 0, 2);  // pour les MUTE et STEPZ
    defineSlot(6, 5, 2);  // pour le déroulement STEPZ
    defineSlot(7, 5, 3);  // pour les STEPZ valeurs

    buffer_and_HARD_setCursor(0, 0);
    buffer_and_HARD_print(PiccoloVersion);

    randomSeed(analogRead(A5));

    for (byte i = 0; i < NbEvenementMax; i++)
    { // layers
        EmptyEvenement(i);
    }

    for (byte i = 0; i < nbLFOMax; i++)
    {
        EmptyLFO(i);
    }

    backToSelfMatrixTotal(midiRedirect);

    deleteAllCCMod(); // 2C01 - remplace une boucle

    for (int i = 0; i < nbLayer; i++)
    {
        pitchableLayer[i] = true;
        nbMaxDeNotesParLayer[i] = 1000;
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

void checkTapTempoButton(void)
{
    if (myButtonTap.risingEdge())
    {
        if (tempoStarted == false)
        {
            tempoStarted = true;
            tapBegin = nowMillis;
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
                                tempNote.endNote = (unsigned long)((float)tempNote.endNote * (float)layerLength[i] / (float)oldlayerLength);
                                // correction int vers long BLACK08
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
                if (shortest == 0)
                {
                    shortest = 1;
                }

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
                    if (oldlayerLength == 0)
                    {
                        oldlayerLength = 1;
                    }

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
                            unsigned int diff = (time2 - time1);
                            if (diff == 0)
                            {
                                diff = 1;
                            }
                            byte j = layerLength[i] / diff; // donne un chiffre indiquant le nb de fois (arrondi à l'unité inférieure) où on retrouve le tap dans la durée initiallayerLength
                            if (j == 0)
                            {
                                j++;
                            }
                            layerLength[i] = j * diff;
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

    if (myButtonJump.risingEdge())
    {
        // si le bouton poussoir CH6 a été poussé, on va faire jump pour sélection des layers

        // le tableau avec les jump qui se trouvait ici jusque BLACK06 est maintenant dans
        // transfoArray

        byte r = 0;
        while ((softwareValueEncoderLeft >= LineCallingTransfo(jump[r])) && (r < tailleJump))
        {
            r++;
        }

        softwareValueEncoderLeft = LineCallingTransfo(jump[r]);

        minimumSoftwareValueEncoderRight = Transfo[softwareValueEncoderLeft][1];
        maximumSoftwareValueEncoderRight = Transfo[softwareValueEncoderLeft][2];
        softwareValueEncoderRight = Transfo[softwareValueEncoderLeft][4];
        buffer_clearScreen_HARD(); // pour nettoyage !
    }
}

void checkModulationButtons(void)
{

    // if (myButtonShift.read() == true ? buttonShiftPressed = true : buttonShiftPressed = false ; )
    buttonShiftPressed = (myButtonShift.read() ? true : false);
    buttonCtrlPressed = (myButtonCtrl.read() ? true : false);
    buttonAltPressed = (myButtonAlt.read() ? true : false);
    buttonMutePressed = (myButtonMute.read() ? true : false);
    buttonSavePressed = (myButtonSave.read() ? true : false);
    buttonLoadPressed = (myButtonLoad.read() ? true : false);
    buttonJSPPressed = (myButtonJSP.read() ? true : false);
    buttonExtraPressed = (myButtonExtra.read() ? true : false);
    synthesePressed = 0;

    synthesePressed = ((byte)buttonShiftPressed << 7) +
                      ((byte)buttonCtrlPressed << 6) +
                      ((byte)buttonAltPressed << 5) +
                      ((byte)buttonMutePressed << 4) +
                      ((byte)buttonSavePressed << 3) +
                      ((byte)buttonLoadPressed << 2) +
                      ((byte)buttonJSPPressed << 1) +
                      ((byte)buttonExtraPressed << 0);
}

void checkDeleteButton(void)
{

    if (myButtonDel.risingEdge())
    { // si le bouton poussoir noir 2 a été poussé, on va faire +1 pour sélection des layers

        // if ( ( millis() - pressedE1time ) > blocktime )
        if (
            Transfo[softwareValueEncoderLeft][0] != 424    // stepz
            && Transfo[softwareValueEncoderLeft][0] != 512 // lapidz
            && Transfo[softwareValueEncoderLeft][0] != 441 // puntz
            && Transfo[softwareValueEncoderLeft][0] != 548 // polyz
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
            // on est dans les séquenceurs style boîte à rythme
            // initalement STEPZ et des variantes
            // PUNTZ n'a plus de sens à partir de Black08 : DISMISS

            // effacer dans les layers actifs toutes les notes stepChannel, stepNote
            // BLACK06 mais aussi les Accents

            if (synthesePressed == 0)
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

                            if (tempNote.note == currentStepzNote && currentStepzNote == 0)
                            // ACCENT, CH indépendant
                            {
                                tempNote.note = 0;
                                tempNote.killnote = true;
                                tempNote.type = 0x80; // BLACK08 pour forcer l'effacement
                                tempNote.velocity = 0;
                                ZEROSetNoteInPile(pInitial[i], j, tempNote);
                            }
                        }
                    }

                    ZEROtreatPileComplete(pInitial[i]);
                    // BLACK 09 c'était l'élément manquant pour que tout soit bien clean !
                }
            }

            else

                if (synthesePressed == binCtrl)
            {
            }
        }
    }
}

void checkLayersButtons(void)
{

    // 1er cas de figure : on appuie sur LAYERS sans aucune modulation

    if (synthesePressed == 0)
    {
        if (myButtonLayer1.risingEdge())
        { // si le bouton poussoir noir bas 1 a été poussé, on va changer valeur de layer 1
            //if ((nowMillis - pressedL1time) > blocktime)
            {

                activeLayer = activeLayer ^ (B0001 << (1 - 1));
                // 1 -> 0    0 -> 1
                activeLayer = (activeLayer & B00001111); // on coupe tout ce qui dépasse au dessus des 4 layers ! attention si on augmente le nb de layers faudra changer cette valeur
                                                         // pressedL1time = nowMillis;
            }

            if (
                Transfo[softwareValueEncoderLeft][0] != 30 && Transfo[softwareValueEncoderLeft][0] != 14 && Transfo[softwareValueEncoderLeft][0] != 246 && Transfo[softwareValueEncoderLeft][0] != 46 && Transfo[softwareValueEncoderLeft][0] != 48 && Transfo[softwareValueEncoderLeft][0] != 14)
            {
                showActiveStatus();
            }
        }

        if (myButtonLayer2.risingEdge())
        { // si le bouton poussoir noir bas 1 a été poussé, on va changer valeur de layer 1
            // if ((nowMillis - pressedL2time) > blocktime)
            {

                activeLayer = activeLayer ^ (B0001 << (2 - 1));
                // 1 -> 0    0 -> 1
                activeLayer = (activeLayer & B00001111); // on coupe tout ce qui dépasse au dessus des 4 layers ! attention si on augmente le nb de layers faudra changer cette valeur
                                                         //   pressedL2time = nowMillis;
            }

            if (
                Transfo[softwareValueEncoderLeft][0] != 30 && Transfo[softwareValueEncoderLeft][0] != 14 && Transfo[softwareValueEncoderLeft][0] != 246 && Transfo[softwareValueEncoderLeft][0] != 46 && Transfo[softwareValueEncoderLeft][0] != 48 && Transfo[softwareValueEncoderLeft][0] != 14)
            {
                showActiveStatus();
            }
        }

        if (myButtonLayer3.risingEdge())
        { // si le bouton poussoir noir bas 1 a été poussé, on va changer valeur de layer 1
            //if ((nowMillis - pressedL3time) > blocktime)
            {

                activeLayer = activeLayer ^ (B0001 << (3 - 1));
                // 1 -> 0    0 -> 1
                activeLayer = (activeLayer & B00001111); // on coupe tout ce qui dépasse au dessus des 4 layers ! attention si on augmente le nb de layers faudra changer cette valeur
                //pressedL3time = nowMillis;
            }

            if (
                Transfo[softwareValueEncoderLeft][0] != 30 && Transfo[softwareValueEncoderLeft][0] != 14 && Transfo[softwareValueEncoderLeft][0] != 246 && Transfo[softwareValueEncoderLeft][0] != 46 && Transfo[softwareValueEncoderLeft][0] != 48 && Transfo[softwareValueEncoderLeft][0] != 14)
            {
                showActiveStatus();
            }
        }

        if (myButtonLayer4.risingEdge())
        { // si le bouton poussoir noir bas 1 a été poussé, on va changer valeur de layer 1
            //if ((nowMillis - pressedL4time) > blocktime)

            activeLayer = activeLayer ^ (B0001 << (4 - 1));
            // 1 -> 0    0 -> 1
            activeLayer = (activeLayer & B00001111); // on coupe tout ce qui dépasse au dessus des 4 layers ! attention si on augmente le nb de layers faudra changer cette valeur
                                                     // pressedL4time = nowMillis;

            if (
                Transfo[softwareValueEncoderLeft][0] != 30 && Transfo[softwareValueEncoderLeft][0] != 14 && Transfo[softwareValueEncoderLeft][0] != 246 && Transfo[softwareValueEncoderLeft][0] != 46 && Transfo[softwareValueEncoderLeft][0] != 48 && Transfo[softwareValueEncoderLeft][0] != 14)
            {
                showActiveStatus();
            }
        }
    }

    if (synthesePressed == binMute) // MUTE seulement
    {

        if (myButtonLayer1.risingEdge())
        { // si le bouton poussoir noir bas 1 a été poussé, on va changer valeur de layer 1
            {
                switches[0] = !switches[0];
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
                Transfo[softwareValueEncoderLeft][0] != 30 && Transfo[softwareValueEncoderLeft][0] != 14 && Transfo[softwareValueEncoderLeft][0] != 246 && Transfo[softwareValueEncoderLeft][0] != 46 && Transfo[softwareValueEncoderLeft][0] != 48 && Transfo[softwareValueEncoderLeft][0] != 14)
            {
                showActiveStatus();
            }

            showMutedStatus();
        }

        if (myButtonLayer2.risingEdge())
        { // si le bouton poussoir noir bas 1 a été poussé, on va changer valeur de layer 1
            {
                switches[1] = !switches[1];
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
                Transfo[softwareValueEncoderLeft][0] != 30 && Transfo[softwareValueEncoderLeft][0] != 14 && Transfo[softwareValueEncoderLeft][0] != 246 && Transfo[softwareValueEncoderLeft][0] != 46 && Transfo[softwareValueEncoderLeft][0] != 48 && Transfo[softwareValueEncoderLeft][0] != 14)
            {
                showActiveStatus();
            }

            showMutedStatus();
        }

        if (myButtonLayer3.risingEdge())
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
                Transfo[softwareValueEncoderLeft][0] != 30 && Transfo[softwareValueEncoderLeft][0] != 14 && Transfo[softwareValueEncoderLeft][0] != 246 && Transfo[softwareValueEncoderLeft][0] != 46 && Transfo[softwareValueEncoderLeft][0] != 48 && Transfo[softwareValueEncoderLeft][0] != 14)
            {
                showActiveStatus();
            }

            showMutedStatus();
        }

        if (myButtonLayer4.risingEdge())
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
                Transfo[softwareValueEncoderLeft][0] != 30 && Transfo[softwareValueEncoderLeft][0] != 14 && Transfo[softwareValueEncoderLeft][0] != 246 && Transfo[softwareValueEncoderLeft][0] != 46 && Transfo[softwareValueEncoderLeft][0] != 48 && Transfo[softwareValueEncoderLeft][0] != 14)
            {
                showActiveStatus();
            }

            showMutedStatus();
        }
    }
}

void checkPadsButtons(void)
{

    // voici une procédure qu'on appelera en dehors du fonctionnement de STEPZ, PUNTZ
    // 1er cas de figure : on appuie sur PADS sans aucune modulation

    if (synthesePressed == 0)
    {
        if (myButtonPad1.risingEdge())
        {
            jumpToTransfo(24); // RECORD ON 
        }

        if (myButtonPad2.risingEdge())
        {
            jumpToTransfo(424); // STEPZ
        }

        if (myButtonPad3.risingEdge())
        {
            jumpToTransfo(466); // DENDE DELAY  
        }

        if (myButtonPad4.risingEdge())
        {
          
            jumpToTransfo(228); // SEQ
        }

        if (myButtonPad5.risingEdge())
        {
              jumpToTransfo(372); // SAVE LOAD
        }

        if (myButtonPad6.risingEdge())
        {
               jumpToTransfo(58); // FUNKY VEL 
         
        }

        if (myButtonPad7.risingEdge())
        {
               jumpToTransfo(127); // EVENTS
           
        }

        if (myButtonPad8.risingEdge())
        {
                  jumpToTransfo(351); // BAROQUE
           
        }
    }

    if (synthesePressed == binCtrl) // CTRL seulement
    {

        if (myButtonPad1.risingEdge())
        {
             jumpToTransfo(472); // LFO 
           
        }

        if (myButtonPad2.risingEdge())
        {
             jumpToTransfo(167); // MIDI 
                                // BLACK08 j'utilise une procédure car il faut changer plusieurs choses qui reviendront de façon répétée
                                // le softwareEncoderLeft, et les valeurs du right
          
        }

        if (myButtonPad3.risingEdge())
        {
              jumpToTransfo(190); // CC MOD 
        }

        if (myButtonPad4.risingEdge())
        {
           Transformation(586, 0); // arrête les Trackers
        }

        if (myButtonPad5.risingEdge())
        {
            jumpToTransfo(340);  // Mémoires 
        }

        if (myButtonPad6.risingEdge())
        {
            jumpToTransfo(111); // Piccolo message
        }

        if (myButtonPad7.risingEdge())
        {
            jumpToTransfo(244); // Hash +1 Editha 
        }

        if (myButtonPad8.risingEdge())
        {
        }
    }
}

void checkMemoryButton(void)
{

    // 1er cas de figure : on appuie sur MEMORY sans aucune modulation

    if (synthesePressed == 0)
    {
        if (myButtonMemory.risingEdge())
        {
        }
    }

    if (synthesePressed == binSave) // Save seulement
    {
        if (myButtonMemory.risingEdge())
        {

            Transformation(538, softwareValueEncoderRight); //
        }
    }

    if (synthesePressed == binLoad) // Load seulement
    {
        if (myButtonMemory.risingEdge())
        {

            Transformation(540, softwareValueEncoderRight); //
        }
    }
}

void checkKaraktButton(void)
{

    // 1er cas de figure : on appuie sur MEMORY sans aucune modulation

    if (synthesePressed == 0)
    {
        if (myButtonKarakt.risingEdge())
        {
        }
    }

    if (synthesePressed == binSave) // Save seulement
    {
        if (myButtonKarakt.risingEdge())
        {

            Transformation(539, softwareValueEncoderRight); //
        }
    }

    if (synthesePressed == binLoad) // Load seulement
    {
        if (myButtonKarakt.risingEdge())
        {
            Transformation(579, softwareValueEncoderRight); //
        }
    }
}

void checkMuteButtons(void) // OBSOLETE BLACK07

{

    // TODO : ici il faut combinaison de Mute et de Layer1

    if (myButtonMute.risingEdge())
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
            Transfo[softwareValueEncoderLeft][0] != 30 && Transfo[softwareValueEncoderLeft][0] != 14 && Transfo[softwareValueEncoderLeft][0] != 246 && Transfo[softwareValueEncoderLeft][0] != 46 && Transfo[softwareValueEncoderLeft][0] != 48 && Transfo[softwareValueEncoderLeft][0] != 14)
        {
            showActiveStatus();
        }

        showMutedStatus();
    }

    if (myButtonMute.risingEdge())
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
            Transfo[softwareValueEncoderLeft][0] != 30 && Transfo[softwareValueEncoderLeft][0] != 14 && Transfo[softwareValueEncoderLeft][0] != 246 && Transfo[softwareValueEncoderLeft][0] != 46 && Transfo[softwareValueEncoderLeft][0] != 48 && Transfo[softwareValueEncoderLeft][0] != 14)
        {
            showActiveStatus();
        }

        showMutedStatus();
    }

    if (myButtonMute.risingEdge())
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
            Transfo[softwareValueEncoderLeft][0] != 30 && Transfo[softwareValueEncoderLeft][0] != 14 && Transfo[softwareValueEncoderLeft][0] != 246 && Transfo[softwareValueEncoderLeft][0] != 46 && Transfo[softwareValueEncoderLeft][0] != 48 && Transfo[softwareValueEncoderLeft][0] != 14)
        {
            showActiveStatus();
        }

        showMutedStatus();
    }

    if (myButtonMute.risingEdge())
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
            Transfo[softwareValueEncoderLeft][0] != 30 && Transfo[softwareValueEncoderLeft][0] != 14 && Transfo[softwareValueEncoderLeft][0] != 246 && Transfo[softwareValueEncoderLeft][0] != 46 && Transfo[softwareValueEncoderLeft][0] != 48 && Transfo[softwareValueEncoderLeft][0] != 14)
        {
            showActiveStatus();
        }

        showMutedStatus();
    }
}

// void checkButton8and9PushButtonsRotaryEncoders ( void ) {

//     if ( digitalRead(62) == HIGH ) {      // digital62 = Analog A8
//         if ( ( millis() - pressedRE8time ) > blocktime ) {

//             counterEncoder = rotKnobLeft.read() ;
//             softwareValueEncoderLeft = counterEncoder / 4 ;

//             u8x8.clear();
//             buffer_and_HARD_setCursor(11, 4);
//             buffer_and_HARD_print("PUSH a8");
//             delay(100);

//             // qd on est au-dessus de la première transfo d'une catégorie, on envoie à la suivante

//             if (  softwareValueEncoderLeft >= LineCallingTransfo(228) ) { softwareValueEncoderLeft = LineCallingTransfo(111); } else
//             if (  softwareValueEncoderLeft >= LineCallingTransfo(218) ) { softwareValueEncoderLeft = LineCallingTransfo(228) ; }else
//             if (  softwareValueEncoderLeft >= LineCallingTransfo(190) ) { softwareValueEncoderLeft = LineCallingTransfo(218) ; }else
//             if (  softwareValueEncoderLeft >= LineCallingTransfo(87) ) { softwareValueEncoderLeft = LineCallingTransfo(190) ; }else
//             if (  softwareValueEncoderLeft >= LineCallingTransfo(81) ) { softwareValueEncoderLeft = LineCallingTransfo(87) ; }else
//             if (  softwareValueEncoderLeft >= LineCallingTransfo(127) ) { softwareValueEncoderLeft = LineCallingTransfo(81) ; }else
//             if (  softwareValueEncoderLeft >= LineCallingTransfo(64) ) { softwareValueEncoderLeft = LineCallingTransfo(127) ; }else
//             if (  softwareValueEncoderLeft >=  LineCallingTransfo(111) ) { softwareValueEncoderLeft = LineCallingTransfo(64) ; }

//             counterEncoder = softwareValueEncoderLeft * 4  ;
//             rotKnobLeft.write(counterEncoder ) ;

//             pressedRE8time = millis();
//         }
//     }

//     if ( digitalRead(63) == HIGH ) {      //  digital63 = A9
//         if ( ( millis() - pressedRE9time ) > blocktime ) {

//             counterEncoder = rotKnobLeft.read() ;
//             softwareValueEncoderLeft = counterEncoder / 4 ;

//             u8x8.clear();
//             buffer_and_HARD_setCursor(11, 4);
//             buffer_and_HARD_print("PUSH a9");
//             delay(100);

//             softwareValueEncoderLeft += 20 ;

//             counterEncoder = softwareValueEncoderLeft * 4  ;
//             rotKnobLeft.write(counterEncoder ) ;

//             pressedRE9time = millis();
//         }
//     }

// }

void checkOK_TransformationButton(void)
{

    if (myButtonOK.risingEdge())
    {

        // if ( ( millis() - pressed3time ) > blocktime )
        {
            buffer_setCursor(0, 6);
            buffer_print("OK");
            // digitalWrite(5, HIGH);

            Transformation(Transfo[softwareValueEncoderLeft][0], softwareValueEncoderRight); //

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

/// @brief 
/// @param phrase info à imprimer
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
                    midiFile_progress++;      // 2C28
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
    sequence[t].repeatRANDOMFRAG = 1;  // BLACK08
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
//            tempNote2.startNote = ((unsigned long)place * duree1subStep + (unsigned long)1) % layerLength[i];
            tempNote2.startNote = ((unsigned long)place * duree1subStep ) % layerLength[i] + 1 ;   // BLACK10
 // TODO DEBUG : cette façon de faire crée des inégalités finale au bout du layer
 // on ne doit jamais utiliser duree1subStep ! 
 
            tempNote2.thereIsAnEnd = true;

            switch (stopModeStepz)
            {
            case 0:
            {
                tempNote2.endNote = (tempNote2.startNote + duree1subStep) % layerLength[i] - (unsigned long)3;
            }
            break;

            case 1:
            {
                tempNote2.endNote = (tempNote2.startNote + ((unsigned long)nbSubStepz * duree1subStep)) % layerLength[i] - 3;
            }
            break;

            case 2:
            {
                tempNote2.endNote = (tempNote2.startNote) % layerLength[i] + (unsigned long) 10;
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
            tempNote2.channelReallyPlayed = stepChannel;
            tempNote2.velocity = stepVelocity;
            tempNote2.killnote = false;
            tempNote2.type = 0x90;

            if (tempNote2.note == 0) // ACCENT
            {
                tempNote2.type = 0x50;
            } // ACCENT  2C24

            if (tempNote2.note == 128) // TOTAL ACCENT 2C24
            {
                tempNote2.type = 0x50;
            } // ACCENT  2C24

            tempNote2.noteReallyPlayed = noteToCreate;
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

    for (byte j = 0; j < 16; j++)
    {
        if (keyboardStepz[j] == 1)
        {
            for (byte i = 0; i < nbLayer; i++)
            {
                if (isLayerActive(i))
                {
                    StepzButtonPushed_LayerA_NoteB_ButtonC(i, currentStepzNote, j);
                }
            }
            keyboardStepz[j] = 0;
        }
    }

    if (myButtonPad1.risingEdge())
    { // si le bouton poussoir noir bas 1 a été poussé, on va changer valeur de layer 1
        for (byte i = 0; i < nbLayer; i++)
        {
            if (isLayerActive(i))
            {
                StepzButtonPushed_LayerA_NoteB_ButtonC(i, currentStepzNote, 0);
            }
        }
    }

    if (myButtonPad2.risingEdge())
    { // si le bouton poussoir noir bas 1 a été poussé, on va changer valeur de layer 1
        for (byte i = 0; i < nbLayer; i++)
        {
            if (isLayerActive(i))
            {
                StepzButtonPushed_LayerA_NoteB_ButtonC(i, currentStepzNote, 1);
            }
        }
    }

    if (myButtonPad3.risingEdge())
    { // si le bouton poussoir noir bas 1 a été poussé, on va changer valeur de layer 1
        for (byte i = 0; i < nbLayer; i++)
        {
            if (isLayerActive(i))
            {
                StepzButtonPushed_LayerA_NoteB_ButtonC(i, currentStepzNote, 2);
            }
        }
    }

    if (myButtonPad4.risingEdge())
    { // si le bouton poussoir noir bas 1 a été poussé, on va changer valeur de layer 1
        for (byte i = 0; i < nbLayer; i++)
        {
            if (isLayerActive(i))
            {
                StepzButtonPushed_LayerA_NoteB_ButtonC(i, currentStepzNote, 3);
            }
        }
    }

    if (myButtonPad5.risingEdge())
    { // si le bouton poussoir noir bas 1 a été poussé, on va changer valeur de layer 1
        for (byte i = 0; i < nbLayer; i++)
        {
            if (isLayerActive(i))
            {
                StepzButtonPushed_LayerA_NoteB_ButtonC(i, currentStepzNote, 4);
            }
        }
    }

    if (myButtonPad6.risingEdge())
    { // si le bouton poussoir noir bas 1 a été poussé, on va changer valeur de layer 1
        for (byte i = 0; i < nbLayer; i++)
        {
            if (isLayerActive(i))
            {
                StepzButtonPushed_LayerA_NoteB_ButtonC(i, currentStepzNote, 5);
            }
        }
    }

    if (myButtonPad7.risingEdge())
    { // si le bouton poussoir noir bas 1 a été poussé, on va changer valeur de layer 1
        for (byte i = 0; i < nbLayer; i++)
        {
            if (isLayerActive(i))
            {
                StepzButtonPushed_LayerA_NoteB_ButtonC(i, currentStepzNote, 6);
            }
        }
    }

    if (myButtonPad8.risingEdge())
    { // si le bouton poussoir noir bas 1 a été poussé, on va changer valeur de layer 1
        for (byte i = 0; i < nbLayer; i++)
        {
            if (isLayerActive(i))
            {
                StepzButtonPushed_LayerA_NoteB_ButtonC(i, currentStepzNote, 7);
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

    if (myButtonPad1.risingEdge())
    {
        velocityPuntz = 10;
        for (byte i = 0; i < nbLayer; i++)
        {
            if (isLayerActive(i))
            {
                PuntzButtonPushed_LayerA_NoteB_ButtonC(i, currentStepzNote, velocityPuntz);
            }
        }
    }

    if (myButtonPad2.risingEdge())
    {
        velocityPuntz = 25;
        for (byte i = 0; i < nbLayer; i++)
        {
            if (isLayerActive(i))
            {
                PuntzButtonPushed_LayerA_NoteB_ButtonC(i, currentStepzNote, velocityPuntz);
            }
        }
    }

    if (myButtonPad3.risingEdge())
    {
        velocityPuntz = 50;
        for (byte i = 0; i < nbLayer; i++)
        {
            if (isLayerActive(i))
            {
                PuntzButtonPushed_LayerA_NoteB_ButtonC(i, currentStepzNote, velocityPuntz);
            }
        }
    }

    if (myButtonPad4.risingEdge())
    {
        velocityPuntz = 65;
        for (byte i = 0; i < nbLayer; i++)
        {
            if (isLayerActive(i))
            {
                PuntzButtonPushed_LayerA_NoteB_ButtonC(i, currentStepzNote, velocityPuntz);
            }
        }
    }

    if (myButtonPad5.risingEdge())
    {
        velocityPuntz = 80;
        for (byte i = 0; i < nbLayer; i++)
        {
            if (isLayerActive(i))
            {
                PuntzButtonPushed_LayerA_NoteB_ButtonC(i, currentStepzNote, velocityPuntz);
            }
        }
    }

    if (myButtonPad6.risingEdge())
    {
        velocityPuntz = 95;
        for (byte i = 0; i < nbLayer; i++)
        {
            if (isLayerActive(i))
            {
                PuntzButtonPushed_LayerA_NoteB_ButtonC(i, currentStepzNote, velocityPuntz);
            }
        }
    }

    if (myButtonPad7.risingEdge())
    {
        velocityPuntz = 110;
        for (byte i = 0; i < nbLayer; i++)
        {
            if (isLayerActive(i))
            {
                PuntzButtonPushed_LayerA_NoteB_ButtonC(i, currentStepzNote, velocityPuntz);
            }
        }
    }

    if (myButtonPad8.risingEdge())
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
            if (tableauSubStepz[j] > 0)
            {
                tableauStepz[CtStepz] = 1;
            };
            CtStepz++;
        }
    }
    numberMaxPageStepz = (CtStepz - 1) / nbStepzAffiches;

    // le tableau Stepz est OK
    // on rajoute 1 et si trop haut on remet à zéro

    tableauStepz[ButtonPushed] = tableauStepz[ButtonPushed] + 1;

    // tableauStepz[ButtonPushed] = 4 ;

    // if (tableauStepz[ButtonPushed] > plusGrandNombreAvecAbits(nbSubStepz))
    if (tableauStepz[ButtonPushed] > 1)
    {
        tableauStepz[ButtonPushed] = 0;
    }

    // on déduit maintenant les subStepz à partir des Stepz ;

    CtStepz = 0;
    bool foundPlace = false;
    //  int place = 0 ;

    for (unsigned int j = 0; j < nbCasesTotal; j++)
    {
        if (CtStepz == ButtonPushed && foundPlace == false)
        {
            //  place = j ;
            foundPlace = true;
        }

        if (tableauCasesTotal[j] == 1)
        {
            tableauSubStepz[j] = tableauStepz[CtStepz];
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
                    if (((unsigned long)tempNote.startNote / (unsigned long)duree1subStep) == (unsigned long)k)
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
                    if (((unsigned long)tempNote.startNote / (unsigned long)duree1subStep) == (unsigned long)k)
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
            tempNote2.startNote = ((unsigned long)k * duree1subStep + (unsigned long)1) % layerLength[i];
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
            tempNote2.channelReallyPlayed = stepChannel;
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

            tempNote2.noteReallyPlayed = noteToCreate;
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

    if (myButtonPad1.risingEdge())
    { // si le bouton poussoir noir bas 1 a été poussé, on va changer valeur de layer 1
        for (byte i = 0; i < nbLayer; i++)
        {
            if (isLayerActive(i))
            {
                PolyZButtonPushed_LayerA_NoteB_ButtonC(i, currentStepzNote, 0);
            }
        }
    }

    if (myButtonPad2.risingEdge())
    { // si le bouton poussoir noir bas 1 a été poussé, on va changer valeur de layer 1
        for (byte i = 0; i < nbLayer; i++)
        {
            if (isLayerActive(i))
            {
                PolyZButtonPushed_LayerA_NoteB_ButtonC(i, currentStepzNote, 1);
            }
        }
    }

    if (myButtonPad3.risingEdge())
    { // si le bouton poussoir noir bas 1 a été poussé, on va changer valeur de layer 1
        for (byte i = 0; i < nbLayer; i++)
        {
            if (isLayerActive(i))
            {
                PolyZButtonPushed_LayerA_NoteB_ButtonC(i, currentStepzNote, 2);
            }
        }
    }

    if (myButtonPad4.risingEdge())
    { // si le bouton poussoir noir bas 1 a été poussé, on va changer valeur de layer 1
        for (byte i = 0; i < nbLayer; i++)
        {
            if (isLayerActive(i))
            {
                PolyZButtonPushed_LayerA_NoteB_ButtonC(i, currentStepzNote, 3);
            }
        }
    }

    if (myButtonPad5.risingEdge())
    { // si le bouton poussoir noir bas 1 a été poussé, on va changer valeur de layer 1
        for (byte i = 0; i < nbLayer; i++)
        {
            if (isLayerActive(i))
            {
                PolyZButtonPushed_LayerA_NoteB_ButtonC(i, currentStepzNote, 4);
            }
        }
    }

    if (myButtonPad6.risingEdge())
    { // si le bouton poussoir noir bas 1 a été poussé, on va changer valeur de layer 1
        for (byte i = 0; i < nbLayer; i++)
        {
            if (isLayerActive(i))
            {
                PolyZButtonPushed_LayerA_NoteB_ButtonC(i, currentStepzNote, 5);
            }
        }
    }

    if (myButtonPad7.risingEdge())
    { // si le bouton poussoir noir bas 1 a été poussé, on va changer valeur de layer 1
        for (byte i = 0; i < nbLayer; i++)
        {
            if (isLayerActive(i))
            {
                PolyZButtonPushed_LayerA_NoteB_ButtonC(i, currentStepzNote, 6);
            }
        }
    }

    if (myButtonPad8.risingEdge())
    { // si le bouton poussoir noir bas 1 a été poussé, on va changer valeur de layer 1
        for (byte i = 0; i < nbLayer; i++)
        {
            if (isLayerActive(i))
            {
                PolyZButtonPushed_LayerA_NoteB_ButtonC(i, currentStepzNote, 7);
            }
        }
    }
}
