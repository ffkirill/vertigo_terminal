#include "timeinforeaderdriver.h"
#include <QTimer>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QByteArray>
#include <QRegExp>
#include <QVariantMap>

TimeInfoReaderDriver::TimeInfoReaderDriver(QObject *parent) :
    QObject(parent),
    m_port(new QSerialPort(this)),
    m_timer(new QTimer(this)),
    responseRegex(QRegExp(QString(".*=(\\S+)\r\n.*")))
{
    QObject::connect(m_port, &QSerialPort::readyRead, this, &TimeInfoReaderDriver::readyRead);
    QObject::connect(m_port, SIGNAL(error(QSerialPort::SerialPortError)),
                     this, SLOT(onError(QSerialPort::SerialPortError)));
    QObject::connect(m_timer, &QTimer::timeout, this, &TimeInfoReaderDriver::timerShot);
}

void TimeInfoReaderDriver::open()
{
    m_port->setPortName(m_portName);
    isOpen = true;
    if (m_port->open(QIODevice::ReadWrite)) {
        if (m_port->setBaudRate(m_baudRate)
                &&m_port->setDataBits(m_dataBits)
                &&m_port->setParity(m_parity)
                &&m_port->setStopBits(m_stopBits)
                &&m_port->setFlowControl(m_flowControl)) {

            m_port->setReadBufferSize(0);

            m_port->setDataTerminalReady(true);
            m_port->setRequestToSend(true);
            setState(State::awaitingSend);

        }
    }
}

void TimeInfoReaderDriver::close()
{
    m_timer->stop();
    m_port->close();
    m_readBuffer.clear();
    isOpen = false;
    setState(State::initial);

}

void TimeInfoReaderDriver::sendRequest()
{
    m_port->write(QByteArray::fromHex("33303030303030300A"));
}

bool TimeInfoReaderDriver::processResponse()
{
    bool result = false;
    if (responseRegex.indexIn(QString(m_readBuffer)) > -1) {
        result = true;
        emit tokenRead({
           {"reader", m_readerId},
           {"token", responseRegex.cap(1)}
        });
    }
    return result;
}

void TimeInfoReaderDriver::setState(TimeInfoReaderDriver::State state)
{
    m_readBuffer.clear();
    m_state = state;
    m_timer->stop();

    switch (state) {
    case State::awaitingSend:
        m_port->setDataTerminalReady(true);
        m_port->setRequestToSend(true);
        m_timer->setInterval(awaitingSilencePeriod);
        m_timer->start();
        break;
    case State::awaitingResponse:
        m_timer->setInterval(awaitingResponsePeriod);
        m_timer->start();
        break;
    case State::sleeping:
        m_port->setDataTerminalReady(false);
        m_port->setRequestToSend(false);
        m_timer->setInterval(awaitingNextReadingPeriod);
        m_timer->start();
        break;
    case State::error:
        m_port->close();
        m_timer->setInterval(awaitingNextReadingPeriod);
        m_timer->start();
        break;
    default:
        break;
    }
}

void TimeInfoReaderDriver::readyRead()
{
    switch (m_state) {

    case State::awaitingSend:
        m_timer->start();
        m_port->readAll();
        break;

    case State::awaitingResponse:
        m_readBuffer.append(m_port->readAll());
        if (processResponse()) {
            setState(State::sleeping);
            m_readBuffer.clear();
        }
        break;
    default:
        m_port->readAll();
        break;
    }
}

void TimeInfoReaderDriver::timerShot()
{
    switch (m_state) {

    case State::awaitingSend:
        m_readBuffer.clear();
        sendRequest();
        setState(State::awaitingResponse);
        break;

    case State::awaitingResponse:
    case State::sleeping:
        m_readBuffer.clear();
        setState(State::awaitingSend);
        break;
    case State::error:
        open();
        break;
    default:
        break;
    }
}

void TimeInfoReaderDriver::onError(QSerialPort::SerialPortError serialPortError)
{
    if (isOpen && serialPortError!=QSerialPort::QSerialPort::NoError) {
        m_port->close();
        emit error({
            {"reader", m_readerId},
            {"error", m_port->errorString()}
        });
        setState(State::error);
    }
}

void TimeInfoReaderDriver::setPortName(const QString &name)
{
    m_portName = name;
}

void TimeInfoReaderDriver::setBaudRate(QSerialPort::BaudRate baudRate)
{
    m_baudRate = baudRate;
}

void TimeInfoReaderDriver::setDataBits(QSerialPort::DataBits dataBits)
{
    m_dataBits = dataBits;
}

void TimeInfoReaderDriver::setParity(QSerialPort::Parity parity)
{
    m_parity = parity;
}

void TimeInfoReaderDriver::setStopBits(QSerialPort::StopBits stopBits)
{
    m_stopBits = stopBits;
}

void TimeInfoReaderDriver::setFlowControl(QSerialPort::FlowControl flowControl)
{
    m_flowControl = flowControl;
}

void TimeInfoReaderDriver::setReaderId(const QString &id)
{
    m_readerId = id;
}
