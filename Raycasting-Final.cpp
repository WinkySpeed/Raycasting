#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;
using namespace sf;

#define screenWidth 1440
#define screenHeight 900
#define mapWidth 24
#define mapHeight 24
// 地板天花板、墙壁、decoration纹理宽高
// 纹理图片都来自《德军总部3D》，因此宽高一样
#define texWidth 64
#define texHeight 64
// 地板天花板墙壁纹理总数
#define textureNum_Wall 8
// decoration纹理总数
#define textureNum_Decoration 3
// illustration纹理总数
#define textureNum_Illustration 12
// decoration总数
#define decorationNum 19
// illustration总数
#define illustrationNum 12
// 人物立绘宽高
// 由于人物立绘各个尺寸不一样，因此需要把所有图片缩放到370x690
// 缩放已经通过python实现，此处代码不考虑
#define illustrationWidth 370
#define illustrationHeight 690

int worldMap[mapWidth][mapHeight] =
    {
        {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 7, 7, 7, 7, 7, 7, 7, 7},
        {4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 7},
        {4, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7},
        {4, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7},
        {4, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 7},
        {4, 0, 4, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 7, 7, 0, 7, 7, 7, 7, 7},
        {4, 0, 5, 0, 0, 0, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 7, 0, 0, 0, 7, 7, 7, 1},
        {4, 0, 6, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 5, 7, 0, 0, 0, 0, 0, 0, 8},
        {4, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 7, 1},
        {4, 0, 8, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 5, 7, 0, 0, 0, 0, 0, 0, 8},
        {4, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 5, 7, 0, 0, 0, 7, 7, 7, 1},
        {4, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 0, 5, 5, 5, 5, 7, 7, 7, 7, 7, 7, 7, 1},
        {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
        {8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
        {6, 6, 6, 6, 6, 6, 0, 6, 6, 6, 6, 0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
        {4, 4, 4, 4, 4, 4, 0, 4, 4, 4, 6, 0, 6, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3},
        {4, 0, 0, 0, 0, 0, 0, 0, 0, 4, 6, 0, 6, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2},
        {4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 2, 0, 0, 5, 0, 0, 2, 0, 0, 0, 2},
        {4, 0, 0, 0, 0, 0, 1, 0, 0, 4, 6, 0, 6, 2, 0, 0, 0, 0, 0, 2, 2, 0, 2, 2},
        {4, 0, 6, 0, 6, 0, 0, 0, 0, 4, 6, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 2},
        {4, 0, 0, 5, 0, 0, 0, 0, 0, 4, 6, 0, 6, 2, 0, 0, 0, 0, 0, 2, 2, 0, 2, 2},
        {4, 0, 6, 0, 6, 0, 0, 0, 0, 4, 6, 0, 6, 2, 0, 0, 5, 0, 0, 2, 0, 0, 0, 2},
        {4, 0, 0, 0, 0, 0, 0, 0, 0, 4, 6, 0, 6, 2, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2},
        {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1, 1, 1, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3}};

double posX = 22.5, posY = 11.5;  // x and y start position
double dirX = -1, dirY = 0;       // initial direction vector
double planeX = 0, planeY = 0.66; // the 2d raycaster version of camera plane
double FOV;                       // FOV视角大小，采用弧度制

double new_Time = 0; // time of current frame
double old_Time = 0; // time of previous frame

Texture texture[textureNum_Wall]; // 墙面、地板天花板纹理数组
                                  // decoration和illustration纹理数组单独开
Clock Myclock;                    // 用于计时，计算这一帧和上一帧的时间

Image Ceiling_image; // 天花板纹理图像，用于(tx, ty)获取颜色
Image Floor_image;   // 地板纹理图像，用于(tx, ty)获取颜色

// debug用，每次手动注释掉鼠标的逻辑烦死我了
bool ifMouse = 1;

// ZBuffer数组，记录屏幕上每个x对应的perpWallDist
double ZBuffer[screenWidth] = {0};

/************************************************************/
/* Decoration
/************************************************************/
// decoration纹理数组
Texture decoration_texture[textureNum_Decoration];
class Decoration
{
public:
    double x;
    double y;
    int texture_index;
    int type; // 用于指明是decoration还是illustration
              // 0为decoration，1为illustration
    Decoration(double x = 0, double y = 0, int texindex = 0)
    {
        this->x = x;
        this->y = y;
        this->texture_index = texindex;
        this->type = 0;
    }
};
// decoration数组，用于记录全部decoration的位置和对应纹理下标
Decoration decoration_lst[decorationNum] =
    {
        {20.5, 11.5, 0}, // green light in front of playerstart
        // green lights in every room
        {18.5, 4.5, 0},
        {10.0, 4.5, 0},
        {10.0, 12.5, 0},
        {3.5, 6.5, 0},
        {3.5, 20.5, 0},
        {3.5, 14.5, 0},
        {14.5, 20.5, 0},

        // row of pillars in front of wall: fisheye test
        {18.5, 10.5, 1},
        {18.5, 11.5, 1},
        {18.5, 12.5, 1},

        // some barrels around the map
        {21.5, 1.5, 2},
        {15.5, 1.5, 2},
        {16.0, 1.8, 2},
        {16.2, 1.2, 2},
        {3.5, 2.5, 2},
        {9.5, 15.5, 2},
        {10.0, 15.1, 2},
        {10.5, 15.8, 2},
};
// arrays used to sort the sprites
// Order数组和Distance数组，分别存储下标和离玩家的距离
int decoration_Order[decorationNum];
double decoration_Distance[decorationNum];

/************************************************************/
/* illustration
/************************************************************/
// illustration纹理数组
Texture illustration_texture[textureNum_Illustration];
class Illustration
{
public:
    double x;
    double y;
    int texture_index;
    int type; // 用于指明是decoration还是illustration
              // 0为decoration，1为illustration
    Illustration(double x = 0, double y = 0, int texindex = 0)
    {
        this->x = x;
        this->y = y;
        this->texture_index = texindex;
        this->type = 1;
    }
};
// illustration数组，用于记录全部illustration的位置和对应纹理下标
Illustration illustration_lst[illustrationNum] =
    {
        // 北原春希立绘，刚进游戏就能看见
        {20.6, 11.5, 0},
        // 小木曾雪菜流汗黄豆立绘
        {18, 3.25, 1},
        // 小木曾雪菜怒立绘
        {8.0, 4.5, 2},
        // 小木曾雪菜哭1
        {21, 8, 3},
        // 小木曾雪菜哭2
        {22.5, 3.5, 4},
        // 冬马和纱斗鸡眼
        {17.1, 14.7, 5},
        // 冬马和纱怒
        {22.5, 18.5, 6},
        // 冬马和纱闭眼
        {16.8, 21.5, 7},
        // 冬马和纱笑
        {3, 12, 8},
        // 杉浦小春
        {10.1, 18.3, 9},
        // 和泉千晶
        {2.5, 21.2, 10},
        // 风冈麻理
        {6.4, 19.2, 11}};
// arrays used to sort the sprites
// Order数组和Distance数组，分别存储下标和离玩家的距离
int illustration_Order[illustrationNum];
double illustration_Distance[illustrationNum];

// function used to sort the sprites
// 此函数用于单独画decoration或单独画illustration
// 如果一起画，则用sortAllSprites函数
void sortSprites(int *order, double *dist, int amount);

/************************************************************/
/* 为了防止decoration和illustration互相的透明遮挡问题
/* 采用两种Sprite一起画的方法，反正这两种单独画的逻辑也是完全一样
/* 为此，需要声明MySprite类，和之前两种Sprite成员变量一样
/* 不需要声明新的纹理数组，可以直接复用decoration和illustration的纹理数组
/* 声明Order和Distance数组，并且初始化数目为Sprite总数
/* 已知Sprite总数为 decorationNum + illustrationNum
/* 需要一个能存下全部Sprite的数组Allsprites_lst，记录位置和下标
/* 还需要对应的排序函数sortAllSprites
/************************************************************/
/************************************************************/
/* MySprite，用于创建数组
/************************************************************/
class MySprite
{
public:
    double x;
    double y;
    int texture_index;
    int type; // 用于指明是decoration还是illustration
              // 0为decoration，1为illustration
    class MySprite(double x = 0, double y = 0, int texindex = 0, int type = -1)
    {
        this->x = x;
        this->y = y;
        this->texture_index = texindex;
        this->type = type;
    }
};
// 全部Sprite的数组，记录位置和下标
MySprite Allsprites_lst[decorationNum + illustrationNum] =
    {
        /************************************************************/
        /* Decoration
        /************************************************************/
        // green light in front of playerstart
        {20.5, 11.5, 0, 0},
        // green lights in every room
        {18.5, 4.5, 0, 0},
        {10.0, 4.5, 0, 0},
        {10.0, 12.5, 0, 0},
        {3.5, 6.5, 0, 0},
        {3.5, 20.5, 0, 0},
        {3.5, 14.5, 0, 0},
        {14.5, 20.5, 0, 0},

        // row of pillars in front of wall: fisheye test
        {18.5, 10.5, 1, 0},
        {18.5, 11.5, 1, 0},
        {18.5, 12.5, 1, 0},

        // some barrels around the map
        {21.5, 1.5, 2, 0},
        {15.5, 1.5, 2, 0},
        {16.0, 1.8, 2, 0},
        {16.2, 1.2, 2, 0},
        {3.5, 2.5, 2, 0},
        {9.5, 15.5, 2, 0},
        {10.0, 15.1, 2, 0},
        {10.5, 15.8, 2, 0},

        /************************************************************/
        /* illustration
        /************************************************************/
        // 北原春希立绘，刚进游戏就能看见
        {20.6, 11.5, 0, 1},
        // 小木曾雪菜流汗黄豆立绘
        {18, 3.25, 1, 1},
        // 小木曾雪菜怒立绘
        {8.0, 4.5, 2, 1},
        // 小木曾雪菜哭1
        {21, 8, 3, 1},
        // 小木曾雪菜哭2
        {22.5, 3.5, 4, 1},
        // 冬马和纱斗鸡眼
        {17.1, 14.7, 5, 1},
        // 冬马和纱怒
        {22.5, 18.5, 6, 1},
        // 冬马和纱闭眼
        {16.8, 21.5, 7, 1},
        // 冬马和纱笑
        {3, 12, 8, 1},
        // 小春
        {10.1, 18.3, 9, 1},
        // 千晶
        {2.5, 21.2, 10, 1},
        // 麻理
        {6.4, 19.2, 11, 1}

};
// Order数组，并且初始化数目为decorationNum + illustrationNum
vector<int> Allsprites_Order(decorationNum + illustrationNum);
// Distance数组，并且初始化数目为decorationNum + illustrationNum
vector<double> Allsprites_Distance(decorationNum + illustrationNum);
// 全部Sprite的排序函数，用于画出全部Sprite
// 注意传递vector的引用
void sortAllSprites(vector<int> &Allsprites_Order, vector<double> &Allsprites_Distance, int amount);

// 获取时间
unsigned long getTicks();
// 读取纹理，分别读取墙面地板天花板纹理、decoration纹理、illustration纹理
// 并且在其中调用修改alpha的函数，防止出现纯黑遮挡
void LoadTexture();
// 修改纹理透明度，用于decoration和illustration纹理纯黑颜色修改为透明 (Alpha通道改为0)
void SetTexture_Alpha(Texture *texture, int start_index, int end_index);
// 纹理墙壁绘制
void Wall_Casting(RenderWindow &window);
// 纹理地板天花板绘制
void Floor_Ceiling_Casting(RenderWindow &window);
// 移动函数，用于改变位置和朝向
// 已添加鼠标操作逻辑
void Move(RenderWindow &window);
// 计算FOV大小，弧度制
double calculate_FOV();
// 画出Decoration Sprite
void Decoration_Casting(RenderWindow &window);
// 画出illustration Sprite
void Illustration_Casting(RenderWindow &window);
// 画出全部Sprite
void Allsprites_Casting(RenderWindow &window);

/************************************************************/
/* 绘制小地图
/************************************************************/
// 小地图的宽度和高度
const int minimapWidth = 24;
const int minimapHeight = 24;
// 小地图的大小（单位：像素）
const int miniMapScale = 15;
bool ifdraw_Minimap = 1;
// 创建一个表示视野范围的三角扇形
VertexArray FOV_Visualize(TriangleFan, screenWidth + 1);
// 记录视野范围每一处的位置
// 在墙壁绘制函数里修改值，在drawMiniMap函数里设置FOV位置
vector<Vector2f> FOV_position(screenWidth + 1);
// 绘制小地图函数
void drawMiniMap(RenderWindow &window);

int main()
{
    RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Raycasting-Final");

    // 加载纹理
    LoadTexture();

    // 是否允许鼠标逻辑控制
    if (ifMouse)
    {
        Mouse::setPosition(Vector2i(screenWidth * 0.5f, screenHeight * 0.5f), window);
        window.setMouseCursorVisible(0);
    }

    // 计算FOV视角
    FOV = calculate_FOV();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            // 考虑到鼠标操作，使用ESC退出
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();
            // sf::Keyboard::isKeyPressed函数会在每一帧都检测键盘的状态
            // sf::Event::KeyPressed事件只会在按键被按下的那一帧触发一次，即使你按住按键不放，它也不会在下一帧再次触发
            // 因此，使用sf::Event::KeyPressed事件可以确保ifdraw_Minimap的值只会被翻转一次
            if (event.type == sf::Event::KeyPressed)
            {
                // 按Tab键、~键或M键开关小地图
                if (event.key.code == sf::Keyboard::Tab || event.key.code == sf::Keyboard::Tilde || event.key.code == sf::Keyboard::M)
                {
                    ifdraw_Minimap = !ifdraw_Minimap;
                }
            }
        }

        window.clear();
        Floor_Ceiling_Casting(window);
        Wall_Casting(window);
        // Decoration_Casting(window);
        // Illustration_Casting(window);

        Allsprites_Casting(window);

        Move(window);

        if (ifdraw_Minimap)
        {
            drawMiniMap(window);
        }

        // cout << "posX: " << posX << " posY: " << posY << endl;

        window.display();
    }

    return 0;
}

unsigned long getTicks()
{
    sf::Time elapsed = Myclock.getElapsedTime();
    return elapsed.asMilliseconds();
}

void LoadTexture()
{
    // 读取墙壁、地板天花板纹理
    texture[0].loadFromFile("pics/eagle.png");
    texture[1].loadFromFile("pics/redbrick.png");
    texture[2].loadFromFile("pics/purplestone.png");
    texture[3].loadFromFile("pics/greystone.png");
    texture[4].loadFromFile("pics/bluestone.png");
    texture[5].loadFromFile("pics/mossy.png");
    texture[6].loadFromFile("pics/wood.png");
    texture[7].loadFromFile("pics/colorstone.png");
    // 地板天花板渲染用的image
    Ceiling_image = texture[6].copyToImage(); // 天花板为木头
    Floor_image = texture[3].copyToImage();   // 地板为石头
    // 读取decoration纹理
    decoration_texture[0].loadFromFile("pics/greenlight.png");
    decoration_texture[1].loadFromFile("pics/pillar.png");
    decoration_texture[2].loadFromFile("pics/barrel.png");
    // 纯黑部分改透明
    SetTexture_Alpha(decoration_texture, 0, 2);
    // 读取illustration纹理
    illustration_texture[0].loadFromFile("illustration/haruki.png");
    illustration_texture[1].loadFromFile("illustration/setsuna0.png");
    illustration_texture[2].loadFromFile("illustration/setsuna1.png");
    illustration_texture[3].loadFromFile("illustration/setsuna2.png");
    illustration_texture[4].loadFromFile("illustration/setsuna3.png");
    illustration_texture[5].loadFromFile("illustration/kazusa0.png");
    illustration_texture[6].loadFromFile("illustration/kazusa1.png");
    illustration_texture[7].loadFromFile("illustration/kazusa2.png");
    illustration_texture[8].loadFromFile("illustration/kazusa3.png");
    illustration_texture[9].loadFromFile("illustration/koharu.png");
    illustration_texture[10].loadFromFile("illustration/chiaki.png");
    illustration_texture[11].loadFromFile("illustration/mari.png");
    // 纯黑部分改透明
    SetTexture_Alpha(illustration_texture, 0, 11);
}

void SetTexture_Alpha(Texture *texture, int start_index, int end_index)
{
    for (int i = start_index; i <= end_index; i++)
    {
        Image tmp = texture[i].copyToImage();
        for (int y = 0; y < tmp.getSize().y; y++)
        {
            for (int x = 0; x < tmp.getSize().x; x++)
            {
                if (tmp.getPixel(x, y) == Color::Black)
                {
                    // 设置为完全透明
                    tmp.setPixel(x, y, sf::Color(0, 0, 0, 0));
                }
            }
        }
        texture[i].loadFromImage(tmp); // 用修改后的图像覆盖原数组的纹理
    }
}

void Floor_Ceiling_Casting(RenderWindow &window)
{
    // 创建一个新的Image
    Image Floor_Ceiling_image;
    Floor_Ceiling_image.create(screenWidth, screenHeight);

    // rayDir for leftmost ray (x = 0) and rightmost ray (x = w)
    float FOV_StartX = dirX - planeX;
    float FOV_StartY = dirY - planeY;
    float FOV_EndX = dirX + planeX;
    float FOV_EndY = dirY + planeY;

    // Vertical position of the camera.
    float posZ = 0.5 * screenHeight;

    // FLOOR AND CEILING CASTING
    for (int y = screenHeight / 2; y < screenHeight; y++) // 其实可以直接从screenHeight / 2开始遍历
    // for (int y = 0; y < screenHeight; y++)
    {
        // Current y position compared to the center of the screen (the horizon)
        int row_y = y - screenHeight / 2;

        // Horizontal distance from the camera to the floor for the current row.
        // 0.5 is the z position exactly in the middle between floor and ceiling.
        float rowDistance = posZ / row_y;

        // real world coordinates of the leftmost column. This will be updated as we step to the right.
        float floorX = posX + rowDistance * FOV_StartX;
        float floorY = posY + rowDistance * FOV_StartY;

        // calculate the real world step vector we have to add for each x (parallel to camera plane)
        // adding step by step avoids multiplications with a weight in the inner loop
        float floorStepX = rowDistance * (FOV_EndX - FOV_StartX) / screenWidth;
        float floorStepY = rowDistance * (FOV_EndY - FOV_StartY) / screenWidth;

        for (int x = 0; x < screenWidth; ++x)
        {
            // the cell coord is simply got from the integer parts of floorX and floorY
            int cellX = (int)(floorX);
            int cellY = (int)(floorY);

            // get the texture coordinate from the fractional part
            int tx = (int)(texWidth * (floorX - cellX)) & (texWidth - 1);
            int ty = (int)(texHeight * (floorY - cellY)) & (texHeight - 1);

            floorX += floorStepX;
            floorY += floorStepY;

            // 计算像素颜色，逐像素赋值
            Color Floorcolor = Floor_image.getPixel(tx, ty);
            Color Ceilingcolor = Ceiling_image.getPixel(tx, ty);
            Floor_Ceiling_image.setPixel(x, y, Floorcolor);
            Floor_Ceiling_image.setPixel(x, screenHeight - y - 1, Ceilingcolor);
        }
    }
    // 创建一个新的Texture
    Texture Floor_Ceiling_texture;
    Floor_Ceiling_texture.loadFromImage(Floor_Ceiling_image);
    // 创建一个新的Sprite并设置其纹理
    Sprite sprite;
    sprite.setTexture(Floor_Ceiling_texture);
    // 把这个Sprite画在屏幕上
    window.draw(sprite);
}

void Wall_Casting(RenderWindow &window)
{
    for (int x = 0; x < screenWidth; x++)
    {
        // calculate ray position and direction
        double cameraX = 2 * x / (double)screenWidth - 1; // x-coordinate in camera space
        double rayDirX = dirX + planeX * cameraX;
        double rayDirY = dirY + planeY * cameraX;
        // which box of the map we're in
        int mapX = int(posX);
        int mapY = int(posY);

        // length of ray from current position to next x or y-side
        double sideDistX;
        double sideDistY;

        /************************************************************/
        /* 避免鱼眼效应相关代码
        /************************************************************/
        // 此段代码会避免鱼眼效应，而不是修正鱼眼效应
        double deltaDistX = (rayDirX == 0) ? 1e30 : std::abs(1 / rayDirX);
        double deltaDistY = (rayDirY == 0) ? 1e30 : std::abs(1 / rayDirY);

        //// 此段代码会造成鱼眼效应
        // double rayDir_len = sqrt(rayDirX * rayDirX + rayDirY * rayDirY);
        // double deltaDistX = (rayDirX == 0) ? 1e30 : std::abs(rayDir_len / rayDirX);
        // double deltaDistY = (rayDirY == 0) ? 1e30 : std::abs(rayDir_len / rayDirY);

        double perpWallDist;

        // what direction to step in x or y-direction (either +1 or -1)
        int stepX;
        int stepY;

        int hit = 0; // was there a wall hit?
        int side;    // was a NS or a EW wall hit?
        // calculate step and initial sideDist
        if (rayDirX < 0)
        {
            stepX = -1;
            sideDistX = (posX - mapX) * deltaDistX;
        }
        else
        {
            stepX = 1;
            sideDistX = (mapX + 1.0 - posX) * deltaDistX;
        }
        if (rayDirY < 0)
        {
            stepY = -1;
            sideDistY = (posY - mapY) * deltaDistY;
        }
        else
        {
            stepY = 1;
            sideDistY = (mapY + 1.0 - posY) * deltaDistY;
        }
        // perform DDA
        while (hit == 0)
        {
            // jump to next map square, either in x-direction, or in y-direction
            if (sideDistX < sideDistY)
            {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            }
            else
            {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }
            // Check if ray has hit a wall
            if (worldMap[mapX][mapY] > 0)
                hit = 1;
        }
        // 教程里这部分解释的比较清楚了
        // 也可以看我在博客里的证明过程，写的更清楚
        if (side == 0)
            perpWallDist = (sideDistX - deltaDistX);
        else
            perpWallDist = (sideDistY - deltaDistY);

        /*********************************************************************************/
        /*  此部分为添加FOV_Visualize用
        /*********************************************************************************/
        // 计算射线终点
        double rayEndX = posX + perpWallDist * rayDirX;
        double rayEndY = posY + perpWallDist * rayDirY;

        // 记录三角扇形顶点位置
        FOV_position[x + 1] = Vector2f(rayEndX * miniMapScale, (mapHeight - rayEndY) * miniMapScale);

        /*********************************************************************************/
        /* 添加ZBuffer
        /*********************************************************************************/
        ZBuffer[x] = perpWallDist;

        // Calculate height of line to draw on screen
        // 可以任意修改墙的高度
        int lineHeight = (int)(screenHeight / perpWallDist);

        // calculate lowest and highest pixel to fill in current stripe
        int drawStart = -lineHeight / 2 + screenHeight / 2;
        if (drawStart < 0)
            drawStart = 0;
        int drawEnd = lineHeight / 2 + screenHeight / 2;
        if (drawEnd >= screenHeight)
            drawEnd = screenHeight - 1;

        /*********************************************************************************/
        /*  此部分替换了原raycasting画线部分
        /*  以下为纹理相关计算，是新内容
        /*  具体和原来的画线函数差不多，都是在具体位置上画出一条线
        /*  只不过这里画的是具体纹理上的一条线，利用了SFML中的sprite
        /*********************************************************************************/

        // texturing calculations
        int texNum = worldMap[mapX][mapY] - 1; // 1 subtracted from it so that texture 0 can be used!

        // calculate value of wallX
        double wallX; // where exactly the wall was hit
        if (side == 0)
            wallX = posY + perpWallDist * rayDirY;
        else
            wallX = posX + perpWallDist * rayDirX;
        wallX -= floor((wallX));

        // x coordinate on the texture
        int texX = int(wallX * double(texWidth));
        // 防止镜像
        if (side == 0 && rayDirX > 0)
            texX = texWidth - texX - 1;
        if (side == 1 && rayDirY < 0)
            texX = texWidth - texX - 1;

        // 为了获取纹理具体某一个坐标的像素颜色
        // 创建一个新的sprite对象
        sf::Sprite wall_sprite;

        // 设置sprite的纹理
        wall_sprite.setTexture(texture[texNum]);

        // 设置sprite的纹理矩形，定义了在纹理中的哪个部分应用到wall_sprite上
        // 宽度为1个像素，相当于原来的画线
        wall_sprite.setTextureRect(sf::IntRect(texX, 0, 1, texHeight));

        // 设置sprite的位置，坐标指定了左上角的位置
        // wall_sprite.setPosition(x, drawStart);        //太几把恶心了，破案了：指定左上角的位置为drawStart
        //                                               //drawStart在特殊情况下会被赋值为0，即if (drawStart < 0) drawStart = 0;
        //                                               //在此情况下绘制的起点就会改变为0，令人恶心
        wall_sprite.setPosition(x, round(0.5f * screenHeight - 0.5f * lineHeight));

        // 设置sprite的缩放因子
        wall_sprite.setScale(1, lineHeight / (float)texHeight);

        // 利用SFML中sprite的setColor功能，使y墙变暗
        // 这个函数实现的是“调制混合”
        // 举个例子：原来的R通道为200，与128混合为：200 * 128 / 255 = 100，实现了颜色除以2的效果
        if (side == 1)
        {
            wall_sprite.setColor(Color(128, 128, 128));
        }
        // 绘制sprite
        window.draw(wall_sprite);
    }
}

// 移动函数，用于改变位置和朝向
// 已经实现鼠标左右转
void Move(RenderWindow &window)
{
    // timing for input and FPS counter
    old_Time = new_Time;
    new_Time = getTicks();
    double frameTime = (new_Time - old_Time) / 1000.0; // frameTime is the time this frame has taken, in seconds

    // speed modifiers
    double moveSpeed = frameTime * 3.0; // the constant value is in squares/second
    // 这个旋转速度计算的是方向键左右逻辑，类似于坦克视角移动
    double rotSpeed = frameTime * 2.0; // the constant value is in radians/second

    // 这些是计算鼠标移动逻辑
    // 获取窗口的中心位置
    Vector2i center(screenWidth * 0.5f, screenHeight * 0.5f);

    // 获取鼠标当前位置
    Vector2i position_now = Mouse::getPosition(window);

    // 计算鼠标的旋转角度
    // 水平方向旋转角度
    double rotation_degree_horizontal = (center.x - position_now.x) * FOV / screenWidth;

    if (ifMouse)
    {
        // 更新方向
        // dir和plane一起更新
        double oldDirX = dirX;
        dirX = dirX * cos(rotation_degree_horizontal) - dirY * sin(rotation_degree_horizontal);
        dirY = oldDirX * sin(rotation_degree_horizontal) + dirY * cos(rotation_degree_horizontal);
        double oldPlaneX = planeX;
        planeX = planeX * cos(rotation_degree_horizontal) - planeY * sin(rotation_degree_horizontal);
        planeY = oldPlaneX * sin(rotation_degree_horizontal) + planeY * cos(rotation_degree_horizontal);

        // 将鼠标位置重置到窗口中心
        Mouse::setPosition(center, window);
    }

    // move forward if no wall in front of you
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        if (worldMap[int(posX + dirX * moveSpeed)][int(posY)] == false)
            posX += dirX * moveSpeed;
        if (worldMap[int(posX)][int(posY + dirY * moveSpeed)] == false)
            posY += dirY * moveSpeed;
    }
    // move backwards if no wall behind you
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        if (worldMap[int(posX - dirX * moveSpeed)][int(posY)] == false)
            posX -= dirX * moveSpeed;
        if (worldMap[int(posX)][int(posY - dirY * moveSpeed)] == false)
            posY -= dirY * moveSpeed;
    }
    // 向右走，原理还是跟前后走一样，只是方向用矩阵相乘重新算了
    // 向右走就是在(dirY, -dirX)这个方向走
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        if (worldMap[int(posX + dirY * moveSpeed * 0.75)][int(posY)] == false)
            posX += dirY * moveSpeed * 0.75;
        if (worldMap[int(posX)][int(posY - dirX * moveSpeed * 0.75)] == false)
            posY -= dirX * moveSpeed * 0.75;
    }
    // 向左走同理
    // 在(-dirY, dirX)方向
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        if (worldMap[int(posX - dirY * moveSpeed * 0.75)][int(posY)] == false)
            posX -= dirY * moveSpeed * 0.75;
        if (worldMap[int(posX)][int(posY + dirX * moveSpeed * 0.75)] == false)
            posY += dirX * moveSpeed * 0.75;
    }
    // rotate to the right
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        // both camera direction and camera plane must be rotated
        double oldDirX = dirX;
        dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
        dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
        double oldPlaneX = planeX;
        planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
        planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
    }
    // rotate to the left
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        // both camera direction and camera plane must be rotated
        double oldDirX = dirX;
        dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
        dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
        double oldPlaneX = planeX;
        planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
        planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
    }
}

