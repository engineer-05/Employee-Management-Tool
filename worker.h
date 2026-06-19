#pragma once
#include <iostream>
#include <string>
using namespace std;

// 职工抽象类
class Worker
{
public:
    virtual void showInfo() = 0;    // 显示职工信息
    virtual string getDeptName() = 0;   // 获取岗位名称

    int id;         // 职工编号
    string name;    // 姓名
    int deptId;     // 岗位名称编号
};
