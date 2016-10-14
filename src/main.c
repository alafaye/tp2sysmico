#include <msp430.h>
#include <intrinsics.h>
#include <HAL_Dogs102x6.h>
#include "Scheduler.h"
#include "ta_32768.h"
#include "SMC_hal.h"

unsigned int tick_led;

void setup_AD(void){


}

// To setup clock to freq indicated at the end of the func.
void setup_clk(void) {
    P1OUT |= BIT2;
    // Target Freq for SMCLK, MCLK 3.2Mhz -> 32kHz/8*25*32
    __disable_interrupt();

    // Choosing XT1 and FLLREFDIV as 8
    UCSCTL3 = SELREF__XT1CLK + FLLREFDIV__8;
    // DCORSEL bits to 0
    UCSCTL1 &= ~(BIT4 | BIT5 | BIT6);
    // DCORSEL to 4 for 3.2Mhz Target
    UCSCTL1 |= DCORSEL_4;
    // FLLN set to 25 and FLLD to 32.
    UCSCTL2 = FLLD__32 + 24;

    // Set all the cloks to DCO ref
    UCSCTL4 = SELA__DCOCLK + SELM__DCOCLK + SELS__DCOCLK;

    // All clks dividers to 1
    UCSCTL5 = DIVA__1 + DIVM__1 + DIVS__1;

    // freq ACKL 32'768
    // freq MCLK 3'276'800
    // freq SMCLK 3'276'800


    //__enable_interrupt();
    P1OUT &= ~BIT2;
}

//Screen intialisation
void screen_init(void){
    Dogs102x6_init();
    Dogs102x6_backlightInit();
    Dogs102x6_setBacklight(3);
    Dogs102x6_setContrast(16);
    Dogs102x6_clearScreen();
    // Dogs102x6_stringDraw(H_X_POS, H_Y_POS, "00:00:00", 0);
}

// General setup function
void init(void){
    // Screen and clock setup
    screen_init();
    setup_clk();
}

// To make the Led blink following a particuliar pattern
// Ticking 3 times at 200ms interval, then off for 3s
void blink_led(void){
    tick_led += 1;
    if(tick_led<=6){
        P1OUT ^= BIT1;
    }
    else if(tick_led==20){
        tick_led=0;
    }
}


// Main function
void main(void)
{

    // Stop watchdog timer to prevent time out reset
    WDTCTL = WDTPW + WDTHOLD;

    //Call init function for some basic setup
    init();

    // Initialisation de l'ordonnanceur
    SCH_Init_T0();

    TA0CTL |= TASSEL_2 + MC_1; // On MCLK up to Max Register Val
    TA0CCR0 = 32767; // Blink period -> 10ms

    // To use clock to drive Led P1.1 through interruptions
    P1DIR |= BIT1;
    P1OUT &= ~BIT1; //LED Off

    // Demarrage de l'ordonnanceur
    SCH_Start();

    // Boucle infinie. La mise eventuelle en basse consommation
    // est faite dans la fonction SCH_Dispatch_Tasks
    while(1)
    {
        SCH_Dispatch_Tasks();
    }
}
