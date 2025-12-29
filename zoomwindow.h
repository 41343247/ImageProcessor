#ifndef ZOOMWINDOW_H
#define ZOOMWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QLabel>
#include <QPushButton>
#include <QToolBar>
#include <QMouseEvent>
#include <QPoint>
#include <QPainter>
#include <QColor>

// Custom QLabel for handling drawing events
class DrawableLabel : public QLabel
{
    Q_OBJECT
public:
    DrawableLabel(QWidget *parent = nullptr);
    void setDrawingEnabled(bool enabled);
    void setImage(const QImage &image);
    QImage getImage() const { return drawingImage; }

signals:
    void imageChanged();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QImage drawingImage;
    bool drawingEnabled;
    bool isDrawing;
    QPoint lastDrawPoint;
    QColor brushColor;
    int brushSize;
};

class ZoomWindow : public QMainWindow
{
    Q_OBJECT

public:
    ZoomWindow(const QImage &image, QWidget *parent = nullptr);
    ~ZoomWindow();

    void setImage(const QImage &image);

private slots:
    void saveImage();
    void toggleBrushTool();

private:
    QImage originalImage;
    DrawableLabel *imageLabel;
    QPushButton *saveButton;
    QPushButton *brushButton;
    QToolBar *toolbar;
    
    bool brushEnabled;
};

#endif // ZOOMWINDOW_H
