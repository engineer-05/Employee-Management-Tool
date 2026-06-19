#pragma once
#include<iostream>
using namespace std;
#include "worker.h"
#include "employee.h"
#include "menager.h"
#include "boss.h"

#include <fstream>
#define FILE "empfile.txt"


class workerManager
{
private:
public:
    workerManager();
    ~workerManager();
    // 人数
    int EmpNum;
    // 职工数组指针
    Worker ** EmpArray;
    
    // 菜单
    void Show_Menu();
    // 退出
    void ExitSystem();
    // 添加职工
    void Add_Emp();
    // 保存到文件
    void save();
    // 判断文件是否为空
    bool FileIsEnpty = false;
    // 获取文件中的人数
    int get_EmpNum();
    // 初始化员工
    void init_Emp();
    // 显示职工
    void show_Emp();
    //判断职工是否存在，存在返回编号，不存在返回-1
    int IsExist(int id);
    // 删除员工
    void delete_Emp();
    // 修改员工
    void mod_Emp();
    // 查找员工
    void find_Emp();
    //排序
    void sort_Emp();
    // 清空文件
    void Clean_File();
};