#pragma once

#include <qfontdatabase.h>
#include <qpropertyanimation.h>
#include <qparallelanimationgroup.h>
#include <qlayoutitem.h>

#include "sidebar.h"

SideBar::SideBar(QWidget* parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground, true);
    setMaximumWidth(_collapsedWidth);

    // Create stylesheet
    setObjectName("sideBar");
    QString sideBarStyleSheet = "QWidget#sideBar{background-color:" + _backgroundColor.name(QColor::HexArgb) + "; }";
    setStyleSheet(sideBarStyleSheet);

    // Create main layout
    _mainLayout = new QVBoxLayout(this);
    _mainLayout->setSpacing(8);
    _mainLayout->setContentsMargins(16, 16, 16, 16);
    _mainLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    _mainLayout->addSpacing(8); // Add the top spacing between the expand button and the window edge

    // Create expand button
    _expandButton = new PushButton(nullptr, this);
    _expandButton->setMargin(18, 16, 18, 16);
    _expandButton->setIndicatorColor(QColor(255, 255, 255, 0)); // Set indicator to transparent
    _expandButton->setHoverColor(QColor(0, 0, 0, 10));
    _expandButton->setPressedColor(QColor(0, 0, 0, 20));
    int buttonSize = _collapsedWidth - _mainLayout->contentsMargins().left() - _mainLayout->contentsMargins().right();
    int buttonSizeMax = _expandedWidth - _mainLayout->contentsMargins().left() - _mainLayout->contentsMargins().right();
    _expandButton->setMinimumSize(buttonSize, buttonSize);
    _expandButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
    _expandButtonIcon = new QLabel(_expandButton);
    _expandButtonIcon->setFont(QFont("Font Awesome 6 Free Solid", 14));
    _expandButtonIcon->setText("\uf0c9");
    _expandButton->setChildWidget(_expandButtonIcon);
    
    // Connect the expand button event to expand / collapse event
    connect(_expandButton, &PushButton::onClick, this, [=]() {
        if (_expanded) {
            collapse();
        }
        else {
            expand();
        }
    });
    
    // Add expand button to main layout
    _mainLayout->addWidget(_expandButton);
    _expandButton->show();

    // Create the page icon button container widget
    _pageIconButtonWidget = new QWidget(this);
    _pageIconButtonWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    _pageIconButtonLayout = new QVBoxLayout(_pageIconButtonWidget);
    _pageIconButtonLayout->setSpacing(8);
    _pageIconButtonLayout->setContentsMargins(0, 0, 0, 0);
    _pageIconButtonLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    _pageIconButtonWidget->setLayout(_pageIconButtonLayout);

    // Create the page text button container widget
    _pageTextButtonWidget = new QWidget(this);
    _pageTextButtonWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    _pageTextButtonLayout = new QVBoxLayout(_pageTextButtonWidget);
    _pageTextButtonLayout->setSpacing(8);
    _pageTextButtonLayout->setContentsMargins(0, 0, 0, 0);
    _pageTextButtonLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    _pageTextButtonWidget->setLayout(_pageTextButtonLayout);
    
    // Add page icon and text button container widgets to main layout
    _mainLayout->addWidget(_pageIconButtonWidget);
    _mainLayout->addWidget(_pageTextButtonWidget);

    // Display the icon buttons
    _pageIconButtonWidget->show();
    _pageTextButtonWidget->hide();
}

SideBar::~SideBar() {}

void SideBar::expand() {
    // Add animation to expand the side bar
    QPropertyAnimation* expandAnimation = new QPropertyAnimation(this, "maximumWidth");
    expandAnimation->setDuration(650);
    expandAnimation->setEasingCurve(QEasingCurve::OutExpo);
    expandAnimation->setStartValue(width());
    expandAnimation->setEndValue(_expandedWidth);
    expandAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    
    // Show the page text buttons
    _pageIconButtonWidget->hide();
    _pageTextButtonWidget->show();

    // Set the expand state
    _expanded = true;
}

void SideBar::collapse() {
    // Add animation to collapse the side bar
    QPropertyAnimation* collapseAnimation = new QPropertyAnimation(this, "maximumWidth");
    collapseAnimation->setDuration(650);
    collapseAnimation->setEasingCurve(QEasingCurve::OutExpo);
    collapseAnimation->setStartValue(width());
    collapseAnimation->setEndValue(_collapsedWidth);
    collapseAnimation->start(QAbstractAnimation::DeleteWhenStopped);

    // Show the page icon buttons
    _pageIconButtonWidget->show();
    _pageTextButtonWidget->hide();

    // Set the expand state
    _expanded = false;
}

void SideBar::setBackgroundColor(QColor color) {
    _backgroundColor = color;
    QString sideBarStyleSheet = "QWidget#sideBar{background-color:" + _backgroundColor.name(QColor::HexArgb) + "; }";
    setStyleSheet(sideBarStyleSheet);
}

void SideBar::setExpandedWidth(int width) {
    if (width <= 0) {
        return;
    }
    _expandedWidth = width;
}

