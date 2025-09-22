#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QDebug>

class ConfigManager
{
public:
    static ConfigManager& getInstance();
    
    // Lädt die Konfiguration aus einer externen Datei
    bool loadConfig(const QString& configPath = "");
    
    // Getter für Google OAuth Credentials
    QString getGoogleClientId() const;
    QString getGoogleClientSecret() const;
    QString getGoogleApiKey() const;
    
    // Prüft ob die Konfiguration geladen wurde
    bool isConfigLoaded() const;
    
    // Gibt den Pfad zur Konfigurationsdatei zurück
    QString getConfigPath() const;

private:
    ConfigManager() = default;
    ~ConfigManager() = default;
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;
    
    QJsonObject m_config;
    bool m_configLoaded = false;
    QString m_configPath;
    
    // Sucht nach der Konfigurationsdatei in verschiedenen Pfaden
    QString findConfigFile() const;
};

#endif // CONFIGMANAGER_H