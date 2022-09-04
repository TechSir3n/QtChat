#include "client.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Client client("127.0.0.1",2323);
    client.resize(240,400);
    client.show();
    return a.exec();
}
