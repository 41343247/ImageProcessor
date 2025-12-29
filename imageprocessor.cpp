#include "imageprocessor.h"
#include "imagetransform.h"
#include "zoomwindow.h"
#include <QHBoxLayout>
#include <QMenuBar>
#include <QFileDialog>
#include <QPainter>
#include <QDebug>

ImageProcessor::ImageProcessor(QWidget *parent)
    : QMainWindow(parent),
      isDragging(false)
{
    setWindowTitle(tr("影像處理"));
    central = new QWidget();
    QHBoxLayout *mainLayout = new QHBoxLayout(central);
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
    int gray = qGray(img.pixel(event->x(),event->y()));
    QString str = "(" + QString::number(event->x()) +", " + QString::number(event->y()) + ")" + " = "+QString::number(gray);

    MousePosLabel->setText(str);
    
    // Update drag selection
    if (isDragging) {
        dragEndPoint = event->pos();
        selectionRect = QRect(dragStartPoint, dragEndPoint).normalized();
        update(); // Trigger repaint to show selection rectangle
    }
}
void ImageProcessor::mousePressEvent(QMouseEvent *event){
    QString str = "(" + QString::number(event->x()) +", " + QString::number(event->y()) + ")";
    if(event->button()==Qt::LeftButton){
        statusBar()->showMessage(tr("左鍵:")+str,1000);
        
        // Start drag selection if mouse is over the image
        if (!img.isNull() && imgWin->geometry().contains(event->pos())) {
            isDragging = true;
            dragStartPoint = event->pos();
            dragEndPoint = event->pos();
            selectionRect = QRect();
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
    
    // Handle drag selection completion
    if (isDragging && event->button() == Qt::LeftButton) {
        isDragging = false;
        dragEndPoint = event->pos();
        selectionRect = QRect(dragStartPoint, dragEndPoint).normalized();
        
        // Check if a valid selection was made
        if (!img.isNull() && selectionRect.width() > 10 && selectionRect.height() > 10) {
            // Map selection coordinates to image coordinates
            QRect imgRect = imgWin->geometry();
            
            // Calculate the scale factor
            qreal scaleX = (qreal)img.width() / imgRect.width();
            qreal scaleY = (qreal)img.height() / imgRect.height();
            
            // Adjust selection rect relative to image widget
            QRect adjustedRect = selectionRect.translated(-imgRect.x(), -imgRect.y());
            
            // Scale to image coordinates
            QRect imageRect(
                adjustedRect.x() * scaleX,
                adjustedRect.y() * scaleY,
                adjustedRect.width() * scaleX,
                adjustedRect.height() * scaleY
            );
            
            // Ensure the rect is within image bounds
            imageRect = imageRect.intersected(QRect(0, 0, img.width(), img.height()));
            
            if (!imageRect.isEmpty()) {
                // Extract the selected region
                QImage zoomedImage = img.copy(imageRect);
                
                // Create and show the zoom window
                ZoomWindow *zoomWin = new ZoomWindow(zoomedImage, this);
                zoomWin->show();
            }
        }
        
        // Clear the selection rectangle
        selectionRect = QRect();
        update();
    }
}

void ImageProcessor::paintEvent(QPaintEvent *event) {
    QMainWindow::paintEvent(event);
    
    // Draw selection rectangle if dragging
    if (isDragging && !selectionRect.isEmpty()) {
        QPainter painter(this);
        painter.setPen(QPen(Qt::blue, 2, Qt::DashLine));
        painter.drawRect(selectionRect);
    }
}
