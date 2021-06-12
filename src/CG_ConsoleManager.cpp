#include "CG_ConsoleManager.h"
#include "./ui_CubicGameEngineWindow.h"


// ConsoleManager

CGConsoleManager::CGConsoleManager(const Ui::CubicGameEngineWindow* ui)
{
    ui_ = ui;

    errorList_ = new CGErrorList(ui);
    errorList_->hideColumn(CGErrorList::MessageType);
    errorList_->hideColumn(CGErrorList::MessageId);

    errorList_->addFilter(CGErrorList::MessageType, QString("error"));
    errorList_->addFilter(CGErrorList::MessageType, QString("warning"));
    errorList_->filterOn();
    errorList_->refresh();
}

CGConsoleManager::~CGConsoleManager()
{
    delete errorList_;
}

CGSaveStruct CGConsoleManager::getSaveStruct() const
{
    return CGSaveStruct();
}

int CGConsoleManager::sendErrorMessage(
        QString code,
        QString description,
        QString project,
        QString file,
        QString row) const
{
    CGErrorList::Message _message;

    _message.messageType = "error";
    _message.messageId = errorList_->getFreeId();
    _message.icon = QIcon(":icons/error");
    _message.code = code;
    _message.description = description;
    _message.descriptionColor = QColor(227, 38, 54);
    _message.project = project;
    _message.file = file;
    _message.row = row;

    errorList_->addMessage(_message);
    return _message.messageId;
}

int CGConsoleManager::sendWarningMessage(
        QString code,
        QString description,
        QString project,
        QString file,
        QString row) const
{
    CGErrorList::Message _message;

    _message.messageType = "warning";
    _message.messageId = errorList_->getFreeId();
    _message.icon = QIcon(":icons/warning");
    _message.code = code;
    _message.description = description;
    _message.descriptionColor = QColor(0, 0, 0);
    _message.project = project;
    _message.file = file;
    _message.row = row;

    errorList_->addMessage(_message);
    return _message.messageId;
}

void CGConsoleManager::showErrorMessages(bool value) const
{
    if (value) {
        errorList_->addFilter(CGErrorList::MessageType, QString("error"));
        errorList_->refresh();
        return;
    }

    errorList_->removeFilter(CGErrorList::MessageType, QString("error"));
    errorList_->refresh();
}

void CGConsoleManager::showWarningMessages(bool value) const
{
    if (value) {
        errorList_->addFilter(CGErrorList::MessageType, QString("warning"));
        errorList_->refresh();
        return;
    }

    errorList_->removeFilter(CGErrorList::MessageType, QString("warning"));
    errorList_->refresh();
}

void CGConsoleManager::refreshErrorListColumnsSize() const
{
    int _newWidth;

    _newWidth = errorList_->getColumnWidth(CGErrorList::MessageType);
    errorList_->setDefaultColumnWidth(CGErrorList::MessageType, _newWidth);

    _newWidth = errorList_->getColumnWidth(CGErrorList::MessageId);
    errorList_->setDefaultColumnWidth(CGErrorList::MessageId, _newWidth);

    _newWidth = errorList_->getColumnWidth(CGErrorList::Icon);
    errorList_->setDefaultColumnWidth(CGErrorList::Icon, _newWidth);

    _newWidth = errorList_->getColumnWidth(CGErrorList::Code);
    errorList_->setDefaultColumnWidth(CGErrorList::Code, _newWidth);

    _newWidth = errorList_->getColumnWidth(CGErrorList::Project);
    errorList_->setDefaultColumnWidth(CGErrorList::Project, _newWidth);

    _newWidth = errorList_->getColumnWidth(CGErrorList::File);
    errorList_->setDefaultColumnWidth(CGErrorList::File, _newWidth);

    _newWidth = errorList_->getColumnWidth(CGErrorList::Row);
    errorList_->setDefaultColumnWidth(CGErrorList::Row, _newWidth);
}

bool CGConsoleManager::removeErrorListMessage(int id) const
{
    return errorList_->removeMessage(id);
}

void CGConsoleManager::clearErrorList() const
{
    errorList_->clearMessages();
}


// ConsoleManager_ErrorList

