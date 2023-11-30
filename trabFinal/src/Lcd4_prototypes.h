#ifndef LCD4_PROTOTYPES_H
#define	LCD4_PROTOTYPES_H
//------------------------------------------------------------------------------
#define display_on      0x0C      //Cursor OFF, Blink off
#define display_off     0x08
#define cursor_on       0x0E
#define cursor_blink    0x0F
#define home            0x02
#define clear           0x01
#define auto_increment  0x06
#define auto_decrement  0x04
#define function_set    0x28       //DL = 0 <Data Lenghth> (4 bit mode). N = 1 <Num lines> (2 Lines).
#define font2           function_set | 0x04     //Set bit 3 only.
//------------------------------------------------------------------------------
void LCD_init(void);
void LCD_transmit(char data);
void LCD_data(char data);
void LCD_command(char command);
void LCD_string(char row, char column, char text[]);
void LCD_getBits(char data, char pinBit[]);
void LCD_sendBits(char pinBit[]);
void LCD_pinDirs(void);
void latch(void);
//------------------------------------------------------------------------------
char rowAddr[] = {0x80,0xC0,0x94,0xD4};  
//------------------------------------------------------------------------------
#endif