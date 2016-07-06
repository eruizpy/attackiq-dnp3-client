//
// Created by user on 7/6/16.
//

#ifndef ATTACKIQ_DNP3_CLIENT_SYNCVAR_H
#define ATTACKIQ_DNP3_CLIENT_SYNCVAR_H

#include <mutex>
#include <condition_variable>
#include <chrono>

template <class T>
class SyncVar
{
public:

    SyncVar(const T& value) : m_value(value)
    {}

    void Set(const T& value)
    {
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_value = value;
        }
        m_cv.notify_all();
    }

    T Get()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_value;
    }

    bool WaitFor(const T& value, std::chrono::steady_clock::duration timeout)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        auto condition = [this, value]() -> bool { return this->m_value == value; };
        return m_cv.wait_for(lock, timeout, condition);
    }

private:

    SyncVar() = delete;
    SyncVar(const SyncVar&) = delete;

    std::mutex m_mutex;
    std::condition_variable m_cv;
    T m_value;
};

#endif //ATTACKIQ_DNP3_CLIENT_SYNCVAR_H
