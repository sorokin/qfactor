#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <memory>
#include <vector>

namespace Ui {
class main_window;
}

class main_window : public QMainWindow
{
    Q_OBJECT

public:
    explicit main_window(QWidget *parent = nullptr);
    ~main_window();

    void input_changed();
    std::vector<uint64_t> factor(uint64_t val);
    QString format_output(uint64_t val, std::vector<uint64_t> const& factors);

private:
    std::unique_ptr<Ui::main_window> ui;
};

#endif // MAIN_WINDOW_H
