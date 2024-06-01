#include "util.hpp"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#define min(a, b) ((a) < (b) ? (a) : (b))

using namespace std;

void util::printUsage(char *argv[])
{
  cout << "Usage: " << endl;
  cout << "  " << argv[0] << " <input_file> [options]" << endl;
  cout << endl;
  cout << "Options:" << endl;
  cout << "  -o <output_file>  Output file name, default is 'output.ppm'" << endl;
  cout << "  -t <type>         Interpolation type (nearest, bilinear), default is 'nearest'" << endl;
  cout << "  -sx <scale_x>     Scale factor in x, default is 2" << endl;
  cout << "  -sy <scale_y>     Scale factor in y, default is 2" << endl;
  cout << "  -c                Compare the output with the original image, default is false" << endl;
}

map<string, string> util::parseArgs(int argc, char *argv[])
{
  map<string, string> args;

  for (int i = 1; i < argc; i++)
  {
    if (argv[i][0] == '-')
    {
      if (i + 1 < argc && argv[i + 1][0] != '-')
      {
        args[argv[i] + 1] = argv[i + 1];
        i++;
      }
      else
      {
        args[argv[i] + 1] = "true";
      }
    }
  }

  // Set default values
  args["o"] = args["o"].empty() ? "output.ppm" : args["o"];
  args["t"] = args["t"].empty() ? "nearest" : args["t"];
  args["sx"] = args["sx"].empty() ? "2" : args["sx"];
  args["sy"] = args["sy"].empty() ? "2" : args["sy"];
  args["c"] = args["c"].empty() ? "false" : args["c"];

  return args;
}

void util::initOpenGL()
{
  int argc = 1;
  char *argv[1] = {(char *)""};
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
}

void util::initLoop()
{
  glutMainLoop();
}

string util::Color::toString()
{
  return "(" + to_string((int)r) + ", " + to_string((int)g) + ", " + to_string((int)b) + ")";
}

util::Image::Image(string filename)
{
  ifstream file(filename, ios::binary);
  if (!file.is_open())
  {
    cout << "Error opening file: " << filename << endl;
    exit(1);
  }

  this->name = filename;

  // Read image header
  string header;
  file >> header;
  if (header != "P6")
  {
    cout << "Invalid image format" << endl;
    exit(1);
  }

  file >> this->width >> this->height;
  file >> this->max_color;

  // Read image data
  file.ignore(1);

  this->pixels = (Color **)malloc(this->height * sizeof(Color *));
  for (int i = 0; i < this->height; i++)
  {
    this->pixels[i] = (Color *)malloc(this->width * sizeof(Color));
    file.read((char *)this->pixels[i], this->width * sizeof(Color));
  }

  file.close();
}

util::Image util::Image::nearestNeighbor(float scale_x, float scale_y)
{
  Image new_image;
  new_image.name = this->name;
  new_image.width = this->width * scale_x;
  new_image.height = this->height * scale_y;
  new_image.max_color = this->max_color;

  // Copy pixels to new image
  new_image.pixels = (Color **)malloc(new_image.height * sizeof(Color *));
  for (int i = 0; i < new_image.height; i++)
  {
    new_image.pixels[i] = (Color *)malloc(new_image.width * sizeof(Color));
    for (int j = 0; j < new_image.width; j++)
    {
      int x = min(ceil(j / scale_x), this->width - 1);
      int y = min(ceil(i / scale_y), this->height - 1);
      new_image.pixels[i][j] = this->pixels[y][x];
    }
  }

  return new_image;
}

util::Image util::Image::bilinear(float scale_x, float scale_y)
{
  Image new_image;
  new_image.name = this->name;
  new_image.width = this->width * scale_x;
  new_image.height = this->height * scale_y;
  new_image.max_color = this->max_color;

  // Copy pixels to new image
  new_image.pixels = (Color **)malloc(new_image.height * sizeof(Color *));
  for (int i = 0; i < new_image.height; i++)
  {
    new_image.pixels[i] = (Color *)malloc(new_image.width * sizeof(Color));
    for (int j = 0; j < new_image.width; j++)
    {
      float x = j / scale_x;
      float y = i / scale_y;
      int x0 = floor(x);
      int y0 = floor(y);
      int x1 = min(ceil(x), this->width - 1);
      int y1 = min(ceil(y), this->height - 1);

      float dx = x - x0;
      float dy = y - y0;

      Color c00 = this->pixels[y0][x0];
      Color c01 = this->pixels[y0][x1];
      Color c10 = this->pixels[y1][x0];
      Color c11 = this->pixels[y1][x1];

      Color c = {
          static_cast<unsigned char>((1 - dx) * (1 - dy) * c00.r + dx * (1 - dy) * c01.r + (1 - dx) * dy * c10.r + dx * dy * c11.r),
          static_cast<unsigned char>((1 - dx) * (1 - dy) * c00.g + dx * (1 - dy) * c01.g + (1 - dx) * dy * c10.g + dx * dy * c11.g),
          static_cast<unsigned char>((1 - dx) * (1 - dy) * c00.b + dx * (1 - dy) * c01.b + (1 - dx) * dy * c10.b + dx * dy * c11.b)};

      new_image.pixels[i][j] = c;
    }
  }

  return new_image;
}

void util::Image::save(string filename)
{
  ofstream file(filename);

  file << "P6" << endl;
  file << this->width << " " << this->height << endl;
  file << this->max_color << endl;

  for (int i = 0; i < this->height; i++)
    for (int j = 0; j < this->width; j++)
      file.write((char *)&this->pixels[i][j], 3);

  file.close();
}

void util::Image::show(string title, int x, int y)
{
  glutInitWindowSize(this->width, this->height);
  glutCreateWindow(title.c_str());
  glutPositionWindow(x, y);

  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, this->width, 0, this->height);

  unsigned char *pixels = (unsigned char *)malloc(this->width * this->height * 3);
  for (int i = 0; i < this->height; i++)
    for (int j = 0; j < this->width; j++)
    {
      pixels[((this->height - 1 - i) * this->width + j) * 3 + 0] = this->pixels[i][j].r;
      pixels[((this->height - 1 - i) * this->width + j) * 3 + 1] = this->pixels[i][j].g;
      pixels[((this->height - 1 - i) * this->width + j) * 3 + 2] = this->pixels[i][j].b;
    }

  glRasterPos2i(0, 0);
  glDrawPixels(this->width, this->height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
  glFlush();

  glutDisplayFunc([]() {});
  glFlush();
}

void util::Image::show()
{
  show(this->name, 100, 100);
}