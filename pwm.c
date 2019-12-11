#include"pwm.h"

/* Configure TIMER_A0 to produce PWM waveform
 * - TODO: reset R (timer counter) register
 * - TODO: select SMCLK (3MHz) in the CTL register
 * - TODO: select reset/set output mode for T0.1 timer using CCTL[1]
 */

void config_pwm_timer(void)
{
    printf("\r\n\n\nMSP 432 Blink\n\n") ; // title line
    //resets the counter register
TIMER_A0->R=0;
    //sets the SMCLK to the CTL register using the TASSEL_2 shortcut
TIMER_A0->CTL |= TASSEL_2;
//sets the up mode using shortcut from manual
TIMER_A0->CCTL[1]|=OUTMOD_7;




}
/* - TODO: Start PWM signal on Pin XX at duty_cycle 100kHz,
 * Note: the DRV2605L PWM input frequency is XXXX
 * - TODO: calculate and set the amount of ticks needed in CCR
 * - TODO: enable/start timer (UP mode)
 * - TODO: Counting and then reset
 * @param uint8_t duty_cycle: 0-100, percentage of time ON */

//CCR1=CCR0*Duty/100
//P1->IFG&=~0b10
void start_pwm(uint8_t duty_cycle)
{
    printf("\r\n\n\nMSP 432 Blink\n\n") ; // title line
    //creates a variable x as an unsigned 8 bit integer
uint8_t x;
//sets x to the period we want
x=CALC_PERIOD(PWM_FREQUENCY*105000);
//sets CCR0 to the calculated period
TIMER_A0->CCR[0]=1/3000;
//sets CCR1 to the duty cycle using the equation above
TIMER_A0->CCR[1]=x*duty_cycle/100;
//sets the CTL to mode control 1
TIMER_A0->CTL |= TASSEL_2 + MC_1;






}
/* Stop Mode: clear all Mode Control bits, MC, in TAxCTL register */
void stop_pwm(void)
{
    //sets the CTL to mode control 0
TIMER_A0->CTL=MC_0;
}
/* Config P2.4 to output TA0.1 waveform */
void config_pwm_gpio(void)
{

                  // P2.1 selected Timer1_A Out1
      P2->DIR |= BIT4;                  // P2.4 set as output
      P2->SEL0 |= BIT4;
      P2->SEL1 &= ~BIT4;         // P2.4 selected Timer0_A Out1

}
