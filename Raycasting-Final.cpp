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
// �ذ��컨�塢ǽ�ڡ�decoration������
// ����ͼƬ�����ԡ��¾��ܲ�3D������˿��һ��
#define texWidth 64
#define texHeight 64
// �ذ��컨��ǽ����������
#define textureNum_Wall 8
// decoration��������
#define textureNum_Decoration 3
// illustration��������
#define textureNum_Illustration 12
// decoration����
#define decorationNum 19
// illustration����
#define illustrationNum 12
// ����������
// ����������������ߴ粻һ���������Ҫ������ͼƬ���ŵ�370x690
// �����Ѿ�ͨ��pythonʵ�֣��˴����벻����
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
double FOV;                       // FOV�ӽǴ�С�����û�����

double new_Time = 0; // time of current frame
double old_Time = 0; // time of previous frame

Texture texture[textureNum_Wall]; // ǽ�桢�ذ��컨����������
                                  // decoration��illustration�������鵥����
Clock Myclock;                    // ���ڼ�ʱ��������һ֡����һ֡��ʱ��

Image Ceiling_image; // �컨������ͼ������(tx, ty)��ȡ��ɫ
Image Floor_image;   // �ذ�����ͼ������(tx, ty)��ȡ��ɫ

// debug�ã�ÿ���ֶ�ע�͵������߼���������
bool ifMouse = 1;

// ZBuffer���飬��¼��Ļ��ÿ��x��Ӧ��perpWallDist
double ZBuffer[screenWidth] = {0};

/************************************************************/
/* Decoration
/************************************************************/
// decoration��������
Texture decoration_texture[textureNum_Decoration];
class Decoration
{
public:
    double x;
    double y;
    int texture_index;
    int type; // ����ָ����decoration����illustration
              // 0Ϊdecoration��1Ϊillustration
    Decoration(double x = 0, double y = 0, int texindex = 0)
    {
        this->x = x;
        this->y = y;
        this->texture_index = texindex;
        this->type = 0;
    }
};
// decoration���飬���ڼ�¼ȫ��decoration��λ�úͶ�Ӧ�����±�
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
// Order�����Distance���飬�ֱ�洢�±������ҵľ���
int decoration_Order[decorationNum];
double decoration_Distance[decorationNum];

/************************************************************/
/* illustration
/************************************************************/
// illustration��������
Texture illustration_texture[textureNum_Illustration];
class Illustration
{
public:
    double x;
    double y;
    int texture_index;
    int type; // ����ָ����decoration����illustration
              // 0Ϊdecoration��1Ϊillustration
    Illustration(double x = 0, double y = 0, int texindex = 0)
    {
        this->x = x;
        this->y = y;
        this->texture_index = texindex;
        this->type = 1;
    }
};
// illustration���飬���ڼ�¼ȫ��illustration��λ�úͶ�Ӧ�����±�
Illustration illustration_lst[illustrationNum] =
    {
        // ��ԭ��ϣ���棬�ս���Ϸ���ܿ���
        {20.6, 11.5, 0},
        // Сľ��ѩ�������ƶ�����
        {18, 3.25, 1},
        // Сľ��ѩ��ŭ����
        {8.0, 4.5, 2},
        // Сľ��ѩ�˿�1
        {21, 8, 3},
        // Сľ��ѩ�˿�2
        {22.5, 3.5, 4},
        // �����ɴ������
        {17.1, 14.7, 5},
        // �����ɴŭ
        {22.5, 18.5, 6},
        // �����ɴ����
        {16.8, 21.5, 7},
        // �����ɴЦ
        {3, 12, 8},
        // ɼ��С��
        {10.1, 18.3, 9},
        // ��Ȫǧ��
        {2.5, 21.2, 10},
        // �������
        {6.4, 19.2, 11}};
// arrays used to sort the sprites
// Order�����Distance���飬�ֱ�洢�±������ҵľ���
int illustration_Order[illustrationNum];
double illustration_Distance[illustrationNum];

// function used to sort the sprites
// �˺������ڵ�����decoration�򵥶���illustration
// ���һ�𻭣�����sortAllSprites����
void sortSprites(int *order, double *dist, int amount);

