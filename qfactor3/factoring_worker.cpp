#include "factoring_worker.h"
#include <cassert>

factoring_result::factoring_result(uint64_t input, std::vector<uint64_t> factors, bool incomplete)
    : input(input)
    , factors(std::move(factors))
    , partial(incomplete)
{}

factoring_worker::factoring_worker()
    : input_version(INPUT_VERSION_QUIT + 1)
    , worker_thread([this] { thread_proc(); })
{}

factoring_worker::~factoring_worker()
{
    input_version = INPUT_VERSION_QUIT;
    input_changed.notify_all();
    worker_thread.join();
}

void factoring_worker::set_input(std::optional<uint64_t> val)
{
    {
        std::lock_guard lg(m);
        input = val;
        ++input_version;
    }
    input_changed.notify_all();
}

std::optional<factoring_result> factoring_worker::get_output() const
{
    std::lock_guard lg(m);
    return output;
}

void factoring_worker::thread_proc()
{
    uint64_t last_input_version = 0;
    for (;;)
    {
        std::optional<uint64_t> input_copy;
        {
            std::unique_lock lg(m);
            input_changed.wait(lg, [&]
            {
                return input_version != last_input_version;
            });

            last_input_version = input_version;
            if (last_input_version == INPUT_VERSION_QUIT)
                break;

            input_copy = input;
        }

        std::optional<factoring_result> result;
        if (input_copy)
            factor(last_input_version, *input_copy);
        else
            store_result(std::nullopt);
    }
}

void factoring_worker::factor(uint64_t last_input_version, uint64_t val)
{
    assert(val >= 1);
    uint64_t const initial_val = val;
    std::vector<uint64_t> factors;
    store_result(factoring_result(initial_val, factors, true));

    while ((val % 2) == 0)
    {
        if (last_input_version != input_version)
            return;

        factors.push_back(2);
        store_result(factoring_result(initial_val, factors, true));
        val /= 2;
    }

    uint64_t current_divisor = 3;

    for (;;)
    {
        if (last_input_version != input_version)
            return;

        if (val == 1)
            break;

        if ((val % current_divisor) == 0)
        {
            factors.push_back(current_divisor);
            store_result(factoring_result(initial_val, factors, true));
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

    store_result(factoring_result(initial_val, factors, false));
}

void factoring_worker::store_result(std::optional<factoring_result> const& result)
{
    std::lock_guard lg(m);
    output = result;

    if (!notify_output_queued)
    {
        QMetaObject::invokeMethod(this, "notify_output");
        notify_output_queued = true;
    }
}

void factoring_worker::notify_output()
{
    {
        std::lock_guard lg(m);
        notify_output_queued = false;
    }
    emit output_changed();
}
