import os
import imageio

png_dir = os.getcwd()+'/output/'
images = []
for file_name in sorted(os.listdir(png_dir)):
    if file_name.endswith('.jpg'):
        file_path = os.path.join(png_dir, file_name)
        images.append(imageio.imread(file_path))
imageio.mimsave('finalOutput/TimeLapse.gif', images,fps=1.5)