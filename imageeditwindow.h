#ifndef IMAGEEDITWINDOW_H
#define IMAGEEDITWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QLabel>
#include <QScrollArea>
#include <QMouseEvent>
#include <QPushButton>
#include <QSlider>
#include <QToolBar>
#include <QAction>
#include <QColorDialog>
#include <QPainter>
#include <QPoint>

class ImageEditWindow : public QMainWindow
{
    Q_OBJECT

public:
    ImageEditWindow(const QImage &image, QWidget *parent = nullptr);
    ~ImageEditWindow();

private slots:
    void saveAsImage();
    void toggleBrushMode();
    void chooseBrushColor();
    void changeBrushSize(int size);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    void createActions();
    void createToolBar();
    void drawLineTo(const QPoint &endPoint);
    QPoint getLabelPosition(QMouseEvent *event) const;

    QImage originalImage;
    QImage editedImage;
    QLabel *imageLabel;
    QScrollArea *scrollArea;
    
    QAction *saveAsAction;
    QAction *brushAction;
    QAction *colorAction;
    QToolBar *toolBar;
    QSlider *brushSizeSlider;
    
    bool brushMode;
    bool drawing;
    QColor brushColor;
    int brushSize;
    QPoint lastPoint;
};

#endif // IMAGEEDITWINDOW_H
