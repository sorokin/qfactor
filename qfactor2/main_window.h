#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QThread>
#include <memory>
#include "worker.h"

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
    QString format_output(uint64_t val, std::vector<uint64_t> const& factors);

private slots:
    void factoring_finished(factoring_result result);

private:
    std::unique_ptr<Ui::main_window> ui;
    QThread worker_thread;
    worker worker_obj;
};

#endif // MAIN_WINDOW_H
