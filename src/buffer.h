#ifndef BUFFER_H_
#define BUFFER_H_
#include <mutex>
#include <condition_variable>
#include <deque>
using std::deque;


class Buffer
{
public:
    void add(int** frame);
    int** remove();
    int size();
private:
    deque<int**> buffer_;
    const unsigned int size_ = 100000;
};

#endif /* BUFFER_H_ */

