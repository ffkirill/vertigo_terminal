#ifndef TIMEINFOREADERDRIVER_H
#define TIMEINFOREADERDRIVER_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QRegExp>
#include <QVariantMap>

class QTimer;
class TimeInfoReaderDriver : public QObject
{
    Q_OBJECT
public:
    explicit TimeInfoReaderDriver(QObject *parent = 0);

private:

    enum class State {
        initial,
        awaitingResponse,
        awaitingSend,
        sleeping,
        error
    };

    State m_state=State::initial;
    QSerialPort *m_port;
    QTimer *m_timer;
    QByteArray m_readBuffer;
    QString m_portName;
    QString m_readerId;
    QSerialPort::BaudRate m_baudRate = QSerialPort::Baud38400;
    QSerialPort::DataBits m_dataBits = QSerialPort::Data7;
    QSerialPort::Parity m_parity = QSerialPort::NoParity;
    QSerialPort::StopBits m_stopBits = QSerialPort::TwoStop;
    QSerialPort::FlowControl m_flowControl = QSerialPort::NoFlowControl;
    quint16 awaitingResponsePeriod = 100;
    quint16 awaitingSilencePeriod = 100;
    quint16 awaitingNextReadingPeriod = 1000*2;
    const QRegExp responseRegex;
    bool isOpen=false;
    void sendRequest();
    bool processResponse();
    void setState(State state);

signals:
    void tokenRead(const QVariantMap token);
    void error(const QVariantMap message);

public slots:
    void open();
    void close();
    void setPortName(const QString &name);
    void setBaudRate(QSerialPort::BaudRate baudRate);
    void setDataBits(QSerialPort::DataBits dataBits);
    void setParity(QSerialPort::Parity parity);
    void setStopBits(QSerialPort::StopBits stopBits);
    void setFlowControl(QSerialPort::FlowControl flowControl);
    void setReaderId(const QString &id);

private slots:
    void readyRead();
    void timerShot();
    void onError(QSerialPort::SerialPortError serialPortError);

};

Q_DECLARE_METATYPE(TimeInfoReaderDriver *)
#endif // TIMEINFOREADERDRIVER_H
