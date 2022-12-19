#include "modelsetter.h"

ModelSetter::ModelSetter(QWidget* parent) : ModelSetter(nullptr, parent) {}

ModelSetter::ModelSetter(Renderable* object, QWidget* parent) :
    QWidget(parent), _object(object)
{
    // Set background color and border radius
    setAttribute(Qt::WA_StyledBackground, true);
    
    // Create main layout
    _mainLayout = new QHBoxLayout(this);
    _mainLayout->setContentsMargins(12, 8, 12, 8);
    _mainLayout->setSpacing(8);
    setLayout(_mainLayout);

    // Create Object Settings Panel
    {
        // Create container widget
        QWidget* _objectSettingPanel = new QWidget(this);
        _objectSettingPanel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        _mainLayout->addWidget(_objectSettingPanel);
        _objectSettingPanel->show();

        // Create container widget layout
        _objectSettingLayout = new QVBoxLayout(_objectSettingPanel);
        _objectSettingLayout->setContentsMargins(0, 0, 0, 0);
        _objectSettingLayout->setSpacing(4);
        _objectSettingPanel->setLayout(_objectSettingLayout);

        // Create attribute adjusters
        _scale = new ModelAttributeSlide("Scale", 0.1, 10, 100, _objectSettingPanel);
        _rotateX = new ModelAttributeSlide("RotateX", 0, 360, 3600, _objectSettingPanel);
        _rotateY = new ModelAttributeSlide("RotateY", 0, 360, 3600, _objectSettingPanel);
        _rotateZ = new ModelAttributeSlide("RotateZ", 0, 360, 3600, _objectSettingPanel);
        
        // Add attribute adjusters to layout
        _objectSettingLayout->addWidget(_scale);
        _objectSettingLayout->addWidget(_rotateX);
        _objectSettingLayout->addWidget(_rotateY);
        _objectSettingLayout->addWidget(_rotateZ);
        _scale->show();
        _rotateX->show();
        _rotateY->show();
        _rotateZ->show();

        // Connect
        connect(_scale, &ModelAttributeSlide::onChangeStart, this, &ModelSetter::onAdjustStart);
        connect(_scale, &ModelAttributeSlide::onChangeEnd, this, &ModelSetter::onAdjustEnd);
        connect(_scale, &ModelAttributeSlide::onChanged, this, [=]() {
            if (_object != nullptr) {
                _object->setScale(_scale->val());
                emit onAdjust();
            }
        });
        connect(_scale, &ModelAttributeSlide::onChangeEnd, this, [=]() {
            if (_object != nullptr) {
                _object->updateBoundary();
            }
        });
        
        connect(_rotateX, &ModelAttributeSlide::onChangeStart, this, &ModelSetter::onAdjustStart);
        connect(_rotateX, &ModelAttributeSlide::onChangeEnd, this, &ModelSetter::onAdjustEnd);
        connect(_rotateX, &ModelAttributeSlide::onChanged, this, [=]() {
            if (_object != nullptr) {
                setRotate();
                emit onAdjust();
            }
        });
        connect(_rotateX, &ModelAttributeSlide::onChangeEnd, this, [=]() {
            if (_object != nullptr) {
                _object->updateBoundary();
            }
        });
        
        connect(_rotateY, &ModelAttributeSlide::onChangeStart, this, &ModelSetter::onAdjustStart);
        connect(_rotateY, &ModelAttributeSlide::onChangeEnd, this, &ModelSetter::onAdjustEnd);
        connect(_rotateY, &ModelAttributeSlide::onChanged, this, [=]() {
            if (_object != nullptr) {
                setRotate();
                emit onAdjust();
            }
        });
        connect(_rotateY, &ModelAttributeSlide::onChangeEnd, this, [=]() {
            if (_object != nullptr) {
                _object->updateBoundary();
            }
        });
        
        connect(_rotateZ, &ModelAttributeSlide::onChangeStart, this, &ModelSetter::onAdjustStart);
        connect(_rotateZ, &ModelAttributeSlide::onChangeEnd, this, &ModelSetter::onAdjustEnd);
        connect(_rotateZ, &ModelAttributeSlide::onChanged, this, [=]() {
            if (_object != nullptr) {
                setRotate();
                emit onAdjust();
            }
        });
        connect(_rotateZ, &ModelAttributeSlide::onChangeEnd, this, [=]() {
            if (_object != nullptr) {
                _object->updateBoundary();
            }
        });
    }

    // Create light switches
    {
        // Create container widget
        _lightSwitchPanel = new QWidget(this);
        _lightSwitchPanel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        _lightSwitchPanel->setMinimumWidth(36);
        _mainLayout->addWidget(_lightSwitchPanel);
        _lightSwitchPanel->show();

        // Create container widget layout
        _lightSwitchLayout = new QVBoxLayout(_lightSwitchPanel);
        _lightSwitchLayout->setContentsMargins(0, 0, 0, 0);
        _lightSwitchLayout->setAlignment(Qt::AlignCenter);
        _lightSwitchLayout->setSpacing(4);
        _lightSwitchPanel->setLayout(_lightSwitchLayout);
        
        // Create light switches
        _lightSwitch = new PushButton(nullptr, _lightSwitchPanel);
        _lightSwitch->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        _lightSwitch->setIndicatorPosition(PushButton::LUI_BTN_POS_BOTTOM);
        _lightSwitchIcon = new QLabel(_lightSwitch);
        _lightSwitchIcon->setFont(QFont("Font Awesome 6 Free Regular", 12));
        _lightSwitchIcon->setText("\uf0eb");
        _lightSwitchIcon->setAlignment(Qt::AlignCenter);
        _lightSwitch->setChildWidget(_lightSwitchIcon);
        
        _lightColorPanel = new PushButton(nullptr, _lightSwitchPanel);
        _lightColorPanel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        _lightColorPanel->setIndicatorPosition(PushButton::LUI_BTN_POS_BOTTOM);
        _lightColorPanelIcon = new QLabel(_lightColorPanel);
        _lightColorPanelIcon->setFont(QFont("Font Awesome 6 Free Solid", 12));
        _lightColorPanelIcon->setText("\uf53f");
        _lightColorPanelIcon->setAlignment(Qt::AlignCenter);
        _lightColorPanel->setStyleSheet("QWidget#indicator{border: 1px solid #5c5c5c;}");
        _lightColorPanel->setChildWidget(_lightColorPanelIcon);

        // Add light switches to layout
        _lightSwitchLayout->addWidget(_lightSwitch);
        _lightSwitchLayout->addWidget(_lightColorPanel);
        _lightSwitch->show();
        _lightColorPanel->show();

        // Connect
        connect(_lightSwitch, &PushButton::onClick, this, [=]() {
            if (_object != nullptr && _object->hasLight()) {
                // Disable light
                _object->disposeLight();
                _lightSwitch->deselect();
                _lightSwitchIcon->setFont(QFont("Font Awesome 6 Free Regular", 12));
                _lightColorPanel->setEnabled(false);
                _lightDistance->setEnabled(false);
                _lightRotateTheta->setEnabled(false);
                _lightRotatePhi->setEnabled(false);
                _lightCutoffAngle->setEnabled(false);
                _lightR->setEnabled(false);
                _lightG->setEnabled(false);
                _lightB->setEnabled(false);
                _lightColorPanel->deselect();
                _lightSettingPanel->show();
                _lightColorSettingPanel->hide();
                _colorPaletteOn = false;
                emit onAdjust();
            }
            else if (_object != nullptr) {
                // Enable light
                _object->makeLight();
                _lightSwitch->select();
                _lightSwitchIcon->setFont(QFont("Font Awesome 6 Free Solid", 12));
                _lightColorPanel->setEnabled(true);
                _lightColorPanel->setColorScheme(QColor(_object->originalLight()->lightColor().x, _object->originalLight()->lightColor().y, _object->originalLight()->lightColor().z));
                _lightDistance->setEnabled(true);
                _lightCutoffAngle->setEnabled(true);
                _lightR->setEnabled(true);
                _lightG->setEnabled(true);
                _lightB->setEnabled(true);
                _lightR->setValue(_object->originalLight()->lightColor().r * 255.0f);
                _lightG->setValue(_object->originalLight()->lightColor().g * 255.0f);
                _lightB->setValue(_object->originalLight()->lightColor().b * 255.0f);
                _lightColorPanel->setColorScheme(QColor(
                    _object->originalLight()->lightColor().r * 255.0f, 
                    _object->originalLight()->lightColor().g * 255.0f, 
                    _object->originalLight()->lightColor().b * 255.0f
                ));
                _lightColorPanel->select();
                _lightDistance->setValue(_object->originalLight()->idealDistance());
                _lightRotateTheta->setValue(glm::degrees(glm::acos(_object->originalLight()->lightDirection().y)));
                _lightRotatePhi->setValue(glm::degrees(glm::atan(_object->originalLight()->lightDirection().x / _object->originalLight()->lightDirection().z)));
                _lightCutoffAngle->setValue(_object->originalLight()->cutOffAngle());
                if (_lightCutoffAngle->val() != 180.0f) {
                    _lightRotateTheta->setEnabled(true);
                    _lightRotatePhi->setEnabled(true);
                }
                else {
                    _lightRotateTheta->setEnabled(false);
                    _lightRotatePhi->setEnabled(false);
                }
                emit onAdjust();
            }
        });
        connect(_lightColorPanel, &PushButton::onClick, this, [=]() {
            if (_object != nullptr && _object->hasLight()) {
                if (!_colorPaletteOn) {
                    _lightSettingPanel->hide();
                    _lightColorSettingPanel->show();
                    _colorPaletteOn = true;
                }
                else {
                    _lightSettingPanel->show();
                    _lightColorSettingPanel->hide();
                    _colorPaletteOn = false;
                }
            }
        });
    }
    
    // Create light setting panel
    {
        // Create container widget
        _lightSettingPanel = new QWidget(this);
        _lightSettingPanel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        _mainLayout->addWidget(_lightSettingPanel);
        _lightSettingPanel->show();
        
        // Create container widget layout
        _lightSettingLayout = new QVBoxLayout(_lightSettingPanel);
        _lightSettingLayout->setContentsMargins(0, 0, 0, 0);
        _lightSettingLayout->setSpacing(4);
        _lightSettingPanel->setLayout(_lightSettingLayout);
        
        // Create attribute adjusters
        _lightDistance = new ModelAttributeSlide("Distance", 10, 3025, 145, _lightSettingPanel);
        _lightDistance->setTransformation(
            [](float x) {
                float y;
                if (x <= 90) {
                    y = 10 + x;
                }
                else {
                    y = 100 + glm::pow(x - 90, 2.0f);
                }
                return y;
            },
            [](float y) {
                float x;
                if (y <= 100) {
                    x = y - 10;
                }
                else {
                    x = glm::sqrt(y - 100) + 90;
                }
                return x;
            }
        );
        _lightRotateTheta = new ModelAttributeSlide("Rotate\u03B8", 0, 360, 3600, _lightSettingPanel);
        _lightRotatePhi = new ModelAttributeSlide("Rotate\u03C6", 0, 360, 3600, _lightSettingPanel);
        _lightCutoffAngle = new ModelAttributeSlide("Cutoff", 0, 180, 1800, _lightSettingPanel);
        
        // Add attribute adjusters to layout
        _lightSettingLayout->addWidget(_lightDistance);
        _lightSettingLayout->addWidget(_lightRotateTheta);
        _lightSettingLayout->addWidget(_lightRotatePhi);
        _lightSettingLayout->addWidget(_lightCutoffAngle);
        _lightDistance->show();
        _lightRotateTheta->show();
        _lightRotatePhi->show();
        _lightCutoffAngle->show();
        
        // Connect
        connect(_lightDistance, &ModelAttributeSlide::onChanged, this, [=]() {
            if (_object != nullptr && _object->hasLight()) {
                _object->originalLight()->setIdealDistance(_lightDistance->val());
                emit onAdjust();
            }
        });
        connect(_lightRotateTheta, &ModelAttributeSlide::onChanged, this, [=]() {
            if (_object != nullptr && _object->hasLight()) {
                setLightDir();
                emit onAdjust();
            }
        });
        connect(_lightRotatePhi, &ModelAttributeSlide::onChanged, this, [=]() {
            if (_object != nullptr && _object->hasLight()) {
                setLightDir();
                emit onAdjust();
            }
        });
        connect(_lightCutoffAngle, &ModelAttributeSlide::onChanged, this, [=]() {
            if (_object != nullptr && _object->hasLight()) {
                _object->originalLight()->setCutOffAngle(_lightCutoffAngle->val());
                if (_lightCutoffAngle->val() != 180.0f) {
                    _lightRotateTheta->setEnabled(true);
                    _lightRotatePhi->setEnabled(true);
                }
                else {
                    _lightRotateTheta->setEnabled(false);
                    _lightRotatePhi->setEnabled(false);
                }
                emit onAdjust();
            }
        });
    }

    // Create color setting panel
    {
        // Create container widget
        _lightColorSettingPanel = new QWidget(this);
        _lightColorSettingPanel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        _mainLayout->addWidget(_lightColorSettingPanel);
        _lightColorSettingPanel->show();
        
        // Create container widget layout
        _lightColorSettingPanelLayout = new QVBoxLayout(_lightColorSettingPanel);
        _lightColorSettingPanelLayout->setContentsMargins(0, 0, 0, 0);
        _lightColorSettingPanelLayout->setSpacing(8);
        _lightColorSettingPanel->setLayout(_lightColorSettingPanelLayout);
        
        // Create color adjusters
        _lightR = new ModelAttributeSlide("R", 0, 255, 2550, _lightColorSettingPanel);
        _lightG = new ModelAttributeSlide("G", 0, 255, 2550, _lightColorSettingPanel);
        _lightB = new ModelAttributeSlide("B", 0, 255, 2550, _lightColorSettingPanel);
        
        // Add color adjusters to layout
        _lightColorSettingPanelLayout->addWidget(_lightR);
        _lightColorSettingPanelLayout->addWidget(_lightG);
        _lightColorSettingPanelLayout->addWidget(_lightB);
        _lightR->show();
        _lightG->show();
        _lightB->show();
        
        // Connect
        connect(_lightR, &ModelAttributeSlide::onChanged, this, [=]() {
            if (_object != nullptr && _object->hasLight()) {
                setLightColor();
                _lightColorPanel->setColorScheme(QColor(_lightR->val(), _lightG->val(), _lightB->val()));
                emit onAdjust();
            }
        });
        connect(_lightG, &ModelAttributeSlide::onChanged, this, [=]() {
            if (_object != nullptr && _object->hasLight()) {
                setLightColor();
                _lightColorPanel->setColorScheme(QColor(_lightR->val(), _lightG->val(), _lightB->val()));
                emit onAdjust();
            }
        });
        connect(_lightB, &ModelAttributeSlide::onChanged, this, [=]() {
            if (_object != nullptr && _object->hasLight()) {
                setLightColor();
                _lightColorPanel->setColorScheme(QColor(_lightR->val(), _lightG->val(), _lightB->val()));
                emit onAdjust();
            }
        });
    }

    // Create delete button
    {
        _deleteBtn = new PushButton(nullptr, this);
        _deleteBtn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        _deleteIcon = new QLabel(_deleteBtn);
        _deleteIcon->setFont(QFont("Font Awesome 6 Free Regular", 12));
        _deleteIcon->setText("\uf2ed");
        _deleteIcon->setAlignment(Qt::AlignCenter);
        _deleteBtn->setChildWidget(_deleteIcon);
        _deleteBtn->setColorScheme(QColor(171, 59, 58));
        _deleteBtn->setIndicatorColor(QColor(171, 59, 58, 0));

        // Add delete button to layout
        _mainLayout->addWidget(_deleteBtn);
        _deleteBtn->show();

        // Connect
        connect(_deleteBtn, &PushButton::onClick, this, [=]() {
            emit onDeleteObject();
        });
    }

    // Update values
    update(_object);
}

