
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

//�����Ҫ�Ĳ���
double posX = 22, posY = 12;  //x and y start position
double dirX = -1, dirY = 0; //initial direction vector
double planeX = 0, planeY = 0.66; //the 2d raycaster version of camera plane

double Mytime = 0; //time of current frame
double oldTime = 0; //time of previous frame

//����һ����ʾ��Ұ��Χ����������
//���ǳ�����ƵĽṹ���⣬Ӧ�ø���Ұ��Ÿ��࣬���������ĳ�Ա��
//�����м��������DDA�㷨�м��㣬ʵ���ϲ�Ӧ��������
VertexArray fov(sf::TriangleFan, screenWidth + 1);
//ǽ����������
Texture texture[8];
//���ڼ�ʱ
Clock Myclock;

//С��ͼ�Ŀ�Ⱥ͸߶�
const int minimapWidth = 24;
const int minimapHeight = 24;
//С��ͼ�Ĵ�С����λ�����أ�             �Ӽ��Ż���<>����,��.�������Ŵ���С
int miniMapScale = 36;

//�Ƿ�α3DЧ��                        �س���
bool DrawFlag = 0;
//�Ƿ�С��ͼ��Ĭ�ϻ����Һܴ�           Tab����~��
bool DrawminimapFlag = 1;
//�Ƿ�С��ͼ�ϵ�FOV                   shift
bool DrawFOV = 0;
//�Ƿ������                           T
bool TextureFlag = 0;
//�Ƿ��컨��ذ�                      B
bool DrawFloorCeilingFlag = 0;

