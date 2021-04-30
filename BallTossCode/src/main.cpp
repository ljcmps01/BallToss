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

const int m_digit=3; //numero de digitos
volatile int s_digit=0;//seleccion de digito
int digits[m_digit];

volatile int score=0; //puntaje
const int m_score=50; //maximo puntaje posible

bool doAni=0; //Habilitador de animacion
int loops=6;  //Cantidad de parpadeos de la animacion (cantidad de parpadeos=loop/2)
int count=50;  //Duracion del  on off de la animacion
int odd=0;
int acum=0;

void simplify(int n){
  int acum=0;
  int resto=n;
  while (acum<m_digit)
  {
    digits[acum]=resto%10;
    acum++;
    resto/=10;
  } 
}

//funcion de multiplexado
void multiplex(){
    if (s_digit<m_digit){
        MUX=0;
        DISP=display[digits[s_digit]];
        MUX=(1<<(4+s_digit));
        s_digit++;
    }
      else{
        s_digit=0;
      } 
}


//funcion de animacion de parpadeo 
void blinking(int point){
  MUX=0;
  while(odd<loops){
    //TO-DO: Arreglar para que funcione con simplify()
    simplify(point);
    while(acum<count){
      if(odd%2){
        multiplex();
      }
      else{
        MUX=0;
      }
    }
    acum=0;
    odd++;
  }
  simplify(score);
  doAni=0;
  acum=0;
  odd=0;
}


//Funcion que verifica si se llegó al puntaje maximo y llama a Simplify
void scoreCheck(){
  simplify(score);
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
  OCR1A=80;  //Mas o menos 10ms
  TIMSK1|=(1<<1);

  Serial.begin(9600);
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


