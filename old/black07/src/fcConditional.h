// groupe de fc qui vont venir teste les layers

/*
ABANDON     case 1 :   affiche+=F("1.Duree Layer") ; break ;    // pour 1 layer
OK  DONE    case 2 :   affiche+= F("2.Nb notes moy ") ; break ;
OK  DONE    case 3 :   affiche+= F("3.Highest pitch") ; break ;
OK  DONE    case 4 :  affiche+=  F("4.Lowest pitch") ; break ;
OK  DONE    case 5 :   affiche+=F("5.Moy pitch ") ; break ;
ABANDON --- case 6 :  affiche+= F("6.Pitch span") ; break ;
OK  DONE    case 7 :   affiche+=F("7.LGNotMin ") ; break ;
OK  DONE    case 8 :   affiche+=F("8.LGNotMax ") ; break ;
OK  DONE    case 9 :   affiche+=F("9.LGNotMoy ") ; break ;
OK  DONE    case 10 :  affiche+=F("10.Layer counter ") ; break ;
OK  DONE    case 11 :   affiche+=F("11.notes counter ") ; break ;
OK  DONE    case 12 :  affiche+= F("12.Time (sec)") ; break ;
OK  DONE    case 13 : affiche+= F("13. Time (min)") ; break ;
case 14 : affiche+= F("14. Slope") ; break ;
case 15 : affiche+= F("15. NtSpaceMin") ; break ;
case 16 : affiche+= F("16. NtSpaceMax") ; break ;
case 17 : affiche+= F("17. NtSpaceMoy ") ; break ;
case 18 : affiche+= F("18. demiTonsMin") ; break ;
case 19 : affiche+= F("19. demiTonsMax") ; break ;
case 20 : affiche+= F("20. demiTonsMoy") ; break ;
--- case 21 : affiche+= F("21. NbdeChannel") ; break ;
DONE    case 22 : affiche+= F("22. VelMin") ; break ;
DONE    case 23 : affiche+= F("23. VelMax") ; break ;
DONE    case 24 : affiche+= F("24. VelMoy") ; break ;
ABANDON      case 25 : affiche+= F("25. NbChannels") ; break ;
OK  DONE    case 26 : affiche+=F("26. DureeLayerMin") ; break ;
OK  DONE    case 27 : affiche+=F("26. DureeLayerMax") ; break ;
OK  DONE    case 28 : affiche+=F("26. DureeLayerMoy") ; break ;
OK  DONE    case 29 :   affiche+= F("2.Nb notes total ") ; break ;
DONE    case 30 : affiche+= F("30. MinPitchSpan") ; break ;
DONE    case 31 : affiche+= F("31. MaxPitchSpan") ; break ;
OK  DONE    case 32 : affiche+= F("32. MoyPitchSpan") ; break ;

*/

// 3 Highest Pitch

/**
 * @brief donne le pitch le plus élevé d'un layer
 * 
 */
byte highestPitch(byte i)
{
    byte Highnote = 0;

    unsigned int taille = mesureTaillePileComplete(pInitial[i]);
    pileNote tempNote;

    for (byte j = 0; j < taille; j++)
    {
        tempNote = ZEROgetNoteFromPile(pInitial[i], j);

        if (tempNote.note > Highnote)
        {
            Highnote = tempNote.note;
        }
    }

    return Highnote;
}

/**
 * @brief renvoie le pitch le plus élevé de plusieurs layers
 * 
 * @param thoseLayers layers qu'on interroge sous forme codée en bits ON et OFF 
 * @return byte renvoie le pitch le plus élevé
 */
byte highestPitch_MultipleLayer(byte thoseLayers)
{
    // on introduit ici ActiveLayer en argument

    byte temp = 0;

    for (byte i = 0; i < nbLayer; i++)
    {

        byte k = ((thoseLayers >> i) & B00000001);

        if (k == B00000001)

        {
            if (highestPitch(i) > temp)
            {
                temp = highestPitch(i);
            }
        }
    }

    return temp;
}

