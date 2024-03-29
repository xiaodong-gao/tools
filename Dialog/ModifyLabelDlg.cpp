#include "ModifyLabelDlg.h"
#include "ui_modifylabeldlg.h"

ModifyLabelDlg::ModifyLabelDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModifyLabelDlg)
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

    /*QObject::connect(ui->lineEditSrcLabelName, &QLineEdit::textChanged, [&](const QString& text) {
        src_label_name_ = text;
    });

    QObject::connect(ui->lineEditDstLabelName, &QLineEdit::textChanged, [&](const QString& text) {
        dst_label_name_ = text;
    });
    */

    QString src_label_path =  QDir::currentPath() + "/Configuration/modify_src_label.txt";
    QString dst_label_path =  QDir::currentPath() + "/Configuration/modify_dst_label.txt";
    std::vector<std::string> obj_src_names = objects_names_from_file(src_label_path.toStdString());
    std::vector<std::string> obj_dst_names = objects_names_from_file(dst_label_path.toStdString());

    for (std::vector<std::string>::iterator iter = obj_src_names.begin(); iter != obj_src_names.end(); ++iter) {
        ui->comboBoxSrcLabelName->addItem(iter->c_str());
    }

    for (std::vector<std::string>::iterator iter = obj_dst_names.begin(); iter != obj_dst_names.end(); ++iter) {
        ui->comboBoxDstLabelName->addItem(iter->c_str());
    }

    QObject::connect(ui->comboBoxSrcLabelName, &QComboBox::currentTextChanged, this, [&](const QString& text) {
        ui->comboBoxSrcLabelName->setCurrentText(text);
        src_label_name_ = text;
    });

    QObject::connect(ui->comboBoxDstLabelName, &QComboBox::currentTextChanged, this, [&](const QString& text) {
        ui->comboBoxDstLabelName->setCurrentText(text);
        dst_label_name_ = text;
    });

    QObject::connect(ui->btnModify, &QPushButton::clicked, this, [&](){
        modify_label();
    });

}

ModifyLabelDlg::~ModifyLabelDlg()
{
    delete ui;
}

std::vector<std::string> ModifyLabelDlg::objects_names_from_file(std::string const filename) {
    std::ifstream file(filename);
    std::vector<std::string> file_lines;
    if (!file.is_open())
        return file_lines;
    for (std::string line; file >> line;)
        file_lines.push_back(line);
    //std::cout << "object names loaded \n";
    return file_lines;
}

void ModifyLabelDlg::modify_label(){
    if(src_folder_path_.isEmpty()){
        QMessageBox msgBox;
        msgBox.setText("path is not find.");
        msgBox.exec();
        return;
    }
    QDir dir(src_folder_path_);
    QStringList filters;
    filters << "*.xml";
    dir.setNameFilters(filters);

    QStringList xmlFiles;
    QFileInfoList fileList = dir.entryInfoList();
    foreach (QFileInfo fileInfo, fileList) {
        QString xmlFilePath = fileInfo.filePath();
        tinyxml2::XMLDocument doc;
        if (doc.LoadFile(xmlFilePath.toStdString().c_str()) != tinyxml2::XML_SUCCESS) {
            return ;
        }
        // Access the root element
        tinyxml2::XMLElement* root = doc.RootElement();
        // Access each <object> element
        for (tinyxml2::XMLElement* objectElement = root->FirstChildElement("object"); objectElement; objectElement = objectElement->NextSiblingElement("object")) {
            const char* name = objectElement->FirstChildElement("name")->GetText();
            if(std::string(name) == src_label_name_.toStdString()){
                objectElement->FirstChildElement("name")->SetText(dst_label_name_.toStdString().c_str());
            }
        }
        doc.SaveFile(xmlFilePath.toStdString().c_str());
    }
}
