#include "main_window.h"
#include "ui_main_window.h"
#include <sstream>

main_window::main_window(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::main_window)
{
    ui->setupUi(this);
    worker_obj.moveToThread(&worker_thread);
    connect(ui->input_edit, &QLineEdit::textChanged, this, &main_window::input_changed);
    connect(&worker_obj, &worker::factoring_finished, this, &main_window::factoring_finished);
    worker_thread.start();
}

main_window::~main_window()
{
    worker_thread.quit();
    worker_thread.wait();
}

void main_window::input_changed()
{
    bool ok;
    long long val = ui->input_edit->text().toLongLong(&ok);
    if (!ok || val <= 0)
    {
        QPalette p;
        p.setColor(QPalette::Text, Qt::red);
        ui->input_edit->setPalette(p);

        val = 0;
    }
    else
    {
        QPalette p;
        ui->input_edit->setPalette(p);
    }

    uint64_t uval = static_cast<uint64_t>(val);
    QMetaObject::invokeMethod(&worker_obj, "factor", Qt::QueuedConnection, Q_ARG(uint64_t, uval));
}

QString main_window::format_output(uint64_t val, std::vector<uint64_t> const& factors)
{
    assert(!factors.empty());

    std::stringstream ss;
    ss << val << " =\n";
    
    for (size_t i = 0; i != factors.size(); ++i)
    {
        ss << "\t";
        ss << factors[i];
        if ((i + 1) != factors.size())
            ss << " ×";
        ss << '\n';
    }
    
    return QString::fromStdString(ss.str());
}

void main_window::factoring_finished(factoring_result result)
{
    if (!result.success)
    {
        ui->output_label->setText("");
        return;
    }
    ui->output_label->setText(format_output(result.input_val, result.factors));
}
