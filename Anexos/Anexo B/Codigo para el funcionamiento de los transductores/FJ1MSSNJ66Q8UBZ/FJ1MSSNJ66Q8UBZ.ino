#include <avr/sleep.h>
#include <avr/power.h>

#define N_PORTS 1
#define N_DIVS 24

#define WAIT_LOT(a) __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");  __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");  __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop");  __asm__ __volatile__ ("nop");  __asm__ __volatile__ ("nop")
#define WAIT_MID(a) __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");  __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");__asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop")
#define WAIT_LIT(a) __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");  __asm__ __volatile__ ("nop"); __asm__ __volatile__ ("nop");  __asm__ __volatile__ ("nop");  __asm__ __volatile__ ("nop");  __asm__ __volatile__ ("nop");  __asm__ __volatile__ ("nop")


#define OUTPUT_WAVE(pointer, d)  PORTC = pointer[d*N_PORTS + 0]

#define N_BUTTONS 6
#define STEP_SIZE 1
#define BUTTON_SENS 2500 
#define N_FRAMES 24

static byte frame = 0;
static byte animation[N_FRAMES][N_DIVS] = 
{{0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa},
{0x9,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x6,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa},
{0x9,0x9,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x6,0x6,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa},
{0x9,0x9,0x9,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x6,0x6,0x6,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa},
{0x9,0x9,0x9,0x9,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x6,0x6,0x6,0x6,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa},
{0x9,0x9,0x9,0x9,0x9,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x6,0x6,0x6,0x6,0x6,0xa,0xa,0xa,0xa,0xa,0xa,0xa},
{0x9,0x9,0x9,0x9,0x9,0x9,0x5,0x5,0x5,0x5,0x5,0x5,0x6,0x6,0x6,0x6,0x6,0x6,0xa,0xa,0xa,0xa,0xa,0xa},
{0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x5,0x5,0x5,0x5,0x5,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0xa,0xa,0xa,0xa,0xa},  
{0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x5,0x5,0x5,0x5,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0xa,0xa,0xa,0xa},
{0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x5,0x5,0x5,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0xa,0xa,0xa},
{0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x5,0x5,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0xa,0xa},
{0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x5,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0xa},
{0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6},
{0x5,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0xa,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6},
{0x5,0x5,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0xa,0xa,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6},
{0x5,0x5,0x5,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0xa,0xa,0xa,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6},
{0x5,0x5,0x5,0x5,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0xa,0xa,0xa,0xa,0x6,0x6,0x6,0x6,0x6,0x6,0x6,0x6},
{0x5,0x5,0x5,0x5,0x5,0x9,0x9,0x9,0x9,0x9,0x9,0x9,0xa,0xa,0xa,0xa,0xa,0x6,0x6,0x6,0x6,0x6,0x6,0x6},
{0x5,0x5,0x5,0x5,0x5,0x5,0x9,0x9,0x9,0x9,0x9,0x9,0xa,0xa,0xa,0xa,0xa,0xa,0x6,0x6,0x6,0x6,0x6,0x6},
{0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x9,0x9,0x9,0x9,0x9,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0x6,0x6,0x6,0x6,0x6},
{0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x9,0x9,0x9,0x9,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0x6,0x6,0x6,0x6},
{0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x9,0x9,0x9,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0x6,0x6,0x6},
{0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x9,0x9,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0x6,0x6},
{0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x9,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0x6}};


