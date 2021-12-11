#include "main_window.h"
#include <QApplication>
#include <QMetaType>
#include "worker.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<factoring_result>();
    qRegisterMetaType<uint64_t>("uint64_t");
    main_window w;
    w.show();
    
    return a.exec();
}
