EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 4
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Notes 900  900  0    50   ~ 0
Adaptation pour les LEDs
$Comp
L Device:R R?
U 1 1 624E1685
P 2150 1700
AR Path="/624E1685" Ref="R?"  Part="1" 
AR Path="/624DD375/624E1685" Ref="R3"  Part="1" 
F 0 "R3" V 2150 1700 50  0000 C CNN
F 1 "200" V 2100 1900 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 2080 1700 50  0001 C CNN
F 3 "~" H 2150 1700 50  0001 C CNN
	1    2150 1700
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 624E168B
P 2150 1800
AR Path="/624E168B" Ref="R?"  Part="1" 
AR Path="/624DD375/624E168B" Ref="R4"  Part="1" 
F 0 "R4" V 2150 1800 50  0000 C CNN
F 1 "200" V 2100 2000 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 2080 1800 50  0001 C CNN
F 3 "~" H 2150 1800 50  0001 C CNN
	1    2150 1800
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 624E1691
P 2150 1900
AR Path="/624E1691" Ref="R?"  Part="1" 
AR Path="/624DD375/624E1691" Ref="R5"  Part="1" 
F 0 "R5" V 2150 1900 50  0000 C CNN
F 1 "200" V 2100 2100 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 2080 1900 50  0001 C CNN
F 3 "~" H 2150 1900 50  0001 C CNN
	1    2150 1900
	0    1    1    0   
$EndComp
Wire Wire Line
	2300 1900 3100 1900
$Comp
L Device:R R?
U 1 1 624E1698
P 2150 1500
AR Path="/624E1698" Ref="R?"  Part="1" 
AR Path="/624DD375/624E1698" Ref="R1"  Part="1" 
F 0 "R1" V 2150 1500 50  0000 C CNN
F 1 "200" V 2100 1700 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 2080 1500 50  0001 C CNN
F 3 "~" H 2150 1500 50  0001 C CNN
	1    2150 1500
	0    1    1    0   
$EndComp
Text GLabel 1800 1700 0    50   Input ~ 0
D49
Text GLabel 1800 1800 0    50   Input ~ 0
D46
Wire Wire Line
	1800 1800 2000 1800
Text GLabel 1800 1900 0    50   Input ~ 0
D44
Wire Wire Line
	1800 1900 2000 1900
Text GLabel 1800 1500 0    50   Input ~ 0
D45
Wire Wire Line
	1800 1500 2000 1500
Wire Wire Line
	1800 1700 2000 1700
Text Notes 1600 1550 2    50   ~ 0
PWM
Text Notes 1600 1850 2    50   ~ 0
PWM
Text Notes 1600 1950 2    50   ~ 0
PWM
Wire Wire Line
	2300 1500 3100 1500
$Comp
L power:VCC #PWR?
U 1 1 624E16AB
P 4100 1250
AR Path="/624E16AB" Ref="#PWR?"  Part="1" 
AR Path="/624DD375/624E16AB" Ref="#PWR0122"  Part="1" 
F 0 "#PWR0122" H 4100 1100 50  0001 C CNN
F 1 "VCC" H 4117 1423 50  0000 C CNN
F 2 "" H 4100 1250 50  0001 C CNN
F 3 "" H 4100 1250 50  0001 C CNN
	1    4100 1250
	1    0    0    -1  
$EndComp
Wire Wire Line
	2300 1700 3100 1700
$Comp
L Device:R R?
U 1 1 624E16B2
P 2150 1600
AR Path="/624E16B2" Ref="R?"  Part="1" 
AR Path="/624DD375/624E16B2" Ref="R2"  Part="1" 
F 0 "R2" V 2150 1600 50  0000 C CNN
F 1 "200" V 2100 1800 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 2080 1600 50  0001 C CNN
F 3 "~" H 2150 1600 50  0001 C CNN
	1    2150 1600
	0    1    1    0   
$EndComp
Wire Wire Line
	2300 1600 3100 1600
Text GLabel 1800 1600 0    50   Input ~ 0
D47
Wire Wire Line
	1800 1600 2000 1600
