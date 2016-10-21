#include <msp430.h>
#include <intrinsics.h>
#include <HAL_Dogs102x6.h>
#include "Scheduler.h"
#include "ta_32768.h"
#include "SMC_hal.h"

unsigned int tick_led;

//Transform factor between 91 and 255
#define TRANSFORM_FACTOR 2.8
// define to the pot val between 0-255
#define VAL_POT ADC12MEM0

// define the rows of the palette
// on three rows
# define row_1 3
# define row_2 4
# define row_3 5

// lenght of the palette
#define lenght 10

void init_AD(void){
    P6SEL = 0x0F; // Enable A/D Channel inputs
    //ADC12 is on + multiple sample and convert
    ADC12CTL0 = ADC12ON+ADC12MSC+ADC12SHT0_8; 
    ADC12CTL1 = ADC12SHP+ADC12CONSEQ_3;
    // Channel A5
    ADC12MCTL5 = ADC12INCH_5 + ADC12EOS; 
    // Enable encoder
    ADC12CTL0 |= ADC12ENC;
    // Start software trigger
    ADC12CTL0 |= ADC12SC; 
}

// To power the potentiometer
void init_pot(void){
    P8DIR |= BIT0;
    P8OUT |= BIT0;
}

//Screen intialisation
void init_screen(void){
    Dogs102x6_init();
    Dogs102x6_backlightInit();
    Dogs102x6_setBacklight(3);
    Dogs102x6_setContrast(16);
    Dogs102x6_clearScreen();
}

// General setup function
void init(void){
    // Screen and pot setup
    init_screen();
    init_AD();
    init_pot();
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

// Clears the line and draw the palette position
// Uses VAL_POT, TRANSFORM_FACTOR
void draw_palette(void){
    // Clear the lines
    Dogs102x6_clearRow(row_1);
    Dogs102x6_clearRow(row_2);
    Dogs102x6_clearRow(row_3);
    // put the value of the pot between 0-101
    int pos_palette = VAL_POT/TRANSFORM_FACTOR;
    // And draw it
    Dogs102x6_horizontalLineDraw(pos_palette, pos_palette + lenght, row_1, 0);
    Dogs102x6_horizontalLineDraw(pos_palette, pos_palette + lenght, row_2, 0);
    Dogs102x6_horizontalLineDraw(pos_palette, pos_palette + lenght, row_2, 0);
}

// Main function
void main(void)
{

    // Stop watchdog timer to prevent time out reset
    WDTCTL = WDTPW + WDTHOLD;

    //Call init function for some basic setup
    init();

    // Add all the tasks
    SCH_Add_Task(blink_led, 0, 10);
    SCH_Add_Task(draw_palette, 0, 10);
    // SCH_Add_Task(draw_ball, 0, 10);

    // Initialisation de l'ordonnanceur
    SCH_Init_T0();

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
