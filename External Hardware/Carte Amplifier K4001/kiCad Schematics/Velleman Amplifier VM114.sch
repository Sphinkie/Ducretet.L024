EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Audio Amplifier 7W Mono - Model VM114"
Date "2020-02-07"
Rev "1"
Comp "Velleman"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Mechanical:MountingHole H1
U 1 1 5E3DE8C3
P 7500 6000
F 0 "H1" H 7600 6046 50  0000 L CNN
F 1 "MountingHole" H 7600 5955 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_DIN965_Pad" H 7500 6000 50  0001 C CNN
F 3 "~" H 7500 6000 50  0001 C CNN
	1    7500 6000
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H2
U 1 1 5E3DEE9B
P 7500 6250
F 0 "H2" H 7600 6296 50  0000 L CNN
F 1 "MountingHole" H 7600 6205 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_DIN965_Pad" H 7500 6250 50  0001 C CNN
F 3 "~" H 7500 6250 50  0001 C CNN
	1    7500 6250
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H3
U 1 1 5E3DF3AA
P 8600 6000
F 0 "H3" H 8700 6046 50  0000 L CNN
F 1 "MountingHole" H 8700 5955 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_DIN965_Pad" H 8600 6000 50  0001 C CNN
F 3 "~" H 8600 6000 50  0001 C CNN
	1    8600 6000
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H4
U 1 1 5E3DF677
P 8600 6250
F 0 "H4" H 8700 6296 50  0000 L CNN
F 1 "MountingHole" H 8700 6205 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_DIN965_Pad" H 8600 6250 50  0001 C CNN
F 3 "~" H 8600 6250 50  0001 C CNN
	1    8600 6250
	1    0    0    -1  
$EndComp
$Comp
L Amplifier_Audio:TDA2003 IC1
U 1 1 5E3DFDCE
P 4000 4050
F 0 "IC1" H 4344 4096 50  0000 L CNN
F 1 "TDA2003" H 4344 4005 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-5_P3.4x3.7mm_StaggerOdd_Lead3.8mm_Vertical" H 4000 4050 50  0001 C CIN
F 3 "http://www.st.com/resource/en/datasheet/cd00000123.pdf" H 4000 4050 50  0001 C CNN
	1    4000 4050
	1    0    0    -1  
$EndComp
$Comp
L Device:C C2
U 1 1 5E3E01B5
P 5500 2750
F 0 "C2" H 5615 2796 50  0000 L CNN
F 1 "100nF" H 5615 2705 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D4.3mm_W1.9mm_P5.00mm" H 5538 2600 50  0001 C CNN
F 3 "~" H 5500 2750 50  0001 C CNN
	1    5500 2750
	1    0    0    -1  
$EndComp
$Comp
L Device:C C1
U 1 1 5E3E0F38
P 3700 5000
F 0 "C1" V 3448 5000 50  0000 C CNN
F 1 "8n2" V 3539 5000 50  0000 C CNN
F 2 "Capacitor_THT:C_Disc_D4.3mm_W1.9mm_P5.00mm" H 3738 4850 50  0001 C CNN
F 3 "~" H 3700 5000 50  0001 C CNN
	1    3700 5000
	0    1    1    0   
$EndComp
$Comp
L Device:C C3
U 1 1 5E3E1922
P 6150 4250
F 0 "C3" H 6035 4204 50  0000 R CNN
F 1 "100nF" H 6035 4295 50  0000 R CNN
F 2 "Capacitor_THT:C_Disc_D4.3mm_W1.9mm_P5.00mm" H 6188 4100 50  0001 C CNN
F 3 "~" H 6150 4250 50  0001 C CNN
	1    6150 4250
	-1   0    0    1   
$EndComp
$Comp
L power:GNDD #PWR05
U 1 1 5E3E2231
P 5500 3100
F 0 "#PWR05" H 5500 2850 50  0001 C CNN
F 1 "GNDD" H 5504 2945 50  0000 C CNN
F 2 "" H 5500 3100 50  0001 C CNN
F 3 "" H 5500 3100 50  0001 C CNN
	1    5500 3100
	1    0    0    -1  
$EndComp
$Comp
L power:GNDD #PWR02
U 1 1 5E3E287A
P 3900 4450
F 0 "#PWR02" H 3900 4200 50  0001 C CNN
F 1 "GNDD" H 3904 4295 50  0000 C CNN
F 2 "" H 3900 4450 50  0001 C CNN
F 3 "" H 3900 4450 50  0001 C CNN
	1    3900 4450
	1    0    0    -1  
