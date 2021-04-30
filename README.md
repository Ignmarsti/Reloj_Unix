### **Reloj tiempo Unix**
En este proyecto vamos a construir un reloj en formato unix. El formato Unix es un formato de tiempo que consiste en el conteo de los segundos que han transcurrido desde el medianoche del 1 de enero de 1970.





###**Materiales**
1. NodeMcu ESP8266 12 E
![](https://www.infootec.net/wp-content/uploads/2015/11/nodemcu_i.jpg)
1. Módulo MAX7219 (x8) (en la imagen vienen 4 módulos unidos)
![](https://opencircuit.es/resources/content/8abdbe617bc8a/crop/900-600/MAX7219-4x-Dot-matrix-module.jpg)
1. Wire jumpers hembra-hembra (x4) (como los de la foto de arriba)
1. [IDE de Arduino](https://www.arduino.cc/en/software "IDE de Arduino")

###**Librerías**
1. NTPClient.h
		Librería para conectarse al servidor NTP. Puedes descargarla [aquí](https://github.com/arduino-libraries/NTPClient "aquí")
1. ESP8266WiFi.h
		Librería necesaria para conectarse a la red WiFi. Puedes descargarla [aquí](https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/src/ESP8266WiFi.h "aquí")
1. WiFiUdp.h
		Viene incluida en la librería anterior
1. MD_Parola.h
		Librería para controlar matrices de LED´s conectadas entre si. Puedes descargarla [aquí](https://github.com/MajicDesigns/MD_Parola "aquí")
1. MD_MAX72xx.h
		Librería para controlar una matriz de LED´s de 8x8. Puedes descargarla [aquí](https://github.com/MajicDesigns/MD_MAX72XX "aquí")
1. SPI.h
		Librería necesaria para la comunicación con dispositivos SPI

###**Código**
Primero comenzaremos incluyendo las librerías que controlan la conexión WiFi y NTP:

```C++
#include <NTPClient.h> //importamos la librería del cliente NTP
#include <ESP8266WiFi.h> //librería necesaria para la conexión wifi
#include <WiFiUdp.h> // importamos librería UDP para comunicar con NTP 
            ```
Seguimos incluyendo las libreías para el control de la matriz de LED´s:
```C++
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
```

Vamos a definir las variables necesarias para el correcto funcionamiento de las bibliotecas, así como los puertos en los que conectaremos la matriz de LED´s al NodeMcu. El tipo de hardware que tenemos lo podemos consultar en la librería MD_MAX7219. Puedes consultarla [aquí](file:///C:/Users/stief/Documents/Arduino/libraries/MD_MAX72XX/docs/class_m_d___m_a_x72_x_x.html "aquí"). Recordad que una matriz de 8x8 se considera un solo dispositivo.
```C++
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 8// 
//--------Pines para ESP8266--------//
#define CLK_PIN   14
#define DATA_PIN  13
#define CS_PIN    2
```
Creamos ahora un objeto para la comunicación con el hardware:
```C++
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);```

Tambien debemos definir el cliente udp para poder comunicarnos con el servidor NTP y especificar el servidor al que nos queremos conectar, así como su offset (tiempo de retraso en la hora de nuestro pais respecto a la zona horaria del servidos NTP al que nos hayamos conectado) y la tasa de actualización. En nuestro caso el offset es 0 ya que estamos tomando la hora del servidor NTP del Real Instituto y Observatorio de la Armada (San Fernando, Cadiz)  que es la hora oficial de España. La tasa de actualización recomiendo que sea alta, porque si es demasiado baja se nota visualmente en el paso de los segundos, que va a trompicones
```C++

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "hora.roa.es",0,300000);
```

Definimos las variables que almacenaran el nombre y la contraseña de la red WiFi a la que nos conectaremos, y una variable para almacenar el tiempo en formato unix.
```C++
const char *ssid     = "Nombre de tu red";
const char *password = "Contraseña de tu red";
unsigned long tiempoUnix;
```
En la función setup() iniciaremos la comunicación serial, aunque solo la usaremos a modo de depuración. También iniciaremos comunicación con la matriz de LED´s, conectaremos el NodeMcu a la red WiFi y una vez conectados iniciaremos el cliente NTP:
```C++
void setup() {
  Serial.begin(115200); 
  P.begin();
  WiFi.begin(ssid, password); 

// Esperamos hasta que se establezca la conexión wifi
 while ( WiFi.status() != WL_CONNECTED ) {///Mientras no se conecte 
    delay ( 500 );
    Serial.print ( "." );//Lo indicamos poniendo puntos suspensivos
  }

  timeClient.begin();
}
```
En la función loop() nos sincronizaremos con el servidor NTP, recogeremos el tiempo en la variable tiempoUnix, y la mostraremos por la matriz de LED´s y por el terminal:
```C++
void loop() {

  timeClient.update();
  tiempoUnix = timeClient.getEpochTime(); 
  Serial.println(tiempoUnix);
  P.print(tiempoUnix,DEC);
  
}
```

###**Montaje**
La mayor complicación que he tenido a la hora de montar el dispositivo ha sido a la hora de unir los dos módulos de 8x32 LED´s de los que disponía, ya que los terminales de salia forman 90 grados con respecto a la base, por lo que he tenido que doblarlos hasta el punto de poder enganchar uno con otro. Esto probablemente se podrá hacer mucho mejor con un soldador y algo de estaño.

![](https://scontent-mad1-1.xx.fbcdn.net/v/t1.6435-9/179765178_10219580067474163_3422660642364219507_n.jpg?_nc_cat=111&ccb=1-3&_nc_sid=730e14&_nc_ohc=0zyB0Ets42EAX8dMxlW&_nc_ht=scontent-mad1-1.xx&oh=3e1ff741da129a9f2a02f4aacfec6e00&oe=60B0497F)

En esta foto podeis ver las conexiones descritas anteriormente con la NodeMcu

![](https://scontent-mad1-1.xx.fbcdn.net/v/t1.6435-9/179748231_10219580067554165_2674197942034751265_n.jpg?_nc_cat=100&ccb=1-3&_nc_sid=730e14&_nc_ohc=cI0qpV-vW0oAX9ju56b&_nc_ht=scontent-mad1-1.xx&oh=4f174b51a483946ceba8a437cdfc98b8&oe=60B2591C)

**Podeis ver un video del reloj unix funcionando** [aquí](https://www.youtube.com/watch?v=H0cN6YOR63k "aquí")
