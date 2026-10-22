// ============================================================================
// mywidget.cpp —— MyWidget 類別的「實作」
//
// 整體運作流程：
//   1. main() 建立 MyWidget → 執行建構子：建出 UI、設定表格 4 個欄位標題
//   2. 使用者在 4 個 lineEdit 輸入資料，按「新增」→ on_pushButton_clicked()   → 資料加到表格最後一列
//   3. 按「匯出」→ on_pushButton_2_clicked() → 把表格每一列組成 "a,b,c,d\n" → 呼叫 Write() 存到 CSV
//   4. 按「匯入」→ on_pushButton_3_clicked() → 選檔 → 呼叫 Read() 讀出整個字串 → 依換行、逗號切開 → 填回表格
//   5. 關閉視窗 → 執行解構子 → delete ui
// ============================================================================

#include "mywidget.h"       // 引入自己的標頭檔（MyWidget 類別宣告）
#include "ui_mywidget.h"    // 引入 uic 根據 mywidget.ui 自動產生的標頭檔，裡面有 Ui::MyWidget 的完整定義
                            //   有了它才能使用 ui->setupUi()、ui->lineEdit、ui->tableWidget 等

#include <QFile>            // QFile：負責開啟 / 關閉檔案（檔案本身）
#include <QDebug>           // qDebug()：把除錯訊息輸出到 Qt Creator 的「應用程式輸出」視窗
#include <QTextStream>   // 講義沒寫，但要明確 include 才能用 QTextStream
                         //   QTextStream：以「文字」方式讀寫檔案（用 << 寫入、readAll() 讀取），類似 cout / cin
#include <QFileDialog>   // 新增：開檔對話框要用
                         //   QFileDialog：跳出系統的「開啟檔案」視窗讓使用者挑檔案

QString mFilename = "D:/test/myfile.csv";   // 存檔路徑（D:/test 資料夾要先存在）
                                            //   全域變數，匯出時固定寫到這個路徑
                                            //   ※ Qt 路徑用 "/"；若用反斜線要寫成 "D:\\test\\myfile.csv"

// 自定義 Write：把字串寫進 txt 檔
//   參數 Filename：要寫入的檔案路徑；str：要寫入的完整內容
void Write(QString Filename, QString str)
{
    QFile mFile(Filename);   // 宣告 QFile
                             //   建立一個 QFile 物件並指定檔名（此時「還沒」真的開啟檔案）
    // 寫入檔案判斷
    if (!mFile.open(QFile::WriteOnly | QFile::Text)) {   // 嘗試開啟檔案：
                                                         //   WriteOnly = 只寫模式（檔案不存在會自動建立；已存在會「清空覆蓋」）
                                                         //   Text      = 文字模式（Windows 下會自動把 \n 轉成 \r\n）
                                                         //   用 | (位元 OR) 把兩個旗標合併
                                                         //   open() 成功回傳 true；前面加 ! → 開檔失敗時才進入 if
        qDebug() << "could not open file for write";     // 開檔失敗（例如 D:/test 資料夾不存在）→ 印出錯誤訊息
        return;                                          // 直接結束函式，不往下寫
    }

    QTextStream out(&mFile); // 建立文字輸出串流 out，綁定到 mFile（傳入位址 &mFile）
    out << str;          // 將內容輸出至 txt 檔
                         //   用法跟 cout << 一樣，把 str 整串寫進檔案
    mFile.flush();       // 刷新 mFile
                         //   把緩衝區 (buffer) 中尚未寫出的資料強制寫入磁碟
    mFile.close();       // 關閉 mFile
                         //   用完一定要關閉，釋放檔案資源（其他程式才能開啟這個檔案）
}

// 自定義 Read：把 txt 檔整個讀成字串
//   參數 Filename：要讀取的檔案路徑；回傳值：檔案的全部內容（失敗回傳空字串）
QString Read(QString Filename)
{
    QFile mFile(Filename);   // 建立 QFile 物件並指定檔名
    if (!mFile.open(QFile::ReadOnly | QFile::Text)) {   // 改成唯讀
                                                        //   ReadOnly = 只讀模式（檔案不存在就會開檔失敗）
                                                        //   Text     = 文字模式（讀入時把 \r\n 轉回 \n）
        qDebug() << "could not open file for read";     // 開檔失敗 → 印出錯誤訊息
        return "";                                      // 回傳空字串，呼叫端可用 isEmpty() 判斷失敗
    }

    QTextStream in(&mFile);        // 建立文字輸入串流 in，綁定到 mFile
    QString text = in.readAll();   // 一次讀完全部內容
                                   //   例如檔案內容為 "001,A,小明,0912\n002,B,小華,0922\n"，就整串存進 text
    mFile.close();                 // 讀完關閉檔案
    return text;                   // 把讀到的內容回傳給呼叫者
}

