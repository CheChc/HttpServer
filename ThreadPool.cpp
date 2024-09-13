#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t numThreads) {
    start(numThreads);
}

ThreadPool::~ThreadPool() {
    stop();
}

void ThreadPool::enqueue(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(mEventMutex);
        mTasks.emplace(std::move(task));
    }
    mEventVar.notify_one();
}

void ThreadPool::start(size_t numThreads) {
    for (size_t i = 0; i < numThreads; ++i) {
        mThreads.emplace_back([this] {
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(mEventMutex);
                    mEventVar.wait(lock, [this] { return mStopping || !mTasks.empty(); });
                    if (mStopping && mTasks.empty())
                        break;
                    task = std::move(mTasks.front());
                    mTasks.pop();
                }
                task();
            }
        });
    }
}

void ThreadPool::stop() {
    {
        std::unique_lock<std::mutex> lock(mEventMutex);
        mStopping = true;
    }
    mEventVar.notify_all();
    for (auto& thread : mThreads) {
        thread.join();
    }
}