void SideBar::setCollapsedWidth(int width) {
    if (width <= 0) {
        return;
    }
    _collapsedWidth = width;
}

void SideBar::selectPage(PageWidget* page) {
    // Check for input validity
    if (page == nullptr) {
        return;
    }
    
    // Deselect current page if there is one
    if (_currentPage != nullptr) {
        // Find the buttons of the current page
        int index = _pageList.indexOf(_currentPage);
        PushButton* currentPageIconButton = _pageButtonList.at(index).first;
        PushButton* currentPageTextButton = _pageButtonList.at(index).second;
        
        // Deselect both the buttons
        currentPageIconButton->deselect();
        currentPageTextButton->deselect();
    }
    
    // Temporarily store the current page
    PageWidget* previousPage = _currentPage;

    // Select the new page
    int index = _pageList.indexOf(page);
    PushButton* newPageIconButton = _pageButtonList.at(index).first;
    PushButton* newPageTextButton = _pageButtonList.at(index).second;
    newPageIconButton->select();
    newPageTextButton->select();
    _currentPage = page;

    // Emit the page selected signal
    emit onPageChanged(previousPage, page);
}

void SideBar::addPage(PageWidget* page) {
    // Call on the page's get button method to get icon button & text button
    PushButton* iconButton = page->getPageIconButton(_pageIconButtonWidget);
    PushButton* textButton = page->getPageTextButton(_pageTextButtonWidget);

    // Store the page in the page list
    _pageList.append(page);

    // Store the corresponding buttons
    _pageButtonList.append(QPair<PushButton*, PushButton*>(iconButton, textButton));
    
    // Add the icon button to the icon button layout
    _pageIconButtonLayout->addWidget(iconButton);
    iconButton->show();
    
    // Add the text button to the text button layout
    _pageTextButtonLayout->addWidget(textButton);
    textButton->show();

    // Resize the buttons and set size policy to fixed
    int buttonSize = _collapsedWidth - _mainLayout->contentsMargins().left() - _mainLayout->contentsMargins().right();
    int buttonSizeMax = _expandedWidth - _mainLayout->contentsMargins().left() - _mainLayout->contentsMargins().right();
    iconButton->setMinimumSize(buttonSize, buttonSize);
    textButton->setMinimumSize(buttonSize, buttonSize);
    iconButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    textButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // Connect the button click event with page selection
    connect(iconButton, &PushButton::onClick, this, [=]() {
        selectPage(page);
    });
    connect(textButton, &PushButton::onClick, this, [=]() {
        selectPage(page);
    });

    // Select the page if there are no pages
    if (_currentPage == nullptr) {
        selectPage(page);
    }
}

void SideBar::insertPage(PageWidget* page, int index) {
    // Call on the page's get button method to get icon button & text button
    PushButton* iconButton = page->getPageIconButton(_pageIconButtonWidget);
    PushButton* textButton = page->getPageTextButton(_pageTextButtonWidget);

    // Store the page
    _pageList.insert(index, page);

    // Store the corresponding buttons
    _pageButtonList.insert(index, QPair<PushButton*, PushButton*>(iconButton, textButton));

    // Add the icon button to the icon button layout
    _pageIconButtonLayout->insertWidget(index, iconButton);
    iconButton->show();

    // Add the text button to the text button layout
    _pageTextButtonLayout->insertWidget(index, textButton);
    textButton->show();

    // Resize the buttons and set size policy to fixed
    int buttonSize = _collapsedWidth - _mainLayout->contentsMargins().left() - _mainLayout->contentsMargins().right();
    int buttonSizeMax = _expandedWidth - _mainLayout->contentsMargins().left() - _mainLayout->contentsMargins().right();
    iconButton->setMinimumSize(buttonSize, buttonSize);
    textButton->setMinimumSize(buttonSize, buttonSize);
    iconButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    textButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // Connect the button click event with page selection
    connect(iconButton, &PushButton::onClick, this, [=]() {
        selectPage(page);
    });
    connect(textButton, &PushButton::onClick, this, [=]() {
        selectPage(page);
    });

    // Select the page if there are no pages
    if (_currentPage == nullptr) {
        selectPage(page);
    }
}

void SideBar::removePage(PageWidget* page) {
    // Get the index of the page from the page list
    int index = _pageList.indexOf(page);
    
    // Get the corresponding buttons
    PushButton* iconButton = _pageButtonList.at(index).first;
    PushButton* textButton = _pageButtonList.at(index).second;

    // Remove the page from the list
    _pageList.removeAt(index);

    // Remove the icon and text buttons from the list
    _pageButtonList.removeAt(index);

    // Remove the icon button from the icon button layout
    _pageIconButtonLayout->removeWidget(iconButton);
    iconButton->hide();

    // Remove the text button from the text button layout
    _pageTextButtonLayout->removeWidget(textButton);
    textButton->hide();

    // Select the first page if the current page is the page being removed
    if (_currentPage == page) {
        if (_pageList.size() > 0) {
            selectPage(_pageList.first());
        }
        else {
            _currentPage = nullptr;
        }
    }
}
