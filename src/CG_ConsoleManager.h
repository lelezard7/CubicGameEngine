#ifndef _CG_CONSOLE_MANAGER_
#define _CG_CONSOLE_MANAGER_

#include "CG_SaveStruct.h"
#include "CG_IdManager.h"
#include "CG_Filter.h"

#include <QTableWidgetItem>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class CubicGameEngineWindow;
}
QT_END_NAMESPACE

class CGErrorList;


class CGConsoleManager
{
    const Ui::CubicGameEngineWindow* ui_;
    CGErrorList* errorList_;

public:
    CGConsoleManager(const Ui::CubicGameEngineWindow* ui);
    ~CGConsoleManager();

    CGSaveStruct getSaveStruct() const;

    int sendErrorMessage(QString code, QString description, QString project, QString file, QString row) const;
    int sendWarningMessage(QString code, QString description, QString project, QString file, QString row) const;

    void showErrorMessages(bool value) const;
    void showWarningMessages(bool value) const;

    void refreshErrorListColumnsSize() const;
    bool removeErrorListMessage(int id) const;
    void clearErrorList() const;

};


class CGErrorList
{
    struct ColumnsWidth
    {
        ColumnsWidth();

        int messageTypeWidth;
        int messageIdWidth;
        int iconWidth;
        int codeWidth;
        int projectWidth;
        int fileWidth;
        int rowWidth;

    };

    class FilterValue
    {
        int number;
        QString string;
        QColor color;
        QIcon icon;

    public:
        FilterValue(int number) : number(number), string(""), color(QColor(0, 0, 0)), icon(QIcon()) {};
        FilterValue(QString string) : number(-1), string(string), color(QColor(0, 0, 0)), icon(QIcon()) {};
        FilterValue(QColor color) : number(-1), string(""), color(color), icon(QIcon()) {};
        FilterValue(QIcon icon) : number(-1), string(""), color(QColor(0, 0, 0)), icon(icon) {};

        bool operator==(const FilterValue& other) const;
        bool operator!=(const FilterValue& other) const;

    };

public:
    struct Message
    {
        Message();

        QString messageType;
        int messageId;
        QIcon icon;
        QString code;
        QString description;
        QColor descriptionColor;
        QString project;
        QString file;
        QString row;

    };

    enum MessageItems
    {
        MessageType,
        MessageId,
        Icon,
        Code,
        Description,
        DescriptionColor,
        Project,
        File,
        Row
    };

private:
    const Ui::CubicGameEngineWindow* ui_;

    CGFilter<MessageItems, FilterValue> filter_;
    std::vector<Message> content_;
    CGIdManager<int> idManager_;

    ColumnsWidth defaultColumnsWidth_;
    bool isRefreshColumnsSize_;
    bool isFilterOn_;

    void sendMessage(int newRowIndex, const Message& message) const;

public:
    CGErrorList(const Ui::CubicGameEngineWindow* ui);
    ~CGErrorList();

    void addMessage(const Message& message);
    bool insertMessage(size_t index, const Message& message);

    Message getMessageById(int id) const;
    Message getMessage(size_t index) const;
    Message getMessageByRow(int row) const;

    size_t messageCount() const;
    int findMessage(int id) const;

    bool removeMessage(int id);
    void clearMessages();

    bool addFilter(MessageItems messagePoints, FilterValue value);
    bool removeFilter(MessageItems messagePoints, FilterValue value);
    bool checkFilter(MessageItems messagePoints, FilterValue value);

    void clearFilter();
    bool checkMessageInFilter(const Message& message) const;

    void filterOn();
    void filterOff();

    CGSaveStruct getSaveStruct() const;
    void refresh() const;
    void clear();

    bool setDefaultColumnWidth(MessageItems column, int newWidth);
    int getDefaultColumnWidth(MessageItems column) const;

    void hideColumn(MessageItems column);
    void showColumn(MessageItems column);

    int getColumnWidth(MessageItems column) const;

    int getFreeId();
    int getRowCount() const;

};

#endif