CGErrorList::CGErrorList(const Ui::CubicGameEngineWindow* ui)
{
    ui_ = ui;
    isFilterOn_ = false;
    isRefreshColumnsSize_ = true;

    ui_->errorList->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    ui_->errorList->setColumnCount(8);
    QStringList _columnTitles = QStringList() << "Тип" << "ID" << "" << "Код" << "Описание" << "Проект" << "Файл" << "Строка";
    ui_->errorList->setHorizontalHeaderLabels(_columnTitles);

    QHeaderView* _horizontalHeader = ui_->errorList->horizontalHeader();
    _horizontalHeader->setDefaultAlignment(Qt::AlignLeft);
    QHeaderView* _verticalHeader = ui_->errorList->verticalHeader();
    _verticalHeader->setDefaultAlignment(Qt::AlignRight);

    defaultColumnsWidth_.messageTypeWidth = 55;
    ui_->errorList->setColumnWidth(0, 55);
    defaultColumnsWidth_.messageIdWidth = 55;
    ui_->errorList->setColumnWidth(1, 55);
    defaultColumnsWidth_.iconWidth = 15;
    ui_->errorList->setColumnWidth(2, 15);
    defaultColumnsWidth_.codeWidth = 55;
    ui_->errorList->setColumnWidth(3, 55);
    ui_->errorList->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    defaultColumnsWidth_.projectWidth = 150;
    ui_->errorList->setColumnWidth(5, 150);
    defaultColumnsWidth_.fileWidth = 150;
    ui_->errorList->setColumnWidth(6, 150);
    defaultColumnsWidth_.rowWidth = 55;
    ui_->errorList->setColumnWidth(7, 55);
}

CGErrorList::~CGErrorList()
{
    clear();
}

CGErrorList::ColumnsWidth::ColumnsWidth()
{
    messageTypeWidth = 0;
    messageIdWidth = 0;
    iconWidth = 0;
    codeWidth = 0;
    projectWidth = 0;
    fileWidth = 0;
    rowWidth = 0;
}

CGErrorList::Message::Message()
{
    messageType = "";
    messageId = -1;
    icon = QIcon();
    code = "";
    description = "";
    descriptionColor = QColor(0, 0, 0);
    project = "";
    file = "";
    row = "";
}

bool CGErrorList::FilterValue::operator==(const FilterValue& other) const
{
    return  number == other.number &&
            string == other.string &&
            color == other.color &&
            icon.name() == other.icon.name();
}

bool CGErrorList::FilterValue::operator!=(const FilterValue& other) const
{
    return !(number == other.number &&
             string == other.string &&
             color == other.color &&
             icon.name() == other.icon.name());
}

void CGErrorList::sendMessage(int newRowIndex, const Message& message) const
{
    ui_->errorList->insertRow(newRowIndex);
    ui_->errorList->setRowHeight(newRowIndex, 10);

    QTableWidgetItem* _item_messageType = new QTableWidgetItem;
    _item_messageType->setText(message.messageType);
    ui_->errorList->setItem(newRowIndex, 0, _item_messageType);

    QTableWidgetItem* _item_messageId = new QTableWidgetItem;
    _item_messageId->setText(QString::number(message.messageId));
    ui_->errorList->setItem(newRowIndex, 1, _item_messageId);

    QTableWidgetItem* _item_icon = new QTableWidgetItem;
    _item_icon->setIcon(message.icon);
    _item_icon->setTextAlignment(Qt::AlignCenter);
    ui_->errorList->setItem(newRowIndex, 2, _item_icon);

    QTableWidgetItem* _item_code = new QTableWidgetItem;
    _item_code->setText(message.code);
    ui_->errorList->setItem(newRowIndex, 3, _item_code);

    QTableWidgetItem* _item_description = new QTableWidgetItem;
    _item_description->setText(message.description);
    _item_description->setTextColor(message.descriptionColor);
    ui_->errorList->setItem(newRowIndex, 4, _item_description);

    QTableWidgetItem* _item_project = new QTableWidgetItem;
    _item_project->setText(message.project);
    ui_->errorList->setItem(newRowIndex, 5, _item_project);

    QTableWidgetItem* _item_file = new QTableWidgetItem;
    _item_file->setText(message.file);
    ui_->errorList->setItem(newRowIndex, 6, _item_file);

    QTableWidgetItem* _item_row = new QTableWidgetItem;
    _item_row->setText(message.row);
    ui_->errorList->setItem(newRowIndex, 7, _item_row);
}

void CGErrorList::addMessage(const Message& message)
{
    if  (message.messageId < 0)
        return;

    if (findMessage(message.messageId) != -1)
        return;

    content_.push_back(message);

    int _newRowIndex = ui_->errorList->rowCount();

    if (!isFilterOn_) {
        sendMessage(_newRowIndex, message);
        return;
    }

    if (checkMessageInFilter(message)) {
        sendMessage(_newRowIndex, message);
    }
}

