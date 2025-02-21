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
#include "ImageView.h"

class ImageViewContainer : public QWidget
{
    Q_OBJECT

public:
    ImageViewContainer(QWidget* parent = nullptr);
	void loadImage(QString& filePath, int idx);

private:
	static constexpr int M_IMAGE_BROWSER_CNT = 2;

	QSlider* m_qSlider;
	QLabel* m_qSliderInfo;
	ImageView* m_imageView[M_IMAGE_BROWSER_CNT];
	int m_percentage = 100;
	ImageView::ScaleMode m_scaleMode = ImageView::FitToWindow;
	
	QHBoxLayout* createSlider();
	void sizeChange(ImageView::ScaleMode mode, int percentage);

protected:
	bool eventFilter(QObject* watched, QEvent* event) override;
};

#endif // IMAGEVIEWCONTAINER_H
