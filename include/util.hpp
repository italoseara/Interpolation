#ifndef UTIL_HPP
#define UTIL_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <map>

namespace util
{
  using namespace std;

  struct Color
  {
    unsigned char r, g, b;

    string toString();
  };

  struct Image
  {
    string name;
    Color **pixels;
    int width, height;
    int max_color;

    Image() : pixels(nullptr), width(0), height(0), max_color(0) {}
    Image(string filename);

    Image nearestNeighbor(float scale_x, float scale_y);
    Image bilinear(float scale_x, float scale_y);
    void save(string filename);
    void show(string title, int x, int y);
    void show();
  };
  
  void printUsage(char *argv[]);
  map<string, string> parseArgs(int argc, char *argv[]);

  void initOpenGL();
  void initLoop();
}

#endif // UTIL_HPP