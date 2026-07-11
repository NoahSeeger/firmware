# LilyGO T-Embed CC1101 Plus – Fehlerprotokoll

Dieses Dokument hält die gemeldeten Probleme und die technische Untersuchung fest.
Der Fokus liegt auf dem T-Embed CC1101 Plus; andere Boards werden erst danach
überprüft.

## Gemeldete Symptome

1. Bad USB funktioniert unter Bruce 1.14, unter 1.15 nicht mehr zuverlässig.
2. Windows erkennt die USB-Verbindung, aber ein gestartetes Skript erzeugt keine
   Tasteneingaben am angeschlossenen PC. Die Ausführung wird gleichzeitig auf dem
   Bruce-Display angezeigt.
3. Das Gerät startet gelegentlich neu, wenn im Bad-USB-Modus das USB-Kabel
   angeschlossen wird.
4. Encoder-/Zurück-Tasten reagieren teilweise nicht.
5. Der obere Zurückbutton soll bei mindestens 15 Sekunden Halten unabhängig vom
   aktuellen Zustand einen Neustart auslösen.

## Vergleich 1.14 zu 1.15

- USB-Pins und `USB_as_HID` des T-Embed CC1101 sind in beiden Tags gleich.
- Der eigentliche `USBHIDKeyboard`-Treiber ist in beiden Tags gleich.
- Der bisherige Startpfad wartete nur auf `tud_mounted()`. Das zeigt USB-
  Enumeration, garantiert aber nicht, dass der HID-Interrupt-Endpunkt bereits
  sendebereit ist.
- `USBHIDKeyboard::sendReport()` liefert bei einem nicht bereiten Endpunkt
  `false`; der bisherige Tastaturcode ignorierte diesen Fehler.

## Aktuelle Korrekturen im Fork

- USB wartet jetzt zusätzlich auf `USBHIDKeyboard::isConnected()`, also auf den
  tatsächlich sendebereiten HID-Endpunkt.
- Die USB-Wartephase hat ein 15-Sekunden-Limit und meldet einen Fehler, statt ein
  Skript mit verlorenen Reports zu starten.
- Der T-Embed-Input-Task überwacht den Zurückbutton unabhängig vom UI/Main-Task.
  Nach 15 Sekunden wird `ESP.restart()` ausgelöst.
- Der Menü-Rebootpfad verwendet denselben 15-Sekunden-Neustart statt des bisher
  konkurrierenden Deep-Sleep-/Restart-Modus.

## BadUSB-HID-Analyse 1.14 vs. 1.15

Bruce 1.15 ergÃ¤nzt ein U2F-HID-GerÃ¤t. In der ursprÃ¼nglichen 1.15-Version
registrierte sich dieses GerÃ¤t bereits wÃ¤hrend der globalen Konstruktion vor
dem BadUSB-Keyboard. Die verwendete Arduino-TinyUSB-Schicht sendet
Keyboard-Reports jedoch Ã¼ber HID-Interface 0. Dadurch konnte Windows das
USB-GerÃ¤t enumerieren, wÃ¤hrend die Reports nicht als Tastatureingaben ankamen.

Der Fork registriert das BadUSB-Keyboard jetzt frÃ¼hzeitig als Interface 0 und
registriert U2F erst beim Ã–ffnen des U2F-MenÃ¼s. ZusÃ¤tzlich kann der
ZurÃ¼ckbutton ein laufendes BadUSB-Skript sauber abbrechen; Text, Delays,
Wiederholungen und Wartephasen prÃ¼fen den Abbruch kooperativ und geben am Ende
alle gedrÃ¼ckten Tasten frei.

## Noch zu verifizieren

Zusätzliche technische Ursache aus der zweiten Prüfung:

- SD-Dateimanager und serielle BadUSB-Befehle löschten das USB-HID-Objekt nach
  dem Lauf. TinyUSB behält den registrierten Gerätezeiger länger; das konnte
  spätere Läufe und Reconnects destabilisieren.
- Diese Einstiegspfade behalten das USB-HID-Objekt nun ebenfalls am Leben.
- Einzelne HID-Reports werden bei einem kurzen Endpoint-Aussetzer bis zu 100 ms
  erneut versucht.
- Der T-Embed-Build wurde erfolgreich erzeugt; offen ist der Test auf echter
  Hardware.
- Die 15-Sekunden-Recovery läuft zusätzlich in einem eigenen FreeRTOS-Task,
  damit sie auch bei einem suspendierten UI/Input-Task erreichbar bleibt.

- Build für `lilygo-t-embed-cc1101`.
- USB-Enumeration und HID-Ready-Zustand mit einem Windows-PC.
- Bad USB mit einem kurzen Testskript (z. B. `STRING` und `ENTER`).
- Anschluss des USB-Kabels vor und während des Bad-USB-Menüs; dabei serielle
  Reset-Ursache und Reset-Grund (`esp_reset_reason()`) auswerten.
- Tasten während eines laufenden Skripts und bei blockierter USB-Wartephase.
