/* ADC1.h
 * Students put your names here
 * Modified: put the date here
 * 12-bit ADC input on ADC1 channel 5, PB18
 */

/**
 * @file      ADC1.h
 * @brief     12-bit ADC input
 * @details   12-bit ADC input on ADC1 channel 5, PB18<br>
 * @version   ECE319K Spring 2025
 * @author    Your names go here
 * @copyright Lab solution, do not post,
 * @warning   AS-IS
 * @note      For more information see  http://users.ece.utexas.edu/~valvano/
 * @date      December 23, 2024
  <table>
<caption id="Lab7pins">Pins for the 12-bit ADC</caption>
<tr><th>Pin <th>GPIO<th>Hardware
<tr><td>PB18 <td>input<td>connected to slide-pot sensor
</table>
  ******************************************************************************/
#ifndef __ADC1_H__
#define __ADC1_H__

/**
 * Initialize 12-bit ADC
 *    - PB18 analog input
 * @param none
 * @return none
 * @brief  Initialize ADC
 * @note LaunchPad_Init has been called; this program should not reset Port B
 */

void ADC_InitDual(ADC12_Regs *adc12,uint32_t channel1,uint32_t channel2, uint32_t reference);
    // Reset ADC and VREF

void ADC_InDual(ADC12_Regs *adc12,int32_t *d1, int32_t *d2); 

void ADCinit(void);
// write code to initialize ADC1 channel 5, PB18
// Your measurement will be connected to PB18
// 12-bit mode, 0 to 3.3V, right justified
// software trigger, no averaging

void ADC_y_Init(void);


/**
 * Input from 12-bit ADC
 * @param none
 * @return sample 12-bit conversion
 * @brief  Input from PB18
 */
uint32_t ADCin(void);
  // write code to sample ADC1 channel 5, PB18 once
  // return digital result (0 to 4095)

uint32_t ADC_y_in(void);

/**
 * Convert raw 12-bit ADC to fixed point distance
 * @param input 12-bit ADC raw sample
 * @return distance as 0.001cm fixed point number
 * @brief  Input from PB18
 */
uint32_t Convert(uint32_t input);
//    determine constants k1 k2 to fit Position=(k1*Data + k2)>>12


/**
 * Convert raw 12-bit ADC to distance
 * @param input 12-bit ADC raw sample
 * @return distance as a floating-point number in cm
 * @brief  Input from PB18
 */
float FloatConvert(uint32_t input);
// do not use this function
// it is added just to show you how SLOW floating point in on a Cortex M0+

#endif // __ADC1_H__
