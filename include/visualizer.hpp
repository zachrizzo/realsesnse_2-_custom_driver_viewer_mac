#pragma once

#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

class Visualizer {
public:
    Visualizer(int width, int height);
    ~Visualizer();

    bool initialize();
    void render_frame(const uint8_t* depth_data);
    bool should_close();
    void process_events();

private:
    bool setup_gl();
    bool compile_shaders();
    void update_texture(const uint8_t* depth_data);

    GLFWwindow* window_;
    int width_;
    int height_;
    GLuint texture_id_;
    GLuint shader_program_;

    static const char* vertex_shader_source;
    static const char* fragment_shader_source;
};
