#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include <pic18fregs.h>

// Set the __CONFIG words:
__code 	 __at (__CONFIG1L) _conf0	= _PLLDIV_DIVIDE_BY_5__20MHZ_INPUT__1L  & _CPUDIV__OSC1_OSC2_SRC___1__96MHZ_PLL_SRC___2__1L & _USBPLL_CLOCK_SRC_FROM_96MHZ_PLL_2_1L;
__code 	 __at (__CONFIG1H) _conf1	= _OSC_HS__HS_PLL__USB_HS_1H;
__code 	 __at (__CONFIG2L) _conf2	= _VREGEN_ON_2L;
__code 	 __at (__CONFIG2H) _conf3	= _WDT_DISABLED_CONTROLLED_2H;
__code 	 __at (__CONFIG3H) _conf4	= _PBADEN_PORTB_4_0__CONFIGURED_AS_DIGITAL_I_O_ON_RESET_3H;
__code 	 __at (__CONFIG4L) _conf5	= _ENHCPU_OFF_4L & _LVP_OFF_4L ;
__code 	 __at (__CONFIG5L) _conf6	= _CP_0_OFF_5L ;

#endif // CONFIG_H_INCLUDED
