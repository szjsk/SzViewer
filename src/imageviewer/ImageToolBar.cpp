#include "ImageToolBar.h"

ImageToolBar::ImageToolBar(QWidget* parent, ImageViewContainer* imageViewContainer)
	: QToolBar(parent), m_imageViewContainer(imageViewContainer)
{
    setMovable(true);

    // QComboBox 생성 및 항목 추가
    QComboBox* scaleModeAction = new QComboBox(this);
    scaleModeAction->addItems({ "size", "FitWindow(1)", "ORIGINAL(2)", "FitByWidth(3)" , "FitByHeight(4)", "FitIfLagrge(5)"});
    connect(scaleModeAction, QOverload<int>::of(&QComboBox::activated), this, [this, scaleModeAction](int index) {
        scaleModeAction->setCurrentIndex(0);
        ImageView::ScaleMode mode = ImageView::FitToWindow;
        switch (index) {
        case 0:
            return;
        case 1:
            mode = ImageView::FitToWindow;
            break;
        case 2:
            mode = ImageView::ORIGINAL;
            break;
        case 3:
            mode = ImageView::FitToWidth;
            break;
        case 4:
            mode = ImageView::FitToHeight;
            break;
        case 5:
            mode = ImageView::FitIfLARGE;
            break;
        default:
            mode = ImageView::FitToWindow;
            break;
        }
        resizeEvent(mode, 0);
        });
    this->addWidget(scaleModeAction);



    QAction* plusAction = new QAction(QIcon(":/path/to/icon.png"), "+", this);
    plusAction->setToolTip("Zoom in 10% of original size 10% (+)");
    connect(plusAction, &QAction::triggered, this, [this]() {resizeEvent(ImageView::ScaleByPercentage, 1);});
    this->addAction(plusAction);

    QAction* minusAction = new QAction(QIcon(":/path/to/icon.png"), "-", this);
    minusAction->setToolTip("Zoom out 10% of original size 10% (+)");
    connect(minusAction, &QAction::triggered, this, [this]() {resizeEvent(ImageView::ScaleByPercentage, -1);});
    this->addAction(minusAction);

    QAction* move1Action = new QAction(QIcon(":/path/to/icon.png"), "<<<", this);
    move1Action->setToolTip("prev folder (PAGE UP)");
    connect(move1Action, &QAction::triggered, this, [this]() {move(ImageView::PrevFolder);});
    this->addAction(move1Action);

    QAction* move2Action = new QAction(QIcon(":/path/to/icon.png"), "<<", this);
    move2Action->setToolTip("first file");
    connect(move2Action, &QAction::triggered, this, [this]() {move(ImageView::First);});
    this->addAction(move2Action);

    QAction* move3Action = new QAction(QIcon(":/path/to/icon.png"), "<", this);
    move3Action->setToolTip("prev file (<-)");
    connect(move3Action, &QAction::triggered, this, [this]() {move(ImageView::Prev);});
    this->addAction(move3Action);

    QAction* move4Action = new QAction(QIcon(":/path/to/icon.png"), ">", this);
    move4Action->setToolTip("next file (->)");
    connect(move4Action, &QAction::triggered, this, [this]() {move(ImageView::Next);});
    this->addAction(move4Action);

    QAction* move5Action = new QAction(QIcon(":/path/to/icon.png"), ">>", this);
    move5Action->setToolTip("last file");
    connect(move5Action, &QAction::triggered, this, [this]() {move(ImageView::Last);});
    this->addAction(move5Action);

    QAction* move6Action = new QAction(QIcon(":/path/to/icon.png"), ">>>", this);
    move6Action->setToolTip("next folder (PAGE DOWN)");
    connect(move6Action, &QAction::triggered, this, [this]() {move(ImageView::NextFolder);});
    this->addAction(move6Action);


    //for split
    QAction* splitAction = new QAction(QIcon(":/path/to/icon.png"), "Split", this);
    splitAction->setToolTip("split page");
    this->addAction(splitAction);

    // 클릭 시 동작 정의
    connect(splitAction, &QAction::triggered, this, [this]() {
        ImageSettingProps& setting = StatusStore::instance().getImageSettings();
        setting.setSplitView(m_imageViewContainer->changeSplitView());
        StatusStore::instance().saveImageSettings(setting);
        });

    QAction* deleteAction = new QAction(QIcon(":/path/to/icon.png"), "delete", this);
    deleteAction->setToolTip("delete file or folder (del)");
    connect(deleteAction, &QAction::triggered, this, [this]() {m_imageViewContainer->deleteImageFile();});
    this->addAction(deleteAction);

}

void ImageToolBar::move(ImageView::MoveMode mode) {
    if (mode == ImageView::NextFolder || mode == ImageView::PrevFolder) {
        m_imageViewContainer->navigateToFolder(mode);
    }
    else {
        m_imageViewContainer->navigateToFile(mode);
    }
}
 void ImageToolBar::resizeEvent(ImageView::ScaleMode mode, int plusMinus) {
     m_imageViewContainer->resizeImage(mode, plusMinus);
}