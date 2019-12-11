#include <ssd1306.h>
#include "msp.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


char printNum[80];
char print[80];
char print2[80];
char print3[80];
char print4[80];
char print5[80];
char print6[80];
unsigned char num = 5;
unsigned int mode =1;
uint8_t status;
//global time variables
int hour,minute=0;
int counter, counter2, counter3, counter4, counter5, counter6;
void Port1_Init(void){
  P1->SEL0 = 0x00;
  P1->SEL1 = 0x00;                        // configure P1.4 and P1.1 as GPIO
  P1->DIR = 0x01;                         // make P1.4 and P1.1 in, P1.0 output
  P1->REN = 0x12;                         // enable pull resistors on P1.4 and P1.1
  P1->OUT = 0x12;                         // P1.4 and P1.1 are pull-up
}
uint8_t Port1_Input(void){
  return (P1->IN&0x12);                   // read P1.4,P1.1 inputs
}
//checks if button one on the PCB is pressed
void Checkbutton(void)
{

          if(P4IN==0b100)
                  mode++;


              if (mode>3)
                  mode=1;
}
//continues to increment the clock even in the other modes
void clock(void)
{   //counter6 is the ones digit of the seconds counter5 is the tens digit
       counter6++;
    if (counter6==10)
    {
        counter6=0;
        counter5++;
    }
    if (counter5==6)
    {
        counter5=0;
        minute++;

    }
    if(minute==60)
    {
        //hour++;
    minute=0;
    }
    }
//checks if the second button on the PCB is pressed
uint8_t Checkbutton2(void)
{
    status = Port1_Input();
          if(P4IN==0b10000)
                  return 1;
          else
              return 0;

}
//checks if the third button on the pcb is pressed
uint8_t Checkbutton3(void)
{
    status = Port1_Input();
          if(P4IN==0b1000000)
                  return 1;
          else
              return 0;

}
//checks if the fourth button has been pressed
uint8_t Checkbutton4(void)
{
    status = Port1_Input();
          if(P4IN==0b01)
                  return 1;


          else
              return 0;
}


