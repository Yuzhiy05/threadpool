#ifndef _UNILITY_
#define _UNILITY_

#include<future>
#include<deque>
#include<type_traits>
#include<functional>
 namespace yzy::detail{
    template<typename A,typename ...Args>
    using  result_of_type=typename std::result_of<A(Args...)>::type;
    typedef size_t s_t;

//任务特权级
    struct simple
    {
        /* data */
    };
    struct privilege
    {
        /* data */
    };
    struct sequence
    {
        /* data */
    };
    
    
    
    template<typename T>
    class futures{
        std::deque<std::future<T>> que;
        public:
        using iterator=typename std::deque<std::future<T>>::iterator;

    //等所有future的结果
    void wait(){
        for(auto& each:que){
            que.wait();
        }
    }
    s_t size(){
        return que.size();
    }
//将future得到的结果拿进vector
    auto get()->std::vector<T>{
        std::vector<T> result;
        for(auto& each:que){
            result.emplac_back(each.get());
        }
        return result;
    }

    auto end()->iterator&{
        return que.end();
    }
    auto begin()->iterator&{
        return que.front();
    }

    void add_back(std::future<T>&& t){
        que.emplace_back(std::move(t));
    }
    void add_front(std::future<T>&& t){
        que.emplace_front(std::move(t));
    }
    
    void for_each(std::function<void (std::future<T>&) fun>){
        for(auto& each:result){
            fun(each);
        }
    }
    void for_each(const iterator& frist,std::function<void(std::future<T>&) fun){
        for(auot i=frist;i!=end();++i)
        fun(*i);
    }
    vodi for_each(const iterator& frist,const iterator& last,std::function<void(future<T>)& fun){
        for(auto i=frist;i!=last;++i)
        fun(*i);
    }
    auto operator[](s_t i)->std::future<T>&{
        return result[i];
    }


};

 }
#endif