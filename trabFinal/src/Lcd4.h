#ifndef LCD4_H
#define	LCD4_H
//------------------------------------------------------------------------------
#include "Lcd4_prototypes.h"
#include "Lcd4_pins.h"
//-----------------------Follow Datasheet instructions--------------------------
void LCD_init()
{
  LCD_pinDirs();

  LCD_command(home);
  LCD_command(function_set);   
  LCD_command(display_on);      
  LCD_command(auto_increment);
}
//------------------------------------------------------------------------------
void LCD_command(char command)
{
  RS = 0;                     //Instruction register ( commands ).
  LCD_transmit(command);
}
//------------------------------------------------------------------------------
void LCD_data(char data)
{
  RS = 1;                     //Data register  ( strings and characters ).
  LCD_transmit(data);
}
//------------------------------------------------------------------------------
void LCD_string(char row, char column, char text[])
{
  LCD_command(rowAddr[row] + column);  //Set coordinates.
  while(*text)
  {
    LCD_data(*text++);
  }
}
//------------------------------------------------------------------------------
void LCD_transmit(char data)
{
  char pinBit[8];
   
  LCD_getBits(data,pinBit);
  LCD_sendBits(pinBit);
}
//------------------------------------------------------------------------------
void LCD_getBits(char data, char pinBit[])
{ 
  for(char i = 0; i < 8; i++)
  {
    pinBit[i] = ((data >> i) & 0x01) ? 1 : 0;
  }
}
//------------------------------------------------------------------------------
void LCD_sendBits(char pinBit[])
{
  //Higher Nibble
  D4 = pinBit[4];
  D5 = pinBit[5];
  D6 = pinBit[6];
  D7 = pinBit[7];
  latch();
  
  //Lower Nibble
  D4 = pinBit[0];
  D5 = pinBit[1];
  D6 = pinBit[2];
  D7 = pinBit[3];
  latch();
}
//------------------------------------------------------------------------------
void latch()
{
  EN  = 1;
  __delay_ms(5);
  EN  = 0;
}
//------------------------------------------------------------------------------
void LCD_pinDirs()
{
  RS_dir = 0;
  EN_dir = 0;
  D4_dir = 0;
  D5_dir = 0;
  D6_dir = 0;
  D7_dir = 0;
}
//------------------------------------------------------------------------------
#endif