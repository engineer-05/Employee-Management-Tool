#include "boss.h"

Boss::Boss(int id, string name, int did)
{
    this->id = id;
    this->name = name;
    this->deptId = did;
}

void Boss::showInfo()
{
    cout << "职工编号：" << this->id << "\t职工姓名：  " << this->name << "\t岗位：" << this->getDeptName() << "\t岗位职责：管理公司所有事物" << endl;
}

string Boss::getDeptName()
{
    return "总裁";
}