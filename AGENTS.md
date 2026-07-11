# AGENTS.md – Arbeitsanleitung für Codex

Diese Datei ist bei jeder Codex-Session zuerst zu lesen. Ergänzende Details
stehen in [FORK_WORKFLOW.md](FORK_WORKFLOW.md) und
[docs/T_EMBED_ISSUES.md](docs/T_EMBED_ISSUES.md).

## Projektziel

Dieses Repository ist Noah Seegers eigener Fork von Bruce Firmware 1.15.
Primäres Zielgerät ist das **LilyGO T-Embed CC1101 Plus**. Neue Fixes,
Verbesserungen und Tests werden zuerst für dieses Board entwickelt und gebaut.
Andere Bruce-Boards sollen möglichst nicht beschädigt werden, haben aber keine
Priorität vor dem T-Embed.

Fokus: native USB-HID-/BadUSB-Funktion, stabile USB-Reconnects, Encoder,
Zurückbutton und ein unabhängiger 15-Sekunden-Recovery-Neustart. Änderungen
sollen klein, nachvollziehbar und updatefähig sein.

Nur eigene Geräte und autorisierte Testsysteme verwenden. Keine Zugangsdaten,
Tokens oder privaten Schlüssel ins Repository schreiben.

## Repository-Struktur

| Pfad | Zweck |
|---|---|
| `src/` | zentrale Firmware, Menüs und Module |
| `boards/` | Board-Pins, Interfaces und PlatformIO-Konfiguration |
| `boards/lilygo-t-embed-cc1101/` | wichtigste Board-Implementierung |
| `lib/Bad_Usb_Lib/` | USB-HID-, BLE-HID- und serielle Tastaturabstraktion |
| `src/modules/badusb_ble/` | DuckyScript-/BadUSB-/BadBLE-Ablauf |
| `src/core/` | Hauptloop, Eingabe, Display, Speicher und Systemfunktionen |
| `include/` | globale Header und Konfiguration |
| `docs/` | Fehlerprotokolle und technische Notizen |
| `.github/workflows/` | CI- und Build-Workflows |
| `.pio/` | lokale PlatformIO-Artefakte; niemals committen |

## Git-Modell

```text
origin   https://github.com/NoahSeeger/firmware.git
upstream https://github.com/BruceDevices/firmware.git
```

- `upstream` liefert offizielle Bruce-Tags und -Änderungen.
- Eigene Commits werden ausschließlich auf `origin` gepusht.
- `custom/1.15` ist der historische Arbeitszweig für Bruce 1.15.
- `main` dient lokal als sauberer Upstream-Baseline-Zweig.
- Vor jeder Änderung `git status`, Branch und Remotes prüfen.
- Keine destruktiven Befehle wie `git reset --hard` oder `git checkout --`
  ohne ausdrückliche Zustimmung.
- Ein Bugfix oder eine klar abgegrenzte Verbesserung pro Commit.
- Vor dem Push `git diff --check` und den passenden Build ausführen.

### Spätere Bruce-Versionen

Den 1.15-Zweig nicht umschreiben. Für ein neues Release einen neuen Zweig
anlegen und eigene Fix-Commits gezielt übernehmen:

```powershell
git fetch upstream --tags
git switch main
git merge --ff-only 1.16
git switch -c custom/1.16
git log --oneline 1.15..custom/1.15
git cherry-pick <eigene-fix-commits>
```

Vorher mit `git range-diff` und den betroffenen Dateien vergleichen. Konflikte
niemals blind auflösen; danach T-Embed bauen und die betroffenen Funktionen
testen. Bereits gepushte Branches nicht ohne Absprache rebasen oder
force-pushen.

## Session-Start

```powershell
Get-Location
git status --short
git branch --show-current
git remote -v
```

Danach diese Datei, `FORK_WORKFLOW.md` und bei T-Embed-/USB-Arbeiten
`docs/T_EMBED_ISSUES.md` lesen. Bestehende lokale Änderungen respektieren und
nicht überschreiben. Vor einer Änderung Symptom, betroffene Pfade und eine
prüfbare Ursache festhalten.

## Build und Test

```powershell
pio run -e lilygo-t-embed-cc1101
```

Falls `pio` nicht im PATH ist:

```powershell
& "$env:USERPROFILE\.platformio\penv\Scripts\pio.exe" run -e lilygo-t-embed-cc1101
```

Upload erst starten, wenn das Board wirklich erkannt wird:

```powershell
pio device list
pio run -e lilygo-t-embed-cc1101 -t upload
```

Ein erfolgreicher Compile beweist nur, dass der Quellcode gebaut wurde. Für
USB- oder Tastenfehler ist zusätzlich ein Hardwaretest erforderlich.

### T-Embed-Abnahmetest

1. Gerät starten und Encoder, Select und Zurück testen.
2. BadUSB öffnen und die HID-Erkennung am eigenen Windows-PC prüfen.
3. Harmloses Testskript mit `STRING`, `ENTER` und ausreichenden Delays ausführen.
4. Dasselbe Skript zweimal nacheinander ausführen.
5. USB im BadUSB-Menü und nach einem Lauf trennen/verbinden.
6. Oberen Zurückbutton 15 Sekunden halten und Neustart prüfen.
7. Bei Reboots Reset-Grund und serielle Meldungen dokumentieren.

Keinen Erfolg behaupten, solange die zum Symptom passende Hardwareprüfung fehlt.

## Bekannte technische Hinweise

- Beim nativen TinyUSB-HID darf das registrierte `USBHIDKeyboard`-Objekt nicht
  nach jedem BadUSB-Lauf gelöscht werden; TinyUSB behält den Gerätezeiger.
- `tud_mounted()` allein beweist keine Sendebereitschaft des HID-Endpoints.
- Die T-Embed-Recovery läuft in einem eigenen Task, damit sie nicht vom
  blockierten oder suspendierten UI-/Input-Task abhängt.
- Neue Erkenntnisse in `docs/T_EMBED_ISSUES.md` ergänzen.
- Lokale PlatformIO-/Toolchain-Reparaturen gehören nicht ungeprüft in Commits.

## Session-Abschluss

```powershell
git diff --check
git status --short
git log -1 --oneline
```

Bericht getrennt nach geändertem Code/Commit, Builds/Tests, ungetesteten
Punkten und dem nächsten konkreten Schritt ausgeben.
