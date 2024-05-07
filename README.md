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
| 李宗逸 | 统筹规划、资料调研、架构适配、写LA版的Tutorial | [Dev_lzy](https://github.com/yifeianyi/proj241-PA_for_LA/wiki/Dev_lzy) | QQ：910002662 |
| 刘奕钊 | 完成loongarch架构的PA实验、校对Tutorial | [Dev_lyz](https://github.com/lemonsuqing/proj241-PA_for_LA/wiki/Dev_lyz) | QQ：1428828189 |
| 李智峰 | 完成loongarch架构的PA实验、校对Tutorial | [Dev_lzf](https://github.com/LiZhiFeng1111111/proj241-PA_for_LA/wiki/Dev_lzf) | QQ：1186288906          |



- [项目管理与协作细节](https://hxu6i1coj1.feishu.cn/base/WWjRbUz3taGa0UsBuEycoVapngk?table=tbl6xudDrnehQGnQ&view=veweOItODk)：涵盖本项目的所有推进细节。
- [git 协同开发流程](): 本团队在该项目中使用git进行协作开发的经验总结。

## 文档与演示
- 技术文档
  - 适配文档：
    - [ ] [loongarch32r工具配置]()
    - [ ] [简易LA32r反汇编器](): llvm 不支持 la32r，无法在itrace中记录汇编代码，故此需求应运而生。
    - [ ] [qemu-system-loongarch32 difftest适配笔记]()
  - 实现笔记：
  - 团队分享:
    - [ ] PA Makefile构成分析
    - [ ] nemu 框架代码分析
    - [ ] 运行时环境 am 源码分析
    - [ ] nanos 启动流程分析
- 实验讲义：
  - [PA_for_LA_Tutorial](NULL):ToDo
- 汇报&演示
  - 初赛阶段性成果汇报

## 项目进度

### PA1
- [x] 单步执行
- [x] 内存扫描
- [x] 表达式求值
- [x] 监视点

### PA2
- [ ] 基础设施
  - [x] LA32r反汇编器
  - [x] iringbuf
  - [x] mtrace
  - [x] LA32r difftest适配
  - [x] difftest
  - [ ] dtrace
  - [ ] ftrace
- [x] 添加基础指令
- [x] 简易标准库实现
- [ ] 外设支持
  - [ ] 串口
  - [ ] 时钟
  - [ ] VGA
  - [ ] 声卡

### PA3
- [ ] 支持上下文切换
- [ ] 模拟Cache
- [ ] Loader 实现
- [ ] 系统调用支持
  - [ ] yield
  - [ ] exit
  - [ ] write
  - [ ] brk
  - [ ] gettimeofday
  - [ ] execve
- [ ] 简易文件系统
  - [ ] 键盘抽象
  - [ ] VGA抽象
  - [ ] 实现定点数库fixedptc
- [ ] 跑通仙剑奇侠传
- [ ] 基础设施
  - [ ] etrace
  - [ ] strace
  - [ ] 快照实现
### PA4

## 参考资料
- [南大PA文档2023](https://nju-projectn.github.io/ics-pa-gitbook/ics2023/)
- [2022年OS大赛开源作品:la-sel4](https://github.com/oscomp/proj97-la-seL4)
- [2022年OS大赛开源作品:rCore的龙芯平台移植](https://github.com/Godones/rCoreloongArch)
- [la32r 工具链](https://gitee.com/loongson-edu/la32r-toolchains)
- [la32 gdb安装](https://blog.csdn.net/greenmoss/article/details/127800221)
- [计算机体系结构基础](https://foxsen.github.io/archbase/)
- [基于QMP实现对qemu虚拟机进行交互](https://zhuanlan.zhihu.com/p/56887210)
- 一生一芯双周分享会(nemu部分):[视频回放](https://space.bilibili.com/238318574?spm_id_from=333.788.0.0)、[相关主题的分享文档](https://docs.qq.com/sheet/DU05xUmxjWmFvaXhj?tab=8nd1jt&login_t=1715061395395)
