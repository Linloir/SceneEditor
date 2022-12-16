#include <qpropertyanimation.h>
#include <qparallelanimationgroup.h>

#include "pagewidget.h"

PageWidget::PageWidget(QWidget* parent) : QWidget(parent) {
    // Construct and set main layout
    _stretchLayout = new QVBoxLayout(this);
    _stretchLayout->setContentsMargins(0, 0, 0, 0);
    _stretchLayout->setSpacing(0);
    setLayout(_stretchLayout);
    
    // Construct content widget
    _contentWidget = new QWidget(this);
    _stretchLayout->addWidget(_contentWidget);
    _contentWidget->show();
    
    // Add opacity effect to real content
    _pageOpacityEffect = new QGraphicsOpacityEffect(_contentWidget);
    _pageOpacityEffect->setOpacity(0);
    _contentWidget->setGraphicsEffect(_pageOpacityEffect);

    // Move offstage
    move(_originPagePosition + QPoint(0, 150));
    hide();
}

PageWidget::~PageWidget() {}

void PageWidget::onStage() {
    // Move up and fade in
    QParallelAnimationGroup* onStageAnimation = new QParallelAnimationGroup(this);
    QPropertyAnimation* moveAnimation = new QPropertyAnimation(this, "pos");
    QPropertyAnimation* fadeInAnimation = new QPropertyAnimation(_pageOpacityEffect, "opacity");
    moveAnimation->setDuration(600);
    moveAnimation->setEasingCurve(QEasingCurve::OutExpo);
    moveAnimation->setStartValue(pos());
    moveAnimation->setEndValue(_originPagePosition);
    fadeInAnimation->setDuration(500);
    fadeInAnimation->setEasingCurve(QEasingCurve::InQuad);
    fadeInAnimation->setStartValue(_pageOpacityEffect->opacity());
    fadeInAnimation->setEndValue(0.999);
    onStageAnimation->addAnimation(moveAnimation);
    onStageAnimation->addAnimation(fadeInAnimation);
    onStageAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    
    // Show page
    show();
}

void PageWidget::offStage() {
    // Move down and fade out
    QParallelAnimationGroup* offStageAnimation = new QParallelAnimationGroup(this);
    //QPropertyAnimation* moveAnimation = new QPropertyAnimation(_contentWidget, "pos");
    QPropertyAnimation* fadeOutAnimation = new QPropertyAnimation(_pageOpacityEffect, "opacity");
    //moveAnimation->setDuration(300);
    //moveAnimation->setEasingCurve(QEasingCurve::OutCubic);
    //moveAnimation->setStartValue(_contentWidget->pos());
    //moveAnimation->setEndValue(_originPagePosition - QPoint(0, 100));
    fadeOutAnimation->setDuration(200);
    fadeOutAnimation->setStartValue(_pageOpacityEffect->opacity());
    fadeOutAnimation->setEndValue(0);
    //offStageAnimation->addAnimation(moveAnimation);
    offStageAnimation->addAnimation(fadeOutAnimation);
    offStageAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    
    // Connect animation finished signal to hide page
    connect(offStageAnimation, &QParallelAnimationGroup::finished, [=]() {
        move(_originPagePosition + QPoint(0, 150));
        hide();
    });
}