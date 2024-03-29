#include "MTaskThread.h"

MTaskThread::MTaskThread(std::vector<QString> vec_model_path, std::vector<QString> vec_data_path,QObject *parent):
    QObject(parent), vec_data_path_{vec_data_path}, vec_model_path_{vec_model_path}{
    std::thread t(&MTaskThread::run,this);
}


MTaskThread::~MTaskThread() noexcept{
    if(task_thread_.joinable())
        task_thread_.join();
}

/*
MTaskThread::MTaskThread(MTaskThread &&rhs) noexcept : stop_(rhs.stop_) {
    rhs.stop_ = false;
}

MTaskThread& MTaskThread::operator=(MTaskThread &&rhs) noexcept {
    if (this != &rhs) {
        stop_ = rhs.stop_;
        rhs.stop_ = false;
    }
    return *this;
}
*/

bool MTaskThread::is_running() noexcept{
    return stop_;
}

void MTaskThread::run() noexcept{
    while(true){
        if(stop_ == true)
        {
            //std::this_thread::sleep(std::chrono::microseconds(1));
            continue;
        }



    }
}

bool MTaskThread::stop(bool stop) noexcept{
    stop_ = stop;
    return stop_;
}

