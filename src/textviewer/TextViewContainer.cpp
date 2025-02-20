#include "TextViewContainer.h"

TextViewContainer::TextViewContainer(QWidget* parent, const TextSettingProps& settings)
	: QWidget(parent), g_settings(settings)
{
 
    //main
    QVBoxLayout* vBoxContainer = new QVBoxLayout(this);

    //for textBrowser
    QHBoxLayout* hBoxBrowser = new QHBoxLayout();
    g_textBrowserArray[0] = createTextBrowser();
    g_textBrowserArray[1] = createTextBrowser();

    hBoxBrowser->addWidget(g_textBrowserArray[0]);
    hBoxBrowser->addWidget(g_textBrowserArray[1]);

    g_textBrowserArray[1]->setVisible(g_settings.isSplitView());

    vBoxContainer->addLayout(hBoxBrowser, 1);

	//for slider
    QHBoxLayout* hBoxSlider = new QHBoxLayout();
    qSlider = new QSlider(Qt::Horizontal, this);
    hBoxSlider->addWidget(qSlider);

    qSliderInfo = new QLabel(this);
    hBoxSlider->addWidget(qSliderInfo);

    vBoxContainer->addLayout(hBoxSlider);

    setLayout(vBoxContainer);

    // 슬라이더 값 변경 시 정보를 업데이트하는 람다 슬롯 연결
    connect(qSlider, &QSlider::valueChanged, this, [this](int value) {
        g_currentPosition = value - 1;
        setPage(g_currentPosition);
        qSliderInfo->setText(QString("page: %1 / %2").arg(value).arg(qSlider->maximum()));
        });

}

QTextBrowser* TextViewContainer::createTextBrowser() {

    QTextBrowser* tb = new QTextBrowser(this);
	refreshFont(tb);
	refreshStyle(tb);

    tb->installEventFilter(this);
    tb->setTextInteractionFlags(Qt::NoTextInteraction);  // 텍스트 상호작용 비활성화
    tb->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  // 세로 스크롤바 비활성화


    return tb;
}

void TextViewContainer::loadText(QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "파일을 열 수 없습니다.";
        return;
    }
    QElapsedTimer timer;
    timer.start();

    QTextStream in(&file);
    g_text = in.readAll();
    file.close();
    qDebug() << "file load time :" << timer.elapsed()/1000 << "s";

    g_currentPosition = 0;

    refreshPage();
}

void TextViewContainer::findPage(const QString& text, long page, long line) {
    int browserIndex = 0;
    if (g_textBrowserArray[1]->isVisible()) {
		browserIndex = (page % 2);
    }
    g_currentPosition = page - browserIndex;

    setPage(g_currentPosition);
    qSlider->setValue(g_currentPosition + 1);

    QTextCursor cursor(g_textBrowserArray[browserIndex]->document());
    cursor = g_textBrowserArray[browserIndex]->document()->find(text, cursor);
    if (!cursor.isNull()) {
        g_textBrowserArray[browserIndex]->setTextCursor(cursor);
    }

}

void TextViewContainer::setPage(long position) {

    for (int i = 0; i < G_TEXT_BROWSER_CNT; i++) {

        if (g_textBrowserArray[i]->isVisible()) {
            QVector<QString> lines = textChunks.value(position + i);

            g_textBrowserArray[i]->clear();
            g_textBrowserArray[i]->setPlainText(lines.join(""));
            g_textBrowserArray[i]->moveCursor(QTextCursor::Start);

        }
    }

}

void TextViewContainer::nextPage() {

    for (int i = 0; i < G_TEXT_BROWSER_CNT; i++) {
        if (g_textBrowserArray[i]->isVisible() && g_currentPosition + 1 < textChunks.size()) {
            g_currentPosition++;
        }
    }

    setPage(g_currentPosition);
    qSlider->setValue(g_currentPosition + 1);

}

void TextViewContainer::prevPage() {
    
    for (int i = 0; i < G_TEXT_BROWSER_CNT; i++) {
        if (g_textBrowserArray[i]->isVisible() && g_currentPosition - i > -1) {
            g_currentPosition--;
        }
    }

    if (g_currentPosition < 0) {
        g_currentPosition = 0;
    }

    setPage(g_currentPosition);
    qSlider->setValue(g_currentPosition + 1);

}

void TextViewContainer::refreshFont(QTextBrowser* tb) {
	QFont f = g_settings.getFont();
    f.setHintingPreference(QFont::PreferFullHinting);
    f.setStyleStrategy(QFont::PreferAntialias);

    tb->setFont(f);
    QString docStyle = QString("body { line-height: %1px; }").arg(f.pointSize());
    tb->document()->setDefaultStyleSheet(docStyle);

}

