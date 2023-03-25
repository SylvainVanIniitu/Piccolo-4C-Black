// void affichage () {}
// sera effacée TODO DEBUG ****

// PROTOTYPE
String GiveNameOfMode(int NNN);
int plusGrandNombreAvecAbits(int a);
String GiveNameOfModeShort(int NNN);

// FC
void affichageLine1()
{

    int potVal2;
    potVal2 = softwareValueEncoderRight;

    switch (Transfo[softwareValueEncoderLeft][0])
    {

        // NOTE**** on peut remplacer quasi tous les printRotFast par un PrintRotFast qui va afficher les noms
        // sauf pour certaines exceptions

    case 14: // CH+Time Info     // ajouter MUTE info
    {

        affiche = String(layerChannel[0] + 1); // attention, le layerChannel n'est pas forcéement le channel de toutes les notes du layer !
        affiche += (F(":"));                   // layerChannel est codé en MIDI hardware : 0 = CHANNEL 1
        affiche += String(layerLength[0]);
        affiche += (F(" "));
        buffer_setCursor(0, 0);
        buffer_print(affiche);

        affiche = String(layerChannel[2] + 1); // attention, le layerChannel n'est pas forcéement le channel de toutes les notes du layer !
        affiche += (F(":"));                   // layerChannel est codé en MIDI hardware : 0 = CHANNEL 1
        affiche += String(layerLength[2]);
        affiche += (F(" "));
        buffer_setCursor(8, 0);
        buffer_print(affiche);

        affiche = String(layerChannel[1] + 1); // attention, le layerChannel n'est pas forcéement le channel de toutes les notes du layer !
        affiche += (F(":"));                   // layerChannel est codé en MIDI hardware : 0 = CHANNEL 1
        affiche += String(layerLength[1]);
        affiche += (F(" "));
        buffer_setCursor(0, 1);
        buffer_print(affiche);

        affiche = String(layerChannel[3] + 1); // attention, le layerChannel n'est pas forcéement le channel de toutes les notes du layer !
        affiche += (F(":"));                   // layerChannel est codé en MIDI hardware : 0 = CHANNEL 1
        affiche += String(layerLength[3]);
        affiche += (F(" "));
        buffer_setCursor(8, 1);
        buffer_print(affiche);
    }
    break;

    case 30:
    { // EYE CANDY
    }
    break;

    case 46: // CH+Ratio Info     // ajouter MUTE info
    {

        affiche = String(layerChannel[0] + 1); // attention, le layerChannel n'est pas forcéement le channel de toutes les notes du layer !
        affiche += (F(":"));                   // layerChannel est codé en MIDI hardware : 0 = CHANNEL 1
        affiche += String(ratioLength[0]);
        affiche += (F(" "));
        buffer_setCursor(0, 0);
        buffer_print(affiche);

        affiche = String(layerChannel[2] + 1); // attention, le layerChannel n'est pas forcéement le channel de toutes les notes du layer !
        affiche += (F(":"));                   // layerChannel est codé en MIDI hardware : 0 = CHANNEL 1
        affiche += String(ratioLength[2]);
        affiche += (F(" "));
        buffer_setCursor(8, 0);
        buffer_print(affiche);

        affiche = String(layerChannel[1] + 1); // attention, le layerChannel n'est pas forcéement le channel de toutes les notes du layer !
        affiche += (F(":"));                   // layerChannel est codé en MIDI hardware : 0 = CHANNEL 1
        affiche += String(ratioLength[1]);
        affiche += (F(" "));
        buffer_setCursor(0, 1);
        buffer_print(affiche);

        affiche = String(layerChannel[3] + 1); // attention, le layerChannel n'est pas forcéement le channel de toutes les notes du layer !
        affiche += (F(":"));                   // layerChannel est codé en MIDI hardware : 0 = CHANNEL 1
        affiche += String(ratioLength[3]);
        affiche += (F(" "));
        buffer_setCursor(8, 1);
        buffer_print(affiche);
    }
    break;



    case 48: // CH+Fidelity Info     //
    {

        affiche = String(layerChannel[0] + 1); // attention, le layerChannel n'est pas forcéement le channel de toutes les notes du layer !
        affiche += (F(":"));                   // layerChannel est codé en MIDI hardware : 0 = CHANNEL 1
        affiche += String(fidelityLayer_[0]);
        affiche += (F(" "));
        buffer_setCursor(0, 0);
        buffer_print(affiche);

        affiche = String(layerChannel[2] + 1); // attention, le layerChannel n'est pas forcéement le channel de toutes les notes du layer !
        affiche += (F(":"));                   // layerChannel est codé en MIDI hardware : 0 = CHANNEL 1
        affiche += String(fidelityLayer_[2]);
        affiche += (F(" "));
        buffer_setCursor(8, 0);
        buffer_print(affiche);

        affiche = String(layerChannel[1] + 1); // attention, le layerChannel n'est pas forcéement le channel de toutes les notes du layer !
        affiche += (F(":"));                   // layerChannel est codé en MIDI hardware : 0 = CHANNEL 1
        affiche += String(fidelityLayer_[1]);
        affiche += (F(" "));
        buffer_setCursor(0, 1);
        buffer_print(affiche);

        affiche = String(layerChannel[3] + 1); // attention, le layerChannel n'est pas forcéement le channel de toutes les notes du layer !
        affiche += (F(":"));                   // layerChannel est codé en MIDI hardware : 0 = CHANNEL 1
        affiche += String(fidelityLayer_[3]);
        affiche += (F(" "));
        buffer_setCursor(8, 1);
        buffer_print(affiche);
    }
    break;

    case 82:
    {
        // TODO DEBUG  à modifier de façon plus jolie !
        currentFileNumber = softwareValueEncoderRight;

        if (SDCardPresente == true)
        {
            NameTransfo = F("Load Nr");
            NameTransfo += String(currentFileNumber);
            printRot2(NameTransfo, 11, 400, 2);
        }
        else
        {
            affiche = "NO SD";
            printRot2(affiche, 11, 400, 2);
        }
    }
    break;

        /* DISMISS 2C27 

    case 84:
    {
        currentFileNumber = softwareValueEncoderRight;

        if (SDCardPresente)
        {
            NameTransfo = F("PLAY MIDI#");
            NameTransfo += String(currentFileNumber);
            printRot2(NameTransfo, 11, 400, 2);
        }
        else
        {
            affiche = "NO SD";
            printRot2(affiche, 11, 400, 2);
        }

        // NameFileNumber(currentFileNumber) ;
        // printRot2(name,10,400,3);
        // midiname = name ;

        // print(F("PLAY MIDI#"));
        // print(currentFileNumber);
        // .print(F("   "));
        // u.setCursor(0, 1);
        // NameFileNumber(currentFileNumber) ;
        // .print(name);
        // midiname = name ;
    }
    break;
*/

    case 93:
    {
        currentFileNumber = softwareValueEncoderRight;

        if (SDCardPresente)
        {
            NameTransfo = F(">LOAD MIDI#");
            NameTransfo += String(currentFileNumber);
            printRot2(NameTransfo, 11, 400, 2);
        }
        else
        {
            printRot2("NO SD", 11, 400, 2);
        }
    }
    break;

    case 111:
    {
        affiche = PiccoloVersion;
        printRot2(affiche, 10, 300, 2);
    }
    break;

    case 126: // Define Which
    {
        switch (Evenement[CurrentEvenement].type)
        {
        case (1):
        {
            printRot2(F("Select CC"), 11, 400, 2);
        }
        break;

        case (2):
        {
            printRot2(F("Select Transfo"), 11, 400, 2);
        }
        break;
        default:
        {
            printRot2(F("Select Type first !"), 11, 400, 2);
        }
        break;
        }
    }
    break;

    case 134:
    {
        // Define Value

        if (Evenement[CurrentEvenement].type == 1)
        {
            printRot2(F(">Select Value to send to CC"), 11, 400, 2);
        }

        if (Evenement[CurrentEvenement].type == 2)
        {
            printRot2(F(">Select Transfo VALUE"), 11, 400, 2);
            // // old potVal2 mapping ;
            // // old potVal2 mapping ;

            // d'abord déterminer quelle est la transfo actuellement envisagée
            // ensuite savoir quelle ligne du tableau Transfo appelle cette Transformation
            // ensuite faire un mapping entre potVal2 et les valeurs données par Transfo !

            // potVal2 = map(potVal2, 0, 1023, Transfo[LineCallingTransfo(Evenement[CurrentEvenement].which)][1], Transfo[LineCallingTransfo(Evenement[CurrentEvenement].which)][2] ) ;

            // dans un premier temps on peut afficher la valeur telle que modifiée par le tableau TRANSFO
            // pour la Transformation[Which]
            // dans second temps il faut viser affichage comme dans le browser habituel

            // String affiche ;
            // affiche = "Send " + String(potVal2) + " to Transfo" + String(Evenement[CurrentEvenement].which) ;
            // printRot2(affiche,10,400,3  );

            // NOTE **** On doit ici compléter par le nom de la transformation
        }
    }
    break;

    case 163:
    { // view Channel direction
        // // old potVal2 mapping ;
        // // old potVal2 mapping ;

        // GetNameTransformation( softwareValueEncoderRight );
        NameTransfo = "CH" + String(potVal2);
        printRot2(NameTransfo, 11, 400, 2);

        //buffer_and_HARD_setCursor(0, 1);
        //printMatrixLine(midiRedirect,potVal2-1);
    }
    break;

    case 246: // layer Counters
    {

        affiche = "1:"; // attention, le layerChannel n'est pas forcéement le channel de toutes les notes du layer !
        affiche += String(layerCounter[0]);
        affiche += (F(" "));
        buffer_setCursor(0, 0);
        buffer_print(affiche);

        affiche = "3:"; // attention, le layerChannel n'est pas forcéement le channel de toutes les notes du layer !
        affiche += String(layerCounter[2]);
        affiche += (F(" "));
        buffer_setCursor(8, 0);
        buffer_print(affiche);

        affiche = "2:"; // attention, le layerChannel n'est pas forcéement le channel de toutes les notes du layer !
        affiche += String(layerCounter[1]);
        affiche += (F(" "));
        buffer_setCursor(0, 1);
        buffer_print(affiche);

        affiche = "4:"; // attention, le layerChannel n'est pas forcéement le channel de toutes les notes du layer !
        affiche += String(layerCounter[3]);
        affiche += (F(" "));
        buffer_setCursor(8, 1);
        buffer_print(affiche);
    }
    break;

    case 373:
    {
        // TODO DEBUG  à modifier de façon plus jolie !
        currentFileNumber = softwareValueEncoderRight;

        if (SDCardPresente == true)
        {
            NameTransfo = F("Load Nr");
            NameTransfo += String(currentFileNumber);
            printRot2(NameTransfo, 11, 400, 2);
        }
        else
        {
            affiche = "NO SD";
            printRot2(affiche, 11, 400, 2);
        }
    }
    break;

    case 508:
    {
        // TODO DEBUG  à modifier de façon plus jolie !
        currentFileNumber = softwareValueEncoderRight;

        if (SDCardPresente == true)
        {
            NameTransfo = F("K_Load Nr");
            NameTransfo += String(currentFileNumber);
            printRot2(NameTransfo, 11, 400, 2);
        }
        else
        {
            affiche = "NO SD";
            printRot2(affiche, 11, 400, 2);
        }
    }
    break;

    default:
    {
        GetNameTransformation(Transfo[softwareValueEncoderLeft][0]);
        printRot2(NameTransfo, 12, 400, 2);
    }
    break;

    } // end of switch avec toutes les valeurs de Transformatiosn

} // fin de la procédure affichage Line 1

