#include "mywidget.h"
#include "ui_mywidget.h"

#include <QFile>
#include <QDebug>
#include <QTextStream>   // 講義沒寫，但要明確 include 才能用 QTextStream

QString mFilename = "D:/test/myfile.csv";   // 存檔路徑（D:/test 資料夾要先存在）

// 自定義 Write：把字串寫進 txt 檔
void Write(QString Filename, QString str)
{
    QFile mFile(Filename);   // 宣告 QFile
    // 寫入檔案判斷
    if (!mFile.open(QFile::WriteOnly | QFile::Text)) {
        qDebug() << "could not open file for write";
        return;
    }

    QTextStream out(&mFile);
    out << str;          // 將內容輸出至 txt 檔
    mFile.flush();       // 刷新 mFile
    mFile.close();       // 關閉 mFile
}

// 自定義 Read：把 txt 檔整個讀成字串
QString Read(QString Filename)
{
    QFile mFile(Filename);
    if (!mFile.open(QFile::ReadOnly | QFile::Text)) {   // 改成唯讀
        qDebug() << "could not open file for read";
        return "";
    }

    QTextStream in(&mFile);
    QString text = in.readAll();   // 一次讀完全部內容
    mFile.close();
    return text;
}

MyWidget::MyWidget(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::MyWidget)
{
    ui->setupUi(this);

    QStringList ColTotle;   // ColTotle << "學號" << "班級" << "姓名" << "電話";

    ui->tableWidget->setColumnCount(4);   // 設定欄位
    ColTotle << QStringLiteral("學號") << QStringLiteral("班級")
             << QStringLiteral("姓名") << QStringLiteral("電話");
    ui->tableWidget->setHorizontalHeaderLabels(ColTotle);
}

MyWidget::~MyWidget()
{
    delete ui;
}

// 新增按鈕
void MyWidget::on_pushButton_clicked()
{
    QTableWidgetItem *inputRow1, *inputRow2, *inputRow3, *inputRow4;

    inputRow1 = new QTableWidgetItem(QString(ui->lineEdit->text()));
    inputRow2 = new QTableWidgetItem(QString(ui->lineEdit_2->text()));
    inputRow3 = new QTableWidgetItem(QString(ui->lineEdit_3->text()));
    inputRow4 = new QTableWidgetItem(QString(ui->lineEdit_4->text()));

    // 在最後面加入新的一列
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());

    // 新的一列索引是 rowCount() - 1
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, inputRow1);
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, inputRow2);
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, inputRow3);
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, inputRow4);

    // 清除 lineEdit
    ui->lineEdit->setText("");
    ui->lineEdit_2->setText("");
    ui->lineEdit_3->setText("");
    ui->lineEdit_4->setText("");
}

// 匯出按鈕
void MyWidget::on_pushButton_2_clicked()
{
    QString saveFile = "";   // 存檔內容

    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
        QStringList fields;   // 這一列的所有欄位

        for (int j = 0; j < ui->tableWidget->columnCount(); j++) {
            QTableWidgetItem *item = ui->tableWidget->item(i, j);
            fields << (item ? item->text() : "");   // 空儲存格當成空字串
        }

        saveFile += fields.join(",") + "\n";   // 逗號只放在欄位之間
    }
    Write(mFilename, saveFile);   // 呼叫 Write
}

// 匯入按鈕
void MyWidget::on_pushButton_3_clicked()
{
    QString text = Read(mFilename);
    if (text.isEmpty()) return;          // 檔案不存在或是空的
    ui->tableWidget->setRowCount(0);     // 先清空表格，避免重複匯入時資料疊兩份
    // 先依換行切成一列一列；最後一行結尾的 \n 會多切出一個空字串，用 SkipEmptyParts 丟掉
    QStringList lines = text.split("\n", Qt::SkipEmptyParts);

    for (const QString &line : lines) {
        QStringList fields = line.split(",");   // 再依逗號切成欄位
        int row = ui->tableWidget->rowCount();  // 新列的索引
        ui->tableWidget->insertRow(row);

        for (int j = 0; j < ui->tableWidget->columnCount() && j < fields.size(); j++) {
            ui->tableWidget->setItem(row, j, new QTableWidgetItem(fields[j]));
        }
    }
}
