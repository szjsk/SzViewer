#include "TextSettingDialog.h"

TextSettingDialog::TextSettingDialog(const TextSettingProps& settings, QWidget* parent)
    : QDialog(parent),
    m_settings(settings)
{

    setWindowTitle(tr("폰트 선택"));

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // 검색 입력란
    QHBoxLayout* searchLayout = new QHBoxLayout;
    QLabel* searchLabel = new QLabel(tr("검색:"), this);
    m_searchEdit = new QLineEdit(this);
    searchLayout->addWidget(searchLabel);
    searchLayout->addWidget(m_searchEdit);
    mainLayout->addLayout(searchLayout);

    // 폰트 콤보박스
    QHBoxLayout* fontLayout = new QHBoxLayout;
    QLabel* fontLabel = new QLabel(tr("폰트:"), this);
    m_fontCombo = new QComboBox(this);
    fontLayout->addWidget(fontLabel);
    fontLayout->addWidget(m_fontCombo);
    mainLayout->addLayout(fontLayout);

    populateFontCombo();

    // 현재 폰트 설정
    int index = m_fontCombo->findText(settings.getFont().family());
    if (index >= 0)
        m_fontCombo->setCurrentIndex(index);

    // 폰트 크기 선택
    QHBoxLayout* sizeLayout = new QHBoxLayout;
    QLabel* sizeLabel = new QLabel(tr("크기:"), this);
    m_fontSizeSpin = new QSpinBox(this);
    m_fontSizeSpin->setRange(6, 100);
    m_fontSizeSpin->setValue(settings.getFont().pointSize() > 0 ? settings.getFont().pointSize() : 14);
    sizeLayout->addWidget(sizeLabel);
    sizeLayout->addWidget(m_fontSizeSpin);
    mainLayout->addLayout(sizeLayout);

    // 볼드, 이탤릭 체크박스
    QHBoxLayout* styleLayout = new QHBoxLayout;
    m_boldCheck = new QCheckBox(tr("볼드"), this);
    m_boldCheck->setChecked(settings.getFont().bold());
    m_italicCheck = new QCheckBox(tr("이탤릭"), this);
    m_italicCheck->setChecked(settings.getFont().italic());
    styleLayout->addWidget(m_boldCheck);
    styleLayout->addWidget(m_italicCheck);
    mainLayout->addLayout(styleLayout);

    // 줄 간격 입력
    QHBoxLayout* lineSpacingLayout = new QHBoxLayout;
    QLabel* lineSpacingLabel = new QLabel(tr("줄 간격:"), this);
    m_lineSpacingSpin = new QDoubleSpinBox(this);
    m_lineSpacingSpin->setRange(0.5, 3.0);
    m_lineSpacingSpin->setSingleStep(0.1);
    m_lineSpacingSpin->setValue(m_settings.getLineSpacing());

    lineSpacingLayout->addWidget(lineSpacingLabel);
    lineSpacingLayout->addWidget(m_lineSpacingSpin);
    mainLayout->addLayout(lineSpacingLayout);

    // 글자색 선택
	m_textColor = settings.getTextColor();
    QHBoxLayout* textColorLayout = new QHBoxLayout;
    QLabel* textColorLabel = new QLabel(tr("글자색:"), this);
    m_textColorButton = new QPushButton(tr("선택"), this);
    connect(m_textColorButton, &QPushButton::clicked, this, &TextSettingDialog::chooseTextColor);
    textColorLayout->addWidget(textColorLabel);
    textColorLayout->addWidget(m_textColorButton);
    mainLayout->addLayout(textColorLayout);

    // 배경색 선택
	m_backgroundColor = settings.getBackgroundColor();
    QHBoxLayout* backgroundColorLayout = new QHBoxLayout;
    QLabel* backgroundColorLabel = new QLabel(tr("배경색:"), this);
    m_backgroundColorButton = new QPushButton(tr("선택"), this);
    connect(m_backgroundColorButton, &QPushButton::clicked, this, &TextSettingDialog::chooseBackgroundColor);
    backgroundColorLayout->addWidget(backgroundColorLabel);
    backgroundColorLayout->addWidget(m_backgroundColorButton);
    mainLayout->addLayout(backgroundColorLayout);

    // 패딩 입력 (상/하/좌/우)
    QGridLayout* paddingLayout = new QGridLayout;
    QLabel* paddingLabel = new QLabel(tr("패딩:"), this);
    QLabel* paddingTopLabel = new QLabel(tr("상:"), this);
    QLabel* paddingBottomLabel = new QLabel(tr("하:"), this);
    QLabel* paddingLeftLabel = new QLabel(tr("좌:"), this);
    QLabel* paddingRightLabel = new QLabel(tr("우:"), this);
    m_paddingTopSpin = new QSpinBox(this);
    m_paddingBottomSpin = new QSpinBox(this);
    m_paddingLeftSpin = new QSpinBox(this);
    m_paddingRightSpin = new QSpinBox(this);
    m_paddingTopSpin->setRange(0, 100);
	m_paddingTopSpin->setValue(m_settings.getPadding().top());
    m_paddingBottomSpin->setRange(0, 100);
    m_paddingBottomSpin->setValue(m_settings.getPadding().bottom());
    m_paddingLeftSpin->setRange(0, 100);
    m_paddingLeftSpin->setValue(m_settings.getPadding().left());
    m_paddingRightSpin->setRange(0, 100);
    m_paddingRightSpin->setValue(m_settings.getPadding().right());
    paddingLayout->addWidget(paddingLabel, 0, 0, 1, 2);
    paddingLayout->addWidget(paddingTopLabel, 1, 0);
    paddingLayout->addWidget(m_paddingTopSpin, 1, 1);
    paddingLayout->addWidget(paddingBottomLabel, 2, 0);
    paddingLayout->addWidget(m_paddingBottomSpin, 2, 1);
    paddingLayout->addWidget(paddingLeftLabel, 3, 0);
    paddingLayout->addWidget(m_paddingLeftSpin, 3, 1);
    paddingLayout->addWidget(paddingRightLabel, 4, 0);
    paddingLayout->addWidget(m_paddingRightSpin, 4, 1);
    mainLayout->addLayout(paddingLayout);

    // OK / Cancel 버튼
    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    mainLayout->addWidget(m_buttonBox);

    // 기본적으로 컨트롤값 변경 시 프리뷰 업데이트 슬롯 연결
    connect(m_searchEdit, &QLineEdit::textChanged, this, &TextSettingDialog::filterFonts);
    connect(m_fontCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(updatePreview()));
    connect(m_fontSizeSpin, SIGNAL(valueChanged(int)), this, SLOT(updatePreview()));
    connect(m_boldCheck, SIGNAL(toggled(bool)), this, SLOT(updatePreview()));
    connect(m_italicCheck, SIGNAL(toggled(bool)), this, SLOT(updatePreview()));
    connect(m_lineSpacingSpin, SIGNAL(valueChanged(double)), this, SLOT(updatePreview()));
    connect(m_paddingTopSpin, SIGNAL(valueChanged(int)), this, SLOT(updatePreview()));
    connect(m_paddingBottomSpin, SIGNAL(valueChanged(int)), this, SLOT(updatePreview()));
    connect(m_paddingLeftSpin, SIGNAL(valueChanged(int)), this, SLOT(updatePreview()));
    connect(m_paddingRightSpin, SIGNAL(valueChanged(int)), this, SLOT(updatePreview()));

    // OK/Cancel에 따른 처리: dlgReject()에서 미리보기 복원, dlgAccept()에서 최종 적용.
    connect(m_buttonBox, &QDialogButtonBox::accepted, this, &TextSettingDialog::dlgAccept);
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &TextSettingDialog::dlgReject);

}

