// 经理
#pragma once
#include <iostream>
using namespace std;
#include "worker.h"

class Menager : public Worker
{
public:
    Menager(int id,string name,int did);
    ~Menager();
    virtual void showInfo();
    virtual string getDeptName();
};