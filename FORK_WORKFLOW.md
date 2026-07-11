# Eigener Bruce-Fork

Dieses Repository basiert auf Bruce 1.15.

## Projektfokus

Unser primäres Zielgerät ist das **LilyGO T-Embed CC1101 Plus**. Neue
Verbesserungen, Fehlerbehebungen und Tests werden zuerst für dieses Board
entwickelt und validiert. Andere von Bruce unterstützte Boards bleiben möglichst
funktionsfähig, haben aber zunächst keine Priorität.

## Branches

- `main`: unser stabiler Stand, der nur getestete Änderungen enthält
- `custom/1.15`: Arbeitsbranch für Änderungen, die auf Bruce 1.15 basieren
- `upstream/*`: offizielle Bruce-Zweige und -Tags; dort wird niemals direkt gearbeitet

## Arbeitsregel

Jede Änderung bekommt einen eigenen, kleinen Commit. Ein Commit sollte genau einen
Bugfix oder eine klar abgegrenzte Verbesserung enthalten. Vor dem Commit bauen und
testen wir möglichst das betroffene Board bzw. die betroffene Funktion.

## Offizielles Update übernehmen

Wenn Bruce 1.16 oder später erscheint:

```powershell
git fetch upstream --tags
git switch main
git merge 1.16
git switch custom/1.15
git rebase main
```

Bei Konflikten wird nicht blind weitergemacht: Die betroffenen Dateien werden
gemeinsam geprüft, der Konflikt gelöst und anschließend der Build getestet.

## Eigener GitHub-Fork

Sobald ein GitHub-Fork angelegt wurde, wird er als `origin` ergänzt:

```powershell
git remote add origin https://github.com/<DEIN-ACCOUNT>/<DEIN-REPO>.git
git push -u origin main
git push -u origin custom/1.15
```

`upstream` bleibt dabei immer `https://github.com/BruceDevices/firmware.git`.

## Repository-Aufteilung

Die lokale Kopie liegt in unserem Arbeitsordner. Nach dem Anlegen eines GitHub-
Forks sieht die Aufteilung so aus:

```text
origin   = dein GitHub-Fork       (push/pull unserer Änderungen)
upstream = BruceDevices/firmware  (nur offizielle Updates)
```

Wir arbeiten niemals direkt auf `upstream` und speichern dort auch keine eigenen
Änderungen.
