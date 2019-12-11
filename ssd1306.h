
#ifndef SSD1306_H_
#define SSD1306_H_

void ssdInit();
void sendCommand (unsigned char command);
void sendData (unsigned char params [], unsigned char flag);
void setCursor (unsigned char x, unsigned char p);
void darwPixel (unsigned char x, unsigned char y, unsigned char clear);
void fillDisplay(unsigned char param);

void draw6x8Str(unsigned char x, unsigned char p, const char str[],
                        unsigned char invert, unsigned char underline);

#endif /* SSD1306_H_ */
