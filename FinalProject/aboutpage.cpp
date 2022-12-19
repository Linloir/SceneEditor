#include <qlayoutitem.h>

#include "aboutpage.h"

AboutPage::AboutPage(QWidget* parent) : 
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
    _titleLabel = new QLabel("ABOUT", _contentWidget);
    _titleLabel->setFont(_titleFont);
    _titleLayout->addWidget(_titleLabel);
    _titleLabel->show();

    // Construct main layout
    _mainWidget = new QWidget(_contentWidget);
    _mainWidget->setObjectName("mainWidget");
    _mainWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    _mainWidget->setStyleSheet("QWidget#mainWidget { background-color: #efefef; border-radius: 8px; }");
    _mainLayout = new QVBoxLayout(_mainWidget);
    _mainLayout->setAlignment(Qt::AlignTop);
    _mainLayout->setContentsMargins(0, 8, 0, 8);
    _mainLayout->setSpacing(8);
    _mainWidget->setLayout(_mainLayout);
    _titleLayout->addWidget(_mainWidget);
    _mainWidget->show();
    
    // Construct contents
    _authorLabel = new LineEditWidget(_mainWidget);
    _authorLabel->mainLayout()->insertWidget(0, new QLabel("Author"));
    _authorLabel->mainLayout()->insertSpacing(1, 8);
    _authorLabel->setBackgroundColor(QColor(0, 0, 0, 0));
    _authorLabel->setHoverColor(QColor(0, 0, 0, 10));
    _authorLabel->setPressedColor(QColor(0, 0, 0, 20));
    _authorLabel->setText("Linloir");
    _authorLabel->setEnabled(false);
    _mainLayout->addWidget(_authorLabel);
    _authorLabel->show();

    _versionLabel = new LineEditWidget(_mainWidget);
    _versionLabel->mainLayout()->insertWidget(0, new QLabel("Version"));
    _versionLabel->mainLayout()->insertSpacing(1, 8);
    _versionLabel->setBackgroundColor(QColor(0, 0, 0, 0));
    _versionLabel->setHoverColor(QColor(0, 0, 0, 10));
    _versionLabel->setPressedColor(QColor(0, 0, 0, 20));
    _versionLabel->setText("1.0.0");
    _versionLabel->setEnabled(false);
    _mainLayout->addWidget(_versionLabel);
    _versionLabel->show();

    _dateLabel = new LineEditWidget(_mainWidget);
    _dateLabel->mainLayout()->insertWidget(0, new QLabel("Last Update"));
    _dateLabel->mainLayout()->insertSpacing(1, 8);
    _dateLabel->setBackgroundColor(QColor(0, 0, 0, 0));
    _dateLabel->setHoverColor(QColor(0, 0, 0, 10));
    _dateLabel->setPressedColor(QColor(0, 0, 0, 20));
    _dateLabel->setText("2020/12/31");
    _dateLabel->setEnabled(false);
    _mainLayout->addWidget(_dateLabel);
    _dateLabel->show();
}

AboutPage::~AboutPage() {}

PushButton* AboutPage::getPageIconButton(QWidget* context) {
    // Check for existed button
    if (_iconButton != nullptr) {
        return _iconButton;
    }

    // Generate new icon button
    _iconButton = new PushButton(nullptr, context);
    _iconButton->setMargin(20, 18, 16, 18);
    _iconButtonLabel = new QLabel(_iconButton);
    _iconButtonLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    QFont iconButtonFont("Font Awesome 6 Free Regular", 12);
    iconButtonFont.setStyleStrategy(QFont::PreferAntialias);
    _iconButtonLabel->setFont(iconButtonFont);
    _iconButtonLabel->setText("\uf059");
    _iconButtonLabel->setAlignment(Qt::AlignLeft);
    _iconButton->setChildWidget(_iconButtonLabel);

    // Return newly generated icon
    return _iconButton;
}

PushButton* AboutPage::getPageTextButton(QWidget* context) {
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
    QFont textButtonFont("Font Awesome 6 Free Regular", 12);
    textButtonFont.setStyleStrategy(QFont::PreferQuality);
    _textButtonIcon->setFont(textButtonFont);
    _textButtonIcon->setText("\uf059");    // set icon to "pen" icon
    _textButtonIcon->setAlignment(Qt::AlignLeft);

    _textButtonLabel = new QLabel(_textButtonWidget);
    _textButtonLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    _textButtonLabel->setText("About");
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
