/*
 * Switch.c
 *
 *  Created on: Nov 5, 2023
 *      Author:
 */
#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
#define PB23INDEX 50
#define PB24INDEX 51
#define PA24INDEX 53 // right
#define PA25INDEX 54 // down
#define PA27INDEX 59 // left
// LaunchPad.h defines all the indices into the PINCM table

// Arm interrupts on fall of PB21
// interrupts will be enabled in main after all initialization



void Switch_Init(void){
    // write this
  IOMUX->SECCFG.PINCM[PB23INDEX] = 0x00040081; // regular GPIO input
  IOMUX->SECCFG.PINCM[PB24INDEX] = 0x00040081; // regular GPIO input

  IOMUX->SECCFG.PINCM[PA24INDEX] = 0x00040081; // GPIO input external resistor //0x00000081; // GPIO output
  IOMUX->SECCFG.PINCM[PA25INDEX] = 0x00040081; // 
  IOMUX->SECCFG.PINCM[PA27INDEX] = 0x00040081; // 

}
// return current state of switches
uint32_t Switch_In(void){
    // write this
    // Button 1 on pin ???
    // Button 1 on pin ???
    // Slider on pin ???
    // Accelerometer on pin ???
    // Heart Rate Sensor on pin ???
  return 0; // replace this line
}