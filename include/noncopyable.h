#pragma once

/**
 * 
 * 继承该noncopyable类的子类无法调用拷贝构造函数和赋值函数
 * 
 */

class noncopyable
{
public:
    noncopyable(const noncopyable &) = delete;          //delete(禁用函数)
    noncopyable &operator=(const noncopyable &) = delete;
protected:
    noncopyable() = default;                            //生成默认的函数定义体
    ~noncopyable() = default;
};