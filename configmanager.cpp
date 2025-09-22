#include "configmanager.h"
#include <QCoreApplication>
#include <QMessageBox>

ConfigManager& ConfigManager::getInstance()
{
    static ConfigManager instance;
    return instance;
}

bool ConfigManager::loadConfig(const QString& configPath)
{
    QString path = configPath;
    
    // Wenn kein Pfad angegeben wurde, suche nach der Konfigurationsdatei
    if (path.isEmpty()) {
        path = findConfigFile();
    }
    
    if (path.isEmpty()) {
        qDebug() << "Keine Konfigurationsdatei gefunden!";
        return false;
    }
    
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Kann Konfigurationsdatei nicht öffnen:" << path;
        return false;
    }
    
    QByteArray data = file.readAll();
    file.close();
    
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    
    if (error.error != QJsonParseError::NoError) {
        qDebug() << "JSON Parse Error:" << error.errorString();
        return false;
    }
    
    m_config = doc.object();
    m_configLoaded = true;
    m_configPath = path;
    
    qDebug() << "Konfiguration erfolgreich geladen von:" << path;
    return true;
}

QString ConfigManager::getGoogleClientId() const
{
    if (!m_configLoaded) {
        qDebug() << "Konfiguration nicht geladen!";
        return QString();
    }
    
    QJsonObject googleOAuth = m_config["google_oauth"].toObject();
    return googleOAuth["client_id"].toString();
}

QString ConfigManager::getGoogleClientSecret() const
{
    if (!m_configLoaded) {
        qDebug() << "Konfiguration nicht geladen!";
        return QString();
    }
    
    QJsonObject googleOAuth = m_config["google_oauth"].toObject();
    return googleOAuth["client_secret"].toString();
}

QString ConfigManager::getGoogleApiKey() const
{
    if (!m_configLoaded) {
        qDebug() << "Konfiguration nicht geladen!";
        return QString();
    }
    
    QJsonObject googleOAuth = m_config["google_oauth"].toObject();
    return googleOAuth["api_key"].toString();
}

bool ConfigManager::isConfigLoaded() const
{
    return m_configLoaded;
}

QString ConfigManager::getConfigPath() const
{
    return m_configPath;
}

QString ConfigManager::findConfigFile() const
{
    QStringList searchPaths;
    
    // 1. Neben der ausführbaren Datei
    searchPaths << QCoreApplication::applicationDirPath() + "/config.json";
    
    // 2. Im Benutzerverzeichnis
    searchPaths << QDir::homePath() + "/.vokabeltrainer/config.json";
    
    // 3. In den Anwendungsdaten
    searchPaths << QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/config.json";
    
    // 4. Im aktuellen Arbeitsverzeichnis
    searchPaths << QDir::currentPath() + "/config.json";
    
    for (const QString& path : searchPaths) {
        if (QFile::exists(path)) {
            return path;
        }
    }
    
    return QString(); // Keine Konfigurationsdatei gefunden
}