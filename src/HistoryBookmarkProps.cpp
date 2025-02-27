#include "HistoryBookmarkProps.h"
#include <QJsonObject>

HistoryBookmarkProps::HistoryBookmarkProps(SavedType value)
    : m_savedType(value)
{
}

HistoryBookmarkProps::SavedType HistoryBookmarkProps::getSavedType() const { return m_savedType; }

SavedFileInfo HistoryBookmarkProps::getFileInfo(QString fileName) {
    return m_fileInfos.value(fileName);
}

QHash<QString, SavedFileInfo> HistoryBookmarkProps::getFileInfos() {
    return m_fileInfos;
}


void HistoryBookmarkProps::addFileInfo(QString filename, long textPosition, QString textLine) {

	SavedFileInfo fileInfo;
    fileInfo.fileName = filename;
    fileInfo.textPosition = textPosition;
    fileInfo.textLine = textLine;

    m_fileInfos.insert(fileInfo.fileName, fileInfo);
}

QJsonObject HistoryBookmarkProps::toJson() const {
    QJsonObject json;
    json["savedType"] = m_savedType;

    QJsonArray fileInfosArray;
    for (const auto& fileInfo : m_fileInfos) {
        fileInfosArray.append(fileInfo.toJson());
    }
    json["fileInfos"] = fileInfosArray;
    return json;
}

void HistoryBookmarkProps::fromJson(const QJsonObject &json) {
    m_savedType = static_cast<SavedType>(json["savedType"].toInt());

    m_fileInfos.clear();
    QJsonArray fileInfosArray = json["fileInfos"].toArray();
    for (const auto& fileInfoValue : fileInfosArray) {
        SavedFileInfo fileInfo;
        fileInfo.fromJson(fileInfoValue.toObject());
        m_fileInfos.insert(fileInfo.fileName, fileInfo);
    }

}
QString HistoryBookmarkProps::getFileName() {
    switch (m_savedType) {
    case HistoryBookmarkProps::TEXT_HISTORY:
        return "text_history.json";
    case HistoryBookmarkProps::TEXT_BOOKMARK:
        return "text_bookmark.json";
    case HistoryBookmarkProps::IMAGE_HISTORY:
        return "image_history.json";
    case HistoryBookmarkProps::IMAGE_BOOKMARK:
        return "image_bookmark.json";
    default:
        return "unknown.json";
    }
}