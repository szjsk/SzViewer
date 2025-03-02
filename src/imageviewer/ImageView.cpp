#include "ImageView.h"

#include "../common/StatusStore.h"

ImageView::ImageView(QWidget* parent, ScaleMode scaleMode, int percentage)
	: QScrollArea(parent), ui_label(new QLabel(this)), m_scaleMode(scaleMode), m_percentage(percentage)
{

	setWidgetResizable(true);
	setContentsMargins(0, 0, 0, 0);  // 스크롤 영역의 마진을 0으로 설정
	setFrameStyle(QFrame::NoFrame);  // 스크롤 영역의 경계를 제거

	ui_label = new QLabel(this);
	ui_label->setAlignment(Qt::AlignCenter);
	ui_label->setContentsMargins(0, 0, 0, 0);  // QLabel의 마진을 0으로 설정
	ui_label->setFrameStyle(QFrame::NoFrame);

	this->setWidget(ui_label);
}

void ImageView::clear() {
	movieStop();
	delete m_pixmap;
	m_pixmap = nullptr;
	delete m_originMovie;
	m_originMovie = nullptr;
	m_originSize = QSize();
	ui_label->clear();
	ui_label->setText("");
}

void ImageView::loadImage(QString& filePath)
{

	StatusStore::instance().getImageHistory().addFileInfo(filePath, -1, "");

	QFileInfo fileInfo(filePath);
	QString suffix = fileInfo.suffix().toLower();

	clear();

	if (suffix == "gif") {
		m_isGif = true;
		m_originMovie = new QMovie(filePath, QByteArray(), ui_label);
		m_originMovie->jumpToFrame(0);
		m_originSize = m_originMovie->currentPixmap().size();
	}
	else {
		m_isGif = false;
		m_pixmap = new QPixmap(filePath);
		m_originSize = m_pixmap->size();
	}
	resize(m_scaleMode, m_percentage);
}

void ImageView::resize(ScaleMode mode, int percentage) {
	m_scaleMode = mode;
	m_percentage = percentage;
	if (m_originMovie == nullptr && m_pixmap == nullptr) {
		return;
	}

	if (m_isGif) {
		setGif(m_originMovie, ui_label);
	}
	else {
		setImg(m_pixmap, ui_label);
	}
	ui_label->adjustSize();
}


void ImageView::setGif(QMovie* movie, QLabel* label) {
	if (!movie->isValid())
	{
		ui_label->movie()->stop();
		ui_label->setMovie(nullptr);
		label->clear();
		label->setText("can not load gif");
		return;
	}
	m_originMovie->jumpToFrame(0);
	movie = getScaledQMovie(movie, m_originSize, m_scaleMode, m_percentage);

	label->setPixmap(QPixmap()); // 기존 이미지는 초기화
	label->setMovie(movie);
	movie->start();
}

void ImageView::setImg(QPixmap* pixmap, QLabel* label) {
	if (pixmap->isNull())
	{
		label->clear();
		label->setText("can not load Image");
		return;
	}

	QPixmap newPixMap = getScaledPixmap(pixmap, m_originSize, m_scaleMode, m_percentage);
	label->setPixmap(newPixMap);
}

QMovie* ImageView::getScaledQMovie(QMovie* movie, QSize originSize, ScaleMode mode, int percentage) {
	int scrollSize = 20;

	switch (mode) {
	case FitToWindow: {
		QSize containerSize = this->size();
		QSize scaledSize = originSize.scaled(containerSize, Qt::KeepAspectRatio);
		movie->setScaledSize(scaledSize);
		break;
	}case FitToWidth: {
		QSize newSize(this->size().width() - scrollSize, originSize.height());
		movie->setScaledSize(newSize);
		break;
	} case FitToHeight: {
		QSize newSize(originSize.width(), this->size().height() - scrollSize);
		movie->setScaledSize(newSize);
		break;
	}case ScaleByPercentage: {
		QSize newSize(originSize.width() * percentage / 100, originSize.height() * percentage / 100);
		movie->setScaledSize(newSize);
		break;
	}case ORIGINAL: {
		movie->setScaledSize(originSize);
		break;
	}case FitIfLARGE: {
		if (originSize.width() > this->size().width() || originSize.height() > this->size().height()) {
			QSize containerSize = this->size();
			QSize scaledSize = originSize.scaled(containerSize, Qt::KeepAspectRatio);
			movie->setScaledSize(scaledSize);
		}
		else {
			movie->setScaledSize(originSize);
		}
		break;
	}
	}

	return movie;
}

QPixmap ImageView::getScaledPixmap(QPixmap* pixmap, QSize originSize, ScaleMode mode, int percentage) {
	int scrollSize = 20;
	switch (mode) {
	case FitToWindow:
		return pixmap->scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
	case FitToWidth: {
		return pixmap->scaled(this->size().width() - scrollSize, originSize.height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	}case FitToHeight:
		return pixmap->scaled(originSize.width(), this->size().height() - scrollSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	case ScaleByPercentage:
		return pixmap->scaled(pixmap->size().width() * percentage / 100, pixmap->size().height() * percentage / 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	case ORIGINAL:
		return pixmap->scaled(originSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	case FitIfLARGE: {
		if (originSize.width() > this->size().width() || originSize.height() > this->size().height()) {
			return pixmap->scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
		}
		return pixmap->scaled(originSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	}default:
		return *pixmap;
	}
}

void ImageView::rotate(int degree, bool direction) {
	if (m_isGif) {
		if (!m_originMovie || !m_originMovie->isValid()) return;

		// GIF의 경우 현재 프레임을 가져와서 회전
		QPixmap currentFrame = m_originMovie->currentPixmap();
		QTransform transform;
		transform.rotate(direction ? degree : -degree);
		currentFrame = currentFrame.transformed(transform, Qt::SmoothTransformation);

		//ui_label->setMovie(nullptr);  // 현재 동영상 중지
		//ui_label->setPixmap(currentFrame);  // 회전된 현재 프레임 표시

	}
	else {
		if (!m_pixmap) return;
		
		// 일반 이미지의 경우 QPixmap 회전
		QTransform transform;
		transform.rotate(direction ? degree : -degree);
		*m_pixmap = m_pixmap->transformed(transform, Qt::SmoothTransformation);

		// 회전된 이미지 표시
		setImg(m_pixmap, ui_label);
	}

	// 회전 후 크기 조정
	//m_originSize = m_isGif ? ui_label->pixmap().size() : m_pixmap->size();
	//resize(m_scaleMode, m_percentage);
}

void ImageView::movieStop() {
	if (m_isGif && m_originMovie) {
		m_originMovie->stop();
		ui_label->setMovie(nullptr);
		delete m_originMovie;
		m_originMovie = nullptr;
	}
}