/************************************************************/
/* Ϊ�˷�ֹdecoration��illustration�����͸���ڵ�����
/* ��������Spriteһ�𻭵ķ��������������ֵ��������߼�Ҳ����ȫһ��
/* Ϊ�ˣ���Ҫ����MySprite�࣬��֮ǰ����Sprite��Ա����һ��
/* ����Ҫ�����µ��������飬����ֱ�Ӹ���decoration��illustration����������
/* ����Order��Distance���飬���ҳ�ʼ����ĿΪSprite����
/* ��֪Sprite����Ϊ decorationNum + illustrationNum
/* ��Ҫһ���ܴ���ȫ��Sprite������Allsprites_lst����¼λ�ú��±�
/* ����Ҫ��Ӧ��������sortAllSprites
/************************************************************/
/************************************************************/
/* MySprite�����ڴ�������
/************************************************************/
class MySprite
{
public:
    double x;
    double y;
    int texture_index;
    int type; // ����ָ����decoration����illustration
              // 0Ϊdecoration��1Ϊillustration
    class MySprite(double x = 0, double y = 0, int texindex = 0, int type = -1)
    {
        this->x = x;
        this->y = y;
        this->texture_index = texindex;
        this->type = type;
    }
};
// ȫ��Sprite�����飬��¼λ�ú��±�
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
        // ��ԭ��ϣ���棬�ս���Ϸ���ܿ���
        {20.6, 11.5, 0, 1},
        // Сľ��ѩ�������ƶ�����
        {18, 3.25, 1, 1},
        // Сľ��ѩ��ŭ����
        {8.0, 4.5, 2, 1},
        // Сľ��ѩ�˿�1
        {21, 8, 3, 1},
        // Сľ��ѩ�˿�2
        {22.5, 3.5, 4, 1},
        // �����ɴ������
        {17.1, 14.7, 5, 1},
        // �����ɴŭ
        {22.5, 18.5, 6, 1},
        // �����ɴ����
        {16.8, 21.5, 7, 1},
        // �����ɴЦ
        {3, 12, 8, 1},
        // С��
        {10.1, 18.3, 9, 1},
        // ǧ��
        {2.5, 21.2, 10, 1},
        // ����
        {6.4, 19.2, 11, 1}

};
// Order���飬���ҳ�ʼ����ĿΪdecorationNum + illustrationNum
vector<int> Allsprites_Order(decorationNum + illustrationNum);
// Distance���飬���ҳ�ʼ����ĿΪdecorationNum + illustrationNum
vector<double> Allsprites_Distance(decorationNum + illustrationNum);
// ȫ��Sprite�������������ڻ���ȫ��Sprite
// ע�⴫��vector������
void sortAllSprites(vector<int> &Allsprites_Order, vector<double> &Allsprites_Distance, int amount);

// ��ȡʱ��
unsigned long getTicks();
// ��ȡ�����ֱ��ȡǽ��ذ��컨������decoration����illustration����
// ���������е����޸�alpha�ĺ�������ֹ���ִ����ڵ�
void LoadTexture();
// �޸�����͸���ȣ�����decoration��illustration��������ɫ�޸�Ϊ͸�� (Alphaͨ����Ϊ0)
void SetTexture_Alpha(Texture *texture, int start_index, int end_index);
// ����ǽ�ڻ���
void Wall_Casting(RenderWindow &window);
// ����ذ��컨�����
void Floor_Ceiling_Casting(RenderWindow &window);
// �ƶ����������ڸı�λ�úͳ���
// ������������߼�
void Move(RenderWindow &window);
// ����FOV��С��������
double calculate_FOV();
// ����Decoration Sprite
void Decoration_Casting(RenderWindow &window);
// ����illustration Sprite
void Illustration_Casting(RenderWindow &window);
// ����ȫ��Sprite
void Allsprites_Casting(RenderWindow &window);

/************************************************************/
/* ����С��ͼ
/************************************************************/
// С��ͼ�Ŀ�Ⱥ͸߶�
const int minimapWidth = 24;
const int minimapHeight = 24;
// С��ͼ�Ĵ�С����λ�����أ�
const int miniMapScale = 15;
bool ifdraw_Minimap = 1;
// ����һ����ʾ��Ұ��Χ����������
VertexArray FOV_Visualize(TriangleFan, screenWidth + 1);
// ��¼��Ұ��Χÿһ����λ��
// ��ǽ�ڻ��ƺ������޸�ֵ����drawMiniMap����������FOVλ��
vector<Vector2f> FOV_position(screenWidth + 1);
// ����С��ͼ����
void drawMiniMap(RenderWindow &window);