void affichageLine2(int thisTransfo)
{
    int potVal2;
    potVal2 = softwareValueEncoderRight;

    busyWithMIDIAffichage = false; // il faut veiller qd on sort de 218 à passer au moins 1x par l'affichage de 217
    // pour toutes les fois où on n'est pas dans 218, fait que cette valeur est fausse
    // NOTE DEBUG je n'ai pas trouvé de moyen plus court pour dire qd on n'est pas dans une fc qui rend cette valeur vraie
    //
    // switch ( Transfo[softwareValueEncoderLeft][0] ) {
    switch (thisTransfo)
    {
        // NOTE**** on peut remplacer quasi tous les printRotFast par un PrintRotFast qui va afficher les noms
        // sauf pour certaines exceptions

    case 6:
    {
        // potVal2 = analogRead(A1);

        switch (potVal2)
        {
        case 1:
            potVal2 = 1;
            break;
        case 2:
            potVal2 = 10;
            break;
        case 3:
            potVal2 = 100;
            break;
        case 4:
            potVal2 = 1000;
            break;
        case 5:
            potVal2 = 10000; // soit 10 secondes            break;
        }

        String affiche;
        affiche = String(potVal2) + F(" msec ");
        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 11:
    { // MULTIPLY

        int potVal3 = (softwareValueEncoderRight / 7) + 1;
        potVal2 = (softwareValueEncoderRight % 7) + 1;

        String affiche;
        affiche = String(potVal2) + F("/") + String(potVal3) + F(" ");
        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 12:
    { // Meltiply

        int potVal3 = (softwareValueEncoderRight / 7) + 1;
        potVal2 = (softwareValueEncoderRight % 7) + 1;

        String affiche;
        affiche = String(potVal2) + F("/") + String(potVal3) + F(" ");
        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 14: // CH+Time Info     // ajouter MUTE info
    {
    }
    break;

    case 20:
    {
        // printRotFast(F("(un)MUTE"));
        //
        // TODO transformer en String et PrintRot2 ****

        affiche = nulString;
        ;
        for (byte i = 0; i < nbLayer; i++)
        {
            // déterminer si la layer i est mute
            // déterminer si la layer i est mute
            byte k = ((muteLayer >> i) & B00000001); // le bit de droite est 1 si layer 0 est MUTE ; le 2ème bit est 1 si layer 1 est MUTE, etc...
            if (k == B00000001)
            {
                affiche += F("M");
            }
            else
            {
                affiche += (F("-"));
            }
        }
        printRot2(affiche, 16, 300, 3);
    };
    break;

    case 23:
    { // AUTO FX

        affiche = nulString;
        if (automodification == true)
        {
            affiche = F(" ON ");
        }
        else
        {
            affiche = F("OFF");
        }

  
        // // old potVal2 mapping ;
        // potVal2 = map(potVal2, 0, 1023, Transfo[softwareValueEncoderLeft][1] , Transfo[softwareValueEncoderLeft][2] + 1); // dépendra de l'effet choisi.

        affiche = affiche + String(potVal2) + F(" sec");
        printRot2(affiche, 16, 400, 3);
    }

    break;

    case 24:
    { // RECORD ON/OFF
        printRot2(record == true ? F("ON ") : F("OFF"), 16, 300, 3);
    }
    break;

    case 25: // MIDIOx MIDI Ox
    {

        // MODIF 2C18 KANNIBAL 2 !!!
        // on enlève le .Read direct, pour se fier à la dernière valeur enrgistrée !
        // 2C19 on enlève l'affichage F8 !

        // byte note(0);
        // byte channel(0);
        // byte velocity(0);
        // String affiche;

        // if (MIDI3.read(0))
        // {
        //     channel = MIDI3.getChannel(); // donne le channel, pas le codage du channel ! CH1 = 0 pour machine
        //     note = MIDI3.getData1();
        //     velocity = MIDI3.getData2();
        //     timenote = nowMillis;
        // }

        // if ( note != 0 ) {
        switch (midiType)
        {
        case midi::NoteOn:
        {
            // digitalWrite(5, HIGH);

            affiche = F("CH");
            affiche += String(channel + 1); //2C28 affichage toujours humain, calculs toujours machine
            affiche = affiche + F("N");
            affiche = affiche + String(note);
            affiche = affiche + ".";
            affiche = affiche + String(velocity);
            printRot2(affiche, 16, 400, 3);

            //  digitalWrite(5, LOW);
        }
        break;

        case midi::NoteOff:
        {
            affiche = F("CH");
            affiche += String(channel + 1); // 2C28 affichage toujours humain, calcul toujours machine
            affiche = affiche + F("N");
            affiche = affiche + String(note);
            affiche = affiche + F("x");
            affiche = affiche + String(velocity);
            printRot2(affiche, 16, 400, 3);
        }
        break;

            // polyPressure            = 0x0A,
            //   56         controlChange           = 0x0B,
            //   57         programChange           = 0x0C,
            //   58         channelPressure         = 0x0D,
            //   59         pitchBend               = 0x0E,
            //
            //

        case midi::AfterTouchPoly:
        {
            affiche = F("PolyPressureCH");
            affiche += String(channel + 1);
            affiche = affiche + F("N");
            affiche = affiche + String(note);
            affiche = affiche + F(":");
            affiche = affiche + String(velocity);
            printRot2(affiche, 16, 400, 3);
        }
        break;

        case midi::ControlChange:
        {
            // if (channel != 0 && note != 0)
            if (note != 0)
            {
                affiche = F("/CC/ CH");
                affiche += String(channel + 1);
                affiche = affiche + F("V");
                affiche = affiche + String(note);
                affiche = affiche + F(":");
                affiche = affiche + String(velocity);
                affiche += F(" ");
                printRot2(affiche, 16, 400, 3);
            }
        }
        break;

        case midi::ProgramChange:
        {
            affiche = F("*PC* CH");
            affiche += String(channel + 1);
            affiche = affiche + F("V");
            affiche = affiche + String(note);
            affiche = affiche + F(":");
            affiche = affiche + String(velocity);
            printRot2(affiche, 16, 400, 3);
        }
        break;

        case midi::PitchBend:
        {
            affiche = F("Bend CH");
            affiche += String(channel + 1);
            affiche = affiche + F("V");
            affiche = affiche + String(note);
            affiche = affiche + F(":");
            affiche = affiche + String(velocity);
            printRot2(affiche, 16, 400, 3);
        }
        break;

        default:
            if (midiType != 248)
            {
                affiche = String(midiType);
                affiche += "/";
                affiche += String(channel + 1);
                affiche = affiche + F("?");
                affiche = affiche + String(note);
                affiche = affiche + F("?");
                affiche = affiche + String(velocity);
                printRot2(affiche, 16, 400, 3);

                break;
            }
        }
        // }
    }
    break;

    case 26:
    {
        // premier de la deuxième ligne à bénéficier de printRot2

        // String affiche ;
        affiche = "1 iter = ";
        affiche += String((int)currentCounterSpeed);
        // affiche += "->";
        // affiche += String((int)meancounterspeed);
        // affiche = affiche + F(" microsec  ");
        //  Rfirsttime[3] = true ; // va forcer l'affichage mis à jour
        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 30:
    { // EYE CANDY
        // buffer_and_HARD_setCursor(random(0, 16), random(0, 2));
        // buffer_and_HARD_print((char)random(33, 255));
        // montrer particulièrement la lenteur d'un écran !!!
    }
    break;

    case 31:
    { // ACTIVE LAYER CHOOSER

        affiche = nulString;

        for (byte i = 0; i < nbLayer; i++)
        {
            // déterminer si la layer i est active
            byte k = ((((byte)potVal2) >> i) & B00000001); // le bit de droite est 1 si layer 0 est ON ; le 2ème bit est 1 si layer 1 est ON, etc...
            if (k == B00000001)
            {
                affiche += String(i + 1);
            }
            else
            {
                affiche += "-";
            }
        }

        printRot2(affiche, 16, 400, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // PILE VERSION : faite et vérifiée
        // 1C94
        /////////////////////////////////////////////////////////////////////////////////////

    case 32:
    {
        // FIRST FREEnote

        affiche = nulString;
        ;

        for (byte i = 0; i < nbLayer; i++)
        {
            affiche += String(mesureTaillePileComplete(pInitial[i]));
            // if (anyFreeNote(i))
            // {
            //     affiche += String(firstfreenote(i));
            // }
            // else
            // {
            //     affiche += F("R");
            // }
            affiche += F(".");
        }

        printRot2(affiche, 16, 400, 3);
    };
    break;

    case 35:
    {
        // MIRROR
        // va devoir indiquer le nb de notes présentes avant ce % là des layers

        affiche = nulString;
        affiche += String(potVal2);
        affiche += F("% = ");

        unsigned long foldtime;
        long calcultime; // doit parfois être négatif !!!!
        int total = 0;

        for (byte i = 0; i < nbLayer; i++)
        {

            if (isLayerActive(i))
            {

                calcultime = (((long)layerLength[i] * (long)potVal2) / (long)100);
                foldtime = (signed int)(calcultime);
                // on doit pouvoir se passer de la variable foldtime en fait !
                // affiche+=String(foldtime);

                //1C97
                unsigned int taille = mesureTaillePileComplete(pInitial[i]);
                pileNote tempNote;

                for (byte j = 0; j < taille; j++)
                { //
                    tempNote = ZEROgetNoteFromPile(pInitial[i], j);

                    if ((tempNote.note != 0) && (tempNote.startNote < foldtime))
                    {
                        total++;
                    }
                }
            }
        }

        affiche += String(total);
        affiche += F(" notes");

        printRot2(affiche, 16, 400, 3);
    };
    break;

    case 39:
    { // Velocity

        if (potVal2 < 127)
        {
            affiche = String(potVal2) + " ";
        }
        else
        {
            affiche = F("RND");
        }
        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 40: // HARMONIZE
    {
        //2C19 modification radicale : gamme pour chaque layer

        affiche = nulString;
        // 2C12 va afficher le gamme du layer 1 et transforme tout d'un coup

        // 2C28 : va de C à C et plus de C à F comme avant !

        for (byte i = 0; i < nbLayer; i++)
        {
            if (isLayerActive(i))
            {
                affiche += GiveNameOfNote(potVal2);
                affiche += " ";
            }
            else
            {
                affiche += (GiveNameOfNote(gammeActuelle[i]));
                affiche += " ";
            }
        }

        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 41: // ROOT NOTE
    {
        // 2C12 va afficher le gamme du layer 1 et transforme tout d'un coup

        // 2C27
        affiche = GiveNameOfNote(potVal2);
        affiche += flechedroite;

        for (byte i = 0; i < nbLayer; i++)
        {
            // if (isLayerActive(i))
            //  {
            //     affiche += GiveNameOfNote(potVal2);
            //     affiche += " ";
            // }
            // else
            {
                affiche += (GiveNameOfNote(gammeActuelle[i]));
                affiche += " ";
            }
        }

        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 42:
    { // affichage du Record MODE
        affiche = nulString;
        ;

        // potVal2 = analogRead(A1);
        // old potVal2 mapping ;
        // old potVal2 mapping ;

        switch (potVal2)
        {
        case 0:
            affiche += (F("1.REC 2 LAYER CH"));
            break;
        case 1:
            affiche += (F("2.REC + SOURCE CH"));
            break; // par défaut
        case 2:
            affiche += (F("3.REC - CH        "));
            break;
        default:
            affiche += (F("oink"));
            break;
        }
        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 45:
    {
        // old potVal2 mapping ;
        // old potVal2 mapping ;

        affiche = nulString;

        affiche = String(potVal2);
        affiche += F("%  ");

        // TODO fonction qui remplace ceci : va combler String jusque 8

        switch (varispeedState)
        {
        case 1:
            affiche += (F("  OFF"));
            break;
        case 2:
        {
            affiche += (F("  ON!"));

            for (byte i = 0; i < nbLayer; i++)
            {
                if (isLayerActive(i))
                {
                    ratioCurrent[i] = potVal2; //
                }
            }
        }
        break;
        case 3:
            affiche += (F("  FIX"));
            break;
        case 4:
            affiche += (F("  MOD"));
            break;
        default:
            affiche += (F("  ???"));
            break;
        }
        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 46:
    { // CH+Ratio Info     // ajouter MUTE info
    }
    break;

    case 47: // Fidelity Black10
    {
affiche = nulString ; 
affiche += potVal2 ;
affiche += ">"; 

        for (byte i = 0; i < nbLayer; i++)
        {
            affiche += afficheValueStraightOrFromSequence(fidelityLayer_[i], "%", 0);
            affiche += " "; 
        }
        printRot2(affiche, 16, 400, 3);
      }
    break;

    case 48:
    { // CH+Fidelity Info     //
    }
    break;
    case 50: // Tabula Rasa
    {

        affiche = nulString;
        ;

        // potVal2 = analogRead(A1);
        // old potVal2 mapping ;
        // old potVal2 mapping ;

        switch (potVal2)
        {
        case 0:
            affiche += (F("0??      "));
            break;
        case 1:
            affiche += (F("Pyramid  "));
            break;
        case 2:
            affiche += (F("Square   "));
            break;
        case 3:
            affiche += (F("Random   "));
            break;
        case 4:
            affiche += (F("Complex  "));
            break;
        case 5:
            affiche += (F("Long 32  "));
            break;
        case 6:
            affiche += (F("SlaveRave"));
            break;
        case 7:
            affiche += (F("Technoid4"));
            break;
        case 8:
            affiche += (F("5/5/3   "));
            break;
        case 9:
            affiche += (F("3/5/3    "));
            break;
        default:
            affiche += (F("--------- "));
            break;
        }
        printRot2(affiche, 16, 400, 3);
    };
    break;

    case 51:
    {

        affiche = nulString;
        ;

        //        potVal2 = analogRead(A1);
        // old potVal2 mapping ;
        // old potVal2 mapping ;
        if (potVal2 < 17)
        {
            affiche = String(potVal2); // le CH MIDI est la valeur +1
        }
        else
        {
            switch (potVal2)
            {
            case 17:
                affiche = F("idem");
            case 18:
                affiche = F("OFF ");
            default:
                break;
            }
        }
        printRot2(affiche, 16, 400, 3);
    };
    break;

    case 58:
    { // FUNKY VEL   FunkyVel FunkyVelValue
        affiche = nulString;

        affiche = String(potVal2);
        affiche += F(" ");

        if (FunkyVel == true)
        {
            affiche += (F("ON  "));
        }
        else
        {
            affiche += (F("OFF "));
        }

        for (byte i = 0; i < nbLayer; i++)
        {
            affiche += String(FunkyVelValue[i]);
            affiche += " ";
        }

        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 63:
    { // REP FRACT TODO DEBUG ne marche pas je crois

        affiche = nulString;

        int potVal3 = (softwareValueEncoderRight / 16) + 1;
        potVal2 = (softwareValueEncoderRight % 16) + 1;

        affiche = F("1+");
        affiche += String(potVal2);
        affiche += (F("/"));
        affiche += String(potVal3);
        affiche += (F("    "));

        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 82:
    {
        // DEBUG TODO **** pas clair ce qui doit figurer en ligne 1 et ligne 2
        // surtout pour l'event Transfo

        currentFileNumber = softwareValueEncoderRight;

        affiche = nulString;

        affiche = String(currentFileNumber);
        affiche += F(".");
        NameFileNumber(currentFileNumber);
        affiche += name;

        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 83:
    {
        printRot2(Daubymodification == true ? F("ON ") : F("OFF"), 16, 300, 3);
    }

    break;

    case 84:
    {
        currentFileNumber = softwareValueEncoderRight;

        affiche = nulString;
        ;

        affiche = String(currentFileNumber);
        affiche += F(".");
        NameFileNumber(currentFileNumber);
        affiche += name;

        printRot2(affiche, 16, 400, 3);
        midiname = name;

        // NameFileNumber(currentFileNumber) ;
        // midiname = name ;
    }
    break;

    case 87:
    {
        // Transfo[LineCallingTransfo(87)][2] = (unsigned int) tailleTotaleMIDI;    // première fois qu'une valeur du tableau est modifié à posteriori, en fc de taille du fichier.
        // DEBUG NOTE pas beau, à modifier !
    }
    break;

    case 93:
        // s'inspirer de 373
        {

            currentFileNumber = softwareValueEncoderRight;
            maximumSoftwareValueEncoderRight = rootFileCount;

            affiche = nulString;

            affiche = String(currentFileNumber);
            affiche += F(".");

            if (oldNumber != currentFileNumber)
            {
                NameFileNumber(currentFileNumber);
            }

            oldNumber = currentFileNumber;
            affiche += name;

            printRot2(affiche, 16, 400, 3);
            fileName = name;

            // midiname = name ;
            // TODO : restreindre la lecture au fichier .mid ?
        }
        break;

    case 94:
    {
        printRot2(SqueezeOn == true ? F("ON ") : F("OFF"), 16, 300, 3);
    }
    break;

    case 95:
    {
        printRot2(KeepLength == true ? F("ON ") : F("OFF"), 16, 300, 3);
    }
    break;

    case 97:
    {
        affiche = nulString;
        ;

        affiche = ("(");
        affiche += String(kNecklace);
        affiche += (F(","));
        affiche += String(mNecklace);
        affiche += (F(") - # "));
        affiche += String(NoteNecklace);
        affiche += (" Sh");
        affiche += String(ShiftNecklace);

        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 106:
    {
        // old potVal2 mapping ;
        // old potVal2 mapping ;
        affiche = nulString;

        MultiplicationsString = GatedString(potVal2, NbMultiplications);
        affiche = MultiplicationsString;
        printRot2(affiche, 16, 400, 3);
        // attention dans ce cas-ci c'est MultiplicationsString qui passe l'info au Transfo, pas PotVal3
        // DEBUG pourquoi ceci ? parce que MultiplicationString est constitué à partir de 2 infos : du 106 et 107
        // on pourrait néanmoins passer les 2 infos.
        // DEBUG Gated Mutliple est typiquement une transfo qui gagnerait à faire l'objet d'un multiple OK
    }
    break;

    case 107:
    {

        // old potVal2 mapping ;
        // old potVal2 mapping ;

        affiche = nulString;
        ;

        affiche = String(potVal2);
        affiche += F(" fois");
        printRot2(affiche, 16, 400, 3);
        // potVal2 = analogRead(A1);

        // Si on fait clic, la transfo 107 va faire  :
        // NbMultiplications = potVal3 ;
        // Transfo[LineCallingTransfo(106)][2] = MaximumBinForSteps(NbMultiplications);
        // et donc la fonction 106 est armée pour créer le GatedString
    }
    break;

    // Transfo[SizeTransfo][4]
    case 108:
    {
        // Vertical Mapping
        affiche = nulString;
        ;

        affiche = (F("("));

        affiche += String(FindLowestNoteOfActive());
        affiche += (F(","));

        affiche += String(FindHighestNoteOfActive());
        affiche += (F(")"));
        affiche += flechedroite;
        affiche += F("(");
        // TODO flèche : (String) ((char)(B01111110) );

        // TODO DEBUG problème qd ceci est affiché appelé par Event : ne sera pas juste !
        //
        affiche += String(Transfo[LineCallingTransfo(108)][1]);
        affiche += (F(","));
        affiche += String(Transfo[LineCallingTransfo(108)][2]);
        affiche += (F(")"));
        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 111:
    {

        affiche = PiccoloDate;
        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 112:
    {
        printRot2(autoHarmonize == true ? F("ON ") : F("OFF"), 16, 300, 3);
        // modèle à appliquer à d'autres exemples plus haut !
    }
    break;

    case 113:
    {
        printRot2(complexSqueeze == true ? F("ON ") : F("OFF"), 16, 300, 3);
    }
    break;

    case 114:
    {
        affiche = nulString;
        ;

        // potVal2 = analogRead(A1);
        // old potVal2 mapping ;
        // old potVal2 mapping ;

        switch (potVal2)
        {
        case 0:
            affiche = (F("1.0??      "));
            break;
        case 1:
            affiche = (F("1.Delete "));
            break;
        case 2:
            affiche = (F("2.Wrap  "));
            break;
        case 3:
            affiche = (F("3.Limit "));
            break;
        case 4:
            affiche = (F("4.Bounce "));
            break;
        default:
            affiche = (F("6.--------- "));
            break;
        }
        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 115:
    {
        affiche = nulString;

        affiche = (playParallelMidiON == true ? F("ON ") : F("OFF"));
        affiche += F("  f:"); // pour file
        affiche += (midiname);

        printRot2(affiche, 16, 400, 3);

        // 2C28 progress
        affiche = "[";
        affiche += String(midiFile_progress);
        affiche += "/";
        affiche += String(midiFile_totalNotes);
        affiche += "]";

        // buffer_and_HARD_setCursor() : le modifie aussi en hardware pour l'écran
        buffer_setCursor(0, 6);
        buffer_print(affiche);
    }
    break;

    case 116:
    {
        affiche = nulString;

        // old potVal2 mapping ;
        // old potVal2 mapping ;

        switch (potVal2)
        {
        case 1:
            affiche = (F("1.Bounce Even"));
            break;
        case 2:
            affiche = (F("2.Bounce ALL"));
            break;
        case 3:
            affiche = (F("3.UK"));
            break;
        case 5:
            affiche = (F("4.UK"));
            break;
        case 6:
            affiche = (F("5.UK"));
            break;
        default:
            affiche = (F("6.UK "));
            break;
        }
        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 119:
    { // Phasage

        affiche = nulString;
        ;
        // affiche = afficheRatio(potVal2, 32, 32, true); // le true indique ici que les valeurs peuvent être négatives
        affiche = String(potVal2);
        affiche += "/";
        affiche += String(nPhasage);
        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 120:
    { // Phasage

        affiche = nulString;
        ;
        // affiche = afficheRatio(potVal2, 32, 32, true); // le true indique ici que les valeurs peuvent être négatives
        affiche = String(kPhasage);
        affiche += "/";
        affiche += String(potVal2);
        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 121:
        // ROTATE
        {
            affiche = nulString;
            ;

            affiche = String(potVal2);
            affiche += F(" [x:");
            affiche += String(CentreX);
            affiche += F(",y:");
            affiche += String(CentreY);
            affiche += F("]");

            printRot2(affiche, 16, 400, 3);
        }
        break;

    // case 122: break ; // printRot2 (F("> Set x (longueur layer)") ); break; // TODO
    // case 123: break ; // printRot2 (F("> Set y (pitch)") ); break;
    case 124:
        break; // printRot2(F("Affichage (x,y)") ); break;  // NOTE**** à combiner avec 121

    case 125:
    { // Activate Event

        affiche = nulString;

        affiche = (Evenement[potVal2].actif == true ? F("ON ") : F("OFF"));
        affiche += " ";
        affiche += giveStringOfEvent(potVal2);
        printRot2(affiche, 16, 300, 3);

        // affiche =  nulString ; ;
        //
        // affiche = String (CurrentEvenement);
        // printRot2(affiche  ,16,400,3);

        // NOTE **** faire un String avec tout ceci pour récapituler
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
        //
    }
    break;

    case 126:
    { // Define Which
        affiche = nulString;
        ;
        // affiche = F( "Evt") ;
        // affiche += String(CurrentEvenement);
        affiche = giveStringOfEvent(CurrentEvenement);

        switch (Evenement[CurrentEvenement].type)
        {
        case (1):
        {
            // old potVal2 mapping ;
            // old potVal2 mapping ;

            affiche += (F(" CC"));
            affiche += String(potVal2);
        }
        break;
        case (2):
        {
            // old potVal2 mapping ;
            // potVal2 = map(potVal2, 0, 1023, 0, SizeTransfo ) ;

            // GetNameTransformation(potVal2,1); // 1 c'est le mode où on a un chiffre devant !
            GetNameTransformation(Transfo[potVal2][0], 1); // 1 c'est le mode où on a un chiffre devant !
            // TODO il faudra changer le chiffre attribué par GetNameTransformation
            //
            affiche += " ";
            affiche += NameTransfo;
            // NOTE **** on doit adapter le printRotFast pour plusieurs lieux
        }
        break;
        default:
        {
            affiche += F(" ouin?");
        }
        break;
        }
        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 127:
    {
        // old potVal2 mapping ;
        // old potVal2 mapping ;

        affiche = nulString;
        ;
        affiche = giveStringOfEvent(CurrentEvenement);
        affiche += (F(" <- "));
        affiche += String(potVal2 + 1);
        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 128:
    { // Delete Event
        affiche = nulString;
        affiche = giveStringOfEvent(CurrentEvenement);

        printRot2(affiche, 16, 400, 3);

        // NOTE **** on pourra adapter plus tard si on veut choisir quel Event Effacer sans d'abord devoir le sélectionner
    }
    break;

    case 129:
    { // Define Type
        // old potVal2 mapping ;
        // old potVal2 mapping ;
        affiche = nulString;
        ;
        affiche = giveStringOfEvent(CurrentEvenement);
        switch (potVal2)
        {
        case 0:
            affiche += (F(" 0.--"));
            break;
        case 1:
            affiche += (F(" 1.CC"));
            break;
        case 2:
            affiche += (F(" 2.Tf/Value"));
            break;
        default:
            affiche += (F(" Ugh?"));
            break;
        }
        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 130:
    {
        // Define Intervalle
        // old potVal2 mapping ;
        affiche = nulString;

        if (Evenement[CurrentEvenement].typeInterval == 4)
        {
            affiche = "NOT FOR MODULO";
        }
        else
        {

            affiche = giveStringOfEvent(CurrentEvenement);

            // old potVal2 mapping ;

            affiche += String((float)potVal2 / (float)10); // on divise par 10 car on exprimera en secondes
            affiche += (" sec");
        }
        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 131:
    { // Define Moment
        affiche = nulString;

        if (Evenement[CurrentEvenement].typeInterval == 4)
        {
            affiche = "NOT FOR MODULO";
        }
        else
        {

            affiche = giveStringOfEvent(CurrentEvenement);
            // Transfo[131][2]=(int) Evenement[CurrentEvenement].intervalle ;
            // 3 semaines pour trouver ce BUG !!!
            // c'était bien une mauvaise idée de faire transformer des valeurs du tableau !
            Transfo[LineCallingTransfo(131)][2] = (int)Evenement[CurrentEvenement].intervalle;
            // DEBUG NOTE à modifier !!!!

            // old potVal2 mapping ;
            // old potVal2 mapping ;

            affiche += String((float)potVal2 / (float)10); // on divise par 10 car on exprimera en secondes
            affiche += (" sec");
        }
        printRot2(affiche, 16, 400, 3);
    }
    break;

        // TODO DEBUG il y a une confusion dans rangeLO et rangeUP pour 'lintervalle '
        // et pour les valeurs de la transformation ou du CC
        // il faut éclaircir tout cela

    case 132:
    { // Define Fidelité Event
        affiche = nulString;
        ;
        affiche = giveStringOfEvent(CurrentEvenement);

        affiche += String(potVal2); // on divise par 10 car on exprimera en secondes
        affiche += (" %");
        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 133:
    { // Define Type Intevalle
        // old potVal2 mapping ;
        // old potVal2 mapping ;

        // byte typeInterval ; // 1 = constant 2 = random(a,b)  3=gaussian   4 = bi intervalle
        // NOTE **** à implémenter
        // Si 1 : OK pour .intervalle et .moment
        // Si 2 : valeur Min, valeur Max : Valeur Max = Intervalle , Valeur Min = Moment
        // Si 3 : Valeur Min = distrib 0 ; Valeur max = distrib totale
        // Si 4 : bof
        //
        //
        affiche = nulString;
        affiche = giveStringOfEvent(CurrentEvenement);

        switch (potVal2)
        {
        case 1:
        {
            affiche += (F(" 1.Const spc"));
        }
        break;
        case 2:
        {
            affiche += (F(" 2.Rnd spc("));
            affiche += String(Evenement[CurrentEvenement].rangeLO_);
            affiche += (F(","));
            affiche += String(Evenement[CurrentEvenement].rangeUP_);
            affiche += (F(")"));
        }
        break;
        case 3:
        {
            affiche += (F(" 3.Gauss spc("));
            affiche += String(Evenement[CurrentEvenement].rangeLO_);
            affiche += (F(","));
            affiche += String(Evenement[CurrentEvenement].rangeUP_);
            affiche += (F(")"));
        }
        break;
        case 4:
        {
            affiche += (F(" 4.MODULO"));
        }
        break;

        default:
            affiche += (F(" Ugh?"));
            break;
        }
        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 134:
    {
        // Define Value
        affiche = nulString;
        affiche = giveStringOfEvent(CurrentEvenement);

        if (Evenement[CurrentEvenement].type == 1)
        {
            // old potVal2 mapping ;
            // old potVal2 mapping ;

            affiche += F(" Send");
            affiche += String(potVal2);
            affiche += F(" to CC");
            affiche += String(Evenement[CurrentEvenement].which);
            printRot2(affiche, 16, 400, 3);
        }
        else if (Evenement[CurrentEvenement].type == 2)
        {
            // // old potVal2 mapping ;
            // // old potVal2 mapping ;

            // d'abord déterminer quelle est la transfo actuellement envisagée
            // ensuite savoir quelle ligne du tableau Transfo appelle cette Transformation
            // ensuite faire un mapping entre potVal2 et les valeurs données par Transfo !

            // potVal2 = map(potVal2, 0, 1023, Transfo[LineCallingTransfo(Evenement[CurrentEvenement].which)][1], Transfo[LineCallingTransfo(Evenement[CurrentEvenement].which)][2] ) ;

            //affichageLine2(softwareValueEncoderRight) ;
            // DEBUG faux on doit indiquer la transformation, pas la valeur !

            affichageLine2(Evenement[CurrentEvenement].which);

            // TODO je crois qu'on doit faire de même pour LO et HI !!! on doit afficher ce que cela vaut pour la transformation donnée

            // dans un premier temps on peut afficher la valeur telle que modifiée par le tableau TRANSFO
            // pour la Transformation[Which]
            // dans second temps il faut viser affichage comme dans le browser habituel

            // affiche =  nulString ;
            // affiche += F("Send ") ;
            // affiche += String(potVal2) ;
            // affiche += F( " to Transfo") ;
            // affiche += String(Evenement[CurrentEvenement].which) ;
            // printRot2(affiche,16,400,3  );

            // NOTE **** On doit ici compléter par le nom de la transformation
        }
        else
        {
            String affiche;
            affiche += F(" Define type first");

            printRot2(affiche, 16, 400, 3);
        }
    }
    break;

    case 135:
    {
        // Define Type
        // old potVal2 mapping ;
        // old potVal2 mapping ;

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
        affiche = nulString;
        ;
        affiche = giveStringOfEvent(CurrentEvenement);

        switch (potVal2)
        {
        case 1:
            affiche += (F(" 1.ct val"));
            break;
        case 2:
            affiche += (F(" 2.inc val ("));
            affiche += (Evenement[CurrentEvenement].incSize);
            affiche += (F(")"));
            break;
        case 3:
            affiche += (F(" 3.dec val ("));
            affiche += (Evenement[CurrentEvenement].incSize);
            affiche += (F(")"));
            break;
        case 4:
            affiche += (F(" 4.rnd val ("));
            affiche += (Evenement[CurrentEvenement].rangeLO_);
            affiche += (F(","));
            affiche += (Evenement[CurrentEvenement].rangeUP_);
            affiche += (F(")"));
            break;
        case 5:
            affiche += (F(" 5.seq val"));
            break;
        default:
            affiche += (F(" Ugh?"));
            break;
        }
        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 136:
    { // SET RANGE UP

        if (Evenement[CurrentEvenement].type == 1)
        {
            String affiche;
            affiche = giveStringOfEvent(CurrentEvenement);

            affiche += F("Max ");
            affiche += String(potVal2);
            affiche += F(" to CC");
            affiche += String(Evenement[CurrentEvenement].which);
            printRot2(affiche, 16, 400, 3);
        }

        if (Evenement[CurrentEvenement].type == 2)
        {
            affichageLine2(Evenement[CurrentEvenement].which);
        }
    }
    break;

    case 137:
    { // SET RANGE LO

        if (Evenement[CurrentEvenement].type == 1)
        {
            String affiche;
            affiche = giveStringOfEvent(CurrentEvenement);

            affiche += F("Min ");
            affiche += String(potVal2);
            affiche += F(" to CC");
            affiche += String(Evenement[CurrentEvenement].which);
            printRot2(affiche, 16, 400, 3);
        }

        if (Evenement[CurrentEvenement].type == 2)
        {
            affichageLine2(Evenement[CurrentEvenement].which);
        }
    }
    break;

    case 139:
    { // Define behavior at the Limit
        // old potVal2 mapping ;
        // old potVal2 mapping ;
        // 139 DEFINE behaviorLimit
        // byte behaviorLimit ; // pour options shape 2,3
        // 1 = Change direction
        // 2 = qd dépasse vers haut ou bas, réapparait de l'autre côté
        // 3 = reste à la limite
        affiche = nulString;
        affiche = giveStringOfEvent(CurrentEvenement);

        switch (potVal2)
        {
        case 1:
            affiche += (F("1.Inversion"));
            break;
        case 2:
            affiche += (F("2.Worm"));
            break;
        case 3:
            affiche += (F("3.Limit"));
            break;
        default:
            affiche += (F("Ugh?"));
            break;
        }
        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 140:
    {

        affiche = nulString;
        affiche = giveStringOfEvent(CurrentEvenement);

        affiche += " CH";

        affiche += String(potVal2);

        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 141:
    { // Define Layers

        // old potVal2 mapping ;
        // old potVal2 mapping ;

        // NOTE **** problème pour sélectionner ACTIVE LAYERS

        affiche = nulString;
        affiche = giveStringOfEvent(CurrentEvenement);

        affiche += " ";

        if (potVal2 >= 16)
        {
            affiche += F("ACTIfS L.");
        }
        else

            for (byte i = 0; i < nbLayer; i++)
            {
                // déterminer si la layer i est mute
                byte k = ((potVal2 >> i) & B00000001); // le bit de droite est 1 si layer 0 est MUTE ; le 2ème bit est 1 si layer 1 est MUTE, etc...
                if (k == B00000001)
                {
                    affiche += String(i + 1);
                }
                else
                {
                    affiche += (F("-"));
                }
            }

        printRot2(affiche, 16, 300, 3);

        // TODO DEBUG : on doit aussi avoir une valeur qui est ACTIVE LAYERS
    }
    break;

        /////////////////////////////
        // BLACK 08
        /////////////////////////////

    case 142 :
    { // iVlvDelta 
        affiche = afficheValueStraightOrFromSequence(potVal2, "", 0);
        printRot2(affiche, 16, 300, 3);
    }
    break;

        /////////////////////////////
        // BLACK 08
        /////////////////////////////

    case 143:
    { // iVlvDelta
        affiche = afficheValueStraightOrFromSequence(potVal2, "", 0);
        printRot2(affiche, 16, 300, 3);
    }
    break;

        /////////////////////////////
        // BLACK 08
        /////////////////////////////

    case 144:
    { // imidiDouble 
        affiche = afficheValueStraightOrFromSequence(potVal2, "", 0);
        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 145:
    { // iShiftDelta
        affiche = afficheValueStraightOrFromSequence(potVal2, "", 0);
        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 146:
    { //  ishiftDuree
        affiche = afficheValueStraightOrFromSequence(potVal2, "", 0);
        printRot2(affiche, 16, 300, 3);
    }
    break;
    case 147:
    { // iDureeMask 
        affiche = afficheValueStraightOrFromSequence(potVal2, "", 0);
        printRot2(affiche, 16, 300, 3);
    }
    break;
    case 148:
    { // iMask2 
        affiche = afficheValueStraightOrFromSequence(potVal2, "", 0);
        printRot2(affiche, 16, 300, 3);
    }
    break;
    case 149:
    { // iCutoffDureeDelta 
        affiche = afficheValueStraightOrFromSequence(potVal2, "", 0);
        printRot2(affiche, 16, 300, 3);
    }
    break;
    case 150 :
    { // iBringToDureeDelta 
        affiche = afficheValueStraightOrFromSequence(potVal2, "", 0);
        printRot2(affiche, 16, 300, 3);
    }
    break;

    /////////////////////////////
    // BLACK 08
    /////////////////////////////
    // /* 2C28 DISMISSED pour %°
    case 151:
    { // NUM VLV to midiSec midi glitch
        // old potVal2 mapping ;
        // old potVal2 mapping ;

        affiche = nulString;
        affiche = afficheValueStraightOrFromSequence(potVal2, "/", 1);
        affiche += String(DenomRatioDeltaToMsec);
        if (potVal2 >= 1 )
        {
        float RatioDeltaToMsecTemp = (float)potVal2 / (float)DenomRatioDeltaToMsec;
        affiche += F(" : ");
        affiche += String(RatioDeltaToMsecTemp, 2);
        }
        printRot2(affiche, 16, 300, 3);
    }
    break;
        // byte NumRatioDeltaToMsec = 4 ;

    case 152:
    { // DENOM VLV to midiSec midi glitch
        // old potVal2 mapping ;
        // old potVal2 mapping ;
        // affiche =  nulString ; ;
        //
        affiche = String(NumRatioDeltaToMsec) + "/" + afficheValueStraightOrFromSequence(potVal2, " ", 1);
        if ( potVal2 > 0 )
        {
        float RatioDeltaToMsecTemp = (float)NumRatioDeltaToMsec / (float)potVal2;
        affiche += F(" : ");
        affiche += String(RatioDeltaToMsecTemp, 2);
        }
        printRot2(affiche, 16, 300, 3);
    }
    break;
        // byte DenomRatioDeltaToMsec = 0 ;

        // NOTE**** il faut aussi penser à un système faisant varier le Ratio en temps réel ( comme le midi player ! )

    case 153:
    { // Create Mask
        // old potVal2 mapping ;
        // old potVal2 mapping ;

        affiche = nulString;
        ;
        affiche = String153;
        if (nowMillis % 1000 < 700)
        {
            affiche += (ChoixString153.substring(potVal2, potVal2 + 1));
        }
        else
        {
            affiche += F("*");
        }
        affiche += (F(" "));
        printRot2(affiche, 16, 300, 3);

        // NOTE**** trouver une fc réutilisable, du style : DisplayString (  &String153, &ChoixString153, potVal2 )
    }
    break;

    case 156:
    { // choix du Swing
        // old potVal2 mapping ;
        // old potVal2 mapping ;

        affiche = nulString;
        ;
        affiche += (preset156[potVal2]);
        affiche += (F(" "));
        printRot2(affiche, 16, 300, 3);

        // NOTE**** trouver une fc réutilisable, du style : DisplayString (  &String153, &ChoixString153, potVal2 )
    }
    break;

        // case 157 : NameTransfo = F("157 Fuse"); break ;
        // case 158 : NameTransfo = F("158 >Type Fuse"); break ;

    case 158: // Define Type of Fuse
    {
        // old potVal2 mapping ;
        // old potVal2 mapping ;
        affiche = nulString;
        ;
        // 158 Define TypeFuse
        // 1 = Same Note
        // 2 = All Notes
        // 3? = même note à travers octaves ?
        switch (potVal2)
        {
        case 0:
            affiche = (F("1.Same Note"));
            break;
        case 1:
            affiche = (F("2.All Notes"));
            break;
        default:
            affiche = (F("Ugh?"));
            break;
        }
        printRot2(affiche, 16, 300, 3);
    }
    break;

        // case 159 : {
        //     // printRotFast (F("Custom Swing Pattern") );
        //     // old potVal2 mapping ;
        //     // old potVal2 mapping ;
        //
        //     // affiche =  nulString ; ;
        //     // affiche = String153;
        //     // affiche += (ChoixString153.substring(potVal2, potVal2+1)) ;
        //     // affiche += (F(" "));
        //     // printRot2(affiche,16,300,3);
        //
        //
        //     affiche =  nulString ; ;
        //     affiche = String159;
        //     affiche += (ChoixString156.substring(potVal2, potVal2+1)) ;
        //     affiche += (F(" "));
        //     printRot2(affiche,16,300,3);
        //
        //     // affiche =  nulString ; ;
        //     // affiche = (patternSwingCustom);
        //     // affiche+= (ChoixString156.substring(potVal2, potVal2+1)) ;
        //     // affiche+= (F(" "));
        //
        //     // printRot2(affiche,16,300,3);
        //     // preset156[taillePresetString156] = String159 ; // remplace le dernier pattern de la liste par le custom !
        //     // NOTE**** trouver une fc réutilisable, du style : DisplayString (  &String153, &ChoixString153, potVal2 )
        //
        // }
        // break ;

    case 159:
    { // Create Custom Swing pattern

        affiche = nulString;
        ;
        affiche = String159;
        if (nowMillis % 1000 < 700)
        {
            affiche += (ChoixString156.substring(potVal2, potVal2 + 1));
        }
        else
        {
            affiche += F("*");
        }
        affiche += (F(" "));
        printRot2(affiche, 16, 300, 3);

        // NOTE**** trouver une fc réutilisable, du style : DisplayString (  &String153, &ChoixString153, potVal2 )
    }
    break;

    case 162:
    { // Select fc spéciales MidiMatrix Redirect
        //GetNameTransformation(Transfo[counter  - 1][0]);
        //printRotFast (NameTransfo );
        // old potVal2 mapping ;
        // old potVal2 mapping ;
        affiche = nulString;
        ;

        switch (potVal2)
        {
        case 1:
            affiche = (F("1.Empty matrix"));
            break;
        case 2:
            affiche = F("2.CH");
            affiche += flechedroite;
            affiche += F("CHorigine");
            break;
        case 3:
            affiche = F("3.Random 1");
            break;
        case 4:
            affiche = F("4.All");
            affiche += flechedroite;
            affiche += F("CH tget");
            break;
        case 5:
            affiche = F("5.CH tget");
            affiche += flechedroite;
            affiche += F("All");
            break;
        case 6:
            affiche = F("6.All");
            affiche += flechedroite;
            affiche += F("4 first");
            break;
        case 7:
            affiche = F("7.Random ");
            break;
        default:
            affiche = F("Ugh?");
            break;
        }
        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 163:
    { // view Channel direction
        // GetNameTransformation(Transfo[counter  - 1][0]);
        // NameTransfo = "CH" + String(potVal2+1) + NameTransfo ;
        // printRot2(NameTransfo,10,400,2);
        // old potVal2 mapping ;
        // old potVal2 mapping ;
        buffer_and_HARD_setCursor(0, 1);
        printMatrixLine(midiRedirect, potVal2 - 1);
        // MODIF 1C86
        // TODO : transformer en fonction qui donne un String DEBUG ****
    }
    break;

    case 164:
    { // Skip Chunck Parallel Midi Player
        printRot2((SkipChunck == true) ? F("ON ") : F("OFF"), 16, 300, 3);
    }
    break;

    case 172: // Send CC to TX81Z - début de l'affichage auto ON/OFF
    {
        printRot2(sendCCtoTX81Z == true ? F("ON ") : F("OFF"), 16, 300, 3);
    }
    break;

    case 179:
    {
        // Horizontal Mapping
        affiche = "";
        affiche += (F("("));

        affiche += String(mapHSourceMin);
        affiche += (F(","));
        affiche += String(mapHSourceMax);

        affiche += F(")");
        affiche += flechedroite;
        affiche += F("(");

        affiche += String(mapHTargetMin);
        affiche += (F(","));
        affiche += String(mapHTargetMax);
        affiche += (F(")"));
        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 181: // Define Type of Breathe
    {
        // old potVal2 mapping ;
        // old potVal2 mapping ;
        affiche = nulString;
        ;
        // 158 Define TypeFuse
        // 1 = Same Note
        // 2 = All Notes
        // 3? = même note à travers octaves ?
        switch (potVal2)
        {
        case 0:
            affiche = (F("1.move"));
            break;
        case 1:
            affiche = (F("2.move all"));
            break;
        case 2:
            affiche = (F("3.all+layer"));
            break;
        default:
            affiche = (F("Ugh?"));
            break;
        }
        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 186: // Fregoli-ze cette note
    {

        // on va afficher la valeur : -24 à +24
        // on indique aussi à quelle note cela correspond
        // pour ça on utilise noteToWorkOn + la valeur

        // potVal2 = map(counter2, 0, 1023, Transfo[softwareValueEncoderLeft][1] , Transfo[softwareValueEncoderLeft][2]  ) ;

        affiche = GiveNameOfNote(noteToWorkOn) + giveSign(potVal2) + F(" > ") + GiveNameOfNote(noteToWorkOn + potVal2) + F("(") + GiveFrenchNameOfNote(noteToWorkOn + potVal2) + F(")");

        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 187: // Cotard-ize cette note ( make it silent )
    {

        // on va afficher la valeur : -24 à +24
        // on indique aussi à quelle note cela correspond
        // pour ça on utilise noteToWorkOn + la valeur

        affiche = nulString;
        ;
        affiche = GiveNameOfNote(noteToWorkOn) + F("(") + GiveFrenchNameOfNote(noteToWorkOn + potVal2) + F(") > ");

        if (isSilentSByte(noteRedirect[noteToWorkOn]))
        {
            affiche += F("SILENT");
        }
        else
        {
            affiche += F("sound ");
        }

        printRot2(affiche, 16, 400, 3);

        // printRot2(( isSilentSByte(noteRedirect[noteToWorkOn])==true?"SILENT":"sound ",16,400,3);
    }
    break;

    case 188: // Macro pour Cotard / Fregoli
    {
        // old potVal2 mapping ;
        // old potVal2 mapping ;
        String affiche;

        switch (potVal2)
        {
        case 0:
            affiche = F("1.Reset Note redirection");
            break;
        case 1:
            affiche = F("2.Randomize Notes");
            break;
        case 2:
            affiche = F("3.Kill all flats");
            break;
        case 3:
            affiche = F("4.Kill all naturals");
            break;
        case 4:
            affiche = F("5.Move Up/Down all flats");
            break;
        case 5:
            affiche = F("6.All to ONE");
            break;
        case 6:
            affiche = F("7.All to RndONE");
            break;
        case 7:
            affiche = F("8.All to RndTWO");
            break;
        case 8:
            affiche = F("9.Rnd Silencer");
            break;
        case 9:
            affiche = F("10.Inverter Scale");
            break;
        default:
            affiche = F("Oink!");
            break;
        }

        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 189: // Set Type CC Mod source
    {
        // old potVal2 mapping ;
        // old potVal2 mapping ;

        String affiche;
        affiche = giveStringOfCCMod(currentCCMod);

        switch (potVal2)
        {
        case 0:
            affiche += F("0.vide - WTF ?");
            break;
        case 1:
            affiche += F("1.velocity");
            break;
        case 2:
            affiche += F("2.pitch ");
            break;
        case 3:
            affiche += F("3.note (C,C#,etc)");
            break;
        case 4:
            affiche += F("4.LG note");
            break;
        default:
            affiche += F("Oink!");
            break;
        }

        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 190:
    {
        // Select CCMOd
        String affiche;
        affiche = giveStringOfCCMod(currentCCMod);

        affiche += F(" -> ");
        affiche += String(potVal2 + 1);
        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 195: // Set Borders Behavior
    {
        // old potVal2 mapping ;
        // old potVal2 mapping ;

        String affiche;
        affiche = giveStringOfCCMod(currentCCMod);

        switch (potVal2)
        {
        case 0:
            affiche += F(" 1.limit value");
            break;
        case 1:
            affiche += F(" 2.expand value");
            break;
        case 2:
            affiche += F(" 3.no CC send beyond");
            break;
        case 3:
            affiche += F(" 4.middle void");
            break;
        default:
            affiche += F(" Oink!");
            break;
        }

        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 198:
    { // Define Layers Concernes

        // old potVal2 mapping ;
        // old potVal2 mapping ;

        // NOTE **** problème pour sélectionner ACTIVE LAYERS, comme Transfo 141

        affiche = nulString;
        affiche = giveStringOfCCMod(currentCCMod);
        affiche += F(" ");

        if (potVal2 >= 16)
        {
            affiche += F("ACTIfS L.");
        }
        else
            for (byte i = 0; i < nbLayer; i++)
            {
                // déterminer si la layer i est mute
                byte k = ((potVal2 >> i) & B00000001); // le bit de droite est 1 si layer 0 est MUTE ; le 2ème bit est 1 si layer 1 est MUTE, etc...
                if (k == B00000001)
                {
                    affiche += String(i + 1);
                }
                else
                {
                    affiche += (F("-"));
                }
            }
        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 199: //  ON/OFF pour envoi CC Mod à partir de Midi //
    {
        // pileCCMod* CCModPtr ;
        //
        // CCModPtr = giveNthPtrPileComplete(CCModPointeurInitial, currentCCMod);
        // byte value = (*CCModPtr).CCMod.layerConcerne ;
        byte value = CCModTableau[currentCCMod].layerConcerne;
        value = (value & B10000) >> 4;

        printRot2("dire qqch sur midi", 16, 300, 3); // TODO DEBUG

        // TODO écrire qqch ici sur le send midi !
    }
    break;

    case 200: //  Add CC Mod
    {

        // pileCCMod* CCModPtr ;
        //
        // CCModPtr = giveNthPtrPileComplete(CCModPointeurInitial, currentCCMod);
        // bool actif = (*CCModPtr).CCMod.actif ;
        bool actif = CCModTableau[currentCCMod].actif;

        String affiche;
        affiche = giveStringOfCCMod(currentCCMod);

        if (actif == true)
        {
            affiche += F(" ON");
        }
        else
        {
            affiche += F(" OFF");
        }
        // printRot2(actif==true?F("ON "):F("OFF"),16,300,3);
        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 201:
    { // MIDI THRU ON/OFF
        printRot2(midiThru == true ? F("ON ") : F("OFF"), 16, 300, 3);
    }
    break;

    case 204:
    {
        // affiche info sur prochaine note 0
        // old potVal2 mapping ;
        // old potVal2 mapping ;

        byte i = potVal2;
        byte j = 0;
        unsigned long a = 0;

        a = layerLength[i];
        actualizeTimeToPlay(i, j);

        affiche = nulString;
        // unsigned int taille = mesureTaillePileComplete(pInitial[i]);
        pileNote tempNote;
        tempNote = ZEROgetNoteFromPile(pInitial[i], j);

        affiche += String(a) + F(" ") + String(tempNote.timeToPlayON) + " L" + String(i);
        printRot2(affiche, 16, 300, 3);
    }
    break;

/////////////////////////////
// BLACK 08 
/////////////////////////////

    case  205 : 
    { // dureePole1 
        affiche = afficheValueStraightOrFromSequence(potVal2, "msec", 0);
        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 206:
    { // min Pole1
        affiche = afficheValueStraightOrFromSequence(potVal2, "x", -10);
        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 207:
    { // max Pole1
        affiche = afficheValueStraightOrFromSequence(potVal2, "x", -10);
        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 208:
    { // dureePole2
        affiche = afficheValueStraightOrFromSequence(potVal2, "msec", 0);
        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 209:
    { // min Pole2
        affiche = afficheValueStraightOrFromSequence(potVal2, "x", -10);
        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 210:
    { // max Pole2
        affiche = afficheValueStraightOrFromSequence(potVal2, "x", -10);
        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 211:
    { // freq Pole1 
        affiche = afficheValueStraightOrFromSequence(potVal2, "%",  0);
        printRot2(affiche, 16, 300, 3);
    }
    break;


    case 212:
    {
        printRot2(turnOnBipolarRandom == true ? F("ON ") : F("OFF"), 16, 300, 3);
    }
    break;

    case 213:
    {
        // Vertical Mapping pour Vélocités
        String affiche;

        affiche = (F("("));

        affiche += String(FindLowestVelocityOfActive());
        affiche += (F(","));

        affiche += String(FindHighestVelocityOfActive());
        affiche += F(")");
        affiche += flechedroite;
        affiche += F("(");

        affiche += String(targetVelociteMin);
        affiche += (F(","));
        affiche += String(targetVelociteMax);
        affiche += (F(")"));
        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 216:
    { // POOF
        affiche = nulString;

    affiche = afficheValueStraightOrFromSequence(potVal2, "msec", 0);

        affiche += F("  ");

        if (turnOnPoof == true)
        {
            affiche += (F("ON "));
        }
        else
        {
            affiche += (F("OFF"));
        }

        if (showFract)
        {

            if (firstActiveLayer() != 50) // çàd si aucune layer n'est active
            {
                fraction = closestFraction(potVal2, layerLength[firstActiveLayer()], 16, false);
                if (fraction.imprecision < 0)
                {
                    affiche = affiche + "<";
                }
                if (fraction.imprecision > 0)
                {
                    affiche = affiche + ">";
                }
                if (fraction.imprecision == 0)
                {
                    affiche = affiche + "=";
                }

                affiche = affiche + String(fraction.num);
                affiche = affiche + F("/");
                affiche = affiche + String(fraction.den);
            }
        }

        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 217:
    { // Filter or KILL note

        affiche = nulString;

        if (killingNote == true)
        {
            affiche += (F("ON  "));

            affiche += String(potVal2);
            affiche += F(" ");
            affiche += GiveNameOfNote(potVal2);
            affiche += F("(");
            affiche += GiveFrenchNameOfNote(potVal2);
            affiche += F(") ");

            noteToKill = potVal2;
        }
        else
        {
            affiche += (F("OFF "));
            noteToKill = 0;
        }

        printRot2(affiche, 16, 400, 3);
        // killingNote = true ; // il est invalidé dans le programme principal, de sorte que qd on change de fonction, il revient à false ;
    };
    break;

    case 218:
    {
        // old potVal2 mapping ;
        // old potVal2 mapping ;

        busyWithMIDIAffichage = true;

        // channelForCell est défini par case 219
        // tandis que noteForCell est potVal2

        byte noteForCell = potVal2;

        if (softwareValueEncoderLeft != oldCounterLeft)
        {
            // cela signife qu'on vient de tourner le bouton pour arriver sur Cell
            // on peut donc prendre cette valeur de potVal2 pour savoir si par après elle sera modifiée
            initPotVal2 = potVal2;
            initKick = true;
            consToCons = true; // si premier KIck dans Cell, les consolidated doivent jouer du consolidated
            atLeastOneKick = false;
        }

        // dans version plus élaborée :
        // si NOTE jouée sur pad non consolidée -> joue ce qui est indiqué par l'écran
        // si note jouée sur pad a été consolidée -> joue ce qui remplace cette note ( vérifie dans un array )
        //          SAUF si entre le play sur pad le bouton rotatif droit a été modifié :
        //          modifier ce bouton après avoir joué sur le pad en mode CELL signifie donc qu'on veut changer la valeur
        // qui de l'array :
        // idéalement un pour chaque note [0..127] - en pratique on peut se contenter d'une dizaine de notes je suppose
        // sur le Wave Pad il y en a 1 + 4 + 8 = 13

        byte note(0);
        byte channel(0);
        byte velocity(0);
        affiche = nulString;

        affiche = F("CH");
        affiche += String(lastChannel + 1);
        affiche += F("N");
        affiche += String(lastPad);

        affiche += " ";
        affiche += flechedroite;
        affiche += F(" CH");
        affiche += String(channelOutForCell + 1);
        affiche += F("N");
        affiche += String(noteForCell);
        printRot2(affiche, 16, 400, 3);

        if (MIDI3.read(0))
        {
            channel = MIDI3.getChannel() - 1; // donne le channel, pas le codage du channel ! CH1 = 0 pour machine
            note = MIDI3.getData1();
            velocity = MIDI3.getData2();
            // timenote = millis() ;
        }

        if (note != 0)
        {
            switch (MIDI3.getType())
            {
            case midi::NoteOn:
            {

                // digitalWrite(5, HIGH);

                currentPad = note;
                // printRot2(affiche,16,400,3);

                noteForCellArray = note; // cette valeur est utilisée dans le transform pour enregistrer la valeur
                channelInForCell = lastChannel;

                // TODO DEBUG il faudrait aussi que Cell tienne compte du channel d'où vient la note IN !

                if (initKick == true)
                {
                    lastPad = currentPad;
                    lastChannel = channel;
                    channelInForCell = channel;
                }

                if ((currentPad == lastPad) && (potVal2 != initPotVal2))
                {
                    consToCons = false;
                }

                if ((initKick == true) && (currentPad != lastPad))
                {
                    consToCons = true;
                }

                if (isNoteCellConsolidated(channel, note))
                // refaire fonction avec Channel + note ?
                {
                    byte place = givePlaceInTableauForNoteCellConsolidated(channel, note);
                    PlaynoteOn_(0x90 + tableauCell[place].channelOut, tableauCell[place].noteOut, velocity);

                    PlaynoteOn_(0x80 + tableauCell[place].channelOut, tableauCell[place].noteOut, 0);
                    // 2C02 : à mettre dans la pileOnce ?

                    affiche = F("PLAYING ");
                    affiche += String(place);
                    printRot2(affiche, 16, 400, 3);

                    //                                if (consToCons == true ) {
                    //                                    byte place = givePlac eInTableauForNoteCellConsolidated(note);
                    //                                    PlaynoteOn_(0x90+tableauCell[place][1], tableauCell[place][2], velocity) ;
                    //
                    //                                    PlaynoteOn_(0x80+tableauCell[place][1], tableauCell[place][2], velocity) ;
                    //
                    //                                }
                    //                                else
                    // {
                    //                                    PlaynoteOn_(0x90+channelForCell-1, noteForCell, velocity) ;
                    //
                    //                                    PlaynoteOn_(0x80+channelForCell-1, noteForCell, velocity) ;
                    //
                    //                                }
                }

                else
                {
                    PlaynoteOn_(0x90 + channelOutForCell, noteForCell, velocity);
                    initPotVal2 = potVal2;

                    PlaynoteOn_(0x80 + channelOutForCell, noteForCell, velocity);
                }
                // si note non consolidée -> joue note du potVal2

                // si note consolidée -> si initPotVal2 != potVal2 -> joue note du potVal2
                //
                // si note consolidée -> si initPotVal2 == potVal2 -> joue note consolidée OUT
                //
                // si potVal2 == initPotVal2 ( çàd on n'a pas tourné ) et la note frappée a déjà été consolidée
                // çàd qu'il existe une note dans l'array qui correspond à cette note
                // alors on joue la note OUT consolidée
                //
                // // si potVal2 == initPotVal2 ( çàd on n'a pas tourné ) et la note frappée n'a pas été consolidée
                // çàd qu'il n' existe pas une note dans l'array qui correspond à cette note
                // alors on joue la note OUT correspondant à potVal2
                //
                // si potVal2 != initPotVal2 ( çàd on a tourné ),
                // même si la note frappée a déjà été consolidée
                // on joue la note qui correspond à potVal2  ( idem à dessus )

                // digitalWrite(5, LOW);

                lastPad = currentPad;
                lastChannel = channel;
                initKick = false;
            } // fin du NoteON
            break;

            default:
                break;
            }
        }
    }
    break;

    case 219:
    { // Select Channel OUT pour CELL
    };
    break;

    case 220:
    { // SOFT PASS ON/OFF
        // printRot2(softPass == true ? F("ON ") : F("OFF"), 16, 300, 3);
        affiche = (softPass == true ? F("ON ") : F("OFF ")); // 2C28
                                                             // TODO simplifier les "ON" et "OFF" en 1 seul string pour toute mémoire

        unsigned int taille = mesureTaillePileComplete(pSoftPassRedirect);
        affiche += "sz:";
        affiche += String(taille);
        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 223:
    {
        affiche = "DISMISSED";

        // 2C22 suppression, car inutilie vu le REC IN

        printRot2(affiche, 16, 400, 3);
    };
    break;

    case 224:
    {
        // ADJUST END
        //
        affiche = nulString;

        // old potVal2 mapping ;
        // old potVal2 mapping ;

        if (potVal2 > 0)
        {
            affiche = F("+");
            affiche += String(potVal2) + F(" msec ");
        }
        else
        {
            affiche = String(potVal2);
            affiche += F(" msec ");
        }

        byte iLatestStart(0);
        byte iLatestEnd(0);
        byte iLatestEndCalculated(0);

        unsigned long latestStart(0);
        unsigned long latestEnd(0);
        unsigned long latestEndCalculated(0);

        unsigned long calculatedEnd;

        for (byte i = 0; i < nbLayer; i++)
        {
            if (isLayerActive(i))
            {
                unsigned int taille = mesureTaillePileComplete(pInitial[i]);
                pileNote tempNote;

                for (byte j = 0; j < taille; j++)
                {
                    tempNote = ZEROgetNoteFromPile(pInitial[i], j);

                    if (tempNote.note != 0)
                    {

                        if (tempNote.startNote > latestStart)
                        {
                            latestStart = tempNote.startNote;
                            iLatestStart = i;
                        }

                        if (tempNote.endNote > latestEnd)
                        {
                            latestEnd = tempNote.endNote;
                            iLatestEnd = i;
                        }

                        if (tempNote.startNote > tempNote.endNote)
                        {
                            calculatedEnd = layerLength[i] + tempNote.endNote;
                        }
                        else
                        {
                            calculatedEnd = tempNote.endNote;
                        }

                        if (calculatedEnd > latestEndCalculated)
                        {
                            latestEndCalculated = calculatedEnd;
                            iLatestEndCalculated = i;
                        }
                    }
                }
            };
        };

        // if ( latestEndCalculated < layerLength[iLatestEndCalculated] ) {
        //     latestEndCalculated += layerLength[iLatestEndCalculated]  ;
        //     // ceci est juste nécessaire pour que l'affichage soit cohérent quend le calcul est plus petit que la layer,
        //     // car juste après on déduit de ce nombre la layer ( utile qd plus grand )
        // }

        affiche += F("S:");
        affiche += String(layerLength[iLatestStart] - latestStart) + F(" E:") + String(layerLength[iLatestEnd] - latestEnd) + F(" C:") + String((int)((int)layerLength[iLatestEndCalculated] - (int)latestEndCalculated));

        printRot2(affiche, 16, 400, 3);
    };

    break;

    case 225:
    {
        // ADJUST START
        //
        affiche = nulString;
        ;

        // old potVal2 mapping ;
        // old potVal2 mapping ;

        if (potVal2 > 0)
        {
            affiche = F("+");
            affiche += String(potVal2);
            affiche += F("msec ");
        }
        else
        {
            affiche = String(potVal2);
            affiche += F("msec ");
        }

        // byte mLatestStart (0) ;
        // byte mLatestEnd (0) ;
        // byte mLatestEndCalculated (0) ;

        // unsigned int latestEndCalculated (0);

        unsigned long earliestStart(0);
        unsigned long earliestEnd(0);
        bool foundStart = false;

        for (byte i = 0; i < nbLayer; i++)
        {
            if (isLayerActive(i))
            {
                unsigned int taille = mesureTaillePileComplete(pInitial[i]);
                pileNote tempNote;

                for (byte j = 0; j < taille; j++)
                {
                    tempNote = ZEROgetNoteFromPile(pInitial[i], j);

                    if (tempNote.note != 0)
                    {
                        if (foundStart == false)
                        {
                            earliestStart = tempNote.startNote;
                            earliestEnd = tempNote.endNote;
                            foundStart = true;
                        }

                        if (tempNote.startNote < earliestStart)
                        {
                            earliestStart = tempNote.startNote;
                        }

                        if (tempNote.endNote < earliestEnd)
                        {
                            earliestEnd = tempNote.endNote;
                        }
                    }
                }
            };
        };
        affiche += F("S:");
        affiche += String(earliestStart) + F(" E:") + String(earliestEnd);
        // + " C:" + String(layerLength[i]-latestEndCalculated) ;

        printRot2(affiche, 16, 400, 3);
    };
    break;

    case 226:
    { // Modify Sequence
        affiche = afficheValueStraightOrFromSequence(potVal2, "", floorSeq);
        if (potVal2 >= 0)
        {
            affiche = "+" + affiche;
        }

        affiche = affiche + " > " + giveStringOfSequence(currentSequence);

        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 227:
    { // Delete Sequence
        // old potVal2 mapping ;
        // old potVal2 mapping ;

        affiche = nulString;

        affiche = giveStringOfSequence(potVal2);

        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 228:
    { // Select Sequence
        // old potVal2 mapping ;
        // old potVal2 mapping ;

        affiche = nulString;
        ;
        affiche = giveStringOfSequence(potVal2);
        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 229:
    { // Erase 1 digit from SEquence
        // old potVal2 mapping ;
        // old potVal2 mapping ;

        String affiche;
        affiche = giveStringOfSequence(potVal2);
        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 230:
    { // SET Type de Sequence ( fwd, back, rnd, a-r, jitter, rndfrag )
        // old potVal2 mapping ;
        // old potVal2 mapping ;

        affiche = nulString;
        affiche = giveShortStringOfSequence(currentSequence);

        // const byte modeFWD = 0 ; //
        // const byte modeBACK = 1 ;//
        // const byte modeRANDOM = 2 ;
        // const byte modeALLERRETOUR = 3 ;
        // const byte modeJITTER = 4 ;
        // const byte modeRANDOMFRAG = 5 ;

        switch (potVal2)
        {
        case 0:
            affiche += (F("1.FWD"));
            break;
        case 1:
            affiche += (F("2.BACK"));
            break;
        case 2:
            affiche += (F("3.Random"));
            break;
        case 3:
            affiche += (F("4.Aller-Retour"));
            break;
        case 4:
            affiche += (F("5.Jitter"));
            break;
        case 5:
            affiche += (F("6.Rnd Fragments (Riffs)"));
            break;
        case 6:
            affiche += F("7.progression a->b->c");
            break;
        case 7:
            affiche += F("8.rnd spectum a-b");
            break;
        case 8:
            affiche += F("9.bipolar A *c-d E*f-g H%deA");
            break;
        case 9:
            affiche += F("10.bipolar A B C%deA");
        case 10:
            affiche += F("11.n repeater");
            break;

            // cmt sont encodées les valeurs A B C E F G ???
            // proposition BLACK08
        case 11:
            affiche += F("12.Markov <-A/100 ->B/100 self/100");
            break;

        default:
            affiche += (F("--------- "));
            break;
        }

        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 231:
    { // SET Data for jitter : jitterMinUP
        // old potVal2 mapping ;
        // old potVal2 mapping ;

        affiche = nulString;

        // const byte modeFWD = 0 ; //
        // const byte modeBACK = 1 ;//
        // const byte modeRANDOM = 2 ;
        // const byte modeALLERRETOUR = 3 ;
        // const byte modeJITTER = 4 ;
        // const byte modeRANDOMFRAG = 5 ;

        if (sequence[currentSequence].modeSeq != 4)
        {
            affiche = F("only for Jitter");
        }
        else
        {
            affiche = giveShortStringOfSequence(currentSequence);
            affiche += " +" + String(potVal2);

            // byte jitterMinUP;
            // byte jitterMaxUP;
        }

        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 232:
    { // SET Data for jitter : jitterMaxUP
        // old potVal2 mapping ;
        // old potVal2 mapping ;

        affiche = nulString;

        if (sequence[currentSequence].modeSeq != 4)
        {
            affiche = F("only for Jitter");
        }
        else
        {
            affiche = giveShortStringOfSequence(currentSequence) + " +" + String(potVal2);
        }

        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 233:
    { // SET Data for jitter : jitterMinDOWN
        // old potVal2 mapping ;
        // old potVal2 mapping ;

        affiche = nulString;

        if (sequence[currentSequence].modeSeq != 4)
        {
            affiche = F("only for Jitter");
        }
        else
        {
            affiche = giveShortStringOfSequence(currentSequence) + " +" + String(potVal2);
        }

        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 234:
    { // SET Data for jitter : jitterMaxDOWN
        // old potVal2 mapping ;
        // old potVal2 mapping ;

        affiche = nulString;

        if (sequence[currentSequence].modeSeq != 4)
        {
            affiche = F("only for Jitter");
        }
        else
        {
            affiche = giveShortStringOfSequence(currentSequence) + " +" + String(potVal2);
        }

        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 235:
    { // SET Data for jitter : jitterStaticMin
        // old potVal2 mapping ;
        // old potVal2 mapping ;

        affiche = nulString;

        if (sequence[currentSequence].modeSeq != 4)
        {
            affiche = F("only for Jitter");
        }
        else
        {
            affiche = giveShortStringOfSequence(currentSequence);
            affiche += F(" +");
            affiche += String(potVal2);
        }

        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 236:
    { // SET Data for jitter : jitterStaticMax
        // old potVal2 mapping ;
        // old potVal2 mapping ;

        affiche = nulString;

        if (sequence[currentSequence].modeSeq != 4)
        {
            affiche = F("only for Jitter");
        }
        else
        {
            affiche = giveShortStringOfSequence(currentSequence) + " +" + String(potVal2);
        }

        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 237:
    { // SET Data for RAndom Frag : Size RAndom Fragment
        // old potVal2 mapping ;
        // old potVal2 mapping ;

        affiche = nulString;

        if (sequence[currentSequence].modeSeq != 5)
        {
            affiche = F("only for RndFragments");
        }
        else
        {
            affiche = giveShortStringOfSequence(currentSequence) + " size:" + String(potVal2);
        }

        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 238:
    { // SET Data for RAndom Frag : Nb Repeats
        // old potVal2 mapping ;
        // old potVal2 mapping ;

        affiche = nulString;

        if (sequence[currentSequence].modeSeq != 5)
        {
            affiche = F("only for RndFragments");
        }
        else
        {
            affiche = giveShortStringOfSequence(currentSequence) + " R:" + String(potVal2);
        }

        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 239:
    { // SET ACTION for Sequence
        // old potVal2 mapping ;
        // old potVal2 mapping ;

        affiche = nulString;

        // byte action ; // 00 = CC    1 = NOte     2 = Velocite    plus tard : 3 = SWING
        // // byte type ; // 0 FWD    1 BACK    2 RND    3 A-R    4 JITTER    5 RND-FRG est MODESEQ
        // byte CC ;
        // byte channel ;
        // byte layer ;

        switch (potVal2)
        {
        case 0:
            affiche += (F("1.Pitch ADD")); // DONE
            break;
        case 1:
            affiche += (F("2.Send CC")); // DONE
            break;
        case 2:
            affiche += (F("3.Velocity ADD")); // DONE
            break;
        case 3:
            affiche += (F("4.Pitch SET")); // 2C02
            break;

        case 4:
            affiche += (F("5.Event List"));
            break;
        case 5:
            affiche += (F("6.LG note ADD")); // DONE - HOORIBLE TODO BETTER
            break;
        case 6:
            affiche += (F("7.LayerL SET")); // DONE 2C20
            break;
        case 7:
            affiche += F("8.LayerL ADD"); // DONE 2C20
            break;
        case 8:
            affiche += F("9.LayerL %"); // DONE 2C20
            break;


        case 9:
            affiche += (F("10.Vel SET")); // DONE 2C20 16-2-2020
            break;
        case 10:
            affiche += F("11.Vel ADD"); // DONE 2C20 16-2-2020 , idem à 2  - REDONE ???
            break;
        case 11:
            affiche += F("12.Vel %"); // DONE 2C20 16-2-2020
            break;


        case 12:
            affiche += F("13.NoteL SET"); // DONE
            break;
        case 13:
            affiche += F("14.NoteL ADD"); // DONE 2C20
            break;
        case 14:
            affiche += F("15.NoteL %"); // DONE 2C20
            break;


        case 15:
            affiche += F("16.Degree ADD"); // DONE 2C20 16-2-2020
            break;
        case 16:
            affiche += F("17.Degree RND"); // DONE 2C20 16-2-2020
            break;
        case 17:
            affiche += F("18.Vel RND"); // DONE 2C20 16-2-2020
            break;
        case 18:
            affiche += F("19.Note ON/OFF"); // TODO
            break;
        case 19:
            affiche += F("20.Pitch RND"); // DONE 2C20 16-2-2020
            break;

        case 20:
            affiche += F("21.Set CH"); // BLACK08
            break;
        case 21:
            affiche += F("22.Add CH"); // BLACK08
            break;
case 22:
            affiche += F("23.Bug CH"); // BLACK09 conserve un bug entre notes et CH 
            break;

case 23:
    affiche += F("24.CC SET"); // DONE
    break;
case 24:
    affiche += F("25.CC ADD"); // DONE 2C20
    break;
     case 25 :
            affiche += F("26.CC %"); // DONE
            break;
     case 26:
         affiche += F("27.CC RND"); // DONE
         break;

     default:
         affiche += (F("--------- "));
         break;

         // 2C02 TODO SWING
        }

        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 240:
    { // SET CC
        // old potVal2 mapping ;
        // old potVal2 mapping ;

        affiche = nulString;
        ;

        // byte action ; // 00 = CC    1 = NOte     2 = Velocite    plus tard : 3 = SWING
        // // byte type ; // 0 FWD    1 BACK    2 RND    3 A-R    4 JITTER    5 RND-FRG est MODESEQ
        // byte CC ;
        // byte channel ;
        // byte layer ;

        if (sequence[currentSequence].action != 0)
        {
            affiche = F("only for CC Action");
        }
        else
        {
            affiche = F("CC");
            affiche += String(potVal2);
        }

        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 241:
    { // SET channel
        // old potVal2 mapping ;
        // old potVal2 mapping ;

        affiche = nulString;
        ;

        // byte action ; // 00 = CC    1 = NOte     2 = Velocite    plus tard : 3 = SWING
        // // byte type ; // 0 FWD    1 BACK    2 RND    3 A-R    4 JITTER    5 RND-FRG est MODESEQ
        // byte CC ;
        // byte channel ;
        // byte layer ;

        if (sequence[currentSequence].action != 0)
        {
            affiche = F("only for CC Action");
        }
        else
        {
            affiche = String(potVal2);
        }

        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 242:
    { // SET layer
        // old potVal2 mapping ;
        // old potVal2 mapping ;

        // NOTE **** problème pour sélectionner ACTIVE LAYERS

        affiche = nulString;

        if (potVal2 >= 16)
        {
            affiche += F("ACTIfS L.");
        }
        else

            for (byte i = 0; i < nbLayer; i++)
            {
                // déterminer si la layer i est mute
                byte k = ((potVal2 >> i) & B00000001); // le bit de droite est 1 si layer 0 est MUTE ; le 2ème bit est 1 si layer 1 est MUTE, etc...
                if (k == B00000001)
                {
                    affiche += String(i + 1);
                }
                else
                {
                    affiche += (F("-"));
                }
            }
        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 243:
    { // SET sequence actif/inactif
        // old potVal2 mapping ;
        // old potVal2 mapping ;

        affiche = nulString;

        affiche = (sequence[potVal2].actif == true ? F("ON ") : F("OFF"));
        affiche += " ";
        affiche += giveStringOfSequence(potVal2);
        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 244:
    { //  ON OFF Activate DynHarmonize
        // affiche =  nulString;
        // affiche = F("layer");
        // affiche += String(potVal2 + 1);
        // affiche += (harmonizeLayer[potVal2] == true ? F(" ON ") : F(" OFF"));

        affiche = "";
        for (int i = 0; i < nbLayer; i++)
        {
            //affiche += String(i);
            if (harmonizeLayer[i])
            {
                affiche += "ON  ";
            }
            else
            {
                affiche += "OFF ";
            }
        }

        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 245:
    { // SET mode pour DynHarmonize
        // old potVal2 mapping ;
        // old potVal2 mapping ;

        affiche = nulString;

        switch (potVal2)
        {
        case 0:
            affiche += (F("1.+/- rnd"));
            break;
        case 1:
            affiche += (F("2.+"));
            break;
        case 2:
            affiche += (F("3.-"));
            break;
        default:
            affiche += (F("--------- "));
            break;
        }

        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 247:
    { //  choix compte layer ou notes
        // old potVal2 mapping ;
        // old potVal2 mapping ;

        affiche = nulString;

        affiche = F("Seq");
        affiche += String(potVal2 + 1);

        affiche += (sequence[potVal2].compteLayer == true ? F(" : Layers") : F(" : Notes"));
        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 248:
    { //  modulo layer Eclipse
        // old potVal2 mapping ;
        // old potVal2 mapping ;

        affiche = nulString;

        if (status248 == 0)
        {
            affiche = F("%modulo ");
            affiche += String(potVal2);
            affiche += F("+0:[");

            byte ex = potVal2;
            affiche += String(ex);
            affiche += ",";

            ex = (2 * potVal2);
            affiche += String(ex);
            affiche += ",";

            ex = (3 * potVal2);
            affiche += String(ex);
            affiche += ",..]";
        }

        if (status248 == 1)
        {
            affiche = F("Kmodulo ");
            affiche += String(eclipseModulo);
            affiche += F("+");
            affiche += String(potVal2);
            affiche += F(":[");

            byte ex = eclipseModulo + potVal2;
            affiche += String(ex);
            affiche += ",";

            ex = (2 * eclipseModulo) + potVal2;
            affiche += String(ex);
            affiche += ",";

            ex = (3 * eclipseModulo) + potVal2;
            affiche += String(ex);
            affiche += ",..]";
        }

        if (status248 == 2)
        {
            if (potVal2 < Transfo[softwareValueEncoderLeft][2] / 2)
            {
                affiche = F("1.mute those");
            }
            else
            {
                affiche = F("2.keep those, mute others");
            }
        }

        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 249:
        // DISMISSED 2C22
        { // turn ON OFF Markov Global
            //  printRot2(markovChainGlobal == true ? F("ON ") : F("OFF"), 16, 300, 3);
        }
        break;

    case 250:
    {
        printRot2(tapAccou == true ? F("ON ") : F("OFF"), 16, 300, 3);
    }
    break;

    case 251:
    {
        affiche = nulString;

        affiche = (GiveNameOfNote(gammeActuelle[0]));

        affiche += GiveNameOfMode(potVal2);

        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 252:
    { // turn ON OFF AutoCleanUp
        printRot2(autoCleanUp == true ? F("ON ") : F("OFF"), 16, 300, 3);
    }
    break;

    case 258:
    { // turn ON OFF hashUnify
        printRot2(hashUnify == true ? F("ON ") : F("OFF"), 16, 300, 3);
    }
    break;

    case 260:
    {
        // DELETE CCMod
        String affiche;
        affiche = giveStringOfCCMod(currentCCMod);
        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 261:
    {
        // Activate Condition
        affiche = nulString;
        ;
        affiche = giveStringOfEvent(CurrentEvenement);
        affiche = (Evenement[CurrentEvenement].conditionActive == true ? F("ON ") : F("OFF"));
        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 262:
    {
        // Condition : which teste : ou plutôt quelle valeur va-t-on tester ?
        affiche = nulString;
        ;
        affiche = giveStringOfEvent(CurrentEvenement);

        affiche += F(" ");

        // TODO DEBUG : faire fonction qui donne le nom de cette transformation
        // sera utile pour le SaveEvents

        affiche += getNameTest(potVal2);

        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 263:
    {
        // Condition : which operation : quelle opération mathématique applique ton à la valeur test ?
        affiche = nulString;
        ;
        affiche = giveStringOfEvent(CurrentEvenement);

        switch (potVal2)
        {
        case 1:
            affiche += (F(" 1.% (modulo)"));
            break;
        case 2:
            affiche += (F(" 2./ (divide)"));
            break;
        case 3:
            affiche += (F(" 3.x (multiply)"));
            break;
        case 4:
            affiche += (F(" 4.%+ (soon)"));
            break;
        case 5:
            affiche += (F(" 5.+ (add)"));
            break;
        // case 6 :  affiche+= (F(" 6.Pitch span")) ; break ;
        // case 7 :   affiche+=(F(" 7.LGNot min ")) ; break ;
        // case 8 :   affiche+= (F(" 8.LGNot max ")) ; break ;
        // case 9 :   affiche+= (F(" 9.LGNot moy ")) ; break ;
        // case 10 :  affiche+=  (F(" 10.Layer counter ")) ; break ;
        // case 11 :   affiche+=(F(" 11.notes counter ")) ; break ;
        // case 12 :  affiche+= (F(" 12.Time  ")) ; break ;
        // case 13 : affiche+= F(" 13.NbActiveLayers") ; break
        default:
            break;
        }
        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 264:
    {
        // Condition : which Value : valeur qui va être utilisée avec l'opération précédente
        affiche = nulString;
        ;
        affiche = giveStringOfEvent(CurrentEvenement);
        affiche += F(" ");
        affiche += String(potVal2);
        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 265:
    {
        // Condition : which comparison < > = pas égal
        affiche = nulString;
        ;
        affiche = giveStringOfEvent(CurrentEvenement);

        switch (potVal2)
        {
        case 1:
            affiche += F(" 1.<");
            break;
        case 2:
            affiche += F(" 2.>");
            break;
        case 3:
            affiche += F(" 3.==");
            break;
        case 4:
            affiche += F(" 4.pas egal");
            break;
        // case 5 :   affiche+=(F(" 5.+ (add)")) ; break ;
        // case 6 :  affiche+= (F(" 6.Pitch span")) ; break ;
        // case 7 :   affiche+=(F(" 7.LGNot min ")) ; break ;
        // case 8 :   affiche+= (F(" 8.LGNot max ")) ; break ;
        // case 9 :   affiche+= (F(" 9.LGNot moy ")) ; break ;
        // case 10 :  affiche+=  (F(" 10.Layer counter ")) ; break ;
        // case 11 :   affiche+=(F(" 11.notes counter ")) ; break ;
        // case 12 :  affiche+= (F(" 12.Time  ")) ; break ;
        // case 13 : affiche+= F(" 13.NbActiveLayers") ; break
        default:
            break;
        }
        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 266:
    {
        // Condition : which Reference: valeur qui va être utilisée avec l'opération précédente
        affiche = nulString;
        ;
        affiche = giveStringOfEvent(CurrentEvenement);
        affiche += F(" ");
        affiche += String(potVal2);
        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 267:
    {
        // Condition : which action
        affiche = nulString;
        ;
        affiche = giveStringOfEvent(CurrentEvenement);

        switch (potVal2)
        {
        case 1:
            affiche += (F(" 1.DO"));
            break;
        case 2:
            affiche += (F(" 2.BLOCK"));
            break;
        //case 3 :   affiche+= (F(" 3.==")) ; break ;
        //case 4 :  affiche+=  (F(" 4.pas egal") ; break ;
        // case 5 :   affiche+=(F(" 5.+ (add)")) ; break ;
        // case 6 :  affiche+= (F(" 6.Pitch span")) ; break ;
        // case 7 :   affiche+=(F(" 7.LGNot min ")) ; break ;
        // case 8 :   affiche+= (F(" 8.LGNot max ")) ; break ;
        // case 9 :   affiche+= (F(" 9.LGNot moy ")) ; break ;
        // case 10 :  affiche+=  (F(" 10.Layer counter ")) ; break ;
        // case 11 :   affiche+=(F(" 11.notes counter ")) ; break ;
        // case 12 :  affiche+= (F(" 12.Time  ")) ; break ;
        // case 13 : affiche+= F(" 13.NbActiveLayers") ; break
        default:
            break;
        }
        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 268: // Affichage des valeurs WhichTest  pour DEBUG et information
    {
        affiche = nulString;

        int temp = resultatTest(potVal2, activeLayer);
        affiche = String(temp);
        affiche += F("=");

        affiche += getNameTest(potVal2);

        // Soit je définis un nom pour chacune des fonctions, soit je fais une fonction générale qui donne fc(untel)

        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 269:
    { // SET layer

        // NOTE **** problème pour sélectionner ACTIVE LAYERS

        affiche = nulString;
        ;
        affiche = giveStringOfEvent(CurrentEvenement);
        affiche += " ";

        if (potVal2 >= 16)
        {
            affiche += F("ACTIfS L.");
        }
        else

            for (byte i = 0; i < nbLayer; i++)
            {
                // déterminer si la layer i est mute
                byte k = ((potVal2 >> i) & B00000001); // le bit de droite est 1 si layer 0 est MUTE ; le 2ème bit est 1 si layer 1 est MUTE, etc...
                if (k == B00000001)
                {
                    affiche += String(i + 1);
                }
                else
                {
                    affiche += (F("-"));
                }
            }
        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 270:
    { //  modulo Layer Event

        affiche = nulString;

        if (Evenement[CurrentEvenement].typeInterval != 4)
        {
            affiche = "ONLY FOR MODULO TIMING";
        }
        else
        {

            if (status270 == 0)
            {
                affiche = F("%modulo ");
                affiche += String(potVal2);
                affiche += F("+0:[");

                byte ex = potVal2;
                affiche += String(ex);
                affiche += ",";

                ex = (2 * potVal2);
                affiche += String(ex);
                affiche += ",";

                ex = (3 * potVal2);
                affiche += String(ex);
                affiche += ",..]";
            }

            if (status270 == 1)
            {
                affiche = F("Kmodulo ");
                affiche += String(Evenement[CurrentEvenement].modulo);
                affiche += F("+");
                affiche += String(potVal2);
                affiche += F(":[");

                byte ex = Evenement[CurrentEvenement].modulo + potVal2;
                affiche += String(ex);
                affiche += ",";

                ex = (2 * Evenement[CurrentEvenement].modulo) + potVal2;
                affiche += String(ex);
                affiche += ",";

                ex = (3 * Evenement[CurrentEvenement].modulo) + potVal2;
                affiche += String(ex);
                affiche += ",..]";
            }

            if (status270 == 2)
            {
                if (potVal2 < Transfo[softwareValueEncoderLeft][2] / 2)
                {
                    affiche = F("1.event just those");
                }
                else
                {
                    affiche = F("2.event all others");
                }
            }
        }
        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 272:
    { // SPAN FLEX - spanflex

        affiche = nulString;
        affiche = String(potVal2);
        affiche += F("/10 ");
        affiche += String(meanPitch_MultipleLayer(activeLayer));

        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 273:
    { // KEEP Syncop BUG
        printRot2(KeepSyncopBUG == true ? F("ON ") : F("OFF"), 16, 300, 3);
    }
    break;

    case 274:
        // REPLICATE Event
        {

            affiche = nulString;
            affiche = giveStringOfEvent(CurrentEvenement);

            affiche += F(" <- Evt");

            affiche += String(potVal2);

            printRot2(affiche, 16, 300, 3);
        }
        break;

    case 277:
    {
        // MIRROR Extended
        // va devoir indiquer le nb de notes présentes avant ce % là des layers

        affiche = nulString;
        affiche += String(potVal2);
        affiche += F("% = ");

        unsigned long foldtime;
        long calcultime; // doit parfois être négatif !!!!
        int total = 0;

        for (byte i = 0; i < nbLayer; i++)
        {

            if (isLayerActive(i))
            {

                calcultime = (((long)layerLength[i] * (long)potVal2) / (long)100);
                foldtime = (unsigned long)(calcultime);
                // on doit pouvoir se passer de la variable foldtime en fait !
                // affiche+=String(foldtime);

                unsigned int taille = mesureTaillePileComplete(pInitial[i]);
                pileNote tempNote;

                for (byte j = 0; j < taille; j++)
                { //
                    tempNote = ZEROgetNoteFromPile(pInitial[i], j);

                    if ((tempNote.note != 0) && (tempNote.startNote < foldtime))
                    {
                        total++;
                    }
                }
            }
        }

        affiche += String(total);
        affiche += F(" notes");

        printRot2(affiche, 16, 400, 3);
    };
    break;

        // {278,0,1000,0},  // Set Compteur d'Event
        // {279,0,1000,0},   // Set Latence
        // {280,0,1000,0},     // Set Echeance
        // {281,0,0,0},    // Switch Echeance ON/OFF

    case 278:
        // COMPTEUR DISPLAY+SET Event
        {

            affiche = nulString;
            affiche = giveStringOfEvent(CurrentEvenement);

            affiche += F(" [");
            affiche += String(Evenement[CurrentEvenement].compteur);
            affiche += F("] ->");

            affiche += String(potVal2);

            printRot2(affiche, 16, 300, 3);
        }
        break;

    case 279:
        // SET Latence Event
        {

            affiche = nulString;
            affiche = giveStringOfEvent(CurrentEvenement);

            affiche += F(" L:");

            affiche += String(potVal2);

            printRot2(affiche, 16, 300, 3);
        }
        break;

    case 280:
        // SET Echeance Event
        {

            affiche = nulString;
            affiche = giveStringOfEvent(CurrentEvenement);

            affiche += F(" E:");

            affiche += String(potVal2);

            printRot2(affiche, 16, 300, 3);
        }
        break;

    case 281:
    { // EVENT ON/OFF ECHEANCE
        affiche = nulString;
        affiche = giveStringOfEvent(CurrentEvenement);

        affiche += F(" ");

        if (Evenement[CurrentEvenement].fugace == true)
        {
            affiche += F("ON ");
        }
        else
        {
            affiche += F("OFF");
        }

        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 282:
        // LOAD EVENTS
        {

            currentFileNumber = softwareValueEncoderRight;

            affiche = nulString;
            ;

            affiche = String(currentFileNumber);
            affiche += F(".");
            NameFileNumber(currentFileNumber);
            affiche += name;

            printRot2(affiche, 16, 400, 3);
        }
        break;

        /////////////////////////////
        // BLACK 08
        /////////////////////////////

    case 283:
    { //
        affiche = afficheValueStraightOrFromSequence(potVal2, "%", 0);
        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 284:
    { // Numerote Transfo
        printRot2(numeroteTransfo == true ? F("ON ") : F("OFF"), 16, 300, 3);
    }
    break;

    case 286:
        // LOAD SEQUENCES
        {

            currentFileNumber = softwareValueEncoderRight;

            affiche = nulString;
            ;

            affiche = String(currentFileNumber);
            affiche += F(".");
            NameFileNumber(currentFileNumber);
            affiche += name;

            printRot2(affiche, 16, 400, 3);
        }
        break;

    case 288:
        // LOAD CCMOD
        {

            currentFileNumber = softwareValueEncoderRight;

            affiche = nulString;
            ;

            affiche = String(currentFileNumber);
            affiche += F(".");
            NameFileNumber(currentFileNumber);
            affiche += name;

            printRot2(affiche, 16, 400, 3);
        }
        break;

    case 290:
        // LOAD Cell
        {

            currentFileNumber = softwareValueEncoderRight;

            affiche = nulString;
            ;

            affiche = String(currentFileNumber);
            affiche += F(".");
            NameFileNumber(currentFileNumber);
            affiche += name;

            printRot2(affiche, 16, 400, 3);
        }
        break;

    case 291:
        // Rename File
        {

            currentFileNumber = softwareValueEncoderRight;

            affiche = nulString;
            ;

            affiche = String(currentFileNumber);
            affiche += F(".");
            NameFileNumber(currentFileNumber);
            affiche += name;
            affiche += F(" <- ");
            affiche += String153;
            affiche += F(".txt");

            printRot2(affiche, 16, 400, 3);
        }
        break;

    case 292:
        // Remove File
        {

            currentFileNumber = softwareValueEncoderRight;

            affiche = nulString;
            ;

            affiche = String(currentFileNumber);
            affiche += F(".");
            NameFileNumber(currentFileNumber);
            affiche += name;

            printRot2(affiche, 16, 400, 3);
        }
        break;

    case 295:
        // Velocity Ratio pour Octaver
        {
            affiche = nulString;
            ;
            affiche = afficheRatio(potVal2, 10, 10);
            printRot2(affiche, 16, 400, 3);
        }
        break;

    case 296:
    { // FUNKY VEL midi player
        affiche = nulString;

        affiche = afficheValueStraightOrFromSequence(potVal2, "vel", 0);
        affiche += F("  ");

        if (FunkyVel == true)
        {
            affiche += (F("ON "));
        }
        else
        {
            affiche += (F("OFF"));
        }
        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 297:
    { // ShiftR  Sequence
        affiche = nulString;
        affiche = giveStringOfSequence(potVal2);
        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 298:
    { // ShiftL  Sequence
        affiche = nulString;
        affiche = giveStringOfSequence(potVal2);
        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 299:
    { // Randomize  Sequence
        affiche = nulString;
        affiche = giveStringOfSequence(potVal2);
        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 300:
    { // Permute Sequence
        affiche = nulString;
        affiche = String(potVal2);
        if (potVal2 >= 0)
        {
            affiche = "+" + affiche;
        }

        affiche += F(" ");
        affiche += giveStringOfSequence(currentSequence);
        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 301:
    { // InvertV   Sequence
        affiche = nulString;
        affiche = giveStringOfSequence(potVal2);
        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 302:
    { // RevertH   Sequence
        affiche = nulString;
        affiche = giveStringOfSequence(potVal2);
        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 304:
    { // Double   Sequence
        affiche = nulString;
        affiche = giveStringOfSequence(potVal2);
        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 305:
    { // Define  Sequence pour Event
        affiche = nulString;
        affiche = giveStringOfSequence(potVal2);
        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 306:
    { // Soft Mute
        printRot2((softMute == true) ? F("ON ") : F("OFF"), 16, 300, 3);
    }
    break;

    case 303:
        // REPLICATE Sequence
        {

            affiche = nulString;
            // affiche = F( "Evt") ;
            // affiche += String(CurrentEvenement);
            affiche = F("Seq");
            affiche += String(currentSequence + 1);
            affiche += " <- Seq";
            affiche += String(potVal2 + 1);

            printRot2(affiche, 16, 300, 3);
        }
        break;

    case 307:
    { // LED Mode
        printRot2(eduLight == true ? F("Eduardo's mode") : F("Old Style"), 16, 300, 3);
    }
    break;

    case 310:
    { // BPM Sniffer
        // affiche =  nulString ;
        affiche = String(BPM);
        affiche += " ";
        if (compteurF8 % 24 == 0)
        {
            affiche += String(compteurF8);
        }
        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 312: // modif 2C12
    {         // Axe
        affiche = nulString;
        affiche = "axes : ";

        for (byte i = 0; i < nbLayer; i++)
        {
            if (hasSymmetricAxe(gammeActuelle[i], modeActuelle[i]))
            {
                int axe = 0;
                axe = getSymmetricAxe(gammeActuelle[i], modeActuelle[i]);
                affiche += String(axe);
                affiche += " ";
            }
        }
        affiche = affiche + F("quarts de tons ");
        printRot2(affiche, 16, 300, 3);
    }

    break;

    case 313:
    { // RealTransposerGamme
        affiche = nulString;

        for (byte i = 0; i < nbLayer; i++)
        {
            affiche += (GiveNameOfNote(gammeActuelle[i]));
            affiche += " ";
        }

        affiche += flechedroite;
        affiche += GiveNameOfNote(potVal2);

        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 314:
    { // Fix To Closest Random Scale and Mode

        affiche = nulString;
        for (byte i = 0; i < nbLayer; i++)
        {
            // if (isLayerActive(i))
            {
                affiche += (GiveNameOfNote(gammeActuelle[i]));
                affiche += GiveNameOfModeShort(modeActuelle[i]);
                affiche += " ";
            }
        }
        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 320: // RealModeTranspose
    {
        affiche = nulString;

        // affiche += GiveNameOfMode(potVal2);

        affiche = GiveNameOfMode(potVal2);
        affiche += "->";

        for (byte i = 0; i < nbLayer; i++)
        {
            affiche += (GiveNameOfNote(gammeActuelle[i]));
            affiche += GiveNameOfModeShort(modeActuelle[i]);
            affiche += " ";
        }

        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 326:
    {
        affiche = "CH" + String(potVal2);
        if (vel2Vol[potVal2 - 1] == false)
        {
            affiche += " OFF";
        }
        else
        {
            affiche += " ON ";
        }
        printRot2(affiche, 16, 400, 3);
    }
    break;

    // activation ON OFF des delay
    case 327:
    {

        // affiche = "Layer " + String(potVal2);
        // if (delayLayer[potVal2-1] == false)
        // {
        //     affiche += " OFF";
        // }
        // else
        // {
        //     affiche += " ON ";
        // }

        affiche = nulString;

        for (int i = 0; i < nbLayer; i++)
        {
            //affiche += String(i);
            if (delayLayer[i])
            {
                affiche += "ON  ";
            }
            else
            {
                affiche += "OFF ";
            }
        }
        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 328: // Affichage nb de délais - négatif = SEQ
    {
        affiche = nulString;
        if (potVal2 >= 0)
        {
            affiche = String(potVal2) + F(" echos ");
        }
        else
        {
            // choix de la séquence pour Kannon
            affiche = giveStringOfSequence(-potVal2 - 1);
        }
        printRot2(affiche, 15, 400, 3);
    }
    break;

        //////////////////////
        /// BLACK 10
        ///////////////////////

    case 329 :
    { // delay msec : black10 introduit une valeur négative ( prémonition )

        affiche = nulString;
        affiche = affiche + afficheValueStraightOrFromSequence(potVal2, "msec", -1000 );
        //  affiche = affiche + " > ";

        printRot2(affiche, 16, 300, 3);
    }
    break;

        /////////////////////////////
        // BLACK 08
        /////////////////////////////

    case 330 :
    { // Delay Drop Velocity 
        affiche = afficheValueStraightOrFromSequence(potVal2, "%vel", 0);
        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 332:
    { // Delay Random Percent 
        affiche = afficheValueStraightOrFromSequence(potVal2, "%", 0);
        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 333: // Repeater
    {
        affiche = nulString;

        if (potVal2 >= 0)
        {
            affiche = String(potVal2);
            affiche += "/32e ";
        }
        else
        {
            // de -37 à -57
            affiche = giveStringOfSequence(-potVal2 - 1);
        }

        printRot2(affiche, 15, 400, 3);
    }
    break;

    case 334: // BitMap
    {
        affiche = nulString;

        if (potVal2 >= 0)
        {
            int nbCases = 8;
            // int nombre = plusGrandNombreAvecAbits(nbCases);

            int i = potVal2;

            for (int j = 0; j < nbCases; j++)
            {
                int k = 0;
                k = (i >> j) & 1;
                affiche = String(k) + affiche; // fait croître devant donc !
            }
        }
        else
        {
            affiche = giveStringOfSequence(-potVal2 - 1);
        }
        printRot2(affiche, 15, 400, 3);
    }
    break;

    case 336:
    {
        // Condition : which comparison < > = pas égal
        affiche = nulString;
        ;

        switch (potVal2)
        {
        case 1:
            affiche += F(" 1.none");
            break;
        case 2:
            affiche += F(" 2.oldest");
            break;
        case 3:
            affiche += F(" 3.random");
            break;
        case 4:
            affiche += F(" 4.closest pitch");
            break;

        default:
            break;
        }
        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 340: // Layer(s) -> Mem(s)
    {
        // les layers sélectionnés vont séquentiellement être envoyés vers la mémoire en potVal2 et celles qui suivent
        // layerLength[potVal2 - 1 + compteur + 4] indique le numéro de pInitial qui y est lié
        // on voudra ici afficher pour la première mémoire sélectionnée
        // si contenu :
        // un signe occupé : *
        // le layer de la première note qui s'y trouve L1
        // si vide : ---

        int compteur = 0; // serait utile si on veut connaître 4 mémoires max !
        int memoireVisee = potVal2 - 1 + compteur + 4;
        unsigned int taille = mesureTaillePileComplete(pInitial[memoireVisee]);

        affiche = "L -> Mem";
        affiche += String(potVal2);

        if (taille > 0)
        {
            pileNote tempNote;
            tempNote = ZEROgetNoteFromPile(pInitial[memoireVisee], 0);
            byte layerMem = tempNote.layer;
            // affiche += "* L";
            affiche += "[";
            affiche += String(taille);
            affiche += "] ";
            affiche += String(layerMem + 1);
        }
        else
        {
            affiche += " empty";
        }
        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 341: //   Mem(s) -> Layer(s)
    {

        int compteur = 0; // serait utile si on veut connaître 4 mémoires max !
        int memoireVisee = potVal2 - 1 + compteur + 4;
        unsigned int taille = mesureTaillePileComplete(pInitial[memoireVisee]);

        affiche = "Mem";
        affiche += String(potVal2);

        if (taille > 0)
        {
            pileNote tempNote;
            tempNote = ZEROgetNoteFromPile(pInitial[memoireVisee], 0);
            byte layerMem = tempNote.layer;
            // affiche += "* L";
            affiche += "[";
            affiche += String(taille);
            affiche += "] ";
            affiche += String(layerMem + 1);
        }
        else
        {
            affiche += " empty ";
        }

        affiche += "->L";

        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 342:
    {

        affiche = "Copy Mem " + String(sourceMem);
        affiche += " to ";
        affiche += String(potVal2 - 1);

        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 346:
    {
        affiche = nulString;

        for (int i = 0; i < nbLayer; i++)
        {
            if (pitchableLayer[i])
            {
                affiche += "ON  ";
            }
            else
            {
                affiche += "OFF ";
            }
        }
        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 347:
    {
        affiche = "Channel " + String(potVal2);
        if (pitchableChannel[potVal2 - 1] == false)
        {
            affiche += " OFF";
        }
        else
        {
            affiche += " ON ";
        }
        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 348:
    {
        printRot2(eraseRecording == true ? F("ON ") : F("OFF"), 16, 300, 3);
    }
    break;

    case 349:
    { // DYNAMIC SWING
        affiche = "";

        for (byte i = 0; i < nbLayer; i++)
        {
            // déterminer si la layer i est active
            if (dynamicSwing[i] == true)
            {
                valueDynamicSwing[i] = potVal2;
                // affiche += String(valueDynamicSwing[i]);
                affiche += afficheValueStraightOrFromSequence(valueDynamicSwing[i], "%", 0);
                affiche += " ";
            }
            else
            {
                affiche += "-- ";
            }
        }

        affiche += "<-";
        affiche += afficheValueStraightOrFromSequence(potVal2, "%", 0);

        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 350:
    { // REC AUTOMATION

        if (recAutomation == true)
        {
            affiche = (F("ON  "));
        }
        else
        {
            affiche = (F("OFF "));
        }
        affiche += String(potVal2);
        affiche += "=step";

        printRot2(affiche, 16, 400, 3);
    }
    break;

    // activation ON OFF du barocco
    // essai d'un autre mode d'affichage pour ON/OFF
    case 351:
    {
        affiche = "";
        for (int i = 0; i < nbLayer; i++)
        {
            //affiche += String(i);
            if (baroccoLayer[i])
            {
                affiche += "ON  ";
            }
            else
            {
                affiche += "OFF ";
            }
        }
        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 352:
    {
        // chois de la séquence pour Barocco
        affiche = giveStringOfSequence(potVal2);
        printRot2(affiche, 16, 400, 3);
    }
    break;

        /////////////////////////////////////
        // BLACK08 : implémentation des SEQ must
        // 1 août 2021, rue Hespel, denrier jour des vacances
        /////////////////////////////////////

    case 354: // Drop Velocity
    {
        affiche = afficheValueStraightOrFromSequence(potVal2, "%", 0);
        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 355: // RAndom percent
    {
        affiche = afficheValueStraightOrFromSequence(potVal2, "%", 0);
        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 356: // Sym Horizontale
    {
        affiche = afficheValueStraightOrFromSequence(potVal2, "%", 0);
        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 357: // Sym verticale
    {
        affiche = afficheValueStraightOrFromSequence(potVal2, "%", 0);
        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 358: // Reflet
    {
        affiche = afficheValueStraightOrFromSequence(potVal2, "%", 0);
        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 359:
    {
        affiche = "CH";
        affiche += String(potVal2);
        if (panMicroSamplerOnChannel[potVal2 - 1] == false)
        {
            affiche += " OFF";
        }
        else
        {
            affiche += " ON ";
        }

        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 360:
    {
        affiche = "";

        for (byte i = 0; i < nbLayer; i++)
        {
            // déterminer si la layer i est active
            if (panMicroSamplerOnLayer[i] == true)
            {

                if (potVal2 >= 0)
                {
                    affiche += String(panMicroSampler[i]);
                    affiche += " ";
                }
                else
                {
                    // affiche = giveStringOfSequence(-potVal2 - 1);
                    affiche += "Sq";
                    affiche += String(-potVal2);
                    affiche += " ";
                }
            }
            else
            {
                affiche += "-- ";
            }
        }

        affiche += "<-";
        affiche += String(potVal2);

        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 361:
    {
        affiche = "CH";
        affiche += String(potVal2);
        if (pitchMicroSamplerOnChannel[potVal2 - 1] == false)
        {
            affiche += " OFF";
        }
        else
        {
            affiche += " ON ";
        }

        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 362:
    {
        affiche = "";

        for (byte i = 0; i < nbLayer; i++)
        {
            // déterminer si la layer i est active
            if (pitchMicroSamplerOnLayer[i] == true)
            {

                if (potVal2 >= 0)
                {
                    affiche += String(pitchMicroSampler[i]);
                    affiche += " ";
                }
                else
                {
                    // de -37 à -57
                    // affiche = giveStringOfSequence(-potVal2 - 1);
                    affiche += "Sq";
                    affiche += String(-potVal2);
                    affiche += " ";
                }
            }
            else
            {
                affiche += "-- ";
            }
        }

        affiche += "<-";
        affiche += String(potVal2);

        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 363:
    { //  ON OFF NEw MARKOV GLOBAL
        affiche = "";
        for (int i = 0; i < nbLayer; i++)
        {
            //affiche += String(i);
            if (participateNEWMarkov[i])
            {
                affiche += "ON  ";
            }
            else
            {
                affiche += "OFF ";
            }
        }

        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 364:
    { //  ON OFF NEw MARKOV ISOLATED LAYERZ

        affiche = "";
        for (int i = 0; i < nbLayer; i++)
        {
            //affiche += String(i);
            if (participateLAYERMarkov[i])
            {
                affiche += "ON  ";
            }
            else
            {
                affiche += "OFF ";
            }
        }

        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 365:
    {
        affiche = nulString;
        if (sendF8)
        {
            affiche += "ON  ";
        }
        else
        {
            affiche += "Off ";
        }

        affiche += String(potVal2);
        affiche += "beats  ";

        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 366: // Pre Reflet

    {
        affiche = afficheValueStraightOrFromSequence(potVal2, "%", 0);
        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 367: // Pre Horizontal
    {
        affiche = afficheValueStraightOrFromSequence(potVal2, "%", 0);
        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 368: // Pre Vertical
    {
        affiche = afficheValueStraightOrFromSequence(potVal2, "%", 0);
        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 369:
        // DERAZER : on sélectionne par layer une Seq
        {
            affiche = "";

            for (byte i = 0; i < nbLayer; i++)
            {
                // déterminer si la layer i est active
                if (derazerActif[i] == true)
                {

                    affiche += String(derazerSeqChoice[i] + 1);
                    affiche += " ";
                }
                else
                {
                    affiche += "- ";
                }
            }

            affiche += "<";
            affiche += String(potVal2 + 1);
            affiche += "Sq";

            printRot2(affiche, 16, 400, 3);
        }
        break;

    case 370:
    {
        affiche = "CH";
        affiche += String(potVal2);
        if (derazerOnChannel[potVal2 - 1] == false)
        {
            affiche += " OFF";
        }
        else
        {
            affiche += " ON ";
        }

        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 372:
    {
        affiche = ">";
        affiche += fileNameSave;
        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 373:
    {
        currentFileNumber = softwareValueEncoderRight;
        maximumSoftwareValueEncoderRight = rootFileCount;

        affiche = nulString;

        affiche = String(currentFileNumber);
        affiche += F(".");

        if (oldNumber != currentFileNumber)
        {
            NameFileNumber(currentFileNumber);
        }

        oldNumber = currentFileNumber;
        affiche += name;

        printRot2(affiche, 16, 400, 3);
        fileName = name;
    }
    break;

    case 374:
    { // Create Mask
        // old potVal2 mapping ;
        // old potVal2 mapping ;

        affiche = nulString;
        affiche = fileNameSave;
        affiche.replace(".txt", "");

        if (nowMillis % 1000 < 700)
        {
            affiche += (ChoixString153.substring(potVal2, potVal2 + 1));
        }
        else
        {
            affiche += F("*");
        }
        affiche += (F(" "));
        printRot2(affiche, 16, 300, 3);

        // NOTE**** trouver une fc réutilisable, du style : DisplayString (  &String153, &ChoixString153, potVal2 )
    }
    break;

    case 375: // OMIT END multi layer
    {         // Basé sur 360
        affiche = nulString;

        for (byte i = 0; i < nbLayer; i++)
        {
            // déterminer si la layer i est active
            if (omitEnd[i] == true)
            {

               // affiche += String(omitEndValue[i]);
               affiche += afficheValueStraightOrFromSequence(omitEndValue[i], "%", 0);
               affiche += "% ";
            }
            else
            {
                affiche += "-- ";
            }
        }

        affiche += "<-";
        affiche += afficheValueStraightOrFromSequence(potVal2, "%", 0);

        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 376:
        // FREEMEM
        {
        }

    case 377:
    { // Generate fileNameSave
        affiche = fileNameSave;
        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 379:
    {
        // TAP REC IN

        // nouvelle version remplaçant Rec Sentence
        // 1. TAP IN : layer devient énormément longue, style > 100h ( pas possible que tombe juste sur ce moment là )
        // 2. jouer les notes, qui rentrent ainsi bien dans la première boucle
        // 3. qd TAP IN again, calcul de longueur de layer, par rapport à zéro
        // 4. soit début de layer = moment du TAP IN #1, soit début de première note ( faire essai )
        // 5. décalage de tous les début et fin de notes par rapport à ce moment là

        if (statusTapRecIn == 0)
        {
            affiche = F(">>>OK TO START  ");
            // affiche += String(potVal2) ;
            // ici on pourrait intégrer le choix START TAP ou START NOTE
            printRot2(affiche, 16, 400, 3);
        }

        if (statusTapRecIn == 1)
        {
            affiche = F("REC >>>OK STOP");
            printRot2(affiche, 16, 400, 3);
        }
    };
    break;

    // activation ON OFF du Kannon
    case 381:
    {
        affiche = nulString;
        for (int i = 0; i < nbLayer; i++)
        {
            //affiche += String(i);
            if (kannonActif[i])
            {
                affiche += "ON  ";
            }
            else
            {
                affiche += "OFF ";
            }
        }
        printRot2(affiche, 16, 400, 3);
    }
    break;

    // pitch du Kannon
    case 383:
    {
        /*
        affiche = nulString;
        if (potVal2 >= -36)
        {
            affiche = String(potVal2) + "/2 tons";
        }
        else
        {
            // de -37 à -59
            affiche = giveStringOfSequence(-potVal2 - 37);
        }
        printRot2(affiche, 16, 400, 3);
*/

        affiche = afficheValueStraightOrFromSequence(potVal2, "/2 tons", -36);
        printRot2(affiche, 16, 400, 3);
    }
    break;

    // stretch du Kannon
    case 384:
    {
        affiche = nulString;
        if (potVal2 >= 0)
        {
            affiche = String(potVal2) + "/16";
        }
        else
        {
            // de -37 à -57
            affiche = giveStringOfSequence(-potVal2 - 1);
        }
        printRot2(affiche, 16, 400, 3);
    }
    break;

        ////////////////////////:::
        // BLACK08
        /////////////////////////

    case 385:
    {
        affiche = afficheValueStraightOrFromSequence(potVal2, "%", 0);
        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 386:
    {
        affiche = afficheValueStraightOrFromSequence(potVal2, "%", 0);
        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 387:
    {
        affiche = afficheValueStraightOrFromSequence(potVal2, "%", 0);
        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 532:
    {
        affiche = afficheValueStraightOrFromSequence(potVal2, "%", 0);
        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 389:
    { // Activate LFO

        affiche = nulString;
        affiche = "LFO";
        affiche += String(potVal2 + 1);
        affiche += " ";
        affiche += (LFO[potVal2].actif == true ? F("ON ") : F("OFF"));
        //  currentLFO = potVal2;
        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 390:
    { // choix du type d'onde

        affiche = nulString;
        affiche = "LFO";
        affiche += String(currentLFO + 1);
        affiche += ":";

        switch (potVal2)
        {
        case 0:
        {
            affiche += F("flat");
        }
        break;
        case 1:
        {
            affiche += F("SEQ??TODO");
        }
        break;
        case 2:
        {
            affiche += F("SIN");
        }
        break;
        case 3:
        {
            affiche += F("SQR");
        }
        break;
        case 4:
        {
            affiche += F("Sample & Hold ");
        }
        break;
        case 5:
        {
            affiche += F("SAW UP DOWN");
        }
        break;
        case 6:
        {
            affiche += F("RAMP UP");
        }
        break;
        case 7:
        {
            affiche += F("RAMP DOWN");
        }
        break;

        default:
        {
            affiche += F("??");
        }
        break;
        }

        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 391:
    { // Duration

        affiche = nulString;
        affiche = "LFO";
        affiche += String(currentLFO + 1);
        affiche += ":";

        float realPotVal2 = (float)(potVal2) / 10;
        affiche += String(realPotVal2);
        affiche += "sec";

        printRot2(affiche, 16, 300, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // CONWAY 2C28
        // 2 août 2020 - fête d'annif de Liv
        // modif du 392 selon modèle du 328 pour intégrer les Seq
        /////////////////////////////////////////////////////////////////////////////////////

    case 392: // Choix du CC, aussi en fc de machine !
        // modif 2C28 pour Seq
        {
            affiche = nulString;

            if (potVal2 < 0)
            {
                affiche = giveStringOfSequence(-potVal2 - 1);
            }
            else
            {

                // 0 = CC NORMAL
                if (LFO[currentLFO].typeMachine == 0)
                {
                    affiche = "CC";
                    affiche += String(potVal2);
                }

                // 1 = CC TX81Z
                if (LFO[currentLFO].typeMachine == 1)
                {
                    affiche = "TX";
                    affiche += String(potVal2);

                    if (potVal2 < 52)
                    {
                        byte subPotVal2 = potVal2 % 13;
                        byte operatorPotVal2 = 4 - (potVal2 / 13);
                        affiche += "OP";
                        affiche += String(operatorPotVal2);
                        affiche += "=";

                        switch (subPotVal2)
                        {
                        case 0:
                            affiche += "Attack Rate <32";
                            break;
                        case 1:
                            affiche += "Decay1 Rate <32";
                            break;
                        case 2:
                            affiche += "Decay2 Rate <32";
                            break;
                        case 3:
                            affiche += "Release Rate <16";
                            break;
                        case 4:
                            affiche += "Decay1 Level <16";
                            break;
                        case 5:
                            affiche += "Level Scaling <100";
                            break;
                        case 6:
                            affiche += "Rate Scaling <4";
                            break;
                        case 7:
                            affiche += "EG Bias Sens <8";
                            break;
                        case 8:
                            affiche += "Ampl Mod Enable <2";
                            break;
                        case 9:
                            affiche += "Key vel sens <8";
                            break;
                        case 10:
                            affiche += "Operator output lvl <100";
                            break;
                        case 11:
                            affiche += "CRS Frequency <64";
                            break;
                        case 12:
                            affiche += "Detune <7";
                            break;
                        }
                    }
                    else if (potVal2 >= 100 && potVal2 < 120)
                    {
                        byte subPotVal2 = (potVal2 - 100) % 5;
                        byte operatorPotVal2 = 4 - ((potVal2 - 100) / 5);
                        affiche += "OP";
                        affiche += String(operatorPotVal2);
                        affiche += "=";

                        switch (subPotVal2)
                        {
                        case 0:
                            affiche += "fixed Freq 0-1";
                            break;
                        case 1:
                            affiche += "fixed freq range 1-7";
                            break;
                        case 2:
                            affiche += "FIN freq range <16";
                            break;
                        case 3:
                            affiche += "OP WaveForm 0-7";
                            break;
                        case 4:
                            affiche += "EG Shift 0-3";
                            break;
                        }
                    }
                    else
                    {
                        switch (potVal2)
                        {
                        case 52:
                            affiche += "Algorithm <8";
                            break;
                        case 53:
                            affiche += "Feedback <8";
                            break;
                        case 54:
                            affiche += "LFO speed <100";
                            break;
                        case 55:
                            affiche += "LFO Delay <100";
                            break;
                        case 56:
                            affiche += "Pitch mod depth <100";
                            break;
                        case 57:
                            affiche += "Amp mod depth <100";
                            break;
                        case 58:
                            affiche += "LFO sync <2";
                            break;
                        case 59:
                            affiche += "LFO wave <4";
                            break;
                        case 60:
                            affiche += "pitch mod sens <8";
                            break;
                        case 61:
                            affiche += "amp mod sens <4";
                            break;
                        case 62:
                            affiche += "transpose 0-24-48";
                            break;
                        case 63:
                            affiche += "poly/mono 0-1";
                            break;
                        case 64:
                            affiche += "pitch bend range 0-12";
                            break;
                        case 65:
                            affiche += "porta mode 0-1";
                            break;
                        case 66:
                            affiche += "porta time <100";
                            break;
                        case 67:
                            affiche += "foot cont vol <100";
                            break;
                        case 68:
                            affiche += "sustain 0-1";
                            break;
                        case 69:
                            affiche += "portamento 0-1";
                            break;
                        case 70:
                            affiche += "chorus 0-1";
                            break;
                        case 71:
                            affiche += "mod wheel pitch <100";
                            break;
                        case 72:
                            affiche += "mod wheel amp <100";
                            break;
                        case 73:
                            affiche += "breath cont pitch <100";
                            break;
                        case 74:
                            affiche += "breath cont amp <100";
                            break;
                        case 75:
                            affiche += "breath cont pitch bias 0-50-99";
                            break;
                        case 76:
                            affiche += "breath cont EG bias <100";
                            break;

                        case 93:
                            affiche += "operator ON bits0-3";
                            break;

                        case 120:
                            affiche += "Reverb rate 0-7";
                            break;
                        case 121:
                            affiche += "foot controller pitch <100";
                            break;
                        case 122:
                            affiche += "foot controller amp < 100";
                            break;
                        }
                    }
                }

                // 2 = SHERMAN
                if (LFO[currentLFO].typeMachine == 2)
                {
                    affiche = "SH";
                    affiche += String(potVal2);

                    {
                        affiche += "=";

                        switch (potVal2)
                        {
                        case 0:
                            affiche += "---";
                            break;
                        case 1:
                            affiche += "INPUT";
                            break;
                        case 2:
                            affiche += "FREQ1";
                            break;
                        case 3:
                            affiche += "FM1";
                            break;
                        case 4:
                            affiche += "RESO1";
                            break;
                        case 5:
                            affiche += "ATTACK1";
                            break;
                        case 6:
                            affiche += "DECAY1";
                            break;
                        case 7:
                            affiche += "RELEASE1";
                            break;
                        case 8:
                            affiche += "FREQ2";
                            break;
                        case 9:
                            affiche += "AM2";
                            break;
                        case 10:
                            affiche += "RESO2";
                            break;
                        case 11:
                            affiche += "ATTACK2";
                            break;
                        case 12:
                            affiche += "RELEASE2";
                            break;
                        default:
                            affiche += "???";
                            break;
                        }
                    }
                }
            }

            // 3 = MICROSAMPLER

            printRot2(affiche, 16, 400, 3);
        }

    case 398: // DRUNK ON/OFF
    {
        affiche = nulString;
        for (int i = 0; i < nbLayer; i++)
        {
            //affiche += String(i);
            if (drunk[i])
            {
                affiche += "ON  ";
            }
            else
            {
                affiche += "OFF ";
            }
        }
        printRot2(affiche, 16, 400, 3);
    }
    break;

    ///////////////////////////
    // BLACK08 
    ////////////////////////////

case 399 :
{
    affiche = afficheValueStraightOrFromSequence(potVal2, "msec", -1000);
    printRot2(affiche, 16, 300, 3);
}
break ;

case 400:
{
    affiche = afficheValueStraightOrFromSequence(potVal2, "msec", -1000);
    printRot2(affiche, 16, 300, 3);
}
break;

case 401:
{
    affiche = afficheValueStraightOrFromSequence(potVal2, "msec",  0);
    printRot2(affiche, 16, 300, 3);
}
break;

case 402:
{
    affiche = nulString;

    for (int i = 0; i < nbLayer; i++)
    {
        if (isLayerActive(i))
        {
            affiche = affiche + GiveNameOfNote(potVal2);
            affiche += GiveNameOfModeShort(modeActuelle[i]);
            affiche += " ";
        }
        else
        {
            affiche = affiche + GiveNameOfNote(gammeActuelle[i]);
            affiche += GiveNameOfModeShort(modeActuelle[i]);
            affiche += " ";
        }
    }

    printRot2(affiche, 16, 400, 3);
    }
    break;

    case 408: // CC Multiplier
    {
        affiche = nulString;
        affiche = "CC x";
        affiche += String(potVal2);
        affiche += "/16";
        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 422: // choose Sort Mode
    {
        affiche = nulString;

        switch (potVal2)
        {
        case 0:
        {
            affiche = F("by Note ASC");
        }
        break;
        case 1:
        {
            affiche = F("by Note DESC");
        }
        break;
        case 2:
        {
            affiche = F("by velocity ASC");
        }
        break;
        case 3:
        {
            affiche = F("by velocity DESC");
        }
        break;
        case 4:
        {
            affiche = F("by start ASC");
        }
        break;
        case 5:
        {
            affiche = F("by start DESC");
        }
        break;
        case 6:
        {
            affiche = F("by end ASC");
        }
        break;
        case 7:
        {
            affiche = F("by end DESC");
        }
        break;
        case 8:
        {
            affiche = F("by duration ASC");
        }
        break;
        case 9:
        {
            affiche = F("by duration DESC");
        }
        break;
        case 10:
        {
            affiche = F("CHAOS");
        }
        break;
        default:
            break;
        }

        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 424: // STEPZ
    {
        // qd affichage de STEPZ, on doit lire les mute différemment, pour l'instant
        // l'affichage consiste à diviser le layer en nombre de Stepz par layer, et d'afficher la composition des substepz
        // on peut dans un premier temps afficher le nombre de notes par step
        // dans second temps on voudra savoir comment elles sont disposées
        // 3 0 0 1
        // 3* 0_ 0_ 1!     1! 1! 1? 1>

        // la modification de Stepz est dans le fichier SubParts, fonction CheckStepzButtons

        // le stepChannel de départ est défini par une fonction
        // mais si on modifie Ctrl-Enc Right, cette valeur va changer
// Alt-Enc Right : modifie la vélocité de note 

        if (softwareValueEncoderRightModulated[6] != oldCH)
        {
            stepChannel = softwareValueEncoderRightModulated[6];
            maximumSoftwareValueEncoderRightModulated[6] = 15;
            minimumSoftwareValueEncoderRightModulated[6] = 0;
        }

        // le stepVelocity de départ est défini par une fonction
        // mais si on modifie Alt-Enc Right, cette valeur va changer

        if (softwareValueEncoderRightModulated[5] != oldVelocity)
        {
            stepVelocity = softwareValueEncoderRightModulated[5];
            maximumSoftwareValueEncoderRightModulated[5] = 127;
            minimumSoftwareValueEncoderRightModulated[5] = 0;
        }

        switch (potVal2)
        {
        case 0:
        {
            affiche = "ACCENT";
            affiche += ",CH";
            affiche += String((stepChannel + 1));
        }
        break;

        case 128:
        {
            affiche = "TOTAL ACCENT ";
        }
        break;

        default:
        {
            affiche = String(potVal2);
            affiche += ",v";
            affiche += String(stepVelocity);
            affiche += ",CH";
            affiche += String((stepChannel + 1));
        }
        break;
        }

        oldCH = softwareValueEncoderRightModulated[6];
        oldVelocity = softwareValueEncoderRightModulated[5];

        currentStepzNote = potVal2; // on doit passer l'info au cas où on appuie sur le bouton STEPZ

        printRot2(affiche, 16, 400, 3);

        for (int i = 0; i < nbLayer; i++)
        {
            if (isLayerActive(i))
            {
afficheBoiteARythme(i, stepChannel,currentStepzNote); 
            }
        }
    }
    break;

    case 429: // Define Stop MODE pour STEPZ
    {
        // affiche = "MODE";
        // printRot2(affiche, 16, 400, 3);

        switch (potVal2)
        {
        case 0:
        {
            affiche = "0subStepz";
        }
        break;
        case 1:
        {
            affiche = "1stepz";
        }
        break;
        case 2:
        {
            affiche = "3=10msec";
        }
        break;
        case 3:
        {
            affiche = "4=nada";
        }
        break;
        default:
        {
            affiche = String(potVal2 - 3);
            affiche += "/32emes";
        }
        break;
        }

        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 433:
    { // GABBERISM : ratio multipliant les divisions, en dixièmes

        affiche = nulString;
        affiche = String(potVal2);
        affiche += F("/10 ");
        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 435:
    { // GABBERISM : ratio multipliant les repeats, en dixièmes

        affiche = nulString;
        affiche = String(potVal2);
        affiche += F("/10 ");
        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 441: // PUNTZ
    {
        // qd affichage de PUNTZ, on lit les stepz comme des valeurs de vélocité !
        // BLACK09 : n'a plus de sens avec Ctrl et Alt qui sont venus changer la donne...  
        // maintien ???  

        // la modification de Puntz est dans le fichier SubParts, fonction CheckPuntzButtons

        // affichage de la note choisie

        if (softwareValueEncoderRightModulated[6] != oldCH)
        {
            stepChannel = softwareValueEncoderRightModulated[6];
            maximumSoftwareValueEncoderRightModulated[6] = 15;
            minimumSoftwareValueEncoderRightModulated[6] = 0;
        }

        // le stepVelocity de départ est défini par une fonction
        // mais si on modifie Alt-Enc Right, cette valeur va changer

        if (softwareValueEncoderRightModulated[5] != oldVelocity)
        {
            stepVelocity = softwareValueEncoderRightModulated[5];
            maximumSoftwareValueEncoderRightModulated[5] = 127;
            minimumSoftwareValueEncoderRightModulated[5] = 0;
        }

        switch (potVal2)
        {
        case 0:
        {
            affiche = "ACCENT";
            affiche += ",CH";
            affiche += String((stepChannel + 1));
        }
        break;

        case 128:
        {
            affiche = "TOTAL ACCENT ";
        }
        break;

        default:
        {
            affiche = String(potVal2);
            affiche += ",v";
            affiche += String(stepVelocity);
            affiche += ",CH";
            affiche += String((stepChannel + 1));
        }
        break;
        }

        oldCH = softwareValueEncoderRightModulated[6];
        oldVelocity = softwareValueEncoderRightModulated[5];

        currentStepzNote = potVal2; // on doit passer l'info au cas où on appuie sur le bouton STEPZ

        printRot2(affiche, 16, 400, 3);

        for (int i = 0; i < nbLayer; i++)
        {

            if (isLayerActive(i))
            {
                afficheBoiteARythme(i, stepChannel, currentStepzNote);
            }
        }
    }
    break;

    case 442:
    { // définit la taille du Cut Slice

        affiche = nulString;
        affiche = F("1/");
        affiche += String(potVal2);
        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 443: //   Cut Slice Mem(s) -> Layer(s)
    {

        int compteur = 0; // serait utile si on veut connaître 4 mémoires max !
        int memoireVisee = potVal2 - 1 + compteur + 4;
        unsigned int taille = mesureTaillePileComplete(pInitial[memoireVisee]);

        affiche = "Mem";
        affiche += String(potVal2);

        if (taille > 0)
        {
            pileNote tempNote;
            tempNote = ZEROgetNoteFromPile(pInitial[memoireVisee], 0);
            byte layerMem = tempNote.layer;
            affiche += "*L";
            affiche += String(layerMem + 1);
        }
        else
        {
            affiche += " empty ";
        }

        affiche += "-> ";
        affiche += String(KcutSlice);
        affiche += "/";
        affiche += String(cutSlice);

        printRot2(affiche, 16, 400, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // BLACK 08 - rue Hespel
        // 28 juillet 2021
        // adapté pour inclure Seq
        ////////////////////////////////////////////////////////////////////////////////////

    case 444:
    {
        affiche = afficheValueStraightOrFromSequence(potVal2, " sec", 0);
        printRot2(affiche, 16, 400, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // BLACK 08 - rue Hespel
        // 28 juillet 2021
        // adapté pour inclure Seq
        ////////////////////////////////////////////////////////////////////////////////////

    case 445:
    {
        affiche = afficheValueStraightOrFromSequence(potVal2, " msec", 0);
        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 446: // Divide pour Dende
    {
        affiche = nulString;

        if (potVal2 >= 0)
        {
            affiche = String(potVal2);
        }
        else
        {
            affiche = giveStringOfSequence(-potVal2 - 1);
        }
        printRot2(affiche, 15, 400, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // BLACK 08 - rue Hespel
        // 28 juillet 2021
        // adapté pour inclure Seq
        ////////////////////////////////////////////////////////////////////////////////////

    case 447:
    {
        affiche = afficheValueStraightOrFromSequence(potVal2, " vel", 0);
        printRot2(affiche, 16, 400, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // BLACK 08 - rue Hespel
        // 28 juillet 2021
        // adapté pour inclure Seq
        ////////////////////////////////////////////////////////////////////////////////////

    case 448:
    {
        affiche = afficheValueStraightOrFromSequence(potVal2, " vel", 0);
        printRot2(affiche, 16, 400, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // BLACK 08 - rue Hespel
        // 28 juillet 2021
        // adapté pour inclure Seq
        ////////////////////////////////////////////////////////////////////////////////////

    case 449:
    {
        affiche = afficheValueStraightOrFromSequence(potVal2, " levels", 1);
        printRot2(affiche, 16, 400, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // BLACK 08 - rue Hespel
        // 28 juillet 2021
        // adapté pour inclure Seq
        ////////////////////////////////////////////////////////////////////////////////////

    case 450:
    {
        affiche = afficheValueStraightOrFromSequence(potVal2, " %", 0);
        printRot2(affiche, 16, 400, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // BLACK 08 - rue Hespel
        // 28 juillet 2021
        // adapté pour inclure Seq
        ////////////////////////////////////////////////////////////////////////////////////

    case 451:
    {
        affiche = afficheValueStraightOrFromSequence(potVal2, " echos", 0);
        printRot2(affiche, 16, 400, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // BLACK 08 - rue Hespel
        // 28 juillet 2021        // adapté pour inclure Seq
        ////////////////////////////////////////////////////////////////////////////////////

    case 452:
    {
        affiche = afficheValueStraightOrFromSequence(potVal2, " echos", 0);
        printRot2(affiche, 16, 400, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // BLACK 08 - rue Hespel
        // 28 juillet 2021        // adapté pour inclure Seq
        ////////////////////////////////////////////////////////////////////////////////////

    case 453:
    {
        affiche = afficheValueStraightOrFromSequence(potVal2, " divisions", 0);
        printRot2(affiche, 16, 400, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // BLACK 08 - rue Hespel
        // 28 juillet 2021        // adapté pour inclure Seq
        ////////////////////////////////////////////////////////////////////////////////////

    case 454:
    {
        affiche = afficheValueStraightOrFromSequence(potVal2, " %stretch", 0);
        printRot2(affiche, 16, 400, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // BLACK 08 - rue Hespel
        // 28 juillet 2021        // adapté pour inclure Seq
        ////////////////////////////////////////////////////////////////////////////////////

    case 455:
    {
        affiche = afficheValueStraightOrFromSequence(potVal2, " /2tons", -24);
        printRot2(affiche, 16, 400, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // BLACK 08 - rue Hespel
        // 28 juillet 2021        // adapté pour inclure Seq
        ////////////////////////////////////////////////////////////////////////////////////

    case 456: // Fidelity Dende
    {
        affiche = afficheValueStraightOrFromSequence(potVal2, " %", 0);
        printRot2(affiche, 16, 400, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // BLACK 08 - rue Hespel
        // 28 juillet 2021        // adapté pour inclure Seq
        ////////////////////////////////////////////////////////////////////////////////////

    case 458: // Stretch Dende
    {
        affiche = afficheValueStraightOrFromSequence(potVal2, " %", 0);
        printRot2(affiche, 16, 400, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // BLACK 08 - rue Hespel
        // 28 juillet 2021        // adapté pour inclure Seq
        ////////////////////////////////////////////////////////////////////////////////////

    case 461:
    {
        affiche = afficheValueStraightOrFromSequence(potVal2, " degres", -12);
        printRot2(affiche, 16, 400, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // BLACK 08 - rue Hespel
        // 28 juillet 2021        // adapté pour inclure Seq
        ////////////////////////////////////////////////////////////////////////////////////

    case 463:
    {
        affiche = afficheValueStraightOrFromSequence(potVal2, " degres", -12);
        printRot2(affiche, 16, 400, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // BLACK 08 - rue Hespel
        // 28 juillet 2021        // adapté pour inclure Seq
        ////////////////////////////////////////////////////////////////////////////////////

    case 464:
    {
        affiche = afficheValueStraightOrFromSequence(potVal2, " degres", -12);
        printRot2(affiche, 16, 400, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // BLACK 08 - rue Hespel
        // 28 juillet 2021        // adapté pour inclure Seq
        ////////////////////////////////////////////////////////////////////////////////////

    case 465:
    {
        // affiche = afficheValueStraightOrFromSequence(potVal2, " degres", -12);
        //  printRot2(affiche, 16, 400, 3);

        int floor = 0;

        affiche = nulString;
        if (potVal2 >= floor)
        {
            switch (potVal2)
            {
            case 0:
                affiche += "no OFF";
                break;
            case 1:
                affiche += "OFF fast ";
                break;
            case 2:
                affiche += "OFF beyond ";
                break;
            case 3:
                affiche += "bug OFF";
                break;
            default:
                affiche += "ERR";
                break;
            }
        }
        else
        {
            // de -37 à -59
            affiche = giveStringOfSequence(-potVal2 - floor - 1);
        }

        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 466:
    { //  ON OFF Dende

        affiche = "";
        for (int i = 0; i < nbLayer; i++)
        {
            //affiche += String(i);
            if (dende[i])
            {
                affiche += "ON  ";
            }
            else
            {
                affiche += "OFF ";
            }
        }

        printRot2(affiche, 16, 300, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // BLACK 08 - rue Hespel
        // 28 juillet 2021        // adapté pour inclure Seq
        ////////////////////////////////////////////////////////////////////////////////////

    case 467: // Fidelity Appear Dende
    {
        affiche = afficheValueStraightOrFromSequence(potVal2, " %", 0);
        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 468: // BitMap pour Dende
    {
        affiche = nulString;

        int floor = 0;

        if (potVal2 >= floor)
        {
            int nbCases = 8;
            // int nombre = plusGrandNombreAvecAbits(nbCases);

            int i = potVal2;

            for (int j = 0; j < nbCases; j++)
            {
                int k = 0;
                k = (i >> j) & 1;
                affiche = String(k) + affiche; // fait croître devant donc !
            }
        }
        else
        {
            affiche = giveStringOfSequence(-potVal2 - floor - 1);
        }
        printRot2(affiche, 15, 400, 3);
    }
    break;

    case 469: //   Add Slice Mem(s) -> Layer(s)
    {

        int compteur = 0; // serait utile si on veut connaître 4 mémoires max !
        int memoireVisee = potVal2 - 1 + compteur + 4;
        unsigned int taille = mesureTaillePileComplete(pInitial[memoireVisee]);

        affiche = "Mem";
        affiche += String(potVal2);

        if (taille > 0)
        {
            pileNote tempNote;
            tempNote = ZEROgetNoteFromPile(pInitial[memoireVisee], 0);
            byte layerMem = tempNote.layer;
            affiche += "*L";
            affiche += String(layerMem + 1);
        }
        else
        {
            affiche += " empty ";
        }

        affiche += "-> +";
        affiche += String(KcutSlice);
        affiche += "/";
        affiche += String(cutSlice);

        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 473:
    { // LFO data

        affiche = nulString;
        affiche = "LFO";
        affiche += String(potVal2 + 1);
        affiche += ":";
        affiche += (LFO[potVal2].actif == true ? F("ON ") : F("OFF"));
        affiche += " type";
        affiche += String(LFO[potVal2].type);
        affiche += " shape";
        affiche += String(LFO[potVal2].shape);
        affiche += " fid";
        affiche += String(LFO[potVal2].fidelite);
        affiche += " dur";
        affiche += String(LFO[potVal2].duration);
        affiche += " dec";
        affiche += String(LFO[potVal2].decalage);
        // affiche += " amp";
        // affiche += String(LFO[potVal2].amplitude);
        affiche += " lo";
        affiche += String(LFO[potVal2].lowest);
        affiche += "hi";
        affiche += String(LFO[potVal2].highest);
        // affiche += "step";
        // affiche += String(LFO[potVal2].stepValue);
        affiche += " ch";
        affiche += String(LFO[potVal2].channel);
        affiche += "CC";
        affiche += String(LFO[potVal2].which);
        affiche += " mach";
        affiche += String(LFO[potVal2].typeMachine);
        affiche += " lay";
        affiche += String(LFO[potVal2].layer);

        //  currentLFO = potVal2;
        printRot2(affiche, 16, 300, 3);
    }
    break;

        /*
  bool actif;          // rajouté en nov2018, sur le modèle de Sequence
    byte type;              // 0 = vide 1 = CC 2 = transfo : on va développer 1 pour l'instant, 2 sera pour plus tard, peut être
    byte shape;             // sin square Sample&Hold saw
    byte fidelite;          // normalement 100, mais on pourra avoir dégradation du signal
    unsigned long duration; // en millisecondes
    unsigned long decalage;
    int amplitude;
    int lowest; // je mets INT car je pense que ce ne sera pas que pour CC, mais aussi Transfo !!!
    int highest;
    int stepValue;  // augment de n à chaque pas.
    int compteur;   // indique la valeur actuelle
    byte direction; // 0 = UP  1 = DOWN

    byte channel;     // channel concerné 0-15
    int which;        // CC Value ou valeur de Transfo
    byte typeMachine; // 0 = normal : CC en 3 byte ; faudra spécifique pour microSampler, TX81Z, WavePad
    int valeur;       // valeur à jouer au moment où eventGOON arrive

    byte layer;              // layer concerné 1-4 ou combinaison ou random ; ou bien layers actives.   par défaut : Actif ; sinon spécifique
    unsigned long eventGOON; // indique à quel moment le prochain Evènement doit avoir lieu

*/

    case 459:
    {
        affiche = nulString;
        // 2C12 va afficher le gamme du layer 1 et transforme tout d'un coup

        for (byte i = 0; i < nbLayer; i++)
        {

            if (isLayerActive(i))
            {
                affiche += GiveNameOfNote(potVal2);
                affiche += " ";
            }
            else
            {
                affiche += GiveNameOfNote(dendeGamme[i]);
                affiche += " ";
            }
        }

        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 460:
    {
        affiche = nulString;
        // 2C12 va afficher le gamme du layer 1 et transforme tout d'un coup

        for (byte i = 0; i < nbLayer; i++)
        {
            if (isLayerActive(i))
            {
                affiche += GiveNameOfModeShort(potVal2);
                affiche += " ";
            }
            else
            {
                affiche += (GiveNameOfModeShort(dendeMode[i]));
                affiche += " ";
            }
        }

        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 477:
        // REPLICATE LFO
        {
            affiche = nulString;
            affiche = "LFO";
            affiche += String(currentLFO + 1); // traduire machine pour utilisateur

            affiche += F(" <- LFO");

            affiche += String(potVal2);

            printRot2(affiche, 16, 300, 3);
        }
        break;

    case 478: // Repeater LFO : détermine le temps
    {
        affiche = nulString;
        affiche = "LFO";
        affiche += String(currentLFO + 1);
        affiche += ":";

        if (potVal2 >= 0)
        {
            affiche += String(potVal2);
            affiche += "/32e ";
        }
        else
        {
            affiche += giveStringOfSequence(-potVal2 - 1);
        }

        printRot2(affiche, 15, 400, 3);
    }
    break;

    case 479: // Define Machine pour CC
    {
        affiche = "LFO";
        affiche += String(currentLFO + 1);
        affiche += ":";
        affiche += String(potVal2);

        switch (potVal2)
        {
        case (0):
        {
            affiche += F("CC normal");
        }
        break;

        case (1):
        {
            affiche += F("TX81Z");
        }
        break;

        case (2):
        {
            affiche += F("Sherman");
        }
        break;
        case (3):
        {
            affiche += F("KG MicroSampler");
        }
        break;

        default:
        {
            affiche = F("---");
        }
        break;
        }
        printRot2(affiche, 15, 400, 3);
    }
    break;

    case 480: // Define Machine Specific pour CC
    {
        affiche = "LFO ";

        if (level480 == 0)
        {
            affiche += "CH";
            affiche += String(potVal2 + 1);
            affiche += ":";
            switch (machineSpecific[potVal2])
            {
            case (0):
            {
                affiche += F("CC normal");
            }
            break;

            case (1):
            {
                affiche += F("TX81Z");
            }
            break;

            case (2):
            {
                affiche += F("Sherman");
            }
            break;
            case (3):
            {
                affiche += F("KG MicroSampler");
            }
            break;

            default:
            {
                affiche = F("---");
            }
            break;
            }
        }

        if (level480 == 1)
        {
            affiche += "CH";
            affiche += String(machineSpecificChannel + 1);
            affiche += ":";
            switch (potVal2)
            {
            case (0):
            {
                affiche += F("CC normal");
            }
            break;

            case (1):
            {
                affiche += F("TX81Z");
            }
            break;

            case (2):
            {
                affiche += F("Sherman");
            }
            break;
            case (3):
            {
                affiche += F("KG MicroSampler");
            }
            break;

            default:
            {
                affiche = F("---");
            }
            break;
            }
        }

        printRot2(affiche, 15, 400, 3);
    }
    break;

    case 482: // affichage msec pour twoPlets
    {
        affiche = nulString;

        if (potVal2 >= 0)
        {
            affiche += String(potVal2);
            affiche += "msec ";
        }
        else
        {
            affiche += giveStringOfSequence(-potVal2 - 1);
        }

        printRot2(affiche, 15, 400, 3);
    }
    break;

    case 483: // affichage 32èmes pour twoPlets
    {
        affiche = nulString;

        if (potVal2 >= 0)
        {
            affiche += String(potVal2);
            affiche += "/32e ";
        }
        else
        {
            affiche += giveStringOfSequence(-potVal2 - 1);
        }

        printRot2(affiche, 15, 400, 3);
    }
    break;

    case 484: // n-Plets
    {
        affiche = nulString;

        if (potVal2 >= 0)
        {
            affiche += String(potVal2);
            affiche += "-plets ";
        }
        else
        {
            affiche += giveStringOfSequence(-potVal2 - 1);
        }

        printRot2(affiche, 15, 400, 3);
    }
    break;

    case 485: // TAP BPM
    {
        printRot2(tapBPM == true ? F("ON ") : F("OFF"), 16, 300, 3);
    }
    break;

    case 486: // 32èmes pour Dende
    {
        affiche = nulString;

        if (potVal2 >= 0)
        {
            affiche = String(potVal2);
            affiche += "/32e ";
        }
        else
        {
            // de -37 à -57
            affiche = giveStringOfSequence(-potVal2 - 1);
        }

        printRot2(affiche, 15, 400, 3);
    }
    break;

    case 489:
    {
        // choix LFO Type 0=Void 1=CC 2=Tf

        switch (potVal2)
        {
        case 0:
            affiche = "void";
            break;
        case 1:
            affiche = "CC";
            break;
        case 2:
            affiche = "Tf";
            break;
        default:
            affiche = "???";
            break;
        }

        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 331: // DOWN pour Delay : ajout 2C19 de Sequences en négatif
    {
        affiche = nulString;

        if (potVal2 >= 0)
        {
            affiche = String(potVal2);
            affiche += "%";
        }
        else
        {
            // de -37 à -57
            affiche = giveStringOfSequence(-potVal2 - 1);
        }

        printRot2(affiche, 15, 400, 3);
    }
    break;

    case 491:
    {
        // potVal2 = analogRead(A1);

        switch (potVal2)
        {
        case 0:
            affiche = "Stop ALL Evts";
            break;
        case 1:
            affiche = "Start ALL Evts";
            break;
        case 2:
            affiche = "Rnd Start/Stop Evts";
            break;
        case 3:
            affiche = "invert Start/Stop Evts";
            break;
        default:
            break;
        }

        printRot2(affiche, 16, 400, 3);
    }
    break;

    case 492:
    { //  ON OFF Activate VARIATIONS // basé sur 244

        affiche = "";
        for (int i = 0; i < nbLayer; i++)
        {
            //affiche += String(i);
            if (variationLayer[i])
            {
                affiche += "ON  ";
            }
            else
            {
                affiche += "OFF ";
            }
        }

        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 496:
    { //  choix Mode Variation : 0 = dégressif 1 = constant

        affiche = "";
        for (int i = 0; i < nbLayer; i++)
        {
            //affiche += String(i);
            if (modeVariation[i] == 0)
            {
                affiche += "Dg ";
            }
            else
            {
                affiche += "Ct ";
            }
        }

        affiche += "<";
        if (potVal2 == 0)
        {
            affiche += "Deg";
        }
        else
        {
            affiche += "Cnt";
        }

        printRot2(affiche, 16, 300, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // THINNER 2C20
        /////////////////////////////////////////////////////////////////////////////////////

    case 498: // basé sur 473:
    {         // Event résumé - data

        affiche = "Evt";
        affiche += String(potVal2 + 1);
        affiche += ":";
        affiche += (Evenement[potVal2].actif == true ? F("ON ") : F("OFF"));
        affiche += " typ";
        affiche += String(Evenement[potVal2].type);
        // 0 = Vide ( il faut initialiser ! )   1 = CC
        // 2 = Transformation
        // (3) = Modification de valeur ( peut être intégrer à transformation ! )
        affiche += "int";
        affiche += String(Evenement[potVal2].intervalle);
        affiche += "mom";
        affiche += String(Evenement[potVal2].intervalle); // exprimé en 0.1 secondes  10 = 1 seconde 100 = 10 secondes 1000 = 100 secondes 10000 = quasi 1 h
        affiche += " fid";
        affiche += String(Evenement[potVal2].fidelite); // en %
        affiche += " typeInt";
        affiche += String(Evenement[potVal2].typeInterval); // 1 = constant 2 = random(a,b)  3=gaussian   4 = bi intervalle
        affiche += " wh";
        affiche += String(Evenement[potVal2].which);
        // si type 1 : quel CC
        // si type 2 : quelle transformation
        // si type 3 : quelle valeur ( comment définir ? liste dans tableau, ou via transformation ? paraît mieux )
        affiche += " val";
        affiche += String(Evenement[potVal2].valeur);
        affiche += " shp";
        affiche += String(Evenement[potVal2].shape);
        // 1 = valeur fixe ( pour valeur )
        // 2 = INC
        // 3 = DEC ( ou bien INC avec valeur négative ? )
        // 4 = RND  ( définir min et max, 0-127 par défaut pour CC )
        // 5 = LISTE : ex 4 valeurs 37,69,120,130
        //  5=liste sequential 6 = liste random
        // // NOTE**** il faudra pour la liste réapprendre les pointeurs; j'en aurai de toutes façons besoins pour passer
        // des fragments (bars) à des morceaux.
        affiche += "UP";
        affiche += String(Evenement[potVal2].rangeUP_); // pour option 2,3,4
                                                        // NOTE DEBUG : le préciser dans affichae, comme j'ai fait pour Seq, c'est bcp plus compréhensible
        affiche += "LO";
        affiche += String(Evenement[potVal2].rangeLO_); // pour option 2,3,4
        affiche += "+";
        affiche += String(Evenement[potVal2].incSize); // définit de combien la valeur en option 2 et 3 ( qui donnent le sens)
        affiche += " bhv";
        affiche += String(Evenement[potVal2].behaviorLimit); // pour options shape 2,3
        // 1 = Change direction
        // 2 = qd dépasse vers haut ou bas, réapparait de l'autre côté
        // 3 = reste à la limite
        affiche += " CH";
        affiche += String(Evenement[potVal2].channel); // channel concerné 0-15
        // voir le codage ligne 257 environ
        affiche += " L";
        affiche += String(Evenement[potVal2].layer); // layer concerné 1-4 ou combinaison ou random ; ou bien layers actives.   par défaut : Actif ; sinon spécifique
        affiche += " COND";
        affiche += (Evenement[potVal2].conditionActive == true ? F("ON ") : F("OFF"));

        if (Evenement[potVal2].conditionActive == true)
        {
            affiche += " test";
            affiche += String(Evenement[potVal2].whichTest);
            affiche += "op";
            affiche += String(Evenement[potVal2].whichOperation);
            // Which Operation 1% 2/ 3x 4%+ 5+
            affiche += "val";
            affiche += String(Evenement[potVal2].whichValue);
            affiche += "comp";
            affiche += String(Evenement[potVal2].whichComparison);
            affiche += "act";
            affiche += String(Evenement[potVal2].whichAction); // 1 DO 2.BLOCK
            affiche += "launch";
            affiche += String(Evenement[potVal2].launch); // utilisé pour ce qui est lié  à des notes ou layer modulo play
            affiche += "testL";
            affiche += String(Evenement[potVal2].whichLayer2test); //
        }
        affiche += "mod";
        affiche += String(Evenement[potVal2].modulo);
        affiche += "modK";
        affiche += String(Evenement[potVal2].moduloK);
        affiche += "modL";
        affiche += String(Evenement[potVal2].moduloLayers);
        affiche += "modMode";
        affiche += String(Evenement[potVal2].moduloMode);

        affiche += " ech";
        affiche += String(Evenement[potVal2].echeance); // decrit le nb de fois maximum qu'un evt peut avoir lieu
        affiche += "lat";
        affiche += String(Evenement[potVal2].latence); // décrit à partir de quand l'event aura réellement lieu
        affiche += "fug";
        affiche += (Evenement[potVal2].fugace == true ? F("ON ") : F("OFF"));
        // si YES, qd compteur arrive à échéance, l'evt est supprimé
        affiche += "SEQ";
        affiche += String(Evenement[potVal2].seqUsedAsList); // TODO DEBUG : mettre àjour initialisation, delete et save

        printRot2(affiche, 16, 300, 3);
    }
    break;

        /*

struct event
{
    bool actif; // rajouté en nov2018, sur le modèle de Sequence
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

    bool conditionActive;
    byte whichTest;
    byte whichOperation;
    // Which Operation 1% 2/ 3x 4%+ 5+
    byte whichValue;
    byte whichComparison;
    byte whichReference;
    byte whichAction;     // 1 DO 2.BLOCK
    bool launch;       // utilisé pour ce qui est lié  à des notes ou layer modulo play
    byte whichLayer2test; //

    byte modulo;
    byte moduloK;
    byte moduloLayers;
    byte moduloMode;

    int compteur; // sera un compteur qui décompte le nb de fois qu'un evt a lieu
    // doit pouvoir être testé
    int echeance;       // decrit le nb de fois maximum qu'un evt peut avoir lieu
    int latence;        // décrit à partir de quand l'event aura réellement lieu
    bool fugace;     // si YES, qd compteur arrive à échéance, l'evt est supprimé
    byte seqUsedAsList; // TODO DEBUG : mettre àjour initialisation, delete et save
*/

        /////////////////////////////////////////////////////////////////////////////////////
        // THINNER 2C20
        /////////////////////////////////////////////////////////////////////////////////////

    case 499: // basé sur 473:
    {         // Sequence résumé - data

        affiche = "Evt";
        affiche += String(potVal2 + 1);
        affiche += ":";

        affiche += (sequence[potVal2].actif == true ? F("ON ") : F("OFF"));

        // int tableausequence[tailleMaxTableauSequence];
        affiche += "siz";
        affiche += String(sequence[potVal2].tailleSeq);
        affiche += " mode";
        affiche += String(sequence[potVal2].modeSeq); // 0 FWD    1 BACK    2 RND    3 A-R    4 JITTER    5 RND-FRG

        affiche += "JIT";
        affiche += String(sequence[potVal2].jitterMinUP);
        affiche += "-";
        affiche += String(sequence[potVal2].jitterMaxUP);
        affiche += "-";
        affiche += String(sequence[potVal2].jitterMinDOWN);
        affiche + "-";
        affiche += String(sequence[potVal2].jitterMaxDOWN);
        affiche += "S";
        affiche += String(sequence[potVal2].jitterStaticMin);
        affiche += "-";
        affiche += String(sequence[potVal2].jitterStaticMax);

        affiche += " Rnd";
        affiche += String(sequence[potVal2].sizeRANDOMFRAG);
        affiche += "R";
        affiche += String(sequence[potVal2].repeatRANDOMFRAG);
        affiche += " ";
        affiche += (sequence[potVal2].compteLayer == true ? F("LAYER") : F("NOTES"));
        affiche += " ACT";
        affiche += String(sequence[potVal2].action); // 00 = CC    1 = NOte     2 = Velocite    plus tard : 3 = SWING
        // byte type ; // 0 FWD    1 BACK    2 RND    3 A-R    4 JITTER    5 RND-FRG est MODESEQ
        affiche += " CC";
        affiche += String(sequence[potVal2].CC);
        affiche += " CH";
        affiche += String(sequence[potVal2].channel);
        affiche += " L";
        affiche += String(sequence[potVal2].layer);

        printRot2(affiche, 16, 300, 3);
    }
    break;

        /*
int tableauSeq[tailleMaxTableauSequence];
    int tailleSeq;
    byte modeSeq; // 0 FWD    1 BACK    2 RND    3 A-R    4 JITTER    5 RND-FRG
    bool actif;

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
    bool beingStatic; // variables internes, qu'on ne modifie pas soi même
    // on pourrait coder les bool sur des bits NOTE DEBUG
    //
    byte sizeRANDOMFRAG;
    byte repeatRANDOMFRAG;
    byte toReachRANDOMFRAG; // variables internes, qu'on ne modifie pas soi même
    byte keepN;

    bool compteLayer; // si cette valeur est fausse il compte les notes
    // si elle est vraie, il compte les layers

    long compteur; // +1 à chaque itération, vaudra un grand nombre

    byte action; // 00 = CC    1 = NOte     2 = Velocite    plus tard : 3 = SWING
    // byte type ; // 0 FWD    1 BACK    2 RND    3 A-R    4 JITTER    5 RND-FRG est MODESEQ
    byte CC;
    byte channel;
    byte layer;
*/

        /////////////////////////////////////////////////////////////////////////////////////
        // THINNER 2C20
        /////////////////////////////////////////////////////////////////////////////////////

    case 500: // basé sur 473:
    {         // Event Actif Viewer

        affiche = "ActifsEvts:";
        for (byte i = 0; i < NbEvenementMax; i++)
        {
            if (Evenement[i].actif == true)
            {
                affiche += String(i + 1);
                affiche += " ";
            }
            if (Evenement[i].actif == false && Evenement[i].type != 0)
            {
                affiche += "(";
                affiche += String(i + 1);
                affiche += ") ";
            }
        }
        printRot2(affiche, 16, 300, 3);
    }

        /////////////////////////////////////////////////////////////////////////////////////
        // THINNER 2C20
        /////////////////////////////////////////////////////////////////////////////////////

    case 501: // basé sur 473:
    {         // Sequence Actif Viewer

        affiche = "ActifsSeq:";
        for (byte i = 0; i < nbSequencesMax; i++)
        {
            if (sequence[i].actif == true)
            {
                affiche += String(i + 1);
                affiche += " ";
            }

            if (sequence[i].actif == false && sequence[i].tailleSeq > 0)
            {
                affiche += "(";
                affiche += String(i + 1);
                affiche += ") ";
            }
        }
        printRot2(affiche, 16, 300, 3);
    }

        /////////////////////////////////////////////////////////////////////////////////////
        // THINNER 2C20
        /////////////////////////////////////////////////////////////////////////////////////

    case 502: // Fold
    {
        if (potVal2 >= 0)
        {
            affiche = String(potVal2) + "/64th";
        }
        else
        {
            // choix de la séquence pour Fold
            affiche = giveStringOfSequence(-potVal2 - 1);
        }

        printRot2(affiche, 16, 300, 3);
    }

        /////////////////////////////////////////////////////////////////////////////////////
        // THINNER 2C20
        /////////////////////////////////////////////////////////////////////////////////////

    case 503: // Fold Shorten
    {
        if (potVal2 >= 0)
        {
            affiche = String(potVal2) + "/64th";
        }
        else
        {
            // choix de la séquence pour Fold
            affiche = giveStringOfSequence(-potVal2 - 1);
        }

        printRot2(affiche, 16, 300, 3);
    }

        /////////////////////////////////////////////////////////////////////////////////////
        // THINNER 2C20
        /////////////////////////////////////////////////////////////////////////////////////

    case 504: // unFold
    {
        if (potVal2 >= 0)
        {
            affiche = String(potVal2) + "/64th";
        }
        else
        {
            // choix de la séquence pour Fold
            affiche = giveStringOfSequence(-potVal2 - 1);
        }

        printRot2(affiche, 16, 300, 3);
    }

        /////////////////////////////////////////////////////////////////////////////////////
        // THINNER 2C20
        /////////////////////////////////////////////////////////////////////////////////////

    case 505: // unFold Longer
    {
        if (potVal2 >= 0)
        {
            affiche = String(potVal2) + "/64th";
        }
        else
        {
            // choix de la séquence pour Fold
            affiche = giveStringOfSequence(-potVal2 - 1);
        }

        printRot2(affiche, 16, 300, 3);
    }

        /////////////////////////////////////////////////////////////////////////////////////
        // CORONA DAYZ 2C22 basé sur 373
        /////////////////////////////////////////////////////////////////////////////////////

    case 508:
    {
        currentFileNumber = softwareValueEncoderRight;

        affiche = String(currentFileNumber);
        affiche += F(".");
        NameFileNumber(currentFileNumber);
        affiche += name;

        printRot2(affiche, 16, 400, 3);
        // midiname = name ;
        fileName = name;
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // CORONA DAYZ 2C22 basé sur 340
        /////////////////////////////////////////////////////////////////////////////////////

    case 509: // Karakts -> K_Mem ; basé sur Layer(s) -> Mem(s)
    {
        // int compteur = 0; // serait utile si on veut connaître 4 mémoires max !
        int memoireVisee = potVal2 - 1;
        unsigned int taille = karakt_mesureTaillePileComplete(pKarakt_Header[memoireVisee]);

        affiche = "K -> Mem";
        affiche += String(potVal2);

        if (taille > 0)
        {
            // pileNote tempNote;
            // tempNote = ZEROgetNoteFromPile(pInitial[memoireVisee], 0);
            // byte layerMem = tempNote.layer;
            affiche += "[";
            affiche += String(taille);
            affiche += "] ";
        }
        else
        {
            affiche += " empty";
        }
        printRot2(affiche, 16, 400, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // CORONA DAYZ 2C22 basé sur 340
        /////////////////////////////////////////////////////////////////////////////////////

    case 510: // K_Mem -> Karakt
    {

        // int compteur = 0; // serait utile si on veut connaître 4 mémoires max !
        int memoireVisee = potVal2 - 1;
        unsigned int taille = karakt_mesureTaillePileComplete(pKarakt_Header[memoireVisee]);

        affiche = "KMem";
        affiche += String(potVal2);

        if (taille > 0)
        {
            affiche += "[";
            affiche += String(taille);
            affiche += "] ";
        }
        else
        {
            affiche += " empty ";
        }

        affiche += "->Karakt";

        printRot2(affiche, 16, 400, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // CORONA DAYZ 2C22 basé sur 340
        /////////////////////////////////////////////////////////////////////////////////////

    case 511: // delete K_Mem
    {
        // int compteur = 0; // serait utile si on veut connaître 4 mémoires max !
        int memoireVisee = potVal2 - 1;
        unsigned int taille = karakt_mesureTaillePileComplete(pKarakt_Header[memoireVisee]);

        affiche = "Del Mem";
        affiche += String(potVal2);

        if (taille > 0)
        {
            affiche += "[";
            affiche += String(taille);
            affiche += "] ";
        }
        else
        {
            affiche += " empty";
        }
        printRot2(affiche, 16, 400, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // CORONA DAYZ 2C23 basé sur 424
        /////////////////////////////////////////////////////////////////////////////////////

    case 512: // LAPIDZ, basé sur STEPZ
    {
        // l'affichage consiste à diviser le layer en nombre de Stepz par layer, et d'afficher la composition des substepz
        // on peut dans un premier temps afficher le nombre de notes par step
        // dans second temps on voudra savoir comment elles sont disposées
        // 3 0 0 1
        // 3* 0_ 0_ 1!     1! 1! 1? 1>

        // pour LAPIDZ, on ne va pas scroller à travers les notes d'un channel, mais les notes préexistantes au layer
        // la première fois qu'on l'affiche on va donc constituer un choix de notes, comme pour Variations
        // les autres fois, on va afficher la note sélectionnée

        int tailleVariations = 0 ;

// TODO ici réintroduire un système pour ne pas calculer tout ça à chaque itération ! 
        // if (Transfo[softwareValueEncoderLeft][0] == 512 && softwareValueEncoderLeft != oldCounterLeft)
        {
            //  firstTime512 = true;
            // constitution de la pile comme Variations

            // ZEROdeleteWholePile(pVariationsLapidz); 
// BLACK09 d'abord absent, puis installé, et enfin supprimé
// en effet, dans lapidz, on veut qu'il y ait mémoire des valeurs effacées
// pour pouvoir faire 0 0 0 0 0 0 0 0 et puis ranimer la flamme avec 0 1 0 0 0 0 0  p.ex.
// on peut par contre imaginer un shortcut pour supprimer, remettre à zéro lapidz

                for (int i = 0; i < nbLayer; i++)
            {
                if (isLayerActive(i))
                {
                    // if (variationLayer[i] == true)
                    {
                        unsigned int taille = mesureTaillePileComplete(pInitial[i]);

                        // parcours de 0 à t

                        for (unsigned int j = 0; j < taille; j++)
                        {
                            pileNote tempNote;
                            tempNote = ZEROgetNoteFromPile(pInitial[i], j);

                            // si t déjà présente dans VariationMem -> Nada
                            // sinon, ajouter note(t) avec channel à VaritionMem

                            if (tempNote.type == 0x90 || tempNote.type == 0x50)
                            {
                                if (checkNoteAChannelBinStepzPiles(tempNote.note, tempNote.channel) == true)
                                {
                                    // NADA
                                }
                                else
                                {
                                    // Ajout NOte à Variations
                                    ZEROaddNewNotetoPile(pVariationsLapidz, tempNote);
                                }
                            }
                        } // la pile Variations devrait contenir toutes les notes différentes

                        
                        //  int tailleVariationsInitiale = tailleVariations;

                        // après, calcul de tt, nombre de notes dans VariationMem

                        // byte percentDisparitionChanceVariation[nbLayer] = {0, 0, 0, 0};
                        // byte minimumNotesVariation[nbLayer] = {1, 1, 1, 1};
                        // byte maximumReduxVariation[nbLayer] = {1, 1, 1, 1};
                        // bool modeMinimumVariation[nbLayer] = {true, true, true, true};

                        // tant que (rnd Variation > %) && nbNotes dans VariationsMem > minmimum
                        // delete une note au hasard
                        // OU tant que (rnd Variation >%) && nbNotes > ( t - reduxMax )
                        // delete une note au hasard

                        // bien indiquer dans le playNote que si CH > 16, pas de playNote.
                    }
                }
            }

        } // SI N'EST PAS FIRST TIME qu'on tombe sur 512
       // else
        {
            //    firstTime512 = false;
        }

        tailleVariations = mesureTaillePileComplete(pVariationsLapidz);
       // Transfo[softwareValueEncoder][2] = tailleVariations; // indique le maximum pour potVal2

        Transfo[LineCallingTransfo(512)][2] = tailleVariations; // indique le maximum pour potVal2
maximumSoftwareValueEncoderRight = tailleVariations ; 

        // la modification de Stepz est dans le fichier SubParts, fonction CheckStepzButtons

        // affichage de la note choisie
        // affiche = nulString ;
        affiche = String(potVal2);
        affiche += "=";

        // afficher ici CH et Note de la vairationPile

        pileNote tempNote;
        tempNote = ZEROgetNoteFromPile(pVariationsLapidz, potVal2 - 1);

        affiche += " CH";
        affiche += String((tempNote.channel + 1));
        affiche += " N";
        affiche += String((tempNote.note));

        int BBB = tempNote.note;
        //  stepVelocity = tempNote.velocity
        stepChannel = tempNote.channel;
        currentStepzNote = tempNote.note; // on doit passer l'info au cas où on appuie sur le bouton STEPZ

        if (BBB == 0)
        {
            affiche = String(potVal2);
            affiche += "=";
            affiche += "ACCENT";
            affiche += ",CH";
            affiche += String((tempNote.channel + 1));
        }

        if (BBB == 128)
        {
            affiche = String(potVal2);
            affiche += "=";
            affiche += "TOTAL ACCENT";
            // affiche += ",CH";
            // affiche += String((tempNote.channel + 1));
        }

        printRot2(affiche, 16, 400, 3);

// on doit ici utiliser une procédure standart pour afficher 
// CHANNEL et NOTE, pour PAGE

        for (int i = 0; i < nbLayer; i++)
        {

            if (  isLayerActive(i))
            {
                afficheBoiteARythme(i,stepChannel,currentStepzNote);
            }

            }
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // CORONA DAYZ 2C23
        /////////////////////////////////////////////////////////////////////////////////////

    case 513: // keep STepzBUG
    {

        printRot2(keepStepzBUG == true ? affiche + F("ON ") : affiche + F("OFF"), 16, 300, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // CORONA DAYZ 2C23
        /////////////////////////////////////////////////////////////////////////////////////

    case 515: // ON OFF pour MuteChannel
    {
        affiche = String(potVal2) + ":";
        printRot2(((CHMute >> (byte)(potVal2 - 1)) & 1) == 0 ? affiche + F("ON   ") : affiche + F("MUTE"), 16, 300, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // CORONA DAYZ 2C23
        /////////////////////////////////////////////////////////////////////////////////////

    case 516: // mute ALL but ONE = CH solo
    {
        affiche = String(potVal2) + ":";
        printRot2(((CHMute >> ((byte)potVal2 - 1)) & 1) == 0 ? affiche + F("ON   ") : affiche + F("MUTE"), 16, 300, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // CORONA DAYZ 2C23
        /////////////////////////////////////////////////////////////////////////////////////

    case 517: // unmute ALL
    {
        affiche = String(potVal2) + ":";
        printRot2(((CHMute >> ((byte)potVal2 - 1)) & 1) == 0 ? affiche + F("ON   ") : affiche + F("MUTE"), 16, 300, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // CORONA DAYZ 2C23
        /////////////////////////////////////////////////////////////////////////////////////

    case 518:
    { // définit la taille k/n du Cut Slice

        affiche = nulString;
        affiche = String(potVal2);
        affiche += "/";
        affiche += String(cutSlice);
        printRot2(affiche, 16, 300, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // CORONA DAYZ 2C23
        /////////////////////////////////////////////////////////////////////////////////////

    case 519:
    { // Gabberito

        affiche = String(potVal2);
        affiche += F("repeats");
        printRot2(affiche, 16, 300, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // CONWAY 2C28
        // 20 juillet 2020
        /////////////////////////////////////////////////////////////////////////////////////

    case 521:
    { // Accent Min

        if (accentMinON == true)
        {
            affiche = "ON  ";
            accentMin = potVal2;
        }
        else
        {
            affiche = "OFF ";
        }

        affiche += String(potVal2);
        affiche += " [";
        affiche += String(accentMin);
        affiche += "-";
        affiche += String(accentMax);
        affiche += "]";
        printRot2(affiche, 16, 300, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // CONWAY 2C28
        // 20 juillet 2020
        /////////////////////////////////////////////////////////////////////////////////////

    case 522:
    { // Accent Max

        if (accentMaxON == true)
        {
            affiche = "ON  ";
            accentMax = potVal2;
        }
        else
        {
            affiche = "OFF ";
        }

        affiche += String(potVal2);
        affiche += " [";
        affiche += String(accentMin);
        affiche += "-";
        affiche += String(accentMax);
        affiche += "]";
        printRot2(affiche, 16, 300, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // 2C26 post hoc
        /////////////////////////////////////////////////////////////////////////////////////

    case 524:
    { // rec Flux
        affiche = "Rec to Mem ";
        affiche += String(potVal2 + 1);

        if (statusFluxRecIn == 0)
        {
            int memoireVisee = potVal2 + 4;
            unsigned int taille = mesureTaillePileComplete(pInitial[memoireVisee]);
            affiche += "[";
            affiche += String(taille);
            affiche += "]";
        }

        if (statusFluxRecIn == 1)
        {
            affiche += " R";

            unsigned int taille = mesureTaillePileComplete(pInitial[pFluxStart + currentFluxMem]);

            affiche += String(taille);
            affiche += " ";
        }

        printRot2(affiche, 16, 300, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // KARAKT FINISHER 2C25 - 8 may 2020
        /////////////////////////////////////////////////////////////////////////////////////

    case 525: // Gabberito 2 : distance
    {
        if (potVal2 >= 0)
        {
            affiche = String(potVal2) + " msec";
        }
        else
        {
            // choix de la séquence pour Gabberito 2
            affiche = giveStringOfSequence(-potVal2 - 1);
        }

        printRot2(affiche, 16, 300, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // KARAKT FINISHER 2C25 - 8 may 2020
        /////////////////////////////////////////////////////////////////////////////////////

    case 526: // Gabberito 2 : K
    {
        if (potVal2 >= 0)
        {
            affiche = "K=" + String(potVal2);
        }
        else
        {
            // choix de la séquence pour Gabberito 2
            affiche = giveStringOfSequence(-potVal2 - 1);
        }

        printRot2(affiche, 16, 300, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // KARAKT FINISHER 2C25 - 8 may 2020
        /////////////////////////////////////////////////////////////////////////////////////

    case 527: // Gabberito 2 : N
    {
        if (potVal2 >= 0)
        {
            affiche = "N=" + String(potVal2);
        }
        else
        {
            // choix de la séquence pour Gabberito 2
            affiche = giveStringOfSequence(-potVal2 - 1);
        }

        printRot2(affiche, 16, 300, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // 2C26 Bufflon2 - 21 may 2020
        /////////////////////////////////////////////////////////////////////////////////////

    case 529: // after 377:
    {         // Generate fileNameSave
        affiche = String(potVal2);
        affiche += " fgts=";
        affiche += fileName;
        printRot2(affiche, 16, 300, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // 2C26 Bufflon2 - 21 may 2020
        /////////////////////////////////////////////////////////////////////////////////////

    case 530: // after 377:
    {         // Generate fileNameSave
        affiche = String(fileName);
        printRot2(affiche, 16, 300, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // / 2C26 Bufflon2 - 31 may 2020
        /////////////////////////////////////////////////////////////////////////////////////

    case 533: // Kannon destination channel
    {
        affiche = "";

        if (potVal2 < 17)
        {
            affiche = "redirect -> CH";
        }
        else
        {
            affiche = "to CH idem";
        }

        affiche += afficheValueStraightOrFromSequence(potVal2, "", 1);
        printRot2(affiche, 16, 300, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // / 2C26 Bufflon2 - 31 may 2020
        /////////////////////////////////////////////////////////////////////////////////////

    case 534: // Kannon Bitmap - basé sur 334 BitMap
    {
        affiche = nulString;

        if (potVal2 >= 0)
        {
            int nbCases = 8;
            // ne palie donc pas tout à fait à la variété des modulo, car est au final toujours sur une base 8
            // int nombre = plusGrandNombreAvecAbits(nbCases);

            int i = potVal2;

            for (int j = 0; j < nbCases; j++)
            {
                int k = 0;
                k = (i >> j) & 1;
                affiche = String(k) + affiche; // fait croître devant donc !
            }
        }
        else
        {
            affiche = giveStringOfSequence(-potVal2 - 1);
            // devrait l'afficher en binaire ! TODO
        }
        printRot2(affiche, 15, 400, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // / 2C26 Bufflon2 - 1st June 2020
        /////////////////////////////////////////////////////////////////////////////////////

    case 535: // Tracker
    {
        affiche = nulString;

        affiche += giveStringOfSequence(potVal2);
        affiche += "->";

        for (int i = 0; i < nbLayer; i++)
        {
            //affiche += String(i);
            if (trackerActif[i])
            {
                affiche += "ON";
            }
            else
            {
                affiche += "OFF";
            }

            affiche += "[";
            affiche += String(trackerSeq[i] + 1);
            affiche += "] ";
        }

        printRot2(affiche, 15, 400, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // / 2C26 Bufflon2 - 7 June 2020
        /////////////////////////////////////////////////////////////////////////////////////

    case 536: // first free mem
    {
        affiche = nulString;

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
                }
                affiche += "M";
                affiche += String(i + 1);
                affiche += " ";
            }
        }

        // la vraie première est dans firstfree
        // d'ici on peut en faire une fonction

        printRot2(affiche, 15, 400, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // / 2C26 Bufflon2 - 7 June 2020
        // basé sur 536 et 509
        /////////////////////////////////////////////////////////////////////////////////////

    case 537: // first KMem free
    {
        // int compteur = 0; // serait utile si on veut connaître 4 mémoires max !

        affiche = nulString;

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
                }
                affiche += "K";
                affiche += String(i + 1);
                affiche += " ";
            }
        }
        printRot2(affiche, 15, 400, 3);
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
            affiche = nulString;
            affiche = ">";

            for (byte i = 0; i < nbLayer; i++)
            {
                if (isLayerActive(i))
                {
                    if (isThereFreeMem())
                    {
                        int target = giveFirstFreeMem();
                        affiche += "Mem";
                        affiche += String(target + 1);
                        affiche += " ";
                    }
                }
            }
            printRot2(affiche, 15, 400, 3);
        }
        break;

        /////////////////////////////////////////////////////////////////////////////////////
        // 2C26 BUFFLON2 - 7 June 2020
        // basé sur 509
        /////////////////////////////////////////////////////////////////////////////////////

    case 539: //  Sauve Karakt vesr K_Mem, first free

    {
        affiche = ">";
        if (isThereFreeKMem())
        {
            int target = giveFirstFreeKMem();
            affiche += "K";
            affiche += String(target + 1);
        }
        printRot2(affiche, 15, 400, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // 2C26 BUFFLON2 - 7 June 2020
        /////////////////////////////////////////////////////////////////////////////////////

    case 540: // Previous Load
    {
        affiche = nulString;
        affiche = "current =";
        affiche += String(currentMem + 1);
        affiche += " prev=";

        // int firstprevious = 100;

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

        affiche += String(index + 1);

        printRot2(affiche, 15, 400, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // 2C26 BUFFLON2 - 26 june 2020
        /////////////////////////////////////////////////////////////////////////////////////

    case 542: // affichage du piano Roll

    {
        // on va afficher la gamme actuelle du premier actif, sinon du 1
        int select = 0;
        for (byte i = 0; i < nbLayer; i++)
        {
            if (isLayerActive(i))
            {
                select = i;
                i = nbLayer; // dès qu'un positif, on interrompt donc
            }
        }

        byte i = select;

        String ligne1, ligne2;
        ligne1 = "";
        ligne2 = "";

        ligne1 += " "; //C
        ligne1 += (isNoteInScaleMode(1, gammeActuelle[i], modeActuelle[i]) ? "C" : ".");
        ligne1 += " "; //D
        ligne1 += (isNoteInScaleMode(3, gammeActuelle[i], modeActuelle[i]) ? "D" : ".");
        ligne1 += " "; //E
        ligne1 += " "; //F
        ligne1 += (isNoteInScaleMode(6, gammeActuelle[i], modeActuelle[i]) ? "F" : ".");
        ligne1 += " "; //G
        ligne1 += (isNoteInScaleMode(8, gammeActuelle[i], modeActuelle[i]) ? "G" : ".");
        ligne1 += " "; //A
        ligne1 += (isNoteInScaleMode(10, gammeActuelle[i], modeActuelle[i]) ? "A" : ".");
        ligne1 += " "; //B

        ligne2 += (isNoteInScaleMode(0, gammeActuelle[i], modeActuelle[i]) ? "C" : ".");
        ligne2 += " "; // C#
        ligne2 += (isNoteInScaleMode(2, gammeActuelle[i], modeActuelle[i]) ? "D" : ".");
        ligne2 += " "; // D#
        ligne2 += (isNoteInScaleMode(4, gammeActuelle[i], modeActuelle[i]) ? "E" : ".");
        ligne2 += (isNoteInScaleMode(5, gammeActuelle[i], modeActuelle[i]) ? "F" : ".");
        ligne2 += " "; // F#
        ligne2 += (isNoteInScaleMode(7, gammeActuelle[i], modeActuelle[i]) ? "G" : ".");
        ligne2 += " "; // G#
        ligne2 += (isNoteInScaleMode(9, gammeActuelle[i], modeActuelle[i]) ? "A" : ".");
        ligne2 += " "; // A#
        ligne2 += (isNoteInScaleMode(11, gammeActuelle[i], modeActuelle[i]) ? "B" : ".");

        buffer_setCursor(0, 2);
        buffer_print(ligne1);
        buffer_setCursor(0, 3);
        buffer_print(ligne2);
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

            {

                unsigned int taille = mesureTaillePileComplete(pInitial[i]);

                affiche += String(taille);
                affiche += " ";
            }
        }
        printRot2(affiche, 15, 400, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // 2C27 PolyRittim - 1 juillet 2020
        /////////////////////////////////////////////////////////////////////////////////////

    case 546: // numérateur time signature
    {
        affiche = String(potVal2);
        affiche += " ";
        affiche += " (";
        affiche += String(numerateurTimeSignature);
        affiche += "/";
        affiche += String(denominateurTimeSignature);
        affiche += ")";
        printRot2(affiche, 15, 400, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // 2C27 PolyRittim - 1 juillet 2020
        /////////////////////////////////////////////////////////////////////////////////////

    case 547: // dénominateur time signature
    {
        affiche = "/";
        affiche += String(potVal2);
        affiche += " ";
        affiche += " (";
        affiche += String(numerateurTimeSignature);
        affiche += "/";
        affiche += String(denominateurTimeSignature);
        affiche += ")";
        printRot2(affiche, 15, 400, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // 2C27 PolyRittim - 1 juillet 2020
        /////////////////////////////////////////////////////////////////////////////////////

    case 548: // POLYZ
    {
        // la modification de Stepz est dans le fichier SubParts, fonction CheckStepzButtons

        switch (potVal2)
        {
        case 0:
        {
            affiche = "ACCENT";
            affiche += ",CH";
            affiche += String((stepChannel + 1));
        }
        break;

        case 128:
        {
            affiche = "TOTAL ACCENT ";
        }
        break;

        default:
        {
            affiche = String(potVal2);
            affiche += ",v";
            affiche += String(stepVelocity);
            affiche += ",CH";
            affiche += String((stepChannel + 1));
        }
        break;
        }

        currentStepzNote = potVal2; // on doit passer l'info au cas où on appuie sur le bouton STEPZ

        printRot2(affiche, 16, 400, 3);

        for (int i = 0; i < nbLayer; i++)
        {
            if (isLayerActive(i))
            {
                // il faut afficher la division du layer
                //2C15 modifié par 2C27

                // 2C15 nombre de Stepz par layers
                // puis nombre de subStepz !
                // 2C17 nombre de PolyZ

                int nbCasesTotal = numerateurTimeSignature * denominateurTimeSignature;
                byte tableauCasesTotal[nbCasesTotal];

                nbStepz = 0;

                for (int j = 0; j < nbCasesTotal; j++)
                {
                    if ((j % numerateurTimeSignature == 0) || (j % denominateurTimeSignature == 0))
                    {
                        tableauCasesTotal[j] = 1;
                        nbStepz++;
                    }
                    else
                    {
                        tableauCasesTotal[j] = 0;
                    }
                    // on remplit les cases du tableau qui divisent num ou dénom, çàd les temps qui sont ON
                }

                //  byte tableauND2[nbCasesTotal]; // 2C28 utile pour comprendre démarche, mais pas utilisé
                byte tableauND2_Stepz[nbStepz];

                nbStepz = 0;

                for (int j = 0; j < nbCasesTotal; j++)
                {
                    if ((j % numerateurTimeSignature == 0) && (j % denominateurTimeSignature == 0))
                    {
                        // tableauND2[j] = 3;
                        tableauND2_Stepz[nbStepz] = 3;
                        nbStepz++;
                    }
                    else if (j % numerateurTimeSignature == 0)
                    {
                        // tableauND2[j] = 1;
                        tableauND2_Stepz[nbStepz] = 1;
                        nbStepz++;
                    }
                    else if (j % denominateurTimeSignature == 0)
                    {
                        // tableauND2[j] = 2;
                        tableauND2_Stepz[nbStepz] = 2;
                        nbStepz++;
                    }
                    else
                    {
                        // tableauND2[j] = 0;
                    }
                }

                // Serial.println(String(nbStepz));

                unsigned int nbCases = nbCasesTotal;
                unsigned long duree1subStep = layerLength[i] / (unsigned long)(nbCases);

                // on peut dès lors déterminer combien de notes dans chaque subStep
                // puis les regrouper par Step _ - = # 11 52

                byte tableauSubStepz[nbCases] = {0}; // tableau avec chaque valeur ON ou OFF du polyrythme
                byte tableauStepz[nbStepz] = {0};    // tableau avec le nombre de ON du polyrythme

                unsigned int taille = mesureTaillePileComplete(pInitial[i]);
                pileNote tempNote;

                // Attention, la note 0 est l'ACCENT !

                for (unsigned int j = 0; j < taille; j++)
                { //
                    tempNote = ZEROgetNoteFromPile(pInitial[i], j);
                    if (tempNote.note == potVal2 && tempNote.channel == stepChannel)
                    {
                        tableauSubStepz[tempNote.startNote / duree1subStep] = 1;
                    }

                    if (currentStepzNote == 128 && tempNote.note == 128) // cas particulier du TOTAL ACCENT
                    {
                        tableauSubStepz[tempNote.startNote / duree1subStep] = 1;
                    }
                }

                nbStepz = 0;
                for (int j = 0; j < nbCasesTotal; j++)
                {
                    if (tableauCasesTotal[j] == 1)
                    {
                        tableauStepz[nbStepz] = tableauSubStepz[j];
                        nbStepz++;
                    }
                }
                numberMaxPageStepz = (nbStepz - 1) / nbStepzAffiches;

                // for (unsigned int j = 0; j < nbStepz; j++)
                // {
                //     for (unsigned k = 0; k < nbSubStepz; k++)
                //     {
                //         tableauStepz[j] = tableauStepz[j] + (tableauSubStepz[j * nbSubStepz + k] << k);
                //     }
                // }

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

                printRot2(affiche, 16, 400, 5);

                // TODO DEBUG l'affichage n'est pas très pertinent quand plusieurs layers sont sélectionnés

                // faire affichage par Stepz avec nbNotes impliquées soit 111 -> 3 , soit 111 -> 8

                // affichage du déroulement du layerActive
                // unsigned int pos = (nowMillis % layerLength[i]) / (layerLength[i] / nbStepz);

                unsigned int pos = giveStepzForPolyZSubStepz(i);

                affiche = nulString;
                affiche = String(currentPageStepz);
                affiche += ":";
                String addit = "";

                for (unsigned int j = 0; j < nbStepzAffiches; j++)
                {
                    unsigned int k = (unsigned int)currentPageStepz * (unsigned int)nbStepzAffiches + j;

                    if (k < nbStepz)
                    {
                        if (k == pos)
                        {
                            affiche += String(tableauND2_Stepz[k]);
                        }
                        else
                        {
                            affiche += " ";
                        };
                    }
                }

                // for (unsigned int j = 0; j < nbStepzAffiches; j++)
                // {
                //     if (currentPageStepz * nbStepzAffiches + j < nbStepz)
                //     {
                //         if (currentPageStepz * nbStepzAffiches + j == pos )
                //         { affiche += "." ; }
                //         else
                //         {affiche += " "; }
                //     }
                // }

                // int startAff = currentPageStepz * nbStepzAffiches ;
                // affiche = affiche.substring(startAff, startAff + nbStepzAffiches) ;
                printRot2(affiche, 16, 400, 6);
            }
        }
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // 2C27 PolyRittim - 14 juillet 2020
        /////////////////////////////////////////////////////////////////////////////////////

    case 549: // infos midi
    {
        affiche = midiFileInfo;
        printRot2(affiche, 16, 400, 3);
        // printRot2("YES", 15, 400, 3);
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
            affiche = "Snd serial : ";
            affiche += (verboseSerial_sendToSerial == true ? F("ON ") : F("OFF"));
        }
        break;

        case 1:
        {
            affiche = "Snd LCD : ";
            affiche += (verboseSerial_sendToLCD == true ? F("ON ") : F("OFF"));
        }
        break;

        case 2:
        {
            affiche = "Snd CH : ";
            affiche += (verboseSerial_sendChannel == true ? F("ON ") : F("OFF"));
        }
        break;

        case 3:
        {
            affiche = "Snd Notes : ";
            affiche += (verboseSerial_sendNotes == true ? F("ON ") : F("OFF"));
        }
        break;

        case 4:
        {
            affiche = "Snd CC : ";
            affiche += (verboseSerial_sendCC == true ? F("ON ") : F("OFF"));
        }
        break;

        case 5:
        {
            affiche = "Snd ValueCC : ";
            affiche += (verboseSerial_valueCC == true ? F("ON ") : F("OFF"));
        }
        break;

        case 6:
        {
            affiche = "Snd Velocity : ";
            affiche += (verboseSerial_velocity == true ? F("ON ") : F("OFF"));
        }
        break;

        case 7:
        {
            affiche = "Snd String : ";
            affiche += (verboseSerial_string == true ? F("ON ") : F("OFF"));
        }
        break;

        case 8:
        {
            affiche = "timeStamp : ";
            affiche += (verboseSerial_timeStamp == true ? F("ON ") : F("OFF"));
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
        // 2C28 Conway - 17 juillet 2020
        /////////////////////////////////////////////////////////////////////////////////////

    case 556: //   Merge ADD, basé sur 341 Mem(s) -> Layer(s)
    {
        // int compteur = 0; // serait utile si on veut connaître 4 mémoires max !
        int memoireVisee = potVal2 - 1 + 4;
        unsigned int taille = mesureTaillePileComplete(pInitial[memoireVisee]);

        affiche = "Mem";
        affiche += String(potVal2);

        if (taille > 0)
        {
            pileNote tempNote;
            tempNote = ZEROgetNoteFromPile(pInitial[memoireVisee], 0);
            byte layerMem = tempNote.layer;
            // affiche += "* L";
            affiche += "[";
            affiche += String(taille);
            affiche += "] ";
            affiche += String(layerMem + 1);
        }
        else
        {
            affiche += " empty ";
        }

        affiche += "ADD to L";

        printRot2(affiche, 16, 400, 3);
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
            affiche = "1UP:";
        }
        break;

        case 1:
        {
            affiche = "1DN:";
        }
        break;

        case 2:
        {
            affiche = "2UP:";
        }
        break;

        case 3:
        {
            affiche = "2DN:";
        }
        break;

        case 4:
        {
            affiche = "3UP:";
        }
        break;

        case 5:
        {
            affiche = "3DN:";
        }
        break;

        case 6:
        {
            affiche = "4UP:";
        }
        break;

        case 7:
        {
            affiche = "4DN:";
        }
        break;
        case 8:
        {
            affiche = "5UP:";
        }
        break;

        case 9:
        {
            affiche = "5DN:";
        }
        break;

        case 10:
        {
            affiche = "6UP:";
        }
        break;

        case 11:
        {
            affiche = "6DN:";
        }
        break;

        case 12:
        {
            affiche = "7UP:";
        }
        break;

        default:
        {
            affiche = "";
        }
        break;
        }

        affiche += giveStringOfSequence(currentSequence);

        printRot2(affiche, 16, 400, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // CONWAY 2C28
        // 23 juillet 2020 - remplace 151 et 152
        /////////////////////////////////////////////////////////////////////////////////////

    case 560:
    { // midi speed
float RatioDeltaToMsec = 0 ;

        if (modifMidiSpeedON == true)
        {
            affiche = "ON  ";
            accentMin = potVal2;

            NumRatioDeltaToMsec = 1000;
            affiche += String(NumRatioDeltaToMsec) ;
            affiche+=  afficheValueStraightOrFromSequence(potVal2, "%", 0);

            DenomRatioDeltaToMsec = potVal2 ; 
            if ( DenomRatioDeltaToMsec == 0)
            {
                DenomRatioDeltaToMsec++;
            }
             RatioDeltaToMsec = (float)NumRatioDeltaToMsec / (float)DenomRatioDeltaToMsec;
        }
        else
        {
            affiche = "OFF ";
        }

        affiche += " 1000/";
        affiche += String(potVal2);

        affiche += F(" : ");
        affiche += String(RatioDeltaToMsec, 2);

        // buffer_clearLine(6);
        printRot2(affiche, 16, 300, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // CONWAY 2C28
        // 27 juillet 2020
        /////////////////////////////////////////////////////////////////////////////////////

    case 563:
    { // midi ratio multiplier

        affiche = "-";

        if (potVal2 > 0)
        {
            affiche = "x";
            affiche += String(potVal2);
        }

        if (potVal2 < 0)
        {
            affiche = "/";
            affiche += String(-potVal2);
        }

if ( rand()%100 == 0 )
{        simplifie(NumRatioDeltaToMsec, DenomRatioDeltaToMsec);
}

        float RatioDeltaToMsec =  (float) NumRatioDeltaToMsec / (float) DenomRatioDeltaToMsec;
        affiche += " > ";
        affiche += String(RatioDeltaToMsec, 2);

        printRot2(affiche, 16, 300, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // CONWAY 2C28
        // 28 juillet 2020
        /////////////////////////////////////////////////////////////////////////////////////

    case 564:
    { // turn ON OFF AutoHarmonizeLivePlaying
        printRot2(autoHarmonizeLivePlaying == true ? F("ON ") : F("OFF"), 16, 300, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // CONWAY 2C28
        // 28 juillet 2020
        /////////////////////////////////////////////////////////////////////////////////////

    case 565: // ROOT NOTE GAMME LIVE
    {
        // 2C12 va afficher le gamme du layer 1 et transforme tout d'un coup

        // 2C27
        affiche = GiveNameOfNote(potVal2);
        affiche += flechedroite;

        affiche += (GiveNameOfNote(gammeActuelleLive));
        affiche += " ";

        printRot2(affiche, 16, 400, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // CONWAY 2C28
        // 28 juillet 2020
        /////////////////////////////////////////////////////////////////////////////////////

    case 566:
    {
        affiche = nulString;

        affiche = GiveNameOfNote(gammeActuelleLive);

        affiche += GiveNameOfMode(potVal2);

        printRot2(affiche, 16, 400, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // CONWAY 2C28
        // 28 juillet 2020
        /////////////////////////////////////////////////////////////////////////////////////

    case 567: // basé sur dynHarmonize
    {

        affiche = nulString;

        switch (potVal2)
        {
        case 0:
            affiche += (F("1.+/- rnd"));
            break;
        case 1:
            affiche += (F("2.plus"));
            break;
        case 2:
            affiche += (F("3.minus"));
            break;
        case 3:
            affiche += (F("4.no Filter"));
            break;
        case 4:
            affiche += (F("5.block outside"));
            break;
        case 5:
            affiche += (F("6.block inside"));
            break;

        default:
            affiche += (F("--------- "));
            break;

            // 0 = + ou - 1 ( fluctue )
            // 1 = + 1
            // 2 = -1
            // 3 = aucun filtre
            // 4 = bloque
            // 5 = bloque inverse ( ne laisse ce qui n'en fait pas partie )
        }

        printRot2(affiche, 16, 300, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // BLACK08
        /////////////////////////////////////////////////////////////////////////////////////
    case 568:
    { // Delay Random Appear 
        affiche = afficheValueStraightOrFromSequence(potVal2, "% ", 0);
        printRot2(affiche, 16, 300, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // BLACK08
        /////////////////////////////////////////////////////////////////////////////////////
    case 569:
    {
        affiche = afficheValueStraightOrFromSequence(potVal2, "vel", 0);
        printRot2(affiche, 16, 300, 3);
    }
    break;
        /////////////////////////////////////////////////////////////////////////////////////
        // CONWAY 2C28
        // 2 août 2020
        /////////////////////////////////////////////////////////////////////////////////////

    case 570:
    { // sustain blocker
        // Très bien mais pour transfo et LFO il faudra créer un SET SustainBlock, qui ne soit pas ON OFF !
        // TODO

        if (sustainBlockerON == true)
        {
            affiche = "ON  ";
            sustainBlockLimit = potVal2;
        }
        else
        {
            affiche = "OFF ";
        }

        //        affiche += String(potVal2);
        //        affiche += " Sz";
        //        affiche += String(mesureTaillePileComplete(pSustainBlock));

        affiche = affiche + afficheValueStraightOrFromSequence(potVal2, "" , 1) + " sz" + String(mesureTaillePileComplete(pSustainBlock)) ;
        printRot2(affiche, 16, 300, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // CONWAY 2C28
        // 5 août 2020
        /////////////////////////////////////////////////////////////////////////////////////

    case 571:
    { // proxy pitcher, basé sur 570 sustain blocker

        if (proxyBlockerON == true)
        {
            affiche = "ON  ";
            proxyBlockLimit = potVal2;
        }
        else
        {
            affiche = "OFF ";
        }

        //affiche += String(potVal2);
        //affiche += " Sz";
        // affiche += String(mesureTaillePileComplete(pProxyBlock));

        //affiche += afficheValueStraightOrFromSequence(potVal2, " Sz" + String(mesureTaillePileComplete(pProxyBlock)), 1);
        affiche = affiche + afficheValueStraightOrFromSequence(potVal2, "", 1) + " sz" + String(mesureTaillePileComplete(pProxyBlock));

        printRot2(affiche, 16, 300, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // BLACK 08 - rue Hespel
        // 26 juillet 2021
        ////////////////////////////////////////////////////////////////////////////////////

    // Proximity Pitch : incluant SEQ
    case 572:
    {
        affiche = nulString;
        if (potVal2 >= -24)
        {
            affiche = String(potVal2) + "/2 tons";
        }
        else
        {
            // de -24 à ..
            affiche = giveStringOfSequence(-potVal2 - 24 - 1);
        }
        printRot2(affiche, 16, 400, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // CONWAY 2C28
        // 24 août 2020
        /////////////////////////////////////////////////////////////////////////////////////

    case 576:
    { // SHUT UP  ON/OFF
        affiche = (shutUP == true ? F("ON ") : F("OFF "));
        printRot2(affiche, 16, 300, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // RESIDENZ1 2C31
        // 5 decembre 2020
        ////////////////////////////////////////////////////////////////////////////////////

    case 577:
    { //
        affiche = nulString;
        affiche += "Seq";
        affiche += String(potVal2 + 1);
        affiche += "->";

        for (byte i = 0; i < nbLayer; i++)
        {
            // if (isLayerActive(i))
            {
                if (turnOnSequentialPostpone[i] == true)
                {
                    affiche += "Sq";
                    affiche += String(seqPostpone[i] + 1);
                    affiche += " ";
                }
                else
                {
                    affiche += "(Sq";
                    affiche += String(seqPostpone[i] + 1);
                    affiche += ")";
                }
                // affiche = giveStringOfSequence(potVal2);
            }
        }

        printRot2(affiche, 15, 300, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // BLACK 06 - rue Hespel
        // 23 juillet 2021
        ////////////////////////////////////////////////////////////////////////////////////

    case 578:
    { //
        String display = "";

        for (int i = 0; i < 50; i++)
        {
            int a = digitalRead(i);
            if ((a == 1) && (i < 22 || i > 27) && i != 15)
            // 22 à 27 étant les encodeurs rotatifs
            // et 15 le midi IN
            {
                display += String(i) + " ";
            }
            //   Serial.print(a);
            if (i % 5 == 0)
            {
                // Serial.print(" ");
            }
        }
        display += "  ";
        // oledWriteStringAtXY(display, 1, 2);
        display += String(synthesePressed, BIN);

        printRot2(display, 15, 300, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // BLACK 08 - rue Hespel
        // 26 juillet 2021
        ////////////////////////////////////////////////////////////////////////////////////

    case 580:
    {
        affiche = nulString;

        affiche += String(potVal2 % 175);
        affiche += " BPM x";
        affiche += String((potVal2 / 175) + 1);
        affiche += " bars";

        printRot2(affiche, 15, 300, 3);
    }
    break;

////////////////////////
// BLACK09 - bienfaiteurs lors du 2ème mercredi de garde 
////////////////////////

    case 581: // Tracker pour Karakt 
    {
        affiche = nulString;

        affiche += giveStringOfSequence(potVal2);
        affiche += "->";

        for (int i = 0; i < nbLayer; i++)
        {
            //affiche += String(i);
            if (trackerKaraktActif[i])
            {
                affiche += "ON";
            }
            else
            {
                affiche += "OFF";
            }

            affiche += "[";
            affiche += String(trackerKaraktSeq[i] + 1);
            affiche += "] ";
        }

        printRot2(affiche, 15, 400, 3);
    }
    break;

        ////////////////////////
        // BLACK09 - bienfaiteurs lors du 2ème mercredi de garde
        ////////////////////////

    case 582: // affichage du keyboard Type 
    {

        affiche = nulString;

        switch (potVal2)
        {
        case 0:
            affiche += (F("1.classic notes"));
            break;
        case 1:
            affiche += (F("2.stepz=WH vel=BL"));
            break;
        case 2:
            affiche += (F("3.padz=WH vel=BL"));
            break;
        case 3:
            affiche += (F("4.Mem Layer"));
            break;
        case 4:
            affiche += (F("5.Mem Karakt"));
            break;
        case 5:
            affiche += (F("6.launch CC string"));
            break;
        case 6:
            affiche += (F("7.one shot Layer "));
            break;
        case 7:
            affiche += (F("8.one shot Karakt"));
            break;
        case 8:
            affiche += (F("9.Lay=WH Kar=BL"));
            break;
        default:
            affiche += (F("--------- "));
            break;

            // 0 = + ou - 1 ( fluctue )
            // 1 = + 1
            // 2 = -1
            // 3 = aucun filtre
            // 4 = bloque
            // 5 = bloque inverse ( ne laisse ce qui n'en fait pas partie )
        }

        printRot2(affiche, 16, 300, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // BLACK 09 - rue Hespel
        // 23 août 2021
        ////////////////////////////////////////////////////////////////////////////////////

    case 583: // DMX send 
    {
        affiche = nulString;

  affiche += " Ch=";
        affiche += String(potVal2 / 256 );
      affiche += " val=" ; 
        affiche += String(potVal2 % 256); 

        printRot2(affiche, 15, 300, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // BLACK 09 - rue Hespel
        // 23 août 2021
        ////////////////////////////////////////////////////////////////////////////////////

    case 584: // DMX Auto Generate 
    {
        printRot2(DMXautoGenerate == true ? F("ON ") : F("OFF"), 15, 300, 3);
    }
    break;

        /////////////////////////////////////////////////////////////////////////////////////
        // BLACK 09 - rue Hespel
        // 23 août 2021
        ////////////////////////////////////////////////////////////////////////////////////

    case 585: // Ezy Multiplier
    {
        /*
on va afficher le nb de Stepz
le choix de multiplier 

qd on multipliera : nb Stepz multiplié
longueur de layer multiplié
multiplication des contenus 
        */

affiche = nulString ; 
affiche = String(nbStepz);
affiche += "x" ;
affiche += String(potVal2);
affiche += "=";

printRot2(affiche, 15, 300, 3);
    }
    break;

//////////////////////
/// BLACK 10
///////////////////////
    case 589: // DMX pinch 
    {
        affiche = nulString;

        affiche += " Ch=";
        affiche += String(potVal2 / 256);
        affiche += " val=";
        affiche += String(potVal2 % 256);

        printRot2(affiche, 15, 300, 3);
    }
    break;

        //////////////////////
        /// BLACK 10
        ///////////////////////

    case 590:
    { // vertical slide CC 
        affiche = afficheValueStraightOrFromSequence(potVal2, "demitons", 0);
        printRot2(affiche, 16, 300, 3);
    }
    break;

        //////////////////////
        /// BLACK 10
        ///////////////////////

    case 591:
    { // vertical slide CC
        affiche = afficheValueStraightOrFromSequence(potVal2, " msec", 0);
        printRot2(affiche, 16, 300, 3);
    }
    break;

        //////////////////////
        /// BLACK 10
        ///////////////////////

    case 592:
    { // vertical slide note 
        affiche = afficheValueStraightOrFromSequence(potVal2, "demitons", 0);
        printRot2(affiche, 16, 300, 3);
    }
    break;

        //////////////////////
        /// BLACK 10
        ///////////////////////

    case 593:
    { // vertical slide note
        affiche = afficheValueStraightOrFromSequence(potVal2, " msec", 0);
        printRot2(affiche, 16, 300, 3);
    }
    break;

        //////////////////////
        /// BLACK 10
        ///////////////////////

    case 595 : // gabbUltrino : distance
    {
        if (potVal2 >= 0)
        {
            affiche = String(potVal2) + " msec";
        }
        else
        {
            // choix de la séquence pour Gabberito 2
            affiche = giveStringOfSequence(-potVal2 - 1);
        }

        printRot2(affiche, 16, 300, 3);
    }
    break;

        //////////////////////
        /// BLACK 10
        ///////////////////////

    case 596 :
    { // set Sequence for gabbUltrino 

        affiche = nulString;

        affiche = giveStringOfSequence(potVal2);

        printRot2(affiche, 16, 300, 3);
    }
    break;

        //////////////////////
        /// BLACK 10
        ///////////////////////
    case 597 : // set to CH & CC 
    {
        affiche = nulString;

        affiche += "CH=";
        affiche += String(( potVal2 / 16 ) + 1 );
        affiche += " CC=";
        affiche += String(potVal2 % 127 );

        printRot2(affiche, 15, 300, 3);
    }
    break;

        //////////////////////
        /// BLACK 10
        ///////////////////////

    case 598 :
    { // prémonition, basé sur proxy pitcher, basé sur 570 sustain blocker
    // et 375

    affiche = nulString;
    affiche = affiche + afficheValueStraightOrFromSequence(potVal2, "", 1) ;
    affiche = affiche + " > ";

    for (byte i = 0; i < nbLayer; i++)
    {
        if (premonitionON[i] == true)
        {
           // affiche += premonitionValue[i] + " "; 
            affiche += afficheValueStraightOrFromSequence(premonitionValue[i], " ", 1);
        }
        else
        {
            affiche += "x ";
        }
    }

    printRot2(affiche, 16, 300, 3);
    }
    break;

        //////////////////////
        /// BLACK 10
        ///////////////////////

    case 599 :
    { // prémonition velocity

        affiche = nulString;
        affiche = affiche + afficheValueStraightOrFromSequence(potVal2, "", 1);
        affiche = affiche + " > ";

        for (byte i = 0; i < nbLayer; i++)
        {
            if (premonitionON[i] == true)
            {
                affiche += afficheValueStraightOrFromSequence(dropPourPremonition[i], "% ", 0);
            }
            else
            {
                affiche += "(";
                affiche += afficheValueStraightOrFromSequence(dropPourPremonition[i], "", 0);
                affiche += ")"; 
            }
        }

        printRot2(affiche, 16, 300, 3);
    }
    break;

        //////////////////////
        /// BLACK 10
        ///////////////////////

    case 600:
    { // prémonition random 

        affiche = nulString;
        affiche = affiche + afficheValueStraightOrFromSequence(potVal2, "", 1);
        affiche = affiche + " > ";

        for (byte i = 0; i < nbLayer; i++)
        {
            if (premonitionON[i] == true)
            {
                affiche += afficheValueStraightOrFromSequence(premonitionRandomAppear[i], "% ", 0);
            }
            else
            {
                affiche += "(";
                affiche += afficheValueStraightOrFromSequence(premonitionRandomAppear[i], "", 0);
                affiche += ")";
            }
        }

        printRot2(affiche, 16, 300, 3);
    }
    break;

        //////////////////////
        /// BLACK 10
        ///////////////////////

    case 601:
    { // stepz CC random 

        affiche = nulString;
        affiche = affiche + afficheValueStraightOrFromSequence(potVal2, "stepz ", 2);
        printRot2(affiche, 16, 300, 3);
    }
    break;

    case 602 :   // choose CH, CC 
    {
        affiche = nulString;
            int CH = ((potVal2) / 127 ) + 1 ;
            int CC = (potVal2 ) % 127;
        affiche = "CH" + String (CH) + " CC" + CC ; 
        printRot2(affiche, 16, 300, 3);
    }
    break ; 

    case 603: // generate ACID bassline 
    {
        affiche = nulString;
        affiche = String(potVal2) + " steps" ;
        printRot2(affiche, 16, 300, 3);
    }
    break ; 

        /////////////////////////////////////////////////////////////////////////////////////
        // FIN DES AFFICHAGES
        /////////////////////////////////////////////////////////////////////////////////////

    // FIN DES CASE d'AFFICHAGE
    default:
    {

    } // end of default
    break;

    } // end of switch avec toutes les valeurs de Transformatiosn

    if ((Transfo[LineCallingTransfo(thisTransfo)][3] & B0001) == 1)
    { // indiquer ici tous ceux qui ne sont pas spéciaux

        // potVal2 = analogRead(A1);
        // // old potVal2 mapping ;
        // potVal2 = map(counter2, 0, 1023, Transfo[counter  - 1][1] , Transfo[counter  - 1][2]  ); // dépendra de l'effet choisi.
        // créer une table de valeur à laquelle l'affichage, puis la procédure se réfereront.
        // qqes spécificités : signe à afficher % demi-tons, signes négatifs, notes

        if ((((Transfo[LineCallingTransfo(thisTransfo)][3]))) == TRANSFO_NomDeNOTE)
        {

            printRot2(String(potVal2) + F(" ") + GiveNameOfNote(potVal2) + F("(") + GiveFrenchNameOfNote(potVal2) + F(") "), 16, 400, 3);
        }

        else

        {

            switch (((Transfo[LineCallingTransfo(thisTransfo)][3])))
            {

                // le bit 1 (droite) décrit si est affichage NL ( min, max, symbole )
                // les bits 2, 3 et 4 (gauche) décrivent le type de symbole
                // 001 = %   010 = msec   011 = Layer   100 = Channel    101 = notes    110 = beats   111 = /2 tons
                // 1000 = la note xxx ( NOTE addition avril 2018 )
                // 1001 = ON/OFF

                // TODO DEBUG 2C03 : code pas du tout intuitif, à réécrire en utilisant les constantes TRANSFO_

            case TRANSFO_NO_VALUE:
            {
                affiche = nulString;
                affiche = String(potVal2);
                affiche += F(" ");
                printRot2(affiche, 16, 400, 3);
            }
            break;

            case TRANSFO_PERCENT:
            {
                affiche = nulString;
                affiche = String(potVal2);
                affiche += F("%");
                printRot2(affiche, 16, 400, 3);
            }
            break;

            case TRANSFO_SIMPLE_VALUE:
            {
                printRot2(String(potVal2), 16, 400, 3);
            }
            break;

            case TRANSFO_MSEC:
            {
                affiche = String(potVal2) + F(" msec ");
                if (showFract)
                {

                    if (firstActiveLayer() != 50)
                    {
                        fraction = closestFraction(potVal2, layerLength[firstActiveLayer()], 16, false);
                        if (fraction.imprecision < 0)
                        {
                            affiche = affiche + "<";
                        }
                        if (fraction.imprecision > 0)
                        {
                            affiche = affiche + ">";
                        }
                        if (fraction.imprecision == 0)
                        {
                            affiche = affiche + "=";
                        }

                        affiche = affiche + String(fraction.num);
                        affiche = affiche + F("/");
                        affiche = affiche + String(fraction.den);
                    }
                }
                printRot2(affiche, 15, 400, 3);
            }
            break;

            case TRANSFO_LAYER:
            {
                printRot2(String(potVal2) + F(" =layer "), 16, 400, 3);
            }
            break;

            case TRANSFO_CHANNEL:
            {
                printRot2(String(potVal2) + F(" =CH "), 16, 400, 3);
            }
            break;

            case TRANSFO_NbNOTES:
            {
                printRot2(String(potVal2) + F(" notes "), 16, 400, 3);
            }
            break;

            case TRANSFO_BEATS:
            {
                printRot2(String(potVal2) + F(" beats "), 16, 400, 3);
            }
            break;

            case TRANSFO_DEMITONS:
            {
                printRot2(String(potVal2) + F(" /2tons "), 16, 400, 3);
            }
            break;

            case TRANSFO_MSEC_SEQ:
            {
                affiche = nulString;
                if (potVal2 >= 0)
                {
                    affiche = String(potVal2) + F(" msec ");
                    if (showFract)
                    {

                        if (firstActiveLayer() != 50)
                        {
                            fraction = closestFraction(potVal2, layerLength[firstActiveLayer()], 16, false);
                            if (fraction.imprecision < 0)
                            {
                                affiche = affiche + "<";
                            }
                            if (fraction.imprecision > 0)
                            {
                                affiche = affiche + ">";
                            }
                            if (fraction.imprecision == 0)
                            {
                                affiche = affiche + "=";
                            }

                            affiche = affiche + String(fraction.num);
                            affiche = affiche + F("/");
                            affiche = affiche + String(fraction.den);
                        }
                    }
                }
                else
                {
                    // choix de la séquence pour Kannon
                    affiche = giveStringOfSequence(-potVal2 - 1);
                }
                printRot2(affiche, 15, 400, 3);
            }
            // NOTE**** On doit augmenter le type de possibilités
            default:
            {
            }
            break;
            }
        }
    }

} // fin de la procédure affichage Line2

void showActiveStatus()
{
    affiche = nulString;

    for (byte i = 0; i < nbLayer; i++)
    {
        // déterminer si la layer i est active
        if (isLayerActive(i))
        {

            affiche = affiche + String(i + 1);
        }
        else
        {

            affiche = affiche + F("-");
        }
    }

    printRot2(affiche, 4, 1000, 1);
    affiche = ""; 
}
