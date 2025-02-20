#ifndef TEXTVIEWCONTAINER_H
#define TEXTVIEWCONTAINER_H

#include <QWidget>
#include <QTextBrowser>
#include <QHBoxLayout>
#include <QFile>
#include <QTextCursor>
#include <QTextDocument>
#include <QSizeF>
#include <QAbstractTextDocumentLayout>
#include <QTimer>
#include <Qslider>
#include <QLabel>
#include "TextSettingProps.h"

class TextViewContainer : public QWidget
{
    Q_OBJECT

public:
    TextViewContainer(QWidget* parent = nullptr, const TextSettingProps& settings = TextSettingProps());
    void loadText(QString& filePath);
    bool changeSplitView();
    void refreshPage();
    void nextPage();
    void prevPage();
    void setSettings(const TextSettingProps& s);
    QHash<long, QVector<QString>>* getTextChunks();
    void findPage(const QString&, long page, long line);

private:
    static constexpr int G_TEXT_BROWSER_CNT = 2;
    TextSettingProps g_settings;

    QString g_text;
    long g_currentPosition;
    QTextBrowser* g_textBrowserArray[G_TEXT_BROWSER_CNT];
    QLabel* qSliderInfo;
    QHash<QChar, int> g_charWidthCache;
    QHash<long, QVector<QString>> textChunks;  
    QTimer* resizeTimer;

    QTextBrowser* createTextBrowser(); //텍스트뷰 생성
    int getLineHeight(QTextBrowser* tb);
    int getFontWidth(QFontMetrics* tb, QChar c);
    void resizeEvent(QResizeEvent* event);
    void setPage(long position);
    void refreshFont(QTextBrowser* tb);
    void refreshStyle(QTextBrowser* tb);
    void applyLineSpacing(QTextBrowser* tb);

    QSlider* qSlider;

protected:
    bool eventFilter(QObject* watched, QEvent* event);

};

#endif // TEXTVIEWCONTAINER_H
