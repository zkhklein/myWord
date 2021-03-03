#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"mychildwnd.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)

{
    ui->setupUi(this);
      initMainWindow();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_aboutAction_triggered()
{
    QMessageBox::about(this,tr("关于"),tr("这是一个基于QT5实现的字处理软件\r\n具备类似 微软 Office Word 的功能。"));
}

void MainWindow::on_aboutQtAction_triggered()
{
    QMessageBox::aboutQt(NULL,"关于 QT 5");
}

void MainWindow::initMainWindow(){
    QFontDatabase fontdb;
    foreach(int fontsize, fontdb.standardSizes()) ui->sizeComboBox->addItem(QString::number(fontsize));
    ui->sizeComboBox->setCurrentIndex(ui->sizeComboBox->findText(QString::number(QApplication::font().pointSize())));

   ui->mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  ui->mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

  refreshMenus();
  connect(ui->mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(refreshMenus()));

  myWndMapper = new QSignalMapper(this);
  connect(myWndMapper,SIGNAL(mapped(QWidget*)), this,SLOT(setActiveSubWindow(QWidget*)));
  addSubWndListMenu();
  connect(ui->menu_W,SIGNAL(aboutToShow()), this, SLOT(addSubWndListMenu()));
}

void MainWindow::docNew()
{
    MyChildWnd *childWnd = new MyChildWnd;
    ui->mdiArea->addSubWindow(childWnd);

    connect(childWnd,SIGNAL(copyAvailable(bool)),ui->copyAction,SLOT(setEnabled(bool)));
    connect(childWnd,SIGNAL(copyAvailable(bool)),ui->cutAction,SLOT(setEnabled(bool)));
    childWnd->newDoc();
    childWnd->show();
    formatEnabled();
}

void MainWindow::docOpen()
{
    QString docName = QFileDialog::getOpenFileName(this,tr("打开"),
                                                   QString("/"),tr("HTML 文档 (*.htm *.html);;所有文件(*.*)"));
    if(!docName.isEmpty()){
       QMdiSubWindow *exist = findChildWnd(docName);
       if(exist){
           ui->mdiArea->setActiveSubWindow(exist);
           return;
       }
       MyChildWnd * childwnd = new MyChildWnd();
       ui->mdiArea->addSubWindow(childwnd);
       connect(childwnd,SIGNAL(copyAvailable(bool)),ui->cutAction,SLOT(setEnabled(bool)));
       connect(childwnd,SIGNAL(copyAvailable(bool)),ui->copyAction,SLOT(setEnabled(bool)));
       if(childwnd->loadDoc(docName)){
           statusBar()->showMessage(tr("文档已打开"),2000);
           childwnd->show();
           formatEnabled();
       }else{
           childwnd->close();
       }

    }
}

void MainWindow::docSave()
{
    if(activateChildWnd() && activateChildWnd()->saveDoc())
        statusBar()->showMessage(tr("保存成功"),2000);
}

void MainWindow::docSaveAs()
{
    if(activateChildWnd() && activateChildWnd()->SaveAsDoc())
        statusBar()->showMessage(tr("保存成功"),2000);
}

void MainWindow::docUndo()
{
    if(activateChildWnd()) activateChildWnd()->undo();
}

void MainWindow::docRedo()
{
    if(activateChildWnd()) activateChildWnd()->redo();
}

void MainWindow::docCut()
{
    if(activateChildWnd()) activateChildWnd()->cut();
}

void MainWindow::docCopy()
{
    if(activateChildWnd()) activateChildWnd()->copy();
}

void MainWindow::formatEnabled(){
    ui->boldAction->setEnabled(true);
    ui->italicAction->setEnabled(true);
      ui->underlineAction->setEnabled(true);
      ui->leftAlignAction->setEnabled(true);
        ui->centerAction->setEnabled(true);
        ui->rightAlignAction->setEnabled(true);
        ui->justifyAction->setEnabled(true);
        ui->colorAction->setEnabled(true);


}

MyChildWnd *MainWindow::activateChildWnd()
{
    if(QMdiSubWindow *actSubWnd = ui->mdiArea->activeSubWindow())
        return qobject_cast<MyChildWnd *>(actSubWnd->widget());
    else
        return 0;

}

QMdiSubWindow *MainWindow::findChildWnd(const QString &docName)
{
     QString canonicalFilePath = QFileInfo(docName).canonicalFilePath();
    foreach(QMdiSubWindow *wnd,ui->mdiArea->subWindowList())
   {
    MyChildWnd *childwnd = qobject_cast<MyChildWnd*>(wnd->widget());
    if(childwnd->myCurDocPath == canonicalFilePath) return wnd;
      }
     return 0;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    ui->mdiArea->closeAllSubWindows();
    if(ui->mdiArea->currentSubWindow())
    {
        event->ignore();
    }else{
        event->accept();
    }
}



void MainWindow::on_newAction_triggered(){
    docNew();
}

void MainWindow::refreshMenus()
{
    bool hasChild = (activateChildWnd() != 0);
    ui->saveAction->setEnabled(hasChild);
    ui->saveAsAction->setEnabled(hasChild);
    ui->printAction->setEnabled(hasChild);
    ui->printPreviewAction->setEnabled(hasChild);
    ui->pasteAction->setEnabled(hasChild);
    ui->closeAction->setEnabled(hasChild);
    ui->closeAllAction->setEnabled(hasChild);
    ui->tileAction->setEnabled(hasChild);
    ui->cascadeAction->setEnabled(hasChild);
    ui->nextAction->setEnabled(hasChild);
    ui->previousAction->setEnabled(hasChild);

    bool hasSelect = (activateChildWnd() && activateChildWnd()->textCursor().hasSelection());
    ui->cutAction->setEnabled(hasSelect);
    ui->copyAction->setEnabled(hasSelect);
    ui->boldAction->setEnabled(hasSelect);
    ui->italicAction->setEnabled(hasSelect);
    ui->underlineAction->setEnabled(hasSelect);
    ui->leftAlignAction->setEnabled(hasSelect);
    ui->centerAction->setEnabled(hasSelect);
    ui->rightAlignAction->setEnabled(hasSelect);
    ui->justifyAction->setEnabled(hasSelect);
    ui->colorAction->setEnabled(hasSelect);
}

void MainWindow::addSubWndListMenu()
{
    ui->menu_W->clear();
    ui->menu_W->addAction(ui->closeAction);
    ui->menu_W->addAction(ui->closeAllAction);
    ui->menu_W->addSeparator();
    ui->menu_W->addAction(ui->tileAction);
    ui->menu_W->addAction(ui->cascadeAction);
    ui->menu_W->addSeparator();
    ui->menu_W->addAction(ui->nextAction);
    ui->menu_W->addAction(ui->previousAction);
      QList<QMdiSubWindow*> wnds =ui->mdiArea->subWindowList();
      if(!wnds.isEmpty())  ui->menu_W->addSeparator();


      for(int i=0; i<wnds.size();++i){
          MyChildWnd *childwnd = qobject_cast<MyChildWnd*>(wnds.at(i)->widget());
          QString menuitem_text;
          if(i<9){
              menuitem_text = tr("&%1 %2").arg(i+1).arg(childwnd->getCurDocName());
          }else
          {
              menuitem_text= tr("%1 %2").arg(i+1).arg(childwnd->getCurDocName());
          }
          QAction *menuitem_act = ui->menu_W->addAction(menuitem_text);
          menuitem_act->setCheckable(true);
          menuitem_act->setChecked(childwnd == activateChildWnd());

          connect(menuitem_act,SIGNAL(triggered()),myWndMapper, SLOT(map()));
          myWndMapper->setMapping(menuitem_act,wnds.at(i));
      }
      formatEnabled();
}

void MainWindow::on_closeAction_triggered()
{
    ui->mdiArea->closeActiveSubWindow();
}

void MainWindow::on_closeAllAction_triggered()
{
    ui->mdiArea->closeAllSubWindows();
}

void MainWindow::on_tileAction_triggered()
{
    ui->mdiArea->tileSubWindows();
}

void MainWindow::on_cascadeAction_triggered()
{
    ui->mdiArea->cascadeSubWindows();
}

void MainWindow::on_nextAction_triggered()
{
    ui->mdiArea->activateNextSubWindow();
}

void MainWindow::on_previousAction_triggered()
{
    ui->mdiArea->activatePreviousSubWindow();
}

void MainWindow::on_openAction_triggered()
{
    docOpen();
}

void MainWindow::on_saveAction_triggered()
{
    docSave();
}

void MainWindow::on_saveAsAction_triggered()
{
    docSaveAs();
}


