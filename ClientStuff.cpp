#include "clientStuff.h"
#include <QFile>
#include <QDir>
#include <iostream>
#include <QDirIterator>

ClientStuff::ClientStuff(
        const QString hostAddress,
        int portMessNumber,
        int portFileNumber,
        QObject *parent
        ) : QObject(parent), m_nNextBlockSize(0)
{
    this->status = false;

    this->host = hostAddress;
    this->portMess = portMessNumber;
    this->portFile = portFileNumber;
    this->fileName = "file";

    this->tcpSocket = new QTcpSocket(this);
    connect(this->tcpSocket, &QTcpSocket::disconnected, this, &ClientStuff::closeConnection);

    this->tcpFileSocket = new QTcpSocket(this);
    connect(this->tcpFileSocket, &QTcpSocket::disconnected, this, &ClientStuff::closeConnection);

    this->timeoutTimer = new QTimer();
    this->timeoutTimer->setSingleShot(true);
    connect(this->timeoutTimer, &QTimer::timeout, this, &ClientStuff::connectionTimeout);
}

void ClientStuff::connect2host()
{
    this->timeoutTimer->start(3000);

    this->connectMessSocket();
    this->connectFileSocket();
}

void ClientStuff::connectMessSocket()
{
    this->tcpSocket->connectToHost(host, this->portMess);
    connect(this->tcpSocket, &QTcpSocket::connected, this, &ClientStuff::connected);
    connect(this->tcpSocket, &QTcpSocket::readyRead, this, &ClientStuff::readyRead);
}

void ClientStuff::connectFileSocket()
{
    this->tcpFileSocket->connectToHost(host, this->portFile);
    connect(this->tcpFileSocket, &QTcpSocket::connected, this, &ClientStuff::connected);
    connect(this->tcpFileSocket, &QTcpSocket::readyRead, this, &ClientStuff::readyReadFile);
}

void ClientStuff::connectionTimeout()
{
    if(this->tcpSocket->state() == QAbstractSocket::ConnectingState)
    {
        this->tcpSocket->abort();
        emit this->tcpSocket->error(QAbstractSocket::SocketTimeoutError);
    }
    if(this->tcpFileSocket->state() == QAbstractSocket::ConnectingState)
    {
        this->tcpFileSocket->abort();
        emit this->tcpFileSocket->error(QAbstractSocket::SocketTimeoutError);
    }
}

void ClientStuff::connected()
{
    this->status = true;
    emit statusChanged(this->status);
}

bool ClientStuff::getStatus()
{
    return this->status;
}

void ClientStuff::readyRead()
{
    QDataStream in(this->tcpSocket);
    while(true)
    {
        if (!this->m_nNextBlockSize)
        {
            if (this->tcpSocket->bytesAvailable() < sizeof(quint16))
            {
                break;
            }
            in >> this->m_nNextBlockSize;
        }

        if (this->tcpSocket->bytesAvailable() < this->m_nNextBlockSize)
        {
            break;
        }

        QString str;
        in >> str;

        if (str == "0")
        {
            str = "Connection closed";
            this->closeConnection();
        }
        if(str.contains("file:"))
        {
            this->fileName = str.remove(0, 5);
            str = "file:" + this->fileName;
            QString newDir = "../figures";

            #ifdef QT_DEBUG
                this->fileName += "/debug";
                newDir += "/debug";
            #else
                #ifdef QT_RELEASE
                   this->fileName = "/release";
                    newDir += "/release";
                #endif
            #endif
       }

        emit hasReadSome(str);
        m_nNextBlockSize = 0;
    }
}

void ClientStuff::readyReadFile()
{
    if(this->fileName == "")
    {
        QDataStream in(this->tcpFileSocket);
        while(true)
        {
            if (!this->m_nNextBlockSize)
            {
                if (this->tcpFileSocket->bytesAvailable() < sizeof(quint16))
                {
                    break;
                }
                in >> this->m_nNextBlockSize;
            }

            if (this->tcpFileSocket->bytesAvailable() < this->m_nNextBlockSize)
            {
                break;
            }

            QByteArray byteArr;
            in >> byteArr;

            QFile file(this->fileName);
            file.open(QIODevice::WriteOnly);
            file.write(byteArr);
            file.close();

            m_nNextFileBlockSize = 0;
        }
    }
}

void ClientStuff::closeConnection()
{
    this->timeoutTimer->stop();

    disconnect(this->tcpSocket, &QTcpSocket::connected, 0, 0);
    disconnect(this->tcpSocket, &QTcpSocket::readyRead, 0, 0);
    disconnect(this->tcpFileSocket, &QTcpSocket::readyRead, 0, 0);

    bool shouldEmit = false;
    switch (this->tcpSocket->state())
    {
        case 0:
            this->tcpSocket->disconnectFromHost();
            shouldEmit = true;
            break;
        case 2:
            this->tcpSocket->abort();
            shouldEmit = true;
            break;
        default:
            this->tcpSocket->abort();
    }

    switch (this->tcpFileSocket->state())
    {
        case 0:
            this->tcpFileSocket->disconnectFromHost();
            shouldEmit = true;
            break;
        case 2:
            this->tcpFileSocket->abort();
            shouldEmit = true;
            break;
        default:
            this->tcpFileSocket->abort();
    }

    if (shouldEmit)
    {
        this->status = false;
        emit statusChanged(this->status);
    }
}
