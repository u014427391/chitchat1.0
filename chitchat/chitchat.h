#ifndef CHITCHAT_H
#define CHITCHAT_H

#include "note.h"
#include "player.h"
#include "login.h"
#include "grapscreen.h"
#include "privatechat.h"
#include "doodleboard.h"
#include "tcpclient.h"
#include "tcpserver.h"

#include <QtGui>
#include <QMenu>
#include <QColor>
#include <QAction>
#include <QToolBar>
#include <QPalette>
#include <QtNetwork>
#include <QListWidget>
#include <QTableWidget>
#include <QTextCodec>
#include <QMainWindow>
#include <QColorDialog>

/**
 *实现群聊功能的头文件,通过构造函数从登陆框里传登录名信息到群聊界面
 */
namespace Ui {
class Chitchat;
}

class Chitchat : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Chitchat
     * @param nickName//登录名
     * @param parent
     */
    explicit Chitchat(QString nickName,QWidget *parent = 0);
    void init();
    void createAction();//创建菜单栏
    void createToolBar();//工具栏
    QString getUserName();//获取登陆名
    QString getMessage();//获取信息

    PrivateChat *privatechat;//私聊类对象
    PrivateChat *privatechat1;

       QListWidget *list;//用于发送表情,还没实现发送表情

    ~Chitchat();
protected:
    void changeEvent(QEvent *e);//改变事件
    void sendMessage(MessageType type,QString serverAddress="");//根据用户状态，发送信息
    void newParticipant(QString userName,QString localHostName,QString ipAddress);//新用户加入
    void participantLeft(QString userName,QString localHostName,QString time);//用户离开
    void closeEvent(QCloseEvent *);//关闭事件
    void hasPendingFile(QString userName,QString serverAddress,
                        QString clientAddress,QString fileName);

     bool eventFilter(QObject *target, QEvent *event);//事件过滤器



private:
    Ui::Chitchat *ui;
    qint32 bb;

    QString name;

    QToolBar *toolBar;
    QAction *sendFile;
    QAction *music;
    QAction *save;
    QAction *clear;
    QAction *note;
    QAction *explore;
    QAction *grapWindowScreen;
    QAction *exitLogin;
    QAction *privateChichat;

    QUdpSocket *udpSocket;
    qint32 port;
    QString fileName;
    TcpServer *server;

    DoodleBoard *doodle;

    QString getIP();

    QColor color;//颜色



    void showxchat(QString name, QString ip);


    bool saveFile(const QString& fileName);//保存聊天记录

private slots:

    void Clear();
    void SendFile();
    void showPlayer();
    void showExplore();
    void showNote();
    void ExitLogin();
    void showGrapScreenTool();

    void Save();


    void processPendingDatagrams();
    void sentFileName(QString);
//    void currentFormatChanged(const QTextCharFormat &format);

//    void currentFormatChanged(const QTextCharFormat &format);

    void Close();
    void on_sentBtn_clicked();

    void on_colorBtn_clicked();
    void on_picBtn_clicked();
    void on_expression_clicked();
    void on_showPanelBtn_clicked();

    void on_fontComboBox_currentFontChanged(const QFont font);
    void on_fontSizeComboBox_currentIndexChanged(const QString size);

//    void on_boldBtn_clicked(bool checked);
//    void on_ItalicBtn_clicked(bool checked);
//    void on_underLineBtn_clicked(bool checked);
    void Bold();
    void Italic();

    void UnderLine();


    void on_tableWidget_clicked(QModelIndex index);
};

#endif // CHITCHAT_H