byte lowestPitch(byte i)
{
    byte Lownote = 0; // on part d'une valeur impossible
    // byte found = 0 ;

    unsigned int taille = mesureTaillePileComplete(pInitial[i]);
    pileNote tempNote;

    for (byte j = 0; j < taille; j++)
    {
        tempNote = ZEROgetNoteFromPile(pInitial[i], j);

        if ((j == 0) || (tempNote.note < Lownote))
        {
            Lownote = tempNote.note;
        }

        // 2C03 found n'est jmais modifié ! refonte de la fonction !
    }

    return Lownote;
}

byte lowestPitch_MultipleLayer(byte thoseLayers)
{
    // on introduit ici ActiveLayer en argument

    byte temp = 0;
    byte found = 0;

    for (byte i = 0; i < nbLayer; i++)
    {

        byte k = ((thoseLayers >> i) & B00000001);

        if (k == B00000001)

        {

            if (lowestPitch(i) < temp && lowestPitch(i) != 0 && found != 0)
            {
                temp = lowestPitch(i);
            }

            if (lowestPitch(i) != 0 && found == 0)
            {
                temp = lowestPitch(i);
                found = 1;
            }
        }
    }

    return temp;
}

byte meanPitch(byte i)
{
    unsigned long totalMean = 0;
    byte nbNotes = 0;

    unsigned int taille = mesureTaillePileComplete(pInitial[i]);
    pileNote tempNote;

    for (byte j = 0; j < taille; j++)
    {
        tempNote = ZEROgetNoteFromPile(pInitial[i], j);

        if (tempNote.note != 0)
        {
            totalMean += tempNote.note;
            nbNotes++;
        }
    }

    if (nbNotes == 0)
    {
        return 0;
    }

    totalMean = totalMean / nbNotes;
    return (byte)totalMean;
}

byte meanPitch_MultipleLayer(byte thoseLayer)
{
    unsigned long totalMean = 0;
    byte nbNotes = 0;

    for (byte i = 0; i < nbLayer; i++)
    {

        byte k = ((thoseLayer >> i) & B00000001);

        if (k == B00000001)
        {
            unsigned int taille = mesureTaillePileComplete(pInitial[i]);
            pileNote tempNote;

            for (byte j = 0; j < taille; j++)
            {
                tempNote = ZEROgetNoteFromPile(pInitial[i], j);

                if (tempNote.note != 0)
                {
                    totalMean += tempNote.note;
                    nbNotes++;
                }
            }
        }
    }

    if (nbNotes == 0)
    {
        return 0;
    }

    totalMean = totalMean / nbNotes;
    return (byte)totalMean;
}

// PITCH SPAN

byte moyPitchSpan_MultipleLayer(byte thoseLayer)
{
    unsigned long totalMean = 0;
    unsigned long totalMeanSpan = 0;
    byte nbNotes = 0;

    for (byte i = 0; i < nbLayer; i++)
    {
        byte k = ((thoseLayer >> i) & B00000001);
        if (k == B00000001)
        {
            unsigned int taille = mesureTaillePileComplete(pInitial[i]);
            pileNote tempNote;

            for (byte j = 0; j < taille; j++)
            {
                tempNote = ZEROgetNoteFromPile(pInitial[i], j);

                if (tempNote.note != 0)
                {
                    totalMean += tempNote.note;
                    nbNotes++;
                }
            }
        }
    }

    if (nbNotes == 0)
    {
        return 0;
    }

    totalMean = totalMean / nbNotes;
    nbNotes = 0;

    for (byte i = 0; i < nbLayer; i++)
    {
        byte k = ((thoseLayer >> i) & B00000001);
        if (k == B00000001)
        {
            unsigned int taille = mesureTaillePileComplete(pInitial[i]);
            pileNote tempNote;

            for (byte j = 0; j < taille; j++)
            {
                tempNote = ZEROgetNoteFromPile(pInitial[i], j);

                if (tempNote.note != 0)
                {
                    totalMeanSpan += abs((long)tempNote.note - (long)totalMean);
                    nbNotes++;
                }
            }
        }
    }

    if (nbNotes == 0)
    {
        return 0;
    }

    totalMeanSpan = totalMeanSpan / nbNotes;
    return totalMeanSpan;
}

