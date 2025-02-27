#ifndef STATUSSTORE_H
#define STATUSSTORE_H

#include <QFont>
#include <QColor>
#include <QMargins>
#include "textviewer/TextSettingProps.h"
#include "imageviewer/ImageSettingProps.h"
#include "HistoryBookMarkProps.h"
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

    HistoryBookmarkProps getTextHistory();
    void setTextHistory(const HistoryBookmarkProps& settings);

    HistoryBookmarkProps getTextBookmark();
    void setTextBookmark(const HistoryBookmarkProps& settings);

    HistoryBookmarkProps getImageHistory();
    void setImageHistory(const HistoryBookmarkProps& settings);

    HistoryBookmarkProps getImageBookmark();
    void setImageBookmark(const HistoryBookmarkProps& settings);

private:
    StatusStore();
    ~StatusStore();
    StatusStore(const StatusStore&) = delete;
    StatusStore& operator=(const StatusStore&) = delete;

    TextSettingProps m_textSettingProps;
    ImageSettingProps m_imageSettingProps;
    HistoryBookmarkProps m_textHistory = HistoryBookmarkProps(HistoryBookmarkProps::TEXT_HISTORY);
    HistoryBookmarkProps m_textBookmark = HistoryBookmarkProps(HistoryBookmarkProps::TEXT_BOOKMARK);
    HistoryBookmarkProps m_imageHistory = HistoryBookmarkProps(HistoryBookmarkProps::IMAGE_HISTORY);
    HistoryBookmarkProps m_imageBookmark = HistoryBookmarkProps(HistoryBookmarkProps::IMAGE_BOOKMARK);

};

#endif // StatusStore_H
