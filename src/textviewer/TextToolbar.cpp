#include "TextToolBar.h"
#include "TextSettingDialog.h"
#include "TextSearchDialog.h"
#include "TextSettingProps.h"
#include "../common/StatusStore.h"
#include <QAction>
#include <QComboBox>
#include <QPushButton>


TextToolBar::TextToolBar(QWidget* parent, TextViewContainer* textViewContainer)
	: QToolBar(parent)
{
	setMovable(true);

	//text setting
	QAction* settingAction = new QAction(QIcon(":/icon/resources/icon/settings_24dp_1F1F1F_FILL0_wght400_GRAD0_opsz24.svg"), "Setting", this);
	connect(settingAction, &QAction::triggered, this, [this, textViewContainer]() {
		openSettingDialog(textViewContainer);
		});
	this->addAction(settingAction);

	//text search
	QAction* searchAction = new QAction(QIcon(":/icon/resources/icon/find_in_page_24dp_1F1F1F_FILL0_wght400_GRAD0_opsz24.svg"), "Search", this);
	connect(searchAction, &QAction::triggered, this, [this, textViewContainer]() {
		openSearchDialog(textViewContainer);
		});
	this->addAction(searchAction);

	//container split
	QAction* splitAction = new QAction(QIcon(":/icon/resources/icon/auto_stories_24dp_1F1F1F.svg"), "Split", this);
	this->addAction(splitAction);
	connect(splitAction, &QAction::triggered, this, [this, textViewContainer]() {
		textViewContainer->changeSplitView();
		textViewContainer->refreshPage(textViewContainer->getFileInfo()->currentPosition);
		});

	//delete file
	QAction* deleteAction = new QAction(QIcon(":/icon/resources/icon/delete_24dp_1F1F1F_FILL0_wght400_GRAD0_opsz24.svg"), "delete", this);
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
	TextSettingDialog dialog(StatusStore::instance().getTextSettings(), this);
	connect(&dialog, &TextSettingDialog::settingsChanged, this, [this, textViewContainer](TextSettingProps settings) {
		textViewContainer->changeStyle(settings);
		});

	if (dialog.exec() == QDialog::Accepted) {
		StatusStore::instance().saveTextSettings(dialog.getTextSettings());
		textViewContainer->refreshPage(textViewContainer->getFileInfo()->currentPosition);
	}
	else {
		textViewContainer->changeStyle(StatusStore::instance().getTextSettings());
		textViewContainer->refreshPage(textViewContainer->getFileInfo()->currentPosition);
	}

}