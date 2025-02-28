#ifndef DELETEFILESDIALOG_H
#define DELETEFILESDIALOG_H

#include <QDialog>
#include <QCheckBox>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDir>
#include <QFile>
#include <QFileInfo>

#ifdef Q_OS_WIN
#include <windows.h>
#include <shlobj.h>
#endif

#ifdef Q_OS_MAC
#import <Foundation/Foundation.h>
#endif

class DeleteFilesDialog : public QDialog {
    Q_OBJECT

public: //method
    explicit DeleteFilesDialog(const QStringList& files, bool isFolder, QWidget* parent = nullptr);
    QStringList getSelectedFiles() const;
    bool isDeleteFolderChecked() const;

private: //variable
    QListWidget* m_fileListWidget;
    QCheckBox* m_deleteFolderCheckBox;
    QPushButton* m_okButton;
    QPushButton* m_cancelButton;

private: //method
    void deleteFileOrFolder();
    void moveToTrash(const QString& filePath);
    void moveFolderToTrash(const QString& folderPath);
};

#endif // DELETEFILESDIALOG_H