int main(void) {
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;
        //sets up the properties of the display
        ssdInit();
        //initalizes port 4 to work as an input
        P4SEL0=0x00;
        P4SEL1=0x00;
        P4DIR=0x00;
        P4REN=0x00;
        //clears the display
        cleardisplay (0x00);

    volatile uint32_t i;
    //intialized the buttons on the  pcb for use
    Port1_Init();
    while(!Checkbutton4())

    {
        //this allows the user to set the value of the minutes with the buttons, until button4 is pressed
        draw(0, 5, "Set up minutes for clock", 0 ,0);
                   draw(60, 8, "minutes", 0 ,0);
                   while(123)
                   {
                       if(Checkbutton2())
                           minute+=1;
                       if(Checkbutton3())
                           minute-=1;



                       if(minute>60)
                           minute=0;
                       hour++;
                       if (minute<0)
                           minute=0;
                       if (Checkbutton4())
                           break;
                       sprintf (print, "%d", minute);
                                  draw(45,  8, print, 1, 0);
                         for (i = 27000; i > 0; i--);

                   }

    }

    cleardisplay (0x00);

    while(!Checkbutton4())

        {//same as above but allows for the setup of the hours variable with the buttons
            draw(0, 5, "Set up hours for clock", 0 ,0);
                       draw(60, 8, "hours", 0 ,0);
                       while(123)
                       {   Checkbutton4();
                           if(Checkbutton2())
                               hour+=1;
                           if(Checkbutton3())
                               hour-=1;



                           if(hour>24)
                               hour=0;
                           if (hour<0)
                               hour=0;
                           if (Checkbutton4())
                               break;
                           sprintf (print, "%d", hour);
                                      draw(45,  8, print, 1, 0);
                             for (i = 27000; i > 0; i--);

                       }

        }


    while (1) {
        //draws the semicolons and goes into the switch statement
        draw(60,  5, ":", 1, 0 );

        num = 0;

        switch(mode)
        {

        case 1:
        //watch time mode
            cleardisplay (0x00);

                      //timer mode
            //series of for loops to increment time on watch, second will count up which eventually
            //counts minnutes up then hours. Prints the time to the user.
                      draw(0, 3, "Clock Mode", 0, 0);
                      draw(60,  5, ":", 1, 0 );
                      draw(80,  5, ":", 1, 0 );
                      for (hour; hour<24; hour++)
                          {//hour incrementation
                          if(Checkbutton3())
                              break;
                              sprintf (print, "%d", hour);
                                  draw(45,  5, print, 1, 0);
                                  if(hour==24)
                                      //resets time back to 0
                                      hour=0;
                                  for(minute; minute<60; minute++)
                                  {if(Checkbutton3())
                                      //minute incrementation
                                      break;
                                      sprintf (print3, "%d", minute);
                                      draw(65,  5, print3, 1, 0);
                                   if(minute==60)
                                       //ensures minutes don't overflow
                                       minute=0;

                                     for (counter5=0; counter5<6; counter5++)
                                                            {
                                         if(Checkbutton3())
                                          break;
                                                            sprintf (print5, "%d", counter5);
                                                            draw(87,  5, print5, 1, 0);
                                                            for(counter6=0; counter6<10; counter6++)
                                                            {
                                                                if(Checkbutton3())
                                                                break;
                                                                //second incrementation
                                                                sprintf (print6, "%d", counter6);
                                                                draw(95,  5, print6, 1, 0);
                                                                for (i = 300000; i > 0; i--);
                                                                if(Checkbutton3())

                                                                break;

                                                            }
                                                            }
                                  }
                                  }

                      //constantly checks if button3 is pressed to break out of the loops and to increment mode and swtich statemtent.


                      if(Checkbutton3())
                          mode++;

        break;

        case 2:
            cleardisplay (0x00);
            //timer mode
            draw(60,  5, ":", 1, 0 );
            draw(0, 3, "Timer Mode", 0, 0);
            for (counter=0; counter<3; counter++)
                {
                //increment minutes digit
                if(Checkbutton2())
                 break;
                    sprintf (print, "%d", counter);
                        draw(50,  5, print, 1, 0);
                        for (counter2=0; counter2<6; counter2++)
                        {//increments seconds tens digit
                            if(Checkbutton2())
                            break;
                        sprintf (print2, "%d", counter2);
                        draw(65,  5, print2, 1, 0);
                        for(counter3=0; counter3<10; counter3++)
                        {
                            if(Checkbutton2())
                              break;
                            sprintf (print, "%d", counter3);
                            //increment seconds ones digit
                            draw(75,  5, print, 1, 0);
                            clock();
                            for (i = 300000; i > 0; i--);
                            Checkbutton();
                            if(Checkbutton2())
                             break;
                        }
                        }




                }
            while(mode==2)
               {
                //loops until button is pressed
                   Checkbutton();
                 //power lra
               }

        break;

        case 3:
            cleardisplay (0x00);
            counter=0;
            draw(10, 5, "Set up minutes for Alarm", 0 ,0);
            draw(60, 8, "minutes", 0 ,0);
            //allows user to set up minutes for alarm similar to before
            while(123)
            {   Checkbutton4();
            //increment value up and down
                if(Checkbutton2())
                    counter+=1;
                if(Checkbutton3())
                    counter-=1;


                //cap of 10 min and no lower then 0 minutes
                if(counter>9)
                    counter=0;
                if(counter<0)
                    counter=0;
                if (Checkbutton4())
                    break;
                sprintf (print, "%d", counter);
                           draw(50,  8, print, 1, 0);
                  for (i = 27000; i > 0; i--);

            }
                cleardisplay(0x00);

            draw(0, 3, "Alarm Mode", 0, 0);
            draw(60,  5, ":", 1, 0 );
       // draw(25,  0, printNum, 1, 0);
            counter--;
             for (counter; counter>-1; counter--)
        {//counts down from the minutes set
            sprintf (print, "%d", counter);
                draw(50,  5, print, 1, 0);
                for (counter2=5; counter2>-1; counter2--)
                {
                sprintf (print2, "%d", counter2);
                draw(65,  5, print2, 1, 0);
                for(counter3=9; counter3>-1; counter3--)
                {//increments down the seconds
                    sprintf (print, "%d", counter3);
                    draw(75,  5, print, 1, 0);
                    clock();
                    for (i = 300000; i > 0; i--);

                }
                }


        }
             //sets up the timerA0 to send PWM to buzzer
             config_pwm_gpio();
                 config_pwm_timer();
                 start_pwm(90);

        draw(7, 7, "Wake up sleepy head", 0, 0);
        while(mode==3)
        {
            Checkbutton();
          //power lra
        }
        //Power piezo buzzer or LRA
        //stops buzzer when user changes modes
        stop_pwm();
        hour--;
        minute--;
        break;
       }
    }

}


