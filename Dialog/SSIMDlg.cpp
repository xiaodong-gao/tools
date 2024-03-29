#include "SSIMDlg.h"
#include "ui_ssimdlg.h"

SSIMDlg::SSIMDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SSIMDlg)
{
    ui->setupUi(this);

    QObject::connect(ui->lineEditModelPath, &QLineEdit::textChanged, [&](const QString& text) {
        model_folder_path_ = text;
    });

    QObject::connect(ui->lineEditDataPath, &QLineEdit::textChanged, [&](const QString& text) {
        data_folder_path_ = text;
    });

    QObject::connect(ui->btnSelectModelPath, &QPushButton::clicked, this, [&](){
        model_folder_path_ = QFileDialog::getExistingDirectory(
            nullptr,
            "Select Folder",
            "./",
            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
        );

        // 检查用户是否选择了文件夹
        if (!model_folder_path_.isEmpty()) {
            ui->lineEditModelPath->setText(model_folder_path_);
        }
    });

    QObject::connect(ui->btnSelectDataPath, &QPushButton::clicked, this, [&](){
        data_folder_path_ = QFileDialog::getExistingDirectory(
            nullptr,
            "Select Folder",
            "./",
            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
        );

        // 检查用户是否选择了文件夹
        if (!data_folder_path_.isEmpty()) {
            ui->lineEditDataPath->setText(data_folder_path_);
        }
    });


    QObject::connect(ui->btnProcess, &QPushButton::clicked, this, [&](){
        run_ssim();
        //stop_ = false;
        //std::thread task_thread_ = std::thread(&SSIMDlg::run_ssim, this);
        //task_thread_.detach();

    }); 
}

SSIMDlg::~SSIMDlg()
{

       
    delete ui;
}

void SSIMDlg::run_ssim(){
    //while(true){
    //    if(stop_ == true){
    //        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    //        continue;
    //    }
        //filter model folder
        QDir model_folder(model_folder_path_);
        // 设置过滤器，只包括扩展名为jpg、png和bmp的文件
        QStringList model_filters;
        model_filters << "*.jpg" << "*.png" << "*.bmp" ;
        model_folder.setNameFilters(model_filters);
        // 获取符合过滤器条件的文件列表
        QStringList model_file_list = model_folder.entryList();
        // 遍历文件列表
        foreach (QString fileName, model_file_list) {
           QString model_image_name = model_folder_path_ + "/" + fileName;
           vec_model_image_.push_back(model_image_name);
        }

        QDir data_folder(data_folder_path_);
        // 设置过滤器，只包括扩展名为jpg、png和bmp的文件
        QStringList data_filters;
        data_filters << "*.jpg" << "*.png" << "*.bmp" ;
        data_folder.setNameFilters(data_filters);
        // 获取符合过滤器条件的文件列表
        QStringList data_file_list = data_folder.entryList();
        // 遍历文件列表
        foreach (QString fileName, data_file_list) {
           QString data_image_name = data_folder_path_ + "/" + fileName;
           vec_data_image_.push_back(data_image_name);
        }

        if(data_file_list.size() != model_file_list.size()){
            QMessageBox msgBox;
            msgBox.setText("model folder is equal data folder.");
            msgBox.exec();
            return;
            //stop_ = true;
            //continue;
        }

        QString ssim_folder = QDir::currentPath() + "/ssim_folder";
        // Check if the directory already exists
        if (!QDir(ssim_folder).exists()) {
            // Attempt to create the directory
            if (QDir().mkdir(ssim_folder)) {
                //qDebug() << "Directory created successfully.";
            }
            else {
                //qDebug() << "Failed to create directory.";
                //return -1;
            }
        }
        else {
            //qDebug() << "Directory already exists.";
        }

        for(int i = 0 ; i < vec_model_image_.size(); ++i){
            cv::Mat model_img = cv::imread(vec_model_image_[i].toStdString().c_str(), cv::IMREAD_GRAYSCALE);
            cv::Mat data_img = cv::imread(vec_data_image_[i].toStdString().c_str(), cv::IMREAD_GRAYSCALE);
            cv::Mat ssim_mat = computeSSIM(model_img, data_img);
            cv::Mat result_img;
            ssim_mat.convertTo(result_img, CV_8U, 255);
            //Create a QFileInfo object with the detailed path
            QFileInfo fileInfo(vec_model_image_[i]);
            //Extract the base name (file name without directory) of the file
            QString fileName = fileInfo.fileName();
            QString save_image_file = ssim_folder + "/" + fileName;
            cv::imwrite(save_image_file.toStdString().c_str(), result_img);
        }
        QMessageBox msgBox;
        msgBox.setText("run finished.");
        msgBox.exec();
       //stop_ = true;
    //}
}

