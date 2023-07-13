# Custom TID 1.0

## Einleitung
Das Custom TID Projekt dient zur individualisierten Anzeige von Inhalten auf den in vielen Opel Fahrezeugen verbauten Triple Info Displays (TID), wie sie zum Beispiel im Corsa B/C oder Astra F/G verbaut worden sind. Dies ist vor allem dann nützlich, wenn ein Fremdradio eingebaut wurde und das Display somit ansonsten nichts mehr anzeigt, da es vorher nur für die Anzeige des Radiostatus des Originalradios genutzt wurde. Als Übertragungsprotokoll wird ein von Opel angepasstes I²C genutzt, welches eine zusätzliche Master Request Leitung besitzt. Die genaue Funktionsweise des Bussystems kann (1) unter den Weiterführenden Links entnommen werden. Als Inspirationsquelle diente der folgende Blogeintrag (2), welcher ebenfalls den Weiterführenden Links entnommen werden kann. 

## Hardware
Als Hardwarebasis für dieses Projekt wird ein Arduino Nano V3 eingesetzt. Dieser ist auf einer selbst erstellten Platine in KiCad aufgesetzt, welche für die Spannungsregulierung von der Boardspannung des Fahrzeugs auf 12V zuständig ist und alle benötigten Datenleitungen vom Auto zum Arduino und anschließend wieder zum Auto zurückführt. Die Steckerbelegung kann vom Auto kann unter (6) nachgeschlagen werden. Da in dem verwendeten Corsa B noch das Originalradio genutzt wird, wurden auf der Platine zusätzlich analoge Schalter IC's zum umschalten des aktuell verwendeten Bussystem Masters angebracht. 

## Software
Die aktuelle Software wurde mit der Arduino Library unter Visual Studio Code mit dem PlatformIO Plugin erstellt. Die Software ist für ein 8 stelliges Display programmiert. Für 10 stellige Displays wie zum Beispiel im Corsa C müssen einige Änderungen im Code vorgenommen werden. Aktuell kann die Geschwindigkeit, die Durchschnittsgeschwindigkeit, die gefahrenen Kilometer, die Zeit in der das Fahrzeug eingeschaltet ist und die Boardspannung dargestellt werden. Um das Geschwindigkeitssignal zu erfassen wurde ein Timer programmiert, welcher eine bestimmte zeit die Impulse des Geschwindigkeitssignals mittels Interrupts erfasst. Die gefahrene Strecke etc. kann mit verschiedenen Formeln daraus abgeleitet werden. Die Baordspannung wird durch einen analogen Pin eingelesen. Da die Pins am Arduino lediglich 5V kompatibel sind musste mittels eines Spannungsteilers auf der Paltine die Spannungs heruntergeregelt werden. Um die Spannungs jedoch korrekt ausgeben zu können muss der Spannungsteiler vor der Ausgabe zurückgerechnet werden. Die verwendeten Formeln werden im folgenden kurz erläutert:

### 1. Radumfang ermitteln
Um den Radumfang zu ermitteln kann in eine Tabelle geschaut werden, in welcher alle gängigen Rad/Reifen Kombinationen aufgelistet sind. Für die in diesem Projekt verwendeten 165/70 R13 Räder beträgt der Umfang 1712 mm beziehungsweise 1,712 m pro Radumdrehung.

### 2. Radumfang auf Impulse des Wegstreckensignals übertragen
Zunächst einmal muss man heruasfinden, wie viele Impulse vom Fahrzeug erzeugt werden. Diese können zum Beispiel einem Datenblatt entnommen werden. Im Corsa B gibt es in der Tachoeinheit eine Zahl, welche die Impulsanzahl pro gefahrenen Kilometer angibt. Hier ist dies K = 16676. Es werden also 16,676 Impulse pro Meter erzeugt. Wenn man diesen Wert mit der Strecke einer Radumdrehung multipliziert, erhält man die Impulse welche pro Radumdrehung erzeugt werden.  
  
16,676 Impulse * 1,712 m = 28,549312 ~ 29 Impulse pro Radumdrehung.  

### 3. Zeitintervall für Impulsmessung festlegen
Um das Zeitintervall zu bestimmen, in welcher Zeit die Impulse gemessen werden sollen, werden der Radumfang und die Impulse benötigt, welche zuvor bereits berechnet wurden. Durch folgende Formel, die auch unter (3) gefunden wurde, kann dann das Zeitintervall bestimmt werden.  
  
