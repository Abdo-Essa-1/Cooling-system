#include "avr/io.h" /* To use the ADC Registers */
#include "adc.h"
#include "common_macros.h" /* To use the macros like SET_BIT */

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

void ADC_init(const ADC_ConfigType * Config_Ptr)
{
	/*Select Reference Voltage*/
	ADMUX = (ADMUX & 0X3F) | ((Config_Ptr -> ref_volt) << REFS0);

	/*Select Channel 0 as initialization*/
	ADMUX = (ADMUX & 0XE0) | 0;

	/* ADCSRA Register Bits Description:
	 * ADEN    = 1 Enable ADC
	 * ADIE    = 0 Disable ADC Interrupt
	 * ADATE   = 0 Disable Auto Trigger*/
	ADCSRA = (1 << ADEN);

	/*Set prescalar*/
	ADCSRA = (ADCSRA & 0XF8) | ((Config_Ptr -> prescalar) & 0x07);


}

uint16 ADC_readChannel(uint8 channel_num)
{
	/*Set Channel number and Start Conversion*/
	ADMUX = (ADMUX & 0XE0) | (channel_num & 0x1F);
	SET_BIT(ADCSRA,ADSC); /* Start conversion write '1' to ADSC */
	while(BIT_IS_CLEAR(ADCSRA,ADIF)); /* Wait for conversion to complete, ADIF becomes '1' */
	SET_BIT(ADCSRA,ADIF); /* Clear ADIF by write '1' to it :) */
	return ADC; /* Read the digital value from the data register */
}
