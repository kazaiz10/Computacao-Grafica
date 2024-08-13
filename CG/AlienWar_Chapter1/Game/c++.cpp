#include "StructesList.h"
#include <learnopengl/model.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <learnopengl/filesystem.h>
#include "XboxControler.h"
unsigned int loadTexture(char const* path);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
float Loading = 0.0f;
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
window Window;
// camera
Camera camera(glm::vec3(-7.9f, 11.5f, 7.9f));
float lastX = Window.getWidth() / 2.0f;
float lastY = Window.getHeight() / 2.0f;
bool firstMouse = true;
float CameraZoom = 25.0f;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
Player player;

std::map<GLchar, Character> Characters;
unsigned int VAO, VBO;

void PrintLoadingBar(float pertecentagem) {
    printf("\r[");
    for (float value = 0.0; value < 1.0f; value += 0.10f) {
        if (value < pertecentagem) printf("*");
        else printf(" ");
    }
    printf("] %f", pertecentagem * 100);
}

// render line of text
// -------------------
void RenderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color)
{
    // activate corresponding render state	
    shader.use();
    glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void RenderLodingScreen(int ColorSeep) {
    window LoadingScreen;
    LoadingScreen.init();
    glfwSetWindowPos(LoadingScreen.getWindow(), 600, 20);
    // OpenGL state
    // ------------
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // compile and setup the shader
    // ----------------------------
    Shader ShaderText("C:/Users/jomig/source/repos/Map/TextShaders/text.vs", "C:/Users/jomig/source/repos/Map/TextShaders/text.fs");
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(LoadingScreen.getWidth()), 0.0f, static_cast<float>(LoadingScreen.getHeight()));
    ShaderText.use();
    glUniformMatrix4fv(glGetUniformLocation(ShaderText.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // FreeType
    // --------
    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }

    // find path to font
    std::string font_name = "C:/Users/jomig/OneDrive/Ambiente de Trabalho/LearnOpenGL-master/resources/fonts/Antonio-Bold.ttf";
    if (font_name.empty())
    {
        std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
    }

    // load font as face
    FT_Face face;
    if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    }
    else {
        // set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, 48);

        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // load first 128 characters of ASCII set
        for (unsigned char c = 0; c < 128; c++)
        {
            // Load character glyph 
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // now store character for later use
            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<unsigned int>(face->glyph->advance.x)
            };
            Characters.insert(std::pair<char, Character>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);


    // configure VAO/VBO for texture quads
    // -----------------------------------
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // render loop
    // -----------
    float BackGroundColor = 0.0f;
    while (!glfwWindowShouldClose(LoadingScreen.getWindow()) && Loading < 1.0f)
    {
        // render
        // ------
        BackGroundColor += 1.0f / ColorSeep;
        float ColorValue = module(sin(BackGroundColor));
        if (ColorValue > 0.5f) ColorSeep = - ColorSeep;
        glClearColor(ColorValue, ColorValue, ColorValue, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //RenderText(ShaderText, "This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
        //RenderText(ShaderText, "(C) LearnOpenGL.com", 540.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));
        RenderText(ShaderText, "Loading", LoadingScreen.getWidth() * 0.35f, LoadingScreen.getHeight() * 0.75f, 2.0f, glm::vec3(1.0f));
        RenderText(ShaderText, to_string(Loading * 100), LoadingScreen.getWidth() * 0.32f, LoadingScreen.getHeight() * 0.55f, 1.2f, glm::vec3(1.0f));
        RenderText(ShaderText, "%", LoadingScreen.getWidth() * 0.60f, LoadingScreen.getHeight() * 0.55f, 1.2f, glm::vec3(1.0f));

        RenderText(ShaderText, "[", LoadingScreen.getWidth() * 0.25f, LoadingScreen.getHeight() * 0.40f, 1.2f, glm::vec3(1.0f));
        for (float value = 0.0; value < 1.0f; value += 0.10f) {
            if (value < Loading) RenderText(ShaderText, "*", LoadingScreen.getWidth() * (0.28f + 0.4f * value), LoadingScreen.getHeight() * 0.392f, 1.0f, glm::vec3(1.0f));
        }
        RenderText(ShaderText, "]", LoadingScreen.getWidth() * 0.71f, LoadingScreen.getHeight() * 0.40f, 1.2f, glm::vec3(1.0f));
        RenderText(ShaderText, "Jose Santos", LoadingScreen.getWidth() * 0.03f, LoadingScreen.getHeight() * 0.03f, 0.7f, glm::vec3(-ColorValue, ColorValue + 0.5f, -ColorValue + 0.6f));
        RenderText(ShaderText, "Francisco Casais", LoadingScreen.getWidth() * 0.25f, LoadingScreen.getHeight() * 0.03f, 0.7f, glm::vec3(ColorValue + 0.6f, ColorValue, 0.1f));
        RenderText(ShaderText, "Luis Landeiras", LoadingScreen.getWidth() * 0.55f, LoadingScreen.getHeight() * 0.03f, 0.7f, glm::vec3(ColorValue + 0.4f, 0.1f, ColorValue + 0.6f));
        RenderText(ShaderText, "Nuno Tavares", LoadingScreen.getWidth() * 0.78f, LoadingScreen.getHeight() * 0.03f, 0.7f, glm::vec3(ColorValue + 0.5f, -ColorValue,-ColorValue + 0.5f));
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(LoadingScreen.getWindow());
        glfwPollEvents();
    }
}

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

void CreateChunks(List* Coords, List* TreeList, List* EnemyList) {
    for (short j = -12; j <= 24; j = j + 12) {
        for (short i = -16; i <= 16; i = i + 16) {
            Sleep(3);
            Tree* top = CreateMap(Window.getSeed()[0], 3);
            List middle = CreateMiddle(Window.getSeed()[1]);
            Tree* bot = CreateMap(Window.getSeed()[2], 3);
            List chuck = getChunkCoords(top, middle, bot);
            for (short index = 0; index < chuck.getsize(); index = index + 2) {
                chuck.getNode(index)->setValue(chuck.getindex(index) + j);
                chuck.getNode(index + 1)->setValue(chuck.getindex(index + 1) + i);
            }
            List NodeValueMiddle;
            for (short index = 0; index < middle.getsize(); index = index + 2) {
                NodeValueMiddle.insert(index);
            }
            TreeList->joinList(TreeTOList(top));
            TreeList->joinList(NodeValueMiddle);
            TreeList->joinList(TreeTOList(bot));
            Coords->joinList(chuck);
            if (i == 0 && j == 0) *EnemyList = EnemyCoordsList(top, bot);
        }
    }
}

void execute(window window) {
    printf("Execute start\n");
    Shader lightingShader("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/2.lighting/5.1.light_casters_directional/5.1.light_casters.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/2.lighting/5.1.light_casters_directional/5.1.light_casters.fs");
    Shader lightCubeShader("C:/Users/jomig/OneDrive/Ambiente de Trabalho/opengl/lighting/shaders/2.1.lamp.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/opengl/lighting/shaders/2.1.lamp.fs");
    printf("Light Shader Ready\n");
    Shader ourShader("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.fs");
    Shader ModelLamp("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.fs");
    Shader ModelFloor("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.fs");
    Shader ModelFloor1("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.fs");
    Shader ModelFloor2("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.fs");
    Shader ModelFloor3("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.fs");
    Shader ModelFloor4("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.fs");
    Shader ModelFloor5("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.fs");
    Shader ModelFloor6("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.fs");
    Shader ModelFloor7("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.fs");
    Shader ModelFloor8("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.fs");
    printf("Model Shader Ready\n");
    Shader sh[34] = {
        Shader("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.fs"),
        Shader("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.fs"),
        Shader("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.fs"),
        Shader("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.fs"),
        Shader("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.fs"),
        Shader("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.fs"),
        Shader("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.fs"),
        Shader("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.fs"),
        Shader("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.fs"),
        Shader("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.fs"),
        Shader("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.fs"),
        Shader("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.fs"),
        Shader("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.fs"),
        Shader("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.fs"),
        Shader("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.fs"),
        Shader("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.fs"),
        Shader("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.fs"),
        Shader("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.fs"),
        Shader("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.fs"),
        Shader("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.fs"),
        Shader("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.fs"),
        Shader("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.fs"),
        Shader("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.fs"),
        Shader("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.fs"),
        Shader("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.fs"),
        Shader("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.fs"),
        Shader("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.fs"),
        Shader("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.fs"),
        Shader("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.fs"),
        Shader("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.fs"),
        Shader("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.fs"),
        Shader("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.fs"),
        Shader("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.fs"),
        Shader("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.vs", "C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/src/3.model_loading/1.model_loading/1.model_loading.fs"),
    };
    printf("SH Shader Ready\n");
    printf("All shadersd ready\n");
    thread Loding(RenderLodingScreen, 5000);
    Model rock("C:/Users/jomig/source/repos/Map/resource/Rock1/rock.obj");
    Loading += 1.0f / 6.0f;
    Model alienOBJ("C:/Users/jomig/OneDrive/Ambiente de Trabalho/alien/alien.obj");
    Loading += 1.0f / 6.0f;
    Model IronManModel("C:/Users/jomig/source/repos/Map/resource/IM/ironam.obj");
    Loading += 1.0f / 6.0f;
    Model FireBall("C:/Users/jomig/source/repos/Map/resource/FireBall/FireBall.obj");
    Loading += 1.0f / 6.0f;
    Model Floor("C:/Users/jomig/source/repos/Map/resource/Floor/floor.obj");
    Loading += 1.0f / 6.0f;
    Model Crytal("C:/Users/jomig/OneDrive/Ambiente de Trabalho/crytal/crystal.obj");
    Loading += 1.0f / 6.0f;
    printf("\n");
    printf("Loading texture\n");
    unsigned int diffuseCube = loadTexture("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/resources/textures/bricks2.jpg");
    unsigned int matrix = loadTexture("C:/Users/jomig/OneDrive/Ambiente de Trabalho/Nova pasta/LearnOpenGL-master/resources/textures/matrix.jpg");
    unsigned int FireBallTexture = loadTexture("C:/Users/jomig/source/repos/Map/resource/FireBall/texture.jpg");
    unsigned int FloorTexture1 = loadTexture("C:/Users/jomig/source/repos/Map/resource/Floor/texture1.jpg");
    Loding.join();
    printf("Create List, Structs and Projetil\n");
    Projectile Projectile(5.0f, glm::vec3(0.0f));
    StructesList StList;
    List Coords, TreeListT;
    List EnemyList;
    printf("Create map\n");
    CreateChunks(&Coords, &TreeListT, &EnemyList);
    StList.insert(Coords, TreeListT);
    EnemyList.PrintList();
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  
    player.setModelScale(glm::vec3(0.3f));
    player.setVelocity(2.0f);
    player.setFireRate(5.0f);

    //unsigned int CrytalTexture = loadTexture("C:/Users/jomig/OneDrive/Ambiente de Trabalho/crytal/Texture.jpg");
    printf("1 ++++\n");
    float CameraX = 0.0f, CameraZ = 0.0f;
    XboxControler XboxControler(3000);
    XboxControler.init();
    printf("Seed -> %i %i %i\n", window.getSeed()[0], window.getSeed()[1], window.getSeed()[2]);
    Enemy Bank[60];
    printf("Enemy number -> %i\n", EnemyList.getsize());
    CreatBank(Bank, EnemyList.getsize(), diffuseCube, matrix);
    SetEnemyPos(Bank, EnemyList);
    short EnemyCount = EnemyList.getsize() / 2;
    printf("2 ++++\n");
    float day = glfwGetTime();
    printf("4 ++++\n");
    printf("5 ++++\n");
    float FireTime = glfwGetTime();
    printf("6 ++++\n");
    StList.ApplyShader();
    glfwShowWindow(Window.getWindow());
    List EnemyKill;
    while (!glfwWindowShouldClose(window.getWindow()))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        float MoveX = 0.0f, MoveZ = 0.0f;
        XboxControler.start(&MoveX, &MoveZ, &CameraZoom, &deltaTime, &player, &Projectile, (currentFrame - FireTime), &FireTime);
        camera.Zoom = CameraZoom;
        player.readKeyBoardInput(&MoveX, &MoveZ, deltaTime, Window.getWindow());
        if (glfwGetKey(window.getWindow(), GLFW_KEY_SPACE) == GLFW_PRESS) {
            if (currentFrame - FireTime >= 1.0f / player.getFireRate()) {
                Projectile.newProjectile(15.0f, glm::vec3(player.getPosition().x, 0.3f, player.getPosition().z), player.getAngle(), glm::vec3(0.0f, 1.0f, 0.0f));
                FireTime = glfwGetTime();
            }
        }
        glm::vec3 Future = player.getPosition() + (glm::vec3(MoveX, 0.0f, MoveZ) * player.getScale());
        if (CheckCollision(Future, StList, 0.7f, 0.0f)) printf("Bateu");
        else {
            player.move(glm::vec3(MoveX, 0.0f, MoveZ));
            CameraX = CameraX + MoveX * player.getScale();
            CameraZ = CameraZ + MoveZ * player.getScale();
        }
        Projectile.MoveAllProject(deltaTime, StList, Bank, EnemyCount);
        printf("\rX -> %f Y -> %f Z -> %f      ", player.getPosition()[0], player.getPosition()[1], player.getPosition()[2]);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        sh[0].use();
        sh[0].setMat4("model", player.getModel());
        for (int index = 1; index <= EnemyCount; index++) {
            sh[index].setMat4("model", Bank[index - 1].getModel());
        }
        ModelLamp.setMat4("model", glm::translate(glm::mat4(1.0f), lightPos));
        ModelFloor.setMat4("model", glm::mat4(1.0f));
        ModelFloor1.setMat4("model", glm::translate(glm::mat4(1.0f), glm::vec3(-15.0f, 0.0f, 0.0f)));
        ModelFloor2.setMat4("model", glm::translate(glm::mat4(1.0f), glm::vec3(15.0f, 0.0f, 0.0f)));
        ModelFloor3.setMat4("model", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -15.0f)));
        ModelFloor4.setMat4("model", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 15.0f)));
        ModelFloor5.setMat4("model", glm::translate(glm::mat4(1.0f), glm::vec3(15.0f, 0.0f, 15.0f)));
        ModelFloor6.setMat4("model", glm::translate(glm::mat4(1.0f), glm::vec3(15.0f, 0.0f, -15.0f)));
        ModelFloor7.setMat4("model", glm::translate(glm::mat4(1.0f), glm::vec3(-15.0f, 0.0f, 15.0f)));
        ModelFloor8.setMat4("model", glm::translate(glm::mat4(1.0f), glm::vec3(-15.0f, 0.0f, -15.0f)));
        Projectile.ApplyShaderModel();
        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("light.direction", -0.2f, -1.0f, -0.3f);
        lightingShader.setVec3("lightPos", lightPos);
        lightingShader.setVec3("viewPos", camera.Position);

         // light properties
        float dayLigth = module(sin((currentFrame - day) / window.getDayDuration()));
        lightingShader.setVec3("light.ambient", dayLigth, dayLigth, dayLigth);
        lightingShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
        lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

        // material properties 
        lightingShader.setFloat("material.shininess", 32.0f);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)Window.getWidth() / (float)Window.getHeight(), 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        view = glm::translate(view, glm::vec3(-CameraX, 0.0f, -CameraZ));
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);

        lightingShader.setMat4("model", player.getModel());

        IronManModel.Draw(sh[0]);

        for (int index = 0; index < Projectile.getSize(); index++) {
            if (Projectile.getShader() != NULL) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, diffuseCube);
                lightingShader.setMat4("model", Projectile.getIndex(index)->getModel());
                FireBall.Draw(*Projectile.getIndex(index)->getShader());
            }
        }

        for (int index = 1; index <= EnemyCount; index++) {
            if (Bank[index - 1].getHealth() > 0) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, Bank[index - 1].getTexture());
                lightingShader.setMat4("model", Bank[index - 1].getModel());
                alienOBJ.Draw(sh[index]);
            }
        }

        for (int index = 0; index < StList.getSize(); index++) {
            lightingShader.setMat4("model", StList.getIndex(index)->getModel());
            if (StList.getIndex(index)->getID() % 2 == 0)   rock.Draw(*StList.getIndex(index)->getShader());
            else {
                Crytal.Draw(*StList.getIndex(index)->getShader());
            }
        }

        if (distance(Projectile.getPosition0(), Projectile.getPosition()) >= 15.0f) {
            Projectile.destroyProjectile();
        }

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, FloorTexture1);
        lightingShader.setMat4("model", glm::mat4(1.0f));
        Floor.Draw(ModelFloor);
        lightingShader.setMat4("model", glm::translate(glm::mat4(1.0f), glm::vec3(-15.0f, 0.0f, 0.0f)));
        Floor.Draw(ModelFloor1);
        lightingShader.setMat4("model", glm::translate(glm::mat4(1.0f), glm::vec3(15.0f, 0.0f, 0.0f)));
        Floor.Draw(ModelFloor2);
        lightingShader.setMat4("model", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0, -15.0f)));
        Floor.Draw(ModelFloor3);
        lightingShader.setMat4("model", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 15.0f)));
        Floor.Draw(ModelFloor4);
        lightingShader.setMat4("model", glm::translate(glm::mat4(1.0f), glm::vec3(15.0f, 0.0f, 15.0f)));
        Floor.Draw(ModelFloor5);
        lightingShader.setMat4("model", glm::translate(glm::mat4(1.0f), glm::vec3(15.0f, 0.0f, -15.0f)));
        Floor.Draw(ModelFloor6);
        lightingShader.setMat4("model", glm::translate(glm::mat4(1.0f), glm::vec3(-15.0f, 0.0f, 15.0f)));
        Floor.Draw(ModelFloor7);
        lightingShader.setMat4("model", glm::translate(glm::mat4(1.0f), glm::vec3(-15.0f, 0.0f, -15.0f)));
        Floor.Draw(ModelFloor8);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseCube);

        //RenderStructes(coords3, lightingShader, cubeVAO, 0);

        // also draw the lamp object
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        lightCubeShader.setMat4("model", model);

        rock.Draw(ModelLamp);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window.getWindow());
        glfwPollEvents();
    }
    
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
}

int main()
{
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    Window.setDayDuration(10);
    Window.init();
    glfwHideWindow(Window.getWindow());
    glfwSetWindowPos(Window.getWindow(), 600, 30);
    glfwSetFramebufferSizeCallback(Window.getWindow(), framebuffer_size_callback);
    glfwSetScrollCallback(Window.getWindow(), scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(Window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    Window.run(execute);
    return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    CameraZoom += yoffset;
}

unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}