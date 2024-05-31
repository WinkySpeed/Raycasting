
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

int worldMap[mapWidth][mapHeight] =
{
  {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,7,7,7,7,7,7,7,7},
  {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
  {4,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
  {4,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
  {4,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
  {4,0,4,0,0,0,0,5,5,5,5,5,5,5,5,5,7,7,0,7,7,7,7,7},
  {4,0,5,0,0,0,0,5,0,5,0,5,0,5,0,5,7,0,0,0,7,7,7,1},
  {4,0,6,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
  {4,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,7,1},
  {4,0,8,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
  {4,0,0,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,7,7,7,1},
  {4,0,0,0,0,0,0,5,5,5,5,0,5,5,5,5,7,7,7,7,7,7,7,1},
  {6,6,6,6,6,6,6,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
  {8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
  {6,6,6,6,6,6,0,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
  {4,4,4,4,4,4,0,4,4,4,6,0,6,2,2,2,2,2,2,2,3,3,3,3},
  {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
  {4,0,0,0,0,0,0,0,0,0,0,0,6,2,0,0,5,0,0,2,0,0,0,2},
  {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
  {4,0,6,0,6,0,0,0,0,4,6,0,0,0,0,0,5,0,0,0,0,0,0,2},
  {4,0,0,5,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
  {4,0,6,0,6,0,0,0,0,4,6,0,6,2,0,0,5,0,0,2,0,0,0,2},
  {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
  {4,4,4,4,4,4,4,4,4,4,1,1,1,2,2,2,2,2,2,3,3,3,3,3}
};

double posX = 22, posY = 11.5;  //x and y start position
double dirX = -1, dirY = 0; //initial direction vector
double planeX = 0, planeY = 0.66; //the 2d raycaster version of camera plane

double Mytime = 0; //time of current frame
double oldTime = 0; //time of previous frame

Texture texture[8];
Clock Myclock;          //���ڼ�ʱ

//��ȡʱ��
unsigned long getTicks();
//��ȡ����
void LoadTexture();
//DDA�㷨�����ڼ���ǽ�ĸ߶Ⱥͻ���
void DDA_algorithm(RenderWindow& window);
//�ƶ����������ڸı�λ�úͳ���
void Move(RenderWindow& window);

int main()
{
    RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Raycasting-texture");

    LoadTexture();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        DDA_algorithm(window);
        Move(window);
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
    texture[0].loadFromFile("pics/eagle.png");
    texture[1].loadFromFile("pics/redbrick.png");
    texture[2].loadFromFile("pics/purplestone.png");
    texture[3].loadFromFile("pics/greystone.png");
    texture[4].loadFromFile("pics/bluestone.png");
    texture[5].loadFromFile("pics/mossy.png");
    texture[6].loadFromFile("pics/wood.png");
    texture[7].loadFromFile("pics/colorstone.png");
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
        //�̳����ⲿ�ֽ��͵ıȽ������
        //Calculate distance projected on camera direction. This is the shortest distance from the point where the wall is
        //hit to the camera plane. Euclidean to center camera point would give fisheye effect!
        //This can be computed as (mapX - posX + (1 - stepX) / 2) / rayDirX for side == 0, or same formula with Y
        //for size == 1, but can be simplified to the code below thanks to how sideDist and deltaDist are computed:
        //because they were left scaled to |rayDir|. sideDist is the entire length of the ray above after the multiple
        //steps, but we subtract deltaDist once because one step more into the wall was taken above.
        if (side == 0) perpWallDist = (sideDistX - deltaDistX);
        else          perpWallDist = (sideDistY - deltaDistY);

        //Calculate height of line to draw on screen
        //���������޸�ǽ�ĸ߶�
        int lineHeight = (int)(screenHeight / perpWallDist);

        //calculate lowest and highest pixel to fill in current stripe
        int drawStart = -lineHeight / 2 + screenHeight / 2;
        if (drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + screenHeight / 2;
        if (drawEnd >= screenHeight) drawEnd = screenHeight - 1;


        /*********************************************************************************/
        /*  �˲����滻��ԭraycasting���߲���
        /*  ����Ϊ������ؼ��㣬��������
        /*  �����ԭ���Ļ��ߺ�����࣬�����ھ���λ���ϻ���һ����
        /*  ֻ�������ﻭ���Ǿ��������ϵ�һ���ߣ�������SFML�е�sprite
        /*********************************************************************************/


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

        //Ϊ�˻�ȡ�������ĳһ�������������ɫ
        //����һ���µ�sprite����
        sf::Sprite wall_sprite;

        //����sprite������
        wall_sprite.setTexture(texture[texNum]);

        //����sprite��������Σ��������������е��ĸ�����Ӧ�õ�wall_sprite��
        //���Ϊ1�����أ��൱��ԭ���Ļ���
        wall_sprite.setTextureRect(sf::IntRect(texX, 0, 1, texHeight));
        //wall_sprite.setTextureRect(sf::IntRect(texX, 0, 1, lineHeight));
        //wall_sprite.setTextureRect(sf::IntRect(static_cast<unsigned short>(round(texX)), 0, 1, lineHeight));

        //����sprite��λ�ã�����ָ�������Ͻǵ�λ��
        //wall_sprite.setPosition(x, drawStart);        //̫���Ѷ����ˣ��ư��ˣ�ָ�����Ͻǵ�λ��ΪdrawStart
        //                                              //drawStart����������»ᱻ��ֵΪ0����if (drawStart < 0) drawStart = 0;
        //                                              //�ڴ�����»��Ƶ����ͻ�ı�Ϊ0�����˶���
        wall_sprite.setPosition(x, round(0.5f * screenHeight - 0.5f * lineHeight));

        //����sprite����������
        wall_sprite.setScale(1, lineHeight / (float)texHeight);


        //����SFML��sprite��setColor���ܣ�ʹyǽ�䰵
        //�������ʵ�ֵ��ǡ����ƻ�ϡ�
        //�ٸ����ӣ�ԭ����Rͨ��Ϊ200����128���Ϊ��200 * 128 / 255 = 100��ʵ������ɫ����2��Ч��
        if (side == 1)
        {
            wall_sprite.setColor(Color(128, 128, 128));
        }
        // ����sprite
        window.draw(wall_sprite);
        
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
    //�����ߣ�ԭ���Ǹ�ǰ����һ����ֻ�Ƿ����þ��������������
    //�����߾�����(dirY, -dirX)���������
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        if (worldMap[int(posX + dirY * moveSpeed * 0.75)][int(posY)] == false) posX += dirY * moveSpeed * 0.75;
        if (worldMap[int(posX)][int(posY - dirX * moveSpeed * 0.75)] == false) posY -= dirX * moveSpeed * 0.75;
    }
    //������ͬ��
    //��(-dirY, dirX)����
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
