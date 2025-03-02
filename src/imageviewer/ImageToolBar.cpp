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
        ImageView::ScaleMode mode;
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
            mode = ImageView::FitIfLARGE;
            break;
        }
        m_imageViewContainer->resizeImage(mode);
        });
    this->addWidget(scaleModeAction);



    QAction* plusAction = new QAction(QIcon(":/icon/resources/icon/zoom_in_24dp_1F1F1F.svg"), "+", this);
    plusAction->setToolTip("Zoom in 10% of original size 10% (+)");
    connect(plusAction, &QAction::triggered, this, [this]() {m_imageViewContainer->resizeImage(ImageView::ScaleByPercentage, true);});
    this->addAction(plusAction);

    QAction* minusAction = new QAction(QIcon(":/icon/resources/icon/zoom_out_24dp_1F1F1F.svg"), "-", this);
    minusAction->setToolTip("Zoom out 10% of original size 10% (+)");
    connect(minusAction, &QAction::triggered, this, [this]() {m_imageViewContainer->resizeImage(ImageView::ScaleByPercentage, false);});
    this->addAction(minusAction);

    QAction* move1Action = new QAction(QIcon(":/icon/resources/icon/fast_rewind_24dp_1F1F1F_FILL0_wght400_GRAD0_opsz24.svg"), "<<<", this);
    move1Action->setToolTip("prev folder (PAGE UP)");
    connect(move1Action, &QAction::triggered, this, [this]() {move(ImageView::PrevFolder);});
    this->addAction(move1Action);

    QAction* move2Action = new QAction(QIcon(":/icon/resources/icon/skip_previous_24dp_1F1F1F_FILL0_wght400_GRAD0_opsz24.svg"), "<<", this);
    move2Action->setToolTip("first file");
    connect(move2Action, &QAction::triggered, this, [this]() {move(ImageView::First);});
    this->addAction(move2Action);

    QAction* move3Action = new QAction(QIcon(":/icon/resources/icon/chevron_left_24dp_1F1F1F.svg"), "<", this);
    move3Action->setToolTip("prev file (<-)");
    connect(move3Action, &QAction::triggered, this, [this]() {move(ImageView::Prev);});
    this->addAction(move3Action);

    QAction* move4Action = new QAction(QIcon(":/icon/resources/icon/chevron_right_24dp_1F1F1F.svg"), ">", this);
    move4Action->setToolTip("next file (->)");
    connect(move4Action, &QAction::triggered, this, [this]() {move(ImageView::Next);});
    this->addAction(move4Action);

    QAction* move5Action = new QAction(QIcon(":/icon/resources/icon/skip_next_24dp_1F1F1F_FILL0_wght400_GRAD0_opsz24.svg"), ">>", this);
    move5Action->setToolTip("last file");
    connect(move5Action, &QAction::triggered, this, [this]() {move(ImageView::Last);});
    this->addAction(move5Action);

    QAction* move6Action = new QAction(QIcon(":/icon/resources/icon/fast_forward_24dp_1F1F1F_FILL0_wght400_GRAD0_opsz24.svg"), ">>>", this);
    move6Action->setToolTip("next folder (PAGE DOWN)");
    connect(move6Action, &QAction::triggered, this, [this]() {move(ImageView::NextFolder);});
    this->addAction(move6Action);

    QAction* rotationVAction = new QAction(QIcon(":/icon/resources/icon/screen_rotation_alt_24dp_1F1F1F_FILL0_wght400_GRAD0_opsz24.svg"), "rotation 180", this);
    rotationVAction->setToolTip("rotation 180 degree");
    connect(rotationVAction, &QAction::triggered, this, [this]() {m_imageViewContainer->flip();});
    this->addAction(rotationVAction);

    QAction* roationRAction = new QAction(QIcon(":/icon/resources/icon/rotate_right_24dp_1F1F1F_FILL0_wght400_GRAD0_opsz24.svg"), "rotation r 90", this);
    roationRAction->setToolTip("rotation 90 degree right");
    connect(roationRAction, &QAction::triggered, this, [this]() {m_imageViewContainer->rotate(45);});
    this->addAction(roationRAction);

    QAction* roationLAction = new QAction(QIcon(":/icon/resources/icon/rotate_left_24dp_1F1F1F_FILL0_wght400_GRAD0_opsz24.svg"), "rotation l 90 ", this);
    roationLAction->setToolTip("rotation 90 degree left");
    connect(roationLAction, &QAction::triggered, this, [this]() {m_imageViewContainer->rotate(-45);});
    this->addAction(roationLAction);



    //for split
    QAction* splitAction = new QAction(QIcon(":/icon/resources/icon/auto_stories_24dp_1F1F1F.svg"), "Split", this);
    splitAction->setToolTip("split page");
    this->addAction(splitAction);

    // 클릭 시 동작 정의
    connect(splitAction, &QAction::triggered, this, [this]() {
        ImageSettingProps& setting = StatusStore::instance().getImageSettings();
        setting.setSplitView(m_imageViewContainer->changeSplitView());
        StatusStore::instance().saveImageSettings(setting);
        });


    QAction* fullScreenAction = new QAction(QIcon(":/icon/resources/icon/zoom_out_map_24dp_1F1F1F.svg"), "full-screan", this);
    fullScreenAction->setToolTip("Full Screen (f)");
    connect(fullScreenAction, &QAction::triggered, this, [this]() {m_imageViewContainer->toggleFullScreen();});
    this->addAction(fullScreenAction);


    QAction* deleteAction = new QAction(QIcon(":/icon/resources/icon/delete_24dp_1F1F1F_FILL0_wght400_GRAD0_opsz24.svg"), "delete", this);
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