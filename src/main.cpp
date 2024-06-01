/**
 * Computação Gráfica - CIC - 2024.1
 * Atividade 5 - Interpolação
 *
 * Aluno: Italo Santana Seara - 202220031
 */

#include <iostream>
#include "util.hpp"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

using namespace std;

int main(int argc, char *argv[])
{
  // Check for usage errors
  if (argc < 2 || argv[1][0] == '-')
  {
    util::printUsage(argv);
    return 1;
  }

  // Parse arguments
  map<string, string> args = util::parseArgs(argc, argv);
  string input_file = argv[1];
  string output_file = args["o"];
  string interpolation_type = args["t"];
  float scale_x = 0.f;
  float scale_y = 0.f;
  bool compare = args["c"] == "true";

  // Check for invalid arguments
  if (interpolation_type != "nearest" && interpolation_type != "bilinear")
  {
    cout << "Invalid interpolation type: \"" << interpolation_type << "\"" << endl;
    return 1;
  }

  // Check for invalid scale factors
  try
  {
    scale_x = stof(args["sx"]);
    scale_y = stof(args["sy"]);
  }
  catch (const invalid_argument &e)
  {
    cout << "Invalid scale factors, must be numbers" << endl;
    return 1;
  }

  if (scale_x <= 0 || scale_y <= 0)
  {
    cout << "Invalid scale factors, must be greater than 0" << endl;
    return 1;
  }

  util::Image image(input_file);

  cout << "Image " << input_file << " loaded" << endl;
  cout << "Image size: " << image.width << "x" << image.height << endl;
  cout << endl;

  // Interpolate image
  util::Image new_image;
  cout << "Interpolating using " << interpolation_type << endl;
  cout << "Scale factor: " << scale_x << "x" << scale_y << endl;
  cout << endl;

  if (interpolation_type == "nearest")
    new_image = image.nearestNeighbor(scale_x, scale_y);
  else if (interpolation_type == "bilinear")
    new_image = image.bilinear(scale_x, scale_y);

  new_image.save(output_file);

  cout << "New image size: " << new_image.width << "x" << new_image.height << endl;
  cout << "Image saved as: " << output_file << endl;

  // Show image
  if (compare)
  {
    cout << endl;
    cout << "Showing images side by side" << endl;

    util::initOpenGL();
    image.show("original", 200, 100);
    new_image.show(interpolation_type, image.width + 210, 100);
    util::initLoop();
  }
  return 0;
}