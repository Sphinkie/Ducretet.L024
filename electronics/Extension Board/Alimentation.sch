EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 4
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Connector:TestPoint_Flag TP?
U 1 1 624C86D1
P 3850 3050
AR Path="/624C86D1" Ref="TP?"  Part="1" 
AR Path="/624BFCA3/624C86D1" Ref="TP2"  Part="1" 
F 0 "TP2" H 4100 3100 50  0000 L CNN
F 1 "GND" H 3950 3200 50  0000 L CNN
F 2 "TestPoint:TestPoint_Loop_D2.60mm_Drill1.4mm_Beaded" H 4050 3050 50  0001 C CNN
F 3 "~" H 4050 3050 50  0001 C CNN
	1    3850 3050
	1    0    0    -1  
$EndComp
$Comp
L Connector:TestPoint_Flag TP?
U 1 1 624C86D7
P 3800 1650
AR Path="/624C86D7" Ref="TP?"  Part="1" 
AR Path="/624BFCA3/624C86D7" Ref="TP1"  Part="1" 
F 0 "TP1" H 4100 1700 50  0000 L CNN
F 1 "V IN" H 3900 1800 50  0000 L CNN
F 2 "TestPoint:TestPoint_Loop_D2.60mm_Drill1.4mm_Beaded" H 4000 1650 50  0001 C CNN
F 3 "~" H 4000 1650 50  0001 C CNN
	1    3800 1650
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG?
U 1 1 624C86DD
P 3900 3250
AR Path="/624C86DD" Ref="#FLG?"  Part="1" 
AR Path="/624BFCA3/624C86DD" Ref="#FLG0103"  Part="1" 
F 0 "#FLG0103" H 3900 3325 50  0001 C CNN
F 1 "PWR_FLAG" V 3900 3600 50  0000 C CNN
F 2 "" H 3900 3250 50  0001 C CNN
F 3 "~" H 3900 3250 50  0001 C CNN
	1    3900 3250
	0    1    1    0   
$EndComp
$Comp
L power:PWR_FLAG #FLG?
U 1 1 624C86E3
P 3850 1300
AR Path="/624C86E3" Ref="#FLG?"  Part="1" 
AR Path="/624BFCA3/624C86E3" Ref="#FLG0104"  Part="1" 
F 0 "#FLG0104" H 3850 1375 50  0001 C CNN
F 1 "PWR_FLAG" V 3850 1600 50  0000 C CNN
F 2 "" H 3850 1300 50  0001 C CNN
F 3 "~" H 3850 1300 50  0001 C CNN
	1    3850 1300
	0    1    1    0   
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J?
U 1 1 624C86E9
P 1750 2600
AR Path="/624C86E9" Ref="J?"  Part="1" 
AR Path="/624BFCA3/624C86E9" Ref="J6"  Part="1" 
F 0 "J6" H 1668 2275 50  0000 C CNN
F 1 "POWER IN" H 1668 2366 50  0000 C CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MKDS-1,5-2-5.08_1x02_P5.08mm_Horizontal" H 1750 2600 50  0001 C CNN
F 3 "~" H 1750 2600 50  0001 C CNN
	1    1750 2600
	-1   0    0    1   
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J?
U 1 1 624C86EF
P 2200 1150
AR Path="/624C86EF" Ref="J?"  Part="1" 
AR Path="/624BFCA3/624C86EF" Ref="J9"  Part="1" 
F 0 "J9" V 2400 1200 50  0000 R CNN
F 1 "ON_OFF" V 2300 1250 50  0000 R CNN
F 2 "Connector_JST:JST_XH_B2B-XH-A_1x02_P2.50mm_Vertical" H 2200 1150 50  0001 C CNN
F 3 "~" H 2200 1150 50  0001 C CNN
	1    2200 1150
	0    -1   -1   0   
$EndComp
$Comp
L Device:CP C?
U 1 1 624C86F5
P 2650 2550
AR Path="/624C86F5" Ref="C?"  Part="1" 
AR Path="/624BFCA3/624C86F5" Ref="C3"  Part="1" 
F 0 "C3" H 2700 2650 50  0000 L CNN
F 1 "100uF" H 2700 2450 50  0000 L CNN
F 2 "Capacitor_THT:CP_Axial_L18.0mm_D8.0mm_P25.00mm_Horizontal" H 2688 2400 50  0001 C CNN
F 3 "~" H 2650 2550 50  0001 C CNN
	1    2650 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	1950 2500 2200 2500
