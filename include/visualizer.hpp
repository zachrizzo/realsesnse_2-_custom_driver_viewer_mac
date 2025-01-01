#pragma once

#include <GLFW/glfw3.h>
#include "camera_device.hpp"
#include <memory>

class Visualizer {
public:
    Visualizer(int width, int height);
    ~Visualizer();

    bool initialize();
    void render_frame(const DepthFrame& frame);
    bool should_close();
    void process_events();

private:
    GLFWwindow* window_;
    int width_;
    int height_;
    GLuint texture_id_;
    GLuint shader_program_;

    bool create_window();
    bool setup_gl();
    bool compile_shaders();
    void update_texture(const DepthFrame& frame);

    // Shader sources
    static const char* vertex_shader_source;
    static const char* fragment_shader_source;
};
