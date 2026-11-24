EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 4
Title "Durcretet L.024 : Extension Board"
Date "2022-04-02"
Rev "1"
Comp "David de Lorenzo"
Comment1 ""
Comment2 ""
Comment3 "ISR pins  = D18 - D19 - D20 - D21 (and D2 - D3 )"
Comment4 "PWM pins = D44 - D45 - D46  (and D2 to  D13)"
$EndDescr
Wire Notes Line
	700  600  5000 600 
Wire Notes Line
	5000 600  5000 6750
Wire Notes Line
	5000 6750 700  6750
Wire Notes Line
	700  6750 700  600 
Text Notes 750  700  0    50   ~ 0
ARDUINO MEGA2560
$Comp
L power:VCC #PWR02
U 1 1 5EDA4336
P 1550 2650
F 0 "#PWR02" H 1550 2500 50  0001 C CNN
F 1 "VCC" H 1567 2823 50  0000 C CNN
F 2 "" H 1550 2650 50  0001 C CNN
F 3 "" H 1550 2650 50  0001 C CNN
	1    1550 2650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR01
U 1 1 5EDAD0A2
P 1550 5100
F 0 "#PWR01" H 1550 4850 50  0001 C CNN
F 1 "GND" H 1555 4927 50  0000 C CNN
F 2 "" H 1550 5100 50  0001 C CNN
F 3 "" H 1550 5100 50  0001 C CNN
	1    1550 5100
	1    0    0    -1  
$EndComp
Text GLabel 3500 6200 3    50   Output ~ 0
SCL
Text GLabel 3600 6200 3    50   Output ~ 0
SDA
Text GLabel 2800 4400 2    50   Output ~ 0
MOSI
Text GLabel 1450 4400 0    50   Input ~ 0
MISO
Text GLabel 1450 4500 0    50   Output ~ 0
SCK
Wire Wire Line
	2200 4400 2800 4400
Wire Wire Line
	2350 4600 2200 4600
Text GLabel 3400 1350 1    50   Input ~ 0
A14
Text GLabel 3600 1350 1    50   Input ~ 0
A12
Wire Wire Line
	3400 1650 3400 1350
Wire Wire Line
	3600 1650 3600 1350
Text GLabel 1450 4300 0    50   Output ~ 0
D48
Wire Wire Line
	2200 3000 2800 3000
Wire Wire Line
	2200 3100 2800 3100
Text GLabel 2800 4300 2    50   Output ~ 0
D49
Wire Wire Line
	2200 3200 2800 3200
NoConn ~ 1700 3200
NoConn ~ 1700 3300
NoConn ~ 1700 3400
NoConn ~ 2200 4500
Wire Wire Line
	2300 2900 2200 2900
NoConn ~ 1700 3000
NoConn ~ 1700 3100
NoConn ~ 3800 1650
NoConn ~ 3700 1650
NoConn ~ 3500 1650
NoConn ~ 3300 1650
Wire Wire Line
	3500 6200 3500 5750
Wire Wire Line
	3600 6200 3600 5750
Text GLabel 1450 4100 0    50   Output ~ 0
D44
Text GLabel 1450 4200 0    50   Output ~ 0
D46
Text GLabel 2800 4000 2    50   Input ~ 0
D43
Text GLabel 2800 3900 2    50   Input ~ 0
D41
Text GLabel 2800 3800 2    50   Input ~ 0
D39
$Comp
L Mechanical:MountingHole H1
U 1 1 5EF76CE6
P 750 7450
F 0 "H1" H 850 7496 50  0000 L CNN
F 1 "MountingHole" H 850 7405 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.7mm_M2.5" H 750 7450 50  0001 C CNN
F 3 "~" H 750 7450 50  0001 C CNN
	1    750  7450
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H2
U 1 1 5EF77663
P 1500 7450
F 0 "H2" H 1600 7496 50  0000 L CNN
F 1 "MountingHole" H 1600 7405 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.7mm_M2.5" H 1500 7450 50  0001 C CNN
F 3 "~" H 1500 7450 50  0001 C CNN
	1    1500 7450
	1    0    0    -1  
$EndComp
Wire Wire Line
	1450 4100 1700 4100
Wire Wire Line
	1450 4200 1700 4200
Wire Wire Line
	1450 4400 1700 4400
Wire Wire Line
	1450 4500 1700 4500
Wire Wire Line
	1550 4600 1700 4600
NoConn ~ 1700 3500
Wire Wire Line
	1550 2900 1700 2900
NoConn ~ 1700 3600
NoConn ~ 2200 3400
Text Notes 3300 2100 0    50   ~ 0
A15 ...  ... A10
Text Notes 3450 5450 0    50   ~ 0
D21 ... ... D18
$Comp
L power:GND #PWR0106
U 1 1 6247305B
P 2350 5100
F 0 "#PWR0106" H 2350 4850 50  0001 C CNN
F 1 "GND" H 2355 4927 50  0000 C CNN
F 2 "" H 2350 5100 50  0001 C CNN
F 3 "" H 2350 5100 50  0001 C CNN
	1    2350 5100
	1    0    0    -1  
