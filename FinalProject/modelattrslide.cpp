#include "modelattrslide.h"

ModelAttributeSlide::ModelAttributeSlide(const QString& label, float min, float max, int step, QWidget* parent) :
    QWidget(parent)
{
    // Create main stretch layout
    _stretchLayout = new QHBoxLayout(this);
    _stretchLayout->setContentsMargins(0, 0, 0, 0);
    _stretchLayout->setSpacing(0);
    setLayout(_stretchLayout);
    
    // Create Slider
    _slider = new Slider(min, max, step, this);
    _stretchLayout->addWidget(_slider);
    _slider->show();
    
    // Create Label
    _label = new QLabel(label, this);
    _label->setMinimumWidth(56);
    _label->setFont(QFont("Corbel", 11));
    _label->show();
    _slider->mainLayout()->insertWidget(0, _label);
    _slider->mainLayout()->insertSpacing(1, 8);
    
    // Create Value label
    _val = new QLabel(this);
    _val->setMinimumWidth(32);
    _val->setFont(QFont("Corbel", 11));
    _val->setText(QString::number(_slider->val(), 'f', 1));
    _val->show();
    _slider->mainLayout()->addSpacing(8);
    _slider->mainLayout()->addWidget(_val);
    
    // Connect
    connect(_slider, &Slider::onChanged, this, &ModelAttributeSlide::onChanged);
    connect(_slider, &Slider::onSetValue, this, [=]() {
        _val->setText(QString::number(_slider->val(), 'f', 1));
    });
    connect(_slider, &Slider::onDragStart, this, &ModelAttributeSlide::onChangeStart);
    connect(_slider, &Slider::onDragEnd, this, &ModelAttributeSlide::onChangeEnd);
}

ModelAttributeSlide::~ModelAttributeSlide()
{}

void ModelAttributeSlide::setLabel(const QString& label)
{
    _label->setText(label);
}

void ModelAttributeSlide::setMin(float min)
{
    _slider->setMin(min);
}

void ModelAttributeSlide::setMax(float max)
{
    _slider->setMax(max);
}

void ModelAttributeSlide::setStep(float step)
{
    _slider->setStep(step);
}

void ModelAttributeSlide::setValue(float val)
{
    _slider->setValue(val);
}

void ModelAttributeSlide::setTransformation(std::function<float(float)> transform, std::function<float(float)> inverse)
{
    _slider->setTransformation(transform, inverse);
}

void ModelAttributeSlide::setEnabled(bool enable)
{
    _slider->setEnabled(enable);
}
