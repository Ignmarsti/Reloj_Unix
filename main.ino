#include <NTPClient.h> //importamos la librería del cliente NTP
#include <ESP8266WiFi.h> //librería necesaria para la conexión wifi
#include <WiFiUdp.h> // importamos librería UDP para comunicar con 
                     // NTP

//////------------------Variables y bibliotecas para el uso de Max7219--------------////
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
unsigned long tiempoUnix; 
//------------PARA ESP82666-----------//
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW////Especificamos el tipo de hardware. Está biblioteca tiene muy buena documentación para este tipo de cosas:file:///C:/Users/stief/Documents/Arduino/libraries/MD_MAX72XX/docs/class_m_d___m_a_x72_x_x.html
#define MAX_DEVICES 8// Cada matriz de 8x8 se considera un modulo intependiente
//--------Pines para ESP8266--------//
#define CLK_PIN   14
#define DATA_PIN  13
#define CS_PIN    2
//----Hardware SPI connection------//
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);///Creamos un objeto para la comunicación con el hardware

//------datos para conectarnos a nuestra red wifi-----//
const char *ssid     = "Nombre de tu red";
const char *password = "Contraseña de tu red";

//Definimos el cliente udp para su uso con el server NTP
WiFiUDP ntpUDP;

///sopecificamos el servidor al que nos queremos conectar, el offset que tendremos respecto a la hora del servido (en nuestro caso 0) y cada cuanto tiempo queremos actualizar la lectura
NTPClient timeClient(ntpUDP, "hora.roa.es",0,300000);///En nuestro caso nos hemos conectado al ntp de la hora roa, que es la establecida por el Real Instituto y Observatorio de la Armada (Cádiz) y que es la hora oficial es Epaña

void setup() {
  Serial.begin(115200); // activamos la conexión serial
  P.begin();// iniciamos el MAX7219
  WiFi.begin(ssid, password); // nos conectamos al wifi

// Esperamos hasta que se establezca la conexión wifi
 while ( WiFi.status() != WL_CONNECTED ) {///Mientras que no se haya conectado a la red wifi
    delay ( 500 );
    Serial.print ( "." );//Lo indicamos poniendo puntos suspensivos hasta que conecte
  }

  timeClient.begin(); ////Iniciamos el cliente NTP
}

void loop() {
  timeClient.update(); //sincronizamos con el server NTP

  //Imprimimos en nuestra matriz de leds la hora actual
  tiempoUnix = timeClient.getEpochTime();///Devuelve un unsigned long con el tiepo que ha transcurridodesde el 1 de enero de 1970 (medianoche GMT)
  Serial.println(tiempoUnix);///Imprimimos por pantalla y por la matriz de leds.
  P.print(tiempoUnix,DEC);
  
}