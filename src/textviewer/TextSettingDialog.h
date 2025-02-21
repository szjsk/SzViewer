#ifndef TextSettingDialog_H
#define TextSettingDialog_H

#include <QDialog>
#include <QFont>
#include <QColor>
#include <QMargins>
#include <QColorDialog>
#include <QFontDatabase>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QLineEdit>
#include "TextSettingProps.h"

class QLineEdit;
class QComboBox;
class QSpinBox;
class QCheckBox;
class QDoubleSpinBox;
class QDialogButtonBox;
class QPushButton;

class TextSettingDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TextSettingDialog(const TextSettingProps& settings, QWidget* parent = nullptr);
    ~TextSettingDialog();

    // 최종 설정값을 반환하는 메소드 (OK 선택 시 사용)
    QFont selectedFont() const;
    double selectedLineSpacing() const;
    QColor selectedTextColor() const;
    QColor selectedBackgroundColor() const;
    QMargins selectedPadding() const;
    TextSettingProps getTextSettings();

signals:
    // 실시간 프리뷰용 시그널 (szViewer 등에서 연결하여 적용)
    void settingsChanged(const TextSettingProps& settings);

private slots:
    void filterFonts(const QString& text);
    void updatePreview();
    void dlgAccept();
    void dlgReject();
    void chooseTextColor();
    void chooseBackgroundColor();

private:
    void populateFontCombo(const QString& filter = QString());

    TextSettingProps m_settings;
    QLineEdit* m_searchEdit;
    QComboBox* m_fontCombo;
    QSpinBox* m_fontSizeSpin;
    QCheckBox* m_boldCheck;
    QCheckBox* m_italicCheck;
    QDoubleSpinBox* m_lineSpacingSpin;
    QDialogButtonBox* m_buttonBox;

    // 색상 및 패딩 선택을 위한 추가 멤버
    QPushButton* m_textColorButton;
    QPushButton* m_backgroundColorButton;
    QColor m_textColor;
    QColor m_backgroundColor;
    QSpinBox* m_paddingTopSpin;
    QSpinBox* m_paddingBottomSpin;
    QSpinBox* m_paddingLeftSpin;
    QSpinBox* m_paddingRightSpin;
};

#endif // TextSettingDialog_H
