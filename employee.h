// 普通员工
#pragma once
#include<iostream>
using namespace std;
#include "worker.h"

class Employee : public Worker
{
public:
    Employee(int id,string name,int did);
    virtual void showInfo();        // 重写父类中的纯虚函数
    virtual string getDeptName();   // 重写父类中的纯虚函数
};
