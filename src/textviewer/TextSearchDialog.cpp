#include "TextSearchDialog.h"
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QTimer>

TextSearchDialog::TextSearchDialog(QWidget* parent, const QHash<long, QVector<QString>>* m_textChunks)
	: QDialog(parent), currentMode(SearchMode), textChunksRefs(m_textChunks)
{
    setWindowTitle("검색/책갈피 다이얼로그");

    // 기본 레이아웃 설정
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QHBoxLayout* topLayout = new QHBoxLayout;

    searchLineEdit = new QLineEdit(this);
    searchButton = new QPushButton("검색", this);
    topLayout->addWidget(searchLineEdit);
    topLayout->addWidget(searchButton);
    mainLayout->addLayout(topLayout);

    // 결과를 보여줄 테이블 설정
    resultTable = new QTableWidget(this);
    resultTable->setColumnCount(3);
    QStringList headers;
    headers << "text" << "page" << "line";
    resultTable->setHorizontalHeaderLabels(headers);
    resultTable->horizontalHeader()->setStretchLastSection(true);
    resultTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    resultTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    resultTable->setSelectionMode(QAbstractItemView::SingleSelection);
    resultTable->setAlternatingRowColors(true);
    
    // 각 열의 너비 설정
    resultTable->setColumnWidth(0, 400); // 라인 열 너비
    resultTable->setColumnWidth(1, 60); // 페이지번호 열 너비
    resultTable->setColumnWidth(2, 60); // 위치 열 너비

    mainLayout->addWidget(resultTable);

    connect(searchButton, &QPushButton::clicked, this, &TextSearchDialog::onSearchButtonClicked);
    connect(resultTable, &QTableWidget::cellClicked, this, [this](int row, int column) {

        long page = resultTable->item(row, 1)->text().toInt()-1;
        long line = resultTable->item(row, 2)->text().toInt();

        emit rowSelected(searchLineEdit->text(), page, line);
        });

    resize(600, 400); // 초기 크기 설정

}

TextSearchDialog::~TextSearchDialog()
{
}

void TextSearchDialog::setMode(Mode mode)
{
    currentMode = mode;
    if (mode == BookmarkMode) {
        searchButton->setText("책갈피");
        // 책갈피 모드에서는 페이지번호와 라인만 표시
        resultTable->setColumnCount(2);
        QStringList headers;
        headers << "라인" << "페이지번호";
        resultTable->setHorizontalHeaderLabels(headers);
    }
    else {
        searchButton->setText("검색");
        resultTable->setColumnCount(3);
        QStringList headers;
        headers << "라인" << "페이지번호" << "위치";
        resultTable->setHorizontalHeaderLabels(headers);
    }
}

void TextSearchDialog::onSearchButtonClicked()
{
    QString searchText = searchLineEdit->text();
    if (searchText.isEmpty()) {
        return;
    }

    // 이전 결과 제거
    resultTable->setRowCount(0);

    // 외부 검색 로직을 위한 시그널 전송
    //emit searchRequested(searchText);

	for (long i = 0; i < textChunksRefs->size(); i++) {
		QVector<QString> lines = textChunksRefs->value(i);
		for (long j = 0; j < lines.size(); j++) {
			QString line = lines.at(j);
			if (line.contains(searchText)) {
				addResult(line, i, j);
			}
		}
	}

}

void TextSearchDialog::addResult(const QString& line, long page, long position)
{
    int row = resultTable->rowCount();
    resultTable->insertRow(row);

    QTableWidgetItem* lineItem = new QTableWidgetItem(line);
    QTableWidgetItem* pageItem = new QTableWidgetItem(QString::number(page+1));
    resultTable->setItem(row, 0, lineItem);
    resultTable->setItem(row, 1, pageItem);

    // 검색 모드에서만 위치 정보를 추가
    if (currentMode == SearchMode && resultTable->columnCount() > 2) {
        QTableWidgetItem* posItem = new QTableWidgetItem(QString::number(position));
        resultTable->setItem(row, 2, posItem);
    }
}
