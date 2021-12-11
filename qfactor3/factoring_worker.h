#pragma once
#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <mutex>
#include <thread>
#include <optional>
#include <vector>
#include <QObject>

struct factoring_result
{
    factoring_result(uint64_t input, std::vector<uint64_t> factors, bool partial);

    uint64_t input;
    std::vector<uint64_t> factors;
    bool partial;
};

class factoring_worker : public QObject
{
    Q_OBJECT

public:
    factoring_worker();
    ~factoring_worker();

    void set_input(std::optional<uint64_t> val);
    std::optional<factoring_result> get_output() const;

signals:
    void output_changed();

private:
    void thread_proc();
    void factor(uint64_t last_input_version, uint64_t val);
    void store_result(std::optional<factoring_result> const& result);

private slots:
    void notify_output();

private:
    mutable std::mutex m;
    std::atomic<uint64_t> input_version;
    std::condition_variable input_changed;
    std::optional<uint64_t> input;
    std::optional<factoring_result> output;
    bool notify_output_queued = false;
    std::thread worker_thread;

    static uint64_t const INPUT_VERSION_QUIT = 0;
};
