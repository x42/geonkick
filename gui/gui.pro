######################################################################
# Automatically generated by qmake (3.0) Sun Oct 15 13:16:09 2017
######################################################################

QMAKE_CXXFLAGS += -std=c++1y
TEMPLATE = app
QT = core gui widgets
TARGET = geonkick
INCLUDEPATH += .
INCLUDEPATH += ../api/src/

LIBS += -L../api/ -lgeonkick -ljack

# Input
HEADERS += globals.h \
           geonkick_theme.h \
           geonkick_widget.h \
           geonkick_label.h \
           geonkick_groupbox.h \
           control_area.h \
           envelope_draw_area.h \
           envelope_point.h \
           envelopes_group_box.h \
           general_envelope.h \
           general_group_box.h \
           gkick_envelope.h \
           gkick_envelope_widget.h \
           gkick_knob.h \
           gkick_oscillator.h \
           gkickapi.h \
           oscillator_envelope.h \
           oscillator_group_box.h \
           mainwindow.h

SOURCES += geonkick_theme.cpp \
           geonkick_widget.cpp \
           geonkick_label.cpp \
           geonkick_groupbox.cpp \
           control_area.cpp \
           envelope_draw_area.cpp \
           envelope_point.cpp \
           envelopes_group_box.cpp \
           general_envelope.cpp \
           general_group_box.cpp \
           gkick_envelope.cpp \
           gkick_envelope_widget.cpp \
           gkick_knob.cpp \
           gkick_oscillator.cpp \
           gkickapi.cpp \
           oscillator_envelope.cpp \
           oscillator_group_box.cpp \
           mainwindow.cpp \
           main.cpp
