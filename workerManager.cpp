#include "workerManager.h"

workerManager ::workerManager() // 构造函数，创建对象时就读文件，如果有数据就存入数组中
{
    // 1.文件不存在
    ifstream ifs;
    ifs.open(FILE, ios::in);

    if (!ifs.is_open())
    {
        cout << "文件不存在" << endl;
        this->EmpNum = 0;
        this->EmpArray = NULL;
        this->FileIsEnpty = true;
        ifs.close();
        return;
    }

    // 2.文件存在，但是数据为空
    char ch;
    ifs >> ch; // 读走一个字符
    if (ifs.eof())
    {
        cout << "文件为空" << endl;
        this->EmpNum = 0;
        this->EmpArray = NULL;
        this->FileIsEnpty = true;
        ifs.close();
        return;
    }

    // 3. 文件存在，数据存在
    int num = this->get_EmpNum();
    cout << "职工人数为：" << num << endl;
    this->EmpNum = num;

    // 开辟空间
    this->EmpArray = new Worker *[num];
    // 将文件中的数据，存到数组中
    this->init_Emp();
}

workerManager ::~workerManager()
{
    if (this->EmpArray != NULL)
    {
        for (int i = 0; i < this->EmpNum; i++)
        {
            delete this->EmpArray[i];
            this->EmpArray[i] = NULL;
        }

        delete[] this->EmpArray;
        this->EmpArray = NULL;
        this->EmpNum = 0;
        this->FileIsEnpty = true;
    }
}

void workerManager::Show_Menu() // 系统菜单
{
    cout << "******************************************" << endl;
    cout << "***********  欢迎使用职工管理系统  ********" << endl;
    cout << "************  0.退出程序管理  ************" << endl;
    cout << "************  1.增加职工信息  ************" << endl;
    cout << "************  2.显示职工信息  ************" << endl;
    cout << "************  3.删除离职职工  ************" << endl;
    cout << "************  4.修改职工信息  ************" << endl;
    cout << "************  5.查找职工信息  ************" << endl;
    cout << "************  6.按照编号排序  ************" << endl;
    cout << "************  7.清空所有文档  ************" << endl;
    cout << "******************************************" << endl;
}

void workerManager ::ExitSystem() // 退出系统
{
    cout << "欢迎下次使用" << endl;
    exit(0);
}

void workerManager ::Add_Emp() // 添加职工
{
    cout << "请输入要添加的职工数：" << endl;
    int add_num;
    cin >> add_num;
    if (add_num > 0)
    {
        // 添加
        // 计算新空间的大小
        int newSize = this->EmpNum + add_num;
        // 开辟新空间
        Worker **newSpace = new Worker *[newSize];
        // 照搬旧数据
        if (this->EmpArray != NULL)
        {
            for (int i = 0; i < this->EmpNum; i++)
            {
                newSpace[i] = this->EmpArray[i];
            }
        }
        // 存入新添加的数据
        for (int i = 0; i < add_num; i++)
        {
            int id;
            string name;

            Worker *worker = NULL;

            cout << "请输入第 " << i + 1 << " 个新职工的编号：" << endl;
            cin >> id;

            cout << "请输入第 " << i + 1 << " 个新职工的名字：" << endl;
            cin >> name;

            // 外层套一个 while 循环，条件为 true，表示一直运行直到遇到 break
            while (true)
            {
                cout << "请输入第 " << i + 1 << " 个新职工的职位：" << endl;
                cout << "1、普通职工" << endl;
                cout << "2、经理" << endl;
                cout << "3、总裁" << endl;

                int did; // 循环内部定义 did
                cin >> did;

                switch (did)
                {
                case 1:
                    worker = new Employee(id, name, did);
                    break;
                case 2:
                    worker = new Menager(id, name, did);
                    break;
                case 3:
                    worker = new Boss(id, name, did);
                    break;
                default:
                    cout << "输入错误，需要重新输入" << endl;
                    // 循环会自动回到 while(true) 的开头，重新开始一轮输入
                    continue; // 显式表示继续下一次循环（其实省略 continue 也会自动回去）
                }
                // 如果执行到了这里，说明上面的 switch 成功匹配了 1/2/3 并创建了对象
                // 此时我们需要跳出整个 while 循环
                break;
            }

            newSpace[this->EmpNum + i] = worker;
        }
        // 释放原来的空间
        delete[] this->EmpArray;

        // 更改新空间的指向和人数的数量
        this->EmpArray = newSpace;
        this->EmpNum = newSize;

        this->FileIsEnpty = false;

        cout << "成功添加 " << add_num << " 名新职工" << endl;
        this->save();
    }
    else
    {
        cout << "输入错误" << endl;
    }
}

