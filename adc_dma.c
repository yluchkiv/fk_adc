#include "stm32f3xx.h"
#include "adc_dma.h"

volatile uint32_t adc_value[2];

void gpio_setup(void)                           //pin PA0
{
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;          // ABH enabling and GPIO A enabling
    __IO uint32_t tmpreg = RCC->AHBENR & (~RCC_AHBENR_GPIOAEN); 
    (void)tmpreg;
    GPIOA->MODER |= GPIO_MODER_MODER0;          // 11: Analog mode - for ADC1
}

void adc_setup(void) // ADC1 and 2 
{
    ADC12_COMMON->CCR |= ADC12_CCR_CKMODE_0;    // 0b01 // Set ADC clock
    ADC1->CR &= ~ADC_CR_ADEN;                   // disable the ADC.
    ADC1->CR |= ADC_CR_ADVREGEN_0;              //01: ADC Voltage regulator enabled.

    ADC1->CR |= ADC_CR_ADCAL;                   // start ADC calibration cycle
    while (ADC1->CR & ADC_CR_ADCAL);            // wait for calibration to complete

    ADC1->SQR1 |= (1 << 6); //ADC_SQR1_SQ1_1;               //(1 << 6) 1st conversion in regular sequence!!!

    //ADC12_COMMON->CCR |= ADC_CCR_DUAL_1;        //00110: Regular simultaneous mode only
    //ADC12_COMMON->CCR |= ADC_CCR_DUAL_2;        //00110: Regular simultaneous mode only

    ADC1->CR |= ADC_CR_ADEN;                    // enable the ADC.
    while (!(ADC1->ISR & ADC_ISR_ADRDY));       // check if enabled
  
}

void take_sample(void)
{
    ADC1->CR |= ADC_CR_ADSTART; //start conversion
    adc_value[0] = ADC1->DR;
}
