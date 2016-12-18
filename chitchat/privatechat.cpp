#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>

#include "privatechat.h"
#include "ui_privatechat.h"
/**
 * @brief PrivateChat::PrivateChat
 * @param pasvusername
 * @param pasvuserip
 * @param parent
 */
PrivateChat::PrivateChat(QString pasvusername, QString pasvuserip,QWidget *parent) :
    QMainWindow(parent),xpasvusername(pasvusername),xpasvuserip(pasvuserip),
    ui(new Ui::PrivateChat)
{
    ui->setupUi(this);
    setWindowTitle(tr("飞鸽局域网聊天器"));
    setWindowIcon(QIcon(":/images/dove.png"));
    /**
     *设置背景
     * @brief pal
     */
    QPalette pal;
    QPixmap pixmap(":/images/background.jpg");
    pal.setBrush(QPalette::Window,QBrush(pixmap));
    setPalette(pal);

    xpasvusername = pasvusername;
    xpasvuserip = pasvuserip;

    actionInit();
    createToolBar();

    connect(ui->exitButton,SIGNAL(clicked()),this,SLOT(Close()));

    ui->chatWith->setText(tr("与%1聊天中   对方IP:%2").arg(xpasvusername).arg(pasvuserip));

    ui->textEdit->setFocusPolicy(Qt::StrongFocus);
        ui->textBrowser->setFocusPolicy(Qt::NoFocus);

        ui->textEdit->setFocus();
        ui->textEdit->installEventFilter(this);

        a = 0;
        is_opened = false;
        //UDP部分
            xchat = new QUdpSocket(this);
            xport = 45457;
         //   xchat->bind(xport, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
            xchat->bind( QHostAddress::QHostAddress(getIP()), xport );
            connect(xchat, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));

            //TCP部分
            server = new TcpServer(this);
            connect(server,SIGNAL(sendFileName(QString)),this,SLOT(sentFileName(QString)));

           // connect(ui->textEdit,SIGNAL(currentCharFormatChanged(QTextCharFormat)),this,SLOT(currentFormatChanged(const QTextCharFormat)));
}


//处理用户离开
void PrivateChat::participantLeft(QString userName,QString localHostName,QString time)
{
    ui->textBrowser->setTextColor(Qt::gray);
    ui->textBrowser->setCurrentFont(QFont("Times New Roman",10));
    ui->textBrowser->append(tr("%1 于 %2 离开！").arg(userName).arg(time));
}

QString PrivateChat::getUserName()  //获取用户名
{
    QStringList envVariables;
    envVariables << "USERNAME.*" << "USER.*" << "USERDOMAIN.*"
                 << "HOSTNAME.*" << "DOMAINNAME.*";
    QStringList environment = QProcess::systemEnvironment();
    foreach (QString string, envVariables)
    {
        int index = environment.indexOf(QRegExp(string));
        if (index != -1)
        {

            QStringList stringList = environment.at(index).split('=');
            if (stringList.size() == 2)
            {
                return stringList.at(1);
                break;
            }
        }
    }
    return false;
}

QString PrivateChat::getIP()  //获取ip地址
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress address, list)
    {
       if(address.protocol() == QAbstractSocket::IPv4Protocol) //我们使用IPv4地址
            return address.toString();
    }
       return 0;
}

