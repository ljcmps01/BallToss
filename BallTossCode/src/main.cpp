  //////////////////////////////
  //  Descripcion de puertos  //
  //PORTB: 4511(0-3)          //
  //PORTC: POINTS(0-2)        //
  //PORTD: MUX(4-5)           //
  //////////////////////////////


//Agregar comentarios

#include <Arduino.h>

#define DISP PORTB
#define MUX PORTD

const int display[10]={0,1,2,3,4,5,6,7,8,9};

const int m_digit=2; //numero de digitos
volatile int s_digit=0;//seleccion de digito

volatile int score=0; //puntaje
const int m_score=9; //maximo puntaje posible

bool doAni=0;
int loops=6;
int count=20;
int odd=0;
int acum=0;

void multiplex(){
  if (s_digit<(m_digit-1)){
      MUX=0;
      DISP=display[score%10];
      MUX=(1<<(4+s_digit));
      s_digit++;
  }
    else{
      MUX=0;
      DISP=display[score/10];
      MUX=(1<<(4+s_digit));
      s_digit=0;
    } 
}

void blinking(int point){
  MUX=0;
  while(odd<loops){
    DISP=display[point];
    while(acum<count){
      if(odd%2){
        MUX|=(1<<4);
      }
      else{
        MUX&=~(1<<4);
      }
    }
    acum=0;
    odd++;
  }
  doAni=0;
  acum=0;
  odd=0;
}

void scoreCheck(){
  if(score>=m_score){
    score=m_score;
    delay(500);
    doAni=1;
    blinking(0);
    score=0;
  }
  else{
    doAni=0;
    delay(500);
  }
}

void setup() {
  //////////////////////////////
  // Configuracion de puertos //
  //////////////////////////////
  MUX=0;
  DDRD|=48;

  DISP=0;
  DDRB=15;

  PORTC|=7;
  DDRC&=~7;

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
  OCR1A=156;  //Mas o menos 10ms
  TIMSK1|=(1<<1);
}

void loop() {
  switch (PINC)
  {
  case 6:
      doAni=1;
      blinking(1);
      score++;
      scoreCheck();
      break;

  case 5:
      doAni=1;
      blinking(2);
      score+=2;
      scoreCheck();
      break;

  case 3:
      doAni=1;
      blinking(3);
      score+=3;
      scoreCheck();
      break;
  
  default:
    break;
  }



}

ISR(TIMER1_COMPA_vect){
  if(doAni){
    acum++;
  }
  else{
    multiplex();
  }
}