// ---------------------------------------------------------------------------
// 建構子：MyWidget 物件被建立時（main.cpp 的 MyWidget w;）自動執行
// ---------------------------------------------------------------------------
MyWidget::MyWidget(QWidget *parent)   // MyWidget:: 表示這是 MyWidget 類別的成員函式
    : QWidget(parent),                // 成員初始化列表：先呼叫父類別 QWidget 的建構子，把 parent 傳上去
    ui(new Ui::MyWidget)              //   並 new 一個 Ui::MyWidget 物件，位址存到成員 ui
{
    ui->setupUi(this);      // ★ 依照 mywidget.ui 的設計，把所有元件（輸入框、按鈕、表格）建立出來並放到「this（本視窗）」上
                            //   同時自動連結 on_xxx_clicked() 這類 slot 與按鈕的 clicked 訊號

    QStringList ColTotle;   // ColTotle << "學號" << "班級" << "姓名" << "電話";
                            //   QStringList = 字串清單（多個 QString 的串列），用來存放欄位標題

    ui->tableWidget->setColumnCount(4);   // 設定欄位
                                          //   表格設為 4 欄（學號、班級、姓名、電話）
    ColTotle << QStringLiteral("學號") << QStringLiteral("班級")      // 用 << 依序把標題加入清單
             << QStringLiteral("姓名") << QStringLiteral("電話");     //   QStringLiteral()：在編譯期就建立 QString，
                                                                     //   中文字串用它比較不會出現亂碼，效率也較好
    ui->tableWidget->setHorizontalHeaderLabels(ColTotle);   // 把清單設為表格「水平表頭」（最上面一排的欄位名稱）
}

// ---------------------------------------------------------------------------
// 解構子：視窗物件被銷毀時（main 結束、w 離開作用域）自動執行
// ---------------------------------------------------------------------------
MyWidget::~MyWidget()
{
    delete ui;              // 釋放建構子裡 new 出來的 ui 物件，避免記憶體洩漏 (memory leak)
                            //   ※ 有 new 就要有 delete
}

// 新增按鈕
//   觸發時機：使用者點擊 pushButton（自動連結）
//   功能：讀取 4 個 lineEdit 的文字 → 在表格最後新增一列 → 填入資料 → 清空輸入框
void MyWidget::on_pushButton_clicked()
{
    QTableWidgetItem *inputRow1, *inputRow2, *inputRow3, *inputRow4;   // 宣告 4 個「表格儲存格」指標，各對應一欄
                                                                       //   ※ 每個指標前都要有 *，否則只有第一個是指標

    inputRow1 = new QTableWidgetItem(QString(ui->lineEdit->text()));     // ui->lineEdit->text() 取得「學號」輸入框文字，
                                                                         //   用它 new 出一個儲存格物件
    inputRow2 = new QTableWidgetItem(QString(ui->lineEdit_2->text()));   // 取得「班級」輸入框文字 → 建立儲存格
    inputRow3 = new QTableWidgetItem(QString(ui->lineEdit_3->text()));   // 取得「姓名」輸入框文字 → 建立儲存格
    inputRow4 = new QTableWidgetItem(QString(ui->lineEdit_4->text()));   // 取得「電話」輸入框文字 → 建立儲存格
                                                                         //   ※ 這些 item 交給表格後，由表格負責 delete，不用自己釋放

    // 在最後面加入新的一列
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());   // rowCount() = 目前列數（例如 3 列 → 索引 0~2）
                                                               //   insertRow(3) 就是在最後面插入第 4 列（索引 3）

    // 新的一列索引是 rowCount() - 1
    //   插入後列數變成 4，最後一列索引 = 4 - 1 = 3
    //   setItem(列, 欄, 儲存格)：把儲存格放到指定位置
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, inputRow1);   // 第 0 欄：學號
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, inputRow2);   // 第 1 欄：班級
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, inputRow3);   // 第 2 欄：姓名
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, inputRow4);   // 第 3 欄：電話

    // 清除 lineEdit
    //   把 4 個輸入框設成空字串，方便使用者輸入下一筆
    ui->lineEdit->setText("");     // 清空學號
    ui->lineEdit_2->setText("");   // 清空班級
    ui->lineEdit_3->setText("");   // 清空姓名
    ui->lineEdit_4->setText("");   // 清空電話
}

