#ifndef LOGIN_H
#define LOGIN_H

#include "chitchat.h"
#include <QDialog>
#include <QTextCodec>

namespace Ui {
class Login;
}
/**
 *登录功能的头文件
 * @brief The Login class
 */
class Login : public QDialog
{
    Q_OBJECT
    
public:
    explicit Login(QWidget *parent = 0);
    ~Login();
    
private slots:
    void on_loginButton_clicked();

    void on_exitButton_clicked();

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
