
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>

#include <random>
#include <map>

class Particle {
public:

    int x;
    int y;
    int radius;
    int velocity;
    QColor color;
};


class MainWindow : public QWidget {
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void paintEvent(QPaintEvent* event) override;

private:

    QColor LinearGradientColor(int idx, int total, QColor from_color, QColor to_color);
    double Random01();

    void GenParticles();
    void UpdateParticles();
    void RenderParticles(QPainter& paiter);

private:

    float fp_rotate_angel_ = 0;

    std::map<std::string, float> leaf_scale_;

    int bottom_leaf_color_ = 0x228833;

    std::vector<Particle> snow_particles_;
    int gen_particle_time_gap_ = 50;
    int max_particles_ = 300;
    int time_elapse_ = 0;

};

#endif // MAINWINDOW_H
