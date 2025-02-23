#include "DeleteFilesDialog.h"

DeleteFilesDialog::DeleteFilesDialog(const QStringList& files, bool isFolder, QWidget* parent)
    : QDialog(parent), m_fileListWidget(new QListWidget(this)), m_deleteFolderCheckBox(new QCheckBox("delete folder with all file", this)),
	m_okButton(new QPushButton("Confirm", this)), m_cancelButton(new QPushButton("Cancel", this))
{
    setWindowTitle("Delete File");

    // 파일 목록을 QListWidget에 추가하고 각 항목에 체크박스를 설정
    for (const QString& file : files) {
        QListWidgetItem* item = new QListWidgetItem(file, m_fileListWidget);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Checked);
    }

    // 레이아웃 설정
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_fileListWidget);
    mainLayout->addWidget(m_deleteFolderCheckBox);
	m_deleteFolderCheckBox->setCheckState(isFolder ? Qt::Checked : Qt::Unchecked);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(m_okButton);
    buttonLayout->addWidget(m_cancelButton);

    mainLayout->addLayout(buttonLayout);

    // 버튼 시그널 연결
    connect(m_okButton, &QPushButton::clicked, this, &DeleteFilesDialog::deleteFileOrFolder);
    connect(m_cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

QStringList DeleteFilesDialog::getSelectedFiles() const {
    QStringList selectedFiles;
    for (int i = 0; i < m_fileListWidget->count(); ++i) {
        QListWidgetItem* item = m_fileListWidget->item(i);
        if (item->checkState() == Qt::Checked) {
            selectedFiles.append(item->text());
        }
    }
    return selectedFiles;
}

bool DeleteFilesDialog::isDeleteFolderChecked() const {
    return m_deleteFolderCheckBox->isChecked();
}

void DeleteFilesDialog::deleteFileOrFolder() {
	
    QStringList selectedFiles = getSelectedFiles();
    if (isDeleteFolderChecked()) {
        QString folderPath = QFileInfo(m_fileListWidget->item(0)->text()).absolutePath();
        moveFolderToTrash(folderPath);

    } else {
        for (const QString& file : selectedFiles) {
            moveToTrash(file);
        }
    }

	accept();
}


void DeleteFilesDialog::moveToTrash(const QString& filePath) {
#ifdef Q_OS_WIN
    // Windows에서 파일을 휴지통으로 이동
    QString nativeFilePath = QDir::toNativeSeparators(filePath);
    wchar_t* file = new wchar_t[nativeFilePath.length() + 2];
    nativeFilePath.toWCharArray(file);
    file[nativeFilePath.length()] = 0;
    file[nativeFilePath.length() + 1] = 0;

    SHFILEOPSTRUCT fileOp;
    memset(&fileOp, 0, sizeof(SHFILEOPSTRUCT));
    fileOp.wFunc = FO_DELETE;
    fileOp.pFrom = file;
    fileOp.fFlags = FOF_ALLOWUNDO | FOF_NOCONFIRMATION;

    SHFileOperation(&fileOp);

    delete[] file;
#elif defined(Q_OS_MAC)
    // macOS에서 파일을 휴지통으로 이동
    NSString* nsFilePath = [NSString stringWithUTF8String : filePath.toUtf8().constData()];
    NSURL* fileURL = [NSURL fileURLWithPath : nsFilePath];
    [[NSFileManager defaultManager]trashItemAtURL:fileURL resultingItemURL : nil error : nil];
#else
    // 기타 플랫폼에서는 파일을 삭제
    QFile::remove(filePath);
#endif
}

void DeleteFilesDialog::moveFolderToTrash(const QString& folderPath) {
    QDir dir(folderPath);
    QFileInfoList fileList = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);

    // 폴더 내의 모든 파일과 폴더를 휴지통으로 이동
    for (const QFileInfo& fileInfo : fileList) {
        if (fileInfo.isDir()) {
            moveFolderToTrash(fileInfo.absoluteFilePath());
        }
        else {
            moveToTrash(fileInfo.absoluteFilePath());
        }
    }

    // 폴더 자체를 휴지통으로 이동
    moveToTrash(folderPath);
}