#include <QCoreApplication>
#include "client.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    printf("starting application...\n\r");
    Client *client = new Client();
    client->connectTo("localhost", 80);

    return a.exec();
}
