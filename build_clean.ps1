# Build Script für Vokabeltrainer ohne eingebettete Secrets
# Dieses Script erstellt eine saubere ausführbare Datei

Write-Host "=== Vokabeltrainer Clean Build Script ===" -ForegroundColor Green

# Prüfe ob Qt installiert ist
$qtPath = $env:QTDIR
if (-not $qtPath) {
    Write-Host "WARNUNG: QTDIR Umgebungsvariable nicht gesetzt. Versuche Qt zu finden..." -ForegroundColor Yellow
    # Versuche Qt in Standard-Pfaden zu finden
    $possiblePaths = @(
        "C:\Qt\6.5.0\msvc2019_64\bin",
        "C:\Qt\6.4.0\msvc2019_64\bin",
        "C:\Qt\5.15.2\msvc2019_64\bin"
    )
    
    foreach ($path in $possiblePaths) {
        if (Test-Path "$path\qmake.exe") {
            $qtPath = $path
            Write-Host "Qt gefunden in: $qtPath" -ForegroundColor Green
            break
        }
    }
    
    if (-not $qtPath) {
        Write-Host "FEHLER: Qt nicht gefunden! Bitte installieren Sie Qt oder setzen Sie QTDIR." -ForegroundColor Red
        exit 1
    }
}

# Setze Qt Pfad
$env:PATH = "$qtPath;$env:PATH"

Write-Host "Verwende Qt aus: $qtPath" -ForegroundColor Cyan

# Erstelle Build-Verzeichnis
$buildDir = "build_clean"
if (Test-Path $buildDir) {
    Write-Host "Lösche altes Build-Verzeichnis..." -ForegroundColor Yellow
    Remove-Item -Recurse -Force $buildDir
}

New-Item -ItemType Directory -Path $buildDir | Out-Null
Set-Location $buildDir

Write-Host "Generiere Makefile mit qmake..." -ForegroundColor Cyan
& qmake.exe ..\vocabeltrainer.pro

if ($LASTEXITCODE -ne 0) {
    Write-Host "FEHLER: qmake fehlgeschlagen!" -ForegroundColor Red
    exit 1
}

Write-Host "Kompiliere Projekt..." -ForegroundColor Cyan
& nmake.exe

if ($LASTEXITCODE -ne 0) {
    Write-Host "FEHLER: Kompilierung fehlgeschlagen!" -ForegroundColor Red
    exit 1
}

# Erstelle Deployment-Verzeichnis
$deployDir = "..\deploy"
if (Test-Path $deployDir) {
    Remove-Item -Recurse -Force $deployDir
}

New-Item -ItemType Directory -Path $deployDir | Out-Null

# Kopiere ausführbare Datei
$exeName = "vocabeltrainer.exe"
if (Test-Path "release\$exeName") {
    Copy-Item "release\$exeName" $deployDir
    Write-Host "Ausführbare Datei kopiert: $exeName" -ForegroundColor Green
} elseif (Test-Path "debug\$exeName") {
    Copy-Item "debug\$exeName" $deployDir
    Write-Host "Ausführbare Datei kopiert (Debug): $exeName" -ForegroundColor Green
} else {
    Write-Host "FEHLER: Ausführbare Datei nicht gefunden!" -ForegroundColor Red
    exit 1
}

# Verwende windeployqt um Qt-Abhängigkeiten zu kopieren
Set-Location $deployDir
Write-Host "Kopiere Qt-Abhängigkeiten..." -ForegroundColor Cyan
& windeployqt.exe $exeName

if ($LASTEXITCODE -ne 0) {
    Write-Host "WARNUNG: windeployqt hatte Probleme, aber das ist oft normal." -ForegroundColor Yellow
}

# Erstelle Beispiel-Konfigurationsdatei im Deployment-Verzeichnis
Copy-Item "..\config_example.json" "config_example.json"

# Erstelle README für Benutzer
$readmeContent = @"
# Vokabeltrainer - Sichere Version

## Installation und Konfiguration

1. Kopieren Sie alle Dateien in ein Verzeichnis Ihrer Wahl
2. Kopieren Sie 'config_example.json' zu 'config.json'
3. Bearbeiten Sie 'config.json' und tragen Sie Ihre echten Google OAuth Credentials ein:
   - client_id: Ihre Google Client ID
   - client_secret: Ihr Google Client Secret  
   - api_key: Ihr Google API Key

## Wichtige Hinweise

- Die config.json Datei enthält Ihre privaten Credentials und sollte NIEMALS geteilt werden
- Bewahren Sie eine Sicherungskopie Ihrer config.json an einem sicheren Ort auf
- Diese Version der Anwendung enthält KEINE eingebetteten Secrets

## Starten der Anwendung

Führen Sie einfach vocabeltrainer.exe aus. Die Anwendung wird automatisch nach der config.json Datei suchen.

## Fehlerbehebung

Falls die Anwendung nicht startet:
1. Überprüfen Sie, ob alle DLL-Dateien vorhanden sind
2. Stellen Sie sicher, dass config.json korrekt formatiert ist
3. Überprüfen Sie, ob Ihre Google OAuth Credentials gültig sind
"@

Set-Content -Path "README.txt" -Value $readmeContent -Encoding UTF8

Write-Host ""
Write-Host "=== BUILD ERFOLGREICH ABGESCHLOSSEN ===" -ForegroundColor Green
Write-Host "Deployment-Verzeichnis: $((Get-Location).Path)" -ForegroundColor Cyan
Write-Host ""
Write-Host "Nächste Schritte:" -ForegroundColor Yellow
Write-Host "1. Kopieren Sie config_example.json zu config.json" -ForegroundColor White
Write-Host "2. Bearbeiten Sie config.json mit Ihren echten Google OAuth Credentials" -ForegroundColor White
Write-Host "3. Testen Sie die Anwendung" -ForegroundColor White
Write-Host ""
Write-Host "WICHTIG: Diese Version enthält KEINE eingebetteten Secrets!" -ForegroundColor Green

Set-Location ..