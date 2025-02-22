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
#include "ImageView.h"
#include "ImageSettingProps.h"

class ImageViewContainer : public QWidget
{
    Q_OBJECT

public:
    ImageViewContainer(QWidget* parent = nullptr, const ImageSettingProps& settings = ImageSettingProps());
	void loadFileList(QString& filePath);
	bool changeSplitView();
	void navigateToFile(ImageView::MoveMode moveMode);
	void navigateToFolder(ImageView::MoveMode moveMode);
	void resizeImage(ImageView::ScaleMode scaleMode, int plusMinus);

private:
	static constexpr int M_IMAGE_BROWSER_CNT = 2;
	ImageSettingProps m_settings;
	QSlider* m_qSlider;
	QLabel* m_qSliderInfo;
	ImageView* m_imageView[M_IMAGE_BROWSER_CNT];
	QStringList m_fileList;
	QString m_fileName;
	int m_currentIndex = 0;
	int m_percentage = 100;
	ImageView::ScaleMode m_scaleMode = ImageView::FitToWindow;
	
	QHBoxLayout* createSlider();
	void sizeChange(ImageView::ScaleMode mode, int percentage);

protected:
	bool eventFilter(QObject* watched, QEvent* event) override;
};

#endif // IMAGEVIEWCONTAINER_H
