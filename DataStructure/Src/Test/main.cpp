#include <string>
#include <iostream>
#include <vector>
#include <functional>
#include <omp.h>
#include <mutex>
#include <memory>
#include <cmath>
#include <array>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <span>
#include <set>
#include <random>
#include <chrono>
#include <future>
#include "Implementation/Algorithm/Sort.h"
#include "Implementation/Algorithm/Heap.h"
using namespace std::chrono;

std::vector<size_t> generate(size_t number, size_t k)
{
    std::vector<size_t> vec;
    for (size_t i = 1; i <= number; ++i) {
        vec.push_back(i);
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dist(0, number - 1);
    for (size_t i = 0; i < k; ++i) {
        size_t a = dist(gen);
        size_t b = dist(gen);
        std::swap(vec[a], vec[b]);
    }
    return vec;
}

template<typename UnidIt, class Pred = my_stl::less<typename UnidIt::value_type>>
bool check(UnidIt first, UnidIt last, Pred pred = Pred())
{
    bool all_valid = true;
#pragma omp parallel for reduction(&: all_valid)
    for (; first != last - 1; ++first)
    {
        if (!pred(*first, *(first + 1)))
        {
            all_valid = false;
        }
    }
    return all_valid;
}
static std::mutex mtx_cout;

template<typename Container, class Pred = my_stl::less<typename Container::value_type>>
void check_and_print(Container const& c, std::string_view sv, Pred pred = Pred())
{
    if (check(c.begin(), c.end(), pred))
    {
        std::unique_lock<std::mutex> lk(mtx_cout);
        std::cout << sv << " sorted\n";
    }
    else
    {
        std::unique_lock<std::mutex> lk(mtx_cout);
        std::cout << sv << " unsorted\n";
    }
}

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

int main()
{
#if 1
    size_t number = 1e4;
    size_t k = 1e4;
    auto timeout = 100000ms;
    auto v = generate(number, k);
    auto v_for_bubble_sort(v);
    auto v_for_selection_sort(v);
    auto v_for_insertion_sort(v);
    auto v_for_merge_sort(v);
    auto v_for_inplace_merge_sort(v);
    auto v_for_quick_sort(v);
    auto v_for_heap_sort(v);
    auto v_for_stl_sort(v);

    std::thread bubble_sort([&]() {
        measure([&]() { my_stl::bubble_sort(v_for_bubble_sort.begin(), v_for_bubble_sort.end()); }, "bubble sort", timeout);
        });
    std::thread selection_sort([&]() {
        measure([&]() { my_stl::selection_sort(v_for_selection_sort.begin(), v_for_selection_sort.end()); }, "selection sort", timeout);
        });
    std::thread insertion_sort([&]() {
        measure([&]() { my_stl::insertion_sort(v_for_insertion_sort.begin(), v_for_insertion_sort.end()); }, "insertion sort", timeout);
        });
    std::thread merge_sort([&]() {
        measure([&]() { my_stl::merge_sort(v_for_merge_sort.begin(), v_for_merge_sort.end()); }, "merge sort", timeout);
        });
    std::thread inplace_merge_sort([&]() {
        measure([&]() { my_stl::inplace_merge_sort(v_for_inplace_merge_sort.begin(), v_for_inplace_merge_sort.end()); }, "inplace merge sort", timeout);
        });
    std::thread quick_sort([&]() {
        measure([&]() { my_stl::quick_sort(v_for_quick_sort.begin(), v_for_quick_sort.end()); }, "quick sort", timeout);
        });
    std::thread heap_sort([&]() {
        measure([&]() { my_stl::heap_sort(v_for_heap_sort.begin(), v_for_heap_sort.end()); }, "heap sort", timeout);
        });
    std::thread stl_sort([&]() {
        measure([&]() { std::sort(v_for_stl_sort.begin(), v_for_stl_sort.end()); }, "stl sort", timeout);
        });

    stl_sort.join();
    quick_sort.join();
    heap_sort.join();
    merge_sort.join();
    inplace_merge_sort.join();

    bubble_sort.join();
    selection_sort.join();
    insertion_sort.join();
#endif

    return 0;
}