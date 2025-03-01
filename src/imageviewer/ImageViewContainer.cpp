#include "ImageViewContainer.h"
#include "../common/FileUtils.h"



ImageViewContainer::ImageViewContainer(QWidget* parent)
	: QWidget(parent)
{
 
    //main
    QVBoxLayout* vBoxContainer = new QVBoxLayout(this);

    QHBoxLayout* hBoxBrowser = new QHBoxLayout();
    hBoxBrowser->setContentsMargins(0, 0, 0, 0);  // 패딩을 0으로 설정
	hBoxBrowser->setSpacing(0);  // 간격을 0으로 설정

    // hBoxBrowser에 두 스크롤 영역 추가
	ui_imageView[0] = new ImageView(this, ImageView::FitToWindow, 100);
    hBoxBrowser->addWidget(ui_imageView[0]);
    ui_imageView[1] = new ImageView(this, ImageView::FitToWindow, 100);
    hBoxBrowser->addWidget(ui_imageView[1]);

    ui_imageView[1]->setVisible(StatusStore::instance().getImageSettings().isSplitView());

    vBoxContainer->addLayout(hBoxBrowser, 1);

    //for slider
    vBoxContainer->addLayout(createSlider());
    setLayout(vBoxContainer);
	this->installEventFilter(this);

    // 슬라이더 값 변경 시 정보를 업데이트하는 람다 슬롯 연결
    connect(ui_qSlider, &QSlider::valueChanged, this, [this](int value) {
        m_currentIndex = value;
        navigateToFile(ImageView::None);
        ui_qSliderInfo->setText(QString("count: %1 / %2").arg(m_currentIndex+1).arg(ui_qSlider->maximum()+1));
        });

}

ImageViewContainer::~ImageViewContainer() {
    //StatusStore::instance().saveImageHistory(StatusStore::instance().getImageHistory());
}

void ImageViewContainer::clear() {
	ui_imageView[0]->clear();
	ui_imageView[1]->clear();
	m_fileList.clear();
	m_currentIndex = 0;
	ui_qSlider->setValue(0);
	ui_qSlider->setMaximum(0);
	ui_qSliderInfo->setText(QString("count: %1 / %2").arg(0).arg(0));
}

QHBoxLayout* ImageViewContainer::createSlider() {
    QHBoxLayout* hBoxSlider = new QHBoxLayout();
    ui_qSlider = new QSlider(Qt::Horizontal, this);
    hBoxSlider->addWidget(ui_qSlider);

    ui_qSliderInfo = new QLabel(this);
    hBoxSlider->addWidget(ui_qSliderInfo);

	return hBoxSlider;
}

void ImageViewContainer::navigateToFolder(ImageView::MoveMode moveMode) {
    QString file;
    
    if (moveMode == ImageView::MoveMode::NextFolder) {
		 file = FileUtils::moveFolder(m_fileName, FileUtils::MoveMode::NextFolder, FileUtils::IMAGE);
	}
    else if (moveMode == ImageView::MoveMode::PrevFolder) {
        file = FileUtils::moveFolder(m_fileName, FileUtils::MoveMode::PrevFolder, FileUtils::IMAGE);
    }

    if (file.isEmpty()) {
        QMessageBox::warning(this, "warning", "can not find next/prev image folder.");
        return;
    }

    loadFileList(file);

}

void ImageViewContainer::loadFileList(QString filePath) {
	m_fileName = filePath;
    QFileInfo fileInfo(m_fileName);
    
    m_fileList.clear();

    m_fileList = FileUtils::getFileList(filePath, FileUtils::IMAGE);
   
    ui_qSlider->setMaximum(m_fileList.size()-1);
    m_currentIndex = m_fileList.indexOf(filePath);
	m_currentIndex = m_currentIndex < 0 ? 0 : m_currentIndex;
    navigateToFile(ImageView::None);

    ui_qSliderInfo->setText(QString("count: %1 / %2").arg(m_currentIndex+1).arg(ui_qSlider->maximum()+1));


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
		if (ui_imageView[i]->isVisible()) {
			ui_imageView[i]->resize(m_scaleMode, m_percentage);
		}
	}
}