void PrivateChat::hasPendingFile(QString userName,QString serverAddress,  //接收文件
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

void PrivateChat::processPendingDatagrams()   //接收数据UDP
{
    while(xchat->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(xchat->pendingDatagramSize());
        xchat->readDatagram(datagram.data(),datagram.size());
        QDataStream in(&datagram,QIODevice::ReadOnly);
        int messageType;
        in >> messageType;
        QString userName,localHostName,ipAddress,messagestr;
        QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        switch(messageType)
        {
            case Xchat:
            {
//                ui.show();
                break;
            }
            case Message:
                {

                    in >>userName >>localHostName >>ipAddress >>messagestr;
                    ui->textBrowser->setTextColor(Qt::blue);
                    ui->textBrowser->setCurrentFont(QFont("Times New Roman",12));
                    ui->textBrowser->append("[ " +localHostName+" ] "+ time);//与主机名聊天中
                    ui->textBrowser->append(messagestr);
            //        ui->textBrowser->show();
                    //this->textBrowser->setTextColor(Qt::blue);
                    //this->textBrowser->setCurrentFont(QFont("Times New Roman",12));
                    //this->textBrowser->append("[ " +localHostName+" ] "+ time);//与主机名聊天中
                    //this->textBrowser->append(messagestr);

            //        a ++;
            //        if( is_opened == false )//加了这句，接收端B不显示端口了
                    {
                        this->show();////解决bug1.收到私聊消息后才显示
                //        ui->textBrowser->show();
                    //    this->show();
                //        ui->textBrowser->show();
                    //    ui.show();
                    //    if( this->show() )
                //        this->hide();
                //        0 == a;
                        is_opened = true;
                    }
                    break;
                }
        case FileName:
            {
                in >>userName >>localHostName >> ipAddress;
                QString clientAddress,fileName;
                in >> clientAddress >> fileName;
                hasPendingFile(userName,ipAddress,clientAddress,fileName);
                break;
            }
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
        case ParticipantLeft:
            {
                in >>userName >>localHostName;
                participantLeft(userName,localHostName,time);
                QMessageBox::information(0,tr("本次对话关闭"),tr("对方结束了对话"),QMessageBox::Ok);
                a = 1;
                ui->textBrowser->clear();
                //is_opened = true;
            //    this->is_opened = false;
                ui->~PrivateChat();

                close();
            //    delete ui;
            //    ui = 0;
                break;
            }
        }
    }
}

void PrivateChat::sentFileName(QString fileName)
{
    this->fileName = fileName;
    sendMessage(FileName);
}

QString PrivateChat::getMessage()  //获得要发送的信息
{
    QString msg = ui->textEdit->toHtml();
    qDebug()<<msg;
    ui->textEdit->clear();
    ui->textEdit->setFocus();
    return msg;
}

//通过私聊套接字发送到对方的私聊专用端口上
void PrivateChat::sendMessage(MessageType type , QString serverAddress)  //发送信息
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
    case Message :
        {
            if(ui->textEdit->toPlainText() == "")
            {
                QMessageBox::warning(0,tr("警告"),tr("发送内容不能为空"),QMessageBox::Ok);
                return;
            }
            message = getMessage();
            out << address  << fileName;
            ui->textBrowser->verticalScrollBar()->setValue(ui->textBrowser->verticalScrollBar()->maximum());
            break;
        }
    case FileName:
            {
                QString clientAddress = xpasvuserip;
                out << address<< fileName;
                break;
            }
    case Refuse:
            {
                out << serverAddress;
                break;
            }
    }
    xchat->writeDatagram(data,data.length(),QHostAddress::QHostAddress(xpasvuserip), 45457);

}

void PrivateChat::showPlayer(){
    Player *player=new Player;
    player->show();
}

void PrivateChat::showExplore(){

    const QUrl AuthorUrl("http://hao.qq.com/");
    QDesktopServices::openUrl(AuthorUrl);

}

void PrivateChat::showNote(){
    Note *n=new Note;
    n->show();
}

void PrivateChat::showGrapScreenTool(){
    GrapScreen *g=new GrapScreen;
    g->show();
}

void PrivateChat::Clear(){
    ui->textBrowser->clear();
}

void PrivateChat::SendFile(){
      server->show();
      server->initServer();
}
bool PrivateChat::eventFilter(QObject *target, QEvent *event)
{
    if(target == ui->textEdit)
    {
        if(event->type() == QEvent::KeyPress)//按下键盘某键
        {
             QKeyEvent *k = static_cast<QKeyEvent *>(event);
             if(k->key() == Qt::Key_Return)//回车键
             {
                on_sendButton_clicked();
                 return true;
             }
        }
    }
    return QWidget::eventFilter(target,event);
}


//void Chitchat::on_sendfile_clicked()
//{
//    if(ui->tableWidget->selectedItems().isEmpty())
//    {
//        QMessageBox::warning(0,tr("选择用户"),tr("请先从用户列表选择要传送的用户！"),QMessageBox::Ok);
//        return;
//    }
//    server->show();
//    server->initServer();
//}



void PrivateChat::Close(){
    //this->close();
    /*delete ui;
    ui = 0;*/
    QMessageBox::StandardButton bt=QMessageBox::question(this,tr("关闭窗口"),
    tr("是否关闭窗口"),QMessageBox::Yes|QMessageBox::No,QMessageBox::No);

    if(bt==QMessageBox::Cancel){
        return;
    }else if(bt==QMessageBox::Yes){
        sendMessage(ParticipantLeft);
           a = 1;
           ui->textBrowser->clear();
           //is_opened = true;
       //    this->is_opened = false;
           close();
           ui->~PrivateChat();

           //this->close();
           /*delete ui;
           ui = 0;*/
    }
}

