// ============================================================================
// main.cpp —— 程式進入點
// 執行流程：建立 QApplication → 建立主視窗 MyWidget → 顯示視窗 → 進入事件迴圈
// ============================================================================

#include "mywidget.h"       // 引入自訂視窗類別 MyWidget 的宣告，main 才認得 MyWidget

#include <QApplication>     // 引入 QApplication：每個 Qt 視窗程式都需要「剛好一個」QApplication 物件

int main(int argc, char *argv[])   // C++ 程式進入點；argc = 命令列參數個數，argv = 參數字串陣列
{
    QApplication a(argc, argv);    // 建立應用程式物件 a：初始化 Qt 環境（字型、樣式、事件系統等），並把命令列參數交給 Qt 處理
                                   // ※ 必須在建立任何視窗 (QWidget) 之前建立，否則程式會出錯

    MyWidget w;                    // 建立主視窗物件 w → 會呼叫 MyWidget 的建構子 (mywidget.cpp)
                                   //   建構子裡會執行 ui->setupUi(this) 把 .ui 檔設計的元件建出來，並設定表格欄位標題

    w.show();                      // 讓視窗顯示在螢幕上（視窗建立後預設是隱藏的，一定要呼叫 show()）

    return QApplication::exec();   // 進入「事件迴圈 (event loop)」：程式停在這裡不斷等待使用者操作（按鈕點擊、鍵盤輸入…）
                                   //   使用者按按鈕 → 發出 clicked() 訊號 → 自動呼叫對應的 slot（例如 on_pushButton_clicked）
                                   //   直到使用者關閉視窗，exec() 才會結束並回傳結束碼，main 再把它 return 給作業系統
}
