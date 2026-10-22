// ============================================================================
// mywidget.h —— MyWidget 類別的「宣告」（只說有哪些成員，實作寫在 mywidget.cpp）
// ============================================================================

#ifndef MYWIDGET_H          // Include Guard（引入防護）：如果還沒定義過 MYWIDGET_H …
#define MYWIDGET_H          // … 就定義它；之後同一個檔案再被 #include 時會被跳過，避免「重複定義」錯誤

#include <QWidget>          // 引入 QWidget：所有視窗元件的基底類別，MyWidget 要繼承它

QT_BEGIN_NAMESPACE          // Qt 巨集：若 Qt 編譯時有使用命名空間，這裡會展開成 namespace XXX {（一般情況下是空的）
namespace Ui { class MyWidget; }   // 「前置宣告」Ui::MyWidget 類別
                                   //   Ui::MyWidget 是 Qt 的 uic 工具根據 mywidget.ui 自動產生的類別（在 ui_mywidget.h）
                                   //   裡面包含畫面上所有元件：lineEdit、pushButton、tableWidget…
                                   //   這裡只先宣告名字，因為下面只用到「指標」，不需要知道完整內容 → 可加快編譯
QT_END_NAMESPACE            // 對應 QT_BEGIN_NAMESPACE 的結尾

class MyWidget : public QWidget    // 定義 MyWidget 類別，以 public 方式繼承 QWidget → MyWidget 本身就是一個視窗
{
    Q_OBJECT                // Qt 巨集：讓這個類別支援「訊號與槽 (signals & slots)」機制
                            //   有它，moc（Meta-Object Compiler）才會產生額外程式碼，使按鈕點擊能連到下面的 slots
                            //   ※ 只要類別有用到 signals/slots，就一定要寫 Q_OBJECT

public:                     // 公開成員：類別外部（例如 main.cpp）可以使用
    MyWidget(QWidget *parent = nullptr);   // 建構子：parent 是父視窗，預設 nullptr 代表它是獨立的最上層視窗
    ~MyWidget();                           // 解構子：視窗被銷毀時呼叫，用來 delete ui 釋放記憶體

private slots:              // 私有「槽函式」：可以被訊號 (signal) 觸發的函式
                            // ★ 自動連結 (auto-connect) 規則：函式名稱寫成 on_<物件名稱>_<訊號名稱>()
                            //   setupUi() 執行時會呼叫 QMetaObject::connectSlotsByName()，
                            //   自動把「pushButton 的 clicked 訊號」接到「on_pushButton_clicked()」，不用自己寫 connect()
    void on_pushButton_clicked();     // 新增：按下 pushButton   時呼叫 → 把 4 個輸入框的內容加到表格最後一列
    void on_pushButton_2_clicked();   // 匯出：按下 pushButton_2 時呼叫 → 把表格內容存成 CSV 檔
    void on_pushButton_3_clicked();   // 匯入：按下 pushButton_3 時呼叫 → 開檔對話框選 CSV，讀進表格

private:                    // 私有成員：只有 MyWidget 自己能用
    Ui::MyWidget *ui;       // 指向「UI 介面物件」的指標；透過 ui->元件名稱 存取畫面元件
                            //   例如 ui->lineEdit、ui->tableWidget
                            //   在建構子中 new 出來，在解構子中 delete
};                          // 類別定義結束（分號不能漏！）

#endif // MYWIDGET_H        // 對應最上面的 #ifndef，Include Guard 結束
