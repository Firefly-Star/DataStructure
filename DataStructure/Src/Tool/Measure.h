#pragma once

#include <string_view>
#include <chrono>
#include <functional>
#include <mutex>
#include <iostream>
#include <thread>

namespace my_stl
{
    using namespace std::chrono;

    inline std::mutex mtx_cout;

    template<typename Func>
    void measure(Func&& func, std::string_view s = "", std::chrono::milliseconds timeout = 3000ms)
    {
        struct Data
        {
            std::mutex mtx_flag;
            bool completed = false;
            std::condition_variable cv;
        };
        Data* data = new Data;
        auto work = [&]()
            {
                std::chrono::time_point start = std::chrono::steady_clock::now();
                func();
                {
                    std::chrono::time_point finish = std::chrono::steady_clock::now();
                    std::unique_lock<std::mutex> lk(data->mtx_flag);
                    if (!data->completed)
                    {
                        data->completed = true;
                        uint64_t duration = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count();
                        {
                            std::unique_lock<std::mutex> lk(mtx_cout);
                            std::cout << s << " takes " << duration << " ms.\n";
                        }
                        data->cv.notify_one();
                    }
                    else
                    {
                        lk.unlock();
                        delete data;
                    }
                }
            };
        std::thread t(work);
        {
            std::unique_lock<std::mutex> lk(data->mtx_flag);
            if (!data->cv.wait_for(lk, timeout, [&]() { return data->completed; }))
            {
                data->completed = true;
                {
                    std::unique_lock<std::mutex> lk(mtx_cout);
                    std::cout << s << " timeout!\n";
                }
            }
            else
            {
                lk.unlock();
                delete data;
            }
        }
        t.join();
    }
}