Wire Wire Line
	2200 2500 2200 1550
Wire Wire Line
	2650 2750 2650 2700
Wire Wire Line
	2650 2350 2650 2400
Connection ~ 2650 2350
Connection ~ 2650 2750
Wire Wire Line
	3850 2750 3850 3050
$Comp
L power:VDD #PWR?
U 1 1 624C8702
P 3800 1200
AR Path="/624C8702" Ref="#PWR?"  Part="1" 
AR Path="/624BFCA3/624C8702" Ref="#PWR0114"  Part="1" 
F 0 "#PWR0114" H 3800 1050 50  0001 C CNN
F 1 "VDD" H 3817 1373 50  0000 C CNN
F 2 "" H 3800 1200 50  0001 C CNN
F 3 "" H 3800 1200 50  0001 C CNN
	1    3800 1200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 624C8708
P 3850 3450
AR Path="/624C8708" Ref="#PWR?"  Part="1" 
AR Path="/624BFCA3/624C8708" Ref="#PWR0115"  Part="1" 
F 0 "#PWR0115" H 3850 3200 50  0001 C CNN
F 1 "GND" H 3855 3277 50  0000 C CNN
F 2 "" H 3850 3450 50  0001 C CNN
F 3 "" H 3850 3450 50  0001 C CNN
	1    3850 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	2650 2350 3050 2350
$Comp
L Device:CP C?
U 1 1 624C870F
P 3050 2550
AR Path="/624C870F" Ref="C?"  Part="1" 
AR Path="/624BFCA3/624C870F" Ref="C4"  Part="1" 
F 0 "C4" H 3100 2650 50  0000 L CNN
F 1 "2200uF" H 3100 2450 50  0000 L CNN
F 2 "Capacitor_THT:CP_Axial_L30.0mm_D18.0mm_P35.00mm_Horizontal" H 3088 2400 50  0001 C CNN
F 3 "~" H 3050 2550 50  0001 C CNN
	1    3050 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	3050 2750 3050 2700
Wire Wire Line
	3050 2400 3050 2350
Wire Wire Line
	3900 3250 3850 3250
Connection ~ 3850 3250
Wire Wire Line
	3850 3250 3850 3450
Wire Wire Line
	3800 1200 3800 1300
Wire Wire Line
	3850 1300 3800 1300
Connection ~ 3800 1300
Wire Wire Line
	2300 2350 2650 2350
Wire Wire Line
	2300 1350 2300 2350
Wire Wire Line
	3800 1300 3800 1650
Connection ~ 3050 2750
Wire Wire Line
	3050 2350 3800 2350
Connection ~ 3050 2350
Wire Wire Line
	2650 2750 2200 2750
Wire Wire Line
	2200 2750 2200 2600
Wire Wire Line
	2200 2600 1950 2600
Wire Wire Line
	3050 2750 3850 2750
$Comp
L Connector:TestPoint_Flag TP?
U 1 1 624D4FED
P 3900 4950
AR Path="/624D4FED" Ref="TP?"  Part="1" 
AR Path="/624BFCA3/624D4FED" Ref="TP3"  Part="1" 
F 0 "TP3" H 4150 5000 50  0000 L CNN
F 1 "6V" H 4000 5100 50  0000 L CNN
F 2 "TestPoint:TestPoint_Loop_D2.60mm_Drill1.4mm_Beaded" H 4100 4950 50  0001 C CNN
F 3 "~" H 4100 4950 50  0001 C CNN
	1    3900 4950
	1    0    0    -1  
$EndComp
$Comp
L Regulator_Linear:LM7806_TO220 U?
U 1 1 624D4FF3
P 2150 5400
AR Path="/624D4FF3" Ref="U?"  Part="1" 
AR Path="/624BFCA3/624D4FF3" Ref="U2"  Part="1" 
F 0 "U2" H 2150 5642 50  0000 C CNN
F 1 "LM7806_TO220" H 2150 5551 50  0000 C CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 2150 5625 50  0001 C CIN
F 3 "http://www.fairchildsemi.com/ds/LM/LM7805.pdf" H 2150 5350 50  0001 C CNN
	1    2150 5400
	1    0    0    -1  
