#include <vector>
#include <array>
#include <iostream>

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cmath>

#if defined(WIN32) || defined(_WIN32)
#  define strcasecmp _stricmp
#endif

struct Vertex
{
  double position[3];
  double normal[3];

  double color_diffuse[3];
  double roughness;
  double metallic;
};

struct Triangle
{
  Vertex v[3];
};

struct Sphere
{
  double position[3];
  double radius;

  double color_diffuse[3];
  double roughness;
  double metallic;
};

struct Light
{
  double position[3];
  double normal[3];
  double color[3];
  double p[4][3];
};

const int MAX_TRIANGLES = 200000;
const int MAX_SPHERES = 1000;
const int MAX_LIGHTS = 1000;

std::vector<Triangle> triangles;
std::vector<Sphere> spheres;
std::vector<Light> lights;

double F0[3];
double ambient_light[3];

#define ASERT(cond)                                                      \
  do {                                                                   \
    if ((cond) == false) {                                               \
      std::cerr << #cond << " failed at line " << __LINE__ << std::endl; \
      exit(1);                                                           \
    }                                                                    \
  } while (0)

void parse_check(const char *expected, char *found)
{
  if (strcasecmp(expected, found)) {
    printf("Expected '%s ' found '%s '\n", expected, found);
    printf("Parse error, abnormal abortion\n");
    exit(1);
  }
}

void parse_doubles(FILE *file, const char *check, double p[3])
{
  char str[512];
  int ret = fscanf(file, "%s", str);
  ASERT(ret == 1);

  parse_check(check, str);

  ret = fscanf(file, "%lf %lf %lf", &p[0], &p[1], &p[2]);
  ASERT(ret == 3);

  printf("%s %lf %lf %lf\n", check, p[0], p[1], p[2]);
}

void parse_double(FILE *file, const char *check, double &r)
{
  char str[512];
  int ret = fscanf(file, "%s", str);
  ASERT(ret == 1);

  parse_check(check, str);

  ret = fscanf(file, "%lf", &r);
  ASERT(ret == 1);

  printf("%s %f\n", check, r);
}

int loadScene(const char *filename)
{
  FILE *file = fopen(filename, "r");
  int number_of_objects;
  char type[50] = { 0 };
  Triangle t;
  Sphere s;
  Light l;

  int ret = fscanf(file, "%i", &number_of_objects);
  ASERT(ret == 1);

  printf("number of objects: %i\n", number_of_objects);

  parse_doubles(file, "amb:", ambient_light);

  parse_doubles(file, "f0:", F0);

  for (int i = 0; i < number_of_objects; i++) {
    int ret = fscanf(file, "%s\n", type);
    ASERT(ret == 1);

    // printf("%s\n", type);
    if (strcasecmp(type, "triangle") == 0) {
      printf("found triangle\n");
      for (int j = 0; j < 3; j++) {
        parse_doubles(file, "pos:", t.v[j].position);
        parse_doubles(file, "nor:", t.v[j].normal);
        parse_doubles(file, "dif:", t.v[j].color_diffuse);

        parse_double(file, "rou:", t.v[j].roughness);
        parse_double(file, "met:", t.v[j].metallic);
      }

      if ((int)triangles.size() == MAX_TRIANGLES) {
        printf("too many triangles, you should increase MAX_TRIANGLES!\n");
        exit(0);
      }

      triangles.push_back(t);
    }
    else if (strcasecmp(type, "sphere") == 0) {
      printf("found sphere\n");

      parse_doubles(file, "pos:", s.position);
      parse_double(file, "rad:", s.radius);
      parse_doubles(file, "dif:", s.color_diffuse);

      parse_double(file, "rou:", s.roughness);
      parse_double(file, "met:", s.metallic);

      if ((int)spheres.size() == MAX_SPHERES) {
        printf("too many spheres, you should increase MAX_SPHERES!\n");
        exit(0);
      }

      spheres.push_back(s);
    }
    else if (strcasecmp(type, "light") == 0) {
      printf("found light\n");
      parse_doubles(file, "p0:", l.p[0]);
      parse_doubles(file, "p1:", l.p[1]);
      parse_doubles(file, "p2:", l.p[2]);
      parse_doubles(file, "p3:", l.p[3]);

      parse_doubles(file, "pos:", l.position);
      parse_doubles(file, "nrm:", l.normal);
      parse_doubles(file, "col:", l.color);

      if ((int)lights.size() == MAX_LIGHTS) {
        printf("too many lights, you should increase MAX_LIGHTS!\n");
        exit(0);
      }
      lights.push_back(l);
    }
    else {
      printf("unknown type in scene description:\n%s\n", type);
      exit(0);
    }
  }
  return 0;
}

int main(int argc, char *argv[])
{
  loadScene(argv[1]);

  return 0;
}