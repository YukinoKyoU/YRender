
#include "Rasterization.h"
#include "Camera.h"
#include <thread>
#include <Windows.h>

// 设置屏幕分辨率的大小
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;

int fps = 0;
void ShowFps(GLFWwindow* window) {
    while (1) {
        Sleep(1000);
        //在窗口标题显示
        std::string text = "YRenderV0.1 fps:" + std::to_string(fps);
        glfwSetWindowTitle(window, text.c_str());
        fps = 0;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

Rasterization* ras;
Camera* camera;
int main()
{
    //初始化GLFW
    glfwInit();
    //配置GLFW
    /*
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);                    //OpenGL版本3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);  //使用核心模式(Core-profile)
    */

    // 创建窗口对象
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "YRender", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    camera = new Camera(glm::vec3(0.0f, 0.0f, 5.0f),
                        glm::vec3(0, 1, 0), 
                        glm::vec3(0, 0, 1));

    glm::mat4 ViewPortMatrix = GetViewPortMatrix(0, 0, SCR_WIDTH, SCR_HEIGHT);
    
    ras = new Rasterization(SCR_WIDTH, SCR_HEIGHT);
    ras->Init();
    
    Mesh box;
    box = box.CreateBox(glm::vec3(0.0, 0.0, 0.0), 0.5);
    Texture tex("container.jpg");
    ras->SetTexture(&tex);

    

    float angle = 0.0f;

    glfwMakeContextCurrent(window);
    //注册framebuffer_size_callback函数,告诉GLFW每当窗口调整大小的时候调用这个函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // 在主动关闭渲染窗口前不断绘制图像并能够接受用户输入
 
    // glad:管理OpenGL的函数指针,在调用任何OpenGL的函数之前我们需要初始化GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    std::thread t(ShowFps, window);
    t.detach();
  
    // 在每次循环的开始前检查一次GLFW是否被要求退出，如果是的话该函数返回true，渲染循环结束
    while (!glfwWindowShouldClose(window))
    {
   
        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        ras->ClearBuffer(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

        ras->setViewMatrix(GetViewMatrix(camera->Position, camera->Front, camera->Right, camera->Up));
        ras->setProjectMatrix(GetPerspectiveMatrix(camera->FOV, camera->Aspect, 0.3f, 100));
        
        ras->setModelMatrix(glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(1.0, 1.0, 0.0)));
        ras->UpdateViewPlanes();

        ras->DrawMesh(box);
        ras->Show();

        fps++;
        //angle += 1.0f;

        

        // 交换颜色缓冲，
        // 前缓冲保存着最终输出的图像，它会在屏幕上显示；而所有的的渲染指令都会在后缓冲上绘制。
        // 当所有的渲染指令执行完毕后，交换前缓冲和后缓冲，使图像立即呈显
        glfwSwapBuffers(window);

        //函数检查有没有触发什么事件，并调用对应的回调函数
        glfwPollEvents();
    }

    // 正确释放/删除之前的分配的所有资源
    glfwTerminate();
    return 0;
}

// 检查用户是否按下了返回键(Esc)
// （如果没有按下，glfwGetKey将会返回GLFW_RELEASE。如果用户的确按下了返回键，将通过glfwSetwindowShouldClose使用把WindowShouldClose属性设置为true的方法关闭GLFW）
void processInput(GLFWwindow* window)
{
  
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        camera->RotatePitch(5.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        camera->RotateYaw(5.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        camera->RotateYaw(-5.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        camera->RotatePitch(-5.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera->MoveForward(0.1f);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera->MoveForward(-0.1f);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera->MoveRight(-0.1f);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera->MoveRight(0.1f);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera->MoveUp(0.1f);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        camera->MoveUp(-0.1f);
    }
    
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        ras->changeRenderMode();
    }
    
}

// 每次窗口大小被调整的时候被调用
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}