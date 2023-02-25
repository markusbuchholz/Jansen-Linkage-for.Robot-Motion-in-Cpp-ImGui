// Markus Buchholz. 2023
// g++ jansen_link.cpp -o t -I/usr/include/python3.8 -lpython3.8

#include <iostream>
#include <tuple>
#include <vector>
#include <math.h>
#include <cmath>
#include <iomanip>
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

//----------- system dynamic parameters --------------------

float link_a = 38.0;
float link_b = 41.5;
float link_c = 39.3;
float link_d = 40.1;
float link_e = 55.8;
float link_f = 39.4;
float link_g = 36.7;
float link_h = 65.7;
float link_i = 49.0;
float link_j = 50.0;
float link_k = 61.9;
float link_l = 7.8;
float link_m = 15.0;

//---------------------------------------------------------------

void plot2D(std::vector<float> X, std::vector<float> Y)
{

    plt::title("Jansen linkage");
    plt::named_plot("path of linkage", X, Y);
    //plt::named_plot("rotation", arcX, arcY);
    plt::xlabel("pos X");
    plt::ylabel("pos Y");
    plt::legend();
    plt::xlabel("pos X");
    plt::ylabel("pos Y");
    plt::show();
}

//---------------------------------------------------------------

std::tuple<std::vector<float>, std::vector<float>> simulation()
{
    
    std::vector<float> x;
    std::vector<float> y;

    float dt = M_PI / 180.0;
    float Ox = 0.0;
    float Oy = 0.0;
    float Bx = -link_a;
    float By = -link_l;

    float gamma = std::acos((link_b * link_b + link_d * link_d - link_e * link_e) / (2 * link_b * link_d));
    float eta = std::acos((link_g * link_g + link_i * link_i - link_h * link_h) / (2 * link_g * link_i));

    float Ax = 0.0;
    float Ay = 0.0;
    float Dx = 0.0;
    float Dy = 0.0;
    float Cx = 0.0;
    float Cy = 0.0;
    float Ex = 0.0;
    float Ey = 0.0;
    float Fx = 0.0;
    float Fy = 0.0;
    float Gx = 0.0;
    float Gy = 0.0;



    for (float t = 0; t < 2 * M_PI ; t = t + dt)
    {

        //------------------------

        Ax = link_m * std::cos(t);
        Ay = link_m * std::sin(t);

        //------------------------

        float AB = std::sqrt((Ax - Bx) * (Ax - Bx) + (Ay - By) * (Ay - By));
        float alpha = std::atan2(Ay - By, Ax - Bx);
        float beta = std::acos((AB * AB + link_b * link_b - link_j * link_j) / (2 * AB * link_b));
        Cx = Bx + link_b * std::cos(alpha + beta);
        Cy = By + link_b * std::sin(alpha + beta);

        //------------------------

        Dx = Bx + link_d * std::cos(alpha + beta + gamma);
        Dy = By + link_d * std::sin(alpha + beta + gamma);

        //------------------------

        float delta = std::acos((AB * AB + link_c * link_c - link_k * link_k) / (2 * AB * link_c));
        Ex = Bx + link_c * std::cos(alpha - delta);
        Ey = By + link_c * std::sin(alpha - delta);

        //------------------------
        float DE = std::sqrt((Ex - Dx) * (Ex - Dx) + (Ey - Dy) * (Ey - Dy));
        float epsilon = std::atan2(Dy - Ey, Dx - Ex);
        float zeta = std::acos((DE * DE + link_g * link_g - link_f * link_f) / (2 * DE * link_g));
        Fx = Ex + link_g * std::cos(epsilon + zeta);
        Fy = Ey + link_g * sin(epsilon + zeta);

        //------------------------
        Gx = Ex + link_i*std::cos(epsilon+zeta+eta);
        Gy = Ey+ link_i*sin(epsilon+zeta+eta); 

        x.push_back(Gx);
        y.push_back(Gy);

    }
    return std::make_tuple(x, y);
}

int main()
{
    auto sim = simulation();
    plot2D(std::get<0>(sim), std::get<1>(sim));
}
