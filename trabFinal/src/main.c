/*
 * File:   Projetinho Felas - Operção Salva Galinha.
 * Authors: Bruno Binelli, Bruno Carboni e Gregory.
 *
 * Created on 14 de Novembro de 2023.
 */

#define RS RD0
#define EN RD1
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7

#define _XTAL_FREQ 8000000 //Define a frequencia do clock - 8Mhz.

#define TRIGGER_PIN   RB0
#define ECHO_PIN      RB4 


#include <xc.h>             //Inclusão da blibioteca do compilador.
#include <pic16f877a.h>     //Inclusão da biblioteca do microcontrolador utilizado.
#include <stdio.h>
#include "Lcd4.h"

//configuração da palavra de programa.
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT enable)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

int dist;
char buffer[20];

void __interrupt() TrataInt()
{
    if(INTCONbits.RBIF == 1)//Checa interrupção veio do pino RB0
    {
        INTCONbits.RBIE = 0; //Disabilita interrupção RB0.
        if(ECHO_PIN == 1)
            T1CONbits.TMR1ON = 1; //Começa o timer.
        if(ECHO_PIN == 0)
        {
           T1CONbits.TMR1ON = 0; //desliga timer
           
           //Explicação da leitura e do cálculo ao fim do código.
           dist = (TMR1L | (TMR1H <<8)) / 58.82; 
           dist = dist + 1; //Arredonda valor.
        }       
    }
    INTCONbits.RBIF = 0;    //limpa flag RB0
    INTCONbits.RBIE = 1;    //habilita interrupção pelo pino RB0.
}


void main()
{
    //---- Config de Portas ----;
    TRISBbits.TRISB0 = 0;   //Define Porta RB0 'TRIGGER_PIN' como saída.
    TRISBbits.TRISB4 = 1;   //Define Porta RB4 'ECHO_PIN' como entrada.
    TRISC = 0;
    PORTC = 0;
    

    //---- Config de Interrupção ----;
    INTCONbits.GIE = 1;      //Ativa Interrupção Global.
    INTCONbits.RBIF = 0;    //Limpa flag de interrupção RB0.
    INTCONbits.RBIE = 1;    //Ativa interrupção pino RB0.
    
    //---- Config do TIMER1 ----;   
    //Pre-scale: 1:2.
    T1CONbits.T1CKPS1 = 0;
    T1CONbits.T1CKPS0 = 1;
    
    LCD_init();
    
    while(1)
    {
        TMR1H = 0;          //Seta valor inicial do timer 1.
        TMR1L = 0;
        
        TRIGGER_PIN = 1;
        __delay_us(10);
        TRIGGER_PIN = 0;
         
        sprintf(buffer, "Distance = %dcm", dist);
        LCD_string(0,0,buffer); 
            
        if(dist <= 200 && dist>= 2 && dist<= 400) //Se o sensor captar algo
        //dentro do intervalo de leitura do sensor [2cm - 400cm], numa distãncia menor que 2m, acende as luzes.
        {
            PORTCbits.RC1 = 1;
            PORTCbits.RC2 = 1;
        }
        else
        {
           PORTCbits.RC1 = 0;
           PORTCbits.RC2 = 0;
        }
        
        if(dist <= 150 && dist >= 2 && dist<= 400) //Se o sensor captar algo 
        //dentro do intervalo de leitura do sensor [2cm - 400cm], numa distância menor que 1.5m, acende o buzzer.
        {
            PORTCbits.RC0 = 1;
        }
        else
        {
            PORTCbits.RC0 = 0;
        }
        
        if(dist < 2 || dist > 400) //Se o sensor tiver fora do intervalo escreve
            
        {
            LCD_command(clear);
            
            while(1)
            {
                if(dist > 2 || dist < 400)
                {
                    LCD_string(0,0,"Out of RanGe!");
                    break;
                }
            }
        }
       
        
        __delay_ms(400);
        
    }
    
    
    return;
}

/* Leitura do tempo:
 Realiza a leitura do timer realizando a operação de 'or' entre os 8 bits mais LSB do timer1 e 8 bits mais MSB do timer1. ex:.
 EXEMPLO:
 * Caso TMR1H=0x0A "aux_timer" ser� 0x0A00 (lembrando que "aux_timer" possui 16 bits).
   Se TMR1L=0xFF, com TMR1 igual o de cima, "aux_timer" ser� 0x0AFF.       
 */


/* Cãlculo da distância:
         
    distãncia = Velocidade * tempo.
    distãncia do objeto até o sensor =  Velocidade * tempo / 2; Onda vai e bate e volta.
         
    Velocidade da onda emitida pelo sensor no ar = 340 m/s = 34000 cm/s.
         
    Tempo = Valor do obtido pelo timer(VT) * Periodo do clock * Pre-scale
    * Periodo do clock = 1/(Fosc/4) = 1/(8MHz/4) = 1/2MHz = 0,5x10^-6 s.
    * Pre-scale = 2
    * 
    * Tempo = VT * 0,5 * 10^-6 * 2 =  VT * 1/10^6 = VT * 0.000001 s = VT * 1 us
         
    distancia = 34000 * VT * 1 * 10^-6 / 2 = VT * 0.0170 cm = VT * 1/58.82 = VT/58.82 .
*/