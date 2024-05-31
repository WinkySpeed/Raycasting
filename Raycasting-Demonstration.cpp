
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;
using namespace sf;

#define screenWidth 1440
#define screenHeight 900
#define mapWidth 24
#define mapHeight 24
#define texWidth 64
#define texHeight 64

int worldMap[mapWidth][mapWidth] =
{
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

//程序必要的参数
double posX = 22, posY = 12;  //x and y start position
double dirX = -1, dirY = 0; //initial direction vector
double planeX = 0, planeY = 0.66; //the 2d raycaster version of camera plane

double Mytime = 0; //time of current frame
double oldTime = 0; //time of previous frame

//创建一个表示视野范围的三角扇形
//还是程序设计的结构问题，应该给玩家安排个类，这个当做类的成员的
//而且中间参数还在DDA算法中计算，实际上不应该这样的
VertexArray fov(sf::TriangleFan, screenWidth + 1);
//墙面纹理数组
Texture texture[8];
//用于计时
Clock Myclock;

//小地图的宽度和高度
const int minimapWidth = 24;
const int minimapHeight = 24;
//小地图的大小（单位：像素）             加减号或者<>键（,和.）操作放大缩小
int miniMapScale = 36;

//是否画伪3D效果                        回车键
bool DrawFlag = 0;
//是否画小地图，默认画而且很大           Tab键或~键
bool DrawminimapFlag = 1;
//是否画小地图上的FOV                   shift
bool DrawFOV = 0;
//是否加纹理                           T
bool TextureFlag = 0;
//是否画天花板地板                      B
bool DrawFloorCeilingFlag = 0;

//获取时间
unsigned long getTicks();
//读取纹理
void LoadTexture();
//画天花板和地板
void DrawFloorCeiling(RenderWindow& window)
{
    RectangleShape CeilingRect(Vector2f(screenWidth, screenHeight / 2));
    CeilingRect.setFillColor(Color(156, 220, 235));

    RectangleShape FloorRect(Vector2f(screenWidth, screenHeight / 2));
    FloorRect.setPosition(0, screenHeight / 2);
    FloorRect.setFillColor(Color(68, 68, 68));

    window.draw(CeilingRect);
    window.draw(FloorRect);
}
//整的新活：用SFML的VertexArray来可视化FOV
void drawMiniMap(sf::RenderWindow& window);
//DDA算法，用于计算墙的高度和画线
void DDA_algorithm(RenderWindow& window);
//移动函数，用于改变位置和朝向
void Move(RenderWindow& window);
//画线
void verLine(sf::RenderWindow& window, int x, int y1, int y2, const sf::Color& color)
{
    // Ensure y1 is less than y2
    if (y2 < y1)
    {
        std::swap(y1, y2);
    }

    // Create a sf::VertexArray for the line
    sf::VertexArray line(sf::Lines, 2);

    // Set the position and color of the line vertices
    line[0].position = sf::Vector2f(x, y1);
    line[0].color = color;
    line[1].position = sf::Vector2f(x, y2);
    line[1].color = color;

    // Draw the line
    window.draw(line);
}

int main()
{
    RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Raycasting-Demonstration");


    LoadTexture();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            
            //sf::Keyboard::isKeyPressed函数会在每一帧都检测键盘的状态
            //sf::Event::KeyPressed事件只会在按键被按下的那一帧触发一次，即使你按住按键不放，它也不会在下一帧再次触发
            //因此，使用sf::Event::KeyPressed事件可以确保DrawminimapFlag的值只会被反转一次
            if (event.type == sf::Event::KeyPressed)
            {
                //按Tab键或~键开关小地图
                if (event.key.code == sf::Keyboard::Tab || event.key.code == sf::Keyboard::Tilde)
                {
                    DrawminimapFlag = !DrawminimapFlag;
                }
                //按加号键或>键增大小地图
                if (event.key.code == sf::Keyboard::Add || event.key.code == sf::Keyboard::Period)
                {
                    miniMapScale++;
                }
                //按减号键或<键缩小小地图
                if (event.key.code == sf::Keyboard::Subtract || event.key.code == sf::Keyboard::Comma)
                {
                    miniMapScale--;
                }
                //按shift键画FOV
                if (event.key.code == sf::Keyboard::LShift)
                {
                    DrawFOV = !DrawFOV;
                }
                //按T是否纹理
                if (event.key.code == sf::Keyboard::T)
                {
                    TextureFlag = !TextureFlag;
                }
                //按B是否画天花板
                if (event.key.code == sf::Keyboard::B)
                {
                    DrawFloorCeilingFlag = !DrawFloorCeilingFlag;
                }
                //按回车是否伪3D效果
                if (event.key.code == sf::Keyboard::Enter)
                {
                    DrawFlag = 1;
                }
            }

        }

        window.clear();
        if (DrawFloorCeilingFlag)
        {
            DrawFloorCeiling(window);
        }

        DDA_algorithm(window);
        Move(window);

        if (DrawminimapFlag)
        {
            drawMiniMap(window);
        }

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
    //texture[0].loadFromFile("pics/eagle.png");
    //texture[1].loadFromFile("pics/redbrick.png");
    //texture[2].loadFromFile("pics/purplestone.png");
    //texture[3].loadFromFile("pics/greystone.png");
    //texture[4].loadFromFile("pics/bluestone.png");
    //texture[5].loadFromFile("pics/mossy.png");
    //texture[6].loadFromFile("pics/wood.png");
    //texture[7].loadFromFile("pics/colorstone.png");
    texture[0].loadFromFile("pics/redbrick.png");
    texture[1].loadFromFile("pics/greystone.png");
    texture[2].loadFromFile("pics/bluestone.png");
    texture[3].loadFromFile("pics/colorstone.png");
    texture[4].loadFromFile("pics/purplestone.png");
}

