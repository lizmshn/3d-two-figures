#define _USE_MATH_DEFINES
#include <iostream>
#include "graphics.h"
#include <math.h>
#include <limits>
#include <algorithm>
#include <thread>
#include "Array.h"
#define WIDTH 600
#define HEIGHT 600
#define ANGLE 15
#define COEF 1.1
#define SPEED 0.05
#define UP 72
#define LEFT 75
#define DOWN 80
#define RIGHT 77
#define MINUS 45
#define PLUS 61
#define W 119
#define S 115
#define Q 113
#define A 97
#define D 100
#define Z 122
#define X 120
#define I 105
#define O 111
#define NOL 48
using namespace std;
class Coordinate
{
public:
double x;
double y;
double z;
};
class Triangle
{
public:
vector<Coordinate> coordinates;
int color;
};
double Deg_to_rad(double deg)
{
return deg * M_PI / 180.0;
}
bool operator==(const Coordinate& one, const Coordinate& two)
{
return (one.x == two.x && one.y == two.y);
}
Coordinate Centroid_figure(const vector<Triangle>& figure)
{
double x = 0;
double y = 0;
double z = 0;
int count_side = figure.size();
for (size_t i = 0; i < count_side; ++i) {
for (size_t j = 0; j < 3; ++j) {
x += figure[i].coordinates[j].x;
y += figure[i].coordinates[j].y;
z += figure[i].coordinates[j].z;
}
}
x /= count_side * 3;
y /= count_side * 3;
z /= count_side * 3;
return
{ x, y, z};
}
void Rotate(vector<Triangle>& figure, double ang[3])
{
double x, y, z;
Coordinate center = Centroid_figure(figure);
Array<double> rotate_x
({
{1, 0, 0},
{0, cos(ang[0]), sin(ang[0])},
{0, -sin(ang[0]), cos(ang[0])}
});
Array<double> rotate_y
({
{cos(ang[1]), 0, -sin(ang[1])}, 
{0, 1, 0},
{sin(ang[1]), 0, cos(ang[1])}
});
Array<double> rotate_z
({
{cos(ang[2]), sin(ang[2]), 0},
{-sin(ang[2]), cos(ang[2]), 0},
{0, 0, 1}
});
Array<double> result = rotate_x * rotate_y * rotate_z;
for (size_t i = 0; i < figure.size(); ++i) {
for (size_t j = 0; j < 3; ++j) {
x = figure[i].coordinates[j].x;
y = figure[i].coordinates[j].y;
z = figure[i].coordinates[j].z;
Array<double> temp
({
{x - center.x,
y - center.y,
z - center.z}
});
Array<double> new_coordinates = temp * result;
figure[i].coordinates[j].x = new_coordinates(0, 0) + center.x;
figure[i].coordinates[j].y = new_coordinates(0, 1) + center.y;
figure[i].coordinates[j].z = new_coordinates(0, 2) + center.z;
}
}
}
void Scaling(vector<Triangle>& figure, double coefficient)
{
Coordinate center = Centroid_figure(figure);
if (coefficient > 0) 
coefficient = coefficient;
else
coefficient = -1 / coefficient;
Array<double> scale_array
({
{coefficient, 0, 0},
{0, coefficient, 0},
{0, 0, coefficient}
});
double x, y, z;
for (size_t i = 0; i < figure.size(); ++i)
{
for (size_t j = 0; j < 3; ++j)
{
x = figure[i].coordinates[j].x;
y = figure[i].coordinates[j].y;
z = figure[i].coordinates[j].z;
Array<double> temp
({
{x - center.x,
y - center.y,
z - center.z}
});
Array<double> new_coordinate = temp * scale_array;
figure[i].coordinates[j].x = new_coordinate(0, 0) + center.x;
figure[i].coordinates[j].y = new_coordinate(0, 1) + center.y;
figure[i].coordinates[j].z = new_coordinate(0, 2) + center.z;
}
}
}
void Moving(vector<Triangle>& figure, double xx, double yy, double zz) 
{
for (size_t i = 0; i < figure.size(); ++i)
{
for (size_t j = 0; j < 3; ++j)
{
figure[i].coordinates[j].x += xx;
figure[i].coordinates[j].y += yy;
figure[i].coordinates[j].z += zz;
}
}
}
bool Intersection(const vector<Coordinate> line_one, const vector<Coordinate> line_two,
double* x, double* y)
{//inter 2lin has 2 point tru-if insert
double c1_x, c1_y, c2_x, c2_y, s, t;
c1_x = line_one[1].x - line_one[0].x;
c1_y = line_one[1].y - line_one[0].y;
c2_x = line_two[1].x - line_two[0].x;
c2_y = line_two[1].y - line_two[0].y;
//scalar param that mul on napravl vectrs find point insert
s = (-c1_y * (line_one[0].x - line_two[0].x) + c1_x * (line_one[0].y - line_two[0].y)) / (-
c2_x * c1_y + c1_x * c2_y);
t = (c2_x * (line_one[0].y - line_two[0].y) - c2_y * (line_one[0].x - line_two[0].x)) / (-
c2_x * c1_y + c1_x * c2_y);
//еif they betwe 0 1 poiny on 2lin
if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
{
*x = line_one[0].x + (t * c1_x);
*y = line_one[0].y + (t * c1_y);
return true;
}
return false;
}
bool Point_window(Coordinate p, vector<Coordinate> w)
{
return p.x >= w[0].x && p.x <= w[1].x && p.y <= w[2].y && p.y >= w[0].y;
}
//pont o on that side all side tria
double Sign(Coordinate p1, Coordinate p2, Coordinate p3)
{//+=left turn -=right 0=kollinearn
return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}
bool Point_triangle(Coordinate p, vector<Coordinate> t)
{
double d1, d2, d3;
d1 = Sign(p, t[0], t[1]);
d2 = Sign(p, t[1], t[2]);
d3 = Sign(p, t[2], t[0]);
return !(((d1 <= 0) || (d2 <= 0) || (d3 <= 0)) && ((d1 >= 0) || (d2 >= 0) || (d3 >= 0)));
}
bool Point_polygon(Coordinate p, vector<Coordinate> polygon)
{
for (size_t i = 0; i < polygon.size(); ++i)
{
const Coordinate& vertex = polygon[i];
if (vertex == p) {
return true;
}
}
return false;
}
Coordinate Centroid_polygon(const vector<Coordinate>& polygon)
{
double x = 0;
double y = 0;
double z = 0;
int count = polygon.size();
for (size_t i = 0; i < count; i++)
{
x += polygon[i].x;
y += polygon[i].y;
z += polygon[i].z;
}
x /= count;
y /= count;
z /= count;
return { x, y, z };
}
vector<Coordinate> Get_polygon_intersection(const vector<Coordinate>& p, const
vector<Coordinate>& w)
{
vector<Coordinate> result;
int count_vert_p = p.size();
int count_vert_w = w.size();
double x, y;
for (size_t i = 0; i < count_vert_w; ++i)
{//has poi pol on anoth
if (Point_triangle(w[i], p))
result.push_back(w[i]);//add ever poin w to pol p
}
for (size_t i = 0; i < count_vert_p; ++i)
{
if (Point_window(p[i], w))
result.push_back(p[i]);//anoth
}
for (size_t i = 0; i < count_vert_p; ++i)
{//flag insert reber ret poin inser rebe
vector<Coordinate> line_p = { p[i], p[(i + 1) % count_vert_p] };
for (size_t j = 0; j < count_vert_w; ++j)
{
vector<Coordinate> line_w = { w[j], w[(j + 1) % count_vert_w] };
if (Intersection(line_p, line_w, &x, &y))
{//if fou add to vect res
if (!Point_polygon({ x, y }, result))
result.push_back({ x, y });
}
}
}
Coordinate center = Centroid_polygon(result);//полученный вектор точек res
сортируется по углу относительно центра многоугольника пересечения
sort(result.begin(), result.end(), [center](Coordinate a, Coordinate b)
{//сортировка нужна, чтобы корректно соединить их
if (a.x - center.x >= 0 && b.x - center.x < 0)
return true;//сравнение квадрант (если a слева от center и b слева=а идет
первой)
if (a.x - center.x < 0 && b.x - center.x >= 0)
return false;//если b справа= b идет первой
if (a.x - center.x == 0 && b.x - center.x == 0)
{
if (a.y - center.y >= 0 || b.y - center.y >= 0)
return a.y > b.y;//вертикальное. приоритет у точки которая выше
по Y
return b.y > a.y;
}
//определитель - получается из координат a b относительно центра.
направление поворота от a к b относительно center
int det = (a.x - center.x) * (b.y - center.y) - (b.x - center.x) * (a.y - center.y);
if (det < 0) 
return true;//a первая
if (det > 0)
return false;//b
//сравнение расстояния до центра. которая дальше - первая
int d1 = (a.x - center.x) * (a.x - center.x) + (a.y - center.y) * (a.y - center.y);
int d2 = (b.x - center.x) * (b.x - center.x) + (b.y - center.y) * (b.y - center.y);
return d1 > d2;
});
return result;
}
void Fill(const vector<Coordinate>& polygon, int color = WHITE)
{
setcolor(color);
int count_vertex = polygon.size();//вычисляются ограничивающий прямоугольник
double bounds[2][2] =
{
{polygon[0].x, polygon[0].y},
{polygon[0].x, polygon[0].y}
};
for (int i = 1; i < count_vertex; ++i)
{//по горизонтальным линиям от нижнего края до верхнего
//ищет левый нижний и правый верхний)
int x = polygon[i].x, y = polygon[i].y;
if (x < bounds[0][0])
bounds[0][0] = x;
else if (x > bounds[1][0])
bounds[1][0] = x;
if (y < bounds[0][1])
bounds[0][1] = y;
else if (y > bounds[1][1])
bounds[1][1] = y;
}
for (double i = bounds[0][1] + 0.01; i <= bounds[1][1]; ++i)
{
double pts[20] = { 0 };
int count = 0;
vector<Coordinate> pt_vec =
{
{bounds[0][0] - 1, i},
{bounds[1][0] + 1, i}
};
for (int j = 0; j < count_vertex; ++j)
{
vector<Coordinate> line =
{
{polygon[j].x, polygon[j].y},
{polygon[(j + 1) % count_vertex].x, polygon[(j + 1) %
count_vertex].y}
};
double x, y;
if (Intersection(pt_vec, line, &x, &y))
pts[count++] = x;//если пересечение=то тестирует на пересеч с
каждой стороной
}
if (count) {//как только все пересечения найдены, pts сортируется по X.
qsort(pts, count, sizeof(double), [](const void* x, const void* y)
{ return (int)(*(double*)x - *(double*)y); });
for (int j = 0; j < count; ++j)
{
if (pts[j + 1])
line(pts[j], i, pts[j + 1], i);//для каждой пары пересечений
рисует горизонтальные линии внутри
}
}
}
}
//отображение с заливкой и границей. если значений нет он сам заполняет
void Show_polygon(const vector<Coordinate>& polygon, int fill_color = WHITE, int 
border_color = WHITE)
{
int count_vert = polygon.size();
if (!count_vert)
return;
if (fill_color != -1)
Fill(polygon, fill_color);
if (border_color != -1)
{
setcolor(border_color);
for (size_t i = 0; i < count_vert; ++i)
{
line(polygon[i].x, polygon[i].y, polygon[(i + 1) % count_vert].x, polygon[(i
+ 1) % count_vert].y);
}
}
}
//поиск и подсчет треугольников, которые пересекаются с окном
int Get_count_polygon_window(const vector<Triangle>& figure, const vector<Coordinate>&
win, vector<Triangle>& ps)
{
int count = 0;//в массиве если есть пересечение, то +счетчик и в вектор ps добавляется
новый треугольник, составленный из точек пересечений с сохранением исходного цвета
for (size_t i = 0; i < figure.size(); ++i)
{
vector<Coordinate> temp = Get_polygon_intersection(figure[i].coordinates, win);
if (temp.size())
{
count++;
ps.push_back({ temp, figure[i].color });
}
}
return count;
}
bool Polygon_window(vector<Coordinate>& poly, vector<Coordinate>& win)
{
for (size_t i = 0; i < poly.size(); ++i)
{
if (!Point_window(poly[i], win))
return false;
}
return true;
}
void Divide(const vector<Triangle>& figure, double x1 = 0, double y1 = 0, double x2 = WIDTH,
double y2 = HEIGHT)
{
if (x2 - x1 < 1 || y2 - y1 < 1)
return;//проверка не слишком ли мала область. возвращает управление
vector<Coordinate> window =
{
{x1, y1},
{x2, y1},
{x2, y2},
{x1, y2}
};
vector<Triangle> polygons;
int count = Get_count_polygon_window(figure, window, polygons);
if (!count)
return;
else if (count == 1)
{
Show_polygon(polygons[0].coordinates, polygons[0].color, -1);//если один -
отображается
return;
}
else
{//содержится ли окно полностью внутри каждого треугольника
bool is_window_polygon;
vector<Triangle> out_tri;
for (size_t i = 0; i < figure.size(); ++i)
{
is_window_polygon = true;
for (size_t j = 0; j < 4; ++j)
{
if (!Point_triangle(window[j], figure[i].coordinates))
is_window_polygon = false;
}
if (is_window_polygon)
{
out_tri.push_back(figure[i]);//если да - то добавляются в
переменную
}
}
if (out_tri.size() == count)
{//если совпадает - находим треугольник с максимальным средним значением
Z
Triangle max_z_polygon = out_tri[0];
double avg_max_z = -9999.0;
double sum_max = 0;//расширяются границы
for (size_t i = 0; i < out_tri.size(); ++i)
{
for (size_t j = 0; j < 3; ++j)
{
sum_max += out_tri[i].coordinates[j].z;
}
if (avg_max_z < sum_max / 3.0) 
{
avg_max_z = sum_max / 3.0;
max_z_polygon = out_tri[i];
}
sum_max = 0;
}
//если не удалось отобразить все треугольники, то метод вызывается
рекурсивно для каждой из 4 подобластей
window =
{
{x1 - 1, y1 - 1},
{x2 + 1, y1 - 1},
{x2 + 1, y2 + 1},
{x1 - 1, y2 + 1}
};
Show_polygon(window, max_z_polygon.color, max_z_polygon.color);
return;
}
}
double xc = (x2 - x1) / 2 + x1;
double yc = (y2 - y1) / 2 + y1;
Divide(figure, x1, y1, xc, yc);
Divide(figure, x1, yc, xc, y2);
Divide(figure, xc, y1, x2, yc);
Divide(figure, xc, yc, x2, y2);
}
void Click(int x, int y)
{
cout << x << " " << y << "\n";
}
void Show_shadow(vector<Triangle>& polygons)
{
float ground = -4.5;
float light[3] = { 0.0, -1.0, 0.0 };
for (int i = 0; i < polygons.size(); ++i)
{
Triangle polygon;
for (int j = 0; j < 3; ++j)
{//point peres light from coord
Coordinate pt = polygons[i].coordinates[j];
float t = (ground - pt.y) / light[1];
pt.x += t * light[0];
pt.y = ground;
pt.z += t * light[2];
pt.y = 1.0 * pt.y / (-pt.z * 0.8);
pt.y = (-pt.y * 0.5 + 0.5) * HEIGHT;
polygon.coordinates.push_back(pt);
}
Show_polygon(polygon.coordinates, RGB(0, 0, 0), -1);
}
}
void Show(const vector<vector<Triangle>>& figures)
{
vector<Triangle> result;//copy tr from ever obj
for (auto& shape : figures) result.insert(result.end(), shape.begin(), shape.end());
int count_side = result.size();
Triangle polygon;
polygon.coordinates.resize(3);
vector<Triangle> vec_tri;
for (size_t i = 0; i < count_side; ++i)
{
for (size_t j = 0; j < 3; ++j)
{
polygon.coordinates[j].x = (result[i].coordinates[j].x * 0.5 + 0.5) * WIDTH;
polygon.coordinates[j].y = (result[i].coordinates[j].y * 0.5 + 0.5) *
HEIGHT;
polygon.coordinates[j].z = result[i].coordinates[j].z;
}
polygon.color = result[i].color;
vec_tri.push_back(polygon);
}
Show_shadow(vec_tri);
double x1 = 0;
double y1 = 0;
double x2 = WIDTH;
double y2 = HEIGHT;
vector<Coordinate> window =
{
{x1, y1},
{x2, y1},
{x2, y2},
{x1, y2}
};
Divide(vec_tri, x1, y1, x2, y2);
}
int main()
{
int window = initwindow(WIDTH, HEIGHT, "window");
registermousehandler(WM_LBUTTONDOWN, Click);
setbkcolor(RGB(201, 201, 201));
vector<Triangle> prism_one =
{
{ {{-1.5, -1.5, -6.0}, {0.0, -1.5, -8.4}, {1.5, -1.5, -6.0}}, 2},
{ {{-1.5, 1.5, -6.0}, {1.5, 1.5, -6.0}, {0.0, 1.5, -8.4}}, 1},
{ {{-1.5, -1.5, -6.0}, {1.5, -1.5, -6.0}, {1.5, 1.5, -6.0}}, 4},
{ {{-1.5, -1.5, -6.0}, {-1.5, 1.5, -6.0}, {1.5, 1.5, -6.0}}, 4},
{ {{-1.5, -1.5, -6.0}, {0.0, -1.5, -8.4}, {-1.5, 1.5, -6.0}}, 3},
{ {{0.0, -1.5, -8.4}, {0.0, 1.5, -8.4}, {-1.5, 1.5, -6.0}}, 3},
{ {{0.0, -1.5, -8.4}, {0.0, 1.5, -8.4}, {1.5, 1.5, -6.0}}, 6},
{ {{0.0, -1.5, -8.4}, {1.5, -1.5, -6.0}, {1.5, 1.5, -6.0}}, },
};
vector<Triangle> prism_two =
{
{ {{-1.5, -1.5, -6.0}, {0.0, -1.5, -8.4}, {1.5, -1.5, -6.0}}, 1},
{ {{-1.5, 1.5, -6.0}, {1.5, 1.5, -6.0}, {0.0, 1.5, -8.4}}, 2},
{ {{-1.5, -1.5, -6.0}, {1.5, -1.5, -6.0}, {1.5, 1.5, -6.0}}, 4},
{ {{-1.5, -1.5, -6.0}, {-1.5, 1.5, -6.0}, {1.5, 1.5, -6.0}}, 4},
{ {{-1.5, -1.5, -6.0}, {0.0, -1.5, -8.4}, {-1.5, 1.5, -6.0}}, 3},
{ {{0.0, -1.5, -8.4}, {0.0, 1.5, -8.4}, {-1.5, 1.5, -6.0}}, 3},
{ {{0.0, -1.5, -8.4}, {0.0, 1.5, -8.4}, {1.5, 1.5, -6.0}}, 6},
{ {{0.0, -1.5, -8.4}, {1.5, -1.5, -6.0}, {1.5, 1.5, -6.0}}, 6},
};
Scaling(prism_one, -COEF * 17);
Scaling(prism_two, -COEF * 17);
Moving(prism_one, SPEED * 5, SPEED * 5, 0);
Rotate(prism_one, new double[3] { Deg_to_rad(90), Deg_to_rad(40), Deg_to_rad(10) });
Rotate(prism_two, new double[3] { Deg_to_rad(-40), Deg_to_rad(-30), Deg_to_rad(5) });
vector<vector<Triangle>> figures =
{
prism_one, prism_two
};
Show(figures);
int c;
c = getch();
cout << c;
bool flag = true;
while (1) {
c = getch();
vector<Triangle>& currentFigure = (flag ? figures[0] : figures[1]);
switch (c) {
case S:
Rotate(currentFigure, new double[3] { Deg_to_rad(ANGLE),
Deg_to_rad(0), Deg_to_rad(0) });
break;
case A:
Rotate(currentFigure, new double[3] { Deg_to_rad(-ANGLE),
Deg_to_rad(0), Deg_to_rad(0) });
break;
case W:
Rotate(currentFigure, new double[3] { Deg_to_rad(0),
Deg_to_rad(ANGLE), Deg_to_rad(0) });
break;
case Q:
Rotate(currentFigure, new double[3] { Deg_to_rad(0), Deg_to_rad(-
ANGLE), Deg_to_rad(0) });
break;
case X:
Rotate(currentFigure, new double[3] { Deg_to_rad(0), Deg_to_rad(0),
Deg_to_rad(ANGLE) });
break;
case Z:
Rotate(currentFigure, new double[3] { Deg_to_rad(0), Deg_to_rad(0),
Deg_to_rad(-ANGLE) });
break;
case PLUS:
Scaling(currentFigure, COEF);
break;
case MINUS:
Scaling(currentFigure, -COEF);
break;
case UP:
Moving(currentFigure, 0, -SPEED, 0);
break;
case DOWN:
Moving(currentFigure, 0, SPEED, 0);
break;
case LEFT:
Moving(currentFigure, -SPEED, 0, 0);
break;
case RIGHT:
Moving(currentFigure, SPEED, 0, 0);
break;
case I:
Moving(currentFigure, 0, 0, -SPEED);
break;
case O:
Moving(currentFigure, 0, 0, SPEED);
break;
case NOL:
flag = !flag;
break;
}
Show(figures);
swapbuffers();
cleardevice();
}
return 0;
}
