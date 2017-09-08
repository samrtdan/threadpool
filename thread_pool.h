#ifndef __THREAD_POOL_H_
#define __THREAD_POOL_H_

#include <vector>
#include <boost/thread.hpp>

enum ThreadPoolState
{
    THREAD_POOL_RUNNING  = 0, // Thread Pool is running 
    THREAD_POOL_SHUTDOWN = 1, // 此时线程池不接受消息，等待队列中其他消息执行完才退出
    THREAD_POOL_STOP = 2, //此时线程池不接受消息，并直接退出，不处理其他消息
    THREAD_POOL_TERMINATED = 3, //所有线程退出了
};

class ThreadPool
{
    public:
        ThreadPool();
        ThreadPool(int work_thread_num);
        ~ThreadPool();

        void shutdown();
        void shutdown_now();

        void Queue(MsgRunCallback & msg);

        /*
         *      * @brief 目前只支持扩容，不支持缩容
         *           */
        int resize(int thread_num);

        void set_max_work_thread_num(int max_work_thread_num){ _max_work_thread_num = max_work_thread_num; }
        void set_max_queue_num(int max_queue_num){ _max_queue_num = max_queue_num; }
    private:
        void thread_main();

    private:
        ThreadPoolState _thread_pool_state;
        int _work_thread_num;
        int _max_work_thread_num;
        boost::mutex _mutex;
        std::vector<boost::thread *> _thread_pool;

        int _max_queue_num;
        BlockingQueue<MsgRunCallback>  _msg_blocking_queue; 
};


#endif

