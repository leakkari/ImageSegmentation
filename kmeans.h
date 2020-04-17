// kmeans.h


extern int* kmeans(
            int  dim,		                   // dimension of data (if grayscale, dim=1)

            double *X,                        // pointer to data
            int   n,                         // size of data (width*height*dim of image)
            
            int   k,                         // number of clusters
            double *cluster_centroid         // initial cluster centroids (can be randomly initialized)
         
           );
