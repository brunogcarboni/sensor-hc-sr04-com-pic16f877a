#ifndef LCD4_PINS_H
#define	LCD4_PINS_H
//------------------------------------------------------------------------------
#define RS     RD0         //Register Select
#define EN     RD1         //Enable
#define D4     RD4         //Data pin
#define D5     RD5
#define D6     RD6
#define D7     RD7
//---------------------------DIRECTION------------------------------------------
#define RS_dir TRISD0
#define EN_dir TRISD1
#define D4_dir TRISD4
#define D5_dir TRISD5
#define D6_dir TRISD6
#define D7_dir TRISD7
//------------------------------------------------------------------------------ 
#endif