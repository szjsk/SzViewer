#pragma once
#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QScrollArea>
#include <QLabel>
#include <QPixmap>
#include <QMovie>
#include <QFileInfo>

class ImageView : public QScrollArea
{
    Q_OBJECT

public:
	enum ScaleMode {
		ORIGINAL,
		FitToWindow,
		FitToWidth,
		FitToHeight,
		ScaleByPercentage
	};

    explicit ImageView(QWidget* parent = nullptr, ScaleMode scaleMode = FitToWindow, int percentage = 100);
	void loadImage(QString& filePath);
	void resize(ScaleMode mode, int percentage);

private:
	QScrollArea* m_scrollArea;
	QLabel* m_label;
	QSize m_originImageSize;
	ScaleMode m_scaleMode;
	QString m_filePath;
	int m_percentage;
	bool m_isGif = false;

	QMovie* createGif(QString& filePath, QLabel* label);
	QPixmap* createImg(QString& filePath, QLabel* label);
	QPixmap getScaledPixmap(QPixmap* pixmap, QSize originSize, ScaleMode mode, int percentage);
	QMovie* getScaledQMovie(QMovie* movie, QSize originSize, ScaleMode mode, int percentage);
};

#endif // IMAGEVIEW_H