ModelSetter::~ModelSetter() {}

void ModelSetter::setRotate() {
    if (_object == nullptr) {
        return;
    }
    // Around X-axis
    _object->setRotation(glm::vec3(1.0f, 0.0f, 0.0f), _rotateX->val());
    // Around Y-axis
    _object->rotate(glm::vec3(0.0f, 1.0f, 0.0f), _rotateY->val());
    // Around Z-axis
    _object->rotate(glm::vec3(0.0f, 0.0f, 1.0f), _rotateZ->val());
}

void ModelSetter::setLightDir() {
    if (_object == nullptr || !_object->hasLight()) {
        return;
    }
    _object->originalLight()->setLightDirection(glm::normalize(glm::vec3(
        cos(glm::radians(_lightRotateTheta->val())) * sin(glm::radians(_lightRotatePhi->val())),
        sin(glm::radians(_lightRotateTheta->val())),
        cos(glm::radians(_lightRotateTheta->val())) * cos(glm::radians(_lightRotatePhi->val()))
    )));
}

void ModelSetter::setLightColor() {
    if (_object == nullptr || !_object->hasLight()) {
        return;
    }
    _object->originalLight()->setLightColor(glm::vec3(_lightR->val() / 255.0f, _lightG->val() / 255.0f, _lightB->val() / 255.0f));
}

