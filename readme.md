# 职工管理系统（Employee Management Tool）

一个基于 C++ **面向对象多态**的控制台职工信息管理工具，支持职工信息的增删改查、排序以及文件持久化存储。

---

## 🏗️ 项目结构

```
├── main.cpp            # 程序入口，主菜单循环
├── worker.h            # 职工抽象基类声明
├── employee.h          # 普通员工类声明
├── employee.cpp        # 普通员工类实现
├── menager.h           # 经理类声明
├── menager.cpp         # 经理类实现
├── boss.h              # 总裁类声明
├── boss.cpp            # 总裁类实现
├── workerManager.h     # 管理类声明（核心业务逻辑）
├── workerManager.cpp   # 管理类实现
└── empfile.txt         # 职工数据文件
```

---

## 🚀 功能特性

| 编号 | 功能 | 说明 |
|:---:|:---|:---|
| 0 | 退出系统 | 结束程序运行 |
| 1 | 增加职工 | 批量添加，选择职位（员工/经理/总裁） |
| 2 | 显示职工 | 列表展示所有职工详细信息 |
| 3 | 删除职工 | 按编号删除指定职工 |
| 4 | 修改职工 | 按编号修改职工编号、姓名、职位 |
| 5 | 查找职工 | 支持按编号 / 姓名 / 职位三种方式查找 |
| 6 | 排序职工 | 支持按编号升序 / 按职位编号降序 |
| 7 | 清空数据 | 一键清空所有职工记录（带确认） |

---

## 🔑 四个关键设计点

### 1. 基类与派生类（多态体系）

系统定义了一个**抽象基类** `Worker`，包含所有职工的共同属性（编号、姓名、职位编号）和两个纯虚函数：

```cpp
class Worker
{
public:
    virtual void showInfo() = 0;      // 显示个人信息
    virtual string getDeptName() = 0; // 返回职位名称
    int id;
    string name;
    int deptId;
};
```

三个派生类分别覆写 `showInfo()` 和 `getDeptName()`，展示各自的岗位职责：

```
Worker（抽象基类）
    │
    ├── Employee  → getDeptName() 返回 "员工"   showInfo() 显示 "个人信息 完成经理的任务"
    ├── Menager   → getDeptName() 返回 "经理"   showInfo() 显示 "个人信息 完成老板的任务"
    └── Boss      → getDeptName() 返回 "总裁"   showInfo() 显示 "个人信息 管理公司所有事物"
```

使用多态的好处：管理类只需操作 `Worker*` 指针，无需关心具体子类。以后新增角色（如"实习生"）只需新增一个派生类，管理代码无需改动。

---

### 2. 基类二级指针管理职工数组（多态的核心）

这是整个系统最重要的设计。`workerManager` 中职工数组的声明为：

```cpp
Worker **EmpArray;  // Worker 的二级指针
```

**为什么用 `Worker**`？**

- 数组本身存的是 **`Worker*`（基类一级指针）**，每个元素指向一个堆上的职工对象
- 数组名 `EmpArray` 作为数组首地址，类型自然就是 `Worker**`
- 由于存放的是基类指针，通过虚函数机制，**同一个 `showInfo()` 调用会根据指针实际指向的子类对象表现出不同的行为**——这就是动态多态

```cpp
// 创建不同类型的职工，统一用 Worker* 存入数组
Worker *worker = NULL;
switch (deptId) {
    case 1: worker = new Employee(id, name, deptId); break;  // 基类指针 → 派生类对象
    case 2: worker = new Menager(id, name, deptId);   break;
    case 3: worker = new Boss(id, name, deptId);      break;
}
this->EmpArray[i] = worker;

// 统一调用，自动分发到正确的子类实现
this->EmpArray[i]->showInfo();
```

运行时的虚函数分发路径：

```
EmpArray[i]->showInfo()
    │
    ├── 实际指向 Employee → 调用 Employee::showInfo()  → "个人信息 岗位职责：完成经理的任务"
    ├── 实际指向 Menager  → 调用 Menager::showInfo()   → "个人信息 岗位职责：完成老板的任务"
    └── 实际指向 Boss     → 调用 Boss::showInfo()      → "个人信息 岗位职责：管理公司所有事物"
```

---

### 3. 构造函数的三段式初始化

程序启动时，构造函数负责从文件恢复数据。它按三种情况分步处理：

