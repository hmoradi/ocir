#include <iostream>
#include <thread>
#include <deque>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include "buffer.h"
std::mutex mu,cout_mu;
std::condition_variable cond;
void Buffer::add(int** frame) {
        while (true) {
            std::unique_lock<std::mutex> locker(mu);
            cond.wait(locker, [this](){return buffer_.size() < size_;});
            buffer_.push_back(frame);
            locker.unlock();
            cond.notify_all();
            return;
        }
}
int** Buffer::remove() {
        while (true)
        {
            std::unique_lock<std::mutex> locker(mu);
            cond.wait(locker, [this](){return buffer_.size() > 0;});
            int** back_frame = buffer_.front();
            buffer_.pop_front(); 
            locker.unlock();
            cond.notify_all();
            return back_frame;
        }
}
int Buffer::size(){
    return buffer_.size();
}
    


