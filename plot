import matplotlib.pyplot as plt
import numpy as np
import cv2

image_segmented_path = ("./image2.jpeg")

class Plot():

    def __init__(self):
        image_segmented_path = "./image2.jpeg"

    def show_original(self):
        '''
        SHOW ORIGINAL IMAGE MEANT TO BE SEGMENTED
        '''

        img = cv2.imread(image_segmented_path)
        img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)

        plt.figure(figsize=(7, 7))
        plt.title("Original Image")
        plt.xticks([]), plt.yticks([])
        plt.imshow(img)
        #plt.show()

        plt.savefig('./Plots/Original_image.png')

        return img

    def show_segmented(self):
        '''
        SHOW SEGMENTED IMAGE FROM
        '''
        # Read data out-putted from C-file
        with open('data.txt', 'r') as f:
            data = f.read()
        # Str -> Int
        data_arr = [int(i) for i in data.split()]

        img = cv2.imread(image_segmented_path)
        h, w, ch = img.shape

        img_clustered = np.reshape(data_arr, ((h, w)))

        try:
            plt.figure(figsize=(7, 7))
            plt.title("Segmented Image")
            plt.imshow(img_clustered, cmap='gray')
            plt.xticks([]), plt.yticks([])
           # plt.show()
            plt.savefig('./Plots/Segmented_image.png')
        except Exception as e:
            print(e)


        return img_clustered
    pass

if __name__ == '__main__':
    plot = Plot()

    # Plot Original and segmented image
    original_img = plot.show_original()
    seg_img = plot.show_segmented()

    # Plot data and clusters from data.txt
    fig = plt.figure()
    plt.title("Pixel count in each group")
    ax = fig.add_axes([0, 0, 1, 1])
    groups = ['Group 1', 'Group 2', 'Group 3']
    group_counts = [227865, 229369,464366]
    ax.bar(groups, group_counts)
    # plt.show()
    plt.savefig('./Plots/Clusters_Plot.png')
