import os
from PIL import Image
import re
import sys

# palette_dodge = {
#     "black": {"r":   0, "g":   0, "b":   0},
#     "white": {"r": 255, "g": 255, "b": 255},
#     "green": {"r":  67, "g": 138, "b":  28},
#     "purple":{"r": 100, "g":  64, "b": 255},
#     "red":   {"r": 191, "g":   0, "b":   0},
#     "yellow":{"r": 255, "g": 243, "b":  56},
#     "violet":{"r": 194, "g": 164, "b": 244}
# }

def main():
    if len(sys.argv) < 2:
        print("Error: enter a path and name")
    else:
        # Test args
        n = len(sys.argv)
        print("Total arguments passed:", n)
        print("\nName of Python script:", sys.argv[0])
        print("\nArguments passed:", end = " ")
        for i in range(1, n):
            print(sys.argv[i], end = " ")
        print("")

        # Convert image
        img_path = os.path.abspath(sys.argv[1])
        img_name = sys.argv[2]
        quantize_image(img_path, img_name)

def quantize_image(path, name):
    img_path = os.path.join(path, name + ".bmp")
    print(f"img_path: {img_path}")
    with Image.open(img_path) as img:
        pixelMap = img.load()
        for i in range(img.size[0]):
            for j in range(img.size[1]):
                px = pixelMap[i, j]
                pixelMap[i, j] = quantize_pixel(px)
        new_name = name + "_quantized"
        img_path_quantized = os.path.join(path, new_name + ".bmp")
        print(f"img_path_quantized: {img_path_quantized}")
        img.save(img_path_quantized)
        img.close()
            
def quantize_pixel(px):
    palette = (
        (  0,   0,   0, 255), # black
        (255, 255, 255, 255), # white
        (255,   0,   0, 255), # red  
        (  0, 255,   0, 255), # green
        (  0,   0, 255, 255), # blue
        (255, 255,   0, 255), # yellow
        (255, 128,   0, 255)  # orange
    )

    min_color_diff = 100000000
    best_color = palette[0]
    
    for color in palette:
        diff_r = px[0] - color[0]
        diff_g = px[1] - color[1]
        diff_b = px[2] - color[2]

        color_diff = (diff_r * diff_r) + (diff_g * diff_g) + (diff_b * diff_b)
        # print(f"color: {color} color_diff: {color_diff}")
        
        if color_diff < min_color_diff:
            min_color_diff = color_diff
            best_color = color

    # print(f"px color = {px} quantized color = {best_color}")
    return best_color

main()