int main()
{
    RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Raycasting-Final");

    // ��������
    LoadTexture();

    // �Ƿ���������߼�����
    if (ifMouse)
    {
        Mouse::setPosition(Vector2i(screenWidth * 0.5f, screenHeight * 0.5f), window);
        window.setMouseCursorVisible(0);
    }

    // ����FOV�ӽ�
    FOV = calculate_FOV();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            // ���ǵ���������ʹ��ESC�˳�
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();
            // sf::Keyboard::isKeyPressed��������ÿһ֡�������̵�״̬
            // sf::Event::KeyPressed�¼�ֻ���ڰ��������µ���һ֡����һ�Σ���ʹ�㰴ס�������ţ���Ҳ��������һ֡�ٴδ���
            // ��ˣ�ʹ��sf::Event::KeyPressed�¼�����ȷ��ifdraw_Minimap��ֵֻ�ᱻ��תһ��
            if (event.type == sf::Event::KeyPressed)
            {
                // ��Tab����~����M������С��ͼ
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
    // ��ȡǽ�ڡ��ذ��컨������
    texture[0].loadFromFile("pics/eagle.png");
    texture[1].loadFromFile("pics/redbrick.png");
    texture[2].loadFromFile("pics/purplestone.png");
    texture[3].loadFromFile("pics/greystone.png");
    texture[4].loadFromFile("pics/bluestone.png");
    texture[5].loadFromFile("pics/mossy.png");
    texture[6].loadFromFile("pics/wood.png");
    texture[7].loadFromFile("pics/colorstone.png");
    // �ذ��컨����Ⱦ�õ�image
    Ceiling_image = texture[6].copyToImage(); // �컨��Ϊľͷ
    Floor_image = texture[3].copyToImage();   // �ذ�Ϊʯͷ
    // ��ȡdecoration����
    decoration_texture[0].loadFromFile("pics/greenlight.png");
    decoration_texture[1].loadFromFile("pics/pillar.png");
    decoration_texture[2].loadFromFile("pics/barrel.png");
    // ���ڲ��ָ�͸��
    SetTexture_Alpha(decoration_texture, 0, 2);
    // ��ȡillustration����
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
    // ���ڲ��ָ�͸��
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
                    // ����Ϊ��ȫ͸��
                    tmp.setPixel(x, y, sf::Color(0, 0, 0, 0));
                }
            }
        }
        texture[i].loadFromImage(tmp); // ���޸ĺ��ͼ�񸲸�ԭ���������
    }
}

