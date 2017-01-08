//********************************************************
//**   Whetstone benchmark DP for  ARM Cx-M7        ******
//**   ST Nucleo-144 Stm32F746 and Stm32F767        ******
//**         Whetstone benchmark DP                 ******
//**           Jovan Ivkovic 2016.                  ******
//**         JovanEps (jovan.eps@gmail.com).        ******
//********************************************************

// Ported for the Hifive board by Riktw ( rik@iseenbeunhaas.com) 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "platform.h"
#include "encoding.h"

/* map the FORTRAN math functions, etc. to the C versions */
#define DSIN    sin
#define DCOS    cos
#define DATAN   atan
#define DLOG    log
#define DEXP    exp
#define DSQRT   sqrt
#define IF      if

/* function prototypes */
void POUT(long N, long J, long K, double X1, double X2, double X3, double X4);
void PA(double E[]);
void P0(void);
void P3(double X, double Y, double *Z);
#define USAGE   "usage: whetdc [-c] [loops]\n"

//#define PRINTOUT 1

uint32_t time_in_secs(uint64_t ticks);

/*
    COMMON T,T1,T2,E1(4),J,K,L
*/
double T,T1,T2,E1[5];
int J,K,L;
int argc = 0;   //Mod for nucleo. Change in code below if you want non-default loop count

//************************************
//**    Whetstone    64b-DP         **
//**        SUB                     **
//************************************
int Whetstone() // ------------ Metoda -----------
{
    printf("Beginning Whetstone benchmark at ");
    
    printf("default 260 MHz ...\n");
    /* used in the FORTRAN version */
    long I;
    long N1, N2, N3, N4, N6, N7, N8, N9, N10, N11;
    double X1,X2,X3,X4,X,Y,Z;
    long LOOP;
    int II, JJ;

    /* added for this version */
    long loopstart = 0;
    uint64_t startsec,finisec = 0;
    double KIPS;
    int continuous;

    loopstart = 25000;       /* 1000 see the note about LOOP below */
    continuous = 0;

    II = 1;     /* start at the first arg (temp use of II here) */
 
LCONT:
/*
********************************************
*   Start benchmark timing at this point.
********************************************
*/
    startsec = 0;
    finisec = 0;
    startsec = get_timer_value();

/*
********************************************
*   The actual benchmark starts here.
********************************************
*/
    T  = .499975;
    T1 = 0.50025;
    T2 = 2.0;
/*
********************************************
*   With loopcount LOOP=10, one million Whetstone instructions
*   will be executed in EACH MAJOR LOOP..A MAJOR LOOP IS EXECUTED
*   'II' TIMES TO INCREASE WALL-CLOCK TIMING ACCURACY.
*
*   LOOP = 1000;
*/
    LOOP = loopstart;
    II   = 1;
    JJ = 1;

IILOOP:
    N1  = 0;
    N2  = 12 * LOOP;
    N3  = 14 * LOOP;
    N4  = 345 * LOOP;
    N6  = 210 * LOOP;
    N7  = 32 * LOOP;
    N8  = 899 * LOOP;
    N9  = 616 * LOOP;
    N10 = 0;
    N11 = 93 * LOOP;
/*
********************************************
*   Module 1: Simple identifiers
********************************************
*/
    X1  =  1.0;
    X2  = -1.0;
    X3  = -1.0;
    X4  = -1.0;

    for (I = 1; I <= N1; I++)
    {
        X1 = (X1 + X2 + X3 - X4) * T;
        X2 = (X1 + X2 - X3 + X4) * T;
        X3 = (X1 - X2 + X3 + X4) * T;
        X4 = (-X1+ X2 + X3 + X4) * T;
    }
#ifdef PRINTOUT
    IF (JJ==II) POUT(N1,N1,N1,X1,X2,X3,X4);
#endif

/*
********************************************
*   Module 2: Array elements
********************************************
*/
    E1[1] =  1.0;
    E1[2] = -1.0;
    E1[3] = -1.0;
    E1[4] = -1.0;

    for (I = 1; I <= N2; I++)
    {
        E1[1] = ( E1[1] + E1[2] + E1[3] - E1[4]) * T;
        E1[2] = ( E1[1] + E1[2] - E1[3] + E1[4]) * T;
        E1[3] = ( E1[1] - E1[2] + E1[3] + E1[4]) * T;
        E1[4] = (-E1[1] + E1[2] + E1[3] + E1[4]) * T;
    }

#ifdef PRINTOUT
    IF (JJ==II) POUT(N2,N3,N2,E1[1],E1[2],E1[3],E1[4]);
#endif

/*
********************************************
*  Module 3: Array as parameter
********************************************
*/
    for (I = 1; I <= N3; I++)
    {
        PA(E1);
    }
#ifdef PRINTOUT
    IF (JJ==II) POUT(N3,N2,N2,E1[1],E1[2],E1[3],E1[4]);
#endif

/*
********************************************
*   Module 4: Conditional jumps
********************************************
*/
    J = 1;
    for (I = 1; I <= N4; I++)
    {
        if (J == 1)
            J = 2;
        else
            J = 3;

        if (J > 2)
            J = 0;
        else
            J = 1;

        if (J < 1)
            J = 1;
        else
            J = 0;
    }

#ifdef PRINTOUT
    IF (JJ==II) POUT(N4,J,J,X1,X2,X3,X4);
#endif

/*
********************************************
*   Module 5: Omitted
*   Module 6: Integer arithmetic
********************************************
*/

    J = 1;
    K = 2;
    L = 3;

    for (I = 1; I <= N6; I++)
    {
        J = J * (K-J) * (L-K);
        K = L * K - (L-J) * K;
        L = (L-K) * (K+J);
        E1[L-1] = J + K + L;
        E1[K-1] = J * K * L;
    }

#ifdef PRINTOUT
    IF (JJ==II) POUT(N6,J,K,E1[1],E1[2],E1[3],E1[4]);
#endif

/*
********************************************
*   Module 7: Trigonometric functions
********************************************
*/
    X = 0.5;
    Y = 0.5;

    for (I = 1; I <= N7; I++)
    {
        X = T * DATAN(T2*DSIN(X)*DCOS(X)/(DCOS(X+Y)+DCOS(X-Y)-1.0));
        Y = T * DATAN(T2*DSIN(Y)*DCOS(Y)/(DCOS(X+Y)+DCOS(X-Y)-1.0));
    }

#ifdef PRINTOUT
    IF (JJ==II)POUT(N7,J,K,X,X,Y,Y);
#endif

/*
********************************************
*   Module 8: Procedure calls
********************************************
*/
    X = 1.0;
    Y = 1.0;
    Z = 1.0;

    for (I = 1; I <= N8; I++)
    {
        P3(X,Y,&Z);
    }
#ifdef PRINTOUT
    IF (JJ==II)POUT(N8,J,K,X,Y,Z,Z);
#endif

/*
********************************************
*   Module 9: Array references
********************************************
*/
    J = 1;
    K = 2;
    L = 3;
    E1[1] = 1.0;
    E1[2] = 2.0;
    E1[3] = 3.0;

    for (I = 1; I <= N9; I++)
    {
        P0();
    }
#ifdef PRINTOUT
    IF (JJ==II) POUT(N9,J,K,E1[1],E1[2],E1[3],E1[4]);
#endif

/*
********************************************
*   Module 10: Integer arithmetic
********************************************
*/
    J = 2;
    K = 3;

    for (I = 1; I <= N10; I++)
    {
        J = J + K;
        K = J + K;
        J = K - J;
        K = K - J - J;
    }

#ifdef PRINTOUT
    IF (JJ==II) POUT(N10,J,K,X1,X2,X3,X4);
#endif

/*
********************************************
*   Module 11: Standard functions
********************************************
*/
    X = 0.75;

    for (I = 1; I <= N11; I++)
    {
        X = DSQRT(DEXP(DLOG(X)/T1));
    }
#ifdef PRINTOUT
    IF (JJ==II) POUT(N11,J,K,X,X,X,X);
#endif

/*
********************************************
*      THIS IS THE END OF THE MAJOR LOOP.
********************************************
*/
    if (++JJ <= II)
        goto IILOOP;

/*
********************************************
*      Stop benchmark timing at this point.
********************************************
*/
   // finisec = time(0);
    finisec = get_timer_value();
    //timer.reset();

/*
*--------------------------------------------------------------------
*      Performance in Whetstone KIP's per second is given by
*
*   (100*LOOP*II)/TIME
*
*      where TIME is in seconds.
*--------------------------------------------------------------------
*/
    printf(" kraj \n");
    double vreme;
    vreme = time_in_secs(finisec - startsec);
    
    if (vreme <= 0)
     {
        printf("Insufficient duration- Increase the LOOP count \n");
        finisec = 0; 
        startsec = 0;
        return 1;
     }

    printf("Loops: %ld , \t Iterations: %d, \t Duration: %.3f sec. \n",
            LOOP, II, vreme);

    KIPS = (100.0 * LOOP * II) / vreme ;
   
  //  if (KIPS >= 1000.0)
  //      printf("C Converted Double Precision Whetstones: %.3f MIPS \n\n", KIPS / 1000);
  //  else
  //      printf("C Converted Double Precision Whetstones: %.3f KIPS \n\n", KIPS);
        
        printf("C Converted Double Precision Whetstones: %.3f MIPS \n\n", KIPS / 1000);

    if (continuous)
        goto LCONT;

    finisec = 0; 
    startsec = 0;
    return 1;
}