//要注意：SFML中的y轴是向下的，而用二维数组计算时y轴是向上的
//需将游戏中的坐标系转换为小地图的坐标系，具体来说，就是将y坐标进行翻转
void drawMiniMap(sf::RenderWindow& window)
{
    // 遍历地图的每一个格子
    for (int x = 0; x < minimapWidth; ++x)
    {
        for (int y = 0; y < minimapHeight; ++y)
        {
            // 创建一个表示地图格子的矩形
            sf::RectangleShape rect(sf::Vector2f(miniMapScale, miniMapScale));
            //rect.setPosition(x * miniMapScale, y * miniMapScale);
            rect.setPosition(x * miniMapScale, (mapHeight - y - 1) * miniMapScale);

            // 根据地图数据设置矩形的颜色
            if (worldMap[x][y] > 0) {
                rect.setFillColor(sf::Color::White);
            }
            else {
                rect.setFillColor(sf::Color::Black);
            }

            // 在窗口上绘制矩形
            window.draw(rect);
        }
    }

    // 创建一个表示玩家位置的圆形
    int radius = 4;
    sf::CircleShape player(radius);
    player.setFillColor(sf::Color::Red);
    //player.setPosition(posX * miniMapScale - player.getRadius(), posY * miniMapScale - player.getRadius());
    player.setPosition(posX * miniMapScale - player.getRadius(), (mapHeight - posY) * miniMapScale - player.getRadius());

    // 在窗口上绘制圆形
    window.draw(player);

    // 设置三角扇形的原点为玩家的位置
    //FOV具体计算的参数在DDA算法里，我觉得这个结构很垃圾，应该在全局变量加个数组，在DDA算法中访问并修改这个参数
    //但是好像没啥时间给我改代码了，能跑就是胜利
    fov[0].position = sf::Vector2f(posX * miniMapScale, (mapHeight - posY) * miniMapScale);
    fov[0].color = sf::Color::Transparent;

    // 在窗口上绘制三角扇形
    if (DrawFOV == 1)
    {
        window.draw(fov);
    }
    
}

