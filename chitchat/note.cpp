#include "note.h"
/**
 * @brief Note::Note
 * @param parent
 */
Note::Note(QWidget *parent) :
    QMainWindow(parent)
{
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    setWindowIcon(QIcon(":/images/note.png"));
    setWindowTitle(tr("记事本"));

    QPalette pal;
    QPixmap pixmap(":/images/background.jpg");
    pal.setBrush(QPalette::Window,QBrush(pixmap));
    setPalette(pal);


    textEdit = new QTextEdit();
    setCentralWidget(textEdit);
    createAction();//创建动作
    createMenu();//创建菜单
    createToolBar();//创建工具栏

    //文件菜单的动作与槽的连接
    connect(newFile,SIGNAL(triggered()),this,SLOT(NewFile()));
    connect(openFile,SIGNAL(triggered()),this,SLOT(OpenFile()));
    connect(save,SIGNAL(triggered()),this,SLOT(Save()));
    connect(saveAs,SIGNAL(triggered()),this,SLOT(SaveAs()));
    connect(exit,SIGNAL(triggered()),this,SLOT(close()));
    //编辑菜单的动作与槽的连接
    connect(cut,SIGNAL(triggered()),textEdit,SLOT(cut()));
    connect(copy,SIGNAL(triggered()),textEdit,SLOT(copy()));
    connect(paste,SIGNAL(triggered()),textEdit,SLOT(paste()));
    connect(undoAction,SIGNAL(triggered()),textEdit,SLOT(undo()));
    connect(redoAction,SIGNAL(triggered()),textEdit,SLOT(redo()));
    //字体设置动作与槽的连接
    connect(fontCom,SIGNAL(activated(QString)),this,
            SLOT(FontComBox(QString)));
    //字号设置动作与槽的连接
    connect(fontSizeCom,SIGNAL(activated(QString)),this,SLOT(FontSizeCom(QString)));

    connect(textEdit,SIGNAL(currentCharFormatChanged(QTextCharFormat&)),
            this,SLOT(showCurrentCharFormatChanged(QTextCharFormat&)));


    //颜色编辑动作与槽的连接
    connect(color,SIGNAL(triggered()),this,SLOT(Color()));
    //格式菜单动作与槽的连接
    connect(bold,SIGNAL(triggered()),this,SLOT(Bold()));
    connect(italic,SIGNAL(triggered()),this,SLOT(Italic()));
    connect(underLine,SIGNAL(triggered()),this,SLOT(UnderLine()));

    connect(group,SIGNAL(triggered(QAction*)),this,SLOT(ALignment(QAction *)));
    //插入菜单动作与槽的连接
    connect(insertImg,SIGNAL(triggered()),this,SLOT(InsertImage()));
    connect(insertTable,SIGNAL(triggered()),this,SLOT(InsertTable()));
    //打印动作与槽的连接
    connect(printText,SIGNAL(triggered()),this,SLOT(PrintText()));
     //查找菜单动作与槽的连接
    connect(find,SIGNAL(triggered()),this,SLOT(Find()));

    connect(about,SIGNAL(triggered()),this,SLOT(About()));
    connect(aboutQt,SIGNAL(triggered()),this,SLOT(AboutQt()));
    resize(600,600);
}


