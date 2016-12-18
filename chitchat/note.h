#ifndef NOTE_H
#define NOTE_H



#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QFont>
#include <QToolBar>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QPrinter>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QActionGroup>
#include <QPrintDialog>
#include <QPushButton>
#include <QToolButton>
#include <QTextStream>
#include <QMessageBox>
#include <QTextCodec>
#include <QMainWindow>
#include <QFileDialog>
#include <QFontDialog>
#include <QApplication>
#include <QInputDialog>
#include <QFontDatabase>
#include <QFontComboBox>
#include <QColorDialog>
/**
 *记事本功能的头文件
 * @brief The Note class
 */

class Note : public QMainWindow
{
    Q_OBJECT

public:
    explicit Note(QWidget *parent = 0);
    void createAction();
    void createMenu();
    void createToolBar();
    void fileRead();//读文件
    void fileWrite();//写文件


public:

    QMenu *file;
    QMenu *edit;
    QMenu *style;
    QMenu *insert;//插入菜单
    QMenu *search;//查找菜单
    QMenu *help;//帮助菜单

    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QToolBar *styleToolBar;
    QToolBar *insertToolBar;
    QToolBar *searchToolBar;
    QToolBar *helpToolBar;

    QTextEdit *textEdit;

    QAction *newFile;
    QAction *openFile;
    QAction *save;
    QAction *saveAs;
    QAction *printText;
    QAction *exit;

    QAction *undoAction;//撤销
    QAction *redoAction;//恢复
    QAction *copy;
    QAction *cut;
    QAction *paste;

    QAction *insertImg;
    QAction *insertTable;

    QAction *find;

    QAction *about;
    QAction *aboutQt;

    QAction *bold;
    QAction *italic;
    QAction *underLine;
    QAction *color;

    QAction *left;
    QAction *center;
    QAction *right;
    QAction *justify;
    QActionGroup *group;


    QLabel *fontLabel;
    QLabel *fontSizeLabel;
    QFontComboBox *fontCom;
    QComboBox *fontSizeCom;

    QString fileName;

    QLineEdit *lineEdit;
    QLabel *label;
    QPushButton *btn;
    QVBoxLayout *layout;

    QLineEdit *column;
    QLineEdit *row;



private slots:
    void findNext();

    void NewFile();
    void OpenFile();
    void Save();
    void SaveAs();
    void Color();
    void Bold();
    void Italic();
    void UnderLine();
    void ALignment(QAction *);
    void PrintText();
    void InsertImage();
    void InsertTable();
    void mergeFormat(QTextCharFormat);
    void FontComBox(QString fontComStr);
    void FontSizeCom(QString fontSizeStr);
    void Find();
    void About();
    void AboutQt();
    void showCurrentCharFormatChanged(const QTextCharFormat &form);

};


#endif // NOTE_H
