#ifndef IMAGEVIEWCONTAINER_H
#define IMAGEVIEWCONTAINER_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSlider>
#include <QLabel>
#include <QScrollArea>
#include <QFileInfo>
#include <QEvent>
#include <QKeyEvent>
#include <QDir>
#include <QCollator>
#include <QMessageBox>
#include <QInputDialog>

#include "ImageView.h"
#include "ImageSettingProps.h"
#include "../common/StatusStore.h"

class ImageViewContainer : public QWidget
{
    Q_OBJECT

public:
    ImageViewContainer(QWidget* parent = nullptr);
	~ImageViewContainer();
	void loadFileList(QString filePath);
	bool changeSplitView();
	void navigateToFile(ImageView::MoveMode moveMode);
	void navigateToFolder(ImageView::MoveMode moveMode);
	void resizeImage(ImageView::ScaleMode scaleMode, int plusMinus);
	void deleteImageFile();
	void clear();



protected:
	bool eventFilter(QObject* watched, QEvent* event) override;

signals:
	void deleteKeyPressed(QStringList files , QString nextFile);

private:
	QHBoxLayout* createSlider();
	void sizeChange(ImageView::ScaleMode mode, int percentage);
	QStringList renameFile(QStringList fileList, int fileIdx, int containerIdx);
	QString renameFolder(QStringList fileList, int fileIdx);

private: //variable
	static constexpr int M_IMAGE_BROWSER_CNT = 2;
	QSlider* ui_qSlider;
	QLabel* ui_qSliderInfo;
	ImageView* ui_imageView[M_IMAGE_BROWSER_CNT];
	QStringList m_fileList;
	QString m_fileName;
	int m_currentIndex = 0;
	int m_percentage = 100;
	ImageView::ScaleMode m_scaleMode = ImageView::FitToWindow;
};

#endif // IMAGEVIEWCONTAINER_H