$Comp
L Connector_Generic:Conn_02x05_Odd_Even J?
U 1 1 624E16BB
P 3300 1700
AR Path="/624E16BB" Ref="J?"  Part="1" 
AR Path="/624DD375/624E16BB" Ref="J15"  Part="1" 
F 0 "J15" H 3350 2117 50  0000 C CNN
F 1 "CNX LEDS" H 3350 2026 50  0000 C CNN
F 2 "MesModules:HE10-10P-horizontal" H 3300 1700 50  0001 C CNN
F 3 "~" H 3300 1700 50  0001 C CNN
	1    3300 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	2300 1800 3100 1800
Wire Wire Line
	3600 1900 4100 1900
Wire Wire Line
	4100 1250 4100 1500
Wire Wire Line
	3600 1800 4100 1800
Connection ~ 4100 1800
Wire Wire Line
	4100 1800 4100 1900
Wire Wire Line
	3600 1700 4100 1700
Connection ~ 4100 1700
Wire Wire Line
	4100 1700 4100 1800
Wire Wire Line
	3600 1600 4100 1600
Connection ~ 4100 1600
Wire Wire Line
	4100 1600 4100 1700
Wire Wire Line
	3600 1500 4100 1500
Wire Wire Line
	4100 1500 4100 1600
Connection ~ 4100 1500
Text GLabel 1350 5950 1    50   Output ~ 0
MISO
Text GLabel 1550 5950 1    50   Input ~ 0
MOSI
Text GLabel 1450 5950 1    50   Input ~ 0
SCK
Wire Wire Line
	1650 5950 1650 6800
Wire Wire Line
	1550 5950 1550 6800
Wire Wire Line
	1450 5950 1450 6800
$Comp
L power:VCC #PWR0103
U 1 1 624E208D
P 1750 6200
F 0 "#PWR0103" H 1750 6050 50  0001 C CNN
F 1 "VCC" H 1767 6373 50  0000 C CNN
F 2 "" H 1750 6200 50  0001 C CNN
F 3 "" H 1750 6200 50  0001 C CNN
	1    1750 6200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0104
U 1 1 624E2093
P 1250 6200
F 0 "#PWR0104" H 1250 5950 50  0001 C CNN
F 1 "GND" H 1255 6027 50  0000 C CNN
F 2 "" H 1250 6200 50  0001 C CNN
F 3 "" H 1250 6200 50  0001 C CNN
	1    1250 6200
	-1   0    0    1   
$EndComp
$Comp
L Connector_Generic:Conn_01x06 J4
U 1 1 624E2099
P 1550 7000
F 0 "J4" V 1550 6500 50  0000 L CNN
F 1 "BUS SPI" V 1650 6800 50  0000 L CNN
F 2 "Connector_JST:JST_XH_B6B-XH-AM_1x06_P2.50mm_Vertical" H 1550 7000 50  0001 C CNN
F 3 "~" H 1550 7000 50  0001 C CNN
	1    1550 7000
	0    1    1    0   
$EndComp
Wire Wire Line
	1350 5950 1350 6800
Text GLabel 1650 5950 1    50   Input ~ 0
D48
Wire Wire Line
	1250 6200 1250 6800
Wire Wire Line
	1750 6200 1750 6800
Wire Wire Line
	2700 6200 2700 6800
$Comp
L power:GND #PWR?
U 1 1 624E8E57
P 2700 6200
AR Path="/624E8E57" Ref="#PWR?"  Part="1" 
AR Path="/624DD375/624E8E57" Ref="#PWR0105"  Part="1" 
F 0 "#PWR0105" H 2700 5950 50  0001 C CNN
F 1 "GND" H 2705 6027 50  0000 C CNN
F 2 "" H 2700 6200 50  0001 C CNN
F 3 "" H 2700 6200 50  0001 C CNN
	1    2700 6200
	-1   0    0    1   
$EndComp
$Comp
L Connector_Generic:Conn_01x03 J?
U 1 1 624E8E5D
P 2800 7000
AR Path="/624E8E5D" Ref="J?"  Part="1" 
AR Path="/624DD375/624E8E5D" Ref="J7"  Part="1" 
F 0 "J7" V 2800 6750 50  0000 L CNN
F 1 "I/O SPARE" V 2950 6800 50  0000 L CNN
F 2 "Connector_JST:JST_XH_B3B-XH-AM_1x03_P2.50mm_Vertical" H 2800 7000 50  0001 C CNN
F 3 "~" H 2800 7000 50  0001 C CNN
	1    2800 7000
	0    1    1    0   
