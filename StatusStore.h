#ifndef STATUSSTORE_H
#define STATUSSTORE_H

#include <QFont>
#include <QColor>
#include <QMargins>
#include "textviewer/TextSettingProps.h"
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

private:
    StatusStore();
    ~StatusStore();
    StatusStore(const StatusStore&) = delete;
    StatusStore& operator=(const StatusStore&) = delete;

    TextSettingProps textSettingProps;
};

#endif // StatusStore_H
