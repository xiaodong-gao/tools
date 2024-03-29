#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Dialog/AnalysisXmlDialog.h"
#include "Dialog/ExtractDataDlg.h"
#include "Dialog/RenameDlg.h"
#include "Dialog/SSIMDlg.h"
#include "Dialog/ModifyLabelDlg.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
