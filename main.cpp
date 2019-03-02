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
ImGuiContext* imgui = 0;
bool show_demo_window = true;
bool show_another_window = false;

EM_JS(int, canvas_get_width, (), {
  return Module.canvas.width;
});

EM_JS(int, canvas_get_height, (), {
  return Module.canvas.height;
});

void draw()
{
  int width = canvas_get_width();
  int height = canvas_get_height();

  glfwSetWindowSize(g_window, width, height);

  ImGui::SetCurrentContext(imgui);

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

  //std::cout << "2nd window" << std::endl;

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


void input()
{
}


void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{

  ImGui::SetCurrentContext(imgui);
  ImGuiIO& io = ImGui::GetIO();
  //io.WantSetMousePos = true;
  std::cout << io.MousePos.x << ", " << io.MousePos.y << std::endl;
  io.MousePos = ImVec2((float)xpos*1.f, (float)ypos*1.f);
  // std::cout << io.MousePos.x << std::endl;
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{

}


void loop()
{
  input();
  draw();
}


int init()
{
  if( !glfwInit() )
  {
      fprintf( stderr, "Failed to initialize GLFW\n" );
      return 1;
  }

  //glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

  // Open a window and create its OpenGL context
  int canvasWidth = 800;
  int canvasHeight = 600;
  g_window = glfwCreateWindow( canvasWidth, canvasHeight, "WebGui Demo", NULL, NULL);
  if( g_window == NULL )
  {
      fprintf( stderr, "Failed to open GLFW window.\n" );
      glfwTerminate();
      return -1;
  }
  glfwMakeContextCurrent(g_window); // Initialize GLEW

  // Create game objects
  // Setup Dear ImGui binding
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

  ImGui_ImplGlfw_InitForOpenGL(g_window, false);
  ImGui_ImplOpenGL3_Init();

  // Setup style
  ImGui::StyleColorsDark();
  //ImGui::StyleColorsClassic();

  // Load Fonts
  io.Fonts->AddFontFromFileTTF("data/xkcd-script.ttf", 23.0f);
  io.Fonts->AddFontFromFileTTF("data/xkcd-script.ttf", 18.0f);
  io.Fonts->AddFontFromFileTTF("data/xkcd-script.ttf", 26.0f);
  io.Fonts->AddFontFromFileTTF("data/xkcd-script.ttf", 32.0f);
  io.Fonts->AddFontDefault();

  imgui =  ImGui::GetCurrentContext();

  // Cursor callbacks
  // glfwSetCursorPosCallback(g_window, cursor_pos_callback);
  glfwSetMouseButtonCallback(g_window, ImGui_ImplGlfw_MouseButtonCallback);
  glfwSetScrollCallback(g_window, ImGui_ImplGlfw_ScrollCallback);
  glfwSetKeyCallback(g_window, ImGui_ImplGlfw_KeyCallback);
  glfwSetCharCallback(g_window, ImGui_ImplGlfw_CharCallback);

  return 0;
}


void quit()
{
  glfwTerminate();
}


extern "C" int main(int argc, char** argv)
{
  if (init() != 0) return 1;

  #ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(loop, 0, 1);
  #endif

  quit();

  return 0;
}
