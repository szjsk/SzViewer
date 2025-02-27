#include "TextToolBar.h"
#include "TextSettingDialog.h"
#include "TextSearchDialog.h"
#include "TextSettingProps.h"

#include <QAction>
#include <QComboBox>
#include <QPushButton>


TextToolBar::TextToolBar(QWidget* parent, TextViewContainer* textViewContainer)
	: QToolBar(parent)
{
	setMovable(true);

	//text setting
	QAction* settingAction = new QAction(QIcon(":/path/to/icon.png"), "Setting", this);
	connect(settingAction, &QAction::triggered, this, [this, textViewContainer]() {
		openSettingDialog(textViewContainer);
		});
	this->addAction(settingAction);

	//text search
	QAction* searchAction = new QAction(QIcon(":/path/to/icon.png"), "Search", this);
	connect(searchAction, &QAction::triggered, this, [this, textViewContainer]() {
		openSearchDialog(textViewContainer);
		});
	this->addAction(searchAction);

	//container split
	QAction* splitAction = new QAction(QIcon(":/path/to/icon.png"), "Split", this);
	this->addAction(splitAction);
	connect(splitAction, &QAction::triggered, this, [this, textViewContainer]() {
		textViewContainer->changeSplitView();
		textViewContainer->refreshPage(textViewContainer->getFileInfo()->currentPosition);
		});

	//delete file
	QAction* deleteAction = new QAction(QIcon(":/path/to/icon.png"), "delete", this);
	connect(deleteAction, &QAction::triggered, this, [this, textViewContainer]() {
		textViewContainer->deleteFile(textViewContainer->getFileInfo());
		});
	this->addAction(deleteAction);
}

void TextToolBar::openSearchDialog(TextViewContainer* textViewContainer)
{
	TextSearchDialog dialog(this, textViewContainer);
	connect(&dialog, &TextSearchDialog::rowSelected, this, [this, textViewContainer](QString searchText, long page, long line) {
		textViewContainer->findPage(searchText, page, line);
		});
	dialog.exec();
}

void TextToolBar::openSettingDialog(TextViewContainer* textViewContainer)
{
	TextSettingDialog dialog(textViewContainer->getTextSettingProps(), this);
	connect(&dialog, &TextSettingDialog::settingsChanged, this, [this, textViewContainer](TextSettingProps settings) {
		textViewContainer->changeStyle(settings);
		});

	if (dialog.exec() == QDialog::Accepted) {
		textViewContainer->saveTextSettingProps(dialog.getTextSettings());
		textViewContainer->refreshPage(textViewContainer->getFileInfo()->currentPosition);
	}
	else {
		textViewContainer->changeStyle(textViewContainer->getTextSettingProps());
		textViewContainer->refreshPage(textViewContainer->getFileInfo()->currentPosition);
	}

}