$EndComp
$Comp
L power:GNDD #PWR07
U 1 1 5E3E2E41
P 6900 3100
F 0 "#PWR07" H 6900 2850 50  0001 C CNN
F 1 "GNDD" H 6904 2945 50  0000 C CNN
F 2 "" H 6900 3100 50  0001 C CNN
F 3 "" H 6900 3100 50  0001 C CNN
	1    6900 3100
	1    0    0    -1  
$EndComp
$Comp
L power:GNDD #PWR01
U 1 1 5E3E374D
P 2500 4450
F 0 "#PWR01" H 2500 4200 50  0001 C CNN
F 1 "GNDD" H 2504 4295 50  0000 C CNN
F 2 "" H 2500 4450 50  0001 C CNN
F 3 "" H 2500 4450 50  0001 C CNN
	1    2500 4450
	1    0    0    -1  
$EndComp
$Comp
L power:GNDD #PWR08
U 1 1 5E3E3D4C
P 7050 6100
F 0 "#PWR08" H 7050 5850 50  0001 C CNN
F 1 "GNDD" H 7054 5945 50  0000 C CNN
F 2 "" H 7050 6100 50  0001 C CNN
F 3 "" H 7050 6100 50  0001 C CNN
	1    7050 6100
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C6
U 1 1 5E3E4847
P 4950 2750
F 0 "C6" H 5068 2796 50  0000 L CNN
F 1 "1000uF" H 5068 2705 50  0000 L CNN
F 2 "Capacitor_THT:C_Radial_D10.0mm_H20.0mm_P5.00mm" H 4988 2600 50  0001 C CNN
F 3 "~" H 4950 2750 50  0001 C CNN
	1    4950 2750
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C4
U 1 1 5E3E5118
P 3200 3950
F 0 "C4" V 2945 3950 50  0000 C CNN
F 1 "10uF" V 3036 3950 50  0000 C CNN
F 2 "Capacitor_THT:C_Radial_D5.0mm_H11.0mm_P2.00mm" H 3238 3800 50  0001 C CNN
F 3 "~" H 3200 3950 50  0001 C CNN
	1    3200 3950
	0    1    1    0   
$EndComp
$Comp
L Device:CP C7
U 1 1 5E3E58BC
P 5750 4050
F 0 "C7" V 5495 4050 50  0000 C CNN
F 1 "1000uF" V 5586 4050 50  0000 C CNN
F 2 "Capacitor_THT:C_Radial_D10.0mm_H20.0mm_P5.00mm" H 5788 3900 50  0001 C CNN
F 3 "~" H 5750 4050 50  0001 C CNN
	1    5750 4050
	0    1    1    0   
$EndComp
$Comp
L Device:CP C5
U 1 1 5E3E6022
P 4000 5500
F 0 "C5" V 4255 5500 50  0000 C CNN
F 1 "470uF" V 4164 5500 50  0000 C CNN
F 2 "Capacitor_THT:C_Radial_D6.3mm_H11.0mm_P2.50mm" H 4038 5350 50  0001 C CNN
F 3 "~" H 4000 5500 50  0001 C CNN
	1    4000 5500
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3350 3950 3700 3950
Wire Wire Line
	3900 3750 3900 2500
Wire Wire Line
	3900 2500 4950 2500
Wire Wire Line
	4950 2500 4950 2600
Wire Wire Line
	4950 2500 5500 2500
Wire Wire Line
	5500 2500 5500 2600
Connection ~ 4950 2500
Wire Wire Line
	5500 2900 5500 3100
Wire Wire Line
	3900 4350 3900 4450
Wire Wire Line
	3700 4150 3400 4150
Wire Wire Line
	3400 4150 3400 5000
Wire Wire Line
	3400 5000 3550 5000
Wire Wire Line
	3850 5500 3400 5500
Wire Wire Line
	3400 5500 3400 5000
Connection ~ 3400 5000
Wire Wire Line
	5900 4050 6150 4050
Wire Wire Line
	6150 4050 6150 4100
