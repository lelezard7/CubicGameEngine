#ifndef _CUBICGAME_ENGINE_WINDOW_
#define _CUBICGAME_ENGINE_WINDOW_

#include "../CG_ConsoleManager.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class CubicGameEngineWindow;
}
QT_END_NAMESPACE


class CubicGameEngineWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::CubicGameEngineWindow* ui_;
    CGConsoleManager* consoleManager_;

public:
    CubicGameEngineWindow(QWidget* parent = nullptr);
    ~CubicGameEngineWindow();

signals:
    void ddd(const CGConsoleManager* consoleManager);

private slots:
    void refreshErrorListColumnsSize(int, int, int);
    void on_button_filter_warning_toggled(bool checked);
    void on_button_filter_error_toggled(bool checked);

};

#endif
