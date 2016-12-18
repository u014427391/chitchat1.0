#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlDatabase>

#include "chitchat.h"
#include "ui_chitchat.h"
/**
 * @brief Chitchat::Chitchat
 * @param nickName
 * @param parent
 */
Chitchat::Chitchat(QString nickName,QWidget *parent) :
    QMainWindow(parent),name(nickName),
    ui(new Ui::Chitchat)
{
    ui->setupUi(this);
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    setWindowIcon(QIcon(":/images/dove.png"));
    setWindowTitle(tr("白鸽局域网聊天器"));

    //设置背景
    QPalette pal;
    QPixmap pixmap(":/images/background.jpg");
    pal.setBrush(QPalette::Window,QBrush(pixmap));
    setPalette(pal);

    createAction();
    createToolBar();


    connect(ui->boldBtn,SIGNAL(clicked()),this,SLOT(Bold()));
    connect(ui->ItalicBtn,SIGNAL(clicked()),this,SLOT(Italic()));
    connect(ui->underLineBtn,SIGNAL(clicked()),this,SLOT(UnderLine()));
    connect(ui->textEdit,SIGNAL(currentCharFormatChanged(QTextCharFormat)),this,SLOT(currentFormatChanged(const QTextCharFormat)));
    connect(ui->closeBtn,SIGNAL(clicked()),this,SLOT(Close()));

    /**
     *
     *设置焦点策略，即主文本框不可以聚集焦点
     */
    ui->textEdit->setFocusPolicy(Qt::StrongFocus);
    ui->textBrowser->setFocusPolicy(Qt::NoFocus);

    ui->textEdit->setFocus();
    ui->textEdit->installEventFilter(this);//设置事件滤过器
    privatechat=NULL;//私聊
    privatechat1=NULL;

    /**
     *udp通信,端口为45455
     */
    udpSocket = new QUdpSocket(this);
    port = 45455;
    bb=0;
    udpSocket->bind(port,QUdpSocket::ShareAddress
                                    | QUdpSocket::ReuseAddressHint);
    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(processPendingDatagrams()));
    sendMessage(NewParticipant);

    server = new TcpServer(this);
    connect(server,SIGNAL(sendFileName(QString)),this,SLOT(sentFileName(QString)));
    //connect(ui->textEdit,SIGNAL(currentCharFormatChanged(QTextCharFormat)),this,SLOT(currentFormatChanged(const QTextCharFormat)));
    setFixedSize(926,578);
}

void Chitchat::init(){

}
/**
 *动作Action
 * @brief Chitchat::createAction
 */
void Chitchat::createAction(){

  sendFile=new QAction(QIcon(":/images/send.png"),tr("发送文件"),this);
  music=new QAction(QIcon(":/images/Music.png"),tr("音乐"),this);
  save=new QAction(QIcon(":/images/save.png"),tr("保存聊天记录"),this);
  clear=new QAction(QIcon(":/images/clear.png"),tr("清屏"),this);
  note=new QAction(QIcon(":/images/note.png"),tr("记事本"),this);
  explore=new QAction(QIcon(":/images/explore.png"),tr("浏览器"),this);
  exitLogin=new QAction(QIcon(":/images/exitLogin.png"),tr("退出登录"),this);
  grapWindowScreen=new QAction(QIcon(":/images/img.png"),tr("截图工具"),this);

  connect(sendFile,SIGNAL(triggered()),this,SLOT(SendFile()));
  connect(music,SIGNAL(triggered()),this,SLOT(showPlayer()));
  connect(save,SIGNAL(triggered()),this,SLOT(Save()));
  connect(clear,SIGNAL(triggered()),this,SLOT(Clear()));
  connect(note,SIGNAL(triggered()),this,SLOT(showNote()));
  connect(exitLogin,SIGNAL(triggered()),this,SLOT(ExitLogin()));
  connect(grapWindowScreen,SIGNAL(triggered()),this,SLOT(showGrapScreenTool()));
  connect(explore,SIGNAL(triggered()),this,SLOT(showExplore()));

}
/**
 *工具栏
 * @brief Chitchat::createToolBar
 */