byte minPitchSpan_MultipleLayer(byte thoseLayer)
{
    unsigned long totalMean = 0;
    // int totalMeanSpan = 0 ;
    byte nbNotes = 0;

    for (byte i = 0; i < nbLayer; i++)
    {
        byte k = ((thoseLayer >> i) & B00000001);
        if (k == B00000001)

        {
            unsigned int taille = mesureTaillePileComplete(pInitial[i]);
            pileNote tempNote;

            for (byte j = 0; j < taille; j++)
            {
                tempNote = ZEROgetNoteFromPile(pInitial[i], j);

                if (tempNote.note != 0)
                {
                    totalMean += tempNote.note;
                    nbNotes++;
                }
            }
        }
    }

    if (nbNotes == 0)
    {
        return 0;
    }

    totalMean = totalMean / nbNotes;
    nbNotes = 0;
    byte smallSpan = 0;
    byte found = 0;
    byte Span = 0;

    for (byte i = 0; i < nbLayer; i++)
    {
        byte k = ((thoseLayer >> i) & B00000001);
        if (k == B00000001)
        {
            unsigned int taille = mesureTaillePileComplete(pInitial[i]);
            pileNote tempNote;

            for (byte j = 0; j < taille; j++)
            {
                tempNote = ZEROgetNoteFromPile(pInitial[i], j);

                if (tempNote.note != 0 && found != 0)
                {
                    Span = abs((long)tempNote.note - (long)totalMean);
                    if (Span < smallSpan)
                    {
                        smallSpan = Span;
                    }
                }

                if (tempNote.note != 0 && found == 0)
                {
                    smallSpan = abs((long)tempNote.note - (long)totalMean);
                    found = 1;
                }
            }
        }
    }

    if (nbNotes == 0)
    {
        return 0;
    }

    return smallSpan;
}

byte maxPitchSpan_MultipleLayer(byte thoseLayer)
{
    unsigned long totalMean = 0;
    // int totalMeanSpan = 0 ;
    byte nbNotes = 0;

    for (byte i = 0; i < nbLayer; i++)
    {
        byte k = ((thoseLayer >> i) & B00000001);
        if (k == B00000001)
        {
            unsigned int taille = mesureTaillePileComplete(pInitial[i]);
            pileNote tempNote;

            for (byte j = 0; j < taille; j++)
            {
                tempNote = ZEROgetNoteFromPile(pInitial[i], j);

                if (tempNote.note != 0)
                {
                    totalMean += tempNote.note;
                    nbNotes++;
                }
            }
        }
    }

    if (nbNotes == 0)
    {
        return 0;
    }

    totalMean = totalMean / nbNotes;
    nbNotes = 0;
    byte smallSpan = 0;
    byte found = 0;
    byte Span = 0;

    for (byte i = 0; i < nbLayer; i++)
    {
        byte k = ((thoseLayer >> i) & B00000001);
        if (k == B00000001)
        {
            unsigned int taille = mesureTaillePileComplete(pInitial[i]);
            pileNote tempNote;

            for (byte j = 0; j < taille; j++)
            {
                tempNote = ZEROgetNoteFromPile(pInitial[i], j);

                if (tempNote.note != 0 && found != 0)
                {
                    Span = abs((long)tempNote.note - (long)totalMean);
                    if (Span > smallSpan)
                    {
                        smallSpan = Span;
                    }
                }

                if (tempNote.note != 0 && found == 0)
                {
                    smallSpan = abs((long)tempNote.note - (long)totalMean);
                    found = 1;
                }
            }
        }
    }

    if (nbNotes == 0)
    {
        return 0;
    }

    return smallSpan;
}
// VELOCITES