$Comp
L Connector_Generic:Conn_01x01 J3
U 1 1 5E3E7BEC
P 7500 2500
F 0 "J3" H 7450 2600 50  0000 L CNN
F 1 "POWER" H 7600 2500 50  0000 L CNN
F 2 "Connector_Pin:Pin_D1.0mm_L10.0mm" H 7500 2500 50  0001 C CNN
F 3 "~" H 7500 2500 50  0001 C CNN
	1    7500 2500
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J4
U 1 1 5E3E8A49
P 7500 2850
F 0 "J4" H 7450 2950 50  0000 L CNN
F 1 "GND" H 7600 2850 50  0000 L CNN
F 2 "Connector_Pin:Pin_D1.0mm_L10.0mm" H 7500 2850 50  0001 C CNN
F 3 "~" H 7500 2850 50  0001 C CNN
	1    7500 2850
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J1
U 1 1 5E3E8E94
P 2000 3950
F 0 "J1" H 2000 3850 50  0000 C CNN
F 1 "IN" H 2150 3950 50  0000 C CNN
F 2 "Connector_Pin:Pin_D1.0mm_L10.0mm" H 2000 3950 50  0001 C CNN
F 3 "~" H 2000 3950 50  0001 C CNN
	1    2000 3950
	-1   0    0    1   
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J2
U 1 1 5E3E9AF8
P 2000 4250
F 0 "J2" H 2000 4150 50  0000 C CNN
F 1 "GND" H 2150 4250 50  0000 C CNN
F 2 "Connector_Pin:Pin_D1.0mm_L10.0mm" H 2000 4250 50  0001 C CNN
F 3 "~" H 2000 4250 50  0001 C CNN
	1    2000 4250
	-1   0    0    1   
$EndComp
Wire Wire Line
	2200 3950 3050 3950
Wire Wire Line
	2200 4250 2500 4250
Wire Wire Line
	2500 4250 2500 4450
Wire Wire Line
	7300 2500 7000 2500
Connection ~ 5500 2500
Wire Wire Line
	6900 3100 6900 2850
Wire Wire Line
	6900 2850 7000 2850
$Comp
L Device:R R3
U 1 1 5E3EAEA7
P 4450 5000
F 0 "R3" V 4243 5000 50  0000 C CNN
F 1 "100" V 4334 5000 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 4380 5000 50  0001 C CNN
F 3 "~" H 4450 5000 50  0001 C CNN
	1    4450 5000
	0    1    1    0   
$EndComp
$Comp
L Device:R R1
U 1 1 5E3EBC4E
P 5150 5050
F 0 "R1" H 5080 5004 50  0000 R CNN
F 1 "470" H 5080 5095 50  0000 R CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 5080 5050 50  0001 C CNN
F 3 "~" H 5150 5050 50  0001 C CNN
	1    5150 5050
	-1   0    0    1   
$EndComp
$Comp
L Device:R R2
U 1 1 5E3EBF18
P 5150 5850
F 0 "R2" H 5080 5804 50  0000 R CNN
F 1 "4,7" H 5080 5895 50  0000 R CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 5080 5850 50  0001 C CNN
F 3 "~" H 5150 5850 50  0001 C CNN
	1    5150 5850
	-1   0    0    1   
$EndComp
Wire Wire Line
	3850 5000 4300 5000
$Comp
L power:GNDD #PWR03
U 1 1 5E3ECF24
P 4950 3100
F 0 "#PWR03" H 4950 2850 50  0001 C CNN
F 1 "GNDD" H 4954 2945 50  0000 C CNN
F 2 "" H 4950 3100 50  0001 C CNN
F 3 "" H 4950 3100 50  0001 C CNN
	1    4950 3100
	1    0    0    -1  
$EndComp
$Comp
L power:GNDD #PWR04
U 1 1 5E3ED18E
P 5150 6100
F 0 "#PWR04" H 5150 5850 50  0001 C CNN
F 1 "GNDD" H 5154 5945 50  0000 C CNN
F 2 "" H 5150 6100 50  0001 C CNN
F 3 "" H 5150 6100 50  0001 C CNN
	1    5150 6100
	1    0    0    -1  
$EndComp
Wire Wire Line
	5150 6000 5150 6100
Wire Wire Line
	4950 2900 4950 3100
Wire Wire Line
	5150 5200 5150 5500
Wire Wire Line
	4150 5500 5150 5500
Connection ~ 5150 5500
Wire Wire Line
	5150 5500 5150 5700
Wire Wire Line
	4300 4050 4700 4050
Wire Wire Line
	4700 4050 5150 4050
Wire Wire Line
	4600 5000 4700 5000
Wire Wire Line
	4700 5000 4700 4050
