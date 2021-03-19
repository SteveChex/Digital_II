/*
    IE3032 - PROCESAMIENTO DE SEÑALES
    DENNY STEVE OZTOY CHEX, 18476
    LAB 4 - JUEGO DE CARRERAS EN LA TIVA C
*/
//*****************************************************************************************************
//                                          LIBRERIAS
//*****************************************************************************************************


//*****************************************************************************************************
//                              VARIABLES, CONSTANTES Y DEFINICIONES
//*****************************************************************************************************

#define SW_1 PF_4 // Botones incorporados en la TIVA C
#define SW_2 PF_0

#define tiempoHumano 100 // Intervalo de tiempo para hacer Debouncing con la funcion millis()

uint8_t play1 = 0, play2 = 0; // Contadores para los jugadores
bool race = false, on = false; // bandera de inicio de carrera y control de botones

unsigned long time1 = 0, time2 = 0; // para controlar las interrupciones con la funcion millis()

//*****************************************************************************************************
//                                            SETUP
//*****************************************************************************************************

void setup() {
  pinMode(SW_1, INPUT_PULLUP); // Botones incorporados asignados como entradas
  pinMode(SW_2, INPUT_PULLUP);

  pinMode(RED_LED, OUTPUT); // Asignando las leds incorporadas como salidas
  pinMode(GREEN_LED, OUTPUT);

  pinMode(BLUE_LED, OUTPUT); // BANDERA DE ERROR: Si esta encendido SOLO, fallo un switch-case.

  // Interrupciones para controlar a los jugadores
  attachInterrupt(digitalPinToInterrupt(SW_1), intplay1, RISING);
  attachInterrupt(digitalPinToInterrupt(SW_2), intplay2, RISING);

  //Todas las Salidas de leds de contadores
  pinMode(PB_5, OUTPUT);
  pinMode(PB_0, OUTPUT);
  pinMode(PB_1, OUTPUT);
  pinMode(PE_4, OUTPUT);
  pinMode(PE_5, OUTPUT);
  pinMode(PB_4, OUTPUT);
  pinMode(PA_5, OUTPUT);
  pinMode(PA_6, OUTPUT);

  pinMode(PD_0, OUTPUT);
  pinMode(PD_1, OUTPUT);
  pinMode(PD_2, OUTPUT);
  pinMode(PD_3, OUTPUT);
  pinMode(PE_1, OUTPUT);
  pinMode(PE_2, OUTPUT);
  pinMode(PE_3, OUTPUT);
  pinMode(PA_7, OUTPUT);

  clearAll(); // Apagar todas las leds
}

//*****************************************************************************************************
//                                            LOOP
//*****************************************************************************************************

void loop() {
  iniciarCarrera(); // Vigila que se pueda iniciar la carrera
  controlCarrera(); // Inicia la carrera y espera su resultado
}

//*****************************************************************************************************
//                                          FUNCIONES
//*****************************************************************************************************

// Control para iniciar la carrera pulsando AMBOS botones
void iniciarCarrera(void) {
  if (!digitalRead(SW_1) && !digitalRead(SW_2) && !race) {
    race = true; // Si los dos botones se presionan simultaneamente, inicia la carrera
    digitalWrite(BLUE_LED, LOW); // Reiniciando el semaforo
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, LOW);
  }
}

// "Convierte" el contador en una salida de semaforo 
void play1Control(void) {
  switch (play1) {
    case 1:
      digitalWrite(PB_5, HIGH);
      break;
    case 2:
      digitalWrite(PB_0, HIGH);
      break;
    case 4:
      digitalWrite(PB_1, HIGH);
      break;
    case 8:
      digitalWrite(PE_4, HIGH);
      break;
    case 16:
      digitalWrite(PE_5, HIGH);
      break;
    case 32:
      digitalWrite(PB_4, HIGH);
      break;
    case 64:
      digitalWrite(PA_5, HIGH);
      break;
    case 128:
      digitalWrite(PA_6, HIGH);
      break;
    default:
      digitalWrite(BLUE_LED, HIGH); // Caso de error
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(RED_LED, LOW);
      break;
  }
}

