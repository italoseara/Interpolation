# Interpolation

Homework from CG class. Scale images according to a given scale factor using multiple interpolation methods.

## Usage

```bash
make
./bin/main <input_image>
```

`<input_image>` is the path to the image you want to scale.
`-o <output_image>` is the path to the output image. If not provided, the output image will be saved as `output.png`.
`-t <type>` is the interpolation type. It can be `nearest`, `bilinear`, `bicubic` or `lanczos`. The default is `nearest`.
`-sx <scale_x>` is the scale factor in the x-axis. The default is 2.
`-sy <scale_y>` is the scale factor in the y-axis. The default is 2.
`-c` is to compare the output image with the original image. The default is false.

## Author

- [Italo Seara](https://github.com/italoseara)