$EndComp
$Comp
L Regulator_Linear:LM7812_TO220 U?
U 1 1 624D4FF9
P 6600 5300
AR Path="/624D4FF9" Ref="U?"  Part="1" 
AR Path="/624BFCA3/624D4FF9" Ref="U1"  Part="1" 
F 0 "U1" H 6700 5050 50  0000 C CNN
F 1 "LM7812_TO220" H 6600 5451 50  0000 C CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 6600 5525 50  0001 C CIN
F 3 "http://www.fairchildsemi.com/ds/LM/LM7805.pdf" H 6600 5250 50  0001 C CNN
	1    6600 5300
	1    0    0    -1  
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J?
U 1 1 624D4FFF
P 3550 5600
AR Path="/624D4FFF" Ref="J?"  Part="1" 
AR Path="/624BFCA3/624D4FFF" Ref="J10"  Part="1" 
F 0 "J10" H 3600 5700 50  0000 R CNN
F 1 "CNX LAMPS" H 3800 5400 50  0000 R CNN
F 2 "MesModules:Bloc_de_jonction_CI-MPT_2.54mm_01x02" H 3550 5600 50  0001 C CNN
F 3 "~" H 3550 5600 50  0001 C CNN
	1    3550 5600
	1    0    0    -1  
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J?
U 1 1 624D5005
P 4500 5600
AR Path="/624D5005" Ref="J?"  Part="1" 
AR Path="/624BFCA3/624D5005" Ref="J13"  Part="1" 
F 0 "J13" H 4550 5700 50  0000 R CNN
F 1 "CNX 6V" H 4600 5400 50  0000 R CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MKDS-1,5-2-5.08_1x02_P5.08mm_Horizontal" H 4500 5600 50  0001 C CNN
F 3 "~" H 4500 5600 50  0001 C CNN
	1    4500 5600
	1    0    0    -1  
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J?
U 1 1 624D500B
P 8900 5450
AR Path="/624D500B" Ref="J?"  Part="1" 
AR Path="/624BFCA3/624D500B" Ref="J14"  Part="1" 
F 0 "J14" H 8850 5550 50  0000 L CNN
F 1 "CNX 12V" H 8750 5250 50  0000 L CNN
F 2 "TerminalBlock_MetzConnect:TerminalBlock_MetzConnect_Type011_RT05502HBWC_1x02_P5.00mm_Horizontal" H 8900 5450 50  0001 C CNN
F 3 "~" H 8900 5450 50  0001 C CNN
	1    8900 5450
	1    0    0    -1  
$EndComp
Wire Wire Line
	2150 5850 2150 5700
$Comp
L power:GND #PWR?
U 1 1 624D5012
P 6000 6050
AR Path="/624D5012" Ref="#PWR?"  Part="1" 
AR Path="/624BFCA3/624D5012" Ref="#PWR0116"  Part="1" 
F 0 "#PWR0116" H 6000 5800 50  0001 C CNN
F 1 "GND" H 6005 5877 50  0000 C CNN
F 2 "" H 6000 6050 50  0001 C CNN
F 3 "" H 6000 6050 50  0001 C CNN
	1    6000 6050
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C?
U 1 1 624D5018
P 2850 5650
AR Path="/624D5018" Ref="C?"  Part="1" 
AR Path="/624BFCA3/624D5018" Ref="C5"  Part="1" 
F 0 "C5" H 2968 5696 50  0000 L CNN
F 1 "10uF" H 2968 5605 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D5.0mm_P2.50mm" H 2888 5500 50  0001 C CNN
F 3 "~" H 2850 5650 50  0001 C CNN
	1    2850 5650
	1    0    0    -1  
$EndComp
Wire Wire Line
	2850 5400 2850 5500
Wire Wire Line
	2150 5850 2850 5850
Wire Wire Line
	2850 5850 2850 5800
Connection ~ 2150 5850
$Comp
L power:VCC #PWR?
U 1 1 624D5022
P 3900 4750
AR Path="/624D5022" Ref="#PWR?"  Part="1" 
AR Path="/624BFCA3/624D5022" Ref="#PWR0117"  Part="1" 
F 0 "#PWR0117" H 3900 4600 50  0001 C CNN
F 1 "VCC" H 3917 4923 50  0000 C CNN
F 2 "" H 3900 4750 50  0001 C CNN
F 3 "" H 3900 4750 50  0001 C CNN
	1    3900 4750
	1    0    0    -1  
$EndComp
Wire Wire Line
	2850 5400 3350 5400