//��ȡʱ��
unsigned long getTicks();
//��ȡ����
void LoadTexture();
//���컨��͵ذ�
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
//�����»��SFML��VertexArray�����ӻ�FOV
void drawMiniMap(sf::RenderWindow& window);
//DDA�㷨�����ڼ���ǽ�ĸ߶Ⱥͻ���
void DDA_algorithm(RenderWindow& window);
//�ƶ����������ڸı�λ�úͳ���
void Move(RenderWindow& window);
//����
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
            
            //sf::Keyboard::isKeyPressed��������ÿһ֡�������̵�״̬
            //sf::Event::KeyPressed�¼�ֻ���ڰ��������µ���һ֡����һ�Σ���ʹ�㰴ס�������ţ���Ҳ��������һ֡�ٴδ���
            //��ˣ�ʹ��sf::Event::KeyPressed�¼�����ȷ��DrawminimapFlag��ֵֻ�ᱻ��תһ��
            if (event.type == sf::Event::KeyPressed)
            {
                //��Tab����~������С��ͼ
                if (event.key.code == sf::Keyboard::Tab || event.key.code == sf::Keyboard::Tilde)
                {
                    DrawminimapFlag = !DrawminimapFlag;
                }
                //���Ӻż���>������С��ͼ
                if (event.key.code == sf::Keyboard::Add || event.key.code == sf::Keyboard::Period)
                {
                    miniMapScale++;
                }
                //�����ż���<����СС��ͼ
                if (event.key.code == sf::Keyboard::Subtract || event.key.code == sf::Keyboard::Comma)
                {
                    miniMapScale--;
                }
                //��shift����FOV
                if (event.key.code == sf::Keyboard::LShift)
                {
                    DrawFOV = !DrawFOV;
                }
                //��T�Ƿ�����
                if (event.key.code == sf::Keyboard::T)
                {
                    TextureFlag = !TextureFlag;
                }
                //��B�Ƿ��컨��
                if (event.key.code == sf::Keyboard::B)
                {
                    DrawFloorCeilingFlag = !DrawFloorCeilingFlag;
                }
                //���س��Ƿ�α3DЧ��
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

//Ҫע�⣺SFML�е�y�������µģ����ö�ά�������ʱy�������ϵ�
//�轫��Ϸ�е�����ϵת��ΪС��ͼ������ϵ��������˵�����ǽ�y������з�ת
void drawMiniMap(sf::RenderWindow& window)
{
    // ������ͼ��ÿһ������
    for (int x = 0; x < minimapWidth; ++x)
    {
        for (int y = 0; y < minimapHeight; ++y)
        {
            // ����һ����ʾ��ͼ���ӵľ���
            sf::RectangleShape rect(sf::Vector2f(miniMapScale, miniMapScale));
            //rect.setPosition(x * miniMapScale, y * miniMapScale);
            rect.setPosition(x * miniMapScale, (mapHeight - y - 1) * miniMapScale);

            // ���ݵ�ͼ�������þ��ε���ɫ
            if (worldMap[x][y] > 0) {
                rect.setFillColor(sf::Color::White);
            }
            else {
                rect.setFillColor(sf::Color::Black);
            }

            // �ڴ����ϻ��ƾ���
            window.draw(rect);
        }
    }

    // ����һ����ʾ���λ�õ�Բ��
    int radius = 4;
    sf::CircleShape player(radius);
    player.setFillColor(sf::Color::Red);
    //player.setPosition(posX * miniMapScale - player.getRadius(), posY * miniMapScale - player.getRadius());
    player.setPosition(posX * miniMapScale - player.getRadius(), (mapHeight - posY) * miniMapScale - player.getRadius());

    // �ڴ����ϻ���Բ��
    window.draw(player);

    // �����������ε�ԭ��Ϊ��ҵ�λ��
    //FOV�������Ĳ�����DDA�㷨��Ҿ�������ṹ��������Ӧ����ȫ�ֱ����Ӹ����飬��DDA�㷨�з��ʲ��޸��������
    //���Ǻ���ûɶʱ����ҸĴ����ˣ����ܾ���ʤ��
    fov[0].position = sf::Vector2f(posX * miniMapScale, (mapHeight - posY) * miniMapScale);
    fov[0].color = sf::Color::Transparent;

    // �ڴ����ϻ�����������
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
        //�̳����ⲿ�ֽ��͵ıȽ������
        //Calculate distance projected on camera direction. This is the shortest distance from the point where the wall is
        //hit to the camera plane. Euclidean to center camera point would give fisheye effect!
        //This can be computed as (mapX - posX + (1 - stepX) / 2) / rayDirX for side == 0, or same formula with Y
        //for size == 1, but can be simplified to the code below thanks to how sideDist and deltaDist are computed:
        //because they were left scaled to |rayDir|. sideDist is the entire length of the ray above after the multiple
        //steps, but we subtract deltaDist once because one step more into the wall was taken above.
        if (side == 0) perpWallDist = (sideDistX - deltaDistX);
        else          perpWallDist = (sideDistY - deltaDistY);

        /*********************************************************************************/
        /*  �˲���Ϊ���FOV��
        /*  �������ߵ��յ�
        /*********************************************************************************/
        double rayEndX = posX + perpWallDist * rayDirX;
        double rayEndY = posY + perpWallDist * rayDirY;

        // �����������εĶ���
        fov[x + 1].position = sf::Vector2f(rayEndX * miniMapScale, (mapHeight - rayEndY) * miniMapScale);
        fov[x + 1].color = sf::Color(255, 255, 255, 150);

        //Calculate height of line to draw on screen
        //���������޸�ǽ�ĸ߶�
        int lineHeight = (int)(screenHeight / perpWallDist) * 3;

        //calculate lowest and highest pixel to fill in current stripe
        int drawStart = -lineHeight / 2 + screenHeight / 2;
        if (drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + screenHeight / 2;
        if (drawEnd >= screenHeight) drawEnd = screenHeight - 1;

        /*������ʱ*/
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
        /*  �˲����滻��ԭraycasting���߲���
        /*  ����Ϊ������ؼ��㣬��������
        /*  �����ԭ���Ļ��ߺ�����࣬�����ھ���λ���ϻ���һ����
        /*  ֻ�������ﻭ���Ǿ��������ϵ�һ���ߣ�������SFML�е�sprite
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