void PrivateChat::save(){
    fileName="E:/records.txt";
    QFile file(fileName);
        if (!file.open(QFile::WriteOnly | QFile::Text)) {
            QMessageBox::warning(this, tr("保存文件"),
                                 tr("无法保存文件 %1:\n %2").arg(fileName)
                                 .arg(file.errorString()));

        }
        QTextStream out(&file);

        QString r= ui->textBrowser->toPlainText();

        QSqlDatabase db=QSqlDatabase::addDatabase("SQLITE");

        db.setDatabaseName("chatingRecords.db");


        QSqlQuery query;

        QVariantList rec;

        rec<<r;

        query.addBindValue(rec);

        if(!query.execBatch())//进行批处理，如果出错就输出错误qDebug()<<query.lastError();

        QMessageBox::warning(this,tr("保存聊天记录!"),tr("保存成功!"));

}
void PrivateChat::actionInit(){

    send=new QAction(QIcon(":/images/send.png"),tr("发送文件"),this);
    music=new QAction(QIcon(":/images/Music.png"),tr("播放音乐"),this);
   SaveRe=new QAction(QIcon(":/images/save.png"),tr("保存历史记录"),this);
    note=new QAction(QIcon(":/images/note.png"),tr("记事本"),this);
    clear=new QAction(QIcon(":/images/clear.png"),tr("清屏"),this);
    explore=new QAction(QIcon(":/images/explore.png"),tr("浏览器"),this);
    grapScreen=new QAction(QIcon(":/images/img.png"),tr("截图工具"),this);

    connect(send,SIGNAL(triggered()),this,SLOT(SendFile()));
    connect(music,SIGNAL(triggered()),this,SLOT(showPlayer()));
    connect(clear,SIGNAL(triggered()),this,SLOT(Clear()));
    connect(note,SIGNAL(triggered()),this,SLOT(showNote()));
    connect(SaveRe,SIGNAL(triggered()),this,SLOT(save()));
    connect(explore,SIGNAL(triggered()),this,SLOT(showExplore()));
    connect(grapScreen,SIGNAL(triggered()),this,SLOT(showGrapScreenTool()));
}


void PrivateChat::createToolBar(){
    toolBar=addToolBar(tr("工具栏"));
    toolBar->addAction(send);
    toolBar->addAction(music);
    toolBar->addAction(SaveRe);
    toolBar->addAction(clear);
    toolBar->addAction(note);
    toolBar->addAction(explore);
    toolBar->addAction(grapScreen);

}

PrivateChat::~PrivateChat()
{
    is_opened=false;
    delete ui;
}

void PrivateChat::on_fontComboBox_currentFontChanged(const QFont font)
{
    ui->textEdit->setFont(font);
    ui->textEdit->setFocus();
}

void PrivateChat::on_comboBox_currentIndexChanged(const QString size)
{
    ui->textEdit->setFontPointSize(size.toDouble());
    ui->textEdit->setFocus();
}


void PrivateChat::on_BoldButton_clicked()
{
    QTextCharFormat format;
    format.setFontWeight(ui->BoldButton->isChecked()?QFont::Bold:QFont::Normal);
    ui->textEdit->mergeCurrentCharFormat(format);

}

void PrivateChat::on_ItalicButton_clicked()
{
    QTextCharFormat format;
    format.setFontItalic(ui->ItalicButton->isChecked());
    ui->textEdit->mergeCurrentCharFormat(format);
}

void PrivateChat::on_UnderlineButton_clicked()
{
    QTextCharFormat format;
    format.setFontUnderline(ui->UnderlineButton->isChecked());
    ui->textEdit->mergeCurrentCharFormat(format);
}




void PrivateChat::on_ColorButton_clicked()
{
    QColor color=QColorDialog::getColor(Qt::white,this);
    if(color.isValid()){

        ui->textEdit->setTextColor(color);
        ui->textEdit->setFocus();
    }
}

void PrivateChat::on_expressionButton_clicked()
{

}

void PrivateChat::on_showPanelButton_clicked()
{
    DoodleBoard *d=new DoodleBoard;
    d->show();
}

void PrivateChat::on_insertPicButton_clicked()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    QTextImageFormat format;
    QString image = QFileDialog::getOpenFileName(this,"insert image","/","png(*.png)");
    format.setName(image);
    cursor.insertImage(format);
}

void PrivateChat::on_sendButton_clicked()
{
        sendMessage(Message);
        QString localHostName = QHostInfo::localHostName();
        QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        ui->textBrowser->setTextColor(Qt::blue);
        ui->textBrowser->setCurrentFont(QFont("Times New Roman",12));
        ui->textBrowser->append("[ " +localHostName+" ] "+ time);
        ui->textBrowser->append(message);
    //    is_opened = true;
}

