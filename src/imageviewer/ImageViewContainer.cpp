#include "ImageViewContainer.h"

ImageViewContainer::ImageViewContainer(QWidget* parent, const ImageSettingProps& settings)
	: QWidget(parent), m_settings(settings)
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

    m_imageView[1]->setVisible(m_settings.isSplitView());

    vBoxContainer->addLayout(hBoxBrowser, 1);

    //for slider
    vBoxContainer->addLayout(createSlider());
    setLayout(vBoxContainer);
	this->installEventFilter(this);

    // 슬라이더 값 변경 시 정보를 업데이트하는 람다 슬롯 연결
    connect(m_qSlider, &QSlider::valueChanged, this, [this](int value) {
        m_currentIndex = value;
        navigateToFile(ImageView::None);
        m_qSliderInfo->setText(QString("count: %1 / %2").arg(m_currentIndex+1).arg(m_qSlider->maximum()+1));
        });

}

void ImageViewContainer::clear() {
	m_imageView[0]->clear();
	m_imageView[1]->clear();
	m_fileList.clear();
	m_currentIndex = 0;
	m_qSlider->setValue(0);
	m_qSlider->setMaximum(0);
	m_qSliderInfo->setText(QString("count: %1 / %2").arg(m_currentIndex + 1).arg(m_qSlider->maximum() + 1));
}

QHBoxLayout* ImageViewContainer::createSlider() {
    QHBoxLayout* hBoxSlider = new QHBoxLayout();
    m_qSlider = new QSlider(Qt::Horizontal, this);
    hBoxSlider->addWidget(m_qSlider);

    m_qSliderInfo = new QLabel(this);
    hBoxSlider->addWidget(m_qSliderInfo);

	return hBoxSlider;
}

void ImageViewContainer::navigateToFolder(ImageView::MoveMode moveMode) {
	QString file = navigateToFolder(m_fileName, moveMode);
    if (!file.isEmpty()) {
        loadFileList(file);
    }
}

QString ImageViewContainer::navigateToFolder(QString fileName, ImageView::MoveMode moveMode) {

    // 현재 파일이 속한 폴더
    QFileInfo currentFileInfo(fileName);
    QDir currentFolder = currentFileInfo.dir();

    // 부모 폴더로 이동
    QDir parentDir = currentFolder;
    if (!parentDir.cdUp()) {
        qDebug() << "부모 폴더로 이동 실패";
        return QString();
    }

    // 부모 폴더 내의 모든 서브 폴더 리스트 (알파벳 순)
    QStringList folderNames = parentDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
    if (folderNames.isEmpty()) {
        qDebug() << "부모 폴더에 하위 폴더가 없습니다.";
        return QString();
    }

    QCollator collator;
    collator.setNumericMode(true);
    std::sort(folderNames.begin(), folderNames.end(),
        [&collator](const QString& s1, const QString& s2) {
            return collator.compare(s1, s2) < 0;
        });

    // 현재 폴더 이름 및 인덱스 결정
    QString currentFolderName = currentFolder.dirName();
    int index = folderNames.indexOf(currentFolderName);
    if (index < 0) {
        qDebug() << "현재 폴더를 부모 폴더 내에서 찾을 수 없습니다.";
        return QString();
    }

    // next 또는 prev 폴더 인덱스 계산
    int nextIndex = -1;
    if (moveMode == ImageView::MoveMode::NextFolder) {
        nextIndex = index + 1;
    }
    else if (moveMode == ImageView::MoveMode::PrevFolder) {
        nextIndex = index - 1;
    }

    // 범위를 벗어나면 아무것도 하지 않음
    if (nextIndex < 0 || nextIndex >= folderNames.size()) {
        qDebug() << "폴더 이동 범위를 벗어났습니다.";
        return QString();
    }

    // 다음(또는 이전) 폴더 경로 결정
    QString nextFolderName = folderNames.at(nextIndex);
    QDir nextFolder(parentDir.absoluteFilePath(nextFolderName));

    QStringList fileNames = nextFolder.entryList(QDir::Files, QDir::Name);

    std::sort(fileNames.begin(), fileNames.end(),
        [&collator](const QString& s1, const QString& s2) {
            return collator.compare(s1, s2) < 0;
        });

	if (!fileNames.isEmpty()) {
        QString file = nextFolder.absoluteFilePath(fileNames[0]);
        //loadFileList(file);
		return file;
	}
    return QString();
}

