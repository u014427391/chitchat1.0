#include <QApplication>
#include <QTextCodec>
#include "login.h"

/**
 * @brief main
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Login l;
    l.show();

    return a.exec();
}
