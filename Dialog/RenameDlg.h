#ifndef RENAMEDLG_H
#define RENAMEDLG_H

#include <QDialog>
#include <QFileDialog>
#include <QDateTime>
#include <QMessageBox>

namespace Ui {
class RenameDlg;
}

class RenameDlg : public QDialog
{
    Q_OBJECT
public:
    explicit RenameDlg(QDialog *parent = nullptr);
    ~RenameDlg();
private slots:
    void on_btnRename_clicked();
private:
    Ui::RenameDlg *ui;
    QString src_folder_path_;
    QString dst_folder_path_;
    QString file_type_;
    QString sn_;
    QString board_id_;
    QString class_id_;
    QString date_;
    QString suffix_;
    QString index_;
    QString name_;
};

#endif // RENAMEDLG_H
