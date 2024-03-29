#ifndef MODIFYLABELDLG_H
#define MODIFYLABELDLG_H

#include <QDialog>
#include <QFileDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QMessageBox>
#include <fstream>
#include <vector>
#include <string>

#include "tinyxml2.h"

namespace Ui {
class ModifyLabelDlg;
}

class ModifyLabelDlg : public QDialog
{
    Q_OBJECT
public:
    explicit ModifyLabelDlg(QWidget *parent = nullptr);
    ~ModifyLabelDlg();
    void modify_label();
    std::vector<std::string> objects_names_from_file(std::string const filename);
private:
    Ui::ModifyLabelDlg *ui;
    QString src_folder_path_;
    QString src_label_name_;
    QString dst_label_name_;
};

#endif // MODIFYLABELDLG_H
