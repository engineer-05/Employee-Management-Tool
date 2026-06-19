#include "employee.h"

    Employee::Employee(int id,string name,int did)
    {
        this->id = id;
        this->name = name;
        this->deptId = did;
    }
    Employee:: ~Employee()
    {
    }
    void Employee :: showInfo()
    {
        cout << "职工编号："<< this->id << "\t职工姓名：  " << this->name << "\t岗位：" << this->getDeptName() <<"\t岗位职责：完成经理的任务" << endl;
    }
    string Employee::getDeptName()
    {
        return "员工";
    }