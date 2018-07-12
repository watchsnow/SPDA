#include "SPDA.h"
#include "ui_SPDA.h"
#include <QKeyEvent>
#include <QIcon>
#include <QStyledItemDelegate>

QSPDA::QSPDA(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QSPDA)
{
    ui->setupUi(this);
    //查找可用的串口
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            ui->m_comboBox_portBox->addItem(serial.portName());
            serial.close();
        }
    }
    initUI();

    qDebug() << QStringLiteral("界面设定成功！");
}


/*******************************************
 * 功能：设置窗口背景
 * 参数：无
 * 返回值：无
 ******************************************/
void QSPDA::setBackground()
{
    QImage t_image;
    t_image.load(":/img/content-bg.gif");
    setAutoFillBackground(true);
    QPalette t_pal(palette());
    t_pal.setBrush(backgroundRole(),QBrush(t_image.scaled(this->size())));
    setPalette(t_pal);
}

/*******************************************
 * 功能：窗口大小改变的时候重绘背景
 * 参数：无
 * 返回值：无
 ******************************************/
void QSPDA::resizeEvent(QResizeEvent *)
{
    setBackground();
}

/*******************************************
 * 功能：为了方便以后调用，写一个静态加载样式的函数
 * 参数：无
 * 返回值：无
 ******************************************/
void QSPDA::setStyle(const QString &style)
{
    QFile t_qss(style);
    t_qss.open(QFile::ReadOnly);
    qApp->setStyleSheet(t_qss.readAll());
    t_qss.close();
}


QSPDA::~QSPDA()
{
    delete ui;
}

bool QSPDA::isOpen() const
{
    if(serial->isOpen())
    {
        return true;
    }

    else
    {
        return false;
    }
}

void QSPDA::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Return:    //数字键盘的Enter键
    case Qt::Key_Enter:     //Enter键
        on_m_pushButton_send_clicked();
        break;
    }
}

/*******************************************
 * 功能：初始化主界面
 * 参数：无
 * 返回值：无
 ******************************************/
void QSPDA::initUI()
{
    setStyle(":/qss/stylesheet.qss");
    ui->m_textEdit_dataReceivingList->setReadOnly(true);
    setBackground();
    this->setWindowIcon(QIcon(":/main.ico"));
    this->setWindowTitle("串口调试助手");

    // 设置代理, 方便设置QComboBox列表项高度
    QStyledItemDelegate *delegate = new QStyledItemDelegate(this);
    ui->m_comboBox_portBox->setItemDelegate(delegate);

    ui->m_comboBox_baudBox->setItemDelegate(delegate);
    ui->m_comboBox_baudBox->addItem("9600");
    ui->m_comboBox_baudBox->addItem("19200");
    ui->m_comboBox_baudBox->addItem("38400");
    ui->m_comboBox_baudBox->addItem("57600");
    ui->m_comboBox_baudBox->addItem("115200");
    ui->m_comboBox_baudBox->addItem("Custom");

    ui->m_comboBox_parityBox->setItemDelegate(delegate);
    ui->m_comboBox_parityBox->addItem("None");
    ui->m_comboBox_parityBox->addItem("Even");
    ui->m_comboBox_parityBox->addItem("Odd");
    ui->m_comboBox_parityBox->addItem("Mark");
    ui->m_comboBox_parityBox->addItem("Space");

    ui->m_comboBox_stopBox->setItemDelegate(delegate);
    ui->m_comboBox_stopBox->addItem("1");
    ui->m_comboBox_stopBox->addItem("1.5");
    ui->m_comboBox_stopBox->addItem("2");

    ui->m_comboBox_bitNumBox->setItemDelegate(delegate);
    ui->m_comboBox_bitNumBox->addItem("5");
    ui->m_comboBox_bitNumBox->addItem("6");
    ui->m_comboBox_bitNumBox->addItem("7");
    ui->m_comboBox_bitNumBox->addItem("8");

    ui->m_comboBox_flowControl->setItemDelegate(delegate);
    ui->m_comboBox_flowControl->addItem("None");
    ui->m_comboBox_flowControl->addItem("RTS/CTS");
    ui->m_comboBox_flowControl->addItem("XON/XOFF");

    //设置波特率下拉菜单默认显示第三项
    ui->m_comboBox_portBox->setCurrentIndex(0);
    ui->m_comboBox_baudBox->setCurrentIndex(0);
    ui->m_comboBox_parityBox->setCurrentIndex(0);
    ui->m_comboBox_stopBox->setCurrentIndex(0);
    ui->m_comboBox_bitNumBox->setCurrentIndex(0);
    ui->m_comboBox_flowControl->setCurrentIndex(0);

    //关闭发送按钮的使能
    ui->m_pushButton_send->setEnabled(false);
}

