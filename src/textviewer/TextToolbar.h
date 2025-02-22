#ifndef TEXTTOOLBAR_H
#define TEXTTOOLBAR_H

#include <QToolBar>
#include <QAction>
#include <QComboBox>
#include <QPushButton>
#include <QWidget>
#include "../StatusStore.h"
#include "TextViewContainer.h"
#include "TextSettingDialog.h"
#include "TextSearchDialog.h"
#include "TextSettingProps.h"

class TextToolBar : public QToolBar
{
    Q_OBJECT
public:
    explicit TextToolBar(QWidget* parent = nullptr, TextViewContainer* textViewContainer = nullptr);

private:
	TextViewContainer* m_textViewContainer;

private slots:
	void openSettingDialog();
	void openSearchDialog();
	void updateSettingPreview(const TextSettingProps& settings);
	void goToTextPage(const QString& searchText, long page, long line);
};

#endif // TEXTTOOLBAR_H