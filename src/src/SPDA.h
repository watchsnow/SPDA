#ifndef SPDA_H
#define SPDA_H

#include <QWidget>
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

namespace Ui {
class QSPDA;
}

class QSPDA : public QWidget
{
    Q_OBJECT

public:
    explicit QSPDA(QWidget *parent = 0);
    ~QSPDA();

public:
    bool isOpen() const;
    void keyPressEvent(QKeyEvent *event);
    void setBackground();
    void resizeEvent(QResizeEvent *);
    static void setStyle(const QString &style);

protected:
    void initUI();//初始化主界面

private slots:
    void onReadData();

    void on_m_pushButton_open_clicked();

    void on_m_pushButton_clear_clicked();

    void on_m_pushButton_send_clicked();

    void on_m_comboBox_baudBox_activated(const QString &arg1);

private:
    Ui::QSPDA *ui;
    QSerialPort *serial;
};

#endif // SPDA_H