byte highestVelocite(byte i)
{

    byte Highnote = 0;

    unsigned int taille = mesureTaillePileComplete(pInitial[i]);
    pileNote tempNote;

    for (byte j = 0; j < taille; j++)
    {
        tempNote = ZEROgetNoteFromPile(pInitial[i], j);

        if (tempNote.velocity > Highnote && (tempNote.note != 0))
        {
            Highnote = tempNote.velocity;
        }
    }

    return Highnote;
}

byte highestVelocite_MultipleLayer(byte thoseLayers)
{
    // on introduit ici ActiveLayer en argument

    byte temp = 0;

    for (byte i = 0; i < nbLayer; i++)
    {

        byte k = ((thoseLayers >> i) & B00000001);

        if (k == B00000001)

        {
            if (highestVelocite(i) > temp)
            {
                temp = highestVelocite(i);
            }
        }
    }

    return temp;
}

byte lowestVelocite(byte i)
{
    byte Lownote = 0;
    byte found = 0;

    unsigned int taille = mesureTaillePileComplete(pInitial[i]);
    pileNote tempNote;

    for (byte j = 0; j < taille; j++)
    {
        tempNote = ZEROgetNoteFromPile(pInitial[i], j);

        if ((tempNote.velocity < Lownote) && tempNote.velocity != 0 && tempNote.note != 0 && found != 0)
        {
            Lownote = tempNote.velocity;
        }

        if (tempNote.velocity != 0 && tempNote.note != 0 && found == 0)
        {
            Lownote = tempNote.velocity;
            found = 1;
        }
    }

    return Lownote;
}

byte lowestVelocite_MultipleLayer(byte thoseLayers)
{
    // on introduit ici ActiveLayer en argument

    byte temp = 0;
    byte found = 0;

    for (byte i = 0; i < nbLayer; i++)
    {

        byte k = ((thoseLayers >> i) & B00000001);

        if (k == B00000001)

        {
            if (lowestVelocite(i) < temp && lowestVelocite(i) != 0 && found != 0)
            {
                temp = lowestVelocite(i);
            }

            if (lowestVelocite(i) != 0 && found == 0)
            {
                temp = lowestVelocite(i);
                found = 1;
            }
        }
    }

    return temp;
}

byte meanVelocite(byte i)
{
    unsigned long totalMean = 0;
    byte nbNotes = 0;

    unsigned int taille = mesureTaillePileComplete(pInitial[i]);
    pileNote tempNote;

    for (byte j = 0; j < taille; j++)
    {
        tempNote = ZEROgetNoteFromPile(pInitial[i], j);

        if (tempNote.velocity != 0 && (tempNote.note != 0))
        {
            totalMean += tempNote.velocity;
            nbNotes++;
        }
    }

    if (nbNotes == 0)
    {
        return 0;
    }

    totalMean = totalMean / nbNotes;
    return (byte)totalMean;
}

byte meanVelocite_MultipleLayer(byte thoseLayer)
{
    unsigned long totalMean = 0;
    byte nbNotes = 0;

    for (byte i = 0; i < nbLayer; i++)
    {

        byte k = ((thoseLayer >> i) & B00000001);

        if (k == B00000001)
        {
            unsigned int taille = mesureTaillePileComplete(pInitial[i]);
            pileNote tempNote;

            for (byte j = 0; j < taille; j++)
            {
                tempNote = ZEROgetNoteFromPile(pInitial[i], j);

                if (tempNote.velocity != 0 && (tempNote.note != 0))
                {
                    totalMean += tempNote.velocity;
                    nbNotes++;
                }
            }
        }
    }

    if (nbNotes == 0)
    {
        return 0;
    }

    totalMean = totalMean / nbNotes;
    return (byte)totalMean;
}

