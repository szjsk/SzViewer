#pragma once

#include <QtWidgets/QMainWindow>
#include "textviewer/TextViewContainer.h"
#include "textviewer/TextSettingDialog.h"
#include "textviewer/TextSearchDialog.h"
#include "textviewer/TextToolBar.h"
#include "AboutDialog.h"
#include "StatusStore.h"
#include "imageviewer/ImageViewContainer.h"
#include "imageviewer/ImageToolbar.h"

#include <qfiledialog.h>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFontDialog>
#include <QStackedWidget>
#include <QMenuBar>
#include <QToolbar>
#include <QSettings>

class SzViewer : public QMainWindow
{
    Q_OBJECT

public:
    SzViewer(QWidget *parent = nullptr);
    ~SzViewer();

private:
    TextViewContainer* m_textViewContainer;
    ImageViewContainer* m_imageViewContainer;
	QStackedWidget* m_stackedWidget;
    TextToolBar* m_textToolBar;
    ImageToolBar* m_imageToolBar;
    bool isTextFile(const QString& fileName);
    void openFile(QString& fileName);

protected:
    void resizeEvent(QResizeEvent* event);
    void dropEvent(QDropEvent* event);
	void dragEnterEvent(QDragEnterEvent* event);
    QToolBar* CommonLeft();
    QToolBar* CommonRight();
    void changeVisible(bool isCurrentTextView);

private slots:
    void openFileDialog();
};