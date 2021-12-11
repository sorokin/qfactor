#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <memory>
#include <vector>
#include "factoring_worker.h"

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
    QString format_output(factoring_result const& result);

private slots:
    void update_output();
    
private:
    std::unique_ptr<Ui::main_window> ui;
    factoring_worker worker;
};

#endif // MAIN_WINDOW_H
