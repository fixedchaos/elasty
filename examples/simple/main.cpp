#include <array>
#include <memory>
#include <bigger/app.hpp>
#include <elasty/elasty.hpp>

namespace
{
    inline glm::vec3 eigen2glm(const Eigen::Vector3d& eigen)
    {
        return glm::vec3(eigen.x(), eigen.y(), eigen.z());
    }
}

class SimpleEngine final : public elasty::Engine
{
public:
    void initializeScene() override
    {
        vertex.x = Eigen::Vector3d::Zero();
        vertex.v = Eigen::Vector3d(0.0, 1.0, 0.0);
        vertex.m = 1.0;
    }

    void stepTime() override
    {
        constexpr double dt = 1.0 / 60.0;
        const Eigen::Vector3d gravity = Eigen::Vector3d(0.0, - 9.8, 0.0);

        // Apply external forces
        const Eigen::Vector3d external_forces = vertex.m * gravity;
        vertex.v = vertex.v + dt * (1.0 / vertex.m) * external_forces;

        // Calculate predicted positions
        Eigen::Vector3d p = vertex.x + dt * vertex.v;

        // Solve constraints
        constexpr unsigned int num_iterations = 10;
        for (unsigned int i = 0; i < num_iterations; ++ i)
        {
            // TODO
        }

        // Apply the results
        vertex.v = (p - vertex.x) * (1.0 / dt);
        vertex.x = p;

        // Update velocities
        vertex.v *= 0.999;
    }

    elasty::Vertex vertex;
};

class SimpleApp final : public bigger::App
{
public:

    SimpleApp();

    void initialize(int argc, char** argv) override;
    void onReset() override;

    void updateApp() override;
    void releaseSharedResources() override;
};

SimpleApp::SimpleApp() {}

void SimpleApp::initialize(int argc, char** argv)
{
    // Register and apply BGFX configuration settings
    reset(BGFX_RESET_VSYNC | BGFX_RESET_MSAA_X8);
}

void SimpleApp::onReset()
{
    constexpr uint32_t bg_color = 0x303030ff;

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, bg_color, 1.0f, 0);
}

void SimpleApp::updateApp()
{
    // Display ImGui components
    ImGui::Begin("Config");
    {
        ImGui::Text("time: %.2f", m_time);
        ImGui::Text("fps: %.2f", 1.0f / m_last_dt);
    }
    ImGui::End();
}

void SimpleApp::releaseSharedResources() {}

int main(int argc, char** argv)
{
    SimpleApp app;
    return app.run(argc, argv);
}