double calculate_FOV()
{
    double length_dir = sqrt(dirX * dirX + dirY * dirY);
    double length_plane = sqrt(planeX * planeX + planeY * planeY);
    // 弧度制
    return atan(length_plane / length_dir) * 2;
    // 角度制
    //// return atan(length_plane / length_dir) * 2 * 180 / acos(-1);
}

// sort algorithm
// sort the sprites based on distance
void sortSprites(int *order, double *dist, int amount)
{
    std::vector<std::pair<double, int>> sprites(amount);
    for (int i = 0; i < amount; i++)
    {
        sprites[i].first = dist[i];
        sprites[i].second = order[i];
    }
    // 针对pair默认就用第一个变量按升序排列
    std::sort(sprites.begin(), sprites.end());
    // restore in reverse order to go from farthest to nearest
    for (int i = 0; i < amount; i++)
    {
        dist[i] = sprites[amount - i - 1].first;
        order[i] = sprites[amount - i - 1].second;
    }
}

// 画出Decoration Sprite
void Decoration_Casting(RenderWindow &window)
{
    for (int i = 0; i < decorationNum; i++)
    {
        decoration_Order[i] = i;
        decoration_Distance[i] = ((posX - decoration_lst[i].x) * (posX - decoration_lst[i].x) + (posY - decoration_lst[i].y) * (posY - decoration_lst[i].y)); // sqrt not taken, unneeded
    }
    // 排序
    // 先画远的再画近的不会导致透明遮挡问题
    // 因为近的Sprite透明地方可以直接显示出远的Sprite
    sortSprites(decoration_Order, decoration_Distance, decorationNum);
    // after sorting the sprites, do the projection and draw them
    for (int i = 0; i < decorationNum; i++)
    {
        // translate sprite position to relative to camera
        double spriteX = decoration_lst[decoration_Order[i]].x - posX;
        double spriteY = decoration_lst[decoration_Order[i]].y - posY;

        // transform sprite with the inverse camera matrix
        //  [ planeX   dirX ] -1                                       [ dirY      -dirX ]
        //  [               ]       =  1/(planeX*dirY-dirX*planeY) *   [                 ]
        //  [ planeY   dirY ]                                          [ -planeY  planeX ]

        double invDet = 1.0 / (planeX * dirY - dirX * planeY); // required for correct matrix multiplication

        double transformX = invDet * (dirY * spriteX - dirX * spriteY);
        double transformY = invDet * (-planeY * spriteX + planeX * spriteY); // this is actually the depth inside the screen, that what Z is in 3D

        // sprite在屏幕上的x坐标
        int spriteScreenX = int((screenWidth / 2) * (1 + transformX / transformY));

        // calculate height of the sprite on screen
        int spriteHeight = abs(int(screenHeight / (transformY))); // using 'transformY' instead of the real distance prevents fisheye
        // calculate width of the sprite
        int spriteWidth = abs(int(screenHeight / (transformY)));

        // calculate lowest and highest pixel to fill in current stripe
        // 计算y上的起点，不需要计算终点，因为竖线直接指定了Sprite的高度
        int drawStartY = -spriteHeight / 2 + screenHeight / 2;
        // if (drawStartY < 0) drawStartY = 0;

        // 计算x上的起点和终点
        int drawStartX = -spriteWidth / 2 + spriteScreenX;
        if (drawStartX < 0)
            drawStartX = 0;
        int drawEndX = spriteWidth / 2 + spriteScreenX;
        if (drawEndX >= screenWidth)
            drawEndX = screenWidth - 1;

        // loop through every vertical stripe of the sprite on screen
        for (int stripe = drawStartX; stripe < drawEndX; stripe++)
        {
            int texX = int(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * texWidth / spriteWidth) / 256;
            // the conditions in the if are:
            // 1) it's in front of camera plane so you don't see things behind you
            // 2) it's on the screen (left)
            // 3) it's on the screen (right)
            // 4) ZBuffer, with perpendicular distance
            if (transformY > 0 && stripe > 0 && stripe < screenWidth && transformY < ZBuffer[stripe])
            {
                // decoration一条线的Sprite
                Sprite decoration_sprite;
                // 设置对应纹理
                decoration_sprite.setTexture(decoration_texture[decoration_lst[decoration_Order[i]].texture_index]);
                // 设置sprite的纹理矩形，定义了在纹理中的哪个部分应用到decoration_sprite上
                // 宽度为1个像素，相当于原来的画线
                decoration_sprite.setTextureRect(sf::IntRect(texX, 0, 1, texHeight));
                // 设置sprite的位置，坐标指定了左上角的位置
                decoration_sprite.setPosition(stripe, drawStartY);
                // 设置sprite的缩放因子
                decoration_sprite.setScale(1, spriteHeight / (float)texHeight);
                // 绘制sprite
                window.draw(decoration_sprite);
            }
        }
    }
}

