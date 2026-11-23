// SPDX-License-Identifier: GPL-3.0-or-later
// The one FT queue: stations that called me, CQs of Auto mode and CQs the operator added.
// (File name kept from the former manual CQ list.)
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "modems/ft8/FtDecodedText.h"
#include "utils/RuntimeI18n.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QPushButton>
#include <QLabel>
#include <QTableWidget>
#include <QItemSelectionModel>
#include <QSignalBlocker>
#include <QBrush>
#include <QPalette>
#include <algorithm>
namespace {
constexpr int kFtDecodeRole=Qt::UserRole+430;
constexpr int kFtPresentationOnlyRole=Qt::UserRole+429;
constexpr int kSortRole=Qt::UserRole+431;
QString uiText(const char *key,const char *fallback){return M3I18n::translate(QString::fromUtf8(key),QString::fromUtf8(fallback));}
// Sorts by a number where the cell has one (position, SNR, km, age), otherwise by text.
class QueueCell : public QTableWidgetItem
{
public:
    using QTableWidgetItem::QTableWidgetItem;
    bool operator<(const QTableWidgetItem &other) const override
    {
        const QVariant a=data(kSortRole), b=other.data(kSortRole);
        if(a.isValid()&&b.isValid())return a.toDouble()<b.toDouble();
        return QTableWidgetItem::operator<(other);
    }
};
QString sourceText(M3::QueueSource source)
{
    switch(source){
    case M3::QueueSource::Caller:return uiText("ft_queue_source_caller","Caller");
    case M3::QueueSource::Manual:return uiText("ft_queue_source_manual","Operator");
    default:return uiText("ft_queue_source_auto","Auto CQ");
    }
}
}
bool MainWindow::addFtDecodeToCqWorkList(QTableWidgetItem *selected, bool cqOnly)
{
    if(!selected || !m_tableFt8Rx || !Ft8Mode::isFamilyMode(ui->cmbMode->currentText()))return false;
    auto *item=m_tableFt8Rx->item(selected->row(),4);if(!item)return false;
    const auto decode=item->data(kFtDecodeRole).value<Ft8RxDecoder::Decode>();
    if(decode.message.isEmpty())return false; // TX/history rows are never queued.
    const auto parsed=FtDecodedText::parse(decode.message,stationCallsign());
    const auto call=(parsed.cq?FtDecodedText::cqersCall(decode.message):parsed.senderCall).trimmed().toUpper();
    if((cqOnly&&!parsed.cq)||call.isEmpty()||isFtCallBlacklisted(call)||FtDecodedText::callMatches(call,stationCallsign()))return false;
    if(m_ftSession.qsoActive&&FtDecodedText::callMatches(call,m_ftSession.dxCall))return false;
    M3::FtQueueEntry entry;
    entry.source=M3::QueueSource::Manual;
    entry.valid=true;entry.call=call;entry.grid=parsed.grid;entry.decode=decode;entry.cq=parsed.cq;
    entry.mode=currentAdifMode().trimmed().toUpper();
    entry.band=m_cmbFt8Band?m_cmbFt8Band->currentText().trimmed().toLower():m_settings.ft8Band;
    entry.queuedUtc=QDateTime::currentDateTimeUtc();
    entry.priorityText=uiText("ft_queue_manual_text","added by the operator");
    if(item->data(kFtPresentationOnlyRole).toBool())entry.cq=false;   // not a live decode: wait for a fresh CQ
    if(m_workQueue.contains(call)&&m_workQueue.entries.at(m_workQueue.indexOf(call)).source==M3::QueueSource::Caller)return false;   // he is a caller already
    if(!m_workQueue.upsert(entry,QDateTime::currentDateTimeUtc()))return false;
    queueFtLogbookQueriesForDecode(decode,true);   // ranking follows as soon as the logbook answers
    return true;
}
void MainWindow::addSelectedFtCqsToWorkList()
{
    if(!m_tableFt8Rx)return;
    auto selected=m_tableFt8Rx->selectionModel()->selectedRows();
    std::sort(selected.begin(),selected.end(),[](const auto&a,const auto&b){return a.row()<b.row();});
    int added=0;
    for(const auto &index:selected)if(addFtDecodeToCqWorkList(m_tableFt8Rx->item(index.row(),4),true))++added;
    appendLog(uiText("ft_queue_added_log","FT queue: %1 selected CQ(s) added; %2 entries. Adding does not start TX.").arg(added).arg(m_workQueue.size()));
    refreshFtCqWorkListUi();showFtCqWorkList();
}
void MainWindow::showFtCqWorkList()
{
    if(!m_ftCqDialog){
        m_ftCqDialog=new QDialog(this);m_ftCqDialog->setObjectName(QStringLiteral("ftCqWorkListDialog"));m_ftCqDialog->setModal(false);
        m_ftCqDialog->setWindowTitle(uiText("ft_queue_title","FT queue"));m_ftCqDialog->resize(980,460);
        auto *layout=new QVBoxLayout(m_ftCqDialog);
        auto *help=new QLabel(uiText("ft_queue_help","Stations that called me, CQs of Auto mode and CQs you added share this queue. The automation takes the next one by its policy: your \"Next\" mark first, then callers (first come first served), then the most valuable CQ; stations that gave no reply wait at the end (grey). Click a column header to sort the view; the order the automation uses does not change. Double-click switches to a station at once. Entries are worked only while Auto mode may answer (stage, TX permission)."),m_ftCqDialog);
        help->setObjectName(QStringLiteral("ftCqHelp"));help->setWordWrap(true);layout->addWidget(help);
        m_ftCqTable=new QTableWidget(0,10,m_ftCqDialog);m_ftCqTable->setObjectName(QStringLiteral("ftCqWorkListTable"));
        m_ftCqTable->setEditTriggers(QAbstractItemView::NoEditTriggers);m_ftCqTable->setSelectionBehavior(QAbstractItemView::SelectRows);m_ftCqTable->setSelectionMode(QAbstractItemView::ExtendedSelection);
        m_ftCqTable->verticalHeader()->hide();m_ftCqTable->horizontalHeader()->setStretchLastSection(true);
        m_ftCqTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        m_ftCqTable->horizontalHeader()->setSortIndicatorShown(true);
        layout->addWidget(m_ftCqTable);
        auto *buttons=new QHBoxLayout;
        auto *next=new QPushButton(uiText("ft_queue_next","Next"),m_ftCqDialog);next->setObjectName(QStringLiteral("ftQueueNext"));
        next->setToolTip(uiText("ft_queue_next_tip","Marks the selected station as the next one to work (again: removes the mark)."));
        auto *remove=new QPushButton(uiText("ft_cq_list_remove","Remove selected"),m_ftCqDialog);remove->setObjectName(QStringLiteral("ftCqRemove"));
        auto *clear=new QPushButton(uiText("ft_cq_list_clear","Clear list"),m_ftCqDialog);clear->setObjectName(QStringLiteral("ftCqClear"));
        auto *add=new QPushButton(uiText("ft_cq_list_add","Add selected CQs"),m_ftCqDialog);add->setObjectName(QStringLiteral("ftQueueAdd"));
        add->setToolTip(uiText("ft_queue_add_tip","Adds the CQ lines selected in the decode table to this queue."));
        auto *close=new QPushButton(uiText("automodes_close","Close"),m_ftCqDialog);
        buttons->addWidget(next);buttons->addWidget(remove);buttons->addWidget(clear);buttons->addWidget(add);buttons->addStretch();buttons->addWidget(close);layout->addLayout(buttons);
        connect(close,&QPushButton::clicked,m_ftCqDialog,&QDialog::hide);
        connect(add,&QPushButton::clicked,this,&MainWindow::addSelectedFtCqsToWorkList);
        connect(next,&QPushButton::clicked,this,[this]{
            const auto rows=m_ftCqTable->selectionModel()->selectedRows();
            if(rows.isEmpty())return;
            auto *cell=m_ftCqTable->item(rows.first().row(),2);if(!cell)return;
            const QString call=cell->text();
            m_workQueue.togglePinned(call);
            const int idx=m_workQueue.indexOf(call);
            const bool pinned=idx>=0&&m_workQueue.entries.at(idx).pinned;
            appendLog(pinned?uiText("ft_queue_next_log","FT queue: %1 is the next station to work.").arg(call):uiText("ft_queue_next_off_log","FT queue: %1 is no longer marked as next.").arg(call));
            refreshFtCqWorkListUi();processWorkQueue();
        });
        connect(remove,&QPushButton::clicked,this,[this]{
            QSet<QString> calls;
            for(const auto &i:m_ftCqTable->selectionModel()->selectedRows())if(auto *cell=m_ftCqTable->item(i.row(),2))calls.insert(cell->text());
            for(const auto &call:calls)m_workQueue.removeCall(call);
            updateFt8SessionQueueUi();refreshFtCqWorkListUi();
        });
        connect(clear,&QPushButton::clicked,this,[this]{m_workQueue.clear();m_ft8AutoCqBusySlots.clear();updateFt8SessionQueueUi();refreshFtCqWorkListUi();});
        // Double-click: switch to this station at once, like a double-click on his line in the decode table.
        connect(m_ftCqTable,&QTableWidget::cellDoubleClicked,this,[this](int row,int){
            auto *cell=m_ftCqTable->item(row,2);if(!cell||!m_tableFt8Rx)return;
            const int idx=m_workQueue.indexOf(cell->text());if(idx<0)return;
            const auto entry=m_workQueue.entries.at(idx);
            for(int r=m_tableFt8Rx->rowCount()-1;r>=0;--r){
                auto *msg=m_tableFt8Rx->item(r,4);if(!msg)continue;
                const auto decode=msg->data(kFtDecodeRole).value<Ft8RxDecoder::Decode>();
                if(decode.message==entry.decode.message&&decode.slotStartUtcMs==entry.decode.slotStartUtcMs){
                    m_tableFt8Rx->selectRow(r);
                    handleFt8DecodeDoubleClicked(msg);
                    refreshFtCqWorkListUi();
                    return;
                }
            }
            appendLog(uiText("ft_queue_switch_gone_log","FT queue: the line of %1 is no longer in the decode table; select his next line there.").arg(entry.call));
        });
    }
    refreshFtCqWorkListUi();m_ftCqDialog->show();m_ftCqDialog->raise();m_ftCqDialog->activateWindow();
}
void MainWindow::refreshFtCqWorkListUi()
{
    if(m_btnFt8CallerList)m_btnFt8CallerList->setText(uiText("ft_queue_button","Queue (%1)").arg(m_workQueue.size()));
    if(m_btnFtCqAdd)m_btnFtCqAdd->setText(uiText("ft_cq_list_add","Add selected CQs"));
    if(!m_ftCqTable)return;
    const QStringList headers{QStringLiteral("#"),uiText("ft_queue_col_source","Source"),uiText("callsign","Callsign"),uiText("locator","Locator"),QStringLiteral("Hz"),QStringLiteral("dB"),QStringLiteral("km"),uiText("ft_queue_col_why","Why"),uiText("ft_queue_col_age","Age"),uiText("status","Status")};
    m_ftCqTable->setHorizontalHeaderLabels(headers);
    const auto header=m_ftCqTable->horizontalHeader();
    const int sortColumn=header->sortIndicatorSection();const Qt::SortOrder sortOrder=header->sortIndicatorOrder();
    const bool sorted=m_ftCqTable->isSortingEnabled();
    m_ftCqTable->setSortingEnabled(false);
    // Preserve the selected calls across the refresh.
    QSet<QString> selectedCalls;
    for(const auto &i:m_ftCqTable->selectionModel()->selectedRows())if(auto *cell=m_ftCqTable->item(i.row(),2))selectedCalls.insert(cell->text());
    const QSignalBlocker blocker(m_ftCqTable);
    const QDateTime now=QDateTime::currentDateTimeUtc();const qint64 nowMs=QDateTime::currentMSecsSinceEpoch();
    const int slotMs=Ft8Mode::profileForMode(ui->cmbMode->currentText()).slotMs;
    const QVector<int> order=m_workQueue.order(now);
    m_ftCqTable->setRowCount(order.size());
    for(int pos=0;pos<order.size();++pos){
        const auto &e=m_workQueue.entries.at(order.at(pos));
        const bool grey=e.noReply(now);
        const bool fresh=e.decode.slotStartUtcMs>0&&nowMs-e.decode.slotStartUtcMs<=2*slotMs+1000;
        QString state;
        if(grey)state=uiText("ft_queue_state_noreply","No reply until %1").arg(e.noReplyUntilUtc.toLocalTime().toString(QStringLiteral("HH:mm")));
        else if(e.source==M3::QueueSource::Caller)state=uiText("ft_queue_state_caller","Called me, waiting");
        else if(!e.cq)state=uiText("ft_cq_list_busy","Working another station");
        else if(!fresh)state=uiText("ft_cq_list_wait","Waiting for new CQ");
        else state=uiText("ft_cq_list_heard","CQ heard");
        if(e.pinned)state=uiText("ft_queue_state_next","Next")+QStringLiteral(" - ")+state;
        const qint64 age=e.lastSeenUtc.isValid()?e.lastSeenUtc.secsTo(now):0;
        struct Cell{QString text;QVariant sort;};
        const QVector<Cell> cells{{QString::number(pos+1),pos+1},{sourceText(e.source),{}},{e.call,{}},{e.grid,{}},{QString::number(e.decode.frequencyHz),e.decode.frequencyHz},
            {QString::number(e.decode.snrDb),e.decode.snrDb},{e.distanceKm>=0?QString::number(qRound(e.distanceKm)):QString(),e.distanceKm},{e.priorityText,{}},
            {QStringLiteral("%1:%2").arg(age/60).arg(age%60,2,10,QLatin1Char('0')),age},{state,{}}};
        for(int col=0;col<cells.size();++col){
            auto *item=new QueueCell(cells[col].text);
            if(cells[col].sort.isValid())item->setData(kSortRole,cells[col].sort);
            if(grey)item->setForeground(m_ftCqTable->palette().brush(QPalette::Disabled,QPalette::Text));
            if(e.pinned){QFont f=item->font();f.setBold(true);item->setFont(f);}
            m_ftCqTable->setItem(pos,col,item);
            if(selectedCalls.contains(e.call))item->setSelected(true);
        }
    }
    m_ftCqTable->setSortingEnabled(true);
    if(sorted&&sortColumn>0)m_ftCqTable->sortByColumn(sortColumn,sortOrder);
    else m_ftCqTable->sortByColumn(0,Qt::AscendingOrder);
}
