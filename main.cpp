#include "SzViewer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SzViewer w;
    w.show();
    return a.exec();
}
/*

글꼴
1. 크기
2. 볼드/이탤릭/일반
4. 줄간격
5. 안티앨라이언싱?
QFont font("맑은 고딕", 12);
font.setStyleStrategy(QFont::PreferAntialiasing);
tb->setFont(font);  // QTextBrowser 또는 QTextEdit에 적용

화면
1. 마진
2. 줄간격
3. 글 사이 간격
4. 터치모드 on/off
5. 글자 색
6. 배경 색


-검색
-책갈피
-히스토리
*/