cv::Mat SSIMDlg::computeSSIM(const cv::Mat& i1, const cv::Mat& i2){

     const double C1 = 6.5025, C2 = 58.5225;
     /***************************** INITS **********************************/
     int d = CV_32F;

     cv::Mat I1, I2;
     i1.convertTo(I1, d); // cannot calculate on one byte large values
     i2.convertTo(I2, d);

     cv::Mat I2_2 = I2.mul(I2); // I2^2
     cv::Mat I1_2 = I1.mul(I1); // I1^2
     cv::Mat I1_I2 = I1.mul(I2); // I1 * I2
    // 获取图像的行和列
    int rows = I2_2.rows;
    int cols = I2_2.cols;

    cv::Mat grayImage(rows, cols, CV_32F, cv::Scalar(0));

    // 遍历图像的每一个像素点
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            grayImage.at<float>(i, j) = (2 * I1_I2.at<float>(i, j)  + C1) / (I1_2.at<float>(i, j) + I2_2.at<float>(i, j) + C2) ;
            // 获取当前像素点的灰度值
            //int grayValue = grayImage.at<uchar>(i, j);
            // 输出像素点的坐标和灰度值
            //std::cout << "Pixel at (" << i << "," << j << "): ";
            //std::cout << "Gray Value=" << grayValue << std::endl;
        }
    }

    return grayImage;
}

cv::Mat SSIMDlg::getMSSIM( const cv::Mat& i1, const cv::Mat& i2)
{
    const double C1 = 6.5025, C2 = 58.5225;
    /***************************** INITS **********************************/
    int d = CV_32F;

    cv::Mat I1, I2;
    i1.convertTo(I1, d); // cannot calculate on one byte large values
    i2.convertTo(I2, d);

    cv::Mat I2_2 = I2.mul(I2); // I2^2
    cv::Mat I1_2 = I1.mul(I1); // I1^2
    cv::Mat I1_I2 = I1.mul(I2); // I1 * I2

    /*************************** END INITS **********************************/

    cv::Mat mu1, mu2; // PRELIMINARY COMPUTING
    GaussianBlur(I1, mu1, cv::Size(11, 11), 1.5);
    GaussianBlur(I2, mu2, cv::Size(11, 11), 1.5);

    cv::Mat mu1_2 = mu1.mul(mu1);
    cv::Mat mu2_2 = mu2.mul(mu2);
    cv::Mat mu1_mu2 = mu1.mul(mu2);
    cv::Mat sigma1_2, sigma2_2, sigma12;

    GaussianBlur(I1_2, sigma1_2, cv::Size(11, 11), 1.5);
    sigma1_2 -= mu1_2;

    GaussianBlur(I2_2, sigma2_2, cv::Size(11, 11), 1.5);
    sigma2_2 -= mu2_2;

    GaussianBlur(I1_I2, sigma12, cv::Size(11, 11), 1.5);
    sigma12 -= mu1_mu2;

    cv::Mat t1, t2, t3;

    t1 = 2 * mu1_mu2 + C1;
    t2 = 2 * sigma12 + C2;
    t3 = t1.mul(t2); // t3 = ((2*mu1_mu2 + C1).*(2*sigma12 + C2))

    t1 = mu1_2 + mu2_2 + C1;
    t2 = sigma1_2 + sigma2_2 + C2;
    t1 = t1.mul(t2); // t1 =((mu1_2 + mu2_2 + C1).*(sigma1_2 + sigma2_2 + C2))

    cv::Mat ssim_map;
    divide(t3, t1, ssim_map); // ssim_map = t3./t1;

    //Scalar mssim = mean( ssim_map ); // mssim = average of ssim map
    return ssim_map;
}
