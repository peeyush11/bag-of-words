"""
The purpose of this script is to generate test data for a bag of visual words approach
from the COIL-100 dataset. It randomly selects objects from the dataset and places them
at random positions with random orientation and scale within certain limits.

The COIL-100 datset consists of images of 100 objects each viewed from 72 different angles.

Reference: http://www.cs.columbia.edu/CAVE/software/softlib/coil-100.php
"""

import cv2
import numpy as np
from pathlib import Path
import shutil


if __name__=='__main__':
  coil_dir = Path('../../data/coil-100/') # where the COIL-100 images are located
  output_dir = Path('../../data/test-1000/') # where to put the generated test data
  image_output_dir = output_dir/Path('images') # put images in subdir to meet file structure used in project

  # parameters
  source_size = 128 # size of the COIL-100 images
  output_size = 2*256 # desired size of test images times two

  num_images = 1000 # number of test images to generate
  num_obj_per_image = 3 # number of objects per test image

  min_distance_from_border = 64 # minimum distance of object centers to border

  min_scale_factor = 0.75 # scaling of objects
  max_scale_factor = 1.5

  background_value = 30 # intensity of background

  # obj_indices = np.array([1, 31, 10, 43, 15, 23]) # object indicies to be used (minimum 1, maximum 100)
  obj_indices = np.arange(1, 26)
  # angles = np.array([30]) # orientations available by COIL-100 are 0 to 355 in steps of 5 deg
  angles = np.arange(0, 360, 5)

  # clean previous output
  if output_dir.exists():
    shutil.rmtree(str(output_dir))
  output_dir.mkdir()
  image_output_dir.mkdir()

  # maximum size of an object patch from COIL-100 after random scaling and rotation
  max_size_after_transformation = int(np.ceil(max_scale_factor*np.sqrt(2.0*source_size*source_size)))

  for image_index in range(num_images):
    print('Sample {}/{}.'.format(image_index, num_images))

    # image to place objects including some padding
    image = np.zeros((output_size+2*max_size_after_transformation,
                      output_size+2*max_size_after_transformation, 3), dtype=np.uint8)

    # choose gray background
    background_pixel = np.array([background_value, background_value, background_value], dtype=np.uint8)
    image[:, :] = background_pixel

    for obj_index in range(num_obj_per_image):
      # sample object
      obj_index = np.random.choice(obj_indices)

      # sample position
      pos_x = np.random.randint(min_distance_from_border, output_size-min_distance_from_border)
      pos_y = np.random.randint(min_distance_from_border, output_size-min_distance_from_border)

      # sample orientation in COIL
      angle = np.random.choice(angles)

      # lookup image in COIL-100
      source_filename = 'obj{}__{}.png'.format(obj_index, angle)
      source_image = cv2.imread(str(coil_dir/Path(source_filename)), cv2.IMREAD_COLOR)

      # sample scaling and rotation to apply
      scale_factor = np.random.uniform(min_scale_factor, max_scale_factor)
      rotation_angle = np.random.uniform(0, 2.0*np.pi)

      # to transform object patch translate to center, rotate, scale, translate back
      translation1 = np.eye(3, dtype=np.float)
      translation1[0, 2] = -0.5*source_size
      translation1[1, 2] = -0.5*source_size

      rotation = np.eye(3, dtype=np.float)
      rotation[0, 0] = np.cos(rotation_angle)
      rotation[1, 1] = np.cos(rotation_angle)
      rotation[0, 1] = -np.sin(rotation_angle)
      rotation[1, 0] = np.sin(rotation_angle)
      rotation[2, 2] = 1.0

      scaling = np.eye(3, dtype=np.float)
      scaling[0, 0] = scale_factor
      scaling[1, 1] = scale_factor
      scaling[0, 1] = 0.0
      scaling[1, 0] = 0.0
      scaling[2, 2] = 1.0

      translation2 = np.eye(3, dtype=np.float)
      translation2[0, 2] = 0.5*max_size_after_transformation
      translation2[1, 2] = 0.5*max_size_after_transformation

      combined = translation2\
                 @rotation\
                 @scaling\
                 @translation1

      transformed_source = cv2.warpAffine(source_image, combined[:2, :], (max_size_after_transformation, max_size_after_transformation))

      # boundaries of the object patch in output including padding
      left = pos_x-max_size_after_transformation//2+max_size_after_transformation
      right = left+max_size_after_transformation
      top = pos_y-max_size_after_transformation//2+max_size_after_transformation
      bottom = top+max_size_after_transformation

      # place patch
      mask = np.any(transformed_source>background_value, axis=-1)[..., None]
      image[top:bottom, left:right] = np.where(mask, transformed_source, image[top:bottom, left:right])

    # undo padding
    image = image[max_size_after_transformation:-max_size_after_transformation,
                  max_size_after_transformation:-max_size_after_transformation]

    # blur and rescale to desired output size
    image = cv2.GaussianBlur(image, (0, 0), sigmaX=1, sigmaY=1)
    image = cv2.resize(image, (256, 256))

    cv2.imwrite(str(image_output_dir/Path('sample_{:06d}.png'.format(image_index))), image)

