
/*
 * Simulador de refrigerasion de servidores
 * Por: Hugo Vargas
 * Fecha: 18 de abril 2022 (32°C)
 * 
 * Este programa ejemplifica el uso de operadores relacionales y operadores
 * logicos:
 * 
 * Condiciones Iniciales
 * Sensor de temperatura y humedad
 * Boton1: Manual. Activa la refrigeración al ser presionado
 * Boton2: Alta demanda
 * Boton3: Sobre carga de funcionamiento
 * 
 * Led1: Refrigeracion manual
 * Led2: Refrigeración automática
 * 
 * -Que se encienda la refrigeracion manual cada que se presione el boton de manual
 * -La refrigeracion manual tiene prioridad a la refrigeracion automática.
 * -Si la termperatura es alta (mayor a 28, por ejemplo), se activa la refrigeracion automática
 * -Si tengo alta demanda o sobre carga, se activa la refrigeración automática
 * -Si tengo alta demanda o sobrecarga de funcionamiento y ademas temperatura alta, se activan ambas refrigeraciones
 * 
 * Conexiones propuestas
 * Botones 14, 15, 13
 * Leds 4, 2
 * DHT11 12
 * 
 */

// Bibliotecas
#include "DHT.h"

// Constantes
#define DHTPIN 3
#define DHTTYPE DHT11 

//Constantes de botones
const int BOTON1 = 13;
const int BOTON2 = 12;
const int BOTON3 = 11;
//Constantes de leds
const int LED1 = 4;
const int LED2 = 2;
//Nivel de temperatura
const int TEMP_H = 28;

// Variables
int boton1_dato;
int boton2_dato;
int boton3_dato;
float t;
long timeNow, timeLast; // Variables de control de tiempo no bloqueante
int wait = 2000;  // Indica la espera cada 2 segundos

// Definición de objetos
DHT dht(DHTPIN, DHTTYPE);

// Prototipo de funciones
void lecturaDHT();
void lecturaBotones();
void logica();

// Condiciones iniciales - Se ejecuta sólo una vez al energizar
void setup() {// Inicio de void setup ()
  // Aquí va tu código
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));

  //Configurar botones
  pinMode (BOTON1, INPUT_PULLUP);
  pinMode (BOTON2, INPUT_PULLUP);
  pinMode (BOTON3, INPUT_PULLUP);
  //Configurar Leds
  pinMode (LED1, OUTPUT);
  pinMode (LED2, OUTPUT);

  dht.begin();//Iniciar comunicacion con el sensor DHT
  timeLast = millis (); // Inicia el control de tiempo

}// Fin de void setup

// Cuerpo del programa - Se ejecuta constamente
void loop() {// Inicio de void loop
  // put your main code here, to run repeatedly:
  // delay(2000);
  timeNow = millis(); // Control de tiempo para esperas no bloqueantes
  
  if (timeNow - timeLast > wait) { // Manda un mensaje por MQTT cada cinco segundos
    timeLast = timeNow; // Actualización de seguimiento de tiempo

    lecturaDHT ();
    
  }// Fin de void loop
  lecturaBotones ();
  logica ();
}
// Funcioes del usuario

// Funcion de lectura del sensor DHT11
void lecturaDHT () {
  //*******Lectura del sensor*********
  // Read temperature as Celsius (the default)
  t = dht.readTemperature();
   // Check if any reads failed and exit early (to try again).
  if (isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  Serial.println(t);
}

// Funcion de lectura de botones
void lecturaBotones () {
  //********Lectura de botones********
  boton1_dato = digitalRead (BOTON1);
  boton2_dato = digitalRead (BOTON2);
  boton3_dato = digitalRead (BOTON3);
}

// Funcion de logica
void logica () {

  //********Automático por temperatura, sobrecarga y alta demanda**********
  if (boton1_dato == 1) {
    digitalWrite (LED1, LOW);
  } else {
    digitalWrite (LED1, HIGH);
  }  
  
  if (t > TEMP_H || boton2_dato == 1 || boton3_dato == 1) {
    digitalWrite (LED2, HIGH);
  } else {
    digitalWrite (LED2, LOW);
  }

  /*
   * if (t > TEMP_H) {
   *  digitalWrite (LED2, HIGH);
   *  } else {
   *  digitalWrite (LED2, LOW);
   *  }
   */
}
