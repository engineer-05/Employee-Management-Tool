#include <iostream>
using namespace std;
#include "workerManager.h"
#include "worker.h"
#include "employee.h"
#include "menager.h"
#include "boss.h"

int main(int argc, char const *argv[])
{
    // 实例化管理者对象
    workerManager wm;
    while (1)
    {
        // 显示菜单
        wm.Show_Menu();
        // 选择
        int choice = 0;
        cout << "请输入你的选择:" << endl;
        cin >> choice;
        switch (choice)
        {
        case 0: // 退出
            wm.ExitSystem();
            break;
        case 1: // 增加
            wm.Add_Emp();
            break;
        case 2: // 显示
            wm.show_Emp();
            break;
        case 3: // 删除
            wm.delete_Emp();
            break;
        case 4: // 修改
            wm.mod_Emp();
            break;
        case 5: // 查找
            wm.find_Emp();
            break;
        case 6: // 排序
            wm.sort_Emp();
            break;
        case 7: // 清空
        wm.Clean_File();
            break;
        default:
            cout << "输入错误,请重新输入" << endl;
            break;
        }
    }
    return 0;
}