void DDA_algorithm(RenderWindow& window)
{
    for (int x = 0; x < screenWidth; x++)
    {
        //calculate ray position and direction
        double cameraX = 2 * x / (double)screenWidth - 1; //x-coordinate in camera space
        double rayDirX = dirX + planeX * cameraX;
        double rayDirY = dirY + planeY * cameraX;
        //which box of the map we're in
        int mapX = int(posX);
        int mapY = int(posY);

        //length of ray from current position to next x or y-side
        double sideDistX;
        double sideDistY;

        double deltaDistX = (rayDirX == 0) ? 1e30 : std::abs(1 / rayDirX);
        double deltaDistY = (rayDirY == 0) ? 1e30 : std::abs(1 / rayDirY);

        double perpWallDist;

        //what direction to step in x or y-direction (either +1 or -1)
        int stepX;
        int stepY;

        int hit = 0; //was there a wall hit?
        int side; //was a NS or a EW wall hit?
        //calculate step and initial sideDist
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
        //perform DDA
        while (hit == 0)
        {
            //jump to next map square, either in x-direction, or in y-direction
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
            //Check if ray has hit a wall
            if (worldMap[mapX][mapY] > 0) hit = 1;
        }
        //教程里这部分解释的比较清楚了
        //Calculate distance projected on camera direction. This is the shortest distance from the point where the wall is
        //hit to the camera plane. Euclidean to center camera point would give fisheye effect!
        //This can be computed as (mapX - posX + (1 - stepX) / 2) / rayDirX for side == 0, or same formula with Y
        //for size == 1, but can be simplified to the code below thanks to how sideDist and deltaDist are computed:
        //because they were left scaled to |rayDir|. sideDist is the entire length of the ray above after the multiple
        //steps, but we subtract deltaDist once because one step more into the wall was taken above.
        if (side == 0) perpWallDist = (sideDistX - deltaDistX);
        else          perpWallDist = (sideDistY - deltaDistY);

        /*********************************************************************************/
        /*  此部分为添加FOV用
        /*  计算射线的终点
        /*********************************************************************************/
        double rayEndX = posX + perpWallDist * rayDirX;
        double rayEndY = posY + perpWallDist * rayDirY;

        // 设置三角扇形的顶点
        fov[x + 1].position = sf::Vector2f(rayEndX * miniMapScale, (mapHeight - rayEndY) * miniMapScale);
        fov[x + 1].color = sf::Color(255, 255, 255, 150);

        //Calculate height of line to draw on screen
        //可以任意修改墙的高度
        int lineHeight = (int)(screenHeight / perpWallDist) * 3;

        //calculate lowest and highest pixel to fill in current stripe
        int drawStart = -lineHeight / 2 + screenHeight / 2;
        if (drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + screenHeight / 2;
        if (drawEnd >= screenHeight) drawEnd = screenHeight - 1;

        /*无纹理时*/
        if (TextureFlag == 0)
        {
            //choose wall color
            Color color;
            switch (worldMap[mapX][mapY])
            {
            case 1:  color = Color::Red;    break; //red
            case 2:  color = Color::Green;  break; //green
            case 3:  color = Color::Blue;   break; //blue
            case 4:  color = Color::White;  break; //white
            default: color = Color::Yellow; break; //yellow
            }

            //give x and y sides different brightness
            if (side == 1)
            {
                color.r /= 2;
                color.g /= 2;
                color.b /= 2;
            }

            //draw the pixels of the stripe as a vertical line
            if (DrawFlag == 1)
            {
                verLine(window, x, drawStart, drawEnd, color);
            }
            
        }

        /*********************************************************************************/
        /*  此部分替换了原raycasting画线部分
        /*  以下为纹理相关计算，是新内容
        /*  具体和原来的画线函数差不多，都是在具体位置上画出一条线
        /*  只不过这里画的是具体纹理上的一条线，利用了SFML中的sprite
        /*********************************************************************************/

        if (TextureFlag == 1)
        {
            //texturing calculations
            int texNum = worldMap[mapX][mapY] - 1; //1 subtracted from it so that texture 0 can be used!

            //calculate value of wallX
            double wallX; //where exactly the wall was hit
            if (side == 0) wallX = posY + perpWallDist * rayDirY;
            else           wallX = posX + perpWallDist * rayDirX;
            wallX -= floor((wallX));

            //x coordinate on the texture
            int texX = int(wallX * double(texWidth));
            if (side == 0 && rayDirX > 0) texX = texWidth - texX - 1;
            if (side == 1 && rayDirY < 0) texX = texWidth - texX - 1;

            // How much to increase the texture coordinate per screen pixel
            double step = 1.0 * texHeight / lineHeight;
            // Starting texture coordinate
            double texPos = (drawStart - screenHeight / 2 + lineHeight / 2) * step;

            //为了获取纹理具体某一个坐标的像素颜色
            //创建一个新的sprite对象
            sf::Sprite wall_sprite;

            //设置sprite的纹理
            wall_sprite.setTexture(texture[texNum]);

            //设置sprite的纹理矩形，定义了在纹理中的哪个部分应用到wall_sprite上
            //宽度为1个像素，相当于原来的画线
            wall_sprite.setTextureRect(sf::IntRect(texX, 0, 1, texHeight));
            //wall_sprite.setTextureRect(sf::IntRect(texX, 0, 1, lineHeight));
            //wall_sprite.setTextureRect(sf::IntRect(static_cast<unsigned short>(round(texX)), 0, 1, lineHeight));

            //设置sprite的位置，坐标指定了左上角的位置
            //wall_sprite.setPosition(x, drawStart);        //太几把恶心了，破案了：指定左上角的位置为drawStart
            //                                              //drawStart在特殊情况下会被赋值为0，即if (drawStart < 0) drawStart = 0;
            //                                              //在此情况下绘制的起点就会改变为0，令人恶心
            wall_sprite.setPosition(x, round(0.5f * screenHeight - 0.5f * lineHeight));

            //设置sprite的缩放因子
            wall_sprite.setScale(1, lineHeight / (float)texHeight);


            //利用SFML中sprite的setColor功能，使y墙变暗
            //这个函数实现的是“调制混合”
            //举个例子：原来的R通道为200，与128混合为：200 * 128 / 255 = 100，实现了颜色除以2的效果
            if (side == 1)
            {
                wall_sprite.setColor(Color(128, 128, 128));
            }
            // 绘制sprite
            if (DrawFlag == 1)
            {
                window.draw(wall_sprite);
            }
        }
    }
}

void Move(RenderWindow& window)
{
    //timing for input and FPS counter
    oldTime = Mytime;
    Mytime = getTicks();
    double frameTime = (Mytime - oldTime) / 1000.0; //frameTime is the time this frame has taken, in seconds


    //speed modifiers
    double moveSpeed = frameTime * 3.0; //the constant value is in squares/second
    double rotSpeed = frameTime * 2.0; //the constant value is in radians/second
    //move forward if no wall in front of you
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        if (worldMap[int(posX + dirX * moveSpeed)][int(posY)] == false) posX += dirX * moveSpeed;
        if (worldMap[int(posX)][int(posY + dirY * moveSpeed)] == false) posY += dirY * moveSpeed;
    }
    //move backwards if no wall behind you
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        if (worldMap[int(posX - dirX * moveSpeed)][int(posY)] == false) posX -= dirX * moveSpeed;
        if (worldMap[int(posX)][int(posY - dirY * moveSpeed)] == false) posY -= dirY * moveSpeed;
    }
    //向右走，原理还是跟前后走一样，只是方向用矩阵相乘重新算了
    //向右走就是在(dirY, -dirX)这个方向走
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        if (worldMap[int(posX + dirY * moveSpeed * 0.75)][int(posY)] == false) posX += dirY * moveSpeed * 0.75;
        if (worldMap[int(posX)][int(posY - dirX * moveSpeed * 0.75)] == false) posY -= dirX * moveSpeed * 0.75;
    }
    //向左走同理
    //在(-dirY, dirX)方向
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        if (worldMap[int(posX - dirY * moveSpeed * 0.75)][int(posY)] == false) posX -= dirY * moveSpeed * 0.75;
        if (worldMap[int(posX)][int(posY + dirX * moveSpeed * 0.75)] == false) posY += dirX * moveSpeed * 0.75;
    }
    //rotate to the right
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        //both camera direction and camera plane must be rotated
        double oldDirX = dirX;
        dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
        dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
        double oldPlaneX = planeX;
        planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
        planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
    }
    //rotate to the left
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        //both camera direction and camera plane must be rotated
        double oldDirX = dirX;
        dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
        dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
        double oldPlaneX = planeX;
        planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
        planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
    }
}

