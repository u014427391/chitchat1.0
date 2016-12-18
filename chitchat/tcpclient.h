#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QDialog>
#include <QTcpSocket>
#include <QHostAddress>
#include <QFile>
#include <QTime>
namespace Ui {
    class TcpClient;
}
/**
 *
 *TCP通信客户端
 * @brief The TcpClient class
 */
class TcpClient : public QDialog
{
    Q_OBJECT

public:
    explicit TcpClient(QWidget *parent = 0);
    ~TcpClient();
    void setHostAddress(QHostAddress address);
    void setFileName(QString fileName){localFile = new QFile(fileName);}

protected:
    void changeEvent(QEvent *e);

private:
    Ui::TcpClient *ui;
    QTcpSocket *tcpClient;
    quint16 blockSize;
    QHostAddress hostAddress;
    qint16 tcpPort;//端口

    qint64 TotalBytes;
    qint64 bytesReceived;
    qint64 bytesToReceive;
    qint64 fileNameSize;
    QString fileName;
    QFile *localFile;
    QByteArray inBlock;

    QTime time;

private slots:

    void newConnect();
    void readMessage();
    void displayError(QAbstractSocket::SocketError);
    void on_tcpClientCancleBtn_clicked();
    void on_tcpClientCloseBtn_clicked();
};

#endif // TCPCLIENT_H
