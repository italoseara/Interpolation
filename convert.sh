#!/bin/bash

# Check if the input file is provided
if [ -z "$1" ]; then
  echo "Usage: $0 <input_file> [output_folder]"
  exit 1
fi

# Check if ffmpeg is installed
if ! command -v ffmpeg; then
  echo "ffmpeg is not installed. Please install ffmpeg and try again."
  exit 1
fi

# Get the input file name and extension
input_file="$1"
output_folder="${2:-.}"
filename=$(basename -- "$input_file")
extension="${filename##*.}"

# Check if the input file exists
if [ ! -f "$input_file" ]; then
  echo "Input file does not exist."
  exit 1
fi

# Check if the input file is already in .ppm format
if [ "$extension" == "ppm" ]; then
  echo "Input file is already in .ppm format."
  exit 0
fi

# Convert the input file to .ppm format using ffmpeg
output_file="${output_folder}/${filename%.*}.ppm"
ffmpeg -i "$input_file" "$output_file"

# Check if the conversion was successful
if [ $? -eq 0 ]; then
  echo "File converted to .ppm format: $output_file"
else
  echo "Failed to convert file to .ppm format."
fi