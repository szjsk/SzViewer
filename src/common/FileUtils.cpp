#include "FileUtils.h"
#include <QCollator>
#include <QFileInfo>
#include <QDir>

FileUtils::FileUtils() {

}

void FileUtils::sortByWindow(QStringList &fileList) {
    QCollator collator;
    collator.setNumericMode(true);
    std::sort(fileList.begin(), fileList.end(),
        [&collator](const QString& s1, const QString& s2) {
            return collator.compare(s1, s2) < 0;
        });
}

QString FileUtils::MoveFile(QStringList &fileList, QString currentFile, int moveIdx) {

	if (fileList.isEmpty() || currentFile.isEmpty()) {
        return QString();
	}

    QFileInfo fileInfo(currentFile);
    QDir dir = fileInfo.dir();

	fileList.indexOf(currentFile);

    int currentIndex = fileList.indexOf(currentFile);
    int idx = currentIndex + moveIdx;
    if (currentIndex > -1 && idx < fileList.size() && idx > -1) {
        return dir.absoluteFilePath(fileList.at(idx));
    }

    return QString();

}

QStringList FileUtils::getFileList(QString currentFile, SupportType type) {
	QFileInfo fileInfo(currentFile);
	QDir dir = fileInfo.dir();
	QStringList fileList = dir.entryList(QDir::Files);
    QStringList newFileBySupprot;
	for (int i = 0; i < fileList.size(); i++) {
		if (isSupportSuffix(dir.absoluteFilePath(fileList[i]), type)) {
			newFileBySupprot.append(dir.absoluteFilePath(fileList[i]));
		}
	}
	sortByWindow(newFileBySupprot);
	return newFileBySupprot;
}

bool FileUtils::isSupportSuffix(QString currentFile, SupportType type) {
    QFileInfo fileInfo(currentFile);

    QString suffix = fileInfo.suffix().toLower();

    if (type == SupportType::IMAGE && (suffix == "jpg" || suffix == "jpeg" || suffix == "png" || suffix == "bmp" || suffix == "gif" || suffix == "webp"
		|| suffix == "tiff" || suffix == "tif" || suffix == "svg" || suffix == "icns" || suffix == "ico" || suffix == "tga" || suffix == "wbmp"
		)) {
		return true;
	}
	else if (type == SupportType::TEXT && (suffix == "txt" || suffix == "ini")) {
		return true;
	}

    return false;
}


QString FileUtils::moveFolder(QString fileName, MoveMode moveMode, SupportType type) {

    // 현재 파일이 속한 폴더
    QFileInfo currentFileInfo(fileName);
    QDir currentFolder = currentFileInfo.dir();

    // 부모 폴더로 이동
    QDir parentDir = currentFolder;
    if (!parentDir.cdUp()) {
        return QString();
    }

    // 부모 폴더 내의 모든 서브 폴더 리스트 (알파벳 순)
    QStringList folderNames = parentDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
    if (folderNames.isEmpty()) {
        return QString();
    }

	sortByWindow(folderNames);

    // 현재 폴더 이름 및 인덱스 결정
    QString currentFolderName = currentFolder.dirName();
    int index = folderNames.indexOf(currentFolderName);
    if (index < 0) {
        return QString();
    }

    // next 또는 prev 폴더 인덱스 계산
    int nextIndex = -1;
    if (moveMode == FileUtils::MoveMode::NextFolder) {
        nextIndex = index + 1;
    }
    else if (moveMode == FileUtils::MoveMode::PrevFolder) {
        nextIndex = index - 1;
    }

    // 범위를 벗어나면 아무것도 하지 않음
    if (nextIndex < 0 || nextIndex >= folderNames.size()) {
        return QString();
    }

    // 다음(또는 이전) 폴더 경로 결정
    QString nextFolderName = folderNames.at(nextIndex);
    QDir nextFolder(parentDir.absoluteFilePath(nextFolderName));

    QStringList fileNames = nextFolder.entryList(QDir::Files, QDir::Name);

    QStringList newFileBySupprot;
    for (int i = 0; i < fileNames.size(); i++) {
        if (isSupportSuffix(nextFolder.absoluteFilePath(fileNames[i]), type)) {
            newFileBySupprot.append(nextFolder.absoluteFilePath(fileNames[i]));
        }
    }
    sortByWindow(newFileBySupprot);

    if (!newFileBySupprot.isEmpty()) {
        QString file = nextFolder.absoluteFilePath(fileNames[0]);
        return file;
    }
    return QString();
}