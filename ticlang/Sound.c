// Sound.c
// Runs on MSPM0
// Sound assets in sounds/sounds.h
// your name
// your data 
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "Sound.h"
#include "../inc/DAC5.h"
#include "../inc/Timer.h"

uint8_t *sound_pt; 
uint32_t soundIndex;
uint32_t length;

void SysTick_IntArm(uint32_t period, uint32_t priority){

  SysTick->CTRL = 0;

  SysTick->LOAD = period-1;
  SysTick->VAL = 0;

  SCB->SHP[1] = (SCB->SHP[1]&(~0xC0000000))|(priority<<30); // priority 2

  SysTick->CTRL = 0x0007;
}

void Sound_Init(void){
// write this
  SysTick_IntArm(80000000/11000, 0x02);
  DAC5_Init();
}
void SysTick_Handler(void){ // called at 11 kHz
  if(soundIndex == length){
    SysTick->LOAD = 0;
  }else{
    DAC5_Out(*sound_pt);
    sound_pt ++;
    soundIndex ++;
  }
}

//Global variable telling it how long the sound is
//******* Sound_Start ************
// This function does not output to the DAC. 
// Rather, it sets a pointer and counter, and then enables the SysTick interrupt.
// It starts the sound, and the SysTick ISR does the output
// feel free to change the parameters
// Sound should play once and stop
// Input: pt is a pointer to an array of DAC outputs
//        count is the length of the array
// Output: none
// special cases: as you wish to implement

void Sound_Start(const uint8_t *pt, uint32_t count){
// write this
  //SysTick->LOAD = 80000000/11000;
    SysTick->LOAD = 7200-1;
  sound_pt = pt;
  length = count;
  soundIndex = 0;
}
//void Sound_Shoot(void){
// write this
//   Sound_Start( shoot, 4080);
// }
// void Sound_Killed(void){
// // write this
//   Sound_Start(invaderkilled, 3377 );

// }
// void Sound_Explosion(void){
// // write this
//   Sound_Start(explosion,8731);

// }

// void Sound_Fastinvader1(void){
//   Sound_Start(fastinvader1, 982);
// }
void Sound_Fastinvader2(void){

}
void Sound_Fastinvader3(void){

}
void Sound_Fastinvader4(void){

}
void Sound_Highpitch(void){

}
