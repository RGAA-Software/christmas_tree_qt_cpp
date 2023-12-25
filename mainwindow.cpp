#include "mainwindow.h"

#include <QColor>
#include <QDebug>
#include <QLinearGradient>
#include <QPainterPath>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) {
    this->resize(800, 600);

    auto timer = new QTimer();
    connect(timer, &QTimer::timeout, this, [this]() {
        fp_rotate_angel_ += 1.2;
        this->update();

        this->GenParticles();
        this->UpdateParticles();

    });
    timer->start(17);

}

MainWindow::~MainWindow() {

}

void MainWindow::paintEvent(QPaintEvent* event) {

    QPainter painter(this);
    painter.setRenderHint(QPainter::RenderHint::Antialiasing, true);

    // background
    painter.setPen(Qt::NoPen);
    QLinearGradient bg_gradient(0, 0, 0, this->height());
    bg_gradient.setColorAt(0.0, QColor(0x000033));
    bg_gradient.setColorAt(1.0, QColor(0x000011));
    painter.setBrush(QBrush(bg_gradient));
    painter.drawRect(this->rect());

    // tree trunk
    int trunk_width = 60;
    int trunk_height = 480;
    int trunk_color = 0x1e131d;
    painter.setBrush(QBrush(trunk_color));
    QPainterPath trunk_path;
    trunk_path.moveTo(this->width()/2, (this->height() - trunk_height));
    trunk_path.lineTo((this->width() - trunk_width)/2, this->height());
    trunk_path.lineTo((this->width() - trunk_width)/2 + trunk_width, this->height());
    trunk_path.lineTo(this->width()/2, (this->height() - trunk_height));
    painter.drawPath(trunk_path);


    // leaves

    int levels = 35;
    int leaves_height = trunk_height - 130;
    int level_gap = leaves_height / levels;

    int leaf_x_gap = 5;
    int leaf_radius = 5;

    bottom_leaf_color_ += 0x020101;

    int from_y = this->height() - trunk_height;
    for (int l = 0; l < levels; l++) {
        int current_y = l * level_gap + from_y;
        int leaf_size = 2*(l + 1) - 1;
        int half_leaf_size = (leaf_size-1)/2;

        painter.setBrush(LinearGradientColor(l, levels, QColor(0xffffff), QColor(bottom_leaf_color_)));
        for (int c = 0; c < leaf_size; c++) {
            float random_scale = Random01();
            auto k = std::to_string(l) + "_" + std::to_string(c);
            if (leaf_scale_.find(k) == leaf_scale_.end()) {
                leaf_scale_.insert(std::make_pair(k, random_scale));
            }

            auto target_scale = leaf_scale_[k];
            painter.drawEllipse(QPointF(this->width()/2 + (-half_leaf_size + c)*leaf_x_gap, current_y), leaf_radius * target_scale, leaf_radius * target_scale);
        }
    }

    // five-pointed star
    painter.save();
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(0xffff00));
    auto x_offset = this->width()/2;
    auto y_offset = this->height() - trunk_height;
    for (int i = 0; i < 5; i++) {
        painter.resetTransform();
        painter.translate(x_offset, y_offset);
        painter.rotate(i * 72 + fp_rotate_angel_);
        QPainterPath path;
        path.moveTo(0, 0);
        path.lineTo(-10, 0);
        path.lineTo(0, 15);
        path.lineTo(10, 0);
        painter.drawPath(path);
    }
    painter.restore();

    // gifts
    painter.setBrush(QBrush(0x554433));
    painter.drawRoundedRect(this->width()/2 - 120, this->height() - 40, 50, 50, 6, 6);

    painter.setBrush(QBrush(0x334455));
    painter.drawRoundedRect(this->width()/2 + 20, this->height() - 30, 70, 50, 6, 6);

    RenderParticles(painter);
}

QColor MainWindow::LinearGradientColor(int idx, int total, QColor from_color, QColor to_color) {
    float percent = idx * 1.0f / total;

    auto target_red = (to_color.red() - from_color.red()) * percent + from_color.red();
    auto target_green = (to_color.green() - from_color.green()) * percent + from_color.green();
    auto target_blue = (to_color.blue() - from_color.blue()) * percent + from_color.blue();
    return QColor(target_red, target_green, target_blue);
}

double MainWindow::Random01() {
    std::random_device rd;
    std::default_random_engine engine(rd());
    std::uniform_real_distribution<double> dist(0, 1.0);
    return dist(engine);
}

void MainWindow::GenParticles() {
    time_elapse_ += 17;
    if (time_elapse_ > gen_particle_time_gap_ && snow_particles_.size() <= this->max_particles_) {
        time_elapse_ = 0;
        Particle particle;
        particle.x = (int)(Random01() * this->width());
        particle.y = (int)(Random01() * 100);
        particle.velocity = std::max(1, (int)(Random01() * 5));
        particle.color = QColor(0xffffff);
        particle.radius = (int)(Random01() * 5);
        snow_particles_.push_back(particle);
    }
}

void MainWindow::UpdateParticles() {
    for (auto& particle : snow_particles_) {
        particle.y += particle.velocity * 3;
    }

    auto it = snow_particles_.begin();
    while(it != snow_particles_.end()) {
        if ((*it).y > this->height()) {
            it = snow_particles_.erase(it);
        }
        else {
            it++;
        }
    }
}

void MainWindow::RenderParticles(QPainter& painter) {
    for (const auto& particle : snow_particles_) {
        painter.setBrush(QBrush(particle.color));
        painter.drawEllipse(QPointF(particle.x, particle.y), particle.radius, particle.radius);
    }

}


















