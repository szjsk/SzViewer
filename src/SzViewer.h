#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SzViewer.h"
#include "textviewer/TextViewContainer.h"
#include "textviewer/TextSettingDialog.h"
#include "StatusStore.h"

#include <qfiledialog.h>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFontDialog>

class SzViewer : public QMainWindow
{
    Q_OBJECT

public:
    SzViewer(QWidget *parent = nullptr);
    ~SzViewer();

private:
    Ui::SzViewerClass ui;
    QMenu* menu;
    TextViewContainer* textViewContainer;
    bool isTextFile(const QString& fileName);
    void openFile(QString& fileName);

protected:
    void resizeEvent(QResizeEvent* event);
    void dropEvent(QDropEvent* event);
	void dragEnterEvent(QDragEnterEvent* event);
    
private slots:
    void openFileDialog();
    void openFontDialog();
    void updateSettingPreview(const TextSettingProps& settings);
};
