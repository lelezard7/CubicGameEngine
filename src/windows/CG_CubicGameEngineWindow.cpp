#include "CG_CubicGameEngineWindow.h"
#include "./ui_CubicGameEngineWindow.h"


CubicGameEngineWindow::CubicGameEngineWindow(QWidget* parent) : QMainWindow(parent)
{
    ui_ = new Ui::CubicGameEngineWindow;
    ui_->setupUi(this);

    consoleManager_ = new CGConsoleManager(ui_);

    connect(ui_->errorList->horizontalHeader(), &QHeaderView::sectionResized,
            this, &CubicGameEngineWindow::refreshErrorListColumnsSize);

    connect(this, &CubicGameEngineWindow::ddd, ui_->viewport, &Viewport::ddd);
    emit ddd(consoleManager_);
}

CubicGameEngineWindow::~CubicGameEngineWindow()
{
    delete consoleManager_;

    delete ui_;
}

void CubicGameEngineWindow::refreshErrorListColumnsSize(int, int, int)
{
    consoleManager_->refreshErrorListColumnsSize();
}

void CubicGameEngineWindow::on_button_filter_warning_toggled(bool checked)
{
    if (checked)
        consoleManager_->showWarningMessages(true);
    else
        consoleManager_->showWarningMessages(false);

}

void CubicGameEngineWindow::on_button_filter_error_toggled(bool checked)
{
    if (checked)
        consoleManager_->showErrorMessages(true);
    else
        consoleManager_->showErrorMessages(false);
}
