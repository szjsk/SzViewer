#pragma once

#include <QtWidgets/QMainWindow>
#include <QMenu>
#include "common/HistoryProps.h"


class SzViewer : public QMainWindow
{
    Q_OBJECT

public:
    SzViewer(QWidget *parent = nullptr);
    ~SzViewer();

private:
    bool isTextFile(const QString& fileName);
    void openFile(QString fileName);
    void addHistoryCheckBox(QMenu* fileMenu, HistoryProps props, HistoryProps::SavedType type);

protected:
    void resizeEvent(QResizeEvent* event);
    void dropEvent(QDropEvent* event);
	void dragEnterEvent(QDragEnterEvent* event);
    QToolBar* CommonLeft();
    QToolBar* CommonRight();
    void changeVisible(bool isCurrentTextView);

private slots:
    void openFileDialog();
	void handleDeleteKey(QStringList files, QString nextFile);
};