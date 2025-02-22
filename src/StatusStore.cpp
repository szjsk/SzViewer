#include "StatusStore.h"
#include <QFile>
#include <QJsonDocument>

StatusStore::StatusStore()
{
    //loadSetting(QString fileName);
}

StatusStore::~StatusStore()
{
    //saveSetting();
}

IJsonSerializable* StatusStore::loadSetting(IJsonSerializable* jsonObj)
{

    QString fileName = jsonObj->getFileName();
    QFileInfo fileInfo(fileName);
    QString absolutePath = fileInfo.absoluteFilePath();

    QFile loadFile(jsonObj->getFileName());
    if (!loadFile.open(QIODevice::ReadOnly)) {
        return jsonObj;
    }

    QByteArray data = loadFile.readAll();
    loadFile.close();

    QJsonDocument loadDoc = QJsonDocument::fromJson(data);
    if (!loadDoc.isNull() && loadDoc.isObject())
    {
        QJsonObject json = loadDoc.object();
		qDebug() << "Loading json:" << json;
        jsonObj->fromJson(json);
    }
    return jsonObj;
}

void StatusStore::saveSetting(IJsonSerializable* jsonObj)
{

    QFile saveFile(jsonObj->getFileName());

    if (!saveFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
		qDebug() << "Couldn't open save file : " << jsonObj->getFileName();
        return;
    }

    QJsonObject json = jsonObj->toJson();
    QJsonDocument saveDoc(json);
    saveFile.write(saveDoc.toJson());

    saveFile.close();
}

TextSettingProps StatusStore::getTextSettings() {

    loadSetting(&m_textSettingProps);

    return m_textSettingProps;
}

void StatusStore::setTextSettings(const TextSettingProps& newSettings) {
    m_textSettingProps = newSettings;
    saveSetting(&m_textSettingProps);
}

ImageSettingProps StatusStore::getImageSettings() {

    loadSetting(&m_imageSettingProps);

    return m_imageSettingProps;
}

void StatusStore::setImageSettings(const ImageSettingProps& newSettings) {
    m_imageSettingProps = newSettings;
    saveSetting(&m_imageSettingProps);
}