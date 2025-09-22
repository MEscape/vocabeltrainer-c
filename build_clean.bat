@echo off
echo === Vokabeltrainer Clean Build Script ===

REM Prüfe ob qmake verfügbar ist
where qmake >nul 2>nul
if %errorlevel% neq 0 (
    echo FEHLER: qmake nicht gefunden! Bitte Qt installieren oder PATH setzen.
    pause
    exit /b 1
)

REM Erstelle Build-Verzeichnis
if exist build_clean rmdir /s /q build_clean
mkdir build_clean
cd build_clean

echo Generiere Makefile mit qmake...
qmake ..\vocabeltrainer.pro
if %errorlevel% neq 0 (
    echo FEHLER: qmake fehlgeschlagen!
    pause
    exit /b 1
)

echo Kompiliere Projekt...
nmake
if %errorlevel% neq 0 (
    echo FEHLER: Kompilierung fehlgeschlagen!
    pause
    exit /b 1
)

REM Erstelle Deployment-Verzeichnis
cd ..
if exist deploy rmdir /s /q deploy
mkdir deploy

REM Kopiere ausführbare Datei
if exist build_clean\release\vocabeltrainer.exe (
    copy build_clean\release\vocabeltrainer.exe deploy\
    echo Ausführbare Datei kopiert: vocabeltrainer.exe
) else if exist build_clean\debug\vocabeltrainer.exe (
    copy build_clean\debug\vocabeltrainer.exe deploy\
    echo Ausführbare Datei kopiert (Debug): vocabeltrainer.exe
) else (
    echo FEHLER: Ausführbare Datei nicht gefunden!
    pause
    exit /b 1
)

REM Kopiere Qt-Abhängigkeiten
cd deploy
windeployqt vocabeltrainer.exe

REM Kopiere Beispiel-Konfiguration
copy ..\config_example.json config_example.json

echo.
echo === BUILD ERFOLGREICH ABGESCHLOSSEN ===
echo Deployment-Verzeichnis: %CD%
echo.
echo Nächste Schritte:
echo 1. Kopieren Sie config_example.json zu config.json
echo 2. Bearbeiten Sie config.json mit Ihren echten Google OAuth Credentials
echo 3. Testen Sie die Anwendung
echo.
echo WICHTIG: Diese Version enthält KEINE eingebetteten Secrets!

cd ..
pause