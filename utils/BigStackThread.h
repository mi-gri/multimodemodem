// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

#include <QThread>

#include <functional>
#include <memory>

// A thread with an explicit, large stack.  std::thread only gets the platform
// default: 512 KB on macOS secondary threads and 1 MB on Windows, while Linux
// gives 8 MB.  The reference FT decoders and the WSPR engine need far more
// (frames of 1.4 MB), so std::thread crashed there with "stack size exceeded".
class M3BigStackThread
{
public:
    static constexpr unsigned kStackBytes = 16u * 1024u * 1024u;

    M3BigStackThread() = default;
    explicit M3BigStackThread(std::function<void()> fn) { start(std::move(fn)); }
    M3BigStackThread(const M3BigStackThread &) = delete;
    M3BigStackThread &operator=(const M3BigStackThread &) = delete;
    ~M3BigStackThread() { join(); }

    void start(std::function<void()> fn)
    {
        join();
        m_thread.reset(QThread::create(std::move(fn)));
        m_thread->setStackSize(kStackBytes);
        m_thread->start();
    }
    bool joinable() const { return m_thread != nullptr; }
    void join()
    {
        if (m_thread == nullptr) return;
        m_thread->wait();
        m_thread.reset();
    }

private:
    std::unique_ptr<QThread> m_thread;
};
