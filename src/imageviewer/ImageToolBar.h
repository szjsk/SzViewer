#ifndef IMAGETOOLBAR_H
#define IMAGETOOLBAR_H

#include <QToolBar>
#include <QAction>
#include <QComboBox>
#include <QPushButton>
#include <QWidget>
#include "../StatusStore.h"
#include "ImageViewContainer.h"

class ImageToolBar : public QToolBar
{
    Q_OBJECT
public:
    explicit ImageToolBar(QWidget* parent = nullptr, ImageViewContainer* imageViewContainer = nullptr);

private:
	ImageViewContainer* m_imageViewContainer;
	void move(ImageView::MoveMode mode);
	void resizeEvent(ImageView::ScaleMode mode, int plusMinus);

private slots:

};

#endif // IMAGETOOLBAR_H