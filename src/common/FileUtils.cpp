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
    qDebug() << "currentFile : " << currentFile;
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

    if (type == SupportType::IMAGE && (suffix == "jpg" || suffix == "jpeg" || suffix == "png" || suffix == "bmp" || suffix == "gif" || suffix == "webp")) {
		return true;
	}
	else if (type == SupportType::TEXT && (suffix == "txt" || suffix == "ini")) {
		return true;
	}

    return false;
}
