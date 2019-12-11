#include <i2c.h>
#include <ssd1306.h>
#include "msp.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "address.h"
#include "font6x8.h"
#include "font12x16.h"

volatile uint32_t i;
unsigned char data [2];
unsigned char *TI_transmit_field;
unsigned char *dataBuffer;

void ssdInit() {
    Command(SSD1306_DISPLAY_OFF);                                   //Entire Display OFF
    Command(SSD1306_SET_DISPLAY_CLOCK_DIV);                         // Set Display Clock Divide Ratio and Oscillator Frequency
    Command(0x80);                                                  // Default Setting for Display Clock Divide Ratio and Oscillator Frequency that is recommended
    Command(SSD1306_SET_MULTIPLEX_RATIO);                           // Set Multiplex Ratio
    Command(0x3F);                                                  // 64 COM lines
    Command(SSD1306_SET_DISPLAY_OFFSET);                            // Set display offset
    Command(0x00);                                                  // 0 offset
    Command(SSD1306_SET_START_LINE | 0x00);                         // Set first line as the start line of the display
    Command(SSD1306_SET_CHARGE_PUMP);                               // Charge pump
    Command(0x14);                                                  // Enable charge dump during display on
    Command(SSD1306_MEMORY_ADDRESS_MODE);                           // Set memory addressing mode
    Command(SSD1306_SET_LCOL_START_ADDRESS);                        // Horizontal addressing mode
    Command(SSD1306_SEGMENT_REMAP | 0x01);                          // Set segment remap with column address 127 mapped to segment 0
    Command(SSD1306_COM_SCAN_INVERSE);                              // Set com output scan direction, scan from com63 to com 0
    Command(SSD1306_SET_COM_PINS_CONFIG);                           // Set com pins hardware configuration
    Command(0x12);                                                  // Alternative com pin configuration, disable com left/right remap
    Command(SSD1306_SET_CONTRAST);                                  // Set contrast control
    Command(0x80);                                                  // Set Contrast to 128
    Command(SSD1306_SET_PRECHARGE_PERIOD);                          // Set pre-charge period
    Command(0xF1);                                                  // Phase 1 period of 15 DCLK, Phase 2 period of 1 DCLK
    Command(SSD1306_SET_VCOM_DESELECT_LVL);                         // Set Vcomh deselect level
    Command(0x40);                                                  // Vcomh deselect level
    Command(SSD1306_ENTIRE_DISPLAY_RESUME);                         // Entire display ON, resume to RAM content display
    Command(SSD1306_NORMAL_DISPLAY);                                // Set Display in Normal Mode, 1 = ON, 0 = OFF
    Command(SSD1306_DISPLAY_ON);                                    // Display on in normal mode
}

void Command (unsigned char com) {
    data[0] = 0x00;
    data[1] = com;

    sendData(data, 2);
}

void sendData (unsigned char *params, unsigned char flag) {
    TI_transmit_field = params;
    i2c_init ();
    i2c_write (TI_transmit_field,flag);
}
void setCursor (unsigned char x, unsigned char p) {
    Command(SSD1306_SET_LCOL_START_ADDRESS | (x & 0x0F));
    Command(SSD1306_SET_HCOL_START_ADDRESS | (x >> 4));
    Command(SSD1306_SET_PAGE_START_ADDRESS | p);
}
void  pixel(unsigned char x, unsigned char y, unsigned char clear) {

    if ((x >= SSD1306_WIDTH) || (y >= SSD1306_HEIGHT)) return;
    setCursor(x, y >> 3);
    data[0] = SSD1306_DATA_MODE;
    if (clear)
        data[1] = (1 << (y & 7));
    else
        data[1] = ~(1 << (y & 7));
    sendData(data, 2);
}
void cleardisplay(unsigned char param) {
  unsigned char p, x;

  dataBuffer = malloc(129);
  dataBuffer[0] = SSD1306_DATA_MODE;
  for (p = 0; p < 8; p++) {
      setCursor(0, p);
    for (x = 0; x < 128; x++) {
        dataBuffer[x + 1] = param;
    }
    sendData(dataBuffer, 129);
  }
  free(dataBuffer);
}

void draw(unsigned char x, unsigned char p, const char str[],
                        unsigned char invert, unsigned char underline) {
  unsigned char i, j, b, buf[FONT6X8_WIDTH + 1];
  unsigned int c;

  i = 0;
  buf[0] = SSD1306_DATA_MODE; // first item "send data mode"
  while (str[i] != '\0') {
    if (str[i] > 191)
      c = (str[i] - 64) * FONT6X8_WIDTH;
    else
      c = str[i] * FONT6X8_WIDTH;
    if (x > (SSD1306_WIDTH - FONT6X8_WIDTH - 1))
    {
      x = 0;
      p++;
    };
    if (p > 7) p = 0;
    setCursor(x, p);
    for (j = 0; j < FONT6X8_WIDTH; j++)
    {
      if (underline)
        b = font6x8[(unsigned int)(c + j)] | 0x80;
      else
        b = font6x8[(unsigned int)(c + j)];
      if (invert)
        buf[j + 1] = b;
      else
        buf[j + 1] = ~b;
    };
    sendData(buf, FONT6X8_WIDTH + 1); // send the buf to display
    x += FONT6X8_WIDTH;
    i++;
  };
}

