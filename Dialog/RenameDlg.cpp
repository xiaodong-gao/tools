#include "RenameDlg.h"
#include "ui_renamedlg.h"

RenameDlg::RenameDlg(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::RenameDlg)
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

    QObject::connect(ui->comboBoxType, &QComboBox::currentTextChanged,this, [&](const QString& text) {
        file_type_ = text;
    });
    ui->comboBoxType->addItem(tr(".bmp"));
    ui->comboBoxType->addItem(tr(".png"));
    ui->comboBoxType->addItem(tr(".jpg"));
    ui->comboBoxType->addItem(tr(".txt"));
    ui->comboBoxType->addItem(tr(".xml"));
    ui->comboBoxType->addItem(tr(".json"));
    ui->comboBoxType->setCurrentText(tr(".bmp"));


    QObject::connect(ui->lineEditSN, &QLineEdit::textChanged, [&](const QString& text) {
        sn_ = text;
    });

    QObject::connect(ui->comboBoxBoardId, &QComboBox::currentTextChanged,this, [&](const QString& text) {
        board_id_ = text;
    });
    ui->comboBoxBoardId->addItem("0-green");
    ui->comboBoxBoardId->addItem("1-black");
    ui->comboBoxBoardId->addItem("2-blue");
    ui->comboBoxBoardId->addItem("3-red");
    ui->comboBoxBoardId->setCurrentText(tr("0-green"));


    QObject::connect(ui->comboBoxClassId, &QComboBox::currentTextChanged,this, [&](const QString& text) {
        class_id_ = text;
    });
    for(int i = 0; i < 40; i++)
        ui->comboBoxClassId->addItem(QString::number(i));
    ui->comboBoxClassId->setCurrentText(tr("0"));

    QObject::connect(ui->lineEditDate, &QLineEdit::textChanged, [&](const QString& text) {
        date_ = text;
    });

    QString currentDate = QDateTime::currentDateTime().date().toString("yyyy-MM-dd");
    ui->lineEditDate->setText(currentDate);

    QObject::connect(ui->lineEditSuffix, &QLineEdit::textChanged, [&](const QString& text) {
        suffix_ = text;
    });

    QObject::connect(ui->lineEditName, &QLineEdit::textChanged, [&](const QString& text) {
        name_ = text;
    });

    QObject::connect(ui->lineEditIndex, &QLineEdit::textChanged, [&](const QString& text) {
        index_ = text;
    });
    ui->lineEditIndex->setText(tr("0"));
}

RenameDlg::~RenameDlg()
{
    delete ui;
}

void RenameDlg::on_btnRename_clicked()
{
    // 创建 QDir 对象
    QDir folder(src_folder_path_);
    // 设置过滤器和排序
    folder.setFilter(QDir::Files | QDir::NoDotAndDotDot); // 过滤文件，并排除 "." 和 ".."
    // 设置多个文件扩展名
    QStringList nameFilters;
    QString current_type =  "*" + ui->comboBoxType->currentText();
    nameFilters << current_type;
    folder.setNameFilters(nameFilters);
    folder.setSorting(QDir::Name); // 根据文件名排序
    // 获取文件列表
    QStringList fileList = folder.entryList();
    int number_ = ui->lineEditIndex->text().toInt();
    // 打印过滤后的文件列表
    for (const QString &fileName : fileList) {
        QString src_file = src_folder_path_ + "/" + fileName;
        QString dst_file_name = sn_ + "-" +
                                QString::number(ui->comboBoxBoardId->currentIndex()) + "-" +
                                class_id_ + "-" +
                                date_ + "-" +
                                suffix_ + "-" + 
                                name_ + "-" + 
                                QString::number(number_++) + ui->comboBoxType->currentText();


        QString dst_file = dst_folder_path_ + "/" + dst_file_name;
        QFile file(src_file);
        file.copy(dst_file);

        //if (current_type == "*.xml") {
        //    MConfiguration::Instance()->modify_yolo_xml(dst_file.toStdString().c_str(), dst_file_name);
        //}

    }
    // 显示信息框
    QMessageBox::information(nullptr, "Information", "Process Finished.");
}