void Note::createAction(){
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    //新建文件
    newFile = new QAction(QIcon(":/images/noteicon/new.png"),tr("新建文件"),this);
    newFile->setShortcut(tr("Ctrl+N"));
    newFile->setStatusTip(tr("新建一个文件"));
    //打开文件
    openFile = new QAction(QIcon(":/images/noteicon/open.png"),tr("打开文件"),this);
    openFile->setShortcut(tr("Ctrl+O"));
    openFile->setStatusTip(tr("打开一个文件"));
    //保存文件
    save = new QAction(QIcon(":/images/noteicon/save.png"),tr("保存"),this);
    save->setShortcut(tr("Ctrl+S"));
    save->setStatusTip(tr("保存文件"));
    //另存为
    saveAs = new QAction(QIcon(":/images/noteicon/saveAs.png"),tr("另存为"),this);
    saveAs->setShortcut(tr("Ctrl+A"));
    saveAs->setStatusTip(tr("文件另存为"));
    //打印
    printText = new QAction(QIcon(":/images/noteicon/printText.png"),tr("打印"),this);
    printText->setShortcut(tr("Ctrl+P"));
    printText->setStatusTip(tr("打印"));
   //退出
    exit = new QAction(QIcon(":/images/noteicon/exit.png"),tr("退出"),this);
    exit->setShortcut(tr("Ctrl+E"));
    exit->setStatusTip(tr("退出"));
    //恢复
    redoAction = new QAction(QIcon(":/images/noteicon/redo.png"),tr("恢复"),this);
    redoAction->setShortcut(tr("Ctrl+R"));
    redoAction->setStatusTip(tr("恢复"));
    //撤销
    undoAction = new QAction(QIcon(":/images/noteicon/undo.png"),tr("撤销"),this);
    undoAction->setShortcut(tr("Ctrl+U"));
    undoAction->setStatusTip(tr("撤销"));
    //剪切
    cut = new QAction(QIcon(":/images/noteicon/cut.png"),tr("剪切"),this);
    cut->setShortcut(tr("Ctrl+X"));
    cut->setStatusTip(tr("剪切"));
    //复制
    copy = new QAction(QIcon(":/images/noteicon/copy.png"),tr("复制"),this);
    copy->setShortcut(tr("Ctrl+C"));
    copy->setStatusTip(tr("复制"));
   //粘贴
    paste = new QAction(QIcon(":/images/noteicon/paste.png"),tr("黏贴"),this);
    paste->setShortcut(tr("Ctrl+P"));
    paste->setStatusTip(tr("粘贴"));

     //粗体
    bold = new QAction(QIcon(":/images/noteicon/bold.png"),tr("粗体"),this);
    bold->setStatusTip(tr("粗体字"));
    bold->setCheckable(true);
    //斜体
    italic = new QAction(QIcon(":/images/noteicon/italic.png"),tr("斜体"),this);
    italic->setStatusTip(tr("斜体字"));
    italic->setCheckable(true);
    //下划线
    underLine = new QAction(QIcon(":/images/noteicon/underline.png"),tr("下划线"),this);
    underLine->setStatusTip(tr("下划线"));
    underLine->setCheckable(true);
    //颜色
    color = new QAction(QIcon(":/images/noteicon/color.png"),tr("颜色"),this);
    color->setShortcut(tr("Ctrl+C"));
    color->setStatusTip(tr("颜色编辑"));

    group = new QActionGroup(this);
    //左对齐
    left = new QAction(QIcon(":/images/noteicon/left.png"),tr("左对齐"),group);
    left->setStatusTip(tr("左对齐"));
    left->setCheckable(true);
    //居中
    center = new QAction(QIcon(":/images/noteicon/center.png"),tr("居中对齐"),group);
    center->setStatusTip(tr("居中对齐"));
    center->setCheckable(true);
    //右对齐
    right = new QAction(QIcon(":/images/noteicon/right.png"),tr("右对齐"),group);
    right->setStatusTip(tr("右对齐"));
    right->setCheckable(true);
    //两端对齐
    justify = new QAction(QIcon(":/images/noteicon/justify.png"),tr("两端对齐"),group);
    justify->setStatusTip(tr("两端对齐"));
    justify->setCheckable(true);

    group->addAction(left);
    group->addAction(center);
    group->addAction(right);
    group->addAction(justify);
    //插入图片
    insertImg = new QAction(QIcon(":/images/noteicon/image.png"),tr("插入图片"),this);
    insertImg->setStatusTip(tr("插入图片"));
    //插入表格
    insertTable = new QAction(QIcon(":/images/noteicon/table.png"),tr("插入表格"),this);
    insertTable->setStatusTip(tr("插入表格"));
    //查找
    find = new QAction(QIcon(":/images/noteicon/find.png"),tr("查找"),this);
    find->setShortcut(tr("Ctrl+F"));
    find->setStatusTip(tr("查找"));
    //关于
    about = new QAction(QIcon(":/images/noteicon/about.png"),tr("关于"),this);
    about->setStatusTip(tr("关于"));

    //字体
    fontLabel = new QLabel(tr("字体"));
    fontCom = new QFontComboBox();
    fontCom->setFontFilters(QFontComboBox::ScalableFonts);
    fontCom->setStatusTip(tr("字体设置"));
    //字号
    fontSizeLabel = new QLabel(tr("字号"));
    QFontDatabase db;
    fontSizeCom = new QComboBox();
    foreach(int size,db.standardSizes())
        fontSizeCom->addItem(QString::number(size));
    fontSizeCom->setStatusTip(tr("字号设置"));
    //关于
    aboutQt = new QAction(tr("关于QT"),this);
    aboutQt->setStatusTip(tr("关于Qt"));

}