Connection ~ 2850 5400
Wire Wire Line
	2850 5850 3350 5850
Connection ~ 2850 5850
Wire Wire Line
	3350 5400 3900 5400
Connection ~ 3350 5400
Wire Wire Line
	6900 5300 7250 5300
Wire Wire Line
	6600 5600 6600 5750
Wire Wire Line
	6600 5750 7250 5750
Wire Wire Line
	8300 5550 8700 5550
Connection ~ 6600 5750
Wire Wire Line
	6000 5300 6300 5300
$Comp
L Device:CP C?
U 1 1 624D5034
P 7250 5500
AR Path="/624D5034" Ref="C?"  Part="1" 
AR Path="/624BFCA3/624D5034" Ref="C6"  Part="1" 
F 0 "C6" H 7368 5546 50  0000 L CNN
F 1 "10uF" H 7368 5455 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D5.0mm_P2.50mm" H 7288 5350 50  0001 C CNN
F 3 "~" H 7250 5500 50  0001 C CNN
	1    7250 5500
	1    0    0    -1  
$EndComp
Wire Wire Line
	7250 5350 7250 5300
Wire Wire Line
	7250 5650 7250 5750
Connection ~ 3900 5400
Wire Wire Line
	3900 5400 3900 4950
$Comp
L power:+12V #PWR?
U 1 1 624D503E
P 8300 4650
AR Path="/624D503E" Ref="#PWR?"  Part="1" 
AR Path="/624BFCA3/624D503E" Ref="#PWR0118"  Part="1" 
F 0 "#PWR0118" H 8300 4500 50  0001 C CNN
F 1 "+12V" H 8315 4823 50  0000 C CNN
F 2 "" H 8300 4650 50  0001 C CNN
F 3 "" H 8300 4650 50  0001 C CNN
	1    8300 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	8300 4650 8300 4850
Wire Wire Line
	7250 5300 8300 5300
Wire Wire Line
	6600 5750 6000 5750
Wire Wire Line
	6000 5750 6000 6050
$Comp
L power:VDD #PWR?
U 1 1 624D5048
P 6000 4650
AR Path="/624D5048" Ref="#PWR?"  Part="1" 
AR Path="/624BFCA3/624D5048" Ref="#PWR0119"  Part="1" 
F 0 "#PWR0119" H 6000 4500 50  0001 C CNN
F 1 "VDD" H 6017 4823 50  0000 C CNN
F 2 "" H 6000 4650 50  0001 C CNN
F 3 "" H 6000 4650 50  0001 C CNN
	1    6000 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	6000 4650 6000 5300
Wire Wire Line
	1600 5850 2150 5850
$Comp
L Connector:TestPoint_Flag TP?
U 1 1 624D5050
P 8300 4850
AR Path="/624D5050" Ref="TP?"  Part="1" 
AR Path="/624BFCA3/624D5050" Ref="TP4"  Part="1" 
F 0 "TP4" H 8550 4900 50  0000 L CNN
F 1 "12V" H 8400 5000 50  0000 L CNN
F 2 "TestPoint:TestPoint_Loop_D2.60mm_Drill1.4mm_Beaded" H 8500 4850 50  0001 C CNN
F 3 "~" H 8500 4850 50  0001 C CNN
	1    8300 4850
	1    0    0    -1  
$EndComp
Connection ~ 8300 5300
Wire Wire Line
	8300 5300 8300 5450
Wire Wire Line
	8300 5450 8700 5450
Wire Wire Line
	8300 5550 8300 5750
Wire Wire Line
	7250 5750 8300 5750
$Comp
L power:GND #PWR?
U 1 1 624D505B
P 1600 6100
AR Path="/624D505B" Ref="#PWR?"  Part="1" 
AR Path="/624BFCA3/624D505B" Ref="#PWR0120"  Part="1" 
F 0 "#PWR0120" H 1600 5850 50  0001 C CNN
F 1 "GND" H 1605 5927 50  0000 C CNN
F 2 "" H 1600 6100 50  0001 C CNN
F 3 "" H 1600 6100 50  0001 C CNN
	1    1600 6100
	1    0    0    -1  
$EndComp
Wire Wire Line
	1600 6100 1600 5850
