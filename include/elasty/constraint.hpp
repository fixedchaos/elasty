#ifndef constraint_hpp
#define constraint_hpp

#include <memory>
#include <vector>
#include <Eigen/Core>

namespace elasty
{
    class Engine;
    struct Particle;

    enum class ConstraintType { Bilateral, Unilateral };

    class Constraint
    {
    public:

        Constraint(const Engine* engine,
                   const std::vector<std::shared_ptr<Particle>>& indices,
                   const double stiffness) :
        m_engine(engine),
        m_particles(indices),
        m_stiffness(stiffness)
        {
        }

        virtual double calculateValue() = 0;
        virtual Eigen::VectorXd calculateGrad() = 0;
        virtual ConstraintType getType() = 0;

        const Engine* m_engine;
        const std::vector<std::shared_ptr<Particle>> m_particles;

        double m_stiffness;
    };

    class BendingConstraint final : public Constraint
    {
    public:

        BendingConstraint(const Engine* engine,
                          const std::shared_ptr<Particle> p_0,
                          const std::shared_ptr<Particle> p_1,
                          const std::shared_ptr<Particle> p_2,
                          const std::shared_ptr<Particle> p_3,
                          const double stiffness,
                          const double dihedral_angle);

        double calculateValue() override;
        Eigen::VectorXd calculateGrad() override;
        ConstraintType getType() override { return ConstraintType::Bilateral; }

    private:

        const double m_dihedral_angle;
    };

    class DistanceConstraint final : public Constraint
    {
    public:

        DistanceConstraint(const Engine* engine,
                           const std::shared_ptr<Particle> p_0,
                           const std::shared_ptr<Particle> p_1,
                           const double stiffness,
                           const double d);
        
        double calculateValue() override;
        Eigen::VectorXd calculateGrad() override;
        ConstraintType getType() override { return ConstraintType::Bilateral; }

    private:

        const double m_d;
    };

    class EnvironmentalCollisionConstraint final : public Constraint
    {
    public:

        EnvironmentalCollisionConstraint(const Engine* engine,
                                         const std::vector<std::shared_ptr<Particle>>& particles,
                                         const double stiffness,
                                         const Eigen::Vector3d& n,
                                         const double d) :
        Constraint(engine, particles, stiffness),
        m_n(n),
        m_d(d)
        {
            assert(particles.size() == 1);
        }

        double calculateValue() override;
        Eigen::VectorXd calculateGrad() override;
        ConstraintType getType() override { return ConstraintType::Unilateral; }

    private:

        const Eigen::Vector3d m_n;
        const double m_d;
    };

    class FixedPointConstraint final : public Constraint
    {
    public:

        FixedPointConstraint(const Engine* engine,
                             const std::vector<std::shared_ptr<Particle>>& particles,
                             const double stiffness,
                             const Eigen::Vector3d& point);

        double calculateValue() override;
        Eigen::VectorXd calculateGrad() override;
        ConstraintType getType() override { return ConstraintType::Bilateral; }

    private:

        const Eigen::Vector3d m_point;
    };

    class IsometricBendingConstraint final : public Constraint
    {
    public:

        IsometricBendingConstraint(const Engine* engine,
                                   const std::vector<std::shared_ptr<Particle>>& particles,
                                   const double stiffness);

        double calculateValue() override;
        Eigen::VectorXd calculateGrad() override;
        ConstraintType getType() override { return ConstraintType::Bilateral; }

    private:

        Eigen::Matrix4d m_Q;
    };
}

#endif /* constraint_hpp */
