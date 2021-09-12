#include "adcMultipin.h"
#include <stdbool.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

/* Was macht diese Library? 
 8 pins per ADCA mit nur 4 channeln round-robin messen
 
 Noch nicht implementiert:
 2 channel werden zeitgleich gemessen, die anderen haben zeit das das signal durch den mux propagiert
 #define SAMPLEPIN1 0
 #define SAMPLEPIN2 1
 #define SAMPLEPIN3 2
 #define SAMPLEPIN4 3
 #define SAMPLEPIN5 4
 #define SAMPLEPIN6 5
 #define SAMPLEPIN7 6
 #define SAMPLEPIN8 7
 */

bool second_half = false;
volatile uint16_t adcResultCache[8];

uint8_t ReadCalibrationByte(uint8_t index)
{ 
  uint8_t result;

  /* Load the NVM Command register to read the calibration row. */ 
  NVM_CMD = NVM_CMD_READ_CALIB_ROW_gc; 
  result = pgm_read_byte(index); 

  /* Clean up NVM Command register. */ 
  NVM_CMD = NVM_CMD_NO_OPERATION_gc;
  return result;
}

void SetupAdcMultipin()
{
	ADCA.CALL = ReadCalibrationByte(PRODSIGNATURES_ADCBCAL0); 
	ADCA.CALH = ReadCalibrationByte(PRODSIGNATURES_ADCBCAL1);

// 	ADCA.CTRLB = ADC_IMPMODE_bm | ADC_FREERUN_bm;
	ADCA.EVCTRL = ADC_EVSEL_7_gc | ADC_EVACT_NONE_gc | ADC_SWEEP_0_gc;
	//ADCA.CTRLB = ADC_RESOLUTION_8BIT_gc;
	ADCA.CTRLB = ADC_RESOLUTION_12BIT_gc;
	//ADCA.CTRLB = 0;

	ADCA.REFCTRL = ADC_REFSEL_INTVCC_gc;
	ADCA.PRESCALER = ADC_PRESCALER_DIV128_gc;
	ADCA.CTRLA = ADC_ENABLE_bm;

	ADCA.CH0.CTRL = ADC_CH_INPUTMODE_SINGLEENDED_gc;
	ADCA.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN0_gc;
	ADCA.CH0.INTCTRL = 0;

	ADCA.CH1.CTRL = ADC_CH_INPUTMODE_SINGLEENDED_gc;
	ADCA.CH1.MUXCTRL = ADC_CH_MUXPOS_PIN1_gc;
	ADCA.CH1.INTCTRL = 0;
	
	ADCA.CH2.CTRL = ADC_CH_INPUTMODE_SINGLEENDED_gc;
	ADCA.CH2.MUXCTRL = ADC_CH_MUXPOS_PIN2_gc;
	ADCA.CH2.INTCTRL = 0;
	
	ADCA.CH3.CTRL = ADC_CH_INPUTMODE_SINGLEENDED_gc;
	ADCA.CH3.MUXCTRL = ADC_CH_MUXPOS_PIN3_gc;
	ADCA.CH3.INTCTRL = ADC_CH_INTMODE_COMPLETE_gc | ADC_CH_INTLVL_LO_gc;
	
	ADCA.CTRLA |= ADC_CH0START_bm | ADC_CH1START_bm | ADC_CH2START_bm | ADC_CH3START_bm;
}

uint16_t getAnalogValue(uint8_t pin)
{
	return adcResultCache[pin];
}


ISR(ADCA_CH3_vect)
{
	if (second_half) {
		ADCA.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN4_gc;
		ADCA.CH1.MUXCTRL = ADC_CH_MUXPOS_PIN5_gc;
		ADCA.CH2.MUXCTRL = ADC_CH_MUXPOS_PIN6_gc;
		ADCA.CH3.MUXCTRL = ADC_CH_MUXPOS_PIN7_gc;
		
		adcResultCache[0] = ADCA.CH0RES;
		adcResultCache[1] = ADCA.CH1RES;
		adcResultCache[2] = ADCA.CH2RES;
		adcResultCache[3] = ADCA.CH3RES;

		ADCA.CTRLA = ADC_ENABLE_bm | ADC_CH0START_bm | ADC_CH1START_bm | ADC_CH2START_bm | ADC_CH3START_bm;
	} else {
		ADCA.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN0_gc;
		ADCA.CH1.MUXCTRL = ADC_CH_MUXPOS_PIN1_gc;
		ADCA.CH2.MUXCTRL = ADC_CH_MUXPOS_PIN2_gc;
		ADCA.CH3.MUXCTRL = ADC_CH_MUXPOS_PIN3_gc;
		
		adcResultCache[4] = ADCA.CH0RES;
		adcResultCache[5] = ADCA.CH1RES;
		adcResultCache[6] = ADCA.CH2RES;
		adcResultCache[7] = ADCA.CH3RES;

		ADCA.CTRLA = ADC_ENABLE_bm | ADC_CH0START_bm | ADC_CH1START_bm | ADC_CH2START_bm | ADC_CH3START_bm;
	}
	second_half = !second_half;
}
