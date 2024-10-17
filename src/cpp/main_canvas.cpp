#include "main_canvas.h"

#include <fstream>

#include "GLFW/glfw3.h"

MainCanvas::MainCanvas(nanogui::Widget *parent):
    nanogui::Canvas(parent, 1), rotation(0.f) {
    std::ifstream file;
    file.open("shaders/main_canvas.vert");
    std::string vs((std::istreambuf_iterator(file)), std::istreambuf_iterator<char>());
    file.close();
    file.open("shaders/main_canvas.frag");
    std::string fs((std::istreambuf_iterator(file)), std::istreambuf_iterator<char>());
    file.close();

    shader = new nanogui::Shader(render_pass(), "main_canvas", vs, fs);

    uint32_t indices[] = {
        3, 2, 6, 6, 7, 3,
        4, 5, 1, 1, 0, 4,
        4, 0, 3, 3, 7, 4,
        1, 5, 6, 6, 2, 1,
        0, 1, 2, 2, 3, 0,
        7, 6, 5, 5, 4, 7
    };

    float positions[] = {
        -1.f, 1.f, 1.f, -1.f, -1.f, 1.f,
        1.f, -1.f, 1.f, 1.f, 1.f, 1.f,
        -1.f, 1.f, -1.f, -1.f, -1.f, -1.f,
        1.f, -1.f, -1.f, 1.f, 1.f, -1.f
    };

    float colors[] = {
        0, 1, 1, 0, 0, 1,
        1, 0, 1, 1, 1, 1,
        0, 1, 0, 0, 0, 0,
        1, 0, 0, 1, 1, 0
    };

    shader->set_buffer("indices", nanogui::VariableType::UInt32, {sizeof(indices)}, indices);
    shader->set_buffer("position", nanogui::VariableType::Float32, {sizeof(positions) / 3, 3}, positions);
    shader->set_buffer("color", nanogui::VariableType::Float32, {sizeof(colors) / 3, 3}, colors);
}

void MainCanvas::setRotation(const float rotation) {
    this->rotation = rotation;
}

void MainCanvas::draw_contents() {
    static const nanogui::Vector3f X_AXIS(1, 0, 0);
    static const nanogui::Vector3f Y_AXIS(0, 1, 0);

    const nanogui::Matrix4f view = nanogui::Matrix4f::look_at(nanogui::Vector3f(0, -2, -10), nanogui::Vector3f(0, 0, 0), nanogui::Vector3f(0, 1, 0));
    const nanogui::Matrix4f model = nanogui::Matrix4f::rotate(Y_AXIS, static_cast<float>(glfwGetTime()));
    const nanogui::Matrix4f model2 = nanogui::Matrix4f::rotate(X_AXIS, rotation);
    const nanogui::Matrix4f proj = nanogui::Matrix4f::perspective(45 * 3.14159265358979323846 / 180, 0.1f, 20.f, static_cast<float>(m_size.x()) / static_cast<float>(m_size.y()));
    const nanogui::Matrix4f mvp = proj * view * model * model2;

    shader->set_uniform("mvp", mvp);
    shader->begin();
    shader->draw_array(nanogui::Shader::PrimitiveType::Triangle, 0, 36, true);
    shader->end();
}