void Floor_Ceiling_Casting(RenderWindow &window)
{
    // ����һ���µ�Image
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
    for (int y = screenHeight / 2; y < screenHeight; y++) // ��ʵ����ֱ�Ӵ�screenHeight / 2��ʼ����
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

            // ����������ɫ�������ظ�ֵ
            Color Floorcolor = Floor_image.getPixel(tx, ty);
            Color Ceilingcolor = Ceiling_image.getPixel(tx, ty);
            Floor_Ceiling_image.setPixel(x, y, Floorcolor);
            Floor_Ceiling_image.setPixel(x, screenHeight - y - 1, Ceilingcolor);
        }
    }
    // ����һ���µ�Texture
    Texture Floor_Ceiling_texture;
    Floor_Ceiling_texture.loadFromImage(Floor_Ceiling_image);
    // ����һ���µ�Sprite������������
    Sprite sprite;
    sprite.setTexture(Floor_Ceiling_texture);
    // �����Sprite������Ļ��
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
        /* ��������ЧӦ��ش���
        /************************************************************/
        // �˶δ�����������ЧӦ����������������ЧӦ
        double deltaDistX = (rayDirX == 0) ? 1e30 : std::abs(1 / rayDirX);
        double deltaDistY = (rayDirY == 0) ? 1e30 : std::abs(1 / rayDirY);

        //// �˶δ�����������ЧӦ
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
        // �̳����ⲿ�ֽ��͵ıȽ������
        // Ҳ���Կ����ڲ������֤�����̣�д�ĸ����
        if (side == 0)
            perpWallDist = (sideDistX - deltaDistX);
        else
            perpWallDist = (sideDistY - deltaDistY);

        /*********************************************************************************/
        /*  �˲���Ϊ���FOV_Visualize��
        /*********************************************************************************/
        // ���������յ�
        double rayEndX = posX + perpWallDist * rayDirX;
        double rayEndY = posY + perpWallDist * rayDirY;

        // ��¼�������ζ���λ��
        FOV_position[x + 1] = Vector2f(rayEndX * miniMapScale, (mapHeight - rayEndY) * miniMapScale);

        /*********************************************************************************/
        /* ���ZBuffer
        /*********************************************************************************/
        ZBuffer[x] = perpWallDist;

        // Calculate height of line to draw on screen
        // ���������޸�ǽ�ĸ߶�
        int lineHeight = (int)(screenHeight / perpWallDist);

        // calculate lowest and highest pixel to fill in current stripe
        int drawStart = -lineHeight / 2 + screenHeight / 2;
        if (drawStart < 0)
            drawStart = 0;
        int drawEnd = lineHeight / 2 + screenHeight / 2;
        if (drawEnd >= screenHeight)
            drawEnd = screenHeight - 1;

        /*********************************************************************************/
        /*  �˲����滻��ԭraycasting���߲���
        /*  ����Ϊ������ؼ��㣬��������
        /*  �����ԭ���Ļ��ߺ�����࣬�����ھ���λ���ϻ���һ����
        /*  ֻ�������ﻭ���Ǿ��������ϵ�һ���ߣ�������SFML�е�sprite
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
        // ��ֹ����
        if (side == 0 && rayDirX > 0)
            texX = texWidth - texX - 1;
        if (side == 1 && rayDirY < 0)
            texX = texWidth - texX - 1;

        // Ϊ�˻�ȡ�������ĳһ�������������ɫ
        // ����һ���µ�sprite����
        sf::Sprite wall_sprite;

        // ����sprite������
        wall_sprite.setTexture(texture[texNum]);

        // ����sprite��������Σ��������������е��ĸ�����Ӧ�õ�wall_sprite��
        // ���Ϊ1�����أ��൱��ԭ���Ļ���
        wall_sprite.setTextureRect(sf::IntRect(texX, 0, 1, texHeight));

        // ����sprite��λ�ã�����ָ�������Ͻǵ�λ��
        // wall_sprite.setPosition(x, drawStart);        //̫���Ѷ����ˣ��ư��ˣ�ָ�����Ͻǵ�λ��ΪdrawStart
        //                                               //drawStart����������»ᱻ��ֵΪ0����if (drawStart < 0) drawStart = 0;
        //                                               //�ڴ�����»��Ƶ����ͻ�ı�Ϊ0�����˶���
        wall_sprite.setPosition(x, round(0.5f * screenHeight - 0.5f * lineHeight));

        // ����sprite����������
        wall_sprite.setScale(1, lineHeight / (float)texHeight);

        // ����SFML��sprite��setColor���ܣ�ʹyǽ�䰵
        // �������ʵ�ֵ��ǡ����ƻ�ϡ�
        // �ٸ����ӣ�ԭ����Rͨ��Ϊ200����128���Ϊ��200 * 128 / 255 = 100��ʵ������ɫ����2��Ч��
        if (side == 1)
        {
            wall_sprite.setColor(Color(128, 128, 128));
        }
        // ����sprite
        window.draw(wall_sprite);
    }
}

// �ƶ����������ڸı�λ�úͳ���
// �Ѿ�ʵ���������ת
void Move(RenderWindow &window)
{
    // timing for input and FPS counter
    old_Time = new_Time;
    new_Time = getTicks();
    double frameTime = (new_Time - old_Time) / 1000.0; // frameTime is the time this frame has taken, in seconds

    // speed modifiers
    double moveSpeed = frameTime * 3.0; // the constant value is in squares/second
    // �����ת�ٶȼ�����Ƿ���������߼���������̹���ӽ��ƶ�
    double rotSpeed = frameTime * 2.0; // the constant value is in radians/second

    // ��Щ�Ǽ�������ƶ��߼�
    // ��ȡ���ڵ�����λ��
    Vector2i center(screenWidth * 0.5f, screenHeight * 0.5f);

    // ��ȡ��굱ǰλ��
    Vector2i position_now = Mouse::getPosition(window);

    // ����������ת�Ƕ�
    // ˮƽ������ת�Ƕ�
    double rotation_degree_horizontal = (center.x - position_now.x) * FOV / screenWidth;

    if (ifMouse)
    {
        // ���·���
        // dir��planeһ�����
        double oldDirX = dirX;
        dirX = dirX * cos(rotation_degree_horizontal) - dirY * sin(rotation_degree_horizontal);
        dirY = oldDirX * sin(rotation_degree_horizontal) + dirY * cos(rotation_degree_horizontal);
        double oldPlaneX = planeX;
        planeX = planeX * cos(rotation_degree_horizontal) - planeY * sin(rotation_degree_horizontal);
        planeY = oldPlaneX * sin(rotation_degree_horizontal) + planeY * cos(rotation_degree_horizontal);

        // �����λ�����õ���������
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
    // �����ߣ�ԭ���Ǹ�ǰ����һ����ֻ�Ƿ����þ��������������
    // �����߾�����(dirY, -dirX)���������
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        if (worldMap[int(posX + dirY * moveSpeed * 0.75)][int(posY)] == false)
            posX += dirY * moveSpeed * 0.75;
        if (worldMap[int(posX)][int(posY - dirX * moveSpeed * 0.75)] == false)
            posY -= dirX * moveSpeed * 0.75;
    }
    // ������ͬ��
    // ��(-dirY, dirX)����
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
    // ������
    return atan(length_plane / length_dir) * 2;
    // �Ƕ���
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
    // ���pairĬ�Ͼ��õ�һ����������������
    std::sort(sprites.begin(), sprites.end());
    // restore in reverse order to go from farthest to nearest
    for (int i = 0; i < amount; i++)
    {
        dist[i] = sprites[amount - i - 1].first;
        order[i] = sprites[amount - i - 1].second;
    }
}

// ����Decoration Sprite
void Decoration_Casting(RenderWindow &window)
{
    for (int i = 0; i < decorationNum; i++)
    {
        decoration_Order[i] = i;
        decoration_Distance[i] = ((posX - decoration_lst[i].x) * (posX - decoration_lst[i].x) + (posY - decoration_lst[i].y) * (posY - decoration_lst[i].y)); // sqrt not taken, unneeded
    }
    // ����
    // �Ȼ�Զ���ٻ����Ĳ��ᵼ��͸���ڵ�����
    // ��Ϊ����Sprite͸���ط�����ֱ����ʾ��Զ��Sprite
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

        // sprite����Ļ�ϵ�x����
        int spriteScreenX = int((screenWidth / 2) * (1 + transformX / transformY));

        // calculate height of the sprite on screen
        int spriteHeight = abs(int(screenHeight / (transformY))); // using 'transformY' instead of the real distance prevents fisheye
        // calculate width of the sprite
        int spriteWidth = abs(int(screenHeight / (transformY)));

        // calculate lowest and highest pixel to fill in current stripe
        // ����y�ϵ���㣬����Ҫ�����յ㣬��Ϊ����ֱ��ָ����Sprite�ĸ߶�
        int drawStartY = -spriteHeight / 2 + screenHeight / 2;
        // if (drawStartY < 0) drawStartY = 0;

        // ����x�ϵ������յ�
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
                // decorationһ���ߵ�Sprite
                Sprite decoration_sprite;
                // ���ö�Ӧ����
                decoration_sprite.setTexture(decoration_texture[decoration_lst[decoration_Order[i]].texture_index]);
                // ����sprite��������Σ��������������е��ĸ�����Ӧ�õ�decoration_sprite��
                // ���Ϊ1�����أ��൱��ԭ���Ļ���
                decoration_sprite.setTextureRect(sf::IntRect(texX, 0, 1, texHeight));
                // ����sprite��λ�ã�����ָ�������Ͻǵ�λ��
                decoration_sprite.setPosition(stripe, drawStartY);
                // ����sprite����������
                decoration_sprite.setScale(1, spriteHeight / (float)texHeight);
                // ����sprite
                window.draw(decoration_sprite);
            }
        }
    }
}

// ����illustration Sprite
// ���illustration��decoration�ֿ������ᵼ��͸���ڵ�����
void Illustration_Casting(RenderWindow &window)
{
    for (int i = 0; i < illustrationNum; i++)
    {
        illustration_Order[i] = i;
        illustration_Distance[i] = ((posX - illustration_lst[i].x) * (posX - illustration_lst[i].x) + (posY - illustration_lst[i].y) * (posY - illustration_lst[i].y)); // sqrt not taken, unneeded
    }
    // ����
    // �Ȼ�Զ���ٻ����Ĳ��ᵼ��͸���ڵ�����
    // ��Ϊ����Sprite͸���ط�����ֱ����ʾ��Զ��Sprite
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

        // sprite����Ļ�ϵ�x����
        int spriteScreenX = int((screenWidth / 2) * (1 + transformX / transformY));

        // calculate height of the sprite on screen
        // ע��illustration������Ҫ���߶ȳ�һ��illustrationHeight / illustrationWidth
        // ���ڱ��ֳ���ȣ���Ȼ���ÿ�����ԭ��ϣ����ѹ����������
        int spriteHeight = abs(int(screenHeight / (transformY))) * illustrationHeight / illustrationWidth; // using 'transformY' instead of the real distance prevents fisheye
        // calculate width of the sprite
        int spriteWidth = abs(int(screenHeight / (transformY)));

        // calculate lowest and highest pixel to fill in current stripe
        // ����y�ϵ���㣬����Ҫ�����յ㣬��Ϊ����ֱ��ָ����Sprite�ĸ߶�
        int drawStartY = -spriteHeight / 2 + screenHeight / 2;
        // if (drawStartY < 0) drawStartY = 0;

        // ����x�ϵ������յ�
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
                // illustrationһ���ߵ�Sprite
                Sprite illustration_sprite;
                // ���ö�Ӧ����
                illustration_sprite.setTexture(illustration_texture[illustration_lst[illustration_Order[i]].texture_index]);
                // ����sprite��������Σ��������������е��ĸ�����Ӧ�õ�illustration_sprite��
                // ���Ϊ1�����أ��൱��ԭ���Ļ���
                illustration_sprite.setTextureRect(sf::IntRect(texX, 0, 1, illustrationHeight));
                // ����sprite��λ�ã�����ָ�������Ͻǵ�λ��
                illustration_sprite.setPosition(stripe, drawStartY);
                // ����sprite����������
                illustration_sprite.setScale(1, spriteHeight / (float)illustrationHeight);
                // ����sprite
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

// �ɹ�����ڵ����⣬ԭ��sortAllSprites����û����vector�����ã�����û����������
void Allsprites_Casting(RenderWindow &window)
{
    for (int i = 0; i < decorationNum + illustrationNum; i++)
    {
        Allsprites_Order[i] = i;
        Allsprites_Distance[i] = ((posX - Allsprites_lst[i].x) * (posX - Allsprites_lst[i].x) + (posY - Allsprites_lst[i].y) * (posY - Allsprites_lst[i].y)); // sqrt not taken, unneeded
    }
    // ����
    // �Ȼ�Զ���ٻ����Ĳ��ᵼ��͸���ڵ�����
    // ��Ϊ����Sprite͸���ط�����ֱ����ʾ��Զ��Sprite
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

        // sprite����Ļ�ϵ�x����
        int spriteScreenX = int((screenWidth / 2) * (1 + transformX / transformY));

        // calculate height of the sprite on screen
        int spriteHeight = 0;
        // decoration��height���ó˱���
        if (Allsprites_lst[Allsprites_Order[i]].type == 0)
        {
            spriteHeight = abs(int(screenHeight / (transformY))); // using 'transformY' instead of the real distance prevents fisheye
        }
        // illustration��height��Ҫ�˱���
        // ���ڱ��ֳ���ȣ���Ȼ���汻ѹ��������
        else if (Allsprites_lst[Allsprites_Order[i]].type == 1)
        {
            spriteHeight = abs(int(screenHeight / (transformY))) * illustrationHeight / illustrationWidth; // using 'transformY' instead of the real distance prevents fisheye
        }

        // calculate width of the sprite
        int spriteWidth = abs(int(screenHeight / (transformY)));

        // calculate lowest and highest pixel to fill in current stripe
        // ����y�ϵ���㣬����Ҫ�����յ㣬��Ϊ����ֱ��ָ����Sprite�ĸ߶�
        int drawStartY = -spriteHeight / 2 + screenHeight / 2;
        // if (drawStartY < 0) drawStartY = 0;

        // ����x�ϵ������յ�
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
                // ��decoration
                if (Allsprites_lst[Allsprites_Order[i]].type == 0)
                {
                    // ����texX
                    int texX = int(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * texWidth / spriteWidth) / 256;
                    // decorationһ���ߵ�Sprite
                    Sprite decoration_sprite;
                    // ���ö�Ӧ����
                    decoration_sprite.setTexture(decoration_texture[Allsprites_lst[Allsprites_Order[i]].texture_index]);
                    // ����sprite��������Σ��������������е��ĸ�����Ӧ�õ�decoration_sprite��
                    // ���Ϊ1�����أ��൱��ԭ���Ļ���
                    decoration_sprite.setTextureRect(sf::IntRect(texX, 0, 1, texHeight));
                    // ����sprite��λ�ã�����ָ�������Ͻǵ�λ��
                    decoration_sprite.setPosition(stripe, drawStartY);
                    // ����sprite����������
                    decoration_sprite.setScale(1, spriteHeight / (float)texHeight);
                    // ����sprite
                    window.draw(decoration_sprite);
                }
                // ��illustration
                else if (Allsprites_lst[Allsprites_Order[i]].type == 1)
                {
                    // ����texX
                    int texX = int(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * illustrationWidth / spriteWidth) / 256;
                    // illustrationһ���ߵ�Sprite
                    Sprite illustration_sprite;
                    // ���ö�Ӧ����
                    illustration_sprite.setTexture(illustration_texture[Allsprites_lst[Allsprites_Order[i]].texture_index]);
                    // ����sprite��������Σ��������������е��ĸ�����Ӧ�õ�illustration_sprite��
                    // ���Ϊ1�����أ��൱��ԭ���Ļ���
                    illustration_sprite.setTextureRect(sf::IntRect(texX, 0, 1, illustrationHeight));
                    // ����sprite��λ�ã�����ָ�������Ͻǵ�λ��
                    illustration_sprite.setPosition(stripe, drawStartY);
                    // ����sprite����������
                    illustration_sprite.setScale(1, spriteHeight / (float)illustrationHeight);
                    // ����sprite
                    window.draw(illustration_sprite);
                }
            }
        }
    }
}

// Ҫע�⣺SFML�е�y�������µģ����ö�ά�������ʱy�������ϵ�
// �轫��Ϸ�е�����ϵת��ΪС��ͼ������ϵ��������˵�����ǽ�y������з�ת
void drawMiniMap(RenderWindow &window)
{
    // ������ͼ��ÿһ������
    for (int x = 0; x < minimapWidth; x++)
    {
        for (int y = 0; y < minimapHeight; y++)
        {
            // ����һ����ʾ��ͼ���ӵľ���
            RectangleShape rect(Vector2f(miniMapScale, miniMapScale));
            rect.setPosition(x * miniMapScale, (mapHeight - y - 1) * miniMapScale);

            // ���ݵ�ͼ�������þ��ε���ɫ
            if (worldMap[x][y] > 0)
            {
                rect.setFillColor(Color::White);
            }
            else
            {
                rect.setFillColor(Color::Black);
            }

            // �ڴ����ϻ��ƾ���
            window.draw(rect);
        }
    }

    // ����һ����ʾ���λ�õ�Բ��
    int radius = 4;
    CircleShape player_circle(radius);
    player_circle.setFillColor(Color::Red);
    player_circle.setPosition(posX * miniMapScale - player_circle.getRadius(), (mapHeight - posY) * miniMapScale - player_circle.getRadius());
    // �ڴ����ϻ���Բ��
    window.draw(player_circle);

    // �����������ε�ԭ��Ϊ��ҵ�λ��
    // FOV������������DDA�㷨�������FOV_position������
    FOV_Visualize[0].position = Vector2f(posX * miniMapScale, (mapHeight - posY) * miniMapScale);
    FOV_Visualize[0].color = Color::Transparent;
    for (int i = 1; i <= screenWidth; i++)
    {
        // ����FOV��λ��
        FOV_Visualize[i].position = FOV_position[i];
    }
    // �ڴ����ϻ�����������
    window.draw(FOV_Visualize);
}