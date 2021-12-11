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
        val = 0;
        return;
    }
    else
    {
        QPalette p;
        ui->input_edit->setPalette(p);
    }

    bool has_job = current_job.has_value();
    current_job = static_cast<uint64_t>(val);

    if (!has_job)
    {
        try
        {
            factor();
        }
        catch (...)
        {
            current_job = std::nullopt;
            throw;
        }
        current_job = std::nullopt;
    }
}

void main_window::factor()
{
retry:
    uint64_t const initial_val = *current_job;
    uint64_t val = initial_val;
    if (val == 0)
    {
        ui->output_label->setText("");
        return;
    }

    std::vector<uint64_t> result;
    ui->output_label->setText(format_output(initial_val, result, true));

    while ((val % 2) == 0)
    {
        QApplication::processEvents(QEventLoop::AllEvents);
        if (initial_val != current_job)
            goto retry;

        result.push_back(2);
        ui->output_label->setText(format_output(initial_val, result, true));
        val /= 2;
    }

    uint64_t current_divisor = 3;

    for (;;)
    {
        QApplication::processEvents(QEventLoop::AllEvents);
        if (initial_val != current_job)
            goto retry;

        if (val == 1)
            break;

        if ((val % current_divisor) == 0)
        {
            result.push_back(current_divisor);
            ui->output_label->setText(format_output(initial_val, result, true));
            val /= current_divisor;
        }
        else
            current_divisor += 2;
    }

    if (result.empty())
    {
        assert(val == 1);
        result.push_back(1);
    }

    ui->output_label->setText(format_output(initial_val, result, false));
}

QString main_window::format_output(uint64_t val, std::vector<uint64_t> const& factors, bool partial)
{
    assert(!factors.empty() || partial);

    std::stringstream ss;
    ss << val << " =\n";
    
    for (size_t i = 0; i != factors.size(); ++i)
    {
        ss << "\t";
        ss << factors[i];
        if ((i + 1) != factors.size() || partial)
            ss << " ×";
        ss << '\n';
    }

    if (partial)
        ss << "\t…\n";

    return QString::fromStdString(ss.str());
}