void ImageViewContainer::loadFileList(QString& filePath) {
	m_fileName = filePath;
    QFileInfo fileInfo(m_fileName);
    QDir dir = fileInfo.dir();

    QStringList fileNames = dir.entryList(QDir::Files, QDir::Name);
    
    m_fileList.clear();
    for (const QString& name : fileNames) {
        QFileInfo eachFileInfo(dir.absoluteFilePath(name));
        QString suffix = eachFileInfo.suffix().toLower();
        if (suffix == "jpg" || suffix == "jpeg" || suffix == "png" || suffix == "bmp" || suffix == "gif" || suffix == "webp") {
            m_fileList.append(dir.absoluteFilePath(name));
        }
    }

    QCollator collator;
    collator.setNumericMode(true);
    std::sort(m_fileList.begin(), m_fileList.end(),
        [&collator](const QString& s1, const QString& s2) {
            return collator.compare(s1, s2) < 0;
        });

    m_currentIndex = m_fileList.indexOf(filePath);
	m_currentIndex = m_currentIndex < 0 ? 0 : m_currentIndex;
    navigateToFile(ImageView::None);

    m_qSlider->setValue(m_currentIndex);
    m_qSlider->setMaximum(m_fileList.size()-1);
    m_qSliderInfo->setText(QString("count: %1 / %2").arg(m_currentIndex+1).arg(m_qSlider->maximum()+1));


    this->window()->activateWindow();
    this->window()->raise();
}

