#ifndef STATUSSTORE_H
#define STATUSSTORE_H

#include <QFont>
#include <QColor>
#include <QMargins>
#include "textviewer/TextSettingProps.h"
#include "imageviewer/ImageSettingProps.h"
#include "IJsonSerializable.h"
#include <QFileInfo>

class StatusStore
{
public:
    static StatusStore& instance() {
        static StatusStore instance;
        return instance;
    }

    IJsonSerializable* loadSetting(IJsonSerializable* jsonObj);
    void saveSetting(IJsonSerializable* jsonObj);

    TextSettingProps getTextSettings();
    void setTextSettings(const TextSettingProps& settings);
    ImageSettingProps getImageSettings();
	void setImageSettings(const ImageSettingProps& settings);

private:
    StatusStore();
    ~StatusStore();
    StatusStore(const StatusStore&) = delete;
    StatusStore& operator=(const StatusStore&) = delete;

    TextSettingProps m_textSettingProps;
    ImageSettingProps m_imageSettingProps;

};

#endif // StatusStore_H
