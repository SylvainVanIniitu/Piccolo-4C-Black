
/* *******************************************************************
/
/
/  FONCTION FRACTALE, piquée au développement de Dende, réalisé avec Antoine Pacifique
/ support music : https://lepacifiquerecords.bandcamp.com
/ but dont forget : https://e42a8.bandcamp.com 
/
/
/ ****************************************************************** */

void FractalDelay(unsigned long &start, unsigned long &_end, float divideFLOAT, int velocityStart, int pitchStart, int MaxLevel, int pourcent, int layerI)
{

// pourcent est rempli par dendeDecay !  

    int divide = round(divideFLOAT);

    if (overLoadCounter >= overLoadLimit)
    {
        return;
    } // qd on atteint limite de nb de notes -> arrêt de récursion

int tempDendeStretch = getValueStraightOrFromSequence(dendeStretch[layerI], 0); 

    unsigned long duree;
    duree = _end - start;
    duree = duree * tempDendeStretch / 100;

    unsigned long dureeOneDelay;
    dureeOneDelay = duree / divide;
    unsigned long moment_;

    int compteurArpeg = 0;
    int placeInArpeg = 0;

    for (int i = 0; i < divide; i++)
    {
int        tempDendeStart = getValueStraightOrFromSequence(dendeStart[layerI],0);
int tempDendeEnd = getValueStraightOrFromSequence(dendeEnd[layerI], 0);

if (i >= tempDendeStart && i <= tempDendeEnd )
{
    if (_bitMapDelay[layerI][i % 8] == 1)
    {
        moment_ = start + i * dureeOneDelay;

        int newVelocity = 0;

        if (i == 0)
        {
            newVelocity = velocityStart;
        }

        if (i == 1)
        {
            // newVelocity = velocityStart * (100 - dendeDrop[layerI]) / 100;

            newVelocity = velocityStart * (getValueStraightOrFromSequence(dendeDrop[layerI], 0)) / 100;
        }

        if (i > 1)
        {
            float differenceEntre2Delay;
            int tempDrop = getValueStraightOrFromSequence(dendeDrop[layerI], 0);
            int tempDendeDown = getValueStraightOrFromSequence(dendeDown[layerI], 0);
            differenceEntre2Delay = (float)(tempDendeDown - tempDrop) / (divide - 2);
            float Fvelo = velocityStart;
            float Fdrop = tempDrop;
            float FnewVelocity = Fvelo * ((Fdrop + (i - 1) * differenceEntre2Delay)) / 100;
            newVelocity = (byte)FnewVelocity;
            if (newVelocity > 127)
            {
                newVelocity = 127;
            }
        }

        int newPitch;
        newPitch = noteADegreesAboveNoteBinGammeCmodeD(compteurArpeg, pitchStart, dendeGamme[layerI], dendeMode[layerI]);

int tempDendeSubFidelity = getValueStraightOrFromSequence(dendeSubFidelity[layerI],0); 

        if (rand() % 100 < tempDendeSubFidelity)
        {
            addNewNotetoPileONCE(pointeurInitial, NoteChannel, newPitch, newVelocity, moment_ / 1000, NOTE_ON, layerI);

int tempDendeNoteOff = getValueStraightOrFromSequence(dendeNoteOff[layerI],0 );

if (tempDendeNoteOff == 0)
// pas de note OFF
{
            }
else
            if (tempDendeNoteOff == 1)
            // mode NoteOFF après durée de l'intervalle prévu
            {
                addNewNotetoPileONCE(pointeurInitial, NoteChannel, newPitch, newVelocity, (moment_ + duree) / 1000, NOTE_OFF, layerI);
            }
else 
            if (tempDendeNoteOff == 2)
            // note OFF qui survient à la fin du delay total
            {
                addNewNotetoPileONCE(pointeurInitial, NoteChannel, newPitch, newVelocity, initialEnd / 1000, NOTE_OFF, layerI);
                // modif 2C22 l'initialEnd n'était jamais spécifié
            }
else 
            if (tempDendeNoteOff == 3)
            // note OFF qui fonctionne mais était un BUG
            {
                addNewNotetoPileONCE(pointeurInitial, NoteChannel, newPitch, newVelocity, moment_ / 1000, NOTE_OFF, layerI);
            }
else 
{
    // comme Note Off 
}
            overLoadCounter++;
        }

        if (MaxLevel > 1 && rand() % 100 < pourcent)
        // pourcent vaut ici dendeDecay[layerI]
        {
            unsigned long newStart = start + i * dureeOneDelay;
            unsigned long newEnd = start + (i + 1) * dureeOneDelay;

            int newPitch;

            int tempDendeDegres = getValueStraightOrFromSequence(dendeDegres[layerI],-12); 
            newPitch = noteADegreesAboveNoteBinGammeCmodeD(tempDendeDegres+ compteurArpeg, pitchStart, dendeGamme[layerI], dendeMode[layerI]);
            
            int tempDendePitch = getValueStraightOrFromSequence(dendePitch[layerI],-24); 
            newPitch = newPitch + tempDendePitch ;

            int tempDendeRatio = getValueStraightOrFromSequence(dendeRatio[layerI],0); 

            FractalDelay(newStart,
                         newEnd,
                         (float)(divideFLOAT * tempDendeRatio / 100),
                         newVelocity,
                         newPitch,
                         MaxLevel - 1,
                         pourcent,
                         layerI);
        }

        if (placeInArpeg % 2 == 0)
        {
            int tempArp1 = getValueStraightOrFromSequence(dendeArp1[layerI],-12) ; 
            compteurArpeg += tempArp1 ;
        }
        else
        {
            int tempArp2 = getValueStraightOrFromSequence(dendeArp2[layerI], -12);
            compteurArpeg += tempArp2 ;
        }

        placeInArpeg++;
    }
    }

int tempDendeRetard = getValueStraightOrFromSequence(dendeRetard[layerI],-99);

        if (tempDendeRetard != 100)
        {
            dureeOneDelay = (unsigned long) ( (float)dureeOneDelay * ((float) 100 + (float)tempDendeRetard ) / (float) 100 );
        }
    }
}

 