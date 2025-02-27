#ifndef TEXTVIEWCONTAINER_H
#define TEXTVIEWCONTAINER_H
#include <QWidget>
#include <QTextBrowser>
#include "TextSettingProps.h"
#include "../common/HistoryProps.h"

class TextViewContainer : public QWidget
{
    Q_OBJECT

public:

    struct PageInfo {
        long firstPosition = -1;
        QVector<QString> lines;
    };

    struct FileInfo {
		QString fileName;
        QString fileNameWithPath;
        int currentPageIdx;
        long currentPosition;
        QStringList fileList;
		QString nextFile;
		QString prevFile;
        QString text;
        QHash<long, PageInfo> pageInfos;
    };

    TextViewContainer(QWidget* parent = nullptr);
    ~TextViewContainer();
    void initTextFile(QString& filePath);
    bool changeSplitView();


    void findPage(const QString&, long page, long line);
	void deleteFile(const FileInfo* fileInfo);
	void clear();
    int findTextPageBy(const FileInfo* fileInfo, long position);
    const FileInfo* getFileInfo();
    void performSearch(QString searchText, const FileInfo* fileInfo);
    void refreshPage(long textPosition);
    void changeStyle(TextSettingProps s);


private:
    QHash<long, PageInfo> calculatePage(const FileInfo* fileInfo, int maxLine, int maxWidth, QTextBrowser* browser);
    QTextBrowser* createTextBrowser(TextSettingProps settings); //텍스트뷰 생성
    int getMaxHeight(QTextBrowser* tb);
    int getMaxWidth(QTextBrowser* tb);

    int getFontWidth(QFontMetrics* tb, QChar c);
    void setPage(FileInfo* fileInfo, int newPageIdx);
    void refreshStyle(TextSettingProps settings, QTextBrowser* tb);
    void applyLineSpacing(QTextBrowser* tb);
    void nextPage(const FileInfo* fileInfo);
    void prevPage(const FileInfo* fileInfo);
    void saveHistory(HistoryProps& history, const FileInfo* fileInfo);
    SavedFileInfo loadHistory(HistoryProps history, QString filePath);
    void testText();

protected:
    bool eventFilter(QObject* watched, QEvent* event);

signals:
    void deleteKeyPressed(QStringList files, QString nextFile);
	void searchResultReady(QString searchText, long page, int line);
};

#endif // TEXTVIEWCONTAINER_H
