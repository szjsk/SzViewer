#include "ImageView.h"

#include "../common/StatusStore.h"

ImageView::ImageView(QWidget* parent, ScaleMode scaleMode , int percentage)
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
	m_originSize = QSize();
	ui_label->clear();
}

void ImageView::loadImage(QString& filePath)
{

    StatusStore::instance().getImageHistory().addFileInfo(filePath, -1, "");

    QFileInfo fileInfo(filePath);
    QString suffix = fileInfo.suffix().toLower();

	movieStop();

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
            QSize newSize(this->size().width()- scrollSize, originSize.height());
            movie->setScaledSize(newSize);
            break;
        } case FitToHeight: {
            QSize newSize(originSize.width(), this->size().height()- scrollSize);
            movie->setScaledSize(newSize);
            break;
        }case ScaleByPercentage: {
            QSize newSize(movie->currentPixmap().width() * percentage / 100, movie->currentPixmap().height() * percentage / 100);
            movie->setScaledSize(newSize);
            break;
        }case ORIGINAL: {
            movie->setScaledSize(originSize);
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
    default:
        return *pixmap;
    }
}

void ImageView::movieStop() {
    if (m_isGif && m_originMovie) {
        m_originMovie->stop();
        ui_label->setMovie(nullptr);
        delete m_originMovie;
        m_originMovie = nullptr;
    }
}