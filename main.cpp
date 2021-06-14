
#include "Rasterization.h"
#include <thread>
#include <Windows.h>

// ������Ļ�ֱ��ʵĴ�С
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;

int fps = 0;
void ShowFps(GLFWwindow* window) {
    while (1) {
        Sleep(1000);
        //�ڴ��ڱ�����ʾ
        std::string text = "YRenderV0.1 fps:" + std::to_string(fps);
        glfwSetWindowTitle(window, text.c_str());
        fps = 0;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


Rasterization* ras;
int main()
{
    //��ʼ��GLFW
    glfwInit();
    //����GLFW
    /*
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);                    //OpenGL�汾3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);  //ʹ�ú���ģʽ(Core-profile)
    */

    // �������ڶ���
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "YRender", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glm::mat4 ViewPortMatrix = GetViewPortMatrix(0, 0, SCR_WIDTH, SCR_HEIGHT);
    
    ras = new Rasterization(SCR_WIDTH, SCR_HEIGHT);
    ras->Init();
    
    //������������������
    Vertex V1(glm::vec3(-0.5, -0.5, 0), glm::vec4(255, 0, 0, 0));
    Vertex V2(glm::vec3(0.5, -0.5, 0), glm::vec4(0, 255, 0, 0));
    Vertex V3(glm::vec3(0, 0.5, 0), glm::vec4(0, 0, 255, 0));

    glfwMakeContextCurrent(window);
    //ע��framebuffer_size_callback����,����GLFWÿ�����ڵ�����С��ʱ������������
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // �������ر���Ⱦ����ǰ���ϻ���ͼ���ܹ������û�����
 
    // glad:����OpenGL�ĺ���ָ��,�ڵ����κ�OpenGL�ĺ���֮ǰ������Ҫ��ʼ��GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //--------------------------------------------------------------------------------------------/
    std::thread t(ShowFps, window);
    t.detach();
    

    // ��ÿ��ѭ���Ŀ�ʼǰ���һ��GLFW�Ƿ�Ҫ���˳�������ǵĻ��ú�������true����Ⱦѭ������
    while (!glfwWindowShouldClose(window))
    {
        
        // input
        // -----
        processInput(window);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        // render
        // ------

       
        ras->ClearBuffer(glm::vec4(0.0f, 0.1f, 0.2f, 0.0f));

        ras->DrawTriangle(V1, V2, V3);
        
        ras->Show();
        fps++;

        

        // ������ɫ���壬
        // ǰ���屣�������������ͼ����������Ļ����ʾ�������еĵ���Ⱦָ����ں󻺳��ϻ��ơ�
        // �����е���Ⱦָ��ִ����Ϻ󣬽���ǰ����ͺ󻺳壬ʹͼ����������
        glfwSwapBuffers(window);

        //���������û�д���ʲô�¼��������ö�Ӧ�Ļص�����
        glfwPollEvents();
    }

    // ��ȷ�ͷ�/ɾ��֮ǰ�ķ����������Դ
    glfwTerminate();
    return 0;
}

// ����û��Ƿ����˷��ؼ�(Esc)
// �����û�а��£�glfwGetKey���᷵��GLFW_RELEASE������û���ȷ�����˷��ؼ�����ͨ��glfwSetwindowShouldCloseʹ�ð�WindowShouldClose��������Ϊtrue�ķ����ر�GLFW��
void processInput(GLFWwindow* window)
{
  
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    /*
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
    */
}

// ÿ�δ��ڴ�С��������ʱ�򱻵���
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}