void Chitchat::createToolBar(){
    toolBar=addToolBar(tr("工具栏"));
    toolBar->addAction(sendFile);
    toolBar->addAction(music);
    toolBar->addAction(save);
    toolBar->addAction(clear);
    toolBar->addAction(note);
    toolBar->addAction(exitLogin);
    toolBar->addAction(explore);
    toolBar->addAction(grapWindowScreen);

}
/**
 *
 *接收数据UDP
 * @brief Chitchat::processPendingDatagrams
 */
void Chitchat::processPendingDatagrams()   //接收数据UDP
{
    while(udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;//字节数组
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(),datagram.size());
        QDataStream in(&datagram,QIODevice::ReadOnly);//数据流
        int messageType;
        in >> messageType;
        QString userName,localHostName,ipAddress,message;
        QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        switch(messageType)
        {
        //处理接收到的信息
            case Message:
                {
                    in >>userName >>localHostName >>ipAddress >>message;//接收对方的用户名、ip、和信息
                    ui->textBrowser->setTextColor(Qt::blue);
                    ui->textBrowser->setCurrentFont(QFont("Times New Roman",12));
                    ui->textBrowser->append("[ " +userName+" ] "+ time);//将对方用户名加到文本浏览器
                    ui->textBrowser->append(message);//将收到的消息加到文本浏览器
                    break;
                }
            //新用户加入
            case NewParticipant:
                {
                    in >>userName >>localHostName >>ipAddress;//新用户的用户名、主机名、ip
                    newParticipant(userName,localHostName,ipAddress);

                    break;
                }
            //用户离开
            case ParticipantLeft:
                {
                    in >>userName >>localHostName;
                    participantLeft(userName,localHostName,time);
                    break;
                }
            //发送文件
        case FileName:
            {
                in >>userName >>localHostName >> ipAddress;
                QString clientAddress,fileName;
                in >> clientAddress >> fileName;
                hasPendingFile(userName,ipAddress,clientAddress,fileName);
                break;
            }
            //拒绝
        case Refuse:
            {
                in >> userName >> localHostName;
                QString serverAddress;
                in >> serverAddress;
                QString ipAddress = getIP();

                if(ipAddress == serverAddress)
                {
                    server->refused();
                }
                break;
            }
        case Xchat:
           {
                in >>userName >>localHostName >>ipAddress;
                /****/
                 // showxchat(userName,ipAddress);
               showxchat(localHostName,ipAddress);//显示与主机名聊天中，不是用户名
                break;
           }
        }
    }
}
/**
 *处理新用户加入
 * @brief Chitchat::newParticipant
 * @param userName
 * @param localHostName
 * @param ipAddress
 */
void Chitchat::newParticipant(QString userName,QString localHostName,QString ipAddress)
{
    bool bb = ui->tableWidget->findItems(localHostName,Qt::MatchExactly).isEmpty();
    if(bb)
    {
        QTableWidgetItem *user = new QTableWidgetItem(userName);//用户名
        QTableWidgetItem *host = new QTableWidgetItem(localHostName);//主机名
        QTableWidgetItem *ip = new QTableWidgetItem(ipAddress);//ip
        ui->tableWidget->insertRow(0);
        //插入一张图片,本来是想实现用户上传自己的图片
        QLabel *imgLabel=new QLabel;
        imgLabel->setPixmap(QPixmap(":/images/usericon/yy.png"));
        ui->tableWidget->setCellWidget(0,0,imgLabel);

        ui->tableWidget->setItem(0,1,user);
        ui->tableWidget->setItem(0,2,host);
        ui->tableWidget->setItem(0,3,ip);


        ui->textBrowser->setTextColor(Qt::gray);
        ui->textBrowser->setCurrentFont(QFont("Times New Roman",10));
        ui->textBrowser->append(tr("%1 在线！").arg(userName));
        ui->onLineUsersNum->setText(tr("在线人数：%1").arg(ui->tableWidget->rowCount())+tr(",登录者:%1").arg(name));
        sendMessage(NewParticipant);//发送信息
    }
}

