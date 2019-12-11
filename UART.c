//
//#include <UART.h>
//
//void EUSCIA2_IRQHandler (void)
//{
//    if (EUSCI_A2->IFG & EUSCI_A_IFG_RXIFG)
//    {
//        if ( RGBcounter < 2 )
//        {
//            ledsRGB[RGBcounter] = EUSCI_A2->RXBUF;
//            RGBcounter++;
//        }
//        else
//        {
//            ledsRGB[2]   =   EUSCI_A2->RXBUF;
//            RGBcounter   =   0;
//            SCB->SCR    &=  ~SCB_SCR_SLEEPONEXIT_Msk;   // Wake the uC up!
////
//
//    }
//}
