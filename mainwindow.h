#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <stdio.h>
#include <QFileDialog>
#include <QCommandLinkButton>
#include <QPlainTextEdit>
#include <QMessageBox>
#include <engine.h>
#include <dialog.h>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    Ui::MainWindow *ui;
    QLabel *Status;
   //  Dialog * aa;
    int Operation;
    Engine *engine;

public slots:
    void ShowDialog();
   // void FromKeyboard();
    void OpenFile1 ();
    void OpenFile2 ();
    void OpenFile3 ();
    void Hide ();
    void Unhide ();
    void About();
    void Help();
    void Noise();
    int mainslot();
signals:
};

#endif // MAINWINDOW_H
