/*

  m2esarduinoir.c
  
  m2tklib = Mini Interative Interface Toolkit Library
  
  event source handler for IRremote library (arduino environment)
  http://www.arcfn.com/2009/08/multi-protocol-infrared-remote-library.html
  
  m2tklib contributed code (issue 34)
  
*/

#include "m2.h"
#include <IRremote.h>

int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);
decode_results results;

uint8_t m2_es_arduino_ir(m2_p ep, uint8_t msg) 
{
  switch(msg)
  {
    case M2_ES_MSG_GET_KEY:
      if (irrecv.decode(&results)) 
      {
        irrecv.resume(); // Receive the next value
        switch(results.value) 
        {
          case 0xFFC23D:
            return M2_KEY_EVENT(M2_KEY_SELECT);         /* the M2_KEY_EVENT() macro will  */
          case 0xFFB04F:                                                /* disable the debounce algorithm */
            return M2_KEY_EVENT(M2_KEY_EXIT);
          case 0xFF02FD:
            return M2_KEY_EVENT(M2_KEY_NEXT);
          case 0xFF22DD:
            return M2_KEY_EVENT(M2_KEY_PREV);
          case 0xFF0000:
            return M2_KEY_EVENT(M2_KEY_DATA_UP);
          case 0xFF0001:
            return M2_KEY_EVENT(M2_KEY_DATA_DOWN);
          default:
            return M2_KEY_NONE;
         }
      }
    default:
      return 0;
  }
} 