void PA(double E[])
{
    J = 0;

L10:
    E[1] = ( E[1] + E[2] + E[3] - E[4]) * T;
    E[2] = ( E[1] + E[2] - E[3] + E[4]) * T;
    E[3] = ( E[1] - E[2] + E[3] + E[4]) * T;
    E[4] = (-E[1] + E[2] + E[3] + E[4]) / T2;
    J += 1;

    if (J < 6)
        goto L10;
}

void P0(void)
{
    E1[J] = E1[K];
    E1[K] = E1[L];
    E1[L] = E1[J];
}

void P3(double X, double Y, double *Z)
{
    double X1, Y1;

    X1 = X;
    Y1 = Y;
    X1 = T * (X1 + Y1);
    Y1 = T * (X1 + Y1);
    *Z  = (X1 + Y1) / T2;
}

uint32_t time_in_secs(uint64_t ticks)
{
  // scale timer down to avoid uint64_t -> double conversion in RV32
  int scale = 256;
  uint32_t delta = ticks / scale;
  uint32_t freq = get_timer_freq() / scale;
  return delta / (double)freq;
}


#ifdef PRINTOUT
void POUT(long N, long J, long K, double X1, double X2, double X3, double X4)
{
    printf("%7ld %7ld %7ld %12.4e %12.4e %12.4e %12.4e\n",
                        N, J, K, X1, X2, X3, X4);
}
#endif   

//*********************************
//**         MAIN block          **
//*********************************
int main()
{
    printf("\n My Benchmark example for Whetstones \n");
    
    while(1)
    {
        Whetstone(); //Call of Whetstone banchmark methode
    }
}