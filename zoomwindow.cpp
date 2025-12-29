#include "zoomwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QScrollArea>
#include <QPainter>

ZoomWindow::ZoomWindow(const QImage &image, QWidget *parent)
    : QMainWindow(parent),
      originalImage(image),
      drawingImage(image),
      brushEnabled(false),
      isDrawing(false),
      brushColor(Qt::red),
      brushSize(3)
{
    setWindowTitle(tr("放大影像"));
    
    // Create central widget
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    
    // Create toolbar
    toolbar = addToolBar(tr("工具"));
    
    // Create Save button
    saveButton = new QPushButton(tr("另存為"), this);
    toolbar->addWidget(saveButton);
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveImage()));
    
    // Create Brush button
    brushButton = new QPushButton(tr("筆刷工具"), this);
    brushButton->setCheckable(true);
    toolbar->addWidget(brushButton);
    connect(brushButton, SIGNAL(clicked()), this, SLOT(toggleBrushTool()));
    
    // Create image label
    imageLabel = new QLabel(this);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setScaledContents(false);
    imageLabel->setPixmap(QPixmap::fromImage(drawingImage));
    
    // Create scroll area for the image
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidget(imageLabel);
    scrollArea->setWidgetResizable(true);
    
    mainLayout->addWidget(scrollArea);
    setCentralWidget(centralWidget);
    
    // Set window size
    resize(800, 600);
    
    // Enable mouse tracking for brush tool
    setMouseTracking(true);
    imageLabel->setMouseTracking(true);
}

ZoomWindow::~ZoomWindow()
{
}

void ZoomWindow::setImage(const QImage &image)
{
    originalImage = image;
    drawingImage = image;
    imageLabel->setPixmap(QPixmap::fromImage(drawingImage));
}

void ZoomWindow::saveImage()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("另存為影像"), "",
        tr("PNG Files (*.png);;JPEG Files (*.jpg);;BMP Files (*.bmp)"));
    
    if (!fileName.isEmpty()) {
        if (drawingImage.save(fileName)) {
            QMessageBox::information(this, tr("成功"), tr("影像已儲存"));
        } else {
            QMessageBox::warning(this, tr("錯誤"), tr("無法儲存影像"));
        }
    }
}

void ZoomWindow::toggleBrushTool()
{
    brushEnabled = brushButton->isChecked();
    if (brushEnabled) {
        brushButton->setText(tr("筆刷工具 (啟用)"));
        setCursor(Qt::CrossCursor);
        imageLabel->setCursor(Qt::CrossCursor);
    } else {
        brushButton->setText(tr("筆刷工具"));
        setCursor(Qt::ArrowCursor);
        imageLabel->setCursor(Qt::ArrowCursor);
    }
}

void ZoomWindow::mousePressEvent(QMouseEvent *event)
{
    if (brushEnabled && event->button() == Qt::LeftButton) {
        // Map the mouse position to the image label coordinate
        QPoint labelPos = imageLabel->mapFrom(this, event->pos());
        
        // Check if the click is within the image label
        if (imageLabel->rect().contains(labelPos)) {
            isDrawing = true;
            lastDrawPoint = labelPos;
        }
    }
}

void ZoomWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (brushEnabled && isDrawing) {
        QPoint labelPos = imageLabel->mapFrom(this, event->pos());
        
        if (imageLabel->rect().contains(labelPos)) {
            // Draw on the image
            QPainter painter(&drawingImage);
            painter.setPen(QPen(brushColor, brushSize, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter.drawLine(lastDrawPoint, labelPos);
            
            lastDrawPoint = labelPos;
            
            // Update the display
            imageLabel->setPixmap(QPixmap::fromImage(drawingImage));
        }
    }
}

void ZoomWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        isDrawing = false;
    }
}

void ZoomWindow::paintEvent(QPaintEvent *event)
{
    QMainWindow::paintEvent(event);
}
