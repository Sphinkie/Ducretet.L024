# DUCRETET L.024

## Extension Board

### Principe

Cette carte réalise l'adaptation des différents controles avec les Entrées/Sorties de l'Arduino.



## Fonctionnalités

* Adaptation pour les LEDs
* Adaptation pour les lampes
* Adaptation pour la lecture du condensateur variable
* Adaptation pour lecture du Mode
* Adaptation pour lecture du commutateur Next/Again
* Etage alimentation : 6V / 12V

## Connexions

* **CNX_MODE** = 4 pins : ? + Mode1 + Mode2 + Mode3

* **CNX_OLED** = 4 pins = VCC + GND + SDA + SCL (bus I2C)

* **CNX_NEXT_AGAIN** = 3 pins = ? + Next + Again

* **CNX_LED_n** = 6x 2 pins = ? + LED_n

* **TST** = 6V + 12V + GND

* **JMP_TUNE** = 2 jumpers : Use_C1 + Use_C2

* **CNX_TUNE** = 3 pins = C0 + C1 + C2

* **CNX_LAMP_1** = 2 pins = 5V + LAMP_1

* **CNX_LAMP_2** = 2 pins = 5V + LAMP_2

* **JMP_POWER** = 2 jumpers = Use_5V + Use_9V            ..... TBC

* **CNX_POWER_ARD** = 2 pins = VCC + GND  = Wire2board

* **CNX_POWER_AMP** = 2 pins = 12V + GND  = Wire2board

* **CNX_POWER_IN** = 2 pins = VIN + GND  = Wire2board

* **CNX_ON_OFF** = 2 pins

  



### Open Points

* Le 5V est-il nécessaire ? Peut-on se limiter au 6V ?

* Les lampes supportent-elles le 9V ? le 6V ?

* Générer une BOL en html

  

