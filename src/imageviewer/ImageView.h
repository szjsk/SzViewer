#pragma once
#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QScrollArea>
#include <QLabel>
#include <QPixmap>
#include <QMovie>
#include <QFileInfo>
#include <QPainter>

class ImageView : public QScrollArea
{
    Q_OBJECT

public:
	enum ScaleMode {
		ORIGINAL,
		FitToWindow,
		FitToWidth,
		FitToHeight,
		ScaleByPercentage,
		FitIfLARGE

	};

	enum MoveMode {
		None,
		Next,
		Last,
		NextFolder,
		Prev,
		First,
		PrevFolder
	};

	struct ImageInfo {
		bool isGif = false;
		QMovie* originMovie;
		QPixmap* originPixmap;
		QPixmap* changePixmap;
		QSize originSize;
	};


    explicit ImageView(QWidget* parent = nullptr);
	void loadImage(QString& filePath, ScaleMode scaleMode, int percentage);
	void resize(ScaleMode mode, int percentage);
	void movieStop();
	void clear();
	void rotate(int degree, bool isFlip);

private:
	QPixmap getScaledPixmap(QPixmap* pixmap, QSize originSize, ScaleMode mode, int percentage);
	QMovie* getScaledQMovie(QMovie* movie, QSize originSize, ScaleMode mode, int percentage);
	
private: //variables
	QScrollArea* ui_scrollArea;
	QLabel* ui_label;
	ImageInfo m_imageInfo;

};

#endif // IMAGEVIEW_H
