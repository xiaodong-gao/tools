#ifndef SSIMDLG_H
#define SSIMDLG_H

#include <QDialog>
#include <QLineEdit>
#include <QFileDialog>
#include <QDebug>
#include <vector>
#include <thread>
#include <chrono>
#include <QMessageBox>
#include "opencv2/opencv.hpp"
#include "DataStructure/MTaskThread.h"


namespace Ui {
class SSIMDlg;
}

class SSIMDlg : public QDialog
{
    Q_OBJECT
public:
    explicit SSIMDlg(QWidget *parent = nullptr);
    ~SSIMDlg();
    void run_ssim();
    cv::Mat getMSSIM(const cv::Mat& i1, const cv::Mat& i2);
    cv::Mat computeSSIM(const cv::Mat& i1, const cv::Mat& i2);
private:
    Ui::SSIMDlg *ui;
    QString model_folder_path_;
    QString data_folder_path_;
    std::vector<QString> vec_model_image_;
    std::vector<QString> vec_data_image_;
    bool stop_{true};
    
};

#endif // SSIMDLG_H
