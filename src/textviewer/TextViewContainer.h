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
#include <QSlider>
#include <QLabel>
#include "TextSettingProps.h"
#include <QFileInfo>
#include <QDir>
#include <QCollator>

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
    static constexpr int M_TEXT_BROWSER_CNT = 2;
    TextSettingProps m_settings;
    QString m_text;
    QString m_fileName;
    long m_currentPosition;
    QTextBrowser* m_textBrowserArray[M_TEXT_BROWSER_CNT];
    QLabel* m_qSliderInfo;
    QHash<QChar, int> m_charWidthCache;
    QHash<long, QVector<QString>> m_textChunks;  
    QTimer* m_resizeTimer;
    QSlider* m_qSlider;

    QTextBrowser* createTextBrowser(); //텍스트뷰 생성
    int getLineHeight(QTextBrowser* tb);
    int getFontWidth(QFontMetrics* tb, QChar c);
    void resizeEvent(QResizeEvent* event);
    void setPage(long position);
    void refreshFont(QTextBrowser* tb);
    void refreshStyle(QTextBrowser* tb);
    void applyLineSpacing(QTextBrowser* tb);
    QString getNextOrPrevFileName(int nextOrPrev);

protected:
    bool eventFilter(QObject* watched, QEvent* event);

};

#endif // TEXTVIEWCONTAINER_H
