#ifndef IMAGESETTINGPROPS_H
#define IMAGESETTINGPROPS_H

#include <QFont>
#include <QColor>
#include <QMargins>
#include <QJsonObject>
#include "../common/IJsonSerializable.h"

class ImageSettingProps : public IJsonSerializable
{
public:
    ImageSettingProps();
    ImageSettingProps(bool splitView,
                 int scaleMode, 
                 bool isAuoNext);

    // getter, setter
    bool isSplitView() const;
    void setSplitView(bool value);

    int getScaleMode() const;
    void setScaleMode(int value);

	bool isAutoNext() const;
	void setAutoNext(bool value);

    // JSON 직렬화/역직렬화 메서드
    QJsonObject toJson() const override;
    void fromJson(const QJsonObject &json) override;
	QString getFileName() override;

private:
    bool m_splitView;
    int m_scaleMode;
    bool m_autoNext;

};

#endif // IMAGESETTINGPROPS_H