void TextViewContainer::refreshStyle(QTextBrowser* tb) {

    QString style = QString("QTextBrowser { "
        "background-color: %1; "
        "padding: %2px %3px %4px %5px; "
        "color: %6; "
        "line-height: %7px; }")
        .arg(g_settings.getBackgroundColor().name())
        .arg(g_settings.getPadding().top())
        .arg(g_settings.getPadding().right())
        .arg(g_settings.getPadding().bottom())
        .arg(g_settings.getPadding().left())
        .arg(g_settings.getTextColor().name())
        .arg(g_settings.getFont().pointSize() * g_settings.getLineSpacing());
    tb->setStyleSheet(style);

}

void TextViewContainer::refreshPage() {
    QElapsedTimer timer;
	textChunks.clear();

    timer.start();
    int maxLine = getLineHeight(g_textBrowserArray[0]);
    int maxWidth = g_textBrowserArray[0]->viewport()->width() - (g_settings.getPadding().left() + g_settings.getPadding().right());
    
    QString line;
    int width = 0;
    QFontMetrics fm(g_textBrowserArray[0]->font());
    int page = 0;
    QVector<QString> lines;

    for (long i = 0; i < g_text.length(); i++) {
		QChar c = g_text.at(i);
		line.append(c);
        width += getFontWidth(&fm, c);
        if (c == '\n' || width >= maxWidth) {
            lines.append(line);
			line.clear();
			width = 0;

			if (lines.size() >= maxLine ) {
				textChunks.insert( page++, lines);
				lines.clear();
			}
        }
    }

    if (!line.isEmpty()) {
        lines.append(line);
    }

    textChunks.insert(page++, lines);

    qDebug() << "file split time :" << timer.elapsed() / 1000 << "s";
    qDebug() << textChunks.value(textChunks.size() - 1).join(" ");
	qSlider->setRange(1, textChunks.size());
    qSlider->setValue(g_currentPosition + 1);
    qSliderInfo->setText(QString("page: %1 / %2").arg(g_currentPosition+1).arg(qSlider->maximum()));

	setPage(g_currentPosition);
}

bool TextViewContainer::changeSplitView() {
    bool newSplit = !g_settings.isSplitView();
    g_settings.setSplitView(newSplit);
    g_textBrowserArray[1]->setVisible(newSplit);
    refreshPage();
	qDebug() << "Split View Changed : " << newSplit;
    return newSplit;
}

void TextViewContainer::setSettings(const TextSettingProps& s) {
    g_settings = s;
    refreshFont(g_textBrowserArray[0]);
    refreshFont(g_textBrowserArray[1]);
    refreshStyle(g_textBrowserArray[0]);
    refreshStyle(g_textBrowserArray[1]);
}


int TextViewContainer::getLineHeight(QTextBrowser* tb) {
    QFontMetrics fm(tb->font());
    int lineHeight = fm.lineSpacing();
	qDebug() << "lineHeight : " << lineHeight << " , " << fm.ascent();

    return ((tb->height() - (g_settings.getPadding().top()+ g_settings.getPadding().bottom())) / lineHeight) - 3;  // 세로 방향으로 표시 가능한 줄 수
}

int TextViewContainer::getFontWidth(QFontMetrics* fm, QChar c) {
    if (!g_charWidthCache.contains(c)) {
        int width = fm->horizontalAdvance(c);
        g_charWidthCache.insert(c,width);  // 캐시에 값 저장
    }
    return g_charWidthCache.value(c);
}

bool TextViewContainer::eventFilter(QObject* watched, QEvent* event) {
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

        if (keyEvent->key() == Qt::Key_Space) {
            qDebug() << "Space Key Pressed";
        }
        else if (keyEvent->key() == Qt::Key_Left) {
            prevPage();
        }
        else if (keyEvent->key() == Qt::Key_Right) {
            nextPage();
        }

        return false;  // 이벤트를 가로채서 처리 완료
    }


    if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        QPoint mousePos = static_cast<QWidget*>(watched)->mapTo(this, mouseEvent->pos());

        int centerX = this->width() / 2;
        qDebug() << "Mouse Released " << mousePos.x() << " center : " << centerX;

        if (mousePos.x() < centerX) {
            prevPage();

            qDebug() << "Mouse Released on the Left Side";
        }
        else {
            nextPage();

            qDebug() << "Mouse Released on the Right Side";
        }
    }

    return QWidget::eventFilter(watched, event);
}

void TextViewContainer::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);

    // 창 크기 변경 시 현재 페이지 기준으로 다시 계산
    //QTimer::singleShot(30, this, [this]() {
       // refreshPage();
    //    });
}

QHash<long, QVector<QString>>* TextViewContainer::getTextChunks() {
	return &textChunks;
}