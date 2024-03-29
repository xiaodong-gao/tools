#ifndef ANALYSISXMLDIALOG_H
#define ANALYSISXMLDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <fstream>
#include <QMessageBox>
#include "tinyxml2.h"
#include <vector>

namespace Ui {
class AnalysisXmlDialog;
}

class AnalysisXmlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AnalysisXmlDialog(QWidget *parent = nullptr);
    ~AnalysisXmlDialog();
    void analysys_xml();
    int analysis_xml_format(const char* xmlFilePath, QString image_name);

private:
    Ui::AnalysisXmlDialog *ui;
    QString src_folder_path_;
};

#endif // ANALYSISXMLDIALOG_H
