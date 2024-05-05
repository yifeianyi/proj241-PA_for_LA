## 项目名称：

proj241-PA_for_LA

## 项目描述：

## 调研情况与分析：

## 项目目标：

.
## 项目管理 & 团队协作：

- [项目管理与协作细节](https://hxu6i1coj1.feishu.cn/base/WWjRbUz3taGa0UsBuEycoVapngk?table=tbl6xudDrnehQGnQ&view=veweOItODk)：涵盖本项目的所有推进细节。
- [git 协同开发流程](): 本项目使用git进行团队协作的经验总结。

## 文档与演示

- 开发日志
  - 李宗逸：[DevLog_lzy](https://gitlab.eduxiji.net/T202412059992867/project2210132-237696/-/wikis/Dev_log/lzy)
  - 刘奕钊：[DevLog_lyz](https://gitlab.eduxiji.net/T202412059992867/project2210132-237696/-/wikis/Dev_log/lyz)
  - 李智峰：[DevLog_lzf](https://gitlab.eduxiji.net/T202412059992867/project2210132-237696/-/wikis/Dev_log/lzf)
- 技术文档
  - 适配文档：
    - loongarch32r工具链构建、编译
    - 简易LA32r反汇编器: llvm 不支持
    - qemu-system-loongarch32 difftest适配笔记
  - 实现笔记:
    - 
- 实验讲义：
  - [PA_for_LA_Tutorial](NULL):ToDo

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