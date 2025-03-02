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


    explicit ImageView(QWidget* parent = nullptr, ScaleMode scaleMode = FitIfLARGE, int percentage = 100);
	void loadImage(QString& filePath);
	void resize(ScaleMode mode, int percentage);
	void movieStop();
	void clear();
	void rotate(int degree, bool direction);

private:
	void setGif(QMovie* movie, QLabel* label);
	void setImg(QPixmap* pixmap, QLabel* label);
	QPixmap getScaledPixmap(QPixmap* pixmap, QSize originSize, ScaleMode mode, int percentage);
	QMovie* getScaledQMovie(QMovie* movie, QSize originSize, ScaleMode mode, int percentage);
	
private: //variables
	QScrollArea* ui_scrollArea;
	QLabel* ui_label;
	ScaleMode m_scaleMode;
	QMovie* m_originMovie;
	QSize m_originSize;
	QPixmap* m_pixmap;
	int m_percentage;
	bool m_isGif = false;
};

#endif // IMAGEVIEW_H