// "Convierte" el contador en una salida de semaforo 
void play2Control(void) {
  switch (play2) {
    case 1:
      digitalWrite(PD_0, HIGH);
      break;
    case 2:
      digitalWrite(PD_1, HIGH);
      break;
    case 4:
      digitalWrite(PD_2, HIGH);
      break;
    case 8:
      digitalWrite(PD_3, HIGH);
      break;
    case 16:
      digitalWrite(PE_1, HIGH);
      break;
    case 32:
      digitalWrite(PE_2, HIGH);
      break;
    case 64:
      digitalWrite(PE_3, HIGH);
      break;
    case 128:
      digitalWrite(PA_7, HIGH);
      break;
    default:
      digitalWrite(BLUE_LED, HIGH); // Caso de error
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(RED_LED, LOW);
      break;
  }
}

void ganadorCarrera (void) {
  // Si Azul está encendido junto con Rojo (Fucsia/Morado), gano el jugador 1
  // Si Rojo está encendido junto con Verde (Amarillo), gano el jugador 2
  // Si Azul, Rojo y Verde estan encendidos (~Blanco), es un empate
  if (play1 < play2) {
    digitalWrite(BLUE_LED, HIGH); // Creando color Fucsia
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
  }
  else if (play2 < play1) {
    digitalWrite(BLUE_LED, LOW); // Creando color Amarillo
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, HIGH);
  }
  else if (play2 == play1) {
    digitalWrite(BLUE_LED, HIGH); // Creando color Blanco
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, HIGH);
  }
  on = false; // "desactiva" los botones
}

void clearAll (void) { // Apaga todas las leds
  digitalWrite(PB_5, LOW);
  digitalWrite(PB_0, LOW);
  digitalWrite(PB_1, LOW);
  digitalWrite(PE_4, LOW);
  digitalWrite(PE_5, LOW);
  digitalWrite(PB_4, LOW);
  digitalWrite(PA_5, LOW);
  digitalWrite(PA_6, LOW);

  digitalWrite(PD_0, LOW);
  digitalWrite(PD_1, LOW);
  digitalWrite(PD_2, LOW);
  digitalWrite(PD_3, LOW);
  digitalWrite(PE_1, LOW);
  digitalWrite(PE_2, LOW);
  digitalWrite(PE_3, LOW);
  digitalWrite(PA_7, LOW);
}

void controlCarrera(void) {
  if (race) { // El semaforo combina los colores de la led incorporada
    digitalWrite(RED_LED, HIGH); // Led en Rojo
    delay(1000);
    digitalWrite(GREEN_LED, HIGH); // Led en Amarillo
    delay(1000);
    digitalWrite(RED_LED, LOW); // Led en Verde
    play1 = 1;
    play2 = 1;
    on = true;
    while (race) {
      play1Control(); // Enciende las leds segun el contador del jugador 1
      play2Control(); // Enciende las leds segun el contador del jugador 2
    }
    clearAll(); // apaga todas las leds
    ganadorCarrera(); // Muestra, con colores, al ganador de la carrera

    race = false; // Deshabilita las funciones de la carrera
  } // EL COLOR CAMBIA AL FINAL DE LA CARRERA
}

//*****************************************************************************************************
//                                  FUNCIONES DE INTERRUPCIONES
//*****************************************************************************************************

// Interrupcion del boton 1
void intplay1(void) {
  if (on) {
    if (millis() > time1 + tiempoHumano) { // Revisa que haya pasado un intervalo humanamente realista
      play1 <<= 1; // Incrementar el bit del contador del jugador 1
      if (play1 == 0) {
        race = false; // Termina la carrera si un jugador hace overflow
      }
      time1 = millis(); // Guarda el ultimo tiempo en que este bloque fue ejecutado
    }
  }
}

// Interrupcion del boton 2
void intplay2(void) {
  if (on) {
    if (millis() > time2 + tiempoHumano) { // Revisa que haya pasado un intervalo humanamente realista
      play2 <<= 1; // Incrementar el bit del contador del jugador 2
      if (play2 == 0) {
        race = false; // Termina la carrera si un jugador hace overflow
      }
      time2 = millis(); // Guarda el ultimo tiempo en que este bloque fue ejecutado
    }
  }
}
