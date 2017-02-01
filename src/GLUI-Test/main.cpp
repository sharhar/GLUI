#include <GLUI/GLUI.h>

#include <math.h>		   //----------------------------------------------------------
#include <GLFW/glfw3.h>    //    Only for testing, won't be needed when using GLUI
#include <iostream>		   //----------------------------------------------------------

#define DEG_TO_RAD 0.0174532925199

using namespace glui;

int main() {
	GLUI::init();

	Window win("GLUI Test", 800, 600, false, 0, NULL);

	Renderer::init(&win);

	glui::Rectangle rect = {100, 100, 100, 50};
	Layout* layout = new AbsoluteLayout(&win, 800, 600);

	Font* font30 = new Font("arial.ttf", 30);
	if (!font30->inited()) {
		win.destroy();
		std::cout << "Could not load arial.ttf!\n";
		return -1;
	}

	Font* font20 = new Font("arial.ttf", 20);
	if (!font20->inited()) {
		win.destroy();
		std::cout << "Could not load arial.ttf!\n";
		return -1;
	}
    
    Theme theme = {};
    theme.body					= color::lightGrey;
    theme.check					= color::black;
    theme.circle				= color::black;
    theme.hover					= color::grey;
    theme.outline				= color::black;
    theme.press					= color::darkGrey;
    theme.text					= color::black;
    theme.popupBackground		= color::lightGrey;
    theme.popupText				= color::black;
    
    TextStyle buttonText = { 30, font30 };
    TextStyle textBoxText = { 20, font20 };
    ButtonDescriptor desc = { buttonText,
        [&]()->void {
            char** chars = new char*[2];
            chars[0] = (char*)"Cancel";
            chars[1] = (char*)"Ok";
            
            PopupDescriptor pDesc = {};
            pDesc.width = 300;
            pDesc.height = 200;
            pDesc.title = "Popup!";
            pDesc.text = "This is a popup!";
            pDesc.btnNum = 2;
            pDesc.btnText = (const char**)chars;
            pDesc.window = &win;
            pDesc.bodyTextStyle = textBoxText;
            pDesc.buttonTextStyle = buttonText;
            
            int result = Window::popup(pDesc, theme);
            
            std::cout << "popup returned: " << result << "\n";
        },
        3, theme
    };
    
    Button button(rect, layout, "Press", desc);
    
    TextBoxDescriptor tdesc = { textBoxText , 1, 2, theme };
    
    rect.h = 30;
    rect.x = 500;
    rect.w = 200;
    
    TextBox textBox(rect, layout, tdesc);
    
    rect.y = 200;
    
    TextBox textBox2(rect, layout, tdesc);
    
    float vertices[] = {
        -0.5f,0.5f,-0.5f,
        -0.5f,-0.5f,-0.5f,
        0.5f,-0.5f,-0.5f,
        0.5f,0.5f,-0.5f,
        
        -0.5f,0.5f,0.5f,
        -0.5f,-0.5f,0.5f,
        0.5f,-0.5f,0.5f,
        0.5f,0.5f,0.5f,
        
        0.5f,0.5f,-0.5f,
        0.5f,-0.5f,-0.5f,
        0.5f,-0.5f,0.5f,
        0.5f,0.5f,0.5f,
        
        -0.5f,0.5f,-0.5f,
        -0.5f,-0.5f,-0.5f,
        -0.5f,-0.5f,0.5f,
        -0.5f,0.5f,0.5f,
        
        -0.5f,0.5f,0.5f,
        -0.5f,0.5f,-0.5f,
        0.5f,0.5f,-0.5f,
        0.5f,0.5f,0.5f,
        
        -0.5f,-0.5f,0.5f,
        -0.5f,-0.5f,-0.5f,
        0.5f,-0.5f,-0.5f,
        0.5f,-0.5f,0.5f
    };
    
    float colors[] = {
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        
        1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        
        0.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 1.0f,
        
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f
    };
    
    GLuint indices[] = {
        0,1,3,
        3,1,2,
        4,5,7,
        7,5,6,
        8,9,11,
        11,9,10,
        12,13,15,
        15,13,14,
        16,17,19,
        19,17,18,
        20,21,23,
        23,21,22
        
    };
    
    GLuint vao;
    GLuint vbo1;
    GLuint vbo2;
    GLuint vbo3;
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    glGenBuffers(1, &vbo1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * 6 * 4, vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenBuffers(1, &vbo2);
    glBindBuffer(GL_ARRAY_BUFFER, vbo2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * 6 * 4, colors, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenBuffers(1, &vbo3);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo3);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint) * 3 * 6 * 2, indices, GL_STATIC_DRAW);
    
    glBindVertexArray(0);
    
    std::string vertSource = "";
    
    vertSource += "#version 330 core\n";
    
    vertSource += "in vec3 position;\n";
    vertSource += "in vec3 color;\n";
    vertSource += "uniform mat4 rx;\n";
    vertSource += "uniform mat4 ry;\n";
    vertSource += "uniform mat4 rz;\n";
    vertSource += "uniform mat4 modelview;\n";
    vertSource += "uniform mat4 projection;\n";
    
    vertSource += "out vec3 color_vary;\n";
    
    vertSource += "void main(void) {\n";
    vertSource += "gl_Position = projection * modelview * rx * ry * rz * vec4(position.xyz, 1);\n";
    vertSource += "color_vary = color;\n";
    vertSource += "}\n";
    
    std::string fragSource = "";
    
    fragSource += "#version 330 core\n";
    fragSource += "out vec4 out_color;\n";
    
    fragSource += "in vec3 color_vary;\n";
    
    fragSource += "void main(void) {\n";
    
    fragSource += "out_color = vec4(color_vary.xyz, 1.0);\n";
    
    fragSource += "}\n";
    
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    GLchar* shadersource = (GLchar*)vertSource.c_str();
    glShaderSource(vertShader, 1, &shadersource, 0);
    shadersource = (GLchar*)fragSource.c_str();
    glShaderSource(fragShader, 1, &shadersource, 0);
    
    glCompileShader(vertShader);
    
    GLint compiled = 0;
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &compiled);
    if (compiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &maxLength);
        
        GLchar* message = (GLchar*)malloc(sizeof(GLchar)*maxLength);
        glGetShaderInfoLog(vertShader, maxLength, &maxLength, message);
        
        std::cout << "Vertex Shader failed to compile:\n";
        std::cout << message << "\n";
        
        glDeleteShader(vertShader);
        return -1;
    }
    
    glCompileShader(fragShader);
    
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &compiled);
    if (compiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &maxLength);
        
        GLchar* message = (GLchar*)malloc(sizeof(GLchar)*maxLength);
        glGetShaderInfoLog(fragShader, maxLength, &maxLength, message);
        
        std::cout << "Fragment Shader failed to compile:\n";
        std::cout << message << "\n";
        
        glDeleteShader(fragShader);
        return -1;
    }
    
    GLuint shaderProgram = glCreateProgram();
    
    glAttachShader(shaderProgram, vertShader);
    glAttachShader(shaderProgram, fragShader);
    
    glBindAttribLocation(shaderProgram, 0, "position");
    glBindAttribLocation(shaderProgram, 1, "color");
    
    glLinkProgram(shaderProgram);
    glValidateProgram(shaderProgram);
    
    GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    GLuint modelviewLoc = glGetUniformLocation(shaderProgram, "modelview");
    GLuint rxLoc = glGetUniformLocation(shaderProgram, "rx");
    GLuint ryLoc = glGetUniformLocation(shaderProgram, "ry");
    GLuint rzLoc = glGetUniformLocation(shaderProgram, "rz");
    
    float* modelview = new float[16];
    modelview[0] = 2;
    modelview[1] = 0;
    modelview[2] = 0;
    modelview[3] = 0;
    
    modelview[4] = 0;
    modelview[5] = 2;
    modelview[6] = 0;
    modelview[7] = 0;
    
    modelview[8] = 0;
    modelview[9] = 0;
    modelview[10] = 1;
    modelview[11] = 0;
    
    modelview[12] = 0;
    modelview[13] = 0;
    modelview[14] = -4;
    modelview[15] = 1;
    
    float xtheta = 0;
    float ytheta = 45;
    float ztheta = 0;
    
    float cx = cos(xtheta*DEG_TO_RAD);
    float sx = sin(xtheta*DEG_TO_RAD);
    float cy = cos(ytheta*DEG_TO_RAD);
    float sy = sin(ytheta*DEG_TO_RAD);
    float cz = cos(ztheta*DEG_TO_RAD);
    float sz = sin(ztheta*DEG_TO_RAD);
    
    float* rx = new float[16];
    rx[0] = 1;
    rx[1] = 0;
    rx[2] = 0;
    rx[3] = 0;
    
    rx[4] = 0;
    rx[5] = cx;
    rx[6] = sx;
    rx[7] = 0;
    
    rx[8] = 0;
    rx[9] = -sx;
    rx[10] = cx;
    rx[11] = 0;
    
    rx[12] = 0;
    rx[13] = 0;
    rx[14] = 0;
    rx[15] = 1;
    
    float* ry = new float[16];
    ry[0] = cy;
    ry[1] = 0;
    ry[2] = -sy;
    ry[3] = 0;
    
    ry[4] = 0;
    ry[5] = 1;
    ry[6] = 0;
    ry[7] = 0;
    
    ry[8] = sy;
    ry[9] = 0;
    ry[10] = cy;
    ry[11] = 0;
    
    ry[12] = 0;
    ry[13] = 0;
    ry[14] = 0;
    ry[15] = 1;
    
    float* rz = new float[16];
    rz[0] = cz;
    rz[1] = sz;
    rz[2] = 0;
    rz[3] = 0;
    
    rz[4] = -sz;
    rz[5] = cz;
    rz[6] = 0;
    rz[7] = 0;
    
    rz[8] = 0;
    rz[9] = 0;
    rz[10] = 1;
    rz[11] = 0;
    
    rz[12] = 0;
    rz[13] = 0;
    rz[14] = 0;
    rz[15] = 1;
    
    float aspect = 1;
    float fov = 80;
    float far = 1000;
    float near = 0.01f;
    
    float t = tan(DEG_TO_RAD*fov/2);
    
    float* projection = new float[16];
    projection[0] = 1/(t*aspect);
    projection[1] = 0;
    projection[2] = 0;
    projection[3] = 0;
    
    projection[4] = 0;
    projection[5] = 1/t;
    projection[6] = 0;
    projection[7] = 0;
    
    projection[8] = 0;
    projection[9] = 0;
    projection[10] = -(far + near)/(far - near);
    projection[11] = -1;
    
    projection[12] = 0;
    projection[13] = 0;
    projection[14] = -(2*far*near)/(far - near);
    projection[15] = 0;
    
    glUseProgram(shaderProgram);
    
    glUniformMatrix4fv(rxLoc, 1, false, rx);
    glUniformMatrix4fv(ryLoc, 1, false, ry);
    glUniformMatrix4fv(rzLoc, 1, false, rz);
    glUniformMatrix4fv(modelviewLoc, 1, false, modelview);
    glUniformMatrix4fv(projectionLoc, 1, false, projection);
    
    glUseProgram(0);
    
    GLPanel panel({ 10, 290, 300, 300 }, {300, 300}, layout,
                  [&]()->void { //Render function
                      glEnable(GL_DEPTH_TEST);
                      glClearColor(0.2f, 0.3f, 0.8f, 1);
                      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                      
                     float rotSpeed = 30;
                      
                      xtheta = glfwGetTime() * rotSpeed / 1;
                      ytheta = glfwGetTime() * rotSpeed / 2;
                      ztheta = glfwGetTime() * rotSpeed / 3;
                      
                      cx = cos(xtheta*DEG_TO_RAD);
                      sx = sin(xtheta*DEG_TO_RAD);
                      cy = cos(ytheta*DEG_TO_RAD);
                      sy = sin(ytheta*DEG_TO_RAD);
                      cz = cos(ztheta*DEG_TO_RAD);
                      sz = sin(ztheta*DEG_TO_RAD);
                      
                      rx[5] = cx;
                      rx[6] = sx;
                      rx[9] = -sx;
                      rx[10] = cx;
                      
                      ry[0] = cy;
                      ry[2] = -sy;
                      ry[8] = sy;
                      ry[10] = cy;
                      
                      rz[0] = cz;
                      rz[1] = sz;
                      rz[4] = -sz;
                      rz[5] = cz;
                      
                      glBindVertexArray(vao);
                      glEnableVertexAttribArray(0);
                      glEnableVertexAttribArray(1);
                      
                      glUseProgram(shaderProgram);
                      
                      glUniformMatrix4fv(rxLoc, 1, false, rx);
                      glUniformMatrix4fv(ryLoc, 1, false, ry);
                      glUniformMatrix4fv(rzLoc, 1, false, rz);
                      
                      glDrawElements(GL_TRIANGLES, 6*6, GL_UNSIGNED_INT, 0);
                      
                      glUseProgram(0);
                      
                      glDisableVertexAttribArray(0);
                      glDisableVertexAttribArray(1);
                      glBindVertexArray(0);
                  },
                  [](GLPanelMouseData* data)->void {
                      
                  }, theme);
    
    CheckBoxDescriptor checkBoxDesc = { 
        { 30, font30 }, 
        3, 
        false,
        theme
    };
    
    CheckBox checkBox({100, 200, 30, 30}, layout, "toggle", checkBoxDesc);
	while (win.isOpen()) {
		win.poll();

        button.poll();
        textBox.poll();
        textBox2.poll();
        panel.poll();
        checkBox.poll();

		Renderer::clear(color::grey);

		Renderer::beginDraw();
        
        button.render();
        textBox.render();
        textBox2.render();
        panel.render();
        checkBox.render();

		Renderer::endDraw();

		win.swap();
	}

	win.destroy();

	GLUI::destroy();

	return 0;
}
