#include "AnalysisXmlDialog.h"
#include "ui_analysisxmldialog.h"

AnalysisXmlDialog::AnalysisXmlDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AnalysisXmlDialog)
{
    ui->setupUi(this);

    QObject::connect(ui->lineEditSrcPath, &QLineEdit::textChanged, [&](const QString& text) {
        src_folder_path_ = text;
    });

    QObject::connect(ui->btnSelectSrcPath, &QPushButton::clicked, this, [&](){
        src_folder_path_ = QFileDialog::getExistingDirectory(
            nullptr,
            "Select Folder",
            "./",
            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
        );

        // 检查用户是否选择了文件夹
        if (!src_folder_path_.isEmpty()) {
            ui->lineEditSrcPath->setText(src_folder_path_);
        }
    });

    QObject::connect(ui->btnRun, &QPushButton::clicked, this, [&](){
        analysys_xml();
    });
}

AnalysisXmlDialog::~AnalysisXmlDialog()
{
    delete ui;
}

int AnalysisXmlDialog::analysis_xml_format(const char* xmlFilePath, QString image_name) {
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(xmlFilePath) != tinyxml2::XML_SUCCESS) {
        return -1;
    }
    // Access the root element
    tinyxml2::XMLElement* root = doc.RootElement();
    //tinyxml2::XMLElement* folderElement = root->FirstChildElement("folder");
    //folderElement->SetText("datasets");
    //tinyxml2::XMLElement* fileNameElement = root->FirstChildElement("filename");
    //fileNameElement->SetText(image_name.toStdString().c_str());
    //tinyxml2::XMLElement* pathElement = root->FirstChildElement("path");
    //pathElement->SetText(image_name.toStdString().c_str());

    // Extract size
    tinyxml2::XMLElement* sizeElement = root->FirstChildElement("size");
    int width = 0, height = 0, depth = 0;
    sizeElement->FirstChildElement("width")->QueryIntText(&width);
    sizeElement->FirstChildElement("height")->QueryIntText(&height);
    sizeElement->FirstChildElement("depth")->QueryIntText(&depth);
    if (width == 0 || height == 0 || depth == 0)
        return -2;

    // Access each <object> element
    int object_size = 0;
    for (tinyxml2::XMLElement* objectElement = root->FirstChildElement("object"); objectElement; objectElement = objectElement->NextSiblingElement("object")) {
        /*const char* name = objectElement->FirstChildElement("name")->GetText();
        for (size_t i = 0; i < vec_names_.size(); ++i) {
            size_t found = vec_names_[i].find(std::string(name));
            if (found != std::string::npos) {
                return -3;
            }
        }*/
        ++object_size;
    }

    if (object_size == 0)
        return -3;
    return 0;
}

void AnalysisXmlDialog::analysys_xml(){
    QString filePath = QDir::currentPath() + "/Configuration/log.txt";
    std::ofstream outputFile(filePath.toStdString().c_str());
    if (!outputFile.is_open()) {
        return;
    }
    QDir xml_folder(src_folder_path_);
    QDir images_folder(src_folder_path_);
    xml_folder.setFilter(QDir::Files | QDir::NoDotAndDotDot); // 过滤文件，并排除 "." 和 ".."
    images_folder.setFilter(QDir::Files | QDir::NoDotAndDotDot); // 过滤文件，并排除 "." 和 ".."

    QStringList xml_filters, images_filters;
    xml_filters << "*.xml";
    images_filters << "*.bmp";
    xml_folder.setNameFilters(xml_filters);
    images_folder.setNameFilters(images_filters);
    xml_folder.setSorting(QDir::Name); // 根据文件名排序
    images_folder.setSorting(QDir::Name);
    // 获取文件列表
    QStringList xml_file_list = xml_folder.entryList();
    QStringList images_file_list = images_folder.entryList();

    int xml_count = xml_file_list.size();
    int images_count = images_file_list.size();

    outputFile << "images size:" << images_count << std::endl << "xml size:" << xml_count << std::endl;

    for (const QString& imageName : images_file_list) {
        QString src_image_file_path = src_folder_path_ + "/" + imageName;
        QString src_xml_file_path = src_folder_path_ + "/" + (imageName.split('.'))[0] + ".xml";

        QFile xml_file(src_xml_file_path);
        if (!xml_file.exists()) {
            outputFile << imageName.toStdString() << " can't find " << ((imageName.split('.'))[0] + ".xml").toStdString() << std::endl;
        }

    }

    for (const QString& fileName : xml_file_list) {
        QString src_xml_file_path = src_folder_path_ + "/" + fileName;
        QString src_image_file_path = src_folder_path_ + "/" + (fileName.split('.'))[0] + ".bmp";

        QFile xml_file(src_xml_file_path);
        QFile image_file(src_image_file_path);

        if (xml_file.exists() && !image_file.exists()) {
            outputFile << ((fileName.split('.'))[0] + ".xml").toStdString() << " can't find." << ((fileName.split('.'))[0] + ".bmp").toStdString() << std::endl;
        }
        else {
            int flag = analysis_xml_format(src_xml_file_path.toStdString().c_str(), (fileName.split('.'))[0] + ".bmp");
            if (flag != 0) {
                outputFile << fileName.toStdString() << " has errors." << std::endl;
            }
        }
        
        //QFile file(src_image_file_path);
        //if (flag == 0 && file.exists()) {
        //    //outputFile << " file analysis is ok." << std::endl;
        //}
        //else if (flag == -1 && file.exists()) {
        //    outputFile << fileName.toStdString() << " has errors." << std::endl;
        //}
        //else if (flag == -2 && file.exists()) {
        //    outputFile << ((fileName.split('.'))[0] + ".xml").toStdString() << " can't find." << ((fileName.split('.'))[0] + ".bmp").toStdString() << std::endl;
        //}
        //else if (flag ==-3 && file.exists()) {
        //    outputFile << ((fileName.split('.'))[0] + ".xml").toStdString() << " width height depth is null" << std::endl;
        //}
    }
    outputFile.close();
    QMessageBox::information(nullptr, "info", "analysis finished.");
}


