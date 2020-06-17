#include "logger.h"

namespace ZLog{

    //TODO: 日志级别
    char *ZLogLevalMsg[] = {
        "INFO",
        "WARRING",
        "ERROR"
    };



    //TODO: 构造
    Logger::Logger(int size = ZLOG_INIT_SIZE) : _size(size), now(0)
    {
            _buf[0] = new std::vector<std::string>(_size);
            _buf[1] = new std::vector<std::string>(_size);
            _mbuf[0] = new std::vector<std::mutex>(_size);
            _mbuf[1] = new std::vector<std::mutex>(_size);
    }

    // TODO: 添加日志
    void Logger::add(std::string &info)
    {
        //计算线程hash
        int pos = this->_hash();
        //加锁
        std::lock_guard<std::mutex> lock(_mbuf[now]->at(pos));
        //格式化日志
        /*
         std::stringstream ss;
         try{
             ss << "[ " << getTime() << " " << std::this_thread::get_id() << " " << __FILE__ << ":" << __LINE__ << " ]";
             ss << "[ " << ZLogLevalMsg[leval] << " ] " << info; 
         } catch (int e){
             ss << "Call gettimeofday() error. Error code: "<< e;
         }
         ss << std::endl;*/

        //添加到buf
        _buf[now]->at(pos) += info;
    }

    //TODO: 计算hash值
    int Logger::_hash(){
        auto tid = std::this_thread::get_id();
        return *((__gthread_t *)&tid) % _size;
     }

    // TODO: 获取时间
    inline std::string getTime(void)
    {
        timeval tv;
        tm * time;
        std::stringstream ss;

        if(gettimeofday(&tv, nullptr) == -1){
            throw errno;
        }
        time = localtime(&(tv.tv_sec));
        char systime[40];
        strftime(systime, 40, "%Y-%m-%d %H:%M:%S.",  time);
        
        ss << systime << (int64_t)(tv.tv_usec / 1000);
        
        return ss.str();
    }

    //TODO: logger线程
    void Logger::logging(std::string logname)
    {
        std::ofstream f(logname,std::ios::app);
        bool isHashNull;

        while(!_stop){
            this->now = !this->now;
            isHashNull = true;
            for(int i= 0; i < this->_size; i++)
            {
                std::lock_guard<std::mutex> lock(_mbuf[!now]->at(i));
                if(_buf[!now]->at(i).compare("")!=0){
                    f << _buf[!now]->at(i) << std::endl;
                    _buf[!now]->at(i) = "";
                    isHashNull = false;
                }
            }
            if(isHashNull){
                //std::this_thread::yield(); 让出时间片并没有什么卵用
                sleep(1);
            }
            f.flush();
        }
        f.close();
    }

    //TODO: 执行logging线程
    void Logger::start(std::string logname){
        this->_stop = false;
        std::thread t(&Logger::logging, this, logname);
        t.detach();
    }

    //TODO: 关闭logging线程 
    void Logger::stop(){
        this->_stop = true;
    }

    //TODO: 析构
    Logger::~Logger(){
        delete this->_buf[0];
        delete this->_buf[1];
        delete this->_mbuf[0];
        delete this->_mbuf[1];
    }

    //TODO: Logger单例类声明

    Logger *SampleLogger::_logger = nullptr;
    std::mutex SampleLogger::m;

    Logger *SampleLogger::get(){
        if(_logger == nullptr){
            std::lock_guard<std::mutex> lock(m);
            if(_logger == nullptr){
                _logger = new Logger();
            }
        }
        return _logger;
    }

    //TODO: 初始化一个log
    _ZLOG::_ZLOG(ZLogLeval leval, std::string file, int line){
            _info = "";
            std::stringstream ss;
            try{
                ss << "[" << getTime() << " " << std::this_thread::get_id() << " " << file << ":" << line << "]";
                ss << "[" << ZLogLevalMsg[leval] << "] " ; 
            } catch (int e){
                ss << "Call gettimeofday() error. Error code: "<< e;
            }
            _info = ss.str();
    }
    //TODO: ZLOG operator <<
    _ZLOG &_ZLOG::operator << (std::string info){
            _info += info;
            return *this;
    }
        _ZLOG &_ZLOG::operator << (int info){
            _info += std::to_string(info);
            return *this;
    }
        _ZLOG &_ZLOG::operator << (short info){
            _info += std::to_string(info);
            return *this;
    }
        _ZLOG &_ZLOG::operator << (long info){
            _info += std::to_string(info);
            return *this;
    }
        _ZLOG &_ZLOG::operator << (long long info){
            _info += std::to_string(info);
            return *this;
    }
        _ZLOG &_ZLOG::operator << (char * info){
            _info += info;
            return *this;
    }
        _ZLOG &_ZLOG::operator << (float info){
            _info += std::to_string(info);
            return *this;
    }
        _ZLOG &_ZLOG::operator << (double info){
            _info += std::to_string(info);
            return *this;
    }
        _ZLOG &_ZLOG::operator << (unsigned int info){
            _info += std::to_string(info);
            return *this;
    }
        _ZLOG &_ZLOG::operator << (unsigned long info){
            _info += std::to_string(info);
            return *this;
    }
        _ZLOG &_ZLOG::operator << (unsigned long long info){
            _info += std::to_string(info);
            return *this;
    }
    //TODO: ~_ZLOG()
    _ZLOG::~_ZLOG(){
         _info += '\n';
        SampleLogger::get()->add(_info);
    }

}
