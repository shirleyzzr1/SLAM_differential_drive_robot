#include <iostream>
#include "turtlelib/circle.hpp"
#include "turtlelib/rigid2d.hpp"
#include <string>
#include <cmath>
#include <armadillo>

namespace turtlelib{
    void Circle::range_cluster(std::vector<double> ranges,double thresh_dist){
        std::vector<double> cur_cluster;
        std::vector<Vector2D> cur_cluster_xy;
        double angle_increment = 0.0174;
        cur_cluster_xy.push_back({ranges[0]*cos(0),ranges[0]*sin(0)});
        cur_cluster.push_back(0);
        for(int i=1;i<ranges.size();i++){
            if(abs(ranges[i]-ranges[i-1])<=thresh_dist){
                cur_cluster.push_back(i);
                cur_cluster_xy.push_back({ranges[i]*cos(i*angle_increment),ranges[i]*sin(i*angle_increment)});
            }
            else{
                //only take clusters larger than 3 points
                if(cur_cluster.size()>3) {
                    clusters.push_back(cur_cluster);
                    xy_clusters.push_back(cur_cluster_xy);
                }
                //start new cluster search
                cur_cluster.clear();
                cur_cluster_xy.clear();
                cur_cluster.push_back(i);
                cur_cluster_xy.push_back({ranges[i]*cos(i*angle_increment),ranges[i]*sin(i*angle_increment)});
            }
        }
        clusters.push_back(cur_cluster);
        xy_clusters.push_back(cur_cluster_xy);
        //check if the start and end belongs to same cluster
        if ((cur_cluster[cur_cluster.size()-1]==(ranges.size()-1))&&(clusters[0][0]==1)
                &&(abs(ranges[0]-ranges[ranges.size()-1])<=thresh_dist))
        {   
            //insert the last cluster to the beginning of the first cluster
            clusters[0].insert(std::begin(clusters[0]), std::begin(cur_cluster), std::end(cur_cluster));
            xy_clusters[0].insert(std::begin(xy_clusters[0]), std::begin(cur_cluster_xy), std::end(cur_cluster_xy));
            clusters.pop_back();
            xy_clusters.pop_back();
        }
    }
    void Circle::circle_fitting(){
        for(int i=0;i<xy_clusters.size();i++){
            Vector2D xy_sum = {0,0};
            int n = xy_clusters[i].size();
            for(int j=0;j<n;j++){
                xy_sum += xy_clusters[i][j];
            }
            Vector2D xy_mean = xy_sum*(1/n);
            arma::mat Z = arma::zeros<arma::mat>(n,4);
            double z_mean = 0;
            for(int j=0;j<n;j++){
                Vector2D xy = xy_clusters[i][j]-xy_mean; 
                Z(j,0) = xy.x*xy.x+xy.y*xy.y;
                Z(j,1) = xy.x;
                Z(j,2) = xy.y;
                Z(j,3) = 1;
            }
            arma::rowvec row_sum = arma::sum(Z,0);
            z_mean = row_sum(0)/n;
            arma::mat M = (Z.t()*Z)/n;

            arma::mat H_inv = arma::zeros<arma::mat>(4,4);
            H_inv(0,3) = 0.5;
            H_inv(3,0) = 0.5;
            H_inv(3,3) = -2*z_mean;

            

        }
    }
    void Circle::classification(){
        for(int i=0;i<xy_clusters.size();i++){
            std::vector<double>angles;
            double sum_angle = 0;
            for(int j=1;j<xy_clusters[i].size()-1;j++){
                //compute the angle between lines (P_random,P_first) (P_random,Plast)
                double p_angle = angle(xy_clusters[i][j]-xy_clusters[i][0],\
                            xy_clusters[i][j]-xy_clusters[i][xy_clusters[i].size()-1]);
                angles.push_back(p_angle);
                sum_angle+=p_angle;
            }
            //compute mean and standard derivation of the angles
            for(int j=0;j<angles.size();j++){
                
            }
        }

    }


}