#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define sqr(x) ((x)*(x))
#define MAX_CLUSTERS 10
#define MAX_ITERATIONS 200
#define BIG_double (INFINITY)


/**
 * Calculates Euclidian Distance
 * */
double euclidian_distance(int dim, double *p1, double *p2){
    double d = 0;
    
    for (int i = 0; i < dim; i++)
      d += sqr(p1[i] - p2[i]);

    return d;  
  }

/**
 * On failure, print error message and exit
 * */
void onFail(char *str){
    printf(str);
    exit(-1);
  }
 
/**
 * Calculates distance between all data points and centroid in each cluster
 * */
void get_dist(int dim, int n, int k, double *data, double *centre, double *res){
    for (int i = 0; i < n; i++) // for each point in data
      for (int j = 0; j < k; j++) // for each cluster
        {
         // calculate distance between point and cluster centroid
          res[i*k + j] = euclidian_distance(dim, &data[i*dim], &centre[j*dim]);
        }
  }
  
/**
 * Calculates Total Distance
 * */
double get_total_dist(int dim, int n, int k, double *data, double *centroids, int *cluster_assignment_index){
    double total_dist = 0;
    
   // for every point
    for (int i = 0; i < n; i++){
       // which cluster is it in?
        int cluster = cluster_assignment_index[i];
       // sum distance
        if (cluster != -1)
          total_dist += euclidian_distance(dim, &data[i*dim], &centroids[cluster*dim]);
      }
      
    return total_dist;
  }

/**
 * Assigns points to clusters
 * */
void assign_cluster(int dim, int n, int k, double *dist_array, int *cluster_index){
   // for each point
    for (int i = 0; i < n; i++){
        int best_index = -1;
        double closest_distance = BIG_double;
        
       // for each cluster
        for (int j = 0; j < k; j++){
           // distance between point and cluster centroid
           
            double cur_distance = dist_array[i*k + j];
			// find min distance
            if (cur_distance < closest_distance){
                best_index = j;
                closest_distance = cur_distance;
              }
          }

       // assign it to the min distance cluster
        cluster_index[i] = best_index;
      }
  }

/**
 *  Calculate the mean of new clusters to find the new centroids
 * */
void update_centroid(int dim, int n, int k, double *data, int *cluster_assignment_index, double *new_cluster_centroid){
    int cluster_size[MAX_CLUSTERS];
  
   // initialize cluster centroid coordinate sums to zero
    for (int i = 0; i < k; i++) {
        cluster_size[i] = 0;
        
        for (int j = 0; j < dim; j++)
          new_cluster_centroid[i*dim + j] = 0;
     }

   // sum all points
   // for every point
    for (int i = 0; i < n; i++) {
       // which cluster is it in?
        int cluster = cluster_assignment_index[i];

       // update count of members in that cluster
        cluster_size[cluster]++;
        
       // sum point coordinates for finding centroid
        for (int j = 0; j < dim; j++)
          new_cluster_centroid[cluster*dim + j] += data[i*dim + j];
      }
      
      
   // now divide each coordinate sum by number of members to find mean/centroid
   // for each cluster
    for (int i = 0; i < k; i++) {
        if (cluster_size[i] == 0)
          printf("WARNING: Empty cluster %d! \n", i);
          
       // for each dimension
        for (int j = 0; j < dim; j++)
          new_cluster_centroid[i*dim + j] /= cluster_size[i];  /// XXXX will divide by zero here for any empty clusters!

      }
  }

/**
 * Get number of elements in each cluster
 * */
void getClusterSize(int n, int k, int *cluster_assignment_index, int *cluster_member_count){
   // initialize cluster member counts
    for (int i = 0; i < k; i++) 
      cluster_member_count[i] = 0;
  
   // count members of each cluster    
    for (int i = 0; i < n; i++)
      cluster_member_count[cluster_assignment_index[i]]++;
  }

/**
 * Print number of elements in each cluster and the cluster centroids
 * */  
