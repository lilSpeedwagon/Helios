#ifndef CODER_H
#define CODER_H

#include <QString>

class Coder
{
private:
    static const QString DEFAULT_KEY;
public:
    Coder() = delete;
    static QString encode(QString str, QString key = DEFAULT_KEY);
    static QString decode(QString str, QString key = DEFAULT_KEY);
};

#endif // CODER_H
