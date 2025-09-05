// Lab9Main.c
// Runs on MSPM0G3507
// Lab 9 ECE319K
// Your name: Anna Ledbetter & Aashvi Maithani
// Last Modified: 12/26/2024

#include <stdio.h>
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "../inc/ST7735.h"
#include "../inc/Clock.h"
#include "../inc/LaunchPad.h"
#include "../inc/TExaS.h"
#include "../inc/Timer.h"
#include "../inc/ADC1.h"
//#include "../inc/ADC.h"
#include "../inc/DAC5.h"
#include "../inc/Arabic.h"
#include "SmallFont.h"
#include "LED.h"
#include "Switch.h"
#include "Sound.h"
#include "sounds/sounds.h"
//#include "../inc/EdgeTriggered.h"

#include "images/images.h"
#include "../inc/JoyStick.h"
#include "../inc/UART.h"

#define ADCVREF_VDDA 0x000

// ****note to ECE319K students****
// the data sheet says the ADC does not work when clock is 80 MHz
// however, the ADC seems to work on my boards at 80 MHz
// I suggest you try 80MHz, but if it doesn't work, switch to 40MHz
void PLL_Init(void){ // set phase lock loop (PLL)
  //Clock_Init40MHz(); // run this line for 40MHz
  Clock_Init80MHz(0);   // run this line for 80MHz
}

// 1. SLIDE DECLARATIONS
uint8_t slide_flag;
uint32_t Output;
uint32_t Data;        // 12-bit ADC
uint32_t Position;    // 32-bit fixed-point 0.001 cm
float FloatPosition;  // 32-bit floating-point cm
uint32_t startTime,stopTime;
uint32_t Offset,ADCtime,Converttime,FloatConverttime,OutFixtime,FloatOutFixtime; // in bus cycles
uint32_t Time;

// 6. ACCELEROMETER DECLERATIONS
int32_t accel_x, accel_y, mag;
uint8_t flag_step = 0;
int32_t mag_list[500] = {0};
uint32_t mag_index = 0;
int32_t accel_x_list[500]  = {0};
int32_t accel_y_list[500]  = {0};

int32_t accel_xd, accel_yd, magd, duty_x, duty_y;
uint32_t T1, T2;
uint32_t T3 = 80000;
int32_t magd_list[100] = {0};
uint32_t magd_index = 0;
int32_t accel_x_listd[100]  = {0};
int32_t accel_y_listd[100]  = {0};
uint8_t flag1 = 0;
uint8_t flag2 = 0;
uint32_t start1, start2;


const int32_t scale_factor = 1250; /// (12.5% per g), divide each by 1000 --> 0.125
const int32_t upper_thresh = 4880890; //3978890; //understeps
const int32_t lower_thresh = 4842265; //3969624; // 
uint8_t flag_low = 0;
uint32_t step_count = 0;

uint8_t flag_right = 0;
uint8_t flag_down = 0;
uint8_t flag_up = 0;
uint8_t flag_return = 0;


void check_step(void) {
  duty_x = (T1 * 10000) / T3;
  duty_y = (T2 * 10000) / T3;
  // return acceleration in g's
  accel_xd = ((duty_x - 5000) * 1000) / scale_factor; 
  accel_yd = ((duty_y - 5000) * 1000) / scale_factor; 

  magd = (accel_xd * accel_xd) + (accel_yd * accel_yd);
  // change to check threshholds and step count
  magd_list[magd_index] = magd;
  magd_index = (magd_index + 1) % 100; 
}

void GROUP1_IRQHandler(void){
    if(GPIOB->CPU_INT. RIS& (1<<23)){ // PB23
        GPIOB->CPU_INT. ICLR = 1<<23;
          if((GPIOB->DIN31_0)&(1>>23)) { 
            // rising edge
            flag1 = 0; 
            start1 = SysTick->VAL;
          } else { 
            // falling edge
            flag1 = 1; 
            T1 = start1 - SysTick->VAL;
          }
        }

    if(GPIOB->CPU_INT. RIS& (1<<24)){ // PB24
        GPIOB->CPU_INT. ICLR = 1<<24;
        if((GPIOB->DIN31_0)&(1>>24)) {
            // rising edge
            flag2 = 0; 
            start2 = SysTick->VAL;
          } else {
            flag2 = 1; 
            T2 = start2 - SysTick->VAL;
          }
        }
    if (flag1 && flag2) {
        flag1 = 0;
        flag2 = 0; 
        check_step();
    }
}