/**
 *处理用户离开
 * @brief Chitchat::participantLeft
 * @param userName
 * @param localHostName
 * @param time
 */
void Chitchat::participantLeft(QString userName,QString localHostName,QString time)
{
    int rowNum = ui->tableWidget->findItems(localHostName,Qt::MatchExactly).first()->row();
    ui->tableWidget->removeRow(rowNum);
    ui->textBrowser->setTextColor(Qt::gray);
    ui->textBrowser->setCurrentFont(QFont("Times New Roman",10));
    ui->textBrowser->append(tr("%1 于 %2 离开！").arg(userName).arg(time));
    ui->onLineUsersNum->setText(tr("在线人数：%1").arg(ui->tableWidget->rowCount()));
}

/**
 *改变事件
 * @brief Chitchat::changeEvent
 * @param e
 */
void Chitchat::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
/**
 *获取ip
 * @brief Chitchat::getIP
 * @return
 */
QString Chitchat::getIP()  //获取ip地址
{
    //获取所有ip,放到列表里
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    /**
     *遍历列表，如果ip协议是IPve4的，则获取
     */
    foreach (QHostAddress address, list)
    {
       if(address.protocol() == QAbstractSocket::IPv4Protocol) //我们使用IPv4地址
            return address.toString();//将QHostAddress类型的ip转换为字符串格式
    }
    //否，返回0
       return 0;
}
/**
 * 发送信息
 *@brief Chitchat::sendMessage
 * @param type
 * @param serverAddress
 */
void Chitchat::sendMessage(MessageType type, QString serverAddress)  //发送信息
{
    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly);
    QString localHostName = QHostInfo::localHostName();
    QString address = getIP();
    out << type << getUserName() << localHostName;

    switch(type)
    {
        case ParticipantLeft:
            {
                break;
            }
        case NewParticipant:
            {
                out << address;
                break;
            }

        case Message :
            {
                if(ui->textEdit->toPlainText() == "")
                {
                    QMessageBox::warning(0,tr("警告"),tr("发送内容不能为空"),QMessageBox::Ok);
                    return;
                }
               out << address << getMessage();
               ui->textEdit->verticalScrollBar()->setValue(ui->textBrowser->verticalScrollBar()->maximum());
               break;

            }
        case FileName:
            {
                int row = ui->tableWidget->currentRow();//获取QTableWidget的行
                QString clientAddress = ui->tableWidget->item(row,3)->text();//客户端ip
                out << address << clientAddress << fileName;
                break;
            }
        case Refuse:
            {
                out << serverAddress;
                break;
            }
    }
    udpSocket->writeDatagram(data,data.length(),QHostAddress::Broadcast, port);
}
/**
 *获取用户名
 * @brief Chitchat::getUserName
 * @return
 */
QString Chitchat::getUserName()  //获取用户名
{

    return name;
}
/**
 *获取要发送的消息
 * @brief Chitchat::getMessage
 * @return
 */
QString Chitchat::getMessage()  //获得要发送的信息
{
    QString msg = ui->textEdit->toHtml();//转换格式

    ui->textEdit->clear();
    ui->textEdit->setFocus();
    return msg;
}
/**
 *关闭事件
 * @brief Chitchat::closeEvent
 */
void Chitchat::closeEvent(QCloseEvent *)
{
    sendMessage(ParticipantLeft);
}

/**
 *发送文件名
 * @brief Chitchat::sentFileName
 * @param fileName
 */
void Chitchat::sentFileName(QString fileName)
{
    this->fileName = fileName;
    sendMessage(FileName);
}
/**
 *接收文件
 * @brief Chitchat::hasPendingFile
 * @param userName
 * @param serverAddress
 * @param clientAddress
 * @param fileName
 */
