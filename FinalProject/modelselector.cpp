#include <qfiledialog.h>

#include "modelselector.h"

ModelSelector::ModelSelector(QWidget* parent) :
    QWidget(parent)
{
    // Set maximum width
    setMaximumWidth(240);
    
    // Set up the main layout
    _stretchLayout = new QVBoxLayout(this);
    _stretchLayout->setContentsMargins(0, 0, 0, 0);
    _stretchLayout->setSpacing(8);
    setLayout(_stretchLayout);

    // Create and add the 'add new' button
    _addNewObjectBtn = new PushButton(nullptr, this);
    _addNewObjectBtn->setChildWidget(new QLabel("Add New", _addNewObjectBtn));
    _addNewObjectBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    _addNewObjectBtn->setMargin(32, 24, 32, 24);
    _addNewObjectBtn->setRadius(10);
    _addNewObjectBtn->setBackgroundColor(QColor(58, 143, 183, 20));
    _stretchLayout->addWidget(_addNewObjectBtn);
    _addNewObjectBtn->show();
    
    // Create the selection list widget
    _objectList = new ScrollListWidget(this);
    _stretchLayout->addWidget(_objectList);
    _objectList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _objectList->show();

    // Connect
    connect(_addNewObjectBtn, &PushButton::onClick, this, &ModelSelector::addNewObject);
}

ModelSelector::~ModelSelector() {}

void ModelSelector::addNewObject() {
    QString path = getNewObjectPath();
    if (path.isEmpty()) {
        return;
    }
    
    Model* model = loadObject(path);
    ModelSelectable* newSelectable = new ModelSelectable(model, this);
    
    _objectSelectables.push_back(newSelectable);
    _objectList->addWidget(newSelectable);
    
    connect(newSelectable, &ModelSelectable::onSelected, this, [=]() {
        emit onObjectSelected(model);
    });
    connect(newSelectable, &ModelSelectable::onRemoved, this, [=]() {
        removeObject(newSelectable);
    });
}

QString ModelSelector::getNewObjectPath() {
    QString path = QFileDialog::getOpenFileName(this, "Select Model", "", "Model Files (*.obj)");
    return path;
}

Model* ModelSelector::loadObject(const QString& path) {
    Model* model = new Model(path.toStdString());
    return model;
}

void ModelSelector::removeObject(ModelSelectable* selectable) {
    // Check if selectable is still in list
    if (!_objectSelectables.contains(selectable)) {
        return;
    }

    // Remove the selectable from the list
    _objectSelectables.removeAll(selectable);

    // Remove the selectable from the list widget
    _objectList->removeWidget(selectable);

    // Delete the selectable
    delete selectable;
}