// 画出illustration Sprite
// 如果illustration与decoration分开画，会导致透明遮挡问题
void Illustration_Casting(RenderWindow &window)
{
    for (int i = 0; i < illustrationNum; i++)
    {
        illustration_Order[i] = i;
        illustration_Distance[i] = ((posX - illustration_lst[i].x) * (posX - illustration_lst[i].x) + (posY - illustration_lst[i].y) * (posY - illustration_lst[i].y)); // sqrt not taken, unneeded
    }
    // 排序
    // 先画远的再画近的不会导致透明遮挡问题
    // 因为近的Sprite透明地方可以直接显示出远的Sprite
    sortSprites(illustration_Order, illustration_Distance, illustrationNum);
    // after sorting the sprites, do the projection and draw them
    for (int i = 0; i < illustrationNum; i++)
    {
        // translate sprite position to relative to camera
        double spriteX = illustration_lst[illustration_Order[i]].x - posX;
        double spriteY = illustration_lst[illustration_Order[i]].y - posY;

        // transform sprite with the inverse camera matrix
        //  [ planeX   dirX ] -1                                       [ dirY      -dirX ]
        //  [               ]       =  1/(planeX*dirY-dirX*planeY) *   [                 ]
        //  [ planeY   dirY ]                                          [ -planeY  planeX ]

        double invDet = 1.0 / (planeX * dirY - dirX * planeY); // required for correct matrix multiplication

        double transformX = invDet * (dirY * spriteX - dirX * spriteY);
        double transformY = invDet * (-planeY * spriteX + planeX * spriteY); // this is actually the depth inside the screen, that what Z is in 3D

        // sprite在屏幕上的x坐标
        int spriteScreenX = int((screenWidth / 2) * (1 + transformX / transformY));

        // calculate height of the sprite on screen
        // 注意illustration这里需要给高度乘一个illustrationHeight / illustrationWidth
        // 用于保持长宽比，不然不好看，北原春希都被压成正方形了
        int spriteHeight = abs(int(screenHeight / (transformY))) * illustrationHeight / illustrationWidth; // using 'transformY' instead of the real distance prevents fisheye
        // calculate width of the sprite
        int spriteWidth = abs(int(screenHeight / (transformY)));

        // calculate lowest and highest pixel to fill in current stripe
        // 计算y上的起点，不需要计算终点，因为竖线直接指定了Sprite的高度
        int drawStartY = -spriteHeight / 2 + screenHeight / 2;
        // if (drawStartY < 0) drawStartY = 0;

        // 计算x上的起点和终点
        int drawStartX = -spriteWidth / 2 + spriteScreenX;
        if (drawStartX < 0)
            drawStartX = 0;
        int drawEndX = spriteWidth / 2 + spriteScreenX;
        if (drawEndX >= screenWidth)
            drawEndX = screenWidth - 1;

        // loop through every vertical stripe of the sprite on screen
        for (int stripe = drawStartX; stripe < drawEndX; stripe++)
        {
            int texX = int(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * illustrationWidth / spriteWidth) / 256;
            // the conditions in the if are:
            // 1) it's in front of camera plane so you don't see things behind you
            // 2) it's on the screen (left)
            // 3) it's on the screen (right)
            // 4) ZBuffer, with perpendicular distance
            if (transformY > 0 && stripe > 0 && stripe < screenWidth && transformY < ZBuffer[stripe])
            {
                // illustration一条线的Sprite
                Sprite illustration_sprite;
                // 设置对应纹理
                illustration_sprite.setTexture(illustration_texture[illustration_lst[illustration_Order[i]].texture_index]);
                // 设置sprite的纹理矩形，定义了在纹理中的哪个部分应用到illustration_sprite上
                // 宽度为1个像素，相当于原来的画线
                illustration_sprite.setTextureRect(sf::IntRect(texX, 0, 1, illustrationHeight));
                // 设置sprite的位置，坐标指定了左上角的位置
                illustration_sprite.setPosition(stripe, drawStartY);
                // 设置sprite的缩放因子
                illustration_sprite.setScale(1, spriteHeight / (float)illustrationHeight);
                // 绘制sprite
                window.draw(illustration_sprite);
            }
        }
    }
}

