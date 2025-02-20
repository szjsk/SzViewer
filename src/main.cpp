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


화면
2. 줄간격이 안먹음.
3. 글 사이 간격 추가
4. 터치모드 on/off 옵션 추가

-검색
-책갈피
-히스토리
*/