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


class ResolutionTransformation:
    # imread path value must be string type.
    input_file_path = str(BASE_DIR / 'images/test.jpg')
    output_file_path = str(BASE_DIR / 'images/test_output.jpg')


    def read(self):
        # original image reading process.
        self.original_image = cv2.imread(ResolutionTransformation.input_file_path, cv2.IMREAD_UNCHANGED)

        print('original image resolution: {}'.format(self.original_image.shape))


    def write(self):
        # write image.
        cv2.imwrite(ResolutionTransformation.output_file_path, self.upsacaling_image)


    def upsacaling(self):
        # basic upscaling image.
        self.upsacaling_image = cv2.resize(self.original_image, dsize=(0 ,0), fx=4.0, fy=4.0, interpolation=cv2.INTER_LINEAR)

        print('upsacling image resolution: {}'.format(self.upsacaling_image.shape))


    def gaussian_filtering(self):
        # gaussian filtering.
        kernel_size = (5, 5)
        self.upscaling_image = cv2.GaussianBlur(self.upsacaling_image, kernel_size, 0)


    def canny_edge_detection(self):
        # canny edge detection.
        under_threshold = 50
        over_threshold = 100
        edges = None
        apetureSize = None # sobel kernel size.
        L2gradient = None
        self.ced_image = cv2.Canny(self.upsacaling_image, under_threshold, over_threshold, edges, apetureSize, L2gradient)

    
    def show(self):
        # show result image.
        cv2.namedWindow('test', cv2.WINDOW_AUTOSIZE)
        cv2.imshow('test', self.upsacaling_image)
        cv2.waitKey(0)
        cv2.destroyAllWindows()


if __name__ == '__main__':
    test = ResolutionTransformation()
    test.read()
    test.upsacaling()
    test.gaussian_filtering()
    test.show()
