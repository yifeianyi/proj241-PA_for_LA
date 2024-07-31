## 队伍
run的全都队
## 赛题

南大PA实验的LoongArch支持 :[proj241-la-pa](https://github.com/oscomp/proj241-la-pa)

**所属赛道** ：2024全国大学生操作系统比赛的“OS功能挑战”赛道

如果你觉得本团队的工作有参考价值😊，请不要吝惜你的⭐

## 项目描述：
对南京大学的PA实验系统进行扩展，添加LoongArch架构支持。

## 调研分析：
1. loongarch作为2021年才发布的新架构，资料相对较少，LA32r作为其教育精简版这点尤为明显。
2. PA实验的框架代码对LA有初步的支持，但实验讲义中无LA32r相关的架构介绍，实现难度目测比其它架构大不少。
3. 团队成员相关基础较差，无人有新架构适配经验，对于完成这种横跨硬件和软件的系统性实验任务，挑战不小。
4. 2024.4.20 才决定报名参赛，时间紧任务重。
5. 队长有一定的体系结构的相关技术背景，项目可以一试。

## 项目目标：
1. 通过PA实验构建一个简单完整的计算机系统, 来深入理解程序如何在计算机上运行。
2. 通过该项目练习如何进行团队协同开发与项目管理。
3. 撰写详细的实验手册，填补PA实验讲义对于loongarch架构相关的介绍空白，让往后的同学做pa实验选择LA32r作为目标架构时,上手难度能与 MIPS、RISC-V 持平。

## 时间规划：
### 初赛规划
- 2024.4.20 - 2024.5.30 完成 pa3 及 相关文档
### 决赛规划 (没进决赛也按该标准继续完成本项目)
- 2024.6.6  - 2024.7.10 完成 pa4 及 相关文档
- 2024.7.12 - 2024.7.31 完成 真机适配 及 性能调优
- 2024.7.31 - 2024.8.x  进一步完善项目及相关文档内容

## 项目管理 & 团队协作：
|        | 分工 | 开发日志 | 联系方式 |
| :----: | :-----: | :----: | :------: |
| 李宗逸 | 统筹规划、资料调研、架构适配 | [Dev_lzy](https://github.com/yifeianyi/proj241-PA_for_LA/wiki/Dev_lzy) | QQ：910002662 |
| 刘奕钊 | 完成loongarch架构的PA实验 | [Dev_lyz](https://github.com/lemonsuqing/proj241-PA_for_LA/wiki/Dev_lyz) | QQ：1428828189 |
| 李智峰 | 完成loongarch架构的PA实验 | [Dev_lzf](https://github.com/LiZhiFeng1111111/proj241-PA_for_LA/wiki/Dev_lzf) | QQ：1186288906          |

- [项目管理与协作细节](https://hxu6i1coj1.feishu.cn/base/WWjRbUz3taGa0UsBuEycoVapngk?table=tbl6xudDrnehQGnQ&view=veweOItODk)：涵盖本项目的所有推进细节。

## 文档与演示
- 技术文档
  - 适配文档：
    - [ ] [loongarch32r工具配置]()
    - [ ] [简易LA32r反汇编器](): 由于llvm 不支持la32r，为了itrace中能记录汇编代码，此需求应运而生。
    - [ ] [qemu-system-loongarch32 difftest适配笔记]()
    - [x] [la32-qemu bug分析报告](/doc/la32-QEMU%20bug分析报告.pdf)
  - 实现笔记：
    - [IOE的外设实现](https://ebsidb6m9g.feishu.cn/wiki/WdsDwlEcqiMEMzkrKOjcfjdDnKd?from=from_copylink)  
    - [基于AM裸机的运行过程分析](https://ebsidb6m9g.feishu.cn/wiki/CmANwfLV4ipZnWkNtCzcYFffn9f?from=from_copylink)
    - [nemu的框架代码](https://ebsidb6m9g.feishu.cn/wiki/FWgXwGUxIiAZlZk9InVcSWzinJf?from=from_copylink)
    - [Nanos-lite启动过程分析](https://ebsidb6m9g.feishu.cn/wiki/Wt88wvqe2iuAoCkyW1uca6TXnod?from=from_copylink)
    - [加载器的实现和难点](https://ebsidb6m9g.feishu.cn/wiki/BPz6w1Ay1iZ3B3ko6IwcseU9nRb?from=from_copylink)
    - [分析游戏到nemu上运行的过程](https://ebsidb6m9g.feishu.cn/wiki/W7Jjwoydiiau7Kkj2W4ccqBRnVe?from=from_copylink)
    - [SDB简易调试器：实现监视点](https://ebsidb6m9g.feishu.cn/wiki/BmzTwl34eiLHfokIlSwc7xDTnhg?from=from_copylink)
    - [ELF学习笔记](https://iiglfr2by0.feishu.cn/docx/FKwXdhuEFofSKvxNmNXctVUHnkh?from=from_copylink)
    - [Klib测试与实现](https://iiglfr2by0.feishu.cn/docx/OezadnxmCofXskxRdRDcQ6kJnGc?from=from_copylink)
    - [NEMU 启动过程分析](https://iiglfr2by0.feishu.cn/docx/ZkxKdvjFFoJyh3xFQkfcXN8FnZc?from=from_copylink)
    - [表达式求值](https://iiglfr2by0.feishu.cn/docx/K3EUdYJskob34zx6TuOcqECCnHg?from=from_copylink)

- 汇报&演示
  - [初赛阶段性成果汇报](/doc/初赛阶段性汇报.pdf)
  - [初赛阶段性演示视频](https://www.alipan.com/s/VR1WY5AAjMZ)
  - 队员个人总结：doc文件夹下

## 项目进度

### PA1
- [x] 单步执行
- [x] 内存扫描
- [x] 表达式求值
- [x] 监视点

### PA2
- [x] 基础设施
  - [x] LA32r反汇编器
  - [x] iringbuf
  - [x] mtrace
  - [x] LA32r difftest适配
  - [x] difftest
  - [x] dtrace
  - [x] ftrace
- [x] 添加基础指令
- [x] 简易标准库实现
- [ ] 外设支持
  - [x] 串口
  - [x] 时钟
  - [x] VGA
  - [ ] 声卡

### PA3
- [x] 实现异常响应机制
- [x] Loader 实现
- [ ] 系统调用支持
  - [x] exit
  - [x] write
  - [ ] brk
  - [ ] gettimeofday
  - [ ] execve
- [ ] 简易文件系统
  - [ ] 键盘抽象
  - [ ] VGA抽象
  - [ ] 实现定点数库fixedptc
- [ ] 跑通仙剑奇侠传
- [ ] 基础设施
  - [x] etrace
  - [ ] strace
  - [ ] 快照实现
### PA4
- [x] yield-os 实现上下文管理
- [ ] 跑通 native RT-Thread

## 参考资料
- [南大PA文档2023](https://nju-projectn.github.io/ics-pa-gitbook/ics2023/)
- [2022年OS大赛开源作品:la-sel4](https://github.com/tyyteam/la-seL4)
- [2022年OS大赛开源作品:rCore的龙芯平台移植](https://github.com/Godones/rCoreloongArch)
- [la32r 工具链](https://gitee.com/loongson-edu/la32r-toolchains)
- [la32 gdb安装](https://blog.csdn.net/greenmoss/article/details/127800221)
- [计算机体系结构基础](https://foxsen.github.io/archbase/)
- [基于QMP实现对qemu虚拟机进行交互](https://zhuanlan.zhihu.com/p/56887210)
- 一生一芯双周分享会(nemu部分):[视频回放](https://space.bilibili.com/238318574?spm_id_from=333.788.0.0)、[相关主题的分享文档](https://docs.qq.com/sheet/DU05xUmxjWmFvaXhj?tab=8nd1jt&login_t=1715061395395)