// games  engine runs at 30Hz
void TIMG12_IRQHandler(void){uint32_t pos,msg;
  if((TIMG12->CPU_INT.IIDX) == 1){ // this will acknowledge
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
// game engine goes here
    // 1) sample slide pot
      Time++;
      // sample 12-bit ADC0 channel 5, slidepot
      // store data into mailbox
      Data = ADCin();
      // set the semaphore
      slide_flag = 1;
      GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
    
    // 2) read input switches

      if ((GPIOA->DIN31_0>>24)&0x1) {
        flag_right = 1;
        Clock_Delay(8000000); // debounce
      }
      if ((GPIOA->DIN31_0>>25)&0x1) {
        Clock_Delay(8000000);
        flag_down = 1;
      }
      if ((GPIOA->DIN31_0>>27)&0x1) {
        Clock_Delay(8000000);
        flag_up = 1;
      }
    
    // 3) move sprites
    // 4) start sounds
    // 5) set semaphore
    // 6) sample accelerometer
      ADC_InDual(ADC0,&accel_x,&accel_y);
      // return acceleration in g's
      mag = (accel_x * accel_x) + (accel_y * accel_y);
      //mag = ((accel_x) * (accel_x-2000)) + ((accel_y-2000) * (accel_y-2000));
      // change to check threshholds and step count
      mag_list[mag_index] = mag;
      accel_x_list[mag_index] = accel_x;
      accel_y_list[mag_index] = accel_y;
      //UART_OutUDec(mag);
      mag_index = (mag_index + 1) % 500;

      // if (mag_index == 0) {
      // ST7735_FillScreen(ST7735_BLACK);
      // ST7735_SetCursor(2, 4);
      // ST7735_OutString("BUFFER FULL!");
      // }
      


      if (mag_index == 499){
        mag_index = mag_index; // break point here
      }
  
      if (mag < lower_thresh) {
        flag_low = 1;
      }
      if ((mag > upper_thresh) && flag_low) {
        flag_low = 0;
        step_count ++;
        flag_step = 1;
      }

    // NO LCD OUTPUT IN INTERRUPT SERVICE ROUTINES
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
  }
}
uint8_t TExaS_LaunchPadLogicPB27PB26(void){
  return (0x80|((GPIOB->DOUT31_0>>26)&0x03));
}


// arrays for words in both languages
uint8_t language;
const char *English[] = {"English", "Angrezi"};

const char *StepCount[] = {"Step Count", "Kadam Ginatee"};
const char *StepCountGoal[] = {"Step Count Goal:\n            ", "Kadam Ginatee\n       Lakshy"};
const char *Congratulations[] = {"CONGRATULATIONS!", "BADHAI HO!"};
// AASHVI FILL IN


// GRAPHICS FUNCTIONS
// 7 x 4
void arrow(uint8_t l){
  if (l == 1){
    ST7735_DrawBitmap(56, 150, blackarrow, 20, 16);
    ST7735_DrawBitmap(56, 150, rightarrow, 20, 16);
  } else {
    ST7735_DrawBitmap(56, 150, blackarrow, 20, 16);
    ST7735_DrawBitmap(56, 150, leftarrow, 20, 16);
  }  
}
 

// MODE FUNCTIONS
void main_screen(uint8_t i){
  ST7735_DrawBitmap(4, 124, retrobrowserwindow, 120, 120);
  ST7735_SetCursor(7,5); 
  ST7735_OutString("Fit-Byte");
  ST7735_SetCursor(1,14); 
  printf("%s       Hindi", English[i]); // "/n for new line"
  arrow(language); 
}

uint32_t step_goal;
int xpos = 27;
int textpos = 31;
uint8_t goal_complete_flag = 0;
const char* msg = "CONGRATULATIONS!";
int textIndex = 0;
int textY = 35;
uint8_t cel_count;



