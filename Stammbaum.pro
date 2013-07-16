TEMPLATE    = app
CONFIG      += qt
TARGET      = Stammbaum
QT          += xml

# install
target.path = ./
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS Stammbaum.pro
sources.path = ./
INSTALLS += target sources

#RESOURCES = combatally.qrc

#RC_FILE = combatally.rc

ICON = tree.icns

HEADERS = \
	connectionpointgraphicsitem.h \
	mainwindow.h \
	person.h \
	persondialog.h \
	persongraphicsitemconnection.h

SOURCES = \
	connectionpointgraphicsitem.cpp \
    main.cpp \
	mainwindow.cpp \
    person.cpp \
	persondialog.cpp \
	persongraphicsitemconnection.cpp

TRANSLATIONS = \
	stammbaum_de.ts \
	stammbaum_en.ts