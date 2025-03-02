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
#include <QApplication>
#include <QMainWindow>
#include "ImageView.h"
#include "ImageSettingProps.h"
#include "../common/StatusStore.h"

class ImageViewContainer : public QWidget
{
    Q_OBJECT

public:

	struct ImageInfo {
		QStringList fileList;
		int currentIndex;
	};

	struct ImageScale {
		int percentage;
		int degree;
		ImageView::ScaleMode scaleMode;
	};

    ImageViewContainer(QWidget* parent = nullptr);
	~ImageViewContainer();
	void loadFileList(QString filePath);
	bool changeSplitView();
	void navigateToFile(ImageInfo* imageInfo, ImageView::MoveMode moveMode);
	void navigateToFolder(const ImageInfo* imageInfo, ImageView::MoveMode moveMode);
	void resizeImage(ImageView::ScaleMode scaleMode, std::optional<bool> isPlus = std::nullopt);
	void deleteImageFile(const ImageInfo* imageInfo);
	void clear();
	void toggleFullScreen(bool isNormal = false);
	void rotate(int degree, bool direction);
	ImageInfo* getImageInfo();

protected:
	bool eventFilter(QObject* watched, QEvent* event) override;

signals:
	void deleteKeyPressed(QStringList files , QString nextFile);

private:
	QHBoxLayout* createSlider();
	QStringList renameFile(QStringList fileList, int fileIdx, int containerIdx);
	QString renameFolder(QStringList fileList, int fileIdx);

private: //variable
	static constexpr int M_IMAGE_BROWSER_CNT = 2;
	QSlider* ui_qSlider;
	QLabel* ui_qSliderInfo;
	ImageView* ui_imageView[M_IMAGE_BROWSER_CNT];
	ImageInfo m_imageInfo;
	ImageScale m_imageScale;
};

#endif // IMAGEVIEWCONTAINER_H
