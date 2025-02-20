#include "SzViewer.h"

SzViewer::SzViewer(QWidget *parent)
    : QMainWindow(parent)
{

    ui.setupUi(this);
    textViewContainer = new TextViewContainer(this, StatusStore::instance().getTextSettings());
    this->setCentralWidget(textViewContainer);
    
    // 드래그 앤 드롭 활성화
    setAcceptDrops(true);


    QAction* fileOpenDialog = new QAction(QIcon(":/path/to/icon.png"), "File", this);
    connect(fileOpenDialog, &QAction::triggered, this, &SzViewer::openFileDialog);
    ui.menuBar->addAction(fileOpenDialog);

    QAction* settingDialog = new QAction(QIcon(":/path/to/icon.png"), "Setting", this);
    connect(settingDialog, &QAction::triggered, this, &SzViewer::openFontDialog);
    ui.menuBar->addAction(settingDialog);

    QAction* split = new QAction(QIcon(":/path/to/icon.png"), "Split", this);

    // 클릭 시 동작 정의
    connect(split, &QAction::triggered, this, [this]() {
		qDebug() << "Split Clicked";
        TextSettingProps setting = StatusStore::instance().getTextSettings();
        setting.setSplitView(textViewContainer->changeSplitView());
		StatusStore::instance().saveSetting(&setting);
     });

    // 메뉴바에 아이콘 버튼 추가

    ui.menuBar->addAction(split);
}

SzViewer::~SzViewer()
{
}

void SzViewer::openFileDialog()
{
    qDebug() << "open File";

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
        textViewContainer->refreshPage();
    }
    else {
        textViewContainer->setSettings(StatusStore::instance().getTextSettings());
        textViewContainer->refreshPage();
    }

}


void SzViewer::resizeEvent(QResizeEvent* event) {
    textViewContainer->refreshPage();
}

// 드래그 엔터 이벤트 처리 : 파일이면 받아들임.
void SzViewer::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

// 드랍 이벤트 처리 : 첫 번째 파일 경로를 사용하여 파일 오픈.
void SzViewer::dropEvent(QDropEvent* event)
{
    QList<QUrl> urls = event->mimeData()->urls();

    qDebug() << "url :: " << urls;
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


    if (suffix == "jpg" || suffix == "jpeg" || suffix == "png" || suffix == "bmp") {
        qDebug() << "이미지 파일 :: " << fileName;
        // 예: 이미지 파일을 처리하는 로직 추가
        // 예를 들면 이미지 뷰어 위젯에 이미지를 표시하는 방식 등이 있을 수 있음.
    }
    else if (isTextFile(fileName)) {
        qDebug() << "fileName :: " << fileName;
        textViewContainer->loadText(fileName);
    }
    else {
        qDebug() << "지원하지 않는 파일 형식 :: " << fileName;
    }
}

// 텍스트 파일 여부를 판단하기 위한 헬퍼 함수
bool SzViewer::isTextFile(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
        return false;

    QByteArray data = file.read(1024);  // 처음 1024 바이트 읽기
    file.close();

    // 널 문자가 있다면 이진 파일로 간주
    if (data.contains('\0'))
        return false;

    return true;
}

void SzViewer::updateSettingPreview(const TextSettingProps& settings)
{
    qDebug() << "updateSettingPreview: "
        << "font:" << settings.getFont()
        << "lineSpacing:" << settings.getLineSpacing()
        << "textColor:" << settings.getTextColor()
        << "backgroundColor:" << settings.getBackgroundColor()
        << "padding:" << settings.getPadding();

    // StatusProperty를 통해 전역 설정을 임시로 업데이트
   // StatusProperty::instance().setTextFont(font);
   // StatusProperty::instance().setLineSpacing(lineSpacing);
   // StatusProperty::instance().setTextColor(textColor);
   // StatusProperty::instance().setBackgroundColor(backgroundColor);
   // StatusProperty::instance().setPadding(padding);

    // 즉시 반영
	textViewContainer->setSettings(settings);
}