bool CGErrorList::insertMessage(size_t index, const Message& message)
{
    if (index > content_.size())
        return false;

    if  (message.messageId < 0)
        return false;

    if (findMessage(message.messageId) != -1)
        return false;

    auto _it = content_.begin();
    std::advance(_it, index);
    content_.insert(_it, message);

    refresh();
    return true;
}

CGErrorList::Message CGErrorList::getMessageById(int id) const
{
    if (id < 0)
        return Message();

    for (size_t i = 0; i < content_.size(); ++i) {
        if (content_[i].messageId == id) {
            return content_[i];
        }
    }

    return Message();
}

CGErrorList::Message CGErrorList::getMessage(size_t index) const
{
    if (index >= content_.size())
        return Message();

    return content_[index];
}

CGErrorList::Message CGErrorList::getMessageByRow(int row) const
{
    QTableWidgetItem* _item = ui_->errorList->item(row, 1);
    return getMessageById(_item->text().toInt());
}

size_t CGErrorList::messageCount() const
{
    return content_.size();
}

int CGErrorList::findMessage(int id) const
{
    if (id < 0)
        return -1;

    for (size_t i = 0; i < content_.size(); ++i) {
        if (content_[i].messageId == id) {
            return i;
        }
    }

    return -1;
}

bool CGErrorList::removeMessage(int id)
{
    if (id < 0)
        return false;

    for (auto it = content_.begin(); it != content_.end(); ++it) {
        if (it->messageId == id) {
            content_.erase(it);
            idManager_.freeId(id);
            refresh();
            return true;
        }
    }

    return false;
}

void CGErrorList::clearMessages()
{
    content_.clear();
    idManager_.freeAll();

    ui_->errorList->clearContents();
    ui_->errorList->setRowCount(0);
}

bool CGErrorList::addFilter(MessageItems messagePoints, FilterValue value)
{
    return filter_.addFilter(messagePoints, value);
}

bool CGErrorList::removeFilter(MessageItems messagePoints, FilterValue value)
{
    return filter_.removeFilter(messagePoints, value);
}

bool CGErrorList::checkFilter(MessageItems messagePoints, FilterValue value)
{
    return filter_.check(messagePoints, value);
}

void CGErrorList::clearFilter()
{
    filter_.clear();
}

bool CGErrorList::checkMessageInFilter(const Message& message) const
{
    if (!filter_.check(MessageType, FilterValue(message.messageType)) &&
        !filter_.check(MessageId, FilterValue(message.messageId)) &&
        !filter_.check(Code, FilterValue(message.code)) &&
        !filter_.check(Description, FilterValue(message.description)) &&
        !filter_.check(DescriptionColor, FilterValue(message.descriptionColor)) &&
        !filter_.check(Project, FilterValue(message.project)) &&
        !filter_.check(File, FilterValue(message.file)) &&
        !filter_.check(Row, FilterValue(message.row)) &&
        !filter_.check(Icon, FilterValue(message.icon)))
        return false;

    return true;
}

void CGErrorList::filterOn()
{
    isFilterOn_ = true;
}

void CGErrorList::filterOff()
{
    isFilterOn_ = false;
}

CGSaveStruct CGErrorList::getSaveStruct() const
{
    // isColumnHidden,

    return CGSaveStruct();
}

void CGErrorList::refresh() const
{
    ui_->errorList->clearContents();
    ui_->errorList->setRowCount(0);

    if (!isFilterOn_) {
        for (size_t i = 0; i < content_.size(); ++i) {
            int _newRowIndex = ui_->errorList->rowCount();
            sendMessage(_newRowIndex, content_[i]);
        }

        return;
    }

    for (size_t i = 0; i < content_.size(); ++i) {
        if (checkMessageInFilter(content_[i])) {
            int _newRowIndex = ui_->errorList->rowCount();
            sendMessage(_newRowIndex, content_[i]);
        }
    }
}

void CGErrorList::clear()
{
    filter_.clear();
    content_.clear();
    idManager_.freeAll();

    defaultColumnsWidth_ = ColumnsWidth();
    isRefreshColumnsSize_ = true;
    isFilterOn_ = false;

    ui_->errorList->clearContents();
    ui_->errorList->setRowCount(0);
}

