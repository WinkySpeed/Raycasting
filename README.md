# 基于SFML的Raycasting实现

[SFML](https://www.sfml-dev.org/)是一个很容易上手的多媒体图形库

[Raycasting](https://en.wikipedia.org/wiki/Ray_casting)是约翰卡马克运用在《德军总部3D》里实现第一人称射击视角的方法，早期第一人称射击游戏基本都沿用这个方法

作为一个游戏玩家，我对世界上第一款第一人称射击游戏的实现方法非常感兴趣，于是就想着能不能复现出来基本算法

于是找到了原教程：[Raycasting (lodev.org)](https://lodev.org/cgtutor/raycasting.html)

仔细研究一下发现确实不难，基础实现只涉及到了初中几何知识与最基本的二维矩阵乘法运算，绘制Sprite部分涉及到了线性代数中的“基变换”

只要具备阅读英文教程的能力和使用简单图形库的实践经验，就可以复现这个有趣的伪3D效果

实现的全部具体过程已经放在个人博客上：[Raycasting](https://winkyspeed.github.io/2023/11/09/学习记录/Raycasting学习记录/)（包含基本实现、纹理墙壁绘制）和[Raycasting_进阶与扩展](https://winkyspeed.github.io/2024/05/24/学习记录/Raycasting学习记录-进阶与扩展/)（包含纹理地板天花板绘制、鼠标控制视角旋转、Sprite绘制），可供参考



# Raycasting-Basic

用二维数组和竖直线条实现了最基本的Raycasting和人物移动逻辑



# Raycasting-Walltexture

在基本实现的基础上添加墙壁纹理，并且修改地图



# Raycasting-Demonstration

在墙壁纹理的基础上，添加小地图和纯色地板天花板，用于课程设计演示

个人感觉最出彩的地方在于，小地图逐渐缩小到左上角，按一下回车键，从2D的平面瞬间切换出一个伪3D的环境，这种二维和三维的强烈反差带来的视觉震撼还是挺有意思的



# Raycasting-Final

在墙壁纹理的基础上：

- 添加了纹理地板天花板
- 增加了鼠标控制视角逻辑
- 增加了《德军总部3D》里木桶、灯、柱子的Sprite绘制
- 增加了《白色相簿2》人物立绘的绘制





# 灵感来源与参考资料

在2023年的暑假，我在B站无意间发现了这个视频：[给二维数组一点小小的伪3D震撼](https://www.bilibili.com/video/BV1AN411S7e9)

然后顺藤摸瓜，找到了原教程：[Raycasting (lodev.org)](https://lodev.org/cgtutor/raycasting.html)

复现期间还参考了一个[Youtuber](https://www.youtube.com/@Kofybrek)制作的两期Raycasting实现视频