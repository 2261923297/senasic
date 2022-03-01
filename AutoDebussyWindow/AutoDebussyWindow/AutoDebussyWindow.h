#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_AutoDebussyWindow.h"

class AutoDebussyWindow : public QMainWindow
{
    Q_OBJECT

public:
    AutoDebussyWindow(QWidget *parent = Q_NULLPTR);

private:
    Ui::AutoDebussyWindowClass ui;
};
