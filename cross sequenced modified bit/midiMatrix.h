// prototype
void buffer_and_HARD_print(String Texte); 

int limiter  (   int   num , int bas , int haut )
// LIMITE a ces chiffres là, INCLUS
{
    int ret ; 
ret = num ; 

    if (num > haut) {
        ret = haut ;
    }

    if (num < bas) {
        ret = bas ;
    }

    return ret ;
}

void midiMatrixSetON ( unsigned int& num , int bbit )
{
    unsigned int temp(0);
    bbit = limiter( bbit, 0, 16);
    temp = ( 1 << bbit );
    num = ( num | temp );
}

void midiMatrixSetOFF( unsigned int& num , int bbit )
{
    unsigned int temp(0);
    bbit = limiter( bbit, 0, 16);
    temp = ( 1 << bbit );
    temp = !temp ;
    num = ( num & temp );
}

boolean isONmidiMatrix(  unsigned int  num , int bbit )
{
    unsigned int temp(0);
    bbit = limiter( bbit, 0, 16);
    temp = ( 1 << bbit );
    num = ( num & temp ) ;
    num = num >> bbit ;
    if ( num == 1 ) {
        return true ;
    } else {
        return false ;
    }
}

// sur l'exemple de int (&tab)[tailleMaxTableau] utilisé dans la modulation

void redirectMatrixAtoB ( unsigned int (&matrice)[16] , int A , int B )
{
    midiMatrixSetON(matrice[A],B);
}

void backToSelfMatrixA ( unsigned int (&matrice)[16] , int A )
{
    midiMatrixSetON(matrice[A],A);
}

void emptyMidiMatrixA ( unsigned int (&matrice)[16], int A )
{
    for ( byte j = 0 ; j < 16 ; j ++ )
    {
        midiMatrixSetOFF(matrice[A], j);
        // ou matrice[A]=0;
    }
}

void emptyMidiMatrixTotal ( unsigned int (&matrice)[16] )
{
    for ( byte j = 0 ; j < 16 ; j ++ )
    {
        matrice[j]=0;
    }
}

void backToSelfMatrixTotal (unsigned int (&matrice)[16] )
{
    emptyMidiMatrixTotal(matrice);

    for ( byte j = 0 ; j < 16 ; j ++ )
    {
        backToSelfMatrixA(matrice,j);
    }
}

void randomizeMatrixTotal (unsigned int (&matrice)[16] )
{
    emptyMidiMatrixTotal(matrice);

    for ( byte j = 0 ; j < 16 ; j ++ )
    {
        midiMatrixSetON(matrice[j],random(0,16));
    }
}

void randomizePolyMatrixTotal (unsigned int (&matrice)[16] , int MAX )
{
    emptyMidiMatrixTotal(matrice);

    for ( byte j = 0 ; j < 16 ; j ++ )
    {
        byte k = random(1,MAX+1);
        for ( byte l = 0 ; l < k ; l++ )
        {
            midiMatrixSetON(matrice[j],random(0,16));
        }
    }
}

void sendAlltoChannelN (unsigned int (&matrice)[16], int N )
{

    for ( byte j = 0 ; j < 16 ; j ++ )
    {
        midiMatrixSetON(matrice[j],N);
    }
}

void sendChannelNtoAll (unsigned int (&matrice)[16], int N )
{
    N = limiter(N,0,15);
    for ( byte j = 0 ; j < 16 ; j ++ )
    {
        midiMatrixSetON(matrice[N],j);
    }
}

void distributeAllto4Channel (unsigned int (&matrice)[16] )
{
    emptyMidiMatrixTotal(matrice);

    for ( byte j = 0 ; j < 16 ; j ++ )
    {
        midiMatrixSetON(matrice[j],random(0,4));
    }
}

// JUSTE POUR DEBUG
void printMatrix (unsigned int (&matrice)[16])
{
    for ( byte i = 0 ; i < 16 ; i++ )
    {
        for ( byte j = 0 ; j < 16 ; j++ )
        {
            Serial.print(isONmidiMatrix(matrice[i], j));
            Serial.print(F(" "));
        }
        Serial.println();
    }
    delay(2000);
    Serial.println();
}

void printMatrixLine (unsigned int (&matrice)[16], int N)
{
    for ( byte i = 0 ; i < 16 ; i++ )
    {
        if (isONmidiMatrix(matrice[N], i))
        {
            u8x8.print(i+1,HEX) ;
        }
        else
        {
            buffer_and_HARD_print(F("-")) ;
        }
    }
}
