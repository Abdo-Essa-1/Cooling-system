#ifndef ADC_H_
#define ADC_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define ADC_MAXIMUM_VALUE    1023
#define ADC_REF_VOLT_VALUE   5


/*******************************************************************************
 *                                Datatypes                                  *
 *******************************************************************************/

typedef enum {
	EXTERNAL_AREF, INTERNAL_AVCC,INTTERNAL_SMALL_VOLT = 3
}ADC_ReferenceVoltage;

typedef enum{
	PRESCALAR_2 = 1, PRESCALAR_4, PRESCALAR_8, PRESCALAR_16, PRESCALAR_32, PRESCALAR_64, PRESCALAR_128
}ADC_Prescalar;

/*To initialize ADC with*/
typedef struct{
	ADC_ReferenceVoltage ref_volt;
	ADC_Prescalar prescalar;
}ADC_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Function responsible for initialize the ADC driver.
 */
void ADC_init(const ADC_ConfigType * Config_Ptr);

/*
 * Description :
 * Function responsible for read analog data from a certain ADC channel
 * and convert it to digital using the ADC driver.
 */
uint16 ADC_readChannel(uint8 channel_num);

#endif /* ADC_H_ */
