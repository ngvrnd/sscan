#!/usr/bin/env python3
"""Script to process all image files in a given path using the 'processImages.app' application."""

import os
import subprocess
import sys

def find_image_files(path):
    """Finds all image files in the given path."""
    image_extensions = ('.jpg', '.jpeg', '.png', '.bmp', '.gif')  # Add more if needed
    image_files = []
    for root, _, files in os.walk(path):
        for file in files:
            if file.lower().endswith(image_extensions):
                image_files.append(os.path.join(root, file))
    return image_files

def run_processImage_on_images(image_files):
    """Runs the 'processImage' application on each image file."""
    for image_file in image_files:
        try:
            # Run the processImage application with the image file as an argument, digging into the app bundle
            subprocess.run(["./processImage.app/Contents/MacOS/processImage", image_file], check=True)
        except subprocess.CalledProcessError as e:
            print(f"Error processing {image_file}: {e}")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: process_all [<path>]")
        sys.exit(1)

    path = sys.argv[1]
    image_files = find_image_files(path)
    run_processImage_on_images(image_files)