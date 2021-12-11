#pragma once
#include <QObject>

struct factoring_result
{
    factoring_result();
    factoring_result(uint64_t input_val, std::vector<uint64_t> factors);

    bool success;
    uint64_t input_val;
    std::vector<uint64_t> factors;
};

Q_DECLARE_METATYPE(factoring_result);

class worker : public QObject
{
    Q_OBJECT

public slots:
    void factor(uint64_t val);

signals:
    void factoring_finished(factoring_result result);
};
