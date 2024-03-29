QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# Set include paths
INCLUDEPATH += "C:/Users/xiaod/Desktop/3dParty/tinyxml2" \
               "C:/Users/xiaod/Desktop/3dParty/filesystem/include" \
               "C:/Users/xiaod/Desktop/3dParty/opencv-4.9.0-windows/build/include"

CONFIG(debug, debug|release) {
    # Debug 模式的配置
    # Set library paths
    LIBS += "C:/Users/xiaod/Desktop/3dParty/tinyxml2/install-debug/lib/tinyxml2.lib" \
            "C:/Users/xiaod/Desktop/3dParty/opencv-4.9.0-windows/build/x64/vc16/lib/opencv_world490d.lib"
} else {
    # Release 模式的配置
    LIBS += "C:/Users/xiaod/Desktop/3dParty/tinyxml2/install-release/lib/tinyxml2.lib" \
            "C:/Users/xiaod/Desktop/3dParty/opencv-4.9.0-windows/build/x64/vc16/lib/opencv_world490.lib"
}

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    DataStructure/MTaskThread.cpp \
    Dialog/ExtractDataDlg.cpp \
    Dialog/AnalysisXmlDialog.cpp \
    Dialog/ModifyLabelDlg.cpp \
    Dialog/RenameDlg.cpp \
    Dialog/SSIMDlg.cpp \
    Dialog/mainwindow.cpp \
    main.cpp

HEADERS += \
    DataStructure/MTaskThread.h \
    Dialog/ExtractDataDlg.h \
    Dialog/AnalysisXmlDialog.h \
    Dialog/ModifyLabelDlg.h \
    Dialog/RenameDlg.h \
    Dialog/SSIMDlg.h \
    Dialog/mainwindow.h

FORMS += \
    Dialog/extractdatadlg.ui \
    Dialog/analysisxmldialog.ui \
    Dialog/mainwindow.ui \
    Dialog/modifylabeldlg.ui \
    Dialog/renamedlg.ui \
    Dialog/ssimdlg.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
