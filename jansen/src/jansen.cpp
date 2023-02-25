// Markus Buchholz, 2023

#include <stdafx.hpp>
#include "imgui_helper.hpp"
#include "ball.h"
#include <tuple>
#include <thread>
#include <chrono>
#include <vector>
#include <math.h>

//---------------------------------------------------------------
std::vector<Point> points = {
	{{400, 300}, 10}};

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

std::tuple<std::vector<float>, std::vector<float>, std::vector<float>, std::vector<float>, std::vector<float>, std::vector<float>, std::vector<float>, std::vector<float>, std::vector<float>, std::vector<float>, std::vector<float>, std::vector<float>, std::vector<float>, std::vector<float>, std::vector<float>, std::vector<float>> simulation()
{

	std::vector<float> a_x;
	std::vector<float> a_y;

	std::vector<float> b_x;
	std::vector<float> b_y;

	std::vector<float> c_x;
	std::vector<float> c_y;

	std::vector<float> d_x;
	std::vector<float> d_y;

	std::vector<float> e_x;
	std::vector<float> e_y;

	std::vector<float> f_x;
	std::vector<float> f_y;

	std::vector<float> g_x;
	std::vector<float> g_y;

	std::vector<float> o_x;
	std::vector<float> o_y;

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

	std::vector<float> time;

	for (int i = 0; i < 3; i++)
	{

		for (float t = 0 / 3; t < 2 * M_PI; t = t + dt)
		{
			time.push_back(t);
		}
	}

	for (auto &t : time)
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
		Gx = Ex + link_i * std::cos(epsilon + zeta + eta);
		Gy = Ey + link_i * sin(epsilon + zeta + eta);

		g_x.push_back(Gx);
		g_y.push_back(Gy);
		a_x.push_back(Ax);
		a_y.push_back(Ay);
		b_x.push_back(Bx);
		b_y.push_back(By);
		c_x.push_back(Cx);
		c_y.push_back(Cy);
		d_x.push_back(Dx);
		d_y.push_back(Dy);
		e_x.push_back(Ex);
		e_y.push_back(Ey);
		f_x.push_back(Fx);
		f_y.push_back(Fy);
		o_x.push_back(Ox);
		o_y.push_back(Oy);
	}
	return std::make_tuple(g_x, g_y, a_x, a_y, b_x, b_y, c_x, c_y, d_x, d_y, e_x, e_y, f_x, f_y, o_x, o_y);
}

int main(int argc, char const *argv[])
{

	int w = 1280;
	int h = 720;
	float scale_x = 3.0;
	float scale_y = 3.0;
	std::string title = "Bouncing ball";
	initImgui(w, h, title);

	// simulation state
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(89.0f / 255.0, 88.0f / 255.0, 87.0f / 255.0, 1.00f);
	ImVec4 ray_color = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);

	// compute ball path
	auto sim = simulation();

	// Main loop
	int ii = 0;
	bool flag = true;
	std::vector<ImVec2> tail;
	std::vector<ImVec2> tail_circle;
	std::vector<ImVec2> tail_arc;

	int LOOPS = 5;

	while (!glfwWindowShouldClose(window) && flag == true)
	{
		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		//----------------------------------------

		ImGui::NewFrame();

		ImGuiWindowFlags window_flags = 0;
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(w, h), ImGuiCond_FirstUseEver);
		window_flags |= ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoResize;
		window_flags |= ImGuiWindowFlags_NoCollapse;
		window_flags |= ImGuiWindowFlags_NoBackground;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

		ImGui::Begin("Simulation", nullptr, window_flags);
		ImDrawList *draw_list = ImGui::GetWindowDrawList();

		ImVec2 G = {(float)points[0].position.x + std::get<0>(sim)[ii] * scale_x, (float)points[0].position.y - std::get<1>(sim)[ii] * scale_y};

		ImVec2 A = {(float)points[0].position.x + std::get<2>(sim)[ii] * scale_x, (float)points[0].position.y - std::get<3>(sim)[ii] * scale_y};

		ImVec2 B = {(float)points[0].position.x + std::get<4>(sim)[ii] * scale_x, (float)points[0].position.y - std::get<5>(sim)[ii] * scale_y};
		ImVec2 C = {(float)points[0].position.x + std::get<6>(sim)[ii] * scale_x, (float)points[0].position.y - std::get<7>(sim)[ii] * scale_y};
		ImVec2 D = {(float)points[0].position.x + std::get<8>(sim)[ii] * scale_x, (float)points[0].position.y - std::get<9>(sim)[ii] * scale_y};
		ImVec2 E = {(float)points[0].position.x + std::get<10>(sim)[ii] * scale_x, (float)points[0].position.y - std::get<11>(sim)[ii] * scale_y};
		ImVec2 F = {(float)points[0].position.x + std::get<12>(sim)[ii] * scale_x, (float)points[0].position.y - std::get<13>(sim)[ii] * scale_y};
		ImVec2 O = {(float)points[0].position.x + std::get<14>(sim)[ii] * scale_x, (float)points[0].position.y - std::get<15>(sim)[ii] * scale_y};

		float radius = 5;
		float size_line = 3;

		draw_list->AddCircleFilled(G, radius, ImColor(ray_color));

		draw_list->AddLine(C, D, ImColor(ray_color), 2.0);
		draw_list->AddLine(C, A, ImColor(ray_color), 2.0);
		draw_list->AddLine(C, B, ImColor(ray_color), 2.0);
		draw_list->AddLine(D, F, ImColor(ray_color), 2.0);
		draw_list->AddLine(D, B, ImColor(ray_color), 2.0);
		draw_list->AddLine(B, E, ImColor(ray_color), 2.0);
		draw_list->AddLine(F, E, ImColor(ray_color), 2.0);
		draw_list->AddLine(A, E, ImColor(ray_color), 2.0);
		draw_list->AddLine(F, G, ImColor(ray_color), 2.0);
		draw_list->AddLine(E, G, ImColor(ray_color), 2.0);
		draw_list->AddLine(A, O, ImColor(ray_color), 2.0);

		tail.push_back(G);
		tail_circle.push_back(A);

		for (int ii = 0; ii < tail.size(); ii++)
		{
			draw_list->AddCircleFilled(tail[ii], 0.5, ImColor(ray_color));
			draw_list->AddCircleFilled(tail_circle[ii], 0.5, ImColor(ray_color));
		}

		ii++;
		if (ii >= std::get<0>(sim).size())
		{
			flag = false;
		}
		ImGui::End();

		//----------------------------------------

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	termImgui();
	return 0;
}
