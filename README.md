以下为游戏的内容介绍

----------------------------------------------------------------------------------------

所有玩家有 8 个属性：
RP 人品
DP 动态规划能力
DS 数据结构能力
GT 图论能力
SM 模拟能力
FS 家庭支持
SC 自信值
AG 年龄。

----------------------------------------------------------------------------------------

每个事件有 12 个属性：

1. 编号，也就是 id；
2. 触发条件；
3. 事件消息；
4. 下一个必定触发的事件（可以没有）；
5. 发生概率提升的事件（可以没有）； 
6. 发生后对 RP 的影响值（加或减）； 
7. 发生后对 DP 的影响值（加或减）；
8. 发生后对 DS 的影响值（加或减）；
9. 发生后对 GT 的影响值（加或减）； 
10. 发生后对 SM 的影响值（加或减）；
11. 发生后对 FS 的影响值（加或减）；
12. 发生后对 SC 的影响值（加或减）； 
13. 发生后对 AG 的影响值（加或减）；



\#您可以使用以下方式注册并创建事件：

使用 '#' 注释.

触发条件的规范写法：

简单条件定义：

<属性名> <二元操作符> <值>

或者

<一元运算符> <值> 

二元操作符包括 ==、!=、>、>=、<、<=，其作用望文生义。
一元运算符只包括两个：#、@。# 表示“该事件曾经发生过”，@ 表示“该事件未发生过”。

举例如下：

```
DP > 10 ------- DP 大于 10

AG <= 18 ------- AG 小于等于 18

\#10002 ------- 编号 10002 的事件发生过

@ 20003 ------- 编号 20003 的事件从未发生过
```

----------------------------------------------------------------------------------------

复合条件定义：

```(<条件>) <关系运算符> (<条件>)```

或者

```!(条件)```

注意，这里的括号必不可少。

关系运算符包括 && 和 ||，作用为与、或。
对于第二种复合条件，作用是将括号内的条件取反。

举例如下：

```
(DP > 10) && (AG <= 18) -------  DP 大于 10 并且 AG 小于等于 18
(SC > 10) || ((AG >= 10) && (DP >= 20)) -------  SC 大于 10，或者 AG 大于等于 10 并且 DP 大于等于 20
!((AG >= 10) && (DP >= 20)) 对 AG 大于等于 10 并且 DP 大于等于 20 取反
```

注意：``(AG >= 10) && (DP >= 20) && (SC >= 10) ``这种表达式是不合法的。

必须写成 ``((AG >= 10) && (DP >= 20)) && (SC >= 10)``。每个括号内最多只能有一个运算符号，多余的请用额外的括号括起来。 

----------------------------------------------------------------------------------------

注册事件的命令如下：

```<事件id> (<条件>) (<影响>) <必触发的事件> [<发生概率提升事件id>, <发生概率提升事件id>, ...] <事件消息>```

注意：这里所有的小括号和中括号都不可少。

如果没有必触发的事件，请填写 -1；
如果事件可以无条件发生、或者该事件没有影响，请使用一对没有内容的括号 "()" 代替（没有引号）。这可能会引起事件处理器的警告，不过没有关系。
如果没有发生概率提升的事件，请使用一对没有内容的括号 "[]" 代替（没有引号），不要写 -1。

影响指的是对玩家属性值的影响。具体格式为：

```<属性名> <变化量>, <属性名> <变化量>....```

举例如下：

```SC +10, DP + 1, FS + 5```

意思是这件事会让玩家的 SC 加上 10、DP 加上 1、FS 加上 5。

规范的注册事件命令，举例如下：

```4080 ((DS >= 10) && (AG <= 18)) (SC +10, DP + 1, FS + 5) -1 [4081, 4082] You AK CSP-J 3000```

具体含义为：

- 注册一个 id 为 4080 的事件；
- 发生的条件为：DS 大于等于 10 且 AG 小于等于 18；
- 发生后，将会让玩家的 SC 加上 10、DP 加上 1、FS 加上 5；
- 下一个必然触发的事件 id 为 -1，也就是没有；
- 事件 id 4081、4082 的事件发生概率获得提升；
- 这件事的消息为 You AK CSP-J 3000。

----------------------------------------------------------------------------------------=

