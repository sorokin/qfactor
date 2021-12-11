#include "main_window.h"
#include "ui_main_window.h"
#include <cassert>
#include <sstream>

main_window::main_window(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::main_window)
{
    ui->setupUi(this);
    connect(ui->input_edit, &QLineEdit::textChanged, this, &main_window::input_changed);
    connect(&worker, &factoring_worker::output_changed, this, &main_window::update_output);
}

main_window::~main_window()
{}

void main_window::input_changed()
{
    bool ok;
    long long val = ui->input_edit->text().toLongLong(&ok);
    if (!ok || val <= 0)
    {
        QPalette p;
        p.setColor(QPalette::Text, Qt::red);
        ui->input_edit->setPalette(p);
        worker.set_input(std::nullopt);
    }
    else
    {
        QPalette p;
        ui->input_edit->setPalette(p);
        worker.set_input(static_cast<uint64_t>(val));
    }
}

QString main_window::format_output(factoring_result const& result)
{
    assert(!result.factors.empty() || result.partial);

    std::stringstream ss;
    ss << result.input << " =\n";

    for (size_t i = 0; i != result.factors.size(); ++i)
    {
        ss << "\t";
        ss << result.factors[i];
        if ((i + 1) != result.factors.size() || result.partial)
            ss << " ×";
        ss << '\n';
    }

    if (result.partial)
        ss << "\t…\n";

    return QString::fromStdString(ss.str());
}

void main_window::update_output()
{
    std::optional<factoring_result> result = worker.get_output();
    if (!result)
    {
        ui->output_label->setText("");
        return;
    }
    ui->output_label->setText(format_output(*result));
}