bool ImageViewContainer::changeSplitView() {
	bool newSplit = !ui_imageView[1]->isVisible();
    StatusStore::instance().getImageSettings().setSplitView(newSplit);
    
	ui_imageView[1]->setVisible(newSplit);
	navigateToFile(ImageView::None);
	sizeChange(m_scaleMode, m_percentage);
	return newSplit;

}

void ImageViewContainer::navigateToFile(ImageView::MoveMode moveMode) {
	bool isSplit = StatusStore::instance().getImageSettings().isSplitView();
    // split 모드이면 2씩 이동, 아니면 1씩 이동
    int step = isSplit ? 2 : 1;

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
    if (currentIndex < 0 && StatusStore::instance().getImageSettings().isAutoNext()) {
		navigateToFolder(ImageView::MoveMode::PrevFolder);
        return;
	}
	else if (currentIndex >= m_fileList.size() && StatusStore::instance().getImageSettings().isAutoNext()) {
		navigateToFolder(ImageView::MoveMode::NextFolder);
		return;
	}else if (currentIndex < 0 || currentIndex >= m_fileList.size()) {
        return;
    }

    QString fileName0;
    QString fileName1;
    // 분할 모드인 경우
    if (isSplit) {
        // 첫 번째 이미지는 짝수 인덱스가 되도록 보정
        if ((currentIndex % 2) != 0 && currentIndex > 0) {
            currentIndex--;
        }
        // 첫번째 이미지 로드
        fileName0 = m_fileList.at(currentIndex);
        ui_imageView[0]->loadImage(fileName0);

        // 두번째 이미지 로드: 다음 이미지(홀수 인덱스)가 존재하면 로드, 없으면 빈 문자열 전달
        if (currentIndex + 1 < m_fileList.size()) {
            fileName1 = m_fileList.at(currentIndex + 1);
            ui_imageView[1]->loadImage(fileName1);
        }
        else {
			QString empty;
            ui_imageView[1]->loadImage(empty);
        }
    }
    // 단일 뷰 모드인 경우
    else {
        fileName0 = m_fileList.at(currentIndex);
        ui_imageView[0]->loadImage(fileName0);
    }

    m_currentIndex = currentIndex;
    ui_qSlider->setValue(m_currentIndex);
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
		else if (keyEvent->key() == Qt::Key_5) {
			sizeChange(ImageView::ScaleMode::FitIfLARGE, 100);
		}
        else if ((keyEvent->key() == Qt::Key_Plus) && m_percentage < 1000) {
            sizeChange(ImageView::ScaleMode::ScaleByPercentage, m_percentage + 10);
        }
        else if ((keyEvent->key() == Qt::Key_Minus) && m_percentage > 10) {
            sizeChange(ImageView::ScaleMode::ScaleByPercentage, m_percentage - 10);
        }
        else if (keyEvent->key() == Qt::Key_Delete && !m_fileList.isEmpty() && m_currentIndex < m_fileList.size() && m_currentIndex >= 0) {
            deleteImageFile();
        }
        else if (keyEvent->key() == Qt::Key_F2 && !(keyEvent->modifiers() & Qt::ControlModifier)) {
            m_fileList = renameFile(m_fileList , m_currentIndex , 0);
        }
        else if (keyEvent->key() == Qt::Key_F3 && StatusStore::instance().getImageSettings().isSplitView()) {
            m_fileList = renameFile(m_fileList, m_currentIndex+1, 1);
		}
		else if (keyEvent->key() == Qt::Key_F2 && keyEvent->modifiers() & Qt::ControlModifier) {
            QString newPath = renameFolder(m_fileList, m_currentIndex);
			if (newPath.isEmpty() == false)
			{
				loadFileList(newPath);
			}
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

QStringList ImageViewContainer::renameFile(QStringList fileList, int fileIdx, int containerIdx) {
    if (fileList.isEmpty() || fileIdx >= fileList.size()) {
        return fileList;
    }

    QString currentFile = fileList.at(fileIdx);
    QFileInfo fileInfo(currentFile);
    QString oldFileName = fileInfo.completeBaseName();
    QString suffix = fileInfo.suffix();

    bool ok;
    QString newFileName = QInputDialog::getText(this, "rename file", "please new file name:", QLineEdit::Normal, oldFileName, &ok);

    if (ok && !newFileName.isEmpty()) {
		ui_imageView[containerIdx]->movieStop();
        QDir dir = fileInfo.dir();
        QString newPath = dir.filePath(newFileName + "." + suffix);

        if (QFile::rename(currentFile, newPath)) {
            // 파일명 변경 성공
            ui_imageView[containerIdx]->loadImage(newPath);
            fileList[fileIdx] = newPath;
            return fileList;
        }
        else {
            QMessageBox::warning(this, "warning", "can not rename.");
        }
    }
    return fileList;
}

QString ImageViewContainer::renameFolder(QStringList fileList, int fileIdx) {
    if (fileList.isEmpty() || fileIdx >= fileList.size()) {
        return QString();
    }

    QString currentFile = fileList.at(fileIdx);
    QFileInfo fileInfo(currentFile);
    QDir dir = fileInfo.dir();
    QString oldFolderName = dir.dirName();
    dir.cdUp();
    QString parentPath = dir.absolutePath();

    bool ok;
    QString newFolderName = QInputDialog::getText(this, "rename folder", "please new folder name:", QLineEdit::Normal, oldFolderName, &ok);

    if (ok && !newFolderName.isEmpty()) {
        QString o = QDir::cleanPath(parentPath + QDir::separator() + oldFolderName);
        QString n = QDir::cleanPath(parentPath + QDir::separator() + newFolderName);
        if (fileInfo.dir().rename(o, n)) {
            return QDir::cleanPath(parentPath + QDir::separator() + newFolderName + QDir::separator() + fileInfo.fileName());
        }
        else {
            QMessageBox::warning(this, "warning", "can not rename.");
        }
    }
    return QString();
}

void ImageViewContainer::deleteImageFile() {
    QStringList files;
    QString nextFile;
    files.append(m_fileList.at(m_currentIndex));
    int nextIndex = m_currentIndex + 1;
    if (StatusStore::instance().getImageSettings().isSplitView() && nextIndex < m_fileList.size()) {
        files.append(m_fileList.at(nextIndex));
        nextIndex++;
    }
    ui_imageView[0]->movieStop();
    ui_imageView[1]->movieStop();

    nextIndex = nextIndex >= m_fileList.size() ? nextIndex + (m_currentIndex - nextIndex) : nextIndex;
    nextIndex = nextIndex < 0 ? 0 : nextIndex;

    emit deleteKeyPressed(files, m_fileList.at(nextIndex));
}

/*
#include <QMap>
#include <QImage>
#include <QByteArray>
#include <QString>
#include <QIODevice>
#include "quazip.h"
#include "quazipfile.h"

// 압축 파일 경로로부터 이미지들을 메모리 내 QMap으로 로드하는 예제
QMap<QString, QImage> loadImagesFromArchive(const QString &zipFilePath) {
    QMap<QString, QImage> imageMap;

    QuaZip zip(zipFilePath);
    if (!zip.open(QuaZip::mdUnzip)) {
        qDebug() << "압축 파일 열기 실패:" << zipFilePath;
        return imageMap;
    }

    QuaZipFile zipFile(&zip);
    // 압축 파일 내부의 모든 파일 순회
    for (bool more = zip.goToFirstFile(); more; more = zip.goToNextFile()) {
        QString fileName = zip.getCurrentFileName();

        // 확장자로 이미지 파일만 선택 (대소문자 구분 X)
        if (fileName.endsWith(".jpg", Qt::CaseInsensitive) ||
            fileName.endsWith(".jpeg", Qt::CaseInsensitive) ||
            fileName.endsWith(".png", Qt::CaseInsensitive) ||
            fileName.endsWith(".bmp", Qt::CaseInsensitive) ||
            fileName.endsWith(".gif", Qt::CaseInsensitive) ||
            fileName.endsWith(".webp", Qt::CaseInsensitive)) {

            if (!zipFile.open(QIODevice::ReadOnly)) {
                qDebug() << "파일 열기 실패:" << fileName;
                continue;
            }

            QByteArray fileData = zipFile.readAll();
            QImage image;
            if (image.loadFromData(fileData)) {
                imageMap.insert(fileName, image);
            } else {
                qDebug() << "이미지 로드 실패:" << fileName;
            }

            zipFile.close();
        }
    }

    zip.close();
    return imageMap;
}

*/