$EndComp
$Comp
L power:VCC #PWR?
U 1 1 624E8E63
P 2900 6200
AR Path="/624E8E63" Ref="#PWR?"  Part="1" 
AR Path="/624DD375/624E8E63" Ref="#PWR0108"  Part="1" 
F 0 "#PWR0108" H 2900 6050 50  0001 C CNN
F 1 "VCC" H 2917 6373 50  0000 C CNN
F 2 "" H 2900 6200 50  0001 C CNN
F 3 "" H 2900 6200 50  0001 C CNN
	1    2900 6200
	1    0    0    -1  
$EndComp
Wire Wire Line
	2900 6800 2900 6200
$Comp
L Device:R R?
U 1 1 624E8E6A
P 2800 6400
AR Path="/624E8E6A" Ref="R?"  Part="1" 
AR Path="/624DD375/624E8E6A" Ref="R6"  Part="1" 
F 0 "R6" H 2750 6550 50  0000 C CNN
F 1 "200" V 2800 6400 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 2730 6400 50  0001 C CNN
F 3 "~" H 2800 6400 50  0001 C CNN
	1    2800 6400
	-1   0    0    1   
$EndComp
Wire Wire Line
	2800 6800 2800 6550
Wire Wire Line
	2800 6250 2800 5950
Text GLabel 2800 5950 1    50   BiDi ~ 0
D36
Text Notes 1100 5500 0    50   ~ 0
Spare connectors
Wire Notes Line
	5250 2300 7500 2300
Wire Notes Line
	7500 2300 7500 700 
Wire Notes Line
	7500 700  5250 700 
Text Notes 5300 900  0    50   ~ 0
Adaptation pour \nRotary Button
$Comp
L power:VCC #PWR?
U 1 1 624FC98A
P 6400 950
AR Path="/624FC98A" Ref="#PWR?"  Part="1" 
AR Path="/624DD375/624FC98A" Ref="#PWR0109"  Part="1" 
F 0 "#PWR0109" H 6400 800 50  0001 C CNN
F 1 "VCC" H 6417 1123 50  0000 C CNN
F 2 "" H 6400 950 50  0001 C CNN
F 3 "" H 6400 950 50  0001 C CNN
	1    6400 950 
	1    0    0    -1  
$EndComp
Wire Wire Line
	6400 950  6400 1050
Wire Wire Line
	6400 1450 6400 1700
Wire Wire Line
	6500 1450 6500 1800
Wire Wire Line
	6600 1450 6600 1900
NoConn ~ 6700 1450
$Comp
L Device:R_Network04 RN?
U 1 1 624FC995
P 6600 1250
AR Path="/624FC995" Ref="RN?"  Part="1" 
AR Path="/624DD375/624FC995" Ref="RN1"  Part="1" 
F 0 "RN1" H 6788 1296 50  0000 L CNN
F 1 "10k" H 6788 1205 50  0000 L CNN
F 2 "Resistor_THT:R_Array_SIP5" V 6875 1250 50  0001 C CNN
F 3 "http://www.vishay.com/docs/31509/csc.pdf" H 6600 1250 50  0001 C CNN
	1    6600 1250
	1    0    0    -1  
$EndComp
Wire Wire Line
	6600 2000 6600 2100
$Comp
L power:GND #PWR?
U 1 1 624FC99C
P 6600 2100
AR Path="/624FC99C" Ref="#PWR?"  Part="1" 
AR Path="/624DD375/624FC99C" Ref="#PWR0110"  Part="1" 
F 0 "#PWR0110" H 6600 1850 50  0001 C CNN
F 1 "GND" H 6600 1950 50  0000 C CNN
F 2 "" H 6600 2100 50  0001 C CNN
F 3 "" H 6600 2100 50  0001 C CNN
	1    6600 2100
	1    0    0    -1  
$EndComp
Wire Notes Line
	5250 700  5250 2300
Wire Wire Line
	6950 2000 6600 2000
$Comp
L Connector_Generic:Conn_01x04 J?
U 1 1 624FC9A4
P 7150 1900
AR Path="/624FC9A4" Ref="J?"  Part="1" 
AR Path="/624DD375/624FC9A4" Ref="J5"  Part="1" 
F 0 "J5" H 7200 1600 50  0000 R CNN
F 1 "MODE" H 7300 2100 50  0000 R CNN
F 2 "Connector_JST:JST_XH_B4B-XH-A_1x04_P2.50mm_Vertical" H 7150 1900 50  0001 C CNN
F 3 "~" H 7150 1900 50  0001 C CNN
	1    7150 1900
	1    0    0    1   