void QSPDA::on_m_pushButton_open_clicked()
{
    if(ui->m_pushButton_open->text()==tr("打开串口"))
    {
        serial = new QSerialPort;
        //设置串口名
        serial->setPortName(ui->m_comboBox_portBox->currentText());
        //打开串口
        if(serial->open(QIODevice::ReadWrite))
        {
            qDebug() << "打开串口成功";
        }
        else
        {
            qDebug() << "打开串口失败";
            ui->m_pushButton_open->setText(tr("打开串口"));
            return;
        }
        //设置波特率
        serial->setBaudRate(ui->m_comboBox_portBox->currentText().toInt());
        //设置数据位数
        switch(ui->m_comboBox_bitNumBox->currentIndex())
        {
        case 0: serial->setDataBits(QSerialPort::Data5); break;
        case 1: serial->setDataBits(QSerialPort::Data6); break;
        case 2: serial->setDataBits(QSerialPort::Data7); break;
        case 3: serial->setDataBits(QSerialPort::Data8); break;
        default: break;
        }
        //设置奇偶校验
        switch(ui->m_comboBox_parityBox->currentIndex())
        {
        case 0: serial->setParity(QSerialPort::NoParity);    break;
        case 1: serial->setParity(QSerialPort::EvenParity);  break;
        case 2: serial->setParity(QSerialPort::OddParity);   break;
        case 3: serial->setParity(QSerialPort::MarkParity);  break;
        case 4: serial->setParity(QSerialPort::SpaceParity); break;
        default: break;
        }
        //设置停止位
        switch(ui->m_comboBox_stopBox->currentIndex())
        {
        case 0: serial->setStopBits(QSerialPort::OneStop);        break;
        case 1: serial->setStopBits(QSerialPort::OneAndHalfStop); break;
        case 2: serial->setStopBits(QSerialPort::TwoStop);        break;
        default: break;
        }
        //设置流控制
        switch(ui->m_comboBox_flowControl->currentIndex())
        {
        case 0:serial->setFlowControl(QSerialPort::NoFlowControl);break;
        case 1:serial->setFlowControl(QSerialPort::HardwareControl);break;
        case 2:serial->setFlowControl(QSerialPort::SoftwareControl);break;
        default: break;
        }
        //关闭设置菜单使能
        ui->m_comboBox_portBox->setEnabled(false);
        ui->m_comboBox_portBox->setEnabled(false);
        ui->m_comboBox_bitNumBox->setEnabled(false);
        ui->m_comboBox_parityBox->setEnabled(false);
        ui->m_comboBox_stopBox->setEnabled(false);
        ui->m_comboBox_baudBox->setEnabled(false);
        ui->m_comboBox_flowControl->setEnabled(false);
        ui->m_pushButton_open->setText(tr("关闭串口"));
        ui->m_pushButton_send->setEnabled(true);
        //连接信号槽
        QObject::connect(serial, &QSerialPort::readyRead, this, &QSPDA::onReadData);
    }
    else
    {
        //关闭串口
        serial->clear();
        if(serial->isOpen())
        {
            serial->close();
            qDebug() << "关闭串口成功";
        }
        serial->deleteLater();
        //恢复设置使能
        ui->m_comboBox_portBox->setEnabled(true);
        ui->m_comboBox_portBox->setEnabled(true);
        ui->m_comboBox_bitNumBox->setEnabled(true);
        ui->m_comboBox_parityBox->setEnabled(true);
        ui->m_comboBox_stopBox->setEnabled(true);
        ui->m_comboBox_baudBox->setEnabled(true);
        ui->m_pushButton_send->setEnabled(false);
        ui->m_comboBox_flowControl->setEnabled(true);
        ui->m_pushButton_open->setText(tr("打开串口"));
    }
}

//清空接受窗口
void QSPDA::on_m_pushButton_clear_clicked()
{
    ui->m_textEdit_dataReceivingList->clear();
}

//发送数据
void QSPDA::on_m_pushButton_send_clicked()
{
    serial->write(ui->m_textEdit_dataSendBox->toPlainText().toUtf8());
    ui->m_textEdit_dataSendBox->clear();
    ui->m_textEdit_dataSendBox->setFocus();
}

//读取接收到的数据
void QSPDA::onReadData()
{
    QByteArray buf;
    buf = serial->readAll();
    if(!buf.isEmpty())
    {
        QString str = ui->m_textEdit_dataReceivingList->toPlainText();
        str+= QString::fromUtf8(buf);
        ui->m_textEdit_dataReceivingList->clear();
        ui->m_textEdit_dataReceivingList->append(str);
        qDebug()<<str;
    }
    buf.clear();
}

void QSPDA::on_m_comboBox_baudBox_activated(const QString &arg1)
{
    Q_UNUSED(arg1)
    if(ui->m_comboBox_baudBox->currentText() == "Custom")
    {
        ui->m_comboBox_baudBox->setEditable(true);
        ui->m_comboBox_baudBox->setCurrentText("");
    }
    else
    {
        ui->m_comboBox_baudBox->setEditable(false);
    }
}