void ModelSetter::update(Renderable* object) {
    // Update settings panel by object's real value
    if (object == nullptr) {
        // Disable all settings
        _scale->setEnabled(false);
        _rotateX->setEnabled(false);
        _rotateY->setEnabled(false);
        _rotateZ->setEnabled(false);
        _lightDistance->setEnabled(false);
        _lightRotateTheta->setEnabled(false);
        _lightRotatePhi->setEnabled(false);
        _lightCutoffAngle->setEnabled(false);
        _lightR->setEnabled(false);
        _lightG->setEnabled(false);
        _lightB->setEnabled(false);
        _lightSwitch->setEnabled(false);
        _lightColorPanel->setEnabled(false);
        _deleteBtn->setEnabled(false);
        _lightSwitchIcon->setFont(QFont("Font Awesome 6 Free Regular", 12));
        _lightSwitch->deselect();
        _lightColorPanel->deselect();
        _deleteBtn->setEnabled(false);
    }
    else {
        // Update settings
        if (_object == nullptr) {
            _scale->setEnabled();
        }
        _scale->setValue(object->scaleVal().x);
        
        if (_object == nullptr) {
            _rotateX->setEnabled();
            _rotateY->setEnabled();
            _rotateZ->setEnabled();
        }
        // Extract x, y, z axis rotation from rotation matrix
        glm::mat4 rotationMatrix = object->rotation();
        float rotateX = glm::degrees(glm::asin(-rotationMatrix[1][2]));
        float rotateY = glm::degrees(glm::atan(rotationMatrix[0][2] / rotationMatrix[2][2]));
        float rotateZ = glm::degrees(glm::atan(rotationMatrix[1][0] / rotationMatrix[1][1]));
        _rotateX->setValue(rotateX);
        _rotateY->setValue(rotateY);
        _rotateZ->setValue(rotateZ);
        
        if (_object == nullptr) {
            _lightSwitch->setEnabled();
        }
        if (object->hasLight()) {
            _lightSwitch->select();
        }
        else {
            _lightSwitch->deselect();
        }
        
        if (object->hasLight()) {
            if (_object == nullptr || !_object->hasLight()) {
                // Enable light related settings
                _lightDistance->setEnabled();
                _lightRotateTheta->setEnabled();
                _lightRotatePhi->setEnabled();
                _lightCutoffAngle->setEnabled();
                _lightR->setEnabled();
                _lightG->setEnabled();
                _lightB->setEnabled();
                _lightColorPanel->setEnabled();
                _lightSwitchIcon->setFont(QFont("Font Awesome 6 Free Solid", 12));
            }
            // Update light related settings
            _lightDistance->setValue(object->originalLight()->idealDistance());
            _lightRotateTheta->setValue(glm::degrees(glm::acos(object->originalLight()->lightDirection().y)));
            _lightRotatePhi->setValue(glm::degrees(glm::atan(object->originalLight()->lightDirection().x / object->originalLight()->lightDirection().z)));
            _lightCutoffAngle->setValue(object->originalLight()->cutOffAngle());
            if (_lightCutoffAngle->val() != 180.0f) {
                _lightRotateTheta->setEnabled(true);
                _lightRotatePhi->setEnabled(true);
            }
            else {
                _lightRotateTheta->setEnabled(false);
                _lightRotatePhi->setEnabled(false);
            }
            _lightR->setValue(object->originalLight()->lightColor().r * 255.0f);
            _lightG->setValue(object->originalLight()->lightColor().g * 255.0f);
            _lightB->setValue(object->originalLight()->lightColor().b * 255.0f);
            _lightColorPanel->select();
            _lightColorPanel->setColorScheme(QColor(
                object->originalLight()->lightColor().r * 255.0f, 
                object->originalLight()->lightColor().g * 255.0f, 
                object->originalLight()->lightColor().b * 255.0f
            ));
        }
        else {
            if (_object != nullptr && _object->hasLight()) {
                // Disable light related settings
                _lightDistance->setEnabled(false);
                _lightRotateTheta->setEnabled(false);
                _lightRotatePhi->setEnabled(false);
                _lightCutoffAngle->setEnabled(false);
                _lightR->setEnabled(false);
                _lightG->setEnabled(false);
                _lightB->setEnabled(false);
                _lightColorPanel->setEnabled(false);
                _lightSwitchIcon->setFont(QFont("Font Awesome 6 Free Regular", 12));
                _lightSwitch->deselect();
                _lightColorPanel->deselect();
            }
        }
        _deleteBtn->setEnabled();
    }
    
    if (_object != object || _object == nullptr) {
        _colorPaletteOn = false;
        _lightColorSettingPanel->hide();
        _lightSettingPanel->show();
    }

    _object = object;
}