Connection ~ 4700 4050
$Comp
L Connector_Generic:Conn_01x01 J5
U 1 1 5E3F0488
P 7500 4050
F 0 "J5" H 7450 4150 50  0000 L CNN
F 1 "HP" H 7600 4050 50  0000 L CNN
F 2 "Connector_Pin:Pin_D1.0mm_L10.0mm" H 7500 4050 50  0001 C CNN
F 3 "~" H 7500 4050 50  0001 C CNN
	1    7500 4050
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J6
U 1 1 5E3F0ABF
P 7500 4350
F 0 "J6" H 7450 4450 50  0000 L CNN
F 1 "GND" H 7600 4350 50  0000 L CNN
F 2 "Connector_Pin:Pin_D1.0mm_L10.0mm" H 7500 4350 50  0001 C CNN
F 3 "~" H 7500 4350 50  0001 C CNN
	1    7500 4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	7300 4350 7050 4350
Wire Wire Line
	7050 4350 7050 6100
Wire Wire Line
	7300 4050 6150 4050
Connection ~ 6150 4050
$Comp
L Device:R R4
U 1 1 5E3F254F
P 6150 5300
F 0 "R4" H 6080 5254 50  0000 R CNN
F 1 "1" H 6080 5345 50  0000 R CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 6080 5300 50  0001 C CNN
F 3 "~" H 6150 5300 50  0001 C CNN
	1    6150 5300
	-1   0    0    1   
$EndComp
Wire Wire Line
	6150 4400 6150 5150
$Comp
L power:GNDD #PWR06
U 1 1 5E3F3922
P 6150 6100
F 0 "#PWR06" H 6150 5850 50  0001 C CNN
F 1 "GNDD" H 6154 5945 50  0000 C CNN
F 2 "" H 6150 6100 50  0001 C CNN
F 3 "" H 6150 6100 50  0001 C CNN
	1    6150 6100
	1    0    0    -1  
$EndComp
Wire Wire Line
	6150 6100 6150 5450
Wire Wire Line
	5150 4900 5150 4050
Connection ~ 5150 4050
Wire Wire Line
	5150 4050 5600 4050
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 5E4019A2
P 7000 2350
F 0 "#FLG0101" H 7000 2425 50  0001 C CNN
F 1 "PWR_FLAG" H 7000 2523 50  0000 C CNN
F 2 "" H 7000 2350 50  0001 C CNN
F 3 "~" H 7000 2350 50  0001 C CNN
	1    7000 2350
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG0102
U 1 1 5E401EBF
P 7000 2800
F 0 "#FLG0102" H 7000 2875 50  0001 C CNN
F 1 "PWR_FLAG" H 7000 2973 50  0000 C CNN
F 2 "" H 7000 2800 50  0001 C CNN
F 3 "~" H 7000 2800 50  0001 C CNN
	1    7000 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	7000 2800 7000 2850
Connection ~ 7000 2850
Wire Wire Line
	7000 2850 7300 2850
Wire Wire Line
	7000 2350 7000 2500
Connection ~ 7000 2500
Wire Wire Line
	7000 2500 5500 2500
Wire Notes Line
	1500 3450 1500 4650
Wire Notes Line
	1500 4650 2250 4650
Wire Notes Line
	2250 4650 2250 3450
Wire Notes Line
	2250 3450 1500 3450
Text Notes 1550 3600 0    50   ~ 0
Input Audio signal
Wire Notes Line
	7250 3850 7250 4600
Wire Notes Line
	7250 4600 7900 4600
Wire Notes Line
	7900 4600 7900 3850
Wire Notes Line
	7900 3850 7250 3850
Text Notes 7350 4700 0    50   ~ 0
Connect a 4 or 8 ohm speaker
Wire Notes Line
	7250 3000 8150 3000
Wire Notes Line
	8150 3000 8150 2300
Wire Notes Line
	8150 2300 7250 2300
Wire Notes Line
	7250 2300 7250 3000
Text Notes 7250 3200 0    50   ~ 0
Power with a very stable 8 to 18 volts DC\nEx: 12v
Text Notes 1100 4900 0    50   ~ 0
Use a 47k Log potentiometer\nto adjust input signal volume
$Comp
L Mechanical:Heatsink HS1
U 1 1 5E421356
P 9850 6200
F 0 "HS1" H 9992 6321 50  0000 L CNN
F 1 "Heatsink" H 9992 6230 50  0000 L CNN
F 2 "Heatsink:Heatsink_Stonecold_HS-132_32x14mm_2xFixation1.5mm" H 9862 6200 50  0001 C CNN
F 3 "~" H 9862 6200 50  0001 C CNN
	1    9850 6200
	1    0    0    -1  
$EndComp
$EndSCHEMATC