void Chitchat::hasPendingFile(QString userName,QString serverAddress,  //接收文件
                            QString clientAddress,QString fileName)
{
    QString ipAddress = getIP();
    if(ipAddress == clientAddress)
    {
        int btn = QMessageBox::information(this,tr("接受文件"),
                                 tr("来自%1(%2)的文件：%3,是否接收？")
                                 .arg(userName).arg(serverAddress).arg(fileName),
                                 QMessageBox::Yes,QMessageBox::No);
        if(btn == QMessageBox::Yes)
        {
            QString name = QFileDialog::getSaveFileName(0,tr("保存文件"),fileName);
            if(!name.isEmpty())
            {
                TcpClient *client = new TcpClient(this);
                client->setFileName(name);
                client->setHostAddress(QHostAddress(serverAddress));
                client->show();

            }

        }
        else{
            sendMessage(Refuse,serverAddress);
        }
    }
}

/**
 *关闭窗口
 * @brief Chitchat::Close
 */
void Chitchat::Close(){
    QMessageBox::StandardButton bt=QMessageBox::question(this,tr("关闭窗口"),
    tr("是否关闭窗口"),QMessageBox::Yes|QMessageBox::No,QMessageBox::No);

    if(bt==QMessageBox::Cancel){
        return;
    }else if(bt==QMessageBox::Yes){
        this->close();
    }
}

/**
 *保存聊天记录
 * @brief Chitchat::Save
 */
void Chitchat::Save(){
    QString path=QFileDialog::getSaveFileName(this,tr("保存文件"),"records.txt","(*.*)");
    QFile file(path);
        if (!file.open(QFile::WriteOnly | QFile::Text)) {
            QMessageBox::warning(this, tr("保存文件"),
                                 tr("无法保存文件 %1:\n %2").arg(path)
                                 .arg(file.errorString()));

        }
        QTextStream out(&file);

        QString r= ui->textBrowser->toPlainText();

        out<<r;

        QSqlDatabase db=QSqlDatabase::addDatabase("SQLITE");

        db.setDatabaseName("chatingRecords.db");


        QSqlQuery query;

        QVariantList rec;

        rec<<r;

        query.addBindValue(rec);

        if(!query.execBatch())//进行批处理，如果出错就输出错误qDebug()<<query.lastError();

        QMessageBox::warning(this,tr("保存聊天记录!"),tr("保存成功!"));

}
/**
 *发送信息
 * @brief Chitchat::on_sentBtn_clicked
 */

void Chitchat::on_sentBtn_clicked()
{
       sendMessage(Message);
}
/**
 *事件滤过器
 * @brief Chitchat::eventFilter
 * @param target
 * @param event
 * @return
 */
bool Chitchat::eventFilter(QObject *target, QEvent *event)
{
    if(target == ui->textEdit)
    {
        if(event->type() == QEvent::KeyPress)
        {
             QKeyEvent *k = static_cast<QKeyEvent *>(event);
             if(k->key() == Qt::Key_Return)
             {
                 on_sentBtn_clicked();
                 return true;
             }
        }
    }
    return QWidget::eventFilter(target,event);
}
/**
 *发送文件
 * @brief Chitchat::SendFile
 */
void Chitchat::SendFile(){
    if(ui->tableWidget->selectedItems().isEmpty()){
          QMessageBox::warning(0,tr("选择用户"),tr("请先从用户列表选择要传送的用户！"),QMessageBox::Ok);
          return;
    }
      server->show();
      server->initServer();
}
/**
 *显示音乐播放器
 * @brief Chitchat::showPlayer
 */
void Chitchat::showPlayer(){
    Player *player=new Player;
    player->show();
}
/**
 *清屏
 * @brief Chitchat::Clear
 */
void Chitchat::Clear(){
    ui->textBrowser->clear();
}
/**
 *显示记事本
 * @brief Chitchat::showNote
 */
void Chitchat::showNote(){

    Note *n=new Note;
    n->show();
}
/**
 *
 *用默认浏览器打开网页
 * @brief Chitchat::showExplore
 */

void Chitchat::showExplore(){

    const QUrl AuthorUrl("http://hao.qq.com/");
    QDesktopServices::openUrl(AuthorUrl);
}

/**
 *显示截图工具
 * @brief Chitchat::showGrapScreenTool
 */