// LONGUEUR NOTES

unsigned int longestNote(byte i)
{

    unsigned long lengthNote = 0;

    unsigned int taille = mesureTaillePileComplete(pInitial[i]);
    pileNote tempNote;

    for (byte j = 0; j < taille; j++)
    {
        tempNote = ZEROgetNoteFromPile(pInitial[i], j);

        if (getDuree(tempNote) > lengthNote && (tempNote.note != 0))

        {
            lengthNote = getDuree(tempNote);
        }
    }

    return lengthNote;
}

unsigned int longestNote_MultipleLayer(byte thoseLayers)
{
    // on introduit ici ActiveLayer en argument

    unsigned int temp = 0;

    for (byte i = 0; i < nbLayer; i++)
    {

        byte k = ((thoseLayers >> i) & B00000001);

        if (k == B00000001)

        {
            if (longestNote(i) > temp)
            {
                temp = longestNote(i);
            }
        }
    }

    return temp;
}

unsigned int shortestNote(byte i)
{

    unsigned long lengthNote = 0;
    byte found = 0;

    unsigned int taille = mesureTaillePileComplete(pInitial[i]);
    pileNote tempNote;

    for (byte j = 0; j < taille; j++)
    {
        tempNote = ZEROgetNoteFromPile(pInitial[i], j);

        if (found > 0 && getDuree(tempNote) < lengthNote && (tempNote.note != 0))
        {
            lengthNote = getDuree(tempNote);
        }

        if (found == 0 && (tempNote.note != 0))
        {
            lengthNote = getDuree(tempNote);
            found = 1;
        }
        // TODO appliquer cette méthode aux autres LOWEST ...
    }

    return lengthNote;
}

unsigned int shortestNote_MultipleLayer(byte thoseLayers)
{
    // on introduit ici ActiveLayer en argument

    unsigned int temp = 0;
    byte found = 0;

    for (byte i = 0; i < nbLayer; i++)
    {

        byte k = ((thoseLayers >> i) & B00000001);

        if (k == B00000001)

        {
            if (shortestNote(i) < temp && shortestNote(i) != 0 && found > 0)
            {
                temp = shortestNote(i);
            }

            if (shortestNote(i) != 0 && found == 0)
            {
                temp = shortestNote(i);
                found = 1;
            }
        }
    }

    return temp;
}

unsigned int meanDurationNote(byte i)
{

    unsigned long totalMean = 0;
    byte nbNotes = 0;

    unsigned int taille = mesureTaillePileComplete(pInitial[i]);
    pileNote tempNote;

    for (byte j = 0; j < taille; j++)
    {
        tempNote = ZEROgetNoteFromPile(pInitial[i], j);

        if (tempNote.note != 0 && getDuree(tempNote) != 0)
        {
            totalMean += getDuree(tempNote);
            nbNotes++;
        }
    }

    if (nbNotes == 0)
    {
        return 0;
    }

    totalMean = totalMean / nbNotes;
    return (unsigned int)totalMean;
}

byte meanDuration_MultipleLayer(byte thoseLayer)
{
    unsigned long totalMean = 0;
    byte nbNotes = 0;

    for (byte i = 0; i < nbLayer; i++)
    {

        byte k = ((thoseLayer >> i) & B00000001);

        if (k == B00000001)
        {
            unsigned int taille = mesureTaillePileComplete(pInitial[i]);
            pileNote tempNote;

            for (byte j = 0; j < taille; j++)
            {
                tempNote = ZEROgetNoteFromPile(pInitial[i], j);

                if (tempNote.note != 0 && getDuree(tempNote) != 0)
                {
                    totalMean += getDuree(tempNote);
                    nbNotes++;
                }
            }
        }
    }

    if (nbNotes == 0)
    {
        return 0;
    }

    totalMean = totalMean / nbNotes;
    return (unsigned int)totalMean;
}

