#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QObject::connect(ui->btnRename, &QPushButton::clicked, this, [&]() {
        RenameDlg rename_dlg_;
        rename_dlg_.exec();
    });

    QObject::connect(ui->btnAnalysis, &QPushButton::clicked, this, [&]() {
        AnalysisXmlDialog analysis_xml_dlg;
        analysis_xml_dlg.exec();
    });

    QObject::connect(ui->btnExtract, &QPushButton::clicked, this, [&]() {
        ExtractDataDlg extract_data_dlg;
        extract_data_dlg.exec();
    });

    QObject::connect(ui->btnSSIM, &QPushButton::clicked, this, [&]() {
        SSIMDlg ssim_dlg_;
        ssim_dlg_.exec();
    });

    QObject::connect(ui->btnModifyLabel, &QPushButton::clicked, this, [&]() {
        ModifyLabelDlg modify_label_dlg_;
        modify_label_dlg_.exec();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

