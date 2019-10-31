# image_repub
ROS package that implements the 16-bit depth image compression algorithm, RVL. 

It allows for use of 16-bit and 32-bit depth images, altohu, in the latter case, inpu images are converted to 16-bit before being compressed (loss of information occurs).

NOTE: In September 2019, ROS added the RVL compression technique to the compressed_image_trasnport package. We reccomend its usage since it is available as a plugin, rather than an independent package like this repo, making it simpler to use.
