#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QMainWindow>
#include <QAction>
#include <QMenu>
#include <QToolBar>
#include <QImage>
#include <QLabel>
#include "imagetransform.h"

class ImageProcessor : public QMainWindow
{
    Q_OBJECT

public:
    ImageProcessor(QWidget *parent = nullptr);
    ~ImageProcessor();

    void createActions();
    void createMenus();
    void createToolBars();
    void loadFile(QString filename);

private slots:
    void showOpenFile();
    void bigFile();
    void smallFile();
    void showGeometryTransform();

private:
    ImageTransform *gWin;
    QWidget     *central;
    QMenu       *fileMenu;
    QToolBar    *fileTool;
    QImage      img;
    QString     filename;
    QLabel      *imgWin;
    QAction     *openFileAction;
    QAction     *exitAction;
    QAction     *geometryAction;

    QAction     *big;
    QAction     *small;
};
#endif // IMAGEPROCESSOR_H
