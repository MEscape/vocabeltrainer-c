# Vokabeltrainer - Sichere Verwendung ohne Secret-Exposition

## Übersicht

Diese Lösung ermöglicht es Ihnen, die ausführbare Datei und alle notwendigen DLLs zu verteilen, ohne dass Ihre Google OAuth Secrets preisgegeben werden.

## Wie es funktioniert

### Vorher (Unsicher)
- Secrets waren direkt im Code hardcodiert
- JSON-Datei mit Credentials wurde in die .exe eingebettet
- Jeder mit Zugang zur .exe konnte die Secrets extrahieren

### Nachher (Sicher)
- Secrets werden aus einer externen `config.json` Datei geladen
- Die .exe enthält KEINE eingebetteten Secrets
- Jeder Benutzer muss seine eigenen Credentials konfigurieren

## Schritt-für-Schritt Anleitung

### 1. Projekt kompilieren

Führen Sie eines der Build-Scripts aus:

**Option A: PowerShell (empfohlen)**
```powershell
.\build_clean.ps1
```

**Option B: Batch-Datei**
```cmd
build_clean.bat
```

### 2. Deployment vorbereiten

Nach dem erfolgreichen Build finden Sie im `deploy` Verzeichnis:
- `vocabeltrainer.exe` (saubere Version ohne Secrets)
- Alle notwendigen Qt-DLLs
- `config_example.json` (Vorlage)

### 3. Konfiguration für Endbenutzer

Jeder Benutzer muss:

1. **Konfigurationsdatei erstellen:**
   ```bash
   copy config_example.json config.json
   ```

2. **Eigene Google OAuth Credentials eintragen:**
   Bearbeiten Sie `config.json`:
   ```json
   {
       "google_oauth": {
           "client_id": "IHRE_ECHTE_CLIENT_ID",
           "client_secret": "IHR_ECHTES_CLIENT_SECRET", 
           "api_key": "IHR_ECHTER_API_KEY"
       }
   }
   ```

### 4. Anwendung starten

Die Anwendung sucht automatisch nach `config.json` in folgenden Orten:
1. Neben der .exe Datei
2. Im Benutzerverzeichnis (`~/.vokabeltrainer/config.json`)
3. In den Anwendungsdaten
4. Im aktuellen Arbeitsverzeichnis

## Sicherheitsvorteile

✅ **Keine eingebetteten Secrets** - Die .exe ist sauber  
✅ **Individuelle Credentials** - Jeder Benutzer verwendet seine eigenen  
✅ **Versionskontrolle sicher** - Keine Secrets im Git Repository  
✅ **Einfache Verteilung** - .exe kann sicher geteilt werden  

## Wichtige Hinweise

⚠️ **Für Entwickler:**
- Committen Sie NIEMALS echte `config.json` Dateien
- Verwenden Sie nur `config_example.json` als Vorlage
- Die echte `config.json` ist in `.gitignore` ausgeschlossen

⚠️ **Für Endbenutzer:**
- Teilen Sie Ihre `config.json` NIEMALS mit anderen
- Bewahren Sie eine Sicherungskopie an einem sicheren Ort auf
- Die Credentials sind vertraulich und persönlich

## Fehlerbehebung

### "Konfigurationsdatei konnte nicht geladen werden"
- Stellen Sie sicher, dass `config.json` existiert
- Überprüfen Sie die JSON-Syntax
- Verwenden Sie `config_example.json` als Vorlage

### "Unvollständige Google OAuth Credentials"
- Alle drei Felder müssen ausgefüllt sein: client_id, client_secret, api_key
- Überprüfen Sie Ihre Google Cloud Console Einstellungen

### Anwendung startet nicht
- Überprüfen Sie, ob alle DLL-Dateien vorhanden sind
- Stellen Sie sicher, dass Qt korrekt installiert ist
- Prüfen Sie die Windows-Ereignisanzeige für Details

## Für Fortgeschrittene

### Alternative Konfigurationspfade

Sie können die Konfigurationsdatei auch an anderen Orten platzieren:
- `%USERPROFILE%\.vokabeltrainer\config.json`
- `%APPDATA%\Vokabeltrainer\config.json`

### Umgebungsvariablen (optional)

Für automatisierte Deployments können Sie auch Umgebungsvariablen verwenden:
- `VOKABELTRAINER_CLIENT_ID`
- `VOKABELTRAINER_CLIENT_SECRET`
- `VOKABELTRAINER_API_KEY`

(Diese Funktion müsste zusätzlich implementiert werden)

## Support

Bei Problemen überprüfen Sie:
1. Qt Installation und PATH
2. Google OAuth Credentials Gültigkeit
3. JSON-Syntax der Konfigurationsdatei
4. Dateiberechtigungen