unsigned int layerCounter_MultipleLayer(byte thoseLayers)
{
    unsigned long totalMean = 0;
    byte nbNotes = 0;

    for (byte i = 0; i < nbLayer; i++)
    {

        byte k = ((thoseLayers >> i) & B00000001);

        if (k == B00000001)

        {
            totalMean += layerCounter[i];
            nbNotes++;
        }
    }

    if (nbNotes == 0)
    {
        return 0;
    }

    totalMean = totalMean / nbNotes;
    return totalMean;
}

unsigned long dureeLayer_MultipleLayer(byte thoseLayers)
{
    unsigned long totalMean = 0;
    byte nbNotes = 0;

    for (byte i = 0; i < nbLayer; i++)
    {

        byte k = ((thoseLayers >> i) & B00000001);

        if (k == B00000001)

        {
            totalMean += layerLength[i];
            nbNotes++;
        }
    }

    if (nbNotes == 0)
    {
        return 0;
    }

    totalMean = totalMean / nbNotes;
    return totalMean;
}

unsigned long dureeLayerMin_MultipleLayer(byte thoseLayers)
{
    unsigned long temp = 0;
    byte found = 0;

    for (byte i = 0; i < nbLayer; i++)
    {

        byte k = ((thoseLayers >> i) & B00000001);

        if (k == B00000001)
        {

            {
                if (layerLength[i] < temp && layerLength[i] != 0 && found > 0)
                {
                    temp = layerLength[i];
                }

                if (layerLength[i] != 0 && found == 0)
                {
                    temp = layerLength[i];
                    found = 1;
                }
            }
        }
    }

    return temp;
}

unsigned long dureeLayerMax_MultipleLayer(byte thoseLayers)
{
    unsigned long temp = 0;

    for (byte i = 0; i < nbLayer; i++)
    {

        byte k = ((thoseLayers >> i) & B00000001);

        if (k == B00000001)
        {

            {
                if (layerLength[i] > temp)
                {
                    temp = layerLength[i];
                }
            }
        }
    }

    return temp;
}

unsigned int nbNotesMoyen_MultipleLayer(byte thoseLayers)
{
    long totalMean = 0;
    byte nbNotes = 0;

    for (byte i = 0; i < nbLayer; i++)
    {

        byte k = ((thoseLayers >> i) & B00000001);

        if (k == B00000001)
        {
            unsigned int taille = mesureTaillePileComplete(pInitial[i]);
            pileNote tempNote;

            for (byte j = 0; j < taille; j++)
            {
                tempNote = ZEROgetNoteFromPile(pInitial[i], j);

                if (tempNote.note != 0)
                {
                    totalMean++;
                }
            }

            nbNotes++;
        }
    }

    if (nbNotes == 0)
    {
        return 0;
    }

    totalMean = totalMean / nbNotes;
    return totalMean;
}

unsigned int nbNotesTotal_MultipleLayer(byte thoseLayers)
{
    long totalMean = 0;
    byte nbNotes = 0;

    for (byte i = 0; i < nbLayer; i++)
    {

        byte k = ((thoseLayers >> i) & B00000001);

        if (k == B00000001)
        {
            unsigned int taille = mesureTaillePileComplete(pInitial[i]);
            pileNote tempNote;

            for (byte j = 0; j < taille; j++)
            {
                tempNote = ZEROgetNoteFromPile(pInitial[i], j);

                if (tempNote.note != 0)
                {
                    totalMean++;
                }
            }

            nbNotes++;
        }
    }

    return totalMean;
}

