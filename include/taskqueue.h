#ifndef TASKQUEUE_H
#define TASKQUEUE_H
#include<deque>
#include<mutex>

namespace yzy{
    namespace detail{
    template<typename T>
    class taskqueue{
        std::mutex qlock;
        std::deque<T> taskque;
    public:
    typedef type_size typename std::deque<T>::type_size;
    taskqueue()=default;
    taskqueue(const taskqueue&)=delete;
    taskqueue(taskqueue&&)=default;
    public:
    void pushqueBack(T& a){
        std::lock_guard<std::mutex> lock(qlock);
        taskque.emplace_back(a);
    }
    void pushqueBack(T&& a){
        std::lock_guard<std::mutex> lock(qlock);
        taskque.emplace_back(std::move(a));
    }
    void pushqueFront(T &a){
        std::lock_guard<std::mutex> lock(qlock);
        taskque.emplace_front(a);
    }
    bool tryPop(T& tmp){
        std::lock_guard<std::mutex> lock(qlock);
        if(!taskque.empty()){
            tmp=std::move(taskque.front());
            taskque.pop_front();
            return true;
        }
        return false;

    }
    type_size length(){
        std::lock_guard<std::mutex> lock(qlock);
        return taskque.size();
    }
    


 };
    }

}


#endif