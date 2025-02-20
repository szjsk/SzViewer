#ifndef TEXTSEARCHDIALOG_H
#define TEXTSEARCHDIALOG_H

#include <QDialog>
#include <QString>

class QLineEdit;
class QPushButton;
class QTableWidget;

class TextSearchDialog : public QDialog
{
    Q_OBJECT

public:
    enum Mode { SearchMode, BookmarkMode };

    explicit TextSearchDialog(QWidget* parent = nullptr, const QHash<long, QVector<QString>>* textChunks = nullptr);
    ~TextSearchDialog();

    // 검색 모드 혹은 책갈피 모드 설정
    void setMode(Mode mode);

signals:
    void rowSelected(const QString& searchText, long page, long line);

private slots:
    // 검색 버튼 클릭 시 호출될 슬롯
    void onSearchButtonClicked();

private:
    Mode currentMode;
    const QHash<long, QVector<QString>>* textChunksRefs;
    QLineEdit* searchLineEdit;
    QPushButton* searchButton;
    QTableWidget* resultTable;
    void addResult(const QString& line, long page, long position);

};

#endif // TEXTSEARCHDIALOG_H