$EndComp
Text GLabel 5500 1900 0    50   Output ~ 0
D39
Text GLabel 5500 1800 0    50   Output ~ 0
D41
Text GLabel 5500 1700 0    50   Output ~ 0
D43
Connection ~ 6400 1700
Wire Wire Line
	6400 1700 6950 1700
Connection ~ 6500 1800
Wire Wire Line
	6500 1800 6950 1800
Connection ~ 6600 1900
Wire Wire Line
	6600 1900 6950 1900
Wire Wire Line
	5500 1900 6600 1900
Wire Wire Line
	5500 1700 6400 1700
Wire Wire Line
	5500 1800 6500 1800
Wire Notes Line
	4850 700  4850 2750
Wire Notes Line
	4850 2750 800  2750
Wire Notes Line
	800  2750 800  700 
Wire Notes Line
	800  700  4850 700 
Text Notes 7400 3800 0    50   ~ 0
Adaptation pour Push Buttons \n(avec IRQ)
Wire Notes Line
	7350 3600 7350 6150
Wire Notes Line
	7350 6150 11100 6150
Wire Notes Line
	11100 6150 11100 3600
Wire Notes Line
	11100 3600 7350 3600
$Comp
L Device:R R?
U 1 1 62509163
P 9000 4200
AR Path="/62509163" Ref="R?"  Part="1" 
AR Path="/624DD375/62509163" Ref="R7"  Part="1" 
F 0 "R7" H 9070 4246 50  0000 L CNN
F 1 "3k3" V 9000 4150 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 8930 4200 50  0001 C CNN
F 3 "~" H 9000 4200 50  0001 C CNN
	1    9000 4200
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 62509169
P 10000 4200
AR Path="/62509169" Ref="R?"  Part="1" 
AR Path="/624DD375/62509169" Ref="R8"  Part="1" 
F 0 "R8" H 10070 4246 50  0000 L CNN
F 1 "3k3" V 10000 4150 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 9930 4200 50  0001 C CNN
F 3 "~" H 10000 4200 50  0001 C CNN
	1    10000 4200
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 6250916F
P 9000 4950
AR Path="/6250916F" Ref="C?"  Part="1" 
AR Path="/624DD375/6250916F" Ref="C1"  Part="1" 
F 0 "C1" H 9115 4996 50  0000 L CNN
F 1 "10nF" H 9115 4905 50  0000 L CNN
F 2 "Capacitor_THT:C_Rect_L7.2mm_W2.5mm_P5.00mm_FKS2_FKP2_MKS2_MKP2" H 9038 4800 50  0001 C CNN
F 3 "~" H 9000 4950 50  0001 C CNN
	1    9000 4950
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 62509175
P 10000 4950
AR Path="/62509175" Ref="C?"  Part="1" 
AR Path="/624DD375/62509175" Ref="C2"  Part="1" 
F 0 "C2" H 10115 4996 50  0000 L CNN
F 1 "10nF" H 10115 4905 50  0000 L CNN
F 2 "Capacitor_THT:C_Rect_L7.2mm_W2.5mm_P5.00mm_FKS2_FKP2_MKS2_MKP2" H 10038 4800 50  0001 C CNN
F 3 "~" H 10000 4950 50  0001 C CNN
	1    10000 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	9000 4350 9000 4400
$Comp
L power:VCC #PWR?
U 1 1 6250917C
P 10000 4000
AR Path="/6250917C" Ref="#PWR?"  Part="1" 
AR Path="/624DD375/6250917C" Ref="#PWR0111"  Part="1" 
F 0 "#PWR0111" H 10000 3850 50  0001 C CNN
F 1 "VCC" H 10017 4173 50  0000 C CNN
F 2 "" H 10000 4000 50  0001 C CNN
F 3 "" H 10000 4000 50  0001 C CNN
	1    10000 4000
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR?
U 1 1 62509182
P 9000 4000
AR Path="/62509182" Ref="#PWR?"  Part="1" 
AR Path="/624DD375/62509182" Ref="#PWR0112"  Part="1" 
F 0 "#PWR0112" H 9000 3850 50  0001 C CNN
F 1 "VCC" H 9017 4173 50  0000 C CNN
F 2 "" H 9000 4000 50  0001 C CNN
F 3 "" H 9000 4000 50  0001 C CNN
	1    9000 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	9000 4000 9000 4050
