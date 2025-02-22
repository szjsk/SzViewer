#include "SzViewer.h"

SzViewer::SzViewer(QWidget *parent)
    : QMainWindow(parent)
{

    //창 설정.
    QSettings settings("SzViewer", "SzViewer-Common");
    QRect geom = settings.value("geometry", QRect(100, 100, 1024, 768)).toRect();
    setGeometry(geom);

    m_stackedWidget = new QStackedWidget(this);

    m_textViewContainer = new TextViewContainer(this, StatusStore::instance().getTextSettings());
    m_stackedWidget->addWidget(m_textViewContainer);

    m_imageViewContainer = new ImageViewContainer(this, StatusStore::instance().getImageSettings());
    m_stackedWidget->addWidget(m_imageViewContainer);

    this->setCentralWidget(m_stackedWidget);

    // 드래그 앤 드롭 활성화
    setAcceptDrops(true);

	QToolBar* leftToolBar = CommonLeft();
    m_textToolBar = new TextToolBar(this, m_textViewContainer);
    m_imageToolBar = new ImageToolBar(this, m_imageViewContainer);
    QToolBar* rightToolBar = CommonRight();

    // 두 툴바를 한 줄에 배치할 컨테이너 위젯 생성
    QWidget* topToolBarContainer = new QWidget(this);
    QHBoxLayout* topToolBarLayout = new QHBoxLayout(topToolBarContainer);
    topToolBarLayout->setContentsMargins(0, 0, 0, 0);
    topToolBarLayout->setSpacing(0);
    topToolBarLayout->setAlignment(Qt::AlignLeft);

    topToolBarLayout->addWidget(leftToolBar, 0);
    topToolBarLayout->addWidget(m_textToolBar, 0);
    topToolBarLayout->addWidget(m_imageToolBar, 0);
    topToolBarLayout->addWidget(rightToolBar, 0);

    // QMainWindow의 메뉴바 영역에 배치 (메뉴바 영역으로 사용)
    this->setMenuWidget(topToolBarContainer);

	changeVisible(false);

}

SzViewer::~SzViewer()
{
    QSettings settings("SzViewer", "SzViewer-Common");
    settings.setValue("geometry", this->geometry());
}

QToolBar* SzViewer::CommonLeft() {
    QToolBar* toolBar = new QToolBar(this);
    toolBar->setMovable(true);
    QAction* fileOpenAction = new QAction(QIcon(":/path/to/icon.png"), "Open", this);
    connect(fileOpenAction, &QAction::triggered, this, &SzViewer::openFileDialog);
    toolBar->addAction(fileOpenAction);

    return toolBar;
}

QToolBar* SzViewer::CommonRight() {
    QToolBar* toolBar = new QToolBar(this);
    toolBar->setMovable(true);
    QAction* imageAction = new QAction(QIcon(":/path/to/icon.png"), "changeView", this);
    connect(imageAction, &QAction::triggered, this, [this]() { changeVisible(!m_imageViewContainer->isVisible());});
    toolBar->addAction(imageAction);

    QAction* helpAction = new QAction(QIcon(":/path/to/icon.png"), "Help", this);
    connect(helpAction, &QAction::triggered, this, [this]() {
        AboutDialog dialog(this);
        dialog.exec();
        });
    toolBar->addAction(helpAction);

    return toolBar;
}

void SzViewer::changeVisible(bool isCurrentTextView) {
    if (isCurrentTextView) {
        m_stackedWidget->setCurrentWidget(m_imageViewContainer);
        m_imageToolBar->setVisible(true);
        m_imageViewContainer->setVisible(true);
        m_textToolBar->setVisible(false);
        m_textViewContainer->setVisible(false);
    }
    else {
        m_stackedWidget->setCurrentWidget(m_textViewContainer);
        m_imageToolBar->setVisible(false);
        m_imageViewContainer->setVisible(false);
        m_textToolBar->setVisible(true);
        m_textViewContainer->setVisible(true);
    }
}

void SzViewer::openFileDialog()
{
	if (m_textViewContainer->isVisible()) {
		QString fileName = QFileDialog::getOpenFileName(this, "Open File",
			QDir::currentPath(), "Text Files (*.txt)");
		openFile(fileName);
	}
    else {
        QString fileName = QFileDialog::getOpenFileName(this, "Open File",
            QDir::currentPath(), "Image Files (*.jpg *.jpeg *.png *.bmp *.gif *.webp)");

        openFile(fileName);
    }

}

void SzViewer::resizeEvent(QResizeEvent* event) {
    if (m_imageViewContainer->isVisible()) {
        //todo
    }
    else if (m_textViewContainer->isVisible()) {
        m_textViewContainer->refreshPage();
    }
}

// 드래그 엔터 이벤트 처리 : 파일이면 받아들임.
void SzViewer::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

// 드랍 이벤트 처리 : 첫 번째 파일 경로를 사용하여 파일 오픈.
void SzViewer::dropEvent(QDropEvent* event)
{
    QList<QUrl> urls = event->mimeData()->urls();

    if (urls.isEmpty() || urls.length() != 1) {
        return;
    }

    QString fileName = urls.first().toLocalFile();
    QFileInfo fileInfo(fileName);

    if (fileInfo.isDir()) {
        QDir dir(fileName);
        QStringList fileList = dir.entryList(QDir::Files, QDir::Name);
        if (fileList.isEmpty()) {
			return;
        }
        fileName = dir.absoluteFilePath(fileList.first());
        openFile(fileName);
    }
    else {
        openFile(fileName);
    }

}

void SzViewer::openFile(QString& fileName) {
    QFileInfo fileInfo(fileName);
    QString suffix = fileInfo.suffix().toLower();

    if (suffix == "jpg" || suffix == "jpeg" || suffix == "png" || suffix == "bmp" || suffix == "gif" || suffix == "webp") {
        qDebug() << "이미지 파일 :: " << fileName;
        // 예: 이미지 파일을 처리하는 로직 추가
        // 예를 들면 이미지 뷰어 위젯에 이미지를 표시하는 방식 등이 있을 수 있음.
        changeVisible(true);
        m_imageViewContainer->loadFileList(fileName);
    }
    else {
        changeVisible(false);
        m_textViewContainer->loadText(fileName);
    }
}