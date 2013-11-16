greaterThan(QT_MAJOR_VERSION, 4):QT += widgets webkitwidgets serialport

# Add more folders to ship with the application, here
folder_01.source = settings
folder_01.target = .
folder_02.source = js
folder_02.target = .
DEPLOYMENTFOLDERS = folder_01 folder_02

# Define TOUCH_OPTIMIZED_NAVIGATION for touch optimization and flicking
#DEFINES += TOUCH_OPTIMIZED_NAVIGATION

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    timeinforeaderdriver.cpp \
    driverinstancefactory.cpp

CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++11

# Please do not modify the following two lines. Required for deployment.
include(html5applicationviewer/html5applicationviewer.pri)
qtcAddDeployment()

HEADERS += \
    timeinforeaderdriver.h \
    driverinstancefactory.h

OTHER_FILES += \
    js/login.js \
    js/drivers.js \
    settings/settings.ini \
    .gitignore
