TEMPLATE = app
TARGET = 2048
INCLUDEPATH += .

CONFIG += c++11

QT += qml quick

HEADERS += \
2048/reactview.h \
2048/reactbridge.h \
2048/reactpipeexecutor.h \
2048/reactnetexecutor.h \
2048/reactmoduleinterface.h \
2048/reactmoduledata.h \
2048/reactmodulemethod.h \
2048/reactcomponentdata.h \
2048/reactnetworking.h \
2048/reacttiming.h \
2048/ubuntuuimanager.h \
2048/ubuntuviewmanager.h \
2048/ubuntuview.h \
2048/ubunturawtextmanager.h \
2048/ubuntutextmanager.h \
2048/ubuntuactivitymanager.h \
2048/ubuntuscrollviewmanager.h \

SOURCES += \
2048/main.cpp \
2048/reactview.cpp \
2048/reactbridge.cpp \
2048/reactpipeexecutor.cpp \
2048/reactnetexecutor.cpp \
2048/reactmoduledata.cpp \
2048/reactmodulemethod.cpp \
2048/reactcomponentdata.cpp \
2048/reactnetworking.cpp \
2048/reacttiming.cpp \
2048/ubuntuuimanager.cpp \
2048/ubuntuviewmanager.cpp \
2048/ubuntuview.cpp \
2048/ubunturawtextmanager.cpp \
2048/ubuntutextmanager.cpp \
2048/ubuntuactivitymanager.cpp \
2048/ubuntuscrollviewmanager.cpp \

RESOURCES += 2048/2048.qrc

TARGET = react-demo