void Chitchat::showGrapScreenTool(){
    GrapScreen *g=new GrapScreen;
    g->show();
}

/**
 *退出登录，即返回登录界面
 * @brief Chitchat::ExitLogin
 */
void Chitchat::ExitLogin(){
    this->close();
    Login *l=new Login;
    l->show();
}

Chitchat::~Chitchat()
{
    delete ui;
}

/**
 *关闭窗口
 * @brief Chitchat::on_colorBtn_clicked
 */
void Chitchat::on_colorBtn_clicked()
{
    QColor color=QColorDialog::getColor(Qt::white,this);
    if(color.isValid()){
        ui->textEdit->setTextColor(color);
        ui->textEdit->setFocus();
    }
}
/**
 *发送表情，还没实现
 * @brief Chitchat::on_expression_clicked
 */
void Chitchat::on_expression_clicked()
{
    list=new QListWidget;

}
/**
 *发送图片
 * @brief Chitchat::on_picBtn_clicked
 */
void Chitchat::on_picBtn_clicked()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    QTextImageFormat format;
    QString image = QFileDialog::getOpenFileName(this,"insert image","/","png(*.png)");
    format.setName(image);
    cursor.insertImage(format);
}
/**
 *显示涂鸦板
 * @brief Chitchat::on_showPanelBtn_clicked
 */
void Chitchat::on_showPanelBtn_clicked()
{
    doodle=new DoodleBoard;
    doodle->show();
}


/**
 *发送框，字体设置
 * @brief Chitchat::on_fontComboBox_currentFontChanged
 * @param font
 */
void Chitchat::on_fontComboBox_currentFontChanged(const QFont font)
{
    ui->textEdit->setFont(font);
    ui->textEdit->setFocus();
}

/**
 *发送框字体大小设置
 * @brief Chitchat::on_fontSizeComboBox_currentIndexChanged
 * @param size
 */
void Chitchat::on_fontSizeComboBox_currentIndexChanged(const QString size)
{
    ui->textEdit->setFontPointSize(size.toDouble());
    ui->textEdit->setFocus();
}

/**
 *发送文本框字体加粗
 * @brief Chitchat::Bold
 */
void Chitchat::Bold()
{
    QTextCharFormat format;
    format.setFontWeight(ui->boldBtn->isChecked()?QFont::Bold:QFont::Normal);
    ui->textEdit->mergeCurrentCharFormat(format);
}
/**
 *文本框字体斜体设置
 * @brief Chitchat::Italic
 */
void Chitchat::Italic()
{
    QTextCharFormat format;
    format.setFontItalic(ui->ItalicBtn->isChecked());
    ui->textEdit->mergeCurrentCharFormat(format);
}
/**
 *下划线
 * @brief Chitchat::UnderLine
 */
void Chitchat::UnderLine()
{
    QTextCharFormat format;
    format.setFontUnderline(ui->underLineBtn->isChecked());
    ui->textEdit->mergeCurrentCharFormat(format);
}
/**
 *QTableWidget单击
 * @brief Chitchat::on_tableWidget_clicked
 * @param index
 */
void Chitchat::on_tableWidget_clicked(QModelIndex index)
{

            if(!privatechat){

            privatechat = new PrivateChat(ui->tableWidget->item(index.row(),1)->text(), //接收主机名
                                   ui->tableWidget->item(index.row(),3)->text()) ;//接收用户IP
            }

            QByteArray data;
            QDataStream out(&data,QIODevice::WriteOnly);
            QString localHostName = QHostInfo::localHostName();
            QString address = getIP();
//            out << Xchat << getUserName() << localHostName << address;
              out<< getUserName() << localHostName << address;
            udpSocket->writeDatagram(data,data.length(),QHostAddress::QHostAddress(ui->tableWidget->item(index.row(),3)->text()), port);

                privatechat->show();
            privatechat->is_opened = true;

 }

void Chitchat::showxchat(QString name, QString ip)
{

    if(!privatechat1)
        /****/
    privatechat1 = new PrivateChat(getUserName(),ip);

    bb++;


}