Wire Wire Line
	10000 4000 10000 4050
$Comp
L power:GND #PWR?
U 1 1 6250918A
P 9000 5150
AR Path="/6250918A" Ref="#PWR?"  Part="1" 
AR Path="/624DD375/6250918A" Ref="#PWR0113"  Part="1" 
F 0 "#PWR0113" H 9000 4900 50  0001 C CNN
F 1 "GND" H 9005 4977 50  0000 C CNN
F 2 "" H 9000 5150 50  0001 C CNN
F 3 "" H 9000 5150 50  0001 C CNN
	1    9000 5150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 62509190
P 10000 5150
AR Path="/62509190" Ref="#PWR?"  Part="1" 
AR Path="/624DD375/62509190" Ref="#PWR0123"  Part="1" 
F 0 "#PWR0123" H 10000 4900 50  0001 C CNN
F 1 "GND" H 10005 4977 50  0000 C CNN
F 2 "" H 10000 5150 50  0001 C CNN
F 3 "" H 10000 5150 50  0001 C CNN
	1    10000 5150
	1    0    0    -1  
$EndComp
Wire Wire Line
	9000 5100 9000 5150
Wire Wire Line
	10000 5100 10000 5150
Connection ~ 9000 4400
Wire Wire Line
	9000 4400 9000 4500
$Comp
L power:GND #PWR?
U 1 1 6250919A
P 10400 5900
AR Path="/6250919A" Ref="#PWR?"  Part="1" 
AR Path="/624DD375/6250919A" Ref="#PWR0124"  Part="1" 
F 0 "#PWR0124" H 10400 5650 50  0001 C CNN
F 1 "GND" H 10400 5750 50  0000 C CNN
F 2 "" H 10400 5900 50  0001 C CNN
F 3 "" H 10400 5900 50  0001 C CNN
	1    10400 5900
	1    0    0    -1  
$EndComp
$Comp
L 74xx:74HC14 U?
U 1 1 625091A0
P 8400 4400
AR Path="/625091A0" Ref="U?"  Part="1" 
AR Path="/624DD375/625091A0" Ref="U3"  Part="1" 
F 0 "U3" H 8550 4300 50  0000 C CNN
F 1 "74HC14" H 8500 4200 50  0000 C CNN
F 2 "Package_DIP:DIP-14_W7.62mm_Socket_LongPads" H 8400 4400 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74HC14" H 8400 4400 50  0001 C CNN
	1    8400 4400
	-1   0    0    1   
$EndComp
$Comp
L 74xx:74HC14 U?
U 7 1 625091A6
P 9600 5800
AR Path="/625091A6" Ref="U?"  Part="7" 
AR Path="/624DD375/625091A6" Ref="U3"  Part="7" 
F 0 "U3" V 9750 5750 50  0000 L CNN
F 1 "74HC14" V 9450 5650 50  0000 L CNN
F 2 "Package_DIP:DIP-14_W7.62mm_Socket_LongPads" H 9600 5800 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74HC14" H 9600 5800 50  0001 C CNN
	7    9600 5800
	0    -1   -1   0   
$EndComp
Text GLabel 8000 4750 0    50   Output ~ 0
D18
Text GLabel 8000 4400 0    50   Output ~ 0
D19
Wire Wire Line
	8100 4400 8000 4400
$Comp
L power:VCC #PWR?
U 1 1 625091AF
P 8550 5600
AR Path="/625091AF" Ref="#PWR?"  Part="1" 
AR Path="/624DD375/625091AF" Ref="#PWR0125"  Part="1" 
F 0 "#PWR0125" H 8550 5450 50  0001 C CNN
F 1 "VCC" H 8567 5773 50  0000 C CNN
F 2 "" H 8550 5600 50  0001 C CNN
F 3 "" H 8550 5600 50  0001 C CNN
	1    8550 5600
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x03 J?
U 1 1 625091B5
P 10750 4500
AR Path="/625091B5" Ref="J?"  Part="1" 
AR Path="/624DD375/625091B5" Ref="J8"  Part="1" 
F 0 "J8" H 10700 4750 50  0000 L CNN
F 1 "NEXT_AGAIN" H 10600 4300 50  0000 L CNN
F 2 "Connector_JST:JST_XH_B3B-XH-A_1x03_P2.50mm_Vertical" H 10750 4500 50  0001 C CNN
F 3 "~" H 10750 4500 50  0001 C CNN
	1    10750 4500
	1    0    0    1   