$Comp
L power:VDD #PWR?
U 1 1 624D5062
P 1600 4750
AR Path="/624D5062" Ref="#PWR?"  Part="1" 
AR Path="/624BFCA3/624D5062" Ref="#PWR0121"  Part="1" 
F 0 "#PWR0121" H 1600 4600 50  0001 C CNN
F 1 "VDD" H 1617 4923 50  0000 C CNN
F 2 "" H 1600 4750 50  0001 C CNN
F 3 "" H 1600 4750 50  0001 C CNN
	1    1600 4750
	1    0    0    -1  
$EndComp
Wire Wire Line
	1850 5400 1600 5400
Wire Wire Line
	1600 5400 1600 4750
Wire Wire Line
	3900 5400 3900 5600
Wire Wire Line
	3900 5600 4300 5600
Wire Wire Line
	4300 5700 3900 5700
Wire Wire Line
	3900 5700 3900 5850
Wire Wire Line
	3350 5700 3350 5850
Connection ~ 3350 5850
Wire Wire Line
	3350 5850 3900 5850
Wire Wire Line
	3350 5400 3350 5600
Wire Wire Line
	2450 5400 2850 5400
Connection ~ 7250 5300
Connection ~ 7250 5750
Text Notes 8650 5900 0    50   ~ 0
Ampli 7W/12V:\n→600mA
$Comp
L Mechanical:Heatsink HS?
U 1 1 624D5076
P 6600 5100
AR Path="/624D5076" Ref="HS?"  Part="1" 
AR Path="/624BFCA3/624D5076" Ref="HS1"  Part="1" 
F 0 "HS1" H 6742 5221 50  0000 L CNN
F 1 "Heatsink" H 6742 5130 50  0000 L CNN
F 2 "MesModules:HeatSink_TO-220" H 6612 5100 50  0001 C CNN
F 3 "~" H 6612 5100 50  0001 C CNN
	1    6600 5100
	1    0    0    -1  
$EndComp
Connection ~ 3800 1650
Wire Wire Line
	3800 1650 3800 2350
Connection ~ 3850 3050
Wire Wire Line
	3850 3050 3850 3250
Connection ~ 3900 4950
Wire Wire Line
	3900 4950 3900 4750
Connection ~ 8300 4850
Wire Wire Line
	8300 4850 8300 5300
Wire Notes Line
	1000 4300 4900 4300
Wire Notes Line
	4900 4300 4900 6450
Wire Notes Line
	4900 6450 1000 6450
Wire Notes Line
	1000 6450 1000 4300
Wire Notes Line
	5250 6450 5250 4300
Wire Notes Line
	5250 4300 9500 4300
Wire Notes Line
	9500 4300 9500 6450
Wire Notes Line
	9500 6450 5250 6450
Text Notes 5350 4400 0    50   ~ 0
Alimentation 12V
Text Notes 1050 4400 0    50   ~ 0
Alimentation 6V
Wire Notes Line
	1000 750  1000 4050
Wire Notes Line
	1000 4050 4900 4050
Wire Notes Line
	4900 4050 4900 750 
Wire Notes Line
	4900 750  1000 750 
Text Notes 1050 900  0    50   ~ 0
Power IN
Wire Wire Line
	2750 2750 3050 2750
Wire Wire Line
	2650 2750 3050 2750
$Comp
L Mechanical:Heatsink HS?
U 1 1 6258C9EF
P 2150 5100
AR Path="/6258C9EF" Ref="HS?"  Part="1" 
AR Path="/624BFCA3/6258C9EF" Ref="HS2"  Part="1" 
F 0 "HS2" H 2292 5221 50  0000 L CNN
F 1 "Heatsink" H 2292 5130 50  0000 L CNN
F 2 "MesModules:HeatSink_TO-220" H 2162 5100 50  0001 C CNN
F 3 "~" H 2162 5100 50  0001 C CNN
	1    2150 5100
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG?
U 1 1 62575958
P 2150 1550
AR Path="/62575958" Ref="#FLG?"  Part="1" 
AR Path="/624BFCA3/62575958" Ref="#FLG?"  Part="1" 
F 0 "#FLG?" H 2150 1625 50  0001 C CNN
F 1 "PWR_FLAG" V 2150 1850 50  0000 C CNN
F 2 "" H 2150 1550 50  0001 C CNN
F 3 "~" H 2150 1550 50  0001 C CNN
	1    2150 1550
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2150 1550 2200 1550
Connection ~ 2200 1550
Wire Wire Line
	2200 1550 2200 1350
Text Label 2000 2500 0    50   ~ 0
VSS
$EndSCHEMATC
