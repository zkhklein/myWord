#ifndef MYCHILDWND_H
#define MYCHILDWND_H

#include <QWidget>
#include <QTextEdit>
#include <QFileInfo>
#include <QTextCodec>
#include <QFileDialog>
#include <QTextDocumentWriter>
#include <QMessageBox>
#include <QCloseEvent>
#include<QtWidgets>

class MyChildWnd:public QTextEdit
{
    Q_OBJECT
public:
    MyChildWnd();
    void newDoc();
    QString getCurDocName();
    bool loadDoc(const QString &docName);
    bool SaveAsDoc();
    void setCurDoc(const QString& docname);
    bool saveDoc();
    bool saveDocOpt(QString docname);
public:
    QString myCurDocPath;

protected:
   void closeEvent(QCloseEvent *event);

private slots:
    void docBeModified();

private:
    bool beSaved;
private:
    bool promptSave();
};

#endif // MYCHILDWND_H
