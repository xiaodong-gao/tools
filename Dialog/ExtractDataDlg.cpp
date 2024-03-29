#include "ExtractDataDlg.h"
#include "ui_extractdatadlg.h"

ExtractDataDlg::ExtractDataDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExtractDataDlg)
{
    ui->setupUi(this);

    QObject::connect(ui->lineEditSrcPath, &QLineEdit::textChanged, [&](const QString& text) {
        src_folder_path_ = text;
    });

    QObject::connect(ui->lineEditDstPath, &QLineEdit::textChanged, [&](const QString& text) {
        dst_folder_path_ = text;
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

    QObject::connect(ui->btnSelectDstPath, &QPushButton::clicked, this, [&](){
        dst_folder_path_ = QFileDialog::getExistingDirectory(
            nullptr,
            "Select Folder",
            "./",
            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
        );
        // 检查用户是否选择了文件夹
        if (!dst_folder_path_.isEmpty()) {
            ui->lineEditDstPath->setText(dst_folder_path_);
        }
    });

    QString label_path = QDir::currentPath() + "/Configuration/labelImg-common.txt";
    std::vector<std::string> obj_names = objects_names_from_file(label_path.toStdString());
    for (std::vector<std::string>::iterator iter = obj_names.begin(); iter != obj_names.end(); ++iter) {
        ui->comboBoxLabel->addItem(iter->c_str());
    }
    QObject::connect(ui->comboBoxLabel, &QComboBox::currentTextChanged, this, [&](const QString& text) {
        ui->comboBoxLabel->setCurrentText(text);
    });


    QObject::connect(ui->btnExtract, &QPushButton::clicked, this, [&]() {
        extract_file_to_dst();
    });

    ui->comboBoxMode->addItem(tr("copy"));
    ui->comboBoxMode->addItem(tr("cut"));
    QObject::connect(ui->comboBoxMode, &QComboBox::currentTextChanged, [&](const QString &text) {
        mode_ = text;
    });
    ui->comboBoxMode->setCurrentText("copy");

}

ExtractDataDlg::~ExtractDataDlg()
{
    delete ui;
}

std::vector<std::string> ExtractDataDlg::objects_names_from_file(std::string const filename) {
    std::ifstream file(filename);
    std::vector<std::string> file_lines;
    if (!file.is_open()) 
        return file_lines;
    for (std::string line; file >> line;) 
        file_lines.push_back(line);
    //std::cout << "object names loaded \n";
    return file_lines;
}

void ExtractDataDlg::extract_file_to_dst() {
    // 创建 QDir 对象
    QDir folder(src_folder_path_);
    // 设置过滤器和排序
    folder.setFilter(QDir::Files | QDir::NoDotAndDotDot); // 过滤文件，并排除 "." 和 ".."
    // 设置多个文件扩展名
    QStringList nameFilters;
    nameFilters << "*.xml";
    folder.setNameFilters(nameFilters);
    folder.setSorting(QDir::Name); // 根据文件名排序
    // 获取文件列表
    QStringList fileList = folder.entryList();
    QString label_name_ = ui->comboBoxLabel->currentText();
    // 打印过滤后的文件列表
    for (const QString& fileName : fileList) {
        QString src_xml_file_path = src_folder_path_ + "/" + fileName;
        QString src_image_file_path = src_folder_path_ + "/" + (fileName.split('.'))[0] + ".bmp";
        int flag = extract_label_name(src_xml_file_path.toStdString().c_str(), label_name_.toStdString());
        if (flag == 1) {
            QString dst_xml_file_path = dst_folder_path_ + "/" + fileName;
            QFile xml_file(src_xml_file_path);
            xml_file.copy(dst_xml_file_path);
            if(mode_ == "cut"){
                xml_file.remove();
            }
            QString dst_image_file_path = dst_folder_path_ + "/" + (fileName.split('.'))[0] + ".bmp";
            QFile image_file(src_image_file_path);
            image_file.copy(dst_image_file_path);
            if(mode_ == "cut"){
                image_file.remove();
            }
        }
    }
    QMessageBox::information(nullptr, "info", "extract data finish.");
}

bool ExtractDataDlg::extract_label_name(const char* xmlFilePath, std::string label_name) {
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(xmlFilePath) != tinyxml2::XML_SUCCESS) {
        return -1;
    }
    // Access the root element
    tinyxml2::XMLElement* root = doc.RootElement();
    // Access each <object> element
    for (tinyxml2::XMLElement* objectElement = root->FirstChildElement("object"); objectElement; objectElement = objectElement->NextSiblingElement("object")) {
        const char* name = objectElement->FirstChildElement("name")->GetText();
        if (label_name == std::string(name))
            return 1;
    }
    return 0;
}
