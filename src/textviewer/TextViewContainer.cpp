#include "TextViewContainer.h"

TextViewContainer::TextViewContainer(QWidget* parent, const TextSettingProps& settings)
	: QWidget(parent), m_settings(settings)
{
 
    //main
    QVBoxLayout* vBoxContainer = new QVBoxLayout(this);

    //for textBrowser
    QHBoxLayout* hBoxBrowser = new QHBoxLayout();
    m_textBrowserArray[0] = createTextBrowser();
    m_textBrowserArray[1] = createTextBrowser();

    hBoxBrowser->addWidget(m_textBrowserArray[0]);
    hBoxBrowser->addWidget(m_textBrowserArray[1]);

    m_textBrowserArray[1]->setVisible(m_settings.isSplitView());

    vBoxContainer->addLayout(hBoxBrowser, 1);

	//for slider
    QHBoxLayout* hBoxSlider = new QHBoxLayout();
    m_qSlider = new QSlider(Qt::Horizontal, this);
    hBoxSlider->addWidget(m_qSlider);

    m_qSliderInfo = new QLabel(this);
    hBoxSlider->addWidget(m_qSliderInfo);

    vBoxContainer->addLayout(hBoxSlider);

    setLayout(vBoxContainer);

    // 슬라이더 값 변경 시 정보를 업데이트하는 람다 슬롯 연결
    connect(m_qSlider, &QSlider::valueChanged, this, [this](int value) {
        m_currentPosition = value - 1;
        setPage(m_currentPosition);
        m_qSliderInfo->setText(QString("page: %1 / %2").arg(value).arg(m_qSlider->maximum()));
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

    this->window()->setWindowTitle(QString("SzViewer - %1").arg(file.fileName()));

    m_fileName = filePath;

    QElapsedTimer timer;
    timer.start();
    QTextStream in(&file);
    m_text = in.readAll();    
    file.close();

    qDebug() << "file load time :" << timer.elapsed()/1000 << "s";

    //init
    m_currentPosition = 0;
    m_qSlider->setValue(0);
    m_qSliderInfo->setText(QString("page: %1 / %2").arg(0).arg(0));

    refreshPage();
    this->window()->activateWindow();
    this->window()->raise();
    m_textBrowserArray[0]->installEventFilter(this);
}

void TextViewContainer::findPage(const QString& text, long page, long line) {
    int browserIndex = 0;
    if (m_textBrowserArray[1]->isVisible()) {
		browserIndex = (page % 2);
    }
    m_currentPosition = page - browserIndex;

    setPage(m_currentPosition);
    m_qSlider->setValue(m_currentPosition + 1);

    QTextCursor cursor(m_textBrowserArray[browserIndex]->document());
    cursor = m_textBrowserArray[browserIndex]->document()->find(text, cursor);
    if (!cursor.isNull()) {
        m_textBrowserArray[browserIndex]->setTextCursor(cursor);
    }

}

void TextViewContainer::setPage(long position) {

    for (int i = 0; i < M_TEXT_BROWSER_CNT; i++) {

        if (m_textBrowserArray[i]->isVisible()) {
            QVector<QString> lines = m_textChunks.value(position + i);

            m_textBrowserArray[i]->clear();
            m_textBrowserArray[i]->setPlainText(lines.join(""));
            m_textBrowserArray[i]->moveCursor(QTextCursor::Start);

            applyLineSpacing(m_textBrowserArray[i]);

        }
    }

}

void TextViewContainer::nextPage() {

    for (int i = 0; i < M_TEXT_BROWSER_CNT; i++) {
        if (m_textBrowserArray[i]->isVisible() && m_currentPosition + 1 < m_textChunks.size()) {
            m_currentPosition++;
        }
    }

    setPage(m_currentPosition);
    m_qSlider->setValue(m_currentPosition + 1);

}

void TextViewContainer::prevPage() {
    
    for (int i = 0; i < M_TEXT_BROWSER_CNT; i++) {
        if (m_textBrowserArray[i]->isVisible() && m_currentPosition - i > -1) {
            m_currentPosition--;
        }
    }

    if (m_currentPosition < 0) {
        m_currentPosition = 0;
    }

    setPage(m_currentPosition);
    m_qSlider->setValue(m_currentPosition + 1);

}

void TextViewContainer::refreshFont(QTextBrowser* tb) {
	QFont f = m_settings.getFont();
    f.setHintingPreference(QFont::PreferFullHinting);
    f.setStyleStrategy(QFont::PreferAntialias);

    tb->setFont(f);
}

void TextViewContainer::refreshStyle(QTextBrowser* tb) {
    QFontMetrics fm(m_settings.getFont());
    int lineHeight = fm.lineSpacing();

    QString style = QString("QTextBrowser { "
        "background-color: %1; "
        "padding: %2px %3px %4px %5px; "
        "color: %6; }")
        .arg(m_settings.getBackgroundColor().name())
        .arg(m_settings.getPadding().top())
        .arg(m_settings.getPadding().right())
        .arg(m_settings.getPadding().bottom())
        .arg(m_settings.getPadding().left())
        .arg(m_settings.getTextColor().name())
        ;

    qDebug() << "lineHeight :: " << lineHeight  << ", m_settings.getLineSpacing() ::  " << m_settings.getLineSpacing() << ", height : " << tb->height();

    tb->setStyleSheet(style);
	applyLineSpacing(tb);

}

void TextViewContainer::applyLineSpacing(QTextBrowser* tb) {
    QFontMetrics fm(tb->font());
    int lineHeight = fm.lineSpacing() * m_settings.getLineSpacing();

    QTextCursor cursor(tb->document());
    cursor.select(QTextCursor::Document);
    QTextBlockFormat blockFormat = cursor.blockFormat();
    // FixedHeight의 경우 픽셀 단위로 간격을 지정합니다.
    blockFormat.setLineHeight(lineHeight, QTextBlockFormat::FixedHeight);
    cursor.setBlockFormat(blockFormat);
}

void TextViewContainer::refreshPage() {
    QElapsedTimer timer;
	m_textChunks.clear();

    timer.start();
    int maxLine = getLineHeight(m_textBrowserArray[0]);
    int maxWidth = m_textBrowserArray[0]->viewport()->width() - (m_settings.getPadding().left() + m_settings.getPadding().right());
    
    QString line;
    int width = 0;
    QFontMetrics fm(m_textBrowserArray[0]->font());
    int page = 0;
    QVector<QString> lines;
    //qDebug() << "maxLine : " << maxLine << ", maxWidth : " << width;

    for (long i = 0; i < m_text.length(); i++) {
		QChar c = m_text.at(i);
		line.append(c);
        width += getFontWidth(&fm, c);
        if (c == '\n' || width >= maxWidth) {
			//qDebug() << "lines.size() : " << lines.size() << ", width : " << width << "line : " << line;
            lines.append(line);
			line.clear();
			width = 0;

			if (lines.size() >= maxLine ) {
				m_textChunks.insert( page++, lines);
				lines.clear();
			}
        }
    }

    if (!line.isEmpty()) {
        lines.append(line);
    }

    m_textChunks.insert(page++, lines);

    qDebug() << "file split time :" << timer.elapsed() / 1000 << "s";
    qDebug() << m_textChunks.value(m_textChunks.size() - 1).join(" ");
	m_qSlider->setRange(1, m_textChunks.size());
    m_qSlider->setValue(m_currentPosition + 1);
    m_qSliderInfo->setText(QString("page: %1 / %2").arg(m_currentPosition+1).arg(m_qSlider->maximum()));

	setPage(m_currentPosition);
}

bool TextViewContainer::changeSplitView() {
    bool newSplit = !m_settings.isSplitView();
    m_settings.setSplitView(newSplit);
    m_textBrowserArray[1]->setVisible(newSplit);
    refreshPage();
	qDebug() << "Split View Changed : " << newSplit;
    return newSplit;
}

void TextViewContainer::setSettings(const TextSettingProps& s) {
    m_settings = s;
    refreshFont(m_textBrowserArray[0]);
    refreshFont(m_textBrowserArray[1]);
    refreshStyle(m_textBrowserArray[0]);
    refreshStyle(m_textBrowserArray[1]);
}


int TextViewContainer::getLineHeight(QTextBrowser* tb) {
    QFontMetrics fm(tb->font());
    int lineHeight = fm.lineSpacing() * m_settings.getLineSpacing();
	qDebug() << "lineHeight : " << lineHeight << " , " << fm.ascent();

    return ((tb->height() - (m_settings.getPadding().top()+ m_settings.getPadding().bottom())) / lineHeight) - 1;  // 세로 방향으로 표시 가능한 줄 수
}

int TextViewContainer::getFontWidth(QFontMetrics* fm, QChar c) {
    if (!m_charWidthCache.contains(c)) {
        int width = fm->horizontalAdvance(c); // 글자의 너비
//        width += fm->leftBearing(c); // 왼쪽 여백
        width += fm->rightBearing(c); // 오른쪽 여백

        // 탭 간격을 고려
        if (c == '\t') {
            width = fm->horizontalAdvance(' ') * 4; // 예: 탭 간격을 4개의 공백으로 설정
        }

        m_charWidthCache.insert(c,width);  // 캐시에 값 저장
    }
    return m_charWidthCache.value(c);
}

bool TextViewContainer::eventFilter(QObject* watched, QEvent* event) {
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_PageDown) {
            QString next = getNextOrPrevFileName(1);
            if (!next.isEmpty()) {
                loadText(next);
            }
        }
        else if (keyEvent->key() == Qt::Key_PageUp) {
            QString prev = getNextOrPrevFileName(-1);
            if (!prev.isEmpty()) {
                loadText(prev);
            }
        }else if (keyEvent->key() == Qt::Key_Left) {
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


QString TextViewContainer::getNextOrPrevFileName(int nextOrPrev) {
    QFileInfo fileInfo(m_fileName);
    QDir dir = fileInfo.dir();
    QStringList fileList = dir.entryList(QDir::Files, QDir::Name);
    QCollator collator;
    collator.setNumericMode(true);
    std::sort(fileList.begin(), fileList.end(),
        [&collator](const QString& s1, const QString& s2) {
            return collator.compare(s1, s2) < 0;
        });
    int currentIndex = fileList.indexOf(fileInfo.fileName());
    int idx = currentIndex + nextOrPrev;
    if (currentIndex != -1 && idx < fileList.size() && idx > -1) {
        return dir.absoluteFilePath(fileList.at(idx));
    }

    return QString();
}

void TextViewContainer::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
}

QHash<long, QVector<QString>>* TextViewContainer::getTextChunks() {
	return &m_textChunks;
}