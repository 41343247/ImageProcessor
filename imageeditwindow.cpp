#include "imageeditwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>
#include <QPainter>
#include <QDebug>
#include <QStatusBar>

ImageEditWindow::ImageEditWindow(const QImage &image, QWidget *parent)
    : QMainWindow(parent)
    , originalImage(image)
    , editedImage(image)
    , brushMode(false)
    , drawing(false)
    , brushColor(Qt::red)
    , brushSize(5)
{
    setWindowTitle(tr("影像編輯 - 拖曳與繪製"));
    resize(800, 600);

    // Create central widget with scroll area
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(false);
    scrollArea->setAlignment(Qt::AlignCenter);
    
    imageLabel = new QLabel();
    imageLabel->setPixmap(QPixmap::fromImage(editedImage));
    imageLabel->setScaledContents(false);
    imageLabel->setMouseTracking(true);
    
    scrollArea->setWidget(imageLabel);
    mainLayout->addWidget(scrollArea);
    
    setCentralWidget(centralWidget);
    
    createActions();
    createToolBar();
    
    setMouseTracking(true);
}

ImageEditWindow::~ImageEditWindow()
{
}

void ImageEditWindow::createActions()
{
    saveAsAction = new QAction(tr("另存新檔"), this);
    saveAsAction->setStatusTip(tr("將影像存為新檔案"));
    connect(saveAsAction, &QAction::triggered, this, &ImageEditWindow::saveAsImage);
    
    brushAction = new QAction(tr("筆刷工具"), this);
    brushAction->setCheckable(true);
    brushAction->setStatusTip(tr("啟用/停用筆刷繪製"));
    connect(brushAction, &QAction::triggered, this, &ImageEditWindow::toggleBrushMode);
    
    colorAction = new QAction(tr("選擇顏色"), this);
    colorAction->setStatusTip(tr("選擇筆刷顏色"));
    connect(colorAction, &QAction::triggered, this, &ImageEditWindow::chooseBrushColor);
}

void ImageEditWindow::createToolBar()
{
    toolBar = addToolBar(tr("工具"));
    toolBar->addAction(saveAsAction);
    toolBar->addSeparator();
    toolBar->addAction(brushAction);
    toolBar->addAction(colorAction);
    toolBar->addSeparator();
    
    // Add brush size slider
    QLabel *sizeLabel = new QLabel(tr("筆刷大小: "));
    toolBar->addWidget(sizeLabel);
    
    brushSizeSlider = new QSlider(Qt::Horizontal);
    brushSizeSlider->setMinimum(1);
    brushSizeSlider->setMaximum(50);
    brushSizeSlider->setValue(brushSize);
    brushSizeSlider->setFixedWidth(150);
    connect(brushSizeSlider, &QSlider::valueChanged, this, &ImageEditWindow::changeBrushSize);
    toolBar->addWidget(brushSizeSlider);
}

void ImageEditWindow::saveAsImage()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("另存影像"), "",
        tr("PNG Files (*.png);;JPEG Files (*.jpg);;BMP Files (*.bmp)"));
    
    if (!fileName.isEmpty()) {
        if (editedImage.save(fileName)) {
            statusBar()->showMessage(tr("影像已儲存至: ") + fileName, 3000);
        } else {
            QMessageBox::warning(this, tr("錯誤"), tr("無法儲存影像"));
        }
    }
}

void ImageEditWindow::toggleBrushMode()
{
    brushMode = brushAction->isChecked();
    if (brushMode) {
        statusBar()->showMessage(tr("筆刷模式已啟用 - 點擊並拖曳以繪製"), 2000);
        setCursor(Qt::CrossCursor);
    } else {
        statusBar()->showMessage(tr("筆刷模式已停用"), 2000);
        setCursor(Qt::ArrowCursor);
    }
}

void ImageEditWindow::chooseBrushColor()
{
    QColor color = QColorDialog::getColor(brushColor, this, tr("選擇筆刷顏色"));
    if (color.isValid()) {
        brushColor = color;
        statusBar()->showMessage(tr("筆刷顏色已更改"), 2000);
    }
}

void ImageEditWindow::changeBrushSize(int size)
{
    brushSize = size;
    statusBar()->showMessage(tr("筆刷大小: ") + QString::number(size), 1000);
}

void ImageEditWindow::mousePressEvent(QMouseEvent *event)
{
    if (brushMode && event->button() == Qt::LeftButton) {
        drawing = true;
        // Convert global position to image label coordinates
        QPoint labelPos = imageLabel->mapFromGlobal(event->globalPos());
        
        // Check if the click is within the image label
        if (imageLabel->rect().contains(labelPos)) {
            lastPoint = labelPos;
        }
    }
}

void ImageEditWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (brushMode && drawing && (event->buttons() & Qt::LeftButton)) {
        QPoint labelPos = imageLabel->mapFromGlobal(event->globalPos());
        
        if (imageLabel->rect().contains(labelPos)) {
            drawLineTo(labelPos);
        }
    }
}

void ImageEditWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && drawing) {
        drawing = false;
    }
}

void ImageEditWindow::paintEvent(QPaintEvent *event)
{
    QMainWindow::paintEvent(event);
}

void ImageEditWindow::drawLineTo(const QPoint &endPoint)
{
    QPainter painter(&editedImage);
    painter.setPen(QPen(brushColor, brushSize, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);
    painter.end();
    
    lastPoint = endPoint;
    
    // Update the displayed image
    imageLabel->setPixmap(QPixmap::fromImage(editedImage));
}
