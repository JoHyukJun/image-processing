import os
import cv2
import numpy as np

from pathlib import Path


# basic settings.
BASE_DIR = Path(__file__).resolve(strict=True).parent.parent

print('base directory: {}'.format(BASE_DIR))
print('opencv version: {}'.format(cv2.__version__))

# numpy settings.
np.set_printoptions(threshold=np.inf, linewidth=np.inf)

# imread path value must be string type.
input_file_path = str(BASE_DIR / 'images/test.jpg')

# original image reading process.
original_image = cv2.imread(input_file_path, cv2.IMREAD_UNCHANGED)

print('original image resolution: {}'.format(original_image.shape))

# basic upscaling image.
upsacaling_image = cv2.resize(original_image, dsize=(0 ,0), fx=4.0, fy=4.0, interpolation=cv2.INTER_LINEAR)

print('upsacling image resolution: {}'.format(upsacaling_image.shape))

# gaussian filtering.
kernel_size = (5, 5)
upscaling_image = cv2.GaussianBlur(upsacaling_image, kernel_size, 0)

# canny edge detection.
under_threshold = 50
over_threshold = 100
edges = None
apetureSize = None # sobel kernel size.
L2gradient = None
ced_image = cv2.Canny(upsacaling_image, under_threshold, over_threshold, edges, apetureSize, L2gradient)


# write image.
output_file_path = str(BASE_DIR / 'images/test_output.jpg')
cv2.imwrite(output_file_path, upsacaling_image)

cv2.namedWindow('test', cv2.WINDOW_AUTOSIZE)
cv2.imshow('test', upsacaling_image)
cv2.waitKey(0)
cv2.destroyAllWindows()