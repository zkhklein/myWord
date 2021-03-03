#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QCloseEvent>

#include <QFontDatabase>
#include <QMessageBox>
#include<QDebug>
#include<QMenu>
#include<QMdiArea>

namespace Ui {
class MainWindow;
}

class MyChildWnd;
class QMdiSubWindow;
class QSignalMapper;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void initMainWindow();
    void docNew();
    void docOpen();
    void docSave();
    void docSaveAs();
    void docUndo();
    void docRedo();
    void docCut();
    void docCopy();
    void docPaste();
private slots:
    void on_aboutAction_triggered();
    void on_aboutQtAction_triggered();
    void on_newAction_triggered();
    void refreshMenus();
    void addSubWndListMenu();
    void on_closeAction_triggered();
    void on_closeAllAction_triggered();
    void on_tileAction_triggered();
    void on_cascadeAction_triggered();
    void on_nextAction_triggered();
    void on_previousAction_triggered();
    void on_openAction_triggered();
    void on_saveAction_triggered();
    void on_saveAsAction_triggered();


private:
    Ui::MainWindow *ui;
    QSignalMapper *myWndMapper;
private:
   void formatEnabled();
   MyChildWnd *activateChildWnd();
   QMdiSubWindow*findChildWnd(const QString& docname);
protected:
   void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