$EndComp
Wire Wire Line
	1550 4600 1550 5100
Wire Wire Line
	2350 4600 2350 5100
$Comp
L power:VCC #PWR0107
U 1 1 6247ECAC
P 2300 2650
F 0 "#PWR0107" H 2300 2500 50  0001 C CNN
F 1 "VCC" H 2317 2823 50  0000 C CNN
F 2 "" H 2300 2650 50  0001 C CNN
F 3 "" H 2300 2650 50  0001 C CNN
	1    2300 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	2300 2650 2300 2900
Wire Wire Line
	1550 2650 1550 2900
Text GLabel 3800 6200 3    50   Input ~ 0
D18
Text GLabel 3700 6200 3    50   Input ~ 0
D19
Wire Wire Line
	3700 5750 3700 6200
Wire Wire Line
	3800 5750 3800 6200
Text GLabel 1450 3700 0    50   BiDi ~ 0
D36
$Sheet
S 5850 950  3650 800 
U 624BFCA3
F0 "Alimentation" 50
F1 "Alimentation.sch" 50
$EndSheet
Text GLabel 2800 4100 2    50   Output ~ 0
D45
$Sheet
S 5850 2200 3650 1300
U 624DD375
F0 "Connectors" 50
F1 "Connectors.sch" 50
$EndSheet
$Sheet
S 5850 3900 3650 1300
U 6250A423
F0 "Audio" 50
F1 "Audio.sch" 50
$EndSheet
NoConn ~ 2200 3300
NoConn ~ 1700 4000
$Comp
L Mechanical:MountingHole H3
U 1 1 62559996
P 2350 7450
F 0 "H3" H 2450 7496 50  0000 L CNN
F 1 "MountingHole" H 2450 7405 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.7mm_M2.5" H 2350 7450 50  0001 C CNN
F 3 "~" H 2350 7450 50  0001 C CNN
	1    2350 7450
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H4
U 1 1 62559CD8
P 3250 7450
F 0 "H4" H 3350 7496 50  0000 L CNN
F 1 "MountingHole" H 3350 7405 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.7mm_M2.5" H 3250 7450 50  0001 C CNN
F 3 "~" H 3250 7450 50  0001 C CNN
	1    3250 7450
	1    0    0    -1  
$EndComp
NoConn ~ 2200 3600
Wire Wire Line
	2200 4200 2800 4200
Wire Wire Line
	2800 4300 2200 4300
Wire Wire Line
	2800 4100 2200 4100
$Comp
L Connector_Generic:Conn_02x18_Odd_Even J1
U 1 1 5ED8EC88
P 2000 3700
F 0 "J1" H 2050 2700 50  0000 C CNN
F 1 "ARDUINO_DIGITAL" H 2000 2600 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x18_P2.54mm_Vertical" H 2000 3700 50  0001 C CNN
F 3 "~" H 2000 3700 50  0001 C CNN
	1    2000 3700
	-1   0    0    -1  
$EndComp
Text GLabel 2800 4200 2    50   Output ~ 0
D47
Text Notes 3000 4150 0    50   ~ 0
PWM
Text Notes 1050 4150 0    50   ~ 0
PWM\n
Text Notes 1050 4250 0    50   ~ 0
PWM
NoConn ~ 2800 3000
NoConn ~ 2800 3100
NoConn ~ 2800 3200
Wire Wire Line
	2200 3800 2800 3800
Wire Wire Line
	2200 3900 2800 3900
Wire Wire Line
	2200 4000 2800 4000
NoConn ~ 2200 3700
NoConn ~ 2200 3500
$Comp
L Connector_Generic:Conn_01x06 J3
U 1 1 6255C865
P 3600 1850
F 0 "J3" V 3600 2150 50  0000 L CNN
F 1 "ARDUINO_ANALOG" V 3700 1500 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 3600 1850 50  0001 C CNN
F 3 "~" H 3600 1850 50  0001 C CNN
	1    3600 1850
	0    1    1    0   
$EndComp
Wire Wire Line
	1700 4300 1450 4300
NoConn ~ 1700 3900
Wire Wire Line
	1700 3700 1450 3700
NoConn ~ 1700 3800
$Comp
L Connector_Generic:Conn_01x04 J2
U 1 1 62574669
P 3700 5550
F 0 "J2" V 3664 5730 50  0000 L CNN
F 1 "ARDUINO_COMMS" V 3573 5730 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 3700 5550 50  0001 C CNN
F 3 "~" H 3700 5550 50  0001 C CNN
	1    3700 5550
	0    1    -1   0   
$EndComp
$EndSCHEMATC
