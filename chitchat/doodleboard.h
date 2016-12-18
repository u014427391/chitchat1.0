#ifndef DOODLEBOARD_H
#define DOODLEBOARD_H

#include "drawwidget.h"


#include <QMenu>
#include <QLabel>
#include <QPixmap>
#include <QMenuBar>
#include <QToolBar>
#include <QSpinBox>
#include <QComboBox>
#include <QTextCursor>
#include <QTextStream>
#include <QPrintDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextCodec>
#include <QMainWindow>
#include <QColorDialog>
/**
 *这是实现涂鸦板功能的头文件
 * @brief The DoodleBoard class
 */
class DoodleBoard : public QMainWindow
{
    Q_OBJECT
public:
    explicit DoodleBoard(QWidget *parent = 0);
    void fileInit();//文件操作类的组件初始
    void editInit();//编辑操作类的组件初始
    void createToolBar();//创建工具栏
private:

    DrawWidget *draw;//画板

    QToolBar *fileTool;//文件工具栏
    QToolBar *editTool;//编辑工具栏
    QLabel *styleLabel;
    QComboBox *styleCom;//画笔风格下拉菜单
    QLabel *widthLabel;
    QSpinBox *widthSpain;//画笔线条号数

    //文件操作类动作事件
    QAction *exit;
    QAction *load;
    QAction *save;

    //编辑操作类动作事件
    QAction *color;
    QAction *clear;
    QAction *send;

private slots:

    void savePic();//保存图片
    void loadPic();//加载图片到涂鸦板
    void showStyle();//改变画笔风格
    void showColor();//改变画笔颜色
    
};

#endif // DOODLEBOARD_H
