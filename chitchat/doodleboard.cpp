#include "doodleboard.h"
/**
 * @brief DoodleBoard::DoodleBoard
 * @param parent
 */
DoodleBoard::DoodleBoard(QWidget *parent) :
    QMainWindow(parent)
{
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale()); //设置编码格式

    setWindowTitle(tr("涂鸦板")); //设置窗口标题

    setWindowIcon(QIcon(":/images/gyachi.png"));//设置标题栏图标

    QPalette pal;
    QPixmap pixmap(":/images/background.jpg");
    pal.setBrush(QPalette::Window,QBrush(pixmap));
    setPalette(pal);



    draw=new DrawWidget; //创建画板对象

    setCentralWidget(draw);//设置画板为中心部件

    fileInit();
    editInit();

    createToolBar();//创建工具栏

    showStyle();//设置格式

    draw->setWidth(widthSpain->value());  //设置线宽

    draw->setColor(Qt::black);//设置颜色

    resize(500,400);

}

void DoodleBoard::fileInit(){
    //保存

      save = new QAction(QIcon(":/images/save.png"),tr("保存"),this);
      save->setStatusTip(tr("保存"));
      connect(save,SIGNAL(triggered()),this,SLOT(savePic()));

      exit = new QAction(QIcon(":/images/exit.png"),tr("退出"),this);
      exit->setStatusTip(tr("退出"));
      connect(exit,SIGNAL(triggered()),this,SLOT(close()));
  //加载图片
      load=new QAction(QIcon(":/images/load.png"),tr("加载"),this);
      load->setStatusTip(tr("加载图片"));
      connect(load,SIGNAL(triggered()),this,SLOT(loadPic()));



}

void DoodleBoard::editInit(){
    //格式

    styleLabel = new QLabel(tr("线性风格:"));
    styleCom = new QComboBox;
    styleCom->setStatusTip(tr("改变画笔风格"));
    styleCom->addItem(tr("DashDotLine"),static_cast<int>(Qt::DashDotDotLine));
    styleCom->addItem(tr("DashDotDotLine"),static_cast<int>(Qt::DashDotDotLine));
    styleCom->addItem(tr("DashLine"),static_cast<int>(Qt::DashLine));
    styleCom->addItem(tr("DotLine"),static_cast<int>(Qt::DotLine));
    styleCom->addItem(tr("SolidLine"),static_cast<int>(Qt::SolidLine));
    connect(styleCom,SIGNAL(activated(int)),this,SLOT(showStyle()));
//线宽
    widthLabel = new QLabel(tr("线宽:"));
    widthSpain = new QSpinBox;
    widthSpain->setStatusTip(tr("改变画笔线宽"));
    widthSpain->setValue(1);
    connect(widthSpain,SIGNAL(valueChanged(int)),draw,SLOT(setWidth(int)));
//颜色
    color = new QAction(QIcon(":/images/color.png"),tr("颜色"),this);
    color->setStatusTip(tr("改变画笔颜色"));
    QPixmap pix(30,30);
    pix.fill(Qt::green);
    connect(color,SIGNAL(triggered()),this,SLOT(showColor()));
//清除
    clear = new QAction(QIcon(":/images/clear.png"),tr("清除"),this);
    clear->setText(tr("清除"));
    clear->setStatusTip(tr("清除"));
    connect(clear,SIGNAL(triggered()),draw,SLOT(Clear()));
}



//创建工具栏
void DoodleBoard::createToolBar(){


//将部件添加到工具栏

    editTool=addToolBar(tr("编辑"));

    fileTool=addToolBar(tr("工具栏"));

    editTool->addWidget(styleLabel);
    editTool->addWidget(styleCom);
    editTool->addWidget(widthLabel);
    editTool->addWidget(widthSpain);
    editTool->addAction(color);
    editTool->addAction(clear);

    fileTool->addAction(save);
    fileTool->addAction(load);
    fileTool->addAction(exit);

}
//格式
void DoodleBoard::showStyle(){

    draw->setStyle(styleCom->itemData(styleCom->currentIndex(),Qt::UserRole).toInt());

}
//颜色
void DoodleBoard::showColor(){

    QColor col=QColorDialog::getColor(Qt::white,this);

    if(col.isValid()){
        draw->setColor(col);
        QPixmap pix(30,30);//画板
        pix.fill(col);//填充选择的颜色
        //color->setIcon(QIcon(pix));//设置按钮颜色
    }
}


void DoodleBoard::savePic(){

    draw->pix->save("E://myDraw.png");
    QMessageBox::about(this,tr("图片保存"),tr("图片保存成功!"));

}

void DoodleBoard::loadPic(){
    QString pic=QFileDialog::getOpenFileName(this,tr("打开文件"),
                                                  "/","picture(*.png)");
    draw->pix->load(pic);
}
