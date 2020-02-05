/*
 * lcd.h
 *
 *  Created on: Dec 18, 2013
 *      Author: Bob
 */

#ifndef LCD_H_
#define LCD_H_

typedef unsigned char byte;

void Lcd_Init();
void Lcd_Cmd(byte cmd);
void Lcd_Data(byte data);
void Lcd_PutCh(int row, int col, char ch);
void Lcd_PutStr(int row, int col, char* str);
void _delay_us(unsigned long delay);
void _delay_ms(unsigned long delay);

#endif /* LCD_H_ */