void display_results(int dim, int n, int k, double *X, int *cluster_assignment_index, double *cluster_centroid){
    int cluster_member_count[MAX_CLUSTERS];
    
    getClusterSize(n, k, cluster_assignment_index, cluster_member_count);
     
    printf("  Final clusters \n");
    for (int i = 0; i < k; i++) 
      printf("    cluster %d:     members: %8d, centroid (%.1f) \n", i, cluster_member_count[i], cluster_centroid[i*dim + 0]);
  }

void copy_assignment_array(int n, int *src, int *tgt){
	int i=0;
    for (i = 0; i < n; i++)
      tgt[i] = src[i];
  }
  
int assignment_change_count(int n, int a[], int b[]){
    int change_count = 0;

    for (int ii = 0; ii < n; ii++)
      if (a[ii] != b[ii])
        change_count++;
        
    return change_count;
  }

/**
 * Performs image segmentation by running kmeans algorithm
 * */
  int*  kmeans(int dim, double *X, int n, int k, double *cluster_centroid){

	// intialize arrays
    double *dist                    = (double *)malloc(sizeof(double) * n * k);
    int   *cluster_assignment_cur  = (int *)malloc(sizeof(int) * n);
    int   *cluster_assignment_prev = (int *)malloc(sizeof(int) * n);
    double *point_move_score        = (double *)malloc(sizeof(double) * n * k);
	
    // Catch errors
    if (!dist || !cluster_assignment_cur || !cluster_assignment_prev || !point_move_score){
		onFail("Error Allocating Arrays");
	}
      

   // Calculate distance between each centroid and each data point
    get_dist(dim, n, k, X, cluster_centroid, dist);
    
	// Assign points to clusters by taking the min distance
	assign_cluster(dim, n, k, dist, cluster_assignment_cur);
    
	// Update cluster array
	copy_assignment_array(n, cluster_assignment_cur, cluster_assignment_prev);

   // Iterations
    double prev_totD = BIG_double;
    int batch_iteration = 0;
	printf("iter  %u      change        Tot_Dist            totD-prev_totD\n", 1);
    while (batch_iteration < MAX_ITERATIONS){

        // update cluster centroids
         update_centroid(dim, n, k, X, cluster_assignment_cur, cluster_centroid);

        // see if we've failed to improve
         double totD = get_total_dist(dim, n, k, X, cluster_centroid, cluster_assignment_cur);
         if (totD > prev_totD)
          // failed to improve - currently solution worse than previous
           {
            // restore old assignments
             copy_assignment_array(n, cluster_assignment_prev, cluster_assignment_cur);
             
            // recalc centroids
             update_centroid(dim, n, k, X, cluster_assignment_cur, cluster_centroid);
             
             printf("  negative progress made on this step - iteration completed (%.2f) \n", totD - prev_totD);
             
            // done with this phase
             break;
           }
           
        // save previous step
         copy_assignment_array(n, cluster_assignment_cur, cluster_assignment_prev);
         
        // move all points to nearest cluster
         get_dist(dim, n, k, X, cluster_centroid, dist);
         assign_cluster(dim, n, k, dist, cluster_assignment_cur);
         
         int change_count = assignment_change_count(n, cluster_assignment_cur, cluster_assignment_prev);
         
         printf("%3d   %u   %9d  %16.2f %17.2f\n", batch_iteration, 1, change_count, totD, totD - prev_totD);
         fflush(stdout);
         
        // done with this phase if nothing has changed
         if (change_count == 0)
           {
             printf("  No more changes - iteration completed \n");
             break;
           }

         prev_totD = totD;
                        
         batch_iteration++;
      }

	display_results(dim, n, k, X, cluster_assignment_cur, cluster_centroid);

    free(dist);
    //free(cluster_assignment_cur);
    free(cluster_assignment_prev);
    free(point_move_score);

	return cluster_assignment_cur;
  }           

   


