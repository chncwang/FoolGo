基于蒙特卡罗树搜索的围棋A.I.

目前只有在9路棋盘上表现出入门级棋力。

底层的数据结构和算法相当高效，在我的MacBook Air上1秒钟能跑4万局蒙特卡罗对局（多线程）。

关键方法：

* 并查集实现的棋串（简单的链表实现，若改为树结构，加上路径压缩，会更高效些，但对整体的表现应该影响不大）。
* 位或运算实现棋串『气』的更新。
* 棋盘的哈希算法用Zobrist哈希，通过哈希表存放每个状态的胜率等信息

在面向对象设计上，这个项目高度可扩展（参考了刘知青教授的《现代计算机围棋基础》），介绍主要的几个类：

* FullBoard类，功能完整的棋盘类，提供高效的落子方法等。
* Player抽象类，提供可扩展的棋步选择方法，子类如RandomPlayer，就是Player类的随机落子的实现。
* Game类，Player类的组合，比如子类MonteCarloGame就组合了两个RandomPlayer。

C++编码指南

* 本项目遵循google的guideline中的多数条目。
* 出于性能的考虑，本项目大量使用模板类，在编译期确定棋盘的大小，以便尽可能多地使用静态数组。
* C + class + 模板，不使用模板元编程。
* 尽量用智能指针管理内存，极少数地方用了raw pointer（方法名New前缀），杜绝内存泄露。
* 用gtest做单元测试，用log4cplus打log

## TODO
如果你有兴趣模仿一下AlphaGo，并且也不喜欢python，那么基于这个项目，从实现论文中的rollout policy network开始，替换掉原来的RandomPlayer，是个非常好的选择。

有想过基于tensorflow，开始做这个事情，大致的步骤：
1. 只过一个隐层实现rollout policy network
2. 根据论文中的列特征，加特征，以期达到论文中rollout policy network的准确率（也有论文提到可以用CNN实现rollout policy network）
3. 用单层的CNN，实现policy network
4. 把policy network融合进蒙特卡罗树搜索
5. 尝试用多层CNN（估计不用指望能达到论文中的准确率）
6. 以上步骤都采用通过人类的棋谱有监督学习，至于value network和强化学习部分，我还不太会，欢迎邮件交流

本人实在是没时间继续写这个项目，欢迎有兴趣的同学pull request，或者邮件交流~

## Make
* 进入项目根目录
* cmake .
* make
* 不怎么会用CMake，所以第三方库的构建没有纳入CMake的管理，需要根据错误信息在特定的目录下放有相关依赖文件（这个项目目前只依赖gtest和log4cplus）

## 问题交流
* Send me an email : chncwang@gmail.com
