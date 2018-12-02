/*
  Control de foco de 100W mediante push buttons para
  incrementar o disminuir brillo usando
  la interrupción externa y el timer 1.
*/
#include <TimerOne.h>

volatile int i = 0;             //Variable a usar como contador
volatile boolean zero_cross = 0; //Boolean para almacenar un "switch" para decirnos si un cruce por cero a ocurrido
int AC_pin = 3;                 //Salida al opto TRIAC
int dim2 = 0;                   //Control luz
int dim = 128;                  //Nivel de intensidad (0-128)  0 = on, 128 = 0ff
int pas = 8;                    //Pasos a contar
int freqStep = 65;              //Este es paso de retardo por brillo en microsegundos para 60Hz

int button1 = 4;                 //Primer push button al pin 4
int button2 = 5;                 //Segundo push button al pin 5

void setup() {
  pinMode(button1, INPUT);                           //Se establece el push button 1 como entrada
  pinMode(button2, INPUT);                           //Se establece el push button 2 como entrada
  pinMode(AC_pin, OUTPUT);                          //Se establece el pin del TRIAC como salida
  attachInterrupt(0, zero_cross_detect, RISING);    //Interrupción externa 0 para el detector de cruce por cero
  Timer1.initialize(freqStep);                      //Se inicializa la librería del Timer 1 para la frecuencia
  Timer1.attachInterrupt(dim_check, freqStep);
  // Use the TimerOne Library to attach an interrupt
}

void zero_cross_detect() {
  zero_cross = true;         //Se establece el boolean a 1 para decirle a la función de nivel de intensidad que un cruce por cero ha ocurrido
  i = 0;
  digitalWrite(AC_pin, LOW);
}

//Activa el TRIAC en el tiempo adecuado del disparo
void dim_check() {
  if (zero_cross == true) {
    if (i >= dim) {
      digitalWrite(AC_pin, HIGH);  //Enciende el foco
      i = 0; //Reset el tiempo de los pasos a contar
      zero_cross = false;  //Reset a la detección de cruce por cero
    }
    else {
      i++;  //Incrementa el tiempo de los pasos a contar
    }
  }
}

void loop() {
  digitalWrite(button1, HIGH); //High
  digitalWrite(button2, HIGH); //High

  if (digitalRead(button1)== LOW) //Low
  {
    if (dim < 127)
    {
      dim = dim + pas;
      if (dim > 127)
      {
        dim = 128;
      }
    }
  }

  if (digitalRead(button2) == LOW) //Low
  {
    if (dim > 5)
    {
      dim = dim - pas;
      if (dim < 0)
      {
        dim = 0;
      }
    }
  }

   while (digitalRead(button1) == LOW) {  }  //LOW
    delay(10); // waiting little bit...
    while (digitalRead(button2) == LOW) {  }  //LOW
    delay(10); // waiting little bit...
  

  dim2 = 255 - 2 * dim;
  if (dim2 < 0)
  {
    dim2 = 0;
  }
}