// 匯出按鈕
//   觸發時機：使用者點擊 pushButton_2
//   功能：逐列逐欄讀出表格內容，組成 CSV 格式字串，寫入 mFilename
//   CSV 格式：同一列的欄位用逗號隔開，每列用換行結尾，例如
//       001,A,小明,0912345678
//       002,B,小華,0922345678
void MyWidget::on_pushButton_2_clicked()
{
    QString saveFile = "";   // 存檔內容
                             //   用來累積整個檔案要寫入的文字，一開始是空字串

    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {   // 外層迴圈：i 走過每一「列」（0 ~ 列數-1）
        QStringList fields;   // 這一列的所有欄位
                              //   每換一列就重新宣告一個空清單

        for (int j = 0; j < ui->tableWidget->columnCount(); j++) {   // 內層迴圈：j 走過這一列的每一「欄」（0 ~ 3）
            QTableWidgetItem *item = ui->tableWidget->item(i, j);    // 取得第 i 列第 j 欄的儲存格指標
                                                                     //   ※ 若該格從沒設定過內容，會回傳 nullptr
            fields << (item ? item->text() : "");   // 空儲存格當成空字串
                                                    //   三元運算子：item 不是 nullptr → 取它的文字；否則放 ""
                                                    //   （直接對 nullptr 呼叫 text() 會讓程式當掉）
        }

        saveFile += fields.join(",") + "\n";   // 逗號只放在欄位之間
                                               //   join(",") 把清單用逗號串起來：["001","A","小明","0912"] → "001,A,小明,0912"
                                               //   再加上換行 "\n"，接到 saveFile 後面
    }
    Write(mFilename, saveFile);   // 呼叫 Write
                                  //   全部列都處理完後，一次把整個字串寫進 D:/test/myfile.csv
}

// 匯入按鈕
//   觸發時機：使用者點擊 pushButton_3
//   功能：讓使用者選 CSV 檔 → 讀出全部文字 → 清空表格 → 依「換行」切成多列、依「逗號」切成多欄 → 填入表格
void MyWidget::on_pushButton_3_clicked()
{
    // 開啟檔案對話框，讓使用者選擇要匯入的 .csv 檔
    //   getOpenFileName(父視窗, 對話框標題, 預設開啟的資料夾, 檔案類型篩選)
    //   篩選字串用 ";;" 分隔多個選項：「CSV 檔 (*.csv)」與「任何檔案 (*.*)」
    //   回傳值：使用者選到的檔案完整路徑；若按「取消」則回傳空字串
    QString fileName = QFileDialog::getOpenFileName(this,
        QStringLiteral("開啟 CSV 檔"), "D:/test",
        QStringLiteral("CSV 檔 (*.csv);;任何檔案 (*.*)"));
    if (fileName.isEmpty()) return;      // 使用者按了取消
                                         //   沒選檔案就直接結束，不做任何事

    QString text = Read(fileName);       // 改讀使用者選的檔案
                                         //   呼叫上面的 Read()，把整個檔案內容讀成一個字串
    if (text.isEmpty()) return;          // 檔案不存在或是空的
                                         //   沒有內容就結束，避免把表格清空卻沒東西可放
    ui->tableWidget->setRowCount(0);     // 先清空表格，避免重複匯入時資料疊兩份
                                         //   列數設為 0 = 刪除所有列（欄位標題仍保留）
    QStringList lines = text.split("\n", Qt::SkipEmptyParts);   // 用換行 "\n" 把整串文字切成「一行一個元素」的清單
                                                                //   Qt::SkipEmptyParts：略過空字串（例如檔案最後的空行）
                                                                //   例："001,A,小明,0912\n002,B,小華,0922\n"
                                                                //     → ["001,A,小明,0912", "002,B,小華,0922"]

    for (const QString &line : lines) {          // 範圍 for 迴圈：依序取出每一行（const 參考 → 不複製、不修改）
        QStringList fields = line.split(",");    // 用逗號把一行切成多個欄位：
                                                 //   "001,A,小明,0912" → ["001", "A", "小明", "0912"]
        int row = ui->tableWidget->rowCount();   // 新列的索引 = 目前的列數（例如目前 0 列 → 新列索引 0）
        ui->tableWidget->insertRow(row);         // 在表格最後插入一列空白列

        for (int j = 0; j < ui->tableWidget->columnCount() && j < fields.size(); j++) {   // j 走過每一欄
                                                                                          //   兩個條件都要成立：
                                                                                          //   j < 表格欄數 (4)    → 欄位太多時多的忽略
                                                                                          //   j < 這行的欄位數    → 欄位太少時不會超出清單範圍而當掉
            ui->tableWidget->setItem(row, j, new QTableWidgetItem(fields[j]));   // 用第 j 個欄位文字建立儲存格，
                                                                                  //   放到第 row 列第 j 欄
        }
    }
}