//创建菜单
void Note::createMenu(){
    //文件菜单
    file = menuBar()->addMenu(tr("文件(&F)"));
    file->addAction(newFile);
    file->addAction(openFile);
    file->addSeparator();
    file->addAction(save);
    file->addAction(saveAs);
    file->addSeparator();
    file->addAction(printText);
    file->addSeparator();
    file->addAction(exit);
    //编辑菜单
    edit = menuBar()->addMenu(tr("编辑(&E)"));
    edit->addAction(undoAction);
    edit->addAction(redoAction);
    edit->addSeparator();
    edit->addAction(cut);
    edit->addAction(copy);
    edit->addAction(paste);
    //插入菜单
    insert = menuBar()->addMenu(tr("插入(&I)"));
    insert->addAction(insertImg);
    insert->addAction(insertTable);
    //查找菜单
    search = menuBar()->addMenu(tr("查找(&F)"));
    search->addAction(find);
    //格式菜单

    style = menuBar()->addMenu(tr("格式(&S)"));
    style->addAction(bold);
    style->addAction(italic);
    style->addAction(underLine);
    style->addSeparator();
    style->addAction(color);
    style->addSeparator();
    style->addAction(left);
    style->addAction(center);
    style->addAction(right);
    style->addAction(justify);
    //帮助菜单
    help = menuBar()->addMenu(tr("帮助(&H)"));
    help->addAction(about);
    help->addAction(aboutQt);

}
//创建工具栏
void Note::createToolBar(){
    //文件
    fileToolBar = addToolBar(tr("文件(&N)"));
    fileToolBar->addAction(newFile);
    fileToolBar->addAction(openFile);
    fileToolBar->addSeparator();

    fileToolBar->addAction(save);
    fileToolBar->addAction(saveAs);
    fileToolBar->addSeparator();

    fileToolBar->addAction(printText);
    fileToolBar->addSeparator();
    //编辑
    editToolBar = addToolBar(tr("编辑(&E)"));
    editToolBar->addAction(undoAction);
    editToolBar->addAction(redoAction);
    editToolBar->addSeparator();
    editToolBar->addAction(cut);
    editToolBar->addAction(copy);
    editToolBar->addAction(paste);
    //格式
    styleToolBar = addToolBar(tr("格式(&S)"));
    styleToolBar->addAction(bold);
    styleToolBar->addAction(italic);
    styleToolBar->addAction(underLine);
    styleToolBar->addSeparator();

    styleToolBar->addAction(left);
    styleToolBar->addAction(center);
    styleToolBar->addAction(right);
    styleToolBar->addAction(justify);
    styleToolBar->addSeparator();

    styleToolBar->addWidget(fontLabel);
    styleToolBar->addWidget(fontCom);
    styleToolBar->addWidget(fontSizeLabel);
    styleToolBar->addWidget(fontSizeCom);
    //插入
    insertToolBar = addToolBar(tr("插入(&I)"));
    insertToolBar->addAction(insertImg);
    insertToolBar->addAction(insertTable);
    insertToolBar->addSeparator();
    //查找
    searchToolBar = addToolBar(tr("查找(&F)"));
    searchToolBar->addAction(find);
    search->addSeparator();
}



