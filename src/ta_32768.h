/*------------------------------------------------------------------*-

  ta_32768.h (v1.00)

  ------------------------------------------------------------------

  - see ta_32768.c for details


  COPYRIGHT
  ---------

  This code is from the book:

  PATTERNS FOR TIME-TRIGGERED EMBEDDED SYSTEMS by Michael J. Pont 
  [Pearson Education, 2001; ISBN: 0-201-33138-1].

  This code is copyright (c) 2001 by Michael J. Pont.

  See book for copyright details and other information.


Modif  : Brunner Nicolas
Date   : 04.09.2008
Reason : adapted for msp430

-*------------------------------------------------------------------*/

#include <msp430.h>
#include "Scheduler.h"

// ------ Public function prototypes -------------------------------

void SCH_Init_T0(void);
void SCH_Start(void);


//For centi and mili seconds
extern unsigned int ms;
extern unsigned int cs;
/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
  -*------------------------------------------------------------------*/
