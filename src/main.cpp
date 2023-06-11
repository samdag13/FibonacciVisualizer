#include "controller.h"
#include <iostream>
// dear imgui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan graphics context creation, etc.)

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include "imgui_internal.h"

// About Desktop OpenGL function loaders:
//  Modern desktop OpenGL doesn't have a standard portable header file to load OpenGL function pointers.
//  Helper libraries are often used for this purpose! Here we are supporting a few common ones (gl3w, glew, glad).
//  You may use another loader/header of your choice (glext, glLoadGen, etc.), or chose to manually implement your own.
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>    // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>    // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>  // Initialize with gladLoadGL()
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

// Include glfw3.h after our OpenGL definitions
#include <GLFW/glfw3.h>

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int, char**)
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if __APPLE__
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
    bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
    bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
    bool err = gladLoadGL() == 0;
#else
    bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
#endif
    if (err)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    //File Import/Export variables
    char newFileName[255] = {};
    char importFileName[255] = {};
    bool fileResult = true;

    //Rendering/drawing variables
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    int nums[2] = { 1, 21 };
    int zoom = 100;
    ImVec2 translation = {0.f, 0.f};
    ImVec4 clear_color = ImVec4(0.8f, 0.8f, 0.8f, 1.00f);
    auto col = IM_COL32(0, 0, 0, 255);

    //Init Controller, generate sequence from default values
    Controller controller;
    controller.SetSequence(nums[0], nums[1], display_w, display_h, zoom);
    std::vector<Cell> cells = controller.GetPopulatedGrid();

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (ImGui::IsMouseDown(0))
        {
            translation[0] += ImGui::GetMouseDragDelta()[0] / 25;
            translation[1] += ImGui::GetMouseDragDelta()[1] / 25;
        }


        //Render Fibonacci sequence before UI elements
        ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
        for each (Cell cell in cells)
        {
            cell.CenterX  += translation[0];
            cell.CenterY  += translation[1];
            cell.OppositeX  += translation[0];
            cell.OppositeY  += translation[1];

            ImVec2 center(cell.CenterX, cell.CenterY);
            ImVec2 opposite(cell.OppositeX, cell.OppositeY);

            draw_list->PathArcTo(center, cell.Radius, cell.AngleMin * IM_PI, cell.AngleMax * IM_PI, 50);
            draw_list->PathStroke(col, false, 2.5f);

            draw_list->AddRect(center, opposite, col);
            draw_list->PathStroke(col, false, 2.5f);
        }

        {
            ImGui::Begin("Fibonacci Sequence Visualizer!");

            ImGui::InputText("New save file name", newFileName, 255);
            if (ImGui::Button("Export Sequence"))
                controller.ExportSequence(std::string(newFileName));

            ImGui::InputText("Name of the file to import", importFileName, 255);
            if (ImGui::Button("Import Sequence"))
            {
                fileResult = controller.ImportSequence(std::string(importFileName));

                nums[0] = controller.GetFirst();
                nums[1] = controller.GetLast();

                controller.SetSequence(nums[0], nums[1], display_w, display_h, zoom);
                cells = controller.GetPopulatedGrid();
            }
            if(!fileResult)
                ImGui::Text("Error, could not import file. Check that file location and name are valid.");

            ImGui::Text("Enter values to define the interval of the sequence.");
            ImGui::InputInt2("Positive integers only", nums);
            if(nums[0] < 1 || nums[1] < 1)
                ImGui::Text("Enter positive integer values only.");

            if (ImGui::Button("Generate Visual Sequence"))
            {
                //Center screen position for new sequences
                translation = { 0.f, 0.f };
                //Adjust based on window resolution
                glfwGetFramebufferSize(window, &display_w, &display_h);
                controller.SetSequence(nums[0], nums[1], display_w, display_h, zoom);
                cells = controller.GetPopulatedGrid();                
            }

            ImGui::Text("Zoom");
            if (ImGui::InputInt("", &zoom)) 
            {
                //Positive integer only. 0 => erase.
                if (zoom < 1)
                    zoom = 1;

                controller.SetSequence(nums[0], nums[1], display_w, display_h, zoom);
                cells = controller.GetPopulatedGrid();
            };

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