void sortAllSprites(vector<int> &Allsprites_Order, vector<double> &Allsprites_Distance, int amount)
{
    std::vector<std::pair<double, int>> sprites(amount);
    for (int i = 0; i < amount; i++)
    {
        sprites[i].first = Allsprites_Distance[i];
        sprites[i].second = Allsprites_Order[i];
    }
    std::sort(sprites.begin(), sprites.end());
    // restore in reverse order to go from farthest to nearest
    for (int i = 0; i < amount; i++)
    {
        Allsprites_Distance[i] = sprites[amount - i - 1].first;
        Allsprites_Order[i] = sprites[amount - i - 1].second;
    }
}

// 成功解决遮挡问题，原因：sortAllSprites排序没传递vector的引用，导致没有正常排序
void Allsprites_Casting(RenderWindow &window)
{
    for (int i = 0; i < decorationNum + illustrationNum; i++)
    {
        Allsprites_Order[i] = i;
        Allsprites_Distance[i] = ((posX - Allsprites_lst[i].x) * (posX - Allsprites_lst[i].x) + (posY - Allsprites_lst[i].y) * (posY - Allsprites_lst[i].y)); // sqrt not taken, unneeded
    }
    // 排序
    // 先画远的再画近的不会导致透明遮挡问题
    // 因为近的Sprite透明地方可以直接显示出远的Sprite
    sortAllSprites(Allsprites_Order, Allsprites_Distance, decorationNum + illustrationNum);
    // after sorting the sprites, do the projection and draw them
    for (int i = 0; i < decorationNum + illustrationNum; i++)
    {
        // translate sprite position to relative to camera
        double spriteX = Allsprites_lst[Allsprites_Order[i]].x - posX;
        double spriteY = Allsprites_lst[Allsprites_Order[i]].y - posY;

        // transform sprite with the inverse camera matrix
        //  [ planeX   dirX ] -1                                       [ dirY      -dirX ]
        //  [               ]       =  1/(planeX*dirY-dirX*planeY) *   [                 ]
        //  [ planeY   dirY ]                                          [ -planeY  planeX ]

        double invDet = 1.0 / (planeX * dirY - dirX * planeY); // required for correct matrix multiplication

        double transformX = invDet * (dirY * spriteX - dirX * spriteY);
        double transformY = invDet * (-planeY * spriteX + planeX * spriteY); // this is actually the depth inside the screen, that what Z is in 3D

        // sprite在屏幕上的x坐标
        int spriteScreenX = int((screenWidth / 2) * (1 + transformX / transformY));

        // calculate height of the sprite on screen
        int spriteHeight = 0;
        // decoration的height不用乘比例
        if (Allsprites_lst[Allsprites_Order[i]].type == 0)
        {
            spriteHeight = abs(int(screenHeight / (transformY))); // using 'transformY' instead of the real distance prevents fisheye
        }
        // illustration的height需要乘比例
        // 用于保持长宽比，不然立绘被压成正方形
        else if (Allsprites_lst[Allsprites_Order[i]].type == 1)
        {
            spriteHeight = abs(int(screenHeight / (transformY))) * illustrationHeight / illustrationWidth; // using 'transformY' instead of the real distance prevents fisheye
        }

        // calculate width of the sprite
        int spriteWidth = abs(int(screenHeight / (transformY)));

        // calculate lowest and highest pixel to fill in current stripe
        // 计算y上的起点，不需要计算终点，因为竖线直接指定了Sprite的高度
        int drawStartY = -spriteHeight / 2 + screenHeight / 2;
        // if (drawStartY < 0) drawStartY = 0;

        // 计算x上的起点和终点
        int drawStartX = -spriteWidth / 2 + spriteScreenX;
        if (drawStartX < 0)
            drawStartX = 0;
        int drawEndX = spriteWidth / 2 + spriteScreenX;
        if (drawEndX >= screenWidth)
            drawEndX = screenWidth - 1;

        // loop through every vertical stripe of the sprite on screen
        for (int stripe = drawStartX; stripe < drawEndX; stripe++)
        {
            // the conditions in the if are:
            // 1) it's in front of camera plane so you don't see things behind you
            // 2) it's on the screen (left)
            // 3) it's on the screen (right)
            // 4) ZBuffer, with perpendicular distance
            if (transformY > 0 && stripe > 0 && stripe < screenWidth && transformY < ZBuffer[stripe])
            {
                // 画decoration
                if (Allsprites_lst[Allsprites_Order[i]].type == 0)
                {
                    // 计算texX
                    int texX = int(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * texWidth / spriteWidth) / 256;
                    // decoration一条线的Sprite
                    Sprite decoration_sprite;
                    // 设置对应纹理
                    decoration_sprite.setTexture(decoration_texture[Allsprites_lst[Allsprites_Order[i]].texture_index]);
                    // 设置sprite的纹理矩形，定义了在纹理中的哪个部分应用到decoration_sprite上
                    // 宽度为1个像素，相当于原来的画线
                    decoration_sprite.setTextureRect(sf::IntRect(texX, 0, 1, texHeight));
                    // 设置sprite的位置，坐标指定了左上角的位置
                    decoration_sprite.setPosition(stripe, drawStartY);
                    // 设置sprite的缩放因子
                    decoration_sprite.setScale(1, spriteHeight / (float)texHeight);
                    // 绘制sprite
                    window.draw(decoration_sprite);
                }
                // 画illustration
                else if (Allsprites_lst[Allsprites_Order[i]].type == 1)
                {
                    // 计算texX
                    int texX = int(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * illustrationWidth / spriteWidth) / 256;
                    // illustration一条线的Sprite
                    Sprite illustration_sprite;
                    // 设置对应纹理
                    illustration_sprite.setTexture(illustration_texture[Allsprites_lst[Allsprites_Order[i]].texture_index]);
                    // 设置sprite的纹理矩形，定义了在纹理中的哪个部分应用到illustration_sprite上
                    // 宽度为1个像素，相当于原来的画线
                    illustration_sprite.setTextureRect(sf::IntRect(texX, 0, 1, illustrationHeight));
                    // 设置sprite的位置，坐标指定了左上角的位置
                    illustration_sprite.setPosition(stripe, drawStartY);
                    // 设置sprite的缩放因子
                    illustration_sprite.setScale(1, spriteHeight / (float)illustrationHeight);
                    // 绘制sprite
                    window.draw(illustration_sprite);
                }
            }
        }
    }
}

