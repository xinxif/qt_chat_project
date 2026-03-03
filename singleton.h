#ifndef SINGLETON_H
#define SINGLETON_H
#include <memory>
#include <mutex>
#include<iostream>
template<typename T>
class Singleton
{
public:

    void print_address()    {std::cout<<instance.get()<<"\n";}
    ~Singleton()            {std::cout<<"this is Singleton destruction\n";}

    static std::shared_ptr<T> get_instance()
    {
        //make_shared需要访问类的构造函数，但是这个单例类的构造函数是protected的，无法被智能指针访问
        //C++11静态局部变量是线程安全的，C++11 保证了它的初始化是原子的
        static std::once_flag s_flag;
        std::call_once(s_flag,[](){instance=std::shared_ptr<T>(new T());});
        return instance;
    }
protected:
    Singleton()=default;
    Singleton(const Singleton&)=delete;
    Singleton&operator=(const Singleton&)=delete;

    static std::shared_ptr<T> instance;
};
template<typename T>
std::shared_ptr<T> Singleton<T>::instance = nullptr;

#endif // SINGLETON_H
