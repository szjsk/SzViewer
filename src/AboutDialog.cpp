#include "AboutDialog.h"


AboutDialog::AboutDialog(QWidget* parent)
	:QDialog(parent)
{
	setWindowTitle("About");
	setFixedSize(400, 300);

    const QString titleText = R"(
       <h2>SzViewer</h2>
    )";

    const QString helpText = R"(
        <p><b>텍스트 뷰어 단축키</b><br>
        ← → : 페이지 좌우 이동<br>
        PgUp, PgDn : 폴더내 다음 파일, 이전 파일</p>
    )";

    const QString aboutText = R"(
        <p><b>작성자</b> : 황재식<br>
        <b>이메일</b> : <a href="mailto:sizz.js@gmail.com">sizz.js@gmail.com</a></p>
        <b>버전</b> : 0.0.2 (2025.02.21)</p>
    )";


    // 레이아웃 및 라벨 추가
    QVBoxLayout* layout = new QVBoxLayout(this);
    QLabel* labelTitle = new QLabel(titleText, this);
    labelTitle->setTextFormat(Qt::RichText);
    labelTitle->setAlignment(Qt::AlignCenter);
    layout->addWidget(labelTitle);

    QLabel* labelHelp = new QLabel(helpText, this);
    labelHelp->setTextFormat(Qt::RichText);
    labelHelp->setAlignment(Qt::AlignLeft);
    layout->addWidget(labelHelp);

    QLabel* labelAbout = new QLabel(aboutText, this);
    labelAbout->setTextFormat(Qt::RichText);
    labelAbout->setAlignment(Qt::AlignLeft);
    layout->addWidget(labelAbout);
    setLayout(layout);

}

AboutDialog::~AboutDialog()
{
}
