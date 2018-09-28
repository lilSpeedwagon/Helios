#include "coder.h"

const QString Coder::DEFAULT_KEY = "abcdQWERTY1234567890keypass";

QString Coder::encode(QString str, QString key) {
    QString resultStr = "";
    for (int i = 0; i < str.size(); i++) {
        int c = str[i].unicode() + key[(int) key.size() / (i + 1)].unicode();
        while (c > 255)  {
            c -= 255;
        }
        resultStr.append((char) c);
    }
    return resultStr;
}

QString Coder::decode(QString str, QString key) {
    QString resultStr = "";
    for (int i = 0; i < str.size(); i++) {
        int c = str[i].unicode() - key[(int) key.size() / (i + 1)].unicode();
        while (c < 0)  {
            c += 255;
        }
        resultStr.append((char) c);
    }
    return resultStr;
}
