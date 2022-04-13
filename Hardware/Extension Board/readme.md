# DUCRETET L.024

## Extension Board

### Principe

Cette carte réalise l'adaptation des différents controles avec les Entrées/Sorties de l'Arduino.

![Extension Board](Extension%20Board.png)

### Fonctionnalités

* Adaptation pour les LEDs
* Adaptation pour les lampes
* Adaptation pour la lecture du condensateur variable
* Adaptation pour lecture du Mode
* Adaptation pour lecture du commutateur Next/Again
* Etage alimentation : 6V / 12V

### Connexions

* **CNX MODE** = 4 pins : GND + Mode1 + Mode2 + Mode3
* **CNX OLED** = 4 pins = VCC + GND + SDA + SCL (bus I2C)
* **CNX NEXT_AGAIN** = 3 pins = GND + Next + Again
* **CNX LEDS** = 5x 2 pins = VCC + LED_n
* **TEST POINTS** = Vin + 6V + 12V + GND
* **JMP_TUNE** = 2 jumpers : Use_C1 + Use_C2
* **CNX TUNE** = 3 pins = C0 + C1 + C2

### Etage Alimentation

* **CNX_LAMPS** = 2 pins = 6V + LAMPS
* **CNX6V ** = 2 pins = VCC + GND  = Wire2board vers Arduino
* **CNX 12V** = 2 pins = 12V + GND  = Wire2board vers Ampli
* **CNX POWER_IN** = 2 pins = VIN + GND  = Wire2board
* **CNX ON_OFF** = 2 pins

## Schemas

Le schema électronique est accessible ici: [Extension board schematic](Extension%20Board.pdf).

Ainsi que que le schéma d'implantation [Extension Board PCB](Extension%20Board%20PCB.pdf).
