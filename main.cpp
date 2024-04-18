#include <cstdlib>
#include <imgui.h>
#include "GLFW/glfw3.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <iostream>
#include <vector>
#define PRECISION 0.035f

struct myPoint {
    float x;
    float y;
};

std::vector<myPoint> Arr;
std::vector<myPoint> result;

float xposToFloat(double xpos, int width)
{
    float meow = static_cast<float>((xpos / width - 0.5f) * 2);
    return meow;
}

float yposToFloat(double ypos, int height)
{
    float meow = static_cast<float>((ypos / height - 0.5f) * (-2));
    return meow;
}

int Fuctorial(int n)
{
    if (n < 1) return 1;
    if (n == 1) return n;
    else return Fuctorial(n - 1) * n;
}

float polinom(int i, int n, float t)
{
    return (Fuctorial(n) / (Fuctorial(i) * Fuctorial(n - i))) * (float)pow(t, i) * (float)pow(1 - t, n - i);
}

void Bize(std::vector<myPoint> Arr)
{
    int j = 0;
    float step = 0.01;
    float t = 0;
    while (t < 1)
    {
        float ytmp = 0;
        float xtmp = 0;
        for (size_t i = 0; i < Arr.size(); i++)
        {
            float b = polinom(i, Arr.size() - 1, t); 
            xtmp += Arr[i].x * b; 
            ytmp += Arr[i].y * b;
        }
        result.push_back(myPoint(xtmp, ytmp));
        j++;
        t += step;
    }
}

int FindPoint(std::vector<myPoint> Arr, float xpos_f, float ypos_f, float precision)
{
    for (int i = 0; i < Arr.size(); i++)
    {
        if ((Arr[i].x + precision >= xpos_f) &&
            (Arr[i].x - precision <= xpos_f) &&
            (Arr[i].y + precision >= ypos_f) &&
            (Arr[i].y - precision <= ypos_f))
        {
            return i;
        }
    }
    return -1;
}

bool is_on_imgui = 0;
bool put_points = 0;
bool move_points_f = 0;
bool move_points_s = 0;
int n_point = -1;

void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (is_on_imgui == 0)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && put_points)
        {
            double xpos, ypos;
            float xpos_f, ypos_f;
            int width, height;
            glfwGetCursorPos(window, &xpos, &ypos);
            glfwGetWindowSize(window, &width, &height);
            xpos_f = xposToFloat(xpos, width);
            ypos_f = yposToFloat(ypos, height);
            myPoint tmp = { xpos_f, ypos_f };
            Arr.push_back(tmp);
            return;
        }

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && move_points_f)
        {
            double xpos, ypos;
            float xpos_f, ypos_f;
            int width, height;
            glfwGetCursorPos(window, &xpos, &ypos);
            glfwGetWindowSize(window, &width, &height);
            xpos_f = xposToFloat(xpos, width);
            ypos_f = yposToFloat(ypos, height);
            n_point = FindPoint(Arr, xpos_f, ypos_f, PRECISION);

            if(n_point != -1)
            {
                move_points_s = 1;
                move_points_f = 0;
            }
            return;
        }

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && move_points_s)
        {
            double xpos, ypos;
            float xpos_f, ypos_f;
            int width, height;
            glfwGetCursorPos(window, &xpos, &ypos);
            glfwGetWindowSize(window, &width, &height);
            xpos_f = xposToFloat(xpos, width);
            ypos_f = yposToFloat(ypos, height);
            if (n_point != -1)
            {
                Arr[n_point].x = xpos_f;
                Arr[n_point].y = ypos_f;
            }
            n_point = -1;
            move_points_s = 0;
            move_points_f = 1;
            result.erase(result.begin(), result.end());
            Bize(Arr);
            return;
        }
    }
    else return;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        std::cout << "ARR\n";
        for (int i = 0; i < Arr.size(); i++)
        {
            std::cout << i << ": ";
            std::cout << Arr[i].x << ", " << Arr[i].y << std::endl;
        }
        std::cout << "ARR END" << std::endl;
    }

    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        std::cout << "RESULT\n";
        for (int i = 0; i < result.size(); i++)
        {
            std::cout << i << ": ";
            std::cout << result[i].x << ", " << result[i].y << std::endl;
        }
        std::cout << "RESULT END" << std::endl;
    }
}

int main()
{
    if (!glfwInit()) {
        return -1;
    }
    GLFWwindow* window = glfwCreateWindow(800, 600, "Daria Kamushkina", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_callback);
    glfwMakeContextCurrent(window);

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");


    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        ImGui::Begin("dashapupu");
        
        ImVec2 imguisize = ImGui::GetWindowSize();
        ImVec2 imguipos = ImGui::GetWindowPos();
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        if (xpos > imguipos[0] - 5 && xpos < imguipos[0] + imguisize[0] + 5 && ypos > imguipos[1] - 5 && ypos < imguipos[1] + imguisize[1] + 5)
            is_on_imgui = 1;
        else
            is_on_imgui = 0;


        if (ImGui::Button("add point"))
            put_points = 1;
        if (ImGui::Button("do not add point"))
            put_points = 0;
        if (ImGui::Button("connect"))
        {
            result.erase(result.begin(), result.end());
            Bize(Arr);
        }
        if (ImGui::Button("clear"))
        {
            Arr.erase(Arr.begin(), Arr.end());
            result.erase(result.begin(), result.end());
        }
        if (ImGui::Button("move points"))
            move_points_f = 1;
        if (ImGui::Button("stop moving"))
            move_points_f = 0;

        ImGui::End();
        ImGui::Render();

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glPointSize(2.0f);
        glBegin(GL_POINTS);

        for (int i = 0; i < Arr.size(); i++)
        {
            glColor3f(1.0f, 0.0f, 0.0f);
            glVertex2f(Arr[i].x, Arr[i].y);
        }

        glEnd();

        glBegin(GL_LINE_STRIP);

        for (int i = 0; i < result.size(); i++)
        {
            glColor3f(0.0f, 1.0f, 1.0f);
            glVertex2f(result[i].x, result[i].y);
        }

        glEnd();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
