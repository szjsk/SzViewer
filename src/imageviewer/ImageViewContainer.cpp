#include "ImageViewContainer.h"

ImageViewContainer::ImageViewContainer(QWidget* parent)
	: QWidget(parent)
{
 
    //main
    QVBoxLayout* vBoxContainer = new QVBoxLayout(this);

    QHBoxLayout* hBoxBrowser = new QHBoxLayout();
    hBoxBrowser->setContentsMargins(0, 0, 0, 0);  // 패딩을 0으로 설정
	hBoxBrowser->setSpacing(0);  // 간격을 0으로 설정

    // hBoxBrowser에 두 스크롤 영역 추가
	m_imageView[0] = new ImageView(this, ImageView::FitToWindow, 100);
    hBoxBrowser->addWidget(m_imageView[0]);
    m_imageView[1] = new ImageView(this, ImageView::FitToWindow, 100);
    hBoxBrowser->addWidget(m_imageView[1]);

    //m_textBrowserArray[1]->setVisible(m_settings.isSplitView());

    vBoxContainer->addLayout(hBoxBrowser, 1);

    //for slider
    vBoxContainer->addLayout(createSlider());
    setLayout(vBoxContainer);
	this->installEventFilter(this);

    // 슬라이더 값 변경 시 정보를 업데이트하는 람다 슬롯 연결
    connect(m_qSlider, &QSlider::valueChanged, this, [this](int value) {
 //       m_currentPosition = value - 1;
   //     m_qSliderInfo->setText(QString("page: %1 / %2").arg(value).arg(m_qSlider->maximum()));
        });

}

QHBoxLayout* ImageViewContainer::createSlider() {
    QHBoxLayout* hBoxSlider = new QHBoxLayout();
    m_qSlider = new QSlider(Qt::Horizontal, this);
    hBoxSlider->addWidget(m_qSlider);

    m_qSliderInfo = new QLabel(this);
    hBoxSlider->addWidget(m_qSliderInfo);

	return hBoxSlider;
}


void ImageViewContainer::loadImage(QString& filePath, int idx) {
	m_imageView[idx]->loadImage(filePath);
}

void ImageViewContainer::sizeChange(ImageView::ScaleMode mode , int percentage) {
    m_scaleMode = mode;
    m_percentage = percentage;
	for (int i = 0; i < M_IMAGE_BROWSER_CNT; i++) {
		if (m_imageView[i]->isVisible()) {
			m_imageView[i]->resize(m_scaleMode, m_percentage);
		}
	}
}


bool ImageViewContainer::eventFilter(QObject* watched, QEvent* event) {
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_PageDown) {
			qDebug() << "Key Pressed : Page Down";
        }
        else if (keyEvent->key() == Qt::Key_PageUp) {
			qDebug() << "Key Pressed : Page Up";
		}
		else if (keyEvent->key() == Qt::Key_Left) {
			qDebug() << "Key Pressed : Left";
		}
		else if (keyEvent->key() == Qt::Key_Right) {
			qDebug() << "Key Pressed : Right";
        }
        else if (keyEvent->key() == Qt::Key_F) {
			sizeChange(ImageView::ScaleMode::FitToWindow, 100);
        }
        else if (keyEvent->key() == Qt::Key_1) {
            sizeChange(ImageView::ScaleMode::FitToWidth, 100);
        }
        else if (keyEvent->key() == Qt::Key_2) {
            sizeChange(ImageView::ScaleMode::FitToHeight, 100);
        }
        else if (keyEvent->key() == Qt::Key_3) {
            sizeChange(ImageView::ScaleMode::ORIGINAL, 100);
        }
        else if (keyEvent->key() == Qt::Key_Plus && m_percentage < 1000) {
            sizeChange(ImageView::ScaleMode::ScaleByPercentage, m_percentage + 10);
        }
        else if (keyEvent->key() == Qt::Key_Minus && m_percentage > 10) {
            sizeChange(ImageView::ScaleMode::ScaleByPercentage, m_percentage - 10);
        }
        return false;  // 이벤트를 가로채서 처리 완료
    }


    if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        QPoint mousePos = static_cast<QWidget*>(watched)->mapTo(this, mouseEvent->pos());

        int centerX = this->width() / 2;
        qDebug() << "Mouse Released " << mousePos.x() << " center : " << centerX;

        if (mousePos.x() < centerX) {
            qDebug() << "Mouse Released on the Left Side";
        }
        else {
            qDebug() << "Mouse Released on the Right Side";
        }
    }

    return QWidget::eventFilter(watched, event);
}