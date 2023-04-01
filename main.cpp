#include <stdio.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#define GLFW_INCLUDE_ES3
#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>


GLFWwindow* g_window;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
bool show_demo_window = true;
bool show_another_window = false;
int g_width;
int g_height;

// Function used by c++ to get the size of the html canvas
EM_JS(int, canvas_get_width, (), {
  return Module.canvas.width;
});

// Function used by c++ to get the size of the html canvas
EM_JS(int, canvas_get_height, (), {
  return Module.canvas.height;
});

// Function called by javascript
EM_JS(void, resizeCanvas, (), {
  js_resizeCanvas();
});

void on_size_changed()
{
  glfwSetWindowSize(g_window, g_width, g_height);

  ImGui::SetCurrentContext(ImGui::GetCurrentContext());
}

void loop()
{
  int width = canvas_get_width();
  int height = canvas_get_height();

  if (width != g_width || height != g_height)
  {
    g_width = width;
    g_height = height;
    on_size_changed();
  }

  glfwPollEvents();

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  // 1. Show a simple window.
  // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
  {
      static float f = 0.0f;
      static int counter = 0;
      ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)
      ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
      ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

      ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our windows open/close state
      ImGui::Checkbox("Another Window", &show_another_window);

      if (ImGui::Button("Button"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
          counter++;
      ImGui::SameLine();
      ImGui::Text("counter = %d", counter);

      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  }

  // 2. Show another simple window. In most cases you will use an explicit Begin/End pair to name your windows.
  if (show_another_window)
  {
      ImGui::Begin("Another Window", &show_another_window);
      ImGui::Text("Hello from another window!");
      if (ImGui::Button("Close Me"))
          show_another_window = false;
      ImGui::End();
  }

  // 3. Show the ImGui demo window. Most of the sample code is in ImGui::ShowDemoWindow(). Read its code to learn more about Dear ImGui!
  if (show_demo_window)
  {
      ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
      ImGui::ShowDemoWindow(&show_demo_window);
  }

  ImGui::Render();

  int display_w, display_h;
  glfwMakeContextCurrent(g_window);
  glfwGetFramebufferSize(g_window, &display_w, &display_h);
  glViewport(0, 0, display_w, display_h);
  glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
  glClear(GL_COLOR_BUFFER_BIT);

  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  glfwMakeContextCurrent(g_window);
}


int init_gl()
{
  if( !glfwInit() )
  {
      fprintf( stderr, "Failed to initialize GLFW\n" );
      return 1;
  }

  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

  // Open a window and create its OpenGL context
  int canvasWidth = g_width;
  int canvasHeight = g_height;
  g_window = glfwCreateWindow(canvasWidth, canvasHeight, "WebGui Demo", NULL, NULL);
  if( g_window == NULL )
  {
      fprintf( stderr, "Failed to open GLFW window.\n" );
      glfwTerminate();
      return -1;
  }
  glfwMakeContextCurrent(g_window); // Initialize GLEW

  return 0;
}


int init_imgui()
{
  // Setup Dear ImGui binding
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui_ImplGlfw_InitForOpenGL(g_window, true);
  ImGui_ImplOpenGL3_Init();

  // Setup style
  ImGui::StyleColorsDark();

  ImGuiIO& io = ImGui::GetIO();

  // Load Fonts
  io.Fonts->AddFontFromFileTTF("data/xkcd-script.ttf", 23.0f);
  io.Fonts->AddFontFromFileTTF("data/xkcd-script.ttf", 18.0f);
  io.Fonts->AddFontFromFileTTF("data/xkcd-script.ttf", 26.0f);
  io.Fonts->AddFontFromFileTTF("data/xkcd-script.ttf", 32.0f);
  io.Fonts->AddFontDefault();

  resizeCanvas();

  return 0;
}


int init()
{
  init_gl();
  init_imgui();
  return 0;
}


void quit()
{
  glfwTerminate();
}


extern "C" int main(int argc, char** argv)
{
  g_width = canvas_get_width();
  g_height = canvas_get_height();
  if (init() != 0) return 1;

  #ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(loop, 0, 1);
  #endif

  quit();

  return 0;
}
