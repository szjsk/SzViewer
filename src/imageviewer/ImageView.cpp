#include "ImageView.h"

ImageView::ImageView(QWidget* parent, ScaleMode scaleMode , int percentage)
    : QScrollArea(parent), m_label(new QLabel(this)), m_scaleMode(scaleMode), m_percentage(percentage)
{

    setWidgetResizable(true);
    setContentsMargins(0, 0, 0, 0);  // 스크롤 영역의 마진을 0으로 설정
    setFrameStyle(QFrame::NoFrame);  // 스크롤 영역의 경계를 제거

    m_label = new QLabel(this);
    m_label->setAlignment(Qt::AlignCenter);
    m_label->setContentsMargins(0, 0, 0, 0);  // QLabel의 마진을 0으로 설정
    m_label->setFrameStyle(QFrame::NoFrame);

	this->setWidget(m_label);
}

void ImageView::loadImage(QString& filePath)
{
	m_filePath = filePath;
    QFileInfo fileInfo(m_filePath);
    QString suffix = fileInfo.suffix().toLower();

    // 이전에 movie가 사용 중이면 정지시킵니다.
    if (m_label->movie()) {
        m_label->movie()->stop();
        m_label->setMovie(nullptr);
    }

    if (suffix == "gif") {
        QMovie* qMovie = createGif(m_filePath, m_label);
        if (qMovie == nullptr) {
            return;
        }
        m_isGif = true;
		m_label->adjustSize(); 
    }
    else {
        QPixmap* qPixMap = createImg(m_filePath, m_label);
        if (qPixMap == nullptr) {
            return;
        }
        m_isGif = false;
        m_label->adjustSize();
    }
}

void ImageView::resize(ScaleMode mode, int percentage) {
	m_scaleMode = mode;
	m_percentage = percentage;
	loadImage(m_filePath);
}

QMovie* ImageView::createGif(QString& filePath, QLabel* label) {
    QMovie* movie = new QMovie(filePath, QByteArray(), label);
    if (!movie->isValid())
    {
        qDebug() << "GIF 로드 실패:" << filePath;
        delete movie;
        return nullptr;
    }
    movie->jumpToFrame(0);
    movie = getScaledQMovie(movie, movie->currentPixmap().size(), m_scaleMode, m_percentage);
    label->setPixmap(QPixmap()); // 기존 이미지는 초기화
    label->setMovie(movie);
    movie->start();
    return movie;
}

QPixmap* ImageView::createImg(QString& filePath, QLabel* label) {
        QPixmap pixmap(filePath);
        if (pixmap.isNull())
        {
            qDebug() << "이미지 로드 실패:" << filePath;
            return nullptr;
        }
        pixmap = getScaledPixmap(&pixmap, pixmap.size(), m_scaleMode, m_percentage);
        label->setPixmap(pixmap);
		return &pixmap;
}

QMovie* ImageView::getScaledQMovie(QMovie* movie, QSize originSize, ScaleMode mode, int percentage) {
    
    switch (mode) {
        case FitToWindow: {
            movie->setScaledSize(this->size());
            break;
        }case FitToWidth: {
            QSize newSize(this->size().width(), originSize.height());
            movie->setScaledSize(newSize);
            break;
        } case FitToHeight: {
            QSize newSize(originSize.width(), this->size().height());
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
    switch (mode) {
    case FitToWindow:
        return pixmap->scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    case FitToWidth:
        return pixmap->scaled(this->size().width(), originSize.height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    case FitToHeight:
        return pixmap->scaled(originSize.width(), this->size().height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    case ScaleByPercentage:
        return pixmap->scaled(pixmap->size().width() * percentage / 100, pixmap->size().height() * percentage / 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    case ORIGINAL:
        return pixmap->scaled(originSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    default:
        return *pixmap;
    }
}