void setup()
{

   DDRC = 0b00001111; //A0 to A3 are the signal outputs
   PORTC = 0b00000000; 
   
   pinMode(10, OUTPUT); //pin 10 (B2) will generate a 40kHz signal to sync 
   pinMode(11, INPUT_PULLUP); //pin 11 (B3) is the sync in
   //please connect pin 10 to pin 11

   for (int i = 2; i < 8; ++i){ //pin 2 to 7 (D2 to D7) are inputs for the buttons
    pinMode(i, INPUT_PULLUP); 
   }

  // generate a sync signal of 40khz in pin 10
  noInterrupts();           // disable all interrupts
  TCCR1A = bit (WGM10) | bit (WGM11) | bit (COM1B1); // fast PWM, clear OC1B on compare
  TCCR1B = bit (WGM12) | bit (WGM13) | bit (CS10);   // fast PWM, no prescaler
  OCR1A =  (F_CPU / 40000L) - 1;
  OCR1B = (F_CPU / 40000L) / 2;
  interrupts();             // enable all interrupts

  // disable everything that we do not need 
  ADCSRA = 0;  // ADC
  power_adc_disable ();
  power_spi_disable();
  power_twi_disable();
  power_timer0_disable();
  //power_usart0_disable();
  Serial.begin(115200);

 byte* emittingPointer = &animation[frame][0];
 byte buttonsPort = 0;

 bool anyButtonPressed;
 bool buttonPressed[N_BUTTONS];
 short buttonCounter = 0;

  // Ciclo infinito
  LOOP:
    while(PINB & 0b00001000); //wait for pin 11 (B3) to go low 
    
    OUTPUT_WAVE(emittingPointer, 0); buttonsPort = PIND; WAIT_LIT();
    OUTPUT_WAVE(emittingPointer, 1); anyButtonPressed = (buttonsPort & 0b11111100) != 0b11111100; WAIT_MID();
    OUTPUT_WAVE(emittingPointer, 2); buttonPressed[0] = buttonsPort & 0b00000100; WAIT_MID();
    OUTPUT_WAVE(emittingPointer, 3); buttonPressed[1] = buttonsPort & 0b00001000; WAIT_MID();
    OUTPUT_WAVE(emittingPointer, 4); buttonPressed[2] = buttonsPort & 0b00010000; WAIT_MID();
    OUTPUT_WAVE(emittingPointer, 5); buttonPressed[3] = buttonsPort & 0b00100000; WAIT_MID();
    OUTPUT_WAVE(emittingPointer, 6); buttonPressed[4] = buttonsPort & 0b01000000; WAIT_MID();
    OUTPUT_WAVE(emittingPointer, 7); buttonPressed[5] = buttonsPort & 0b10000000; WAIT_MID();
    OUTPUT_WAVE(emittingPointer, 8); WAIT_LOT();
    OUTPUT_WAVE(emittingPointer, 9); WAIT_LOT();
    OUTPUT_WAVE(emittingPointer, 10); WAIT_LOT();
    OUTPUT_WAVE(emittingPointer, 11); WAIT_LOT();
    OUTPUT_WAVE(emittingPointer, 12); WAIT_LOT();
    OUTPUT_WAVE(emittingPointer, 13); WAIT_LOT();
    OUTPUT_WAVE(emittingPointer, 14); WAIT_LOT();
    OUTPUT_WAVE(emittingPointer, 15); WAIT_LOT();
    OUTPUT_WAVE(emittingPointer, 16); WAIT_LOT();
    OUTPUT_WAVE(emittingPointer, 17); WAIT_LOT();
    OUTPUT_WAVE(emittingPointer, 18); WAIT_LOT();
    OUTPUT_WAVE(emittingPointer, 19); WAIT_LOT();
    OUTPUT_WAVE(emittingPointer, 20); WAIT_LOT();
    OUTPUT_WAVE(emittingPointer, 21); WAIT_LOT();
    OUTPUT_WAVE(emittingPointer, 22); WAIT_LOT();
    OUTPUT_WAVE(emittingPointer, 23); 


    if( anyButtonPressed ){
       ++buttonCounter;
       if (buttonCounter > BUTTON_SENS){
        buttonCounter = 0;
        
        if (! buttonPressed[0] ) {
          if( frame < STEP_SIZE ) { 
            frame = N_FRAMES-1;
         }else{
            frame-=STEP_SIZE; 
         }
        }
        else if (! buttonPressed[1] ) { 
          if ( frame >= N_FRAMES-STEP_SIZE ) { 
            frame = 0;
          }else {
            frame+=STEP_SIZE; 
          }  
       }else if (! buttonPressed[2] ) { 
          frame = 0;
       }
        emittingPointer = & animation[frame][0];
       }
    }else {
      buttonCounter = 0;
    }
    
  goto LOOP;
  
}

void loop(){}
