#include "SzViewer.h"

SzViewer::SzViewer(QWidget *parent)
    : QMainWindow(parent)
{

    m_ui.setupUi(this);
    m_stackedWidget = new QStackedWidget(this);

    m_textViewContainer = new TextViewContainer(this, StatusStore::instance().getTextSettings());
    m_stackedWidget->addWidget(m_textViewContainer);

    m_imageViewContainer = new ImageViewContainer(this);
    m_stackedWidget->addWidget(m_imageViewContainer);

    this->setCentralWidget(m_stackedWidget);

    // 드래그 앤 드롭 활성화
    setAcceptDrops(true);


    QAction* fileOpenAction = new QAction(QIcon(":/path/to/icon.png"), "Open", this);
    connect(fileOpenAction, &QAction::triggered, this, &SzViewer::openFileDialog);
    m_ui.menuBar->addAction(fileOpenAction);

    QAction* settingAction = new QAction(QIcon(":/path/to/icon.png"), "Setting", this);
    connect(settingAction, &QAction::triggered, this, &SzViewer::openFontDialog);
    m_ui.menuBar->addAction(settingAction);

    QAction* searchAction = new QAction(QIcon(":/path/to/icon.png"), "Search", this);
    connect(searchAction, &QAction::triggered, this, &SzViewer::openSearchDialog);
    m_ui.menuBar->addAction(searchAction);

    QAction* splitAction = new QAction(QIcon(":/path/to/icon.png"), "Split", this);

    // 클릭 시 동작 정의
    connect(splitAction, &QAction::triggered, this, [this]() {
        TextSettingProps setting = StatusStore::instance().getTextSettings();
        setting.setSplitView(m_textViewContainer->changeSplitView());
		StatusStore::instance().saveSetting(&setting);
     });

    // 메뉴바에 아이콘 버튼 추가

    m_ui.menuBar->addAction(splitAction);


    QAction* helpAction = new QAction(QIcon(":/path/to/icon.png"), "Help", this);
    connect(helpAction, &QAction::triggered, this, [this]() {
        AboutDialog dialog(this);
		dialog.exec();
    });
    m_ui.menuBar->addAction(helpAction);
    

    QAction* imageAction = new QAction(QIcon(":/path/to/icon.png"), "image", this);
    connect(imageAction, &QAction::triggered, this, [this]() {
        if (m_imageViewContainer->isVisible()) {
            m_stackedWidget->setCurrentWidget(m_textViewContainer);
            m_imageViewContainer->setVisible(false);
            m_textViewContainer->setVisible(true);
        }
        else {
            m_stackedWidget->setCurrentWidget(m_imageViewContainer);
            m_textViewContainer->setVisible(false);
            m_imageViewContainer->setVisible(true);
        }

        });
    m_ui.menuBar->addAction(imageAction);

   // this->installEventFilter(this);

}

SzViewer::~SzViewer()
{
}

void SzViewer::openSearchDialog()
{
    TextSearchDialog dialog(this, m_textViewContainer->getTextChunks());
    connect(&dialog, &TextSearchDialog::rowSelected, this, &SzViewer::goToTextPage);
    dialog.exec();
}

void SzViewer::goToTextPage(const QString& searchText, long page, long line)
{
	m_textViewContainer->findPage(searchText, page, line);
}

void SzViewer::openFileDialog()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open File",
        QDir::currentPath(), "Text Files (*.txt);;Image Files (*.jpg)");

    openFile(fileName);
}

void SzViewer::openFontDialog()
{
    TextSettingDialog dialog(StatusStore::instance().getTextSettings(), this);
    connect(&dialog, &TextSettingDialog::settingsChanged, this, &SzViewer::updateSettingPreview);

    if (dialog.exec() == QDialog::Accepted) {
        TextSettingProps settings = dialog.getTextSettings();
		StatusStore::instance().saveSetting(&settings);
        m_textViewContainer->refreshPage();
    }
    else {
        m_textViewContainer->setSettings(StatusStore::instance().getTextSettings());
        m_textViewContainer->refreshPage();
    }

}


void SzViewer::resizeEvent(QResizeEvent* event) {
    m_textViewContainer->refreshPage();
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

    if (suffix == "jpg" || suffix == "jpeg" || suffix == "png" || suffix == "bmp" || suffix == "gif") {
        qDebug() << "이미지 파일 :: " << fileName;
        // 예: 이미지 파일을 처리하는 로직 추가
        // 예를 들면 이미지 뷰어 위젯에 이미지를 표시하는 방식 등이 있을 수 있음.
        m_stackedWidget->setCurrentWidget(m_imageViewContainer);
        m_textViewContainer->setVisible(false);
        m_imageViewContainer->setVisible(true);
        m_imageViewContainer->loadImage(fileName, 0);
    }
    else {
        m_stackedWidget->setCurrentWidget(m_imageViewContainer);
        m_textViewContainer->setVisible(false);
        m_imageViewContainer->setVisible(true);
        m_textViewContainer->loadText(fileName);
    }
}

void SzViewer::updateSettingPreview(const TextSettingProps& settings)
{
    // 즉시 반영
	m_textViewContainer->setSettings(settings);
}