$EndComp
Wire Wire Line
	8700 4400 9000 4400
Wire Wire Line
	8550 5600 8550 5800
Wire Wire Line
	8550 5800 9100 5800
Wire Wire Line
	10400 5900 10400 5800
Wire Wire Line
	10400 5800 10100 5800
$Comp
L 74xx:74HC14 U?
U 2 1 625091C0
P 8400 4750
AR Path="/625091C0" Ref="U?"  Part="2" 
AR Path="/624DD375/625091C0" Ref="U3"  Part="2" 
F 0 "U3" H 8550 4650 50  0000 C CNN
F 1 "74HC14" H 8500 4550 50  0000 C CNN
F 2 "Package_DIP:DIP-14_W7.62mm_Socket_LongPads" H 8400 4750 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74HC14" H 8400 4750 50  0001 C CNN
	2    8400 4750
	-1   0    0    1   
$EndComp
Wire Wire Line
	8000 4750 8100 4750
Wire Wire Line
	8700 4750 10000 4750
Connection ~ 10000 4750
Wire Wire Line
	10000 4750 10000 4800
Wire Wire Line
	10400 5800 10400 4600
Wire Wire Line
	10400 4600 10550 4600
Connection ~ 10400 5800
Text Notes 8000 4650 2    50   ~ 0
INPUTS\nwith ISR
Wire Wire Line
	10000 4350 10000 4400
Wire Wire Line
	10550 4500 9000 4500
Connection ~ 9000 4500
Wire Wire Line
	9000 4500 9000 4800
Wire Wire Line
	10550 4400 10000 4400
Connection ~ 10000 4400
Wire Wire Line
	10000 4400 10000 4750
$Comp
L Connector_Generic:Conn_01x04 J?
U 1 1 6251438B
P 10700 1650
AR Path="/6251438B" Ref="J?"  Part="1" 
AR Path="/624DD375/6251438B" Ref="J16"  Part="1" 
F 0 "J16" H 10650 1850 50  0000 L CNN
F 1 "BUS I2C" H 10600 1350 50  0000 L CNN
F 2 "Connector_JST:JST_XH_B4B-XH-A_1x04_P2.50mm_Vertical" H 10700 1650 50  0001 C CNN
F 3 "~" H 10700 1650 50  0001 C CNN
	1    10700 1650
	1    0    0    -1  
$EndComp
Text GLabel 10250 1750 0    50   Input ~ 0
SDA
Text GLabel 10250 1650 0    50   Input ~ 0
SCL
Wire Wire Line
	10250 1650 10500 1650
Wire Wire Line
	10250 1750 10500 1750
$Comp
L power:GND #PWR?
U 1 1 62514395
P 10250 1900
AR Path="/62514395" Ref="#PWR?"  Part="1" 
AR Path="/624DD375/62514395" Ref="#PWR07"  Part="1" 
F 0 "#PWR07" H 10250 1650 50  0001 C CNN
F 1 "GND" H 10255 1727 50  0000 C CNN
F 2 "" H 10250 1900 50  0001 C CNN
F 3 "" H 10250 1900 50  0001 C CNN
	1    10250 1900
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR?
U 1 1 6251439B
P 10250 1500
AR Path="/6251439B" Ref="#PWR?"  Part="1" 
AR Path="/624DD375/6251439B" Ref="#PWR06"  Part="1" 
F 0 "#PWR06" H 10250 1350 50  0001 C CNN
F 1 "VCC" H 10267 1673 50  0000 C CNN
F 2 "" H 10250 1500 50  0001 C CNN
F 3 "" H 10250 1500 50  0001 C CNN
	1    10250 1500
	1    0    0    -1  