unsigned int nbStartNotesDifferents(byte i)
{

    // unsigned long totalDifferents = 0 ;

    unsigned int taille = mesureTaillePileComplete(pInitial[i]);

    if (taille == 0)
    {
        return 0;
    }
    if (taille == 1)
    {
        return 1;
    }

    pileNote tempNote;

    unsigned long tableauTailles[taille] = {0};
    unsigned int tailleFinale = taille; // exprimera la taille du tableau réduit, et donc le nombre de start différents

    for (byte j = 0; j < taille; j++)
    {
        tempNote = ZEROgetNoteFromPile(pInitial[i], j);
        tableauTailles[j] = tempNote.startNote;
    }

    for (unsigned int j = 0; j < tailleFinale; j++)
    {
        for (unsigned int p = j+1 ; p < tailleFinale ; p++)
        {
            if (tableauTailles[p] == tableauTailles[j])
            {
                for (unsigned int z = p; z < tailleFinale - 1; z++)
                {
                    tableauTailles[z] = tableauTailles[z + 1]; // on shifte toute la série
                }
                tailleFinale--; // et on réduit la taille
            }
        }
        // on a rempli le tableau avec le début de la note
    }

    return tailleFinale;
}

/**
 * @brief va renvoyer la distance entre 2 startNote la plus faible, non nulle
 * 
 * @param i layer
 * @return unsigned long 
 */
unsigned long shortestNonNulStartDistance(byte i)
{

    // unsigned long totalDifferents = 0 ;

    unsigned int taille = mesureTaillePileComplete(pInitial[i]);

    if (taille == 0)
    {
        return 0;
    }
    if (taille == 1)
    {
        return 0;
    }

    pileNote tempNote;

    unsigned long tableauTailles[taille] = {0};
    // unsigned int tailleFinale = taille ; // exprimera la taille du tableau réduit, et donc le nombre de start différents

    for (unsigned int j = 0; j < taille; j++)
    {
        tempNote = ZEROgetNoteFromPile(pInitial[i], j);
        tableauTailles[j] = tempNote.startNote;
    }

    unsigned long minimum = 0;

    for (unsigned int  j = 0; j < taille-1; j++)
    {
        for (unsigned int z = j+1; z < taille; z++)
        {
            unsigned long delta;
            // delta = abs((signed long)((signed long)tableauTailles[j] - (signed long)tableauTailles[z]));
delta = distanceEntre2StartSimple(tableauTailles[j],tableauTailles[z],layerLength[i]) ; 
            
            if ( minimum == 0 && delta > 0 )
            {
                minimum = delta ; 
            }

            if (delta != 0 && delta < minimum)
            {
                minimum = delta;
            }
        }
    }

    return minimum;
}

unsigned int nbNotesAtDistance(byte i, unsigned long distance)
{
    unsigned int taille = mesureTaillePileComplete(pInitial[i]);

    if (taille == 0)
    {
        return 0;
    }
    if (taille == 1)
    {
        return 0;
    }

    int nbNotesFound = 0;

    pileNote tempNote;

    unsigned long tableauTailles[taille] = {0};
    // unsigned int tailleFinale = taille ; // exprimera la taille du tableau réduit, et donc le nombre de start différents

    for (unsigned int j = 0; j < taille; j++)
    {
        tempNote = ZEROgetNoteFromPile(pInitial[i], j);
        tableauTailles[j] = tempNote.startNote;
    }

    // unsigned long minimum = 0;
    bool foundNote = false;

    for (unsigned int j = 0; j < taille ; j++)
    {
        foundNote = false;

        for (unsigned int z = 0 ; z < taille; z++)
        {
            unsigned long delta;
            // delta = abs((signed long)((signed long)tableauTailles[j] - (signed long)tableauTailles[z]));
          //  	long dist = distanceEntre2StartSimple(tempNote2.startNote,centerTime,layerLength[i]); 
			// 			if ( dist < potVal2)
            delta = distanceEntre2StartSimple(tableauTailles[j],tableauTailles[z],layerLength[i]) ; 
            if (delta == distance)
            {
                foundNote = true;
            }
        }

        if (foundNote == true) // çàd cette note est au moins 1x à la bonne distance d'une autre
        {
            nbNotesFound++;
        }
    }

    return nbNotesFound;
}