// 要注意：SFML中的y轴是向下的，而用二维数组计算时y轴是向上的
// 需将游戏中的坐标系转换为小地图的坐标系，具体来说，就是将y坐标进行翻转
void drawMiniMap(RenderWindow &window)
{
    // 遍历地图的每一个格子
    for (int x = 0; x < minimapWidth; x++)
    {
        for (int y = 0; y < minimapHeight; y++)
        {
            // 创建一个表示地图格子的矩形
            RectangleShape rect(Vector2f(miniMapScale, miniMapScale));
            rect.setPosition(x * miniMapScale, (mapHeight - y - 1) * miniMapScale);

            // 根据地图数据设置矩形的颜色
            if (worldMap[x][y] > 0)
            {
                rect.setFillColor(Color::White);
            }
            else
            {
                rect.setFillColor(Color::Black);
            }

            // 在窗口上绘制矩形
            window.draw(rect);
        }
    }

    // 创建一个表示玩家位置的圆形
    int radius = 4;
    CircleShape player_circle(radius);
    player_circle.setFillColor(Color::Red);
    player_circle.setPosition(posX * miniMapScale - player_circle.getRadius(), (mapHeight - posY) * miniMapScale - player_circle.getRadius());
    // 在窗口上绘制圆形
    window.draw(player_circle);

    // 设置三角扇形的原点为玩家的位置
    // FOV顶点具体计算在DDA算法里，保存在FOV_position数组中
    FOV_Visualize[0].position = Vector2f(posX * miniMapScale, (mapHeight - posY) * miniMapScale);
    FOV_Visualize[0].color = Color::Transparent;
    for (int i = 1; i <= screenWidth; i++)
    {
        // 设置FOV的位置
        FOV_Visualize[i].position = FOV_position[i];
    }
    // 在窗口上绘制三角扇形
    window.draw(FOV_Visualize);
}