//封装的读文件,用于精简程序
void Note::fileRead(){
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly|QFile::Text)){
         QMessageBox::warning(this,tr("多文档编辑器"),
                tr("不可以打开文件%1:\n%2").arg(fileName).arg(file.errorString()));
         return;
    }
        QTextStream in(&file);
        QApplication::setOverrideCursor(Qt::WaitCursor);
        textEdit->setPlainText(in.readAll());
        QApplication::restoreOverrideCursor();
}
//封装的写文件,用于精简程序
void Note::fileWrite(){
    QFile file(fileName);

    if(!file.open(QFile::WriteOnly|QFile::Text)){
        QMessageBox::warning(this,tr("多文档编辑器"),
                             tr("不可以写入文件%1:/n%2").arg(fileName).arg(file.errorString()));
        return;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out<<textEdit->toPlainText();
    QApplication::restoreOverrideCursor();
    return;
}
//新建文件
void Note::NewFile(){
    Note *n = new Note;
    n->show();
}
//打开文件
void Note::OpenFile(){
    fileName = QFileDialog::getOpenFileName(this,tr("打开文件"),"/","allFile(*.*)");
    if(!fileName.isEmpty()){

        if(textEdit->document()->isEmpty()){
            fileRead();
        }else{
            Note *n = new Note;
            n->show();
            QFile file(fileName);
            if(!file.open(QFile::ReadOnly|QFile::Text)){
                 QMessageBox::warning(this,tr("多文档编辑器"),
                        tr("不可以打开文件%1:\n%2").arg(fileName).arg(file.errorString()));
                 return;
            }
                QTextStream in(&file);
                QApplication::setOverrideCursor(Qt::WaitCursor);
                n->textEdit->setPlainText(in.readAll());
                QApplication::restoreOverrideCursor();


        }

    }
}
//保存文件
void Note::Save(){

    if(fileName.isEmpty()){
       fileName = QFileDialog::getSaveFileName(this,tr("另存为"),tr("未命名.txt"));
       fileWrite();
    }else{
        fileWrite();
    }

}
//另存为
void Note::SaveAs(){

    fileName = QFileDialog::getSaveFileName(this,tr("另存为"),tr("未命名.txt"));
    fileWrite();
}








//字体设置
void Note::FontComBox(QString fontComStr){
    QTextCharFormat format;
    format.setFontFamily(fontComStr);
    mergeFormat(format);
}
void Note::mergeFormat(QTextCharFormat format){
    QTextCursor cursor = textEdit->textCursor();
    if(!cursor.hasSelection())
       cursor.select(QTextCursor::WordUnderCursor);
       cursor.mergeCharFormat(format);
       textEdit->mergeCurrentCharFormat(format);

}
//字号设置
void Note::FontSizeCom(QString fontSizeStr){
    QTextCharFormat format;
    format.setFontPointSize(fontSizeStr.toFloat());
    textEdit->mergeCurrentCharFormat(format);
}





//颜色设置
void Note::Color(){
    QColor color = QColorDialog::getColor(Qt::blue,this);
    if(color.isValid()){
        QTextCharFormat format;
        format.setForeground(color);
        textEdit->mergeCurrentCharFormat(format);
    }
}
//粗体
void Note::Bold(){
    QTextCharFormat format;
    format.setFontWeight(bold->isChecked()?QFont::Bold:QFont::Normal);
    textEdit->mergeCurrentCharFormat(format);
}
//斜体
void Note::Italic(){
    QTextCharFormat format;
    format.setFontItalic(italic->isChecked());
    textEdit->mergeCurrentCharFormat(format);

}
//下划线
void Note::UnderLine(){

    QTextCharFormat format;
    format.setFontUnderline(underLine->isChecked());
    textEdit->mergeCurrentCharFormat(format);
}
//对齐
void Note::ALignment(QAction *act){
    if(act == left){
        textEdit->setAlignment(Qt::AlignLeft);
    }else if(act == center){
        textEdit->setAlignment(Qt::AlignCenter);
    }else if(act == right){
        textEdit->setAlignment(Qt::AlignRight);
    }else if(act == justify){
        textEdit->setAlignment(Qt::AlignJustify);
    }

}





//插入图片
void Note::InsertImage(){

    QTextCursor cursor = textEdit->textCursor();
    QTextImageFormat format;
    QString image = QFileDialog::getOpenFileName(this,"insert image","/","png(*.png)");
    format.setName(image);
    cursor.insertImage(format);

}

//插入表格,用到输入对话框
void Note::InsertTable(){
    bool ok;
    int a = QInputDialog::getInt(this,tr("插入表格"),tr("输入行数:"),0,0,300,1,&ok);
    int b = QInputDialog::getInt(this,tr("插入表格"),tr("输入列数:"),0,0,300,1,&ok);
    if(ok){

        QTextCursor cursor = textEdit->textCursor();

        QTextTableFormat format;
        format.setCellSpacing(0);
        format.setCellPadding(30);

        cursor.insertTable(a,b,format);

    }
}


//查找
void Note::Find(){
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("查找"));
    lineEdit = new QLineEdit(dialog);
    label = new QLabel(this);
    btn = new QPushButton(dialog);
    btn->setText(tr("下一个"));
    connect(btn,SIGNAL(clicked()),this,SLOT(findNext()));

    layout = new QVBoxLayout();
    layout->addWidget(lineEdit);
    layout->addWidget(btn);
    dialog->setLayout(layout);
    dialog->resize(60,60);
    dialog->show();

}

void Note::findNext(){

    QString str = lineEdit->text();

    bool isfind = textEdit->find(str,QTextDocument::FindBackward);

    if(isfind){
        label->setText(tr("查找目标在,行号:%1 列号:%2!").arg(textEdit->textCursor().blockNumber()).arg(textEdit->textCursor().columnNumber()));
        //statusBar()->addPermanentWidget(label);

    }

}
//打印
void Note::PrintText(){
    QPrinter printer;
    QPrintDialog printDialog(&printer,this);
    if(printDialog.exec()){
        QTextDocument *textDocument = textEdit->document();
        textDocument->print(&printer);
    }
}




//帮助
void Note::About(){
    QMessageBox::about(this,tr("关于"),tr("这是一个记事本"));

}

void Note::AboutQt(){
    QMessageBox::aboutQt(this,tr("Qt编辑器"));
}

void Note::showCurrentCharFormatChanged(const QTextCharFormat &form){

    fontCom->setCurrentIndex(fontCom->findText(form.fontFamily()));
    fontSizeCom->setCurrentIndex(fontSizeCom->findText(QString::number(form.fontPointSize())));
    bold->setChecked(form.font().bold());
    italic->setChecked(form.font().italic());
    underLine->setChecked(form.font().underline());
}

