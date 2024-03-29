#ifndef MTASKTHREAD_H
#define MTASKTHREAD_H

#include <thread>
#include <chrono>
#include <QObject>
#include <vector>
#include <QString>

class MTaskThread : public QObject{
    Q_OBJECT
public:
    explicit MTaskThread(std::vector<QString> vec_model_path, std::vector<QString> vec_data_path,QObject *parent = nullptr);
    explicit MTaskThread(int id);
    ~MTaskThread() noexcept;

    MTaskThread(MTaskThread const&rhs) = delete;
    MTaskThread& operator=(MTaskThread const&rhs) = delete;
    MTaskThread(MTaskThread &&rhs) = delete ;
    MTaskThread& operator=(MTaskThread &&rhs) =delete ;

    bool is_running() noexcept;
    void run() noexcept;
    bool stop(bool stop) noexcept;
private:
    std::thread task_thread_;
    bool stop_;
    std::vector<QString> vec_model_path_;
    std::vector<QString> vec_data_path_;
};

#endif // MTASKTHREAD_H