bool CGErrorList::setDefaultColumnWidth(MessageItems column, int newWidth)
{
    if (!isRefreshColumnsSize_)
        return false;

    if (newWidth < 0)
        return false;

    switch (column)
    {
        case MessageType: defaultColumnsWidth_.messageTypeWidth = newWidth; return true;
        case MessageId:   defaultColumnsWidth_.messageIdWidth   = newWidth; return true;
        case Icon:        defaultColumnsWidth_.iconWidth        = newWidth; return true;
        case Code:        defaultColumnsWidth_.codeWidth        = newWidth; return true;
        case Project:     defaultColumnsWidth_.projectWidth     = newWidth; return true;
        case File:        defaultColumnsWidth_.fileWidth        = newWidth; return true;
        case Row:         defaultColumnsWidth_.rowWidth         = newWidth; return true;

        default:    return false;
    }
}

int CGErrorList::getDefaultColumnWidth(MessageItems column) const
{
    switch (column)
    {
        case MessageType: return defaultColumnsWidth_.messageTypeWidth;
        case MessageId:   return defaultColumnsWidth_.messageIdWidth;
        case Icon:        return defaultColumnsWidth_.iconWidth;
        case Code:        return defaultColumnsWidth_.codeWidth;
        case Project:     return defaultColumnsWidth_.projectWidth;
        case File:        return defaultColumnsWidth_.fileWidth;
        case Row:         return defaultColumnsWidth_.rowWidth;

        default:    return -1;
    }
}

void CGErrorList::hideColumn(MessageItems column)
{
    switch (column)
    {
        case MessageType: ui_->errorList->hideColumn(0); return;
        case MessageId:   ui_->errorList->hideColumn(1); return;
        case Icon:        ui_->errorList->hideColumn(2); return;
        case Code:        ui_->errorList->hideColumn(3); return;
        case Project:     ui_->errorList->hideColumn(5); return;
        case File:        ui_->errorList->hideColumn(6); return;
        case Row:         ui_->errorList->hideColumn(7); return;
        case Description: {
            ui_->errorList->hideColumn(4);
            isRefreshColumnsSize_ = false;

            ui_->errorList->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
            ui_->errorList->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
            ui_->errorList->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
            ui_->errorList->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
            ui_->errorList->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);
            ui_->errorList->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Stretch);
        } return;

        default:    return;
    }
}

void CGErrorList::showColumn(MessageItems column)
{
    switch (column)
    {
        case MessageType: ui_->errorList->showColumn(0); return;
        case MessageId:   ui_->errorList->showColumn(1); return;
        case Icon:        ui_->errorList->showColumn(2); return;
        case Code:        ui_->errorList->showColumn(3); return;
        case Project:     ui_->errorList->showColumn(5); return;
        case File:        ui_->errorList->showColumn(6); return;
        case Row:         ui_->errorList->showColumn(7); return;
        case Description: {
            ui_->errorList->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive);
            ui_->errorList->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Interactive);
            ui_->errorList->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Interactive);
            ui_->errorList->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Interactive);
            ui_->errorList->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Interactive);
            ui_->errorList->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Interactive);

            ui_->errorList->setColumnWidth(0, defaultColumnsWidth_.messageTypeWidth);
            ui_->errorList->setColumnWidth(1, defaultColumnsWidth_.messageIdWidth);
            ui_->errorList->setColumnWidth(3, defaultColumnsWidth_.codeWidth);
            ui_->errorList->setColumnWidth(5, defaultColumnsWidth_.projectWidth);
            ui_->errorList->setColumnWidth(6, defaultColumnsWidth_.fileWidth);
            ui_->errorList->setColumnWidth(7, defaultColumnsWidth_.rowWidth);

            ui_->errorList->showColumn(4);
            isRefreshColumnsSize_ = true;
        } return;

        default:    return;
    }
}

int CGErrorList::getColumnWidth(MessageItems column) const
{
    switch (column)
    {
        case MessageType: return ui_->errorList->columnWidth(0);
        case MessageId:   return ui_->errorList->columnWidth(1);
        case Icon:        return ui_->errorList->columnWidth(2);
        case Code:        return ui_->errorList->columnWidth(3);
        case Description: return ui_->errorList->columnWidth(4);
        case Project:     return ui_->errorList->columnWidth(5);
        case File:        return ui_->errorList->columnWidth(6);
        case Row:         return ui_->errorList->columnWidth(7);

        default:    return -1;
    }
}

int CGErrorList::getFreeId()
{
    return idManager_.getFreeId();
}

int CGErrorList::getRowCount() const
{
    return ui_->errorList->rowCount();
}
