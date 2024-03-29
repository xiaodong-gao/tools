#ifndef EXTRACTDATADLG_H
#define EXTRACTDATADLG_H

#include <QDialog>
#include <QFileDialog>
#include <QLineEdit>
#include <vector>
#include <fstream>
#include <QMessageBox>
#include "tinyxml2.h"

namespace Ui {
class ExtractDataDlg;
}

class ExtractDataDlg : public QDialog
{
    Q_OBJECT
public:
    explicit ExtractDataDlg(QWidget *parent = nullptr);
    ~ExtractDataDlg();
    std::vector<std::string> objects_names_from_file(std::string const filename);
    void extract_file_to_dst();
    bool extract_label_name(const char* xmlFilePath, std::string label_name);
private:
    Ui::ExtractDataDlg *ui;
    QString src_folder_path_;
    QString dst_folder_path_;
    QString mode_;
    //QString label_name_;
};

#endif // EXTRACTDATADLG_H
