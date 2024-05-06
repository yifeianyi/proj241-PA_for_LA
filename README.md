## 队伍
run的全都队
## 赛题

南大PA实验的LoongArch支持 :[proj241-la-pa](https://github.com/oscomp/proj241-la-pa)

**所属赛道** ：2023全国大学生操作系统比赛的“OS功能挑战”赛道

如果你觉得本团队的工作有参考价值😊，请不要吝惜你的⭐
## 项目名称：

proj241-PA_for_LA

## 项目描述：
对南京大学的PA实验系统进行扩展，添加LoongArch架构支持。

## 调研分析：

## 项目目标：

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
| 李宗逸 | 统筹规划、资料调研、架构适配 | [Dev_lzy](https://gitlab.eduxiji.net/T202412059992867/project2210132-237696/-/wikis/Dev_log/lzy) | QQ:910002662 |
| 刘奕钊 | 校对 | [Dev_lyz](https://gitlab.eduxiji.net/T202412059992867/project2210132-237696/-/wikis/Dev_log/lyz) |         |
| 李智峰 |          | [Dev_lzf](https://gitlab.eduxiji.net/T202412059992867/project2210132-237696/-/wikis/Dev_log/lzf) | QQ:1186288906          |



- [项目管理与协作细节](https://hxu6i1coj1.feishu.cn/base/WWjRbUz3taGa0UsBuEycoVapngk?table=tbl6xudDrnehQGnQ&view=veweOItODk)：涵盖本项目的所有推进细节。
- [git 协同开发流程](): 本项目使用git进行团队协作的经验总结。

## 文档与演示
- 技术文档
  - 适配文档：
    - loongarch32r工具链构建、编译
    - 简易LA32r反汇编器: llvm 不支持
    - qemu-system-loongarch32 difftest适配笔记
  - 实现笔记
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
- [ ] 添加基础指令
- [ ] 简易标准库实现
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
- [2022年OS大赛开源作品]()
- [la32r 工具链](https://gitee.com/loongson-edu/la32r-toolchains)
- [la32 gdb安装](https://blog.csdn.net/greenmoss/article/details/127800221)
- [计算机体系结构基础](https://foxsen.github.io/archbase/)
- [基于QMP实现对qemu虚拟机进行交互](https://zhuanlan.zhihu.com/p/56887210)