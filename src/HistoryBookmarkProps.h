#ifndef HISTORYBOOKMARKPROPS_H
#define HISTORYBOOKMARKPROPS_H

#include <QFont>
#include <QColor>
#include <QMargins>
#include <QJsonObject>
#include <QJsonArray>
#include "IJsonSerializable.h"

struct SavedFileInfo {
    QString fileName;
    long long textPosition;
    QString textLine;

    QJsonObject toJson() const {
        QJsonObject json;
        json["fileName"] = fileName;
        json["textPosition"] = textPosition;
        json["textLine"] = textLine;
        return json;
    }

    void fromJson(const QJsonObject& json) {
        fileName = json["fileName"].toString();
        textPosition = json["textPosition"].toVariant().toLongLong();
        textLine = json["textLine"].toString();
    }
};

class HistoryBookmarkProps : public IJsonSerializable
{
public:

    enum SavedType {
        TEXT_HISTORY,
        TEXT_BOOKMARK,
        IMAGE_HISTORY,
        IMAGE_BOOKMARK
    };
    HistoryBookmarkProps(SavedType value);


    // getter, setter
    SavedType getSavedType() const;

    SavedFileInfo getFileInfo(QString fileName);
    QHash<QString, SavedFileInfo> getFileInfos();
    void addFileInfo(QString filename, long textPosition, QString textLine);

    // JSON 직렬화/역직렬화 메서드
    QJsonObject toJson() const override;
    void fromJson(const QJsonObject &json) override;
	QString getFileName() override;

private:
    SavedType m_savedType;
    QHash<QString, SavedFileInfo> m_fileInfos;
};

#endif // HISTORYBOOKMARKPROPS_H