void see_count_mode(void) {
  ST7735_FillScreen(0x0000); 
  ST7735_SetCursor(1,1); 
  // screen = 160 x 128, box = 80 x 40
  ST7735_FillRect(44, 40, 40, 80, ST7735_YELLOW);
  ST7735_FillRect(46, 42, 36, 76, ST7735_BLACK);
  while(flag_down == 0) {    
    if(step_count < step_goal) {
      goal_complete_flag = 0;
      ST7735_SetCursor(1,1);   
      printf("%s:\n        %1u/%2u           ", StepCount[language], step_count, step_goal);
      Sound_Start(Trumpet, 32);
      Clock_Delay(10000000);

      if (flag_step != 0) {
        flag_step = 0;

        uint32_t height = (step_count * 76) / step_goal;

        // draw bar
        ST7735_FillRect(44, 40, 40, height, ST7735_YELLOW);
      }

  } else {
    cel_count = 0;
    if (!goal_complete_flag) {
      ST7735_FillScreen(ST7735_BLACK);  // wipe screen once
      goal_complete_flag = 1;
      xpos = (128 - 74) / 2;  // start at center
    }

    Sound_Start(metal, 3525);
    Clock_Delay(10000000);

    // trophy 
    ST7735_DrawBitmap(xpos, 150, trophy, 74, 100);

    //  "CONGRATULATIONS!" 
    int textX = textpos;   
    int textY = 35;
    // clear previous text row (just one 8px-tall line)
    ST7735_FillRect(0, textY, 128, 12, ST7735_BLACK);
    // new text
    ST7735_SetCursor(1, textY / 8);
    for (int i = 0; i < 16; i++) {
      int j = (textIndex + i) % 16;
      printf("%s", Congratulations[language]);
    }

    Clock_Delay1ms(75);
    

    // trophy movement
    xpos += 3;
    if (xpos > 128) {
      xpos = -74;
      cel_count ++;
    }
      if (cel_count == 3) flag_down = 1;

    // text movement
    textIndex++;
    if (textIndex >= 16) textIndex = 0;
  }
}
  flag_return = 1;
}


void set_goal_mode(void) {
  ST7735_FillScreen(0x0000); 
  flag_right = 0;
  while((flag_down == 0)&&(flag_up == 0)) { // maybe add flag up
 // ???
 // DAC_in
 // Convert to step range
  if (slide_flag) {
      Clock_Delay(100000);
      slide_flag = 0;
      GPIOB->DOUTTGL31_0 = RED; // toggle PB26 (minimally intrusive debugging)
      //Output = Convert(Data); // convert Data to Position
      //step_goal = Output / 20;
      step_goal = Convert(Data);
      if (step_goal < 10) {step_goal = 10;}
      ST7735_SetCursor(2,5);
      printf("%s %1u", StepCountGoal[language], step_goal); // ??? change to 2 languages
    }
  }
  if (flag_up){
    flag_up = 0;
    step_count = 0;
    see_count_mode();
  }
  flag_return = 1;
  // will continue back to main if flag_down
}



// REAL MAIN
int main () {
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  UART_Init();
  ST7735_InitPrintf(INITR_BLACKTAB);
    //note: if you colors are weird, see different options for
    // ST7735_InitR(INITR_REDTAB); inside ST7735_InitPrintf()  
    // print an arrow init pointing at English ???
  Switch_Init();
  ADCinit(); //PB18 = ADC1 channel 5, slidepot
  // ST7735_PlotClear(0,2000); // ???
  // initialize interrupts on TimerG12 at 30 Hz
  // 80MHz --> 30Hz.  80MHz/period = 30MHz
  // HERE
  Sound_Init();
  //EdgeTriggered_Init();
  ADC_InitDual(ADC0,7,5,ADCVREF_VDDA); //accelerometer X,Z (analog)
  // ADC0_Init(5,ADCVREF_VDDA); //accelerometer Y (analog)
  TimerG12_IntArm(2666666, 0);
  // initialize semaphore
  slide_flag = 0;
  Time = 0;
  language = 0;
  ST7735_FillScreen(0x0000);       // set screen to black
  main_screen(language);
  __enable_irq();
  while(1){
    // 1. change language
    if(flag_return){
      flag_return = 0;
      ST7735_FillScreen(0x0000);       // set screen to black
      main_screen(language);
    }
    if (flag_down) {
      flag_down = 0;
      language = (language + 1) % 2;
      main_screen(language);
    }

    // 2. enter other modes
    if (flag_up){
      flag_up = 0;
      see_count_mode();
    }
    if (flag_right){
      flag_right = 0;
      set_goal_mode();
    }
  }
}



uint32_t M=1;
uint32_t Random32(void){
  M = 1664525*M+1013904223;
  return M;
}
uint32_t Random(uint32_t n){
  return (Random32()>>16)%n;
}