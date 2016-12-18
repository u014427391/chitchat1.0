#ifndef GRAPSCREEN_H
#define GRAPSCREEN_H

#include <QtGui>
#include <QAction>
#include <QLabel>
#include <QPoint>
#include <QPixmap>
#include <QToolBar>
#include <QTextCodec>
#include <QMouseEvent>
#include <QRubberBand>
#include <QApplication>
#include <QMainWindow>


namespace Ui {
class GrapScreen;
}
/**
 *屏幕截屏功能的头文件
 * @brief The GrapScreen class
 */
class GrapScreen : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit GrapScreen(QWidget *parent = 0);
    void createEventFilter();
    void createWindow();
     bool eventFilter(QObject *, QEvent *);
    ~GrapScreen();
    
private:
    Ui::GrapScreen *ui;
    QPoint point1;//鼠标开始点击的点
    QPoint point2;

    QAction *save;//保存截图
    QAction *grapWindowGreen;//截屏
    QToolBar *toolBar;

    QLabel *saveLabel;
    QLabel *fullScreenLabel;

    QPixmap fullScreenPixmap;

    QRubberBand *rubberBand;//橡皮带

     bool leftMousePress;//鼠标是否左击

private slots:

    void savePic();


    void GrapWindowScreen();

};

#endif // GRAPSCREEN_H
