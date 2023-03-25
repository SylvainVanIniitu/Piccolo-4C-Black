
/* *******************************************************************
/
/
/  FONCTION FRACTALE, piquée à Dende, développé avec Antoine Pacifique
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

    unsigned long duree;
    duree = _end - start;
    duree = duree * dendeStretch[layerI] / 100;

    unsigned long dureeOneDelay;
    dureeOneDelay = duree / divide;
    unsigned long moment_;

    int compteurArpeg = 0;
    int placeInArpeg = 0;

    for (int i = 0; i < divide; i++)
    {
        if (i >= dendeStart[layerI] && i <= dendeEnd[layerI] )
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
                    newVelocity = velocityStart * (dendeDrop[layerI]) / 100;
                }

                if (i > 1)
                {
                    float differenceEntre2Delay;
                    differenceEntre2Delay = (float)(dendeDown[layerI] - dendeDrop[layerI]) / (divide - 2);
                    float Fvelo = velocityStart;
                    float Fdrop = dendeDrop[layerI];
                    float FnewVelocity = Fvelo * ((Fdrop + (i - 1) * differenceEntre2Delay)) / 100;
                    newVelocity = (byte)FnewVelocity;
                    if (newVelocity > 127)
                    {
                        newVelocity = 127;
                    }
                }

                int newPitch;
                newPitch = noteADegreesAboveNoteBinGammeCmodeD(compteurArpeg, pitchStart, dendeGamme[layerI], dendeMode[layerI]);

                if (rand() % 100 < dendeSubFidelity[layerI])
                {
                    addNewNotetoPileONCE(pointeurInitial, NoteChannel, newPitch, newVelocity, moment_ / 1000, NOTE_ON, layerI);

                    if (dendeNoteOff[layerI] == 0) 
                    // pas de note OFF
                    {
                    }

                    if (dendeNoteOff[layerI] == 1)
                    // mode NoteOFF après durée de l'intervalle prévu
                    {
                        addNewNotetoPileONCE(pointeurInitial, NoteChannel, newPitch, newVelocity, (moment_ + duree) / 1000, NOTE_OFF, layerI);
                    }

                    if (dendeNoteOff[layerI] == 2)
                    // note OFF qui survient à la fin du delay total
                    {
                        addNewNotetoPileONCE(pointeurInitial, NoteChannel, newPitch, newVelocity, initialEnd / 1000, NOTE_OFF, layerI);
                        // modif 2C22 l'initialEnd n'était jamais spécifié 
                    }

                    if (dendeNoteOff[layerI] == 3)
                    // note OFF qui fonctionne mais était un BUG
                    {
                        addNewNotetoPileONCE(pointeurInitial, NoteChannel, newPitch, newVelocity, moment_ / 1000, NOTE_OFF, layerI);
                    }

                    overLoadCounter++;
                }

                if (MaxLevel > 1 && rand() % 100 < pourcent)
                // pourcent vaut ici dendeDecay[layerI]
                {
                    unsigned long newStart = start + i * dureeOneDelay;
                    unsigned long newEnd = start + (i + 1) * dureeOneDelay;

                    int newPitch;
                    newPitch = noteADegreesAboveNoteBinGammeCmodeD(dendeDegres[layerI] + compteurArpeg, pitchStart, dendeGamme[layerI], dendeMode[layerI]);
                    newPitch = newPitch + dendePitch[layerI];

                    FractalDelay(newStart,
                                 newEnd,
                                 (float)(divideFLOAT * dendeRatio[layerI] / 100),
                                 newVelocity,
                                 newPitch,
                                 MaxLevel - 1,
                                 pourcent,
                                 layerI);
                }

                if (placeInArpeg % 2 == 0)
                {
                    compteurArpeg += dendeArp1[layerI];
                }
                else
                {
                    compteurArpeg += dendeArp2[layerI];
                }

                placeInArpeg++;
        }
    }

        if (dendeRetard[layerI] != 100)
        {
            dureeOneDelay = (unsigned long) ( (float)dureeOneDelay * ((float) 100 + (float)dendeRetard[layerI]) / (float) 100 );
        }
    }
}

 