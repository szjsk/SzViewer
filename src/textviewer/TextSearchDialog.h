#ifndef TEXTSEARCHDIALOG_H
#define TEXTSEARCHDIALOG_H

#include <QDialog>
#include <QString>
#include "TextViewContainer.h"

class QLineEdit;
class QPushButton;
class QTableWidget;

class TextSearchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TextSearchDialog(QWidget* parent = nullptr, TextViewContainer* textViewContainer = nullptr);
    ~TextSearchDialog();

signals:
    void rowSelected(const QString& searchText, long page, long line);
    void searchResultReady(const QString& line, long page, long position);


private:
    void addResult(QString line, long page, long position);
};

#endif // TEXTSEARCHDIALOG_H
