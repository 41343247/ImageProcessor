#include "imageprocessor.h"
#include "imagetransform.h"
#include "imageeditwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QFileDialog>
#include <QDebug>

ImageProcessor::ImageProcessor(QWidget *parent)
    : QMainWindow(parent)
    , zoomLevel(1.0)
    , isDragging(false)
{
    setWindowTitle(tr("影像處理"));
    central = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    
    // Zoom control layout
    QHBoxLayout *zoomLayout = new QHBoxLayout();
    zoomLabel = new QLabel(tr("縮放等級: 100%"));
    zoomLabel->setFixedWidth(120);
    zoomSlider = new QSlider(Qt::Horizontal);
    zoomSlider->setMinimum(10);
    zoomSlider->setMaximum(400);
    zoomSlider->setValue(100);
    zoomSlider->setTickPosition(QSlider::TicksBelow);
    zoomSlider->setTickInterval(50);
    connect(zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(zoomChanged(int)));
    
    zoomLayout->addWidget(zoomLabel);
    zoomLayout->addWidget(zoomSlider);
    mainLayout->addLayout(zoomLayout);
    
    imgWin = new QLabel();
    QPixmap *initPixmap = new QPixmap(300,200);
    gWin = new ImageTransform();
    initPixmap->fill(QColor(255,255,255));
    imgWin->resize(300,200);
    imgWin->setScaledContents(true);
    imgWin->setPixmap(*initPixmap);
    mainLayout->addWidget(imgWin);
    setCentralWidget(central);
    createActions();
    createMenus();
    createToolBars();

    statusLabel = new QLabel;
    statusLabel->setText(tr("指標位置:"));
    statusLabel->setFixedWidth(100);
    MousePosLabel = new QLabel;
    MousePosLabel->setText(tr(" "));
    MousePosLabel->setFixedWidth(100);
    statusBar()->addPermanentWidget(statusLabel);
    statusBar()->addPermanentWidget(MousePosLabel);
    setMouseTracking(true);
    imgWin->setMouseTracking(true);
    central->setMouseTracking(true);

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

    geometryAction = new QAction(tr("幾何轉換"),this);
    geometryAction->setShortcut(tr("Ctrl+G"));
    geometryAction->setStatusTip(tr("影像幾何轉換"));
    connect(geometryAction,SIGNAL(triggered(bool)),this,SLOT(showGeometryTransform()));
    connect(exitAction,SIGNAL(triggered(bool)),gWin,SLOT(close()));
}

void ImageProcessor::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("檔案&F"));
    fileMenu->addAction(openFileAction);
    fileMenu->addAction(exitAction);

    fileMenu = menuBar()->addMenu(tr("工具&T"));
    fileMenu->addAction(big);
    fileMenu->addAction(small);

    fileMenu->addAction(geometryAction);
}

void ImageProcessor::createToolBars()
{
    fileTool = addToolBar("file");
    fileTool->addAction(openFileAction);

    fileTool = addToolBar("tool");
    fileTool->addAction(big);
    fileTool->addAction(small);

    fileTool->addAction(geometryAction);
}

void ImageProcessor::loadFile(QString filename)
{
    qDebug()<<QString("file name:%1").arg(filename);
    QByteArray ba = filename.toLatin1();
    printf("FN:%s\n",(char *) ba.data());
    img.load(filename);
    
    // Apply zoom level
    QImage scaledImg = img.scaled(img.width() * zoomLevel, img.height() * zoomLevel, 
                                   Qt::KeepAspectRatio, Qt::SmoothTransformation);
    imgWin->setPixmap(QPixmap::fromImage(scaledImg));
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

void ImageProcessor::showGeometryTransform(){
    if(!img.isNull())
        gWin->srcImg=img;
    gWin->inWin->setPixmap(QPixmap::fromImage(gWin->srcImg));
    gWin->show();
}

void ImageProcessor::mouseDoubleClickEvent(QMouseEvent *event){
    QString str = "(" + QString::number(event->x()) +", " + QString::number(event->y()) + ")";
    statusBar()->showMessage(tr("雙擊:")+str,1000);
}
void ImageProcessor::mouseMoveEvent(QMouseEvent *event){
    if (!img.isNull() && event->x() >= 0 && event->x() < img.width() && 
        event->y() >= 0 && event->y() < img.height()) {
        int gray = qGray(img.pixel(event->x(),event->y()));
        QString str = "(" + QString::number(event->x()) +", " + QString::number(event->y()) + ")" + " = "+QString::number(gray);
        MousePosLabel->setText(str);
    }
    
    // Detect dragging
    if (!img.isNull() && (event->buttons() & Qt::LeftButton)) {
        int distance = (event->pos() - dragStartPos).manhattanLength();
        if (distance >= 10) {  // Minimum drag distance
            isDragging = true;
        }
    }
}
void ImageProcessor::mousePressEvent(QMouseEvent *event){
    QString str = "(" + QString::number(event->x()) +", " + QString::number(event->y()) + ")";
    if(event->button()==Qt::LeftButton){
        statusBar()->showMessage(tr("左鍵:")+str,1000);
        // Start drag detection
        if (!img.isNull()) {
            dragStartPos = event->pos();
            isDragging = false;
        }
    }
    else if(event->button()==Qt::RightButton){
        statusBar()->showMessage(tr("右鍵:")+str,1000);
    }
    else if(event->button()==Qt::MiddleButton){
        statusBar()->showMessage(tr("中鍵:")+str,1000);
    }
}
void ImageProcessor::mouseReleaseEvent(QMouseEvent *event){
    QString str = "(" + QString::number(event->x()) +", " + QString::number(event->y()) + ")";
    statusBar()->showMessage(tr("釋放:")+str,1000);
    
    // If dragging occurred, open edit window
    if (isDragging && !img.isNull()) {
        openEditWindow();
        isDragging = false;
    }
}

void ImageProcessor::zoomChanged(int value)
{
    zoomLevel = value / 100.0;
    zoomLabel->setText(tr("縮放等級: ") + QString::number(value) + "%");
    
    // Re-display image with new zoom level
    if (!img.isNull()) {
        QImage scaledImg = img.scaled(img.width() * zoomLevel, img.height() * zoomLevel, 
                                       Qt::KeepAspectRatio, Qt::SmoothTransformation);
        imgWin->setPixmap(QPixmap::fromImage(scaledImg));
    }
}

void ImageProcessor::openEditWindow()
{
    if (!img.isNull()) {
        // Create enlarged image based on zoom level
        QImage enlargedImg = img.scaled(img.width() * zoomLevel, img.height() * zoomLevel, 
                                        Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ImageEditWindow *editWin = new ImageEditWindow(enlargedImg);
        editWin->setAttribute(Qt::WA_DeleteOnClose);
        editWin->show();
        statusBar()->showMessage(tr("已開啟影像編輯視窗"), 2000);
    }
}
