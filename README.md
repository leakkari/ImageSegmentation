# ImageSegmentation

To run :

1. Git clone repo
```console
git https://github.com/leakkari/ImageSegmentation.git
```


2. Compile project
```console
gcc kmeans_tst.c -lm
```

3. Run k-means on the image
```console
./a.out
```
This runs k-means on the image "image2.jpg", and saves the output of the k-means algorithm in "data.txt". To plot the segmented image we use the a python script.

4. Plot Segmented Image and Cluster 
```console
python3 plot
```
The plots are saved in the /Plots Directory. 
