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

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    QImage originalImage;
    QImage drawingImage;
    QLabel *imageLabel;
    QPushButton *saveButton;
    QPushButton *brushButton;
    QToolBar *toolbar;
    
    bool brushEnabled;
    bool isDrawing;
    QPoint lastDrawPoint;
    QColor brushColor;
    int brushSize;
};

#endif // ZOOMWINDOW_H