void ImageViewContainer::resizeImage(ImageView::ScaleMode scaleMode, int plusMinus) {

    if (scaleMode == ImageView::ScaleByPercentage) {
        sizeChange(ImageView::ScaleMode::ScaleByPercentage, m_percentage + (10 * plusMinus));
    }
    else {
        sizeChange(scaleMode, 100);
    }
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

bool ImageViewContainer::changeSplitView() {
	bool newSplit = !m_imageView[1]->isVisible();
    m_settings.setSplitView(newSplit);
	m_imageView[1]->setVisible(newSplit);
	navigateToFile(ImageView::None);
	sizeChange(m_scaleMode, m_percentage);
	return newSplit;

}

void ImageViewContainer::navigateToFile(ImageView::MoveMode moveMode) {
    // split 모드이면 2씩 이동, 아니면 1씩 이동
    int step = m_settings.isSplitView() ? 2 : 1;

    int currentIndex = m_currentIndex;
    // 이동 모드에 따른 인덱스 변경
    if (moveMode == ImageView::MoveMode::Next) {
        currentIndex += step;
    }
    else if (moveMode == ImageView::MoveMode::Prev) {
        currentIndex -= step;
	}
	else if (moveMode == ImageView::MoveMode::First) {
        currentIndex = 0;
	}
	else if (moveMode == ImageView::MoveMode::Last) {
        currentIndex = m_fileList.size() - 1;
	}
    // moveMode가 None인 경우 m_currentIndex는 그대로 사용

    // 범위 보정
    if (currentIndex < 0 || currentIndex >= m_fileList.size()) {
        return;
    }
    QString fileName0;
    QString fileName1;
    // 분할 모드인 경우
    if (m_settings.isSplitView()) {
        // 첫 번째 이미지는 짝수 인덱스가 되도록 보정
        if ((currentIndex % 2) != 0 && currentIndex > 0) {
            currentIndex--;
        }
        // 첫번째 이미지 로드
        fileName0 = m_fileList.at(currentIndex);
        m_imageView[0]->loadImage(fileName0);

        // 두번째 이미지 로드: 다음 이미지(홀수 인덱스)가 존재하면 로드, 없으면 빈 문자열 전달
        if (currentIndex + 1 < m_fileList.size()) {
            fileName1 = m_fileList.at(currentIndex + 1);
            m_imageView[1]->loadImage(fileName1);
        }
        else {
			QString empty;
            m_imageView[1]->loadImage(empty);
        }
    }
    // 단일 뷰 모드인 경우
    else {
        fileName0 = m_fileList.at(currentIndex);
        m_imageView[0]->loadImage(fileName0);
    }

    m_currentIndex = currentIndex;
    m_qSlider->setValue(m_currentIndex);
    this->window()->setWindowTitle(QString("SzViewer - %1       /        %2").arg(fileName0).arg(fileName1));


}

bool ImageViewContainer::eventFilter(QObject* watched, QEvent* event) {

    if (this->isVisible() == false) {
        return QWidget::eventFilter(watched, event);
    }

    if (event->type() == QEvent::KeyRelease) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_PageDown) {
			navigateToFolder(ImageView::MoveMode::NextFolder);
        }
        else if (keyEvent->key() == Qt::Key_PageUp) {
            navigateToFolder(ImageView::MoveMode::PrevFolder);
        }
		else if (keyEvent->key() == Qt::Key_Left) {
			navigateToFile(ImageView::MoveMode::Prev);
		}
		else if (keyEvent->key() == Qt::Key_Right) {
			navigateToFile(ImageView::MoveMode::Next);
        }
        else if (keyEvent->key() == Qt::Key_1) {
			sizeChange(ImageView::ScaleMode::FitToWindow, 100);
        }
        else if (keyEvent->key() == Qt::Key_2) {
            sizeChange(ImageView::ScaleMode::ORIGINAL, 100);
        }
        else if (keyEvent->key() == Qt::Key_3) {
            sizeChange(ImageView::ScaleMode::FitToWidth, 100);
        }
        else if (keyEvent->key() == Qt::Key_4) {
            sizeChange(ImageView::ScaleMode::FitToHeight, 100);
        }
        else if ((keyEvent->key() == Qt::Key_Plus || keyEvent->key() == Qt::Key_5) && m_percentage < 1000) {
            sizeChange(ImageView::ScaleMode::ScaleByPercentage, m_percentage + 10);
        }
        else if ((keyEvent->key() == Qt::Key_Minus || keyEvent->key() == Qt::Key_6) && m_percentage > 10) {
            sizeChange(ImageView::ScaleMode::ScaleByPercentage, m_percentage - 10);
        }else if (keyEvent->key() == Qt::Key_Delete && !m_fileList.isEmpty() && m_currentIndex < m_fileList.size() && m_currentIndex >= 0) {
            deleteImageFile();
        }
        return false;  // 이벤트를 가로채서 처리 완료
    }


    if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        QPoint mousePos = static_cast<QWidget*>(watched)->mapTo(this, mouseEvent->pos());

        int centerX = this->width() / 2;

        if (mousePos.x() < centerX) {
            navigateToFile(ImageView::MoveMode::Prev);
        }
        else {
            navigateToFile(ImageView::MoveMode::Next);
        }
    }

    return QWidget::eventFilter(watched, event);
}

void ImageViewContainer::deleteImageFile() {
    QStringList files;
    QString nextFile;
    files.append(m_fileList.at(m_currentIndex));
    int nextIndex = m_currentIndex + 1;
    if (m_settings.isSplitView() && nextIndex < m_fileList.size()) {
        files.append(m_fileList.at(nextIndex));
        nextIndex++;
    }
    m_imageView[0]->movieStop();
    m_imageView[1]->movieStop();

    nextIndex = nextIndex >= m_fileList.size() ? nextIndex + (m_currentIndex - nextIndex) : nextIndex;
    nextIndex = nextIndex < 0 ? 0 : nextIndex;

    emit deleteKeyPressed(files, m_fileList.at(nextIndex));
}