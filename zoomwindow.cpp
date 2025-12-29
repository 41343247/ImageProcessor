#include "zoomwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QScrollArea>
#include <QPainter>

// DrawableLabel implementation
DrawableLabel::DrawableLabel(QWidget *parent)
    : QLabel(parent),
      drawingEnabled(false),
      isDrawing(false),
      brushColor(Qt::red),
      brushSize(3)
{
    setMouseTracking(true);
}

void DrawableLabel::setDrawingEnabled(bool enabled)
{
    drawingEnabled = enabled;
    if (enabled) {
        setCursor(Qt::CrossCursor);
    } else {
        setCursor(Qt::ArrowCursor);
    }
}

void DrawableLabel::setImage(const QImage &image)
{
    drawingImage = image;
    setPixmap(QPixmap::fromImage(drawingImage));
    // Set fixed size to match image size for proper display
    setFixedSize(drawingImage.size());
}

void DrawableLabel::mousePressEvent(QMouseEvent *event)
{
    if (drawingEnabled && event->button() == Qt::LeftButton) {
        isDrawing = true;
        lastDrawPoint = event->pos();
    }
    QLabel::mousePressEvent(event);
}

void DrawableLabel::mouseMoveEvent(QMouseEvent *event)
{
    if (drawingEnabled && isDrawing) {
        QPoint currentPos = event->pos();
        
        // Draw on the image
        QPainter painter(&drawingImage);
        painter.setPen(QPen(brushColor, brushSize, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter.drawLine(lastDrawPoint, currentPos);
        
        lastDrawPoint = currentPos;
        
        // Update the display
        setPixmap(QPixmap::fromImage(drawingImage));
        emit imageChanged();
    }
    QLabel::mouseMoveEvent(event);
}

void DrawableLabel::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        isDrawing = false;
    }
    QLabel::mouseReleaseEvent(event);
}

// ZoomWindow implementation
ZoomWindow::ZoomWindow(const QImage &image, QWidget *parent)
    : QMainWindow(parent),
      originalImage(image),
      brushEnabled(false)
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
    
    // Create drawable image label
    imageLabel = new DrawableLabel(this);
    imageLabel->setAlignment(Qt::AlignCenter);
    
    // Scale image to fit window (800x600 minus toolbar)
    QImage scaledImage = image.scaled(780, 550, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    imageLabel->setImage(scaledImage);
    
    // Create scroll area for the image
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidget(imageLabel);
    scrollArea->setWidgetResizable(false);
    
    mainLayout->addWidget(scrollArea);
    setCentralWidget(centralWidget);
    
    // Set window size
    resize(800, 600);
}

ZoomWindow::~ZoomWindow()
{
}

void ZoomWindow::setImage(const QImage &image)
{
    originalImage = image;
    imageLabel->setImage(image);
}

void ZoomWindow::saveImage()
{
    QImage currentImage = imageLabel->getImage();
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("另存為影像"), "",
        tr("PNG Files (*.png);;JPEG Files (*.jpg);;BMP Files (*.bmp)"));
    
    if (!fileName.isEmpty()) {
        if (currentImage.save(fileName)) {
            QMessageBox::information(this, tr("成功"), tr("影像已儲存"));
        } else {
            QMessageBox::warning(this, tr("錯誤"), tr("無法儲存影像"));
        }
    }
}

void ZoomWindow::toggleBrushTool()
{
    brushEnabled = brushButton->isChecked();
    imageLabel->setDrawingEnabled(brushEnabled);
    if (brushEnabled) {
        brushButton->setText(tr("筆刷工具 (啟用)"));
    } else {
        brushButton->setText(tr("筆刷工具"));
    }
}
