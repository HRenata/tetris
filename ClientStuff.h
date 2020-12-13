#ifndef CLIENTSTUFF_H
#define CLIENTSTUFF_H

#include <QString>
#include <QTcpSocket>
#include <QDataStream>
#include <QTimer>

class ClientStuff : public QObject
{
    Q_OBJECT

public:
    ClientStuff(const QString hostAddress, int portMessVal, int portFileVal, QObject *parent = 0);

    QTcpSocket *tcpSocket;
    QTcpSocket *tcpFileSocket;
    bool getStatus();

public slots:
    void closeConnection();
    void connect2host();

signals:
    void statusChanged(bool);
    void hasReadSome(QString msg);

private slots:
    void readyRead();
    void readyReadFile();
    void connected();
    void connectionTimeout();

private:
    void connectFileSocket();
    void connectMessSocket();
    QString host;
    int portMess;
    int portFile;
    bool status;
    quint16 m_nNextBlockSize;
    quint16 m_nNextFileBlockSize;
    QTimer *timeoutTimer;
    QString fileName;
};

#endif // CLIENTSTUFF_H