void workerManager ::save() // 在添加或删除职工后，保存到文件
{
    ofstream ofs;
    ofs.open(FILE, ios::out); // 模式不需要打双引号

    for (int i = 0; i < this->EmpNum; i++)
    {
        ofs << this->EmpArray[i]->id << " " << this->EmpArray[i]->name << " " << this->EmpArray[i]->deptId << endl;
    }
    ofs.close();
}

int workerManager ::get_EmpNum() // 读文件，获取职工数量
{
    ifstream ifs;
    ifs.open(FILE, ios::in);

    int id;
    string name;
    int did;

    int num;
    while (ifs >> id && ifs >> name && ifs >> did)
    {
        num++;
    }
    return num;
}

void workerManager ::init_Emp() // 文件不为空时，初始化职工
{
    ifstream ifs;
    ifs.open(FILE, ios::in);
    int id;
    string name;
    int did;

    int i = 0;
    while (ifs >> id && ifs >> name && ifs >> did)
    {
        Worker *worker = NULL;
        switch (did)
        {
        case 1:
            worker = new Employee(id, name, did);
            break;
        case 2:
            worker = new Menager(id, name, did);
            break;
        case 3:
            worker = new Boss(id, name, did);
            break;
        default:
            break;
        }
        this->EmpArray[i++] = worker;
    }
    ifs.close();
}

void workerManager ::show_Emp() // 显示职工
{
    if (this->FileIsEnpty)
    {
        cout << "文件不存在或者文件为空" << endl;
    }
    else
    {
        for (int i = 0; i < this->EmpNum; i++)
        {
            // 利用多态调用程序接口
            this->EmpArray[i]->showInfo();
        }
    }
}

int workerManager ::IsExist(int id) // 判断职工是否存在，存在返回编号，不存在返回-1
{
    int index = -1;
    for (int i = 0; i < this->EmpNum; i++)
    {
        if (this->EmpArray[i]->id = id)
        {
            index = i;
            break;
        }
    }
    return index;
}

void workerManager ::delete_Emp() // 删除员工     本质：数据前移覆盖原数据
{
    if (this->FileIsEnpty)
    {
        cout << "文件不存在或者文件为空，无法删除" << endl;
    }
    else
    {
        cout << "请输入想要删除的员工编号：" << endl;
        int id;
        cin >> id;
        int index = this->IsExist(id);
        if (index == -1)
        {
            cout << "删除失败，系统没有编号为 " << id << " 的员工" << endl;
        }
        else
        {
            // 前移数据，覆盖要删除的数据
            for (int i = index; i + 1 < this->EmpNum; i++)
            {
                this->EmpArray[i] = this->EmpArray[i + 1];
            }
            this->EmpNum--;
            // 将移动好的数据重新写入文件
            this->save();
            cout << "删除成功" << endl;
        }
    }
}

void workerManager ::mod_Emp()
{
    if (this->FileIsEnpty)
    {
        cout << "文件不存在或内容为空" << endl;
    }
    else
    {
        cout << "请输入要修改的员工编号：" << endl;
        int id;
        cin >> id;
        int res = this->IsExist(id);
        if (res == -1)
        {
            cout << "不存在员工编号为 " << id << " 的员工" << endl;
        }
        else
        {
            int new_id = 0;
            string new_name = "";
            int new_did = 0;

            cout << "要修改的员工信息：" << endl;
            this->EmpArray[res]->showInfo();
            delete this->EmpArray[res];

            cout << "请输入新职工号：" << endl;
            cin >> new_id;

            cout << "请输入新姓名：" << endl;
            cin >> new_name;

            cout << "请输入职位：" << endl;
            cout << "1.普通职工" << endl;
            cout << "2.经理" << endl;
            cout << "3.总裁" << endl;
            cin >> new_did;

            Worker *worker = NULL;
            switch (new_did)
            {
            case 1:
                worker = new Employee(new_id, new_name, new_did);
                break;
            case 2:
                worker = new Menager(new_id, new_name, new_did);
                break;
            case 3:
                worker = new Boss(new_id, new_name, new_did);
                break;
            }

            // 更新数据存入数组
            this->EmpArray[res] = worker;
            cout << "修改成功" << endl;
            this->save();
        }
    }
}

