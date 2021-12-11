#include "main_window.h"
#include "ui_main_window.h"
#include <cassert>
#include <sstream>
#include <QEventLoop>

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
    unsigned long long val = ui->input_edit->text().toULongLong(&ok);
    if (!ok || val == 0)
    {
        QPalette p;
        p.setColor(QPalette::Text, Qt::red);
        ui->input_edit->setPalette(p);
        ui->output_label->setText("");
        return;
    }
    else
    {
        QPalette p;
        ui->input_edit->setPalette(p);
    }

    uint64_t uval = static_cast<uint64_t>(val);
    ui->output_label->setText(format_output(uval, factor(uval)));
}

std::vector<uint64_t> main_window::factor(uint64_t val)
{
    assert(val >= 1);
    std::vector<uint64_t> result;

    while ((val % 2) == 0)
    {
        QApplication::processEvents(QEventLoop::AllEvents);
        result.push_back(2);
        val /= 2;
    }

    uint64_t current_divisor = 3;

    for (;;)
    {
        QApplication::processEvents(QEventLoop::AllEvents);
        if (val == 1)
            break;

        if ((val % current_divisor) == 0)
        {
            result.push_back(current_divisor);
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

    return result;
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
