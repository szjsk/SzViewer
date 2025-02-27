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
#include "../common/StatusStore.h"

class ImageViewContainer : public QWidget
{
    Q_OBJECT

public:
    ImageViewContainer(QWidget* parent = nullptr);
	~ImageViewContainer();
	void loadFileList(QString& filePath);
	bool changeSplitView();
	void navigateToFile(ImageView::MoveMode moveMode);
	QString navigateToFolder(QString fileName, ImageView::MoveMode moveMode);
	void navigateToFolder(ImageView::MoveMode moveMode);
	void resizeImage(ImageView::ScaleMode scaleMode, int plusMinus);
	void deleteImageFile();
	void clear();

private:
	QHBoxLayout* createSlider();
	void sizeChange(ImageView::ScaleMode mode, int percentage);

protected:
	bool eventFilter(QObject* watched, QEvent* event) override;

signals:
	void deleteKeyPressed(QStringList files , QString nextFile);
};

#endif // IMAGEVIEWCONTAINER_H