TextSettingDialog::~TextSettingDialog()
{
}

void TextSettingDialog::populateFontCombo(const QString& filter)
{
    m_fontCombo->clear();

    QFontDatabase fd;
    fd.writingSystemName(QFontDatabase::Korean);
    QStringList families = fd.families();
    qDebug() << "Available fonts:" << families;

    if (!filter.isEmpty()) {
        QString filterLower = filter.toLower();
        for (const QString& family : families) {
            if (family.toLower().contains(filterLower))
                m_fontCombo->addItem(family);
        }
    }
    else {
        m_fontCombo->addItems(families);
    }
}

void TextSettingDialog::filterFonts(const QString& text)
{
    populateFontCombo(text);
}

QFont TextSettingDialog::selectedFont() const
{
    QFont font;
    font.setFamily(m_fontCombo->currentText());
    font.setPointSize(m_fontSizeSpin->value());
    font.setBold(m_boldCheck->isChecked());
    font.setItalic(m_italicCheck->isChecked());
    return font;
}

double TextSettingDialog::selectedLineSpacing() const
{
    return m_lineSpacingSpin->value();
}

QColor TextSettingDialog::selectedTextColor() const
{
    return m_textColor;
}

QColor TextSettingDialog::selectedBackgroundColor() const
{
    return m_backgroundColor;
}

QMargins TextSettingDialog::selectedPadding() const
{
    return QMargins(m_paddingLeftSpin->value(), m_paddingTopSpin->value(),
        m_paddingRightSpin->value(), m_paddingBottomSpin->value());
}

void TextSettingDialog::chooseTextColor()
{
    QColor color = QColorDialog::getColor(m_settings.getTextColor(), this, tr("글자색 선택"));
    if (color.isValid()) {
        m_textColor = color;
        m_textColorButton->setStyleSheet(QString("background-color: %1").arg(color.name()));
        updatePreview();
    }
}

void TextSettingDialog::chooseBackgroundColor()
{
    QColor color = QColorDialog::getColor(m_settings.getBackgroundColor(), this, tr("배경색 선택"));
    if (color.isValid()) {
        m_backgroundColor = color;
        m_backgroundColorButton->setStyleSheet(QString("background-color: %1").arg(color.name()));
        updatePreview();
    }
}

TextSettingProps TextSettingDialog::getTextSettings()
{
    m_settings.setFont(selectedFont());
    m_settings.setLineSpacing(selectedLineSpacing());
    m_settings.setTextColor(selectedTextColor());
    m_settings.setBackgroundColor(selectedBackgroundColor());
    m_settings.setPadding(selectedPadding());

    return m_settings;
}

// 이 슬롯은 모든 컨트롤의 변경시 호출되어 preview를 emit합니다.
void TextSettingDialog::updatePreview()
{
	qDebug() << "---- updatePreview: " ;
	m_settings.setFont(selectedFont());
	m_settings.setLineSpacing(selectedLineSpacing());
	m_settings.setTextColor(selectedTextColor());
	m_settings.setBackgroundColor(selectedBackgroundColor());
	m_settings.setPadding(selectedPadding());

    emit settingsChanged(m_settings);
}

// OK를 눌렀을 때 최종 설정 적용
void TextSettingDialog::dlgAccept()
{
    accept();
}

// Cancel을 눌렀을 때 원래 상태로 복원하기 위한 슬롯
void TextSettingDialog::dlgReject()
{
    reject();
}
