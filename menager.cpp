#include "menager.h"

Menager::Menager(int id, string name, int did)
{
    this->id = id;
    this->name = name;
    this->deptId = did;
}

Menager::~Menager()
{
}
void Menager::showInfo()
{
    cout << "职工编号：" << this->id << "\t职工姓名：  " << this->name << "\t岗位：" << this->getDeptName() << "\t岗位职责：完成老板的任务" << endl;
}
string Menager::getDeptName()
{
    return "经理";
}