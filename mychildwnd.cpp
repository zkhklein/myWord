#include "mychildwnd.h"

#include <QFileInfo>


MyChildWnd::MyChildWnd()
{
    setAttribute(Qt::WA_DeleteOnClose);
    beSaved = false;
}

void MyChildWnd::newDoc(){
    static int wndSeqNum = 1;
    myCurDocPath = tr("Word 文档 %1").arg(wndSeqNum++);
    setWindowTitle(myCurDocPath + "[*]" + tr(" - MyWord"));
    connect(document(),SIGNAL(contentsChanged()), this, SLOT(docBeModified()));

}

QString MyChildWnd::getCurDocName(){
    return QFileInfo(myCurDocPath).fileName();
}

bool MyChildWnd::loadDoc(const QString &docName)
{
    if(!docName.isEmpty()){
        if(!QFile::exists(docName)) return false;
        QFile doc(docName);
        if(!doc.open(QFile::ReadOnly)) return false;
        QByteArray text =doc.readAll();
        QTextCodec *text_codec = Qt::codecForHtml(text);
        QString str = text_codec->toUnicode(text);
        if(Qt::mightBeRichText(str)){
            this->setHtml(str);
        }else{
            str = QString::fromLocal8Bit(text);
            this->setPlainText(str);
        }
        setCurDoc(docName);
        connect(document(),SIGNAL(contentsChanged()),this, SLOT(docBeModified()));
        return true;
    }
}

bool MyChildWnd::SaveAsDoc()
{
    QString docName = QFileDialog::getSaveFileName(this,tr("另存为"),myCurDocPath,tr("HTML 文档 "
                                                                                  "(*.htm *.html);;所有文件(*.*)"));
    if(docName.isEmpty()) return false;
    else return saveDocOpt(docName);
}

void MyChildWnd::setCurDoc(const QString &docName)
{
    myCurDocPath = QFileInfo(docName).canonicalFilePath();
}

bool MyChildWnd::saveDoc()
{
    if(!beSaved) return SaveAsDoc();
    else return saveDocOpt(myCurDocPath);
}

bool MyChildWnd::saveDocOpt(QString docName)
{
    if(!(docName.endsWith(".htm", Qt::CaseInsensitive) || docName.endsWith(".html", Qt::CaseInsensitive)));
    {
      //  docName += ".html";
    }
    QTextDocumentWriter writer(docName);
    bool success = writer.write(this->document());
    if(success) setCurDoc(docName);
    return success;

}

void MyChildWnd::closeEvent(QCloseEvent *event)
{
    if(promptSave()){
        event->accept();
    }else{
        event->ignore();
    }

}

void MyChildWnd::docBeModified(){

    setWindowModified(document()->isModified());
    beSaved = true;
    document()->setModified(false);
    setWindowModified(false);
    setWindowTitle(getCurDocName()+"[*]");
}

bool MyChildWnd::promptSave()
{
    if(!document()->isModified()) return true;
    QMessageBox::StandardButton result;
    result = QMessageBox::warning(this, tr("MyWord"), tr("文档'%1'已被更改，保存吗？").arg(getCurDocName()),
                                  QMessageBox::Save | QMessageBox::Discard|QMessageBox::Cancel);
    if(result == QMessageBox::Save) return saveDoc();
    else if(result == QMessageBox::Cancel) return false;
    return true;
}


