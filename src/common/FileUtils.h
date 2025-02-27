#ifndef FILEUTILS_H
#define FILEUTILS_H
#include <QStringList>

class FileUtils
{
public:
	FileUtils();
	static void sortByWindow(QStringList& fileList);
	static QString MoveFile(QStringList& fileList, QString currentFile, int moveIdx);
	static QStringList getFileList(QString currentFile);
};

#endif 