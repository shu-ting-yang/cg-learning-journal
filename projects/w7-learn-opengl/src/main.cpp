//GLAD is a library that manages OpenGL function pointers
#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <GLFW/glfw3.h>// create an OpenGL context, define a window, and handle inputs
#include <iostream> // for input and output operations

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  // Make sure the viewport matches the new window dimensions; note that width and
  // height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}
int main()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);// Telling GLFW we want to use the core-profile means we'll get access to a smaller subset of OpenGL features without backwards-compatible features we no longer need.
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow* window = glfwCreateWindow(800, 600, "Learn OpenGL", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);// Make the window's context on the current thread
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) //we pass the function pointer to GLAD, to load the addresses of the OpenGL functions pointers
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }
  glViewport(0, 0, 800, 600); // Set the viewport size to the window size
                              //Behind the scenes OpenGL uses the data specified via glViewport to transform the 2D coordinates it processed to coordinates on your screen. For example, a processed point of location (-0.5,0.5) would (as its final transformation) be mapped to (200,450) in screen coordinates.
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // Set the viewport size to the window size when the window is resized
  // Main loop
  while (!glfwWindowShouldClose(window))
  {
    glfwSwapBuffers(window); // Swap the front and back buffers
    // Check for events and call the corresponding callback functions
    glfwPollEvents(); // Poll for and process events(e.g. keyboard input, mouse movement, etc.)
  }
  glfwTerminate(); // Terminate GLFW, clearing any resources allocated by GLFW
  std::cout << "GLFW terminated successfully" << std::endl;
  return 0;
  
}