```cpp
workerManager::workerManager()
{
    ifstream ifs;
    ifs.open(FILE, ios::in);

    // 情况①：文件不存在
    if (!ifs.is_open()) {
        this->EmpNum = 0;
        this->EmpArray = NULL;
        this->FileIsEmpty = true;      // 标记：文件不存在
        return;
    }

    // 情况②：文件存在但内容为空
    char ch;
    ifs >> ch;
    if (ifs.eof()) {
        this->EmpNum = 0;
        this->EmpArray = NULL;
        this->FileIsEmpty = true;      // 标记：文件为空
        return;
    }

    // 情况③：文件存在且有数据
    this->FileIsEnpty = false;      // 标记：文件不为空
    this->EmpNum = this->get_EmpNum();
    this->EmpArray = new Worker *[this->EmpNum];
    this->init_Emp();   // 将文件中的数据，存到数组中
}
```

`init_Emp()` 根据文件中每条记录的 `deptId` 字段（1/2/3）判断角色类型，用 `switch` 创建对应的派生类对象，统一通过 `Worker*` 存入数组。

---

### 4. 内存释放

`Worker**` 数组涉及**两层动态分配**，析构函数必须按**从内到外**的顺序释放：

```cpp
workerManager::~workerManager()
{
    if (this->EmpArray != NULL)
    {
        // 第一层：逐个释放每个堆上的职工对象（Employee/Menager/Boss）
        for (int i = 0; i < this->EmpNum; i++)
        {
            delete this->EmpArray[i];
            this->EmpArray[i] = NULL;
        }

        // 第二层：释放指针数组本身
        delete[] this->EmpArray;
        this->EmpArray = NULL;
        this->EmpNum = 0;
        this->FileIsEmpty = true;
    }
}
```

**注意顺序**：必须先 `delete` 每个对象，再 `delete[]` 数组。如果反过来，数组被释放后指针悬空，对象再也找不回来，造成内存泄漏。

---

## 🔧 两个难点及解决方案

### 难点 1：如何实现数据持久化？

**问题**：程序每次运行，职工信息都在内存中。一旦退出，所有增删改的结果全部丢失。

**解决方案**：使用文件流进行**全量写回**。每次增/删/改/排序操作完成后，立即调用 `save()` 将内存中的全部职工数据覆写到 `empfile.txt`：

```cpp
void workerManager::save()
{
    ofstream ofs;
    ofs.open(FILE, ios::out);   // 以覆盖模式打开
    for (int i = 0; i < this->EmpNum; i++) {
        ofs << this->EmpArray[i]->id << " "
            << this->EmpArray[i]->name << " "
            << this->EmpArray[i]->deptId << endl;
    }
    ofs.close();
}
```

**数据格式**：每行一条记录 `<编号> <姓名> <职位编号>`，例如：

```
1 唐僧 3
2 孙悟空 2
3 白龙马 1
4 猪八戒 1
5 沙和尚 1
```

其中职位编号含义：`1` = 普通员工，`2` = 经理，`3` = 总裁。

**流程闭环**：

```
程序启动 → 读文件(构造函数) → 还原对象数组 → 菜单操作 → save()写回文件 → 退出
              ↑                                                  |
              └──────────────── 下次启动自动恢复 ←──────────────────┘
```

这种"启动时读文件 → 操作后全量写回"的策略实现简单，对于职工管理这类小数据量场景完全够用。

---

### 难点 2：文件状态标志位

**问题**：程序中多处操作（显示、删除、修改、查找、排序、清空）都需要先判断是否有职工数据。如果每次都重新打开文件检查，效率低且代码冗余。

**解决方案**：引入一个 **`FileIsEmpty` 标志位**，在构造函数中一次性确定，之后所有操作直接查标志位：

```cpp
bool FileIsEmpty;  // true = 无数据，false = 有数据
```

| 场景 | FileIsEmpty 的值 |
|:---|:---|
| 文件不存在 | `true` |
| 文件存在但内容为空 | `true` |
| 文件存在且有数据 | `false` |
| 新增职工后 | 置为 `false` |
| 清空所有数据后 | 置为 `true` |

**使用示例**——显示职工前先检查：

```cpp
void workerManager::show_Emp()
{
    if (this->FileIsEmpty) {
        cout << "文件不存在或者文件为空" << endl;
        return;
    }
    for (int i = 0; i < this->EmpNum; i++)
        this->EmpArray[i]->showInfo();
}
```

标志位让状态判断变成 O(1) 的布尔检查，代码清晰且无冗余 I/O。

---

## 🔧 编译与运行

### 环境要求

- 支持 C++11 或以上标准的编译器（g++ / MSVC / clang++）

### 编译

```bash
g++ -o main main.cpp employee.cpp menager.cpp boss.cpp workerManager.cpp
```

### 运行

```bash
./main
```

运行后将看到系统菜单，输入对应数字即可操作。

---

## 📄 开源协议

本项目仅供学习交流使用。
