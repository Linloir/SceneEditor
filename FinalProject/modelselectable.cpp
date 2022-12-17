#include <qlabel.h>

#include "modelselectable.h"
#include "sceneviewer.h"

ModelSelectable::ModelSelectable(Model* model, QWidget* parent) :
    QWidget(parent), _model(model)
{
    // Set size
    setFixedHeight(150);
    setSizeIncrement(QSizePolicy::Expanding, QSizePolicy::Fixed);
    
    // Create stretch layout to hold the main button
    _stretchLayout = new QHBoxLayout(this);
    _stretchLayout->setContentsMargins(0, 0, 0, 0);
    _stretchLayout->setSpacing(0);
    setLayout(_stretchLayout);
    
    // Create main button widget
    _mainBtn = new PushButton(nullptr, this);
    _mainBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _mainBtn->setMargin(0, 0, 0, 0);
    _mainBtn->setRadius(10);
    _mainBtn->setBackgroundColor(QColor(58, 143, 183, 15));
    _stretchLayout->addWidget(_mainBtn);
    _mainBtn->show();

    // Create widget to hold the button content
    _mainBtnChild = new QWidget(_mainBtn);
    _mainBtnChild->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //_mainBtnChild->setObjectName("mainBtnChild");
    //_mainBtnChild->setStyleSheet("QWidget#mainBtnChild{border: 3px solid " + _borderColor.name(QColor::HexArgb) + ";"
    //    "border-radius:" + QString::number(_cornerRadius) + "px;}");
    _mainBtn->setChildWidget(_mainBtnChild);
    _mainBtnChild->show();

    // Create layout to hold the button content
    _mainLayout = new QHBoxLayout(_mainBtnChild);
    _mainLayout->setContentsMargins(16, 12, 16, 12);
    _mainLayout->setSpacing(12);
    _mainBtnChild->setLayout(_mainLayout);

    // Create model thumbnail widget
    _thumbnailContainer = new RoundedCornerWidget(_mainBtnChild);
    _thumbnailContainer->setCornerRadius(_cornerRadius);
    _thumbnailContainer->setBorderColor(QColor(255, 255, 255, 0));
    _thumbnailContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _mainLayout->addWidget(_thumbnailContainer);
    _thumbnailContainer->show();
    
    _thumbnailWidget = new ModelThumbnailWidget(_model, _mainBtnChild);
    _thumbnailWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    _thumbnailContainerLayout = new QHBoxLayout(_thumbnailContainer->mainWidget());
    _thumbnailContainerLayout->setContentsMargins(0, 0, 0, 0);
    _thumbnailContainerLayout->setSpacing(0);
    _thumbnailContainer->mainWidget()->setLayout(_thumbnailContainerLayout);
    _thumbnailContainerLayout->addWidget(_thumbnailWidget);
    _thumbnailWidget->show();

    // Create delete button
    _deleteButton = new PushButton(nullptr, _mainBtnChild);
    _deleteButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    _deleteIcon = new QLabel(_deleteButton);
    _deleteIcon->setFont(QFont("Font Awesome 6 Free Regular", 12));
    _deleteIcon->setText("\uf2ed");
    _deleteIcon->setAlignment(Qt::AlignCenter);
    _deleteButton->setChildWidget(_deleteIcon);
    _deleteButton->setColorScheme(QColor(171, 59, 58));
    _deleteButton->setIndicatorColor(QColor(171, 59, 58, 0));
    _deleteIcon->show();
    _mainLayout->addWidget(_deleteButton);
    _deleteButton->show();

    // Connect
    connect(_mainBtn, &PushButton::onClick, this, &ModelSelectable::onSelected);
    connect(_deleteButton, &PushButton::onClick, this, &ModelSelectable::onRemoved);
}

ModelSelectable::~ModelSelectable()
{
    delete _model;
}