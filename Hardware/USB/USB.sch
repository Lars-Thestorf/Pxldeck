EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
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
L Connector:USB_OTG J1
U 1 1 611BF9E2
P 4100 4000
F 0 "J1" H 4157 4467 50  0000 C CNN
F 1 "USB_OTG" H 4157 4376 50  0000 C CNN
F 2 "Connector_USB:USB_Micro-B_Wuerth_629105150521" H 4250 3950 50  0001 C CNN
F 3 " ~" H 4250 3950 50  0001 C CNN
	1    4100 4000
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x05_Male J2
U 1 1 611C1A35
P 5000 4000
F 0 "J2" H 4972 3932 50  0000 R CNN
F 1 "Conn_01x05_Male" H 4972 4023 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 5000 4000 50  0001 C CNN
F 3 "~" H 5000 4000 50  0001 C CNN
	1    5000 4000
	-1   0    0    1   
$EndComp
Wire Wire Line
	4400 4100 4500 4100
Wire Wire Line
	4600 4100 4600 4200
Wire Wire Line
	4700 4200 4700 4450
Wire Wire Line
	4100 4450 4100 4400
Wire Wire Line
	4400 4200 4600 4200
Wire Wire Line
	4600 4100 4800 4100
Wire Wire Line
	4500 3900 4800 3900
Wire Wire Line
	4500 3900 4500 4100
Wire Wire Line
	4400 4000 4800 4000
Wire Wire Line
	4700 4200 4800 4200
Wire Wire Line
	4100 4450 4700 4450
Wire Wire Line
	4400 3800 4800 3800
$Comp
L Mechanical:MountingHole_Pad H1
U 1 1 611CCE3C
P 4100 4650
F 0 "H1" V 4054 4800 50  0000 L CNN
F 1 "MountingHole_Pad" V 4145 4800 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.2mm_M2_Pad" H 4100 4650 50  0001 C CNN
F 3 "~" H 4100 4650 50  0001 C CNN
	1    4100 4650
	0    1    1    0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H2
U 1 1 611CDEC0
P 4100 4850
F 0 "H2" V 4054 5000 50  0000 L CNN
F 1 "MountingHole_Pad" V 4145 5000 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.2mm_M2_Pad" H 4100 4850 50  0001 C CNN
F 3 "~" H 4100 4850 50  0001 C CNN
	1    4100 4850
	0    1    1    0   
$EndComp
Wire Wire Line
	4000 4850 4000 4650
Connection ~ 4000 4650
Wire Wire Line
	4000 4650 4000 4400
Text Label 4700 3800 0    50   ~ 0
5V
Text Label 4700 3900 0    50   ~ 0
D-
Text Label 4700 4000 0    50   ~ 0
D+
Text Label 4700 4100 0    50   ~ 0
ID
Text Label 4700 4200 0    50   ~ 0
GND
Text Label 4000 4700 1    50   ~ 0
Shield
$EndSCHEMATC
