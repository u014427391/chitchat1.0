#include "grapscreen.h"
#include "ui_grapscreen.h"
/**
 * @brief GrapScreen::GrapScreen
 * @param parent
 */
GrapScreen::GrapScreen(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GrapScreen)
{
    ui->setupUi(this);
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    setWindowIcon(QIcon(":/images/img.png"));
    setWindowTitle(tr("截图工具"));
    /**
     *设置背景图片
     * @brief pal
     */
    QPalette pal;
    QPixmap pixmap(":/images/pic.jpg");
    pal.setBrush(QPalette::Window,QBrush(pixmap));
    setPalette(pal);


    createWindow();
    createEventFilter();

}

void GrapScreen::createWindow(){

    grapWindowGreen=new QAction(QIcon(":/images/img.png"),tr("截图工具"),this);
    toolBar=addToolBar(tr("工具栏"));
    toolBar->addAction(grapWindowGreen);


    fullScreenLabel=new QLabel();
    rubberBand=new QRubberBand(QRubberBand::Rectangle,fullScreenLabel);
    leftMousePress = false;

    saveLabel=new QLabel();
    save=new QAction(QIcon(":/images/save.png"),tr("保存"),saveLabel);
    saveLabel->addAction(save);
    saveLabel->setContextMenuPolicy(Qt::ActionsContextMenu);

    connect(grapWindowGreen,SIGNAL(triggered()),this,SLOT(GrapWindowScreen()));

    connect(save,SIGNAL(triggered()),this,SLOT(savePic()));

    setFixedSize(250,250);

}

/**
 *事件滤过器
 * @brief GrapScreen::eventFilter
 * @param o
 * @param e
 * @return
 */
bool GrapScreen::eventFilter(QObject *o, QEvent *e){
    if(o!=fullScreenLabel){
        return GrapScreen::eventFilter(o,e);
    }

    QMouseEvent *mouseEvent=static_cast<QMouseEvent*>(e);
/**
 *鼠标左键时
 */
    if((mouseEvent->button()==Qt::LeftButton)&&(mouseEvent->type()==QEvent::MouseButtonPress)){
        leftMousePress=true;

        point1=mouseEvent->pos();

        if(!rubberBand){
            rubberBand=new QRubberBand(QRubberBand::Rectangle,fullScreenLabel);

        }

        rubberBand->setGeometry(QRect(point1,QSize()));//矩形

        rubberBand->show();

        this->hide();//隐藏窗口


        return true;

    }
/**
 *鼠标移动
 */
    if ((mouseEvent->type() == QEvent::MouseMove)&& (leftMousePress)){
        if (rubberBand){
                  rubberBand->setGeometry(QRect(point1, mouseEvent->pos()).normalized());
       }
       // saveLabel->show();
        return true;
    }
/**
 *鼠标释放
 */
    if((mouseEvent->button()==Qt::LeftButton)&&(mouseEvent->type()==QEvent::MouseButtonRelease)){

        leftMousePress=false;

        if(rubberBand){
            point2=mouseEvent->pos();

            QRect rect=QRect(point1,point2);

            saveLabel->setPixmap(fullScreenPixmap.grabWidget(fullScreenLabel,rect.x(),rect.y(),rect.width(),rect.height()));


            saveLabel->setFixedSize(rect.width(),rect.height());

            saveLabel->show();

            rubberBand->hide();

            fullScreenLabel->hide();
        }
        return true;
    }

    return false;
}
/**
 *fullScreenLabel创建事件滤过器
 * @brief GrapScreen::createEventFilter
 */
void GrapScreen::createEventFilter(){

    fullScreenLabel->installEventFilter(this);
}
/**
 *截图功能
 * @brief GrapScreen::GrapWindowScreen
 */
void GrapScreen::GrapWindowScreen(){

    if(!fullScreenLabel){
        fullScreenLabel=new QLabel();
    }

    fullScreenPixmap=QPixmap::grabWindow(QApplication::desktop()->winId());

    fullScreenLabel->setPixmap(fullScreenPixmap);

   fullScreenLabel->showFullScreen();//全屏
}
/**
 *保存截图
 * @brief GrapScreen::savePic
 */
void GrapScreen::savePic(){

    QString fileName=QFileDialog::getSaveFileName(saveLabel,tr("保存截图"),"/","(*.png)");

    if(!fileName.isEmpty()){
         fullScreenPixmap.save(fileName);
    }
}

GrapScreen::~GrapScreen()
{
    delete ui;
}
