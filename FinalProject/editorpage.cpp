#pragma once

#include <qfontdatabase.h>

#include "editorpage.h"

EditorPage::EditorPage(QWidget* parent) :
    PageWidget(parent)
{
    _contentWidget->setMouseTracking(true);

    // Construct title layout
    _titleLayout = new QVBoxLayout(_contentWidget);
    _titleLayout->setContentsMargins(28, 46, 28, 28);
    _titleLayout->setSpacing(18);
    _titleLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    _contentWidget->setLayout(_titleLayout);
    
    // Construct title
    _titleLabel = new QLabel("EDITOR", _contentWidget);
    _titleLabel->setFont(_titleFont);
    _titleLayout->addWidget(_titleLabel);
    _titleLabel->show();
    
    // Construct main layout
    _mainWidget = new QWidget(_contentWidget);
    _mainWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _mainLayout = new QHBoxLayout(_mainWidget);
    _mainLayout->setContentsMargins(0, 0, 0, 0);
    _mainLayout->setSpacing(16);
    _mainWidget->setLayout(_mainLayout);
    _titleLayout->addWidget(_mainWidget);
    _mainWidget->show();

    // Generate model selector
    _modelSelector = new ModelSelector(_mainWidget);
    _mainLayout->addWidget(_modelSelector);
    _modelSelector->show();
        
    // Generate scene viewer
    _sceneViewerContainer = new RoundedCornerWidget(_mainWidget);
    _sceneViewerContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _sceneViewerContainerLayout = new QVBoxLayout(_sceneViewerContainer->mainWidget());
    _sceneViewerContainerLayout->setContentsMargins(0, 0, 0, 0);
    _sceneViewerContainerLayout->setSpacing(0);
    _sceneViewerContainer->mainWidget()->setLayout(_sceneViewerContainerLayout);
    _sceneViewer = new SceneViewer(_sceneViewerContainer->mainWidget());
    _sceneViewerContainerLayout->addWidget(_sceneViewer);
    _sceneViewer->show();
    _mainLayout->addWidget(_sceneViewerContainer);
    _sceneViewerContainer->show();
}

EditorPage::~EditorPage() {}

PushButton* EditorPage::getPageIconButton(QWidget* context) {
    // Check for existed button
    if (_iconButton != nullptr) {
        return _iconButton;
    }
    
    // Generate new icon button
    _iconButton = new PushButton(nullptr, context);
    _iconButton->setMargin(20, 18, 16, 18);
    _iconButtonLabel = new QLabel(_iconButton);
    _iconButtonLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    QFont iconButtonFont("Font Awesome 6 Free Solid", 12);
    iconButtonFont.setStyleStrategy(QFont::PreferAntialias);
    _iconButtonLabel->setFont(iconButtonFont);
    _iconButtonLabel->setText("\uf304");    // set icon to "pen" icon
    _iconButtonLabel->setAlignment(Qt::AlignLeft);
    _iconButton->setChildWidget(_iconButtonLabel);
    
    // Return newly generated icon
    return _iconButton;
}

PushButton* EditorPage::getPageTextButton(QWidget* context) {
    // Check for existed button
    if (_textButton != nullptr) {
        return _textButton;
    }

    // Generate new text button
    _textButton = new PushButton(nullptr, context);
    _textButton->setMargin(20, 18, 16, 18);
    _textButtonWidget = new QWidget(_textButton);
    _textButtonLayout = new QHBoxLayout(_textButtonWidget);
    _textButtonLayout->setContentsMargins(0, 0, 0, 0);
    _textButtonLayout->setSpacing(12);
    _textButtonWidget->setLayout(_textButtonLayout);

    // Generate text button contents
    _textButtonIcon = new QLabel(_textButtonWidget);
    QFont textButtonFont("Font Awesome 6 Free Solid", 12);
    textButtonFont.setStyleStrategy(QFont::PreferQuality);
    _textButtonIcon->setFont(textButtonFont);
    _textButtonIcon->setText("\uf304");    // set icon to "pen" icon
    _textButtonIcon->setAlignment(Qt::AlignLeft);
    
    _textButtonLabel = new QLabel(_textButtonWidget);
    _textButtonLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    _textButtonLabel->setText("Editor");
    _textButtonLabel->setAlignment(Qt::AlignLeft);
    
    // Add text button contents to layout
    _textButtonLayout->addWidget(_textButtonIcon);
    _textButtonLayout->addWidget(_textButtonLabel);
    _textButtonIcon->show();
    _textButtonLabel->show();

    // Set text button child widget
    _textButton->setChildWidget(_textButtonWidget);
    _textButtonWidget->show();

    // Return newly generated text button
    return _textButton;
}
