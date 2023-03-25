
// prototype

// PROTOTYPES DE CERTAINES FONCTIONS

// FONCTIONS EN TANT QUE TELLES

void generateDMX(byte DMXmode, byte cmd, byte pitch, byte velocity)
{
    if (DMXmode == 0)
    {
        dmxTx.set(random(5), random(256)); // version un peu basique, à rafiner !
    }
    else if (DMXmode == 1)
    {
    }
    else
    {
        // NADA
    }
}
