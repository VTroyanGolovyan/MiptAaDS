#include <mutex>
#include <condition_variable>
#include <functional>
#include <thread>
#include <vector>
#include <queue>
#include <iostream>
#include <optional>
#include <future>

namespace tp {


  template<typename T>
  class BlockingQueue {
  public:
    BlockingQueue() = default;

    bool Push(const T& value) {
      std::lock_guard<std::mutex> lock(m_);
      if (closed_) { return false; }
      queue_.push_back(value);
      wait_task_.notify_one();
      return true;
    }

    std::optional<T> Take() {
      std::unique_lock<std::mutex> lock(m_);
      while (queue_.empty() && !closed_) {
        wait_task_.wait(lock);
      }
      if (queue_.empty()) {
        return std::nullopt;
      }
      T value = std::move(queue_.front());
      queue_.pop_front();
      return value;
    }

    bool Empty() {
      std::lock_guard<std::mutex> lock(m_);
      return queue_.empty();
    }

    void Close() {
      std::lock_guard<std::mutex> lock(m_);
      closed_ = true;
      wait_task_.notify_all();
    }

  private:
    std::mutex m_;
    std::condition_variable wait_task_;
    bool closed_;
    std::deque<T> queue_;
  };

  class ThreadPool;

  thread_local ThreadPool* current_tp;

  inline ThreadPool* CurrentThreadPool() {
    return current_tp;
  }

  class ThreadPool {
    public:
    ThreadPool(size_t n) {
      for (size_t i = 0; i < n; ++i) {
        threads_.emplace_back([this] {
          Worker();
        });
      }
    }

    ~ThreadPool() = default;
  
    void Submit(const std::function<void(void)>& task) {
      tasks_cnt_.fetch_add(1);
      task_queue_.Push(task);
    }

    void Join() {
      joined_.store(true);
      for (auto& th : threads_) {
        th.join();
      }
    }

    void Worker() {
      current_tp = this;
      while (auto task = task_queue_.Take()) {
        (*task)();
        if (tasks_cnt_.fetch_sub(1) == 1 && joined_) {
          task_queue_.Close();
        }
      }
    }

    private:
      std::vector<std::thread> threads_;
      std::atomic<size_t> tasks_cnt_{};
      std::atomic<bool> joined_{false};
      BlockingQueue<std::function<void(void)>> task_queue_;
  };

}
int main() {
  tp::ThreadPool tp(std::thread::hardware_concurrency());
  tp.Submit([](){
    std::cout << "KEK" << std::endl;
  });
  tp.Submit([](){
    std::cout << "KEK" << std::endl;
    tp::CurrentThreadPool()->Submit([](){
      std::cout << "Ya next" << std::endl;
    });
  });
  tp.Submit([](){
    std::cout << "KEK" << std::endl;
  });
  tp.Submit([](){
    std::cout << "KEK" << std::endl;
  });
  tp.Submit([](){
    std::cout << "KEK" << std::endl;
  });
  std::atomic<size_t> test{0};
  std::promise<size_t> promise;
  std::future<size_t> ft = promise.get_future();
  
  for (size_t i = 0; i < 500000; ++i) {
    tp.Submit([&promise, &test, i](){ 
      ++test;
      if (i == 100000) {
        promise.set_value(1);
      }
      tp::CurrentThreadPool()->Submit([&](){
         ++test;
      });
    });
  }
  std::cout << ft.get() << std::endl;
  tp.Join();
  std::cout << "KEK\n";
  std::cout << test;
}