unsigned int giveNbOfNthNoteAtDistance(byte i, int n, unsigned long distance)
{
    unsigned int taille = mesureTaillePileComplete(pInitial[i]);

    if (taille == 0)
    {
        return 0;
    }
    if (taille == 1)
    {
        return 0;
    }

    int nbNotesFound = 0;

    pileNote tempNote;

    unsigned long tableauTailles[taille] = {0};
    // unsigned int tailleFinale = taille ; // exprimera la taille du tableau réduit, et donc le nombre de start différents

    for (unsigned int j = 0; j < taille; j++)
    {
        tempNote = ZEROgetNoteFromPile(pInitial[i], j);
        tableauTailles[j] = tempNote.startNote;
    }

    // unsigned long minimum = 0;
    bool foundNote = false;

    for (unsigned int j = 0; j < taille ; j++)
    {
        foundNote = false;

// int targetZ ; 

        for (unsigned int z = 0 ; z < taille; z++)
        {
            unsigned long delta;
            // delta = abs((signed long)((signed long)tableauTailles[j] - (signed long)tableauTailles[z]));
            delta = distanceEntre2StartSimple(tableauTailles[j],tableauTailles[z],layerLength[i]) ; 
            
            if (delta == distance )
            {
                foundNote = true;
              //  targetZ = z; 
            }
        }

        if (foundNote == true && nbNotesFound == n ) // çàd cette note est au moins 1x à la bonne distance d'une autre
        {
            return j ; 
        }

        if (foundNote == true ) // çàd cette note est au moins 1x à la bonne distance d'une autre
        {
            nbNotesFound++;
        }
    }

    return 0;
}

long resultatTest(byte n, byte thoseLayers)
{

    switch (n)
    {
    case 2:
    {
        return nbNotesMoyen_MultipleLayer(thoseLayers);
    }
    break;
    case 3:
    {
        return highestPitch_MultipleLayer(thoseLayers);
    }
    break;
    case 4:
    {
        return lowestPitch_MultipleLayer(thoseLayers);
    }
    break;
    case 5:
    {
        return meanPitch_MultipleLayer(thoseLayers);
    }
    break;

    case 7:
    {
        return shortestNote_MultipleLayer(thoseLayers);
    }
    break;
    case 8:
    {
        return longestNote_MultipleLayer(thoseLayers);
    }
    break;
    case 9:
    {
        return meanDuration_MultipleLayer(thoseLayers);
    }
    break;

    case 10:
    {
        return layerCounter_MultipleLayer(thoseLayers);
    }
    break;
    case 11:
    {
        return TotalNoteCounter;
    }
    break;
    // pour l'instant je restreins au nombre total de notes, plus tard, sans doute affiner vers Total par layer ?
    case 12:
    {
        return (nowMillis / 1000);
    }
    break;
    case 13:
    {
        return (nowMillis / 60000);
    }
    break;

    case 22:
    {
        return lowestVelocite_MultipleLayer(thoseLayers);
    }
    break;
    case 23:
    {
        return highestVelocite_MultipleLayer(thoseLayers);
    }
    break;
    case 24:
    {
        return meanVelocite_MultipleLayer(thoseLayers);
    }
    break;

    case 26:
    {
        return dureeLayerMin_MultipleLayer(thoseLayers);
    }
    break;
    case 27:
    {
        return dureeLayerMax_MultipleLayer(thoseLayers);
    }
    break;
    case 28:
    {
        return dureeLayer_MultipleLayer(thoseLayers);
    }
    break;
    case 29:
    {
        return nbNotesTotal_MultipleLayer(thoseLayers);
    }
    break;

    case 30:
    {
        return minPitchSpan_MultipleLayer(thoseLayers);
    }
    break;
    case 31:
    {
        return maxPitchSpan_MultipleLayer(thoseLayers);
    }
    break;
    case 32:
    {
        return moyPitchSpan_MultipleLayer(thoseLayers);
    }
    break;
    default:
        return 0;
        break;
    }
}
