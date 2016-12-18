#ifndef PRIVATECHAT_H
#define PRIVATECHAT_H

#include "note.h"
#include "grapscreen.h"
#include "player.h"
#include "tcpclient.h"
#include "tcpserver.h"
#include "doodleboard.h"


#include <QtNetwork>
#include <QtGui>
#include <QMenu>
#include <QColor>
#include <QPalette>
#include <QListWidget>
#include <QAction>
#include <QToolBar>
#include <QMainWindow>
#include <QMessageBox>
#include <QColorDialog>
#include <QFileDialog>
#include <QTextCharFormat>

enum MessageType
{
    Message,
    NewParticipant,
    ParticipantLeft,
    FileName,
    Refuse,
    Xchat
};
//枚举变量标志信息的类型，分别为消息，新用户加入，和用户退出

namespace Ui {
class PrivateChat;
}
/**
 *
 *实现私聊功能的头文件
 * @brief The PrivateChat class
 */

class PrivateChat : public QMainWindow
{
    Q_OBJECT

public:
    explicit PrivateChat(QString pasvusername, QString pasvuserip,QWidget *parent = 0);
    QString xpasvuserip;//用户ip
    QString xpasvusername;//登录名

    void actionInit();
    void createToolBar();

    QUdpSocket *xchat;
       qint32 xport;//端口
       void sendMessage(MessageType type,QString serverAddress="");//发送信息
       quint16 a;
   //    static  qint32 is_opened = 0;
       bool is_opened;

    ~PrivateChat();
protected:
    void hasPendingFile(QString userName,QString serverAddress,  //接收文件
                                    QString clientAddress,QString fileName);
        void participantLeft(QString userName,QString localHostName,QString time);
        bool eventFilter(QObject *target, QEvent *event); //事件过滤器

private:
    Ui::PrivateChat *ui;

    QAction *send;
    QAction *music;
    QAction *SaveRe;
    QAction *clear;
    QAction *note;
    QAction *explore;
    QAction *grapScreen;//截屏
    QToolBar *toolBar;

    TcpServer *server;
    QColor color;//颜色
    bool saveFile(const QString& fileName);//保存聊天记录
    QString getMessage();
    QString getIP();
    QString getUserName();//获得用户名
    QString message;
    QString fileName;

private slots:
    void sentFileName(QString);

    void processPendingDatagrams();

    void Close();
    void save();
    void SendFile();
    void Clear();
    void showPlayer();
    void showNote();
    void showExplore();
    void showGrapScreenTool();

    void on_fontComboBox_currentFontChanged(const QFont font);
    void on_comboBox_currentIndexChanged(const QString size);
    void on_BoldButton_clicked();
    void on_ItalicButton_clicked();
    void on_UnderlineButton_clicked();
    void on_ColorButton_clicked();
    void on_expressionButton_clicked();//表情
    void on_showPanelButton_clicked();//涂鸦板
    void on_insertPicButton_clicked();//插入图片
    void on_sendButton_clicked();

};

#endif // PRVATECHAT_H
