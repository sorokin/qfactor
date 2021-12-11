#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <memory>
#include <vector>
#include <optional>

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
    void factor();
    QString format_output(uint64_t val, std::vector<uint64_t> const& factors, bool partial);

private:
    std::unique_ptr<Ui::main_window> ui;
    std::optional<uint64_t> current_job;
};

#endif // MAIN_WINDOW_H
