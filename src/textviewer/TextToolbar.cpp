#include "TextToolBar.h"

TextToolBar::TextToolBar(QWidget* parent, TextViewContainer* textViewContainer)
	: QToolBar(parent), m_textViewContainer(textViewContainer)
{
    setMovable(true);

    QAction* settingAction = new QAction(QIcon(":/path/to/icon.png"), "Setting", this);
    connect(settingAction, &QAction::triggered, this, &TextToolBar::openSettingDialog);
    this->addAction(settingAction);

    QAction* searchAction = new QAction(QIcon(":/path/to/icon.png"), "Search", this);
    connect(searchAction, &QAction::triggered, this, &TextToolBar::openSearchDialog);
    this->addAction(searchAction);

    QAction* splitAction = new QAction(QIcon(":/path/to/icon.png"), "Split", this);
	this->addAction(splitAction);

    // 클릭 시 동작 정의
    connect(splitAction, &QAction::triggered, this, [this]() {
        TextSettingProps setting = StatusStore::instance().getTextSettings();
        setting.setSplitView(m_textViewContainer->changeSplitView());
        StatusStore::instance().saveSetting(&setting);
        });
}

void TextToolBar::openSearchDialog()
{
    TextSearchDialog dialog(this, m_textViewContainer->getTextChunks());
    connect(&dialog, &TextSearchDialog::rowSelected, this, &TextToolBar::goToTextPage);
    dialog.exec();
}


void TextToolBar::goToTextPage(const QString& searchText, long page, long line)
{
    m_textViewContainer->findPage(searchText, page, line);
}


void TextToolBar::openSettingDialog()
{
    TextSettingDialog dialog(StatusStore::instance().getTextSettings(), this);
    connect(&dialog, &TextSettingDialog::settingsChanged, this, &TextToolBar::updateSettingPreview);

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

void TextToolBar::updateSettingPreview(const TextSettingProps& settings)
{
    // 즉시 반영
    m_textViewContainer->setSettings(settings);
}