$EndComp
Text GLabel 8400 1750 0    50   Input ~ 0
SDA
Text GLabel 8400 1650 0    50   Input ~ 0
SCL
$Comp
L power:GND #PWR?
U 1 1 625143A3
P 8700 1900
AR Path="/625143A3" Ref="#PWR?"  Part="1" 
AR Path="/624DD375/625143A3" Ref="#PWR04"  Part="1" 
F 0 "#PWR04" H 8700 1650 50  0001 C CNN
F 1 "GND" H 8705 1727 50  0000 C CNN
F 2 "" H 8700 1900 50  0001 C CNN
F 3 "" H 8700 1900 50  0001 C CNN
	1    8700 1900
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR?
U 1 1 625143A9
P 9150 1100
AR Path="/625143A9" Ref="#PWR?"  Part="1" 
AR Path="/624DD375/625143A9" Ref="#PWR05"  Part="1" 
F 0 "#PWR05" H 9150 950 50  0001 C CNN
F 1 "VCC" H 9167 1273 50  0000 C CNN
F 2 "" H 9150 1100 50  0001 C CNN
F 3 "" H 9150 1100 50  0001 C CNN
	1    9150 1100
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J?
U 1 1 625143AF
P 9500 1650
AR Path="/625143AF" Ref="J?"  Part="1" 
AR Path="/624DD375/625143AF" Ref="J12"  Part="1" 
F 0 "J12" H 9450 1850 50  0000 L CNN
F 1 "BUS I2C" H 9400 1350 50  0000 L CNN
F 2 "Connector_JST:JST_XH_B4B-XH-A_1x04_P2.50mm_Vertical" H 9500 1650 50  0001 C CNN
F 3 "~" H 9500 1650 50  0001 C CNN
	1    9500 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	10250 1900 10250 1850
Wire Wire Line
	10250 1850 10500 1850
Wire Wire Line
	10250 1550 10250 1500
Wire Wire Line
	10250 1550 10500 1550
Wire Wire Line
	8400 1650 8950 1650
Wire Wire Line
	9300 1550 9150 1550
Wire Wire Line
	9150 1550 9150 1150
Wire Wire Line
	9300 1750 8700 1750
Wire Wire Line
	9300 1850 8700 1850
Wire Wire Line
	8700 1850 8700 1900
Wire Notes Line
	11000 650  11000 2500
Wire Notes Line
	11000 2500 8000 2500
Wire Notes Line
	8000 2500 8000 650 
Wire Notes Line
	8000 650  11000 650 
Text Notes 8150 950  0    50   ~ 0
I2C bus
$Comp
L Device:R R?
U 1 1 625143C4
P 8700 1350
AR Path="/625143C4" Ref="R?"  Part="1" 
AR Path="/624DD375/625143C4" Ref="R10"  Part="1" 
F 0 "R10" V 8600 1300 50  0000 L CNN
F 1 "3k3" V 8700 1300 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 8630 1350 50  0001 C CNN
F 3 "~" H 8700 1350 50  0001 C CNN
	1    8700 1350
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 625143CA
P 8950 1350
AR Path="/625143CA" Ref="R?"  Part="1" 
AR Path="/624DD375/625143CA" Ref="R11"  Part="1" 
F 0 "R11" V 8850 1300 50  0000 L CNN
F 1 "3k3" V 8950 1300 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 8880 1350 50  0001 C CNN
F 3 "~" H 8950 1350 50  0001 C CNN
	1    8950 1350
	1    0    0    -1  
$EndComp
Wire Wire Line
	8700 1200 8700 1150
Wire Wire Line
	8700 1150 8950 1150
Connection ~ 9150 1150
Wire Wire Line
	8950 1200 8950 1150
Connection ~ 8950 1150
Wire Wire Line
	8950 1150 9150 1150
Wire Wire Line
	9150 1100 9150 1150
Wire Wire Line
	8950 1500 8950 1650
Connection ~ 8950 1650
Wire Wire Line
	8950 1650 9300 1650
Wire Wire Line
	8700 1500 8700 1750
Connection ~ 8700 1750
Wire Wire Line
	8700 1750 8400 1750
Wire Notes Line
	950  5350 3300 5350
Wire Notes Line
	3300 5350 3300 7400
Wire Notes Line
	3300 7400 950  7400
Wire Notes Line
	950  7400 950  5350
