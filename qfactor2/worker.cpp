#include "worker.h"

factoring_result::factoring_result()
    : success(false)
    , input_val(0)
{}

factoring_result::factoring_result(uint64_t input_val, std::vector<uint64_t> factors)
    : success(true)
    , input_val(input_val)
    , factors(std::move(factors))
{}

void worker::factor(uint64_t input_val)
{
    uint64_t val = input_val;
    if (val == 0)
    {
        emit factoring_finished(factoring_result());
        return;
    }

    std::vector<uint64_t> factors;

    while ((val % 2) == 0)
    {
        factors.push_back(2);
        val /= 2;
    }

    uint64_t current_divisor = 3;

    for (;;)
    {
        if (val == 1)
            break;

        if ((val % current_divisor) == 0)
        {
            factors.push_back(current_divisor);
            val /= current_divisor;
        }
        else
            current_divisor += 2;
    }

    if (factors.empty())
    {
        assert(val == 1);
        factors.push_back(1);
    }

    emit factoring_finished(factoring_result(input_val, factors));
}
