  //////////////////////////////
  // Descripcion de puertos   //
  //PORTB: 4511(0-3)          //
  //PORTC: POINTS(0-2)        //
  //PORTD: MUX(4-5)           //
  //////////////////////////////


#include <Arduino.h>
//CORREGIR PUERTOS
#define MUX PORTB
#define BUTTON PORTC
#define DISPLAY PORTD 

const int m_digit=4; //numero de digitos
volatile int s_digit=0;//seleccion de digito
void setup() {
  //////////////////////////////
  // Configuracion de puertos //
  //////////////////////////////
  MUX&=~(1<<5);
  DDRB=63;

  //////////////////////////////
  //  Configuracion de timer  //
  //////////////////////////////
  // Limpio registros
  TCCR1A=0;
  TCCR1B=0;
  // Ajusto prescaler
  TCCR1B|=(1<<0)|(1<<2); 
  // Habilito modo de comparacion y valor del registro de comparacion
  TCCR1B|=(1<<3);
  OCR1A=15;  //Mas o menos 10ms
  TIMSK1|=(1<<1);
}

void loop() {
}

ISR(TIMER1_COMPA_vect){
  if (s_digit<m_digit)
  {
    MUX=(1<<s_digit);
    s_digit++;
  }
  else s_digit=0;
  
}