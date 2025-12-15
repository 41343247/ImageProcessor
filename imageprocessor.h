#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QMainWindow>
#include <QAction>
#include <QMenu>
#include <QToolBar>
#include <QImage>
#include <QLabel>

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

private:
    QWidget     *central;
    QMenu       *fileMenu;
    QToolBar    *fileTool;
    QImage      img;
    QString     filename;
    QLabel      *imgWin;
    QAction     *openFileAction;
    QAction     *exitAction;

    QAction     *big;
    QAction     *small;
};
#endif // IMAGEPROCESSOR_H
