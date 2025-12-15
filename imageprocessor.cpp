#include "imageprocessor.h"
#include <QHBoxLayout>
#include <QMenuBar>
#include <QFileDialog>
#include <QDebug>

ImageProcessor::ImageProcessor(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("影像處理"));
    central = new QWidget();
    QHBoxLayout *mainLayout = new QHBoxLayout(central);
    imgWin = new QLabel();
    QPixmap *initPixmap = new QPixmap(300,200);
    initPixmap->fill(QColor(255,255,255));
    imgWin->resize(300,200);
    imgWin->setScaledContents(true);
    imgWin->setPixmap(*initPixmap);
    mainLayout->addWidget(imgWin);
    setCentralWidget(central);
    createActions();
    createMenus();
    createToolBars();
}

ImageProcessor::~ImageProcessor() {}

void ImageProcessor::createActions()
{
    openFileAction = new QAction(tr("開啟檔案&O"),this);
    openFileAction->setShortcut(tr("Ctrl+O"));
    openFileAction->setStatusTip(tr("開啟影像檔案"));
    connect(openFileAction,SIGNAL(triggered(bool)),this,SLOT(showOpenFile()));

    exitAction = new QAction(tr("結束&Q"),this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("退出程式"));
    connect(exitAction,SIGNAL(triggered(bool)),this,SLOT(close()));

    big = new QAction(tr("放大"),this);
    exitAction->setStatusTip(tr("放大"));
    connect(big,SIGNAL(triggered(bool)),this,SLOT(bigFile()));

    small = new QAction(tr("縮小"),this);
    exitAction->setStatusTip(tr("縮小"));
    connect(small,SIGNAL(triggered(bool)),this,SLOT(smallFile()));
}

void ImageProcessor::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("檔案&F"));
    fileMenu->addAction(openFileAction);
    fileMenu->addAction(exitAction);

    fileMenu = menuBar()->addMenu(tr("工具&T"));
    fileMenu->addAction(big);
    fileMenu->addAction(small);
}

void ImageProcessor::createToolBars()
{
    fileTool = addToolBar("file");
    fileTool->addAction(openFileAction);

    fileTool = addToolBar("tool");
    fileTool->addAction(big);
    fileTool->addAction(small);
}

void ImageProcessor::loadFile(QString filename)
{
    qDebug()<<QString("file name:%1").arg(filename);
    QByteArray ba = filename.toLatin1();
    printf("FN:%s\n",(char *) ba.data());
    img.load(filename);
    imgWin->setPixmap(QPixmap::fromImage(img));
}

void ImageProcessor::showOpenFile()
{
    filename = QFileDialog::getOpenFileName(this,tr("開啟影像"),tr("."),"bmp(*.bmp);;png(*.png)"";;Jpng(*.jpg)");
    if(!filename.isEmpty()){
        if(img.isNull()){
            loadFile(filename);
        }else{
            ImageProcessor *newIPWin = new ImageProcessor();
            newIPWin->show();
            newIPWin->loadFile(filename);
        }
    }
}

void ImageProcessor::bigFile()
{
    if(!filename.isEmpty()){
        ImageProcessor *newIPWin = new ImageProcessor();
        newIPWin->show();
        newIPWin->loadFile(filename);
    }
}
void ImageProcessor::smallFile()
{
    if(!filename.isEmpty()){
        ImageProcessor *newIPWin = new ImageProcessor();
        newIPWin->show();
        newIPWin->loadFile(filename);
    }
}