$Comp
L Connector_Generic:Conn_01x03 J?
U 1 1 62538B21
P 3550 3900
AR Path="/62538B21" Ref="J?"  Part="1" 
AR Path="/624DD375/62538B21" Ref="J11"  Part="1" 
F 0 "J11" H 3600 4100 50  0000 R CNN
F 1 "TUNE" H 3650 3700 50  0000 R CNN
F 2 "Connector_JST:JST_XH_B3B-XH-A_1x03_P2.50mm_Vertical" H 3550 3900 50  0001 C CNN
F 3 "~" H 3550 3900 50  0001 C CNN
	1    3550 3900
	1    0    0    -1  
$EndComp
Wire Notes Line
	4900 3100 4900 4550
Wire Notes Line
	4900 4550 800  4550
Wire Notes Line
	800  4550 800  3100
Text Notes 1050 3500 0    50   ~ 0
Adaptation pour mesure de Capacité Variable
Wire Notes Line
	800  3100 4900 3100
$Comp
L Device:R R?
U 1 1 62538B2C
P 2050 4150
AR Path="/62538B2C" Ref="R?"  Part="1" 
AR Path="/624DD375/62538B2C" Ref="R9"  Part="1" 
F 0 "R9" V 2050 4150 50  0000 C CNN
F 1 "3M" V 2150 4150 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P15.24mm_Horizontal" V 1980 4150 50  0001 C CNN
F 3 "~" H 2050 4150 50  0001 C CNN
	1    2050 4150
	0    1    1    0   
$EndComp
Wire Wire Line
	3350 3800 1900 3800
Text GLabel 1450 3900 0    50   Output ~ 0
A12
Text GLabel 1450 3800 0    50   Output ~ 0
A14
Wire Wire Line
	1450 3800 1900 3800
Wire Wire Line
	1900 3800 1900 4150
Wire Wire Line
	1450 3900 2200 3900
Connection ~ 2200 3900
Wire Wire Line
	2200 3900 2200 4150
$Comp
L power:GND #PWR?
U 1 1 62538B3A
P 3100 4150
AR Path="/62538B3A" Ref="#PWR?"  Part="1" 
AR Path="/624DD375/62538B3A" Ref="#PWR03"  Part="1" 
F 0 "#PWR03" H 3100 3900 50  0001 C CNN
F 1 "GND" H 3105 3977 50  0000 C CNN
F 2 "" H 3100 4150 50  0001 C CNN
F 3 "" H 3100 4150 50  0001 C CNN
	1    3100 4150
	1    0    0    -1  
$EndComp
Wire Wire Line
	2200 3900 2550 3900
Connection ~ 1900 3800
Text Label 3200 3800 0    50   ~ 0
C2
Text Label 3200 3900 0    50   ~ 0
C1
Text Label 3200 4000 0    50   ~ 0
C0
$Comp
L Device:Jumper_NO_Small JP?
U 1 1 62538B45
P 2800 4000
AR Path="/62538B45" Ref="JP?"  Part="1" 
AR Path="/624DD375/62538B45" Ref="JP2"  Part="1" 
F 0 "JP2" H 3000 3950 50  0000 C CNN
F 1 "Jumper" H 2800 3950 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 2800 4000 50  0001 C CNN
F 3 "~" H 2800 4000 50  0001 C CNN
	1    2800 4000
	1    0    0    -1  
$EndComp
$Comp
L Device:Jumper_NC_Small JP?
U 1 1 62538B4B
P 2800 3900
AR Path="/62538B4B" Ref="JP?"  Part="1" 
AR Path="/624DD375/62538B4B" Ref="JP1"  Part="1" 
F 0 "JP1" H 3000 3850 50  0000 C CNN
F 1 "Jumper" H 2800 3850 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 2800 3900 50  0001 C CNN
F 3 "~" H 2800 3900 50  0001 C CNN
	1    2800 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	2900 3900 3350 3900
Wire Wire Line
	2900 4000 3100 4000
Wire Wire Line
	3100 4150 3100 4000
Connection ~ 3100 4000
Wire Wire Line
	3100 4000 3350 4000
Wire Wire Line
	2700 4000 2550 4000
Wire Wire Line
	2550 4000 2550 3900
Connection ~ 2550 3900
Wire Wire Line
	2550 3900 2700 3900
Text Notes 3300 4450 0    50   ~ 0
verifier s'il faut connecter à la masse
$EndSCHEMATC