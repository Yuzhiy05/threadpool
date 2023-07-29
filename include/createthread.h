#ifndef CREATETHREAD_
#define CREATETHREAD_
#include<thread>

namespace yzy::detail{

    struct join {};
    struct detach{};
    template<typename T>
    class createthread{};
    template<>
    class createthread<join>
    {
        std::thread thr;
        public:
        createthread(std::thread&& t):thr(std::move(t)){}
        createthread(const createthread&)=delete;
        createthread(createthread&& t)=default;
        ~createthread(){if(thr.joinable())thr.join();}
        typedef std::thread::id id;
        id get_id(){
            return thr.get_id();
        }


    };
    template<>
    class createthread<detach>
    {
        std::thread thr;  
    public:
    createthread(std::thread&&t):thr(std::move(t)){}
    createthread(const createthread&)=delete;
    createthread(createthread&& t)=default;
    ~createthread(){if(thr.joinable())thr.detach();}
    typedef std::thread::id id;
    id get_id(){
        return thr.get_id();
    }
   
    };
    
    
    

    
}
#endif