void workerManager ::find_Emp()
{
    if (this->FileIsEnpty)
    {
        cout << "文件不存在或内容为空,无法查找" << endl;
    }
    else
    {
        cout << "请输入你要查找的方式" << endl;
        cout << "1. 通过员工编号查找" << endl;
        cout << "2. 通过员工名字查找" << endl;
        cout << "3. 通过员工职位编号查找" << endl;
        int choice;
        cin >> choice;
        if (choice == 1)
        {
            cout << "请输入你要查找的员工编号：" << endl;
            int id;
            cin >> id;
            int res = this->IsExist(id);
            if (res == -1)
            {
                cout << "没有编号为 " << id << " 的员工" << endl;
            }
            else
            {
                this->EmpArray[res]->showInfo();
            }
        }
        else if (choice == 2)
        {
            cout << "请输入你要查找的员工名字：" << endl;
            string name;
            cin >> name;
            bool flag = false;
            for (int i = 0; i < this->EmpNum; i++)
            {
                if (this->EmpArray[i]->name == name)
                {
                    this->EmpArray[i]->showInfo();
                    flag = true;
                }
            }
            if (flag == false)
            {
                cout << "查无此人" << endl;
            }
        }
        else if (choice == 3)
        {
            cout << "请输入你要查找的员工职位编号：" << endl;
            cout << "1、普通职工" << endl;
            cout << "2、经理" << endl;
            cout << "3、总裁" << endl;
            int did;
            cin >> did;
            bool flag = false;
            for (int i = 0; i < this->EmpNum; i++)
            {
                if (this->EmpArray[i]->deptId == did)
                {
                    this->EmpArray[i]->showInfo();
                    flag = true;
                }
            }

            if (flag == false)
            {
                switch (did)
                {
                case 1:
                    cout << "该公司没有普通职工" << endl;
                    break;
                case 2:
                    cout << "该公司没有经理" << endl;
                    break;
                case 3:
                    cout << "该公司没有总裁" << endl;
                    break;

                default:
                    cout << "该公司没有员工职位编号为 " << did << " 的员工" << endl;
                    break;
                }
            }
        }
        else
        {
            cout << "输入错误" << endl;
        }
    }
}

void workerManager ::sort_Emp()
{
    if (this->FileIsEnpty)
    {
        cout << "文件不存在或内容为空,无法排序" << endl;
    }
    else
    {
        cout << "请输入排序方式" << endl;
        cout << "1. 按照员工编号升序排序" << endl;
        cout << "2. 按照职位编号降序排序" << endl;
        int choice;
        cin >> choice;
        if (choice == 1)
        {
            int min_index;
            for (int i = 0; i < this->EmpNum - 1; i++)
            {
                min_index = i;
                for (int j = i + 1; j < this->EmpNum; j++)
                {
                    if (this->EmpArray[j]->id < this->EmpArray[min_index]->id)
                    {
                        min_index = j;
                    }
                }
                if (min_index != i)
                {
                    Worker *worker = this->EmpArray[i];
                    this->EmpArray[i] = this->EmpArray[min_index];
                    this->EmpArray[min_index] = worker;
                }
            }
            cout << "排序完成" << endl;
            this->save();
            this->show_Emp();
        }
        else if (choice == 2)
        {
            int max_index;
            for (int i = 0; i < this->EmpNum - 1; i++)
            {
                max_index = i;
                for (int j = i + 1; j < this->EmpNum; j++)
                {
                    if (this->EmpArray[j]->deptId > this->EmpArray[max_index]->deptId)
                    {
                        max_index = j;
                    }
                }
                if (max_index != i)
                {
                    Worker *worker = this->EmpArray[i];
                    this->EmpArray[i] = this->EmpArray[max_index];
                    this->EmpArray[max_index] = worker;
                }
            }
            cout << "排序完成" << endl;
            this->save();
            this->show_Emp();
        }
        else
        {
            cout << "输入错误" << endl;
        }
    }
}

void workerManager ::Clean_File()
{
    if (this->FileIsEnpty)
    {
        cout << "文件不存在或内容为空,无需清空" << endl;
        return;
    }
    else
    {
        cout << "确认清空？" << endl;
        cout << "1. 确认" << endl;
        cout << "2. 取消" << endl;

        int choice;
        cin >> choice;
        if (choice == 1)
        {
            ofstream ofs(FILE, ios::trunc);
            ofs.close();

            if (this->EmpArray != NULL)
            {
                for (int i = 0; i < this->EmpNum; i++)
                {
                    delete this->EmpArray[i];
                    this->EmpArray[i] = NULL;
                }

                delete[] this->EmpArray;
                this->EmpArray = NULL;
                this->EmpNum = 0;
                this->FileIsEnpty = true;
            }

            cout << "清空文件完成" << endl;
        }
    }
}