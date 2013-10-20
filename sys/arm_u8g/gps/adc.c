/* 
  adc.c
  
  LPC11xx ADC converter

*/

#include <stdint.h>
#include "LPC11xx.h"

uint16_t adc_get_value(uint8_t adc)
{
  uint32_t gdr;
  uint32_t d = 12;

  LPC_SYSCON->SYSAHBCLKCTRL |= 1<<16;	/* enable IOCON clock */
  LPC_SYSCON->SYSAHBCLKCTRL |= 1<<13;	/* enable ADC clock */
  LPC_SYSCON->PDRUNCFG &= ~(1<<4);	/* disable power down ADC */

  adc &= 7;
  
  switch(adc)
  {
    case 0:
      LPC_IOCON->R_PIO0_11 = 2;			/* ADC enable, Analog enable, disable pullup/pulldown */
      break;
    case 1:
      LPC_IOCON->R_PIO1_0 = 2;			/* ADC enable, Analog enable, disable pullup/pulldown */
      break;
    case 2:
      LPC_IOCON->R_PIO1_1 = 2;			/* ADC enable, Analog enable, disable pullup/pulldown */
      break;
    case 3:
      LPC_IOCON->R_PIO1_2 = 2;			/* ADC enable, Analog enable, disable pullup/pulldown */
      break;
    case 4:
      LPC_IOCON->SWDIO_PIO1_3 = 2;			/* ADC enable, Analog enable, disable pullup/pulldown */
      break;
    case 5:
      LPC_IOCON->PIO1_4 = 1;				/* ADC enable, Analog enable, disable pullup/pulldown */
      break;
    default:
      return 0;
  }
  
  /*
    select adc port
    divide freq by d to get <4.5 Mhz at 48 Mhz system clock
    use sw controlled mode
    user 11 clocks per adc measure
    start conversion now
  */
  
  
    LPC_ADC->CR = (1<<adc) | (d<<8) ;
    LPC_ADC->CR |= (1<<24);
    
    /* wait for completion */
    for(;;)
    {
      gdr = LPC_ADC->GDR;
      if ( (gdr & (1UL<<31)) != 0 )
	break;
    }
  
  LPC_SYSCON->PDRUNCFG |= 1<<4;	/* power down ADC */
  LPC_SYSCON->SYSAHBCLKCTRL &= ~(1<<13);	/* disable ADC clock */
  
  return (gdr>>6)&0x03ff;
}
