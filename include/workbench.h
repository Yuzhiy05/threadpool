#ifndef _WORKBEN_
#define _WORKEBN_

#include<map>
#include <memory>
#include <future>
#include <iostream>
#include <condition_variable>
#include"utility.h"
#include"createthread.h"
#include"taskqueue.h"

namespace yzy::detail
{
    struct workbench
    {
        typedef createthread<detach> work;
        typedef std::map<work::id,work> work_map;
        
        s_t decline=0;
        s_t task_done_workers=0;
        bool is_waiting=false;
        bool desturucting =false;
        std::string name{"deault"};

        work_map workers{};
        taskqueue<std::function<void()>>taskq{};


        std::mutex lockq{};
        std::condition_variable thread_cv{};
        std::condition_variable task_done_cv{};


        public:

        explicit workbench(const char*_name="default",int works=1):name(_name){
            for(int i=0;i<std::max(works,1);++i)
            add_worker();
        }

        workbench(const workbench&)=delete;
        workbench(workbench&&)=delete;
        ~workbench(){
            std::unique_lock<std::mutex> lock(lockq);
            decline=workers.size();
            desturucting=true;
            thread_cv.wait(lock,[this]{return !decline;});
        }

        public:
        void add_worker(){
            std::lock_guard<std::mutex>lock(lockq);
            std::thread t(&workbench::mission,this);
            workers.emplace(t.get_id(),std::move(t));
        }

        void deal_worker(){
            std::lock_guard<std::mutex> lock(lockq);
            if(workers.empty()){
                throw std::runtime_error("workspace: Try to delete too many branchs");
            }
            else{
                decline++;
            }

        }

    bool wait_task(unsigned timeout=-1){
        bool rel;
        {
            std::unique_lock<std::mutex> locker(lockq);
            is_waiting =true;
            rel=task_done_cv.wait_for(locker,std::chrono::milliseconds(timeout),[this]{return task_done_workers>=workers.size();});
            task_done_workers=0;
            is_waiting=false;
        }
        thread_cv.notify_all();
        return rel;
    }  
    public:

    s_t number_wrokers(){
        std::lock_guard<std::mutex> lock(lockq);
        return workers.size();
    }

    s_t number_taskq(){
        return taskq.length();
    }
    auto get_name()->const std::string&{
        return name;
    }
     public:
     template<typename T=simple,typename F,typename R=detail::result_of_type<F>,typename RR=typename std::enable_if<std::is_void<R>::value>::type>
    auto submit_misson(F&& task)->typename std::enable_if_t<std::is_same_v<T,simple>>{
            taskq.pushqueBack([task]{
                try{
                    task();
                }catch(const std::exception& ex){
                std::cerr<<"workspace: worker["<< std::this_thread::get_id()<<"] caught exception:\n  what(): "<<ex.what()<<'\n'<<std::flush;
                }
                catch(...){
                     std::cerr<<"workspace: worker["<< std::this_thread::get_id()<<"] caught unknown exception\n"<<std::flush;
            }
            });
    }

    template<typename T,typename F,typename R=detail::result_of_type<F>,typename RR=typename std::enable_if<std::is_void<R>::value>::type>
    auto submit_misson(F&& task)->typename std::enable_if_t<std::is_same_v<T,privilege>>{
        taskq.pushqueFront([task]{
                try{
                    task();
                }catch(const std::exception& ex){
                std::cerr<<"workspace: worker["<< std::this_thread::get_id()<<"] caught exception:\n  what(): "<<ex.what()<<'\n'<<std::flush;
                }
                catch(...){
                     std::cerr<<"workspace: worker["<< std::this_thread::get_id()<<"] caught unknown exception\n"<<std::flush;
            }
            });
    }
        template <typename T, typename F, typename... Fs>
    auto submit(F&& task, Fs&&... tasks) -> typename std::enable_if_t<std::is_same_v<T, sequence>> {
        taskq.pushqueFront([=]{
            try {
                this->rexec(task, tasks...);
            } catch (const std::exception& ex) {
                std::cerr<<"workspace: worker["<< std::this_thread::get_id()<<"] caught exception:\n  what(): "<<ex.what()<<'\n'<<std::flush;
            } catch (...) {
                std::cerr<<"workspace: worker["<< std::this_thread::get_id()<<"] caught unknown exception\n"<<std::flush;
            }
        });
    }
    //有返回值用std：；future存
    


    };
    
    
} // namespace yzy::detail



























#endif