**T = (3,6 * U) / p | T = Zeitintervall; 3,6 Umrechnungsfaktor für Km/h; U = Reifenumfang; p = Impulse**  
  
**(3,6 * 1,712 m) / 29 = 0,2125s**   

### 4. Timer einstellen

Genutzt wurde der 16-Bit Timer1. Der verwendete Prescaler beträgt 64. Als Interruptquelle wird der Input Capture Pin genutzt, welcher auf steigende Flanken reagiert. Außerdem wird das Clear Timer on Compare Match Interrupt genutzt, welcher den Timer vor dem eigentlichen Überlauf auf 0 zurücksetzen kann, damit feinere Zeiten eingestellt werden können. Dadurch war es möglich den Timer alle 0,2125s, wie auch zuvor berechnet, resetten zu können.  
  
Erklärungen zu AVR Timern unter (4)
Berechnungen zur richtigen Einstellung von Timern unter (5)
  
Um in das Clear Timer on Compare Register den richtigen Wert zu schreiben, muss folgende Formel genutzt werden. Die + 1 beim Register wird benötigt da der Reset des Timers ebenfalls mitgezählt werden muss.
  
**(OCR1A + 1) = Taktgeber / (Frequenz * 2 * N) | Taktgeber = Takt vom Arduino; Frequenz = Zeitintervall in Frequenz  umrechnen -> 1 / 0,2125 = 4,705 Hz; N = Prescaler**  
  
**16000000 Hz / (4,705 Hz * 2 * 64) = 26568,4814 + 1**  

### 5. Geschwindigkeit berechnen

Korrekturfaktor von 0.8 verwendet, um zu schnelles fahren zu vermeiden. Die Geschwindigkeitsanzeige sollte ähnliche bis gleiche Werte, wie ein Navigationsgerät über GPS anzeigen. Die Geschwindigkeit sollte also im Gegensatz zum tacho geringer ausfallen, da diese immer ein wenig vorlaufen. Die wird je höher die Geschwindigkeit ist immer mehr.  

**speed = (((prevImpulseCount * METERS_PER_IMPULSE) / IMPULSE_MEASURING_SECONDS) * 3.6) + 0.8**  

### 6. Durchschnittsgeschwindigkeit berechnen

Zunächst müssen noch die gefahrenen Meter erfasst werden. Die können auch zur Anzeige der gefahrenen Kilometer genutzt werden. Die 0.5 werden zum aufrunden des Ergebnisses genutzt. 

**meters = (meters + (odometerCount * METERS_PER_IMPULSE)) + 0.5**  
  
Im Anschluss kann man daraus eine Durchschnittsgeschwindigkeit berrechnen.  

**avgSpeed = ((meters / uptime) * 3.6) + 0.5**  

### 7. Boardspannung berechnen

Zuerst muss der anlog eingelesene Wert in eine Spannung in Volt zurückgerechnet werden. (0V = 0; 5V = 1024)  

**dividerVoltage = (boardVoltage / 1024) * 5**  

Der Spannungsteiler kann dann folgendermaßen zurückgerechnet werden. Die Werte R1 und R2 sind die Widerstandswerte in Ohm.  

**carVoltage = ((R1 + R2) / R2) * dividerVoltage**  


### 8. Weitere Berechnungen

Weitere Berechnungen, wie beispielsweise die Realisierung der Zeitmessung, können dem Quellcode dieses Projekts entnommen werden.

## Weiterführende Links

(1) *Opel Radio Display Bus Protokoll - PDF im Projektordner*  
(2) *Blogeintrag - https://www.motor-talk.de/blogs/gorgeous188/einkaufsliste-custom-tid-t4604001.html*  
(3) *Impulsumrechung - http://www.rolandgruber.de/OpelTID.htm*  
(4) *AVR Timer - https://www.mikrocontroller.net/articles/AVR-GCC-Tutorial/Die_Timer_und_Z%C3%A4hler_des_AVR*  
(5) *AVR Timer konfigurieren - http://evolutec.publicmsg.de/index.php?menu=software&content=prescalertools*  
(6) *TID Steckerbelegung - PDF im Projektordner*  
