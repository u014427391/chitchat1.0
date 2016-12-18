#include "drawwidget.h"
/**
 * @brief DrawWidget::DrawWidget
 * @param parent
 */
DrawWidget::DrawWidget(QWidget *parent) :
    QWidget(parent)
{

    setAutoFillBackground(true);
    QPalette palette(QPalette(Qt::white));

    pix = new QPixmap(size());

    pix->fill(Qt::white);//填充颜色

}
//setX函数用于主函数传参数
void DrawWidget::setStyle(int s){
    style=s;
}

void DrawWidget::setColor(QColor c){
    color=c;
}

void DrawWidget::setWidth(int w){
    width=w;
}

//鼠标移动事件
void DrawWidget::mouseMoveEvent(QMouseEvent *e){

    QPainter *painter = new QPainter;//打印器

    QPen pen;//画笔
    pen.setStyle((Qt::PenStyle)style);
    pen.setWidth(width);
    pen.setColor(color);

    painter->begin(pix);
    painter->setPen(pen);
    painter->drawLine(startPos,e->pos());//画线
    painter->end();
    startPos=e->pos();
    update();

}
//鼠标按下事件
void DrawWidget::mousePressEvent(QMouseEvent *e){

    startPos=e->pos();

}
//paint事件
void DrawWidget::paintEvent(QPaintEvent *e){

    QPainter painter(this);
    painter.drawPixmap(QPoint(0,0),*pix);
}
//重新设置大小事件
void DrawWidget::resizeEvent(QResizeEvent *e){

    if(height()>pix->height()){

        QPixmap *newPix=new QPixmap(size());
        newPix->fill(Qt::white);
        QPainter painter(newPix);
        painter.drawPixmap(QPoint(0,0),*pix);
        pix=newPix;

    }

    QWidget::resizeEvent(e);
}
//清除事件
void DrawWidget::Clear(){

    QPixmap *clearPix = new QPixmap(size());

    clearPix->fill(Qt::white);

    pix=clearPix;//新创建的画布覆盖原来的画布

    update();
}
