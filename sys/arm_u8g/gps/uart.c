/*
  uart.c
  
  LPC11xx UART procedures

*/

/*============================================================*/
/* LPC11xx UART */


#include <stddef.h>
#include "uart.h"
#include "LPC11xx.h"
#include "config.h"


volatile int16_t uart_data;
volatile int32_t uart_byte_cnt = 0;
void (*uart_rx_handler)(uint8_t data) = NULL;

void UART_Init(uint8_t is_48_mhz, void (*rx_handler)(uint8_t data))
{
  LPC_SYSCON->SYSAHBCLKCTRL |= 1<<16;	/* enable IOCON clock */
  LPC_SYSCON->SYSAHBCLKCTRL |= 1<<12;	/* enable UART clock */
#ifdef USE_GPS_4800BAUD  
  LPC_SYSCON->UARTCLKDIV = 2;			/* PCLK = Master Clock / 2 */
#else
  /* default to 9600 */
  LPC_SYSCON->UARTCLKDIV = 1;			/* PCLK = Master Clock / 1 */
#endif  
  
  uart_rx_handler = rx_handler;
  
  /*
    LPC1114:
      TXD PIO1_7
      RXD PIO1_6
  */
  LPC_IOCON->PIO1_6 = 1;		/* connect UART to RXD pin */
  LPC_IOCON->PIO1_7 = 1;		/* connect UART to TXD pin */
  
  /*

    48MHz/4800 			udiv=2, dll=250 divaddval=1, mulval=4

    12MHz/9600			DLM=0,DLL=71,DIVADDVAL=1,MULVAL=10		<===
    48MHz/9600			DLM=0,DLL=250,DIVADDVAL=1,MULVAL=4
    50MHz/9600			DLM=0,DLL=217,DIVADDVAL=5,MULVAL=10

    12MHz/38400			DLM=0,DLL=16,DIVADDVAL=2,MULVAL=9
    48MHz/38400			DLM=0,DLL=71,DIVADDVAL=1,MULVAL=10
    50MHz/38400			DLM=0,DLL=46,DIVADDVAL=10,MULVAL=13

    12MHz/115200			DLM=0,DLL=4,DIVADDVAL=5,MULVAL=8
    48MHz/115200			DLM=0,DLL=22,DIVADDVAL=2,MULVAL=11
    50MHz/115200			DLM=0,DLL=19,DIVADDVAL=3,MULVAL=7
  */
  LPC_UART->LCR = 3 | 128;		/* 8 data bits, one stop bit, enable divider register */
  if ( is_48_mhz != 0 )
    LPC_UART->DLL = 250 /* dll */;
  else
    LPC_UART->DLL = 71 /* dll */;
  LPC_UART->DLM = 0;
  LPC_UART->LCR = 3;			/* 8 data bits, one stop bit, disable divider register */
  if ( is_48_mhz != 0 )
    LPC_UART->FDR  = (4 /* mulval */ << 4) | 1 /* divaddval */;
  else
    LPC_UART->FDR  = (10 /* mulval */ << 4) | 1 /* divaddval */;
  LPC_UART->IER = 0;			/* no interrupts */
  LPC_UART->FCR = 1;			/* FIFO enable, generate IRQ qith one char in the buffer */
  LPC_UART->MCR = 0;  
  LPC_UART->TER = 1<<7;			/* enable transmit */
  LPC_UART->IER = 1;			/* enable receive data interrupt */
  NVIC_EnableIRQ(UART_IRQn);
}

int UART_IsDataAvailable(void)
{
  if ( ( LPC_UART->LSR & 1 ) != 0 )
    return 1;
  return 0;
}

int UART_ReadData(void)
{
  if ( UART_IsDataAvailable() != 0 )
    return LPC_UART->RBR;
  return -1;
}

void __attribute__ ((interrupt, used)) UART_Handler(void)
{
  uint32_t iir = LPC_UART->IIR;
  if ( (iir & 1) == 0 )
  {
    while ( ( LPC_UART->LSR & 1 ) != 0 )
    {
      uart_byte_cnt++;
      uart_data = LPC_UART->RBR;
      // pq_AddChar(&pq, (uint8_t)uart_data);
      if ( uart_rx_handler != NULL )
	uart_rx_handler((uint8_t)uart_data);
    }
  }
}


void UART_SendData(int data)
{
  while( (LPC_UART->LSR & (1<<5)) == 0 )
    ;
  LPC_UART->THR = data;
}

void UART_SendStr(const char *str)
{
  while( *str != '\0' )
    UART_SendData( (int)(unsigned char)*str++ );
}
